// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Print.h"
#include "JFCFileException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrint::CPrint()
{
	m_pCampagne=NULL;
	m_fAlloc=0;
}

CPrint::~CPrint()
{
	if(m_fAlloc==0)return;
	
	for(int b=0;b<m_NbBActif;b++)
	{
// TODO
		int nbparc=2; //theApp.TabBinome[m_NroBActifs[b]].NbParc;
		for(int p=0;p<nbparc;p++)
		{	
			for(int j=0 ; j<m_pCampagne->m_NbSemaine; j++)
			{	
				delete [] m_Spots[b][p][j];
			}
			delete [] m_Spots[b][p];
		}
		delete [] m_Passage[b];
		delete [] m_Spots[b];
	}
	delete [] m_Passage;
	delete [] m_Spots;

	m_fAlloc=0;
}


#define LARGCOLG	(22)
#define LARGCOL		(12)
#define LARGCOL2	(28)
#define COEFFPIXH	(190.0)

void CPrint::Initialise(CDC * pDC, SIniPrint * pInit)
{
	ASSERT(m_pCampagne!=NULL);


//	m_bmp.LoadBitmap(IDB_BITMAP_RMB);
	CString nomlogo;
	nomlogo.Format("%s\\NewlogoJfc.bmp",theApp.m_Chemin);
	LoadBitmap(pDC,nomlogo,0);

	pInit->m_NbLigneTete=17;
	pInit->m_NbLignePieds=6;    // avant 4
	pInit->m_Marge[0]=2;	//haut
	pInit->m_Marge[1]=2;	//bas
	pInit->m_Marge[2]=5;	//gauche
	pInit->m_Marge[3]=5;	//droite

	LoadFont(pDC, FONT_TIME7);
	LoadFont(pDC, FONT_TIME8);
	LoadFont(pDC, FONT_TIME8B);
	LoadFont(pDC, FONT_TIME10);
	LoadFont(pDC, FONT_TIME10B);
	LoadFont(pDC, FONT_TIME14);
	
	LoadPen(pDC, PEN_1NOIR);
	LoadPen(pDC, PEN_2NOIR);
	LoadPen(pDC, PEN_4NOIR);
//	pInit->m_PoliceDef=&m_Font[FONT_TIME12];
	pInit->m_fOrientation=0;		//0:libre 1:Paysage 2:Portrait

	SInfoTab InfoTab;

	
	// Ajout des tableaux...
	m_TableType.RemoveAll();

	if(m_pCampagne->m_PrintCalendrier)
	{	
		InitCalendrier();		

		InfoTab.m_NbLigneTitre=5;		
		InfoTab.m_NbLigneTete=2;		
		InfoTab.m_NbLigneCorps=m_NbBActif*2;		
		InfoTab.m_NbLignePieds=2;		
		InfoTab.m_NbLigneTotal=0;		
		InfoTab.m_NbLigneInterLigne=2;		
		InfoTab.m_fColonne=0;		
		InfoTab.m_NbCarGauche=0;		
		InfoTab.m_CarColonne.SetSize(0);		
		InfoTab.m_NbLigneGarantie=2;		
		pInit->m_InfoTab.Add(InfoTab);		
		m_TableType.Add(1);		

		if(m_NbTab>1)		
		{		
			InfoTab.m_NbLigneTitre=0;		
			pInit->m_InfoTab.Add(InfoTab);		
			m_TableType.Add(2);		
		}		
		if(m_NbTab>2)		
		{		
			pInit->m_InfoTab.Add(InfoTab);		
			m_TableType.Add(3);		
		}		
		if(m_NbLegende>0)		
		{		
			InfoTab.m_NbLigneTitre=5;		
			InfoTab.m_NbLigneTete=0;		
			InfoTab.m_NbLigneCorps=m_NbLegende+2;		
			InfoTab.m_NbLignePieds=0;		
			InfoTab.m_NbLigneTotal=0;		
			InfoTab.m_NbLigneInterLigne=2;		
			InfoTab.m_fColonne=0;		
			InfoTab.m_NbCarGauche=0;		
			InfoTab.m_CarColonne.SetSize(0);		
			InfoTab.m_NbLigneGarantie=1;		
			pInit->m_InfoTab.Add(InfoTab);		
			m_TableType.Add(4);		
		}		
	}
	if(m_pCampagne->m_PrintGraph)
	{

		// Mémo-Ddémémo		
		InfoTab.m_NbLigneTitre=7;		
		InfoTab.m_NbLigneTete=0;		
		InfoTab.m_NbLigneCorps=38; // ?		
		InfoTab.m_NbLignePieds=0;		
		InfoTab.m_NbLigneTotal=0;		
		InfoTab.m_NbLigneInterLigne=5;		
		InfoTab.m_fColonne=0;		
		InfoTab.m_NbCarGauche=0;		
		InfoTab.m_CarColonne.SetSize(0);		
		InfoTab.m_NbLigneGarantie=1;		

		int MaxCible = NBCIBLECALCUL;

		for(int nbcible=0;nbcible<NBCIBLECALCUL;nbcible++) 
		{

			// UNE page par cible
				if(m_pCampagne->m_NumCible[nbcible]==-1)continue; 
				pInit->m_InfoTab.Add(InfoTab);		
				m_TableType.Add(5+nbcible);	
		}	
	}
	if(m_pCampagne->m_PrintPerf)
	{
		int multicible=0;
		for(int cib=0;cib<NBCIBLECALCUL;cib++)
		{
			if(m_pCampagne->m_NumCible[cib]!=-1)
			{
				multicible++;
			}
			
		}
		if(multicible<2)
		{
			// Performances de la cible chargée		
			InfoTab.m_NbLigneTitre=5;		
			InfoTab.m_NbLigneTete=0;		
			InfoTab.m_NbLigneCorps=35;		
			InfoTab.m_NbLignePieds=0;		
			InfoTab.m_NbLigneTotal=0;		
			InfoTab.m_NbLigneInterLigne=3;		
			InfoTab.m_fColonne=0;		
			InfoTab.m_NbCarGauche=0;		
			InfoTab.m_CarColonne.SetSize(0);		
			InfoTab.m_NbLigneGarantie=1;		
			pInit->m_InfoTab.Add(InfoTab);		
			m_TableType.Add(0);	
		}
		
		else
		{
			// Performances multicible		
			InfoTab.m_NbLigneTitre=5;		
			InfoTab.m_NbLigneTete=0;		
			InfoTab.m_NbLigneCorps=41;		
			InfoTab.m_NbLignePieds=0;		
			InfoTab.m_NbLigneTotal=0;		
			InfoTab.m_NbLigneInterLigne=3;		
			InfoTab.m_fColonne=0;		
			InfoTab.m_NbCarGauche=0;		
			InfoTab.m_CarColonne.SetSize(0);		
			InfoTab.m_NbLigneGarantie=1;		
			pInit->m_InfoTab.Add(InfoTab);		
			m_TableType.Add(10);	
		}
	}

	if(m_pCampagne->m_PrintPerfAgglo)
	{

		// Performances sur les agglomérations	
		InfoTab.m_NbLigneTitre=5;		
		InfoTab.m_NbLigneTete=0;		
		InfoTab.m_NbLigneCorps=37;		
		InfoTab.m_NbLignePieds=0;		
		InfoTab.m_NbLigneTotal=0;		
		InfoTab.m_NbLigneInterLigne=3;		
		InfoTab.m_fColonne=0;		
		InfoTab.m_NbCarGauche=0;		
		InfoTab.m_CarColonne.SetSize(0);		
		InfoTab.m_NbLigneGarantie=1;		

		for(int nbcible=0;nbcible<NBCIBLECALCUL;nbcible++) 
		{

			// UNE page par cible
				if(m_pCampagne->m_NumCible[nbcible]==-1)continue; 
				pInit->m_InfoTab.Add(InfoTab);		
				m_TableType.Add(11+nbcible);	
		}	
	}

			
	//ASSERT(0);

}

