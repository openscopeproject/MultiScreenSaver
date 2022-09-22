#include <vector>
#include <wx/cmdline.h>
#include <wx/wx.h>

#include "config.h"
#include "saverframe.h"

static const wxCmdLineEntryDesc g_cmdLineDesc[] = {
    {wxCMD_LINE_SWITCH, "h", "help", "Show command line help", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
    {wxCMD_LINE_OPTION, "c", "", "Show configuration dialog", wxCMD_LINE_VAL_NONE},
    {wxCMD_LINE_OPTION, "C", "", "Show configuration dialog", wxCMD_LINE_VAL_NONE},
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

    void OnTimer(const wxTimerEvent &e);
    void OnClose(wxEvent &e);
    void OnFrameClose(wxCloseEvent &e);

    void StartScreensaver();
    int enumerated_monitor = 1;

  private:
    Config config;
    bool show_config = false;
    bool show_preview = false;
    long preview_hwnd = 0;
    static BOOL CALLBACK monitorEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM lparam);

    std::vector<RECT> monitors;
    std::vector<SaverFrame *> frames;
    int update_frame = 0;
    wxTimer timer;
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxApp::OnInit();

    wxImage::AddHandler(new wxJPEGHandler());
    wxImage::AddHandler(new wxPNGHandler());

    if (show_config)
    {
        CONFIG_DIALOG *frame = new CONFIG_DIALOG(config);
        SetTopWindow(frame);
        frame->Show();
    }
    else if (show_preview)
    {
        // TODO
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
    LPARAM lparam = reinterpret_cast<LPARAM>(this);

    EnumDisplayMonitors(NULL, NULL, &App::monitorEnumProc, lparam);

    for (const auto &rect : monitors)
    {
        const wxString path =
            (rect.right - rect.left > rect.bottom - rect.top) ? config.landscapeDir : config.portraitDir;

        SaverFrame *frame = new SaverFrame(
            path, config.recursive, config.scale, wxPoint(rect.left + config.margins, rect.top + config.margins),
            wxSize(rect.right - rect.left - 2 * config.margins, rect.bottom - rect.top - 2 * config.margins));

        frame->Show();
        frame->Draw();
        frame->Bind(wxEVT_LEFT_UP, &App::OnClose, this);
        frame->Bind(wxEVT_CLOSE_WINDOW, &App::OnFrameClose, this);

        frames.push_back(frame);
    }

    Bind(wxEVT_TIMER, &App::OnTimer, this);
    timer.Start(config.period * 1000);
}

void App::OnTimer(const wxTimerEvent &e)
{
    if (frames.empty())
        return;

    if (config.stagger)
    {
        auto frame = frames[update_frame++];
        update_frame %= frames.size();
        frame->LoadNextImage();
        frame->Draw();
    }
    else
    {
        for (const auto &frame : frames)
            frame->LoadNextImage();
        for (const auto &frame : frames)
            frame->Draw();
    }
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
        frame->Close();
}

void App::OnFrameClose(wxCloseEvent &e)
{
    SaverFrame *frame = static_cast<SaverFrame *>(e.GetEventObject());
    std::vector<SaverFrame *>::iterator where = std::find(frames.begin(), frames.end(), frame);

    if (where != frames.end())
        frames.erase(where);

    frame->Destroy();
}
