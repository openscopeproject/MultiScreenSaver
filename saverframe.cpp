#include <wx/dcgraph.h>
#include <wx/dir.h>

#include "saverframe.h"

SaverFrame::SaverFrame(const wxPoint &aPos, const wxSize &aSize)
    : wxFrame(NULL, wxID_ANY, "PhotoScreensaver", aPos, aSize, 0 /*wxSTAY_ON_TOP */)
{
    renderer = wxGraphicsRenderer::GetDirect2DRenderer();

    Bind(wxEVT_PAINT, &SaverFrame::OnPaint, this);

    for (const auto ext : {"*.jpg", "*.jpeg", "*.png"})
        wxDir::GetAllFiles("d:/downloads/wallpapers", &files, ext);

    loadRandomImage();

    Bind(wxEVT_TIMER, &SaverFrame::OnTimer, this);
    timer = new wxTimer(this);
    timer->Start(10000);
}

SaverFrame::~SaverFrame()
{
    timer->Stop();
    delete timer;
}

void SaverFrame::OnPaint(const wxPaintEvent &e)
{
    wxGraphicsBitmap bitmap = renderer->CreateBitmapFromImage(img);

    wxPaintDC dc(this);
    dc.SetBackground(*wxBLACK);
    dc.Clear();

    wxGraphicsContext *gc = renderer->CreateContext(this);

    wxGCDC gdc(gc);
    gdc.SetBackground(*wxBLACK);

    gc->DrawBitmap(bitmap, (this->GetSize().x - img.GetWidth()) / 2, (this->GetSize().y - img.GetHeight()) / 2,
                   img.GetWidth(), img.GetHeight());
}

void SaverFrame::loadRandomImage()
{
    if (files.size() == 0)
        return;

    wxString filename = files[rand() % files.size()];
    img = wxImage(filename);

    // Rescale to fit
    double scale;
    wxSize sz = this->GetSize();
    if (sz.x * img.GetHeight() > sz.y * img.GetWidth())
        scale = sz.x * 1.0 / img.GetWidth();
    else
        scale = sz.y * 1.0 / img.GetHeight();

    img.Rescale(ceil(img.GetWidth() * scale), ceil(img.GetHeight() * scale), wxIMAGE_QUALITY_BICUBIC);
}

void SaverFrame::OnTimer(const wxTimerEvent &e)
{
    loadRandomImage();
    Refresh();
}
