#if !defined(AFX_JFCVERTSPLIT_H__5548660A_6793_416A_9BAC_EDEAB5B4E0B5__INCLUDED_)
#define AFX_JFCVERTSPLIT_H__5548660A_6793_416A_9BAC_EDEAB5B4E0B5__INCLUDED_

// on inclut les définitions nécessaires
#include "JFControl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// JFCVertSplit

class AFX_EXT_CLASS JFCVertSplit : public JFControl
{
public:
	// le constructeur
	JFCVertSplit(UINT Message = 0);

	// la fonction pour activer les déplacements
	BOOL SetRange(LONG Right, LONG Left);

	// le destructeur
	virtual ~JFCVertSplit();

protected:
	//{{AFX_VIRTUAL(JFCVertSplit)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	// la fonction pour dessiner le rectangle
	LONG OnDrawRectangle(LONG Mode);

protected:
	BOOL m_Track;     // le flag de tracking
	LONG m_TrackPos;  // la position du tracking
	LONG m_DeltaPos;  // le décalage de la position du tracking
	LONG m_DispRight; // le déplacement maximum vers la droite
	LONG m_DispLeft;  // le déplacement maximum vers la gauche
	UINT m_Msg;       // le message de mise à jour

protected:
	//{{AFX_MSG(JFCVertSplit)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JFCVERTSPLIT_H__5548660A_6793_416A_9BAC_EDEAB5B4E0B5__INCLUDED_)
