// MyTabCtrl.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Offre2OngletCtrl.h"

#include "Offre2MainDialog.h"

#include "Offre2Cibles.h"
#include "Offre2Supports.h"
#include "Offre2Formats.h"
#include "Offre2Periode.h"

// COffre2OngletCtrl

IMPLEMENT_DYNAMIC(COffre2OngletCtrl, CTabCtrl)

// Gestionnaires de messages de COffre2OngletCtrl

BEGIN_MESSAGE_MAP(COffre2OngletCtrl, CTabCtrl)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


COffre2OngletCtrl::COffre2OngletCtrl(COffre2MainDialog * pDialogOffre2)
{
	ASSERT(pDialogOffre2);

	// on initialise les Id des dialogues des différentes pages	
	m_DialogID[COffre2MainDialog::eodoPeriode]			= COffre2Periode::IDD;
	m_DialogID[COffre2MainDialog::eodoTerrainsSupports]	= COffre2Supports::IDD;
	m_DialogID[COffre2MainDialog::eodoCibles]			= COffre2Cibles::IDD;
	m_DialogID[COffre2MainDialog::eodoFormats]			= COffre2Formats::IDD;

	// on initialise les dialogues des différentes pages	
	m_ppOnglets[COffre2MainDialog::eodoPeriode]			= new COffre2Periode(pDialogOffre2);
	m_ppOnglets[COffre2MainDialog::eodoTerrainsSupports]= new COffre2Supports(pDialogOffre2);
	m_ppOnglets[COffre2MainDialog::eodoCibles]			= new COffre2Cibles(pDialogOffre2);
	m_ppOnglets[COffre2MainDialog::eodoFormats]			= new COffre2Formats(pDialogOffre2);
	m_pCurPage											= 0;

	m_CurTabIdent										= COffre2MainDialog::eodoPeriode;
	m_appColor											= pDialogOffre2->GetAppColor();

	// on invalide l'itérateur
	m_TabIter = m_IndexMap.end();
}

COffre2OngletCtrl::~COffre2OngletCtrl()
{
}

BOOL COffre2OngletCtrl::InitDialogs()
{
	BOOL bCreation = TRUE;

	// attention l'ordre de création est important
	bCreation &= m_ppOnglets[COffre2MainDialog::eodoFormats]->Create(m_DialogID[COffre2MainDialog::eodoFormats],GetParent());
	bCreation &= m_ppOnglets[COffre2MainDialog::eodoCibles]->Create(m_DialogID[COffre2MainDialog::eodoCibles],	GetParent());
	bCreation &= m_ppOnglets[COffre2MainDialog::eodoPeriode]->Create(m_DialogID[COffre2MainDialog::eodoPeriode],GetParent());
	bCreation &= m_ppOnglets[COffre2MainDialog::eodoTerrainsSupports]->Create(m_DialogID[COffre2MainDialog::eodoTerrainsSupports],GetParent());

	// on vérifie que l'onglet de depart est correct 
	IndexMap::iterator tab_iter = m_IndexMap.find(m_CurTabIdent);
	ASSERT(tab_iter != m_IndexMap.end());

	if (bCreation)
		SetCurSel(tab_iter->second);

	return bCreation;
}

