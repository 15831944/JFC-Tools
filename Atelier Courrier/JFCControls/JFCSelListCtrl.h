/*
//	JFC Informatique et Média 2006
// 
//	Descriptif:
//	ListCtrl avec sélection multiple type JFC (directement avec mouse)
*/

#pragma once

#include "jfclistctrl.h"

class AFX_EXT_CLASS JFCSelListCtrl : public JFCListCtrl
{
public:
	JFCSelListCtrl();
	virtual ~JFCSelListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
	int m_idxPrec, m_idxFirst, m_idxHaut, m_idxBas;
	bool m_bFirstSel;
};
