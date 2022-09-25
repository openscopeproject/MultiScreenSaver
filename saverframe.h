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
        return m_bitmaps[m_bitmapIndex % 3];
    };
    wxGraphicsBitmap& nextBitmap()
    {
        return m_bitmaps[(m_bitmapIndex + 1) % 3];
    };
    wxGraphicsBitmap& prevBitmap()
    {
        return m_bitmaps[(m_bitmapIndex + 2) % 3];
    };
    wxSize& currentBitmapSize()
    {
        return m_originalImgSizes[m_bitmapIndex % 3];
    };
    wxSize& nextBitmapSize()
    {
        return m_originalImgSizes[(m_bitmapIndex + 1) % 3];
    };
    wxSize& prevBitmapSize()
    {
        return m_originalImgSizes[(m_bitmapIndex + 2) % 3];
    };

    void drawBitmap(wxGraphicsContext* gc, wxGraphicsBitmap& bitmap, wxSize& originalSize);

    wxSize getScaledSize(const wxSize& originalSize);

    wxGraphicsRenderer* m_renderer;
    wxSize m_originalImgSizes[3];
    wxGraphicsBitmap m_bitmaps[3];
    Config::SCALE m_scaleMode;
    int m_imgIndex;
    int m_bitmapIndex;
    wxArrayString m_files;
};

class SaverFrame : public wxFrame
{
  public:
    SaverFrame(const wxString& aPath, const bool aRecursive, const Config::SCALE aScale, const wxPoint& aPos,
               const wxSize& aSize);

    RenderWindow* renderer;
};
