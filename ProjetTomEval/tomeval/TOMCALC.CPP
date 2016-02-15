/*------------------------------------------------------------------------
 Module:        C:\TOMEVAL\TOMCALC.C
 Author:        REMI
 Projet:        TOMEVAL
 State:         OK
 Creation Date: 8/9/97
 Description:   fc de calcul pour l'évaluation
------------------------------------------------------------------------*/
#include "stdafx.h"
//#include "tomeval.h"		// prototypes structures et classes
#include <math.h>

// variables pour le calcul
double *PT0;
double *PT1;
long **TP;
double ***TrusquinBinomial; // [proba][probaweight:2-MEDIAWEIGHTMAX][0->MEDIAWEIGHTMAX]
short TailleVB;
double * VBmacro,*VBtempo,*VBx,*VBdupp,*B0;



#ifdef OPTION_MEDIACONSULT
// ******************************
// ******************************
// ******************************
// ***  OPTION_MEDIACONSULT  ****
// ******************************
// ******************************
// ******************************
short CTomjob::EvaluerPlan()
{
	AlloueCalcul(); // Not used !!!

	ASSERT(T->NbSource==1); // MONO source
	short p;

	if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_LOADHABITUDE,100,0L);

	CalculCourbeReponse(0);
	for(p=0;p<T->NbPlan;p++){
		if(m_fBreakDetected)return(1);
		EvaluerMonteCarlo(p);
		if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_PROGRESS,p+1,((p+1)*100)/T->NbPlan);
	}


	// calcul des populations ventilés
	long * pCoeffEx=T->CoeffExCible[0];
	int NbIndiv=T->NbIndivC[0];
	for(int i=0;i<NbIndiv;i++)
	{
		for(int v=0;v<T->NbVentilation;v++)
		{
			int NrC=T->NrClasseVent[0][v][i];
			ASSERT(NrC<T->NbClasseVent[v]);
			T->PopventGRP[v][NrC]+=pCoeffEx[i];
		}
	}

	T->SrcWeight[0]=+1.0;
	ConsolideResultat();
	CalculCouvertureValide();
	return(0);
}

extern "C"
{
	float _stdcall RAN2 (int*);
}




//#define TRACEMONTECARLO
#ifdef TRACEMONTECARLO
#include "TraceFile.h"
#endif

void CTomjob::EvaluerMonteCarlo(short p,char fZoom)
{
	if(fZoom)for(int i=0;i<=T->NbFoisToucheMax;i++)T->Zoom_Distrib[i]=0;
	else for(int i=0;i<=T->NbFoisToucheMax;i++)T->Distrib[0][p][i]=0;

	if(fZoom)T->Zoom_NbContact=T->Zoom_NbContactValide=0;
	else T->NbContact[p]=T->NbContactValide[p]=0;
	int NbSupport;
	SSupport * pSupport;

	if(fZoom)
	{
		NbSupport=T->Zoom_NbSupport;
		pSupport=T->Zoom_Support;
	}
	else
	{
		NbSupport=T->NbSupport[0];
		pSupport=T->Support[0];
	}
	int NbIndiv=T->NbIndivC[0];
	long * pCoeffEx=T->CoeffExCible[0];

	CWordArray CtsIndiv;
	CtsIndiv.SetSize(NbIndiv);
	for(int ind=0;ind<NbIndiv;ind++)CtsIndiv[ind]=0;

	bool pbrubrica=0;

	if(NbSupport==1)
	{ // CAS PARTICULIER:
		// En monte carlo, lorsq'il n'y a qu'un seul spot,
		// on ne joue plus les probabilités, mais on prend
		// le spot entier...
		int sup=0;

		WORD NbInsert;
		if(!T->fJobMarginal)NbInsert=pSupport[sup].NbInsert[p];
		else NbInsert=pSupport[sup].Margi_NbInsert;
		if(NbInsert==1)
		{

			// On recherche dans le .COD le RubricaMainFrame
			DWORD RubricaMainFrame=m_FicCod->TrouveRubricaMainFrame(pSupport[sup].Station,pSupport[sup].CodeHoraire);
			if(RubricaMainFrame==0)
			{
				ASSERT(0);
				AfxMessageBox("Il manque des Rubricas dans le fichier .COD");
				return;
			}

			// RubricaMainFrame et NbInsert sont maintenant connus

			// Recherche de l'index de ce rubrica:
			int idxrub;
			if(!m_FicAuxItaly->m_MapRubrica.Lookup(RubricaMainFrame,idxrub))
			{
				ASSERT(0);
				AfxMessageBox("Il manque des Rubricas dans le fichier .COD");
				return;
			}

			CRubrica & Rubrica=m_FicAuxItaly->m_TabProbaRubrica[idxrub];
			int nb=Rubrica.m_TabProba.GetSize();

			// Maintenant que l'on dispose du vecteur précalculé res
			double NbContacts=0;
			for(int i=0;i<nb;i++) // nb
			{
				int ind=Rubrica.m_TabProba[i].m_Nr;
				long CoeffEx=pCoeffEx[ind];
				if(CoeffEx<=0)continue;
				double cts=CoeffEx*Rubrica.m_TabProba[i].m_Proba;
				NbContacts+=cts;

				// Calcul des résultats ventilés
				for(int v=0;v<T->NbVentilation;v++)
				{
					int NrC=T->NrClasseVent[0][v][ind];
					T->resCOUVvent[p][v][NrC]+=cts;
					T->resGRPvent[p][v][NrC]+=cts;

					// Les ventilations en validé:
					T->resCOUVventValide[p][v][NrC]+=cts*T->CourbeReponse[0];
					T->resGRPventValide[p][v][NrC]+=cts*T->CourbeReponse[0];
				}
			}
			if(fZoom)
			{
				T->Zoom_NbContact+=NbContacts;
				T->Zoom_Distrib[1]+=NbContacts;
				T->Zoom_NbContactValide+=NbContacts*T->CourbeReponse[0];
			}
			else
			{
				T->NbContact[p]+=NbContacts;
				T->Distrib[0][p][1]+=NbContacts;
				T->NbContactValide[p]+=NbContacts*T->CourbeReponse[0];
			}

			return;
		}
	}

#ifdef TRACEMONTECARLO
	CTraceFile dbg;
	CString txt;
	dbg.Open("plan1.dbg");
#endif

	// Evaluation classique
	for(int sup=0;sup<NbSupport;sup++)
	{
		WORD NbInsert;
		if(!T->fJobMarginal)NbInsert=pSupport[sup].NbInsert[p];
		else NbInsert=pSupport[sup].Margi_NbInsert;
		if(NbInsert==0)continue;

		// On recherche dans le .COD le RubricaMainFrame
		DWORD RubricaMainFrame=m_FicCod->TrouveRubricaMainFrame(pSupport[sup].Station,pSupport[sup].CodeHoraire);
		if(RubricaMainFrame==0)
		{
			pbrubrica=1;
			continue;
		}

		// RubricaMainFrame et NbInsert sont maintenant connus

		// Recherche de l'index de ce rubrica:
		int idxrub;
		if(!m_FicAuxItaly->m_MapRubrica.Lookup(RubricaMainFrame,idxrub))
		{
			pbrubrica=1;
			continue;
		}

		CRubrica & Rubrica=m_FicAuxItaly->m_TabProbaRubrica[idxrub];
		int idx;
		BYTE * res;
		int nb=Rubrica.m_TabProba.GetSize();
		if(!Rubrica.m_IdxMapNbInsert.Lookup(NbInsert,idx))
		{
			// Création du tirage pour cette rubrica, ce nombre d'insert:
			res=new BYTE[nb];
			memset(res,0,nb);
			int Seed;
			for(int i=0;i<nb;i++)
			{
				int nr=Rubrica.m_TabProba[i].m_Nr;

				// Initialisation de fonction RAN3
				//Seed=-(RubricaMainFrame+(1+nr)*1000);
				Seed=-((1+nr)*1000)+RubricaMainFrame;

#ifdef TRACEMONTECARLO
			txt.Format("Individu %d Seed=%d\n",nr,Seed);
			dbg.Write(txt);
#endif
				float proba=Rubrica.m_TabProba[i].m_Proba;
				for(int ins=0;ins<NbInsert;ins++)
				{
					float check;
					if(!ins)check=RAN2(&Seed);
					else check=RAN2(&Seed);
					if(check<=proba)res[i]++;

#ifdef TRACEMONTECARLO
			txt.Format("%f // %f\n",check,proba);
			dbg.Write(txt);
#endif
				}
			}

			// Rangement du vecteur:
			idx=Rubrica.m_CtsIndiv.Add((void*)res);
			Rubrica.m_IdxMapNbInsert.SetAt(NbInsert,idx);
		}
		else
		{
			res=(BYTE *)Rubrica.m_CtsIndiv[idx];
		}

		// Maintenant que l'on dispose du vecteur précalculé res
		double NbContacts=0;
		for(int i=0;i<nb;i++) // nb
		{
			int ind=Rubrica.m_TabProba[i].m_Nr;
			//if(ind>80)break;
			CtsIndiv[ind]+=res[i];
			NbContacts+=pCoeffEx[ind]*Rubrica.m_TabProba[i].m_Proba*NbInsert;
			for(int v=0;v<T->NbVentilation;v++)
			{
				int NrC=T->NrClasseVent[0][v][ind];
				T->resGRPvent[p][v][NrC]+=pCoeffEx[ind]*Rubrica.m_TabProba[i].m_Proba*NbInsert;
			}

#ifdef TRACEMONTECARLO
			txt.Format("Individu %d (Nr=%d)\n",ind,m_FicInd->m_NrIndiv[ind]);
			dbg.Write(txt);
			txt.Format("Apport %d contacts\n",res[i]);
			dbg.Write(txt);
#endif


		}
//		NbContacts/=1000;
//		T->NbContact[p]+=NbContacts*1000;

		if(fZoom)T->Zoom_NbContact+=NbContacts;
		else T->NbContact[p]+=NbContacts;
	}



	// Distributions des contacts
	for(ind=0;ind<NbIndiv;ind++)
	{
		long CoeffEx=pCoeffEx[ind];
		if(CoeffEx<=0)continue;
		WORD NbContactReduit=CtsIndiv[ind];
		WORD NbContactReel=CtsIndiv[ind];
		if(NbContactReduit<=0)continue;
		if(NbContactReduit>T->NbFoisToucheMax)NbContactReduit=T->NbFoisToucheMax;
		if(fZoom)T->Zoom_Distrib[NbContactReduit]+=CoeffEx;
		else T->Distrib[0][p][NbContactReduit]+=CoeffEx;
		//T->NbContact[p]+=NbContact*CoeffEx;
		// Calcul des résultats ventilés

		if(fZoom)T->Zoom_NbContactValide+=NbContactReel*CoeffEx*T->CourbeReponse[NbContactReduit-1];
		else T->NbContactValide[p]+=NbContactReel*CoeffEx*T->CourbeReponse[NbContactReduit-1];

		for(int v=0;v<T->NbVentilation;v++)
		{
			int NrC=T->NrClasseVent[0][v][ind];
			T->resCOUVvent[p][v][NrC]+=CoeffEx;
			//T->resGRPvent[p][v][NrC]+=NbContactReel*CoeffEx; cf plus haut

			// Les ventilations en validé:
			T->resCOUVventValide[p][v][NrC]+=CoeffEx*T->CourbeReponse[NbContactReduit-1];
			T->resGRPventValide[p][v][NrC]+=NbContactReel*CoeffEx*T->CourbeReponse[NbContactReduit-1];
		}
	}

	if(pbrubrica)
	{
		ASSERT(0);
		AfxMessageBox("Il manque des Rubricas dans le fichier .COD");
	}
}


#else

short CTomjob::EvaluerPlan()
{
	short p,s,v,j;

	if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_LOADHABITUDE,100,0L);
	AlloueCalcul();
	for(s=0;s<T->NbSource;s++){
		CalculCoeffExRed(s);
		CalculMediaWeight(s);
		for(v=0;v<T->NbVentilation;v++){
			CalculCoeffExRedVentile(s,v);
			CalculMediaWeightVentile(s,v);
		}
		for(p=0;p<T->NbPlan;p++){
			if(m_fBreakDetected)return(1);
			EvaluerSousPlan(p,s);
			for(v=0;v<T->NbVentilation;v++){
				if(m_fBreakDetected)return(1);
				if(m_fExtern)CalculVentilation(p,s,v);
				else CalculVentilationInterne(p,s,v);
			}
			if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_PROGRESS,p+1,((p+1)*100)/T->NbPlan);
		}
		if(m_fApportJournalier)TrouveJourActif(s);
	}
	if(m_fMultiMedia){
		if(m_fQuickJob){
			//CalculApportScanner();
			//TODO
		}
		else CalculTableauCroise();
		return(0);
	}

	ConsolideResultat();
	CalculCouvertureValide();
	if(m_fApportJournalier){
		for(s=0;s<T->NbSource;s++){
			for(j=0;j<T->Jour.GetSize();j++){
				EvaluerApportJournalier(s,j);
			}
		}
		EvaluerApportSemaine();
		EvaluerApportSemaineGlissante();
	}
	return(0);
}
#endif

short CTomjob::EvaluerPlanMarginal(char fZoom,bool fCalculMonoSource)
{
#ifdef OPTION_MEDIACONSULT
	CalculCourbeReponse(0);
	EvaluerMonteCarlo(0,fZoom);
#else
	if(fCalculMonoSource)EvaluerSousPlan(0,m_NewSrc,0,fZoom);
	else EvaluerSousPlan(0,m_NewSrc,1,fZoom);
#endif

	if(m_fMultiMedia){
		if(m_fQuickJob)CalculApportScanner(m_NewSrc);
		else CalculTableauCroise(1,fZoom);
		return(0);
	}

	ConsolideResultat(1,fZoom);
	CalculCouvertureValide(1,fZoom);

	return(0);
}


