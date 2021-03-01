#if !defined(AFX_ATPCreusetHypo_A3_H__38608BC1_0503_4A21_8735_2FF5C249EBBC__INCLUDED_)
#define AFX_ATPCreusetHypo_A3_H__38608BC1_0503_4A21_8735_2FF5C249EBBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_A3

class CATPCreusetHypo_A3 : public JFCTitle
{
public:
	// le constructeur
	CATPCreusetHypo_A3();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_A3();
	
protected:
	// la fonction pour dessiner l'élément
	JVoid OnDrawItem(CDC & dc, RECT rect);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_A3)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo_A3)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_A3_H__38608BC1_0503_4A21_8735_2FF5C249EBBC__INCLUDED_)
