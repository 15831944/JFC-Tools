#pragma once

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPPupitre_A1.h"
//#include "ATPPupitre_A2.h"
#include "ATPPupitre_A3.h"
//#include "ATPPupitre_A4.h"
#include "ATPPupitre_A5.h"
#include "ATPPupitre_A6.h"
#include "ATPPupitre_A7.h"
#include "ATPPupitre_A8.h"
#include "ATPPupitre_A9.h"
//#include "ATPPupitre_A10.h"
//#include "ATPPupitre_A11.h"

// A FAIRE (système liste comme partout)
#include "ATPListePupitre_A2.h"
#include "ATPListePupitre_A4.h"

/////////////////////////////////////////////////////////////////////////////
// CATPPupitre
#define IDC_CR_LIST_TERRAINPUPITRE	1
#define IDC_CR_LIST_CIBLEPUPITRE	2

class CATPPupitre : public JFCSplitterPane
{
public:
	DECLARE_DYNCREATE(CATPPupitre)
	// le constructeur
	CATPPupitre();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	virtual void SetColors();

	// le destructeur
	virtual ~CATPPupitre();

public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPPupitre)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPupitre)
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
	// CATPPupitre_A2 m_A2; 
	CATPListePupitre_A2 m_ListTerrains; // JFCSelListBox m_ListTerrains;

	// Titre Cible
	CATPPupitre_A3 m_A3; 

	// Object selection cible
	// CATPPupitre_A4 m_A4; 
	CATPListePupitre_A4 m_ListCibles;

	// Titre Format
	CATPPupitre_A5 m_A5; 

	// Object Selection Format
	CATPPupitre_A6 m_A6; 

	// Object Selection Tous Formats
	CATPPupitre_A7 m_A7; 

	// Objects Ascenseur Format
	CATPPupitre_A8 m_A8; 

	// Object Selection Format
	CATPPupitre_A9 m_A9; 

	// Titre RefAffinité
//	CATPPupitre_A10 m_A10; 

	// Object RefAffinité
//	CATPPupitre_A11 m_A11; 
	// JFCSelListBox m_ListRefAff;

	// Font spéciale liste
	CFont m_FontCreuset;
};

