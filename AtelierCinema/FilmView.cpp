// FilmView.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "FilmDoc.h"
#include "FilmView.h"
#include "DlgProduitFilm.h"
#include "DlgOptionCalculFilm.h"
#include "dlgbeta.h"
#include "dlgbudgetnet.h"
#include "DlgOptionCalculMemo.h"

extern CCinemaApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilmView

IMPLEMENT_DYNCREATE(CFilmView, CFormView)

CFilmView::CFilmView()
	: CFormView(CFilmView::IDD)
{
	//{{AFX_DATA_INIT(CFilmView)
	m_LibCampagneFilm = _T("");
	m_CibleBase = _T("");
	m_Periode = _T("");
	//}}AFX_DATA_INIT
	CWaitCursor wait;
	
	m_pCampagneFilm = NULL;

}

CFilmView::~CFilmView()
{
}

void CFilmView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilmView)
	DDX_Control(pDX, IDC_PERIODE, m_BtnPeriode);
	DDX_Control(pDX, IDC_CIBLEBASE, m_BtnCibleBase);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_CADRE0, m_Cadre0);
	DDX_Control(pDX, IDC_MAJFILM, m_MajFilms);
	DDX_Control(pDX, IDC_PERF, m_BtnPerf);
	DDX_Control(pDX, IDC_GRAPHE, m_BtnGraphe);
	DDX_Text(pDX, IDC_CAMPAGNEFILM, m_LibCampagneFilm);
	DDX_Text(pDX, IDC_CIBLEBASE, m_CibleBase);
	DDX_Text(pDX, IDC_PERIODE, m_Periode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilmView, CFormView)
	//{{AFX_MSG_MAP(CFilmView)
	ON_BN_CLICKED(IDC_GRAPHE, OnGraphe)
	ON_BN_CLICKED(IDC_PERF, OnPerf)
	ON_BN_CLICKED(IDC_MAJFILM, OnMajfilm)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_PRINT2, OnFilePrint2)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_CREATE()
	ON_COMMAND(ID_PARAMTRES_OPTIONDECALCUL, OnParamtresOptiondecalcul)
	ON_COMMAND(ID_PARAMTRES_BTA, OnParamtresBta)
	ON_COMMAND(ID_PARAMTRES_EXERCICE, OnParamtresExercice)
	ON_COMMAND(ID_PARAMTRES_CIBLE1, OnParamtresCible1)
	ON_COMMAND(ID_PARAMTRES_BUDGETNET, OnParamtresBudgetnet)
	ON_COMMAND(ID_PARAMTRES_OPTIONDECALCULMEMO, OnParamtresOptiondecalculmemo)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFilmView message handlers

void CFilmView::OnInitialUpdate() 
{

	CWaitCursor wait;	
	CFormView::OnInitialUpdate();

	// Récupération de la campagne film doc	
	m_pCampagneFilm = &GetDocument()->m_CampagneFilm;

	// Initialise Résultats 1
	GetDocument()->m_DlgPerfFilm.Create(IDD_PERFORMANCESFILM,this);
	GetDocument()->m_DlgPerfFilm.Initialise(m_pCampagneFilm);
	m_pCampagneFilm->m_PerfFilm = &GetDocument()->m_DlgPerfFilm;

	// Initialise Graphe Résultats Memo-dememo
	GetDocument()->m_DlgGrapheFilm.Create(IDD_GRAPHEFILM,this);
	GetDocument()->m_DlgGrapheFilm.Initialise(m_pCampagneFilm,0);
	m_pCampagneFilm->m_GrapheFilm = &GetDocument()->m_DlgGrapheFilm;

	// --------   Initialisation de la fenêtre analyse film -------
	//
	// Subclasse the window created with the User Control
	m_GridVisuFilm.SubclassDlgItem(IDC_GRID_FILM,this);
	
	// Nom de la campagne associé
	m_pCampagneFilm->m_NomVagueCampagne  = theApp.m_NomVagueAtelierCinema;

	// Initialisation des informations générales
	InitInfoGene();

	// Init Grid Visu Film
	m_GridVisuFilm.Init(m_pCampagneFilm);

	// Initialisation Grid des produits films
	InitGridFilm();

	
	// Charge des produits films sélectionnés
	if (LoadGridFilm() == false)
	{
		AfxMessageBox ("Attention, aucun film sélectionné !!!!");
		//return FALSE;
	}

	// Initialisation du tableaudes produits films
	m_pCampagneFilm->m_TabProduitFilm.InitTabProduitFilm();
	
	// Mise à jour des infos	
	UpdateData(0);


}


