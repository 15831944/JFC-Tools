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

	// R�cup�re infos p�riode
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

	// Pour la visu dans performances: copie des donn�es de classe calcul dans campagnefilm
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

	// M�morisation moyenne
	double m_MemoMoy;

	// ratio de m�mo
	double m_RatioMemo;

	// donn�es pour courbe de m�mo-d�m�mo
	double MemoTot[NBSEMAINECALCULMAX];
	double CouvTot[NBSEMAINECALCULMAX];
	double DistTot[NBSEMAINECALCULMAX];
	double PtMemoCampTot; // Points de m�mo, pour les semaine "actives"
	double	NbIndivTouchesTot; // Nombre d'individus ayant �t� touch� au moins une fois, en millier
	double	NbIndivMemoTot;	// Nombre d'individus  ayant m�moris� au moins une fois, en millier

};

#endif // !defined(AFX_CAMPAGNEFILM_H__96AFB761_41AD_4D3C_ADCB_8B7A471ACDDC__INCLUDED_)
