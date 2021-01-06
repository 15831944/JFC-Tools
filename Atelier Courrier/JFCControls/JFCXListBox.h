#pragma once

#include "XListBox.h"

// CJFCXListBox

class AFX_EXT_CLASS CJFCXListBox : public CXListBox
{
	DECLARE_DYNAMIC(CJFCXListBox)

public:
	CJFCXListBox();
	virtual ~CJFCXListBox();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// notifie le parent d'un changement de sélection
	void NotifySelChanged();

private:
	int			m_idxPrec, m_idxFirst, m_idxHaut, m_idxBas;
	bool		m_bFirstSel;
	bool		m_bMouseMove;
};