/////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation Infos générales
//
void CFilmView::InitInfoGene()
{
	
	// Nom de l'étude
	m_LibCampagneFilm = "Nouvelle Campagne Produit Films";
		
	// Nom de la période courante
	COleDateTime DateDeb = m_pCampagneFilm->m_DateDebut;
	COleDateTime DateFin = DateDeb + COleDateTimeSpan(m_pCampagneFilm->m_NbSemaine * 7 - 1,0,0,0); 
	m_Periode =  DateDeb.Format("%d-%m-%Y") + "/" + DateFin.Format("%d-%m-%Y");


	// Nom de la cible de base
	m_CibleBase = m_pCampagneFilm->m_LibCible[0];

}


/////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation Grid des produits films
//
void CFilmView::InitGridFilm()
{

	// Subclasse the window created with the User Control
	//m_GridVisuFilm.SubclassDlgItem(IDC_GRID_FILM,this);

	// MODIF FRED: la grille doit être "scrollable" au besoin en fonction du nb de ligne.
	m_GridVisuFilm.ModifyStyle(NULL,WS_HSCROLL|WS_VSCROLL);

	// Initialisation de la grille visu films
	m_GridVisuFilm.Initialize();
	m_GridVisuFilm.GetParam()->EnableUndo(FALSE);

	// Nombre de lignes = nombre de produits films 
	int NbLigne = m_pCampagneFilm->m_TabProduitFilm.GetSize();
	if (NbLigne == 0) NbLigne = 1;

	// + 1 ligne total vertical
	NbLigne ++;

	m_GridVisuFilm.SetRowCount(NbLigne);

	// Nombre de colonnes = Colonne Libelle + Prix + CNC + Nb Coeff Usure (nb semaines) + Colonne Total
	int NbSemaine = m_pCampagneFilm->m_NbSemaine;
	m_GridVisuFilm.SetColCount(ColCNC + NbSemaine + 1);

	// Init des entetes
	m_GridVisuFilm.SetStyleRange(CGXRange(0,ColLibFilm),CGXStyle().SetValue("Libellé Film"));		
	m_GridVisuFilm.SetStyleRange(CGXRange(0,ColTarif),CGXStyle().SetValue("Tarif"));		
	m_GridVisuFilm.SetStyleRange(CGXRange(0,ColCNC),CGXStyle().SetValue("Tickets"));
	
	// Init des entetes semaines
	COleDateTime DateCur = m_pCampagneFilm->m_DateDebut;
    for (int i = 0;i<NbSemaine;i++)
	{
		// Affiche libellé semaine (format ddmmyy)
		CString TxtSem;
		TxtSem = DateCur.Format("%d-%m-%Y");
		m_GridVisuFilm.SetStyleRange(CGXRange(0,i+ColCNC+1),CGXStyle().SetValue(TxtSem));		

		// Passe à la colonne semaine suivante
		DateCur = DateCur + COleDateTimeSpan(7,0,0,0); 
	}


	// Largeur colonne libellé Produit de base (plus nécessair)
	// m_GridVisuFilm.SetColWidth(ColLibProduit,ColLibProduit,150);
	
	// Largeur colonne sélection film
	m_GridVisuFilm.SetColWidth(ColSelectFilm,ColSelectFilm,15);

	// Largeur colonne libellé Film (modifiable)
	m_GridVisuFilm.SetColWidth(ColLibFilm,ColLibFilm,182); //  ,182);

	// Largeur colonne tarif
	m_GridVisuFilm.SetColWidth(ColTarif,ColTarif,60);

	// Largeur colonne CNC
	m_GridVisuFilm.SetColWidth(ColCNC,ColCNC,60);

	// Largeur des colonnes coeff usure/semaine
	for (i= ColCNC+1;i<m_GridVisuFilm.GetColCount();i++) 
		m_GridVisuFilm.SetColWidth(i,i,64);
	

	// Initialisation des valeurs tarifs à 0
	/*
	for (i=0; i<NbLigne;i++)
	{
		m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColTarif),CGXStyle().SetValue("0"));
	}
	*/

	int InxCol = m_GridVisuFilm.GetColCount();

	// Init Couleur des lignes
	for (int r=1;r<NbLigne;r++)
	{
		for (int c=1;c<m_GridVisuFilm.GetColCount();c++)
		{
			if (c == ColTarif)
				m_GridVisuFilm.SetStyleRange( CGXRange(r,c,r,c),
								              CGXStyle().SetInterior(CGXBrush().SetColor(RGB_Tarif)));
			else if (c== ColCNC)
				m_GridVisuFilm.SetStyleRange( CGXRange(r,c,r,c),
								              CGXStyle().SetInterior(CGXBrush().SetColor(RGB_CNC)));
			else
				m_GridVisuFilm.SetStyleRange( CGXRange(r,c,r,c),
								              CGXStyle().SetInterior(CGXBrush().SetColor(RGB(255,255,255))));
		}
	}

	/*
	//m_GridVisuFilm.SetStyleRange( CGXRange().SetRows(1,NbLigne),
	//							  CGXStyle().SetInterior(CGXBrush().SetColor(RGB(255,255,255))));

	// Couleur colonne tarif
	m_GridVisuFilm.SetStyleRange( CGXRange().SetCols(ColTarif,ColTarif),
								  CGXStyle().SetInterior(CGXBrush().SetColor(RGB_Tarif)));

	// Couleur colonne CNC
	m_GridVisuFilm.SetStyleRange( CGXRange().SetCols(ColCNC,ColCNC),
								  CGXStyle().SetInterior(CGXBrush().SetColor(RGB_CNC)));
	*/

	// Couleur colonne totaux lignes
	InxCol = ColCNC+NbSemaine+1;
	m_GridVisuFilm.SetStyleRange( CGXRange().SetCols(ColCNC+NbSemaine+1,ColCNC+NbSemaine+1),
								  CGXStyle().SetInterior(CGXBrush().SetColor(RGB_Totaux)));

	// Couleur ligne totaux colonnes
	m_GridVisuFilm.SetStyleRange( CGXRange().SetRows(NbLigne,NbLigne),
								  CGXStyle().SetInterior(CGXBrush().SetColor(RGB_Totaux)));


	// Init des paramètres
	m_GridVisuFilm.GetParam()->EnableTrackColWidth(0); 
	m_GridVisuFilm.GetParam()->EnableTrackRowHeight(0); 
	m_GridVisuFilm.GetParam()->SetEnterKeyAction(GX_DOWN);
	m_GridVisuFilm.GetParam()->SetActivateCellFlags(FALSE); 
	m_GridVisuFilm.GetParam()->SetGridLineStyle(PS_SOLID);
	m_GridVisuFilm.GetParam()->EnableSelection(GX_SELNONE );
	
	m_GridVisuFilm.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND,RGB(255,255,255));
	
	// Colonne Select centrée
	m_GridVisuFilm.SetStyleRange( CGXRange().SetCols(ColSelectFilm,ColSelectFilm),
							 CGXStyle().SetHorizontalAlignment(DT_CENTER));

	// Les résultats sont cadrés à droite ( à partir de la colonne tarif)
	m_GridVisuFilm.SetStyleRange( CGXRange().SetCols(ColTarif,ColCNC+NbSemaine+1),
							 CGXStyle().SetHorizontalAlignment(DT_RIGHT));

	// Fin initialisation
	m_GridVisuFilm.GetParam()->EnableUndo(TRUE);

}


