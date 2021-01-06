#pragma once

// on inclut les définitions nécessaires
#include "JFControl.h"

/////////////////////////////////////////////////////////////////////////////
// JFCPane

class AFX_EXT_CLASS JFCPane : public JFControl
{
public:
	// le constructeur
	JFCPane();

	// le destructeur
	virtual ~JFCPane();

protected:
	// les modes de déplacement
	enum { MGER_NONE = 0, MGER_MOVE, MGER_SIZE, MGER_SHIFT, MGER_STRETCH, MGER_SHIFTANDMOVE, MGER_SHIFTANDSIZE, MGER_STRETCHANDSIZE };

protected:
	// la fonction pour configurer les modes de déplacement
	void SetMode(CWnd* pWnd, LONG HMode, LONG VMode);

	// la fonction pour configurer la gestion des ascenseurs
	void SetScrollBar(CWnd* pWnd, CWnd* pHScroll, CWnd* pVScroll);

	// la fonction pour configurer les fenêtres du stretching
	void SetStretchWnd(CWnd* pHStretch, CWnd* pVStretch);

protected:
	// la fonction pour modifier les coefficients de stretch
	void SetStretchCoef(LONG HCoef, LONG VCoef);

private:
	// la définition d'un élément
	struct Link
	{
		LONG  m_HPos;     // la position horizontale initiale
		LONG  m_VPos;     // la position verticale initiale
		LONG  m_HSize;    // la taille horizontale initiale
		LONG  m_VSize;    // la taille verticale initiale
		LONG  m_HMode;    // le mode de déplacement horizontal
		LONG  m_VMode;    // le mode de déplacement vertical
		CWnd* m_pHScroll; // l'ascenseur horizontal
		CWnd* m_pVScroll; // l'ascenseur vertical
	};

private:
	// la fonction pour déplacer les éléments
	void OnUpdateWndPos(LONG cx, LONG cy);

	// la fonction pour mettre à jour les positions
	void OnUpdatePosition(LONG dx, LONG sx, LONG mode, LONG & pos, LONG & size);

private:
	// les fonctions pour mettre à jour les éléments
	void OnUpdateHorzScroll(CWnd* pScroll);
	void OnUpdateVertScroll(CWnd* pScroll);

private:
	LONG m_HSize; // la taille horizontale de la fenêtre
	LONG m_VSize; // la taille verticale   de la fenêtre

private:
	LONG m_HStretch; // la taille horizontale du stretch
	LONG m_VStretch; // la taille verticale   du stretch

private:
	LONG m_HCoef; // le coef horizontal du stretch
	LONG m_VCoef; // la coef vertical   du stretch

protected:
	CMap<CWnd*, CWnd*, Link, Link &> m_Items; // les éléments

protected:
	//{{AFX_VIRTUAL(JFCPane)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

