// CampagneFilm.h: interface for the CCampagneFilm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMPAGNEFILM_H__96AFB761_41AD_4D3C_ADCB_8B7A471ACDDC__INCLUDED_)
#define AFX_CAMPAGNEFILM_H__96AFB761_41AD_4D3C_ADCB_8B7A471ACDDC__INCLUDED_

#include "ProduitFilm.h"
#include "CampagneGenerique.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCampagneFilm : public CCampagneGenerique
{
public:
	CCampagneFilm();
	virtual ~CCampagneFilm();

	// Les calculs
	void LanceCalculPerformances();

	// table des produits films
	CTableProduitFilm m_TabProduitFilm;

	// Récupère infos période
	void SetExercice(COleDateTime datedebut, int nbsemaine);

	// Informations options da calcul // Exemple
	enum {OPTIONCALC1 = 1, OPTIONCALC2 = 2};

	// Pour les impresions
	int m_PrintPerfFilm;
	int m_PrintGraphFilm;

	//analyse Film
	CWnd * m_PerfFilm;
	CWnd * m_GrapheFilm;


	// Maj Grille Visu Film
	bool m_UpdateFilm;
	bool m_UpdateSelectFilm;

	// Pour la visu dans performances: copie des données de classe calcul dans campagnefilm
	double m_Population;
	double m_BudgetBrut;
	long m_BudgetNet;

	// tickets
	double m_NbContacts;

	// Couverture
	double m_Couverture;

	// Repetition
	double m_Repetition;

	// GRP
	double m_GRP;

	// CPM brut
	double m_CPMbrut;

	// CPM net
	double m_CPMnet;

	double m_NbIndiv;

	// Mémorisation moyenne
	double m_MemoMoy;

	// ratio de mémo
	double m_RatioMemo;

	// données pour courbe de mémo-démémo
	double MemoTot[NBSEMAINECALCULMAX];
	double CouvTot[NBSEMAINECALCULMAX];
	double DistTot[NBSEMAINECALCULMAX];
	double PtMemoCampTot; // Points de mémo, pour les semaine "actives"
	double	NbIndivTouchesTot; // Nombre d'individus ayant été touché au moins une fois, en millier
	double	NbIndivMemoTot;	// Nombre d'individus  ayant mémorisé au moins une fois, en millier

};

#endif // !defined(AFX_CAMPAGNEFILM_H__96AFB761_41AD_4D3C_ADCB_8B7A471ACDDC__INCLUDED_)
