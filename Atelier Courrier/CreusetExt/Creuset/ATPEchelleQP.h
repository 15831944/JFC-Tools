#if !defined(AFX_ATPECHELLEQP_H)
#define AFX_ATPECHELLEQP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPEchelle_T1.h"
#include "ATPEchelle_A1.h"
#include "ATPEchelle_A2.h"
#include "ATPEchelle_B1.h"
#include "ATPEchelle_B2.h"
#include "ATPEchelle_C1.h"
#include "ATPEchelle_C2.h"
#include "ATPEchelleQP_D1.h"
#include "ATPEchelleQP_D2.h"
#include "ATPEchelle_F1.h"
#include "ATPEchelle_F2.h"

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CATPEchelle

class CATPEchelleQP : public JFCSplitterPane
{
public:
	DECLARE_DYNCREATE(CATPEchelleQP)
	// le constructeur
	CATPEchelleQP();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPEchelleQP();

	void SetColors();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPEchelleQP)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPEchelleQP)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnTous();
	afx_msg void OnBtnAucun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

private:
	CATPEchelle_T1 m_T1; // le composant T1
	CATPEchelle_A1 m_A1; // le composant A1
	CATPEchelle_A2 m_A2; // le composant A2
	CATPEchelle_B1 m_B1; // le composant B1
	CATPEchelle_B2 m_B2; // le composant B2
	//CATPEchelle_C1 m_C1; // le composant C1
	//CATPEchelle_C2 m_C2; // le composant C2
	CATPEchelleQP_D1 m_D1; // le composant D1
	CATPEchelleQP_D2 m_D2; // le composant D2
	CButtonST m_E1; // le composant E2
	CButtonST m_E2; // le composant E2
	CATPEchelle_F1 m_F1; // le composant F1
	CATPEchelle_F2 m_F2; // le composant F2

private:
	JFCScroll m_HS; // le composant HS
	JFCScroll m_VS; // le composant VS

	// Nombre d'insertion pour un supprot du plateau
	JInt32	OnGetNbInsertions(JUnt32 IdSupport) const;

	// la fonction pour récupérer le nombre d'insertions d'un Support sur la période d'action uniquement
	JInt32  OnGetNbInsertionsPA(JUnt32 IdSupport) const;

	// la fonction pour récupérer le nombre d'insertions d'un Support sur toute l'étude
	JInt32  OnGetNbInsertionsEtude(JUnt32 IdSupport) const;

	// Suppression de toutes les insertions d'un support
	JVoid	SuppInsertionSupport(JUnt32 IdxSupport);

	
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPECHELLEQP_H)
