// DlgOffre2_Periode.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "Offre2Periode.h"
#include "Offre2MainDialog.h"

//=============================================================================	
// Boîte de dialogue COffre2Periode

IMPLEMENT_DYNAMIC(COffre2Periode, COffre2OngletBase)

//=============================================================================	
// Gestionnaires de messages de COffre2Periode
BEGIN_MESSAGE_MAP(COffre2Periode, COffre2OngletBase)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS,	IDC_FD_SPINS,		OnChangeSpinS)
	ON_NOTIFY(UDN_DELTAPOS,	IDC_FD_SPINJ,		OnChangeSpinJ)
	ON_NOTIFY(MCN_SELCHANGE,IDC_MONTHCALENDAR1, &COffre2Periode::OnStartCalendarSelChange)
	ON_NOTIFY(MCN_SELCHANGE,IDC_MONTHCALENDAR2, &COffre2Periode::OnEndCalendarSelChange)
END_MESSAGE_MAP()

//=============================================================================	
COffre2Periode::COffre2Periode(COffre2MainDialog * pDialogOffre)
//=============================================================================	
	:	COffre2OngletBase(IDD, pDialogOffre)
{
	m_bDateDReadOnly = false;
}

//=============================================================================	
COffre2Periode::~COffre2Periode()
//=============================================================================	
{
}

//=============================================================================	
void COffre2Periode::DoDataExchange(CDataExchange* pDX)
{
	COffre2OngletBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FD_SPINS,			m_SpinS);
	DDX_Control(pDX, IDC_FD_SPINJ,			m_SpinJ);
	DDX_Control(pDX, IDC_MONTHCALENDAR1,	m_StartCalendar);
	DDX_Control(pDX, IDC_MONTHCALENDAR2,	m_EndCalendar);
	DDX_Control(pDX, IDC_FD_PERIOD_TITRE1,	m_ttlTitre1);
	DDX_Control(pDX, IDC_FD_PERIOD_TITRE2,	m_ttlTitre2);
	DDX_Control(pDX, IDC_FD_PERIOD_TITRE3,	m_ttlTitre3);
	DDX_Control(pDX, IDC_FD_PERIOD_TITRE4,	m_ttlTitre4);
}

//=============================================================================	
HBRUSH COffre2Periode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//=============================================================================	
{
	const int id = pWnd->GetDlgCtrlID();
	switch (nCtlColor)
	{
		// on paint le dialogue lui même
		case CTLCOLOR_DLG:
		return m_pDlgOffre->GetBackgroundBrush();

		// il s'agit d'un contrôle STATIC
		case CTLCOLOR_STATIC:

		switch(id)
		{
			case IDC_FD_TEXTS:
			case IDC_FD_TEXTJ:
				// on modifie le pinceau et la couleur de fond
				pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
				return ::GetSysColorBrush(COLOR_WINDOW);
		}
		break;
	}

	// on 'paint' tout le reste en transparent
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

//=============================================================================	
// les fonctions pour manipuler la date de début
//=============================================================================	

JVoid COffre2Periode::SetDateD(JDate DateD)
{
	// on modifie la date de début
	m_DateD = DateD;
}

void COffre2Periode::SetDateDReadOnly(bool bReadOnly /*= true*/)
{
	m_bDateDReadOnly = bReadOnly;
}


JDate COffre2Periode::GetNewDateD() const
{
	// on renvoie la date de début
	return (m_DateND);
}

//=============================================================================	
// les fonctions pour manipuler la date de fin
//=============================================================================	

JVoid COffre2Periode::SetDateF(JDate DateF)
{
	// on modifie la date de fin
	m_DateF = DateF;
}

JDate COffre2Periode::GetNewDateF() const
{
	// on renvoie la date de fin
	return (m_DateNF);
}

//=============================================================================	
// la fonction pour calculer la durée maximum
JInt32 COffre2Periode::OnGetMaxDuree(JDate DateD) const
//=============================================================================	
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

//=============================================================================	
// la fonction pour mettre à jour la boîte
//=============================================================================	

VOID COffre2Periode::OnUpdateSpin()
{
	// on récupère la date de début
	SYSTEMTIME SysTimeD; m_StartCalendar.GetCurSel(&SysTimeD);

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

	m_EndCalendar.SetCurSel(&SysTimeF);

	// on met à jour les spins
	m_SpinS.SetPos(m_Duree / 7);
	m_SpinJ.SetPos(m_Duree % 7);
}

VOID COffre2Periode::OnUpdateDateFin()
{
	// on récupère la date de début
	SYSTEMTIME SysTimeD; m_StartCalendar.GetCurSel(&SysTimeD);

	// on récupère la date de fin
	SYSTEMTIME SysTimeF; m_EndCalendar.GetCurSel(&SysTimeF);

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

		m_EndCalendar.SetCurSel(&SysTimeF);
	}

	m_Duree = m_DateNF - m_DateND +1;

	// on calcule la durée maximum
	JInt32 Max = OnGetMaxDuree(m_DateND);

	// on corrige la durée de la période
	if (m_Duree < 1) m_Duree = 1; else if (m_Duree > Max) m_Duree = Max;

	JInt32 iYear, iMonth, iDay;

	// on définit la date de début
	//COleDateTimeSpan _1daySpan(1, 0, 0, 0);
	m_DateND.GetDate(iDay, iMonth, iYear);
	const COleDateTime & odEndmin = COleDateTime(iYear, iMonth, iDay, 0, 0, 0);// + _1daySpan;

	// on redéfinit la date max en fonction de l'éventuelle nouvelle date de début
	JDate dateMax;
	dateMax = m_DateND + Max - 1;
	dateMax.GetDate(iDay, iMonth, iYear);
	const COleDateTime & odEndmax = COleDateTime(iYear, iMonth, iDay, 0, 0, 0);
	
	// on bloque la date max à 1 journée + tard que la date nouvelle date de début
	m_EndCalendar.SetRange(&odEndmin, &odEndmax);

	// on met à jour les spins
	m_SpinS.SetPos(m_Duree / 7);
	m_SpinJ.SetPos(m_Duree % 7);
}

