#if !defined(AFX_ATPZoomPlan_A1_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_)
#define AFX_ATPZoomPlan_A1_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPZoomPlan_A1
class CATPZoomPlan;

class CATPZoomPlan_A1 : public JFCGrid
{
public:
	// le constructeur
	CATPZoomPlan_A1();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	void SetParent(CATPZoomPlan* ATPZoomPlan);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPZoomPlan_A1();


protected:
	// la fonction pour dessiner l'int�rieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPZoomPlan_A1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPZoomPlan_A1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

	CATPZoomPlan * m_pZoom;


};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPZoomPlan_A1_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_)
