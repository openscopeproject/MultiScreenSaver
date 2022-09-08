#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/timer.h>
#include <wx/wx.h>

class SaverFrame : public wxFrame
{
  public:
    SaverFrame(const wxString &aPath, const wxPoint &aPos, const wxSize &aSize);

    void OnPaint(const wxPaintEvent &e);

    void LoadNextImage();

  private:
    wxGraphicsRenderer *renderer;
    wxImage img;
    int img_index;
    wxArrayString files;
};
