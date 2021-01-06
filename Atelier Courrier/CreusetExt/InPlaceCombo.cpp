// InPlaceCombo.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "InPlaceCombo.h"
#include ".\inplacecombo.h"


// CInPlaceCombo

IMPLEMENT_DYNAMIC(CInPlaceCombo, CClrSelComboBox)

CInPlaceCombo::CInPlaceCombo(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID)
{
    DWORD dwEditStyle = //WS_CHILD | WS_VISIBLE | CBS_OWNERDRAWFIXED  | dwStyle;
						CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_SORT | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE
						| dwStyle;

	if (!Create(dwEditStyle, rect, pParent, nID))
		return;

	SetFocus();
}

CInPlaceCombo::~CInPlaceCombo()
{
}


BEGIN_MESSAGE_MAP(CInPlaceCombo, CClrSelComboBox)
    ON_WM_KILLFOCUS()
    ON_WM_CREATE()
END_MESSAGE_MAP()

void CInPlaceCombo::OnKillFocus(CWnd* pNewWnd)
{
    CClrSelComboBox::OnKillFocus(pNewWnd);
    EndCombo();
}


void CInPlaceCombo::PostNcDestroy() 
{
    CClrSelComboBox::PostNcDestroy();
     delete this;	
}

// Gestionnaires de messages CInPlaceCombo

void CInPlaceCombo::EndCombo()
{
    CString str;

    // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
    // that validates input can cause this to get called multiple times
    // causing assertions because the edit control goes away the first time.
    static BOOL bAlreadyEnding = FALSE;

    if(bAlreadyEnding)
        return;

    bAlreadyEnding = TRUE;
    GetWindowText(str);

    // Send Notification to parent
 /*   GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;

    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.strText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar;

    CWnd* pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );*/

    // Close this window (PostNcDestroy will delete this)
    if (IsWindow(GetSafeHwnd()))
        SendMessage(WM_CLOSE, 0, 0);
    bAlreadyEnding = FALSE;
}

