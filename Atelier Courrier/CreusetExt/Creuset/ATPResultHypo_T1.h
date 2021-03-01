#if !defined(AFX_ATPResultHypo_T1_H__D362DEF7_917C_4420_B6FB_A84BA4EF7853__INCLUDED_)
#define AFX_ATPResultHypo_T1_H__D362DEF7_917C_4420_B6FB_A84BA4EF7853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPResultHypo_T1

class CATPResultHypo_T1 : public JFCTitle
{
public:
	// le constructeur
	CATPResultHypo_T1();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPResultHypo_T1();

protected:
	// la fonction pour dessiner l'élément
	JVoid OnDrawItem(CDC & dc, RECT rect);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPResultHypo_T1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPResultHypo_T1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPResultHypo_T1_H__D362DEF7_917C_4420_B6FB_A84BA4EF7853__INCLUDED_)
