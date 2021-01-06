#pragma once
// on inclut les d�finitions n�cessaires
#include "JLib.h"
#include "JFCViewport.h"

/////////////////////////////////////////////////////////////////////////////
// JFCCalendar

#define MSG_CALENDAR_SELCHANGING 666
#define MSG_CALENDAR_SELCHANGED MSG_CALENDAR_SELCHANGING+1

class AFX_EXT_CLASS JFCCalendar : public JFCViewport
{
public:
	// le constructeur
	JFCCalendar();

	// les fonctions pour manipuler la p�riode
	void SetRange(JDate   debut, JDate   fin, BOOL update = TRUE);
	void GetRange(JDate & debut, JDate & fin);

	// la fonction pour rendre visible une date
	void EnsureVisible(JDate date);

	// le destructeur
	virtual ~JFCCalendar();

	// les fonctions pour manipuler la p�riode selectionnee
	void SetSelRange(JDate   first, JDate   last, BOOL update = TRUE);
	void GetSelRange(JDate & first, JDate & last);
	void BtnT();
protected:
	void Reset();

protected:
	// la fonction pour calculer le span des �l�ments
	void OnGetItemSpan(LONG i, LONG y, LONG & first, LONG & last);

	// la fonction pour r�cup�rer la s�lection des �l�ments
	BOOL OnGetItemSelection(JDate date);
	// la fonction pour r�cup�rer l'�tat des �l�ments
	LONG OnGetItemState(LONG i, LONG j);

	// les fonctions pour dessiner les �l�ments
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat);
	virtual void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	virtual void OnEndDrawing(CDC & dc);

protected:
	JDate m_Debut; // la date de d�but
	JDate m_Fin;   // la date de fin
	
protected:
	BOOL m_FlagSel;  // l'indicateur de s�lection
protected:
	LONG m_EtatSel;  // le nouvel �tat des �l�ments s�lectionn�s

	LONG m_FrstSelTmp; // la premi�re position initiale  de la s�lection
	LONG m_LastSelTmp; // la premi�re position terminale de la s�lection

	LONG m_FrstSel; // la deuxi�me position initiale  de la s�lection
	LONG m_LastSel; // la deuxi�me position terminale de la s�lection

	CPen   m_PenA;   // le stylo A
	CPen   m_PenG;   // le stylo G
	CFont  m_FontS;  // la fonte S
	CBrush m_BrushD; // le pinceau D

	LONG m_Decalage; // le d�calage

protected:
	//{{AFX_VIRTUAL(JFCCalendar)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCCalendar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

