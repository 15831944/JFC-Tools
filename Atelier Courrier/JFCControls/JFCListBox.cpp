#include "StdAfx.h"
#include ".\jfclistbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(JFCListBox, CListBox)

JFCListBox::JFCListBox(void)
{
	m_ColorBk	= ::GetSysColor(COLOR_WINDOW);
	m_ColorSel	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_ColorText = ::GetSysColor(COLOR_WINDOWTEXT); 
	m_ColorSelText = ::GetSysColor(COLOR_HIGHLIGHTTEXT); 
}

JFCListBox::~JFCListBox(void)
{
}

BEGIN_MESSAGE_MAP(JFCListBox, CListBox)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

void JFCListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
		// If there are no list box items, skip this message. 

		if (lpDrawItemStruct->itemID == -1) 
			return; 

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		
		CString sLabel;
		GetText( lpDrawItemStruct->itemID, sLabel);

		// Save these values to restore them when done drawing.
		COLORREF crOldTextColor = pDC->GetTextColor();
		COLORREF crOldBkColor = pDC->GetBkColor();

		// item selected
		if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			pDC->SetTextColor(m_ColorSelText);
			pDC->SetBkColor(m_ColorSel);
		}
		else// item deselected
		{
			pDC->SetTextColor(m_ColorText);
			pDC->SetBkColor(m_ColorBk);
		}
	


		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, pDC->GetBkColor());

		RECT Rect = lpDrawItemStruct->rcItem;
		Rect.left+=5; 
		Rect.right-=5;
		pDC->DrawText( sLabel, -1, &Rect, DT_WORDBREAK | DT_NOPREFIX );

		if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			// Draw the focus rect 
			pDC->DrawFocusRect(&(lpDrawItemStruct->rcItem)); 
		}

		// Reset the background color and the text color back to their
		// original values.
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkColor(crOldBkColor);
}

void JFCListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
		CPaintDC dc(this);
		CString sLabel;
		GetText( lpMeasureItemStruct->itemID, sLabel);
		CRect rcLabel;

		GetItemRect(lpMeasureItemStruct->itemID, rcLabel);
		// Make left margin of 5 pixels
		rcLabel.left+=5;
		// Make right margin 5 pixels
		rcLabel.right-=5;
		// Calculate the required rectangle for the text and set the item height for this 
		// specific item based on the return value (new height).
		dc.SelectObject(GetFont()); 
		lpMeasureItemStruct->itemHeight = dc.DrawText( sLabel, -1, rcLabel, 
												DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );
}

BOOL JFCListBox::RecreateListBox(LPVOID lpParam/*=NULL*/)
{
	return RecreateListBox(this, lpParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Recrée une liste déjà existante en vue de changer ses propriétés (car ModifyStyle ne marche pas)
BOOL JFCListBox::RecreateListBox(CListBox* pList, LPVOID lpParam/*=NULL*/)
{
	if (pList == NULL)
		return FALSE;
	if (pList->GetSafeHwnd() == NULL)
		return FALSE;

	CWnd* pParent = pList->GetParent();
	if (pParent == NULL)
		return FALSE;

	// get current attributes
	DWORD dwStyle = pList->GetStyle();
	DWORD dwStyleEx = pList->GetExStyle();
	CRect rc;
	pList->GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);	// map to client co-ords
	UINT nID = pList->GetDlgCtrlID();
	CFont* pFont = pList->GetFont();
	CWnd* pWndAfter = pList->GetNextWindow(GW_HWNDPREV);

	// create the new list box and copy the old list box items 
	// into a new listbox along with each item's data, and selection state
	JFCListBox listNew;
	if (! listNew.CreateEx(dwStyleEx, _T("LISTBOX"), _T(""), dwStyle, 
                                rc, pParent, nID, lpParam))
	  return FALSE;
	listNew.SetFont(pFont);
	int nNumItems = pList->GetCount();
	BOOL bMultiSel = (dwStyle & LBS_MULTIPLESEL || dwStyle & LBS_EXTENDEDSEL);
	for (int n = 0; n < nNumItems; n++)
	{
		CString sText;
		pList->GetText(n, sText);
		int nNewIndex = listNew.AddString(sText);
		listNew.SetItemData(nNewIndex, pList->GetItemData(n));
		if (bMultiSel && pList->GetSel(n))
			listNew.SetSel(nNewIndex);
	}
	if (! bMultiSel&& nNumItems)
	{
		int nCurSel = pList->GetCurSel();
		if (nCurSel != -1)
		{
			CString sSelText;
			// get the selection in the old list
			pList->GetText(nCurSel, sSelText);
			// now find and select it in the new list
			listNew.SetCurSel(listNew.FindStringExact(-1, sSelText));
		}
	}
	// destroy the existing window, then attach the new one
	pList->DestroyWindow();
	HWND hwnd = listNew.Detach();
	pList->Attach(hwnd);

	// position correctly in z-order
	pList->SetWindowPos(pWndAfter == NULL ? &CWnd::wndBottom
                                 : pWndAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

