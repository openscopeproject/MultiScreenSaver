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

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Landscape"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_landscapeDir = new wxDirPickerCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer1->Add( m_landscapeDir, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("Portrait"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_portraitDir = new wxDirPickerCtrl( sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer1->Add( m_portraitDir, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );


	sbSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );


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


	fgSizer2->Add( 0, 0, 0, 0, 5 );


	fgSizer2->Add( 0, 0, 0, 0, 5 );

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

	m_preload = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Preload images (uses more RAM)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_preload->SetValue(true);
	sbSizer2->Add( m_preload, 0, wxALL|wxEXPAND, 5 );

	m_stagger = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, wxT("Stagger updates on different monitors"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_stagger, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( sbSizer2, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );


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
