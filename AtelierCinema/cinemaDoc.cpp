// cinemaDoc.cpp : implementation of the CCinemaDoc class
//

#include "stdafx.h"
#include "cinema.h"

#include "cinemaDoc.h"
#include "DlgSelection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCinemaDoc

IMPLEMENT_DYNCREATE(CCinemaDoc, CDocument)

BEGIN_MESSAGE_MAP(CCinemaDoc, CDocument)
	//{{AFX_MSG_MAP(CCinemaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCinemaDoc construction/destruction

CCinemaDoc::CCinemaDoc()
{
	m_Campagne.m_pDoc=this;
}

CCinemaDoc::~CCinemaDoc()
{
}

BOOL CCinemaDoc::OnNewDocument()
{
	
	CDlgChangerPeriode dlg;
	if(dlg.DoModal()!=IDOK)return FALSE;
	CDlgFonctionCible dlg2;
	dlg2.m_Mode=0;

	if(dlg2.DoModal()!=IDOK)return FALSE;
	m_Campagne.m_NumCible[0] = dlg2.m_NumCible;
	m_Campagne.m_LibCible[0] = dlg2.m_Nom;

	// construction d'une sélection
	CDlgSelection sel;
	if(sel.DoModal()!=IDOK)return FALSE;

	CWaitCursor wait;

	// Libération de la sélection
	m_Campagne.m_Selection.RemoveAll();
    m_Campagne.m_Selection.FreeMemory();

	// On récupère les codes et on construit une nouvelle sélection
	for(int idcode=0;idcode<sel.GetNbCode();idcode++)
	{
		CString code;
		code = sel.GetCode(idcode);
		m_Campagne.m_Selection.AddCode(code); 
	}
	m_Campagne.m_Selection.InitTab();
	m_Campagne.m_Selection.BindAccelerateur();
	m_Campagne.m_Selection.BindMap();
	m_Campagne.m_Selection.ConstruitListeBacComplexeActifs();	


	m_Campagne.SetExercice(dlg.m_DateDebut,dlg.m_NbSemaine);
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCinemaDoc serialization

void CCinemaDoc::Serialize(CArchive& ar)
{

	int NrVersion;
	if (ar.IsStoring())
	{
		int NrVersion=3;

		ar << NrVersion;

		// Libellés de la boite de dialogue ParamPrint
		ar << m_Campagne.m_NomCampagne;
		ar << m_Campagne.m_NomAnnonceur;
		ar << m_Campagne.m_NomCentrale;
		ar << m_Campagne.m_NomProduit;
		
		ar << m_Campagne.m_DateDebut;
		ar << m_Campagne.m_NbSemaine;

		ar << m_Campagne.m_NbFormat;
		m_Campagne.m_LibFormat.Serialize(ar);

		ar << m_Campagne.m_NumCible[0];
		ar << m_Campagne.m_LibCible[0];
		for(int cible=0;cible<NBCIBLECALCUL;cible++)
		{
			ar << m_Campagne.m_IndexCibleImprime[cible];
		}


		ar << m_Campagne.m_Beta;

		for( int h=0;h<NBHYPOTHESE;h++)
		{
			ar << m_Campagne.m_Resultat.BudgetNet[h];
		}

		ar << m_Campagne.m_flagBinomeAgregat;
		ar << m_Campagne.CalculSaisonnalite;
		ar << m_Campagne.fPiloteDurees;
		ar << m_Campagne.CalculDememo;
		ar << m_Campagne.RedressementCNC;
		ar << m_Campagne.NumeroModaliteAgglo;
		
		// CALCUL FRED
		ar << m_Campagne.BaseCalcul;

		m_Campagne.m_Selection.Serialize(ar); 
		for(h=0;h<NBHYPOTHESE;h++)
		{
			for(int p=0;p<m_Campagne.m_Selection.GetNbParcTotal();p++)
			{
				for(int s=0;s<m_Campagne.m_NbSemaine;s++)
				{
					for(int f=0;f<NBFORMATMAX;f++)
					{
						ar << m_Campagne.m_Passage[h][p][s][f];
					}
				}
			}
		}
	}
	else // Chargement
	{
		ar >> NrVersion;

		// Libellés de la boite de dialogue ParamPrint
		ar >> m_Campagne.m_NomCampagne;
		ar >> m_Campagne.m_NomAnnonceur;
		ar >> m_Campagne.m_NomCentrale;
		ar >> m_Campagne.m_NomProduit;

		ar >> m_Campagne.m_DateDebut;
		ar >> m_Campagne.m_NbSemaine;

		ar >> m_Campagne.m_NbFormat;
		m_Campagne.m_LibFormat.Serialize(ar);

		ar >> m_Campagne.m_NumCible[0];
		ar >> m_Campagne.m_LibCible[0];
		for(int cible=0;cible<NBCIBLECALCUL;cible++)
		{
			ar >> m_Campagne.m_IndexCibleImprime[cible];
		}

		ar >> m_Campagne.m_Beta;

		for( int h=0;h<NBHYPOTHESE;h++)
		{
			ar >> m_Campagne.m_Resultat.BudgetNet[h];
		}

		ar >> m_Campagne.m_flagBinomeAgregat;
		//m_Campagne.MajFiltreVisu();
		ar >> m_Campagne.CalculSaisonnalite;
		ar >> m_Campagne.fPiloteDurees;
		ar >> m_Campagne.CalculDememo;
		if(NrVersion>=1)
			ar >> m_Campagne.RedressementCNC;
		if(NrVersion>=3)
			ar >> m_Campagne.NumeroModaliteAgglo;


		// CALCUL FRED
		if(NrVersion>=2)
			ar >> m_Campagne.BaseCalcul;

		// on récupère le contenu des sélections des lignes
		m_Campagne.m_Selection.Serialize(ar); 
		// TODO : harmoniser les lignes de la sélection si les codes n'existent plus.
		m_Campagne.SetExercice(m_Campagne.m_DateDebut,m_Campagne.m_NbSemaine);

		// On récupère que les passages valides pour les lignes de sélection qui n'ont pas été enlevé.
		for(h=0;h<NBHYPOTHESE;h++)
		{
			for(int p=0;p<m_Campagne.m_Selection.TabPassageValide.GetSize();p++)
			{
				for(int s=0;s<m_Campagne.m_NbSemaine;s++)
				{
					for(int f=0;f<NBFORMATMAX;f++)
					{
						char poubelle;
						int idxPassage=m_Campagne.m_Selection.TabPassageValide[p];
						if(idxPassage>=0)
							ar >>	m_Campagne.m_Passage[h][idxPassage][s][f] ;
						else
							ar >>	poubelle;
					}
				}
			}
		}
	
	
	
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCinemaDoc diagnostics

#ifdef _DEBUG
void CCinemaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCinemaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCinemaDoc commands



