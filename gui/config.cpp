#include <wx/config.h>

#include "config.h"

Config::Config()
{
    // read settings from registry
    wxRegConfig reg("PhotoScreensaver", "OpenScopeProject");

    landscapeDir = reg.Read("landscapeDir", "");
    portraitDir = reg.Read("portraitDir", "");
    recursive = reg.ReadBool("recursive", true);
    scale = static_cast<SCALE>(reg.ReadLong("scale", SCALE::FILL));
    period = reg.ReadLong("period", 10);
    margins = reg.ReadLong("margins", 0);
    preload = reg.ReadBool("preload", true);
    stagger = reg.ReadBool("stagger", false);
}

void Config::SaveToRegistry()
{
    wxRegConfig reg("PhotoScreensaver", "OpenScopeProject");

    reg.Write("landscapeDir", landscapeDir);
    reg.Write("portraitDir", portraitDir);
    reg.Write("recursive", recursive);
    reg.Write("scale", static_cast<long>(scale));
    reg.Write("period", period);
    reg.Write("margins", margins);
    reg.Write("preload", preload);
    reg.Write("stagger", stagger);
}

CONFIG_DIALOG::CONFIG_DIALOG(Config &config) : CONFIG_BASE(nullptr), m_config(config)
{
    m_landscapeDir->SetPath(m_config.landscapeDir);
    m_portraitDir->SetPath(m_config.portraitDir);
    m_recursiveScan->SetValue(m_config.recursive);
    m_scale->SetSelection(static_cast<long>(m_config.scale));
    m_period->SetValue(m_config.period);
    m_margins->SetValue(m_config.margins);
    m_preload->SetValue(m_config.preload);
    m_stagger->SetValue(m_config.stagger);

    SetSizeHints(500, -1);
    Fit();
}

void CONFIG_DIALOG::OnClose(wxCloseEvent &e)
{
    m_config.landscapeDir = m_landscapeDir->GetPath();
    m_config.portraitDir = m_portraitDir->GetPath();
    m_config.recursive = m_recursiveScan->GetValue();
    m_config.scale = static_cast<Config::SCALE>(m_scale->GetSelection());
    m_config.period = m_period->GetValue();
    m_config.margins = m_margins->GetValue();
    m_config.preload = m_preload->GetValue();
    m_config.stagger = m_stagger->GetValue();

    m_config.SaveToRegistry();

    Destroy();
}
