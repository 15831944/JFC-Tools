// CalculMemoDememo.cpp: implementation of the CCalculMemoDememo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "CalculMemoDememo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalculMemoDememo::CCalculMemoDememo()
{
}

CCalculMemoDememo::~CCalculMemoDememo()
{
}

// Pour avoir les variables dans les fichiers
// #define DEBUGGRAPHE
CCalculMemoDememo::CResultat  CCalculMemoDememo::CalculCourbeMemoDememo(CCalculMemoDememo::CParametre * ParamCalcul)
{
	m_Parametre=ParamCalcul;

	MiseAJourIndex();

	unsigned char SMax,ASMax,tps,PremiereSemaine;
	int T,i,BlocSuper;
//	DEFBLOCS *Block; REMI_CINEMA
	char *b, *v, *z, *s, *p, *w;
	float d,d_,c,c_,dmoy,dmoy_,a_,jp,kp_,lp2_,lp1_,j,k_,l2_,l1_,RM,RA,RD2,RD1,n_,n,m_,delta2_,delta1_;
	float Ac_,An_,Ar_,Am_,Adelta1_,Adelta2_,Admoy_,Aro_,ro_;
	float beta,teta,gamma,pi,coef,tmbeta,tmgama,tmteta,RMv;
	float *M_;
	// depuis avril 1996 :
	// AncienRo_ est le ro_ de la semaine d'avant
	// Aro_ est le ro_ du début de la vague
	// Aepsi_ est le epsi_ de la semaine d'avant calculé depuis le début de la vague
	float epsi,epsi_,Aepsi_,ARM,AncienRo_;
	float rovirt_;
	// duplicata de téta en phi pour les démémorisés pendant la vague
	float phi,tmphi;
	float KdeDememo;

	float *TabC_, *TabD_;

#ifdef DEBUGGRAPHE
	FILE* fd,*fd1,*fd2,*fd3;
#endif
	if(ParamCalcul->m_OptionCalcul_DememoPendantMemo)KdeDememo=1.0;	else KdeDememo=0.0;

	b=m_Index.b;
	v=m_Index.v;
	w=m_Index.w;
	z=m_Index.z;
	s=m_Index.s;
	p=m_Index.p;
//	pi=(float)Fen->Abaques[iCib].Effectif;
	pi=ParamCalcul->m_BasePopulation;
	T=ParamCalcul->m_NbSemaineCalcul;
	//Block=Calend->DefBlocs;
	M_=new float[T];
	TabC_=new float[T];
	TabD_=new float[T];

	// allouer les autres tableau pr GRP et nb contact

	RA=RD2=RD1=RM=RMv=0;
	ASMax=0;
	Admoy_=0.0;
	Ac_=0.0;
	An_=Ar_=Am_=0.0;
	Adelta2_=Adelta1_=0.0;
	AncienRo_=Aro_=0.0;
	Aepsi_=epsi_=0.0;
	rovirt_=0.0;
	SMax=Borne(s,0,T);
#ifdef DEBUGGRAPHE
	fd=fopen("Test.dat","wt");
	fd1=fopen("Test1.dat","wt");
	fd2=fopen("Test2.dat","wt");
	fd3=fopen("test3.dat","wt");
	//fd2=fopen("Test2.dat","wt");
	fprintf(fd," t   s   z   b   v   w   p   c     c_    d     d_    a_  beta      jp    \n");
    fprintf(fd1,"   kp_       lp_        j         k_        l_        delta_   n         epsi     \n");
    fprintf(fd3,"     n_        m_        ro_      M_    RA     RD    RM    S    TMBETA    TMGAMA   TMTETA\n");
#endif

//	Fen->PtMemoCamp[iCib][Hypo][Bino]=0.0; // pt memo de la campagne
//    Calend->BudgetCamp[ligne]=0.0; // budget de la campagne

	// calcul lors de reliquat
	tps=0;
	while (!b[tps]) {
		if(tps>=T){
			break;
		}
		M_[tps]=0;
		TabC_[tps]=0;
		TabD_[tps]=0;
//		Calend->grp[ligne][tps]=0.0; INUTILE POUR LE CINEMA
		tps++;
	}
	// initialiser a zero les tableaux GRP et couverture
	PremiereSemaine=tps;

	m_=c=c_=d=d_=(float)0.0;


	for(;tps<T;tps++) { // boucle sur l'ensemble de la campagne
		// calcul des grandeurs indépendantes

#ifdef DEBUGGRAPHE
	fprintf(fd2,"\n\n// calcul des grandeurs indépendantes\n");
	fprintf(fd2,"NbChanceSem[%d][%d]=%d\n",Hypo,tps,NbChanceSem[Hypo][tps]);
#endif
		
		
		if(tps<ParamCalcul->m_NbSemaine && ParamCalcul->m_NbChanceSem[tps]>0){
			c=c_=d=d_=+0.0;
			for(i=1;i<=ParamCalcul->m_NbChanceSem[tps];i++) {
				d+=(float)i*(ParamCalcul->m_NbContactBarre[tps][i-1]);
				c+=(float)(ParamCalcul->m_NbContactBarre[tps][i-1]);
#ifdef DEBUGGRAPHE
	fprintf(fd2,"NbContactBarre[%d][%d][%d][%d]=%.0f\n",iCib,Hypo,tps,i-1,NbContactBarre[iCib][Hypo][tps][i-1]);
#endif


			}
			for(i=1;i<=ParamCalcul->m_NbChanceCum[tps];i++) 
			{
				d_+=(float)i*(ParamCalcul->m_NbContactPlaque[tps][i-1]);
				c_+=(float)(ParamCalcul->m_NbContactPlaque[tps][i-1]);
			}
			if(c>+0.0)dmoy=d/c; else dmoy=+0.0;
			if(c_>+0.0)dmoy_=d_/c_; else dmoy_=+0.0;
		}

//	recopie des valeurs couverture et distribution pour le tableau de la courbe de mémo-démémo
		TabC_[tps]=c_;
		TabD_[tps]=d_;
		if(c==+0.0){
#ifdef DEBUGGRAPHE
	fprintf(fd2,"\n\nc==+0.0  -> goto PhaseDememorisation;\n");
#endif
			goto PhaseDememorisarion;
		}


		if (v[tps]) { // on se trouve sur une vague ->Mémorisation
			if (tps>PremiereSemaine) BlocSuper=(b[tps]-b[tps-1]);
			else BlocSuper=(b[tps]);
			// cas de bloc non superposé
			if (BlocSuper==1){ // *******************************************************
				a_=Ac_+c-c_;
				//Calend->grp[ligne][tps]=d/pi*100.0;  // calcul grp

				// calcul des assiettes fictives
				jp=lp1_=lp2_=kp_=0.0;
				if (tps==PremiereSemaine || Admoy_==+0.0)gamma=1;
				else gamma=1.0+0.1*(POWTEST(((float)(z[tps-1])/Admoy_),1.5)-1);
				teta=(float)ASMax/((float)ASMax+2.0*(Admoy_+1.0)*(Admoy_+2.0));
				phi=(float)1.0/((float)1+2.0*(Admoy_+1.0)*(Admoy_+2.0));
				coef=((p[tps]==1)?0.0:1.0);

#ifdef DEBUGGRAPHE
	fprintf(fd2,"\n\nbloc %d \n",b[tps]);
#endif



				// recherche du beta...
				//beta=Fen->Index[Hypo][Bino].ModeleFormat[tps][0]->Beta[i-1];
				//beta=Fen->Index[Hypo][Bino].ModeleFormat[tps][tp-1]->Beta[i-1];
				// A FAIRE

				for(i=1;i<=ParamCalcul->m_NbChanceSem[tps];i++) {
					beta=BetaSuivantModele(ParamCalcul->m_ValBeta[min(i-1,19)],ParamCalcul->m_DureeMoy[tps]);
					jp=jp+beta*ParamCalcul->m_NbContactBarre[tps][i-1];
					kp_=kp_+(beta/gamma)*ParamCalcul->m_NbContactPlaque[tps][i-1];
					if(beta!=0){
						lp1_=lp1_+POWTEST(beta,phi)*(float)ParamCalcul->m_NbContactBarre[tps][i-1];
						lp2_=lp2_+coef*POWTEST(beta,teta)*(float)ParamCalcul->m_NbContactBarre[tps][i-1];
					}
					else {
						lp1_=lp1_;// pour éviter le domain error
						lp2_=lp2_;// pour éviter le domain error
					}
				}
				for(;i<=ParamCalcul->m_NbChanceCum[tps];i++) {
					beta=BetaSuivantModele(ParamCalcul->m_ValBeta[min(i-1,19)],ParamCalcul->m_DureeMoy[tps]);
					kp_=kp_+(beta/gamma)*ParamCalcul->m_NbContactPlaque[tps][i-1];
					if(beta!=0){
						lp2_=lp2_+coef*POWTEST(beta,teta)*(float)ParamCalcul->m_NbContactPlaque[tps][i-1];
					}
					else {
						lp2_=lp2_;// pour éviter le domain error
					}
				}


				/*
				for(i=1;i<=20;i++) {
					//courbe de reponse
//					beta=Fen->ValBeta[i-1]*beta0;

					//  A FAIRE
					//beta=Fen->Index[Hypo][Bino].ModeleFormat[tps][0]->Beta[i-1];
					beta=+1.0;


					jp=jp+beta*Fen->NbContactBarre[iCib][Hypo][tps][i-1];
					kp_=kp_+(beta/gamma)*Fen->NbContactPlaque[iCib][Hypo][tps][i-1];
					if(beta!=0){
						lp1_=lp1_+POWTEST(beta,phi)*(float)Fen->NbContactPlaque[iCib][Hypo][tps][i-1];
						lp2_=lp2_+coef*POWTEST(beta,teta)*(float)Fen->NbContactPlaque[iCib][Hypo][tps][i-1];
					}
					else {
						lp1_=lp1_;// pour éviter le domain error
						lp2_=lp2_;// pour éviter le domain error
					}
#ifdef DEBUGGRAPHE
	if(b[tps]>=10)
		fprintf(fd2,"%d   %d   %d    %f    %f   %f\n",i,(Block[b[tps]-1].DistBloc[i-1]),
		(Block[b[tps]-1].DistCumul[i-1]),beta,(beta/gamma),POWTEST(beta,teta));
#endif
				}*/
				tmbeta=(jp/c);
				j=(c-a_)*(jp/c);
				tmgama=(kp_/c_);
				k_=a_*RA*(kp_/c_);
				tmphi=(lp1_/c_);
				tmteta=(lp2_/c_);
				l1_=a_*RD1*(lp1_/c_);
				l2_=a_*RD2*(lp2_/c_);

				// calcul de la démémo pendant vague
				if(Ac_>0.0){
					ARM=AncienRo_/(Ac_);
					rovirt_=(Ac_)*(1-POWTEST((1-ARM),Admoy_/(Admoy_+1)));
//					epsi=1.0*(AncienRo_*(AncienRo_-rovirt_)/Ac_); // calul raporté sur les mémorisés
					epsi=KdeDememo*(AncienRo_-rovirt_); // calcul sur la couverture
				}
				else epsi=0.0;

				// calcul des assiettes
				epsi_=Aepsi_+epsi;
				delta1_=Adelta1_-l1_+epsi;
				delta2_=Adelta2_-l2_;
				n=j+k_+l1_+l2_;
				n_=An_+n;

				if (delta1_<0.0) delta1_=0.0;
				if (delta2_<0.0) delta2_=0.0;
				ro_=Aro_+n_-epsi_;
				m_=ro_+delta1_+delta2_;
				M_[tps]=100*ro_/pi;


				// calcul des ratios
				RA=(c_-m_)/c_;
				RM=ro_/c_;
				RD1=delta1_/c_;
				RD2=delta2_/c_;
				if (RD1<0.0) RD1=0.0;
				if (RD2<0.0) RD2=0.0;

				// sauvegarde des Anciennes assiettes
				Adelta1_=delta1_;
				Adelta2_=delta2_;
				An_=n_;
				Aepsi_=epsi_;
			}
			else { // cas des blos superposés **********************************************
				AfxMessageBox("PB dans fc calculCourbe->Cas des bloques superposés");
			} // fin du traitement du cas des blos superposés
		} // fin de traitement de période de mémorisation
		else {  // intervague ->Démémorisation ************************************************
			PhaseDememorisarion:
#ifdef DEBUGGRAPHE
		fprintf(fd2,"\n\nPhaseDememorisation:\n");
#endif
			ro_=c_*(1-POWTEST((1-RMv),dmoy_/(dmoy_+(s[tps]*(s[tps]+1))/2)));
			M_[tps]=ro_/pi*100;
			delta2_=Adelta2_+(Aro_-ro_);
			if(c_>+0.0){
				RM=ro_/c_;
				RD2=delta2_/c_;
				RA=(c_-m_)/c_;
			}
			RD1=0.0;
			if (RD2<0.0) RD2=0.0;
			Adelta2_=delta2_;
			Aro_=ro_;
		}
		// mise à jour des variable pour le tour suivant
		if (s[tps]==SMax) {
	    	if(v[tps]) {  // en fin de vague
				RMv=RM;
				An_=0.0;
				Aro_=ro_;
				Aepsi_=epsi_=0.0;
				delta2_=delta2_+delta1_;
				delta1_=0.0;
			}
			else { //en fin d'intervague donc de période
				// rien à faire ...
			}
			ASMax=SMax;
			SMax=Borne(s,tps+1,T);
		}
		// maj des variables de la semaine -1
		AncienRo_=ro_;

		Ac_=c_;
		Admoy_=dmoy_;
//		Fen->PtMemoCamp[iCib][Hypo][Bino]+=M_[tps];

#ifdef DEBUGGRAPHE
	fprintf(fd,"%2d  %2d  %2d  %2d  %2d  %2d  %2d",tps+1,s[tps],z[tps],b[tps],v[tps],w[tps],p[tps]);
    fprintf(fd," %5.0f %5.0f %5.0f %5.0f %5.0f %0.2f %9.3f \n\n",c,c_,d,d_,a_,0/*beta0*/,jp);
   	fprintf(fd1,"%9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f \n\n",kp_,lp2_,j,k_,l2_,delta2_,n,epsi);
   	fprintf(fd3," %9.3f %9.3f %9.3f %5.2f %0.4f %0.4f %0.4f  %2d %f %f %f\n\n",n_,m_,ro_,M_[tps],RA,RD2,RM,SMax,tmbeta,tmgama,tmteta);
#endif
	}
#ifdef DEBUGGRAPHE
	fclose(fd);
	fclose(fd1);
	fclose(fd2);
	fclose(fd3);
#endif
	// affectation du vecteur calculée
	// Fen->GRPCamp[Hypo][Bino]=d_/pi*100;
	// Fen->RMfin[Hypo][Bino]=RM;


	m_Resultat.Initialise(T); //=new CResultat(T);

	
	m_Resultat.m_NbIndivTouchesTot=c_;///1000.0;// division par 1000 pour convertir en milliers

	// Correction de la version 3.1i (11/05/01)
	// le bug vient du fait que lorsque les saisonnalités sont actives,
	// le résultat semaine par semaine est different de celui-ci...
	// donc la ligne suivante est en commentaire.
	// if(CalculSaisonnalite==0)
	{
		// NOTE:
		// lors du calcul avec la saisonalité,
		// Les couvertures sont tassés à causes des probas > 100
		// Donc pour ne pas perdre des contacts, 
		// On laisse le calcul de base des contacts s'occuper de ce résultat.
		// NbContactTot[iCib][Hypo]=d_/1000.0; 
	}

	// NOTE:
	// Pour tous les autres cas, normalement le nombre de contacts,
	// et la couverture donne des résultats équivalent au résultat de la mémo.
	// mais pour être sûr de retourner un résultat cohérant, 
	// on délivre le résultat mémo-démémo.

	m_Resultat.m_NbIndivMemoTot=m_; //100*m_/pi;///1000.0;
	m_Resultat.m_PtMemoCampTot=+0.0;
	int NbSemaineActive=0;

	for(i=0;i<T;i++)
	{
		m_Resultat.m_MemoTot[i]=M_[i];
		m_Resultat.m_CouvTot[i]=TabC_[i];
		m_Resultat.m_DistTot[i]=TabD_[i];
		if(ParamCalcul->m_SelectCalend[i])
		{
			m_Resultat.m_PtMemoCampTot+=m_Resultat.m_MemoTot[i];
			NbSemaineActive++;
		}
	}
	if (NbSemaineActive>0)
	{
		m_Resultat.m_MemoMoy=m_Resultat.m_PtMemoCampTot/NbSemaineActive;
	}
	else
	{
		m_Resultat.m_MemoMoy=0;
	}

	delete [] M_;
	delete [] TabC_;
	delete [] TabD_;



	delete [] m_Index.s;
	delete [] m_Index.z;
	delete [] m_Index.b;
	delete [] m_Index.v;
	delete [] m_Index.w;
	delete [] m_Index.p;

	return m_Resultat;
}



