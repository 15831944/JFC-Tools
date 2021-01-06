#if !defined(AFX_ATPCreusetPlan_C3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_)
#define AFX_ATPCreusetPlan_C3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCWave.h"
class CATPCreusetPlan;

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetPlan_C3

class CATPCreusetPlan_C3 : public JFCWave
{
public:
	// le constructeur
	CATPCreusetPlan_C3();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetPlan_C3();

	JVoid SetParent(CATPCreusetPlan * pATPCreusetPlan);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();
	virtual LONG OnGetItemState(LONG indice, JDate date);

protected:
	//{{AFX_VIRTUAL(CATPCreusetPlan_C3)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetPlan_C3)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	CATPCreusetPlan * m_pATPCreusetPlan;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetPlan_C3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_)
