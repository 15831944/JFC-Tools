#if !defined(AFX_JFCVIEW_H__2F607178_7E50_4392_AC97_3E565E9508B9__INCLUDED_)
#define AFX_JFCVIEW_H__2F607178_7E50_4392_AC97_3E565E9508B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JFControl.h"

/////////////////////////////////////////////////////////////////////////////
// JFCView

class JFCView : public JFControl
{
public:
	// le constructeur
	JFCView();

	// le destructeur
	virtual ~JFCView();

public:
	// Définition et récupération du pas de graduation en X
	long GetPasGraduationX();
	void SetPasGraduationX(LONG PasGraduationX);

	// Définition et récupération du pas de graduation en Y
	long GetPasGraduationY();
	void SetPasGraduationY(LONG PasGraduationY);

protected:
	// les fonctions pour récupérer les dimensions
	long GetHorzSize();
	long GetVertSize();

protected:
	// les fonctions pour convertir les positions
	bool GetIdxAtPos(LONG x, LONG y, LONG & i, LONG & j);
	bool GetPosAtIdx(LONG i, LONG j, LONG & x, LONG & y);

protected:
	// les fonctions pour convertir les positions en indices de tracking
	void GetTrackIdx(LONG x, LONG y, LONG & i, LONG & j);
	void GetTrackPos(LONG i, LONG j, LONG & x, LONG & y);

protected:
	// les fonctions pour dessiner les éléments
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnDrawItems(CDC & dc, RECT rect);
	virtual void OnEndDrawing(CDC & dc);

private:
	// la fonction pour mettre à jour les dimensions
	void OnSetSize();

private:
	LONG m_HorzSize;		// la taille horizontale
	LONG m_VertSize;		// la taille verticale
    LONG m_PasGraduationX;  // le pas de graduation en X 
	LONG m_PasGraduationY;  // le pas de graduation en Y 

protected:
	//{{AFX_VIRTUAL(JFCView)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JFCVIEW_H__2F607178_7E50_4392_AC97_3E565E9508B9__INCLUDED_)
