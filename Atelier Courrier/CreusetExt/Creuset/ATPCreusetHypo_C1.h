#if !defined(AFX_ATPCreusetHypo_C1_H__E36D565E_25C5_49AB_ACF5_56B95D98608A__INCLUDED_)
#define AFX_ATPCreusetHypo_C1_H__E36D565E_25C5_49AB_ACF5_56B95D98608A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_C1  : Entete Colonne Nom des hypothèses

class CATPCreusetHypo_C1 : public JFCGrid
{
public:
	// le constructeur
	CATPCreusetHypo_C1();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_C1();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG j);
	// JVoid OnRButtonDown(LONG i, LONG j);
	JVoid OnMouseMove(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_C1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	//{{AFX_MSG(CATPCreusetHypo_C1)
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

#endif // !defined(AFX_ATPCreusetHypo_C1_H__E36D565E_25C5_49AB_ACF5_56B95D98608A__INCLUDED_)
