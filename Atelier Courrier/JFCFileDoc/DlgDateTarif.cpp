// Boite dialogue s�lection date tarifaire
// cr�� le 20/04/2004
// DlgDateTarif.cpp : fichier d'impl�mentation
//
#include "stdafx.h"
#include "DlgDateTarif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// Bo�te de dialogue CDlgDateTarif
IMPLEMENT_DYNAMIC(CDlgDateTarif, CDialog)

////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CDlgDateTarif::CDlgDateTarif(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDateTarif::IDD, pParent)
{
	// Date Tarifaire (par d�faut date du jour)
	COleDateTime DateCur = COleDateTime::GetCurrentTime();
	m_DateTar.SetDate(DateCur.GetDay(),DateCur.GetMonth(),DateCur.GetYear());  
}

////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgDateTarif::~CDlgDateTarif()
{
}

////////////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue
BOOL CDlgDateTarif::OnInitDialog()
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// R�cup date cur
	JInt32 Jour,Mois,Annee;
	m_DateTar.GetDate(Jour,Mois,Annee);

	// on initialise controle date de tarification d�part
	COleDateTime DateCur;
	DateCur.SetDate(Annee,Mois,Jour);
	if (!m_CtrlDateTar.SetTime(DateCur)) return FALSE;

	// Mise en place des contr�les
	SetupControls();

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des contr�les
void CDlgDateTarif::SetupControls()
{
	// Mise en place des couleurs
	SetColors();
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des couleurs
void CDlgDateTarif::SetColors(CATPColors::APPCOLOR app)
{
	m_appColor = app;
}

void CDlgDateTarif::SetColors()
{
	// Couleur du background
	m_BkDateTar.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_BkDateTar.SetSunken(true);

	// Label date tarification
	m_LabelDateTar.SetBkColor(CATPColors::GetColorMedium(m_appColor),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_LabelDateTar.SetTextColor(CATPColors::GetColorSuperDark(m_appColor));

	m_CtrlDateTar.SetMonthCalColor(MCSC_TITLEBK, CATPColors::GetColorDark(m_appColor));
		
	// Les boutons
	m_BtnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor );
	m_BtnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor );
}

////////////////////////////////////////////////////////////////////////////////////////
// Modification  de la date de tarification
JVoid CDlgDateTarif::SetDateTar(JDate DateTar)
{
	// on modifie la date de tarification
	m_DateTar = DateTar;
}

////////////////////////////////////////////////////////////////////////////////////////
// Exit boite de dialogue avec r�cup�ration nouvelle date saisie
void CDlgDateTarif::OnOk()
{
	// R�cup�ration de la nouvelle date tarification
	SYSTEMTIME SysTimeD; 
	m_CtrlDateTar.GetTime(&SysTimeD);

	// on met � jour la date de d�but
	m_DateTar.SetDate((JInt32)SysTimeD.wDay, (JInt32)SysTimeD.wMonth, (JInt32)SysTimeD.wYear);

	// Puis on sort de la boite de dialogue
	OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de la date de tarification
JDate CDlgDateTarif::GetDateTar() const
{
	// on renvoie la date de tarification
	return (m_DateTar);
}

void CDlgDateTarif::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_LABEL_DATETAR, m_LabelDateTar);
	DDX_Control(pDX, IDC_CS_DATETAR_SEL, m_CtrlDateTar);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_CS_BK_DATETAR, m_BkDateTar);
}

BEGIN_MESSAGE_MAP(CDlgDateTarif, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()


