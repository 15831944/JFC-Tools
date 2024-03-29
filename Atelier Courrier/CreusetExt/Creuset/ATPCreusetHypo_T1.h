#if !defined(AFX_ATPCreusetHypo_T1_H__D362DEF7_917C_4420_B6FB_A84BA4EF7853__INCLUDED_)
#define AFX_ATPCreusetHypo_T1_H__D362DEF7_917C_4420_B6FB_A84BA4EF7853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_T1 : Titre en haut du Creuset = Definition des hypotheses

class CATPCreusetHypo_T1 : public JFCTitle
{
public:
	// le constructeur
	CATPCreusetHypo_T1();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_T1();

protected:
	// la fonction pour dessiner l'�l�ment
	JVoid OnDrawItem(CDC & dc, RECT rect);

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_T1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo_T1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_T1_H__D362DEF7_917C_4420_B6FB_A84BA4EF7853__INCLUDED_)