void COffre2OngletCtrl::ActivateTabDialogs()
{
	// on récupère l'idx de la sélection, on cherche l'identifiant correspondant dans la map
	const int tabidx = GetCurSel();

	m_CurTabIdent = COffre2MainDialog::eodoNumOnglets;
	IndexMap::const_iterator tab_it = m_IndexMap.begin();
	while(tab_it != m_IndexMap.end())
	{
		if (tab_it->second == tabidx)
			m_CurTabIdent = tab_it->first;
		++tab_it;
	}

	// on vérifie la validité de l'identifiant
	ASSERT(m_CurTabIdent != COffre2MainDialog::eodoNumOnglets);

	if(m_ppOnglets[m_CurTabIdent]->m_hWnd)
		m_ppOnglets[m_CurTabIdent]->ShowWindow(SW_HIDE);

	CRect clientRect;
	CRect wndRect;

	GetClientRect(clientRect);
	AdjustRect(FALSE, clientRect);
	GetWindowRect(wndRect);
	GetParent()->ScreenToClient(wndRect);
	clientRect.OffsetRect(wndRect.left,wndRect.top);

	// on cache tous les onglets existants réellement
	tab_it = m_IndexMap.begin();
	while(tab_it != m_IndexMap.end())
	{
		m_ppOnglets[tab_it->first]->SetWindowPos(&wndTop,
			clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(),
			(m_CurTabIdent == tab_it->first)?SWP_SHOWWINDOW:SWP_HIDEWINDOW);
		++tab_it;
	}

	m_ppOnglets[m_CurTabIdent]->ShowWindow(SW_SHOW);
}

// récupère l'onglet courant
COffre2OngletBase * COffre2OngletCtrl::GetActiveTab()
{
	return m_ppOnglets[m_CurTabIdent];
}

// définit l'onglet de démarrage
void COffre2OngletCtrl::SetStartTab(COffre2MainDialog::eOngletsDlgOffre ident)
{
	m_CurTabIdent = ident;
}

// récupère un onglet
COffre2OngletBase * COffre2OngletCtrl::GetTab (COffre2MainDialog::eOngletsDlgOffre ident)
{
	// on check la validité de l'idx de l'onglet et du ptr vers l'onglet
	ASSERT(ident < COffre2MainDialog::eodoNumOnglets && (static_cast<int>(ident) >=0));

	// on récupère l'index réel de l'onglet dans l'array
	IndexMap::const_iterator tab_it = m_IndexMap.find(ident);
	ASSERT(tab_it != m_IndexMap.end());

	// on vérifie le dlg correspondant
	const int realidx = tab_it->second;
	ASSERT(m_ppOnglets[ident]);

	// avant de renvoyer l'onglet
	return m_ppOnglets[ident];
}

// récupère un onglet (en fonction de son index)
COffre2OngletBase * COffre2OngletCtrl::GetTabFromIndex (const int index)
{
	// on cherche l'identifiant correspondant dans la map
	IndexMap::const_iterator tab_it = m_IndexMap.begin();
	while(tab_it != m_IndexMap.end())
	{
		if (tab_it->second == index)
			return m_ppOnglets[tab_it->first];
		++tab_it;
	}
	return 0;
}

void COffre2OngletCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS) // Drawing one Tab
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

void COffre2OngletCtrl::AddTab(COffre2MainDialog::eOngletsDlgOffre ident, LPCTSTR caption)
{
	// on check la valeur de l'index à ajouter
	ASSERT(ident >= 0 && ident < COffre2MainDialog::eodoNumOnglets);
	const int count = GetItemCount();

	// on ajoute l'onglet en fin, on récupère l'index réel, on le vérifie
	LONG realIdx = InsertItem(count, caption);
	ASSERT(realIdx == m_IndexMap.size() && realIdx < COffre2MainDialog::eodoNumOnglets);

	// on lie l'index de l'onglet dans le ctrl à sa valeur identifiante
	m_IndexMap[ident] = realIdx;
}


// itération sur les onglets présents

COffre2OngletBase * COffre2OngletCtrl::GetTab()
{
	return m_ppOnglets[m_TabIter->first];
}

void COffre2OngletCtrl::GoFirstTab()
{
	// se place sur le premier onglet
	m_TabIter = m_IndexMap.begin();
}

void COffre2OngletCtrl::GoNextTab()
{
	// se place sur le prochain onglet
	++m_TabIter;
}

bool COffre2OngletCtrl::IsTabValid() const
{
	return (m_TabIter != m_IndexMap.end());
}

bool COffre2OngletCtrl::IsTabValid(COffre2MainDialog::eOngletsDlgOffre & ongIdent) const
{
	if (m_TabIter == m_IndexMap.end())
		return false;
	else
		ongIdent = m_TabIter->first;
		return true;
}
