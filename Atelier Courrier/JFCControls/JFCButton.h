// on inclut les d�finitions n�cessaires
#pragma once

#include "JFControl.h"

/////////////////////////////////////////////////////////////////////////////
// JFCButton

class AFX_EXT_CLASS JFCButton : public JFControl
{
public:
	// le constructeur
	JFCButton();

	// le destructeur
	virtual ~JFCButton();
	
protected:
	// la fonction pour r�cup�rer l'�tat de l'�l�ment
	virtual LONG OnGetItemState();

protected:
	// les fonctions pour s�lectionner l'�l�ment
	virtual bool OnBeginSelecting(LONG & etat);
	virtual void OnSelectItem(LONG etat);
	virtual void OnEndSelecting();

protected:
	// la fonction pour dessiner l'�l�ment
	virtual void OnDrawItem(CDC & dc, RECT rect);

protected:
	//{{AFX_VIRTUAL(JFCButton)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	LONG m_State; // l'�tat du bouton
};

