#if !defined(AFX_ATPResultHypo_A2_H__0B46684E_B18F_4A5E_922A_B189F7F02F01__INCLUDED_)
#define AFX_ATPResultHypo_A2_H__0B46684E_B18F_4A5E_922A_B189F7F02F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPResult_GrandeursHypo.h"
/////////////////////////////////////////////////////////////////////////////
// CATPResultHypo_A2 = Liste des supports

class CATPResultHypo_A2 : public JFCGrid
{
public:
	// le constructeur
	CATPResultHypo_A2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPResultHypo_A2();

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
	//{{AFX_VIRTUAL(CATPResultHypo_A2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPResultHypo_A2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

	CATPResult_GrandeursHypo m_ResultGrandeurs;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPResultHypo_A2_H__0B46684E_B18F_4A5E_922A_B189F7F02F01__INCLUDED_)
