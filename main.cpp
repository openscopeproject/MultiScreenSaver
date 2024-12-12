/*
 * This file is part of MultiScreenSaver.
 *
 * Copyright (c) qu1ck 2022
 *
 * MultiScreenSaver is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * MultiScreenSaver is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with MultiScreenSaver.
 * If not, see https://www.gnu.org/licenses/.
 */

#include <wx/cmdline.h>
#include <wx/mstream.h>
#include <wx/wx.h>

#include <exif.h>

#include <vector>

#include "config.h"
#include "saverframe.h"

static const wxCmdLineEntryDesc g_cmdLineDesc[] = {
    {wxCMD_LINE_SWITCH, "h", "help", "Show command line help", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
    {wxCMD_LINE_OPTION, "c", "", "Show configuration dialog", wxCMD_LINE_VAL_STRING},
    {wxCMD_LINE_OPTION, "C", "", "Show configuration dialog", wxCMD_LINE_VAL_STRING},
    {wxCMD_LINE_OPTION, "p", "", "Show preview in child window", wxCMD_LINE_VAL_NUMBER},
    {wxCMD_LINE_OPTION, "P", "", "Show preview in child window", wxCMD_LINE_VAL_NUMBER},
    {wxCMD_LINE_SWITCH, "s", "", "Run the screensaver", wxCMD_LINE_VAL_NONE},
    {wxCMD_LINE_SWITCH, "S", "", "Run the screensaver", wxCMD_LINE_VAL_NONE},
    {wxCMD_LINE_PARAM, "i", "ignored", "Ignored parameter", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},

    {wxCMD_LINE_NONE}};

class rotatingJPEGHandler : public wxJPEGHandler
{
  public:
    virtual bool LoadFile(wxImage* image, wxInputStream& stream, bool verbose = true, int index = -1) override
    {
        wxMemoryInputStream stream_in_memory(stream, wxInvalidOffset);
        wxStreamBuffer* buf = stream_in_memory.GetInputStreamBuffer();

        easyexif::EXIFInfo result;
        int code = result.parseFrom((unsigned char*)buf->GetBufferPos(), buf->GetBytesLeft());

        if (!wxJPEGHandler::LoadFile(image, stream_in_memory, verbose, index))
        {
            return false;
        }
        if (code)
        {
            // exif parse error, ignore exif
            return true;
        }

        switch (result.Orientation)
        {
        case 1:
        case 2:
            // unrotated image
            break;
        case 3:
        case 4:
            // 180 deg off
            *image = image->Rotate180();
            break;
        case 5:
        case 6:
            // needs correction 90 deg clockwise
            *image = image->Rotate90(true);
            break;
        case 7:
        case 8:
            // needs correction 90 deg counterclockw
            *image = image->Rotate90(false);
            break;
        default:
            // unknown rotation
            break;
        }
        switch (result.Orientation)
        {
        case 2:
        case 4:
        case 5:
        case 7:
            // in these cases, the image was (also) mirrored
            *image = image->Mirror();
            break;
        default:
            // not mirrored (or unknown)
            break;
        }

        return true;
    }
};

class App : public wxApp
{
  public:
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    void OnTimer(const wxTimerEvent& e);
    void OnClose(wxEvent& e);
    void OnFrameClose(wxCloseEvent& e);
    void OnKey(wxKeyEvent& e);

    void StartScreensaver();

  private:
    void switchImage(bool forward);

    static BOOL CALLBACK monitorEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM lparam);

    Config m_config;
    bool m_showConfig = false;
    bool m_showPreview = false;
    long m_previewHwnd = 0;
    std::vector<RECT> m_monitors;
    std::vector<SaverFrame*> m_frames;
    std::vector<RenderWindow*> m_renderers;
    int m_updateRenderer = 0;
    wxTimer m_timer;
    wxIconBundle m_icons;
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxApp::OnInit();

    wxImage::AddHandler(new rotatingJPEGHandler());
    wxImage::AddHandler(new wxPNGHandler());

    m_icons = wxIconBundle("APP_ICON", 0);

    if (m_showConfig)
    {
        CONFIG_DIALOG* frame = new CONFIG_DIALOG(m_config);
        frame->SetIcons(m_icons);
        SetTopWindow(frame);
        frame->Show();
    }
    else if (m_showPreview)
    {
        wxWindow* wnd = new wxWindow();
#pragma warning(push)
#pragma warning(disable : 4312)
        wnd->SetHWND((WXHWND)m_previewHwnd);
#pragma warning(pop)

        RenderWindow* renderer =
            new RenderWindow(wnd, m_config.landscapeDir, m_config.recursive, m_config.scale, m_config.transition);
        m_renderers.push_back(renderer);
        SetTopWindow(renderer);
        renderer->Show();

        StartScreensaver();
    }
    else
    {
        LPARAM lparam = reinterpret_cast<LPARAM>(this);

        EnumDisplayMonitors(NULL, NULL, &App::monitorEnumProc, lparam);

        for (const auto& rect : m_monitors)
        {
            const wxString path =
                (rect.right - rect.left > rect.bottom - rect.top) ? m_config.landscapeDir : m_config.portraitDir;

            SaverFrame* frame = new SaverFrame(
                path, m_config.recursive, m_config.scale, m_config.transition,
                wxPoint(rect.left + m_config.margins, rect.top + m_config.margins),
                wxSize(rect.right - rect.left - 2 * m_config.margins, rect.bottom - rect.top - 2 * m_config.margins));

            frame->SetIcons(m_icons);
            frame->renderer->Bind(wxEVT_LEFT_UP, &App::OnClose, this);
            frame->renderer->Bind(wxEVT_CLOSE_WINDOW, &App::OnFrameClose, this);
            frame->renderer->Bind(wxEVT_KEY_DOWN, &App::OnKey, this);

            m_frames.push_back(frame);
            m_renderers.push_back(frame->renderer);
        }

        StartScreensaver();
    }

    return true;
}

