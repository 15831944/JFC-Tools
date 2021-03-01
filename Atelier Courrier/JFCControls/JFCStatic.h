#pragma once

// on inclut les d�finitions n�cessaires
#include "JFControl.h"

/////////////////////////////////////////////////////////////////////////////
// JFCStatic

class AFX_EXT_CLASS JFCStatic : public JFControl
{
public:
	// le constructeur
	JFCStatic();

	// le destructeur
	virtual ~JFCStatic();

protected:
	// la fonction pour dessiner l'�l�ment
	virtual void OnDrawItem(CDC & dc, RECT rect);

protected:
	//{{AFX_VIRTUAL(JFCStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

