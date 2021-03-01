#pragma once

// on inclut les définitions nécessaires
#include "JFCViewportSelect.h"

/////////////////////////////////////////////////////////////////////////////
// JFCListBoxS window

class AFX_EXT_CLASS JFCListBoxS : public JFCViewportSelect
{
public:
	// le constructeur
	JFCListBoxS();

	// les fonctions pour manipuler le nombre d'éléments
	void SetCount(LONG nombre, BOOL update = TRUE);
	LONG GetCount();

	// le destructeur
	virtual ~JFCListBoxS();

protected:
	// la fonction pour récupérer le libellé des éléments
	virtual const TCHAR* OnGetItemLibelle(LONG indice);

protected:
	// la fonction pour récupérer la sélection des éléments
	virtual LONG OnGetItemSelection(LONG indice);

protected:
	// les fonctions pour sélectionner les éléments
	virtual bool OnBeginSelecting(LONG & etat);
	virtual void OnSelectItem(LONG indice, LONG etat);
	virtual void OnEndSelecting();

private:
	// les fonctions pour dessiner les éléments
	bool OnBeginDrawing(CDC & dc);
	void OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat);
	void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	void OnEndDrawing(CDC & dc);

private:
	// les fonctions pour masquer la dimension horizontale
	LONG OnGetItemSelection(LONG i, LONG j);
	void OnSelectItem(LONG i, LONG j, LONG etat);

private:
	LONG m_Count; // le nombre d'éléments

private:
	CPen   m_PenG;   // le stylo G
	COLORREF	m_ColorText;
	CFont  m_FontS;  // la fonte S
	CBrush m_BrushB; // le pinceau B

protected:
	//{{AFX_VIRTUAL(JFCListBoxS)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCListBoxS)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

