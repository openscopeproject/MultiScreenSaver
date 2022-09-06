#include <vector>
#include <wx/cmdline.h>
#include <wx/wx.h>

#include "config.h"
#include "saverframe.h"

static const wxCmdLineEntryDesc g_cmdLineDesc[] = {
    {wxCMD_LINE_SWITCH, "h", "help", "Show command line help", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
    {wxCMD_LINE_SWITCH, "c", "", "Show configuration dialog", wxCMD_LINE_VAL_NONE},
    {wxCMD_LINE_SWITCH, "C", "", "Show configuration dialog", wxCMD_LINE_VAL_NONE},
    {wxCMD_LINE_OPTION, "p", "", "Show preview in child window", wxCMD_LINE_VAL_NUMBER},
    {wxCMD_LINE_OPTION, "P", "", "Show preview in child window", wxCMD_LINE_VAL_NUMBER},
    {wxCMD_LINE_SWITCH, "s", "", "Run the screensaver", wxCMD_LINE_VAL_NONE},
    {wxCMD_LINE_SWITCH, "S", "", "Run the screensaver", wxCMD_LINE_VAL_NONE},

    {wxCMD_LINE_NONE}};

class App : public wxApp
{
  public:
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser &parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser &parser);

    void OnClose(wxEvent &e);
    void OnFrameClose(wxCloseEvent &e);

    void StartScreensaver();
    int enumerated_monitor = 1;

  private:
    bool show_config = false;
    bool show_preview = false;
    long preview_hwnd = 0;
    static BOOL CALLBACK monitorEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM lparam);

    std::vector<RECT> monitors;
    std::vector<SaverFrame *> frames;
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxApp::OnInit();

    wxImage::AddHandler(new wxJPEGHandler());
    wxImage::AddHandler(new wxPNGHandler());

    if (show_config)
    {
        CONFIG *frame = new CONFIG(nullptr);
        frame->Show();
    }
    else if (show_preview)
    {
        return false;
    }
    else
    {
        StartScreensaver();
    }

    return true;
}

void App::OnInitCmdLine(wxCmdLineParser &parser)
{
    parser.SetDesc(g_cmdLineDesc);
}

bool App::OnCmdLineParsed(wxCmdLineParser &parser)
{
    show_config = !parser.Found("s") && !parser.Found("S") && !parser.Found("p") && !parser.Found("P");
    show_preview = parser.Found("p", &preview_hwnd) || parser.Found("P", &preview_hwnd);

    return true;
}

void App::StartScreensaver()
{
    srand(time(0));

#define MARGIN 30
    LPARAM lparam = reinterpret_cast<LPARAM>(this);

    EnumDisplayMonitors(NULL, NULL, &App::monitorEnumProc, lparam);

    for (const auto &rect : monitors)
    {
        SaverFrame *frame =
            new SaverFrame(wxPoint(rect.left + MARGIN, rect.top + MARGIN),
                           wxSize(rect.right - rect.left - 2 * MARGIN, rect.bottom - rect.top - 2 * MARGIN));

        frame->Show();
        frame->Bind(wxEVT_LEFT_UP, &App::OnClose, this);
        frame->Bind(wxEVT_CLOSE_WINDOW, &App::OnFrameClose, this);

        frames.push_back(frame);
    }
#undef MARGIN
}

BOOL CALLBACK App::monitorEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM lparam)
{
    App *app = reinterpret_cast<App *>(lparam);

    app->monitors.push_back(*rect);

    return true;
}

void App::OnClose(wxEvent &e)
{
    for (const auto &frame : frames)
    {
        frame->Close();
    }
}

void App::OnFrameClose(wxCloseEvent &e)
{
    SaverFrame *frame = static_cast<SaverFrame *>(e.GetEventObject());
    std::vector<SaverFrame *>::iterator where = std::find(frames.begin(), frames.end(), frame);

    if (where != frames.end())
        frames.erase(where);

    frame->Destroy();
}
