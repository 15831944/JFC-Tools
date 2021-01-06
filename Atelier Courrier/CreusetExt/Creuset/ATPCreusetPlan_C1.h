#if !defined(AFX_ATPCreusetPlan_C1_H__E36D565E_25C5_49AB_ACF5_56B95D98608A__INCLUDED_)
#define AFX_ATPCreusetPlan_C1_H__E36D565E_25C5_49AB_ACF5_56B95D98608A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCCalendar.h"
class CATPCreusetPlan;
/////////////////////////////////////////////////////////////////////////////
// CATPCreusetPlan_C1

class CATPCreusetPlan_C1 : public JFCCalendar
{
public:
	// le constructeur
	CATPCreusetPlan_C1();
	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	// la fonction pour mettre à jour le composant
	virtual JVoid Update();
	// le destructeur
	virtual ~CATPCreusetPlan_C1();

	JVoid SetParent(CATPCreusetPlan * pATPCreusetPlan);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetPlan_C1)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetPlan_C1)
	afx_msg LRESULT OnSelChanging(WPARAM, LPARAM);
	afx_msg LRESULT OnSelChanged(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	CATPCreusetPlan * m_pATPCreusetPlan;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetPlan_C1_H__E36D565E_25C5_49AB_ACF5_56B95D98608A__INCLUDED_)
