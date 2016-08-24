// PrintFilm.cpp: implementation of the CPrintFilm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "PrintFilm.h"
#include "JFCFileException.h"

extern CCinemaApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////////////
CPrintFilm::CPrintFilm()
{
	// Init pointeur campagne films 
	m_pCampagneFilm = NULL;
}

CPrintFilm::~CPrintFilm()
{
	delete calcul;
}

///////////////////////////////////////////////////////////////////////////////////////
// Récupération Campagne Films
//
void CPrintFilm::SetInfo(CCampagneFilm * pCampagneFilm)
{
	// Récup campagne films
	m_pCampagneFilm = pCampagneFilm;

	// Calcul des résultats et stocke dans Résultat
	calcul = new CCalculProduitFilm(m_pCampagneFilm);
	m_pResultEntree = calcul->CalculNbEntre(); 

}

// ----------------  Définition des constantes impressions films ---------------

// Maximum de films par page
#define MAXFILMPAGE	(30)   
// Maximum de colonnes semaine					
#define MAXCOLSEM   (5)

///////////////////////////////////////////////////////////////////////////////////////
// Initialisation des tableaux campagne flms à imprimer
//
void CPrintFilm::Initialise(CDC * pDC, SIniPrint * pInit)
{

	// Test validité Campagne Films
	ASSERT(m_pCampagneFilm!=NULL);

	// Chargement du logo
	CString nomlogo;
	nomlogo.Format("%s\\NewlogoJfc.bmp",theApp.m_Chemin);
	LoadBitmap(pDC,nomlogo,0);

	// Init pour entete générale impression
	pInit->m_NbLigneTete=17;
	pInit->m_NbLignePieds=5;    // avant 4
	pInit->m_Marge[0]=2;	//haut
	pInit->m_Marge[1]=2;	//bas
	pInit->m_Marge[2]=5;	//gauche
	pInit->m_Marge[3]=5;	//droite

	// Chargement des fonts utilisables
	LoadFont(pDC, FONT_TIME7);
	LoadFont(pDC, FONT_TIME8);
	LoadFont(pDC, FONT_TIME8B);
	LoadFont(pDC, FONT_TIME10);
	LoadFont(pDC, FONT_TIME10B);
	LoadFont(pDC, FONT_TIME14);
	
	// Chargement des épaisseurs 
	LoadPen(pDC, PEN_1NOIR);
	LoadPen(pDC, PEN_2NOIR);
	LoadPen(pDC, PEN_4NOIR);

	// Type d'orientation (0:libre 1:Paysage 2:Portrait)
	pInit->m_fOrientation=0;		

	// Structute stockage info tableau impression
	SInfoTab InfoTab;
	
	// Init Tableau des infos générales impression
	m_TableType.RemoveAll();

	// Init Impression Grille Films
	InitGrilleFilm();		


	// seulement 1 cible de référence - affichage grille films (y copris le total)
	for (int InxFilm = 0; InxFilm <= m_pCampagneFilm->m_TabProduitFilm.GetSize()+1;InxFilm++)
	{	
		for (int InxSem = 0;InxSem <= m_pCampagneFilm->m_NbSemaine;InxSem += MAXCOLSEM)
		{
			if (InxFilm % MAXFILMPAGE == 0) 
			{
				// Définition paramétres tableau grille film
				InfoTab.m_NbLigneTitre=7;		
				InfoTab.m_NbLigneTete=3;
				InfoTab.m_NbLigneCorps      = m_pCampagneFilm->m_TabProduitFilm.GetSize();
				InfoTab.m_NbLignePieds      = 0;		
				InfoTab.m_NbLigneTotal      = 0;		
				InfoTab.m_NbLigneInterLigne = 0;		
				InfoTab.m_fColonne          = 0;		
				InfoTab.m_NbCarGauche       = 0;		
				InfoTab.m_CarColonne.SetSize(0);		
				InfoTab.m_NbLigneGarantie   = 1;		
				pInit->m_InfoTab.Add(InfoTab);		
				m_TableType.Add(0);	

				// Définition paramétres tableau total en colonne grille film
				// + 1 tableau pour total en lignes
				InfoTab.m_NbLigneTitre			= 0;
				InfoTab.m_NbLigneTete			= 3;
				InfoTab.m_NbLigneCorps			= 0;   // 0
				InfoTab.m_NbLignePieds			= 0;   // 4
				InfoTab.m_NbLigneTotal			= 0;
				InfoTab.m_NbLigneInterLigne		= 5;
				InfoTab.m_fColonne				= 0;
				InfoTab.m_NbCarGauche			= 0;
				InfoTab.m_NbLigneGarantie		= 1;
				InfoTab.m_CarColonne.SetSize(0);
				pInit->m_InfoTab.Add(InfoTab);
				m_TableType.Add(1);	

			}
		}
	}

	// Nombre de tableau pour l'affichage grille film
	m_NbTabGrilleFilm = m_TableType.GetSize();
	

	// Init Impression Performances
	if(m_pCampagneFilm->m_PrintPerfFilm)
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
		m_TableType.Add(m_NbTabGrilleFilm);	
	}

	// Init Impression Graphe Mémo-démémo
	if(m_pCampagneFilm->m_PrintGraphFilm)
	{
		// Définition des paramètres tableau
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

		// seulement 1 cible de référence
		pInit->m_InfoTab.Add(InfoTab);		
		m_TableType.Add(m_NbTabGrilleFilm + 1);	
	}


			
}


