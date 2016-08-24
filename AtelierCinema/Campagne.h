// Campagne.h: interface for the CCampagne class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMPAGNE_H__72B9B3C1_0BA1_11D4_989E_004095420987__INCLUDED_)
#define AFX_CAMPAGNE_H__72B9B3C1_0BA1_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "Selection.h"	// Added by ClassView
#include "tarif.h"	// Added by ClassView
#include "Saisonnalite.h"
#include "Cible.h"	// Added by ClassView
#include "CampagneGenerique.h"
#include "ResultatPerformance.h"

typedef CArray<double,double> CDoubleArray2;

class CCampagne : public CCampagneGenerique
{
public:
	void SetInit();
	void FreeParcPassage(char **** thePassages,int nb);
	void FreeQuotaBacUsedParLigne();
	void AllocBudget();
	void FreeBudget();
	void AllocParc();
	void FreeParc();
	enum {BUDGETPERF=-1,HYPOTHESEFORMATACTIF=-2,HYPOTHESES=-4,HYPOTHESESPOP=-5,TOUT=-6,TOUTHYPOTHESEACTIVE=-7,TOUTHYPOTHESEVALIDE=-8,CIBLEUTILE=-9,MULTICIBLE=-10,TOUTHYPOTHESEACTIVEIFNOTVALIDE=-11,TOUTPOURIMPRESSION=-12};
	int m_PrintPerf;
	int m_PrintPerfAgglo;
	int m_PrintGraph;
	int m_PrintCalendrier;
	int m_Multicible;
	int m_IndexCibleImprime[NBCIBLECALCUL];
	int m_NbCibleImprime;

	// mécanique d'alternance
	void UpdateMap(int effaceformats);

	/*
	int *** m_MapInterdit;
	*/

	/*
	CSelection m_Selection;
	*/

	int m_PrintRequest; //0:Calendrier 1:Graphe 2:Performances 3:Multicible 4:Tout
	void CopierDansPressePapier();
	bool fPressePapier;
	void CollerHypotheseActive();
	void CopierHypotheseActive(int NrHypo);



	//CDlgAttente m_DlgAttente;

	// Pour calculer la SD:
	int m_TabCPM[NBHYPOTHESE];
	double m_TabCouv[NBHYPOTHESE];
	double m_TabMemo[NBHYPOTHESE];
	double m_Mixte[NBHYPOTHESE];
	int m_ClasementMixte[NBHYPOTHESE];
	double m_PoidsEco,m_PoidsCouv,m_PoidsMemo;
	int m_NbTop20;
	int m_RealiseMin;
	int m_RealiseMax;

	
	void CopyHypothese(int HypSource,int HypDestination);
	void Efface(int h);
	void CalculSyntheseDimensionelle();

	// CString LibCible[NBCIBLECALCUL];

	BYTE m_VisuHypothese[NBHYPOTHESE];
	void FreeCoeffFormat();
	void CalculCoeffFormat();

	//CDocument * m_pDoc;

	void FreeMemory();
//	double	NbContactTot[NBCIBLECALCUL][NBHYPOTHESE];
	double	*PQind;
	double	DureeMoy[NBHYPOTHESE][NBSEMAINEMAX];
	double	* NbContactBarre[NBCIBLECALCUL][NBHYPOTHESE][NBSEMAINEMAX];
	double	* NbContactPlaque[NBCIBLECALCUL][NBHYPOTHESE][NBSEMAINEMAX];

	double NbContactsCumulParSemaine[NBCIBLECALCUL][NBHYPOTHESE][NBSEMAINEMAX];


	int		**NbSecInsert[NBHYPOTHESE];//[NBPARCPROBA][NBSEMAINEMAX];
	int		**m_NbPassageSem[NBHYPOTHESE];//[NBPARCPROBA][NBSEMAINEMAX];

	int NrDessin;
	int EtatCalcul[NBHYPOTHESE];
	int fCalculStop[NBHYPOTHESE];
	//double MemoMoy[NBCIBLECALCUL][NBHYPOTHESE];
	bool fZeroBinomeSelected;
	//double MemoNrSemaineX[NBCIBLECALCUL][NBHYPOTHESE];

	int NbChanceTot[NBHYPOTHESE];
	int NbChanceSem[NBHYPOTHESE][NBSEMAINEMAX];
	int NbChanceCum[NBHYPOTHESE][NBSEMAINEMAX];

	//int NumCible[NBCIBLECALCUL];
	//double BasePopulation[NBCIBLECALCUL];
	//int NbCas[NBCIBLECALCUL];

	//double BudgetTot[NBHYPOTHESE];
	//long BudgetNet[NBHYPOTHESE];
	double **Budget; //[hypothese][binome]

	CResultatPerformance m_Resultat;

