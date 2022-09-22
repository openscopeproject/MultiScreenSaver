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
    : wxFrame(NULL, wxID_ANY, "PhotoScreensaver", aPos, aSize, 0 /*wxSTAY_ON_TOP */), scaleMode(aScale)
{
    renderer = wxGraphicsRenderer::GetDirect2DRenderer();

    wxDir dir(aPath);
    dir.Traverse(ImageScanner(aPath, aRecursive, files));

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(files.begin(), files.end(), generator);
    img_index = 0;

    LoadNextImage();

    Bind(wxEVT_PAINT, &SaverFrame::OnPaint, this);
    Bind(wxEVT_ERASE_BACKGROUND, &SaverFrame::OnErase, this);
}

void SaverFrame::Draw()
{
    wxWindowDC dc(this);
    wxGraphicsContext *gc = renderer->CreateContext(dc);
    gc->SetInterpolationQuality(wxInterpolationQuality::wxINTERPOLATION_BEST);

    // Rescale to fit
    double scale = 1;
    wxSize sz = GetSize();

    bool ratioBigger = sz.x * originalImgSize.y > sz.y * originalImgSize.x;

    if (ratioBigger && scaleMode == Config::SCALE::FILL || !ratioBigger && scaleMode == Config::SCALE::FIT)
        scale = sz.x * 1.0 / originalImgSize.x;

    if (!ratioBigger && scaleMode == Config::SCALE::FILL || ratioBigger && scaleMode == Config::SCALE::FIT)
        scale = sz.y * 1.0 / originalImgSize.y;

    wxSize newSize = wxSize(originalImgSize.x * scale, originalImgSize.y * scale);

    gc->SetBrush(*wxBLACK_BRUSH);
    gc->DrawRectangle(0, 0, sz.x, sz.y);
    gc->DrawBitmap(bitmap, (sz.x - newSize.x) / 2, (sz.y - newSize.y) / 2, newSize.x, newSize.y);

    delete gc;
}

void SaverFrame::LoadNextImage()
{
    if (files.size() == 0)
        return;

    wxString filename = files[img_index++];
    img_index %= files.size();
    wxImage img(filename);
    bitmap = renderer->CreateBitmapFromImage(img);
    originalImgSize = wxSize(img.GetWidth(), img.GetHeight());
}
