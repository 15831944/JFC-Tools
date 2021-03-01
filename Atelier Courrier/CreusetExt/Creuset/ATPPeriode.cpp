// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPeriode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPeriode::CATPPeriode(CWnd* pParent) : CDialog(CATPPeriode::IDD, pParent)
{
	m_bDateDReadOnly = false;
	//{{AFX_DATA_INIT(CATPPeriode)
	//}}AFX_DATA_INIT
}

////////////////////////////////////////////////
// les fonctions pour manipuler la date de début

JVoid CATPPeriode::SetDateD(JDate DateD)
{
	// on modifie la date de début
	m_DateD = DateD;
}

void CATPPeriode::SetDateDReadOnly(bool bRead /*= true*/)
{
	m_bDateDReadOnly = bRead;
}


JDate CATPPeriode::GetDateD() const
{
	// on renvoie la date de début
	return (m_DateD);
}

//////////////////////////////////////////////
// les fonctions pour manipuler la date de fin

JVoid CATPPeriode::SetDateF(JDate DateF)
{
	// on modifie la date de fin
	m_DateF = DateF;
}

JDate CATPPeriode::GetDateF() const
{
	// on renvoie la date de fin
	return (m_DateF);
}

void CATPPeriode::DoDataExchange(CDataExchange* pDX)
{
	// on appelle le gestionnaire de base
	this->CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CATPPeriode)
	DDX_Control(pDX, IDC_CR_DATED, m_CtrlDateD);
	DDX_Control(pDX, IDC_CR_DATEF, m_CtrlDateF);
	DDX_Control(pDX, IDC_CR_SPINS, m_SpinS);
	DDX_Control(pDX, IDC_CR_SPINJ, m_SpinJ);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPPeriode, CDialog)
	//{{AFX_MSG_MAP(CATPPeriode)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_CR_DATED, OnChangeDateD)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_CR_DATEF, OnChangeDateF)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CR_SPINS, OnChangeSpinS)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CR_SPINJ, OnChangeSpinJ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////
// la fonction pour calculer la durée maximum

JInt32 CATPPeriode::OnGetMaxDuree(JDate DateD) const
{
	// on initialise le maximum
	JInt32 Max = 731;//2ans = 730j

	// on calcule la date de fin
	JDate DateF = DateD + Max;

	// on décompose les dates de début et de fin
	JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);
	JInt32 DayF, MonthF, YearF; DateF.GetDate(DayF, MonthF, YearF);

	// on corrige le maximum
	if (DayF != DayD) Max -= 1;

	// on renvoie le maximum
	return (Max);
}

//////////////////////////////////////////
// la fonction pour mettre à jour la boîte

VOID CATPPeriode::OnUpdateSpin()
{
	// on récupère la date de début
	SYSTEMTIME SysTimeD; m_CtrlDateD.GetTime(&SysTimeD);

	// on met à jour la date de début
	m_DateND.SetDate((JInt32)SysTimeD.wDay, (JInt32)SysTimeD.wMonth, (JInt32)SysTimeD.wYear);

	// on calcule la durée maximum
	JInt32 Max = this->OnGetMaxDuree(m_DateND);

	// on corrige la durée de la période
	if (m_Duree < 1) m_Duree = 1; else if (m_Duree > Max) m_Duree = Max;

	// on met à jour la date de fin
	m_DateNF = m_DateND + (m_Duree - 1);

	SYSTEMTIME SysTimeF;
	JInt32 Day, Month, Year; m_DateNF.GetDate(Day, Month, Year);

	// on met à jour la date de fin
	SysTimeF.wYear         = (JInt16)Year;
	SysTimeF.wMonth        = (JInt16)Month; 
	SysTimeF.wDayOfWeek    = 0;
	SysTimeF.wDay          = (JInt16)Day;
	SysTimeF.wHour         = 0;
	SysTimeF.wMinute       = 0;
	SysTimeF.wSecond       = 0;
	SysTimeF.wMilliseconds = 0;
	m_CtrlDateF.SetTime(&SysTimeF);


	// on met à jour les spins
	m_SpinS.SetPos(m_Duree / 7);
	m_SpinJ.SetPos(m_Duree % 7);
}