	// données pour courbe de mémo-démémo
	double MemoTot[NBCIBLECALCUL][NBHYPOTHESE][NBSEMAINECALCULMAX];
	double CouvTot[NBCIBLECALCUL][NBHYPOTHESE][NBSEMAINECALCULMAX];
	double DistTot[NBCIBLECALCUL][NBHYPOTHESE][NBSEMAINECALCULMAX];
	//double PtMemoCampTot[NBCIBLECALCUL][NBHYPOTHESE]; // Points de mémo, pour les semaine "actives"
	//double	NbIndivTouchesTot[NBCIBLECALCUL][NBHYPOTHESE]; // Nombre d'individus ayant été touché au moins une fois, en millier
	//double	NbIndivMemoTot[NBCIBLECALCUL][NBHYPOTHESE]; // Nombre d'individus  ayant mémorisé au moins une fois, en millier

	class MODELEFORMAT
	{
		public:
		class MODELEFORMAT *Next;
		char FlagFormat[NBFORMATMAX]; // [NbFormat de l'étude]
		float Beta[NBCLASSE];
	}*ListeModeleFormat;

	
	class INDEX
	{
	public:
		// tout par NbSemaineCalcul
		char *s;
		char *z;
		char *b;
		char *v;
		char *w;
		char *p;

		// Lien au modèle format
		MODELEFORMAT ***ModeleFormat; // [NbSemaineCalcul][Nrbloc] tableau de pointeur vers la liste chainée dynamique
	} Index[NBHYPOTHESE];	

	// CALCUL FRED
	// Les cibles	
	VECTCIBLE Cible[NBCIBLECALCUL];

	// les vecteurs cibles pour le calcul base régionale 
	VECTCIBLE CibleUtile[NBCIBLECALCUL];

	// Préparation des vecteurs
	void PrepareVecteurIndividus(int cible);

	// Calcul de la population des cibles utiles depuis les vecteurs
	void CalculPopCibleUtile(int cible);

	void SetExercice(COleDateTime DateDebut,int NbSemaine,bool glisse=0);

	//void MajFiltreVisu();

	/*
	CWnd * m_Btn[4];
	*/

	CWnd * m_Perf;
	CWnd * m_PerfAgglo;
// MODIF AURELIEN : SUPPR PERF2
//	CWnd * m_Perf2;
	CWnd * m_Graphe;

	void CalculPassageACCOGRIL();
	void MajAffichagePerf();
	CCampagne();
	virtual ~CCampagne();



	short CalculCouvertures(int H,int iCib);
	void CalculTout();
	void CalculToutHypothese(int H);
	void CalculBasePopulation();
	void CalculToutBudget();
	void CalculToutBudgetHypothese(int H);

	void CalculTotalBudget(int H);
	void CalculBudgetLigne(int H,int B);
	virtual void LanceCalcul(int bDebut);
	void CalculToutMemo();
	void CalculToutMemoHypothese(int H);
	void CalculMemoNrSemaineX(int h);
	void CalculTotalMemo(int H);
	void CalculMemo(int H);
	char Borne(char *s,int deb,int max);
	double POWTEST(double x,double y);
	void CalculCourbeMemoDememo(int iCib,int Hypo);
	float BetaSuivantModele(float Beta0,float Duree);
	MODELEFORMAT * TouveBetaF(int Hypo,int Parcsp,int Sem);
	void LibererListeModelesBetaF(MODELEFORMAT *Mod);
	void MiseAJourIndex(int Hypo);
	int ChargeCible(int NrCible,int iCib);


	// Les hypotheses pour résultats
	int m_HypotheseActive;
	int m_HypotheseValide;

	// Test si semaine active
	bool SemaineActive(int NoSemaine);

	/*
	// les accelerateurs graphiques
	HDC hDCspeed;
	HBITMAP hBmpNewspeed,hBmpOldspeed;
	*/

	/*
	// définition physique
	int NbColone;
	int LargColone;
	int NbLigne;
	int HautLigne;
	int HautLigneTitre;
	*/

	// ascenceurs
	HWND m_hAscH;
	int m_PosAscH;
	int m_PosAscHmax;

	HWND m_hAscV;
	int m_PosAscV;
	int m_PosAscVmax;


	//*** les données importées à l'objet ***
	/*
	char SetEnCours;
	*/

	/* info période
	COleDateTime DateDebut; // codé 010180
	int NbSemaine;
	int NbSemaineCalcul;
	*/


	// Les formats
	int m_NbFormat;
	CStringArray m_LibFormat;
	CByteArray m_FlagFormat; // état des formats: 1Select 2:Visu

	int OutilsActif; // voir palette outils

	//*** les données privées de l'objet ***
	//	int NbParcTotal;
	char  **** m_Passage;			// par[hypothese][Parc][semaine][Format]
	int     ** m_NbPassage;			// par [hypothese][parc]
	double  ** m_NbPassageMoyen;	// [hypothese][parc]



