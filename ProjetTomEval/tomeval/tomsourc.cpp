#include "stdafx.h"
//#include "tomeval.h"		// prototypes structures et classes
#include "FicAUX.h"		// prototypes structures et classes
#include "FicCIB.h"		// prototypes structures et classes
#include "FicVague.h"		// prototypes structures et classes
#include "Source.h"
#include "Convert.h"

struct  SUPPORT1
{
	char 	Code[5+1];
	short 	Station;
	char 	Jour;
	short  	Heure;
	short 	CodeTom1;
	short 	CodeTom2;
	short 	CodeTom3;
};


static SLISTESUPPORT *GListeSupport=NULL;
static CSource CacheSrc;
static CConvert Convert;

short CTomjob::FindSupportCalcul(short Sup,short Src,SLISTESUPPORT *S,char fZoom){
	struct SCouvSupports *Supp,*Supp1,*Supp2;
	SSupport *SuportRecherche;
	short PS,PS1,PS2;
	short NbCas,NbCas1,NbCas2;
	DWORD Horaire;
	char *p;		
	int FlagPanelFCA = 0;
	if(p = strrchr(S->NomFic,'\\')) {
		if (!strnicmp(p-5,"SFR23\\",6)) FlagPanelFCA = 1;
	}

	if(!fZoom){
		SuportRecherche=&T->Support[Src][Sup];
	}
	else {
		SuportRecherche=&T->Zoom_Support[Sup];
	}
	Supp1=FindSupportOriginel(S,SuportRecherche->Station,SuportRecherche->CodeHoraire,SuportRecherche->Jour,SuportRecherche->GRP,T->CoeffExCible[Src]);
	if((!Supp1) && (!FlagPanelFCA)) return(1);
	if(FlagPanelFCA || SuportRecherche->Option==1) // ancrage
	{
		if((SuportRecherche->CodeHoraire%100)>=45) Horaire=SuportRecherche->CodeHoraire+55;
		else Horaire=SuportRecherche->CodeHoraire+15;
		Supp2=FindSupportOriginel(S,SuportRecherche->Station,Horaire,SuportRecherche->Jour,SuportRecherche->GRP,T->CoeffExCible[Src]);
		if((!Supp1) && (!Supp2)) return(1);
		NbCas1 = NbCas2 = 0;
		if (Supp1)NbCas1=Supp1->NbCas;
		if (Supp2)NbCas2=Supp2->NbCas;
		NbCas=NbCas1+NbCas2;
		if(NbCas==0){
			// même si on ne l'utilise pas, on est obligé de l'allouer
			// pour que l calcul (qui valire nbcas) passe !
			SuportRecherche->Sup=new SCouvSupports;
			SuportRecherche->Sup->NbCas=0;
			SuportRecherche->Sup->Indiv=NULL;
			SuportRecherche->fAlloc=(fZoom)?2:1;
			return(0);
		}
		Supp=new SCouvSupports;
		Supp->Indiv=new SCouvIndiv [NbCas];
		SuportRecherche->fAlloc=(fZoom)?2:1;
		PS=PS1=PS2=0;
		while(1)
		{
			if(PS1<NbCas1 && PS2<NbCas2)
			{
				if(Supp1->Indiv[PS1].NrIndiv==Supp2->Indiv[PS2].NrIndiv) // Même Individu
				{
					Supp->Indiv[PS].NrIndiv=Supp1->Indiv[PS1].NrIndiv;
					Supp->Indiv[PS++].Proba=(Supp1->Indiv[PS1++].Proba+Supp2->Indiv[PS2++].Proba)/2;
				}
				else
				{
					if(Supp1->Indiv[PS1].NrIndiv<Supp2->Indiv[PS2].NrIndiv) // Individu 1 Inferier à 2
					{
						Supp->Indiv[PS].NrIndiv=Supp1->Indiv[PS1].NrIndiv;
						Supp->Indiv[PS++].Proba=Supp1->Indiv[PS1++].Proba/2;
					}
					else // Individu 2 inferier à 1
					{
						Supp->Indiv[PS].NrIndiv=Supp2->Indiv[PS2].NrIndiv;
						Supp->Indiv[PS++].Proba=Supp2->Indiv[PS2++].Proba/2;
					}
				}
				continue; // On boucle
			}
			if(PS1<NbCas1) // Copie le restant du support 1
			{
				for(;PS1<NbCas1;PS1++,PS++)
				{
					Supp->Indiv[PS].NrIndiv=Supp1->Indiv[PS1].NrIndiv;
					Supp->Indiv[PS].Proba=Supp1->Indiv[PS1].Proba/2;
				}
				break;
			}
			if(PS2<NbCas2) // Copie le restant du support 2
			{
				for(;PS2<NbCas2;PS2++,PS++)
				{
					Supp->Indiv[PS].NrIndiv=Supp2->Indiv[PS2].NrIndiv;
					Supp->Indiv[PS].Proba=Supp2->Indiv[PS2].Proba/2;
				}
			}
			break;
		}
		Supp->NbCas=PS;
		if (SuportRecherche->CoefDivGrp != 1.0) {
			for(PS=0;PS<Supp->NbCas;PS++)
			{
				Supp->Indiv[PS].Proba = (short)(0.501+Supp->Indiv[PS].Proba*SuportRecherche->CoefDivGrp);
			}	
		}
		SuportRecherche->Sup=Supp;
		//delete [] Supp;
	}
	else if (SuportRecherche->CoefDivGrp != 1.0) {
		SuportRecherche->fAlloc=(fZoom)?2:1;
		Supp=new SCouvSupports;
		Supp->Indiv=new SCouvIndiv [Supp1->NbCas];
		Supp->NbCas=Supp1->NbCas;
		for(PS=0;PS<Supp1->NbCas;PS++)
		{
			Supp->Indiv[PS].NrIndiv = Supp1->Indiv[PS].NrIndiv;
			Supp->Indiv[PS].Proba = (short)(0.501+Supp1->Indiv[PS].Proba*SuportRecherche->CoefDivGrp);
		}
		SuportRecherche->Sup=Supp;
	}
	else
		SuportRecherche->Sup=Supp1;

	return(0);
}


