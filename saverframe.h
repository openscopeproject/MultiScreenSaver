#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include "config.h"

class RenderWindow : public wxWindow
{
  public:
    RenderWindow(wxWindow* parent, const wxString& aPath, const bool aRecursive, const Config::SCALE aScale);

    void OnPaint(const wxPaintEvent& e)
    {
        Draw();
    };
    void OnErase(const wxEraseEvent& e){};
    void Draw();
    void Transition(bool forward, double tick);
    void Increment(bool forward);

    void LoadNextImage(bool forward);

  private:
    wxGraphicsBitmap& currentBitmap()
    {
        return bitmaps[bitmap_index % 3];
    };
    wxGraphicsBitmap& nextBitmap()
    {
        return bitmaps[(bitmap_index + 1) % 3];
    };
    wxGraphicsBitmap& prevBitmap()
    {
        return bitmaps[(bitmap_index + 2) % 3];
    };
    wxSize& currentBitmapSize()
    {
        return originalImgSizes[bitmap_index % 3];
    };
    wxSize& nextBitmapSize()
    {
        return originalImgSizes[(bitmap_index + 1) % 3];
    };
    wxSize& prevBitmapSize()
    {
        return originalImgSizes[(bitmap_index + 2) % 3];
    };

    void drawBitmap(wxGraphicsContext* gc, wxGraphicsBitmap& bitmap, wxSize& originalSize);

    wxSize getScaledSize(const wxSize& originalSize);

    wxGraphicsRenderer* renderer;
    wxSize originalImgSizes[3];
    wxGraphicsBitmap bitmaps[3];
    Config::SCALE scaleMode;
    int img_index;
    int bitmap_index;
    wxArrayString files;
};

class SaverFrame : public wxFrame
{
  public:
    SaverFrame(const wxString& aPath, const bool aRecursive, const Config::SCALE aScale, const wxPoint& aPos,
               const wxSize& aSize);

    RenderWindow* renderer;
};
