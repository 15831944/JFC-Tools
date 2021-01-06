#pragma once

// on inclut les définitions nécessaires
#include "JFCViewport.h"
#include "JFCEditGrid.h"

#define LIGHTBLUE  RGB(0,0,255)

/////////////////////////////////////////////////////////////////////////////
// JFCGrid

class AFX_EXT_CLASS JFCGrid : public JFCViewport
{
public:
	// le constructeur
	JFCGrid();

	// les fonctions pour manipuler le nombre d'éléments horizontaux
	void SetHorzCount(LONG nombre, BOOL update = TRUE);
	LONG GetHorzCount();

	// les fonctions pour manipuler le nombre d'éléments verticaux
	void SetVertCount(LONG nombre, BOOL update = TRUE);
	LONG GetVertCount();

	// Positionnement boite édition saisie
	void SetEditBox(CRect Rect, LONG i, LONG j);

	// Autorise ou n'autorise pas le mode accés édition sur cellule (par défaut autorisé)
	void SetAccesEditBox(bool OkEdit = false);

	// Fermeture boite édit box
	void CloseEditBox();

	BOOL IsValid(int nRow, int nCol) const;

	// le destructeur
	virtual ~JFCGrid();

protected:
	// les fonctions pour éditer les éléments

	// Début édition sur la cellule, avec passage texte par défaut
	virtual bool OnBeginEditing(CString &CurString, LONG i, LONG j);

	// Retour édition
	virtual bool OnEditItem(const TCHAR* pString, LONG i, LONG j);

	// Annuation édition
	virtual void OnCancelEditing();

	// Fin édition
	virtual void OnEndEditing();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	virtual void OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	virtual void OnLButtonDown(LONG i, LONG j);
	virtual void OnLButtonUp(LONG i, LONG j);
	virtual void OnRButtonDown(LONG i, LONG j);
	virtual void OnMouseMove(LONG i, LONG j);
	
	virtual void InitToolTip();

private:
	// les fonctions pour dessiner les éléments
	bool OnBeginDrawing(CDC & dc);
	void OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat);
	void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	void OnEndDrawing(CDC & dc);

	// foctions pour calculer derniere colonne ou ligne visible
	bool GetLastRowDispo(LONG & LastRowVisible, LONG & x1, LONG & y1, LONG & x2, LONG  & y2);
	bool GetLastColDispo(LONG & LastColVisible, LONG & x1, LONG & y1, LONG & x2, LONG  & y2);

protected:
	LONG m_HorzCount; // le nombre d'éléments horizontaux
	LONG m_VertCount; // le nombre d'éléments verticaux

protected:
	LONG m_HorzCursor; // le curseur horizontal
	LONG m_VertCursor; // le curseur vertical

protected:
	CPen   m_PenA;   // le stylo A
	CPen   m_PenG;   // le stylo G
	CFont  m_FontS;  // la fonte S
	CBrush m_BrushA; // le pinceau A
	CBrush m_BrushB; // le pinceau B
	CBrush m_BrushC; // le pinceau C
	CBrush m_BrushD; // le pinceau D

	CBrush m_BrushLight;	// le pinceau degradé light
	CBrush m_BrushMedium;	// le pinceau dégradé médium
	CBrush m_BrushDark;		// le pinceau dégradé dark

	LONG		m_EditBox_Row;
	LONG		m_EditBox_Col;

private:
	JFCEditGrid m_EditBox;			// la boite d'édition
	bool		m_AccesModeEdit;	// Accés ou pas au mode édition cellule 
	

protected:
	//{{AFX_VIRTUAL(JFCGrid)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCGrid)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMsgCancelEditing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgEndEditing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingLeft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingRight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditing(WPARAM wParam, LPARAM lParam);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

	CToolTipCtrl m_tool;
};
