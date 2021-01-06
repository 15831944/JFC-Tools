// on inclut les définitions nécessaires
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
	// la fonction pour récupérer l'état de l'élément
	virtual LONG OnGetItemState();

protected:
	// les fonctions pour sélectionner l'élément
	virtual bool OnBeginSelecting(LONG & etat);
	virtual void OnSelectItem(LONG etat);
	virtual void OnEndSelecting();

protected:
	// la fonction pour dessiner l'élément
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
	LONG m_State; // l'état du bouton
};