char CCalculMemoDememo::Borne(char *s,int deb,int max)
{
	char i,j;
	j=0;
	while(!s[j+deb]) j++;

	i=1;
	while ((i+j+deb<max) && (s[i+j+deb-1]<s[i+j+deb]))
		i++;
	return(i);
}
void CCalculMemoDememo::MiseAJourIndex()
{
	m_Index.s=new char[m_Parametre->m_NbSemaineCalcul];
	m_Index.z=new char[m_Parametre->m_NbSemaineCalcul];
	m_Index.b=new char[m_Parametre->m_NbSemaineCalcul];
	m_Index.v=new char[m_Parametre->m_NbSemaineCalcul];
	m_Index.w=new char[m_Parametre->m_NbSemaineCalcul];
	m_Index.p=new char[m_Parametre->m_NbSemaineCalcul];

	char s,z,b,v,w,p;
	int t;
	int max;
	int f;//,sp,Parc; //,NbSP
	char fQQChose;

	s=v=w=p=1;
	b=z=0;
	t=1;

	max = m_Parametre->m_NbSemaine;
	//NbSP=GEtude->Univers.Binomes[0][Bino].NbParc;

	f=0; // en attendant de boucler
	//sp=0;// en attendant de boucler
	//Parc=NrDirectParc[Bino][sp]; // en attendant de boucler

	// positionne sur le premier blocs
	while (t<=max) 
	{
		if(m_Parametre->m_NbChanceSem[t-1]>0)goto FinWhile1;
		m_Index.s[t-1]=0;
		m_Index.z[t-1]=0;
		m_Index.v[t-1]=0;
		m_Index.w[t-1]=0;
		m_Index.p[t-1]=0;
		m_Index.b[t-1]=0;
		t++;
	}
	FinWhile1:

	while (t<=max) {
		while (t<=max) { //&& (Fen->Passage[Hypo][Parc][t-1][f]
			fQQChose=0;
			if(m_Parametre->m_NbChanceSem[t-1]>0)fQQChose=1;
			if(!fQQChose)goto FinWhile2;
			m_Index.s[t-1]=s;
			z++;
			m_Index.z[t-1]=z;
			m_Index.v[t-1]=v;
			m_Index.w[t-1]=0;
			m_Index.p[t-1]=p;
			// b est le nombre de bloc pour cette semaine
			// b=b+Fen->Passage[Hypo][Parc][t-1][f];

			/* A FAIRE -> Coeff format
			NrSpUsed=0;
			for(sp=0;sp<NbSP;sp++){
				for(f=0;f<GEtude->m_NbFormat;f++){
					if((!Fen->fPiloteDurees) && (!(Fen->FlagFormat[f]&1)))continue;
					if(Fen->Passage[Hypo][Parc+sp][t-1][f])break;
				}
				if(f<GEtude->m_NbFormat){
					Fen->Index[Hypo][Bino].ModeleFormat[t-1][NrSpUsed]=TouveBetaF(Fen,Hypo,Parc+sp,t-1);
					NrSpUsed++;
					b++;
				}
			}
			Fen->Index[Hypo][Bino].b[t-1]=b;*/
			b++;
			m_Index.b[t-1]=b;
			s++;
			t++;
		}
		FinWhile2:
 		s=1;
		while((t<=max)) 
		{
			if(m_Parametre->m_NbChanceSem[t-1]>0)goto FinWhile3;
			m_Index.s[t-1]=s;
			m_Index.z[t-1]=z;
			m_Index.v[t-1]=0;
			m_Index.w[t-1]=w;
			m_Index.p[t-1]=p;
			m_Index.b[t-1]=b;
			s++;
			t++;
		}
		FinWhile3:
		s=1;
		v++;
		w++;
		p++;
	}

	z=m_Index.z[max-1];
	p=m_Index.p[max-1];
	b=m_Index.b[max-1];
	if (m_Index.v[max-1]) {
		s=1;
		w=m_Index.v[max-1];
	}
	else {
		s=m_Index.s[max-1]+1;
		w=m_Index.w[max-1];
	}
	while (t<=m_Parametre->m_NbSemaineCalcul) {
		m_Index.s[t-1]=s;
		m_Index.z[t-1]=z;
		m_Index.v[t-1]=0;
		m_Index.w[t-1]=w;
		m_Index.p[t-1]=p;
		m_Index.b[t-1]=b;
		s++;
		t++;
	}
}

double CCalculMemoDememo::POWTEST(double x,double y){
	if(x<0.0 || y<0){
//		AfxMessageBox("Domaine mathématique invalide (x^y)");
//		ASSERT(0);
		return(0.0);
	}
	if(x==0.0 || y==0){
		return(0.0);
	}
	return(pow(x,y));
}



float CCalculMemoDememo::BetaSuivantModele(float Beta0,float Duree)
{
	double alpha0,beta;

	// on se blinde dans le domaine matheu...
	if(Beta0<=0)return(0);
	if(Beta0>=1)return(1);
	if(Duree>m_Parametre->m_DureeMaximum)Duree=m_Parametre->m_DureeMaximum;
	if(Duree<=0){
		AfxMessageBox("Durée invalide dans fc BetaSuivantModele");
		return(0);
	}

	// calcul...
	alpha0=log(Beta0)/log(sin(m_Parametre->m_DureeBaseBeta*M_PI/(2*m_Parametre->m_DureeMaximum)));
	beta=POWTEST(sin(Duree*M_PI/(2*m_Parametre->m_DureeMaximum)),alpha0);
	return(beta);
}