bool CTomjob::LoadPanel(LPCSTR Repertoire,LPCSTR Vague,HWND hWndMaitre)
{
	CacheSrc.LoadSource(Repertoire);
	CacheSrc.LoadVague(Vague,hWndMaitre);
	return 1; //OK
}


// Lier le liste de suports au tomjob
short CTomjob::LierSource()
{
#ifdef OPTION_MEDIACONSULT
		short NbIndiv;
		short Src=0,/*Sup,*/r;
	
		// récupération de la source 
		// !! pour l'instant 1 seule vague autorisée !!
//		CFicAUXitaly *FicAux;
		CFicLBX *FicLbx;
		CFicVagueCIB *FicCib;
		
		CacheSrc.LoadSource(48);
		CacheSrc.LoadVague(T->NomFicCouv[Src],T->NomFicPoids[Src],T->NomFicIND[Src],T->NomFicCOD[Src],m_hWndMaitre,T->NomFicVentil[Src],T->NomFicNbModal[Src]);
		NbIndiv=CacheSrc.GetNbIndiv();
		FicCib=CacheSrc.GetFicCib();
//		FicAux=CacheSrc.GetFicAux();
		m_FicLBX=FicLbx=CacheSrc.GetFicLbx();
		m_FicAuxItaly=CacheSrc.GetFicAuxItaly();
		m_FicCod=CacheSrc.GetFicCod();
		m_FicInd=CacheSrc.GetFicInd();

		/*FicCib.LoadVague((LPCSTR)T->NomFicPoids[Src],NbIndiv);
		FicAux.LoadVague((LPCSTR)T->NomFicCouv[Src],m_hWndMaitre);
		FicLbx.LoadVagueCRX((LPCSTR)T->NomFicVentil[Src],T->NomFicNbModal[Src]);*/
		
		// maintenant on connait le nb d'indiv
		T->NbIndivC[Src]=NbIndiv;
		T->fIndivRegion[Src]=new bool* [T->NbRegion[Src]+1];
		for (r=0;r <= T->NbRegion[Src];r++){
			T->fIndivRegion[Src][r]=new bool [T->NbIndivC[Src]];
			memset(T->fIndivRegion[Src][r],0xFFFFFFFF,T->NbIndivC[Src]*sizeof(bool));
		}

		T->CoeffExCible[Src]=new long [NbIndiv];
		T->NrRatio[Src]=new short [NbIndiv];
		T->CoeffExRed[Src]=new double [NbIndiv];
		FicCib->LoadPoidsCibleIdx(0,T->CoeffExCible[Src]);
		
		// liaison des probas aux supports du plan
/*		Inutile depuis la Monte-carlo
		for(Sup=0;Sup<T->NbSupport[Src];Sup++){
			T->Support[Src][Sup].Sup=new SCouvSupports;
			FicAux->TrouveProbas(T->Support[Src][Sup].Station,T->Support[Src][Sup].CodeHoraire,T->Support[Src][Sup].HorairePassage,T->Support[Src][Sup].Jour,(short&)T->Support[Src][Sup].Sup->NbCas,T->Support[Src][Sup].Sup->Indiv);
		}*/

		// les renseignements sur les ventilations
		short v,s;
		for(v=0;v<T->NbVentilation;v++){
			T->NbClasseVent[v]=FicLbx->GetNbModalite(T->NrCritereVent[v]);
			T->PopGRPclassVent[Src][v]=new double [T->NbClasseVent[v]];
			T->NrClasseVent[Src][v]=new char [T->NbIndivC[Src]];
			FicLbx->GetNrModaliteIndiv(T->NrCritereVent[v],T->NrClasseVent[Src][v],T->NbIndivC[Src]);
			for(s=0;s<T->NbSupport[Src] ;s++){
				T->Support[Src][s].GRPvent[v]=new double [T->NbClasseVent[v]];
			}
		}
		LoadDefaultProba();
//		if(m_fCalculRatio)	FicAux->LoadRatio(T->CoeffExCible[Src],NbIndiv,T->NrRatio[Src],m_fCalculRatio);
		return(0);
#else	
	if(m_fExtern){ // cas moteur en externe...
		short NbIndiv;
		short Src,Sup,r;
		CFicAUX *FicAux;
		CFicLBX *FicLbx;
		CFicVagueCIB *FicCib;

		for(Src=0;Src<T->NbSource;Src++){
		
			// récupération de la source 
			// !! pour l'instant 1 seule source autorisée !!
			// Par contre plusieurs vagues de cette même source
			// peuvent êtres chargée
			CacheSrc.LoadSource(T->NomFicCouv[Src]);
			CacheSrc.LoadVague(T->CodeVagueCouv[Src],m_hWndMaitre);
			Convert.Load(T->NomFicCouv[Src]);
			NbIndiv=CacheSrc.GetNbIndiv();
			FicCib=CacheSrc.GetFicCib();
			FicAux=CacheSrc.GetFicAux();
			m_FicLBX=FicLbx=CacheSrc.GetFicLbx();
			
			// maintenant on connait le nb d'indiv
			T->NbIndivC[Src]=NbIndiv;
			T->fIndivRegion[Src]=new bool* [T->NbRegion[Src]+1];
			for (r=0;r <= T->NbRegion[Src];r++){
				T->fIndivRegion[Src][r]=new bool [T->NbIndivC[Src]];
				memset(T->fIndivRegion[Src][r],0xFFFFFFFF,T->NbIndivC[Src]*sizeof(bool));
			}

			T->CoeffExCible[Src]=new long [NbIndiv];
			T->NrRatio[Src]=new short [NbIndiv];
			T->CoeffExRed[Src]=new double [NbIndiv];
	#ifdef OPTION_MEDIATOP
 			FicCib->LoadPoidsCible(Convert.GetNrCible(T->LibCible),T->CoeffExCible[Src]);
	#endif
	#ifdef OPTION_CARAT
			FicCib->LoadPoidsCible(Convert.GetNrCible(T->NrCible),T->CoeffExCible[Src]);
	#endif
	#ifdef OPTION_JFC
			FicCib->LoadPoidsCible(T->NrCible,T->CoeffExCible[Src]);
	#endif
	#ifdef OPTION_CHECKUP
			FicCib->LoadPoidsCible(T->NrCible,T->CoeffExCible[Src]);
	#endif
	#ifdef OPTION_ZOOMRADIOTELE
			// On travaille directement sur l'index !
			FicCib->LoadPoidsCible(T->NrCible+1,T->CoeffExCible[Src]);
	#endif
	#ifdef OPTION_TOPCHRONO
			// On travaille directement sur l'index !
			FicCib->LoadPoidsCible(T->NrCible+1,T->CoeffExCible[Src]);
	#endif
			
//#define VERIF			 
#ifdef VERIF			
//			Vérification des asymptotes
			{
				CString txt;
				double touche,tele,radio,total=0;
				bool *V=new bool[NbIndiv];
				bool *Vtele=new bool[NbIndiv];
				bool *Vradio=new bool[NbIndiv];
				int i;
				for(i=0;i<NbIndiv;i++){
					total+=T->CoeffExCible[Src][i];
					Vtele[i]=0;
					Vradio[i]=0;
				}
				for(int s=0;s<FicAux->m_NbStation;s++){
					for(int i=0;i<NbIndiv;i++)V[i]=0;
					for(int j=0;j<FicAux->m_NbJour[s];j++){
						for(int p=0;p<FicAux->m_NbPlage[s][j];p++){
							for(int c=0;c<FicAux->m_NbCas[s][j][p];c++){
								if(FicAux->m_TabProba[s][j][p][c].Proba<=0 ||
									FicAux->m_TabProba[s][j][p][c].Proba>1000){
									AfficheErreur("Erreur dans le fichier AUX: une proba est fausse");
								}
								else {
									V[FicAux->m_TabProba[s][j][p][c].NrIndiv]=1;
								}
								if(FicAux->m_NrStation[s]<100)Vtele[FicAux->m_TabProba[s][j][p][c].NrIndiv]=1;
								else Vradio[FicAux->m_TabProba[s][j][p][c].NrIndiv]=1;
							}
						}
					}
					touche=0;
					for(i=0;i<NbIndiv;i++)if(V[i])touche+=T->CoeffExCible[Src][i];
					txt.Format("station: %d\nAsymptote sur la cible: %.2f",FicAux->m_NrStation[s],100*touche/total);
					AfficheErreur(txt);
				}	
				tele=radio=touche=0;
				for(i=0;i<NbIndiv;i++){
					if(Vtele[i] || Vradio[i])touche+=T->CoeffExCible[Src][i];
					if(Vtele[i])tele+=T->CoeffExCible[Src][i];
					if(Vradio[i])radio+=T->CoeffExCible[Src][i];
				}
				txt.Format("Asymptote radio: %.2f\nAsymptote tele: %.2f\nAsymptote radio+tele: %.2f",100*radio/total,100*tele/total,100*touche/total);
				AfficheErreur(txt);


				delete [] V;
				delete [] Vtele;
				delete [] Vradio;
			}
#endif
			


			// liaison des probas aux supports du plan
			for(Sup=0;Sup<T->NbSupport[Src];Sup++){
				T->Support[Src][Sup].Sup=new SCouvSupports;
				FicAux->TrouveProbas(T->Support[Src][Sup].Station,(short)T->Support[Src][Sup].CodeHoraire,T->Support[Src][Sup].HorairePassage,T->Support[Src][Sup].Jour,(short&)T->Support[Src][Sup].Sup->NbCas,T->Support[Src][Sup].Sup->Indiv);

			}

			// les renseignements sur les ventilations
			short v,s;
			for(v=0;v<T->NbVentilation;v++){
				T->NbClasseVent[v]=FicLbx->GetNbModalite(T->NrCritereVent[v]);
				T->PopGRPclassVent[Src][v]=new double [T->NbClasseVent[v]];
				T->NrClasseVent[Src][v]=new char [T->NbIndivC[Src]];
				FicLbx->GetNrModaliteIndiv(T->NrCritereVent[v],T->NrClasseVent[Src][v],T->NbIndivC[Src]);
				for(s=0;s<T->NbSupport[Src] ;s++){
					T->Support[Src][s].GRPvent[v]=new double [T->NbClasseVent[v]];
				}
			}
			LoadDefaultProba();
			if(m_fCalculRatio)FicAux->LoadRatio(T->CoeffExCible[Src],NbIndiv,T->NrRatio[Src],m_fCalculRatio);
		}
		return(0);
	}


	// chargement des données en interne ...
	SLISTESUPPORT *S=NULL;
	short Src,Sup; //StaS
	bool fMessage=false;

	for(Src=0;Src<T->NbSource;Src++)
	{
		S=TrouveSource(T->TypeSuppU,T->NrSourceCouv[Src],T->NrVagueCouv[Src],T->NomFicCouv[Src],T->FormatSource[Src]);
		if(!S) return(1);
		for(Sup=0;Sup<T->NbSupport[Src];Sup++){		// Faire un lien entre le support d'insertion et le proba
			if(FindSupportCalcul(Sup,Src,S,0))fMessage=true;
		}
		if(m_fVentilCouv){
			CFicLBX FicLbx;
			CString txt1,txt2;
			short l=strlen(T->NomFicCouv[Src]);
			txt2=&T->NomFicCouv[Src][l-4];
			txt1=T->NomFicCouv[Src];
			txt1.SetAt(l-9,0);
			FicLbx.LoadVague(txt1,txt2);
			short v,s;
			for(v=0;v<T->NbVentilation;v++){
				T->NbClasseVent[v]=FicLbx.GetNbModalite(T->NrCritereVent[v]);
				T->PopGRPclassVent[Src][v]=new double[T->NbClasseVent[v]];
				T->NrClasseVent[Src][v]=new char[T->NbIndivC[Src]];
				FicLbx.GetNrModaliteIndiv(T->NrCritereVent[v],T->NrClasseVent[Src][v],T->NbIndivC[Src]);
				for(s=0;s<T->NbSupport[Src] ;s++){
					T->Support[Src][s].GRPvent[v]=new double [T->NbClasseVent[v]];
				}
			}
		}
	}
	LoadDefaultProba();
	if(fMessage)AfficheErreur("PB dans le calcul:\nAu moins un support n'est pas trouvé dans le Panel");

	return(0);
#endif
}

