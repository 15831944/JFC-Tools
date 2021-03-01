#pragma once

#include "DlgAdvision.h"

class AFX_EXT_CLASS CAdvision
{
public:
	CAdvision(void);
	~CAdvision(void);
	void Show(CWnd *pWnd = NULL);
	void Hide();

	void Open(CString path);

	BOOL GetState();

	BOOL CanClose();
	void Close();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

private:
	// Fenetre application Advision
	CDlgAdvision	m_DlgAdvision;
};