void PrepareCalcul(void)
{
	short n,u,p,x,y;
	double * Trusquin,* Tavant;

	// probabilité d'être touché 0 et 1 fois
	PT0=new double [1+BASEPROBA];
	PT1=new double [1+BASEPROBA];
	for(n=0;n<=BASEPROBA;n++){
		PT1[n]=((double)n)/(double)(BASEPROBA);
		PT0[n]=1.0-PT1[n];
	}

	// calcul du triangle de pascal
	TP= new long*[TAILLETRIPASCALE];
	for(n=0;n<TAILLETRIPASCALE;n++){
		TP[n]=new long[1+n];
		TP[n][0]=1;
		for(u=1;u<n;u++)TP[n][u]=TP[n-1][u-1]+TP[n-1][u];
		TP[n][n]=1;
	}

	// calcul et alloc pour Macro-évaluation
	TailleVB=sizeof(double)*VBINOMIALMAX;

	VBmacro=new double [VBINOMIALMAX];
	VBx=new double [VBINOMIALMAX];
	VBdupp=new double [VBINOMIALMAX];
	B0=new double [VBINOMIALMAX];
	memset(B0,0,TailleVB);
	B0[0]=+1.0;

	// le trusquin binomial
	TrusquinBinomial=new double**[1+BASEPROBA];
	for(p=0;p<=BASEPROBA;p++){
		TrusquinBinomial[p]=new double*[1+MEDIAWEIGHTMAX];
		for(x=1;x<=MEDIAWEIGHTMAX;x++){
			TrusquinBinomial[p][x]=new double[1+x];
			memset(TrusquinBinomial[p][x],0,sizeof(double)*(1+x));
			Trusquin=TrusquinBinomial[p][x];
			Tavant=TrusquinBinomial[p][x-1];
			if(x==1){
				Trusquin[0]=PT0[p];
				Trusquin[1]=PT1[p];
			}
			else {
				y=x;
				Trusquin[y]+=Tavant[y-1]*PT1[p];
				for(y--;y>0;y--)Trusquin[y]=Tavant[y-1]*PT1[p]+Tavant[y]*PT0[p];
				Trusquin[0]=Tavant[0]*PT0[p];
			}
		}
		for(x=1;x<=MEDIAWEIGHTMAX;x++){
			Trusquin=TrusquinBinomial[p][x];
			for(y=x-1;y>0;y--)Trusquin[y]+=Trusquin[y+1];
		}
	}
}

void FinPrepareCalcul(void)
{
	short n,p,x;


	for(p=0;p<=BASEPROBA;p++){
		for(x=1;x<=MEDIAWEIGHTMAX;x++){
			delete [] TrusquinBinomial[p][x];
		}
		delete [] TrusquinBinomial[p];
	}
	delete [] TrusquinBinomial;


	delete [] VBmacro;
	delete [] VBx;
	delete [] VBdupp;
	delete [] B0;

	delete [] PT0;
	delete [] PT1;

	for(n=0;n<TAILLETRIPASCALE;n++){
		delete [] TP[n];
	}
	delete [] TP;
}


void CTomjob::AlloueCalcul(){
	short i,s,p,v;

	T->maxS=T->maxI=0;
	for(s=0;s<T->NbSource;s++){
		if(T->NbSupport[s]>T->maxS)T->maxS=T->NbSupport[s];
		if(T->NbIndivC[s]>T->maxI)T->maxI=T->NbIndivC[s];
	}

	if(T->fJobMarginal){
		// Evaluation marginal
		T->maxS+=NBSPOTPREALLOC;
		T->Margi_MW=new double* [T->NbSource];
		T->Margi_CoeffIndiv=new double* [T->NbSource];
		T->Margi_VB=new double** [T->NbSource];
		T->Margi_P=new double* [T->NbSource];
		T->Margi_PP=new double* [T->NbSource];
#ifndef OPTION_POLOGNE
		T->Margi_MWW=new double* [T->NbSource];
		T->Margi_PPP=new double* [T->NbSource];
#endif
		for(s=0;s<T->NbSource;s++){
			T->Margi_MW[s]=new double [T->NbSupportAlloue[s]];
			T->Margi_CoeffIndiv[s]=new double [T->maxI];
			T->Margi_VB[s]=new double* [T->maxI];
			for(i=0;i<T->maxI;i++)T->Margi_VB[s][i]=new double [VBINOMIALMAX];
			T->Margi_P[s]=new double [T->maxI];
			T->Margi_PP[s]=new double [T->maxI];
#ifndef OPTION_POLOGNE
			T->Margi_MWW[s]=new double [T->NbSupportAlloue[s]];
			T->Margi_PPP[s]=new double [T->maxI];
#endif
		}
		T->Margi_Marquage=new char [T->maxI];

		// Zoom
		T->Zoom_NbSupportAlloue=T->maxS;
		T->Zoom_Support=new SSupport [T->Zoom_NbSupportAlloue];
		T->Zoom_MW=new double [T->Zoom_NbSupportAlloue];
		T->Zoom_CoeffIndiv=new double [T->maxI];
		T->Zoom_VB=new double* [T->maxI];
		for(i=0;i<T->maxI;i++)T->Zoom_VB[i]=new double [VBINOMIALMAX];
		T->Zoom_P=new double [T->maxI];
		T->Zoom_PP=new double [T->maxI];
#ifndef OPTION_POLOGNE
		T->Zoom_MWW=new double [T->Zoom_NbSupportAlloue];
		T->Zoom_PPP=new double [T->maxI];
#endif
		T->Zoom_Distrib=new double [VBINOMIALMAX];

		// les spots en marginal
		m_NewStation=new short [NBSPOTMARGIPLUSMOINS];
		m_NewCodeHoraire=new DWORD [NBSPOTMARGIPLUSMOINS];
#ifdef HORAIRE_THEORIQUE
		m_NewHorairePassage=new short [NBSPOTMARGIPLUSMOINS];
#endif
		m_NewJour=new short [NBSPOTMARGIPLUSMOINS];
		m_NewOption=new short [NBSPOTMARGIPLUSMOINS];
		m_NewNbInsert=new short [NBSPOTMARGIPLUSMOINS];
		m_NewRegionForce=new short [NBSPOTMARGIPLUSMOINS];
		m_NewSup=new short [NBSPOTMARGIPLUSMOINS];
		m_NewGRP=new double [NBSPOTMARGIPLUSMOINS];
		m_NewCoefDivGrp=new double [NBSPOTMARGIPLUSMOINS];
	}
	else {
		// Evaluation classique
		T->Margi_Marquage=new char [T->maxI];
		T->MW=new double [T->maxS];
		T->CoeffIndiv=new double [T->maxI];
		T->VB=new double* [T->maxI];
		for(i=0;i<T->maxI;i++)T->VB[i]=new double [VBINOMIALMAX];
		T->P=new double [T->maxI];
		T->PP=new double [T->maxI];
#ifndef OPTION_POLOGNE
		T->MWW=new double [T->maxS];
		T->PPP=new double [T->maxI];
#endif
	}
	T->Distrib=new double** [T->NbSource];
	for(s=0;s<T->NbSource;s++){
		T->Distrib[s]=new double* [T->NbPlan];
		for(p=0;p<T->NbPlan;p++)T->Distrib[s][p]=new double [VBINOMIALMAX];
	}

	if(m_fCalculRatio){
		T->ContactRatio=new double* [T->NbPlan];
		T->IndividuRatio=new double* [T->NbPlan];
		for(p=0;p<T->NbPlan;p++){
			T->ContactRatio[p]=new double [m_fCalculRatio];
			memset(T->ContactRatio[p],0,sizeof(double)*m_fCalculRatio);
			T->IndividuRatio[p]=new double [m_fCalculRatio];
			memset(T->IndividuRatio[p],0,sizeof(double)*m_fCalculRatio);
		}
	}
	if(!T->fJobMarginal){
		T->NbContact=new double [T->NbPlan];
		memset(T->NbContact,0,sizeof(double)*T->NbPlan);
	}
	else {
		T->NbContact=new double [1+T->NbSource];
		memset(T->NbContact,0,sizeof(double)*(1+T->NbSource));
	}

	T->NbIndivT=new double [T->NbPlan];
#ifdef CALCULASYMPTOTE
	T->NbIndivAssymptote=new double [T->NbPlan];
#endif

	T->DistribExact=new double* [T->NbPlan];
	T->DistribCumulee=new double* [T->NbPlan];
	for(p=0;p<T->NbPlan;p++){
		T->DistribExact[p]=new double [T->NbFoisToucheMax];
		T->DistribCumulee[p]=new double [T->NbFoisToucheMax];
	}
	T->Zoom_DistribExact=new double [T->NbFoisToucheMax];
	T->Zoom_DistribCumulee=new double [T->NbFoisToucheMax];
	if(T->NbVentilation>0){
		int src,sup;

		if(m_fCorrigeCoeffEx){
			T->CoeffExRedVentile=new double** [T->NbSource];
			for(src=0;src<T->NbSource;src++){
				T->CoeffExRedVentile[src]=new double* [T->NbVentilation];
				for(v=0;v<T->NbVentilation;v++){
					T->CoeffExRedVentile[src][v]=new double [T->NbIndivC[src]];
				}
			}
		}

		for(src=0;src<T->NbSource;src++)for(sup=0;sup<T->NbSupport[src];sup++){
			T->Support[src][sup].SupVent=new SCouvSupports ** [T->NbVentilation];
			for(v=0;v<T->NbVentilation;v++){
				T->Support[src][sup].SupVent[v]=new SCouvSupports* [T->NbClasseVent[v]];
			}
		}

		T->PopCouvclassVent=new double** [T->NbSource];
		T->SrcWeightVent=new double** [T->NbSource];
		for(s=0;s<T->NbSource;s++){
			T->PopCouvclassVent[s]=new double* [T->NbVentilation];
			T->SrcWeightVent[s]=new double* [T->NbVentilation];
			for(v=0;v<T->NbVentilation;v++){
				T->PopCouvclassVent[s][v]=new double [T->NbClasseVent[v]];
				T->SrcWeightVent[s][v]=new double [T->NbClasseVent[v]];
			}
		}
		T->resGRPvent=new double** [T->NbPlan];
		T->resCOUVvent=new double** [T->NbPlan];
		for(p=0;p<T->NbPlan;p++){
			T->resGRPvent[p]=new double* [T->NbVentilation];
			T->resCOUVvent[p]=new double* [T->NbVentilation];
			for(v=0;v<T->NbVentilation;v++){
				T->resGRPvent[p][v]=new double [T->NbClasseVent[v]];
				T->resCOUVvent[p][v]=new double [T->NbClasseVent[v]];
				memset(T->resGRPvent[p][v],0,sizeof(double)*T->NbClasseVent[v]);
				memset(T->resCOUVvent[p][v],0,sizeof(double)*T->NbClasseVent[v]);
			}
		}
#ifdef OPTION_MEDIACONSULT
		T->resGRPventValide=new double** [T->NbPlan];
		T->resCOUVventValide=new double** [T->NbPlan];
		for(p=0;p<T->NbPlan;p++){
			T->resGRPventValide[p]=new double* [T->NbVentilation];
			T->resCOUVventValide[p]=new double* [T->NbVentilation];
			for(v=0;v<T->NbVentilation;v++){
				T->resGRPventValide[p][v]=new double [T->NbClasseVent[v]];
				T->resCOUVventValide[p][v]=new double [T->NbClasseVent[v]];
				memset(T->resGRPventValide[p][v],0,sizeof(double)*T->NbClasseVent[v]);
				memset(T->resCOUVventValide[p][v],0,sizeof(double)*T->NbClasseVent[v]);
			}
		}
#endif

		T->MWvent=new double** [T->maxS];
		for(s=0;s<T->maxS;s++){
			T->MWvent[s]=new double* [T->NbVentilation];
			for(v=0;v<T->NbVentilation;v++){
				T->MWvent[s][v]=new double [T->NbClasseVent[v]];
			}
		}
#ifndef OPTION_POLOGNE
		T->MWWvent=new double** [T->maxS];
		for(s=0;s<T->maxS;s++){
			T->MWWvent[s]=new double* [T->NbVentilation];
			for(v=0;v<T->NbVentilation;v++){
				T->MWWvent[s][v]=new double [T->NbClasseVent[v]];
			}
		}
#endif
		T->Qvent=new double [T->maxI];
		T->Ventil=new double*** [T->NbSource];
		for(s=0;s<T->NbSource;s++){
			T->Ventil[s]=new double** [T->NbPlan];
			for(p=0;p<T->NbPlan;p++){
				T->Ventil[s][p]=new double* [T->NbVentilation];
				for(v=0;v<T->NbVentilation;v++){
					T->Ventil[s][p][v]=new double [T->NbClasseVent[v]];
					memset(T->Ventil[s][p][v],0,sizeof(double)*T->NbClasseVent[v]);
				}
			}
		}
		T->PopventCouv=new double* [T->NbVentilation];
		T->NbCasVentilation=new unsigned short* [T->NbVentilation];
		T->PopventGRP=new double* [T->NbVentilation];
		for(v=0;v<T->NbVentilation;v++){
			T->PopventCouv[v]=new double [T->NbClasseVent[v]];
			T->NbCasVentilation[v]=new unsigned short [T->NbClasseVent[v]];
			T->PopventGRP[v]=new double [T->NbClasseVent[v]];
			memset(T->PopventGRP[v],0,sizeof(double)*T->NbClasseVent[v]);
			memset(T->PopventCouv[v],0,sizeof(double)*T->NbClasseVent[v]);
			memset(T->NbCasVentilation[v],0,sizeof(short)*T->NbClasseVent[v]);
		}
	}
	T->CourbeReponse=new double [T->NbFoisToucheMax];
	T->CouvertureValide=new double [T->NbPlan];
	memset(T->CouvertureValide,0,sizeof(double)*T->NbPlan);
#ifdef OPTION_MEDIACONSULT
	T->NbContactValide=new double [T->NbPlan];
	memset(T->NbContactValide,0,sizeof(double)*T->NbPlan);
#endif
	T->PopCibleCOUV=new double [T->NbSource];
	T->SrcWeight=new double [T->NbSource];
	T->fCalculAlloue=1;
}

