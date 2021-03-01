#pragma once
#include "JFC_MultiListBox.h"

class AFX_EXT_CLASS CListBoxItems : public JFC_MultiListBox<IItem const *>
{
public:
	CListBoxItems(void);
	~CListBoxItems(void);

protected:
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