/////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des infos grilles films
//
bool CFilmView::LoadGridFilm()
{
	
	//Invalidate(FALSE);

	// Evaluation de la répartition visiteurs par semaine
	CCalculProduitFilm::CResultatNbEntree * pResult;

	CCalculProduitFilm * calcul;
	calcul = new CCalculProduitFilm(m_pCampagneFilm);
	pResult = calcul->CalculNbEntre(); 

	CProduitFilm ProduitFilm;

	// Redéfinition du nb de colone grille
	m_GridVisuFilm.SetColCount(ColCNC + m_pCampagneFilm->m_NbSemaine + 1);
	
	// Initialisation des libellés
	for (int i = 0;i<=m_pCampagneFilm->m_TabProduitFilm.GetSize();i++)
	{

		// Libellé produit de base (non modifiable)
		//CString TxtProduit = m_pCampagneFilm->m_Selection.GetLibelle(i); 
		//m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColLibProduit),CGXStyle().SetValue(TxtProduit));

		// Etat de la sélection produit
		CString TxtProduit;
		// Libellé Film au départ idem
		if(i<m_pCampagneFilm->m_TabProduitFilm.GetSize())
		{
			// Récup Produit Film
			ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(i);
			if (ProduitFilm.m_Select == TRUE)
				m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColSelectFilm),CGXStyle().SetValue("X"));
			else
				m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColSelectFilm),CGXStyle().SetValue(""));


			TxtProduit = ProduitFilm.m_Libelle;
		}
		else
		{
			TxtProduit="Total";
			m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColSelectFilm),CGXStyle().SetValue(""));
		}
		m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColLibFilm),CGXStyle().SetValue(TxtProduit));

		// Prix de base
		CString TxtTarif;
		TxtTarif.Format("%0.0f",pResult->m_Tarif[i]);
		m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColTarif),CGXStyle().SetValue(TxtTarif));


		// CNC - Nombre d'entrée
		CString TxtCNC;
		TxtCNC.Format("%0.0f",pResult->m_NbEntreeFilm[i]); 
		int NbEnt = pResult->m_NbEntreeFilm[i];
		m_GridVisuFilm.SetStyleRange(CGXRange(i+1,ColCNC),CGXStyle().SetValue(TxtCNC));

		// Affichage des infos semaines
		//AfficheResultSemaine(i,ProduitFilm,Result);

		/*
		// Boucle sur toutes les semaines
		for (int InxSem = 0;InxSem <=m_pCampagneFilm->m_NbSemaine;InxSem++)
		{
			CString TxtResult;
			TxtResult.Format("%0.0f",Result->m_NbEntreeCible[i][InxSem]);
			m_GridVisuFilm.SetStyleRange(CGXRange(i+1,InxSem+4),CGXStyle().SetValue(TxtResult));
				
		}
		*/
		

	}		

	// Update calcul 
	UpdateResultat();

	// Dernière ligne, colonne select > vide
	//m_GridVisuFilm.SetStyleRange(CGXRange(m_pCampagneFilm->m_TabProduitFilm.GetSize()+1,ColSelectFilm),CGXStyle().SetValue(""));
	m_GridVisuFilm.SetStyleRange(CGXRange(m_pCampagneFilm->m_TabProduitFilm.GetSize()+1,ColLibFilm),CGXStyle().SetValue("Total"));

	delete calcul;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Update Affichage des résultats semaines 
