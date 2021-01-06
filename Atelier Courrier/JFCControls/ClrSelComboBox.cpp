// ClrSelComboBox.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ClrSelComboBox.h"
#include "ATPColors.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClrSelComboBox

IMPLEMENT_DYNAMIC(CClrSelComboBox, CComboBox)
CClrSelComboBox::CClrSelComboBox()
{
	m_selColor = ::GetSysColor(COLOR_HIGHLIGHT);
}

CClrSelComboBox::~CClrSelComboBox()
{
}


BEGIN_MESSAGE_MAP(CClrSelComboBox, CComboBox)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()


// Gestionnaires de messages CClrSelComboBox

void CClrSelComboBox::SetSelectionColor(COLORREF selclr)
{
	m_selColor = selclr;
}

void CClrSelComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->itemID == CB_ERR)
		return;

	CString		sItem;
	CBrush		pBrush;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CFont FontS;
	FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), pDC);
	CFont* pOldFont = pDC->GetCurrentFont();
	pDC->SelectObject(&FontS);

	// If we have the focus, draw the item with a sel color
	// if we haven´t, just plain blank
	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		pBrush.CreateSolidBrush( m_selColor );
		pDC->FillRect(&lpDrawItemStruct->rcItem, &pBrush);
		pDC->SetTextColor( ::GetSysColor(COLOR_HIGHLIGHTTEXT) );
	}
	else
	{
		pBrush.CreateSolidBrush( ::GetSysColor(COLOR_WINDOW) );
		pDC->FillRect(&lpDrawItemStruct->rcItem, &pBrush);
		pDC->SetTextColor( ::GetSysColor(COLOR_WINDOWTEXT) );
	}
	
	// Copy the text of the item to a string
	if (GetStyle() & CBS_HASSTRINGS  )
		GetLBText(lpDrawItemStruct->itemID, sItem);
	else
		sItem = (LPCTSTR) lpDrawItemStruct->itemData;

	//pour un plus bel alignement
	sItem.Insert(0, " ");

	pDC->SetBkMode(TRANSPARENT);
	// Draw the text
	pDC->DrawText(sItem, &lpDrawItemStruct->rcItem, DT_VCENTER | DT_SINGLELINE );

	// Cleanup up all the stuff !
	pBrush.DeleteObject();

	pDC->SelectObject(pOldFont);
}

void CClrSelComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->itemID != CB_ERR)
	{
		CString lpszText;
		//if (GetStyle() & CBS_HASSTRINGS )
		//	GetLBText(lpMeasureItemStruct->itemID, lpszText);
		//else
		//	lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
		lpszText="Toto";
		CSize   sz;
		CDC*    pDC = GetDC();

		sz = pDC->GetTextExtent(lpszText);

		ReleaseDC(pDC);

		lpMeasureItemStruct->itemHeight = 1*sz.cy;
	}

}

void CClrSelComboBox::PreSubclassWindow() 
{
	//Let the parent do its thing
	CComboBox::PreSubclassWindow();

	//combo box must manage the strings
//	ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & CBS_HASSTRINGS);

	//combo box must be owner draw variable
//	ASSERT(GetWindowLong(m_hWnd, GWL_STYLE) & CBS_OWNERDRAWVARIABLE);

	//Set the Height of the combo box to just contain line
	::SendMessage(m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, 15L);
}