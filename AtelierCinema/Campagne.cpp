// Campagne.cpp: implementation of the CCampagne class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "TableBAC.h"

extern CCinemaApp theApp;
#include "Campagne.h"
#include "CAlendrierHebdoCinema.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCampagne::CCampagne()
{
	m_init=0;
	CWaitCursor wait;

	// Saisonnalité activée : demande du 20/02/2002
	CalculSaisonnalite=1;
	fPiloteDurees=1; // 0:Durées actives 1:Toutes

	CalculDememo=0;
	RedressementCNC=1; // Redressement CNC actif

	// Par défaut n° de l'option agglomération, désormais on prend agglo 50000 -- modif alain 2005
	// NumeroModaliteAgglo=5; //200000 habitants et +
	NumeroModaliteAgglo = 3;  //50000 habitants et +

	m_NumCible[0]=1;
	for(int cible=1;cible<NBCIBLECALCUL;cible++)
	{
		m_NumCible[cible]=-1;
	}
	
	m_HypotheseActive=0;
	m_HypotheseValide=0;
	m_VisuHypothese[0]=2;
	m_VisuHypothese[1]=0;
	m_VisuHypothese[2]=0;
	m_VisuHypothese[3]=0;
	m_VisuHypothese[4]=0;

	m_BlocPass=NULL;

	OutilsActif=0;
	m_PosAscV=0;
	
	// Init des formats
	m_NbFormat=1;
	m_LibFormat.Add("30");
	m_FlagFormat.Add(3);
	m_FlagFormat.Add(0);
	m_FlagFormat.Add(0);
	m_FlagFormat.Add(0);
	m_FlagFormat.Add(0);

	CalculCourbeReponse();

	m_flagBinomeAgregat=0;

	NrTempoActif=0;


	for(int c=0;c<NBCIBLECALCUL;c++)
	{
		for(int h=0;h<NBHYPOTHESE;h++)
		{
			for(int s=0;s<NBSEMAINEMAX;s++)
			{
				NbContactBarre[c][h][s]=NULL;
				NbContactPlaque[c][h][s]=NULL;
			}
		}
	}

	PQind=new double[theApp.NbIndiv];
	CoeffFormat=NULL;
	fPressePapier=0;

	/* reporté dans CCampagneGén
	hBmpOldspeed=0;
	hDCspeed=0;
	hBmpNewspeed=0;
	*/

	m_MapCpxUsedParLigne=0;
	m_QuotaBacUsedParLigne=0;
	m_ListeCpxParLigne=0;
	m_ListeComplexeParBAC=0;
	m_MapParcAtcif=0;

	Budget=NULL;

	//*** les données privées de l'objet ***
	m_Passage		 = NULL;	// par[hypothese][Parc][semaine][Format]
	m_NbPassage		 = NULL;	// par [hypothese][parc]
	m_NbPassageMoyen = NULL;	// [hypothese][parc]
	


}

CCampagne::~CCampagne()
{
	
	FreeMemory();

	delete [] PQind;

	for(int c=0;c<NBCIBLECALCUL;c++)
	{
		for(int h=0;h<NBHYPOTHESE;h++)
		{
			for(int s=0;s<NBSEMAINEMAX;s++)
			{
				if(NbContactBarre[c][h][s])
				{
					delete [] NbContactBarre[c][h][s];
					NbContactBarre[c][h][s]=0;
				}
				if(NbContactPlaque[c][h][s])
				{
					delete [] NbContactPlaque[c][h][s];
					NbContactPlaque[c][h][s]=0;
				}
			}
		}
	}

	if(m_MapCpxUsedParLigne) delete [] m_MapCpxUsedParLigne;

	// ADD FRED
	// libération du tableau du quota
	FreeQuotaBacUsedParLigne();

	if(m_ListeCpxParLigne) delete [] m_ListeCpxParLigne;
	if(m_ListeComplexeParBAC) delete [] m_ListeComplexeParBAC;
	if(m_MapParcAtcif)delete [] m_MapParcAtcif;
}


void CCampagne::CalculPassageACCOGRIL()
{
	int p,s,f, s_mod_sem;
	int MaxTabSem;
	char fBeep;
	fBeep=0;
	CString code;
	int nbparc = m_Selection.GetNbParcTotal();
	bool flgmessage=0;
	//int start = GetTickCount();
	CByteArray * tarifs=NULL;
	for(p=0;p<nbparc;p++){
		m_NbPassage[m_HypotheseActive][p]=0;
		code = m_Selection.GetCodeFromParc(p);
		if((code[0]=='B')&&(code.GetLength()==6))code+="P";
		if(p%2==0)
		{
			tarifs = m_Tarif.GetTarif(code);
		}

		// Index max tab tarif
		MaxTabSem = tarifs->GetSize();
		for(s=0;s<m_NbSemaine;s++)
		{

			// Bidouille (en passant de 53 semaines maxi à +, je suis obligé de ramener l'index semaine au début de table)
			s_mod_sem = s%(MaxTabSem);

			// on élimine les passages qui se trouvent dans des espaces non comercialisé
			//if(m_Tarif.GetTarif(code,s,p%2)<0)
			if(!tarifs->GetAt(s_mod_sem))
			{
				for(f=0;f<m_NbFormat;f++)
				{
					if(m_Passage[m_HypotheseActive][p][s_mod_sem][f])
						fBeep=1;
					m_Passage[m_HypotheseActive][p][s_mod_sem][f]=0;
				}
			}
			else 
			{
				for(f=0;f<m_NbFormat;f++)
				{
					if(!(m_FlagFormat[f]&1))continue;
					if(m_Passage[m_HypotheseActive][p][s_mod_sem][f])
					{
						// Correction en cas d'erreur
						if(m_MapInterdit[s_mod_sem][p][f])
						{
							// Changement de format
							if(OutilsActif==4)
							{
								// Passage est ajouté pour ce format (déjà fait)
								// Il faut le retirer pour l'autre (format inactif).
								for(int format=0;format<m_NbFormat;format++)
								{
									if(!(m_FlagFormat[format]&3)) m_Passage[m_HypotheseActive][p][s_mod_sem][format]=0;
							
								}
								// modification de l'interdiction
								//m_MapInterdit[s][p][f]=0;
						
							}
							else
							{
								// Erreur
								// On enlève le passage, et on affiche un message
								flgmessage=1;
								m_Passage[m_HypotheseActive][p][s_mod_sem][f]=0;
							}
						}
						else 
						{
							// ok
							m_NbPassage[m_HypotheseActive][p]++;
						}
					}
				}
			}
		
		}
	}

	for(int ligne=0;ligne<m_Selection.GetSize();ligne++)
	{
		double moyenne=0;
		int nbparc = m_Selection.GetNbParc(ligne);
		for(int p=0;p<nbparc;p++)
		{
			int parc=m_Selection.GetNrDirectParc(ligne,p);
			moyenne+=m_NbPassage[m_HypotheseActive][parc];
		}
		moyenne/=m_Selection.GetNbParc(ligne);
		ASSERT(moyenne>=0);
		for(p=0;p<m_Selection.GetNbParc(ligne);p++)
		{
			int parc=m_Selection.GetNrDirectParc(ligne,p);
			m_NbPassageMoyen[m_HypotheseActive][parc]=moyenne;
		
		}
	}
	UpdateMap(0);
	if(fBeep)MessageBeep(-1);
	if(flgmessage)AfxMessageBox("Ce passage n'est pas autorisé par la mécanique d'alternance!!");
	//int end = GetTickCount();
	//int duree = end-start;
}

// Cette fonction alloue l'environement nécessaire pour le nombre de semaines et
// pour le nombre de sélections chargés.
void CCampagne::SetExercice(COleDateTime datedebut, int nbsemaine, bool glisse)
{
	FreeMemory();
	m_DateDebut		    = datedebut;
	m_NbSemaine		    = nbsemaine;
	m_NbSemaineCalcul	= 52+m_NbSemaine;

	m_DebutCalend		= 0;
	m_FinCalend		    = m_NbSemaine-1;
	m_NrSemaineX	    = m_NbSemaine-2;
	if(m_NrSemaineX<=0 && m_NbSemaine>=1) m_NrSemaineX=0;
	BindVecteurCalendrier();

	// initialisation du tarif
	m_Tarif.InitialiseExercice(datedebut, nbsemaine);

	// initialisation des saisonnalités
	m_Saison.InitialiseExercice(datedebut, nbsemaine);

	// initialisation des variables liées à la sélection
	AllocParc();

	AllocBudget();

	CalculCoeffFormat();

	for(int h=0;h<NBHYPOTHESE;h++)
	{
		Index[h].s=new char[m_NbSemaineCalcul];
		Index[h].z=new char[m_NbSemaineCalcul];
		Index[h].b=new char[m_NbSemaineCalcul];
		Index[h].v=new char[m_NbSemaineCalcul];
		Index[h].w=new char[m_NbSemaineCalcul];
		Index[h].p=new char[m_NbSemaineCalcul];
	}


	// intitialisation du tableau des index de cibles imprimées à -1 pour inactive
	for(int cible=0;cible<NBCIBLECALCUL;cible++)
	{
		m_IndexCibleImprime[cible]=-1;
	}

	m_NbCibleImprime=0;

	m_init=1;
}

void CCampagne::CalculTout()
{
	HCURSOR oldcur=::SetCursor(theApp.hCurWait);
	CalculBasePopulation();
	CalculToutBudget();
	CalculToutMemo();
	::SetCursor(oldcur);
}

void CCampagne::CalculToutHypothese(int H)
{
	// HCURSOR oldcur=::SetCursor(theApp.hCurWait);
	CalculBasePopulation();
	ASSERT(AfxCheckMemory());
	CalculToutBudgetHypothese(H);
	CalculToutMemoHypothese(H);
	// ::SetCursor(oldcur);
}



void CCampagne::CalculToutBudget()
{
	HCURSOR oldcur=::SetCursor(theApp.hCurWait);
	int h;
	
	for(h=0;h<NBHYPOTHESE;h++)
	{
		CalculToutBudgetHypothese(h);
	}
	::SetCursor(oldcur);
}

void CCampagne::CalculToutBudgetHypothese(int H)
{
	//HCURSOR oldcur=::SetCursor(theApp.hCurWait);

	for(int b=0;b<m_Selection.GetSize();b++)
		CalculBudgetLigne(H,b);
	ASSERT(AfxCheckMemory());

	CalculTotalBudget(H);
	EtatCalcul[H]=1; // AVIRER

	//::SetCursor(oldcur);
}


#include "dlgperformances.h"
#include "dlgperfagglo.h"
#include "dlggraphe.h"

// bDebut = HYPOTHESESPOP ou -3 on ne charge que la première cible
// bDebut = CIBLEUTILE (calcul de la cible utile)
// bDebut = MULTICIBLE (chargement des cibles et calcul du vecteur individu pour l'impression multicible.)
// bDebut = TOUTHYPOTHESEVALIDE (calcul de l'hypothese valide)
// bDebut = TOUTHYPOTHESEACTIVE (calcul de l'hypothese active)
// bDebut = TOUTHYPOTHESEACTIVEIFNOTVALIDE (calcul de l'hypothese active lorsque celle-ci n'est pas valide)
// bDebut = TOUT (calcul tout)
// bDebut = HYPOTHESESPOP (calcul de tout le budget, toutes les mémos, toute la population)
// bDebut = HYPOTHESES (calcul tout le budget et tout le mémo)
// bDebut = HYPOTHESEFORMATACTIF (calcul tout le budget et tout le mémo si non fPiloteDurees )
// bDebut = BUDGETPERF (calcul tout budget et tout mémo si boite performances ou graphique)


