/*------------------------------------------------------------------------
 Module:        C:\TOMEVAL\TOMSTRU.H
 Author:        remi
 Projet:        tomeval
 State:         OK
 Creation Date: 9/9/97
 Description:   Structures
------------------------------------------------------------------------*/
// option globale exclusive
//#define OPTION_CHECKUP
//#define OPTION_CARAT
//#define OPTION_JFC
//#define OPTION_MEDIATOP
//#define OPTION_MEDIACONSULT
//#define OPTION_ZOOMRADIOTELE
//#define OPTION_TOPCHRONO
//#define OPTION_POLOGNE	Ajouter les spots de proba null

// pour faire de la mise au point
//#define CALCULSANSMEDIAWEIGHT

// pour calculer les contacts quand on n'a pas la population
// de la source de GRP
//#define CALCULSANSSOURCEWEIGHT

// dimension du calcul classique
#define NBFOISTOUCHEMAXSTD (20)
#define NBFOISTOUCHEMAXMARGINAL (20)
#define NBFOISTOUCHEMAXZOOM (10)

// valeure mxi de présentation des résultat
#define NBCLASSEMAXRESULTAT (20)
#define TAILLETRIPASCALE (100)
#define BASEPROBA (1000)
#define VBINOMIALMAX (1+NBFOISTOUCHEMAXSTD)
// paramètre max du trusquin
#define MEDIAWEIGHTMAX (5)
// Nb de spot alloué en avance en marginal
#define NBSPOTPREALLOC (100)
// Nb de spot(ligne) maximum dans un tomjob marginal
#ifdef OPTION_POLOGNE
	#define NBSPOTMARGIPLUSMOINS (500)
#else
	#define NBSPOTMARGIPLUSMOINS (100)
#endif
// dimension de calcul en marginal


#ifdef OPTION_CHECKUP
//.....
// à enlever pour mes tests, puis à remettre !!!
#define HORAIRE_THEORIQUE

// à enlever pour mes tests, puis à remettre !!!
#define DEADLINE (2001,2,1)
#endif

#ifdef OPTION_CARAT

#define DEADLINE (2005,2,1)

#endif

#ifdef OPTION_JFC
#define REPETITION_OPTIMALE
#define DEADLINE (2005,2,1)
#endif

#ifdef OPTION_ZOOMRADIOTELE
#define REPETITION_OPTIMALE
#define DEADLINE (2002,1,1)
#define CALCULSANSSOURCEWEIGHT
#undef NBFOISTOUCHEMAXMARGINAL
#define NBFOISTOUCHEMAXMARGINAL (10)
#endif

#ifdef OPTION_TOPCHRONO
#define REPETITION_OPTIMALE
#define DEADLINE (2001,1,1)
#define CALCULSANSSOURCEWEIGHT
#define CALCULASYMPTOTE
#undef NBFOISTOUCHEMAXMARGINAL
#define NBFOISTOUCHEMAXMARGINAL (10)
#endif

#ifdef OPTION_MEDIATOP
#define DEADLINE (2002,2,1)
#endif

#ifdef OPTION_MEDIACONSULT
#define DEADLINE (2003,3,1)
// Courbe de réponse imposéé dans le job
#define COURBEDEREPONSEIMPOSEE
#undef NBFOISTOUCHEMAXMARGINAL
#undef NBFOISTOUCHEMAXSTD 
#undef NBCLASSEMAXRESULTAT
#undef NBFOISTOUCHEMAXZOOM
#undef NBSPOTMARGIPLUSMOINS

#define NBFOISTOUCHEMAXSTD (31)
#define NBFOISTOUCHEMAXMARGINAL (31)
#define NBCLASSEMAXRESULTAT (31)
#define NBFOISTOUCHEMAXZOOM (31)
#define NBSPOTMARGIPLUSMOINS (5000)

#endif

// les structures
#include "ficlbx.h"
#include "ficauxitaly.h"
#include "ficind.h"
#include "ficcod.h"


