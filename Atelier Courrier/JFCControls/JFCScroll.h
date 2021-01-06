#pragma once

/////////////////////////////////////////////////////////////////////////////
// JFCScroll

class AFX_EXT_CLASS JFCScroll : public CWnd
{
public:
	// le constructeur
	JFCScroll();

	// la fonction pour créer un contrôle
	BOOL Create(DWORD dwStyle, const RECT & Rect, CWnd* pParentWnd, UINT nID = 0xFFFF);

	// le destructeur
	virtual ~JFCScroll();

	// la fonction pour manipuler la configuration de l'ascenseur
	void SetRange(LONG minimum, LONG maximum, LONG page);

	// les fonctions pour manipuler la position de l'ascenseur
	void SetPosition(LONG position);
	LONG GetPosition();

protected:
	//{{AFX_VIRTUAL(JFCScroll)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCScroll)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

