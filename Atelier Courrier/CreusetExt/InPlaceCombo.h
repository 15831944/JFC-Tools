#pragma once

#include "ClrSelComboBox.h"
// CInPlaceCombo

class CInPlaceCombo : public CClrSelComboBox
{
	DECLARE_DYNAMIC(CInPlaceCombo)

public:
    CInPlaceCombo(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID);
	virtual ~CInPlaceCombo();
 
	void EndCombo();

protected:
	DECLARE_MESSAGE_MAP()

//	virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg void OnKillFocus(CWnd* pNewWnd);

	virtual void PostNcDestroy();
};