struct SCouvIndiv
{
	unsigned short	NrIndiv; // Numero de l'individu
	unsigned short	Proba;	// Probabilité en millième

	SCouvIndiv()
	{
		NrIndiv = 0; // Numero de l'individu
		Proba   = 0;	// Probabilité en millième
	}
};

struct SCouvSupports
{
	unsigned short	Jour; // Jour de la support 0 à 6
	unsigned short	Horaire; // Horaire de la support
	unsigned short	NbCas;	// Nombre cas dans cette support
	struct SCouvIndiv *Indiv; // liste des individus

	SCouvSupports()
	{
		Jour = 0; // Jour de la support 0 à 6
		Horaire = 0; // Horaire de la support
		NbCas = 0;	// Nombre cas dans cette support
		Indiv = 0; // liste des individus
	}
};

struct SSupport{
	short Station; // NrUnique
	short Jour;
	DWORD CodeHoraire;
	short HorairePassage;
	short Option;
	double GRP;
	double CoefDivGrp;	// Le % de GRP utilisé
	double **GRPvent;// [nrventilation][nrclasse]
	short * NbInsert; // [plan]
	short Margi_NbInsert;
	char NrRegionForce; // =NbRegion si pas forcé
	char Prive[30];
	bool fJour; // voir m_fCalculApportJour

	// lien avec la source de C% 
	struct SCouvSupports *Sup;
	// pour les ventilations, il peut y avoir un défaut de proba,
	// dans ce cas, on a 1 pointeur par ventil 
	struct SCouvSupports ***SupVent; // [critère][classe]
	char fAlloc; // 1 alloué std //2 alloué pour le zoom
	SSupport()
	{
		Station = 0; // NrUnique
		Jour    = 0;
		CodeHoraire = 0;
		HorairePassage = 0;
		Option = 0;
		GRP =0;
		GRPvent = 0;// [nrventilation][nrclasse]
		NbInsert = 0; // [plan]
		Margi_NbInsert = 0;
		NrRegionForce = 0; // =NbRegion si pas forcé
		Prive[0] = 0;
		fJour = false; // voir m_fCalculApportJour

		// lien avec la source de C% 
		Sup = 0;
		// pour les ventilations, il peut y avoir un défaut de proba,
		// dans ce cas, on a 1 pointeur par ventil 
		SupVent = 0; // [critère][classe]
		fAlloc = 0; // 1 alloué std //2 alloué pour le zoom
	}
};

class SJour {
public:
	char Libelle[30];
	char LibJour[30];
	double Individu;
	SJour()
	{
		Libelle[0] = 0;
		LibJour[0] = 0;
		Individu = 0;
	}
};


class STOMJOB{
public:
	short NrEtude;
	CString LibEtude;
	short NrCible;
	CString LibCible;

	short TypeSuppU;
	short Beta; // -1 si calcul non demandé
	CArray <float,float&> CourbeReponseImpose;
	short Mini; // Carat
	short Maxi; // Carat
	short NbPlan;
	short NbSource;
	short NbVentilation;
	short *NbClasseVent; // (interne)
	short *NrCritereVent; // en ventilation sur C% (externe)

	// pour chaque source :
	short *NrSourceCouv,*NrVagueCouv,*FormatSource;
	CString* CodeVagueCouv;
	char **NomFicCouv; // AUX
#ifdef OPTION_MEDIACONSULT
	char **NomFicIND;	// numéro unique des individus mainframe
	char **NomFicCOD;	// code rubrica mainframe
	char **NomFicPoids; // PDX
	char **NomFicVentil; // CRC
	char **NomFicNbModal; // CRX
#endif
	long *PopCibleGRP;
	double ***PopGRPclassVent; //[src][crit][class]
	short *NbIndivC;	// Nb d'indiv sur la source de C% [src]
	long **CoeffExCible; // coeff d'extrapolation brut de la cible
	short **NrRatio; // Vecteur des ratio des individue en fc de la consomation
	long **CoeffExEnsemble; // coeff d'extrapolation brut de l'Ensemble (non pondéré)
	double **CoeffExRed; // coeff d'extrapolation corrigé
	double ***CoeffExRedVentile; // coeff d'extrapolation corrigé pour les ventilations
	char ***NrClasseVent; // [src][nrventilation][nrindiv]
	int *NbSupport;
	int *NbSupportAlloue; // pour le marginal
	