void CTomjob::LoadDefaultProba()
{
	// Construction d'un vecteur de proba à 1 GRP sur la cible
	// basé sur l'écoute radio en générale ...
	int Src,nbcas,c,i;
	for(Src=0;Src<T->NbSource;Src++)
	{
		nbcas=0;
		for(i=0;i<T->NbIndivC[Src];i++)if(T->CoeffExCible[Src][i]>0)nbcas++;
		T->DefaultSupport.NbCas=nbcas;
		T->DefaultSupport.Indiv=new SCouvIndiv [nbcas];
		for(i=0,c=0;i<T->NbIndivC[Src];i++)if(T->CoeffExCible[Src][i]>0){
			T->DefaultSupport.Indiv[c].NrIndiv=i;
			T->DefaultSupport.Indiv[c].Proba=1;//BASEPROBA/1000;
			c++;
		}
	}
}



// Lier un nouveau support
short CTomjob::LierNouveauSpot(short Src,short Sup,char fZoom)
{
	int *pNbSupport;
	SSupport *Support;
	if(fZoom){
		Support=T->Zoom_Support;
		pNbSupport=&T->Zoom_NbSupport;
	}
	else {
		Support=T->Support[m_NewSrc];
		pNbSupport=&T->NbSupport[m_NewSrc];
	}
	
	if(m_fExtern){
		CFicAUX *FicAux;
	
		// récupération de la source 
		// !! pour l'instant 1 seule source autorisée !!
		CacheSrc.LoadSource(T->NomFicCouv[Src]);
		CacheSrc.LoadVague(T->CodeVagueCouv[Src]);
		FicAux=CacheSrc.GetFicAux();
		
		// liaison des probas aux supports du plan
		Support[Sup].Sup=new SCouvSupports;

#ifdef HORAIRE_THEORIQUE
		FicAux->TrouveProbas(Support[Sup].Station,(short)Support[Sup].CodeHoraire,Support[Sup].HorairePassage,Support[Sup].Jour,(short&)Support[Sup].Sup->NbCas,Support[Sup].Sup->Indiv);
#else 
		FicAux->TrouveProbas(Support[Sup].Station,(short)Support[Sup].CodeHoraire,(short)Support[Sup].CodeHoraire,Support[Sup].Jour,(short&)Support[Sup].Sup->NbCas,Support[Sup].Sup->Indiv);
#endif
		

		return(0);
	}

	SLISTESUPPORT *S=NULL;

	S=TrouveSource(T->TypeSuppU,T->NrSourceCouv[Src],T->NrVagueCouv[Src],T->NomFicCouv[Src],T->FormatSource[Src]);
	if(!S) return(1);
	FindSupportCalcul(Sup,Src,S,fZoom);
	return(0);
}


