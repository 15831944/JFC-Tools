#if !defined(AFX_ATPCreusetHypo_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
#define AFX_ATPCreusetHypo_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPCreusetHypo_T1.h"
#include "ATPCreusetHypo_A1.h"
#include "ATPCreusetHypo_A2.h"
#include "ATPCreusetHypo_A3.h"
//#include "ATPCreusetHypo_B1.h"
//#include "ATPCreusetHypo_B2.h"
//#include "ATPCreusetHypo_B3.h"
#include "ATPCreusetHypo_C1.h"
#include "ATPCreusetHypo_C2.h"
#include "ATPCreusetHypo_C3.h"

#include "BtnST.h"

#include "ATPCreusetHypo_E1.h"
#include "ATPCreusetHypo_E2.h"
#include "ATPCreusetHypo_E3.h"

//////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo

class CATPCreusetHypo : public JFCSplitterPane
{
public:
	DECLARE_DYNCREATE(CATPCreusetHypo)
	// le constructeur
	CATPCreusetHypo();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo();

public:
	// la fonction pour configurer le graphe
	JVoid SetGraphe(JFControl & Control);

	// la fonction pour récupérer l'ascenseur horizontal
	JFCScroll & GetHorzScrollBar();

	void SetColors();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();
	static LONG GetCYIdeal();

	JUnt32	m_NoLigSupportActif;	// dernier support activé
	JUnt32	m_IdSupportActif;		// dernier id support actif

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnEffacer();
	afx_msg void OnBtnEffacerToute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	JATPDocCreuset* m_pATPDocCreuset; // le document

	CATPCreusetHypo_T1 m_T1;	// Titre grille insertion plan global	
	CATPCreusetHypo_A1 m_A1;	// Entete Support			/ colonne 1
	CATPCreusetHypo_A2 m_A2;	// Bloc supports			/ colonne 1
	CATPCreusetHypo_A3 m_A3;	// Entete Nb Insertions		/ colonne 1 
	//CATPCreusetHypo_B1 m_B1;	// Entete µ					/ colonne 2
	//CATPCreusetHypo_B2 m_B2;	// Bloc µ supports			/ colonne 2
	//CATPCreusetHypo_B3 m_B3;	// Bloc vide				/ colonne 2
	CATPCreusetHypo_C1 m_C1;	// Entete N° Hypo			/ colonne 3
	CATPCreusetHypo_C2 * m_C2;	// Bloc insertions			/ colonne 3
	CATPCreusetHypo_C3 m_C3;	// Bloc Tot insert par hypo / colonne 3

	// CATPCreusetHypo_D1 m_D1;	// Bouton Offre
	// CATPCreusetHypo_D2 m_D2;	// Bouton Plateau

	CButtonST m_D4;
	CButtonST m_D3;
	//CATPCreusetHypo_D4 m_D4;	// Bouton effacer hypothese active
	//CATPCreusetHypo_D3 m_D3;	// Bouton effacer insertions formats actifs

	CATPCreusetHypo_E1 m_E1;	// Entete N° Hypo			/ colonne 3
	CATPCreusetHypo_E2 * m_E2;	// Bloc insertions			/ colonne 3
	CATPCreusetHypo_E3 m_E3;	// Bloc Tot insert par hypo / colonne 3

	JFCScroll m_HS; // le composant HS
	JFCScroll m_VS; // le composant VS


};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
