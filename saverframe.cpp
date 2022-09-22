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
    ImageScanner(const wxString &aRoot, const bool aRecursive, wxArrayString &aFiles)
        : root(aRoot), recursive(aRecursive), files(aFiles)
    {
    }

    wxDirTraverseResult OnFile(const wxString &filename)
    {
        wxFileName fn(filename);
        wxString ext = fn.GetExt().Lower();
        for (const wxString &t : SUPPORTED_EXTENSIONS)
        {
            if (ext == t)
            {
                files.Add(filename);
                break;
            }
        }
        return wxDIR_CONTINUE;
    }

    wxDirTraverseResult OnDir(const wxString &dirname)
    {
        if (dirname != root && !recursive)
            return wxDIR_IGNORE;

        return wxDIR_CONTINUE;
    }

  private:
    wxString root;
    bool recursive;
    wxArrayString &files;
};

SaverFrame::SaverFrame(const wxString &aPath, const bool aRecursive, const Config::SCALE aScale, const wxPoint &aPos,
                       const wxSize &aSize)
    : wxFrame(NULL, wxID_ANY, "PhotoScreensaver", aPos, aSize, 0 /*wxSTAY_ON_TOP */), scaleMode(aScale), bitmap_index(0)
{
    renderer = wxGraphicsRenderer::GetDirect2DRenderer();

    wxDir dir(aPath);
    dir.Traverse(ImageScanner(aPath, aRecursive, files));

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(files.begin(), files.end(), generator);
    img_index = 0;

    for (int i = 0; i < 3; i++)
    {
        LoadNextImage(true);
        Increment(true);
    }
    Bind(wxEVT_PAINT, &SaverFrame::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &SaverFrame::OnErase, this);
}

wxSize SaverFrame::getScaledSize(const wxSize &originalSize)
{
    double scale = 1;
    wxSize sz = GetSize();

    bool ratioBigger = sz.x * originalSize.y > sz.y * originalSize.x;

    if (ratioBigger && scaleMode == Config::SCALE::FILL || !ratioBigger && scaleMode == Config::SCALE::FIT)
        scale = sz.x * 1.0 / originalSize.x;

    if (!ratioBigger && scaleMode == Config::SCALE::FILL || ratioBigger && scaleMode == Config::SCALE::FIT)
        scale = sz.y * 1.0 / originalSize.y;

    return wxSize(originalSize.x * scale, originalSize.y * scale);
}

void SaverFrame::Draw()
{
    wxWindowDC dc(this);
    wxGraphicsContext *gc = renderer->CreateContext(dc);
    gc->SetInterpolationQuality(wxInterpolationQuality::wxINTERPOLATION_BEST);

    gc->SetBrush(*wxBLACK_BRUSH);
    gc->DrawRectangle(0, 0, GetSize().x, GetSize().y);

    drawBitmap(gc, currentBitmap(), currentBitmapSize());

    delete gc;
}

void SaverFrame::Transition(bool forward, double tick)
{
    wxWindowDC dc(this);
    wxGraphicsContext *gc = renderer->CreateContext(dc);
    gc->SetInterpolationQuality(wxInterpolationQuality::wxINTERPOLATION_BEST);

    gc->SetBrush(*wxBLACK_BRUSH);
    gc->DrawRectangle(0, 0, GetSize().x, GetSize().y);

    drawBitmap(gc, currentBitmap(), currentBitmapSize());
    gc->BeginLayer(tick);
    if (forward)
        drawBitmap(gc, nextBitmap(), nextBitmapSize());
    else
        drawBitmap(gc, prevBitmap(), prevBitmapSize());
    gc->EndLayer();

    delete gc;
}

void SaverFrame::drawBitmap(wxGraphicsContext *gc, wxGraphicsBitmap &bitmap, wxSize &originalSize)
{
    wxSize newSize = getScaledSize(originalSize);
    gc->DrawBitmap(bitmap, (GetSize().x - newSize.x) / 2, (GetSize().y - newSize.y) / 2, newSize.x, newSize.y);
}

void SaverFrame::Increment(bool forward)
{
    int increment = forward ? 1 : -1;
    bitmap_index += increment;
    bitmap_index %= 3;
}

void SaverFrame::LoadNextImage(bool forward)
{
    if (files.size() == 0)
        return;

    int increment = forward ? 1 : -1;
    wxString filename = files[img_index];
    img_index += increment;
    img_index %= files.size();
    wxImage img(filename);
    bitmaps[(bitmap_index + 1) % 3] = renderer->CreateBitmapFromImage(img);
    originalImgSizes[(bitmap_index + 1) % 3] = wxSize(img.GetWidth(), img.GetHeight());
}
