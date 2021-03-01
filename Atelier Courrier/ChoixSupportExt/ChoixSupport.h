#pragma once

#include "DlgChoixSupport.h"

class AFX_EXT_CLASS CChoixSupport
{
public:
	// Constructeur
	CChoixSupport(void);

	// Destructeur
	~CChoixSupport(void);

	// Visualisation analyse de l'offre
	void Show(CWnd *pWnd = NULL);

	// Masque annalyse de l'offre
	void Hide();

	// Fermeture analayse de l'offre
	void Close();

	BOOL GetState();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	void Open(CString path);

	// Fermeture ou non de la boite de dialogue choix supports
	JBool CanClose();

private:
	CDlgChoixSupport m_DlgChoixSupport;
};
