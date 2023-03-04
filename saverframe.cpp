/*
 * This file is part of MultiScreenSaver.
 *
 * Copyright (c) qu1ck 2022
 *
 * MultiScreenSaver is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * MultiScreenSaver is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with MultiScreenSaver.
 * If not, see https://www.gnu.org/licenses/.
 */

#include <algorithm>
#include <random>
#include <wx/dcgraph.h>
#include <wx/dir.h>
#include <wx/filename.h>

#include "saverframe.h"

static const wxString SUPPORTED_EXTENSIONS[] = {"jpg", "jpeg", "png"};

class ImageScanner : public wxDirTraverser
{
  public:
    ImageScanner(const wxString& aRoot, const bool aRecursive, wxArrayString& aFiles)
        : root(aRoot), recursive(aRecursive), files(aFiles)
    {
    }

    wxDirTraverseResult OnFile(const wxString& filename)
    {
        wxFileName fn(filename);
        wxString ext = fn.GetExt().Lower();
        for (const wxString& t : SUPPORTED_EXTENSIONS)
        {
            if (ext == t)
            {
                files.Add(filename);
                break;
            }
        }
        return wxDIR_CONTINUE;
    }

    wxDirTraverseResult OnDir(const wxString& dirname)
    {
        if (dirname != root && !recursive)
            return wxDIR_IGNORE;

        return wxDIR_CONTINUE;
    }

  private:
    wxString root;
    bool recursive;
    wxArrayString& files;
};

RenderWindow::RenderWindow(wxWindow* parent, const wxString& aPath, const bool aRecursive, const Config::SCALE aScale)
    : wxWindow(parent, wxID_ANY, wxPoint(0, 0), wxDefaultSize, wxBORDER_NONE), m_scaleMode(aScale)
{
    m_renderer = wxGraphicsRenderer::GetDirect2DRenderer();

    wxDir dir(aPath);
    dir.Traverse(ImageScanner(aPath, aRecursive, m_files));

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(m_files.begin(), m_files.end(), generator);
    m_imgIndex = -1;

    for (int i = 0; i < 3; i++)
    {
        Increment(true);
        LoadNextImage(true);
    }

    Bind(wxEVT_PAINT, &RenderWindow::OnPaint, this);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

wxSize RenderWindow::getScaledSize(const wxSize& originalSize)
{
    double scale = 1;
    wxSize sz = GetParent()->GetSize();

    bool ratioBigger = sz.x * originalSize.y > sz.y * originalSize.x;

    if (ratioBigger && m_scaleMode == Config::SCALE::FILL || !ratioBigger && m_scaleMode == Config::SCALE::FIT)
        scale = sz.x * 1.0 / originalSize.x;

    if (!ratioBigger && m_scaleMode == Config::SCALE::FILL || ratioBigger && m_scaleMode == Config::SCALE::FIT)
        scale = sz.y * 1.0 / originalSize.y;

    return wxSize(originalSize.x * scale, originalSize.y * scale);
}

void RenderWindow::Draw()
{
    wxWindowDC dc(GetParent());
    wxGraphicsContext* gc = m_renderer->CreateContext(dc);
    gc->SetInterpolationQuality(wxInterpolationQuality::wxINTERPOLATION_BEST);

    gc->SetBrush(*wxBLACK_BRUSH);
    gc->DrawRectangle(0, 0, GetParent()->GetSize().x, GetParent()->GetSize().y);

    if (m_files.size() == 0)
    {
        wxDouble width = 0;
        wxDouble height = 0;
        gc->SetFont(gc->CreateFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).Scaled(4), *wxWHITE));
        gc->GetTextExtent("No images found", &width, &height);
        gc->DrawText("No images found", GetSize().x / 2 - width / 2, GetSize().y / 2 - height / 2);
    }
    else
    {
        drawBitmap(gc, m_bitmaps[m_imgIndex % 3], m_originalImgSizes[m_imgIndex % 3]);
    }

    delete gc;
}

void RenderWindow::Transition(bool forward, double tick)
{
    if (m_files.size() == 0)
        return;

    wxWindowDC dc(GetParent());
    wxGraphicsContext* gc = m_renderer->CreateContext(dc);
    gc->SetInterpolationQuality(wxInterpolationQuality::wxINTERPOLATION_BEST);

    gc->SetBrush(*wxBLACK_BRUSH);
    gc->DrawRectangle(0, 0, GetParent()->GetSize().x, GetParent()->GetSize().y);

    drawBitmap(gc, m_bitmaps[m_imgIndex % 3], m_originalImgSizes[m_imgIndex % 3]);
    gc->BeginLayer(tick);
    if (forward)
        drawBitmap(gc, m_bitmaps[(m_imgIndex + 1) % 3], m_originalImgSizes[(m_imgIndex + 1) % 3]);
    else
        drawBitmap(gc, m_bitmaps[(m_imgIndex + 2) % 3], m_originalImgSizes[(m_imgIndex + 2) % 3]);
    gc->EndLayer();

    delete gc;
}

void RenderWindow::drawBitmap(wxGraphicsContext* gc, wxGraphicsBitmap& bitmap, wxSize& originalSize)
{
    wxSize newSize = getScaledSize(originalSize);
    gc->DrawBitmap(bitmap, (GetParent()->GetSize().x - newSize.x) / 2, (GetParent()->GetSize().y - newSize.y) / 2,
                   newSize.x, newSize.y);
}

void RenderWindow::Increment(bool forward)
{
    m_imgIndex = (m_imgIndex + (forward ? 1 : -1) + m_files.size()) % m_files.size();
}

void RenderWindow::LoadNextImage(bool forward)
{
    if (m_files.size() == 0)
        return;

    int increment = forward ? 1 : -1;
    wxImage img(m_files[(m_imgIndex + increment + m_files.size()) % m_files.size()]);
    int bitmapIndex = (m_imgIndex + increment + 3) % 3;
    m_bitmaps[bitmapIndex] = m_renderer->CreateBitmapFromImage(img);
    m_originalImgSizes[bitmapIndex] = img.GetSize();
}

SaverFrame::SaverFrame(const wxString& aPath, const bool aRecursive, const Config::SCALE aScale, const wxPoint& aPos,
                       const wxSize& aSize)
    : wxFrame(nullptr, wxID_ANY, "MultiScreenSaver", aPos, aSize, 0 /*wxSTAY_ON_TOP */)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    renderer = new RenderWindow(this, aPath, aRecursive, aScale);
    sizer->Add(renderer, 1, wxGROW, 0);
    SetSizer(sizer);
    Layout();

    Bind(wxEVT_PAINT, &RenderWindow::OnPaint, renderer);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}
