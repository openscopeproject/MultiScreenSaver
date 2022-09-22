#ifndef __config__
#define __config__

#include <wx/wx.h>

#include "config_base.h"

struct Config
{
    wxString landscapeDir;
    wxString portraitDir;
    bool recursive;
    enum SCALE
    {
        FILL,
        FIT,
        ORIGINAL,
    } scale;
    int period;
    int margins;
    bool preload;
    bool stagger;

    Config();
    void SaveToRegistry();
};

class CONFIG_DIALOG : public CONFIG_BASE
{
  public:
    CONFIG_DIALOG(Config &config);

    void OnClose(wxCloseEvent &e);

  private:
    Config &m_config;
};

#endif // __config__
