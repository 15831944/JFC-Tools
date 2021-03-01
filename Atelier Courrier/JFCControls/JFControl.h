#pragma once
#include "ATPColors.h"
/////////////////////////////////////////////////////////////////////////////
// JFControl

class AFX_EXT_CLASS JFControl : public CWnd
{
public:
	// le constructeur
	JFControl();

	// la fonction pour créer un contrôle
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT & rect, CWnd* pParentWnd, UINT nID = 0xFFFF);

	// le destructeur
	virtual ~JFControl();

public:
	// les fonctions d'enregistrement de la classe
	static BOOL RegisterClass();
	static BOOL RegisterClass(HINSTANCE hInstance);

	// Sets the controls application color scheme
	virtual void SetAppColor(CATPColors::APPCOLOR AppColor) 
	{ 
		m_CATPColorsApp = AppColor; 
	};

protected:
	CATPColors::APPCOLOR			m_CATPColorsApp;

protected:
	//{{AFX_MSG(JFControl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

