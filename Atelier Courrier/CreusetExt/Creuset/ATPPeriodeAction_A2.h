#if !defined(ATPPERIODEACTION_A2_H)
#define ATPPERIODEACTION_A2_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCWave.h"

/////////////////////////////////////////////////////////////////////////////
// CATPPeriodeAction_A2

class CATPPeriodeAction_A2 : public JFCWave
{
public:
	// le constructeur
	CATPPeriodeAction_A2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	JVoid SetHypo(JBool bHypo);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPPeriodeAction_A2();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();
	virtual LONG OnGetItemState(LONG indice, JDate date);

protected:
	//{{AFX_VIRTUAL(CATPPeriodeAction_A2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPeriodeAction_A2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// les classes amies
	friend class CATPPeriodeAction;

	JATPDocCreuset* m_pATPDocCreuset; // le document
	JBool m_bHypo;

	CATPPeriodeAction * m_parent; //la fenetre parente

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ATPPERIODEACTION_A2_H)