extern double *PT1;
#include "math.h"

struct SCouvSupports * CTomjob::FindSupportOriginel(SLISTESUPPORT *S,short Station,DWORD Horaire,short Jour,double GRP,long * Coeff)
{
	short Sta,Sup;
	int i;
	if(T->TypeSuppU==1) for(Sta=0;Sta<S->NbStations;Sta++){ // en écran
		if(S->NrStation[Sta]==Station){
			for(Sup=0;Sup<S->NbSupports[Sta];Sup++){
				if(S->Supports[Sta][Sup].Horaire==Horaire && (S->Supports[Sta][Sup].Jour & Jour))return(&S->Supports[Sta][Sup]);
			}
		}

		// l'écran n'est pas trouvé... on lui en trouve un de substitution...
		// Recherche de l'ecran d'avant
		SCouvSupports * SAvant,*SApres;
		SAvant=NULL;
		SApres=NULL;
		for(Sta=0;Sta<S->NbStations;Sta++){
			if(S->NrStation[Sta]==Station){
				for(Sup=0;Sup<S->NbSupports[Sta];Sup++){
					if(S->Supports[Sta][Sup].Jour & Jour){
						if(S->Supports[Sta][Sup].Horaire>=Horaire){
							SApres=&S->Supports[Sta][Sup];
							goto AvantApresTrouve;
						}
						SAvant=&S->Supports[Sta][Sup];
					}
				}
			}
		}
AvantApresTrouve:
		if(SAvant==NULL)return(SApres);
		if(SApres==NULL)return(SAvant);
		// on prend celui qui a le GRP le + proche...
		// ceci est fait ici, et pas chez liem, car il faut
		// retrouver le même résultat dans la pige;
		// celle-ci n'a (je suppose) pas la grille de substitution de Laurent Bliault
		double GRPavant=0,GRPapres=0,d1,d2;
		for(i=0;i<SAvant->NbCas;i++){
			GRPavant+=Coeff[SAvant->Indiv[i].NrIndiv]*PT1[SAvant->Indiv[i].Proba];
		}
		for(i=0;i<SApres->NbCas;i++){
			GRPapres+=Coeff[SApres->Indiv[i].NrIndiv]*PT1[SApres->Indiv[i].Proba];
		}

		// ici ça bug si on bosse en multi-source...
		// pour l'instant on n'a pas le cas
#ifdef _DEBUG   
		if(T->NbSource>1)AfficheErreur("Big Problème dans la fc:\nFindSupportOriginel()");
#endif

		GRP=T->PopCibleGRP[0]*GRP/100;
		d1=fabs(GRP-GRPavant);
		d2=fabs(GRP-GRPapres);
		if(d1>d2)return(SApres); else return(SAvant);
	}
	else for(Sta=0;Sta<S->NbStations;Sta++){ // en zone (1/4H ou 1/2H)
		if(S->NrStation[Sta]==Station){
			for(Sup=0;Sup<S->NbSupports[Sta];Sup++){
				if(S->Supports[Sta][Sup].Jour & Jour)
				{
					// Si c'est le dernier support du panel
					// ou que c'est le dernier de la station...
					if ((Sup>=S->NbSupports[Sta]-1) || (S->Supports[Sta][Sup].Horaire>S->Supports[Sta][Sup+1].Horaire))
					{
						return(&S->Supports[Sta][Sup]);
					}

					if(Horaire>=S->Supports[Sta][Sup].Horaire && Horaire<S->Supports[Sta][Sup+1].Horaire)
					{
						return(&S->Supports[Sta][Sup]);
					}
				}
			}
		}
	}
	return(NULL);
}
// liberation des ancrages alloués
void CTomjob::DelierSource()
{
	short Src,Sup;

	for(Src=0;Src<T->NbSource;Src++){
		for(Sup=0;Sup<T->NbSupport[Src];Sup++){
			if(T->Support[Src][Sup].fAlloc){
				delete [] T->Support[Src][Sup].Sup->Indiv;
				delete  T->Support[Src][Sup].Sup;
			}
		}
	}

	// en externe, il faut liberer les supports !
	if(m_fExtern)for(Src=0;Src<T->NbSource;Src++){
		for(Sup=0;Sup<T->NbSupport[Src];Sup++){
			delete [] T->Support[Src][Sup].Sup;
		}
	}
	delete [] T->DefaultSupport.Indiv;
	AnnuleMemoZoom();
}

