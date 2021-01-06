// Offre2SupportsOngletCtrl.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Offre2SupportsOngletCtrl.h"
#include "Offre2MainDialog.h"

// headers onglets de sélection dans l'onglet de choix de Supports
#include "Offre2SupportsSource.h"
#include "Offre2SupportsSegments.h"
#include "Offre2SupportsTypesPoids.h"

using namespace std;

// COffre2SupportsOngletCtrl

IMPLEMENT_DYNAMIC(COffre2SupportsOngletCtrl, CTabCtrl)

BEGIN_MESSAGE_MAP(COffre2SupportsOngletCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// Gestionnaires de messages de COffre2SupportsOngletCtrl

// MyTabCtrl.cpp : fichier d'implémentation
//

COffre2SupportsOngletCtrl::COffre2SupportsOngletCtrl(COffre2Supports  * pOngletTitres, COffre2MainDialog * pDialogOffre2)
	:	m_pOngletTitres(pOngletTitres)
{
	ASSERT(m_pOngletTitres);

	// on initialise les Id des dialogues des différentes pages
	m_DialogID[SelectionSource]		= Offre2SupportsSource::IDD;
	m_DialogID[SelectionSegments]	= Offre2SupportsSegments::IDD;
	m_DialogID[SelectionTypesPoids]	= Offre2SupportsTypesPoids::IDD;

	// on initialise les dialogues des différentes pages	
	m_ppOnglets[SelectionSource]	= new Offre2SupportsSource;
	m_ppOnglets[SelectionSegments]	= new Offre2SupportsSegments;
	m_ppOnglets[SelectionTypesPoids]= new Offre2SupportsTypesPoids;

	m_pCurPage						= 0;

	m_CurOngletIdx					= SelectionSource;
	m_appColor						= pDialogOffre2->GetAppColor();

	// on définit le ptr vers le dialogue Offre dns chq page à onglet
	for (int idx=0; idx < NumOnglets; ++idx)
		m_ppOnglets[idx]->SetData(m_pOngletTitres, pDialogOffre2);
}

COffre2SupportsOngletCtrl::~COffre2SupportsOngletCtrl()
{
}

BOOL COffre2SupportsOngletCtrl::InitDialogs()
{
	BOOL bCreation = TRUE;
	bCreation &= m_ppOnglets[SelectionSource]->Create(m_DialogID[SelectionSource],GetParent());
	bCreation &= m_ppOnglets[SelectionSegments]->Create(m_DialogID[SelectionSegments],GetParent());
	bCreation &= m_ppOnglets[SelectionTypesPoids]->Create(m_DialogID[SelectionTypesPoids],GetParent());

	if (bCreation) SetCurSel(m_CurOngletIdx);

	return bCreation;
}

void COffre2SupportsOngletCtrl::ActivateTabDialogs()
{
	// l'idx de l'onglet sélectionné
	int selidx = static_cast<eOngletType>(GetCurSel());

	m_CurOngletIdx = NumOnglets;

	// on cherche quel est le type de cet onglet (sachant que certaines peuvent avoir été supprimés, d'oû la map)
	map<eOngletType, int>::iterator ong_it = m_Onglets.begin();
	while (ong_it != m_Onglets.end())
	{
		if (ong_it->second == selidx)
			m_CurOngletIdx = ong_it->first;
		++ong_it;
	}

	ASSERT(m_CurOngletIdx != -1);

	if(m_ppOnglets[m_CurOngletIdx]->m_hWnd)
		m_ppOnglets[m_CurOngletIdx]->ShowWindow(SW_HIDE);

	CRect clientRect;
	CRect wndRect;

	GetClientRect(clientRect);
	AdjustRect(FALSE, clientRect);
	GetWindowRect(wndRect);
	GetParent()->ScreenToClient(wndRect);
	clientRect.OffsetRect(wndRect.left,wndRect.top);

	for(int nCount=0; nCount < NumOnglets; nCount++)
		m_ppOnglets[nCount]->SetWindowPos(&wndTop, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), (m_CurOngletIdx == nCount)?SWP_SHOWWINDOW:SWP_HIDEWINDOW);

	m_ppOnglets[m_CurOngletIdx]->ShowWindow(SW_SHOW);
}


// récupère l'onglet courant
COffre2SupportsOngletBase *  COffre2SupportsOngletCtrl::GetActiveTab()
{
	ASSERT(m_CurOngletIdx != -1);
	ASSERT(m_CurOngletIdx < static_cast<int>(m_Onglets.size()));
	return m_ppOnglets[m_CurOngletIdx];
}

// récupère un onglet
COffre2SupportsOngletBase * COffre2SupportsOngletCtrl::GetTab (eOngletType ongletIdx)
{
	// on check la validité de l'idx de l'onglet et du ptr vers l'onglet
	ASSERT(ongletIdx < NumOnglets && (static_cast<int>(ongletIdx) >=0));
	ASSERT(m_ppOnglets[ongletIdx]);

	// avant de renvoyer l'onglet
	return m_ppOnglets[ongletIdx];
}


BOOL COffre2SupportsOngletCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, CATPColors::GetColorMedium(m_appColor));
	pDC->SetBkColor(RGB(255, 0, 0));
	return TRUE;
}


void COffre2SupportsOngletCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS) // Drawing one Tab
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rc(lpDIS->rcItem);
	TCHAR szTabText[255];
	TC_ITEM tci;
	tci.cchTextMax = sizeof(szTabText)-1;
	tci.pszText = szTabText;
	tci.mask = TCIF_TEXT;
	GetItem(lpDIS->itemID, &tci);

	if (lpDIS->itemState & ODS_SELECTED)
	{
		pDC->SetBkColor(CATPColors::GetColorMedium(m_appColor));
		pDC->FillSolidRect(&lpDIS->rcItem, CATPColors::GetColorMedium(m_appColor));
		pDC->SetTextColor(CATPColors::GetColorWhite());
		pDC->DrawText(szTabText, &lpDIS->rcItem,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		pDC->SetBkColor(CATPColors::GetColorLight(m_appColor));
		pDC->FillSolidRect(&lpDIS->rcItem, CATPColors::GetColorLight(m_appColor));
		pDC->SetTextColor(RGB(0,0,0));
		pDC->DrawText(szTabText, &lpDIS->rcItem,
			DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
	}
}