	char m_PassageTempo[NBFORMATMAX];

	// le bloc de selection
	char fBloc;
	int CaseS,CaseP;
	RECT RDebut;
	RECT RBloc;

	/*
	// en deplacement de bloc (fBloc=2)
	int OffsetX,OffsetY,NbS,NbP,CoinDebutX,CoinDebutY;
	*/
	int m_NbP;
	int m_NbS;
	int m_OffsetX,m_OffsetY;
	int m_CoinDebutX,m_CoinDebutY;

	char ***m_BlocPass; // par[NbP][NbS][NbFormat]

	int m_SDebut,m_PDebut;
		


	// la sélection des binomes
	//CByteArray SelectBinome; // par [NbBinôme]


	// le filtre des binomes affichés
//	int NbBinomeVisu;
//	CWordArray NrBinomeVisu; // par [NbBinomeVisu]
//	int NbParcVisu;
//	CWordArray NrParcVisu;

	// les formats
	//int NbFormat;
	//char **LibFormats; // *NbFormats
	double **CoeffFormat; // [Format][Semaine]    voir si [Binome] (plus tard)
	int *Impacts; // id

	/*// Tarifs
	CTarif m_Tarif;
	*/

	// Saisonnalité
	CSaisonnalite m_Saison;

	int NrTempoActif;

	BOOL m_flagBinomeAgregat;
	CString m_NomProduit;
	CString m_NomCentrale;
	CString m_NomAnnonceur;
	// CString m_NomCampagne;


public:
	CByteArray m_ListeBacUtilise;
	CWordArray * m_ListeComplexeParBAC;
	CWordArray * m_ListeCpxParLigne;
	CByteArray * m_MapCpxUsedParLigne; // Peut utiliser un CBitArray
	CDoubleArray2 ** m_QuotaBacUsedParLigne; // [Ligne][Bac][Semaine]

	inline void GetCodeArrayFromSelection(int ligne,int semaine,CStringArray & CodeBAC,CWordArray & IndexCpx);

private:
	// CWordArray * m_ListeCpxParLigne;
	// CByteArray * m_MapCpxUsedParLigne; // Peut utiliser un CBitArray
	// CDoubleArray2 ** m_QuotaBacUsedParLigne; // [Ligne][Bac][Semaine]
	// CWordArray * m_ListeComplexeParBAC;
	// CByteArray m_ListeBacUtilise;
	
	CByteArray * m_MapParcAtcif; // Nombre de secondes par [parc][semaine]
	CByteArray m_SemaineActive; // Semaine active lorsque 1 passage actif
	// inline void GetCodeArrayFromSelection(int ligne,int semaine,CStringArray & CodeBAC,CWordArray & IndexCpx);
	inline bool FindProbabilite(int Hyp,int NrIndividu, int Semaine, WORD &p1, WORD &p2, double & coeffCorrection,BYTE & NbSeconde);
	inline void PrepareBacParLigne();
	inline void PrepareParcActifs(int Hypothese);

	int m_init;
};

/*
typedef struct tagETUDEEXPORT {
	char * CheminData;
	char * CheminUnivers;
	char * CheminZone;
	char * CheminSource;

	struct TABLEUNIVERS Noyau; // le noyau d'origine
	struct TABLEUNIVERS Univers; // le noyau réduit

	int Numero;
	char Nom[LIBELLEMAX+1];
	int DateDebut_010180; // INUTILISE
	int DateFin_010180; // INUTILISE

	// la période d'action ???
	int DateDebutPA; // codé en nb de j. depuis 010180 >0 et <=52
	int NbSemainePA; // >0

	// les options
	struct ETUDEOPTION Option;  // INUTILISE

	// la date de la structure...
	short NrStructureValide;
	unsigned int DateValide;

	// les formats
	int NbFormat;
	char **LibFormats; // *NbFormats
	int **Coefs; // [Semaine][Format]    voir si [Binome] (plus tard)
	int *Impacts; // id

	// Les tarifs
	long ****Tarifs; // [Region][Binome][Parc][Semaine] (dans l'ordre de l'univers)

	// Saisonnalité
	int ***Saisons; // [Region][Binome][Semaine]

	// Les cibles==== A VIRER
	int NbCibles; // A condition qu'elles soient valides sur toutes les régions
	int *NumCibles;
	char **NomCibles;
	int *NbDateAbaque; //nombre de dates differentes toutes binomes confondus
	int **DateAbaque;  //
	// Les cibles==== A VIRER

	int *Vnrosemaine;
	int *Vnromois;
	int *Vnroannee;
} ETUDEEXPORT;
*/


#endif // !defined(AFX_CAMPAGNE_H__72B9B3C1_0BA1_11D4_989E_004095420987__INCLUDED_)