void CTomjob::LibereCalcul(){
	short i,s,p,v;

	if(!T->fCalculAlloue)return;
	T->fCalculAlloue=0;
	delete [] T->SrcWeight;
	delete [] T->PopCibleCOUV;
	delete [] T->CourbeReponse;
	delete [] T->CouvertureValide;
#ifdef OPTION_MEDIACONSULT
	delete [] T->NbContactValide;
#endif

	if(T->NbVentilation>0){
		for(v=0;v<T->NbVentilation;v++){
			delete [] T->PopventCouv[v];
			delete [] T->NbCasVentilation[v];
			delete [] T->PopventGRP[v];
		}
		delete [] T->PopventCouv;
		delete [] T->NbCasVentilation;
		delete [] T->PopventGRP;

		for(s=0;s<T->NbSource;s++){
			for(p=0;p<T->NbPlan;p++){
				for(v=0;v<T->NbVentilation;v++){
					delete [] T->Ventil[s][p][v];
				}
				delete [] T->Ventil[s][p];
			}
			delete [] T->Ventil[s];
		}
		delete [] T->Ventil;
		delete [] T->Qvent;
		for(s=0;s<T->maxS;s++){
			for(v=0;v<T->NbVentilation;v++){
				delete [] T->MWvent[s][v];
			}
			delete [] T->MWvent[s];
		}
		delete [] T->MWvent;

#ifndef OPTION_POLOGNE
		for(s=0;s<T->maxS;s++){
			for(v=0;v<T->NbVentilation;v++){
				delete [] T->MWWvent[s][v];
			}
			delete [] T->MWWvent[s];
		}
		delete [] T->MWWvent;
#endif


		for(p=0;p<T->NbPlan;p++){
			for(v=0;v<T->NbVentilation;v++){
				delete [] T->resGRPvent[p][v];
				delete [] T->resCOUVvent[p][v];
			}
			delete [] T->resGRPvent[p];
			delete [] T->resCOUVvent[p];
		}
		delete [] T->resGRPvent;
		delete [] T->resCOUVvent;

#ifdef OPTION_MEDIACONSULT
		for(p=0;p<T->NbPlan;p++){
			for(v=0;v<T->NbVentilation;v++){
				delete [] T->resGRPventValide[p][v];
				delete [] T->resCOUVventValide[p][v];
			}
			delete [] T->resGRPventValide[p];
			delete [] T->resCOUVventValide[p];
		}
		delete [] T->resGRPventValide;
		delete [] T->resCOUVventValide;
#endif

		for(s=0;s<T->NbSource;s++){
			for(v=0;v<T->NbVentilation;v++){
				delete [] T->PopCouvclassVent[s][v];
				delete [] T->SrcWeightVent[s][v];
			}
			delete [] T->PopCouvclassVent[s];
			delete [] T->SrcWeightVent[s];
		}
		delete [] T->SrcWeightVent;
		delete [] T->PopCouvclassVent;


		int src,sup;
		for(src=0;src<T->NbSource;src++)for(sup=0;sup<T->NbSupport[src];sup++){
			for(v=0;v<T->NbVentilation;v++){
				delete [] T->Support[src][sup].SupVent[v];
			}
			delete [] T->Support[src][sup].SupVent;
		}

		if(m_fCorrigeCoeffEx){
			for(src=0;src<T->NbSource;src++){
				for(v=0;v<T->NbVentilation;v++){
					delete [] T->CoeffExRedVentile[src][v];
				}
				delete [] T->CoeffExRedVentile[src];
			}
			delete [] T->CoeffExRedVentile;
		}

	} // cas des ventilations

	if(m_fCalculRatio){
		for(p=0;p<T->NbPlan;p++){
			delete [] T->ContactRatio[p];
			delete [] T->IndividuRatio[p];
		}
		delete [] T->ContactRatio;
		delete [] T->IndividuRatio;
	}

	delete [] T->Zoom_DistribCumulee;
	delete [] T->Zoom_DistribExact;
	for(p=0;p<T->NbPlan;p++){
		delete [] T->DistribCumulee[p];
		delete [] T->DistribExact[p];
	}
	delete [] T->DistribCumulee;
	delete [] T->DistribExact;

	delete [] T->NbContact;
	delete [] T->NbIndivT;
#ifdef CALCULASYMPTOTE
	delete [] T->NbIndivAssymptote;
#endif

	for(s=0;s<T->NbSource;s++){
		for(p=0;p<T->NbPlan;p++){
			delete [] T->Distrib[s][p];
		}
		delete [] T->Distrib[s];
	}
	delete [] T->Distrib;

	if(T->fJobMarginal){
		delete [] m_NewCoefDivGrp;
		delete [] m_NewGRP;
		delete [] m_NewSup;
		delete [] m_NewRegionForce;
		delete [] m_NewNbInsert;
		delete [] m_NewOption;
		delete [] m_NewJour;
		delete [] m_NewCodeHoraire;
#ifdef HORAIRE_THEORIQUE
		delete [] m_NewHorairePassage;
#endif
		delete [] m_NewStation;


		// Zoom
		delete [] T->Zoom_Support;
		delete [] T->Zoom_MW;
		delete [] T->Zoom_CoeffIndiv;
		for(i=0;i<T->maxI;i++)delete [] T->Zoom_VB[i];
		delete [] T->Zoom_VB;
		delete [] T->Zoom_P;
		delete [] T->Zoom_PP;
		delete [] T->Zoom_Distrib;
#ifndef OPTION_POLOGNE
		delete [] T->Zoom_MWW;
		delete [] T->Zoom_PPP;
#endif

		for(s=0;s<T->NbSource;s++){
			delete [] T->Margi_PP[s];
			delete [] T->Margi_P[s];
			for(i=0;i<T->maxI;i++)delete [] T->Margi_VB[s][i];
			delete [] T->Margi_VB[s];
			delete [] T->Margi_CoeffIndiv[s];
			delete [] T->Margi_MW[s];
#ifndef OPTION_POLOGNE
			delete [] T->Margi_PPP[s];
			delete [] T->Margi_MWW[s];
#endif
		}
		delete [] T->Margi_PP;
		delete [] T->Margi_P;
		delete [] T->Margi_VB;
		delete [] T->Margi_CoeffIndiv;
		delete [] T->Margi_MW;
		delete [] T->Margi_Marquage;
#ifndef OPTION_POLOGNE
		delete [] T->Margi_PPP;
		delete [] T->Margi_MWW;
#endif
	}
	else {
		// Evaluation classique
		delete [] T->PP;
		delete [] T->P;
		for(i=0;i<T->maxI;i++){
			delete [] T->VB[i];
		}
		delete [] T->VB;
		delete [] T->CoeffIndiv;
		delete [] T->MW;
		delete [] T->Margi_Marquage;
#ifndef OPTION_POLOGNE
		delete [] T->PPP;
		delete [] T->MWW;
#endif
	}
}

void CTomjob::CalculCoeffExRed(short src)
{
	short i,r;
	double *k,*popCOUV;
	double popGRPContreRegion,popCOUVContreRegion,d;
	if(!m_fCorrigeCoeffEx){
		for(i=0;i<T->NbIndivC[src];i++)T->CoeffExRed[src][i]=T->CoeffExCible[src][i];
		return;
	}

	k=new double [T->NbRegion[src]];
	popCOUV=new double [T->NbRegion[src]];
	memset(popCOUV,0,sizeof(double)*T->NbRegion[src]);

	// calcul des pop sur C%
	popCOUVContreRegion=0;
	for(i=0;i<T->NbIndivC[src];i++){
		r=T->NrRegionIndiv[src][i];
		popCOUV[r]+=T->CoeffExEnsemble[src][i];
		if(r>0)popCOUV[0]+=T->CoeffExEnsemble[src][i];
		else popCOUVContreRegion+=T->CoeffExEnsemble[src][i];
	}

	// calcul des coeff k
	popGRPContreRegion=T->popGRPregion[src][0];
	for(r=1;r<T->NbRegion[src];r++){
		d=(double)(T->popGRPregion[src][0])*popCOUV[r];
		if(d>0)k[r]=((double)(T->popGRPregion[src][r])*popCOUV[0])/d;
		else k[r]=+0.0;
		popGRPContreRegion-=T->popGRPregion[src][r];
	}

	// calcul de coeff k[0] (national)
	// ce sera eventuellement utilisé pour corrigé les indiv
	// qui n'appartiennent pas à une sous région.

	d=(double)(T->popGRPregion[src][0])*popCOUVContreRegion;
	if(d>0)k[0]=((double)(popGRPContreRegion)*popCOUV[0])/d;
	else k[0]=0;

	// Mise en application de k
	for(i=0;i<T->NbIndivC[src];i++){
		r=T->NrRegionIndiv[src][i];
		T->CoeffExRed[src][i]=T->CoeffExCible[src][i] * k[r];
	}

	delete [] k;
	delete [] popCOUV;

/*	// une petite verif s'impose ...
	double p=0;
	long p2=0;
	for(i=0;i<T->NbIndivC[src];i++){
		p+=T->CoeffExRed[src][i];
		p2+=T->CoeffExCible[src][i];
	}*/
}

void CTomjob::CalculCoeffExRedVentile(short src,short v)
{
	short i,r,c;
	double *k,*popCOUV;
	double popGRPContreRegion,popCOUVContreRegion,d;
	if(!m_fCorrigeCoeffEx){
		//for(i=0;i<T->NbIndivC[src];i++)T->CoeffExRedVentile[src][v][i]=T->CoeffExCible[src][i];
		return;
	}

	k=new double [T->NbRegion[src]];
	popCOUV=new double [T->NbRegion[src]];


	for(c=0;c<T->NbClasseVent[v];c++){
		for(r=0;r<T->NbRegion[src];r++){
			k[r]=+0.0;
			popCOUV[r]=+0.0;
		}

		// calcul des pop sur C%
		popCOUVContreRegion=0;
		for(i=0;i<T->NbIndivC[src];i++){
			if(c!=T->NrClasseVent[src][v][i])continue;
			r=T->NrRegionIndiv[src][i];
			popCOUV[r]+=T->CoeffExEnsemble[src][i];
			if(r>0)popCOUV[0]+=T->CoeffExEnsemble[src][i];
			else popCOUVContreRegion+=T->CoeffExEnsemble[src][i];
		}

		// calcul des coeff k
		popGRPContreRegion=T->popGRPregionVentile[src][0][v][c];
		for(r=1;r<T->NbRegion[src];r++){
			d=(double)(T->popGRPregionVentile[src][0][v][c])*popCOUV[r];
			if(d>0)k[r]=((double)(T->popGRPregionVentile[src][r][v][c])*popCOUV[0])/d;
			else k[r]=+0.0;
			popGRPContreRegion-=T->popGRPregionVentile[src][r][v][c];
		}

		// calcul de coeff k[0] (national)
		// ce sera eventuellement utilisé pour corrigé les indiv
		// qui n'appartiennent pas à une sou région.

		d=(double)(T->popGRPregionVentile[src][0][v][c])*popCOUVContreRegion;
		if(d>0)k[0]=((double)(popGRPContreRegion)*popCOUV[0])/d;
		else k[0]=0;

		// Mise en application de k
		for(i=0;i<T->NbIndivC[src];i++){
			if(c!=T->NrClasseVent[src][v][i])continue;
			r=T->NrRegionIndiv[src][i];
			T->CoeffExRedVentile[src][v][i]=T->CoeffExCible[src][i] * k[r];
		}


	/*	// une petite verif s'impose ...
		double p=0;
		long p2=0;
		for(i=0;i<T->NbIndivC[src];i++){
			p+=T->CoeffExRed[src][i];
			p2+=T->CoeffExCible[src][i];
		}*/
	}
	delete [] k;
	delete [] popCOUV;
}



