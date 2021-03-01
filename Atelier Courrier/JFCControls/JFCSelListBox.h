/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFCSelListBox
//
//	Descriptif:
//	ListBox avec selection type JFC et recr�ation dynamique pour changement de mode de s�lection
*/

#pragma once
#include "afxwin.h"
#include ".\JFCListBox.h"

class AFX_EXT_CLASS JFCSelListBox :
	public JFCListBox
{
public:
	JFCSelListBox(void);
	virtual ~JFCSelListBox(void);

	BOOL RecreateListBox(LPVOID lpParam=NULL);

private:
	static BOOL RecreateListBox(CListBox* pList, LPVOID lpParam=NULL);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// Selectionne ou deselectionne un item sauf s'il est sur-selectionn�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void NotifySelChanged();
	void SelectItemRange(bool bSel, int idxDeb, int idxFin);

private:
	bool m_bMouseMove;
	int m_idxPrec, m_idxFirst, m_idxHaut, m_idxBas;
	bool m_bFirstSel;
};
