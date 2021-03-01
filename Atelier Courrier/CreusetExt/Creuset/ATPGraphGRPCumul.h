#if !defined(AFX_ATPGraphGRPCumul_H__3B3DA0CA_5C09_4F33_9257_0BC9910B9701__INCLUDED_)
#define AFX_ATPGraphGRPCumul_H__3B3DA0CA_5C09_4F33_9257_0BC9910B9701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPGraphGraduation.h"
#include "ATPGraphGRPCumul_B1.h"

#include "JFCFlatPane.h"
#include "AbscisseGrph.h"
#include "JFCTruc.h"

/////////////////////////////////////////////////////////////////////////////
// CATPGraphGRPCumul

class CATPGraphGRPCumul : public JFCFlatPane
{
public:
	// le constructeur
	CATPGraphGRPCumul();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPGraphGRPCumul();
	
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
	//{{AFX_VIRTUAL(CATPGraphGRPCumul)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPGraphGRPCumul)
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
	CATPGraphGRPCumul_B1 m_B1; // le composant B1
	CAbscisseGrph	m_A2;
	JFCTruc m_btnPrint;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphGRPCumul_H__3B3DA0CA_5C09_4F33_9257_0BC9910B9701__INCLUDED_)