void CTomjob::CalculMediaWeight(short src)
{
	double NbContP,NbCont;
	unsigned short s,i,NrI;
	double pop=0;
	bool *Exclusif;
	double *MW;
#ifndef OPTION_POLOGNE
	double *MWW;
#endif


	T->m_StatProbaDef_NbSpot=0;

	if(T->fJobMarginal){
		MW=T->Margi_MW[src];
	}
	else {
		MW=T->MW;
	}
#ifndef OPTION_POLOGNE
	if(T->fJobMarginal){
		MWW=T->Margi_MWW[src];
	}
	else {
		MWW=T->MWW;
	}
#endif

	for(i=0;i<T->NbIndivC[src];i++){
		pop+=T->CoeffExRed[src][i];
	}
	T->PopCibleCOUV[src]=pop; // +++ bien pour vérifier +++
	if(T->PopCibleCOUV[src]<=0.0){
		AfficheErreur("La population de la source de couverture est invalide.");
		T->PopCibleCOUV[src] = T->PopCibleGRP[src];// (Eviter le plantage)un plantage va suivre ...
	}

#ifdef CALCULSANSMEDIAWEIGHT
	T->PopCibleGRP[src]=T->PopCibleCOUV[src];
#endif
#ifdef CALCULSANSSOURCEWEIGHT
	T->PopCibleGRP[src]=(long)T->PopCibleCOUV[src];
#endif

	T->SrcWeight[src]=((double)T->PopCibleGRP[src])/T->PopCibleCOUV[src];

	// Affichage de SrcWeight
	//sprintf(tamponalloc,"SrcWeight=%f",(float)T->SrcWeight[src]);
	//AfficheMessage(tamponalloc);

#ifdef CALCULSANSMEDIAWEIGHT
	T->SrcWeight[src]=+1.0;
	for(s=0;s<T->NbSupport[src];s++)MW[s]=+1.0;
	return;
#endif

//	sprintf(tamponalloc,"PopCouv=%f",(float)T->PopCibleCOUV[src]);
//	AfficheMessage(tamponalloc);

	for(s=0;s<T->NbSupport[src];s++){
		MW[s]=+0.0;
#ifndef OPTION_POLOGNE
		MWW[s]=+0.0;
#endif
		if(T->Support[src][s].Sup==NULL)continue;
		NbContP=+0.0;
		Exclusif=T->fIndivRegion[src][T->Support[src][s].NrRegionForce];
		for(i=0;i<T->Support[src][s].Sup->NbCas;i++){
			NrI=T->Support[src][s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			NbContP+=T->CoeffExRed[src][NrI]*PT1[T->Support[src][s].Sup->Indiv[i].Proba];
		}

		NbCont=((double)T->PopCibleGRP[src])*T->Support[src][s].GRP/100.0;

		// détection d'un default de proba
#ifdef OPTION_POLOGNE
		if(NbContP==0 && NbCont>+0.0){
			// on libère éventuellement le support
			if(T->Support[src][s].fAlloc){
				delete [] T->Support[src][s].Sup->Indiv;
				delete  T->Support[src][s].Sup;
			}
			T->Support[src][s].fAlloc=0;

			T->Support[src][s].Sup=&T->DefaultSupport;
			for(i=0;i<T->Support[src][s].Sup->NbCas;i++){
				NrI=T->Support[src][s].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				NbContP+=T->CoeffExRed[src][NrI]*PT1[T->Support[src][s].Sup->Indiv[i].Proba];
			}
			T->m_StatProbaDef_NbSpot+=T->Support[src][s].NbInsert[0];
		}
#endif
		if(NbCont > 0.0 && NbContP > 0.0 && T->SrcWeight[src] > 0.0){
			MW[s]=(NbCont / NbContP)/T->SrcWeight[src];
#ifndef OPTION_POLOGNE
			MWW[s]=MW[s];
#endif
		}
		else {
#ifdef OPTION_POLOGNE
			MW[s]=+0.0;
#else
			if (NbCont > 0.0 && NbContP == 0.0 && T->SrcWeight[src] > 0.0) // détection d'un defaut de proba
			{
				// on libère éventuellement le support
				if(T->Support[src][s].fAlloc){
					delete [] T->Support[src][s].Sup->Indiv;
					delete  T->Support[src][s].Sup;
				}
				T->Support[src][s].fAlloc=0;
				double NbContPP = 0.0;
				T->Support[src][s].Sup=&T->DefaultSupport;
				for(i=0;i<T->Support[src][s].Sup->NbCas;i++){
					NrI=T->Support[src][s].Sup->Indiv[i].NrIndiv;
					if(!Exclusif[NrI])continue;
					NbContPP+=T->CoeffExRed[src][NrI]*PT1[T->Support[src][s].Sup->Indiv[i].Proba];
				}
				if(NbContPP > 0.0) MWW[s]=(NbCont/NbContPP)/T->SrcWeight[src];
				T->m_StatProbaDef_NbSpot=1;
			}
#endif

		}
	}
}

void CTomjob::CalculMediaWeightNouveauSpot(short src,short sup,char fZoom)
{
	double NbContP,NbCont;
	unsigned short i,NrI;
	bool *Exclusif;
	double *MW;
#ifndef OPTION_POLOGNE
	double *MWW;
#endif

	// ZOOM
//	short *pNbSupport;
	SSupport *Support;
	if(fZoom){
		Support=T->Zoom_Support;
//		pNbSupport=&T->Zoom_NbSupport;
	}
	else {
		Support=T->Support[m_NewSrc];
//		pNbSupport=&T->NbSupport[m_NewSrc];
	}
	// ZOOM



	if(T->fJobMarginal){
		if(fZoom) MW=T->Zoom_MW;
		else MW=T->Margi_MW[src];
#ifndef OPTION_POLOGNE
		if(fZoom) MWW=T->Zoom_MWW;
		else MWW=T->Margi_MWW[src];
#endif
	}
	else {
		AfficheErreur("Fonction prévue pour le calcul marginal");
		return;
	}

	MW[sup]=+0.0;
#ifndef OPTION_POLOGNE
	MWW[sup]=+0.0;
#endif
	NbContP=+0.0;
	Exclusif=T->fIndivRegion[src][Support[sup].NrRegionForce];
	for(i=0;i<Support[sup].Sup->NbCas;i++){
		NrI=Support[sup].Sup->Indiv[i].NrIndiv;
		if(!Exclusif[NrI])continue;
		NbContP+=T->CoeffExRed[src][NrI]*PT1[Support[sup].Sup->Indiv[i].Proba];
	}

	NbCont=((double)T->PopCibleGRP[src])*Support[sup].GRP/100.0;
	// détection d'un default de proba
#ifdef OPTION_POLOGNE
	if(NbContP==0 && NbCont>+0.0){
		Support[sup].Sup=&T->DefaultSupport;
		for(i=0;i<Support[sup].Sup->NbCas;i++){
			NrI=Support[sup].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			NbContP+=T->CoeffExRed[src][NrI]*PT1[Support[sup].Sup->Indiv[i].Proba];
		}
	}
#endif

	if(NbCont > 0.0 && NbContP > 0.0 && T->SrcWeight[src] > 0.0){
		MW[sup]=(NbCont/NbContP)/T->SrcWeight[src];
#ifndef OPTION_POLOGNE
		MWW[sup]= MW[sup];
#endif
	}
	else {
#ifdef OPTION_POLOGNE
		MW[sup]=+0.0;
#else
		if (NbCont > 0.0 && NbContP == 0.0 && T->SrcWeight[src] > 0.0){ // détection d'un defaut de proba
			double NbContPP = 0.0;
			Support[sup].Sup=&T->DefaultSupport;
			for(i=0;i<Support[sup].Sup->NbCas;i++){
				NrI=Support[sup].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				NbContPP+=T->CoeffExRed[src][NrI]*PT1[Support[sup].Sup->Indiv[i].Proba];
			}
			if(NbContPP > 0.0)MWW[sup]=(NbCont/NbContPP)/T->SrcWeight[src];
			T->m_StatProbaDef_NbSpot=1;
		}
#endif
	}
}

void CTomjob::CalculMediaWeightVentile(short src,short v)
{
	double NbCont;
	unsigned short sup,i,NrI,NrC,c;
	bool *Exclusif;


	// le support de base est au départ le même
	for(NrC=0;NrC<T->NbClasseVent[v];NrC++){
		for(sup=0;sup<T->NbSupport[src];sup++){
			T->Support[src][sup].SupVent[v][NrC]=T->Support[src][sup].Sup;
		}
	}

	if(!m_fVentilCouv){ // ventilation sur GRP *************************************
		// calcul pour chaque src C% la population par ventilation
		double NbContP;
		for(c=0;c<T->NbClasseVent[v];c++)T->PopCouvclassVent[src][v][c]=0.0;
		for(i=0;i<T->NbIndivC[src];i++){
			c=T->NrClasseVent[src][v][i];
			T->PopCouvclassVent[src][v][c]+=T->CoeffExRedVentile[src][v][i];
		}

		// calcul du SrcWeight pour chacune des cellules
		for(c=0;c<T->NbClasseVent[v];c++){
			if(T->PopCouvclassVent[src][v][c]>0.0){
				T->SrcWeightVent[src][v][c]=T->PopGRPclassVent[src][v][c]/T->PopCouvclassVent[src][v][c];
			}
			else T->SrcWeightVent[src][v][c]=+0.0;
		}

		for(NrC=0;NrC<T->NbClasseVent[v];NrC++){
			for(sup=0;sup<T->NbSupport[src];sup++){
				T->MWvent[sup][v][NrC]=+0.0;
#ifndef OPTION_POLOGNE
				T->MWWvent[sup][v][NrC]=+0.0;
#endif
				NbContP=0;
				Exclusif=T->fIndivRegion[src][T->Support[src][sup].NrRegionForce];
				for(i=0;i<T->Support[src][sup].SupVent[v][NrC]->NbCas;i++){
					NrI=T->Support[src][sup].SupVent[v][NrC]->Indiv[i].NrIndiv;
					if(!Exclusif[NrI])continue;
					if(NrC!=T->NrClasseVent[src][v][NrI])continue;
					NbContP+=T->CoeffExRedVentile[src][v][NrI]*PT1[T->Support[src][sup].SupVent[v][NrC]->Indiv[i].Proba];
				}
				NbCont=((double)T->PopCibleGRP[src])*T->Support[src][sup].GRPvent[v][NrC]/100.0;
	// détection d'un default de proba
#ifdef OPTION_POLOGNE
				if(NbContP==0 && NbCont>+0.0){
					T->Support[src][sup].SupVent[v][NrC]=&T->DefaultSupport;
					for(i=0;i<T->Support[src][sup].SupVent[v][NrC]->NbCas;i++){
						NrI=T->Support[src][sup].SupVent[v][NrC]->Indiv[i].NrIndiv;
						if(!Exclusif[NrI])continue;
						if(NrC!=T->NrClasseVent[src][v][NrI])continue;
						NbContP+=T->CoeffExRedVentile[src][v][NrI]*PT1[T->Support[src][sup].SupVent[v][NrC]->Indiv[i].Proba];
					}
				}
#endif
				if(NbCont > 0.0 && NbContP > 0.0 && T->SrcWeightVent[src][v][NrC] > 0.0){
					T->MWvent[sup][v][NrC]=(NbCont/NbContP)/T->SrcWeightVent[src][v][NrC];
#ifndef OPTION_POLOGNE
					T->MWWvent[sup][v][NrC]=T->MWvent[sup][v][NrC];
#endif
				}
				else {
#ifdef OPTION_POLOGNE
					T->MWvent[sup][v][NrC]=+0.0;
#else
					if (NbCont > 0.0 && NbContP == 0.0 && T->SrcWeightVent[src][v][NrC] > 0.0){ // détection d'un defaut de proba
						double NbContPP = 0.0;
						T->Support[src][sup].SupVent[v][NrC]=&T->DefaultSupport;
						for(i=0;i<T->Support[src][sup].SupVent[v][NrC]->NbCas;i++){
							NrI=T->Support[src][sup].SupVent[v][NrC]->Indiv[i].NrIndiv;
							if(!Exclusif[NrI])continue;
							if(NrC!=T->NrClasseVent[src][v][NrI])continue;
							NbContPP+=T->CoeffExRedVentile[src][v][NrI]*PT1[T->Support[src][sup].SupVent[v][NrC]->Indiv[i].Proba];
						}
						if(NbContPP>0.0) T->MWWvent[sup][v][NrC]=(NbCont/NbContPP)/T->SrcWeightVent[src][v][NrC];
						T->m_StatProbaDef_NbSpot=1;
					}
#endif
				}
			}
		}
	}
	else { // ventilation sur la source de couverture *******************************

		for(c=0;c<T->NbClasseVent[v];c++)T->SrcWeightVent[src][v][c]=T->SrcWeight[src];
		double *NbContP;
		// calcul de populations de la source de GRP à partir
		// des ventilatons issus de la source de C%
		short c;
		double GRPCouv,GRPCouvTot,NbContTotCouv,ratio,pop;
		for(c=0;c<T->NbClasseVent[v];c++)T->PopGRPclassVent[src][v][c]=0.0;
		for(i=0;i<T->NbIndivC[src];i++){
			NrC=T->NrClasseVent[src][v][i];
			// on est toujours sans correction des coeff !
			T->PopGRPclassVent[src][v][NrC]+=T->CoeffExRed[src][i];
		}
		// redressement des population de src C% sur src GRP
		for(c=0;c<T->NbClasseVent[v];c++){
			pop=T->PopGRPclassVent[src][v][c]*T->SrcWeight[src];
			T->PopGRPclassVent[src][v][c]=pop;
		}

		NbContP=new double [T->NbClasseVent[v]];
		for(sup=0;sup<T->NbSupport[src];sup++){
			NbContTotCouv=0;
			for(c=0;c<T->NbClasseVent[v];c++)T->Support[src][sup].GRPvent[v][c]=+0.0;
			memset(NbContP,0,sizeof(double)*T->NbClasseVent[v]);
			Exclusif=T->fIndivRegion[src][T->Support[src][sup].NrRegionForce];
			for(i=0;i<T->Support[src][sup].Sup->NbCas;i++){
				NrI=T->Support[src][sup].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				NrC=T->NrClasseVent[src][v][NrI];
				NbContP[NrC]+=T->CoeffExRed[src][NrI]*PT1[T->Support[src][sup].Sup->Indiv[i].Proba];
				NbContTotCouv+=T->CoeffExRed[src][NrI]*PT1[T->Support[src][sup].Sup->Indiv[i].Proba];
			}
			GRPCouvTot=NbContTotCouv*=100.0/T->PopCibleCOUV[src];
			if(GRPCouvTot>+0.0)ratio=T->Support[src][sup].GRP/GRPCouvTot;
			else ratio=+0.0;
			for(NrC=0;NrC<T->NbClasseVent[v];NrC++){
				if(NbContP[NrC]>+0.0){
					GRPCouv=NbContP[NrC]*100.0/T->PopCibleCOUV[src];
					T->Support[src][sup].GRPvent[v][NrC]=GRPCouv*ratio;
					NbCont=((double)T->PopCibleGRP[src])*T->Support[src][sup].GRPvent[v][NrC]/100.0;
					T->MWvent[sup][v][NrC]=(NbCont/NbContP[NrC])/T->SrcWeight[src];
				}
				else T->MWvent[sup][v][NrC]=+0.0;
			}
		}
		delete [] NbContP;
	}
}

void CTomjob::EvaluerSousPlan(short sp,short src,char fCalculMarginal,char fZoom)
{
	unsigned short i,s,n,f,NrI,u,ux,ub,NbVBx;

	double p,q;

	// duplicata
	short p1000;
	double *B;
	short NbIndiv,NbSupport;
	double *CoeffEx;
	double CoeffIndiv,*CI;
	double NbCont;
	short NbInsert,NbBoucle;
	double *P,*PP,*MW;
#ifndef OPTION_POLOGNE
	double *PPP,*MWW;
#endif
	double *Distrib;
	double **VB;
	bool *Exclusif;
	double Repart;
	double *Trusquin;

	// on duplique ce qui sert souvent
	CoeffEx=T->CoeffExRed[src];
	NbIndiv=T->NbIndivC[src];
	SSupport *Support;
	if(fZoom){
		Support=T->Zoom_Support;
		NbSupport=T->Zoom_NbSupport;
	}
	else {
		Support=T->Support[src];
		NbSupport=T->NbSupport[src];
	}
	if(T->fJobMarginal){
		if(fZoom){
			P=T->Zoom_P;
			PP=T->Zoom_PP;
			MW=T->Zoom_MW;
			VB=T->Zoom_VB;
			CI=T->Zoom_CoeffIndiv;
#ifndef OPTION_POLOGNE
			PPP=T->Zoom_PPP;
			MWW=T->Zoom_MWW;
#endif
		}
		else {
			P=T->Margi_P[src];
			PP=T->Margi_PP[src];
			MW=T->Margi_MW[src];
			VB=T->Margi_VB[src];
			CI=T->Margi_CoeffIndiv[src];
#ifndef OPTION_POLOGNE
			PPP=T->Margi_PPP[src];
			MWW=T->Margi_MWW[src];
#endif
		}
	}
	else {
		P=T->P;
		PP=T->PP;
		MW=T->MW;
		VB=T->VB;
		CI=T->CoeffIndiv;
#ifndef OPTION_POLOGNE
		PPP=T->PPP;
		MWW=T->MWW;
#endif
	}


	// Calcul de P et PP(Pprime)
	if(fCalculMarginal){
		if(!fZoom)T->NbContact[0]=0;
		else T->Zoom_NbContact=0;
		// Mise à jour de P[i] et PP[i]
		short sup;
		for(sup=0;sup<m_NbNewSup;sup++){
			s=m_NewSup[sup];
#ifdef OPTION_POLOGNE
			if(MW[s]<=+0.0)continue;
#endif
			Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
			for(i=0;i<Support[s].Sup->NbCas;i++){
				NrI=Support[s].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				p=PT1[Support[s].Sup->Indiv[i].Proba]*m_NewNbInsert[sup];
#ifdef OPTION_POLOGNE
				P[NrI]+=p;
				PP[NrI]+=p*MW[s];
#else
				if(MW[s]>0.0){
					P[NrI]+=p;
					PP[NrI]+=p*MW[s];
				}
				PPP[NrI]+=p*MWW[s];
#endif
			}
		}
		// Initialisation du vecteur binomial
		for(i=0;i<NbIndiv;i++)if(T->Margi_Marquage[i])memcpy(VB[i],B0,TailleVB);
	}
	else { // Calcul classique
		// Calcul de P[i] et PP[i]
		memset(P,0,sizeof(double)*NbIndiv);
		memset(PP,0,sizeof(double)*NbIndiv);
#ifndef OPTION_POLOGNE
		memset(PPP,0,sizeof(double)*NbIndiv);
#endif
		for(s=0;s<NbSupport;s++){
#ifdef OPTION_POLOGNE
			if(MW[s]<=+0.0)continue;
#endif
			if(!T->fJobMarginal)NbInsert=Support[s].NbInsert[sp];
			else NbInsert=Support[s].Margi_NbInsert;
			if(NbInsert<1)continue;
			Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
			for(i=0;i<Support[s].Sup->NbCas;i++){
				NrI=Support[s].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				p=PT1[Support[s].Sup->Indiv[i].Proba]*NbInsert;
#ifdef OPTION_POLOGNE
				P[NrI]+=p;
				PP[NrI]+=p*MW[s];
#else
				if(MW[s]>0.0){
					P[NrI]+=p;
					PP[NrI]+=p*MW[s];
				}
				PPP[NrI]+=p*MWW[s];
#endif
			}
		}
		// Initialisation du vecteur binomial
		for(i=0;i<NbIndiv;i++)memcpy(VB[i],B0,TailleVB);
		memset(T->Margi_Marquage,1,T->NbIndivC[src]);
	}


	// calcul du Coeff. flottant et du NbContact
	memset(CI,0,sizeof(double)*NbIndiv);
	NbCont=+0.0;// (facultatif)
#ifdef OPTION_POLOGNE
	double Contact;
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		Contact=CoeffEx[i]*PP[i];
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			//PP[i]=1.1*P[i]; // pour verif
			CI[i]=PP[i]/P[i];
			NbCont+=Contact;// (NB1:perte de contacts dans le 20+)
		}
	}
	NbCont*=T->SrcWeight[src];
