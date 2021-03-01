#if !defined(AFX_ATPCreusetPlan_A0_H__INCLUDED_)
#define AFX_ATPCreusetPlan_A0_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetPlan_A0

class CATPCreusetPlan_A0 : public JFCTitle
{
public:
	// le constructeur
	CATPCreusetPlan_A0();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetPlan_A0();

protected:
	// la fonction pour dessiner l'élément
	JVoid OnDrawItem(CDC & dc, RECT rect);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetPlan_A0)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetPlan_A0)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetPlan_A0_H__INCLUDED_)