VOID CATPPeriode::OnUpdateDateFin()
{
	// on récupère la date de début
	SYSTEMTIME SysTimeD; m_CtrlDateD.GetTime(&SysTimeD);
	// on récupère la date de fin
	SYSTEMTIME SysTimeF; m_CtrlDateF.GetTime(&SysTimeF);

	// on met à jour la date de début
	m_DateND.SetDate((JInt32)SysTimeD.wDay, (JInt32)SysTimeD.wMonth, (JInt32)SysTimeD.wYear);
	m_DateNF.SetDate((JInt32)SysTimeF.wDay, (JInt32)SysTimeF.wMonth, (JInt32)SysTimeF.wYear);
	if(m_DateNF <= m_DateND)
	{
		m_DateNF = m_DateND+1;
		SYSTEMTIME SysTimeF;
		JInt32 Day, Month, Year; m_DateNF.GetDate(Day, Month, Year);
		// on met à jour la date de fin
		SysTimeF.wYear         = (JInt16)Year;
		SysTimeF.wMonth        = (JInt16)Month; 
		SysTimeF.wDayOfWeek    = 0;
		SysTimeF.wDay          = (JInt16)Day;
		SysTimeF.wHour         = 0;
		SysTimeF.wMinute       = 0;
		SysTimeF.wSecond       = 0;
		SysTimeF.wMilliseconds = 0;
		m_CtrlDateF.SetTime(&SysTimeF);
	}

	m_Duree = m_DateNF - m_DateND +1;

	// on calcule la durée maximum
	JInt32 Max = this->OnGetMaxDuree(m_DateND);

	// on corrige la durée de la période
	if (m_Duree < 1) m_Duree = 1; else if (m_Duree > Max) m_Duree = Max;

	JDate dateMax;
	dateMax = m_DateND + Max-1;
	JInt32 Day, Month, Year; dateMax.GetDate(Day, Month, Year);
	m_CtrlDateF.SetRange(NULL, &COleDateTime(Year,Month,Day,0,0,0));

	// on met à jour les spins
	m_SpinS.SetPos(m_Duree / 7);
	m_SpinJ.SetPos(m_Duree % 7);
}

//////////////////////////////////
// CATPPeriode message handlers

BOOL CATPPeriode::OnInitDialog() 
{
	// on crée la date
	SYSTEMTIME SysTimeD;

	// on initialise les accélérateurs
	UDACCEL AccelJ[3] = { { 0, 1 }, { 2, 2 }, { 4, 3 } };
	UDACCEL AccelS[3] = { { 0, 1 }, { 1, 4 }, { 2, 8 } };

	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// on configure les spins
	m_SpinJ.SetRange(0,  6); m_SpinJ.SetAccel(sizeof(AccelJ) / sizeof(AccelJ[0]), AccelJ); m_SpinJ.SetBase(10);
	m_SpinS.SetRange(0, 104); m_SpinS.SetAccel(sizeof(AccelS) / sizeof(AccelS[0]), AccelS); m_SpinS.SetBase(10);

	// on vérifie la validité de la date de début
	if (m_DateD.IsValid())
	{
		// on décompose la date de début
		JInt32 Day, Month, Year; m_DateD.GetDate(Day, Month, Year);

		// on met à jour la date de début
		SysTimeD.wYear         = (JInt16)Year;
		SysTimeD.wMonth        = (JInt16)Month; 
		SysTimeD.wDayOfWeek    = 0;
		SysTimeD.wDay          = (JInt16)Day;
		SysTimeD.wHour         = 0;
		SysTimeD.wMinute       = 0;
		SysTimeD.wSecond       = 0;
		SysTimeD.wMilliseconds = 0;

		// on initialise la durée
		// on initialise la durée
		if (m_DateF.IsValid())
			m_Duree = (m_DateF - m_DateD) + 1;
		else
		{
			m_Duree = 364;
			m_DateF = m_DateD + m_Duree;
		}
	}
	else
	{
		// on récupère la date du système

		::GetLocalTime(&SysTimeD);

		// on met à jour la date de début
		SysTimeD.wDay = 01;
		SysTimeD.wMonth = 01;
		m_DateD.SetDate(SysTimeD.wDay, SysTimeD.wMonth, SysTimeD.wYear);
		m_DateF.SetDate(31,12,SysTimeD.wYear);

		// on initialise la durée
		m_Duree = (m_DateF-m_DateD)+1;
	}

	// on initialise la date de départ
	m_CtrlDateD.SetTime(&SysTimeD);

	// on définit les limites de la date de départ
	JInt32 Max = this->OnGetMaxDuree(m_DateD);
	JDate dateMax;
	dateMax = m_DateD + Max-1;
	JInt32 Day, Month, Year; dateMax.GetDate(Day, Month, Year);
	m_CtrlDateF.SetRange(NULL, &COleDateTime(Year,Month,Day,0,0,0));

	SetupControls();
	
	// on met à jour la boîte
	this->OnUpdateSpin();

	m_CtrlDateD.EnableWindow( m_bDateDReadOnly ? FALSE:TRUE );

	// on quitte
	return (TRUE);
}