void CPrint::InitCalendrier()
{
	m_NbBActif=0;
	int Hyp=m_pCampagne->m_HypotheseActive;
	//nb binomes actifs et noms
	for(int b=0;b<m_pCampagne->m_Selection.GetSize();b++)

	{	
		// CORRECTION BUG 20/02/2002:plantage dans les templates : abandon de l'utilisation du CStringArray Nom_Binome 

		if(m_pCampagne->m_Selection.SelectBinome[b])
		{
			m_NroBActifs.Add(b);
			m_NbBActif++;
		}
	}
	for(b=0;b<m_NbBActif;b++)
	{
		m_BBudgets.Add(m_pCampagne->Budget[Hyp][m_NroBActifs[b]]);
	}

	
	m_Passage=new int * [m_NbBActif];
	for(b=0;b<m_NbBActif;b++)
	{
		int nbparc=m_pCampagne->m_Selection.GetNbParc(m_NroBActifs[b]);
		m_Passage[b]=new int [nbparc];
		for(int p=0;p<nbparc;p++)
		{
			m_Passage[b][p]=0;
		}
		for(int fmt=0;fmt<m_pCampagne->m_NbFormat;fmt++)
		{
			if((!m_pCampagne->fPiloteDurees) && (!(m_pCampagne->m_FlagFormat[fmt]&1)))continue;
			for(int p=0;p<nbparc;p++)
				{	
				int NbP=0;
				for(int s=0;s<m_pCampagne->m_NbSemaine;s++)
				{
					int p2=m_pCampagne->m_Selection.GetNrDirectParc(m_NroBActifs[b],p);
					if(m_pCampagne->m_Passage[Hyp][p2][s][fmt])	NbP++;
				}
				m_Passage[b][p]+=NbP;
			}
	   	}
	}

	m_Spots=new char *** [m_NbBActif];
	for(int i=0 ; i<m_NbBActif ; i++)
	{	
		int nbparc=m_pCampagne->m_Selection.GetNbParc(m_NroBActifs[i]);
		m_Spots[i]=new char ** [nbparc];
		for(int p=0;p<nbparc;p++)
		{	
			m_Spots[i][p]=new char *[m_pCampagne->m_NbSemaine];
			for(int j=0 ; j<m_pCampagne->m_NbSemaine; j++)
			{	
				m_Spots[i][p][j]=new char [4];
				m_Spots[i][p][j][0]='0';
			}
		}
	}

	// puis les spots
	char L[100];
	m_NbLegende=0;
	for(int fmt=0;fmt<m_pCampagne->m_NbFormat;fmt++)
	{
		if((!m_pCampagne->fPiloteDurees) && (!(m_pCampagne->m_FlagFormat[fmt]&1)))continue;
		for(int b=0;b<m_NbBActif ;b++)
		{	
			int nbparc=m_pCampagne->m_Selection.GetNbParc(m_NroBActifs[b]);
			for(int p=0;p<nbparc;p++)
			{
				for(int s=0;s<m_pCampagne->m_NbSemaine;s++)
				{	
					if(m_pCampagne->m_Passage[Hyp][m_pCampagne->m_Selection.GetNrDirectParc(m_NroBActifs[b],p)][s][fmt])
					{	
						if(fmt>0 && m_Spots[b][p][s][0]!='0')
						{	/*if(Spots[b][p][s][0]>='A' && Spots[b][p][s][0]<='X')*/
							sprintf(L, "%s+%s", m_Spots[b][p][s], m_pCampagne->m_LibFormat[fmt]);
							for(i=0 ; i<m_NbLegende; i++)
							{	if(!strcmp(m_LegendeFormat[i][1], L))
									break;
							}
							if(i>=m_NbLegende)
							{	if(i>NBFORMATMAX-2)
								{	
									i=NBFORMATMAX-1;
									sprintf(m_LegendeFormat[i][0], "@");
									strcpy(m_LegendeFormat[i][1],"Autres");
									m_NbLegende=NBFORMATMAX;
								}
								else
								{	sprintf(m_LegendeFormat[i][0], "%c", i+65);
									strcpy(m_LegendeFormat[i][1], L);
									m_NbLegende++;
								}
							}
							strcpy(m_Spots[b][p][s], m_LegendeFormat[i][0]);
						}
						else strcpy(m_Spots[b][p][s], m_pCampagne->m_LibFormat[fmt]);
					}
				}
			}
		}
	}

	m_TSemaine[0][0]=0;
	m_NbTab=1;

	const int NBSEMCALEND=19/*24*/;
	
	if(m_pCampagne->m_NbSemaine>NBSEMCALEND)
	{	
		m_NbTab=2;
		m_TSemaine[0][1]=NBSEMCALEND+m_TSemaine[0][0];
		m_TSemaine[1][0]=m_TSemaine[0][1];
		if(m_pCampagne->m_NbSemaine>NBSEMCALEND*2)
		{	
			m_NbTab=3;
			m_TSemaine[1][1]=NBSEMCALEND*2+m_TSemaine[0][0];
			m_TSemaine[2][0]=m_TSemaine[1][1];
			m_TSemaine[2][1]=m_pCampagne->m_NbSemaine+m_TSemaine[0][0];
		}
		else
		{
			m_TSemaine[1][1]=m_pCampagne->m_NbSemaine+m_TSemaine[0][0];
		}
	}
	else
	{
		m_TSemaine[0][1]=m_pCampagne->m_NbSemaine+m_TSemaine[0][0];
	}
	m_fAlloc=1;
}




