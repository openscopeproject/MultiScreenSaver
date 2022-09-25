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

#ifndef __SAVERFRAME_H__
#define __SAVERFRAME_H__

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

#endif // __SAVERFRAME_H__
