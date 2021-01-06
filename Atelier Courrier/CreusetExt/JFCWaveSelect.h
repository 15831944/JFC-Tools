#if !defined(AFX_JFCWaveSelect_H__22887966_38B3_4F1C_B564_7938611F0ED4__INCLUDED_)
#define AFX_JFCWaveSelect_H__22887966_38B3_4F1C_B564_7938611F0ED4__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JLib.h"
#include "JFCWave.h"

/////////////////////////////////////////////////////////////////////////////
// JFCWaveSelect

class JFCWaveSelect : public JFCWave
{
public:
	// le constructeur
	JFCWaveSelect();
	// le destructeur
	virtual ~JFCWaveSelect();

	bool HasSelection();
	void ResetSelection();
	void GetSelRange(LONG &iStart, LONG& jStart, LONG &iStop, LONG& jStop);

	void AllowSelection(bool allow = true);

protected:
	// la fonction pour récupérer l'état des éléments
	virtual LONG OnGetItemState(LONG i, LONG j);
	// la fonction pour récupérer l'état des éléments
	virtual LONG OnGetItemState(LONG indice, JDate date);

	// les fonctions pour capter les clicks de la souris
	virtual void OnLButtonDown(LONG indice, JDate date);
	virtual void OnLButtonUp(LONG indice, JDate date);
	virtual void OnRButtonDown(LONG indice, JDate date);

	// la fonction pour capter les déplacements de la souris
	virtual void OnMouseMove(LONG indice, JDate date);



protected:
	//{{AFX_VIRTUAL(JFCWaveSelect)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCWaveSelect)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPoint m_ptFirst;
	CRect m_rectSel;
	bool m_bMoved;
	bool m_bSelAllowed;
};

#endif // !defined(AFX_JFCWaveSelect_H__22887966_38B3_4F1C_B564_7938611F0ED4__INCLUDED_)