bool CPrint::DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig,short PremierNrTab,short DernierNrTab)
{
/*	CString txt;
	pDC->SelectObject(&m_Font[FONT_TIME8]);
	txt.Format("Tete Ligne %d     Page:%d ",NrLig,NrPage);
	EcritG(pDC,txt, pR);
	return 1;*/

	if (NrPage == 4)
		int stop = 0;

	// Est on en multicible?
	int Multicible	= 0;
	int NumCible	= 0;

	CCampagne * C=m_pCampagne;
	CString txt,libcible,indicecible;
	for(int cible=0;cible<NBCIBLECALCUL;cible++)
	{
		if(m_pCampagne->m_NumCible[cible]!=-1)
		{
			Multicible++;
		}
	}

	
	switch(NrLig)
	{
	case 0:
		//JFC Titre
		EcritG(pDC, "JFC", pR, 0, 100, 0, &m_Font[FONT_TIME10]);
		txt="Atelier Cinéma";
		pR->top=pR->top-pR->Height()/2;
		pR->bottom=pR->bottom+pR->Height()/2;
		EcritC(pDC, txt, pR, 20, 80, &m_Font[FONT_TIME14]);
		break;
	case 2:
		//nom de l'étude
		EcritC(pDC,C->m_pDoc->GetTitle(), pR, 0, 100, &m_Font[FONT_TIME8B]);
		break;
	case 3:{
		//date
		COleDateTime date=COleDateTime::GetCurrentTime();
		txt.Format("Le %02d/%02d/%4d - %02dH%02d", date.GetDay(), date.GetMonth(), date.GetYear(), date.GetHour(), date.GetMinute());
		EcritG(pDC, txt, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		txt.Format("Page %2d", NrPage+1);
		EcritD(pDC, txt, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		break;
		}
	case 4:
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;
	case 5:
	{
		// Vague
		// lecture du label dans fichier NomVague.txt
		CStdioFile NomVague;
		CString chemin;
		
		CJFCFileException e;
		CString nom;
		CString labelvague="75000 médiamétrie";
		nom.Format("%s\\SOURCE\\NomVague.txt",theApp.m_Chemin);
		// Chargement du fichier
		if( !NomVague.Open(nom, CFile::modeRead|CFile::typeText|CFile::shareDenyWrite, &e ) )   
		{
			CString msg;
			msg.Format("Impossible d'ouvrir %s : ",nom);
			e.AfficheErreur(msg);
		}
		else
		{
			// lecture du label
			NomVague.ReadString(labelvague);
		}

		EcritC(pDC,labelvague, pR, 0, 100, &m_Font[FONT_TIME8]);
		break;
	}
	case 6:{
		CRect R=*pR;
		R.top+=m_HLig/2;
		R.bottom+=5*m_HLig/2;
		
		RemplisRect(pDC, &R, RGB(192,255,192));
/*		if(m_pInfo->m_bPreview || flagCouleur
			RemplisRect(pDC, &R, RGB(192,255,192));
		else
			RemplisRect(pDC, &R, RGB(192,192,192));*/
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		}
		break;
	case 9:{
		CRect R=*pR;
		R.top-=3*m_HLig/2;
		R.bottom=R.top+m_HLig;

		txt="SIMULATION DE CAMPAGNE";

		EcritC(pDC,txt, &R, 0, 100, &m_Font[FONT_TIME8B]);
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;
		}
	case 10:
		txt="";
		NumCible = -1;

		if(Multicible<2)
		{
			EcritG(pDC, "Cible", pR, 1, 100, 0,&m_Font[FONT_TIME8]); 
			txt.Format(" : %s",C->m_LibCible[0]);
		}
		else
		{
			if(((m_TableType[PremierNrTab]>=5)&&(m_TableType[PremierNrTab]<10))||m_TableType[PremierNrTab]>=11)
			{
				int val=5;
				if(m_TableType[PremierNrTab]>=11)val=11;
				if(C->m_NumCible[m_TableType[PremierNrTab]-val]!=-1)
				{
					EcritG(pDC, "Cible", pR, 1, 100, 0,&m_Font[FONT_TIME8]); 
					int num = C->m_NumCible[m_TableType[PremierNrTab]-val];
					txt.Format(" : %s",theApp.m_Cible.m_LibCible[theApp.m_Cible.GetIndexFromNumero(num)]);
				}
			}
			else
			{
				if(m_TableType[PremierNrTab]==1)
				{
					EcritG(pDC, "Multicible", pR, 1, 100, 0,&m_Font[FONT_TIME8]);
					txt.Format(" : %s",C->m_LibCible[0]);
				}
				else
				{
					EcritG(pDC, "Multicible", pR, 1, 100, 0,&m_Font[FONT_TIME8]);
				}
			}
		}	

		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
		
		
		// Poids de la cible
		{
			int idxCible=-1;
			if((Multicible<2) || (m_TableType[PremierNrTab]==1))
				idxCible=0;
			else
			{
				int ValInd = m_TableType[PremierNrTab];


				// if((m_TableType[PremierNrTab]>=5) && (m_TableType[PremierNrTab]<10))
				if(((m_TableType[PremierNrTab]>=5)&&(m_TableType[PremierNrTab]<10))||m_TableType[PremierNrTab]>=11)
				{
					int val=5;
					if(m_TableType[PremierNrTab]>=11)val=11;
					if(C->m_NumCible[m_TableType[PremierNrTab]-val]!=-1)

					idxCible=m_TableType[PremierNrTab]-val;
					//idxCible=m_TableType[PremierNrTab]-5;
				}

			}
			if(idxCible>=0)
			{
				txt.Format("%.0f%%",C->CibleUtile[idxCible].PctUni);
				EcritC(pDC, txt, pR, 0, 100, &m_Font[FONT_TIME8]);
			}

			// ALAIN -- Bidouille
			NumCible = idxCible;

		}
		

		// Effectif de la cible
		txt="Effectif  :";
		EcritD(pDC, txt, pR,50,91,0,&m_Font[FONT_TIME8]);
		txt="";

		if(Multicible<2)
		{
			txt.Format("%9.0f",C->m_Resultat.BasePopulation[0]);
		}
		else
		{
			// if((m_TableType[PremierNrTab]>=5) && (m_TableType[PremierNrTab]<10))
			if(((m_TableType[PremierNrTab]>=5)&&(m_TableType[PremierNrTab]<10))||m_TableType[PremierNrTab]>=11)
			{
		
				int val=5;
				if(m_TableType[PremierNrTab]>=11)val=11;
				if(C->m_NumCible[m_TableType[PremierNrTab]-val]!=-1)
				{
					int idxCible;
					idxCible=m_TableType[PremierNrTab]-val;
					if (idxCible >= 0)
						txt.Format("%9.0f",C->CibleUtile[idxCible].PopPon);
				}
				else
					// ALAIN
					if (NumCible>0)
						txt.Format("%9.0f",C->CibleUtile[NumCible].PopPon);

			}
			else
			{	
				if(m_TableType[PremierNrTab]==1)
				{
					txt.Format("%9.0f",C->m_Resultat.BasePopulation[0]);
				}
				else
					// ALAIN
					if (NumCible > 0)
						txt.Format("%9.0f",C->m_Resultat.BasePopulation[NumCible]);

			}
		}

		EcritD(pDC, txt, pR,91,99,0,&m_Font[FONT_TIME8]);
		
		break;

	case 11:
		txt="Saisonnalité";
		EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
		if(C->CalculSaisonnalite)txt=" : Active";
		else txt=" : Inactive";
		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
		
		txt="Nombre d'interviews :";
		EcritD(pDC, txt, pR, 50, 91, 0, &m_Font[FONT_TIME8]);
		txt="";
		if(Multicible<2)
		{
			txt.Format("%ld",C->m_Resultat.NbCas[0]);
		}
		else 
		{

			//if((m_TableType[PremierNrTab]>=5)&& (m_TableType[PremierNrTab]<10))
			if(((m_TableType[PremierNrTab]>=5)&&(m_TableType[PremierNrTab]<10))||m_TableType[PremierNrTab]>=11)
			{
		
				int val=5;
				if(m_TableType[PremierNrTab]>=11)val=11;
				if(C->m_NumCible[m_TableType[PremierNrTab]-val]!=-1)
				//if(C->m_NumCible[m_TableType[PremierNrTab]-5]!=-1)
				{
					int idxCible;
					idxCible=m_TableType[PremierNrTab]-val;
					if (idxCible >= 0)
						//txt.Format("%ld",C->CibleUtile[m_TableType[PremierNrTab]-5].NbCas);
						txt.Format("%ld",C->CibleUtile[idxCible].NbCas);
				}
			}
			else
			{
				if(m_TableType[PremierNrTab]==1)
				{
					txt.Format("%ld",C->m_Resultat.NbCas[0]);
				}

			}
		}
		EcritD(pDC, txt, pR, 91, 99, 0, &m_Font[FONT_TIME8]);
	break;
	case 12:
		// CALCUL FRED
		txt="Base";
		EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
		if(!C->BaseCalcul)
		{
			txt=" : BAC couverts";/*" : Régionale"*/;
		}
		else txt=" : Nationale";
		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
	break;
	case 13:
		
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		
	break;

	case 14:
		txt="Campagne";
		EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
		txt=" : "+C->m_NomCampagne; 
		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
		txt="Centrale";
		EcritG(pDC, txt, pR, 50, 100, 0,&m_Font[FONT_TIME8]);
		txt=" : "+C->m_NomCentrale; 
		EcritG(pDC, txt, pR, 58, 100, 0,&m_Font[FONT_TIME8]);

	break;
	case 15:
		txt="Annonceur";
		EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
		txt=" : "+C->m_NomAnnonceur; 
		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
		txt="Description";
		EcritG(pDC, txt, pR, 50, 100, 0,&m_Font[FONT_TIME8]);
		txt=" : "+C->m_NomProduit; 
		EcritG(pDC, txt, pR, 58, 100, 0,&m_Font[FONT_TIME8]);

	break;


	case 16:
		{
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		CRect R=*pR;
		R.top=pR->top-(12*m_HLig);
		R.bottom=R.top+m_HLig;
		TraitV(pDC, &R, pR,0,100,&m_Pen[PEN_4NOIR]);
		}
	break;
	}
	return 1;
}

bool CPrint::DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig)
{
	/*DC->SelectObject(&m_Font[FONT_TIME8]);
	pDC->SelectObject(&m_Pen[PEN_1NOIR]);
	CString txt;
	txt.Format("Pieds Ligne %d     Page:%d ",NrLig,NrPage);
	EcritG(pDC,txt, pR);
	return 1;*/

	// on teste la ligne a dessiner
	CString txt;
	CCampagne * C=m_pCampagne;

	switch (NrLig)
	{
		case 1:
			{
				if (C->BaseCalcul == 0)
					txt.LoadString(ID_PIEDPAGE1_BAC_COUVERTS);
				else
					txt.LoadString(ID_PIEDPAGE1_POPNAT);
					
				EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
			}
			break;

		case 2:
			{
				txt.LoadString(ID_PIEDPAGE2);
				EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
			}
			break;

		case 3:
			{
				txt.LoadString(ID_PIEDPAGE3);
				EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
			}
			break;

		case 4:
			{
				CRect r;
				r=*pR;
				r.bottom=r.top+r.Height()*4;
				DessineBitmap( pDC, 0,&r, 0, 100 );
			}
			break;

		case 5:
	/*{
				// on calcule la position du logo
				CRect R  = *pR;
				R.bottom = R.top + 4 * m_HLig;
				// on dessine le logo
	//			DessineBitmap( pDC, &R, BITMAP_1, 0, 100 );
			}*/
			break;
	/*	case 7:
			{
				// on selectionne un stylo fin noir
				CPen *pPen = pDC->SelectObject( &m_Pen[PEN_1NOIR] );
				// on ecrit la raison sociale
				EcritC( pDC,"Copyright JFC(C)-2000", pR, 0, 100, &m_Font[FONT_TIME8] );
				// on restaure l'ancien stylo
				pDC->SelectObject(pPen);
			}
			break;*/
	}

	// on quitte
	return TRUE;

}

bool CPrint::DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig)
{
	//	ASSERT(AfxCheckMemory());
	CString txt;
	pDC->SelectObject(&m_Font[FONT_TIME8]);
	pDC->SelectObject(&m_Pen[PEN_2NOIR]);
	CCampagneAgglo * C=m_pCampagne;
	int Hyp=C->m_HypotheseActive;
	CRect R;

	// Attention modif Octobre
	//int IndiceCible = NrTab;
	int IndiceCible = 0;


	switch(m_TableType[NrTab])
	{
		case 0: // Performances
			switch(NrType){
				case PRINT_INTERLIGNE:break;
				case PRINT_TITRE:
					switch(NrLig){
						case 2:
							EcritC(pDC,"PERFORMANCES",pR,0,100,&m_Font[FONT_TIME8B]); 
						break;
					}
				break;
				case PRINT_CORPS:
					switch(NrLig){
						case 0:TraitH(pDC,pR,20,80);break;
						case 1:
							// Budget brut
							EcritG(pDC,"Budget brut",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_Resultat.BudgetTot[Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 2:TraitH(pDC,pR,20,80);break;
						case 3:
							// budget net
							EcritG(pDC,"Budget net",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%d.00 €",C->m_Resultat.BudgetNet[Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 4:
							TraitH(pDC,pR,20,80);
							R=*pR;
							R.OffsetRect(0,-R.Height()*4);
							TraitV(pDC,&R,pR,20,80);
							TraitV(pDC,&R,pR,60,80);
							break;
						case 5:TraitH(pDC,pR,20,80);break;
						case 6:
							// Résultat 
							txt="Distribution des contacts";
							EcritG(pDC,txt,pR,20,60,1,&m_Font[FONT_TIME8B]);
						break;
						case 7:TraitDoubleH(pDC,pR,20,80);break;
						case 8:
							// Nombre de contacts distribués
							EcritG(pDC,"Nombre de contacts distribués",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000.0*C->m_Resultat.NbContactTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 9:TraitH(pDC,pR,20,80);break;
						case 10:
							// GRP
							EcritG(pDC,"GRP",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_Resultat.NbContactTot[IndiceCible][Hyp]/C->m_Resultat.BasePopulation[IndiceCible]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 11:TraitH(pDC,pR,20,80);break;
						case 12:
							// CPM brut
							EcritG(pDC,"Coût par mille contacts brut",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_Resultat.BudgetTot[Hyp]/C->m_Resultat.NbContactTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 13:TraitH(pDC,pR,20,80);break;
						case 14:
							// CPM net
							EcritG(pDC,"Coût par mille contacts net",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_Resultat.BudgetNet[Hyp]/C->m_Resultat.NbContactTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 15:TraitDoubleH(pDC,pR,20,80);break;
						case 16:
							// NbIndiv touchés
							EcritG(pDC,"Nombre d'individus touchés au moins 1 fois",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_Resultat.NbIndivTouchesTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 17:TraitH(pDC,pR,20,80);break;
						case 18:
							{
							// C% cumulée
							EcritG(pDC,"Couverture cumulée en %",pR,20,60,1,&m_Font[FONT_TIME8]);
							float Nb = C->m_Resultat.NbIndivTouchesTot[IndiceCible][Hyp];
							float Pop = C->m_Resultat.BasePopulation[IndiceCible];
							txt.Format("%.2f",100000*C->m_Resultat.NbIndivTouchesTot[IndiceCible][Hyp]/C->m_Resultat.BasePopulation[IndiceCible]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
							}	
						break;
						case 19:TraitH(pDC,pR,20,80);break;
						case 20:
							// Répétition
							EcritG(pDC,"Répétition",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.NbContactTot[IndiceCible][Hyp]/C->m_Resultat.NbIndivTouchesTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 21:
							TraitH(pDC,pR,20,80);
							R=*pR;
							R.OffsetRect(0,-R.Height()*16);
							TraitV(pDC,&R,pR,20,80);
							TraitV(pDC,&R,pR,60,80);
							break;
						case 22:TraitH(pDC,pR,20,80);break;
						case 23:
							// Résultat Mémo-Démémo
							EcritG(pDC,"Résultat Mémo-Démémo",pR,20,60,1,&m_Font[FONT_TIME8B]);
						break;
						case 24:TraitDoubleH(pDC,pR,20,80);break;
						case 25:
							// NbIndiv ayant mémorisé
							EcritG(pDC,"Nombre d'individus ayant été mémorisé au moins 1 fois",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_Resultat.NbIndivMemoTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 26:TraitH(pDC,pR,20,80);break;
						case 27:
							// mémorisation moyenne
							EcritG(pDC,"Couverture mémorisée moyenne sur la période en %",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.MemoMoy[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 28:TraitH(pDC,pR,20,80);break;
						case 29:
							// Mémo à la semaine X
							EcritG(pDC,"Couverture mémorisée à +x semaines fin période en %",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.MemoNrSemaineX[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 30:TraitH(pDC,pR,20,80);break;
						case 31:
							// points de mémo
							EcritG(pDC,"Points de mémorisation (surface mémorielle)",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",C->m_Resultat.PtMemoCampTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 32:TraitH(pDC,pR,20,80);break;
						case 33:
							// RM
							EcritG(pDC,"Ratio de mémorisation",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.NbIndivMemoTot[IndiceCible][Hyp]/C->m_Resultat.NbIndivTouchesTot[IndiceCible][Hyp]);
							EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
						break;
						case 34:TraitH(pDC,pR,20,80);break;
					}
				case PRINT_PIEDS:break;
				case PRINT_TOTAL:break;
			}
		break;
		case 1: // Calendrier
		case 2: // Calendrier (2)
		case 3: // Calendrier (3)
			switch(NrType){
				case PRINT_INTERLIGNE:break;
				case PRINT_TITRE:
				
					switch(NrLig){
						case 2:
							EcritC(pDC,"CALENDRIER",pR,0,100,&m_Font[FONT_TIME8B]); 
						break;

					}
				break;
				case PRINT_TETE:
				if(NrLig)break;
				{
					R=*pR;
					CRect R2;
					int tab=m_TableType[NrTab]-1;

					// Le cadre et les traits verticaux
					TraitHtop(pDC,&R,0,100);

					R2=*pR; 
					R2.bottom=R2.top+R2.Height()*2;
					TraitHbottom(pDC,&R2,0,100);

					TraitVfull(pDC,&R2,&R2,0,28);
					TraitVfull(pDC,&R2,&R2,31,88);
					TraitVfull(pDC,&R2,&R2,88,100); 

					// Les entête Binôme P et Budget
					R2=R; R2.bottom=R2.top+2*R2.Height();
					EcritC(pDC,"Produits",&R2,0,28,&m_Font[FONT_TIME8B]);
					EcritC(pDC,"P",&R2,28,31,&m_Font[FONT_TIME8B]);
					EcritC(pDC,"Budget",&R2,88,100,&m_Font[FONT_TIME8B]);

					// calendrier
					// La ligne mois
					int j=m_TSemaine[tab][0];
					int AnneePrecedente=m_pCampagne->m_NrAnnee[j];
					int MoisPrecedent=m_pCampagne->m_NrMois[j];
					int PosMoisPrecedent=j;
					R2=*pR; 
					R2.bottom=R2.top+R2.Height()*2;
					R=*pR;
					for(int i=j;i<m_TSemaine[tab][1];i++)
					{	
						if(m_pCampagne->m_NrMois[i]==MoisPrecedent) continue;
						int col=31+3*(i-j);
						TraitVfull(pDC,&R2,&R2,col,col,&m_Pen[PEN_4NOIR]);
						if(i-PosMoisPrecedent>=3){
							txt.Format("%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
							int col1=31+3*(PosMoisPrecedent-j);
							EcritC(pDC,txt,pR,col1,col,&m_Font[FONT_TIME7]);
						}
						AnneePrecedente=m_pCampagne->m_NrAnnee[i];
						MoisPrecedent=m_pCampagne->m_NrMois[i];
						PosMoisPrecedent=i;
					}

					// on rajoute le dernier mois
					int col=31+3*(i-j);
					TraitVfull(pDC,&R2,&R2,col,col,&m_Pen[PEN_4NOIR]);
					if(i-PosMoisPrecedent>=3){
						txt.Format("%s %d",theApp.m_LibMois[MoisPrecedent-1],AnneePrecedente);
						int col1=31+3*(PosMoisPrecedent-j);
						EcritC(pDC,txt,pR,col1,col,&m_Font[FONT_TIME7]);
					}

					// La ligne entre les semaines et les mois
					TraitHbottom(pDC,&R,31,88,&m_Pen[PEN_1NOIR]);
					// On passe à la ligne des semaines maintenant
					R.OffsetRect(0,R.Height());


					j=m_TSemaine[tab][0];
					for(i=j;i<m_TSemaine[tab][1];i++)
					{	
						int col1=31+3*(i-j);
						int col2=34+3*(i-j);
						txt.Format("%d",m_pCampagne->m_NrSemaine[i]);
						EcritC(pDC,txt,&R,col1,col2,&m_Font[FONT_TIME7]);
					}
							
					/* La double ligne horizontale sous le calendrier
					TraitHbottom(pDC,&R,0,100,&m_Pen[PEN_2NOIR]);
					R2=R; R2.OffsetRect(0,R.Height()/10);
					TraitHbottom(pDC,&R2,0,100,&m_Pen[PEN_2NOIR]);
					R.OffsetRect(0,R.Height());*/


				}
				break;

				case PRINT_CORPS:
				{
					int tab=m_TableType[NrTab]-1;

					if(NrLig%2)break;
					{
						int e=1;
						e=e++;
						R=*pR;
						CRect R2;
						int tab=m_TableType[NrTab]-1;


						// Les colonnes

						// Le cadre et les traits verticaux
						
						R2=*pR;
						R2=R2;
						R2.bottom=R2.top+R2.Height()*2/*(m_NbBActif)*/;
						TraitVfull(pDC,&R2,&R2,0,28);
						TraitVfull(pDC,&R2,&R2,31,88);
						TraitVfull(pDC,&R2,&R2,88,100);
						
						// Tracé du quadrillage vertical de la grille
						for(int c=0;c<=19;c++)
						{
							int col=31+c*3;
							R2=*pR; 
							R2.OffsetRect(0,10); 
							R2.bottom=R2.top+R2.Height()*2/*(m_NbBActif)*/;
							R2.top-=pR->Height()/5;
							TraitVfull(pDC,&R2,&R2,col,col,&m_Pen[PEN_1NOIR]);
						}

						
						// Les binômes
						int b=NrLig/2;
//						for(int b=0;b<m_NbBActif;b++)
						{
							int bino=m_NroBActifs[b];
							EcritG(pDC,m_pCampagne->m_Selection.GetLibelle(bino),&R,0,28,1,&m_Font[FONT_TIME8]);
							TraitHbottom(pDC,&R,31,88,&m_Pen[PEN_1NOIR]);

							// Affichage des passages
							if(m_Passage[b][0]==m_Passage[b][1])
							{
								R2=R; R2.bottom=R2.top+R2.Height()*2;
								txt.Format("%d",m_Passage[b][0]);
								EcritC(pDC,txt,&R2,28,31,&m_Font[FONT_TIME8]);
							}
							else
							{
								R2=R;
								txt.Format("%d",m_Passage[b][0]);
								EcritC(pDC,txt,&R2,28,31,&m_Font[FONT_TIME7]);
								R2.OffsetRect(0,R2.Height());
								txt.Format("%d",m_Passage[b][1]);
								EcritC(pDC,txt,&R2,28,31,&m_Font[FONT_TIME7]);
							}

							// Le Budget
							/*if(tab==m_NbTab-1)
							{*/
							// Affichage dans tous les tableaux
							R2=R; 
							R2.bottom=R2.top+2*R2.Height();
							txt.Format("%d",m_BBudgets[b]);
							EcritD(pDC,txt,&R2,88,100,1,&m_Font[FONT_TIME8]);
							//}
							
							// Le trait horizontal entre les parcs
							R.OffsetRect(0,R.Height());
							TraitHbottom(pDC,&R,0,100,&m_Pen[PEN_2NOIR]);

							R.OffsetRect(0,R.Height());
						}

						// Affichage des spots dans la grille
						b=NrLig/2;
						//for(b=0;b<(m_NbBActif);b++)
						{
							int nbparc=2;
							for(int p=0;p<nbparc;p++)
							{
								for(int s=m_TSemaine[tab][0];s<m_TSemaine[tab][1];s++)
								{
									if(m_Spots[b][p][s][0]=='0')continue;
									R=*pR;
									R.OffsetRect(0,R.Height()*(p));
									int x=s-m_TSemaine[tab][0];
									int col1=31+x*3;
									int col2=col1+3;
									EcritC(pDC,m_Spots[b][p][s],&R,col1,col2,&m_Font[FONT_TIME7]);
								}
							}
						}
					}
				break;
				}
						
/*						
						
						case 1:
							// Budget brut
							EcritG(pDC,"Budget brut",pR,20,60,1,&m_Font[FONT_TIME8]);
							txt.Format("%d",C->BudgetTot[Hyp]);
							EcritD(pDC,txt,pR,60,79,1,&m_Font[FONT_TIME8]);
						break;
						case 34:TraitH(pDC,pR,20,80);break;*/
				case PRINT_PIEDS:
				{
					int tab=m_TableType[NrTab]-1;

					if (NrLig)break;
					{
						R=*pR;
						CRect R2;
						TraitVfull(pDC,&R2,&R2,88,100);
						R2=*pR; 
						R2.bottom=R2.top+R2.Height()*2;
						TraitHbottom(pDC,&R2,0,100);
						TraitVfull(pDC,&R2,&R2,0,28);
						TraitVfull(pDC,&R2,&R2,31,100/*88*/);
						//Le Total
						R2=R; 
						R2.bottom=R2.top+2*R2.Height();
						txt.Format("%.2f",m_pCampagne->m_Resultat.BudgetTot[Hyp]);
						EcritD(pDC,txt,&R2,88,100,1,&m_Font[FONT_TIME10B]);
						EcritD(pDC,"TOTAL :",&R2,50,88,1,&m_Font[FONT_TIME10B]);

						break;
					}
					break;
				}
				case PRINT_TOTAL:break;
			}
		break;
		case 4:
			switch(NrType){
				case PRINT_TITRE:
					switch(NrLig){
						case 2:
							EcritC(pDC,"LEGENDE",pR,0,100,&m_Font[FONT_TIME8B]); 
						break;
					}
				break;
				case PRINT_CORPS:
					if(NrLig)break;
					{
						R=*pR;
						// Les colonnes
						TraitH(pDC,&R,40,60);
						R.OffsetRect(0,R.Height());
						for(int l=0;l<m_NbLegende;l++)
						{
							EcritG(pDC,m_LegendeFormat[l][0],&R,40,45,1,&m_Font[FONT_TIME8]);
							EcritG(pDC,m_LegendeFormat[l][1],&R,45,60,1,&m_Font[FONT_TIME8]);
							R.OffsetRect(0,R.Height());
						}
						TraitH(pDC,&R,40,60);
						TraitV(pDC,pR,&R,40,45);
						TraitV(pDC,pR,&R,45,60);
					}
				break;
			}
		break;
		case 5:
		case 6: // Courbe de Mémo-Démémo
		case 7:
		case 8:
		case 9:
		{
			// Modif Alain Sept 2004 : en multi cible tenir compte du n° de la cible pour les résultats
			// soit NrCible
			// Bug d'avant : afficher toujours les résultats cible 0
			//
			int NrCible=m_TableType[NrTab]-5;
			switch(NrType){
				case PRINT_TITRE:
					switch(NrLig){
						case 2:
							EcritC(pDC,"COURBE DE MEMO-DEMEMO",pR,0,100,&m_Font[FONT_TIME8B]); 
						break;
						case 4:
							txt.Format("Points de mémorisation = %.2f",m_pCampagne->m_Resultat.PtMemoCampTot[NrCible][Hyp]);
							EcritC(pDC,txt,pR,0,100,&m_Font[FONT_TIME8B]); 
						break;
					}
				break;
				case PRINT_CORPS:
					if(NrLig)break;
					{
						R=*pR;
						R.OffsetRect(0,-R.Height());
						EcritG(pDC,"Couverture mémorisée",&R,0,100,0,&m_Font[FONT_TIME7]); 
						R=*pR;
						R.bottom=R.top+R.Height()*38;
						
						// enlever le comentaire pour voir la place réservée
						
						//TraitHtop(pDC,&R,0,100);
						//TraitHbottom(pDC,&R,0,100);
						//TraitVfull(pDC,&R,&R,0,100);

						int ox=R.Width()/30;
						int oy=pR->Height()*6;
						int nbsemaine=m_pCampagne->m_NbSemaineCalend;
						int ux=(R.Width()-ox)/nbsemaine;

						// Cadrillage en pointillé :

						// Cadrillage verticale
						CPen dotpen;
						dotpen.CreatePen(PS_DOT,1,RGB(0,0,0));
						for(int x=0;x<=nbsemaine;x++)
						{
							if(x==0)pDC->SelectObject(&m_Pen[PEN_1NOIR]);
							else pDC->SelectObject(&dotpen);
							pDC->MoveTo(R.left+ox+x*ux,R.top);
							pDC->LineTo(R.left+ox+x*ux,R.bottom-oy);
							txt.Format("%d",x);
							pDC->TextOut(R.left+ox+x*ux,R.bottom-oy,txt);
						}

						// calcul y max
						int ymax=0;
						for (int i=0;i<nbsemaine;i++)
						{
							if(ymax<m_pCampagne->MemoTot[NrCible][Hyp][i+m_pCampagne->m_DebutCalend]*100)
							{
	   							ymax=m_pCampagne->MemoTot[NrCible][Hyp][i+m_pCampagne->m_DebutCalend]*100;
							}
						}
						int Ech;
						if (ymax>80) Ech=5;
						else if (ymax>50) Ech=4;
						else if (ymax>30) Ech=3;
						else if (ymax>10) Ech=2;
						else Ech=1;

						// calcul pas des graduation
						int nby,unitey;
						
						switch(Ech) {
							case 1: // 0-10%
								nby=10;
								unitey=1;
							break;
							case 2: // 0-30 %
								nby=6;
								unitey=5;
							break;
							case 3: // 0-50%
								nby=10;
								unitey=5;
							break;
							case 4: // 0-80 %
								nby=8;
								unitey=10;
							break;
							case 5: // 0-100 %
								nby=10;
								unitey=10;
							break;
							default: ASSERT(0);
						}

						float coeffy=(R.Height()-oy)/(nby*unitey);
						int hy=R.top+coeffy*nby*unitey;

						for(int y=0;y<=nby;y++)
						{
							if(y==nby)pDC->SelectObject(&m_Pen[PEN_1NOIR]);
							else pDC->SelectObject(&dotpen);
							pDC->MoveTo(R.left+ox,R.top+y*unitey*coeffy);
							pDC->LineTo(R.right,R.top+y*unitey*coeffy);

							txt.Format("%d %%",(nby-y)*unitey);
							pDC->TextOut(R.left,R.top+y*unitey*coeffy,txt);
						}

						// Dessin de la courbe
						pDC->LineTo(R.left+ox,hy);
						float fy;
						for (i=0;i<nbsemaine;i++)
						{
							fy = m_pCampagne->MemoTot[NrCible][Hyp][i+m_pCampagne->m_DebutCalend]*100.0;
							fy*=coeffy;
							y=hy-fy;
							pDC->LineTo(R.left+ox+(1+i)*ux,y);
						}

						// La mémorisation moyenne
						/*fy = m_pCampagne->MemoMoy[NrCible][Hyp];
						fy*=coeffy/unitey;
						y=R.bottom-oy-fy;
						pDC->LineTo(R.left+ox+i*ux,y);
						pDC->LineTo(R.left+ox+i*ux,y);
						*/

						// Le tableau des valeures
						pDC->SelectObject(&m_Font[FONT_TIME7]);
						int hline=pR->Height();
						R.top=R.bottom-oy+2*hline;
						for (i=1;i<4;i++)
						{
							pDC->MoveTo(R.left,R.top+hline*i);
							pDC->LineTo(R.right,R.top+hline*i);
						}
						CRect Rc;
						Rc=R;
						Rc.right=Rc.left+ox;
						pDC->DrawText("GRP",&Rc,DT_CENTER|DT_VCENTER);
						Rc.OffsetRect(0,hline);
						pDC->DrawText("C %",&Rc,DT_CENTER|DT_VCENTER);
						Rc.OffsetRect(0,hline);
						pDC->DrawText("Répét.",&Rc,DT_CENTER|DT_VCENTER);
						Rc.OffsetRect(0,hline);
						pDC->DrawText("M %",&Rc,DT_CENTER|DT_VCENTER);

						for (i=0;i<nbsemaine;i++)
						{
							Rc=R;
							Rc.left=R.left+ox+i*ux;
							Rc.right=Rc.left+ux;

							pDC->MoveTo(Rc.left,R.top);
							pDC->LineTo(Rc.left,R.top+hline*4);
							
							// Le GRP
							float grp=m_pCampagne->NbContactsCumulParSemaine[NrCible][Hyp][i+m_pCampagne->m_DebutCalend]*100.0/m_pCampagne->m_Resultat.BasePopulation[NrCible];
							txt.Format("%.2f",grp);
							pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);
							
							// C%
							Rc.OffsetRect(0,hline);
							float couverture=m_pCampagne->CouvTot[NrCible][Hyp][i+m_pCampagne->m_DebutCalend]*100.0;
							txt.Format("%.2f",couverture);
							pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);

							// Repetition
							Rc.OffsetRect(0,hline);
							if(couverture>0)txt.Format("%.2f",grp/couverture);
							else txt="***";
							pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);

							// Mémorisation
							Rc.OffsetRect(0,hline);
							float memorisation=m_pCampagne->MemoTot[NrCible][Hyp][i+m_pCampagne->m_DebutCalend]*100.0;
							txt.Format("%.2f",memorisation);
							pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);

						}
					}
				break;
			}
		}
		break;
			case 10: // Performances multicible
			switch(NrType)
			{
				case PRINT_INTERLIGNE:break;
				case PRINT_TITRE:
					switch(NrLig){
						case 2:
							EcritC(pDC,"PERFORMANCES MULTICIBLES",pR,0,100,&m_Font[FONT_TIME8B]); 
						break;
					}
				break;
				case PRINT_CORPS:
					switch(NrLig){
						case 0: 
						{
							// Lignes d'entete: Nom des cibles
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{
								if(C->m_NumCible[cible]!=-1)
								{
									int num=C->m_NumCible[cible];
									txt=theApp.m_Cible.m_LibCible[theApp.m_Cible.GetIndexFromNumero(num)];
									short col=40+(cible*12);
									EcritC(pDC,txt,pR,col,col+12,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 1:

							TraitH(pDC,pR,0,100);
							R=*pR;
							R.OffsetRect(0,-R.Height()*2);
							TraitH(pDC,&R,40,100);
							TraitV(pDC,&R,pR,40,52);
							TraitV(pDC,&R,pR,64,76);
							TraitV(pDC,&R,pR,88,100);
							break;
						case 2:
						{
							EcritG(pDC,"Effectif",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int c=0;c<NBCIBLECALCUL;c++)
							{
							
								if(C->m_NumCible[c]!=-1)
								{
									txt.Format("%.0f",C->CibleUtile[c].PopPon); 
									short col=40+(c*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
							break;

						}
						case 3:TraitH(pDC,pR,0,100);break;

						case 4:
						{
							EcritG(pDC,"Nb d'interviews",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{
							
								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%ld",C->CibleUtile[cible].NbCas); 
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}

							}
							break;
						}

							break;
						case 5:
							TraitH(pDC,pR,0,100);
							R=*pR;
							R.OffsetRect(0,-R.Height()*4);
							TraitV(pDC,&R,pR,0,40);
							TraitV(pDC,&R,pR,40,52);
							TraitV(pDC,&R,pR,64,76);
							TraitV(pDC,&R,pR,88,100);

							break;
						case 6:TraitH(pDC,pR,0,100);break;

						case 7:
						{
							// Budget brut
							EcritG(pDC,"Budget brut",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{
								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f €",C->m_Resultat.BudgetTot[Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);

								}
							}
						}
						break;
						case 8:TraitH(pDC,pR,0,100);break;
						case 9:
						{
							// budget net
							EcritG(pDC,"Budget net",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%d.00 €",C->m_Resultat.BudgetNet[Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 10:
							TraitH(pDC,pR,0,100);
							R=*pR;
							R.OffsetRect(0,-R.Height()*4);
							TraitV(pDC,&R,pR,0,100);
							TraitV(pDC,&R,pR,40,52);
							TraitV(pDC,&R,pR,64,76);
							TraitV(pDC,&R,pR,76,88);

							break;
						case 11:TraitH(pDC,pR,0,100);break;
						case 12:
							// Résultat 
							txt="Distribution des contacts";
							EcritG(pDC,txt,pR,0,40,1,&m_Font[FONT_TIME8B]);

						break;
						case 13:TraitDoubleH(pDC,pR,0,100);break;
						case 14:
						{
							// Nombre de contacts distribués
							EcritG(pDC,"Nombre de contacts distribués",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{
								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.0f",1000.0*C->m_Resultat.NbContactTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
								
							}
						}
						break;
						case 15:TraitH(pDC,pR,0,100);break;
						case 16:
						{	// GRP
							EcritG(pDC,"GRP",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f",100000*C->m_Resultat.NbContactTot[cible][Hyp]/C->m_Resultat.BasePopulation[cible]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}

							}
						}
						break;
						case 17:TraitH(pDC,pR,0,100);break;
						case 18:
						{	// CPM brut
							EcritG(pDC,"Coût par mille contacts brut",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f €",C->m_Resultat.BudgetTot[Hyp]/C->m_Resultat.NbContactTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 19:TraitH(pDC,pR,0,100);break;
						case 20:
						{	// CPM net
							EcritG(pDC,"Coût par mille contacts net",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f €",C->m_Resultat.BudgetNet[Hyp]/C->m_Resultat.NbContactTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 21:TraitDoubleH(pDC,pR,0,100);break;
						case 22:
						{	// NbIndiv touchés
							EcritG(pDC,"Nombre d'individus touchés au moins 1 fois",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.0f",1000*C->m_Resultat.NbIndivTouchesTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}

						}
						break;
						case 23:TraitH(pDC,pR,0,100);break;
						case 24:
						{	// C% cumulée
							EcritG(pDC,"Couverture cumulée en %",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f",100000 * C->m_Resultat.NbIndivTouchesTot[cible][Hyp]/C->m_Resultat.BasePopulation[cible]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}

						}
						break;
						case 25:TraitH(pDC,pR,0,100);break;
						case 26:
						{	// Répétition
							EcritG(pDC,"Répétition",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f",C->m_Resultat.NbContactTot[cible][Hyp]/C->m_Resultat.NbIndivTouchesTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}

							}
						}
						break;
						case 27:
							TraitH(pDC,pR,0,100);
							R=*pR;
							R.OffsetRect(0,-R.Height()*16);
							TraitV(pDC,&R,pR,0,100);
							TraitV(pDC,&R,pR,40,52);
							TraitV(pDC,&R,pR,64,76);
							TraitV(pDC,&R,pR,88,100);
						
							break;
						case 28:TraitH(pDC,pR,0,100);break;
						case 29:
							// Résultat Mémo-Démémo
							EcritG(pDC,"Résultat Mémo-Démémo",pR,0,40,1,&m_Font[FONT_TIME8B]);
						break;
						case 30:TraitDoubleH(pDC,pR,0,100);break;
						case 31:
						{	// NbIndiv ayant mémorisé
							EcritG(pDC,"Nombre d'individus ayant été mémorisé au moins 1 fois",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.0f",1000*C->m_Resultat.NbIndivMemoTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 32:TraitH(pDC,pR,0,100);break;
						case 33:
						{	// mémorisation moyenne
							EcritG(pDC,"Couverture mémorisée moyenne sur la période en %",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f",C->m_Resultat.MemoMoy[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 34:TraitH(pDC,pR,0,100);break;
						case 35:
						{	// Mémo à la semaine X
							EcritG(pDC,"Couverture mémorisée à +x semaines fin période en %",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f",C->m_Resultat.MemoNrSemaineX[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}

						}
						break;
						case 36:TraitH(pDC,pR,0,100);break;
						case 37:
						{	// points de mémo
							EcritG(pDC,"Points de mémorisation (surface mémorielle)",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.0f",C->m_Resultat.PtMemoCampTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 38:TraitH(pDC,pR,0,100);break;
						case 39:
						{
							// RM
							EcritG(pDC,"Ratio de mémorisation",pR,0,40,1,&m_Font[FONT_TIME8]);
							for(int cible=0;cible<NBCIBLECALCUL;cible++)
							{

								if(C->m_NumCible[cible]!=-1)
								{
									txt.Format("%.2f",C->m_Resultat.NbIndivMemoTot[cible][Hyp]/C->m_Resultat.NbIndivTouchesTot[cible][Hyp]);
									short col=40+(cible*12);
									EcritD(pDC,txt,pR,col,col+12,1,&m_Font[FONT_TIME8]);
								}
							}
						}
						break;
						case 40:TraitH(pDC,pR,0,100);break;
					}

				case PRINT_PIEDS:break;
				case PRINT_TOTAL:break;
			break;
		}
		break;


		case 11:
		case 12: // Perf sur les agglomérations
		case 13:
		case 14:
		case 15:
		{
			int NrCible=m_TableType[NrTab]-11;
			switch(NrType){
				case PRINT_INTERLIGNE:break;

				case PRINT_TITRE:
					switch(NrLig)
					{
						case 2:
							EcritC(pDC,"PERFORMANCES SUR LES AGGLOMERATIONS",pR,0,100,&m_Font[FONT_TIME8B]); 
						break;
					}
					break;

				case PRINT_CORPS:
					switch(NrLig)
					{
						case 0:
							TraitH(pDC,pR,10,64);
							break;

						case 1:
							// Budget brut
							EcritG(pDC,"Budget brut",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_Resultat.BudgetTot[Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							//	txt.Format("%.2f €",C->m_ResultatAgglo.BudgetTot[Hyp]);
							//	EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 2:TraitH(pDC,pR,10,64);
							break;

						case 3:
							// budget net
							EcritG(pDC,"Budget net",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%d.00 €",C->m_Resultat.BudgetNet[Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							break;

						case 4:
							TraitH(pDC,pR,10,64);
							R=*pR;
							R.OffsetRect(0,-R.Height()*4);
							TraitV(pDC,&R,pR,10,51);
							TraitV(pDC,&R,pR,51,64);
							break;

						case 5:
							TraitH(pDC,pR,10,90);
							break;

						case 6:
							// Résultat 
							txt="Distribution des contacts";
							EcritG(pDC,txt,pR,10,51,1,&m_Font[FONT_TIME8B]);
							EcritC(pDC,"BAC",pR,51,64,&m_Font[FONT_TIME8B]);
							EcritC(pDC,"Agglo.",pR,64,77,&m_Font[FONT_TIME8B]);
							EcritC(pDC,"Migrés",pR,77,90,&m_Font[FONT_TIME8B]);
							break;

						case 7:
							TraitDoubleH(pDC,pR,10,90);
							break;

						case 8:
							// Nombre de contacts distribués
							EcritG(pDC,"Nombre de contacts distribués",pR,10,51,1,&m_Font[FONT_TIME8]);

							double ValCib0;
							ValCib0 = C->m_Resultat.NbContactTot[0][Hyp];
							double ValCib1;
							ValCib1 = C->m_Resultat.NbContactTot[1][Hyp];

							txt.Format("%.0f",1000.0*C->m_Resultat.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000.0*C->m_ResultatAgglo.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000.0*C->m_ResultatMigre.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							break;

						case 9:
							TraitH(pDC,pR,10,90);
							break;

						case 10:
							// GRP
							EcritG(pDC,"GRP",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_Resultat.NbContactTot[NrCible][Hyp]/C->m_Resultat.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_ResultatAgglo.NbContactTot[NrCible][Hyp]/C->m_ResultatAgglo.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_ResultatMigre.NbContactTot[NrCible][Hyp]/C->m_ResultatMigre.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							break;

						case 11:
							TraitH(pDC,pR,10,90);
							break;

						case 12:
							// CPM brut
							EcritG(pDC,"Coût par mille contacts brut",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_Resultat.BudgetTot[Hyp]/C->m_Resultat.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_ResultatAgglo.BudgetTot[Hyp]/C->m_ResultatAgglo.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 13:
							TraitH(pDC,pR,10,90);
							break;

						case 14:
							// CPM net
							EcritG(pDC,"Coût par mille contacts net",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_Resultat.BudgetNet[Hyp]/C->m_Resultat.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f €",C->m_ResultatAgglo.BudgetNet[Hyp]/C->m_ResultatAgglo.NbContactTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 15:
							TraitDoubleH(pDC,pR,10,90);
							break;

						case 16:
							// Effectif cible
							EcritG(pDC,"Effectif cible",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%8.0f",C->m_Resultat.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%8.0f",C->m_ResultatAgglo.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%8.0f",C->m_ResultatMigre.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							break;
	
						case 17:
							TraitH(pDC,pR,10,90);
							break;

						case 18:
							// NbIndiv touchés
							EcritG(pDC,"Nombre d'individus touchés au moins 1 fois",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_Resultat.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_ResultatAgglo.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_ResultatMigre.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							break;

						case 19:
							TraitH(pDC,pR,10,90);
							break;

						case 20:
							{
							// C% cumulée
							EcritG(pDC,"Couverture cumulée en %",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_Resultat.NbIndivTouchesTot[NrCible][Hyp]/C->m_Resultat.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_ResultatAgglo.NbIndivTouchesTot[NrCible][Hyp]/C->m_ResultatAgglo.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",100000*C->m_ResultatMigre.NbIndivTouchesTot[NrCible][Hyp]/C->m_ResultatMigre.BasePopulation[NrCible]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							}	
							break;

						case 21:
							TraitH(pDC,pR,10,90);
							break;

						case 22:
							// Répétition
							EcritG(pDC,"Répétition",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.NbContactTot[NrCible][Hyp]/C->m_Resultat.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,63,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_ResultatAgglo.NbContactTot[NrCible][Hyp]/C->m_ResultatAgglo.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,63,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_ResultatMigre.NbContactTot[NrCible][Hyp]/C->m_ResultatMigre.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							break;

						case 23:
							TraitH(pDC,pR,10,90);
							R=*pR;
							R.OffsetRect(0,-R.Height()*16);
							TraitV(pDC,&R,pR,10,51);
							TraitV(pDC,&R,pR,51,64);
							TraitV(pDC,&R,pR,77,90);
							break;

						case 24:
							TraitH(pDC,pR,10,90);
							break;

						case 25:
							// Résultat Mémo-Démémo
							EcritG(pDC,"Résultat Mémo-Démémo",pR,10,51,1,&m_Font[FONT_TIME8B]);
							break;

						case 26:
							TraitDoubleH(pDC,pR,10,90);
							break;

						case 27:
							// NbIndiv ayant mémorisé
							EcritG(pDC,"Nombre d'individus ayant été mémorisé au moins 1 fois",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_Resultat.NbIndivMemoTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_ResultatAgglo.NbIndivMemoTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",1000*C->m_Resultat.NbIndivMemoTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,77,90,1,&m_Font[FONT_TIME8]);
							break;

						case 28:
							TraitH(pDC,pR,10,90);
							break;

						case 29:
							// mémorisation moyenne
							EcritG(pDC,"Couverture mémorisée moyenne sur la période en %",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.MemoMoy[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_ResultatAgglo.MemoMoy[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 30:
							TraitH(pDC,pR,10,90);
							break;

						case 31:
							// Mémo à la semaine X
							EcritG(pDC,"Couverture mémorisée à +x semaines fin période en %",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.MemoNrSemaineX[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_ResultatAgglo.MemoNrSemaineX[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 32:
							TraitH(pDC,pR,10,90);
							break;

						case 33:
							// points de mémo
							EcritG(pDC,"Points de mémorisation (surface mémorielle)",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",C->m_Resultat.PtMemoCampTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.0f",C->m_ResultatAgglo.PtMemoCampTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 34:
							TraitH(pDC,pR,10,90);
							break;

						case 35:
							// Ratio de mémoristaion
							EcritG(pDC,"Ratio de mémorisation",pR,10,51,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_Resultat.NbIndivMemoTot[NrCible][Hyp]/C->m_Resultat.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,51,64,1,&m_Font[FONT_TIME8]);
							txt.Format("%.2f",C->m_ResultatAgglo.NbIndivMemoTot[NrCible][Hyp]/C->m_ResultatAgglo.NbIndivTouchesTot[NrCible][Hyp]);
							EcritD(pDC,txt,pR,64,77,1,&m_Font[FONT_TIME8]);
							break;

						case 36:
							TraitH(pDC,pR,10,90);
							break;
					}
				case PRINT_PIEDS:break;
				case PRINT_TOTAL:break;
			}
		break;




		}

		break;
		default:
			ASSERT(0);
	}

	return 1;
}







bool CPrint::DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab)
{
	CRect R;
	switch(m_TableType[NrTab])
	{
		case 0: // perf
			R=pR_haut;
			R.OffsetRect(0,pR_haut->Height()*22);
			TraitV(pDC,&R,pR_bas,20,80);
			TraitV(pDC,&R,pR_bas,60,80);
			//TraitV(pDC,pR_haut,pR_bas,20,80);
		break;

		case 11: // performances agglo cible 1
		case 12: // performances agglo cible 2
		case 13: // performances agglo cible 3
		case 14: // performances agglo cible 4
		case 15: // performances agglo cible 5
			R=pR_haut;
			R.OffsetRect(0,pR_haut->Height()*22);
			TraitV(pDC,&R,pR_bas,10,90);
			TraitV(pDC,&R,pR_bas,51,64);
			TraitV(pDC,&R,pR_bas,64,77);
			//TraitV(pDC,pR_haut,pR_bas,20,80);
		break;

		
		case 10: // performances mutlicible
			R=pR_haut;
			R.OffsetRect(0,pR_haut->Height()*28);
			TraitV(pDC,&R,pR_bas,0,40);
			TraitV(pDC,&R,pR_bas,40,52);
			TraitV(pDC,&R,pR_bas,64,76);
			TraitV(pDC,&R,pR_bas,88,100);

		break;

	}
	return 1;
}


void CPrint::SetInfo(CCampagneAgglo * pCampagne)
{
	m_pCampagne=pCampagne;
}

bool CPrint::DrawCol(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig,short NrCol,short NbCol)
{
	return(0);
}


