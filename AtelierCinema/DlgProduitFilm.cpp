// DlgProduitFilm.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "DlgProduitFilm.h"

extern CCinemaApp theApp; 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProduitFilm dialog


CDlgProduitFilm::CDlgProduitFilm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProduitFilm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProduitFilm)
	m_DateDeb = COleDateTime::GetCurrentTime();
	m_NbEntreeCNC = 0;
	m_Titre = _T("");
	m_Tarif = 0;
	m_NbSemaine = 0;
	m_NoFilm = _T("");
	//}}AFX_DATA_INIT

	// Par défaut on veut saisir plusieurs films
	m_ModePlusieurFilm = true;

	// Par défaut pas le film en cours
	m_NumFilm		   = 0;
	m_pCampagneFilm    = NULL;

	// N° de la page saisie asocié
	m_NumPageFilmCur   = 1;
	m_NbTotPageFilm	   = 1;

	// Par défaut aucun réseau de distribution
	m_CodeReseauDistr = "";

}


void CDlgProduitFilm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProduitFilm)
	DDX_Control(pDX, IDC_EDIT_TITRE, m_EditTitre);
	DDX_Control(pDX, IDC_SUPPFILM, m_BtnSuppFilm);
	DDX_Control(pDX, IDC_COMBO_CRITERE, m_CmbCritere);
	DDX_Control(pDX, IDC_COMBO_RESEAUDISTR, m_CmbReseauDistr);
	DDX_Control(pDX, IDC_CADRE3, m_Cadre3);
	DDX_Control(pDX, IDC_CADRE2, m_Cadre2);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME, m_DateDeb);
	DDX_Text(pDX, IDC_EDIT_ENTREECNC, m_NbEntreeCNC);
	DDX_Text(pDX, IDC_EDIT_TITRE, m_Titre);
	DDX_Text(pDX, IDC_EDIT_TARIF, m_Tarif);
	DDX_Text(pDX, IDC_EDIT_NBSEM, m_NbSemaine);
	DDX_Text(pDX, IDC_NOFILM, m_NoFilm);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation Grid Coefficients Usure / par semaine
//
void CDlgProduitFilm::InitGridCoeffUsure()
{

	// Subclasse the window created with the User Control
	m_GridCoeffUsure.SubclassDlgItem(IDC_GRID_COEFFUSURE,this);
	
	// Initialisation de la grille coeff usure
	m_GridCoeffUsure.Initialize();
	m_GridCoeffUsure.GetParam()->EnableUndo(FALSE);

	// Nombre de lignes = 1 (juste pour les coeffs)
	m_GridCoeffUsure.SetRowCount(1);

	// Nombre de colonnes = Infos + Nb Coeff Usure (nb semaines)
	m_GridCoeffUsure.SetColCount(m_NbSemaine);

	// Init des entetes
	m_GridCoeffUsure.SetStyleRange(CGXRange(0,0),CGXStyle().SetValue("Coeff.Usure"));		
	m_GridCoeffUsure.SetStyleRange(CGXRange(1,0),CGXStyle().SetValue("0 - 100%"));		
	
	// Init des entetes semaines
	COleDateTime DateCur = m_DateDeb;
	for (int i = 0;i<m_NbSemaine;i++)
	{
		// Affiche libellé semaine (format ddmmyy)
		CString TxtSem;
		TxtSem = DateCur.Format("%d-%m-%Y");
		m_GridCoeffUsure.SetStyleRange(CGXRange(0,i+1),CGXStyle().SetValue(TxtSem));		

		// Passe à la colonne semaine suivante
		DateCur = DateCur + COleDateTimeSpan(7,0,0,0); 
	}

	// Largeur colonne info
	m_GridCoeffUsure.SetColWidth(0,0,100);
	
	// Largeur colonne coeff semaine
	m_GridCoeffUsure.SetColWidth(1,m_NbSemaine,75);

	// Initialisation des valeurs coeffs à 0
	for (i = 0;i<m_NbSemaine;i++)
	{
		if (i > 0)
			m_GridCoeffUsure.SetStyleRange(CGXRange(1,i+1),CGXStyle().SetValue("0")
																	 .SetControl(GX_IDS_CTRL_MASKEDIT)
																	 .SetUserAttribute(GX_IDS_UA_INPUTMASK, _T("##")));
	}

	// Couleur colonne freq
	m_GridCoeffUsure.SetStyleRange( CGXRange().SetCols(1,1),
								    CGXStyle().SetInterior(CGXBrush().SetColor(RGB_CoeffUsurePremSem)));

	m_GridCoeffUsure.SetFrozenCols(1,1,TRUE);



	// Init des paramètres
	m_GridCoeffUsure.GetParam()->EnableTrackColWidth(0); 
	m_GridCoeffUsure.GetParam()->EnableTrackRowHeight(0); 
	m_GridCoeffUsure.GetParam()->SetEnterKeyAction(GX_DOWN);
	m_GridCoeffUsure.GetParam()->SetActivateCellFlags(FALSE); 
	m_GridCoeffUsure.GetParam()->SetGridLineStyle(PS_SOLID);
	m_GridCoeffUsure.GetParam()->EnableSelection(GX_SELNONE );
	
	m_GridCoeffUsure.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND,RGB_BleuJFC);
	
	// Colonne info / libellé centré
	m_GridCoeffUsure.SetStyleRange( CGXRange().SetCols(0,0),
							 CGXStyle().SetHorizontalAlignment(DT_CENTER));

	// Les résultats sont cadrés à droite ( à partir de la 1ere semaine)
	m_GridCoeffUsure.SetStyleRange( CGXRange().SetCols(1,m_NbSemaine),
							 CGXStyle().SetHorizontalAlignment(DT_RIGHT));


	// Fin initialisation
	m_GridCoeffUsure.GetParam()->EnableUndo(TRUE);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation Grid Coefficients Modalités