//
void CFilmView:: UpdateAffichageResult(CCalculProduitFilm::CResultatNbEntree * pResult)
{

	// Update affichage grille film / Nb entrées
	for (int InxFilm = 0;InxFilm<=m_pCampagneFilm->m_TabProduitFilm.GetSize();InxFilm++)
	{
		for (int InxSem = 0;InxSem <=m_pCampagneFilm->m_NbSemaine;InxSem++)
		{
			CString TxtResult;
			TxtResult.Format("%0.0f",pResult->m_NbEntreeCible[InxFilm][InxSem]);
			m_GridVisuFilm.SetStyleRange(CGXRange(InxFilm+1,InxSem+4),CGXStyle().SetValue(TxtResult));
		}
	}

	// Les totaux lignes et colonnes
	AfficheTotalColonne(pResult);
	AfficheTotalLigne(pResult);

}



/////////////////////////////////////////////////////////////////////////////////////////////
// Affichage des résultats semaines  (PROVISOIRE !!!!!!)
//
void CFilmView::AfficheResultSemaine(int InxProduitFilm,CProduitFilm ProduitFilm,CCalculProduitFilm::CResultatNbEntree * resultat)
{
	
	// En attente Résultat
	/*double BaseCNC = ProduitFilm.m_EntreeCNC;
	double Result = BaseCNC;

	// Boucle sur toutes les semaines
	for (int InxSem = 0;InxSem < m_pCampagneFilm;InxSem++)
	{
		Result = resultat->m_NbEntreeCible[InxProduitFilm][InxSem];
		CString TxtResult;
		TxtResult.Format("%0.0f",Result);
		m_GridVisuFilm.SetStyleRange(CGXRange(InxProduitFilm+1,ColCNC + InxSem),CGXStyle().SetValue(TxtResult));
			
	}*/
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Affichage des totaux colonnes
//
void CFilmView::AfficheTotalColonne(CCalculProduitFilm::CResultatNbEntree * Result)
{

	// Entete Total Colonnes 
	m_GridVisuFilm.SetStyleRange(CGXRange(m_pCampagneFilm->m_TabProduitFilm.GetSize()+1,ColLibFilm),CGXStyle().SetValue("Total"));

	// Balayage de toutes les lignes
	double TotTarif = 0;
	double TotCNC   = 0;
	CArray <double,double&> TotSem;
	CProduitFilm ProduitFilm;

	// Dimensionne vecteur tot CNC / semaines
	TotSem.SetSize(m_pCampagneFilm->m_NbSemaine);

	for (int i = 0;i<m_pCampagneFilm->m_TabProduitFilm.GetSize();i++)
	{
		// Récup Produit Film
		ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(i);

		// Total Prix des films
		TotTarif += ProduitFilm.m_Tarif;

		// Total CNC
		// TotCNC += ProduitFilm.m_EntreeCNC * ProduitFilm.m_NbSemaine; 
		CString TxtValue = m_GridVisuFilm.GetValueRowCol(i+1,ColCNC); 		
		TotCNC += atof(TxtValue);

		// pour les valeurs semaines
		for (int InxSem = 0;InxSem < m_pCampagneFilm->m_NbSemaine;InxSem++) 
		{
			// Récup valeur
			TxtValue = m_GridVisuFilm.GetValueRowCol(i+1,ColCNC+InxSem+1); 		

			// Cumul
			TotSem[InxSem] += atof(TxtValue);
		}
	}

	// Affichage des totaux
	for (i = ColTarif ;i < m_GridVisuFilm.GetColCount();i++)
	{	
		// Total tarif
		if (i == ColTarif)
		{
			m_GridVisuFilm.SetStyleRange(CGXRange(m_pCampagneFilm->m_TabProduitFilm.GetSize()+1,i),CGXStyle().SetValue(TotTarif));
			continue;
		}				

		// Total CNC
		if (i == ColCNC)
		{
			m_GridVisuFilm.SetStyleRange(CGXRange(m_pCampagneFilm->m_TabProduitFilm.GetSize()+1,i),CGXStyle().SetValue(TotCNC));
			continue;
		}

		// Total CNC / semaines
		m_GridVisuFilm.SetStyleRange(CGXRange(m_pCampagneFilm->m_TabProduitFilm.GetSize()+1,i),CGXStyle().SetValue(TotSem[i-ColCNC-1]));

	}

	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Affichage des totaux lignes  (total CNC / semaines)
//
void CFilmView::AfficheTotalLigne(CCalculProduitFilm::CResultatNbEntree * Result)
{

	CProduitFilm ProduitFilm;
	double TotCNC;

	// Entete Total Lignes
	m_GridVisuFilm.SetStyleRange(CGXRange(0,m_GridVisuFilm.GetColCount()),CGXStyle().SetValue("Total"));

	// Balayage de toutes les lignes
	for (int i = 0;i<m_pCampagneFilm->m_TabProduitFilm.GetSize() + 1;i++)
	{

		// Ici Total des totaux
		TotCNC = 0;

		// Affichage des totaux Nb Entrées
		for (int InxCol = ColCNC+1;InxCol < m_GridVisuFilm.GetColCount();InxCol++)
		{
			CString TxtValue = m_GridVisuFilm.GetValueRowCol(i+1,InxCol); 					
			TotCNC += atof(TxtValue);
		}

		// Affichage total Nb Entrées
		m_GridVisuFilm.SetStyleRange(CGXRange(i+1,m_pCampagneFilm->m_NbSemaine+ColCNC+1),CGXStyle().SetValue(TotCNC));		
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Affichage écran des résultats
//
// Graphe des performances sélection films
void CFilmView::OnGraphe() 
{

	if(GetDocument()->m_DlgGrapheFilm.IsWindowVisible()==0)
	{
		// Calcul Performance Sélection Films
		m_pCampagneFilm->LanceCalculPerformances();

		// Affichage résultats
		GetDocument()->m_DlgGrapheFilm.MajAffichageACCOGRAP();
		GetDocument()->m_DlgGrapheFilm.ShowWindow(SW_SHOWNORMAL);
	}
	else 
	{
		// Cache le graphe
		GetDocument()->m_DlgGrapheFilm.ShowWindow(SW_HIDE);
	}
	
}

// Résultats des performances sélection films
void CFilmView::OnPerf() 
{
	
	if(GetDocument()->m_DlgPerfFilm.IsWindowVisible()==0)
	{
		// Calcul Performance Sélection Films
		m_pCampagneFilm->LanceCalculPerformances();

		// Affichage des résultats
		GetDocument()->m_DlgPerfFilm.ShowWindow(SW_SHOWNORMAL);
		//GetDocument()->m_DlgPerfFilm.MajAffichagePerf();

	}
	else 
	{
		// Cache fenêtre résultats
		GetDocument()->m_DlgPerfFilm.ShowWindow(SW_HIDE);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des films à consulter
//
void CFilmView::OnMajfilm() 
{
	
	// Modification sélection des films (lance la boite produit film en mode itératif)
	CDlgProduitFilm DlgProduitFilm;
	DlgProduitFilm.m_ModePlusieurFilm = false;
	DlgProduitFilm.m_pCampagneFilm = m_pCampagneFilm;

	// On se positionnera sur le 1er film
	DlgProduitFilm.m_NumFilm  = 1;

	// Sinon on se positionne sur le film courant
	ROWCOL nRow, nCol;
    if (m_GridVisuFilm.GetCurrentCell(nRow, nCol))
	{
		// Il faut que ce soit un film, pas la dernière ligne totale
		if (nRow < m_GridVisuFilm.GetRowCount()) 
			DlgProduitFilm.m_NumFilm = nRow;
		else
			return;
	}



	// Appel dialogue pour modifier les films
	if (DlgProduitFilm.DoModal() == IDOK) 
	{
		// Mise à jour de la vue grille films
		DlgProduitFilm.m_pCampagneFilm->m_UpdateFilm = true;
		m_pCampagneFilm->m_pDoc->UpdateAllViews(NULL,1);
	
		// Rafraichit la grille résultat
		this->RedrawWindow(); 
	
		// Campagne film modifiée
		m_pCampagneFilm->m_pDoc->SetModifiedFlag();
	}

	/*
	// Appel dialogue pour modifier les fiilms
	if (DlgProduitFilm.DoModal() == IDOK) 
	{		

		DlgProduitFilm.m_pCampagneFilm->m_UpdateFilm = false;

		// Mise à jour de la grille film et des résultats
		//
		// Initialisation des informations générales
		InitInfoGene();

		// Initialisation du tableau des produits films
		m_pCampagneFilm->m_TabProduitFilm.InitTabProduitFilm();

		// Initialisation Grid des produits films
		InitGridFilm();

		// Charge des produits films sélectionnés
		if (LoadGridFilm() == false)
		{
			AfxMessageBox ("Attention, aucun film sélectionné !!!!");
			//return FALSE;
		}

		// Mise à jour de la vue campagne films 
		UpdateResultat();

	}
	*/
	
	// Rafraichissement de la fenêtre
	this->RedrawWindow(); 

	// Mise à jour des infos	
	UpdateData(0);


	//////////////////////////////////////////////////////////////////////////////
	// A VOIR
	/*
	void CSampleView::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol, CPoint point, UINT flags)
	{
		TRACE("OnClickedButtonRowCol(%ld,%ld,...,%d)\n", nRow, nCol, flags);
		GXCELL nType = GX_CELLEDIT;                              

		const CGXSttyle& style = LookupStyleRowCol(nRow, nCol);
   
		CString s;
		s.Format("Button pressed at Row %ld, Column %ld.\n", nRow, nCol);

		if (style.GetIncludeValue( ) && style.GetValueRef( ).GetLength( ) > 0)
			 s. Format("Current Text is '%s'", (LPCTSTR) style.GetValueRef( ));

		AfxMessageBox(s);
	}
	*/
	//////////////////////////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la vue Campagne Films
//
void CFilmView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	if (m_pCampagneFilm == NULL) return;

	if (m_pCampagneFilm->m_UpdateFilm == true)
	{
		// Pas de mise à jour de tout la grille si seulement mise à jour Sélection
		if (m_pCampagneFilm->m_UpdateSelectFilm == false)
		{
			// Initialisation des informations générales
			InitInfoGene();

			// Initialisation Grid des produits films
			InitGridFilm();
		}

		// Charge des produits films sélectionnés
		if (LoadGridFilm() == false)
		{
			AfxMessageBox ("Attention, aucun film sélectionné !!!!");
			//return FALSE;
		}

		// Initialisation du tableau des produits films
		m_pCampagneFilm->m_TabProduitFilm.InitTabProduitFilm();
		
		// Mise à jour des infos	
		UpdateData(0);
	}

	m_pCampagneFilm->m_UpdateFilm       = false;
	m_pCampagneFilm->m_UpdateSelectFilm = false;
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Impression Résultats
//

void CFilmView::OnFilePrint2() 
{
	if(OpenDialogPrint())
	{
		//PrepareCalculPrint();
		OnFilePrint();
	}
}

void CFilmView::OnFilePrintPreview() 
{
	if(OpenDialogPrint())
	{
		//PrepareCalculPrint();
		CView::OnFilePrintPreview();
	}
}

void CFilmView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	
	CFormView::OnBeginPrinting(pDC, pInfo);

	// Nouvelle instance impression film
	m_PrintFilm = new CPrintFilm;

	// Récup campagne films pour mise en place impression
	CCampagneFilm * C=&GetDocument()->m_CampagneFilm;
	m_PrintFilm->SetInfo(C);

	// Commence l'impression
	m_PrintFilm->OnBeginPrinting(pDC,pInfo);

}

void CFilmView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// Fin de l'impression
	m_PrintFilm->OnEndPrinting(pDC,pInfo);
	delete m_PrintFilm;
	m_PrintFilm=NULL;

	// A VOIR ????
	UndoPrepareCalculPrint();
}

BOOL CFilmView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFilmView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// Lance l'impression
	m_PrintFilm->OnPrint(pDC,pInfo);
}

bool CFilmView::UndoPrepareCalculPrint()
{
	CCampagneFilm * C=&GetDocument()->m_CampagneFilm;
	//C->m_NumCible[0]=-1;
	return 1;
}

/*bool CFilmView::PrepareCalculPrint()
{
	CCampagneFilm * C=&GetDocument()->m_CampagneFilm;

	// On calcule sur l'hypothèse active toutes les cibles d'impression
	for(int cib=1;cib<NBCIBLECALCUL;cib++)
	{
		C->m_NumCible[cib]=C->m_IndexCibleImprime[cib];
		if(C->m_NumCible[cib]<0)continue;
		C->ChargeCible(C->m_NumCible[cib],cib);
		C->PrepareVecteurIndividus(cib);
		C->CalculCibleUtile(cib);
	}
	C->LanceCalcul(CCampagne::TOUTHYPOTHESEACTIVE);
	return 1;
}*/

// retourne 1 si OK
bool CFilmView::OpenDialogPrint()
{
	CCampagneFilm * C=&GetDocument()->m_CampagneFilm;


	// mise à jour des libellés
	/*m_DlgParamPrint.m_NomCampagne = C->m_NomCampagne;
	m_DlgParamPrint.m_NomAnnonceur = C->m_NomAnnonceur;
	m_DlgParamPrint.m_NomCentrale = C->m_NomCentrale;
	m_DlgParamPrint.m_NomProduit = C->m_NomProduit;

	m_DlgParamPrint.m_DlgMulticible.m_NumeroCible[0]=C->m_NumCible[0];

	if (m_DlgParamPrint.DoModal()!=IDOK)return 0;

	C->m_PrintGraph=m_DlgParamPrint.m_Graphe;
	C->m_PrintPerf=m_DlgParamPrint.m_Performances;
	C->m_PrintCalendrier=m_DlgParamPrint.m_Calendrier;
	C->m_NomCampagne = m_DlgParamPrint.m_NomCampagne;
	C->m_NomAnnonceur = m_DlgParamPrint.m_NomAnnonceur;
	C->m_NomCentrale = m_DlgParamPrint.m_NomCentrale;
	C->m_NomProduit = m_DlgParamPrint.m_NomProduit;

	for(cib=0;cib<NBCIBLECALCUL;cib++)
		C->m_IndexCibleImprime[cib]=m_DlgParamPrint.m_DlgMulticible.m_NumeroCible[cib];*/

	C->LanceCalculPerformances();
	
	
	return 1;
}




///////////////////////////////////////////////////////////////////////////////////////
// CFilmView diagnostics

#ifdef _DEBUG
void CFilmView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFilmView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFilmDoc* CFilmView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFilmDoc)));
	return (CFilmDoc*)m_pDocument;
}
#endif //_DEBUG






