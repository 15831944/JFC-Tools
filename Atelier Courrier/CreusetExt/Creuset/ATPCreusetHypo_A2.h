#if !defined(AFX_ATPCreusetHypo_A2_H__7A272741_4CCA_4016_A173_76917179B918__INCLUDED_)
#define AFX_ATPCreusetHypo_A2_H__7A272741_4CCA_4016_A173_76917179B918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
class CATPCreusetHypo;

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_A2

class CATPCreusetHypo_A2 : public JFCGrid
{
public:
	// le constructeur
	CATPCreusetHypo_A2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	virtual JVoid SetParent(CATPCreusetHypo* pATPCreusetHypo);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_A2();
	
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
	//{{AFX_VIRTUAL(CATPCreusetHypo_A2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo_A2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	CATPCreusetHypo* m_pATPCreusetHypo; // le parent
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_A2_H__7A272741_4CCA_4016_A173_76917179B918__INCLUDED_)
