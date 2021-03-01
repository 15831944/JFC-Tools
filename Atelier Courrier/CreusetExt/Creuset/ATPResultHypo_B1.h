#if !defined(AFX_ATPResultHypo_B1_H__D4707AB1_B83B_4963_A636_8F1A43589601__INCLUDED_)
#define AFX_ATPResultHypo_B1_H__D4707AB1_B83B_4963_A636_8F1A43589601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPResultHypo_B1 = Entete nom des hypotheses

class CATPResultHypo_B1 : public JFCGrid
{
public:
	// le constructeur
	CATPResultHypo_B1();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPResultHypo_B1();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG j);
	JVoid OnRButtonDown(LONG i, LONG j);
	JVoid OnMouseMove(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPResultHypo_B1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	//{{AFX_MSG(CATPResultHypo_B1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	int m_nbFormatsActifs;
	LONG m_prev_i; 
	LONG m_prev_j;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPResultHypo_B1_H__D4707AB1_B83B_4963_A636_8F1A43589601__INCLUDED_)
