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

#ifndef __CONFIG_H__
#define __CONFIG_H__

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
    bool stagger;

    Config();
    void SaveToRegistry();
};

class CONFIG_DIALOG : public CONFIG_BASE
{
  public:
    CONFIG_DIALOG(Config& config);

    void OnClose(wxCloseEvent& e);

  private:
    Config& m_config;
};

#endif // __CONFIG_H__