void CPrintFilm::InitGrilleFilm()
{
	// Nombre de tab consacré à l'affichage grille films 
	m_NbTabGrilleFilm = 0;

}


///////////////////////////////////////////////////////////////////////////////////////
// Impression Entete Page
//
bool CPrintFilm::DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig,short PremierNrTab,short DernierNrTab)
{
	CString txt;

	// Récup Campagne Films
	CCampagneFilm * C = m_pCampagneFilm;

    // On n'est pas en multicible?
	int Multicible=0;

	switch(NrLig)
	{
	case 0:
		// JFC Titre
		EcritG(pDC, "JFC", pR, 0, 100, 0, &m_Font[FONT_TIME10]);
		txt="Atelier Cinéma";
		pR->top=pR->top-pR->Height()/2;
		pR->bottom=pR->bottom+pR->Height()/2;
		EcritC(pDC, txt, pR, 20, 80, &m_Font[FONT_TIME14]);
		break;

	case 2:
		// Nom de l'étude campagne films
		EcritC(pDC,C->m_pDoc->GetTitle(), pR, 0, 100, &m_Font[FONT_TIME8B]);
		break;

	case 3:{
		// Date période analyse
		COleDateTime date=COleDateTime::GetCurrentTime();
		txt.Format("Le %02d/%02d/%4d - %02dH%02d", date.GetDay(), date.GetMonth(), date.GetYear(), date.GetHour(), date.GetMinute());
		EcritG(pDC, txt, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		txt.Format("Page %2d", NrPage+1);
		EcritD(pDC, txt, pR, 0, 100, 0, &m_Font[FONT_TIME8]);
		break;
		}

	case 4:
		// Séparateur horizontal
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;

	case 5:{
		// Informations Vague en cours (!!! Attention EN DUR actuellement
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
		// Fin du cadre entete
		CRect R=*pR;
		R.top+=m_HLig/2;
		R.bottom+=5*m_HLig/2;
		RemplisRect(pDC, &R, RGB(192,255,192));
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		}
		break;

	case 9:{
		// Titre de l'analyse
		CRect R=*pR;
		R.top-=3*m_HLig/2;
		R.bottom=R.top+m_HLig;

		txt="SIMULATION DE CAMPAGNE FILMS";

		EcritC(pDC,txt, &R, 0, 100, &m_Font[FONT_TIME8B]);
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;
		}

	case 10:{
		// Information Cible de référence
		txt="";
		EcritG(pDC, "Cible", pR, 1, 100, 0,&m_Font[FONT_TIME8]); 
		txt.Format(" : %s",C->m_LibCible[0]);
		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
		
		// Effectif de la cible
		txt="Effectif  :";
		EcritD(pDC, txt, pR,50,91,0,&m_Font[FONT_TIME8]);
		txt="";

		txt.Format("%9.0f",C->m_Population);
		EcritD(pDC, txt, pR,91,99,0,&m_Font[FONT_TIME8]);
		
		break;
		}

	case 11:
		// Saisonnalité - Nombre d'interviews
		txt="Saisonnalité";
		EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);

		txt=" : Inactive";
		EcritG(pDC, txt, pR, 9, 100, 0,&m_Font[FONT_TIME8]);
		
		txt="Nombre de contacts :";
		EcritD(pDC, txt, pR, 50, 91, 0, &m_Font[FONT_TIME8]);
		txt="";
		txt.Format("%9.0f",C->m_NbIndiv);

		EcritD(pDC, txt, pR, 91, 99, 0, &m_Font[FONT_TIME8]);
		break;

	case 12:
		break;

	case 13:
		// Trait horizontal de fermeture
		TraitH(pDC, pR,0,100,&m_Pen[PEN_4NOIR]);
		break;

	case 14:{
		// Période analyse
		txt="Période Analyse";
		EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);

		// Debut et fin date analyse
		CString TxtDateDeb,TxtDateFin;
		TxtDateDeb = C->m_DateDebut.Format("%d-%m-%Y");
		COleDateTime DateFin;
		DateFin = C->m_DateDebut + COleDateTimeSpan(C->m_NbSemaine*7-1,0,0,0);
		TxtDateFin = DateFin.Format("%d-%m-%Y");

		// Affichage intervalle date
		txt.Format("%s%s%s%s"," : ",TxtDateDeb," / ",TxtDateFin); 
		EcritG(pDC, txt, pR, 12, 100, 0,&m_Font[FONT_TIME8]);
		}
		break;

	case 15:
		break;

	case 16:{
		// Fermeture Cadre
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


///////////////////////////////////////////////////////////////////////////////////////
// Impression des colonnes
//
bool CPrintFilm::DrawCol(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig,short NrCol,short NbCol)
{
	return(0);
}


///////////////////////////////////////////////////////////////////////////////////////
// Impression des tableaux résultats
//
bool CPrintFilm::DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig)
{
	CString txt;
	CRect R, RBas;


	// Style impression
	pDC->SelectObject(&m_Font[FONT_TIME8]);
	pDC->SelectObject(&m_Pen[PEN_2NOIR]);

	// Récupère campagne
	CCampagneFilm * C = m_pCampagneFilm;

	if (m_TableType[NrTab] < m_NbTabGrilleFilm)
	{

		// ------------------------- Impression Tab Grille Films ---------------------
		//
		switch(NrType)
		{
			case PRINT_INTERLIGNE:
				break;

			case PRINT_TITRE:

				switch(NrLig){
					case 4:
						{
							// Récup Date deb / Date Fin
							CString txt;
							int NoSemDeb = ((NrTab/2) * MAXCOLSEM);
							int NoSemFin = NoSemDeb + MAXCOLSEM-1;
							if (NoSemFin > m_pCampagneFilm->m_NbSemaine)
								NoSemFin = m_pCampagneFilm->m_NbSemaine-1;

							// Conversion en format date date deb/date fin	
							CString TxtDateDeb,TxtDateFin;
							COleDateTime DateDeb;
							DateDeb = C->m_DateDebut + COleDateTimeSpan(NoSemDeb*7,0,0,0);
							TxtDateDeb = DateDeb.Format("%d-%m-%Y"); 
							COleDateTime DateFin;
							DateFin = C->m_DateDebut + COleDateTimeSpan(NoSemFin*7+6,0,0,0);
							TxtDateFin = DateFin.Format("%d-%m-%Y"); 

							// Affichage
							txt.Format("%s%s au %s","GRILLE FILMS du ",TxtDateDeb,TxtDateFin);
							EcritC(pDC,txt,pR,0,100,&m_Font[FONT_TIME8B]); 
						}	
					break;
				}

				break;

			case PRINT_TETE:					
				{	
					if (NrTab % 2 == 0)
					{
						switch (NrLig)
						{
							case 0 :
								// Trait horizontal tableau
								if (((NrTab/2)+1) * MAXCOLSEM <= m_pCampagneFilm->m_NbSemaine)
									TraitH(pDC, pR,0,100);
								else
								{
									int NbSemRest = MAXCOLSEM - (((NrTab/2)+1) * MAXCOLSEM -  m_pCampagneFilm->m_NbSemaine);
									TraitH(pDC, pR,0,50 + NbSemRest*10);	
								}
								break;

							case 1 :{
								// ----------------  Entete Grille films -----------------
								R=*pR;
								RBas=*pR;
								R.OffsetRect(0,-pR->Height());
								RBas.OffsetRect(0,pR->Height());

								// Libellé films
								txt.Format("%s","Libellé films");
								EcritC(pDC,txt,pR,0,30,&m_Font[FONT_TIME8]);
								TraitV(pDC,&R,&RBas,0,30);

								// Tarif films
								txt.Format("%s","Tarif");
								EcritC(pDC,txt,pR,30,40,&m_Font[FONT_TIME8]);
								TraitV(pDC,&R,&RBas,30,40);

								// Entrée films
								txt.Format("%s","Tickets");
								EcritC(pDC,txt,pR,40,50,&m_Font[FONT_TIME8]);
								TraitV(pDC,&R,&RBas,40,50);

								// Nombre de semaines
								int NbSemaine = m_pCampagneFilm->m_NbSemaine;

								// Les entrées semaines
								for (int InxSem = 0; InxSem < MAXCOLSEM ;InxSem++ )
								{
									int NoSem = ((NrTab/2) * MAXCOLSEM) + InxSem + 1;

									if (NoSem <= m_pCampagneFilm->m_NbSemaine)
									{										
										txt.Format("%s %d","Sem",NoSem);
										EcritC(pDC,txt,pR,50+InxSem*10,50+(InxSem+1)*10,&m_Font[FONT_TIME8]);

										TraitV(pDC,&R,&RBas,50+InxSem*10,50+(InxSem+1)*10);
									}
								}

								break;
							}

							case 2:
								// Trait horizontal fin entete
								if (((NrTab/2)+1) * MAXCOLSEM <= m_pCampagneFilm->m_NbSemaine)
									TraitH(pDC, pR,0,100);
								else
								{
									int NbSemRest = MAXCOLSEM - (((NrTab/2)+1) * MAXCOLSEM -  m_pCampagneFilm->m_NbSemaine);
									TraitH(pDC, pR,0,50 + NbSemRest*10);	
								}
								break;
						}
						break;
					}

					else
					{
						// Impression des totaux en colonne
						switch (NrLig)
						{
							case 0 :
								// Trait horizontal tableau
								R=*pR;
								R.OffsetRect(0,-pR->Height()/8);
								if (((NrTab/2)+1) * MAXCOLSEM <= m_pCampagneFilm->m_NbSemaine)
								{
									TraitH(pDC, pR,0,100);
									TraitH(pDC, &R,0,100);
								}
								else
								{
									int NbSemRest = MAXCOLSEM - (((NrTab/2)+1) * MAXCOLSEM -  m_pCampagneFilm->m_NbSemaine);
									TraitH(pDC, pR,0,50 + NbSemRest*10);	
									TraitH(pDC, &R,0,50 + NbSemRest*10);	
								}
								break;

							case 1 :
								{
									// Encadrement couleur des totaux
									CRect R=*pR;
									R.top-=m_HLig/2;
									R.bottom+=m_HLig/2;

									if (((NrTab/2)+1) * MAXCOLSEM > m_pCampagneFilm->m_NbSemaine)
									{
										int NbSemRest = MAXCOLSEM - (((NrTab/2)+1) * MAXCOLSEM -  m_pCampagneFilm->m_NbSemaine);
										R.right = R.left + (R.right - R.left)/2 + (NbSemRest * (R.right - R.left)/10);
									}
									RemplisRect(pDC, &R, RGB(192,255,192));
									

									// Résultats totaux en colonne
									txt.Format("%s","Total");
									EcritC(pDC,txt,pR,0,30,&m_Font[FONT_TIME8]);

									// Les films à totaliser
									int DebFilm;
									int FinFilm;

									int NbTabSem = (m_pCampagneFilm->m_NbSemaine) / MAXCOLSEM + 1;
									DebFilm = ((NrTab / (NbTabSem*2)) * MAXFILMPAGE);
									FinFilm = DebFilm + MAXFILMPAGE;
									if (FinFilm >= m_pCampagneFilm->m_TabProduitFilm.GetSize())
										FinFilm = m_pCampagneFilm->m_TabProduitFilm.GetSize()-1;

									// Les totaux tarifs
									double TotTarif = 0.0;
									for (int InxFilm = DebFilm;InxFilm <= FinFilm;InxFilm++)
									{
										CProduitFilm ProduitFilm;
										ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(InxFilm);
										TotTarif += ProduitFilm.m_Tarif; 
									}
									txt.Format("%7.0f",TotTarif);
									EcritC(pDC,txt,pR,30,40,&m_Font[FONT_TIME8]);

									// Les totaux Entree Source
									long TotEntree = 0;
									for (InxFilm = DebFilm;InxFilm <= FinFilm;InxFilm++)
									{
										CProduitFilm ProduitFilm;
										ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(InxFilm);
										TotEntree += ProduitFilm.m_EntreeCNC; 
									}
									txt.Format("%7d",TotEntree);
									EcritC(pDC,txt,pR,40,50,&m_Font[FONT_TIME8]);

									// Les autres colonnes semaines
									for (int InxSem = 0; InxSem < MAXCOLSEM ;InxSem++ )
									{
										int NoSem = ((NrTab/2) * MAXCOLSEM) + InxSem; // + 1;

										if (NoSem < m_pCampagneFilm->m_NbSemaine)
										{	
									
											double TotEntree = 0.0;
											for (int InxFilm = DebFilm;InxFilm <= FinFilm;InxFilm++)
											{				
												double TotCNC = m_pResultEntree->m_NbEntreeCible[InxFilm][NoSem];
												TotEntree += m_pResultEntree->m_NbEntreeCible[InxFilm][NoSem];
												
											}
											txt.Format("%7.0f",TotEntree);
											EcritC(pDC,txt,pR,50+InxSem*10,50+(InxSem+1)*10,&m_Font[FONT_TIME8]);
											//TraitV(pDC,&R,&RBas,50+InxSem*10,50+(InxSem+1)*10);
										}
									}
								}
								break;

							case 2 :
								// Les traits verticaux
								R=*pR;
								RBas=*pR;
								R.OffsetRect(0,- 2* pR->Height());

								// Colonne Total / tarif / entrée
								TraitV(pDC,&R,&RBas,0,30);
								TraitV(pDC,&R,&RBas,30,40);
								TraitV(pDC,&R,&RBas,40,50);

								// Les autres colonnes semaines
								for (int InxSem = 0; InxSem < MAXCOLSEM ;InxSem++ )
								{
									int NoSem = ((NrTab/2) * MAXCOLSEM) + InxSem + 1;
									if (NoSem <= m_pCampagneFilm->m_NbSemaine)
									{										
										TraitV(pDC,&R,&RBas,50+InxSem*10,50+(InxSem+1)*10);
									}
								}

								// Trait horizontal fin total
								if (((NrTab/2)+1) * MAXCOLSEM <= m_pCampagneFilm->m_NbSemaine)
								{
									TraitH(pDC, pR,0,100);
									TraitH(pDC, &R,0,100);
								}
								else
								{
									int NbSemRest = MAXCOLSEM - (((NrTab/2)+1) * MAXCOLSEM -  m_pCampagneFilm->m_NbSemaine);
									TraitH(pDC, pR,0,50 + NbSemRest*10);	
									TraitH(pDC, &R,0,50 + NbSemRest*10);	
								}
								break;
						}
						break;
					}

				}

			case PRINT_CORPS:
				{	
					if (NrTab % 2 == 0)
					{
						R=*pR;
						RBas=*pR;
						R.OffsetRect(0,-pR->Height());
						RBas.OffsetRect(0,pR->Height());

						// Indice produit
						int InxFilm;
						int NbTabSem = (m_pCampagneFilm->m_NbSemaine) / MAXCOLSEM + 1;

						InxFilm = ((NrTab % NbTabSem) * MAXFILMPAGE + NrLig) % MAXFILMPAGE;

						if (InxFilm < m_pCampagneFilm->m_TabProduitFilm.GetSize())
						{
							// Recup Film
							CProduitFilm ProduitFilm;
							ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(InxFilm);

							// Ligne film 
							txt.Format("%s",ProduitFilm.m_Libelle);
							//EcritC(pDC,txt,pR,0,30,&m_Font[FONT_TIME8]);
							EcritG(pDC,txt,pR,0,30,1,&m_Font[FONT_TIME8]);
							TraitV(pDC,&R,&RBas,0,30);					

							// Tarif Film
							txt.Format("%7.0f",ProduitFilm.m_Tarif);
							EcritC(pDC,txt,pR,30,40,&m_Font[FONT_TIME8]);
							TraitV(pDC,&R,&RBas,30,40);					

							// Entree CNC
							txt.Format("%d",ProduitFilm.m_EntreeCNC);
							EcritC(pDC,txt,pR,40,50,&m_Font[FONT_TIME8]);
							TraitV(pDC,&R,&RBas,40,50);					

							// Les résultats semaine
							for (int InxSem = 0; InxSem < MAXCOLSEM ;InxSem++ )
							{
								int NoSem = ((NrTab/2) * MAXCOLSEM) + InxSem; //  + 1;
								if (NoSem < m_pCampagneFilm->m_NbSemaine)
								{	
									// Valeur Nb entréée semaine
									txt.Format("%7.0f",m_pResultEntree->m_NbEntreeCible[InxFilm][NoSem]);
									EcritC(pDC,txt,pR,50+InxSem*10,50+(InxSem+1)*10,&m_Font[FONT_TIME8]);
									
									// Le trait vertical associé
									TraitV(pDC,&R,&RBas,50+InxSem*10,50+(InxSem+1)*10);
								}
							}

						}
					}
				}
				break;
			

			case PRINT_PIEDS:
			{	
				TraitH(pDC, pR,0,100);
				break;
			}

		}

	}

	else if (m_TableType[NrTab] == m_NbTabGrilleFilm)
	{	

		// ------------------------- Impression Tab Performances ---------------------
		//
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
						txt.Format("%.2f €",C->m_BudgetBrut);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 2:TraitH(pDC,pR,20,80);break;
					case 3:
						// budget net
						EcritG(pDC,"Budget net",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%d.00 €",C->m_BudgetNet);
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
						txt.Format("%.0f",C->m_NbContacts);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 9:TraitH(pDC,pR,20,80);break;
					case 10:
						// GRP
						EcritG(pDC,"GRP",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f",C->m_GRP);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 11:TraitH(pDC,pR,20,80);break;
					case 12:
						// CPM brut
						EcritG(pDC,"Coût par mille contacts brut",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f €",C->m_CPMbrut);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 13:TraitH(pDC,pR,20,80);break;
					case 14:
						// CPM net
						EcritG(pDC,"Coût par mille contacts net",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f €",C->m_CPMnet);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 15:TraitDoubleH(pDC,pR,20,80);break;
					case 16:
						// NbIndiv touchés
						EcritG(pDC,"Nombre d'individus touchés au moins 1 fois",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.0f",C->m_NbIndiv);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 17:TraitH(pDC,pR,20,80);break;
					case 18:
						// C% cumulée
						EcritG(pDC,"Couverture cumulée en %",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f",100*C->m_NbIndiv/C->m_Population);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 19:TraitH(pDC,pR,20,80);break;
					case 20:
						// Répétition
						EcritG(pDC,"Répétition",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f",C->m_Repetition);
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
						txt.Format("%.0f",C->NbIndivMemoTot);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 26:TraitH(pDC,pR,20,80);break;
					case 27:
						// mémorisation moyenne
						EcritG(pDC,"Couverture mémorisée moyenne sur la période en %",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f",C->m_MemoMoy);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 28:TraitH(pDC,pR,20,80);break;
					case 29:
						// Mémo à la semaine X
						EcritG(pDC,"Couverture mémorisée à +x semaines fin période en %",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f",C->MemoTot[C->m_NbSemaineCalcul-1]);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 30:TraitH(pDC,pR,20,80);break;
					case 31:
						// points de mémo
						EcritG(pDC,"Points de mémorisation (surface mémorielle)",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.0f",C->PtMemoCampTot);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 32:TraitH(pDC,pR,20,80);break;
					case 33:
						// RM
						EcritG(pDC,"Ratio de mémorisation",pR,20,60,1,&m_Font[FONT_TIME8]);
						txt.Format("%.2f",C->m_RatioMemo);
						EcritD(pDC,txt,pR,60,80,1,&m_Font[FONT_TIME8]);
					break;
					case 34:TraitH(pDC,pR,20,80);break;
				}
			

			case PRINT_PIEDS:break;
			case PRINT_TOTAL:break;
		}

	}	

	else if (m_TableType[NrTab] == m_NbTabGrilleFilm+1)
	{
		// ------------------------- Impression Courne Mémo-Démémo --------------------
		//
	// ------------------------- Impression Courne Mémo-Démémo --------------------
		//
		switch(NrType){
			case PRINT_TITRE:
				switch(NrLig){
					case 2:
						EcritC(pDC,"COURBE DE MEMO-DEMEMO",pR,0,100,&m_Font[FONT_TIME8B]); 
					break;

					case 4:
						txt.Format("Points de mémorisation = %.2f",C->PtMemoCampTot);
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
					
					int ox=R.Width()/30;
					int oy=pR->Height()*6;

					// Nb de semaines de calcul
					// Correction fred
					// Affichage du nombre de semaine calendrier comme dans l'objet CPrint associé à Campagne.
					// A VERIFIER 
					int nbsemaine=C->m_NbSemaineCalend;
					int ux=(R.Width()-ox)/nbsemaine;

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
					double ymax=0;

					for (int i=0;i<nbsemaine;i++)
					{
						if(ymax<C->MemoTot[i])
						{
							// MODIF FRED
							// la memo n'est plus divisée par 100 car elle est calculée différememnt dans les nouvelles 
							// classes de calcul.
	   						ymax=C->MemoTot[i];
						}
					}
					// MODIF FRED
					// Ajout d'échelles adaptées aux nouvelles valeurs de la couverture mémorisée
					int Ech;
					if (ymax>80.0) Ech=10;
					else if (ymax>50.0) Ech=9;
					else if (ymax>30.0) Ech=8;
					else if (ymax>10.0) Ech=7;
					else if (ymax>5.0) Ech=6;
					else if (ymax>3.0) Ech=5;
					else if (ymax>1.0) Ech=4;
					else if (ymax>0.5) Ech=3;
					else if (ymax>0.3) Ech=2;
					else if (ymax>0.1) Ech=1;



					else Ech=1;

					// calcul pas des graduation
					double nby,unitey;
					
					switch(Ech) {
						case 1: // 0-0.3 %
							nby=6;
							unitey=0.05;
						break;
						case 2: // 0-0.5 %
							nby=10;
							unitey=0.05;
						break;
						case 3: // 0-1 %
							nby=10;
							unitey=0.1;
						break;
						case 4: // 0-3 %
							nby=6;
							unitey=0.5;
						break;
						case 5: // 0-5 %
							nby=10;
							unitey=0.5;
						break;
						case 6: // 0-10%
							nby=10;
							unitey=1;
						break;
						case 7: // 0-30 %
							nby=6;
							unitey=5;
						break;
						case 8: // 0-50%
							nby=10;
							unitey=5;
						break;
						case 9: // 0-80 %
							nby=8;
							unitey=10;
						break;
						case 10: // 0-100 %
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

						txt.Format("%2.2f %%",(nby-y)*unitey);
						pDC->TextOut(R.left,R.top+y*unitey*coeffy,txt);
					}

					// Dessin de la courbe
					pDC->LineTo(R.left+ox,hy);
					float fy;

					// MODIF FRED
					// Nouvelles mémorisations
					for (i=0;i<nbsemaine;i++)
					{
						fy = C->MemoTot[i]/**100.0*/;
						fy*=coeffy;
						y=hy-fy;
						pDC->LineTo(R.left+ox+(1+i)*ux,y);
					}

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

						// MODIF FRED
						// Nouvelles mémorisations

						// Le GRP
						float grp=C->DistTot[i]/**100.0*/;
						txt.Format("%.2f",grp);
						pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);
						
						// C%
						Rc.OffsetRect(0,hline);
						float couverture=C->CouvTot[i]/**100.0*/;
						txt.Format("%.2f",couverture);
						pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);

						// Repetition
						Rc.OffsetRect(0,hline);
						if(couverture>0)txt.Format("%.2f",grp/couverture);
						else txt="***";
						pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);

						// Mémorisation
						Rc.OffsetRect(0,hline);
						float memorisation=C->MemoTot[i]/**100.0*/;
						txt.Format("%.2f",memorisation);
						pDC->DrawText(txt,&Rc,DT_CENTER|DT_VCENTER);

					}
				}
		break;
		}
		
	}



	return 1;
}


///////////////////////////////////////////////////////////////////////////////////////
// Les tracés verticaux
//
bool CPrintFilm::DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab)
{
	CRect R;
	if (m_TableType[NrTab] < m_NbTabGrilleFilm)
	{
		// Tracé verticaux pour la grille films
		/*
		R=pR_haut;
		R.OffsetRect(0,pR_haut->Height()*2);
		TraitV(pDC,&R,pR_bas,0,30);
		TraitV(pDC,&R,pR_bas,30,40);
		TraitV(pDC,&R,pR_bas,40,50);
		TraitV(pDC,&R,pR_bas,50,100);
		*/
	}
	else if (m_TableType[NrTab] == m_NbTabGrilleFilm)
	{
		// Tracé verticaux pour tableau performances
		R=pR_haut;
		R.OffsetRect(0,pR_haut->Height()*22);
		TraitV(pDC,&R,pR_bas,20,80);
		TraitV(pDC,&R,pR_bas,60,80);
	}
	else
	{
		// aucun tracé verticaux pour mémo-démémo
	}
	
	return 1;
}


///////////////////////////////////////////////////////////////////////////////////////
// Impression Pied de page >> LOGO
//
bool CPrintFilm::DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig)
{
	// on teste la ligne a dessiner
	CString txt;
	CCampagneFilm * C = m_pCampagneFilm;
	switch (NrLig)
	{
		case 1:
			{
				txt.LoadString(ID_PIEDPAGE_PRINTFILM);
				EcritG(pDC, txt, pR, 1, 100, 0,&m_Font[FONT_TIME8]);
			}
			break;

		case 3:{
			CRect r;
			r=*pR;
			r.bottom=r.top+r.Height()*4;
			DessineBitmap( pDC, 0,&r, 0, 100 );
		}
		break;
	}

	// on quitte
	return TRUE;
}




