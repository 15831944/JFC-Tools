#pragma once

// on inclut les définitions nécessaires
#include "JFControl.h"

/////////////////////////////////////////////////////////////////////////////
// JFCHorzSplit

class AFX_EXT_CLASS JFCHorzSplit : public JFControl
{
public:
	// le constructeur
	JFCHorzSplit(UINT Message = 0);

	// la fonction pour activer les déplacements
	BOOL SetRange(LONG Down, LONG Up);

	// le destructeur
	virtual ~JFCHorzSplit();

protected:
	//{{AFX_VIRTUAL(JFCHorzSplit)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	// la fonction pour dessiner le rectangle
	LONG OnDrawRectangle(LONG Mode);

protected:
	BOOL m_Track;    // le flag de tracking
	LONG m_TrackPos; // la position du tracking
	LONG m_DeltaPos; // le décalage de la position du tracking
	LONG m_DispDown; // le déplacement maximum vers le bas
	LONG m_DispUp;   // le déplacement maximum vers le haut
	UINT m_Msg;      // le message de mise à jour

protected:
	//{{AFX_MSG(JFCHorzSplit)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

