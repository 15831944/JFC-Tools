// CSupportsListCtrl.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Resource.h"
#include "SupportsListCtrl.h"
#include <JSRCPresse.h>
#include <IDLimits.h>
#include <AllTypeOfNode.h>
#include "FormatStr.h"


CSupportsListCtrl::CSupportsListCtrl()
{
}

CSupportsListCtrl::~CSupportsListCtrl()
{
}

IMPLEMENT_DYNAMIC(CSupportsListCtrl, JFCListCtrl)

bool CSupportsListCtrl::IsAttribue(int ID) const
{
	bool bRet = false;

	if ( ID > IDTITREAPPARENTE_MIN 
		&& ID < IDTITREAPPARENTE_MAX
		&& CRegroupements::GetInstance()->GetTitreApparenteByID( ID )->GetParentID() != 0 )
			bRet = true;

	if( CRegroupements::GetInstance()->GetTitreByID(ID) )
		bRet = true;

	return bRet;
}

static int CALLBACK 
MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	//CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
	//CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);
	//CRegroupements::GetInstance()->GetIElemByID(ID);
	CString    strItem1 = CRegroupements::GetInstance()->GetIElemByID(static_cast<JUnt32>(lParam1))->GetLabel().AsJCharPtr();
	CString    strItem2 = CRegroupements::GetInstance()->GetIElemByID(static_cast<JUnt32>(lParam2))->GetLabel().AsJCharPtr();
	return CCompareTitre::CompareTitre(strItem1, strItem2);
}



void CSupportsListCtrl::Sort()
{
	SortItems(MyCompareProc, (LPARAM) this);
}