	short *NbRegion;
	short **NrRegionVect;		// [NrSource][NbRegion]
	bool ***fIndivRegion; //[NrSource][NrRegion+1(toutes)][indiv]
	char** NrRegionIndiv; //[NrSource][indiv]
	long** popGRPregion; //[NrSource][NrRegion]
	long**** popGRPregionVentile; //[NrSource][NrRegion][critère][classe]

	// pour chaque support:
	struct SSupport **Support;

	// tampon pour le calcul classique
	short maxS,maxI;
	double *MW;  // [support]
	double ***MWvent;  // [support][nrventilation][nrclasse]

	double *PopCibleCOUV; 
	double *SrcWeight;
	double ***PopCouvclassVent; //[src][crit][class]
	double ***SrcWeightVent; //[src][crit][class]
	double *CoeffIndiv; // [indivC]
	double **VB; // [indivC][classe]: Vecteur Binomial
	double *Qvent;// [indivC] Qpour ventiler
	double *P,*PP;
	double ***Distrib; // [source][Plan][ClasseContact]
	double ****Ventil; // [source][Plan][nrventilation][nrclasse]

	// tampon pour le calcul en marginal
	char fJobMarginal;
	double **Margi_MW;  // [source][support]
	double **Margi_CoeffIndiv; // [source][indivC]
	double ***Margi_VB; // [source][indivC][classe]: Vecteur Binomial
	double **Margi_P,**Margi_PP;
	char *Margi_Marquage; // [indivC]

	// tampon pour le calcul du zoom
	int Zoom_NbSupport;
	int Zoom_NbSupportAlloue; 
	struct SSupport *Zoom_Support;// [maxsupport]
	double *Zoom_MW;  // [maxsupport]
	double *Zoom_CoeffIndiv; // [maxindivC]
	double **Zoom_VB; // [maxindivC][classe]: Vecteur Binomial
	double *Zoom_P,*Zoom_PP; //[maxindivC]
	char fZoomCopied;
	double *Zoom_Distrib; // [ClasseContact]

	// les résultats
	double PopCibTotalGRP;
	double PopCibTotalCOUV;
	short NbCasTotal;

	double *NbIndivT; // [Plan]
#ifndef OPTION_POLOGNE
	double *MWW; // [support]
	double ***MWWvent; // [support][nrventilation][nrclasse]
	double *PPP;
	double **Margi_MWW; // [source][support]
	double **Margi_PPP;
	double *Zoom_MWW; // [maxsupport]
	double *Zoom_PPP; //[maxindivC]
#endif

#ifdef CALCULASYMPTOTE
	double *NbIndivAssymptote; // [Plan]
#endif
	double *NbContact; // [Plan]

	double **ContactRatio; // [Plan][Ratio]
	double **IndividuRatio; // [Plan][Ratio]

	short NbFoisToucheMax;
	short NbClasseContact;
	double *CourbeReponse;	// [ClasseContact]
	double *CouvertureValide; // [Plan]
#ifdef OPTION_MEDIACONSULT
	double *NbContactValide; // [Plan]
#endif
	double **DistribCumulee; // [Plan][ClasseContact]
	double **DistribExact; // [Plan][ClasseContact]

	// résultat de la ventilation
	double ***resGRPvent; // [Plan][crit][Classe]
	double ***resCOUVvent; // [Plan][crit][Classe]
#ifdef OPTION_MEDIACONSULT
	double ***resGRPventValide; // [Plan][crit][Classe]
	double ***resCOUVventValide; // [Plan][crit][Classe]
#endif
	double **PopventCouv; //[crit][Classe]
	unsigned short **NbCasVentilation; //[crit][Classe]
	double **PopventGRP; //[crit][Classe]