#else
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		NbCont+=CoeffEx[i]*PPP[i];
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			CI[i]=PP[i]/P[i];
		}
	}
	NbCont*=T->SrcWeight[src];
#endif
	// distribution du nombre des contacts,
	// en conservant en mémoire les résultats pour
	// le marginal
	if(!T->fJobMarginal){
		// cas de l'évaluation classique
		T->NbContact[sp]+=NbCont;
	}
	else {
		if(fCalculMarginal){
			// calcul marginal
			if(!fZoom){
				T->NbContact[1+src]=NbCont;
				T->NbContact[0]=0;
				for(int z=0;z<T->NbSource;z++)T->NbContact[0]+=T->NbContact[1+z];
			}
			else {
				// ZOOM
				for(int z=0;z<T->NbSource;z++)if(z!=src)NbCont+=T->NbContact[1+z];
				T->Zoom_NbContact=NbCont;
			}
		}
		else {
			// Cas de l'évaluation non marginal d'un job qui peut devenir marginal
			T->NbContact[0]+=NbCont;
			T->NbContact[1+src]=NbCont;
		}
	}


	if(m_fCalculRatio){
		// on ne le met pas dans la boucle ci-dessus
		// pour gagner du temps ....
		for(i=0;i<NbIndiv;i++){
			if(CoeffEx[i]<=0)continue;
#ifdef OPTION_POLOGNE
			Contact=CoeffEx[i]*PP[i];
			if((P[i]>+0.0) && (PP[i]>+0.0)){
				T->ContactRatio[sp][T->NrRatio[src][i]]+=Contact*T->SrcWeight[src];
			}
#else
			T->ContactRatio[sp][T->NrRatio[src][i]]+=CoeffEx[i]*PPP[i]*T->SrcWeight[src];
#endif
		}
	}

	// calcul du vecteur binomial pour chaque individu
	if(m_fCalculCHI){
		double CoeffCHI=T->CoeffCHI;
		for(s=0;s<NbSupport;s++){
		if(MW[s]<=+0.0)continue;
		if(!T->fJobMarginal)NbInsert=Support[s].NbInsert[sp];
		else NbInsert=Support[s].Margi_NbInsert;
		if(NbInsert<1)continue;
		NbBoucle=NbInsert;
		Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
		for(i=0;i<Support[s].Sup->NbCas;i++){
			NrI=Support[s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			if(!T->Margi_Marquage[NrI])continue;
			CoeffIndiv=CI[NrI];
			if(CoeffIndiv<=0.0)continue;
			p1000=Support[s].Sup->Indiv[i].Proba;
			B=VB[NrI];
			p=PT1[p1000]*CoeffIndiv;
			q=1-p;
			{ // *** calcul en MACRO-EVALUATION ***
				// Le Vecteur Binomial de x (VBx)
				// reconstitution de VBx avec le trusquin
				memset(VBx,0,TailleVB);
				NbVBx=(short)ceil(CoeffIndiv);
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				Repart=p;
				Trusquin=TrusquinBinomial[p1000][NbVBx];
				for(u=1;u<=NbVBx;u++){
					if(Trusquin[u]<Repart){
						VBx[u]=Trusquin[u];
						Repart-=Trusquin[u];
					}
					else {
						VBx[u]=Repart;
						break;
					}
				}
				NbVBx=u;
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				// transformation en exactement touchés
				Repart=VBx[NbVBx];
				for(u=NbVBx-1;u>0;u--){
					VBx[u]-=Repart;
					Repart+=VBx[u];
				}
				VBx[0]=1-Repart;

				// Correction de l'hypothèse de relation d'indépendance++++++
				if(NbVBx==1)NbVBx++;
				double PointGlisse;
				PointGlisse=VBx[1]*CoeffCHI;
				if(PointGlisse>+0.0){
					VBx[1]-=PointGlisse;
					PointGlisse/=2.0;
					VBx[2]+=PointGlisse;
					VBx[0]+=PointGlisse;
				}


/*				// recomptage des non-touchés
						-> remplacé par VBx[0]+=PointGlisse;
				Repart=+1.0;
				for(u=1;u<=NbVBx;u++)Repart-=VBx[u];
				VBx[0]=Repart;*/
				// fin de la correction++++++++++++++++++++++++++++++++++++++

				// calcul des duplications
				if(NbInsert>1 && m_fCalculDupp){
					NbBoucle=NbInsert-1;
					for(f=0;f<NbBoucle;f++)CalculDuplication(NbVBx,T->CoeffDupp,p);
					NbBoucle=1;
				}


				if(m_fQuickJob){
					// calcul des non-touchés uniquement
					for(f=0;f<NbBoucle;f++){
						B[0]*=VBx[0];
					}
				}
				else{
					// le processus 'MACRO'
					for(f=0;f<NbBoucle;f++){
						memset(VBmacro,0,TailleVB);
						for(ux=0;ux<=NbVBx;ux++){
							for(ub=0;ub<=T->NbFoisToucheMax;ub++){
								u=ux+ub;
								if(u>T->NbFoisToucheMax)u=T->NbFoisToucheMax;
								//if(VBx[ux]<0)AfxMessageBox("VBx[ux]");
								//if(B[ub]<0)AfxMessageBox("B[ub]");
								VBmacro[u]+=VBx[ux]*B[ub];
							}
						}
						VBtempo=VB[NrI];
						B=VB[NrI]=VBmacro;
						VBmacro=VBtempo;
	//					memcpy(B,0,TailleVB);
					}
				}
			}
		}
	}
	}
	else for(s=0;s<NbSupport;s++){
		if(MW[s]<=+0.0)continue;
		// Calcul + rapide ? sans calcul issue du coeff CHI
		// si le trusquin n'est pas utilisé,
		// c'est une micro-évaluation qui est faite,
		// à la place de la macro-évaluation.
		if(!T->fJobMarginal)NbInsert=Support[s].NbInsert[sp];
		else NbInsert=Support[s].Margi_NbInsert;
		if(NbInsert<1)continue;
		Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
		for(i=0;i<Support[s].Sup->NbCas;i++){
			NrI=Support[s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			if(!T->Margi_Marquage[NrI])continue;
			CoeffIndiv=CI[NrI];
			if(CoeffIndiv<=0.0)continue;
			p1000=Support[s].Sup->Indiv[i].Proba;
			B=VB[NrI];
			p=PT1[p1000]*CoeffIndiv;
			q=1-p;
			if(p<=TrusquinBinomial[p1000][2][1]){
				if(m_fQuickJob){
					// calcul des non-touchés uniquement
					q=pow(q,NbInsert);
					B[0]*=q;
				}
				else {
					// *** calcul en MICRO-EVALUATION ***
					for(f=0;f<NbInsert;f++){
						n=T->NbFoisToucheMax;
						//touchés n fois et +
						B[n]=B[n-1]*p+B[n];
						for(n--;n>=1;n--){
							// touchés n fois
								B[n]=B[n-1]*p+B[n]*q;
						}
						B[0]*=q; // non touchés
					}
				}
			}
			else { // *** calcul en MACRO-EVALUATION ***
				// Le Vecteur Binomial de x (VBx)
				// reconstitution de VBx avec le trusquin
				memset(VBx,0,TailleVB);
				NbVBx=(short)ceil(CoeffIndiv);
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				Repart=p;
				Trusquin=TrusquinBinomial[p1000][NbVBx];
				for(u=1;u<=NbVBx;u++){
					if(Trusquin[u]<Repart){
						VBx[u]=Trusquin[u];
						Repart-=Trusquin[u];
					}
					else {
						VBx[u]=Repart;
						break;
					}
				}
				NbVBx=u;
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				// transformation en exactement touchés
				Repart=VBx[NbVBx];
				for(u=NbVBx-1;u>0;u--){
					VBx[u]-=Repart;
					Repart+=VBx[u];
				}
				VBx[0]=1-Repart;

				if(m_fQuickJob){
					// calcul des non-touchés uniquement
					q=pow(VBx[0],NbInsert);
					B[0]*=q;
				}
				else {
					// le processus 'MACRO'
					for(f=0;f<NbInsert;f++){
						memset(VBmacro,0,TailleVB);
						//for(u=0;u<=T->NbFoisToucheMax;u++)VBmacro[u]=0;
						for(ux=0;ux<=NbVBx;ux++){
							for(ub=0;ub<=T->NbFoisToucheMax;ub++){
								u=ux+ub;
								if(u>T->NbFoisToucheMax)u=T->NbFoisToucheMax;
								//if(VBx[ux]<0)AfxMessageBox("VBx[ux]");
								//if(B[ub]<0)AfxMessageBox("B[ub]");
								VBmacro[u]+=VBx[ux]*B[ub];
							}
						}
						VBtempo=VB[NrI];
						B=VB[NrI]=VBmacro;
						VBmacro=VBtempo;
//						memcpy(B,0,TailleVB);
					}
				}
			}
		}
	}

	// intégration
	if(m_fMultiMedia)return; // cf CalculTableauCroise()
	if(!fZoom)Distrib=T->Distrib[src][sp];
	else Distrib=T->Zoom_Distrib;
	for(n=0;n<=T->NbFoisToucheMax;n++)Distrib[n]=+0.0;
	for(i=0;i<NbIndiv;i++){
		if(CI[i]<=0)continue;
		B=VB[i];
		if(m_fQuickJob){
			Distrib[1]+=(1-B[0])*CoeffEx[i];
		}
		else {
			for(n=0;n<=T->NbFoisToucheMax;n++){
				Distrib[n]+=B[n]*CoeffEx[i];
			}
		}
	}

	/*// vérification au débugage  des C% et GRP
#define NBFOISTOUCHEMAX=100
	NbCont=0;
	for(n=0;n<=NBFOISTOUCHEMAX;n++){
		NbCont+=Distrib[n]*(double)n;
	}*/


	if(m_fCalculRatio){
		for(i=0;i<NbIndiv;i++){
			if(CI[i]<=0)continue;
			B=VB[i];
			for(n=1;n<=T->NbFoisToucheMax;n++){
				T->IndividuRatio[sp][T->NrRatio[src][i]]+=B[n]*CoeffEx[i]*T->SrcWeight[src];
			}
		}
	}


#ifdef CALCULASYMPTOTE
	{
		bool * touche=new bool[NbIndiv];
		for(i=0;i<NbIndiv;i++)touche[i]=0;
		for(s=0;s<NbSupport;s++){
			if(!T->fJobMarginal)NbInsert=Support[s].NbInsert[sp];
			else NbInsert=Support[s].Margi_NbInsert;
			if(NbInsert<1)continue;
			Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
			for(i=0;i<Support[s].Sup->NbCas;i++){
				NrI=Support[s].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				// if(Support[s].Sup->Indiv[i].Proba>0) inutile;
				touche[NrI]=1;
			}
		}
		T->NbIndivAssymptote[sp]=+0.0;
		for(i=0;i<NbIndiv;i++)if(touche[i])T->NbIndivAssymptote[sp]+=CoeffEx[i];
		delete [] touche;
	}
#endif

}


void CTomjob::CalculVentilation(short sp,short src,short v)
{

	unsigned short i,s,NrI,NrC,NbVBx,u;

	double p,q;
	short p1000;

	// duplicata
	short NbIndiv,NbSupport;
	double *CoeffEx;
	double CoeffIndiv;
	short NbInsert;
	double *P,*PP,***MWvent;
#ifndef OPTION_POLOGNE
	double *PPP,***MWWvent,*MW;
#endif
	double *Q;
	double *Ventil;
	bool *Exclusif;
	double *NbCont;
	double Repart;
	double *Trusquin;

	// on duplique ce qui sert souvent
	if(m_fCorrigeCoeffEx)CoeffEx=T->CoeffExRedVentile[src][v];
	else CoeffEx=T->CoeffExRed[src];
	NbIndiv=T->NbIndivC[src];
	NbSupport=T->NbSupport[src];
	P=T->P;
	PP=T->PP;
	MWvent=T->MWvent;
	Q=T->Qvent;
#ifndef OPTION_POLOGNE
	PPP=T->PPP;
	MWWvent=T->MWWvent;
	MW=T->MW;
#endif

	// calcul du Coeff. flottant et du NbContact
	memset(T->CoeffIndiv,0,sizeof(double)*NbIndiv);
	memset(P,0,sizeof(double)*NbIndiv);
	memset(PP,0,sizeof(double)*NbIndiv);
#ifndef OPTION_POLOGNE
	memset(PPP,0,sizeof(double)*NbIndiv);
#endif
	for(s=0;s<NbSupport;s++){
		NbInsert=T->Support[src][s].NbInsert[sp];
		if(NbInsert<1)continue;
		Exclusif=T->fIndivRegion[src][T->Support[src][s].NrRegionForce];
		for(i=0;i<T->Support[src][s].Sup->NbCas;i++){
			NrI=T->Support[src][s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			NrC=T->NrClasseVent[src][v][NrI];
#ifdef OPTION_POLOGNE
			if(MWvent[s][v][NrC]<=+0.0)continue;
			p=PT1[T->Support[src][s].Sup->Indiv[i].Proba]*NbInsert;
			P[NrI]+=p;
			PP[NrI]+=p*MWvent[s][v][NrC];
#else
			p=PT1[T->Support[src][s].Sup->Indiv[i].Proba]*NbInsert;
			if(MW[s]>0.0 && MWvent[s][v][NrC]>0.0){
				P[NrI]+=p;
				PP[NrI]+=p*MWvent[s][v][NrC];
			}
			PPP[NrI]+=p*MWWvent[s][v][NrC];
#endif
		}
	}

	NbCont=new double [T->NbClasseVent[v]];// (contrôle)
	memset(NbCont,0,sizeof(double)*T->NbClasseVent[v]);
#ifdef OPTION_POLOGNE
	double Contact;
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		Contact=CoeffEx[i]*PP[i];
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			//PP[i]=1.2*P[i];
			T->CoeffIndiv[i]=PP[i]/P[i];
			// calcul des contacts (facultatif)
			NrC=T->NrClasseVent[src][v][i];
			NbCont[NrC]+=Contact;
		}
	}
#else
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		// calcul des contacts (facultatif)
		NrC=T->NrClasseVent[src][v][i];
		NbCont[NrC]+=CoeffEx[i]*PPP[i];;
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			T->CoeffIndiv[i]=PP[i]/P[i];
		}
	}
