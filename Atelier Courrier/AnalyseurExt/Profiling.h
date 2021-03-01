#pragma once

#include "DlgProfiling.h"

class AFX_EXT_CLASS CProfiling
{
public:
	CProfiling(void);
	~CProfiling(void);
	void Show(CWnd *pWnd = NULL);
	void Hide();

	void Open(CString path);

	BOOL GetState();

	BOOL CanClose();
	void Close();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

private:

	CDlgProfiling	m_DlgProfiling;
};