void CCampagne::LanceCalcul(int bDebut)
{
	if(bDebut==HYPOTHESESPOP || bDebut==-3)
	{
		// On ne charge que la première cible
		if(m_NumCible[0]>=0) ChargeCible(m_NumCible[0],0);
	}

	CDlgPerformances * dlg1=(CDlgPerformances * )m_Perf;
	int hDlgPerf=dlg1->IsWindowVisible();

	CDlgPerfAgglo * dlgAgglo=(CDlgPerfAgglo * )m_PerfAgglo;
	int hDlgPerfAgglo=dlgAgglo->IsWindowVisible();

	CDlgGraphe * dlg2=(CDlgGraphe * )m_Graphe;
	int hDlgGraph=dlg2->IsWindowVisible();
	int hDlgPanorama=0;

	if(hDlgPerfAgglo)
	{
		// Si les agglo sont présente, il faut calculer en premier
		// Les performances sur les agglomérations...
		PrepareVecteurIndividus(0);
		CalculPopCibleUtile(0);
		//CalculPerformanceAgglo(0,m_HypotheseActive);
	}
	
	
	// chargement de la cible pour calcul
	if(bDebut==CIBLEUTILE)
	{
		PrepareVecteurIndividus(0);
		CalculPopCibleUtile(0);
	}
	// chargement des cibles pour impression multicible
	if(bDebut==MULTICIBLE)
	{
		for(int cib=0;cib<NBCIBLECALCUL;cib++)
		{
			if(m_IndexCibleImprime[cib]!=-1)
			{
				ChargeCible(m_IndexCibleImprime[cib],cib);
				PrepareVecteurIndividus(cib);
				CalculPopCibleUtile(cib);
			}
		}
	}

	// fonctions de calcul seules
	if(bDebut==TOUTHYPOTHESEVALIDE){
		ASSERT(AfxCheckMemory());
		CalculToutHypothese(m_HypotheseValide);
		return;
	}
	if(bDebut==TOUTHYPOTHESEACTIVE){
		CalculToutHypothese(m_HypotheseActive);
		goto dessin;
		//return; //Modifier le 18/04/02 par goto dessin
	}
	if(bDebut==TOUTHYPOTHESEACTIVEIFNOTVALIDE){
		if(m_HypotheseActive!=m_HypotheseValide)
			CalculToutHypothese(m_HypotheseActive);
		return;
	}
	if(bDebut==TOUT){
		CalculTout();
		return;
	}

// MODIF AURELIEN : SUPPR PERF2
//	if(!(hDlgGraph || hDlgPanorama || hDlgPerf|| hDlgPerfAgglo || hDlgPerf2))return;
	if(!(hDlgGraph || hDlgPanorama || hDlgPerf|| hDlgPerfAgglo))return;


	if(bDebut==HYPOTHESESPOP)
	{
// MODIF AURELIEN : SUPPR PERF2
//		if(hDlgPerf||hDlgPerf2)CalculToutBudget();
		if(hDlgPerf)CalculToutBudget();
		CalculBasePopulation();
		CalculToutMemo();
		goto dessin;
	}
	if(bDebut==HYPOTHESES)
	{
// MODIF AURELIEN : SUPPR PERF2
//		if(hDlgPerf||hDlgPerf2)CalculToutBudget();
		if(hDlgPerf)CalculToutBudget();
		CalculToutMemo();
		goto dessin;
	}

	/*if(bDebut==-3){
		if(hDlgPerf||hDlgPerf2)CalculToutBudget();
		if(BaseCalcul==0)CalculBasePopulation();
		CalculToutMemo();
		goto dessin;
	}*/
	if(bDebut==HYPOTHESEFORMATACTIF)
	{
		if(!fPiloteDurees)
		{
// MODIF AURELIEN : SUPPR PERF2
//			if(hDlgPerf||hDlgPerf2)CalculToutBudget();
			if(hDlgPerf)CalculToutBudget();
			CalculToutMemo();
			goto dessin;
		}
	}
	/*if(bDebut<0){
		bDebut=0;
		bFin=m_Selection.GetSize()-1;
	}
	else bFin=bDebut;

	if(hDlgPerf||hDlgPerf2){
		for(b=bDebut;b<=bFin;b++)CalculBudgetLigne(HypotheseActive,b);
		CalculTotalBudget(m_HypotheseActive);
	}*/

	if(bDebut==BUDGETPERF)
	{
// MODIF AURELIEN : SUPPR PERF2
//		if(hDlgPerf||hDlgPerf2||hDlgGraph)
		if(hDlgPerf||hDlgGraph)
		{
			CalculToutBudget();
			CalculMemo(m_HypotheseActive);
			CalculTotalMemo(m_HypotheseActive);
		}
	}

	dessin: // on DESSINE ...
	if(hDlgPerf)dlg1->MajAffichagePerf();
	if(hDlgPerfAgglo)dlgAgglo->MajAffichagePerf();
	if(hDlgGraph)dlg2->MajAffichageACCOGRAP();
	m_pDoc->SetModifiedFlag();
}

void CCampagne::CalculToutMemo()
{
	HCURSOR oldcur=::SetCursor(theApp.hCurWait);
	for(int h=0;h<NBHYPOTHESE;h++)
	{
		CalculToutMemoHypothese(h);
	}
	::SetCursor(oldcur);
}

void CCampagne::CalculToutMemoHypothese(int H)
{

	// Chiant le curseur qui flash 
	// HCURSOR oldcur=::SetCursor(theApp.hCurWait);

	CalculMemo(H);
	CalculTotalMemo(H);

	// ::SetCursor(oldcur);
}


void CCampagne::CalculBasePopulation()
{

	int iCib;

	// MULTICIBLE
	// A calculer pour chaque cible

	for(iCib=0;iCib<NBCIBLECALCUL;iCib++)
	{
		m_Resultat.BasePopulation[iCib]=0;
		if(m_NumCible[iCib]<0) continue;


		double NbCas = CibleUtile[iCib].NbCas;


		m_Resultat.BasePopulation[iCib] = CibleUtile[iCib].PopPon;
		m_Resultat.NbCas[iCib] = CibleUtile[iCib].NbCas;
		
		/*
		switch(BaseCalcul){
			case 0: // Régional
				for(b=0;b<F->NbBinomeAff;b++){ //GEtude->Univers.NbBinomes[0]
					if(!F->SelectBinome[F->NrBinomeAff[b]])continue;
					F->BasePopulation[iCib]+=F->Abaques[iCib][F->NrBinomeAff[b]].Effectif;
				}
			break;
			case 1:
				AfficheErreur("Le calcul en base multi-régional à faire...");
			break;
			case 2: // National
				F->BasePopulation[iCib]=F->Abaques[iCib][NRBINOMENATIONAL].Effectif;
			break;
		}*/
	}
}

void CCampagne::CalculTotalBudget(int H)
{
	int b;
	m_Resultat.BudgetTot[H]=0;
	for(b=0;b<m_Selection.GetSize();b++)
	{
		if(!m_Selection.SelectBinome[b])continue;
		m_Resultat.BudgetTot[H]+=Budget[H][b];
	}
}

void CCampagne::CalculBudgetLigne(int H,int B)
{
	int s,p,f,Parc;
	// int Offset;
	double CoeffTarif;

//	if(B==5||B==6||B>8)Offset=0;
//	else if(B==7||B==8)Offset=GEtude->NbSemainePA;
//	else Offset=2*GEtude->NbSemainePA;
	ASSERT(AfxCheckMemory());

	Budget[H][B]=0L;
	ASSERT(AfxCheckMemory());

	for(s=0;s<m_NbSemaine;s++){
		for(f=0;f<m_NbFormat;f++){
			if((!fPiloteDurees) && (!(m_FlagFormat[f]&1)))continue;
			CoeffTarif=CoeffFormat[f][s]; 
			ASSERT(AfxCheckMemory());

			for(p=0;p<m_Selection.GetNbParc(B);p++)
			{
				Parc=m_Selection.GetNrDirectParc(B,p);
				if(m_Passage[H][Parc][s][f]){
//					ASSERT(AfxCheckMemory());

					CString code = m_Selection.GetCodeFromParc(Parc);
					if((code[0]=='B')&&(code.GetLength()==6))code+="P";
					// Si le code est un code bac probabilisé
					double tarif = m_Tarif.GetTarif(code,s,p)*CoeffTarif;
					Budget[H][B]+=tarif;

//					ASSERT(AfxCheckMemory());
				}
			}
		}
	}
}

void CCampagne::CalculMemoNrSemaineX(int h)
{
	int x;
	int iCib;

	x = m_NrSemaineX;

	for(iCib=0;iCib<NBCIBLECALCUL;iCib++)
	{
		m_Resultat.MemoNrSemaineX[iCib][h]=0.0;
		if(m_NumCible[iCib]<0) continue;

		if(x<0 || x>=m_NbSemaine || m_Resultat.BasePopulation[iCib]<=0.0)return;
		/*
		for(b=0;b<F->NbBinomeAff;b++){
			if(!F->SelectBinome[F->NrBinomeAff[b]])continue;
			F->MemoNrSemaineX[iCib][h]+=F->Memo[iCib][h][F->NrBinomeAff[b]][x];
		}*/
		m_Resultat.MemoNrSemaineX[iCib][h]=100.0*MemoTot[iCib][h][x];
	}
}


void CCampagne::CalculTotalMemo(int H)
{
	int s,b;
	int iCib;

	//CalculSaisonnalitee(F,H);
	fZeroBinomeSelected=1;
	for(iCib=0;iCib<NBCIBLECALCUL;iCib++)
	{
		if(m_NumCible[iCib]<0)continue;
		for(s=0;s<m_NbSemaineCalcul;s++)
		{
		//	F->MemoTot[iCib][H][s]=+0.0;
		//	F->CouvTot[iCib][H][s]=+0.0;
		//	F->DistTot[iCib][H][s]=+0.0;
			if(m_Resultat.BasePopulation[iCib]>0.0)
			{
			/*	for(b=0;b<F->NbBinomeAff;b++){
					if(!F->SelectBinome[F->NrBinomeAff[b]])continue;
					F->MemoTot[iCib][H][s]+=F->Memo[iCib][H][F->NrBinomeAff[b]][s];
					F->CouvTot[iCib][H][s]+=F->Couv[iCib][H][F->NrBinomeAff[b]][s];
					F->DistTot[iCib][H][s]+=F->Dist[iCib][H][F->NrBinomeAff[b]][s];
				}*/
				MemoTot[iCib][H][s]/=m_Resultat.BasePopulation[iCib];
				CouvTot[iCib][H][s]/=m_Resultat.BasePopulation[iCib];
				DistTot[iCib][H][s]/=m_Resultat.BasePopulation[iCib];
			}
		}
		for(b=0;b<m_Selection.GetSize();b++)
		{
			if(!m_Selection.SelectBinome[b])continue;
			fZeroBinomeSelected=0;
		}
		if(m_Resultat.BasePopulation[iCib]>0.0 && m_NbSemaineCalend > 0)
		{
			m_Resultat.PtMemoCampTot[iCib][H]=100*m_Resultat.PtMemoCampTot[iCib][H]/m_Resultat.BasePopulation[iCib];
			m_Resultat.MemoMoy[iCib][H]=m_Resultat.PtMemoCampTot[iCib][H]/m_NbSemaineCalend;
		}
		else 
		{
			m_Resultat.MemoMoy[iCib][H]=+0.0;
			m_Resultat.PtMemoCampTot[iCib][H]=+0.0;
		}
	}
	CalculMemoNrSemaineX(H);
}