void CFilmView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Coloriage fond bleu JFC
	CBrush fond(RGB_BleuJFC);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

}


int CFilmView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Positionnement en haut à gauche
	/* marche pas ???
	CRect rect;
	GetClientRect(& rect);
	MoveWindow(0,0,rect.Height(),rect.Width(),FALSE);
	*/
	
	return 0;
}



void CFilmView::OnParamtresOptiondecalcul() 
{

	// Instance boite de dialogue option calcul
	CDlgOptionCalculFilm DlgOptionCalcul;


	if(m_pCampagneFilm->CalculDememo)DlgOptionCalcul.m_Dememorisation=0;
	else DlgOptionCalcul.m_Dememorisation=1;
	//if(m_pCampagneFilm->BaseCalcul)DlgOptionCalcul.
	if(DlgOptionCalcul.DoModal()!=IDOK) return;

	if(DlgOptionCalcul.m_Dememorisation)m_pCampagneFilm->CalculDememo=0;
	else m_pCampagneFilm->CalculDememo=1;


	UpdateResultat();
}

void CFilmView::OnParamtresBta() 
{
	CDlgBeta dlg;
	dlg.m_Beta=m_pCampagneFilm->m_Beta;
	if(dlg.DoModal()!=IDOK)return ;
	m_pCampagneFilm->m_Beta=dlg.m_Beta;

	UpdateResultat();
}