// Trouve un liste de supports
SLISTESUPPORT *CTomjob::TrouveSource(short TypeSuppU, short Source, short Vague, char *NomFic, short Format)
{
	SLISTESUPPORT *Support=NULL;

	Support=GListeSupport;
	while(Support)
	{
		if(Support->TypeSuppU==TypeSuppU && Support->Source==Source && Support->Vague==Vague) return(Support);
		Support=Support->Next;
	}
	Support=new SLISTESUPPORT;
	Support->NomFic=new char [strlen(NomFic)+1];
	strcpy(Support->NomFic,NomFic);
	Support->TypeSuppU=TypeSuppU;
	Support->Source=Source;
	Support->Vague=Vague;
	if(ChargerSource(Support,Format))return(NULL);
	Support->Next=GListeSupport;
	GListeSupport=Support;
	return(Support);
}


// Ou Format == Format du fichier Med et Aud
// 				0 == JFC Radio France, Belgique et Télé France, Espagne
short CTomjob::ChargerSource(SLISTESUPPORT *Support, short Format)
{
	switch(Format)
	{
		case 0: return(ChargerSup0(Support));
	}
	return(1);
}

// Liberer toutes les sources chargées
void CTomjob::LibererSource(void)
{
	short x,y;
	SLISTESUPPORT *S=NULL;
	SLISTESUPPORT *SS=NULL;

	if(m_fExtern){
	}
	else {
		S=GListeSupport;
		while(S)
		{
			if(S->Supports)
			{
				for(x=0;x<S->NbStations;x++)
				{
					if(S->Supports[x])
					{
						for(y=0;y<S->NbSupports[x];y++)
						{
							if(S->Supports[x][y].Indiv) delete [] S->Supports[x][y].Indiv;
						}
						delete [] S->Supports[x];
					}
				}
				delete [] S->Supports;
			}
			if(S->NomFic) delete [] S->NomFic;
			if(S->NrStation) delete [] S->NrStation;
			if(S->NbSupports) delete [] S->NbSupports;
			SS=S;
			S=S->Next;
			delete [] SS;
		}
		GListeSupport=NULL;
	}
}

