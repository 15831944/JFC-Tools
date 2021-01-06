#if !defined(AFX_ATPZoomPlan_A2_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_)
#define AFX_ATPZoomPlan_A2_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
class CATPZoomPlan;

/////////////////////////////////////////////////////////////////////////////
// CATPZoomPlan_A2

class CATPZoomPlan_A2 : public JFCGrid
{
public:
	// le constructeur
	CATPZoomPlan_A2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	void SetParent(CATPZoomPlan* ATPZoomPlan);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPZoomPlan_A2();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPZoomPlan_A2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPZoomPlan_A2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

	CATPZoomPlan * m_pZoom;

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPZoomPlan_A2_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_)