	// résultat du zoom
	double Zoom_NbIndivT;
	double Zoom_NbContact;
	double Zoom_CouvertureValide;
	double Zoom_NbContactValide;
	double *Zoom_DistribCumulee; // [ClasseContact]
	double *Zoom_DistribExact; // [ClasseContact]

	char fCalculAlloue;
	double CoeffCHI; // Coeff Coerrection Hypothèse d'indépendance
	double CoeffDupp; // Coeff des duplicatas

	// pseudo proba pour support sans proba!
	struct SCouvSupports DefaultSupport;

	// Apport par jour
	CArray<SJour,SJour>Jour;
	CArray<SJour,SJour>Semaine;
	CArray<SJour,SJour>SemaineGliss;

	STOMJOB(){
// MODIF SYLVAIN
	NrEtude = 0;
	NrCible = 0;

	TypeSuppU = 0;
	Beta = 0;

	Mini = 0; // Carat
	Maxi = 0; // Carat
	NbPlan = 0;
	NbSource = 0;
	NbVentilation = 0;
	NbClasseVent = 0; // (interne)
	NrCritereVent = 0; // en ventilation sur C% (externe)

	// pour chaque source :
	NrSourceCouv = 0; NrVagueCouv = 0; FormatSource = 0;
	CodeVagueCouv = 0;
	NomFicCouv = 0; // AUX
#ifdef OPTION_MEDIACONSULT
	NomFicIND = 0;	// numéro unique des individus mainframe
	NomFicCOD = 0;	// code rubrica mainframe
	NomFicPoids = 0; // PDX
	NomFicVentil = 0; // CRC
	NomFicNbModal = 0; // CRX
#endif
	PopCibleGRP = 0;
	PopGRPclassVent = 0; //[src][crit][class]
	NbIndivC = 0;	// Nb d'indiv sur la source de C% [src]
	CoeffExCible = 0; // coeff d'extrapolation brut de la cible
	NrRatio = 0; // Vecteur des ratio des individue en fc de la consomation
	CoeffExEnsemble = 0; // coeff d'extrapolation brut de l'Ensemble (non pondéré)
	CoeffExRed = 0; // coeff d'extrapolation corrigé
	CoeffExRedVentile = 0; // coeff d'extrapolation corrigé pour les ventilations
	NrClasseVent = 0; // [src][nrventilation][nrindiv]
	NbSupport = 0;
	NbSupportAlloue = 0; // pour le marginal
	
	NbRegion = 0;
	NrRegionVect = 0;		// [NrSource][NbRegion]
	fIndivRegion = 0; //[NrSource][NrRegion+1(toutes)][indiv]
	NrRegionIndiv = 0; //[NrSource][indiv]
	popGRPregion = 0; //[NrSource][NrRegion]
	popGRPregionVentile = 0; //[NrSource][NrRegion][critère][classe]

	// pour chaque support:
	Support = 0;

	// tampon pour le calcul classique
	maxS = 0;maxI = 0;
	MW = 0; // [support]
	MWvent = 0; // [support][nrventilation][nrclasse]

	PopCibleCOUV = 0; 
	SrcWeight = 0;
	PopCouvclassVent = 0; //[src][crit][class]
	SrcWeightVent = 0; //[src][crit][class]
	CoeffIndiv = 0; // [indivC]
	VB = 0; // [indivC][classe]: Vecteur Binomial
	Qvent = 0;// [indivC] Qpour ventiler
	P = 0; PP = 0;
	Distrib=0; // [source][Plan][ClasseContact]
	Ventil=0; // [source][Plan][nrventilation][nrclasse]

	// tampon pour le calcul en marginal
	fJobMarginal = 0;
	Margi_MW = 0; // [source][support]
	Margi_CoeffIndiv = 0; // [source][indivC]
	Margi_VB = 0; // [source][indivC][classe]: Vecteur Binomial
	Margi_P = 0; Margi_PP = 0;
	Margi_Marquage = 0; // [indivC]

	// tampon pour le calcul du zoom
	Zoom_NbSupport = 0;
	Zoom_NbSupportAlloue = 0; 
	Zoom_Support = 0;// [maxsupport]
	Zoom_MW = 0; // [maxsupport]
	Zoom_CoeffIndiv = 0; // [maxindivC]
	Zoom_VB = 0; // [maxindivC][classe]: Vecteur Binomial
	Zoom_P = 0; Zoom_PP = 0; //[maxindivC]
#ifndef OPTION_POLOGNE
	PPP = 0;
	Margi_PPP = 0;
	Zoom_PPP = 0; //[maxindivC]
#endif
	fZoomCopied = 0;
	Zoom_Distrib = 0; // [ClasseContact]

	// les résultats
	PopCibTotalGRP = 0;
	PopCibTotalCOUV = 0;
	NbCasTotal = 0;

	NbIndivT = 0; // [Plan]
#ifdef CALCULASYMPTOTE
	NbIndivAssymptote = 0; // [Plan]
#endif
	NbContact = 0; // [Plan]

	ContactRatio = 0; // [Plan][Ratio]
	IndividuRatio = 0; // [Plan][Ratio]

	NbFoisToucheMax = 0;
	NbClasseContact = 0;
	CourbeReponse = 0;	// [ClasseContact]
	CouvertureValide = 0; // [Plan]
#ifdef OPTION_MEDIACONSULT
	NbContactValide = 0; // [Plan]
#endif
	DistribCumulee = 0; // [Plan][ClasseContact]
	DistribExact = 0; // [Plan][ClasseContact]

	// résultat de la ventilation
	resGRPvent = 0; // [Plan][crit][Classe]
	resCOUVvent = 0; // [Plan][crit][Classe]
#ifdef OPTION_MEDIACONSULT
	resGRPventValide = 0; // [Plan][crit][Classe]
	resCOUVventValide = 0; // [Plan][crit][Classe]
#endif
	PopventCouv = 0; //[crit][Classe]
	NbCasVentilation=0; //[crit][Classe]
	PopventGRP=0; //[crit][Classe]

	// résultat du zoom
	Zoom_NbIndivT = 0;
	Zoom_NbContact = 0;
	Zoom_CouvertureValide = 0;
	Zoom_NbContactValide = 0;
	Zoom_DistribCumulee = 0; // [ClasseContact]
	Zoom_DistribExact = 0; // [ClasseContact]

	fCalculAlloue = 0;
	CoeffCHI = 0; // Coeff Coerrection Hypothèse d'indépendance
	CoeffDupp = 0; // Coeff des duplicatas

// MODIF SYLVAIN
		Jour.RemoveAll();
		Semaine.RemoveAll();
	}

