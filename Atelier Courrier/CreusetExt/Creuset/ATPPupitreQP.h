#pragma once

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPPupitre_A1.h"
#include "ATPPupitre_A3.h"

#include "ATPListePupitre_A2.h"
#include "ATPListePupitre_A4.h"

/////////////////////////////////////////////////////////////////////////////
// CATPPupitreQP
#define IDC_CR_LIST_TERRAINPUPITRE	1
#define IDC_CR_LIST_CIBLEPUPITRE	2

class CATPPupitreQP : public JFCSplitterPane
{
public:
	DECLARE_DYNCREATE(CATPPupitreQP)
	// le constructeur
	CATPPupitreQP();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	virtual void SetColors();

	// le destructeur
	virtual ~CATPPupitreQP();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPPupitreQP)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPupitreQP)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelChangeListTerrain();
	afx_msg void OnSelChangeListCible();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

private:
	// Titre Terrain
	CATPPupitre_A1 m_A1; 

	// Object Selection Terrain
	CATPListePupitre_A2 m_ListTerrains; 

	// Titre Cible
	CATPPupitre_A3 m_A3; 

	// Object selection cible
	// CATPPupitre_A4 m_A4; 
	CATPListePupitre_A4 m_ListCibles;

	// Font spéciale liste
	CFont m_FontCreuset;
};

