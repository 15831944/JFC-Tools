#if !defined(AFX_ATPCreusetPlan_B2_H__9C0B0B1C_586C_4794_AF8D_7679B32B7C3C__INCLUDED_)
#define AFX_ATPCreusetPlan_B2_H__9C0B0B1C_586C_4794_AF8D_7679B32B7C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////////
// CATPCreusetPlan_B2 : Zone indication nombre d'insertions sur chaque support

class CATPCreusetPlan_B2 : public JFCGrid
{
public:
	// le constructeur
	CATPCreusetPlan_B2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetPlan_B2();

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
	//{{AFX_VIRTUAL(CATPCreusetPlan_B2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetPlan_B2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetPlan_B2_H__9C0B0B1C_586C_4794_AF8D_7679B32B7C3C__INCLUDED_)
