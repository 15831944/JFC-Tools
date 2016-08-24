// CampagneFilm.cpp: implementation of the CCampagneFilm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "CampagneFilm.h"
#include "Cible.h"	
#include "DlgPerformanceFilm.h"
#include "DlgGrapheFilm.h"
#include "CalculProduitFilm.h"

extern CCinemaApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCampagneFilm::CCampagneFilm()
{
	// Initialise le  tableau des produits films
	m_TabProduitFilm.InitTabProduitFilm(); 

	// Par défaut pas de mise à jour film
	m_UpdateFilm = false;
	m_UpdateSelectFilm = false;

	m_Population=0;
	m_BudgetBrut=0;
	m_BudgetNet=0;
	m_NbSemaineCalcul=0;
	m_NbContacts=0;

	m_Couverture=0;

	m_Repetition=0;

	// GRP
	m_GRP=0;

	// CPM brut
	m_CPMbrut=0;

	// CPM net
	m_CPMnet=0;

	m_NbIndiv=0;

	// Mémorisation moyenne
	m_MemoMoy=0;

	// ratio de mémo
	m_RatioMemo=0;
	for(int sem=0;sem<NBSEMAINECALCULMAX;sem++)
	{
		// données pour courbe de mémo-démémo
		MemoTot[sem]=0;
		CouvTot[sem]=0;
		DistTot[sem]=0;
	}
	PtMemoCampTot=0; 
	NbIndivTouchesTot=0; 
	NbIndivMemoTot=0;	
}

CCampagneFilm::~CCampagneFilm()
{
}

// Récup uniquement Date début et Nb Semaines
void CCampagneFilm::SetExercice(COleDateTime datedebut, int nbsemaine)
{
	// Infos période
	m_DateDebut		  = datedebut;
	m_NbSemaine		  = nbsemaine;
	m_NbSemaineCalcul = NBSEMAINECALCULMAX;

	m_DebutCalend		= 0;
	m_FinCalend		    = m_NbSemaine-1;
	m_NrSemaineX	    = m_NbSemaine-2;

	if(m_NrSemaineX<=0 && m_NbSemaine>=1) m_NrSemaineX=0;
	BindVecteurCalendrier();

	// initialisation du tarif
	m_Tarif.InitialiseExercice(datedebut, nbsemaine);

}

//////////////////////////////////////////////////////////////////////////////////////////////
// lancement des calculs
// 
void CCampagneFilm::LanceCalculPerformances()
{
	m_SetEnCours=1;

	CCalculProduitFilm * calcul = new CCalculProduitFilm(this);


	// Récupération des résultats de la mémo-démémo
	calcul->CalculPerformances(); 

	// Remise a jour des variables de campagne film a partir du nouveau résultat
	// Une cible de calcul, une hypothèse
	m_Population=calcul->m_ResultatPermormances.m_Population;
	m_BudgetBrut=calcul->m_ResultatPermormances.m_BudgetBrut;
	m_BudgetNet=calcul->m_ResultatPermormances.m_BudgetNet;

	// tickets
	m_NbContacts=calcul->m_ResultatPermormances.m_NbContacts;
	m_NbIndiv=calcul->m_ResultatPermormances.m_NbIndiv;

	// Couverture
	m_Couverture=calcul->m_ResultatPermormances.m_Couverture();

	// Repetition
	m_Repetition=calcul->m_ResultatPermormances.m_Repetition();

	// GRP
	m_GRP=calcul->m_ResultatPermormances.GRP();

	// CPM brut
	m_CPMbrut=calcul->m_ResultatPermormances.CPMbrut();

	// CPM net
	m_CPMnet=calcul->m_ResultatPermormances.CPMnet();

	// mémo moyenne
	m_MemoMoy=calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_MemoMoy;			

	// ratio de mémo
	m_RatioMemo=calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_RationMemo();
	m_NbSemaineCalcul=calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_NbSemaine;
		
	for(int sem=0;sem<calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_NbSemaine;sem++)
	{
		CouvTot[sem] = calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_CouvTot[sem]; 
		MemoTot[sem] = calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_MemoTot[sem]; 
		DistTot[sem] = calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_DistTot[sem]; 
	}
	NbIndivMemoTot = calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_NbIndivMemoTot; 
	NbIndivTouchesTot = calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_NbIndivTouchesTot; 
	PtMemoCampTot = calcul->m_ResultatPermormances.m_pResultatMemoDememo.m_PtMemoCampTot; 

	delete 	calcul;
	m_SetEnCours=0;

	
}