	// Nombre de spots du plan de base ou la proba par défaut est utilisée
	int m_StatProbaDef_NbSpot;
};


// Liste de supports se fait par Source, Vague et Region
struct SLISTESUPPORT
{
	struct SLISTESUPPORT		*Next;

	short				TypeSuppU;
	short				Source;
	short				Vague;
	char				*NomFic;

	short				NbStations; // Toutes Stations sont uniques
	short				*NrStation;
	short				*NbSupports;
	struct SCouvSupports	**Supports;

	SLISTESUPPORT()
	{
		Next = 0;

		TypeSuppU = 0;
		Source   = 0;
		Vague = 0;
		NomFic = 0;

		NbStations = 0; // Toutes Stations sont uniques
		NrStation = 0;
		NbSupports = 0;
		Supports = 0;
	}
};

class CTomjob{

	bool m_fSourceAllocated; // En library, pour savoir si les vecteurs sources sont alloués
	char m_fCorrigeCoeffEx; // correction des coeff d'extrapolation
	char m_fExtern; // 1 si TypesuppU>100
	char m_fVentilCouv; // 1 si ventilation sur source de C%
	char m_fCalculCHI; // calcul Correction d'Hypothèse d'Indépendance
	char m_fCalculDupp; // calcul les duplicatas (écrans exacte)
	char m_fQuickJob; // calcul des couverture sans les distributions