HBRUSH CATPPeriode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// on appelle le gestionnaire de base
	HBRUSH hBrush = this->CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// on teste s'il s'agit d'un contrôle STATIC
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		// on récupère l'ID du contrôle
		INT IDCtrl = pWnd->GetDlgCtrlID();

		// on modifie le pinceau et la couleur de fond
		if      (IDCtrl == IDC_CR_TEXTS) { hBrush = ::GetSysColorBrush(COLOR_WINDOW); pDC->SetBkColor(::GetSysColor(COLOR_WINDOW)); }
		else if (IDCtrl == IDC_CR_TEXTJ) { hBrush = ::GetSysColorBrush(COLOR_WINDOW); pDC->SetBkColor(::GetSysColor(COLOR_WINDOW)); }
	}

	// on renvoie le pinceau
	return (hBrush);
}

void CATPPeriode::OnChangeDateD(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// on met à jour la boîte
	this->OnUpdateDateFin();

	// on initialise le code de retour
	*pResult = 0;
}

void CATPPeriode::OnChangeDateF(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// on met à jour la boîte
	this->OnUpdateDateFin();

	// on initialise le code de retour
	*pResult = 0;
}

void CATPPeriode::OnChangeSpinS(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// on récupère les informations
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	// on met à jour la durée et la boîte
	if      (pNMUpDown->iDelta > 0) { m_Duree += pNMUpDown->iDelta * 7; this->OnUpdateSpin(); }
	else if (pNMUpDown->iDelta < 0) { m_Duree += pNMUpDown->iDelta * 7; this->OnUpdateSpin(); }

	// on annule le delta
	pNMUpDown->iDelta = 0;

	// on initialise le code de retour
	*pResult = 0;
}

void CATPPeriode::OnChangeSpinJ(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// on récupère les informations
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	// on met à jour la durée et la boîte
	if      (pNMUpDown->iDelta > 0) { m_Duree += pNMUpDown->iDelta; this->OnUpdateSpin(); }
	else if (pNMUpDown->iDelta < 0) { m_Duree += pNMUpDown->iDelta; this->OnUpdateSpin(); }

	// on annule le delta
	pNMUpDown->iDelta = 0;

	// on initialise le code de retour
	*pResult = 0;
}

void CATPPeriode::OnCancel() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnCancel();
}

void CATPPeriode::OnOK() 
{
	// on met à jour les dates
	m_DateD = m_DateND; m_DateF = m_DateNF;

	// on appelle le gestionnaire de base
	this->CDialog::OnOK();
}

void CATPPeriode::SetupControls()
{
	m_Backgnd.SubclassDlgItem(IDC_CR_BK, this);
	m_Ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_Txt1.SubclassDlgItem(IDC_CR_TXT, this);
	m_Txt2.SubclassDlgItem(IDC_CR_TXT2, this);
	m_Txt3.SubclassDlgItem(IDC_CR_TXT3, this);
	m_Txt4.SubclassDlgItem(IDC_CR_TXT4, this);
	m_Txt5.SubclassDlgItem(IDC_CR_TXT5, this);
	m_btnOk.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	SetColors();
}

void CATPPeriode::SetColors()
{
	m_Backgnd.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Backgnd.SetSunken(true);
	
	m_CtrlDateD.SetMonthCalColor(MCSC_TITLEBK, CATPColors::GetColorDark(CATPColors::COLORCREUSET));
	m_CtrlDateF.SetMonthCalColor(MCSC_TITLEBK, CATPColors::GetColorDark(CATPColors::COLORCREUSET));
	
	m_Ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_Txt1.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt1.SetTextColor(CATPColors::GetColorBlack());
	
	m_Txt2.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt2.SetTextColor(CATPColors::GetColorBlack());
	
	m_Txt3.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt3.SetTextColor(CATPColors::GetColorBlack());
	m_Txt4.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt4.SetTextColor(CATPColors::GetColorBlack());
	
	m_Txt5.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt5.SetTextColor(CATPColors::GetColorBlack());
	

	m_btnOk.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
}