void CFilmView::OnParamtresExercice() 
{
	CDlgChangerPeriode dlg;
	dlg.m_DateDebut=m_pCampagneFilm->m_DateDebut;
	dlg.m_NbSemaine=m_pCampagneFilm->m_NbSemaine;
	if(dlg.DoModal()!=IDOK)return ;
	m_pCampagneFilm->SetExercice(dlg.m_DateDebut,dlg.m_NbSemaine);
	
	// Changement de la date et de la durée des produits 
	// récupération de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagneFilm->m_TabProduitFilm;
	bool pbduree=false;
	// Bouclage par film (calcul du nombre d'entrée par film, par semaine)
	for(int idxfilm=0;idxfilm<tabprod->GetSize() ;idxfilm++)
	{
		CProduitFilm prod = tabprod->GetAt(idxfilm);
		int idxsemaineproduit = (prod.m_Debut-m_pCampagneFilm->m_DateDebut)/7;

		// Pour les plage à l'extérieur de la campagne: message d'erreur
		if((idxsemaineproduit+prod.m_NbSemaine>m_pCampagneFilm->m_NbSemaine)||(prod.m_Debut<m_pCampagneFilm->m_DateDebut))
		{
			pbduree=true;
		}
	}
	if(pbduree)AfxMessageBox("Dans cette campagne certains films distribue des contacts à l'extérieur de la campagne");

	// Mise a jour grille
	InitGridFilm();
	LoadGridFilm();

	// Mise à jour des résultats
	UpdateResultat();


}
void CFilmView::OnParamtresCible1() 
{
	CDlgFonctionCible dlg;
	dlg.m_Mode=0;
	if(dlg.DoModal()!=IDOK)return ;
	m_pCampagneFilm->m_NumCible[0]=dlg.m_NumCible;
	m_pCampagneFilm->m_LibCible[0]=dlg.m_Nom;

	// changement du libellé de la cible dans CEdit
	m_CibleBase = dlg.m_Nom;

	UpdateData(0);

	// Chargement de la grille films
	LoadGridFilm();

	// Mise à jour des résultats
	UpdateResultat();

}
void CFilmView::OnParamtresBudgetnet() 
{
	CDlgBudgetNet dlg;
	dlg.m_BudgetNet=m_pCampagneFilm->m_BudgetNet;
	if(dlg.DoModal()!=IDOK)return;
	m_pCampagneFilm->m_BudgetNet=dlg.m_BudgetNet;

	UpdateResultat();
}
void CFilmView::OnParamtresOptiondecalculmemo() 
{
	CDlgOptionCalculMemo dlg;

	// Option de calcul (Nationale ou Régionale)
	if(!m_pCampagneFilm->BaseCalcul)
	{
		dlg.m_BaseFilmRegionale=1;
	}
	else 
	{ 
		dlg.m_BaseFilmRegionale=0;
	}
	

	if(m_pCampagneFilm->CalculDememo)dlg.m_Dememorisation=0;
	else dlg.m_Dememorisation=1;

	// Nr de semaine début
	dlg.m_DebutCalend = m_pCampagneFilm->m_DebutCalend+1;

	// Nr de semaine fin
	dlg.m_FinCalend  = m_pCampagneFilm->m_FinCalend+1;

	// Nr de semaine X
	dlg.m_NrSemaineX  = m_pCampagneFilm->m_NrSemaineX+1;

	dlg.m_NbSemaineCampagne = m_pCampagneFilm->m_NbSemaine;

	if(dlg.DoModal()!=IDOK)return;

	// Nr de semaine début
	m_pCampagneFilm->m_DebutCalend=dlg.m_DebutCalend-1;

	// Nr de semaine fin
	m_pCampagneFilm->m_FinCalend=dlg.m_FinCalend-1;

	// Nr de semaine X
	m_pCampagneFilm->m_NrSemaineX=dlg.m_NrSemaineX-1;

	if(dlg.m_Dememorisation)m_pCampagneFilm->CalculDememo=0;
	else m_pCampagneFilm->CalculDememo=1;

	if(dlg.m_BaseFilmRegionale) 
		m_pCampagneFilm->BaseCalcul=0;
	else 
		m_pCampagneFilm->BaseCalcul=2;


	// Recalcul des semaines actives
	m_pCampagneFilm->BindVecteurCalendrier();

	// Mise à jour du résultat
	UpdateResultat();

}


