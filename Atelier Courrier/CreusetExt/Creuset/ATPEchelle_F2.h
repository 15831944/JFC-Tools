#if !defined(AFX_ATPECHELLE_F2_H__INCLUDED_)
#define AFX_ATPECHELLE_F2_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPEchelle_F2

class CATPEchelle_F2 : public JFCGrid
{
public:
	// le constructeur
	CATPEchelle_F2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPEchelle_F2();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG j);
	JVoid OnRButtonDown(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPEchelle_F2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPEchelle_F2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPECHELLE_F2_H__INCLUDED_)
