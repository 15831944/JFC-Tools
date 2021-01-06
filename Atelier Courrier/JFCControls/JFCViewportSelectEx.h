#pragma once

// on inclut les définitions nécessaires
#include "JFCViewport.h"

/////////////////////////////////////////////////////////////////////////////
// JFCViewportSelectEx

class AFX_EXT_CLASS JFCViewportSelectEx : public JFCViewport
{
public:
	// le constructeur
	JFCViewportSelectEx();

	// le destructeur
	virtual ~JFCViewportSelectEx();

protected:
	// la fonction pour récupérer la sélection des éléments
	virtual LONG OnGetItemSelection(LONG i, LONG j);

protected:
	// les fonctions pour sélectionner les éléments
	virtual bool OnBeginSelecting(LONG & etat);
	virtual void OnSelectItem(LONG i, LONG j, LONG etat);
	virtual void OnEndSelecting();

private:
	// la fonction pour récupérer l'état des éléments
	LONG OnGetItemState(LONG i, LONG j);

private:
	BOOL m_FlagSel; // l'indicateur de sélection
	LONG m_EtatSel; // le nouvel état des éléments sélectionnés

private:
	LONG m_HorzSel1; // la position initiale  de la sélection horizontale
	LONG m_HorzSel2; // la position terminale de la sélection horizontale

private:
	LONG m_VertSel1; // la position initiale  de la sélection verticale
	LONG m_VertSel2; // la position terminale de la sélection verticale

protected:
	//{{AFX_VIRTUAL(JFCViewportSelectEx)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCViewportSelectEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

