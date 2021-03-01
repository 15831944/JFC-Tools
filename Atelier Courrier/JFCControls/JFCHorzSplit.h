#pragma once

// on inclut les d�finitions n�cessaires
#include "JFControl.h"

/////////////////////////////////////////////////////////////////////////////
// JFCHorzSplit

class AFX_EXT_CLASS JFCHorzSplit : public JFControl
{
public:
	// le constructeur
	JFCHorzSplit(UINT Message = 0);

	// la fonction pour activer les d�placements
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
	LONG m_DeltaPos; // le d�calage de la position du tracking
	LONG m_DispDown; // le d�placement maximum vers le bas
	LONG m_DispUp;   // le d�placement maximum vers le haut
	UINT m_Msg;      // le message de mise � jour

protected:
	//{{AFX_MSG(JFCHorzSplit)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

