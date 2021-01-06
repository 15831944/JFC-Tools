// MyTabCtrl.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "OffreTabCtrl.h"

#include "DlgOffre2.h"


// COffreTabCtrl

IMPLEMENT_DYNAMIC(COffreTabCtrl, CTabCtrl)

// Gestionnaires de messages de COffreTabCtrl

BEGIN_MESSAGE_MAP(COffreTabCtrl, CTabCtrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, &COffreTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()


COffreTabCtrl::COffreTabCtrl(CDialogOffre2 * pDialogOffre2)
{
	ASSERT(pDialogOffre2);

	// on initialise les Id des dialogues des différentes pages	
	m_DialogID[CDialogOffre2::eodoPeriode]	= COngletOffre2_Periode::IDD;
	m_DialogID[CDialogOffre2::eodoTitres]	= COngletOffre2_Titres::IDD;
	m_DialogID[CDialogOffre2::eodoCibles]	= COngletOffre2_Cibles::IDD;
	m_DialogID[CDialogOffre2::eodoFormats]	= COngletOffre2_Formats::IDD;

	// on initialise les dialogues des différentes pages	
	m_ppOnglets[CDialogOffre2::eodoPeriode]	= new COngletOffre2_Periode;
	m_ppOnglets[CDialogOffre2::eodoTitres]		= new COngletOffre2_Titres;
	m_ppOnglets[CDialogOffre2::eodoCibles]		= new COngletOffre2_Cibles;
	m_ppOnglets[CDialogOffre2::eodoFormats]	= new COngletOffre2_Formats;
	m_pCurPage								= 0;

	m_CurOngletIdx							= CDialogOffre2::eodoPeriode;
	m_appColor								= CATPColors::CREUSET;

	// on définit le ptr vers le dialogue Offre dns chq page à onglet
	for (int idx=0; idx < CDialogOffre2::eodoNumOnglets; ++idx)
		m_ppOnglets[idx]->SetData(pDialogOffre2);
}

COffreTabCtrl::~COffreTabCtrl()
{
}

BOOL COffreTabCtrl::InitDialogs()
{
	BOOL bCreation = TRUE;
	bCreation &= m_ppOnglets[CDialogOffre2::eodoPeriode]->Create(m_DialogID[CDialogOffre2::eodoPeriode],GetParent());
	bCreation &= m_ppOnglets[CDialogOffre2::eodoTitres]->Create(m_DialogID[CDialogOffre2::eodoTitres],GetParent());
	bCreation &= m_ppOnglets[CDialogOffre2::eodoCibles]->Create(m_DialogID[CDialogOffre2::eodoCibles],GetParent());
	bCreation &= m_ppOnglets[CDialogOffre2::eodoFormats]->Create(m_DialogID[CDialogOffre2::eodoFormats],GetParent());

	if (bCreation) SetCurSel(m_CurOngletIdx);

	return bCreation;
}

void COffreTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	ActivateTabDialogs();
	*pResult = 0;
}

void COffreTabCtrl::ActivateTabDialogs()
{
	m_CurOngletIdx = static_cast<CDialogOffre2::eOngletsDlgOffre>(GetCurSel());
	if(m_ppOnglets[m_CurOngletIdx]->m_hWnd)
		m_ppOnglets[m_CurOngletIdx]->ShowWindow(SW_HIDE);

	CRect clientRect;
	CRect wndRect;

	GetClientRect(clientRect);
	AdjustRect(FALSE, clientRect);
	GetWindowRect(wndRect);
	GetParent()->ScreenToClient(wndRect);
	clientRect.OffsetRect(wndRect.left,wndRect.top);

	for(int nCount=0; nCount < CDialogOffre2::eodoNumOnglets; nCount++)
	{
		m_ppOnglets[nCount]->SetWindowPos(&wndTop, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), SWP_HIDEWINDOW);
	}

	m_ppOnglets[m_CurOngletIdx]->SetWindowPos(&wndTop, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), SWP_SHOWWINDOW);
	m_ppOnglets[m_CurOngletIdx]->ShowWindow(SW_SHOW);
}


// récupère l'onglet courant
COngletOffre2 *  COffreTabCtrl::GetActiveTab()
{
	return m_ppOnglets[m_CurOngletIdx];
}

// définit l'onglet de démarrage
void COffreTabCtrl::SetStartTab(CDialogOffre2::eOngletsDlgOffre eOngletIdx)
{
	m_CurOngletIdx = eOngletIdx;
}

// récupère un onglet
COngletOffre2 * COffreTabCtrl::GetTab (CDialogOffre2::eOngletsDlgOffre ongletIdx)
{
	// on check la validité de l'idx de l'onglet et du ptr vers l'onglet
	ASSERT(ongletIdx < CDialogOffre2::eodoNumOnglets && (static_cast<int>(ongletIdx) >=0));
	ASSERT(m_ppOnglets[ongletIdx]);

	// avant de renvoyer l'onglet
	return m_ppOnglets[ongletIdx];
}
