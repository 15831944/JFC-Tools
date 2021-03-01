#pragma once

// on inclut les d�finitions n�cessaires
#include "JFControl.h"
#include "JFCScroll.h"

/////////////////////////////////////////////////////////////////////////////
// JFCViewport

class AFX_EXT_CLASS JFCViewport : public JFControl
{
public:
	// le constructeur
	JFCViewport();

	// les fonctions pour d�finir les ascenseurs externes
	void SetHorzScroll(JFCScroll* pScroll, BOOL Master, BOOL update = TRUE);
	void SetVertScroll(JFCScroll* pScroll, BOOL Master, BOOL update = TRUE);

	// les fonctions pour r�cup�rer les ascenseurs externes
	JFCScroll* GetHorzScroll();
	JFCScroll* GetVertScroll();

	// le destructeur
	virtual ~JFCViewport();

	// les fonctions pour d�finir la taille des �l�ments
	void SetHorzItemSize(LONG size, BOOL border, BOOL update = TRUE);
	void SetVertItemSize(LONG size, BOOL border, BOOL update = TRUE);
	LONG GetHorzItemSize() {return m_HorzSize;};
	LONG GetVertItemSize() {return m_VertSize;};

protected:
	// les fonctions pour d�finir l'intervalle des �l�ments
	void SetHorzItemRange(LONG first, LONG last, BOOL update = TRUE);
	void SetVertItemRange(LONG first, LONG last, BOOL update = TRUE);

protected:
	// la fonction pour rendre visible un �l�ment
	bool EnsureVisible(LONG i, LONG j);

protected:
	// les fonctions pour convertir les indices et les positions
	bool GetIdxAtPos(LONG x, LONG y, LONG & i,  LONG & j);
	bool GetPosAtIdx(LONG i, LONG j, LONG & x1, LONG & y1, LONG & x2, LONG & y2);

protected:
	// la fonction pour convertir les positions en indices de tracking
	void GetTrackIdx(LONG x, LONG y, LONG & i, LONG & j, BOOL scroll);

protected:
	// la fonction pour r�cup�rer l'�tat des �l�ments
	virtual LONG OnGetItemState(LONG i, LONG j);

protected:
	// les fonctions pour dessiner les �l�ments
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat);
	virtual void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	virtual void OnEndDrawing(CDC & dc);

// private:

public:
	// la fonction pour configurer les ascenseurs actifs
	void OnSetScrollBar(BOOL update);

	// les fonctions pour modifier la position des ascenseurs actifs
	bool OnSetHorzPosition(LONG position);
	bool OnSetVertPosition(LONG position);

	// les fonctions pour r�cup�rer la position des ascenseurs actifs
	LONG OnGetHorzPosition();
	LONG OnGetVertPosition();

// private:
public:
	// les fonctions pour faire d�filer l'affichage
	void OnHorzScrollItems(BOOL forward);
	void OnVertScrollItems(BOOL forward);

	// les fonctions pour actualiser la position d'affichage
	void OnHorzScrollPosition();
	void OnVertScrollPosition();

private:
	// les fonctions pour dessiner les �l�ments
	void OnDrawStates(CDC & dc, LONG x, LONG y, LONG cx, LONG cy, LONG i, LONG j, LONG ci, LONG cj);
	virtual void OnDrawItems (CDC & dc, LONG x, LONG y, LONG cx, LONG cy, LONG i, LONG j, LONG ci, LONG cj);

protected:
	// les fonctions pour dessiner la fen�tre
	virtual void OnDrawWindow(CDC & dc, LONG x, LONG y, LONG dx, LONG dy);
	virtual void OnDrawBorder(CDC & dc, RECT client, LONG x, LONG y, LONG dx, LONG dy);

private:
	LONG m_HorzFirst; // le premier �l�ment horizontal
	LONG m_HorzLast;  // le dernier �l�ment horizontal
	LONG m_HorzSize;  // la taille des �l�ments horizontaux

private:
	LONG m_VertFirst; // le premier �l�ment vertical
	LONG m_VertLast;  // le dernier �l�ment vertical
	LONG m_VertSize;  // la taille des �l�ments verticaux

private:
	BOOL m_HorzBorder; // l'indicateur des bordures horizontales	
	BOOL m_VertBorder; // l'indicateur des bordures verticales
	
private:
	BOOL m_HorzMaster; // l'indicateur de l'ascenseur horizontal
	BOOL m_VertMaster; // l'indicateur de l'ascenseur vertical

private:
	JFCScroll* m_pHorzScroll; // le pointeur sur l'ascenseur horizontal
	JFCScroll* m_pVertScroll; // le pointeur sur l'ascenseur vertical

protected:
	//{{AFX_VIRTUAL(JFCViewport)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCViewport)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

