#if !defined(AFX_ATPGraphCouvJour_H__862CA4E1_ECF7_4BD6_B7C0_566C75EDB94F__INCLUDED_)
#define AFX_ATPGraphCouvJour_H__862CA4E1_ECF7_4BD6_B7C0_566C75EDB94F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPGraphGraduation.h"
#include "ATPGraphCouvJour_B1.h"
#include "JFCFlatPane.h"
#include "AbscisseGrph.h"
#include "JFCTruc.h"

/////////////////////////////////////////////////////////////////////////////
// CATPGraphCouvJour

class CATPGraphCouvJour : public JFCFlatPane
{
public:
	// le constructeur
	CATPGraphCouvJour();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPGraphCouvJour();

	void IncrMax();
	void DecrMax();
	void SetAuto();

public:
	// la configuration pour configurer l'ascenseur
	JFControl & SetHorzScrollBar(JFCScroll & Scroll);

	// la fonction pour récupérer le controle des abscisses
	JFControl & GetAbcsisseControl();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPGraphCouvJour)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPGraphCouvJour)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CString GetLabel();

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

private:
	friend class CATPSplitterPlan;
	CATPGraphGraduation m_A1; // le composant A1
	CATPGraphCouvJour_B1 m_B1; // le composant B1
	CAbscisseGrph	m_A2;
	JFCTruc m_btnPrint;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphCouvJour_H__862CA4E1_ECF7_4BD6_B7C0_566C75EDB94F__INCLUDED_)
