// XListBox.cpp  Version 1.3
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
//
// Modifications: 
//
//		Aurélien Rainone (10.10.2008) :
//			*	added support for any color for text, selected text,
//				background and selected background
//			*	added support for bold, italic and underlined text
//			*	added formating of text by a sort of templates system
//				(goes with XTemplate.h)
//
//
// TODO : 
//			*	add support for changing an already added text string
//
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XLISTBOX_H
#define XLISTBOX_H

#include <atlstr.h>
#include <afxwin.h>

#include "XTemplate.h"

///////////////////////////////////////////////////////////////////////////////
//  CXListBox class

class AFX_EXT_CLASS CXListBox : public CListBox
{
	// Constructors
public:
	CXListBox();
	virtual ~CXListBox();

	// Attributes
public:
	int			m_cxExtent;
	int			m_nTabPosition;
	BOOL		m_bColor;

	// NOTE - following list must be kept in sync with ColorPickerCB.cpp

	enum Color { Black,  White, Maroon,  Green,
		Olive,  Navy,  Purple,  Teal,
		Silver, Gray,  Red,     Lime,
		Yellow, Blue,  Fuschia, Aqua };

	void EnableColor(BOOL bEnable);

	// Operations
public:
	int AddLine(LPCTSTR lpszLine, const HXTemplateHandle hTemplate = CXTemplateManager::GetSystemTemplate());
	int AddString(LPCTSTR lpszItem);
	void EnableLineNumbers(BOOL bEnable) { m_bLineNumbers = bEnable; }
	int FindString(int nStartAfter, LPCTSTR lpszItem) const;
	int FindStringExact(int nStartAfter, LPCTSTR lpszItem) const;
	int GetText(int nIndex, LPTSTR lpszBuffer) const;
	void GetText(int nIndex, CString& rString) const;
	int GetTextLen(int nIndex) const;
	int GetTextWithColor(int nIndex, LPTSTR lpszBuffer) const;
	void GetTextWithColor(int nIndex, CString& rString) const;
	int GetVisibleLines();
	int InsertString(int nIndex, LPCTSTR lpszItem, const HXTemplateHandle hTemplate = CXTemplateManager::GetSystemTemplate());
	int _cdecl Printf(const HXTemplateHandle hTemplate, UINT nID, LPCTSTR lpszFmt, ...);
	virtual void ResetContent();
	int SelectString(int nStartAfter, LPCTSTR lpszItem);
	void SetContextMenuId(UINT nId) { m_nContextMenuId = nId; }
	virtual void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	void SetGutterColor(COLORREF crGutter) { m_ColorGutter = crGutter; }
	void SetGutterWidth(int nWidth) { m_nGutterWidth = nWidth; }
	void SetLineNoColor(COLORREF crLineNo) { m_ColorLineNo = crLineNo; }
	void SetLogFile(LPCTSTR lpszLogFile) { m_strLogFile = lpszLogFile; }
	void SetTabPosition(int nSpacesPerTab);

	void SetBackgroundColor(int nIndex, COLORREF bc, bool bSelected = false);
	COLORREF GetBackgroundColor(int nIndex, bool bSelected = false) const;

	void SetTextColor(int nIndex, COLORREF tc, bool bSelected = false);
	COLORREF GetTextColor(int nIndex, bool bSelected = false) const;

	void ChangeItemTemplate(int nIndex, const HXTemplateHandle hnewTemplate);	
	void ChangeItemRangeTemplate(const HXTemplateHandle hnewTemplate, int nFirstItem = 0, int nLastItem = LB_ERR);	

	void SelectItemRange(bool bSel, int idxDeb, int idxFin);

	// Implementation
protected:
	int SearchString(int nStartAfter, LPCTSTR lpszItem, BOOL bExact) const;

#define MAXTABSTOPS 100
	int m_nTabStopPositions[MAXTABSTOPS];
	int m_nSpaceWidth;
	int m_nAveCharWidth;
	UINT m_nContextMenuId;
	COLORREF m_ColorGutter;
	COLORREF m_ColorLineNo;
	int m_nGutterWidth;
	CString m_strLogFile;
	BOOL m_bLineNumbers;

	virtual void PreSubclassWindow();

	virtual int  CompareItem (LPCOMPAREITEMSTRUCT lpCIS);
	virtual void DrawItem (LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem (LPMEASUREITEMSTRUCT lpMIS);

	// Generated message map functions
	//{{AFX_MSG(CXListBox)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	// Create option string from a template handle
	void CreateOptionString(CString & s, const HXTemplateHandle hTemplate);
	void CreateOptionString(CString & s, COLORREF textcolor, COLORREF backcolor, COLORREF seltextcolor, COLORREF selbackcolor, bool bBold = false, bool bItalic = false, bool bUnderline = false);

	CFont * m_pOldFont;
	CFont * m_pCurFont;
};
#include <iterator>

//=============================================================================	
// pour récuperer la liste des élements sélectionnés dans une listbox multiselection
// return false si rien sélectionné
template <class C>
bool GetControlSelection(C & contSel, const CListBox & listbox)
//=============================================================================	
{
	contSel.clear();

	std::insert_iterator<C> pos(contSel, contSel.end());

	// Boucle sur les terrains actuellement sélectionnés
	const int nMax = listbox.GetSelCount();

	if (nMax > 0)
	{
		int * arraySel = new int[nMax];
		int arrIdx = 0;
		listbox.GetSelItems(nMax, arraySel); 
		while (arrIdx < nMax)
		{
			// on ajoute l'idx sélection au vecteur
			*pos = arraySel[arrIdx];

			++arrIdx; ++pos;
		}
		// on supprime l'array
		if (arraySel) delete[] arraySel; arraySel = 0;
	}
	return (!contSel.empty());
}


//=============================================================================	
// pour récuperer la liste des élements sélectionnés dans une listbox multiselection
// return false si rien sélectionné
template <class C>
bool GetControlSelectedData(C & contSel, const CListBox & listbox)
//=============================================================================	
{
	contSel.clear();

	std::insert_iterator<C> pos(contSel, contSel.end());

	// Boucle sur les terrains actuellement sélectionnés
	const int nMax = listbox.GetSelCount();

	if (nMax > 0)
	{
		int * arraySel = new int[nMax];
		int arrIdx = 0;
		listbox.GetSelItems(nMax, arraySel); 
		while (arrIdx < nMax)
		{
			// on ajoute l'idx sélection au vecteur
			//C::value_type val = reinterpret_cast<C::value_type>(listbox.GetItemData(arraySel[arrIdx]));
			C::value_type val = (C::value_type)listbox.GetItemData(arraySel[arrIdx]);
			*pos = val;

			++arrIdx; ++pos;
		}
		// on supprime l'array
		if (arraySel) delete[] arraySel; arraySel = 0;
	}
	return (!contSel.empty());
}

#endif
///////////////////////////////////////////////////////////////////////////////
