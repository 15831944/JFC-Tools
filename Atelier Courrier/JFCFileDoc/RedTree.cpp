#include "StdAfx.h"
#include "RedTree.h"
#include "DlgSupport.h"

#include <AllTypeOfNode.h>
#include <AllTypeOfIterator.h>
#include <IDLimits.h>

CRedTree::CRedTree(void)
{
	m_pchTip = NULL;
	m_pwchTip = NULL;
}

CRedTree::~CRedTree(void)
{
}

BEGIN_MESSAGE_MAP(CRedTree, CSupportTree)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

bool CRedTree::IsAttribue(int ID) const
{
	bool bRet = false;

	CLot * lot = m_pDlgParent->m_pRegroupements->GetLotByID(ID);
	if ( lot == NULL || !lot->IsLot() )
		return bRet;

	IIterateur* it = lot->CreateIterateur();
	CTitre* titre = (CTitre*)it->getHead();
	while (titre)
	{
		if ( IsTitreAttribue(titre->GetIDAudience()) )
			bRet = true;
		titre = (CTitre*)it->getNext();
	}
	delete it;

	return bRet;
}

bool CRedTree::IsTitreAttribue(int ID) const
{
	bool bRet = false;
	CTerrain* pTerrain;
	
	CKeyTerrain k;
	k.m_IDSource = m_pDlgParent->m_pData->m_offre.m_IdSourceAudience;
	
	for (m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst();
		m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid();
		m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext() )
	{
		k.m_IDTerrain = m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
		pTerrain = m_pDlgParent->m_pData->m_OffreTerrainManager->GetTerrain(k);
		bRet = ! pTerrain->HasSupport( ID );
	}
	return bRet;
}

//Define OnToolTipText(). This is the handler for the TTN_NEEDTEXT notification from 
//support ansi and unicode 
BOOL CRedTree::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXT* pTTTA = (TOOLTIPTEXT*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = (UINT) pNMHDR->idFrom;
	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 600);

	// Do not process the message from built in tooltip 
	//if( nID == (UINT)m_hWnd &&
	//	(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
	//	( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
	//	return FALSE;

	HTREEITEM hitem = (HTREEITEM) pNMHDR->idFrom;
	INode * node = (INode*) GetItemData(hitem); //get item data
	
	if(node)
	{
		// Attention si énormément de ligne (on passera en même énuméré avec plusieurs titres par ligne)
		// Compte le nombre d'élément
        IIterateur* it = node->CreateIterateur();
		int NbElmt = node->GetNbFils(); 
		
		if (it)
		{
			INode * nd = it->getHead();
			int CptItem = 1;
			while(nd)
			{
				if (NbElmt > 40 && CptItem % 4 != 0)
				{
					// 4 élmts sur une même ligne
					strTipText.AppendFormat("%s", nd->GetLabel().AsJCharPtr());
					if (CptItem < NbElmt) strTipText.AppendFormat("%s", " ; ");
				}
				else
					// Passe à la ligne
					strTipText.AppendFormat("%s\r\n", nd->GetLabel().AsJCharPtr());

				nd = it->getNext();

				// 1 élmt de +
				CptItem++;
			}
			strTipText.TrimRight();
			delete it;
		}
	}
	strTipText.AppendChar('\0');

#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif

	*pResult = 0;

	return TRUE;    
}

// Determine whether a point is in the bounding rectangle of the specified tool
int CRedTree::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	RECT rect;
	UINT nFlags;
	HTREEITEM hitem = HitTest( point, &nFlags );
	if( nFlags & TVHT_ONITEMLABEL  )
	{
		GetItemRect( hitem, &rect, TRUE );
		pTI->hwnd = m_hWnd;
		pTI->uId = reinterpret_cast<UINT_PTR>(hitem);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return (int)pTI->uId;
	}
	return -1;
}