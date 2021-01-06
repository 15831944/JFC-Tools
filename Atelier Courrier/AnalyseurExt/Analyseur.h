#pragma once

#include "DlgAnalyseur.h"

class AFX_EXT_CLASS CAnalyseur
{
public:

	// Constructeur
	CAnalyseur(void);

	// Destructeur
	~CAnalyseur(void);

	// Visibilité boite de dialogue
	void Show(CWnd *pWnd = NULL);
	void Hide();

	void Open(CString path);

	BOOL CanClose();
	void Close();

	BOOL GetState();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

private:

	CDlgAnalyseur	m_DlgAnalyseur;
};
