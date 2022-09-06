#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <wx/wx.h>

class SaverFrame : public wxFrame
{
  public:
    SaverFrame(const wxPoint &aPos, const wxSize &aSize);
    ~SaverFrame();

    void OnPaint(const wxPaintEvent &e);
    void OnTimer(const wxTimerEvent &e);

  private:
    void loadRandomImage();

    wxGraphicsRenderer *renderer;
    wxTimer *timer;
    wxImage img;
    wxArrayString files;
};
