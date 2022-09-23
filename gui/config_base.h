///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CONFIG_BASE
///////////////////////////////////////////////////////////////////////////////
class CONFIG_BASE : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel1;
		wxStaticText* m_staticText1;
		wxDirPickerCtrl* m_landscapeDir;
		wxStaticText* m_staticText2;
		wxDirPickerCtrl* m_portraitDir;
		wxCheckBox* m_recursiveScan;
		wxStaticText* m_staticText5;
		wxChoice* m_scale;
		wxStaticText* m_staticText3;
		wxSpinCtrl* m_period;
		wxStaticText* m_staticText4;
		wxSpinCtrl* m_margins;
		wxCheckBox* m_stagger;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }


	public:

		CONFIG_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("PhotoScreensaver configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxSTAY_ON_TOP|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~CONFIG_BASE();

};

