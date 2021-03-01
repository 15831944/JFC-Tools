#pragma once

// on inclut les d�finitions n�cessaires
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
	// la fonction pour r�cup�rer la s�lection des �l�ments
	virtual LONG OnGetItemSelection(LONG i, LONG j);

protected:
	// les fonctions pour s�lectionner les �l�ments
	virtual bool OnBeginSelecting(LONG & etat);
	virtual void OnSelectItem(LONG i, LONG j, LONG etat);
	virtual void OnEndSelecting();

private:
	// la fonction pour r�cup�rer l'�tat des �l�ments
	LONG OnGetItemState(LONG i, LONG j);

private:
	BOOL m_FlagSel; // l'indicateur de s�lection
	LONG m_EtatSel; // le nouvel �tat des �l�ments s�lectionn�s

private:
	LONG m_HorzSel; // la position initiale de la s�lection horizontale
	LONG m_VertSel; // la position initiale de la s�lection verticale

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
