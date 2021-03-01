/*
//	JFC Informatique et M�dia
//	2005
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFCGridMultiCell
//
//	Descriptif:
//	Glille JFC avec plusieurs sous cellules 
*/

#pragma once
#include "JFCGrid.h"
#include "JFCEditGrid.h"

class AFX_EXT_CLASS JFCGridMultiCell : public JFCGrid
{
public:

	JFCGridMultiCell(void);
	virtual ~JFCGridMultiCell(void);


	// les fonctions pour manipuler le nombre d'�l�ments horizontaux
	void SetHorzCellCount(LONG nombre, BOOL update = TRUE);
	LONG GetHorzCellCount();

	// R�cup�ration du n� de la colonne format
	LONG GetNoColFormat()
	{return m_NoColFormat;};

	// R�cup�ration des infos zone s�lectionn�es
	void GetRowColBeginSelect(LONG &Row1, LONG &Col1, LONG &NoFormat1);

	// Init zone s�lectionn�es
	void InitZoneSelect();

	// Ecriture et lecture mode �dition
	void SetModeEdit(bool ModeBloc);
	bool GetModeEdit();

protected:
	// la fonction pour dessiner l'int�rieur des cellules
	virtual void OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);
	virtual void OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	virtual void OnLButtonDown(LONG i, LONG ii, LONG j);
	virtual void OnLButtonUp(LONG i, LONG ii, LONG j);
	virtual void OnRButtonDown(LONG i, LONG ii, LONG j);
	virtual void OnMouseMove(LONG i, LONG ii, LONG j);
	//virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	// les fonctions pour �diter les �l�ments
	virtual bool OnBeginEditing(CString &CurString, LONG i, LONG j);
	virtual bool OnEditItem(const TCHAR* pString, LONG i, LONG j);
	virtual void OnCancelEditing();
	virtual void OnEndEditing();
	virtual void OnCancelMode();

protected:

	/*
	// les fonctions pour �diter les �l�ments
	virtual bool OnBeginEditing(LONG i, LONG j);
	virtual bool OnEditItem(const TCHAR* pString, LONG i, LONG j);
	virtual void OnCancelEditing();
	virtual void OnEndEditing();
	*/

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg LRESULT OnMsgNewEditingDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingLeft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditingRight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgNewEditing(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	// R�cup�ration du n� de la colonne format
	LONG GetNoFormat(CPoint point);

	// Calcul Zone r�elle d'�dition via la colonne format
	void ZoneEditFormat(RECT &rect_edit, LONG x1, LONG x2, LONG NoColFormat);

	// foctions pour calculer derniere colonne ou ligne visible
	bool GetLastRowDispo(LONG & LastRowVisible, LONG & x1, LONG & y1, LONG & x2, LONG  & y2);
	bool GetLastColDispo(LONG & LastColVisible, LONG & x1, LONG & y1, LONG & x2, LONG  & y2);

	LONG m_HorzCellCount;		// le nombre d'�l�ments horizontaux
	LONG m_NoColFormat;			// le no colonne format

	LONG m_NoColFormatDown;		// le no colonne format down
	LONG m_BeginRow;			// N� de la ligne debut drag drop
	LONG m_BeginCol;			// N� de la colonne debut drag drop

	BOOL m_ModeBlocEdit;		// Mode �dition / par cellule ou par bloc	

	// Special Mode Edit avec saut sur les autres lignes ou colonnes � voir
	BOOL SpecialEditWithJump;

	// Special pour drag drop zone edit cellule
	CPoint m_initialPoint;
	CRect  m_lastRect;
	CBrush m_BrushDragDrop;

};