#endif
	for(NrC=0;NrC<T->NbClasseVent[v];NrC++){
		T->resGRPvent[sp][v][NrC]+=NbCont[NrC]*T->SrcWeightVent[src][v][NrC];// (facultatif)
	}
	delete [] NbCont;

	// calcul des couvertures
	for(i=0;i<NbIndiv;i++)Q[i]=1.0;
	for(s=0;s<NbSupport;s++){
#ifndef OPTION_POLOGNE
		if(MW[s]<=+0.0)continue;
#endif
		NbInsert=T->Support[src][s].NbInsert[sp];
		if(NbInsert<1)continue;
		Exclusif=T->fIndivRegion[src][T->Support[src][s].NrRegionForce];
		for(i=0;i<T->Support[src][s].Sup->NbCas;i++){
			NrI=T->Support[src][s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			CoeffIndiv=T->CoeffIndiv[NrI];
			if(CoeffIndiv<=0)continue;
			NrC=T->NrClasseVent[src][v][NrI];
			if(MWvent[s][v][NrC]<=+0.0)continue;
			p1000=T->Support[src][s].Sup->Indiv[i].Proba;
			p=PT1[p1000]*CoeffIndiv;
			q=1-p;
			if(p>TrusquinBinomial[p1000][2][1]){
				// Le Vecteur Binomial de x (VBx)
				// reconstitution de VBx avec le trusquin
				memset(VBx,0,TailleVB);
				NbVBx=(short)ceil(CoeffIndiv);
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				Repart=p;
				Trusquin=TrusquinBinomial[p1000][NbVBx];
				for(u=1;u<=NbVBx;u++){
					if(Trusquin[u]<Repart){
						VBx[u]=Trusquin[u];
						Repart-=Trusquin[u];
					}
					else {
						VBx[u]=Repart;
						break;
					}
				}
				NbVBx=u;
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				// transformation en exactement touchés
				Repart=VBx[NbVBx];
				for(u=NbVBx-1;u>0;u--){
					VBx[u]-=Repart;
					Repart+=VBx[u];
				}
				VBx[0]=1-Repart;
				// Correction de l'hypothèse de relation d'indépendance++++++
				if(m_fCalculCHI){
					double CoeffCHI=T->CoeffCHI;
					double PointGlisse;
					PointGlisse=VBx[1]*CoeffCHI;
					if(PointGlisse>+0.0){
						PointGlisse/=2.0;
						VBx[0]+=PointGlisse;
					}
				}
				// fin de la correction++++++++++++++++++++++++++++++++++++++

				// les non touchés:
				q=VBx[0];
			}
			else {
				// Correction de l'hypothèse de relation d'indépendance++++++
				if(m_fCalculCHI){
					double CoeffCHI=T->CoeffCHI;
					double PointGlisse;
					PointGlisse=p*CoeffCHI;
					if(PointGlisse>+0.0){
						PointGlisse/=2.0;
						q+=PointGlisse;
					}
				}
				// fin de la correction++++++++++++++++++++++++++++++++++++++
				if(NbInsert>1 && m_fCalculDupp){
					// préparation du cas qui suit...
					memset(VBx,0,TailleVB);
					VBx[0]=q;
					VBx[1]=p;
					NbVBx=1;
				}
			}

			if(NbInsert>1){
				if(m_fCalculDupp){
					// le fameux cas préparé ci-dessus...
					short f,NbBoucle=NbInsert-1;
					for(f=0;f<NbBoucle;f++)CalculDuplication(NbVBx,T->CoeffDupp,p);
					q=VBx[0];
				}
				else {
					q=pow(q,NbInsert);
				}
			}

			// OH, le beau calcul !
			Q[NrI]*=q;
			// OH, le beau calcul !
		}
	}

	// intégration
	Ventil=T->Ventil[src][sp][v];
	for(i=0;i<NbIndiv;i++){
		if(T->CoeffIndiv[i]<=0)continue;
		NrC=T->NrClasseVent[src][v][i];
		Ventil[NrC]+=(1.0-Q[i])*CoeffEx[i];
	}
}

// Cette méthode de calcul de ventilation est utilisé en interne,
// ou un defaut de proba sur certain spot peut entrainer des probas
// differentes d'une classe à l'autre.
void CTomjob::CalculVentilationInterne(short sp,short src,short v)
{

	unsigned short i,s,NrI,NrC,NbVBx,u;

	double p,q;
	short p1000;

	// duplicata
	short NbIndiv,NbSupport;
	double *CoeffEx;
	double CoeffIndiv;
	short NbInsert;
	double *P,*PP,***MWvent;
#ifndef OPTION_POLOGNE
	double *PPP,***MWWvent,*MW;
#endif
	double *Q;
	double *Ventil;
	bool *Exclusif;
	double *NbCont;
	double Repart;
	double *Trusquin;

	// on duplique ce qui sert souvent
	if(m_fCorrigeCoeffEx)CoeffEx=T->CoeffExRedVentile[src][v];
	else CoeffEx=T->CoeffExRed[src];
	NbIndiv=T->NbIndivC[src];
	NbSupport=T->NbSupport[src];
	P=T->P;
	PP=T->PP;
	MWvent=T->MWvent;
	Q=T->Qvent;
#ifndef OPTION_POLOGNE
	PPP=T->PPP;
	MWWvent=T->MWWvent;
	MW=T->MW;
#endif

	// calcul du Coeff. flottant et du NbContact
	memset(T->CoeffIndiv,0,sizeof(double)*NbIndiv);
	memset(P,0,sizeof(double)*NbIndiv);
	memset(PP,0,sizeof(double)*NbIndiv);
#ifndef OPTION_POLOGNE
	memset(PPP,0,sizeof(double)*NbIndiv);
#endif

	for(NrC=0;NrC<T->NbClasseVent[v];NrC++){
		for(s=0;s<NbSupport;s++){
#ifdef OPTION_POLOGNE
			if(MWvent[s][v][NrC]<=+0.0)continue;
#endif
			NbInsert=T->Support[src][s].NbInsert[sp];
			if(NbInsert<1)continue;
			Exclusif=T->fIndivRegion[src][T->Support[src][s].NrRegionForce];
			for(i=0;i<T->Support[src][s].SupVent[v][NrC]->NbCas;i++){
				NrI=T->Support[src][s].SupVent[v][NrC]->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				if(NrC!=T->NrClasseVent[src][v][NrI])continue;
				p=PT1[T->Support[src][s].SupVent[v][NrC]->Indiv[i].Proba]*NbInsert;
#ifdef OPTION_POLOGNE
				P[NrI]+=p;
				PP[NrI]+=p*MWvent[s][v][NrC];
#else
				if(MW[s]>0.0 && MWvent[s][v][NrC]>0.0){
					P[NrI]+=p;
					PP[NrI]+=p*MWvent[s][v][NrC];
				}
				PPP[NrI]+=p*MWWvent[s][v][NrC];
#endif
			}
		}
	}

	NbCont=new double [T->NbClasseVent[v]];// (contrôle)
	memset(NbCont,0,sizeof(double)*T->NbClasseVent[v]);
#ifdef OPTION_POLOGNE
	double Contact;
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		Contact=CoeffEx[i]*PP[i];
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			//PP[i]=1.2*P[i];
			T->CoeffIndiv[i]=PP[i]/P[i];
			// calcul des contacts (facultatif)
			NrC=T->NrClasseVent[src][v][i];
			NbCont[NrC]+=Contact;


		}
	}
#else
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		// calcul des contacts (facultatif)
		NrC=T->NrClasseVent[src][v][i];
		NbCont[NrC]+=CoeffEx[i]*PPP[i];
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			T->CoeffIndiv[i]=PP[i]/P[i];
		}
	}
#endif
	for(NrC=0;NrC<T->NbClasseVent[v];NrC++){
		T->resGRPvent[sp][v][NrC]+=NbCont[NrC]*T->SrcWeightVent[src][v][NrC];// (facultatif)
	}
	delete [] NbCont;

	// calcul des couvertures
	for(i=0;i<NbIndiv;i++)Q[i]=1.0;
	for(NrC=0;NrC<T->NbClasseVent[v];NrC++)for(s=0;s<NbSupport;s++){
#ifdef OPTION_POLOGNE
		if(MWvent[s][v][NrC]<=+0.0)continue;
#else
		if(MW[s]<=+0.0 || MWvent[s][v][NrC]<=+0.0)continue;
#endif
		NbInsert=T->Support[src][s].NbInsert[sp];
		if(NbInsert<1)continue;
		Exclusif=T->fIndivRegion[src][T->Support[src][s].NrRegionForce];
		for(i=0;i<T->Support[src][s].SupVent[v][NrC]->NbCas;i++){
			NrI=T->Support[src][s].SupVent[v][NrC]->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			if(NrC!=T->NrClasseVent[src][v][NrI])continue;
			CoeffIndiv=T->CoeffIndiv[NrI];
			if(CoeffIndiv<=0)continue;
			p1000=T->Support[src][s].SupVent[v][NrC]->Indiv[i].Proba;
			p=PT1[p1000]*CoeffIndiv;
			q=1-p;
			if(p>TrusquinBinomial[p1000][2][1]){
				// Le Vecteur Binomial de x (VBx)
				// reconstitution de VBx avec le trusquin
				memset(VBx,0,TailleVB);
				NbVBx=(short)ceil(CoeffIndiv);
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				Repart=p;
				Trusquin=TrusquinBinomial[p1000][NbVBx];
				for(u=1;u<=NbVBx;u++){
					if(Trusquin[u]<Repart){
						VBx[u]=Trusquin[u];
						Repart-=Trusquin[u];
					}
					else {
						VBx[u]=Repart;
						break;
					}
				}
				NbVBx=u;
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				// transformation en exactement touchés
				Repart=VBx[NbVBx];
				for(u=NbVBx-1;u>0;u--){
					VBx[u]-=Repart;
					Repart+=VBx[u];
				}
				VBx[0]=1-Repart;
				// Correction de l'hypothèse de relation d'indépendance++++++
				if(m_fCalculCHI){
					double CoeffCHI=T->CoeffCHI;
					double PointGlisse;
					PointGlisse=VBx[1]*CoeffCHI;
					if(PointGlisse>+0.0){
						PointGlisse/=2.0;
						VBx[0]+=PointGlisse;
					}
				}
				// fin de la correction++++++++++++++++++++++++++++++++++++++

				// les non touchés:
				q=VBx[0];
			}
			else {
				// Correction de l'hypothèse de relation d'indépendance++++++
				if(m_fCalculCHI){
					double CoeffCHI=T->CoeffCHI;
					double PointGlisse;
					PointGlisse=p*CoeffCHI;
					if(PointGlisse>+0.0){
						PointGlisse/=2.0;
						q+=PointGlisse;
					}
				}
				// fin de la correction++++++++++++++++++++++++++++++++++++++
				if(NbInsert>1 && m_fCalculDupp){
					// préparation du cas qui suit...
					memset(VBx,0,TailleVB);
					VBx[0]=q;
					VBx[1]=p;
					NbVBx=1;
				}
			}

			if(NbInsert>1){
				if(m_fCalculDupp){
					// le fameux cas préparé ci-dessus...
					short f,NbBoucle=NbInsert-1;
					for(f=0;f<NbBoucle;f++)CalculDuplication(NbVBx,T->CoeffDupp,p);
					q=VBx[0];
				}
				else {
					q=pow(q,NbInsert);
				}
			}

			// OH, le beau calcul !
			Q[NrI]*=q;
			// OH, le beau calcul !
		}
	}

	// intégration
	Ventil=T->Ventil[src][sp][v];
	for(i=0;i<NbIndiv;i++){
		if(T->CoeffIndiv[i]<=0)continue;
		NrC=T->NrClasseVent[src][v][i];
		Ventil[NrC]+=(1.0-Q[i])*CoeffEx[i];
	}
}

