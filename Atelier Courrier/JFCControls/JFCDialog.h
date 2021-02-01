#pragma once

#include "JFCUpdate.h"
/////////////////////////////////////////////////////////////////////////////
// JFCDialog

class AFX_EXT_CLASS JFCDialog : public CDialog
{
public:
	// les constructeurs
	JFCDialog();
	JFCDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	JFCDialog(LPCTSTR lpszTemplateName, CWnd* pParent = NULL);

	// le destructeur
	virtual ~JFCDialog();

	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);
protected:

protected:
	// les modes de d�placement
	enum { MGER_NONE = 0, MGER_MOVE, MGER_SIZE, MGER_SHIFT, MGER_STRETCH, MGER_SHIFTANDMOVE, MGER_SHIFTANDSIZE, MGER_STRETCHANDSIZE };

protected:
	// la fonction pour configurer les modes de d�placement
	void SetMode(CWnd* pWnd, LONG HMode, LONG VMode);

	// la fonction pour configurer la gestion des ascenseurs
	void SetScrollBar(CWnd* pWnd, CWnd* pHScroll, CWnd* pVScroll);

	// la fonction pour configurer les fen�tres du stretching
	void SetStretchWnd(CWnd* pHStretch, CWnd* pVStretch);

protected:
	// la fonction pour modifier les coefficients de stretch
	void SetStretchCoef(LONG HCoef, LONG VCoef);

private:
	// la d�finition d'un �l�ment
	struct Link
	{
		LONG  m_HPos;     // la position horizontale initiale
		LONG  m_VPos;     // la position verticale initiale
		LONG  m_HSize;    // la taille horizontale initiale
		LONG  m_VSize;    // la taille verticale initiale
		LONG  m_HMode;    // le mode de d�placement horizontal
		LONG  m_VMode;    // le mode de d�placement vertical
		CWnd* m_pHScroll; // l'ascenseur horizontal
		CWnd* m_pVScroll; // l'ascenseur vertical
	};

private:
	// la fonction pour d�placer les �l�ments
	void OnUpdateWndPos(LONG cx, LONG cy);

	// la fonction pour mettre � jour les positions
	void OnUpdatePosition(LONG dx, LONG sx, LONG mode, LONG & pos, LONG & size);

private:
	// les fonctions pour mettre � jour les �l�ments
	void OnUpdateHorzScroll(CWnd* pScroll);
	void OnUpdateVertScroll(CWnd* pScroll);

private:
	LONG m_HSize; // la taille horizontale de la bo�te
	LONG m_VSize; // la taille verticale   de la bo�te

private:
	LONG m_HStretch; // la taille horizontale du stretch
	LONG m_VStretch; // la taille verticale   du stretch

private:
	LONG m_HCoef; // le coef horizontal du stretch
	LONG m_VCoef; // la coef vertical   du stretch

private:
	CMap<CWnd*, CWnd*, Link, Link &> m_Items; // les �l�ments

protected:
	//{{AFX_VIRTUAL(JFCDialog)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
