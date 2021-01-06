#pragma once
#include "JFC_MultiListBox.h"

class AFX_EXT_CLASS CListBoxDimensions : public JFC_MultiListBox<IModalite const *>
{
public:
	CListBoxDimensions(void);
	~CListBoxDimensions(void);

public:
	VOID SetRightFocus(BOOL fRightFocus);

protected:
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_fRightFocus;
};