void CTomjob::ConsolideResultat(char fCalculMarginal,char fZoom){
	short s,p,n,v,c,i;

	if(!fCalculMarginal){
		T->PopCibTotalGRP=0;
		T->PopCibTotalCOUV=0;
		T->NbCasTotal=0;
		for(s=0;s<T->NbSource;s++){
			T->PopCibTotalGRP+=T->PopCibleGRP[s];
			T->PopCibTotalCOUV+=T->PopCibleCOUV[s];
			for(i=0;i<T->NbIndivC[s];i++)if(T->CoeffExCible[s][i]>0L)T->NbCasTotal++;
		}
	}

	if(fZoom){
		for(n=0;n<T->NbFoisToucheMax;n++)T->Zoom_DistribExact[n]=+0.0;
		for(s=0;s<T->NbSource;s++){
			if(s==m_NewSrc)for(n=0;n<T->NbFoisToucheMax;n++)T->Zoom_DistribExact[n]+=T->Zoom_Distrib[n+1]*T->SrcWeight[s];
			else {
				for(n=0;n<T->NbFoisToucheMax;n++)T->Zoom_DistribExact[n]+=T->Distrib[s][0][n+1]*T->SrcWeight[s];
				for(;n<NBFOISTOUCHEMAXMARGINAL;n++)T->Zoom_DistribExact[T->NbFoisToucheMax-1]+=T->Distrib[s][0][n+1]*T->SrcWeight[s];
			}
		}
#ifndef OPTION_MEDIACONSULT
		for(n=0;n<T->NbFoisToucheMax;n++)T->Zoom_DistribExact[n]*=100.0/(double)T->PopCibTotalGRP;
#endif

		// la distribution cumulée
		n=T->NbFoisToucheMax-1;
		T->Zoom_DistribCumulee[n]=T->Zoom_DistribExact[n];
		for(n--;n>=0;n--)T->Zoom_DistribCumulee[n]=T->Zoom_DistribCumulee[n+1]+T->Zoom_DistribExact[n];
#ifdef OPTION_MEDIACONSULT
		T->Zoom_NbIndivT=T->Zoom_DistribCumulee[0];
#else
		T->Zoom_NbIndivT=T->PopCibTotalGRP*T->Zoom_DistribCumulee[0]/100.0;
#endif

//		return; // arret utile pour la vérification
		// on retranche les résultats trouvés à celui du plan
		T->Zoom_NbContact-=T->NbContact[0];
		T->Zoom_NbIndivT-=T->NbIndivT[0];
#ifdef OPTION_MEDIACONSULT
		T->Zoom_NbContactValide-=T->NbContactValide[0];
#endif		T->Zoom_CouvertureValide-=T->CouvertureValide[0];
		for(n=0;n<T->NbFoisToucheMax;n++){
			T->Zoom_DistribCumulee[n]-=T->DistribCumulee[0][n];
			T->Zoom_DistribExact[n]-=T->DistribExact[0][n];
		}
		return; // fin du traitement pour le zoom
	}

	for(p=0;p<T->NbPlan;p++){
		for(n=0;n<T->NbFoisToucheMax;n++)T->DistribExact[p][n]=+0.0;
		for(s=0;s<T->NbSource;s++){
			for(n=0;n<T->NbFoisToucheMax;n++)T->DistribExact[p][n]+=T->Distrib[s][p][n+1]*T->SrcWeight[s];
		}
#ifndef OPTION_MEDIACONSULT
		for(n=0;n<T->NbFoisToucheMax;n++)T->DistribExact[p][n]*=100.0/(double)T->PopCibTotalGRP; //T->PopCibTotalCOUV;
#endif
		// la distribution cumulée
		n=T->NbFoisToucheMax-1;
		T->DistribCumulee[p][n]=T->DistribExact[p][n];
		for(n--;n>=0;n--)T->DistribCumulee[p][n]=T->DistribCumulee[p][n+1]+T->DistribExact[p][n];
#ifdef OPTION_MEDIACONSULT
		T->NbIndivT[p]=T->DistribCumulee[p][0];
#else
		T->NbIndivT[p]=T->PopCibTotalGRP*T->DistribCumulee[p][0]/100.0;
#endif
	}

	if(T->NbVentilation>0){
		// les populations ventilées:
		// si le GRP ventile, c'est la population de la source de GRP qui est valide
		// si la  source de couverture ventile, alors c'est la source de C% qui est valide

		// calcul pour src C%
		for(v=0;v<T->NbVentilation;v++){
			for(s=0;s<T->NbSource;s++){
				for(i=0;i<T->NbIndivC[s];i++){
					c=T->NrClasseVent[s][v][i];
					T->PopventCouv[v][c]+=T->CoeffExRed[s][i];
					if(T->CoeffExCible[s][i]>0L) T->NbCasVentilation[v][c]++;
				}
			}
		}

#ifndef OPTION_MEDIACONSULT

		// calcul pour src GRP
		for(v=0;v<T->NbVentilation;v++){
			for(c=0;c<T->NbClasseVent[v];c++){
				for(s=0;s<T->NbSource;s++){
					T->PopventGRP[v][c]+=T->PopGRPclassVent[s][v][c];
				}
			}
		}

		// les GRP et C% ventilés sont calculés sur la base de la population ventilé
		for(p=0;p<T->NbPlan;p++){
			for(v=0;v<T->NbVentilation;v++){
				for(c=0;c<T->NbClasseVent[v];c++){
					if(T->PopventGRP[v][c]>0)T->resGRPvent[p][v][c]*=100.0/T->PopventGRP[v][c]; // T->PopventCouv[v][c]; //T->PopCibTotal;
					else T->resGRPvent[p][v][c]=0.0;
					for(s=0;s<T->NbSource;s++){
						T->resCOUVvent[p][v][c]+=T->Ventil[s][p][v][c]*T->SrcWeightVent[s][v][c];
					}
					if(T->PopventGRP[v][c]>0)T->resCOUVvent[p][v][c]*=100.0/T->PopventGRP[v][c]; // T->PopventCouv[v][c]; //T->PopCibTotal;
					else T->resCOUVvent[p][v][c]=0.0;
				}
			}
		}
#endif
	}
}



// Cette fonction est utilisée pour un job Multi-Média
// Et Les sources sont toujours les mêmes...
// Chacunes représentent un média
// Pour l'instant cette fonction accepte:
// Ni zoom,ni ventilations, ni multiplan..., et 2 Médias
// L'intégration n'a pas encore eut lieu.
void CTomjob::CalculTableauCroise(char fCalculMarginal,char fZoom){

	int i;

	T->PopCibTotalGRP=T->PopCibleGRP[0];
	T->PopCibTotalCOUV=T->PopCibleCOUV[0];
	T->NbCasTotal=0;
	for(i=0;i<T->NbIndivC[0];i++)if(T->CoeffExCible[0][i]>0L)T->NbCasTotal++;

	if(fZoom){
		AfficheErreur("ERREUR: Calcul croisé du zoom à développer...");
		return;
	}

	if(T->NbVentilation>0){
		AfficheErreur("ERREUR: Calcul croisé avec des ventilations à développer...");
		return;
	}
	if(T->NbPlan>1){
		AfficheErreur("ERREUR: Calcul croisé avec plusieurs plans à développer...");
		return;
	}

	if(T->NbSource!=2){
		AfficheErreur("ERREUR: Calcul croisé avec deux médias seulement. Sinon, à développer...");
		return;
	}

	if(T->SrcWeight[0]!=1){
		AfficheErreur("ERREUR: Calcul croisé avec un SrcWeight!=1 !!! à développer...");
		return;
	}

	// Les variables pour gazzer
	short NbIndiv=T->NbIndivC[0];
	double **VB1=T->Margi_VB[0]; // Média 1: Radio
	double **VB2=T->Margi_VB[1]; // Média 2: Télé
	double *CI=T->Margi_CoeffIndiv[0];
	double *B1,*B2;
	double CoeffEx;
	int n1,n2,nb,size;
	nb=T->NbFoisToucheMax+1;
	size=nb*sizeof(double);

	// Allocation d'une matrice pour calcul par individu
	double ** M;
	M=new double*[nb];
	for(n1=0;n1<nb;n1++)M[n1]=new double[nb];

	// Préparation du Tableau croisé
	for(n1=0;n1<nb;n1++){
		memset(m_TabCroise[n1],0,size);
		memset(m_TabCroiseValide[n1],0,size);
	}

	// C'est parti Emile...
	for(i=0;i<NbIndiv;i++)
	{
		//if(CI[i]<=0)continue;
		CoeffEx=T->CoeffExRed[0][i];
		if(CoeffEx<=0)continue;
		B1=VB1[i];
		B2=VB2[i];

		// Reset de la matrice
		for(n1=0;n1<nb;n1++)memset(M[n1],0,size);

		// Calcul croisé de cet individu
		for(n1=0;n1<nb;n1++){
			for(n2=0;n2<nb;n2++){
				M[n1][n2]=B1[n1]*B2[n2];
			}
		}

		// Intégration de l'individu dans le tableau croisé
		if(T->Beta>0){ // Couverture validée
			for(n1=0;n1<nb;n1++){
				for(n2=0;n2<nb;n2++){
					m_TabCroiseValide[n1][n2]+=M[n1][n2]*CoeffEx*m_TabBeta[n1][n2];
				}
			}
		}

		// Couverture cumulée
		for(n1=0;n1<nb;n1++){
			for(n2=0;n2<nb;n2++){
				m_TabCroise[n1][n2]+=M[n1][n2]*CoeffEx;
			}
		}
	}

	// Transformation des individus touchés en %
	double ratioGRP=100.0/(double)T->PopCibTotalGRP;
	for(n1=0;n1<nb;n1++){
		for(n2=0;n2<nb;n2++){
			m_TabCroise[n1][n2]*=ratioGRP;
			if(T->Beta>0)m_TabCroiseValide[n1][n2]*=ratioGRP;
			else m_TabCroiseValide[n1][n2]=m_TabCroise[n1][n2];
		}
	}

	// Liberation de la matrice des individus
	for(n1=0;n1<nb;n1++)delete [] M[n1];
	delete [] M;
}



void CTomjob::SetVecteurBinomialPreCalcule(int NrMedia,double **VB)
{
	for (int i=0;i<T->NbIndivC[NrMedia];i++)
	{
		for (int c=0;c<=NBFOISTOUCHEMAXZOOM;c++)
		{
			T->Margi_VB[NrMedia][i][c]=VB[i][c];
//			T->m_TabCroise[NrMedia][i][c]=VB[i][c];
		}
	}

#ifdef _DEBUG
	{
		double couv=0;
		for (int i=0;i<T->NbIndivC[NrMedia];i++)
		{
			couv+=(1-T->Margi_VB[NrMedia][i][0])*T->CoeffExRed[NrMedia][i];

		}
		CString txt;
		txt.Format("Couverture CTOMJOB -> %4.2f%%",couv);
		TRACE ("Couverture CTOMJOB -> .0f individus %4.2f%%",couv,100*couv/T->PopCibleCOUV[NrMedia]);
	}
#endif

	CalculTableauCroise();
}

// Dans Zoom radio - Télé
// 0:Radio 1:Télé
void CTomjob::CalculQuartiles(int NrSrc)
{
	// Les variables pour gazzer
	short NbIndiv=T->NbIndivC[0];
	double **VB;
	VB=T->Margi_VB[NrSrc];
	double *CI=T->Margi_CoeffIndiv[0];
	double *B;
	double CoeffEx;
	int n,nb,size;
	nb=T->NbFoisToucheMax+1;
	size=nb*sizeof(double);

	double *V;
	V=new double[nb];
	memset(V,0,size);
	int i;

	// Calcul de la distribution Télé
	for(i=0;i<NbIndiv;i++){
		CoeffEx=T->CoeffExRed[0][i];
		if(CoeffEx<=0)continue;
		B=VB[i];

		// Intégration de l'individu dans le tableau croisé
		for(n=0;n<nb;n++){
			V[n]+=B[n]*CoeffEx;
		}
	}

	// Transformation des individus touchés en %
	double ratioGRP=100.0/(double)T->PopCibTotalGRP;
	for(n=0;n<nb;n++){
		V[n]*=ratioGRP;
	}

	// Calcul de la couverture à 1 et +
	double couv=+0.0;
	for(n=1;n<nb;n++){
		couv+=V[n];
	}

	for(n=0;n<5;n++){
		m_Quartiles[n]=m_GRPQuartiles[n]=+0.0;
	}

	// Les non touchés
	m_Quartiles[0]=V[0];
	m_GRPQuartiles[0]=0;

	// Recherche des seuils des quartiles
	double cumul=+0.0;
	for(n=1;n<nb;n++){
		cumul+=V[n];
		m_Quartiles[1]+=V[n];
		m_GRPQuartiles[1]+=V[n]*n;
		if(cumul>=couv*0.25)break;
	}
	m_Seuil1=n;
	if(m_Seuil1>=nb)m_Seuil1=nb-1;
	for(n++;n<nb;n++){
		cumul+=V[n];
		m_Quartiles[2]+=V[n];
		m_GRPQuartiles[2]+=V[n]*n;
		if(cumul>=couv*0.5)break;
	}
	m_Seuil2=n;
	if(m_Seuil2>=nb)m_Seuil2=nb-1;
	for(n++;n<nb;n++){
		cumul+=V[n];
		m_Quartiles[3]+=V[n];
		m_GRPQuartiles[3]+=V[n]*n;
		if(cumul>=couv*0.75)break;
	}
	m_Seuil3=n;
	if(m_Seuil3>=nb)m_Seuil3=nb-1;
	// le dernier quartile
	for(n++;n<nb;n++){
		m_Quartiles[4]+=V[n];
		m_GRPQuartiles[4]+=V[n]*n;
	}

	delete [] V;
}

// Dans Zoom radio - Télé
// 0:Radio 1:Télé
void CTomjob::CalculApportScanner(int NrSrc){

	// Les variables pour gazzer
	short NbIndiv=T->NbIndivC[0];
	double **VB=T->Margi_VB[NrSrc];

	int i;

	// Calcul de l'apport
	m_ApportScanner=+0.0;
	for(i=0;i<NbIndiv;i++){
		if(m_CoeffExComp[i]<=0)continue;
		// Intégration de l'individu
		// Pour le média avec le critère de complémentarité
		m_ApportScanner+=(1-VB[i][0])*m_CoeffExComp[i];
	}
}

void CTomjob::CalculCibleComplementaire(int NrSrc,char flag){

	// Les variables pour gazzer
	short NbIndiv=T->NbIndivC[0];
	double **VB=T->Margi_VB[NrSrc];
	double *CI=T->Margi_CoeffIndiv[0];
	double *B;
	double CoeffEx;
	int n,nb,size;
	nb=T->NbFoisToucheMax+1;
	size=nb*sizeof(double);

	int i;

	if(!m_CoeffExComp)m_CoeffExComp=new double[NbIndiv];

	// Calcul de l'apport
	m_ApportScanner=+0.0;
	for(i=0;i<NbIndiv;i++){
		CoeffEx=T->CoeffExRed[NrSrc][i];
		m_CoeffExComp[i]=0;
		if(CoeffEx<=0)continue;
		B=VB[i];

		// Calcul du coefficient de complémentarité
		double cc=0;
		if(flag & 1)cc+=B[0];
		if(flag & 2)for(n=1;n<=m_Seuil1;n++)cc+=B[n];
		if(flag & 4)for(n=m_Seuil1+1;n<=m_Seuil2;n++)cc+=B[n];
		if(flag & 8)for(n=m_Seuil2+1;n<=m_Seuil3;n++)cc+=B[n];
		if(flag & 16)for(n=m_Seuil3+1;n<=nb;n++)cc+=B[n];
		if(cc<=0)continue;
		m_CoeffExComp[i]=cc*CoeffEx;
	}
}