	short *m_NewStation,*m_NewJour,*m_NewOption,*m_NewNbInsert;
	DWORD * m_NewCodeHoraire;
#ifdef HORAIRE_THEORIQUE
	short *m_NewHorairePassage;
#endif
	short *m_NewRegionForce;
	double *m_NewGRP,*m_NewCoefDivGrp;
	short m_NewSrc,*m_NewSup,m_NbNewSup;

	STOMJOB *T;
	CStdioFile m_TraceFile;

	short GetRes(FILE * Fic, char *buff, short lrecl);
	SCouvSupports * FindSupportOriginel(SLISTESUPPORT *S,short Station,DWORD Horaire,short Jour,double GRP,long * Coeff);
	short FindSupportCalcul(short Sup,short Src,SLISTESUPPORT *S,char fZoom);

	void DelierSource();
	SLISTESUPPORT *TrouveSource(short TypeSuppU, short Source, short Vague, char *NomFic, short Format);
	short ChargerSource(SLISTESUPPORT *Support, short Format);
	short ChargerSup0(SLISTESUPPORT *Supports);
	void AlloueCalcul();
	void LibereCalcul();
	void CalculMediaWeight(short src);
	void CalculMediaWeightVentile(short src,short v);
	void EvaluerSousPlan(short sp,short src,char fCalculMarginal=0,char fZoom=0);
	void CalculVentilation(short sp,short src,short v);
	void CalculVentilationInterne(short sp,short src,short v);

	void ConsolideResultat(char fCalculMarginal=0,char fZoom=0);
	void CalculTableauCroise(char fCalculMarginal=0,char fZoom=0);
	void CalculQuartiles(int NrSrc);
	double CalculBetaZero(short coeff);
	void CalculCouvertureValide(char fCalculMarginal=0,char fZoom=0);
	void CalculCourbeReponse(char fCalculMarginal);

	HWND m_hWndMaitre;	
	CString m_NomFicRESTOM;
	CString m_NomFicTOMJOB;
	CString m_NomFicCIBJOB;

public:
	 ~CTomjob();
	 CTomjob();
	short LierSource();
	short LectureTomjob(LPCSTR NomFicTOMJOB,LPCSTR NomFicRESTOM,HWND hWndMaitre=0,char fCalculMarginal=0);
	long EcritureRestom(char fZoom=0);

	void AlloueStructTomjob(char fMarginal=0,bool fCouverture=0);
	void LibereStructTomjob(void);
	short EvaluerPlan();

#ifdef OPTION_MEDIACONSULT
	void EvaluerMonteCarlo(short p,char fZoom=false);
#endif


	void LibererSource(void);
	char m_fBreakDetected; 
	short m_NrLien;