// Chargement de supports d'un fichier source de FORMAT 0
short CTomjob::ChargerSup0(SLISTESUPPORT *Supports)
{
	short y,Sta,Sup; //z
	unsigned short NbSupports,x;
	//struct CouvIndiv *Indiv;
	SUPPORT1	Support;
	FILE		*fp_med,*fp_aud;
	long		*Position;
	short		*Count;
	short		LastCount;
	short		CurStation;
	short		NbStations;
	short		*NbSupport;
	short		*NrStation;
	short NbProbaFausse=0;

	sprintf(tamponalloc,"%s.MED",Supports->NomFic);
	if (NULL==(fp_med=fopen(tamponalloc,"rb"))){
		char MsgErr[1000];
		sprintf(MsgErr,"Erreur d'ouverture du fichier: %s",tamponalloc);
		AfficheErreur(MsgErr);
		return(1);
	}
	fread(&NbSupports,sizeof(unsigned short),1,fp_med);
	sprintf(tamponalloc,"%s.AUD",Supports->NomFic);
	if ((fp_aud=fopen(tamponalloc,"rb"))==NULL){
		char MsgErr[1000];
		sprintf(MsgErr,"Erreur d'ouverture du fichier: %s",tamponalloc);
		AfficheErreur(MsgErr);
		return(1);
	}
	fseek(fp_aud,-((long)sizeof(short)),SEEK_END); // trouve dernier
	fread(&LastCount,sizeof(short),1,fp_aud); // Nombre de cas
	fseek(fp_aud,(long)0,SEEK_SET);
	Position=new long [NbSupports];
	memset(Position,0,sizeof(long)*NbSupports);
	Count=new short[NbSupports];
	memset(Count,0,sizeof(short)*NbSupports);
	fread(Position,sizeof(long),NbSupports,fp_aud); // Nombre de cas
	for(x=0;x<NbSupports-1;x++)
	{
		Count[x]=(Position[x+1]-Position[x])/(2*sizeof(short));
	}
	Count[x]=LastCount;

#define NBSTATIONMAX 1000

	NbSupport=new short[NBSTATIONMAX];
	memset( NbSupport,0, sizeof(short)*NBSTATIONMAX);
	NrStation=new short[NBSTATIONMAX];
	memset( NrStation,0, sizeof(short)*NBSTATIONMAX);
	NbStations=0;
	CurStation=0;
	for(x=0;x<NbSupports;x++)
	{
		fread(&Support,sizeof(SUPPORT1),1,fp_med);
		if(Support.Station!=CurStation)
		{
			NbStations++;		// D'origine 1
			if(NbStations>=NBSTATIONMAX)
			{
				AfficheErreur("Erreur dans la lecture du support: le nombre de station est trop grand !!!");
				AfxThrowUserException();
				ASSERT(0);
				break;
			}
			NrStation[NbStations]=CurStation=Support.Station;
		}
		NbSupport[NbStations]++;
	}

	Supports->NbStations=NbStations;		// Nombre de station unique
	Supports->NrStation=new short[NbStations];
	memset(Supports->NrStation ,0, sizeof(short)*NbStations );
	Supports->NbSupports=new short[NbStations];
	memset( Supports->NbSupports,0, sizeof(short)* NbStations);
	for(x=0;x<NbStations;x++)
	{
		Supports->NrStation[x]=NrStation[x+1];		// Passer à d'ogigine 0
		Supports->NbSupports[x]=NbSupport[x+1];
	}
	CurStation=0;
	
	Supports->Supports=new SCouvSupports* [Supports->NbStations];
	memset( Supports->Supports,0, sizeof(SCouvSupports*)* Supports->NbStations);
	fseek(fp_med,(long)sizeof(unsigned short),SEEK_SET);
	for(x=0;x<NbSupports;x++)
	{
		fread(&Support,sizeof(SUPPORT1),1,fp_med);
		if(Support.Station!=CurStation)
		{
			for(Sta=0;Sta<NbStations; Sta++) if(Support.Station==Supports->NrStation[Sta]) break;
			Supports->Supports[Sta]=new SCouvSupports [Supports->NbSupports[Sta]];
			memset( Supports->Supports[Sta],0, sizeof(SCouvSupports)* Supports->NbSupports[Sta]);
			CurStation=Support.Station;
			Sup=0;
		}
		else Sup++;
		//if (Sta==54  && Sup==51 ) ASSERT(0);

		Supports->Supports[Sta][Sup].Jour=Support.Jour;
		Supports->Supports[Sta][Sup].Horaire=Support.Heure;
		Supports->Supports[Sta][Sup].NbCas=Count[x];
		if(Supports->Supports[Sta][Sup].NbCas<=0)continue;
		fseek(fp_aud,Position[x],SEEK_SET);
		Supports->Supports[Sta][Sup].Indiv=new SCouvIndiv [Count[x]];
		memset(Supports->Supports[Sta][Sup].Indiv ,0, sizeof(SCouvIndiv)*Count[x] );
		fread(Supports->Supports[Sta][Sup].Indiv,sizeof(struct SCouvIndiv),Count[x],fp_aud);
		for(y=0;y<Count[x];y++)
		{
			Supports->Supports[Sta][Sup].Indiv[y].NrIndiv-=1; // start individus at 0 et On peut appliquer le CoefDivGrp Ici, mais malheureusement
			if(Supports->Supports[Sta][Sup].Indiv[y].Proba>BASEPROBA){				// il est lié au insertion pas à la station
				Supports->Supports[Sta][Sup].Indiv[y].Proba=BASEPROBA;
				NbProbaFausse++;
			}
		}
	}
	fclose(fp_aud);
	fclose(fp_med);
	delete [] NrStation;
	delete [] NbSupport;
	delete [] Position;
	delete [] Count;

/*	if(NbProbaFausse>0){
		sprintf(tamponalloc,"%d proba(s) semblent fausses !!!",NbProbaFausse);
		AfficheErreur(tamponalloc);
	}*/

	return(0);
}
