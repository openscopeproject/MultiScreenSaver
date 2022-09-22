#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include "config.h"

class SaverFrame : public wxFrame
{
  public:
    SaverFrame(const wxString &aPath, const bool aRecursive, const Config::SCALE aScale, const wxPoint &aPos,
               const wxSize &aSize);

    void OnPaint(const wxPaintEvent &e){};
    void OnErase(const wxEraseEvent &e){};
    void Draw();

    void LoadNextImage();

  private:
    wxGraphicsRenderer *renderer;
    wxSize originalImgSize;
    wxGraphicsBitmap bitmap;
    Config::SCALE scaleMode;
    int img_index;
    wxArrayString files;
};