	// les fonction du calcul marginal
	short LectureTomjobMarginal(char fZoom);
	short InsertNouveauSpot(char fZoom);
//	short InsertNouveauZoom();
	short EvaluerPlanMarginal(char fZoom,bool fCalculMonoSource=0);
	void DebutZoom(){T->NbFoisToucheMax=NBFOISTOUCHEMAXZOOM;}
	void FinZoom(){T->NbFoisToucheMax=NBFOISTOUCHEMAXMARGINAL;}
	short CopieSourceZoom();

private:
	short TrouveNrIndexRegion(long NrUniqueReg,short src);
	void MarqueLesIndividus(short src,short sup,char fZoom);
	short LierNouveauSpot(short Src,short Sup,char fZoom);
	short InsertNewSup(short Sup,char fZoom);
	void AnnuleMemoZoom();
	void CalculCoeffExRed(short src);
	void CalculCoeffExRedVentile(short src,short v);
	void CalculDuplication(unsigned short & NbVBx,double Coeff,double);
	void CalculMediaWeightNouveauSpot(short src,short sup,char fZoom);
	void LoadDefaultProba();

private:
	void TrouveJourActif(short src);
	long EcritureRestom_Carat(char fZoom);
	long EcritureRestom_MediaConsultant(char fZoom);
	long EcritureRestom_Mediatop(char fZoom);
	void EvaluerApportSemaine();
	void EvaluerApportSemaineGlissante();
	short m_fCalculRatio;
	char * tamponalloc;
	CFicLBX *m_FicLBX;
#ifdef OPTION_MEDIACONSULT
	CFicAUXitaly * m_FicAuxItaly;
	CFicIND * m_FicInd;
	CFicCOD * m_FicCod;
#endif
	bool m_fApportJournalier;
	bool m_fMultiMedia;
	void EvaluerApportJournalier(short src,short jour);

public:
	// Chargement à l'avance du panel
	bool LoadPanel(LPCSTR Repertoire,LPCSTR Vague,HWND hWndMaitre);

	
	// La définition du job
	void SetJob0_New(HWND hWndMaitre,char fCalculMarginal);
	void SetJob1_Etude(int Nr,LPCSTR Libelle);
	void SetJob2_Cible(int Nr,LPCSTR Libelle);
	void SetJob3_TypeSuppU(int Nr);
	void SetJob4_Beta(int Nr);
	void SetJob4_Beta(int BetaMedia1,int BetaMedia2,int VisualTransfert);

	void SetJob5_NbPlan(int Nr);
	void SetJob6_NbSource(int Nr);
	void SetJob7_Ventilation(int Nb,int * Nr);

	// Les sources
	void SetSource1_Definition(int NrSrc,LPCSTR LibVague,int Format);
	void SetSource2_Repertoire(int NrSrc,LPCSTR directory);
	void SetSource3_Population(int NrSrc,long Population);
	void SetSource4_Region(int NrSrc,int Nb,int * Nr);
	void SetSource5_Support(int NrSrc,int Nb,bool fCalculMonoSource=0);

	// Les supports
	void SetSupport1_Definition(int NrSrc,int NrSupp,int NrStation,int Horaire,int Jour,int NrRegion,int Option,double GRP,LPCSTR Prive,int * NbInsert,double * GRP_ventile = NULL,bool fCalculMonoSource=0);

	void SetVecteurBinomialPreCalcule(int NrMedia,double **VB);


	// Les modes
	void SetMode_QuickJob(bool flag);

	// Les Résultats
	double GetResultat_Couverture();
#ifdef CALCULASYMPTOTE
	double GetResultat_Assymptote();
#endif
	double ** GetResultat_TableauCroise();
	double ** GetResultat_TableauCroiseValide();
	double * GetResultat_Quartiles(int NrSource,bool fCalcul);
	double * GetResultat_GRPQuartiles();
	long GetResultat_NbCasCible();
	long GetResultat_PopulationCible();

	int GetResultat_NbClasseContact();
	double GetResultat_ApportScanner();

	void CalculCibleComplementaire(int NrSrc,char flag);

	void CalculApportScanner(int NrSrc);


private:
	void AlloueJob();

	// Multi-média *************************
	double ** m_TabCroise; // si m_fMultimedia
	double ** m_TabCroiseValide; // si m_fMultimedia, si Beta
	double ** m_TabBeta; // si m_fMultimedia
	// Multi-média *************************
	// Scanner *****************************
	double *m_Quartiles;
	double *m_GRPQuartiles;
	double m_ApportScanner;
	int m_Seuil1,m_Seuil2,m_Seuil3; // Seuils des quartiles
	double *m_CoeffExComp;
	// Scanner *****************************
};