//
void CDlgProduitFilm::InitGridCoeffModa()
{
	// Subclasse the window created with the User Control
	m_GridCoeffModa.SubclassDlgItem(IDC_GRID_COEFFMODA,this);
	
	// Initialisation de la grille coefficient modalités
	m_GridCoeffModa.Initialize();
	m_GridCoeffModa.GetParam()->EnableUndo(FALSE);

	// Attention on doit forcément avoir au moins 1 modalité
	if (m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff > 0)
	{

		// Nombre de lignes = nombre de lignes modalités
		m_GridCoeffModa.SetRowCount(m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff);


		// Nb colonnes = Libelle modalité + Coeff Frequentation + Coeff Pluri-fréquentation
		m_GridCoeffModa.SetColCount(2);

		// Init des entetes
		m_GridCoeffModa.SetStyleRange(CGXRange(0,0),CGXStyle().SetValue("Modalités"));		
		m_GridCoeffModa.SetStyleRange(CGXRange(0,1),CGXStyle().SetValue(" Indice Freq."));		
		m_GridCoeffModa.SetStyleRange(CGXRange(0,2),CGXStyle().SetValue(" Ind. Pluri-Freq"));		

		// Largeur colonne libellé modalité
		CRect R;
		m_GridCoeffModa.GetWindowRect(&R);
		float LargGrid = R.right - R.left - 4;
		m_GridCoeffModa.SetColWidth(0,0,LargGrid/2);
		
		// Largeur des colonnes coefficients
		/*
		m_GridCoeffModa.SetColWidth(1,1,LargGrid/5-10);
		m_GridCoeffModa.SetColWidth(2,2,LargGrid/5-10);
		*/
		m_GridCoeffModa.SetColWidth(1,1,LargGrid/4-10);
		m_GridCoeffModa.SetColWidth(2,2,LargGrid/4-10);

		// Couleur colonne freq
		m_GridCoeffModa.SetStyleRange( CGXRange().SetCols(1,1),
									  CGXStyle().SetInterior(CGXBrush().SetColor(RGB_CoeffFreq)));

		// Couleur de fond
		//m_GridCoeffModa.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND,RGB_BleuJFC);
		m_GridCoeffModa.GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND,RGB(255,255,255));

		// Libelle modalité calé à gauche
		m_GridCoeffModa.SetStyleRange( CGXRange().SetCols(0,0),
									   CGXStyle().SetHorizontalAlignment(DT_LEFT));

		// Les coeffs sont cadrés à droite ( à partir des colonnes coeffs)
		m_GridCoeffModa.SetStyleRange( CGXRange().SetCols(1,2),
									   CGXStyle().SetHorizontalAlignment(DT_RIGHT));
	}

	// Formatage entrée numérique
	for (int InxModa=1;InxModa <= m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff;InxModa++)
	{
		// Colonne coeff fréquentation
		m_GridCoeffModa.SetStyleRange(CGXRange(InxModa,1),CGXStyle().SetValue("0")
													   			    .SetControl(GX_IDS_CTRL_MASKEDIT)
																	.SetUserAttribute(GX_IDS_UA_INPUTMASK, _T("###")));
		// Colonne coeff pluri-fréquentation
		m_GridCoeffModa.SetStyleRange(CGXRange(InxModa,2),CGXStyle().SetValue("0")
													   			    .SetControl(GX_IDS_CTRL_MASKEDIT)
																	.SetUserAttribute(GX_IDS_UA_INPUTMASK, _T("###")));
	}

	// Init des paramètres
	m_GridCoeffModa.GetParam()->EnableTrackColWidth(0); 
	m_GridCoeffModa.GetParam()->EnableTrackRowHeight(0); 
	m_GridCoeffModa.GetParam()->SetEnterKeyAction(GX_DOWN);
	m_GridCoeffModa.GetParam()->SetActivateCellFlags(FALSE); 
	m_GridCoeffModa.GetParam()->SetGridLineStyle(PS_SOLID);
	m_GridCoeffModa.GetParam()->EnableSelection(GX_SELNONE );

	// Fin initialisation
	m_GridCoeffModa.GetParam()->EnableUndo(TRUE);

	// Chargement des modalités
	LoadGridCritere();

	int NbRow = m_GridCoeffModa.GetRowCount(); 
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des réseaux de distribution
bool CDlgProduitFilm::LoadReseauDistribution()
{
	// ajout de tous les libellés produits
	for(int InxProduit=0;InxProduit<theApp.m_TableCinema.GetTableProduit()->GetSize();InxProduit++)
	{
		CString libelle = theApp.m_TableCinema.GetTableProduit()->GetLibelle(InxProduit);
		m_CmbReseauDistr.AddString(libelle);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Recherche le libellé du produit
//
CString CDlgProduitFilm :: GetLibProduit(CString CodeProduit)
{
	// recherche libelle produit via code produit
	for(int InxProduit=0;InxProduit<theApp.m_TableCinema.GetTableProduit()->GetSize();InxProduit++)
	{
		CString Code = theApp.m_TableCinema.GetTableProduit()->GetCode(InxProduit);
		if (Code == CodeProduit)
		{			
			// Retourne le libellé produit correspondant
			return theApp.m_TableCinema.GetTableProduit()->GetLibelle(InxProduit);
		}
	}

	// Code produit non trouvé
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////
// Recherche le code du produit
//
CString CDlgProduitFilm::GetCodeProduit(CString LibProduit)
{
	// recherche code produit via libellé produit
	for(int InxProduit=0;InxProduit<theApp.m_TableCinema.GetTableProduit()->GetSize();InxProduit++)
	{
		CString Libelle = theApp.m_TableCinema.GetTableProduit()->GetLibelle(InxProduit);
		if (Libelle == LibProduit)
		{			
			// Retourne le code produit correspondant
			return theApp.m_TableCinema.GetTableProduit()->GetCode(InxProduit);
		}
	}

	// Libellé produit non trouvé
	return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des modalités critère courant
//
bool CDlgProduitFilm::LoadGridCritere()
{
	// initialise la grille avec nouveau nombre de modalité
	m_GridCoeffModa.SetRedraw(FALSE);
	m_GridCoeffModa.SetRowCount(m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff);
		
	// Vérifie si n° critère valide
	int NoCritActif = m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff;
	if(NoCritActif < 0) return false;

	// Affiche les modalités du critère
	for(int InxModa = 0; InxModa <theApp.m_Cible.m_NbClasse[NoCritActif]; InxModa++)
	{
		CString LibModa = theApp.m_Cible.m_LibClasse[NoCritActif][InxModa];
		m_GridCoeffModa.SetStyleRange(CGXRange(InxModa+1,0),CGXStyle().SetValue(LibModa));
	}
	
	// Redessine le grid
	m_GridCoeffModa.SetRedraw(TRUE);
	m_GridCoeffModa.InvalidateRect(NULL, FALSE);
	m_GridCoeffModa.UpdateWindow();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Recherche le code du critère
//
int CDlgProduitFilm::GetCodeCritere(CString LibCritere)
{

	for (int i = 0;i< theApp.m_Cible.m_NbCritere; i++)
	{
		CString txt;
		if (LibCritere == theApp.m_Cible.m_LibCritere[i])
			return i;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des critères disponibles
//
bool CDlgProduitFilm::ChargeCritere()
{
	// Init Liste Critère
	m_CmbCritere.Clear();

	if (theApp.m_Cible.m_NbCritere > 0)
	{
		// Charge tous les critères existants
		for (int i = 0;i< theApp.m_Cible.m_NbCritere; i++)
		{
			CString txt;
			txt.Format("%s", theApp.m_Cible.m_LibCritere[i]);
			m_CmbCritere.AddString(txt); 
		}

		// On affiche par défaut le critère présélectionné
		m_CmbCritere.SetCurSel(m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff);

		
		
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Récupération tableau coeffs usure
//
void CDlgProduitFilm::RecupTabCoeffUsure(CArray<float,float&> &TCoeffUsure)
{
	// Vire tous les éléments
	TCoeffUsure.RemoveAll(); 

	// Récup tous les coeffs par semaine
	for (int i = 0;i <m_NbSemaine;i++)
	{
		CString TxtValue = m_GridCoeffUsure.GetValueRowCol(1,i+1); 
		float ValCoeff = atof(TxtValue); 
		TCoeffUsure.Add(ValCoeff);
	}		
}	

/////////////////////////////////////////////////////////////////////////////////////////////
// Récupération tableau coeffs fréquence et pluri-fréq.
//
void CDlgProduitFilm::RecupTabCoeffModalite(CArray<float,float&> &TCoeffFreq,CArray<float,float&> &TCoeffPluriFreq)
{
	// Vire tous les coeffs
	TCoeffFreq.RemoveAll();
	TCoeffPluriFreq.RemoveAll();

	// Récup tous les coeffs des modalités critères courant
	for (int i = 0;i <m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff;i++)
	{

		// Coeff fréquentation
		CString TxtValue = m_GridCoeffModa.GetValueRowCol(i+1,ColFreq); 
		float ValCoeff = atof(TxtValue); 
		TCoeffFreq.Add(ValCoeff);

		// Coeff Pluri-fréquentation
		TxtValue = m_GridCoeffModa.GetValueRowCol(i+1,ColPluriFreq); 
		ValCoeff = atof(TxtValue); 
		TCoeffPluriFreq.Add(ValCoeff);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation générale des infos films
//
void CDlgProduitFilm::InitInfoFilm(int NoFilm)
{

	if (NoFilm == 0 || NoFilm > m_pCampagneFilm->m_TabProduitFilm.GetSize() )
	{
		// Page saisie vierge

		// Init Titre
		m_Titre = "";

		// Pas de réseau au début
		m_CmbReseauDistr.SetWindowText(""); 
		
		// Init Tarif
		m_Tarif = 0;

		// Init Date Début
		m_DateDeb = m_pCampagneFilm->m_DateDebut;

		// Init Nb Semaines
		m_NbSemaine = m_pCampagneFilm->m_NbSemaine;

		// Init Nb Entrées
		m_NbEntreeCNC = 0;

		// 1ere date sauvegardée
		ExDateDebut = m_pCampagneFilm->m_DateDebut;
	}

	else if (NoFilm <=  m_pCampagneFilm->m_TabProduitFilm.GetSize())
	{
		// Récupération info film existant
		CProduitFilm ProduitFilm;
		ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(NoFilm-1);
		
		// Init Titre
		m_Titre = ProduitFilm.m_Libelle; 
		
		// Recherche libellé code
		CString LibProduit = GetLibProduit(ProduitFilm.m_CodeProduit); 
		//m_CmbReseauDistr.SetWindowText(LibProduit); 
		m_CmbReseauDistr.SelectString(0,LibProduit); 

		// Init Tarif
		m_Tarif = ProduitFilm.m_Tarif;

		// Init Date Début
		m_DateDeb = ProduitFilm.m_Debut;
		
		// Init Nb Semaines
		m_NbSemaine = ProduitFilm.m_NbSemaine;

		// Init Nb Entrées
		m_NbEntreeCNC = ProduitFilm.m_EntreeCNC;

		// Init Critère / modalités
		int NbModa = theApp.m_Cible.m_NbClasse[ProduitFilm.m_CodeCritCoeff];
		m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff = NbModa; 
		m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff = ProduitFilm.m_CodeCritCoeff; 

		// No du film courant
		m_NumPageFilmCur = NoFilm;

		// 1ere date sauvegardée
		ExDateDebut = ProduitFilm.m_Debut;


	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde infos produit films
//
void CDlgProduitFilm::SaveProduitFilm(int NoFilm)
{

	CProduitFilm ProduitFilm;

	// Mise à jour des variables locales
	UpdateData(1);

	// Indice film incorrect
	if (NoFilm < 1) return;
	
	// Récup Titre
	ProduitFilm.m_Libelle = m_Titre; 
	
	// Récup Code Réseau
	CString LibProduit;
	m_CmbReseauDistr.GetWindowText(LibProduit);
	CString CodeProduit = GetCodeProduit(LibProduit);
	ProduitFilm.m_CodeProduit = CodeProduit;
	
	// Récup Tarif
	ProduitFilm.m_Tarif = m_Tarif;

	// Récup Date Début
	ProduitFilm.m_Debut = m_DateDeb;

	// Récup Nb Semaines
	ProduitFilm.m_NbSemaine = m_NbSemaine;

	// Récup Nb Entrées
	ProduitFilm.m_EntreeCNC = m_NbEntreeCNC;

	// Récup Tableau Coeff Usure
	//CArray <float,float&> TCoeffUsure;
	RecupTabCoeffUsure(ProduitFilm.m_TabCoeffUsure);

	// Récup Critère Coeff Fréq / Pluri-Fréq
	m_CmbCritere.GetWindowText(ProduitFilm.m_NomCritCoeff); 
	ProduitFilm.m_CodeCritCoeff = GetCodeCritere(ProduitFilm.m_NomCritCoeff);
		
	// Récup Tableau Modalités Coeff Fréq / Pluri-Fréq
	RecupTabCoeffModalite(ProduitFilm.m_TabCoeffFreq ,ProduitFilm.m_TabCoeffPluriFreq);

	if (NoFilm > m_pCampagneFilm->m_TabProduitFilm.GetSize()) 
		// Ajout nouvel élmt Produit Film
		m_pCampagneFilm->m_TabProduitFilm.AddFilm(ProduitFilm); 				
	else
		// Remplacement info film
		m_pCampagneFilm->m_TabProduitFilm.SetAt(NoFilm-1,ProduitFilm);

}

BEGIN_MESSAGE_MAP(CDlgProduitFilm, CDialog)
	//{{AFX_MSG_MAP(CDlgProduitFilm)
	ON_CBN_SELCHANGE(IDC_COMBO_RESEAUDISTR, OnSelchangeComboReseaudistr)
	ON_BN_CLICKED(IDC_PREC_FILM, OnPrecFilm)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SUIV_FILM, OnSuivFilm)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NBSEM, OnDeltaposSpinNbsem)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIME, OnCloseupDatetime)
	ON_BN_CLICKED(IDC_SUPPFILM, OnSuppfilm)
	ON_CBN_SELCHANGE(IDC_COMBO_CRITERE, OnSelchangeComboCritere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProduitFilm message handlers


////////////////////////////////////////////////////////////////////////////////////////
// Modification du réseau de distribution
//
void CDlgProduitFilm::OnSelchangeComboReseaudistr() 
{
}

////////////////////////////////////////////////////////////////////////////////////////
// Passe au film précédent de la table des films
//
void CDlgProduitFilm::OnPrecFilm() 
{
	
	// Init la fenêtre de saisie
	UpdateData(true);
	if (m_NumPageFilmCur > 1)
	{

		if (SaisieProduitOk() == true)
		{
			// sauve les infos film en cours
			SaveProduitFilm(m_NumPageFilmCur);
		}

		// Film précédent
		m_NumFilm-- ;
		m_NumPageFilmCur--;

		// réaffiche les infos films précédent
		InitPageSaisie(m_NumFilm,true);

		// Validation bouton suppression
		m_BtnSuppFilm.EnableWindow(TRUE);
	}

}

////////////////////////////////////////////////////////////////////////////////////////
// Passe au film suivant de la table des films
//
void CDlgProduitFilm::OnSuivFilm() 
{
	// Avant tout tester si tous les champs obligatoires ont été saisis
	UpdateData(true);
	if (SaisieProduitOk() == true)
	{

		// sauve les infos film en cours
		SaveProduitFilm(m_NumPageFilmCur);

		// 1 film en +
		m_NumFilm++ ;
		m_NumPageFilmCur++;

		// Réinitialise la page saisie avec les nouvelles infos film
		InitPageSaisie(m_NumFilm,true);
	}
	else
		AfxMessageBox ("Saisie incomplète !!!!!");

}

////////////////////////////////////////////////////////////////////////////////////////
// Couleur de fond
//
void CDlgProduitFilm::OnPaint() 
{
	CPaintDC dc(this); 
	
	// Choix de la couleur de fond
	CBrush fond(RGB_BleuJFC); 

	// Remplissage arrière-plan
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

	// Positionnement sur saisie titre
	m_EditTitre.SetFocus();
}

BOOL CDlgProduitFilm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Chargement des réseaux distribution
	LoadReseauDistribution();

	// Chargement des critéres pour coeff fréquentation
	ChargeCritere();

	if (m_ModePlusieurFilm == true)
	{
		// Initialisation page saisie vierge
		InitPageSaisie(0,false);

		// C'est le 1er film
		m_NumFilm = 1;
	}
	else
	{
		// Recharge les anciens films
		InitPageSaisie(m_NumFilm,false);
	}
	
	// Init bouton suppression
	// MODIF FRED
	// Le premier film visualisé doit pouvoir être supprimmé lorsque au moins un film est déjà présent.
	//m_BtnSuppFilm.EnableWindow(FALSE);

	// Mise à jour affichage
	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////////////
// Initialisation de la page saisie (NoFilm = 0   / Init page vierge)
//									( NoFilm > 0  / Init avec ex info films					
void CDlgProduitFilm::InitPageSaisie(int NoFilm,bool OnlyMaj)
{
	
	// Initialisation des infos films
	InitInfoFilm(NoFilm);

	// Positionnement sur saisie titre
	m_EditTitre.SetFocus();
	
	if (OnlyMaj == false)
	{
		// Initialisation de la grille Coeff Usure
		InitGridCoeffUsure();

		// Initialisation de la grille Coeff Modalite
		InitGridCoeffModa();
	}
	
	if (NoFilm > 0)
	{
		// Récupération des coefficients usures
		UpdateGrilleUsure(NoFilm);

		// Récupération des coeffs fréq et pluri-fréq
		UpdateGrilleModa(NoFilm);
	}

	// Maj nombre de films total
	//if ( m_NumPageFilmCur > m_NbTotPageFilm) m_NbTotPageFilm++;

	// Update bouton suppression
	if (m_NumPageFilmCur > m_pCampagneFilm->m_TabProduitFilm.GetSize())
		// Suppression impossible / nouvelle page
		m_BtnSuppFilm.EnableWindow(FALSE);
	else
		// Update bouton suppression
		m_BtnSuppFilm.EnableWindow(TRUE);


	// Mise à jour du n° de page en entête de boite de dialogue
	m_NbTotPageFilm = m_pCampagneFilm->m_TabProduitFilm.GetSize()+1; 
	m_NoFilm.Format("%d/%d",m_NumPageFilmCur,m_NbTotPageFilm);
	UpdateData(0);

}

////////////////////////////////////////////////////////////////////////////////////////
// Changement nombre de semaines validité du film
//
void CDlgProduitFilm::OnDeltaposSpinNbsem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	NM_UPDOWN*  pNMUpDown= (NM_UPDOWN*)pNMHDR;
	
	UpdateData(1);
	m_NbSemaine-=pNMUpDown->iDelta;

	// On limite au nombre de semaine maximum (nb semaine période de travail)
	if(m_NbSemaine<1)
		m_NbSemaine=1;

	if(m_NbSemaine>m_pCampagneFilm->m_NbSemaine)
		m_NbSemaine=m_pCampagneFilm->m_NbSemaine;

	UpdateData(0);

	// Mise à jour Grille Coeffs Usures
	UpdateGrilleUsure(m_NumFilm);

	*pResult = 0;

}


///////////////////////////////////////////////////////////////////////////////////////
// Changement de la date 
//
bool CDlgProduitFilm :: ChangeDateFilm()
{

	// Récupère nouvelle saisie date
	UpdateData(1);

	if(m_DateDeb.GetDayOfWeek()!=4)
	{
		MessageBox("Les films commencent toujours un mercredi.");
		return false;
	}

	// Limites périodes
	COleDateTime DebPeriode = m_pCampagneFilm->m_DateDebut;
	COleDateTime FinPeriode = m_pCampagneFilm->m_DateDebut;
	FinPeriode = DebPeriode + COleDateTimeSpan((m_pCampagneFilm->m_NbSemaine) * 7 - 1,0,0,0); 

	// Nb semaine produit en cours
	int NbSemProduit = m_NbSemaine;

	// Mise à jour Nb Semaine (attention DateDeb + NbSemaine < Fin Période)
	if ((m_DateDeb < DebPeriode) || ((m_DateDeb + COleDateTimeSpan((NbSemProduit * 7)-1,0,0,0)) > FinPeriode))
	{
		// Ici début non cohérent avec nb semaines et limite période
		MessageBox("Début période non valide");
		return false;
	}
	
	return true;

}

///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour grille usure
//
void CDlgProduitFilm::UpdateGrilleUsure(int NoFilm)
{

	// Grille Usure en attente
	m_GridCoeffUsure.SetRedraw(FALSE);

	// Mise à jour Nb colonnes Infos + Nb Coeff Usure (nb semaines)
	m_GridCoeffUsure.SetColCount(m_NbSemaine);

	// Init des entetes semaines
	COleDateTime DateCur = m_DateDeb;
	for (int i = 0;i<m_NbSemaine;i++)
	{
		// Affiche libellé semaine (format ddmmyy)
		CString TxtSem;
		TxtSem = DateCur.Format("%d-%m-%Y");
		m_GridCoeffUsure.SetStyleRange(CGXRange(0,i+1),CGXStyle().SetValue(TxtSem));		

		// Passe à la colonne semaine suivante
		DateCur = DateCur + COleDateTimeSpan(7,0,0,0); 
	}

	// Largeur colonne coeff semaine
	m_GridCoeffUsure.SetColWidth(1,m_NbSemaine,75);

	if (NoFilm  <= m_pCampagneFilm->m_TabProduitFilm.GetSize())
	{
		// Récupération des valeurs coeffs précédemment saisies
		int Tailleab = m_pCampagneFilm->m_TabProduitFilm.GetSize();

		// Récup info films
		CProduitFilm ProduitFilm;
		ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(NoFilm-1);			

		for (i = 0;i<ProduitFilm.m_TabCoeffUsure.GetSize();i++)
		{
			float CoeffUsure = 0.0;

			if (i != 0)
				// Récupére coefficient précédemment saisi
				CoeffUsure = ProduitFilm.m_TabCoeffUsure.GetAt(i);			

			// On l'affiche
			if (i < m_NbSemaine)
				m_GridCoeffUsure.SetStyleRange(CGXRange(1,i+1),CGXStyle().SetValue(CoeffUsure));
		}
	}
	else
	{
		// Initialisation des valeurs coeffs à 0 pour nouvelle page produit
		for (i = 0;i<m_NbSemaine;i++)
		{
			m_GridCoeffUsure.SetStyleRange(CGXRange(1,i+1),CGXStyle().SetValue("0"));
		}
	}

	// Les résultats sont cadrés à droite ( à partir de la 1ere semaine)
	m_GridCoeffUsure.SetStyleRange( CGXRange().SetCols(1,m_NbSemaine),
							 CGXStyle().SetHorizontalAlignment(DT_RIGHT));

	// Fin initialisation
	m_GridCoeffUsure.GetParam()->EnableUndo(TRUE);

	// Rafraichissement Grille
	m_GridCoeffUsure.SetRedraw(TRUE);
	m_GridCoeffUsure.InvalidateRect(NULL, FALSE);
	m_GridCoeffUsure.UpdateWindow();

}	

///////////////////////////////////////////////////////////////////////////////////////
// Récupération des coeffs fréq et pluri-fréq
//
void CDlgProduitFilm::UpdateGrilleModa(int NoFilm)
{
	// Grille Freq en attente
	m_GridCoeffModa.SetRedraw(FALSE);

	if (NoFilm  <= m_pCampagneFilm->m_TabProduitFilm.GetSize())
	{
		// récup produit film
		CProduitFilm ProduitFilm;
		ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(NoFilm-1);			

		// Les précédents coefficients fréquentation
		int NbTailleCoeff = ProduitFilm.m_TabCoeffFreq.GetSize();

		// Nom du critere de base pour ce produit
		m_CmbCritere.SelectString(0,ProduitFilm.m_NomCritCoeff);

		// Nombre de modalités pour de critere
		int NbModalite = theApp.m_Cible.m_NbClasse[m_CmbCritere.GetCurSel()];

		// Nombre de lignes = nombre de lignes modalités
		//m_GridCoeffModa.SetRowCount(ProduitFilm.m_TabCoeffFreq.GetSize());
		m_GridCoeffModa.SetRowCount(NbModalite);
		
		//int NbModa = ProduitFilm.m_TabCoeffFreq.GetSize();
		int NoCritActif = m_CmbCritere.GetCurSel();

		// Affiche les modalités du critère
		for(int InxModa = 0; InxModa <NbModalite; InxModa++)
		{
			CString LibModa = theApp.m_Cible.m_LibClasse[NoCritActif][InxModa];
			m_GridCoeffModa.SetStyleRange(CGXRange(InxModa+1,0),CGXStyle().SetValue(LibModa));
		}


		//for (int i = 0;i<ProduitFilm.m_TabCoeffFreq.GetSize();i++)
		for (int i = 0;i<NbModalite;i++)
		{			
			// Coefficient fréquentation
			float CoeffFreq = 0;
			if (i < ProduitFilm.m_TabCoeffFreq.GetSize())
				CoeffFreq = ProduitFilm.m_TabCoeffFreq.GetAt(i);

			m_GridCoeffModa.SetStyleRange(CGXRange(i+1,ColFreq),CGXStyle().SetValue(CoeffFreq));
		}

		// Les précédents coefficients pluri-fréquentation
		//NbModa = ProduitFilm.m_TabCoeffPluriFreq.GetSize();
		//for (i = 0;i<ProduitFilm.m_TabCoeffPluriFreq.GetSize();i++)
		for (i = 0;i<NbModalite;i++)
		{			
			// Coefficient pluri-fréquentation
			float CoeffPluriFreq = 0;
			if (i < ProduitFilm.m_TabCoeffPluriFreq.GetSize())
				CoeffPluriFreq = ProduitFilm.m_TabCoeffPluriFreq.GetAt(i);

			m_GridCoeffModa.SetStyleRange(CGXRange(i+1,ColPluriFreq),CGXStyle().SetValue(CoeffPluriFreq));
		}
	}
	else
	{
		// Initialisation des valeurs coeffs à 0 pour nouvelle page produit
		for (int i= 0;i < m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff;i++)
		{
			// Init Coeff Freq
			m_GridCoeffModa.SetStyleRange(CGXRange(i+1,ColFreq),CGXStyle().SetValue("0"));

			// Init Coeff Pluri-Freq
			m_GridCoeffModa.SetStyleRange(CGXRange(i+1,ColPluriFreq),CGXStyle().SetValue("0"));

		}
	}

	// Formatage entrée numérique
	for (int InxModa=1;InxModa <= m_GridCoeffModa.GetRowCount() ;InxModa++)
	{
		// Colonne coeff fréquentation
		m_GridCoeffModa.SetStyleRange(CGXRange(InxModa,1),CGXStyle().SetControl(GX_IDS_CTRL_MASKEDIT)
																	.SetUserAttribute(GX_IDS_UA_INPUTMASK, _T("###")));
		// Colonne coeff pluri-fréquentation
		m_GridCoeffModa.SetStyleRange(CGXRange(InxModa,2),CGXStyle().SetControl(GX_IDS_CTRL_MASKEDIT)
																	.SetUserAttribute(GX_IDS_UA_INPUTMASK, _T("###")));
	}

	// Rafraichissement Grille Coeff Modalités
	m_GridCoeffModa.SetRedraw(TRUE);
	m_GridCoeffModa.InvalidateRect(NULL, FALSE);
	m_GridCoeffModa.UpdateWindow();		

	int NbRows = m_GridCoeffModa.GetRowCount(); 

}


///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la date 
//
void CDlgProduitFilm::OnCloseupDatetime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	// Changement de date début analayse film
	if (ChangeDateFilm() == true )
	{
		// Mise à jour Grille Coeffs Usures
		UpdateData(0);
		UpdateGrilleUsure(m_NumFilm);
		ExDateDebut = m_DateDeb;
	}
	else
	{
		// Sauve et maj nouvelle date début film;
		// Récupère ancienne date
		m_DateDeb = ExDateDebut;
		UpdateData(0);
	}

}

///////////////////////////////////////////////////////////////////////////////////////
// Suppression du film en cours
//
void CDlgProduitFilm::OnSuppfilm() 
{
	
	// Suppression film du tableau de films
	CProduitFilm ProduitFilm;
	ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(m_NumFilm-1);
	m_pCampagneFilm->m_TabProduitFilm.SuppFilm(ProduitFilm.m_Libelle ); 
	
	// Maj no du film courant
	m_NumPageFilmCur--;
	if (m_NumPageFilmCur == 0) m_NumPageFilmCur = 1;
	m_NumFilm = m_NumPageFilmCur;

	// Initialisation de la fenêtre de dialogue
	InitPageSaisie(m_NumFilm,true);
	
	// Etat bouton suppression
	if (m_NumFilm > m_pCampagneFilm->m_TabProduitFilm.GetSize())
		m_BtnSuppFilm.EnableWindow(FALSE);
	else
		m_BtnSuppFilm.EnableWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////
// Test si tous les champs obligatoires saisis
//
bool CDlgProduitFilm::SaisieProduitOk()
{

	/*
	// Champs Titre film obligatoire et unique
	if (m_Titre == "") return false; 

	// Vérifie unicité du titre film
	if (TitreFilmExistant(m_Titre) == true)
	{
		AfxMessageBox("Titre film déjà existant",MB_ICONEXCLAMATION); 
		return false;
	}

	// Champs réseau distribution obligatoire
	int CurSel = m_CmbReseauDistr.GetCurSel();
	if (m_CmbReseauDistr.GetCurSel() < 0) return false;

	// Champs CNC obligatoire
	if (m_NbEntreeCNC <= 0) return false;

	return true;
	*/

	// AJOUT ALAIN 2003
	// Init erreur possible
	CString TxtErreur = "";
	CString Txt;

	// Champs Titre film obligatoire et unique
	if (m_Titre == "") TxtErreur = TxtErreur + "\n- Manque saisie libellé produit";

	// Vérifie unicité du titre film
	if (TitreFilmExistant(m_Titre) == true)	TxtErreur = TxtErreur + "\n- Libellé produit déjà existant";

	// Champs réseau distribution obligatoire
	CString TxtReseau;
	m_CmbReseauDistr.GetWindowText(TxtReseau);
	if (m_CmbReseauDistr.SelectString(0,TxtReseau) == CB_ERR) TxtErreur = TxtErreur + "\n- Manque choix réseau distribution";

	// Champs CNC obligatoire
	if (m_NbEntreeCNC <= 0) TxtErreur = TxtErreur  + "\n- Manque saisie nb d'entrées";

	if (TxtErreur != "")
	{
		AfxMessageBox(TxtErreur,MB_ICONEXCLAMATION); 
		return false;
	}
	else
		return true;



}

///////////////////////////////////////////////////////////////////////////////////////
// Test si titre film existant
//
bool CDlgProduitFilm::TitreFilmExistant(CString TitreFilm)
{
	CProduitFilm ProduitFilm;

	// Passe en caractère majuscule
	TitreFilm.MakeUpper();
	
	// Balaye tous les films de la campagne
	for (int i=0; i<m_pCampagneFilm->m_TabProduitFilm.GetSize();i++)
	{
		if (i != (m_NumPageFilmCur-1))
		{
			// Récup le produit film
			ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(i);

			// Si même libellé on arrête
			if (ProduitFilm.m_Libelle == TitreFilm)
				return true;
		}
	}

	// Nouveau titre film
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// On retourne dans l'affichage résultat films
//
void CDlgProduitFilm::OnOK() 
{

	UpdateData(true);

	// Sauve dernière page visu
	if (SaisieProduitOk() == true)
	{
		SaveProduitFilm(m_NumPageFilmCur);
		CDialog::OnOK();
	}

	// Sinon on continue la saisie
	else
		AfxMessageBox("Saisie incorrecte !!!!");
		return;

	
	
}

///////////////////////////////////////////////////////////////////////////////////////
// Change le critere pour coefficient fréquenttion / pluri-fréquentation
//
void CDlgProduitFilm::OnSelchangeComboCritere() 
{
	// Récup le nouvel indice critère
	if (m_CmbCritere.GetCurSel() !=  m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff)
	{
		m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff = m_CmbCritere.GetCurSel(); 
		m_pCampagneFilm->m_TabProduitFilm.m_NbModaliteCoeff = theApp.m_Cible.m_NbClasse[m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff];
			
		// Réaffiche les nouvelles modalités
		LoadGridCritere();

		// Mise à jour nouveau critere associé et nouvelle modalité
		if (m_NumFilm <= m_pCampagneFilm->m_TabProduitFilm.GetSize())
		{
			CProduitFilm ProduitFilm;
			ProduitFilm = m_pCampagneFilm->m_TabProduitFilm.GetAt(m_NumFilm-1);	
			ProduitFilm.m_CodeCritCoeff = m_pCampagneFilm->m_TabProduitFilm.m_NrCritCoeff; 
			m_CmbCritere.GetLBText(ProduitFilm.m_CodeCritCoeff,ProduitFilm.m_NomCritCoeff);  
			m_pCampagneFilm->m_TabProduitFilm.SetAt(m_NumFilm-1,ProduitFilm);
		}

		// Recharge la nouvelle grille modalité
		UpdateGrilleModa(m_NumFilm);
	}	

	
}
