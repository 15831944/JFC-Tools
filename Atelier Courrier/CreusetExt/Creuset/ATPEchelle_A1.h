#if !defined(AFX_ATPECHELLE_A1_H__B45EA816_4464_4BA6_8FF7_19B82210AAFA__INCLUDED_)
#define AFX_ATPECHELLE_A1_H__B45EA816_4464_4BA6_8FF7_19B82210AAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPEchelle_A1

class CATPEchelle_A1 : public JFCGrid
{
public:
	// le constructeur
	CATPEchelle_A1();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPEchelle_A1();

protected:
	// la fonction pour dessiner l'int�rieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG j);
	JVoid OnRButtonDown(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPEchelle_A1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPEchelle_A1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPECHELLE_A1_H__B45EA816_4464_4BA6_8FF7_19B82210AAFA__INCLUDED_)
