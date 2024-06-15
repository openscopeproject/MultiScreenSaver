///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "config_base.h"

///////////////////////////////////////////////////////////////////////////

CONFIG_BASE::CONFIG_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Image folders") ), wxVERTICAL );

	m_panel2 = new wxPanel( sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, wxT("Landscape"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_landscapeDir = new wxDirPickerCtrl( m_panel2, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer1->Add( m_landscapeDir, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("Portrait"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_portraitDir = new wxDirPickerCtrl( m_panel2, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer1->Add( m_portraitDir, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );


	m_panel2->SetSizer( fgSizer1 );
	m_panel2->Layout();
	fgSizer1->Fit( m_panel2 );
	sbSizer1->Add( m_panel2, 1, wxEXPAND, 5 );

	m_recursiveScan = new wxCheckBox( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Scan folders recursively"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_recursiveScan, 0, wxALL, 5 );


	bSizer3->Add( sbSizer1, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Slideshow settings") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->AddGrowableCol( 3 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText5 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Scale image to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer2->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_scaleChoices[] = { wxT("Fill"), wxT("Fit"), wxT("Original") };
	int m_scaleNChoices = sizeof( m_scaleChoices ) / sizeof( wxString );
	m_scale = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_scaleNChoices, m_scaleChoices, 0 );
	m_scale->SetSelection( 0 );
	fgSizer2->Add( m_scale, 0, wxALL|wxEXPAND, 5 );

	m_staticText9 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Transition"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer2->Add( m_staticText9, 0, wxALL, 5 );

	wxString m_transitionChoices[] = { wxT("None"), wxT("Blend") };
	int m_transitionNChoices = sizeof( m_transitionChoices ) / sizeof( wxString );
	m_transition = new wxChoice( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_transitionNChoices, m_transitionChoices, 0 );
	m_transition->SetSelection( 1 );
	fgSizer2->Add( m_transition, 0, wxALL|wxEXPAND, 5 );

	m_staticText3 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Update period (sec)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_period = new wxSpinCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS, 1, 10000, 10 );
	fgSizer2->Add( m_period, 0, wxALL|wxEXPAND, 5 );

	m_staticText4 = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Window margins (px)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer2->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_margins = new wxSpinCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS, 0, 1000, 0 );
	fgSizer2->Add( m_margins, 0, wxALL|wxEXPAND, 5 );


	sbSizer2->Add( fgSizer2, 0, wxEXPAND, 5 );

	m_stagger = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Stagger updates on different monitors"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_stagger, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( sbSizer2, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Keyboard actions") ), wxVERTICAL );

	m_staticText6 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("<Space> - pause/resume slideshow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	sbSizer3->Add( m_staticText6, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("Arrow keys - next/previous image"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	sbSizer3->Add( m_staticText7, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( sbSizer3->GetStaticBox(), wxID_ANY, wxT("<Escape> or mouse click - exit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	sbSizer3->Add( m_staticText8, 0, wxALL, 5 );


	bSizer3->Add( sbSizer3, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	m_panel1->SetSizer( bSizer3 );
	m_panel1->Layout();
	bSizer3->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CONFIG_BASE::OnClose ) );
}

CONFIG_BASE::~CONFIG_BASE()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CONFIG_BASE::OnClose ) );

}
