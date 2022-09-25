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

RenderWindow::RenderWindow(wxWindow* parent, const wxString& aPath, const bool aRecursive, const Config::SCALE aScale)
    : wxWindow(parent, wxID_ANY, wxPoint(0, 0), parent->GetClientSize(), wxBORDER_NONE), m_scaleMode(aScale),
      m_bitmapIndex(0)
{
    m_renderer = wxGraphicsRenderer::GetDirect2DRenderer();

    wxDir dir(aPath);
    dir.Traverse(ImageScanner(aPath, aRecursive, m_files));

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(m_files.begin(), m_files.end(), generator);
    m_imgIndex = 0;

    for (int i = 0; i < 3; i++)
    {
        LoadNextImage(true);
        Increment(true);
    }
    Bind(wxEVT_PAINT, &RenderWindow::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &RenderWindow::OnErase, this);
}

wxSize RenderWindow::getScaledSize(const wxSize& originalSize)
{
    double scale = 1;
    wxSize sz = GetSize();

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
    gc->DrawRectangle(0, 0, GetSize().x, GetSize().y);

    drawBitmap(gc, currentBitmap(), currentBitmapSize());

    delete gc;
}

void RenderWindow::Transition(bool forward, double tick)
{
    wxWindowDC dc(GetParent());
    wxGraphicsContext* gc = m_renderer->CreateContext(dc);
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

void RenderWindow::drawBitmap(wxGraphicsContext* gc, wxGraphicsBitmap& bitmap, wxSize& originalSize)
{
    wxSize newSize = getScaledSize(originalSize);
    gc->DrawBitmap(bitmap, (GetSize().x - newSize.x) / 2, (GetSize().y - newSize.y) / 2, newSize.x, newSize.y);
}

void RenderWindow::Increment(bool forward)
{
    m_bitmapIndex = (m_bitmapIndex + (forward ? 1 : 2)) % 3;
}

void RenderWindow::LoadNextImage(bool forward)
{
    if (m_files.size() == 0)
        return;

    int increment = forward ? 1 : -1;
    wxString filename = m_files[m_imgIndex];
    m_imgIndex += increment;
    m_imgIndex = (m_imgIndex + m_files.size()) % m_files.size();
    wxImage img(filename);
    m_bitmaps[(m_bitmapIndex + increment + 3) % 3] = m_renderer->CreateBitmapFromImage(img);
    m_originalImgSizes[(m_bitmapIndex + increment + 3) % 3] = wxSize(img.GetWidth(), img.GetHeight());
}

SaverFrame::SaverFrame(const wxString& aPath, const bool aRecursive, const Config::SCALE aScale, const wxPoint& aPos,
                       const wxSize& aSize)
    : wxFrame(nullptr, wxID_ANY, "MultiScreenSaver", aPos, aSize, 0 /*wxSTAY_ON_TOP */)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    renderer = new RenderWindow(this, aPath, aRecursive, aScale);
    sizer->Add(renderer, 1, wxGROW, 0);
}