//=============================================================================	
BOOL COffre2Periode::OnInitDialog()
//=============================================================================	
{
	// on appelle le gestionnaire de base
	COffre2OngletBase::OnInitDialog();

	// couleur des titres
	m_ttlTitre1.SetFontName(CATPColors::GetFontLabelName());
	m_ttlTitre1.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlTitre1.SetBorder(false);
	m_ttlTitre1.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlTitre1.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	m_ttlTitre2.SetFontName(CATPColors::GetFontLabelName());
	m_ttlTitre2.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlTitre2.SetBorder(false);
	m_ttlTitre2.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlTitre2.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	m_ttlTitre3.SetFontName(CATPColors::GetFontLabelName());
	m_ttlTitre3.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlTitre3.SetBorder(false);
	m_ttlTitre3.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlTitre3.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	m_ttlTitre4.SetFontName(CATPColors::GetFontLabelName());
	m_ttlTitre4.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlTitre4.SetBorder(false);
	m_ttlTitre4.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlTitre4.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	// couleurs background des calendriers
	m_StartCalendar.SetColor(MCSC_BACKGROUND, CATPColors::GetColorSelect(m_pDlgOffre->GetAppColor()));
	m_EndCalendar.SetColor(MCSC_BACKGROUND, CATPColors::GetColorSelect(m_pDlgOffre->GetAppColor()));

	JInt32 iYear, iMonth, iDay;

	// on crée la date
	SYSTEMTIME SysTimeD;

	// on initialise les accélérateurs
	UDACCEL AccelJ[3] = { { 0, 1 }, { 2, 2 }, { 4, 3 } };
	UDACCEL AccelS[3] = { { 0, 1 }, { 1, 4 }, { 2, 8 } };

	// on configure les spins
	m_SpinJ.SetRange(0,  6); m_SpinJ.SetAccel(sizeof(AccelJ) / sizeof(AccelJ[0]), AccelJ); m_SpinJ.SetBase(10);
	m_SpinS.SetRange(0, 104); m_SpinS.SetAccel(sizeof(AccelS) / sizeof(AccelS[0]), AccelS); m_SpinS.SetBase(10);

	COleDateTime odStart, odEnd;

	// on vérifie la validité de la date de début
	if (m_DateD.IsValid())
	{
		// on décompose la date de début
		m_DateD.GetDate(iDay, iMonth, iYear);
		odStart = COleDateTime(iYear, iMonth, iDay, 0, 0, 0);

		// on met à jour la date de début
		SysTimeD.wYear         = static_cast<JInt16>(iYear);
		SysTimeD.wMonth        = static_cast<JInt16>(iMonth); 
		SysTimeD.wDayOfWeek    = 0;
		SysTimeD.wDay          = static_cast<JInt16>(iDay);
		SysTimeD.wHour         = 0;
		SysTimeD.wMinute       = 0;
		SysTimeD.wSecond       = 0;
		SysTimeD.wMilliseconds = 0;

		// on initialise la durée
		if (m_DateF.IsValid())
		{
			m_Duree = (m_DateF - m_DateD) + 1;
		}
		else
		{
			m_Duree = 364;
			m_DateF = m_DateD + m_Duree;
		}

		// on décompose la date de fin
		m_DateD.GetDate(iDay, iMonth, iYear);
		odEnd = COleDateTime(iYear, iMonth, iDay, 0, 0, 0);
	}
	else
	{
		// on récupère la date du système

		::GetLocalTime(&SysTimeD);

		// on met à jour la date de début
		SysTimeD.wDay = 01;
		SysTimeD.wMonth = 01;
		m_DateD.SetDate(SysTimeD.wDay, SysTimeD.wMonth, SysTimeD.wYear);
		odStart = COleDateTime(SysTimeD.wYear, SysTimeD.wMonth, SysTimeD.wDay, 0, 0, 0);
		odEnd = COleDateTime(SysTimeD.wYear, 12, 31, 0, 0, 0);
		m_DateF.SetDate(31,12,SysTimeD.wYear);

		// on initialise la durée
		m_Duree = (m_DateF-m_DateD)+1;
	}

	// on initialise la date de départ
	m_StartCalendar.SetCurSel(&SysTimeD);

	// on définit les limites de la date de départ
	JInt32 Max = this->OnGetMaxDuree(m_DateD);
	JDate dateMax;
	dateMax = m_DateD + Max-1;
	JInt32 Day, Month, Year; dateMax.GetDate(Day, Month, Year);

	// on crée les COleDateTime correspondants aux bornes de la classe JDate
	JDate::GetMin().GetDate(iDay, iMonth, iYear);
	const COleDateTime & dtMin = COleDateTime(iYear, iMonth, iDay, 0, 0, 0);
	JDate::GetMax().GetDate(iDay, iMonth, iYear);
	const COleDateTime & dtMax = COleDateTime(iYear, iMonth, iDay, 0, 0, 0);

	COleDateTimeSpan _1daySpan(1, 0, 0, 0);
	const COleDateTime odEndmin = COleDateTime(odStart + _1daySpan);
	m_StartCalendar.SetRange(&dtMin, &dtMax);
	m_EndCalendar.SetRange(&odEndmin, &COleDateTime(Year, Month, Day, 0, 0, 0));
	
	// on met à jour la boîte
	this->OnUpdateSpin();

	m_StartCalendar.EnableWindow(m_bDateDReadOnly ? FALSE:TRUE);

	// on quitte
	return (TRUE);
}