void CTomjob::CalculCouvertureValide(char fCalculMarginal,char fZoom){

	short p,n;
	double * ptrDistribCumulee;


#ifndef OPTION_MEDIACONSULT
	if(fZoom){
		T->NbClasseContact=NBFOISTOUCHEMAXZOOM;
		return;
	}
#endif

	// calcul du NbClasseContact
	T->NbClasseContact=0;
	for(p=0;p<T->NbPlan;p++){
		if(fZoom)ptrDistribCumulee=T->Zoom_DistribCumulee;
		else ptrDistribCumulee=T->DistribCumulee[p];
		for(n=0;n<T->NbFoisToucheMax;n++){
#ifdef OPTION_MEDIACONSULT
		if(ptrDistribCumulee[n]>=1000 && n+1>T->NbClasseContact)T->NbClasseContact=n+1;
#else
		if(ptrDistribCumulee[n]>=0.005 && n+1>T->NbClasseContact)T->NbClasseContact=n+1;
#endif
		}
	}
	T->NbClasseContact++;
	if(T->NbClasseContact>NBCLASSEMAXRESULTAT)T->NbClasseContact=NBCLASSEMAXRESULTAT;
	if(T->NbClasseContact>T->NbFoisToucheMax)T->NbClasseContact=T->NbFoisToucheMax;

#ifndef OPTION_MEDIACONSULT
	CalculCourbeReponse(fCalculMarginal);
#endif

	// calcul de la couverture validée
	for(p=0;p<T->NbPlan;p++){
		if(fZoom)T->Zoom_CouvertureValide=0;
		else T->CouvertureValide[p]=0;
		for(n=0;n<T->NbFoisToucheMax;n++){
			if(fZoom)T->Zoom_CouvertureValide+=T->Zoom_DistribExact[n]*T->CourbeReponse[n];
			else T->CouvertureValide[p]+=T->DistribExact[p][n]*T->CourbeReponse[n];
		}
	}
}

void CTomjob::CalculCourbeReponse(char fCalculMarginal)
{

	double Beta0; //,Beta;
	Beta0=0;
	int n;
#ifndef COURBEDEREPONSEIMPOSEE
	if(T->Beta<0){
		if(!fCalculMarginal)for(n=0;n<T->NbFoisToucheMax;n++)T->CourbeReponse[n]=1.0;
		return;
	}
#endif

#ifdef COURBEDEREPONSEIMPOSEE
	for(n=0;n<T->NbFoisToucheMax;n++){
		int n2=min(n,T->CourbeReponseImpose.GetSize()-1);
		if(n2>=0)T->CourbeReponse[n]=T->CourbeReponseImpose[n2]/100.0;
		else T->CourbeReponse[n]=+1.0;
	}
#else
	if(!fCalculMarginal){
#ifdef REPETITION_OPTIMALE
		Beta0=CalculBetaZero(T->Beta);
#else
		Beta0=(double)T->Beta/100.0;
#endif //REPETITION_OPTIMALE
		for(n=0;n<T->NbFoisToucheMax;n++){
			T->CourbeReponse[n]=1-pow(1-Beta0,n+1);
		}
	}
#endif //COURBEDEREPONSEIMPOSEE
}



double CTomjob::CalculBetaZero(short coeff){
	double resultat0a20[]={1.0 , .86 , .63 , .48 , .39 , .32 , .28 , .24 , .22 , .20 , .18 , .16 , .15 , .14 , .13 , .12 , .12 , .11 , .10 , .10 , .9};
	if(coeff<=0)return(resultat0a20[0]);
	if(coeff>=20)return(resultat0a20[20]);
	return(resultat0a20[coeff]);
}


// !!! 1 variable par référence et
// calcul sur le vercteur globale vbx
void CTomjob::CalculDuplication(unsigned short & Nb,double Coeff,double NbContSpot)
{
	unsigned short i,t;
	double PointGlisse,Nouveaux;

//	Coeff=1.0;


		// une petiti vérif ...
/*	double s=+0.0;
	for(i=0;i<=Nb;i++)s+=VBx[i];
	if(s>1.00000001){
		AfficheMessage("we have a pb !");
	}*/


	Nouveaux=+0.0;
	memcpy(VBdupp,VBx,TailleVB);
	for(i=Nb;i>0;i--){
		if(i==NBFOISTOUCHEMAXSTD){
			Nouveaux+=VBx[i]*Coeff;
		}
		else {
			PointGlisse=VBx[i]*Coeff;
			t=i+1;
			VBx[t]+=PointGlisse;
			Nouveaux+=PointGlisse;
			VBx[i]-=PointGlisse;
		}
	}
	Nouveaux=NbContSpot-Nouveaux;
	if(Nouveaux<0){
		// trop ont été touchés...
		// ce cas est normallement impossible
		Nouveaux=+0.0;
	}

	if(Nouveaux>VBx[0]){
		// perte de GRP du à un coeff inssufisant
		// on pourrait chiader le truc, mais cela
		// ne changera, à mon avis, pas grand chose.
		// on cosidère donc 100% !
		for(i=2;i<=NBFOISTOUCHEMAXSTD;i++){
			VBx[i]=VBdupp[i-1];
		}
		VBx[NBFOISTOUCHEMAXSTD]+=VBdupp[NBFOISTOUCHEMAXSTD];
		VBx[0]=VBdupp[0];
		VBx[1]=+0.0;
	}
	else {
		VBx[0]-=Nouveaux;
		VBx[1]+=Nouveaux;
	}


	// on augmente toujours de 1
	Nb++;
	if(Nb>NBFOISTOUCHEMAXSTD)Nb=NBFOISTOUCHEMAXSTD;

	// une petiti vérif ...
/*	s=0;
	for(i=0;i<=Nb;i++)s+=VBx[i];
	if(s>1.00000001){
//		AfficheMessage("we have a pb !");
	}*/
}


void CTomjob::EvaluerApportJournalier(short src,short jour)
{

	short sp=0; // toujours sur le premier plan
	unsigned short i,s,f,NrI,u,NbVBx;

	double p,q;

	// duplicata
	short p1000;
	double *B;
	short NbIndiv,NbSupport;
	double *CoeffEx;
	double CoeffIndiv,*CI;
	short NbInsert,NbBoucle;
	double *P,*PP,*MW;
#ifndef OPTION_POLOGNE
	double *PPP,*MWW;
#endif
	double **VB;
	bool *Exclusif;
	double Repart;
	double *Trusquin;

	if((!src) && (!jour)){
		// On reset tout
		for(s=0;s<T->NbSupport[src];s++){
			T->Support[src][s].fJour=false;
		}
	}

	// on marque les supports à prendre pour ce jour
	for(s=0;s<T->NbSupport[src];s++){
		if(!strcmp(T->Support[src][s].Prive,T->Jour[jour].Libelle))T->Support[src][s].fJour=true;
	}

	// On Recal des MédiaWeight sur ces insertions ?
	// >Non, ils sont déjà calculés, et justes en plus!

	// on duplique ce qui sert souvent
	CoeffEx=T->CoeffExRed[src];
	NbIndiv=T->NbIndivC[src];
	SSupport *Support;
	Support=T->Support[src];
	NbSupport=T->NbSupport[src];
	P=T->P;
	PP=T->PP;
	MW=T->MW;
	VB=T->VB;
	CI=T->CoeffIndiv;
#ifndef OPTION_POLOGNE
	PPP=T->PPP;
	MWW=T->MWW;
#endif

	// Calcul de P et PP(Pprime)
	// Calcul de P[i] et PP[i]
	memset(P,0,sizeof(double)*NbIndiv);
	memset(PP,0,sizeof(double)*NbIndiv);
#ifndef OPTION_POLOGNE
	memset(PPP,0,sizeof(double)*NbIndiv);
#endif
	for(s=0;s<NbSupport;s++){
#ifdef OPTION_POLOGNE
		if(MW[s]<=+0.0)continue;
#endif
		if(!Support[s].fJour)continue;
		NbInsert=Support[s].NbInsert[sp];
		if(NbInsert<1)continue;
		Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
		for(i=0;i<Support[s].Sup->NbCas;i++){
			NrI=Support[s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			p=PT1[Support[s].Sup->Indiv[i].Proba]*NbInsert;
#ifdef OPTION_POLOGNE
			P[NrI]+=p;
			PP[NrI]+=p*MW[s];
#else
			if(MW[s]>0.0){
				P[NrI]+=p;
				PP[NrI]+=p*MW[s];
			}
			PPP[NrI]+=p*MWW[s];
#endif
		}
	}

	// Initialisation du vecteur binomial
	for(i=0;i<NbIndiv;i++)memcpy(VB[i],B0,TailleVB);
	memset(T->Margi_Marquage,1,T->NbIndivC[src]);

	// calcul du Coeff. flottant et du NbContact
	memset(CI,0,sizeof(double)*NbIndiv);
	for(i=0;i<NbIndiv;i++){
		if(CoeffEx[i]<=0)continue;
		if((P[i]>+0.0) && (PP[i]>+0.0)){
			CI[i]=PP[i]/P[i];
		}
	}

	// calcul du vecteur binomial pour chaque individu
	if(m_fCalculCHI){
		double CoeffCHI=T->CoeffCHI;
		for(s=0;s<NbSupport;s++){
			if(MW[s]<=+0.0)continue;
			if(!Support[s].fJour)continue;
			NbInsert=Support[s].NbInsert[sp];
			if(NbInsert<1)continue;
			NbBoucle=NbInsert;
			Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
			for(i=0;i<Support[s].Sup->NbCas;i++){
				NrI=Support[s].Sup->Indiv[i].NrIndiv;
				if(!Exclusif[NrI])continue;
				if(!T->Margi_Marquage[NrI])continue;
				CoeffIndiv=CI[NrI];
				if(CoeffIndiv<=0.0)continue;
				p1000=Support[s].Sup->Indiv[i].Proba;
				B=VB[NrI];
				p=PT1[p1000]*CoeffIndiv;
				q=1-p;
				{ // *** calcul en MACRO-EVALUATION ***
					// Le Vecteur Binomial de x (VBx)
					// reconstitution de VBx avec le trusquin
					memset(VBx,0,TailleVB);
					NbVBx=(short)ceil(CoeffIndiv);
					if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
					Repart=p;
					Trusquin=TrusquinBinomial[p1000][NbVBx];
					for(u=1;u<=NbVBx;u++){
						if(Trusquin[u]<Repart){
							VBx[u]=Trusquin[u];
							Repart-=Trusquin[u];
						}
						else {
							VBx[u]=Repart;
							break;
						}
					}
					NbVBx=u;
					if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
					// transformation en exactement touchés
					Repart=VBx[NbVBx];
					for(u=NbVBx-1;u>0;u--){
						VBx[u]-=Repart;
						Repart+=VBx[u];
					}
					VBx[0]=1-Repart;

					// Correction de l'hypothèse de relation d'indépendance++++++
					if(NbVBx==1)NbVBx++;
					double PointGlisse;
					PointGlisse=VBx[1]*CoeffCHI;
					if(PointGlisse>+0.0){
						VBx[1]-=PointGlisse;
						PointGlisse/=2.0;
						VBx[2]+=PointGlisse;
						VBx[0]+=PointGlisse;
					}

					// calcul des duplications
					if(NbInsert>1 && m_fCalculDupp){
						NbBoucle=NbInsert-1;
						for(f=0;f<NbBoucle;f++)CalculDuplication(NbVBx,T->CoeffDupp,p);
						NbBoucle=1;
					}


					// calcul des non-touchés uniquement
					for(f=0;f<NbBoucle;f++){
						B[0]*=VBx[0];
					}
				}
			}
		}
	}
	else for(s=0;s<NbSupport;s++){
		if(MW[s]<=+0.0)continue;
		// Calcul + rapide ? sans calcul issue du coeff CHI
		// si le trusquin n'est pas utilisé,
		// c'est une micro-évaluation qui est faite,
		// à la place de la macro-évaluation.
		if(!Support[s].fJour)continue;
		NbInsert=Support[s].NbInsert[sp];
		if(NbInsert<1)continue;
		Exclusif=T->fIndivRegion[src][Support[s].NrRegionForce];
		for(i=0;i<Support[s].Sup->NbCas;i++){
			NrI=Support[s].Sup->Indiv[i].NrIndiv;
			if(!Exclusif[NrI])continue;
			if(!T->Margi_Marquage[NrI])continue;
			CoeffIndiv=CI[NrI];
			if(CoeffIndiv<=0.0)continue;
			p1000=Support[s].Sup->Indiv[i].Proba;
			B=VB[NrI];
			p=PT1[p1000]*CoeffIndiv;
			q=1-p;
			if(p<=TrusquinBinomial[p1000][2][1]){
				// calcul des non-touchés uniquement
				q=pow(q,NbInsert);
				B[0]*=q;
			}
			else { // *** calcul en MACRO-EVALUATION ***
				// Le Vecteur Binomial de x (VBx)
				// reconstitution de VBx avec le trusquin
				memset(VBx,0,TailleVB);
				NbVBx=(short)ceil(CoeffIndiv);
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				Repart=p;
				Trusquin=TrusquinBinomial[p1000][NbVBx];
				for(u=1;u<=NbVBx;u++){
					if(Trusquin[u]<Repart){
						VBx[u]=Trusquin[u];
						Repart-=Trusquin[u];
					}
					else {
						VBx[u]=Repart;
						break;
					}
				}
				NbVBx=u;
				if(NbVBx>MEDIAWEIGHTMAX)NbVBx=MEDIAWEIGHTMAX;
				// transformation en exactement touchés
				Repart=VBx[NbVBx];
				for(u=NbVBx-1;u>0;u--){
					VBx[u]-=Repart;
					Repart+=VBx[u];
				}
				VBx[0]=1-Repart;

				// calcul des non-touchés uniquement
				q=pow(VBx[0],NbInsert);
				B[0]*=q;
			}
		}
	}

	// intégration
	for(i=0;i<NbIndiv;i++){
		if(CI[i]<=0)continue;
		B=VB[i];
		T->Jour[jour].Individu+=(1-B[0])*CoeffEx[i]*T->SrcWeight[src];
	}
}

void CTomjob::TrouveJourActif(short src)
{
	int s,j;
	SJour J;
	if(!src)T->Jour.RemoveAll();
	for(s=0;s<T->NbSupport[src];s++){
		for(j=0;j<T->Jour.GetSize();j++){
			if(!strcmp(T->Jour[j].Libelle,T->Support[src][s].Prive))goto suivant;
		}
	// on ajoute le jour
		J.Individu=0;
		strcpy(J.Libelle,T->Support[src][s].Prive);
		T->Jour.Add(J);
suivant:;
	}
}