void CCampagne::MajAffichagePerf()
{
	CDlgPerformances * dlg1=(CDlgPerformances * )m_Perf;
	int hDlgPerf=dlg1->IsWindowVisible();
	if(hDlgPerf)dlg1->MajAffichagePerf();
}

void CCampagne::CalculMemo(int H)
{
	int iCib;

	if(fCalculStop[H])
	{
		fCalculStop[H]=2;
		return;
	}

	recommence:
	if(EtatCalcul[H]<0)return;
	fCalculStop[H]=1;
	EtatCalcul[H]=0;

	MajAffichagePerf(); 
	//	Calcul des distributions et de la mémo-démémo
	for(iCib=0;iCib<NBCIBLECALCUL;iCib++)
	{
		if(m_NumCible[iCib]<0)continue;
		if(CalculCouvertures(H,iCib))goto recommence;
		MiseAJourIndex(H); 
		CalculCourbeMemoDememo(iCib,H); 
	}
	
	EtatCalcul[H]=3;
	NrDessin=0;

	MajAffichagePerf();
	fCalculStop[H]=0;
}

short CCampagne::CalculCouvertures(int H,int iCib)
{

	TRACE("Calcul de l'hypothèse %d cible %d*********************\n",H,iCib);
	ASSERT(AfxCheckMemory());

	// variables pour boucler
	int i,c,s,z,b;

	// le pointeur sur la cible
	double * vCible;
	vCible = CibleUtile[iCib].Vecteur;
	
	// Pour calculer les distributions
	// on boucle sur les lignes de la sélection afin d'identifier les BACs et les complexes
	// avec lesquels on va travailler de façon à préparer le vecteur des individus qui doit être utilisé

	// ATTENTION, cette ruse n'est possible que parce que les individus sont attachés à un BAC et un seul !

	// les distributions (qui ne seront pas liberées)
	for(s=0;s<m_NbSemaine;s++)
	{
		if(NbContactBarre[iCib][H][s])
		{
			delete [] NbContactBarre[iCib][H][s];
			NbContactBarre[iCib][H][s]=0;
		}
		if(NbContactPlaque[iCib][H][s])
		{
			delete [] NbContactPlaque[iCib][H][s];
			NbContactPlaque[iCib][H][s]=0;
		}

		NbContactBarre[iCib][H][s]=new double[NBFOISTOUCHEMAX];
		for(z=0;z<NBFOISTOUCHEMAX;z++)
		{
			NbContactBarre[iCib][H][s][z]=+0.0;
		}
		
		NbContactPlaque[iCib][H][s]=new double[NBFOISTOUCHEMAX];
		for(z=0;z<NBFOISTOUCHEMAX;z++)
		{
			NbContactPlaque[iCib][H][s][z]=+0.0;
		}


		// Visu distri
		double NbContact1 = NbContactPlaque[iCib][H][s][0]; 
		double NbContact2 = NbContactPlaque[iCib][H][s][0]; 
		double NbContact3 = NbContactPlaque[iCib][H][s][0]; 
		double NbContact4 = NbContactPlaque[iCib][H][s][0]; 

	}

	PrepareParcActifs(H);
	PrepareBacParLigne();

	// Recherche des individus qui peuvent etre utilisé en fc de la sélection
	// et en fonction de la cible
	// Préparation des individus

	CDWordArray NrIndiv; // Nr réel de l'individu
	// MODIF FRED : 16/05/2002
	// MODIF FRED : 03/07/2002 
	// Ajout d'un tableau de booléen indiquant pour chaque bac probabilisé si les individus ont déjà été ajoutés.
	// Cela permet d'éviter la multiplication des individus lors de la présence des bacs régie (même index bac probabilisé).

	CByteArray flagIndivAjoute;
	flagIndivAjoute.SetSize(theApp.m_NbBacProbabilise); 
	for(b=0;b<theApp.m_NbBacProbabilise;b++)
		flagIndivAjoute.Add(0);

	// for(b=0;b<3*theApp.m_NbBacProbabilise;b++)
	for(b=0;b<NB_BLOC_BAC*theApp.m_NbBacProbabilise;b++)
	{
		if(!m_ListeBacUtilise[b])continue;
		
		// BAC simple
		double coeff=+1.0;


		// ERREUR CODE BAC ALAIN !!!!!!
		// MODIF FRED: 16/05/2002  BAC REGIE
		// récupération de l'index du bac proba à partir du code de base.
		CString codebase = theApp.m_TableCinema.GetTableBac()->GetCode(b);
		int idxproba = theApp.m_TableCinema.GetTableBac()->FindIndexFromCode(codebase);

		if(flagIndivAjoute[idxproba])continue;
		flagIndivAjoute[idxproba]=1;
		for(int i=0;i<theApp.m_NbIndivSupp[idxproba];i++)
		{
			//TRACE("indexproba:%d,NrIndivSupport:%f\n",idxproba,theApp.m_NrIndivSupp[idxproba][i]);
			int NrI=theApp.m_NrIndivSupp[idxproba][i];
			if(vCible[NrI]>0)
			{
				NrIndiv.Add(NrI);
				//TRACE("Individu %d\n",NrI);
			}
		}
	}
	int NbIndiv=theApp.NbIndiv;
	int NbIndivCalcul=NrIndiv.GetSize();

	//TRACE("Nombre d'individus utile:%d\n",NbIndivCalcul);

	// Préparation du vecteur binomial de l'individu
	double *VBi=new double[NBFOISTOUCHEMAX+1]; // vecteur instantané pour la semaine
	double *VBc=new double[NBFOISTOUCHEMAX+1]; // vecteur cumulé pour toutes les semaines
	double *Macro=new double[NBFOISTOUCHEMAX+1]; // vecteur pour calculer la macro-évaluation
	double *BTempo;
	int    TailleVB=(NBFOISTOUCHEMAX+1)*sizeof(double);

	// On boucle maintenant sur les individus, sur les semaines et sur les supports
	// qui sont achetés afin de calculer la barre et la plaque.

	double NbContactsParSemaine[NBSEMAINEMAX];
	double NbSecondesParSemaine[NBSEMAINEMAX];

	NbChanceTot[H]=0;
	for(s=0;s<NBSEMAINEMAX;s++)
	{
		NbChanceSem[H][s]=0;
		NbChanceCum[H][s]=0;
		NbContactsCumulParSemaine[iCib][H][s]=NbContactsParSemaine[s]=NbSecondesParSemaine[s]=0;
	}
	
	double NbContacts=0;
#ifdef _DEBUG
	long SigmaP1=0;
#endif
			
	for(i=0;i<NbIndivCalcul;i++)
	{
		// Préparation des variables de l'individu
		int NrI=NrIndiv[i];
		//ASSERT(NrI!=41104);
		//TRACE("Nr d'individu:%d:%d\n",i,NrI);
		// Calcul du coefficient d'extrapolation
		double CoeffEx=vCible[NrI];

		// Initialisation du vecteur binomial CUMULE de cet individu
		VBc[0]=+1.0;for(c=1;c<NBFOISTOUCHEMAX+1;c++)VBc[c]=+0.0;

		int NbChanceDansPlaque=0;

		// Boucle sur les semaines
		for(s=0;s<m_NbSemaine;s++)
		{
			if(m_SemaineActive[s])
			{
				// On passe en revue tous les passages de la semaine pour savoir 
				// si l'individu sur lequel on travaille à une chance d'être touché.
				// Note: si on passe plusieurs formats dans le même passage
				// il ne sera compté qu'une fois.

				WORD p1,p2;
				double coeffCorrection=1.0;
				double QuotaBAC=1.0;
				BYTE NbSeconde=0;

				if(FindProbabilite(H,NrI,s,p1,p2,QuotaBAC,NbSeconde))
				{
					//TRACE("%d\t%d\t%d\t%f\n",NrI,p1,p2,QuotaBAC);
					int u,ub,up;
					// Cet individu a une chance d'être touché donc on recalcule le vecteur binomial cumulé
					// Le vecteur binomial instantané est celui de la matrice puisque
					// l'individu ne peut être touché qu'une fois.

					// Prise en compte de saisonnalite
					// Prise en compte de la correction CNC 
					if(CalculSaisonnalite)
					{
						coeffCorrection*=m_Saison.GetSaisonnalite(s);
					}
					// V3.5
					if(RedressementCNC)
					{
						int indexBac=theApp.m_NrBacParIndiv[NrI];
						coeffCorrection*=theApp.m_CoeffCorrectionCNC[indexBac];
					}


					// Calcul du nombre de contacts avant que l'on
					// touche à la plurifréquentation
					double contact=(double)((double)p2*CoeffEx*coeffCorrection*QuotaBAC)/(double)PROBABILISATION;

					NbContacts+=contact;
					NbContactsParSemaine[s]+=contact;
					NbSecondesParSemaine[s]+=contact*NbSeconde;

					//if(s==0)TRACE("NbContact=%.f\n",NbContactsParSemaine[s]);


					// Calcul de p et p' en tenant compte du redressement CNC et de la saisonalité
					// la part de marché des complexes qui se trouvent dans un BAC est utilisé après (V3.5)
					//TRACE("%d\t%d\t%d --->",NrI,p1,p2);

					double proba=p1*coeffCorrection;
					p1=(WORD)(proba+.4999999999);
					if(p1==0)
					{
						for(c=0;c<NbChanceDansPlaque;c++)
						{
							int cts=c+1;
							ASSERT(VBc[cts]*CoeffEx>=0);
							NbContactPlaque[iCib][H][s][c]+=VBc[cts]*CoeffEx;
						}
						continue;
					}
					proba=p2*coeffCorrection;
					p2=(WORD)(proba+.4999999999);

					//TRACE("%d\t%d\t%d\n",p1,p2);


					// Depuis qu'il existe une correction avec l'hypothèse 30 jours
					// il est possible que p2 soit inférieur à p1.
					// Ce n'est donc plus un bug à ce point du programme
					//if(p2<p1)p2=p1; puisque l'hypothese 30 est annulée


					// Limitation des corrections à l'échantillon
					if(p1>PROBABILISATION)p1=PROBABILISATION;
					if(p2>PROBABILISATION)p2=PROBABILISATION;

					// Pour voir rapidement l'asymptote (TEST)
					// p1=p2=PROBABILISATION;
					// Pour voir rapidement l'asymptote (TEST)


					//double verif=0; // VERIFICATION

					int NbChanceDansBarre=theApp.m_NbMPP[p1][p2];
					VBi[0]=1.0;
					for(up=1;up<=NbChanceDansBarre;up++)
					{
						double proba=theApp.m_MPP[p1][p2][up]*QuotaBAC;
						VBi[up]=proba;
						VBi[0]-=proba;
						//verif+=proba*up;
					}

					//ASSERT(verif<theApp.m_PT1[p2]+.0000001);
					//ASSERT(verif>theApp.m_PT1[p2]-.0000001);

#ifdef _DEBUG
	/*SigmaP1+=p1;
	TRACE("Individu %d Poids %.0f Proba1 %d Proba2 %d Contacts%.0f SigmaP1 %d\n",NrI,CoeffEx,p1,p2,NbContacts,SigmaP1);*/
#endif	
					// Macro-évaluation dans le Vecteur binomial cumulé de l'individu (VBc)
					memset(Macro,0,TailleVB);
					for(ub=0;ub<=NbChanceDansBarre;ub++)
					{
						for(up=0;up<=NbChanceDansPlaque;up++)
						{
							u=ub+up;
							if(u>NBFOISTOUCHEMAX)u=NBFOISTOUCHEMAX;
							Macro[u]+=VBi[ub]*VBc[up];
						}
					}
					BTempo=VBc;VBc=Macro;Macro=BTempo;

					// intégration de cet individu dans la barre et dans la plaque
					for(c=0;c<NbChanceDansBarre;c++)
					{
						int cts=c+1;
						ASSERT(VBi[cts]*CoeffEx>=0);
						NbContactBarre[iCib][H][s][c]+=VBi[cts]*CoeffEx;
					}
					NbChanceDansPlaque+=NbChanceDansBarre;
					if(NbChanceDansPlaque>NBFOISTOUCHEMAX)NbChanceDansPlaque=NBFOISTOUCHEMAX;
					if(NbChanceDansBarre>NbChanceSem[H][s])NbChanceSem[H][s]=NbChanceDansBarre;
					if(NbChanceDansPlaque>NbChanceCum[H][s])NbChanceCum[H][s]=NbChanceDansPlaque;
				} // Fin du cas d'une proba trouvée
			} // Fin du cas des semaines actives
			// Il faut étendre l'effet de la plaque même sur les semaines
			// qui ne sont pas actives pour cet individu car cette même semaine peut être 
			// active par l'intermédiaire d'un autre individu.
			for(c=0;c<NbChanceDansPlaque;c++)
			{
				int cts=c+1;
				ASSERT(VBc[cts]*CoeffEx>=0);
				NbContactPlaque[iCib][H][s][c]+=VBc[cts]*CoeffEx;
			}
		} // boucle sur les semaines
	} // boucle sur les individus
	NbChanceTot[H]=NbChanceCum[H][m_NbSemaine-1];
	m_Resultat.NbContactTot[iCib][H]=NbContacts/1000;

	for(s=0;s<m_NbSemaine;s++)
	{
		if(NbContactsParSemaine[s]>0)
		{
			DureeMoy[H][s]=NbSecondesParSemaine[s]/NbContactsParSemaine[s];
		}
		else
		{
			DureeMoy[H][s]=BASEFORMATREF;
		}

		if(s>0)
		{
			NbContactsCumulParSemaine[iCib][H][s]=NbContactsCumulParSemaine[iCib][H][s-1]+NbContactsParSemaine[s];
		}
		else
		{
			NbContactsCumulParSemaine[iCib][H][s]=NbContactsParSemaine[s];
		}


		/*
			//Test pour vérifier les contacts
		{
			double contacts=0;
			for(z=0;z<NBFOISTOUCHEMAX;z++)
				contacts+=NbContactPlaque[iCib][H][s][z]*(z+1);
			TRACE("%d\t%.f\t%.f\t\n",s,NbContactsCumulParSemaine[iCib][H][s],contacts);

		}
		*/
	}

	delete [] VBi;
	delete [] VBc;
	delete [] Macro;
		
	return 0; // OK
}



