// Boite dialogue ordonnancement des headers grille échelle
// créé le 29/04/2004
// DlgOrdreHeader.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgOrdreHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Boîte de dialogue CDlgOrdreHeader
IMPLEMENT_DYNAMIC(CDlgOrdreHeader, CDialog)

////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CDlgOrdreHeader::CDlgOrdreHeader(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOrdreHeader::IDD, pParent)
{
	m_OrdreEntete.SetSize(3);
	m_OrdreEntete.SetAt(0,1);  
	m_OrdreEntete.SetAt(1,2);  
	m_OrdreEntete.SetAt(2,3);  
}

////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgOrdreHeader::~CDlgOrdreHeader()
{
}

void CDlgOrdreHeader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_CS_BK_ORDREHEADER, m_BkOrdreHeader);
	DDX_Control(pDX, IDC_CS_BTN_HEADER1, m_BtnHeader1);
	DDX_Control(pDX, IDC_CS_BTN_HEADER2, m_BtnHeader2);
	DDX_Control(pDX, IDC_CS_BTN_HEADER3, m_BtnHeader3);
}


BEGIN_MESSAGE_MAP(CDlgOrdreHeader, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_CS_BTN_HEADER1, OnBnClickedCsBtnHeader1)
	ON_BN_CLICKED(IDC_CS_BTN_HEADER2, OnBnClickedCsBtnHeader2)
	ON_BN_CLICKED(IDC_CS_BTN_HEADER3, OnBnClickedCsBtnHeader3)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue
BOOL CDlgOrdreHeader::OnInitDialog()
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// Mise en place des contrôles
	SetupControls();

	// Affichage des textes headers
	SetTexteHeader();

	// Centrage de la fenêtre
	this->CenterWindow();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des contrôles
void CDlgOrdreHeader::SetupControls()
{
	// Mise en place des couleurs
	SetColors();
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des couleurs
void CDlgOrdreHeader::SetColors()
{
	// Couleur du background
	m_BkOrdreHeader.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::MEDIUM_DEGRAD);
	m_BkOrdreHeader.SetSunken(true);

	// Les boutons
	m_BtnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);

	// Bouton positionnement header ligne 1
	m_BtnHeader1.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnHeader1.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnHeader1.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS );

	// Bouton positionnement header ligne 2
	m_BtnHeader2.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnHeader2.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnHeader2.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);

	// Bouton positionnement header ligne 3
	m_BtnHeader3.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnHeader3.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
	m_BtnHeader3.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS) , CATPColors::COLORCHOIXSUPPORTS);
}

void CDlgOrdreHeader::OnOk()
{
	// Validation du nouveau choix ordre entete
	OnOK();
}

//////////////////////////////////////////////////////////////////////////////////////
// Modification des entetes ligne 1
void CDlgOrdreHeader::OnBnClickedCsBtnHeader1()
{
	// Incrémente n° ordre
	JUnt32 Tmp			= m_OrdreEntete[0];
	m_OrdreEntete[0]	= m_OrdreEntete[1];
	m_OrdreEntete[1]	= Tmp;

	// Affichage nouveau texte
	SetTexteHeader();
}

//////////////////////////////////////////////////////////////////////////////////////
// Modification des entetes ligne 2
void CDlgOrdreHeader::OnBnClickedCsBtnHeader2()
{
	// Incrémente n° ordre
	JUnt32 Tmp			= m_OrdreEntete[1];
	m_OrdreEntete[1]	= m_OrdreEntete[2];
	m_OrdreEntete[2]	= Tmp;

	// Affichage nouveau texte
	SetTexteHeader();
}

//////////////////////////////////////////////////////////////////////////////////////
// Modification des entetes ligne 3
void CDlgOrdreHeader::OnBnClickedCsBtnHeader3()
{
	// Incrémente n° ordre
	JUnt32 Tmp			= m_OrdreEntete[0];
	m_OrdreEntete[0]	= m_OrdreEntete[2];
	m_OrdreEntete[2]	= Tmp;

	// Affichage nouveau texte
	SetTexteHeader();
}

//////////////////////////////////////////////////////////////////////////////////////
// Affichage des nouveaux textes header
void CDlgOrdreHeader::SetTexteHeader()
{
	// Modifie le texte header 1
	if (m_OrdreEntete[0] == 1)
		m_BtnHeader1.SetWindowText("Entêtes terrains");
	else if (m_OrdreEntete[0] == 2)
		m_BtnHeader1.SetWindowText("Entêtes cibles");
	else
		m_BtnHeader1.SetWindowText("Entêtes grandeurs");

	// Modifie le texte header 2
	if (m_OrdreEntete[1] == 1)
		m_BtnHeader2.SetWindowText("Entêtes terrains");
	else if (m_OrdreEntete[1] == 2)
		m_BtnHeader2.SetWindowText("Entêtes cibles");
	else
		m_BtnHeader2.SetWindowText("Entêtes grandeurs");

	// Modifie le texte header 3
	if (m_OrdreEntete[2] == 1)
		m_BtnHeader3.SetWindowText("Entêtes terrains");
	else if (m_OrdreEntete[2] == 2)
		m_BtnHeader3.SetWindowText("Entêtes cibles");
	else
		m_BtnHeader3.SetWindowText("Entêtes grandeurs");

}
