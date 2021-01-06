/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCListBox
//
//	Descriptif:
//	ListBox avec couleur de sélection personnalisable
//	et méthode de recréation pour appliquer un changement de mode de sélection
*/

#pragma once
#include "afxwin.h"

class AFX_EXT_CLASS JFCListBox :
	public CListBox
{
public:
	DECLARE_DYNAMIC(JFCListBox)
	JFCListBox(void);
	virtual ~JFCListBox(void);
	void SetTextColor(COLORREF Text) { m_ColorText = Text; Invalidate(true); }
	void SetBackColor(COLORREF Back) { m_ColorBk = Back; Invalidate(true); }
	void SetSelectColor(COLORREF Sel) { m_ColorSel = Sel; Invalidate(true); }
	void SetSelectedTextColor(COLORREF Sel) { m_ColorSelText = Sel; Invalidate(true); }

	BOOL RecreateListBox(LPVOID lpParam=NULL);

private:
	static BOOL RecreateListBox(CListBox* pList, LPVOID lpParam=NULL);

protected:
	COLORREF m_ColorBk;
	COLORREF m_ColorSel;
	COLORREF m_ColorText;
	COLORREF m_ColorSelText;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
};