///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des Résultats
//
void CFilmView::UpdateResultat()
{
	bool grvisible = GetDocument()->m_DlgGrapheFilm.IsWindowVisible()?true:false;
	bool perfvisible = GetDocument()->m_DlgPerfFilm.IsWindowVisible()?true:false;

	if(grvisible||perfvisible)
	{
		m_pCampagneFilm->LanceCalculPerformances();
		if(perfvisible)
		{
			// Calcul Performance Sélection Films
			GetDocument()->m_DlgPerfFilm.MajAffichagePerf();

		}
		if(grvisible)
		{
			// Calcul Performance Sélection Films
			GetDocument()->m_DlgGrapheFilm.MajAffichageACCOGRAP();
		}
	}

	// A tous les coups Update Affichage Résultats
	CCalculProduitFilm::CResultatNbEntree * pResult;

	CCalculProduitFilm * calcul;
	calcul = new CCalculProduitFilm(m_pCampagneFilm);
	pResult = calcul->CalculNbEntre();
	UpdateAffichageResult(pResult);

	delete calcul;

	//Mise a jour de la grille
	Invalidate();
	UpdateWindow(); 

}

void CFilmView::OnClose() 
{
	// Voir si sauvegarde du dossier films courant
	

	// Fin de l'analyse films
	CFormView::OnClose();
}

BOOL CFilmView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.cx = 100;
	cs.cy = 100;
	return CFormView::PreCreateWindow(cs);
}
