#if !defined(AFX_ATPCreusetPlan_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
#define AFX_ATPCreusetPlan_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPCreusetPlan_A0.h"
#include "ATPCreusetPlan_A1.h"
#include "ATPCreusetPlan_A2.h"
#include "ATPCreusetPlan_A3.h"
#include "ATPCreusetPlan_B1.h"
#include "ATPCreusetPlan_B2.h"
#include "ATPCreusetPlan_B3.h"
#include "ATPCreusetPlan_C1.h"
#include "ATPCreusetPlan_C2.h"
#include "ATPCreusetPlan_C3.h"

#include "ATPBtnHypothese.h"
#include "ATPBtnPlanMaitre.h"

#include "TextProgressCtrl.h"

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetPlan

class CATPCreusetPlan : public JFCSplitterPane
{
public:
	DECLARE_DYNCREATE(CATPCreusetPlan)
	// le constructeur
	CATPCreusetPlan();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetPlan();

public:
	// la fonction pour configurer le graphe
	JVoid SetGraphe(JFControl & Control);

	// la fonction pour récupérer l'ascenseur horizontal
	JFCScroll & GetHorzScrollBar();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();

	static LONG GetCYMin();
	static LONG GetCYIdeal();

	void Invalidate(BOOL bErase = TRUE );
	void SetColors();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetPlan)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetPlan)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnPlus();
	afx_msg void OnBtnMoins();
	afx_msg void OnBtnT();
	afx_msg void OnBtnPlateau();
	afx_msg void OnBtnPlan();
	afx_msg void OnBtnEffacer();
	afx_msg void OnBtnPlanEtendu();
	afx_msg void OnBtnPlanTemoin();
	afx_msg void OnBtnSelHypo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

private:
	friend class CATPSplitterPlan;
	friend class CATPCreusetPlan_C2;
	friend class CATPCreusetPlan_C3;
	CATPCreusetPlan_A0 m_A0; 
	CATPCreusetPlan_A1 m_A1; 
	CATPCreusetPlan_A2 m_A2; 
	CATPCreusetPlan_A3 m_A3; 
	CATPCreusetPlan_B1 m_B1; 
	CATPCreusetPlan_B2 m_B2; 
	CATPCreusetPlan_B3 m_B3; 
	CATPCreusetPlan_C1 m_C1; 
	CATPCreusetPlan_C2 m_C2; 
	CATPCreusetPlan_C3 m_C3; 

	CButtonST m_D1;				// le btn Plateau D1
	CButtonST m_D2;				// le btn Plan D2
	CButtonST m_D3;				// le btn Effacer D3

	CButtonST m_BtnT;			// le btn T
	CButtonST m_BtnPlus;		// le btn +
	CButtonST m_BtnMoins;		// le btn -
	CButtonST m_BtnAff;			// le btn Aff

	// CButtonST m_BtnPE;			// le btn Plan Etendu
	// CButtonST m_BtnPT;			// le btn Plan Temoin
	CATPBtnPlanMaitre m_BtnPM;	// le btn Plan maitre
	CATPBtnHypothese m_LstHypo; //la liste des hypotheses
	

private:
	JFCScroll m_HS; // le composant HS
	JFCScroll m_VS; // le composant VS

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetPlan_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
