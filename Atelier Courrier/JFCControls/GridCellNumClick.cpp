// CGridCellNumClick.cpp: implementation of the CGridCellNumClick class.
//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellNumClick.h"
#include "inplaceedit.h"
#include "GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridCellNumClick, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// Create a control to do the editing
BOOL CGridCellNumClick::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    m_bEditing = FALSE;
    
    // CInPlaceEdit auto-deletes itself
 //   m_pEditWnd = new CInPlaceEdit(GetGrid(), rect, /*GetStyle() |*/ ES_NUMBER, nID, nRow, nCol,
//		GetText(), nChar);

    return TRUE;
}

// Cancel the editing.
void CGridCellNumClick::EndEdit()
{
 //   if (m_pEditWnd)
  //      ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
}

void CGridCellNumClick::OnClick( CPoint PointCellRelative)
{
	CGridCell::OnClick(PointCellRelative);
}

void CGridCellNumClick::OnClickDown( CPoint PointCellRelative)
{
	int nb;
	CString nbstr;
	nb= atoi(GetText());
	nb++;
	nbstr.Format("%d", nb);
	SetText(nbstr);
	CGridCell::OnClickDown(PointCellRelative);
}
void CGridCellNumClick::OnDblClick( CPoint PointCellRelative)
{
	OnClick(PointCellRelative);
	CGridCell::OnDblClick(PointCellRelative);
}


void CGridCellNumClick::OnRClick( CPoint PointCellRelative)
{
	int nb;
	CString nbstr;
	nb= atoi(GetText());
	if (nb>1)
		nb--;
	nbstr.Format("%d", nb);
	SetText(nbstr);
	CGridCell::OnRClick(PointCellRelative);
}

