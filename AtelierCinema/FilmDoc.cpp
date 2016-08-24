// FilmDoc.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "FilmDoc.h"
#include "CampagneFilm.h"
#include "DlgCritere.h"
#include "DlgProduitFilm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCinemaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFilmDoc

IMPLEMENT_DYNCREATE(CFilmDoc, CDocument)

CFilmDoc::CFilmDoc()
{
	// Récup dans campagne le document
	m_CampagneFilm.m_pDoc=this;

	// Init Update View
	m_UpdateView = false;
}

BOOL CFilmDoc::OnNewDocument()
{

	// Choix de la période pour Produit Films
	CDlgChangerPeriode DlgPeriode;
	if(DlgPeriode.DoModal()!=IDOK)return FALSE;

	// Sélection de la cible
	CDlgFonctionCible DlgCible;
	DlgCible.m_Mode=0;
	if(DlgCible.DoModal()!=IDOK)return FALSE;

	// Sélection du critère pour coeff fréq et pluri-fréq
	CDlgCritere DlgCritere;
	if(DlgCritere.DoModal()!=IDOK)return FALSE;
	m_CampagneFilm.m_TabProduitFilm.m_NrCritCoeff     = DlgCritere.m_NoCritere;
	m_CampagneFilm.m_TabProduitFilm.m_NbModaliteCoeff = DlgCritere.m_NbModalite;


	// ---------------------  Récupère des infos sélections -----------------------
	//
	// Récupération info période analyse
	m_CampagneFilm.SetExercice(DlgPeriode.m_DateDebut,DlgPeriode.m_NbSemaine);

	// Récup sélection cible courante
	m_CampagneFilm.m_NumCible[0] = DlgCible.m_NumCible;
	m_CampagneFilm.m_LibCible[0] = DlgCible.m_Nom; 
	m_CampagneFilm.m_NbSemaine = DlgPeriode.m_NbSemaine;
	m_CampagneFilm.m_DateDebut.SetDate(DlgPeriode.m_DateDebut.GetYear(),
									   DlgPeriode.m_DateDebut.GetMonth(),
									   DlgPeriode.m_DateDebut.GetDay()); 

	// Choix des films (lance la boite produit film en mode itératif)
	CDlgProduitFilm DlgProduitFilm;
	DlgProduitFilm.m_ModePlusieurFilm = true;
	DlgProduitFilm.m_pCampagneFilm = &m_CampagneFilm;

	// Appel dialogue saisie infos nouveaux films
	if(DlgProduitFilm.DoModal()!=IDOK) return FALSE;

	// Ouverture du nouveau document concernant la campagne films
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFilmDoc::~CFilmDoc()
{
}


BEGIN_MESSAGE_MAP(CFilmDoc, CDocument)
	//{{AFX_MSG_MAP(CFilmDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilmDoc diagnostics

#ifdef _DEBUG
void CFilmDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFilmDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilmDoc serialization
//
void CFilmDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ------------------- Sauvegarde de la campagne film ---------------

		// N° de la version
		int NrVersion = 3;
		ar << NrVersion;

		//////////////// A FAIRE (GESTION VAGUES)
		// Nom de la vague (en prévision)
		CString NomVague;
		NomVague = m_CampagneFilm.m_NomVagueCampagne;
		ar << NomVague;
			
		// Code de la vague (en prévision)
		CString CodeVague = "0001";
		ar << CodeVague;
		/////////////////////////////////////////
		
		// Nom de la campagne films
		ar << m_CampagneFilm.m_NomCampagne;

		// Début campagne films
		ar << m_CampagneFilm.m_DateDebut;

		// Nb semaines pour cette campagne films
		ar << m_CampagneFilm.m_NbSemaine;

		// Cible de référece
		ar << m_CampagneFilm.m_NumCible[0];
		ar << m_CampagneFilm.m_LibCible[0];
		
		// Les options de calcul
		CString OptionCalcul;
		ar << OptionCalcul;

		// Beta
		ar << m_CampagneFilm.m_Beta;

		// Budget net
		ar << m_CampagneFilm.m_BudgetNet;

		// Memo
		ar << m_CampagneFilm.CalculDememo;

		// Sauvegarde Option de calcul (Régional ou National)
		ar << m_CampagneFilm.BaseCalcul;

		// Nombre de films
		int NbFilms = m_CampagneFilm.m_TabProduitFilm.GetSize();
		ar << NbFilms;
		
		// Sauvegarde de toutes les infos pour chaque film
		CProduitFilm ProduitFilm;
		for (int InxFilm = 0; InxFilm < NbFilms; InxFilm++)
		{

			// Récup produit
			ProduitFilm = m_CampagneFilm.m_TabProduitFilm.GetAt(InxFilm); 

			// Film Sélectionné O/N
			ar << ProduitFilm.m_Select;
			
			// Titre du film
			ar << ProduitFilm.m_Libelle; 

			// Code Réseau distribution
			ar << ProduitFilm.m_CodeProduit; 

			// Tarif
			ar << ProduitFilm.m_Tarif; 

			// Début prise en compte film
			ar << ProduitFilm.m_Debut;
			
			// Nb semaines
			ar << ProduitFilm.m_NbSemaine; 

			// Nb entrées CNC
			ar << ProduitFilm.m_EntreeCNC; 

			// Tableau des coefficients d'usure par semaine
			for (int i=0; i<ProduitFilm.m_NbSemaine; i++)
			{
				ar << ProduitFilm.m_TabCoeffUsure[i]; 
			}

			// Code critère pour coeff fréq et pluri-fréq / et nom du critère
			ar << ProduitFilm.m_CodeCritCoeff; 
			ar << ProduitFilm.m_NomCritCoeff; 

			// Nb modalités pour ce critère / pour coeff
			int NbModa = ProduitFilm.m_TabCoeffFreq.GetSize();
			ar << NbModa; 

			// Tableau des coeffs fréq
			for (i=0; i < NbModa; i++) ar << ProduitFilm.m_TabCoeffFreq[i];  
			
			// Tableau des coeff pluri-fréq
			for (i=0; i < NbModa; i++) ar << ProduitFilm.m_TabCoeffPluriFreq[i]; 
		}


	}
	else
	{
		// ---------------------- Récupération de la campagne film ----------------------

		// N° de la version
		int NrVersion;
		ar >> NrVersion;

		//////////////// A FAIRE (GESTION VAGUES)
		// Nom de la vague (en prévision)
		CString NomVague;
		ar >> NomVague;

		if (NomVague != theApp.m_NomVagueAtelierCinema)
			AfxMessageBox ("Incohérence vague !!!!");
			
		// Code de la vague (en prévision)
		CString CodeVague;
		ar >> CodeVague;
		/////////////////////////////////////////

		// Nom de la campagne films
		ar >> m_CampagneFilm.m_NomCampagne;

		// Début campagne films
		ar >> m_CampagneFilm.m_DateDebut;

		// Nb semaines pour cette campagne films
		ar >> m_CampagneFilm.m_NbSemaine;
		m_CampagneFilm.SetExercice(m_CampagneFilm.m_DateDebut,m_CampagneFilm.m_NbSemaine);

		// Cible de référece
		ar >> m_CampagneFilm.m_NumCible[0];
		ar >> m_CampagneFilm.m_LibCible[0];
		
		// Les options de calcul
		CString OptionCalcul;
		ar >> OptionCalcul;

		if(NrVersion>1)
		{
			// Beta
			ar >> m_CampagneFilm.m_Beta;

			// Budget net
			ar >> m_CampagneFilm.m_BudgetNet;

			// Memo
			ar >> m_CampagneFilm.CalculDememo;

			if (NrVersion > 2)
				// Sauvegarde Option de calcul (Régional ou National)
				ar >> m_CampagneFilm.BaseCalcul;
		}
		// Nombre de films
		int NbFilms; 
		ar >> NbFilms;

		// Init Tableau des films
		m_CampagneFilm.m_TabProduitFilm.RemoveAll();
		
		// Sauvegarde de toutes les infos pour chaque film
		CProduitFilm ProduitFilm;
		for (int InxFilm = 0; InxFilm < NbFilms; InxFilm++)
		{
			// Film Sélectionné O/N
			ar >> ProduitFilm.m_Select;
			
			// Titre du film
			ar >> ProduitFilm.m_Libelle; 

			// Code Réseau distribution
			ar >> ProduitFilm.m_CodeProduit; 

			// Tarif
			ar >> ProduitFilm.m_Tarif; 

			// Début prise en compte film
			ar >> ProduitFilm.m_Debut;
			
			// Nb semaines
			ar >> ProduitFilm.m_NbSemaine; 

			// Nb entrées CNC
			ar >> ProduitFilm.m_EntreeCNC; 

			// Tableau des coefficients d'usure par semaine
			ProduitFilm.m_TabCoeffUsure.RemoveAll(); 
			for (int i=0; i<ProduitFilm.m_NbSemaine; i++)
			{
				float CoeffUsure;
				ar >> CoeffUsure;
				ProduitFilm.m_TabCoeffUsure.Add(CoeffUsure);
			}

			// Code critère pour coeff fréq et pluri-fréq / et nom du critère
			ar >> ProduitFilm.m_CodeCritCoeff; 
			ar >> ProduitFilm.m_NomCritCoeff; 

			// Nb modalités pour ce critère / pour coeff
			int NbModa;
			ar >> NbModa;
			ProduitFilm.m_TabCoeffFreq.RemoveAll(); 
			
			// Récup Tableau des coeffs fréq
			for (i=0; i < NbModa; i++)
			{
				float CoeffFreq;
				ar >> CoeffFreq;
				ProduitFilm.m_TabCoeffFreq.Add(CoeffFreq);
			}
			
			// Récup Tableau des coeff pluri-fréq
			ProduitFilm.m_TabCoeffPluriFreq.RemoveAll();
			for (i=0; i < NbModa; i++)
			{
				float CoeffPluriFreq;
				ar >> CoeffPluriFreq;
				ProduitFilm.m_TabCoeffPluriFreq.Add(CoeffPluriFreq); 
			}

			// Récup produit dans tableau
			m_CampagneFilm.m_TabProduitFilm.Add(ProduitFilm);

		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CFilmDoc commands
