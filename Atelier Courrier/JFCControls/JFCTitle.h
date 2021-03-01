#pragma once

// on inclut les d�finitions n�cessaires
#include "JFControl.h"
/////////////////////////////////////////////////////////////////////////////
// JFCTitle

class AFX_EXT_CLASS JFCTitle : public JFControl
{
public:
	// le constructeur
	JFCTitle();

	// le destructeur
	virtual ~JFCTitle();

	/*
	void SetFontName(TCHAR* FontName);

	void SetBorder(BOOL bSet);
	*/

	// la fonction pour activer le bord et d�finir sa couleur
	void SetBorderColor(COLORREF crBorder);

	// la fonction pour indiquer l'application o� le controle se place
	void SetAppColor(CATPColors::APPCOLOR AppColor);

protected:
	// la fonction pour dessiner l'�l�ment
	virtual void OnDrawItem(CDC & dc, RECT rect);

	// la fonction pour dessinner le bord (en relief)
	virtual void OnDrawBorder(CDC & dc, RECT rc);

protected:
	CATPColors::APPCOLOR			m_CATPColorsApp;

protected:
	//{{AFX_VIRTUAL(JFCTitle)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCTitle)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	bool		m_crBorderSet;
	COLORREF	m_crBorder;
};

