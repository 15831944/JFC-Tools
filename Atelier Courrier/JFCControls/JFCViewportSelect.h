#pragma once

// on inclut les définitions nécessaires
#include "JFCViewport.h"

/////////////////////////////////////////////////////////////////////////////
// JFCViewportSelect

class AFX_EXT_CLASS JFCViewportSelect : public JFCViewport
{
public:
	// le constructeur
	JFCViewportSelect();

	// le destructeur
	virtual ~JFCViewportSelect();

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
	LONG m_HorzSel; // la position initiale de la sélection horizontale
	LONG m_VertSel; // la position initiale de la sélection verticale

protected:
	//{{AFX_VIRTUAL(JFCViewportSelect)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCViewportSelect)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
