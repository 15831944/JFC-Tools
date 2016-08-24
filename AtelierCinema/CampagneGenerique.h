// CampagneGenerique.h: interface for the CCampagneGenerique class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMPAGNEGENERIQUE_H__C942E037_6244_4AE3_8C06_066BDD5D1F25__INCLUDED_)
#define AFX_CAMPAGNEGENERIQUE_H__C942E037_6244_4AE3_8C06_066BDD5D1F25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Selection.h"
#include "tarif.h"	
	

// Définition constantes générales
#define NBCLASSE 20
#define NBHYPOTHESE 5
#define NBFORMATMAX 5
#define NBCIBLECALCUL (20)
#define NBSEMAINEMAX 106// 53
#define NBSEMAINECALCULMAX (NBSEMAINEMAX+52)
#define NBUBINOMEMAX 2
#define NBFOISTOUCHEMAX (20)
#define BASEFORMATMAXI (120.0)
#define BASEFORMATREF (30)
#define M_PI (3.1415926535897932384626433832795)


class CCampagneGenerique  
{
public:
	CCampagneGenerique();
	virtual ~CCampagneGenerique();

	// Sélection des semaines actives pour mémo-démémo
	void MajInfoCalendACCOGRIL();
	void BindVecteurCalendrier();

	// Tableau des cibles
	int m_NumCible[NBCIBLECALCUL];
	CString m_LibCible[NBCIBLECALCUL];

	// Document associé
	CDocument * m_pDoc;

	// Nom de la campagne
	CString m_NomCampagne;

	// Nom de la vague
	CString m_NomVagueCampagne;

	// Infos période
	COleDateTime m_DateDebut;
	int m_NbSemaine;
	int m_NbSemaineCalcul;

	// Beta
	int m_Beta;

	// Sélection en cours
	CSelection m_Selection;

	// Tarifs
	CTarif m_Tarif;

	// Map alternac
	int *** m_MapInterdit;

	// définition physique
	int m_NbColone;
	int m_LargColone;
	int m_NbLigne;
	int m_HautLigne;
	int m_HautLigneTitre;

	// les accelerateurs graphiques
	HDC m_hDCspeed;
	HBITMAP m_hBmpNewspeed,m_hBmpOldspeed;

	// Les boites résultats
	CWnd * m_Btn[4];

	// flag pour le calcul mémo-démémo
	int CalculDememo; 

	// Les calculs
	//virtual void LanceCalcul(int bDebut);


	// variables spécifiques (en vrac)

	// en deplacement de bloc (fBloc=2)
	// int m_OffsetX,m_OffsetY,m_NbS,m_NbP,m_CoinDebutX,m_CoinDebutY;
	// int m_CoinDebutX,m_CoinDebutY;


	// int m_SDebut,m_PDebut;
	
	//*** les données importées à l'objet ***
	char m_SetEnCours;


	// options de calcul
	int CalculSaisonnalite; 
	int BaseCalcul; // 0:Régional 2:National AVIRER
	int fPiloteDurees;// 0:Durées actives 1:Toutes
	int RedressementCNC;
	int NumeroModaliteAgglo;


	// sélection dans le calendrier
	int m_DebutCalend;
	int m_FinCalend;
	int m_NrSemaineX;
	int m_NbSemaineCalend;
	CByteArray SelectCalend; // par NbSemaine;
	char fCurCalend;
	int PosXCurCalend;

	// dessin visuel du calendrier
	// par [NbSemaine]
	CWordArray m_NrSemaine;
	CWordArray m_NrMois;
	CWordArray m_NrAnnee;

	void CalculCourbeReponse();
	double ValBeta[NBCLASSE];	//Courbe de réponse



};

#endif // !defined(AFX_CAMPAGNEGENERIQUE_H__C942E037_6244_4AE3_8C06_066BDD5D1F25__INCLUDED_)