char CCampagne::Borne(char *s,int deb,int max)
{
	char i,j;
	j=0;
	while(!s[j+deb]) j++;

	i=1;
	while ((i+j+deb<max) && (s[i+j+deb-1]<s[i+j+deb]))
		i++;
	return(i);
}


double CCampagne::POWTEST(double x,double y){
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


// Pour avoir les variables dans les fichiers
// #define DEBUGGRAPHE
void CCampagne::CalculCourbeMemoDememo(int iCib,int Hypo)
{
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
	if(CalculDememo)KdeDememo=1.0;	else KdeDememo=0.0;

	b=Index[Hypo].b;
	v=Index[Hypo].v;
	w=Index[Hypo].w;
	z=Index[Hypo].z;
	s=Index[Hypo].s;
	p=Index[Hypo].p;
//	pi=(float)Fen->Abaques[iCib].Effectif;
	pi=m_Resultat.BasePopulation[iCib];
	T=m_NbSemaineCalcul;
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
		
		
		if(tps<m_NbSemaine && NbChanceSem[Hypo][tps]>0){
			c=c_=d=d_=+0.0;
			for(i=1;i<=NbChanceSem[Hypo][tps];i++) {
				d+=(float)i*(NbContactBarre[iCib][Hypo][tps][i-1]);
				c+=(float)(NbContactBarre[iCib][Hypo][tps][i-1]);
#ifdef DEBUGGRAPHE
	fprintf(fd2,"NbContactBarre[%d][%d][%d][%d]=%.0f\n",iCib,Hypo,tps,i-1,NbContactBarre[iCib][Hypo][tps][i-1]);
#endif


			}
			for(i=1;i<=NbChanceCum[Hypo][tps];i++) 
			{
				d_+=(float)i*(NbContactPlaque[iCib][Hypo][tps][i-1]);
				c_+=(float)(NbContactPlaque[iCib][Hypo][tps][i-1]);
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

				for(i=1;i<=NbChanceSem[Hypo][tps];i++) {
					beta=BetaSuivantModele(ValBeta[min(i-1,19)],DureeMoy[Hypo][tps]);
					jp=jp+beta*NbContactBarre[iCib][Hypo][tps][i-1];
					kp_=kp_+(beta/gamma)*NbContactPlaque[iCib][Hypo][tps][i-1];
					if(beta!=0){
						lp1_=lp1_+POWTEST(beta,phi)*(float)NbContactPlaque[iCib][Hypo][tps][i-1];
						lp2_=lp2_+coef*POWTEST(beta,teta)*(float)NbContactPlaque[iCib][Hypo][tps][i-1];
					}
					else {
						lp1_=lp1_;// pour éviter le domain error
						lp2_=lp2_;// pour éviter le domain error
					}
				}
				for(;i<=NbChanceCum[Hypo][tps];i++) {
					beta=BetaSuivantModele(ValBeta[min(i-1,19)],DureeMoy[Hypo][tps]);
					kp_=kp_+(beta/gamma)*NbContactPlaque[iCib][Hypo][tps][i-1];
					if(beta!=0){
						lp2_=lp2_+coef*POWTEST(beta,teta)*(float)NbContactPlaque[iCib][Hypo][tps][i-1];
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

	// division par 1000 pour convertir en milliers
	m_Resultat.NbIndivTouchesTot[iCib][Hypo]=c_/1000.0;

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

	m_Resultat.NbIndivMemoTot[iCib][Hypo]=m_/1000.0;
	m_Resultat.PtMemoCampTot[iCib][Hypo]=+0.0;

	for(i=0;i<T;i++)
	{
		MemoTot[iCib][Hypo][i]=M_[i]*(pi)/100.0;
		CouvTot[iCib][Hypo][i]=TabC_[i];
		DistTot[iCib][Hypo][i]=TabD_[i];
		if(SelectCalend[i])m_Resultat.PtMemoCampTot[iCib][Hypo]+=MemoTot[iCib][Hypo][i];
	}

	delete [] M_;
	delete [] TabC_;
	delete [] TabD_;
}



float CCampagne::BetaSuivantModele(float Beta0,float Duree)
{
	double alpha0,beta;

	// on se blinde dans le domaine matheu...
	if(Beta0<=0)return(0);
	if(Beta0>=1)return(1);
	if(Duree>BASEFORMATMAXI)Duree=BASEFORMATMAXI;
	if(Duree<=0){
		AfxMessageBox("Durée invalide dans fc BetaSuivantModele");
		return(0);
	}

	// calcul...
	alpha0=log(Beta0)/log(sin(BASEFORMATREF*M_PI/(2*BASEFORMATMAXI)));
	beta=POWTEST(sin(Duree*M_PI/(2*BASEFORMATMAXI)),alpha0);
	return(beta);
}

CCampagne::MODELEFORMAT * CCampagne::TouveBetaF(int Hypo,int Parcsp,int Sem)
{
	char *flag;
	int f,i;
	MODELEFORMAT *M,*Mnew;
	int Duree;
	flag=new char [m_NbFormat];

	// construction du masque de recherche 'format'
	for(f=0;f<m_NbFormat;f++){
		if((!fPiloteDurees) && (!(m_FlagFormat[f]&1)))continue;
		if(m_Passage[Hypo][Parcsp][Sem][f]){
			flag[f]=1;
		}
	}

	// recherche dans la liste des modèles existants
	for(M=ListeModeleFormat;M;M=M->Next){
		for(f=0;f<m_NbFormat;f++){
			if(M->FlagFormat[f]!=flag[f])break;
		}
		if(f>=m_NbFormat)break;
	}
	if(M)goto FinfcTouveBetaF;

	// construction d'un nouveau modèle
	Mnew=new MODELEFORMAT;
	Mnew->Next=NULL;
	for(f=0;f<m_NbFormat;f++)Mnew->FlagFormat[f]=flag[f];

	// calcul du nouveau modèle
	Duree=0;
	for(f=0;f<m_NbFormat;f++)if(flag[f])Duree+=atoi(m_LibFormat[f]);
	for(i=0;i<20;i++)Mnew->Beta[i]=BetaSuivantModele(ValBeta[i],Duree);

	// liaison du nouveau modèle dans la liste chainée
	if(ListeModeleFormat){
		for(M=ListeModeleFormat;M->Next;M=M->Next);
		M->Next=Mnew;
	}
	else 
	{
		ListeModeleFormat=M=Mnew;
	}
	M=Mnew;

	FinfcTouveBetaF:
	delete [] flag;
	return(M);
}

void CCampagne::LibererListeModelesBetaF(CCampagne::MODELEFORMAT *Mod){
	MODELEFORMAT *M;

	while(Mod){
		M=Mod->Next;
		delete Mod;
		Mod=M;
	}
}


void CCampagne::MiseAJourIndex(int Hypo)
{
	char s,z,b,v,w,p;
	int t;
	int max;
	int f;//,sp,Parc; //,NbSP
	char fQQChose;

	s=v=w=p=1;
	b=z=0;
	t=1;

	max = m_NbSemaine;
	//NbSP=GEtude->Univers.Binomes[0][Bino].NbParc;

	f=0; // en attendant de boucler
	//sp=0;// en attendant de boucler
	//Parc=NrDirectParc[Bino][sp]; // en attendant de boucler

	// positionne sur le premier blocs
	while (t<=max) 
	{
		if(NbChanceSem[Hypo][t-1]>0)goto FinWhile1;
		Index[Hypo].s[t-1]=0;
		Index[Hypo].z[t-1]=0;
		Index[Hypo].v[t-1]=0;
		Index[Hypo].w[t-1]=0;
		Index[Hypo].p[t-1]=0;
		Index[Hypo].b[t-1]=0;
		t++;
	}
	FinWhile1:

	while (t<=max) {
		while (t<=max) { //&& (Fen->Passage[Hypo][Parc][t-1][f]
			fQQChose=0;
			if(NbChanceSem[Hypo][t-1]>0)fQQChose=1;
			if(!fQQChose)goto FinWhile2;
			Index[Hypo].s[t-1]=s;
			z++;
			Index[Hypo].z[t-1]=z;
			Index[Hypo].v[t-1]=v;
			Index[Hypo].w[t-1]=0;
			Index[Hypo].p[t-1]=p;
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
			Index[Hypo].b[t-1]=b;
			s++;
			t++;
		}
		FinWhile2:
 		s=1;
		while((t<=max)) 
		{
			if(NbChanceSem[Hypo][t-1]>0)goto FinWhile3;
			Index[Hypo].s[t-1]=s;
			Index[Hypo].z[t-1]=z;
			Index[Hypo].v[t-1]=0;
			Index[Hypo].w[t-1]=w;
			Index[Hypo].p[t-1]=p;
			Index[Hypo].b[t-1]=b;
			s++;
			t++;
		}
		FinWhile3:
		s=1;
		v++;
		w++;
		p++;
	}

	z=Index[Hypo].z[max-1];
	p=Index[Hypo].p[max-1];
	b=Index[Hypo].b[max-1];
	if (Index[Hypo].v[max-1]) {
		s=1;
		w=Index[Hypo].v[max-1];
	}
	else {
		s=Index[Hypo].s[max-1]+1;
		w=Index[Hypo].w[max-1];
	}
	while (t<=m_NbSemaineCalcul) {
		Index[Hypo].s[t-1]=s;
		Index[Hypo].z[t-1]=z;
		Index[Hypo].v[t-1]=0;
		Index[Hypo].w[t-1]=w;
		Index[Hypo].p[t-1]=p;
		Index[Hypo].b[t-1]=b;
		s++;
		t++;
	}
}


// retourne une erreur


int CCampagne::ChargeCible(int NrCible,int iCib)
{

	Cible[iCib].Cible=NrCible; //  + 2;

	// on appelle l'application cible
	theApp.m_Cible.GetVectCible(&Cible[iCib]);

	// on calcule la somme du vecteur
	/*
	pop=0L;
	for(ind=0;ind<NBINDIVIDUS;ind++){
		GetLong64K(dlong,F->Cible.Vecteur,ind);
		pop+=dlong;
	}
	sprintf(tamponalloc,"cible= %ld",pop);
	AfficheMessage(tamponalloc);*/



	return(0);
}

void CCampagne::FreeMemory()
{

	if(!m_init)return;

	for(int h=0;h<NBHYPOTHESE;h++)
	{
		delete [] Index[h].s;
		delete [] Index[h].z;
		delete [] Index[h].b;
		delete [] Index[h].v;
		delete [] Index[h].w;
		delete [] Index[h].p;
	}

	FreeCoeffFormat();
	// libération des parcs
	FreeParc();
	// libération du budget
	FreeBudget();

	m_init=0;
}

void CCampagne::CalculCoeffFormat()
{
	// récupération de la table des formats
	CTableFormat * FormatTab = theApp.m_TableCinema.GetTableFormat(); 
	//FreeCoeffFormat();
	m_NbFormat = m_LibFormat.GetSize();
	CoeffFormat= new double *[m_NbFormat];
	for(int f=0;f<m_NbFormat;f++)
	{
		double coeff=+1.0;
		for(int i=0;i<FormatTab->GetSize();i++)
		{
			if(FormatTab->GetAt(i).GetLibelle()==m_LibFormat[f])
			{
				coeff = FormatTab->GetCoefficient(FormatTab->GetLibelle(i)); 
				break;
			}
		}
		ASSERT(i<FormatTab->GetSize());
		CoeffFormat[f]= new double [m_NbSemaine];
		for(int s=0;s<m_NbSemaine;s++)
		{
			CoeffFormat[f][s]=coeff;
		}
	}
}

void CCampagne::FreeCoeffFormat()
{
	if(CoeffFormat==NULL)return;
	for(int s=0;s<m_NbFormat;s++)
	{
		delete [] CoeffFormat[s];
	}
	delete [] CoeffFormat;
	CoeffFormat=NULL;
}


void CCampagne::CalculSyntheseDimensionelle()
{
	int i;

	double Eco,minEco,maxEco,moyEco,ETyEco,pasEco;
	double Couv,minCouv,maxCouv,moyCouv,ETyCouv,pasCouv;
	double Memo,minMemo,maxMemo,moyMemo,ETyMemo,pasMemo;
	double Ecart,A,B,C,Delta;

	// Calcul des minis, des maxi, des cumuls...
	// Le Premier est pris d'emblé (rapidité)
	minEco=maxEco=moyEco=m_TabCPM[0];
	minCouv=maxCouv=moyCouv=m_TabCouv[0];
	minMemo=maxMemo=moyMemo=m_TabMemo[0];
	for(i=1;i<m_NbTop20;i++)
	{
		// L'écomomie
		Eco=m_TabCPM[i];
		moyEco+=Eco;
		minEco=min(minEco,Eco);
		maxEco=max(maxEco,Eco);

		// La couverture
		Couv=m_TabCouv[i];
		moyCouv+=Couv;
		minCouv=min(minCouv,Couv);
		maxCouv=max(maxCouv,Couv);

		// La mémorisation
		Memo=m_TabMemo[i];
		moyMemo+=Memo;
		minMemo=min(minMemo,Memo);
		maxMemo=max(maxMemo,Memo);
	}

	// calcul des moyennes...
	moyEco/=m_NbTop20;
	moyCouv/=m_NbTop20;
	moyMemo/=m_NbTop20;

	// calcul de l'ecart type pour chaque grandeur...
	ETyEco=+0.0;
	ETyCouv=+0.0;
	ETyMemo=+0.0;
	// On boucle sur les écrans...
	for(i=0;i<m_NbTop20;i++){

		Eco=m_TabCPM[i];
		Ecart=Eco-moyEco;
		ETyEco+=(Ecart*Ecart);

		Ecart=m_TabCouv[i]-moyCouv;
		ETyCouv+=(Ecart*Ecart);

		Ecart=m_TabMemo[i]-moyMemo;
		ETyMemo+=(Ecart*Ecart);
	}
	ETyEco=(float)sqrt((double)(ETyEco/m_NbTop20));
	ETyCouv=(float)sqrt((double)(ETyCouv/m_NbTop20));
	ETyMemo=(float)sqrt((double)(ETyMemo/m_NbTop20));

	// calcul du pas de chaque grandeur...
	if(ETyEco) pasEco=m_PoidsEco/ETyEco;
	else pasEco=0.0;

	if(ETyCouv) pasCouv=m_PoidsCouv/ETyCouv;
	else pasCouv=0.0;

	if(ETyMemo) pasMemo=m_PoidsMemo/ETyMemo;
	else pasMemo=0.0;

	// calcul de la diagonale...
	A=(maxCouv-minCouv)*pasCouv;
	B=(maxEco-minEco)*pasEco;
	C=(maxMemo-minMemo)*pasMemo;

	Delta=sqrt(A*A+B*B+C*C)*0.01;

	// Si Delta est invalide...
	if(Delta==0.0){
		for(i=0;i<m_NbTop20;i++)m_Mixte[i]=+1.0;
	}

	// on calcul le vecteur Multi maintenant...
	// On boucle sur les écrans
	else for(i=0;i<m_NbTop20;i++){
		
		// La couverture
		A=(m_TabCouv[i]-minCouv)*pasCouv;

		// L'écomomie (inversé...)
		B=(maxEco-m_TabCPM[i])*pasEco;

		// La mémorisation
		C=(m_TabMemo[i]-minMemo)*pasMemo;

		// La racine carré de la somme des carés
		m_Mixte[i]=sqrt(A*A+B*B+C*C)/Delta;
		ASSERT(m_Mixte[i]>=0);
	}


	// Maintenant que m_Mixte existe, on construit le classement.

	for(i=0;i<m_NbTop20;i++){
		double Max=-1;
		int posmax=-1;
		for(int y=0;y<m_NbTop20;y++){
			if(m_Mixte[y]>Max)
			{
				Max=m_Mixte[y];
				posmax=y;
			}
		}

		if(posmax>=0)
		{
			m_ClasementMixte[i]=posmax;
			m_Mixte[posmax]=-m_Mixte[posmax];
		}
		else 
		{

			ASSERT(0);
			// Pour éviter un plantage:
			m_ClasementMixte[i]=0;
		}
	}
}


// Efface l'hypothèse h
void CCampagne::Efface(int h)
{
	for(int p=0;p<m_Selection.GetNbParcTotal();p++)
	{
		m_NbPassage[h][p]=0;
		for(int s=0;s<m_NbSemaine;s++)
		{
			for(int f=0;f<m_NbFormat;f++)
			{
				m_Passage[h][p][s][f]=0;
			}
		}
	}
}


void CCampagne::CopyHypothese(int HypSource,int HypDestination)
{
	for(int p=0;p<m_Selection.GetNbParcTotal();p++)
	{
		for(int s=0;s<m_NbSemaine;s++)
		{
			for(int f=0;f<m_NbFormat;f++)
			{
				m_Passage[HypDestination][p][s][f]=m_Passage[HypSource][p][s][f];
			}
		}
	}
}

// NrHypo==-1 pour copier dans presse-machin
void CCampagne::CopierHypotheseActive(int NrHypo)
{

	int p,s,f;
	if(NrHypo==m_HypotheseActive)return;

	if(NrHypo==-1){
		for(p=0;p<m_Selection.GetNbParcTotal();p++){
			for(s=0;s<m_NbSemaine;s++){
				for(f=0;f<m_NbFormat;f++){
					m_Passage[NBHYPOTHESE][p][s][f]=m_Passage[m_HypotheseActive][p][s][f];
				}
			} // boucle des semaines
		} // boucle des parcs
		fPressePapier=1;
	}
	else {
		for(p=0;p<m_Selection.GetNbParcTotal();p++){
			for(s=0;s<m_NbSemaine;s++){
				for(f=0;f<m_NbFormat;f++){
					m_Passage[m_HypotheseActive][p][s][f]=m_Passage[NrHypo][p][s][f];
				}
			} // boucle des semaines
		} // boucle des parcs
		//ChangeHypotheseActiveACCOGRIL(F->hWnd,F->Passage[F->HypActive],1);
		LanceCalcul(CCampagne::HYPOTHESES); // recalcule toutes les hypo !
	}
}

// on copie dans l'hypothese active ce qu'il y a dans le presse-machin
void CCampagne::CollerHypotheseActive()
{
	if(!fPressePapier)
	{
		AfxMessageBox("Le presse-papier est vide !");
		return;
	}
	CopierHypotheseActive(NBHYPOTHESE);
}


void CCampagne::CopierDansPressePapier()
{
	// NB:
	// Modif 32 bits:
	// strcat remplace PCText=stpcpy(PCText,tamponalloc);	
	
	int				TextSize=0; // taille du memoire pour FORMAT TEXT
	HGLOBAL			hGlobalText=0; // handle to global memory Text
	char 			*PCText,*TextPtr;
	int i,Hyp;
	char buf[128];
	float repet;

	char tamponalloc[1000];
   //GetProfileString("intl","sDecimal",",",tamponalloc,10);
   //Virgule=*tamponalloc;

	CString Unite,txt;
	Unite="e";

	// on évalue la longueur à allouer ...
	TextSize+=100; //titre
	TextSize+=100; //nom cible

	TextSize+=15*100;// 15 preformances
	TextSize+=m_NbSemaineCalcul*100;// la courbe de mémo-démémo

	hGlobalText=GlobalAlloc(GHND,(DWORD)TextSize);
	PCText=TextPtr=(char*)GlobalLock(hGlobalText);

	// le titre
	sprintf(tamponalloc,"%s\r\n\r\n","Performances");
	strcat(PCText,tamponalloc);

	// modif alain avril 2005 -- cible de base
	sprintf(tamponalloc,"%s\t%s\n\r\n","Cible de base",m_LibCible[0]);
	strcat(PCText,tamponalloc);

	// les performances LIBELLE*VALEURS
	
	Hyp=m_HypotheseActive;

	// Budget brut
	sprintf(tamponalloc,"%s\t%8.2f %s\r\n","Budget brut",m_Resultat.BudgetTot[Hyp],Unite);
	strcat(PCText,tamponalloc);

	// budget net
	sprintf(tamponalloc,"%s\t%5ld.00 %s\r\n","Budget net",m_Resultat.BudgetNet[Hyp],Unite);
	strcat(PCText,tamponalloc);

	// Nombre de contacts distribués
	sprintf(tamponalloc,"%s\t%8.0f\r\n","Nb de contacts",1000*m_Resultat.NbContactTot[0][Hyp]);
	strcat(PCText,tamponalloc);

	// GRP
	if (m_Resultat.BasePopulation[0]<=0)
		sprintf(tamponalloc,"%s\r\n","GRP");
	else 
	{
		//sprintf(tamponalloc,"%s\t%8.2f\r\n","GRP",100000*NbContactTot[0][Hyp]/BasePopulation[0]);
		txt.Format("%s\t%8.2f\r\n","GRP",m_Resultat.GRP(0,Hyp));
	}
	strcat(PCText,txt);

	// CPM brut
	if(m_Resultat.NbContactTot[0][Hyp]<=0)sprintf(tamponalloc,"%s\r\n","CPM brut");
	else 
	{
		sprintf(tamponalloc,"%s\t%8.2f %s\r\n","CPM brut",m_Resultat.CPMbrut(0,Hyp),Unite);
	}
	strcat(PCText,tamponalloc);

	// CPM net
	if(m_Resultat.NbContactTot[0][Hyp]<=0)sprintf(tamponalloc,"%s\r\n","CPM net");
	else 
	{
		sprintf(tamponalloc,"%s\t%8.2f %s\r\n","CPM net",m_Resultat.CPMnet(0,Hyp),Unite);
	}
	strcat(PCText,tamponalloc);

	// Population de la cible
	sprintf(tamponalloc,"%s\t%8.0f\r\n","Effectif cible",m_Resultat.BasePopulation[0]);

	//TransfoVirgule();
	strcat(PCText,tamponalloc);

	// NbIndiv touchés
	sprintf(tamponalloc,"%s\t%8.0f\r\n","Nb ind. touchés",1000*m_Resultat.NbIndivTouchesTot[0][Hyp]);
	strcat(PCText,tamponalloc);

	// C% cumulée
	if(m_Resultat.BasePopulation[0]<=0)sprintf(tamponalloc,"%s\r\n","C% cumulée");
	else sprintf(tamponalloc,"%s\t%8.2f %%\r\n","C% cumulée",m_Resultat.Couverture(0,Hyp));
	//TransfoVirgule();
	strcat(PCText,tamponalloc);

	// Répétition
	if(m_Resultat.NbIndivTouchesTot[0][Hyp]<=0)sprintf(tamponalloc,"%s\r\n","Répétition");
	else sprintf(tamponalloc,"%s\t%8.2f\r\n","Répétition",m_Resultat.Repetition(0,Hyp));
	strcat(PCText,tamponalloc);

	// NbIndiv ayant mémorisé
	sprintf(tamponalloc,"%s\t%8.0f\r\n","Nb ind. mémo 1+",1000*m_Resultat.NbIndivMemoTot[0][Hyp]);
	strcat(PCText,tamponalloc);

	// mémorisation moyenne
	sprintf(tamponalloc,"%s\t%8.2f %%\r\n","C% mémo moy.",m_Resultat.MemoMoy[0][Hyp]);
	strcat(PCText,tamponalloc);

	// Mémo à la semaine X
	sprintf(tamponalloc,"%s\t%8.2f %%\r\n","C% mémo +x sem.",m_Resultat.MemoNrSemaineX[0][Hyp]);
	strcat(PCText,tamponalloc);

	// points de mémo
	sprintf(tamponalloc,"%s\t%8.0f\r\n","Points de mémo",m_Resultat.PtMemoCampTot[0][Hyp]);
	strcat(PCText,tamponalloc);

	// RM
	if(m_Resultat.NbIndivTouchesTot[0][Hyp]<=0)sprintf(tamponalloc,"%s\r\n","Ratio de mémo");
	else sprintf(tamponalloc,"%s\t%8.2f\r\n","Ratio de mémo",m_Resultat.RatioMemo(0,Hyp));
	strcat(PCText,tamponalloc);

	// la courbe de mémo-démémo
//	sprintf(tamponalloc,"\r\n\r\n%s\r\n\r\n",GetDico0(SD_TITREDLGGRAPH));
	// le titre semaine	GRP	couverture	répétition	mémorisation
	sprintf(tamponalloc,"\r\n\r\n%s","Semaine");
	sprintf(buf,"\t%s","GRP");
	strcat(tamponalloc, buf);
	sprintf(buf,"\t%s","Couverture");
	strcat(tamponalloc, buf);
	sprintf(buf,"\t%s","Répétition");
	strcat(tamponalloc, buf);
	sprintf(buf,"\t%s\r\n\r\n","Mémorisation");
	strcat(tamponalloc, buf);
	strcat(PCText,tamponalloc);
	for(i=0;i<m_NbSemaineCalend;i++)
	{
		double GRP=NbContactsCumulParSemaine[0][Hyp][m_DebutCalend+i]*100.0/m_Resultat.BasePopulation[0];
		double Couverture=CouvTot[0][Hyp][m_DebutCalend+i]*100.0;
		if(CouvTot[0][Hyp][m_DebutCalend+i]>0.0)
		
			repet=GRP/Couverture;
		else
			repet=0.0;
		sprintf(tamponalloc,"%d\t%f\t%f\t%f\t%f\r\n",m_NrSemaine[m_DebutCalend+i],GRP, Couverture, repet, MemoTot[0][Hyp][m_DebutCalend+i]*100.0);
		strcat(PCText,tamponalloc);
	}

	// Bloc Info Pied de page
	// ID_PIEDPAGE1_POPNAT
	// ID_PIEDPAGE1_BAC_COUVERTS
	// ID_PIEDPAGE2
	CString Txt;
	
	if (BaseCalcul == 0)
		Txt.LoadString(ID_PIEDPAGE1_BAC_COUVERTS);
	else
		Txt.LoadString(ID_PIEDPAGE1_POPNAT);

	sprintf(tamponalloc,"\n%s\n",Txt);
	strcat(PCText,tamponalloc);

	Txt.LoadString(ID_PIEDPAGE2);
	sprintf(tamponalloc,"%s\n",Txt);
	strcat(PCText,tamponalloc);

	Txt.LoadString(ID_PIEDPAGE3);
	sprintf(tamponalloc,"%s\n",Txt);
	strcat(PCText,tamponalloc);

	if((PCText-TextPtr)>=TextSize)
	{
		AfxMessageBox("Erreur: Ecrasement de variable dans CCampagne::CopierDansPressePapier");
	}

	// on balance tout ça dans le clipboard
	GlobalUnlock(hGlobalText);
	if(!OpenClipboard(AfxGetMainWnd()->m_hWnd))
	{
		GlobalFree(hGlobalText);
		return; //(FALSE);
	}
	EmptyClipboard();
	SetClipboardData(CF_TEXT,hGlobalText);
	CloseClipboard();
	return; //(TRUE);
}


inline void CCampagne::GetCodeArrayFromSelection(int ligne,int semaine,CStringArray & CodeBAC,CWordArray & IndexCpx)
{
	CodeBAC.RemoveAll();
	IndexCpx.RemoveAll();

	switch(m_Selection.GetType(ligne))
	{
		case tagSelection::BAC:
			CodeBAC.Add(m_Selection.GetCode(ligne));
			IndexCpx.Add(-1); // BAC simple
		break;
		case tagSelection::COMPLEXE:
			{
				int indexCpx=m_Selection.GetIndex(ligne);
				tagComplexe Cpx=theApp.m_TableCinema.GetTableComplexe()->GetAt(indexCpx);
				int indexBAC=Cpx.m_indexBAC;
				tagBAC bac=theApp.m_TableCinema.GetTableBac()->GetAt(indexBAC);
				CodeBAC.Add(bac.m_Code); // BAC auquel il est attaché
				IndexCpx.Add(indexCpx); // BAC avec un Code de complexe
			}
		break;
		case tagSelection::PRODUIT:
		case tagSelection::PRODUITFIGE:
			{
				int indexProduit=m_Selection.GetIndex(ligne);
				tagProduit P=theApp.m_TableCinema.GetTableProduit()->GetAt(indexProduit);
				
				// NB: Pour les produits figé il faut faire un tableau different pour chaque semaine
				
				CTableBAC * pTableBAC=theApp.m_TableCinema.GetTableBac();
				CTableComplexe * pTableCpx=theApp.m_TableCinema.GetTableComplexe();

				// Ajout des BACs qui sont dans le produit
				for(int b=0;b<P.m_DescriptionIndexBac[semaine].GetSize();b++)
				{
					// On ajoute tous les BAC du produit
					CodeBAC.Add(pTableBAC->GetCodeUtilePourCalcul(P.m_DescriptionIndexBac[semaine][b]));
					IndexCpx.Add(-1); // BAC simple
				}
				
				// Ajout des complexes qui sont dans le produit
				for(int c=0;c<P.m_DescriptionIndexComplexe[semaine].GetSize();c++)
				{
					int indexCpx=P.m_DescriptionIndexComplexe[semaine][c];
					tagComplexe Cpx=pTableCpx->GetAt(indexCpx);
					int indexBAC=Cpx.m_indexBAC;
					tagBAC bac=theApp.m_TableCinema.GetTableBac()->GetAt(indexBAC);
					CodeBAC.Add(bac.m_Code); // BAC auquel il est attaché
					IndexCpx.Add(indexCpx); // BAC avec un Code de complexe
				}
			}
		break;

		default: ASSERT(0);
	}
}


// Préparation des bac utile par ligne
inline void CCampagne::PrepareParcActifs(int Hypothese)
{
	m_SemaineActive.SetSize(m_NbSemaine);
	for (int s=0;s<m_NbSemaine;s++)m_SemaineActive[s]=0;

	if(m_MapParcAtcif) delete [] m_MapParcAtcif;
	m_MapParcAtcif=new CByteArray[m_Selection.GetNbParcTotal()];

	for(int p=0;p<m_Selection.GetNbParcTotal();p++)
	{
		m_MapParcAtcif[p].SetSize(m_NbSemaine);
		int ligne=m_Selection.GetNrDirectBinome(p);
		if(!m_Selection.SelectBinome[ligne])
		{
			// Lorsque la ligne n'est pas sélectionnée
			for (int s=0;s<m_NbSemaine;s++)
			{
				m_MapParcAtcif[p].SetAt(s,0);
			}
			continue;
		}

		for (int s=0;s<m_NbSemaine;s++)
		{
			m_MapParcAtcif[p].SetAt(s,0);
			for(int f=0;f<m_NbFormat;f++)
			{
				if((!fPiloteDurees) && (!(m_FlagFormat[f]&1)))continue;
				if(m_Passage[Hypothese][p][s][f])
				{
					BYTE nbseconde=m_MapParcAtcif[p][s];
					nbseconde+=atoi(m_LibFormat[f]);
					m_MapParcAtcif[p].SetAt(s,nbseconde);
					m_SemaineActive[s]=1;
					break;
				}
			}
		}
	}
}



// Préparation des bacs utiles par ligne
// Ainsi que des Complexes qui sont à identifier dans les BAC.
inline void CCampagne::PrepareBacParLigne()
{
	CTableComplexe * TableCpx=theApp.m_TableCinema.GetTableComplexe();

	static CMap<CString,LPCSTR,BYTE,BYTE> mapBacNonTrouve;

	if(m_MapCpxUsedParLigne) delete [] m_MapCpxUsedParLigne;
	m_MapCpxUsedParLigne=new CByteArray[m_Selection.GetSize()];

	// TODO REMI : Alloué correctement lorsque fred modifie la sélection.
	// TODO REMI : Il est possible de faire ce calcul dans la fonction parent plutôt que dans le calcul couverture
/*
	if(!m_QuotaBacUsedParLigne)
	{
		// Première allocation des quotas
		m_QuotaBacUsedParLigne=new CDoubleArray2 * [theApp.m_NbBacProbabilise];
		for(int b=0;b<theApp.m_NbBacProbabilise;b++)
			m_QuotaBacUsedParLigne[b]=0;
	}

	// Allocation des quotas en fc du nombre de ligne:
	
	for(int b=0;b<theApp.m_NbBacProbabilise;b++)
	{
		if(m_QuotaBacUsedParLigne[b])delete [] m_QuotaBacUsedParLigne[b];
		m_QuotaBacUsedParLigne[b]=new CDoubleArray2 [m_Selection.GetSize()];
	}
	*/

	FreeQuotaBacUsedParLigne();
	if(!m_QuotaBacUsedParLigne)
	{
		// Première allocation des quotas
		// MODIF FRED: 03/07/2002 : bacs proba + bacs régie.
		m_QuotaBacUsedParLigne=new CDoubleArray2 * [theApp.m_NbBacProbabilise];

		// ERREUR CODE BAC ALAIN !!!!!!
		// MODIF FRED: 16/05/2002 BAC REGIE
		for(int b=0;b<theApp.m_NbBacProbabilise;b++)
			m_QuotaBacUsedParLigne[b]=new CDoubleArray2 [m_Selection.GetSize()];
	}

	int nbcpx=theApp.m_TableCinema.GetTableComplexe()->GetSize();
	int nbbac=theApp.m_NbBacProbabilise;
	for(int l=0;l<m_Selection.GetSize();l++)
	{
		m_MapCpxUsedParLigne[l].SetSize(nbcpx);
		for(int cpx=0;cpx<nbcpx;cpx++)
			m_MapCpxUsedParLigne[l][cpx]=0;
	}

	if(m_ListeCpxParLigne) delete [] m_ListeCpxParLigne;
	m_ListeCpxParLigne=new CWordArray[m_Selection.GetSize()];

	// ERREUR CODE BAC ALAIN !!!!!!
	// MODIF FRED : 16/05/2002 BAC REGIE
	// les bacs proba + les bacs régieA + bac régie M + Bac N
	/*
	m_ListeBacUtilise.SetSize(3*theApp.m_NbBacProbabilise);
	for(int b=0;b<3*theApp.m_NbBacProbabilise;b++)m_ListeBacUtilise[b]=0;
	*/
	m_ListeBacUtilise.SetSize(NB_BLOC_BAC*theApp.m_NbBacProbabilise);
	for(int b=0;b<NB_BLOC_BAC*theApp.m_NbBacProbabilise;b++)m_ListeBacUtilise[b]=0;

	if(m_ListeComplexeParBAC) delete [] m_ListeComplexeParBAC;
	// m_ListeComplexeParBAC=new CWordArray[3*theApp.m_NbBacProbabilise];
	m_ListeComplexeParBAC=new CWordArray[NB_BLOC_BAC*theApp.m_NbBacProbabilise];

	bool flag=0;
	bool flagBACNONtrouve=0;


	// A VIRER
	/*
	for(int ii=0;ii<679;ii++)
	{
		tagBAC bac  = theApp.m_TableCinema.GetTableBac()->GetAt(ii);
		int c= 0;
		c = c + 1;
	}
	*/


	for(int ligne=0;ligne<m_Selection.GetSize();ligne++)
	{
		if(!m_Selection.SelectBinome[ligne])continue;

		// Si c'est un produit figé il faut utiliser le nombre de semaines
		// sinon c'est qu'il n'y a qu'une semaine
		int NbSemaineLigne=1;
		if(m_Selection.GetType(ligne)==tagSelection::PRODUITFIGE)
		{
			int indexProduit=m_Selection.GetIndex(ligne);
			tagProduit P=theApp.m_TableCinema.GetTableProduit()->GetAt(indexProduit);
			ASSERT(P.m_flagProduitFige==1);
			NbSemaineLigne=P.GetDuree();
		}

		// ERREUR CODE BAC ALAIN !!!!!!
		// MODIF FRED : 16/05/2002 : BAC REGIE
		for(int b=0;b<nbbac;b++)
			m_QuotaBacUsedParLigne[b][ligne].SetSize(NbSemaineLigne);
		for(int sem=0;sem<NbSemaineLigne;sem++)
		{
			double zero=+0.0;
			for(int b=0;b<nbbac;b++)
				m_QuotaBacUsedParLigne[b][ligne].SetAt(sem,zero);

			// on recherche les BAC correspondants à la ligne
			CStringArray CodeBAC;
			CWordArray IndexCpx;
			GetCodeArrayFromSelection(ligne,sem,CodeBAC,IndexCpx);

			// On recherche dans les bac les codes trouvés
			int NbBac = CodeBAC.GetSize();
			for(int cod=0;cod<CodeBAC.GetSize();cod++)
			{
				int b;
				LPCSTR codebac=CodeBAC.GetAt(cod);

				if(theApp.m_MapIndexBac.Lookup(codebac,b))
				{
					if(!m_ListeBacUtilise[b])
					{
						m_ListeBacUtilise.SetAt(b,1);
					}

					// ERREUR CODE BAC ALAIN !!!!!!
					// MODIF FRED : 15/05/2002 BAC REGIE
					tagBAC bac = theApp.m_TableCinema.GetTableBac()->GetAt(b);
					/*
					CString CodeBacReel;
					CodeBacReel = codebac;
					CodeBacReel = CodeBacReel.Left(6);
					tagBAC bac = theApp.m_TableCinema.GetTableBac()->GetBac(CodeBacReel);
					*/
					
					// On récupère l'index du bac probabilisé à l'aide du code de base.
					CString code = bac.GetCode();
					
					int idxproba = theApp.m_TableCinema.GetTableBac()->FindIndexFromCode(code);
					
					WORD indexCpx=IndexCpx[cod];
					if(indexCpx!=(WORD)-1)
					{
						// On ajoute ce complexe dans la ligne
						m_ListeCpxParLigne[ligne].Add(indexCpx);
						m_MapCpxUsedParLigne[ligne][indexCpx]=1;

						// On indique que le bac est utilisé pour ses complexes

						zero=m_QuotaBacUsedParLigne[idxproba][ligne][sem]+TableCpx->GetAt(indexCpx).m_PartDeMarche;
						m_QuotaBacUsedParLigne[idxproba][ligne].SetAt(sem,zero);
						ASSERT(m_QuotaBacUsedParLigne[idxproba][ligne][sem]<=+1.00001);

						// On ajoute ce complexe si il n'est pas déjà dans la liste
						// des BAC qui sont à traiter
						for(int c=0;c<m_ListeComplexeParBAC[b].GetSize();c++)
						{
							if(m_ListeComplexeParBAC[b][c]==indexCpx)break;
						}
						if(c>=m_ListeComplexeParBAC[b].GetSize())
						{
							m_ListeComplexeParBAC[b].Add(indexCpx);
						}
					}
					else
					{
						// MODIF FRED: 03/07/2002: ajout du quota du bac régie.
						zero=m_QuotaBacUsedParLigne[idxproba][ligne][sem]+bac.m_Quota;
						m_QuotaBacUsedParLigne[idxproba][ligne].SetAt(sem,zero);
						ASSERT(m_QuotaBacUsedParLigne[idxproba][ligne][sem]<=+1.0);
					}
				}
				else 
				{
					BYTE v=1;
					if(!mapBacNonTrouve.Lookup(codebac,v))
					{
						CString txt;
						txt.Format("Erreur: Le BAC %s n'est pas trouvé dans le fichier des audiences",codebac);
						AfxMessageBox(txt);
						mapBacNonTrouve.SetAt(codebac,v);
					}
				}
			}	
		}
	}

	if(flag) // TODO comment préparer les messages ?
	{
		AfxMessageBox("Au moment de la préparation des statistiques le processus de préparation trouve des lignes orphelines de probabilités ????");
	}


	/*{
		// Info pour débuggage
		// On analyse les quotas par bac de la première semaine

		TRACE("\n-----ANALYSE DES QUOTAS PAR BAC PROBA-----\n");
		for(int ligne=0;ligne<m_Selection.GetSize();ligne++)
		{
			TRACE("Ligne %d\n",ligne);
			for (int idxproba=0;idxproba<theApp.m_NbBacProbabilise;idxproba++)
			{
				if (m_QuotaBacUsedParLigne[idxproba][ligne][0]>0)
				{
					TRACE("BACPROBA:%d\tQUOTA:%f\n",idxproba,m_QuotaBacUsedParLigne[idxproba][ligne][0]*1000000);
				}
			}
		}
		TRACE("\n\n\n");
	}*/

}


// Retourne 1 si une probabilité est trouvée
// Référence dans p1 et p2;
inline bool CCampagne::FindProbabilite(int Hyp,int NrIndividu, int Semaine, WORD &p1, WORD &p2, double & coeffCorrection,BYTE & NbSeconde)
{
	int indexIndividu=theApp.m_NrIndivDirect[NrIndividu];
	int indexBac=theApp.m_NrBacParIndiv[NrIndividu];

	ASSERT(indexBac>=0);

	double DemiParc=0;
	double Quota=+0.0;
	double ComptageTemps=+0.0;
	double QuotaNbSemaine=+0.0;

	for(int p=0;p<m_Selection.GetNbParcTotal();p++)
	{
		if(!m_MapParcAtcif[p][Semaine])continue;

		int ligne=m_Selection.GetNrDirectBinome(p);
		int NrSemaineProduit;

		if(m_Selection.m_TypeParLigne[ligne]==tagSelection::PRODUITFIGE)
		{
			// Pour le produit figé on ne traite que la première ligne
			if(p%2)continue;
			
			// Recherche du numéro de semaine du produit
			NrSemaineProduit=-1;
			for(int f=0;f<m_NbFormat;f++)
			{
				if((!fPiloteDurees) && (!(m_FlagFormat[f]&1)))continue;
				if((NrSemaineProduit=m_Passage[Hyp][p][Semaine][f])!=0)
				{
					break;
				}
			}
			
			if(NrSemaineProduit<1)
			{
				// Ce n'est pas normal de ne pas trouver 
				// une semaine puisque le produit est actif.
				ASSERT(0);
				continue;
			}
			NrSemaineProduit--;
			DemiParc=1;
		}
		else
		{
			// Complexe, BAC ou produit "libre"
			// On considère que les individus sont partagés
			// on n'utilise donc qu'un individu sur 2 pour un demi-parc.
			//if((p%2) != (indexIndividu%2))continue;
			NrSemaineProduit=0;
			DemiParc=.5; // Demi Parc
		}
		double q=m_QuotaBacUsedParLigne[indexBac][ligne][NrSemaineProduit]*DemiParc;


		// A VIRER
		/*
		double qBrest  = m_QuotaBacUsedParLigne[indexBac][0][NrSemaineProduit]*DemiParc; 
		double qBrestM = m_QuotaBacUsedParLigne[indexBac][1][NrSemaineProduit]*DemiParc;
		double qBrestN = m_QuotaBacUsedParLigne[indexBac][2][NrSemaineProduit]*DemiParc;
		*/
				
		Quota+=q;
		ComptageTemps  += m_MapParcAtcif[p][Semaine]*q;
		QuotaNbSemaine += m_NbPassageMoyen[Hyp][p]*q;
	}

	if(Quota>+0.0)
	{
		ASSERT(Quota<=1.0001);
		ASSERT(indexIndividu>=0 && indexIndividu<theApp.m_NbIndivSupp[indexBac]); 
		
		
		// Prise en compte de la probabilité à 30 jours
		// -------------------------------------------------------
		p1=theApp.m_Proba1[indexBac][indexIndividu];
		/*


  BUG Janvier 2004: Le bug détecté vien lorsque, avec les 195 commplexes de Paris, avec trois semaines,
  On ne trouve pas le même résultat avec le BAC ou avec les complexes...


		QuotaNbSemaine/=Quota;
		if(QuotaNbSemaine>=4)
		{
			// à 4 semaines et plus on est toujours à 100 % sur la proba 28 jours
			p1=theApp.m_Proba30[indexBac][indexIndividu];
		}
		else
		{
			if(QuotaNbSemaine<=1)
			{
			// à 1 semaine ou moins (0.5)on est toujours à 100 % sur la proba 1 semaine
			p1=theApp.m_Proba1[indexBac][indexIndividu];
			}
			else
			{
				ASSERT(QuotaNbSemaine>=1);
				// Avec une semaine on doit être à 100% sur la proba 1
				// sinon entre les deux on pondère
				QuotaNbSemaine=(QuotaNbSemaine-1)/3;
				ASSERT(QuotaNbSemaine>=0 && QuotaNbSemaine<=1);
				p1=( (1-QuotaNbSemaine) * theApp.m_Proba1[indexBac][indexIndividu] ) +( QuotaNbSemaine*theApp.m_Proba30[indexBac][indexIndividu] );
			}
		}
		*/
		p2=theApp.m_Proba2[indexBac][indexIndividu];
		//if(p2<p1)p2=p1; -> fait plus tard pour ne pas perturber le calcul des contacts
		
		// V3.5
		// LE REDRESSEMENT CNC EST EFFECTUE PLUS TARD
		//if(RedressementCNC)coeffCorrection=theApp.m_CoeffCorrectionCNC[indexBac]*Quota;
		//else coeffCorrection=Quota;
		coeffCorrection=Quota;
		
		NbSeconde=ComptageTemps/Quota;
		return true;
	}
	return 0;
}



void CCampagne::UpdateMap(int effaceformats)
{
	// On commence par initialiser le map avec des 0
	for(int f=0;f<NBFORMATMAX;f++)
	{
		if(m_FlagFormat[f]&1)break;
	}
	for(int sem=0;sem<m_NbSemaine;sem++){
		for(int parc=0;parc<m_Selection.GetNbParcTotal();parc++)
		{
			if(effaceformats)
			{
				for(int idf=0;idf<NBFORMATMAX;idf++)
				{
					// On remet à zéro pour le format actif
					m_MapInterdit[sem][parc][idf]=0;

				}
			}
			else 
			{
					m_MapInterdit[sem][parc][f]=0;
			}
		}
	}
	// On controle les passages
	// S'il y a un passage, on met à jour le map (on utilise pour cela la fonction IsIntersection de CSelection)
	// On boucle sur les semaines
		

	for(sem=0;sem<m_NbSemaine;sem++)
	{
		for(int parcA=0;parcA<m_Selection.GetNbParcTotal();parcA++)
		{
			int ligneA = m_Selection.GetNrDirectBinome(parcA);

			// des passages dans les autres formats
			for(int idxf=0;idxf<m_NbFormat;idxf++)		
			{		
				if(!m_Passage[m_HypotheseActive][parcA][sem][idxf])continue;		
					
				if(idxf!=f)			
				{					
					// interdiction venant des autres formats	
					m_MapInterdit[sem][parcA][f]=1;			
				}		
				// Mise à jour du map
				// On boucle sur les parcs à partir du parc actuel

				for(int parcB=0;parcB<m_Selection.GetNbParcTotal();parcB++)
				{
					int ligneB = m_Selection.GetNrDirectBinome(parcB);
					int fige=0;
					// la case sélectionnée appartient elle à une ligne de produit figé. 		
					if(m_Selection.GetType(ligneB)==tagSelection::PRODUITFIGE)
					{
						fige=1;		
					}		

					if(ligneA==ligneB)continue;

					if((parcA%2)==(parcB%2))
					{
						m_Selection.GetSize();
						if(m_Selection.m_MapIntersection[ligneA][ligneB]==1)
						{
							if(fige)	
							{	
								if(parcB%2==0)	
								{	
									m_MapInterdit[sem][parcB][f]=1;	
									m_MapInterdit[sem][parcB+1][f]=1;	
								}	
								else	
								{	
									m_MapInterdit[sem][parcB][f]=1;	
									m_MapInterdit[sem][parcB-1][f]=1;	
								}	

							}	
							else 	
							{	
								m_MapInterdit[sem][parcB][f]=1;	
							}	
							
						}
					}

				}
			}
		}
	}
}

// CALCUL FRED
// Préparation des vecteurs de cibles de la campagne
void CCampagne::PrepareVecteurIndividus(int cible)
{
	if(!BaseCalcul)
	{
		// préparation du masque régional
		m_Selection.PrepareMasqueRegional();
	}
	else
	{
		// préparation du masque national
		m_Selection.PrepareMasqueNational();

	}

	if(CibleUtile[cible].Vecteur==NULL)
	{
		CibleUtile[cible].Vecteur = new double[theApp.NbIndiv];
	}
	// On boucle sur les individus total et on construit le nouveau vecteur 	
	for(int ind=0;ind<theApp.NbIndiv;ind++)	
	{	
		// Initialisation à 0 du vecteur	
		CibleUtile[cible].Vecteur[ind] = 0;	
		if(m_Selection.m_MasqueRegional[ind]==1)	
		{	
			CibleUtile[cible].Vecteur[ind] =  Cible[cible].Vecteur[ind];	

		}
	}	

}

// CALCUL FRED
// calcul des cibles utiles = pour la base de calcul AD HOC
void CCampagne::CalculPopCibleUtile(int numcible)
{
	
	double pop=0;
	int nbdecas=0;
	double poptot=0; //population totale
	for(int ind=0;ind<theApp.NbIndiv;ind++)
	{
		pop+=CibleUtile[numcible].Vecteur[ind];
		if(CibleUtile[numcible].Vecteur[ind]>0)nbdecas++;
	}
	// Population
	// On somme le coefficient d'extrapolation pour tous les individus de la cible
	CibleUtile[numcible].PopPhy=pop;
	CibleUtile[numcible].PopPon=pop;

	// Nb de cas
	CibleUtile[numcible].NbCas = nbdecas;

	// Population universelle 
	double * CoeffEx =  theApp.CoeffEx;
	for(ind=0;ind<theApp.NbIndiv;ind++)
	{
		poptot+=CoeffEx[ind];

	}
	
	// Pourcentage universel
	if(poptot>0)
		CibleUtile[numcible].PctUni= (100.0*pop)/poptot;
	else
		CibleUtile[numcible].PctUni=0;

	/* A REMETTRE ALAIN
#ifdef _DEBUG
	double poptest=0;
	for(ind=0;ind<theApp.m_NbIndivSupp[295];ind++)
	{
		int nri = theApp.m_NrIndivSupp[295][ind];
		poptest+=theApp.CoeffEx[nri];
	}
#endif
	*/
}

void CCampagne::FreeParcPassage(char **** thePassages,int nb)
{
	for(int h=0;h<NBHYPOTHESE+1;h++)
	{
		for(int p=0;p<nb;p++)
		{
			for(int s=0;s<m_NbSemaine;s++)
			{
				delete [] thePassages[h][p][s];
			}
			delete [] thePassages[h][p];
		}
		delete [] thePassages[h];
	}
	delete [] thePassages;
}


// libération des variables définie par parcs 
void CCampagne::FreeParc()
{
	if(m_Passage)FreeParcPassage(m_Passage,m_Selection.GetNbParcTotal());

	for(int h=0;h<NBHYPOTHESE+1;h++)
	{
		delete [] m_NbPassage[h];
		delete [] m_NbPassageMoyen[h];
	}
	delete [] m_NbPassage;
	delete [] m_NbPassageMoyen;

	// vecteurs alloués pour le calcul
	for(h=0;h<NBHYPOTHESE;h++)
	{
		for(int p=0;p<m_Selection.GetNbParcTotal();p++)
		{
			delete [] NbSecInsert[h][p];
			delete [] m_NbPassageSem[h][p];
		}
		delete [] NbSecInsert[h];
		delete [] m_NbPassageSem[h];
	}

	// libération de MapInterdit
	for(int sem=0;sem<m_NbSemaine;sem++)
	{
		for(int parc=0;parc<m_Selection.GetNbParcTotal();parc++)
		{
			delete[] m_MapInterdit[sem][parc]; 
		}
		delete[] m_MapInterdit[sem]; 
	}
	delete m_MapInterdit;
}

// Initialise les variables liées aux parcs
void CCampagne::AllocParc()
{
	// Initialisation des passages
	m_Passage         = new char***[NBHYPOTHESE+1];
	m_NbPassage       = new int *[NBHYPOTHESE+1];
	m_NbPassageMoyen  = new double *[NBHYPOTHESE+1];
	for(int h=0;h<NBHYPOTHESE+1;h++)
	{
		m_Passage[h]=new char**[m_Selection.GetNbParcTotal()];
		m_NbPassage[h]=new int [m_Selection.GetNbParcTotal()];
		m_NbPassageMoyen[h]=new double [m_Selection.GetNbParcTotal()];
		for(int p=0;p<m_Selection.GetNbParcTotal();p++)
		{
			// initialisation du nombre de passages
			m_NbPassage[h][p]=0;
			m_NbPassageMoyen[h][p]=0;
			m_Passage[h][p]=0;
			m_Passage[h][p]=new char*[m_NbSemaine];
			for(int s=0;s<m_NbSemaine;s++)
			{
				m_Passage[h][p][s]=new char[NBFORMATMAX];
				for(int f=0;f<NBFORMATMAX;f++)
				{
					m_Passage[h][p][s][f]=0;
				}
			}
		}
	}
	// vecteurs alloués pour le calcul
	for(h=0;h<NBHYPOTHESE;h++)
	{
		NbSecInsert[h]    = new int * [m_Selection.GetNbParcTotal()];
		m_NbPassageSem[h] = new int * [m_Selection.GetNbParcTotal()];
		for(int p=0;p<m_Selection.GetNbParcTotal();p++)
		{
			NbSecInsert[h][p]    = new int [NBSEMAINEMAX];
			m_NbPassageSem[h][p] = new int [NBSEMAINEMAX];
		}
	}
	m_MapInterdit = new int **[m_NbSemaine];
	for(int sem=0;sem<m_NbSemaine;sem++)
	{
		// le map est définit par semaine et par demi-parc
		m_MapInterdit[sem] = new int *[m_Selection.GetNbParcTotal()];
		for(int parc=0;parc<m_Selection.GetNbParcTotal();parc++)
		{
			m_MapInterdit[sem][parc] = new int[NBFORMATMAX];
			for(int f=0;f<NBFORMATMAX;f++)
			{
				// intialisation du map
				m_MapInterdit[sem][parc][f]=0; 
			}
		}

	}
}

void CCampagne::FreeBudget()
{
	if(Budget==NULL)return;
	for(int h=0;h<NBHYPOTHESE;h++)
	{
		delete [] Budget[h];
	}
	delete [] Budget;
	Budget=NULL;
}

void CCampagne::AllocBudget()
{
	ASSERT(Budget==NULL);

	Budget=new double* [NBHYPOTHESE];
	for(int h=0;h<NBHYPOTHESE;h++)
	{
		fCalculStop[h]=0;
		EtatCalcul[h]=0;
		Budget[h]=new double[m_Selection.GetSize()];
		m_Resultat.BudgetNet[h]=0;
	}
}

void CCampagne::FreeQuotaBacUsedParLigne()
{
	if(m_QuotaBacUsedParLigne)
	{
		for(int b=0;b<theApp.m_NbBacProbabilise;b++)
			if(m_QuotaBacUsedParLigne[b])delete [] m_QuotaBacUsedParLigne[b];
		delete [] m_QuotaBacUsedParLigne;
	}
	m_QuotaBacUsedParLigne=NULL;
}

void CCampagne::SetInit()
{
	m_init=1;
}

// Test si semaine active
bool CCampagne::SemaineActive(int NoSemaine)
{
	if (m_SemaineActive[NoSemaine])
		return true;
	else
		return false;
}
