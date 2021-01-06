#pragma once

#include "DlgItems.h"

class AFX_EXT_CLASS CRepItems
{
public:
	// Constructeur (avec mode résultat, et mode visualisation (standard ou simplifie))
	CRepItems(int ShowResults = SHOW_ALL,int ShowMode = SHOWMODE_STANDARD,int ShowSite = SHOW_NOSITE);

	// Destructeur
	virtual ~CRepItems(void);

	void Show(CWnd *pWnd = NULL);
	void Hide();

	BOOL GetState();

	BOOL CanClose();
	void Close();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

private:

	CDlgItems	m_DlgItems;
};
