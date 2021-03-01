#include "StdAfx.h"
#include "RedListBox.h"
#include "DlgSupport.h"
#include "FormatStr.h"

#include <AllTypeOfNode.h>
#include <AllTypeOfIterator.h>
#include <IDLimits.h>

CRedListBox::CRedListBox(void)
{
	m_pchTip = NULL;
	m_pwchTip = NULL;
}

CRedListBox::~CRedListBox(void)
{
}

BEGIN_MESSAGE_MAP(CRedListBox, JFC_MultiListBox<IElem const *>)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

bool CRedListBox::IsAttribue(int ID) const
{
	bool bRet = false;
	switch(m_pDlgParent->m_internalMode)
	{
	case CDlgSupport::LOT :
		bRet = IsLotAttribue(ID);
		break;
	case CDlgSupport::TITRE :
		bRet = IsTitreAttribue(ID);
		break;
	case CDlgSupport::COUPLAGE :
		bRet = IsCouplageAttribue(ID);
		break;
	default:
		break;
	}

	return bRet;
}

bool CRedListBox::IsLotAttribue(int ID) const
{
	bool bRet = false;
	CLot * lot = m_pDlgParent->m_pRegroupements->GetLotByID(ID);
	if ( lot == NULL || !lot->IsLot() )
		return bRet;

	IIterateur* it = lot->CreateIterateur();
	CTitreLot* tlot = (CTitreLot*)it->getHead();
	while (tlot)
	{
		if ( IsTitreAttribue(tlot->GetID()) )
			bRet = true;
		tlot = (CTitreLot*)it->getNext();
	}
	delete it;
	return bRet;
}

bool CRedListBox::IsCouplageAttribue(int ID) const
{
	bool bRet = false;
	//pour les couplages fixes
	//if ( ID > IDCOUPLEFIXE_MIN && ID < IDCOUPLEFIXE_MAX )
	//{
	//	JCouplageFixe * cplfixe = (JCouplageFixe*)m_pDlgParent->m_srcPresse->m_TBLCouplagesFixes.GetElemByID(ID);
	//	JList<JUnt32> lstID;
	//	cplfixe->GetFils(lstID);
	//	for ( lstID.MoveFirst(); lstID.IsValid(); lstID.MoveNext() )
	//	{
	//		if ( IsTitreAttribue(lstID.GetItem()) )
	//			bRet = true;
	//	}
	//}

	//pour les couplages libres
	if ( ID > IDCOUPLELIBRE_MIN && ID < IDCOUPLELIBRE_MAX )
	{
		CCouplageLibre * cpllibre = m_pDlgParent->m_pRegroupements->GetCouplageLibreByID(ID);
		IIterateur* it = cpllibre->CreateIterateur();
		CTitreCouplageLibre* tcpllibre = (CTitreCouplageLibre*)it->getHead();
		while (tcpllibre)
		{
			if ( IsTitreAttribue(tcpllibre->GetID()) )
				bRet = true;
			tcpllibre = (CTitreCouplageLibre*)it->getNext();
		}
		delete it;
	}

	return bRet;
}

bool CRedListBox::IsCouplageAttribue(IElem * elem) const
{
	bool bRet = false;
	//pour les couplages fixes
	if ( elem->IsCouplageFixe() )
	{
		JCouplageFixe * cplfixe = elem->AsCouplageFixe();
		JList<JUnt32> lstID;
		cplfixe->GetFils(lstID);
		for ( lstID.MoveFirst(); lstID.IsValid(); lstID.MoveNext() )
		{
			if ( IsTitreAttribue(lstID.GetItem()) )
				bRet = true;
		}

	}

	//pour les couplages libres
	if ( elem->IsCouplageLibre() )
	{
		CCouplageLibre * cpllibre = elem->AsCouplageLibre();
		IIterateur* it = cpllibre->CreateIterateur();
		CTitreCouplageLibre* tcpllibre = (CTitreCouplageLibre*)it->getHead();
		while (tcpllibre)
		{
			if ( IsTitreAttribue(tcpllibre->GetID()) )
				bRet = true;
			tcpllibre = (CTitreCouplageLibre*)it->getNext();
		}
		delete it;
	}
	
	return bRet;
}

bool CRedListBox::IsTitreAttribue(int ID) const
{
	bool bRet = true;
	CTerrain* pTerrain;
	
	IElem * elem = m_pDlgParent->m_pRegroupements->GetIElemByID(ID);
	
	int IDAudience = 0;
	if(elem == NULL)
		return false;

	if(elem->IsCouplage())
		return IsCouplageAttribue(elem);

	IDAudience = elem->AsITitre()->GetIDAudience();


		
	CKeyTerrain k;
	k.m_IDSource = m_pDlgParent->m_pData->m_offre.m_IdSourceAudience;
	
	for (m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst();
		m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid();
		m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext() )
	{
		k.m_IDTerrain = m_pDlgParent->m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();

		pTerrain = m_pDlgParent->m_pData->m_OffreTerrainManager->GetTerrain(k);
		
		//si le terrain a le support 
		for(m_pDlgParent->m_pData->m_offre.m_TBLIdSegmentsAudience.MoveFirst();
			m_pDlgParent->m_pData->m_offre.m_TBLIdSegmentsAudience.IsValid();
			m_pDlgParent->m_pData->m_offre.m_TBLIdSegmentsAudience.MoveNext())
		{
			JInt32 idSegment = m_pDlgParent->m_pData->m_offre.m_TBLIdSegmentsAudience.GetItem();
			if (pTerrain->SupportBelongsToSegment( IDAudience, idSegment ))
				bRet = false;
		}
	}

	return bRet;
}

int CRedListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	int row;
	RECT cellrect;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	row  = ItemFromPoint(point,tmp);  //we call the ItemFromPoint function to determine the row,
	if ( row == -1 ) 
		return -1;

	//set up the TOOLINFO structure. GetItemRect(row,&cellrect);
	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)((row));   //The ‘uId’ is assigned a value according to the row value.
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	return (int)pTI->uId;

}


//Define OnToolTipText(). This is the handler for the TTN_NEEDTEXT notification from 
//support ansi and unicode 
BOOL CRedListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = (UINT)pNMHDR->idFrom;

	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
	IElem* elem = (IElem*)GetItemData(nID);
	if (elem)
	{
		if (elem->IsCouplage())
		{
			JList<JUnt32> lstID;
			elem->AsCouplage()->GetFils(lstID);
			for(lstID.MoveFirst(); lstID.IsValid(); lstID.MoveNext())
				strTipText.AppendFormat("%s\r\n", m_pDlgParent->m_srcPresse->m_TBLTitres.GetLabelByID(lstID.GetItem()));
		}
		else if (!elem->IsTitre())
		{
			// A FAIRE - ALAIN Déploiement tooltip
			IIterateur* it = ((INode*)elem)->CreateIterateur();
			if (it)
			{
				INode * nd = it->getHead();
				while(nd)
				{
					strTipText.AppendFormat("%s\r\n", nd->GetLabel().AsJCharPtr());
					nd = it->getNext();
				}
				delete it;
			}
		}
	}
	strTipText.TrimRight();
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

int CRedListBox::CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	return CCompareTitre::CompareTitre(
		((IElem *)lpCompareItemStruct->itemData1)->GetLabel().AsJCharPtr(),
		((IElem *)lpCompareItemStruct->itemData2)->GetLabel().AsJCharPtr());
}