//=============================================================================	
//						Les gestionnaires d'evenements
//=============================================================================	


void COffre2Periode::OnChangeSpinS( NMHDR* pNMHDR, LRESULT* pResult )
{
	// on récupère les informations
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	// on met à jour la durée et la boîte
	if      (pNMUpDown->iDelta > 0) { m_Duree += pNMUpDown->iDelta * 7; OnUpdateSpin(); }
	else if (pNMUpDown->iDelta < 0) { m_Duree += pNMUpDown->iDelta * 7; OnUpdateSpin(); }

	// on annule le delta
	pNMUpDown->iDelta = 0;

	// on initialise le code de retour
	*pResult = 0;

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDlgOffre->EnableOK();
}

void COffre2Periode::OnChangeSpinJ( NMHDR* pNMHDR, LRESULT* pResult )
{
	// on récupère les informations
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	// on met à jour la durée et la boîte
	if      (pNMUpDown->iDelta > 0) { m_Duree += pNMUpDown->iDelta; OnUpdateSpin(); }
	else if (pNMUpDown->iDelta < 0) { m_Duree += pNMUpDown->iDelta; OnUpdateSpin(); }

	// on annule le delta
	pNMUpDown->iDelta = 0;

	// on initialise le code de retour
	*pResult = 0;

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDlgOffre->EnableOK();
}

//=============================================================================	
void COffre2Periode::OnStartCalendarSelChange(NMHDR *pNMHDR, LRESULT *pResult)
//=============================================================================	
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);

	// on met à jour la boîte
	OnUpdateDateFin();

	SYSTEMTIME SysTime;

	// on met à jour l'indicateur de date de début
	m_StartCalendar.GetCurSel(&SysTime);

	// on met à jour l'indicateur de date de fin
	m_EndCalendar.GetCurSel(&SysTime);

	*pResult = 0;

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDlgOffre->EnableOK();
}

//=============================================================================	
void COffre2Periode::OnEndCalendarSelChange(NMHDR *pNMHDR, LRESULT *pResult)
//=============================================================================	
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);

	// on met à jour la boîte
	OnUpdateDateFin();

	SYSTEMTIME SysTimeF;

	// on met à jour l'indicateur de date de fin
	m_EndCalendar.GetCurSel(&SysTimeF);

	*pResult = 0;

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDlgOffre->EnableOK();
}

//=============================================================================	
// retourne true si l'état de la sélection est correct
bool COffre2Periode::IsSelectionOk() const
//=============================================================================	
{
	return (m_DateND.IsValid() && m_DateNF.IsValid());
}