#pragma once
#include "JFC_MultiListBox.h"

class AFX_EXT_CLASS CListBoxSas : public JFC_MultiListBox<IQuestion const *>
{
public:

	CListBoxSas(void);
	~CListBoxSas(void);

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