void App::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(g_cmdLineDesc);
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser)
{
    m_showConfig = !parser.Found("s") && !parser.Found("S") && !parser.Found("p") && !parser.Found("P");
    m_showPreview = parser.Found("p", &m_previewHwnd) || parser.Found("P", &m_previewHwnd);

    return true;
}

void App::StartScreensaver()
{
    for (const auto& frame : m_frames)
        frame->Show();

    Bind(wxEVT_TIMER, &App::OnTimer, this);
    m_timer.Start(m_config.period * 1000);
}

void App::OnTimer(const wxTimerEvent& e)
{
    if (m_renderers.empty())
        return;

    switchImage(true);
}

void App::switchImage(bool forward)
{
    if (m_config.stagger)
    {
        if (forward)
            m_updateRenderer = (m_updateRenderer + 1) % m_renderers.size();

        RenderWindow* renderer = m_renderers[m_updateRenderer];

        if (m_config.transition != Config::TRANSITION::NONE)
        {
            for (double tick = 0.02; tick < 1.0; tick += .02)
            {
                renderer->Transition(forward, tick);
                wxMilliSleep(16);
            }
        }

        renderer->Increment(forward);
        renderer->Draw();
        renderer->LoadNextImage(forward);

        if (!forward)
            m_updateRenderer = (m_updateRenderer + m_renderers.size() - 1) % m_renderers.size();
    }
    else
    {
        if (m_config.transition != Config::TRANSITION::NONE)
        {
            for (double tick = 0.02; tick < 1.0; tick += .02)
            {
                for (const auto& renderer : m_renderers)
                    renderer->Transition(forward, tick);
                wxMilliSleep(16);
            }
        }
        for (const auto& renderer : m_renderers)
        {
            renderer->Increment(forward);
            renderer->Draw();
        }
        for (const auto& renderer : m_renderers)
            renderer->LoadNextImage(forward);
    }
}

BOOL CALLBACK App::monitorEnumProc(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM lparam)
{
    App* app = reinterpret_cast<App*>(lparam);

    app->m_monitors.push_back(*rect);

    return true;
}

void App::OnClose(wxEvent& e)
{
    for (const auto& frame : m_frames)
        frame->Close();
}

void App::OnFrameClose(wxCloseEvent& e)
{
    SaverFrame* frame = static_cast<SaverFrame*>(e.GetEventObject());
    std::vector<SaverFrame*>::iterator where = std::find(m_frames.begin(), m_frames.end(), frame);

    if (where != m_frames.end())
        m_frames.erase(where);

    frame->Destroy();
}

void App::OnKey(wxKeyEvent& e)
{
    if (e.GetUnicodeKey() == WXK_SPACE)
    {
        if (m_timer.IsRunning())
            m_timer.Stop();
        else
            m_timer.Start(m_config.period * 1000);
    }
    else if (e.GetKeyCode() == WXK_LEFT || e.GetKeyCode() == WXK_RIGHT)
    {
        switchImage(e.GetKeyCode() == WXK_RIGHT);
    }
    else if (e.GetKeyCode() == WXK_ESCAPE)
    {
        OnClose(wxCommandEvent());
    }
}
