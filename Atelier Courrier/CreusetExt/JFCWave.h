#if !defined(AFX_JFCWAVE_H__22887966_38B3_4F1C_B564_7938611F0ED4__INCLUDED_)
#define AFX_JFCWAVE_H__22887966_38B3_4F1C_B564_7938611F0ED4__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JLib.h"
#include "JFCViewport.h"

/////////////////////////////////////////////////////////////////////////////
// JFCWave

class JFCWave : public JFCViewport
{
public:
	// le constructeur
	JFCWave();

	// les fonctions pour manipuler le nombre d'éléments
	void SetCount(LONG nombre, BOOL update = TRUE);
	LONG GetCount();

	// les fonctions pour manipuler la période
	void SetRange(JDate   debut, JDate   fin, BOOL update = TRUE);
	void GetRange(JDate & debut, JDate & fin);

	// le destructeur
	virtual ~JFCWave();

protected:
	// la fonction pour récupérer l'état des éléments
	virtual LONG OnGetItemState(LONG indice, JDate date);

	// les fonctions pour capter les clicks de la souris
	virtual void OnLButtonDown(LONG indice, JDate date);
	virtual void OnLButtonUp(LONG indice, JDate date);
	virtual void OnRButtonDown(LONG indice, JDate date);

	// la fonction pour capter les déplacements de la souris
	virtual void OnMouseMove(LONG indice, JDate date);

	// la fonction pour récupérer l'état des éléments
	virtual LONG OnGetItemState(LONG i, LONG j);

	// les fonctions pour dessiner les éléments
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat);
	virtual void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	virtual void OnEndDrawing(CDC & dc);

	LONG m_Count; // le nombre d'éléments

	JDate m_Debut; // la date de début
	JDate m_Fin;   // la date de fin


protected:
	//{{AFX_VIRTUAL(JFCWave)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCWave)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DrawStatus(Graphics& grf, RECT rect, LONG etat);

	void DrawSuperpose(Graphics& grf, RECT rect, LONG etat);

	void DrawChromie4(Graphics& grf, RECT rect);
	void DrawChromie3(Graphics& grf, RECT rect);
	void DrawChromie2(Graphics& grf, RECT rect);
	void DrawChromieMono(Graphics& grf, RECT rect);

	void DrawStatut1(Graphics& grf, RECT rect);
	void DrawStatut2(Graphics& grf, RECT rect);


	void DrawElemStart(Graphics& grf, RECT rect, LONG etat);
	void DrawElemStop(Graphics& grf, RECT rect, LONG etat);
	void DrawElem(Graphics& grf, RECT rect, LONG etat);
	void DrawQtd(Graphics& grf, RECT rect, LONG etat);

	LONG Hsize; 

};

#define JFCWAVE_EXTEND			0xFF000000

#define JFCWAVE_EXTEND_MASK		0x00FFFFFF


#define JFCWAVE_FOND			0x00FF0000

#define JFCWAVE_FOND_SEL		0x00020000
#define JFCWAVE_FOND_GRIS		0x00030000
#define JFCWAVE_FOND_RED		0x00040000
#define JFCWAVE_FOND_TRF1		0x00050000
#define JFCWAVE_FOND_TRF2		0x00060000
#define JFCWAVE_FOND_RED2		0x00070000

#define JFCWAVE_FOND_MASK		0xFF00FFFF


#define JFCWAVE_ELEM			0x000000FF

#define JFCWAVE_ELEM_ELEM			0x00000001
#define JFCWAVE_ELEM_ELEM_BORD		0x00000002
#define JFCWAVE_ELEM_START			0x00000003
#define JFCWAVE_ELEM_START_BORD		0x00000004
#define JFCWAVE_ELEM_STOP			0x00000005
#define JFCWAVE_ELEM_STOP_BORD		0x00000006
#define JFCWAVE_ELEM_QTD			0x00000007
#define JFCWAVE_ELEM_QTD_BORD		0x00000008

#define JFCWAVE_ELEM_MASK		0xFFFFFF00


#define JFCWAVE_STATE			0x0000FF00

#define JFCWAVE_STATE_PLEIN		0x00000100
#define JFCWAVE_STATE_BORD		0x00000200
#define JFCWAVE_STATE_SUPERPOSE	0x00000300
#define JFCWAVE_STATE_CHROMIE1	0x00000400
#define JFCWAVE_STATE_CHROMIE2	0x00000500
#define JFCWAVE_STATE_CHROMIE3	0x00000600
#define JFCWAVE_STATE_CHROMIE4	0x00000700
#define JFCWAVE_STATE_STATUS1	0x00000800
#define JFCWAVE_STATE_STATUS2	0x00000900
#define JFCWAVE_STATE_STATUS3	0x00000A00
#define JFCWAVE_STATE_STATUS4	0x00000B00
#define JFCWAVE_STATE_STATUS5	0x00000C00

#define JFCWAVE_STATE_MASK		0xFFFF00FF



/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JFCWAVE_H__22887966_38B3_4F1C_B564_7938611F0ED4__INCLUDED_)
