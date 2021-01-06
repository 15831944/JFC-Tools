#if !defined(AFX_ATPResultHypo_H__D92F785C_31AE_43E0_8174_FCE190CBD0BF__INCLUDED_)
#define AFX_ATPResultHypo_H__D92F785C_31AE_43E0_8174_FCE190CBD0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPResultHypo_T1.h"
#include "ATPResultHypo_A1.h"
#include "ATPResultHypo_A2.h"
#include "ATPResultHypo_B1.h"
#include "ATPResultHypo_B2.h"
#include "ATPResultHypo_C1.h"
#include "ATPResultHypo_C2.h"

#include "TextProgressCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CATPResultHypo

class CATPResultHypo : public JFCSplitterPane
{
public:
	DECLARE_DYNCREATE(CATPResultHypo)
	// le constructeur
	CATPResultHypo();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// Set Horizontal Scroll
	virtual JFControl &  SetHorzScrollBar1(JFCScroll & HScroll);
	virtual JFControl &  SetHorzScrollBar2(JFCScroll & HScroll);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	void SetColors();

	// le destructeur
	virtual ~CATPResultHypo();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPResultHypo)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPResultHypo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	JATPDocCreuset* m_pATPDocCreuset; // le document

protected:
	CATPResultHypo_T1 m_T1; // le composant T1
	CATPResultHypo_A1 m_A1; // le composant A1
	CATPResultHypo_A2 m_A2; // le composant A2
	CATPResultHypo_B1 m_B1; // le composant B1
	CATPResultHypo_B2 m_B2; // le composant B2
	CATPResultHypo_C1 m_C1; // le composant C1
	CATPResultHypo_C2 m_C2; // le composant C2

protected:
	JFCScroll m_VS; // le composant VS

friend class CATPSplitterHypo;
	CTextProgressCtrl m_progress;

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPResultHypo_H__D92F785C_31AE_43E0_8174_FCE190CBD0BF__INCLUDED_)
