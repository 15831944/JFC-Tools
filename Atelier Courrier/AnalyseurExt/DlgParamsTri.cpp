// DlgParams.cpp : implementation file
//

#include "stdafx.h"
#include "DlgParamsTri.h"
#include "AnalyseurDoc.h"
#include "AnalyseurEnums.h"
#include ".\dlgparamstri.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Boîte de dialogue CDlgParamsTri

IMPLEMENT_DYNAMIC(CDlgParamsTri, CDialog)

///////////////////////////////////////////////////////////////////////////////
// Constructeur
CDlgParamsTri::CDlgParamsTri(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamsTri::IDD, pParent)
{
	m_pDoc = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgParamsTri::~CDlgParamsTri()
{
}

///////////////////////////////////////////////////////////////////////////////
// Récupèration paramétres en cours pour tri analyseur
void CDlgParamsTri::SetParams(CAnalyseurDoc *pDoc, CPoint Pt)
{	
	// Récupèration du document général analyseur
	m_pDoc			= pDoc;

	// Position de la fenêtre
	m_PosFen = Pt;
		
}

///////////////////////////////////////////////////////////////////////////////
// Récupération des élmts tri en cours
void CDlgParamsTri::SetParamsTri()
{
	if (m_pDoc)
	{
		// Les sélections en cours
		m_TypeTri		= m_pDoc->Get_TypeTri();
		m_ModeTri		= m_pDoc->Get_ModeTri(); 

		//////////////////////////////////////////// Les grandeurs (Effectif, Nb cas etc ...) dispos pour le tri
		// Les sélections dispos grandeur(et chacune des sélection courante)
		m_Lst_Grandeurs.ResetContent(); 
		for (m_pDoc->m_MapGrandeursDispos.MoveFirst(); m_pDoc->m_MapGrandeursDispos.IsValid(); m_pDoc->m_MapGrandeursDispos.MoveNext())
		{
			// Récupération de l'item data (attention x 1000 pour avoir même ordre que dans analyseur)
			JUnt32 ItemData = m_pDoc->m_MapGrandeursDispos.GetKey() % 10000;

			// Récupèration du libellé grandeur
			CString LibGrandeur = m_pDoc->m_MapGrandeursDispos.GetItem();

			// Ajoute à la liste paramétres tris dispo
			JInt32 InxLst = m_Lst_Grandeurs.AddString(LibGrandeur); 

			// Info ident result
			m_Lst_Grandeurs.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne la grandeur tri sélection en cours
		if (m_Lst_Grandeurs.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Lst_Grandeurs.GetCount(); InxLst++)	
			{
				// Récup item data de l'élmt
				JInt32 ItemData = m_Lst_Grandeurs.GetItemData(InxLst);

				// Si c'est celui courant, on le sélectionne
				if (ItemData == m_pDoc->Get_GrandeurTri())
				{
					m_Lst_Grandeurs.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune sélection, on se positionne sur le 1er elmt de la liste des grandeurs dispos
			if (m_Lst_Grandeurs.GetCurSel() < 0)
			{
				m_Lst_Grandeurs.SetCurSel(0);
				JInt32 ItemData = m_Lst_Grandeurs.GetItemData(0);
				
				RESULTTYPE ResultType = (RESULTTYPE) ItemData;
				m_pDoc->Set_GrandeurTri(ResultType); 
			}
		}

		//////////////////////////////////////////// Les modes affichage résultats= (Grandeur, H%, V%, et autres) dispos pour le tri
		// Les sélections dispos résultats (et chacune des sélection courante)
		m_Lst_Results.ResetContent();
		for (m_pDoc->m_MapResultCellDispo.MoveFirst(); m_pDoc->m_MapResultCellDispo.IsValid(); m_pDoc->m_MapResultCellDispo.MoveNext())
		{
			// Récupération de l'item data 
			JUnt32 ItemData = m_pDoc->m_MapResultCellDispo.GetKey();

			// Récupèration du libellé résultat
			CString LibResult = m_pDoc->m_MapResultCellDispo.GetItem();

			// Attention si c'est la grandeur global
			if (LibResult == "Global")
			{
				// Mode calcul global standard
				if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
				{
					// LibResult = "IA";  Indice affinité
					LibResult.LoadString(IDS_AN_ABR_CONCOMITENCE);
				}
				else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
				{
					// LibResult = "IP";
					LibResult.LoadString(IDS_AN_ABR_COEFFYULE); 
				}
				else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
				{
					// LibResult = "ZSCORE";
					LibResult.LoadString(IDS_AN_ABR_ZSCORE);
				}
				else
				{
					// LibResult = "G%";
					LibResult.LoadString(IDS_AN_ABR_GLOBAL);
				}
			}

			// Ajoute à la liste paramétres tris dispo
			JInt32 InxLst = m_Lst_Results.AddString(LibResult); 

			// Info ident result
			m_Lst_Results.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne le resultat tri sélection en cours
		if (m_Lst_Results.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Lst_Results.GetCount(); InxLst++)	
			{
				// Récup item data de l'élmt
				JInt32 ItemData = m_Lst_Results.GetItemData(InxLst);

				// Si c'est celui courant, on le sélectionne
				if (ItemData == m_pDoc->Get_ResultTri())
				{
					m_Lst_Results.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune sélection, on se positionne sur le 1er elmt de la liste des résultats dispos
			if (m_Lst_Results.GetCurSel() < 0)
			{
				// On sélectionne le 1er elmt
				m_Lst_Results.SetCurSel(0);
				JInt32 ItemData = m_Lst_Results.GetItemData(0);
				
				// On met à jour le doc
				RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData; 
				m_pDoc->Set_ResultTri(ResultStyle);
			}
		}
	}
}

void CDlgParamsTri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CDlgParamsTri, CDialog)
	ON_BN_CLICKED(IDC_AN_RAD_CROISSANT,		OnBnClickedAnRadCroissant)
	ON_BN_CLICKED(IDC_AN_RAD_DECROISSANT,	OnBnClickedAnRadDecroissant)
	ON_BN_CLICKED(IDC_AN_RAD_AUCUNTRI,		OnBnClickedAnRadAucuntri)
	ON_BN_CLICKED(IDC_AN_RAD_GLOBAL,		OnBnClickedAnRadGlobal)
	ON_BN_CLICKED(IDC_AN_RAD_PARBLOC,		OnBnClickedAnRadParbloc)
	ON_BN_CLICKED(IDOK,						OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,					OnBnClickedCancel)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgParamsTri

BOOL CDlgParamsTri::OnInitDialog()
{
	CDialog::OnInitDialog();

	// On définit les caractéristqies des différents controles
	SetupControls();

	// Affiche les elmts liste grandeurs et résultats dispo
	SetParamsTri();

	// On positionne l'état des contrôles
	SetControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

void CDlgParamsTri::SetupControls()
{
	// Les boutons de sortie
	m_Btn_OK			.SubclassDlgItem(IDOK,							this);
	m_Btn_CANCEL		.SubclassDlgItem(IDCANCEL,						this);
	
	// Boite de sélection ordre tri
	m_Box_OrdreTri		.SubclassDlgItem(IDC_AN_STATIC_ORDRE_TRI,		this);
	m_Ttl_OrdreTri		.SubclassDlgItem(IDC_AN_STATIC_TTL_ORDRETRI,	this);
	m_Rad_TriCroissant	.SubclassDlgItem(IDC_AN_RAD_CROISSANT,			this);
	m_Rad_TriDecroissant.SubclassDlgItem(IDC_AN_RAD_DECROISSANT,		this);
	m_Rad_AucunTri		.SubclassDlgItem(IDC_AN_RAD_AUCUNTRI,			this);

	// Boite de sélection grandeur tri
	m_Box_GrandeursTri	.SubclassDlgItem(IDC_AN_STATIC_GRANDEUR_TRI,	this);
	m_Ttl_GrandeurTri	.SubclassDlgItem(IDC_AN_STATIC_TTL_GRANDEURTRI,	this);
	m_Lst_Grandeurs		.SubclassDlgItem(IDC_AN_LIST_TRI_GRANDEURS,		this);
	
	// Boite de sélection résultat cellule tri
	m_Box_ResultsTri	.SubclassDlgItem(IDC_AN_STATIC_RESULT_TRI,		this);
	m_Ttl_ResultTri		.SubclassDlgItem(IDC_AN_STATIC_TTL_RESULTTRI,	this);
	m_Lst_Results		.SubclassDlgItem(IDC_AN_LIST_TRI_RESULTATS,		this);

	// Boite de sélection nature tri
	m_Box_NatureTri		.SubclassDlgItem(IDC_AN_STATIC_NATURE_TRI,		this);
	m_Ttl_NatureTri		.SubclassDlgItem(IDC_AN_STATIC_TTL_NATURETRI,	this);
	m_Rad_Global		.SubclassDlgItem(IDC_AN_RAD_GLOBAL,				this);
	m_Rad_ParBloc		.SubclassDlgItem(IDC_AN_RAD_PARBLOC,			this);
	
	// Définition boite sélection ordre tri
	m_Box_OrdreTri		.Reset();
	m_Box_OrdreTri		.SetTransparent(false);
	m_Box_OrdreTri		.SetSunken(true);
	m_Box_OrdreTri		.AddTopWindow(m_Ttl_OrdreTri);
	m_Box_OrdreTri		.AddTopWindow(m_Rad_TriCroissant);
	m_Box_OrdreTri		.AddTopWindow(m_Rad_TriDecroissant);
	m_Box_OrdreTri		.AddTopWindow(m_Rad_AucunTri);

	// Style des radio-boutons
	m_Rad_TriCroissant				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Rad_TriCroissant				.SetTextAlignement(DT_LEFT);
	m_Rad_TriCroissant				.DrawBorder(false); 
	
	m_Rad_TriDecroissant			.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1); 
	m_Rad_TriDecroissant			.SetTextAlignement(DT_LEFT);
	m_Rad_TriDecroissant			.DrawBorder(false); 
	
	m_Rad_AucunTri					.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1); 
	m_Rad_AucunTri					.SetTextAlignement(DT_LEFT);
	m_Rad_AucunTri					.DrawBorder(false); 
	
	// Définition boite sélection résultat cellule tri
	m_Box_ResultsTri.Reset();
	m_Box_ResultsTri.SetTransparent(false);
	m_Box_ResultsTri.SetSunken(true);
	m_Box_ResultsTri.AddTopWindow(m_Ttl_ResultTri);
	m_Box_ResultsTri.AddTopWindow(m_Lst_Results);

	// Définition boite sélection résultat cellule tri
	m_Box_GrandeursTri.Reset();
	m_Box_GrandeursTri.SetTransparent(false);
	m_Box_GrandeursTri.SetSunken(true);
	m_Box_GrandeursTri.AddTopWindow(m_Ttl_GrandeurTri);
	m_Box_GrandeursTri.AddTopWindow(m_Lst_Grandeurs);

	// Définition boite sélection nature tri
	m_Box_NatureTri.Reset();
	m_Box_NatureTri.SetTransparent(false);
	m_Box_NatureTri.SetSunken(true);
	m_Box_NatureTri.AddTopWindow(m_Ttl_NatureTri);
	m_Box_NatureTri.AddTopWindow(m_Rad_Global);
	m_Box_NatureTri.AddTopWindow(m_Rad_ParBloc);

	// Style des radio-boutons
	m_Rad_Global				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Rad_Global				.SetTextAlignement(DT_LEFT);
	m_Rad_Global				.DrawBorder(false); 

	m_Rad_ParBloc				.SetIcon(IDI_AN_RADIOON1,IDI_AN_RADIOOFF1);  
	m_Rad_ParBloc				.SetTextAlignement(DT_LEFT);
	m_Rad_ParBloc				.DrawBorder(false); 

	// Définition des couleurs
	SetColors();

	// Positionnement de la fenêtre
	CRect Rect;
	GetWindowRect(Rect);
	SetWindowPos(NULL, m_PosFen.x, m_PosFen.y, Rect.Width(), Rect.Height(), NULL);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des couleurs controles
void CDlgParamsTri::SetColors()
{
	// Couleur fond des boites de sélection
	m_Box_OrdreTri.							SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::MEDIUM_DEGRAD);
	m_Box_GrandeursTri.						SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::MEDIUM_DEGRAD);
	m_Box_ResultsTri.						SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::MEDIUM_DEGRAD);
	m_Box_NatureTri.						SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::MEDIUM_DEGRAD);

	// Couleur fond et de texte des titres
	m_Ttl_OrdreTri.							SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_Ttl_OrdreTri.							SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	m_Ttl_GrandeurTri.						SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_Ttl_GrandeurTri.						SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	m_Ttl_ResultTri.						SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_Ttl_ResultTri.						SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	m_Ttl_NatureTri.						SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_Ttl_NatureTri.						SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	
	// Couleur des boutons radio
	m_Rad_TriCroissant.						SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Rad_TriCroissant.						SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_TriCroissant.						SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_TriDecroissant.					SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Rad_TriDecroissant.					SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_TriDecroissant.					SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_AucunTri.							SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Rad_AucunTri.							SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_AucunTri.							SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		

	m_Rad_Global.							SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Rad_Global.							SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_Global.							SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_ParBloc.							SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);		
	m_Rad_ParBloc.							SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	m_Rad_ParBloc.							SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);		
	
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Positionnement état des controles
void CDlgParamsTri::SetControls()
{
	// Check sélection ordre de tri en cours
	if(m_TypeTri == m_pDoc->TYPE_CROISSANT)
		CheckRadioButton(IDC_AN_RAD_CROISSANT ,IDC_AN_RAD_AUCUNTRI ,IDC_AN_RAD_CROISSANT);

	else if (m_TypeTri == m_pDoc->TYPE_DECROISSANT)
		CheckRadioButton(IDC_AN_RAD_CROISSANT ,IDC_AN_RAD_AUCUNTRI ,IDC_AN_RAD_DECROISSANT);

	else if (m_TypeTri == m_pDoc->TYPE_BASE)
		CheckRadioButton(IDC_AN_RAD_CROISSANT ,IDC_AN_RAD_AUCUNTRI ,IDC_AN_RAD_AUCUNTRI);

	// Check sélection nature du tri
	if(m_ModeTri == m_pDoc->MODE_LIGNE) 
		CheckRadioButton(IDC_AN_RAD_GLOBAL ,IDC_AN_RAD_PARBLOC ,IDC_AN_RAD_GLOBAL);
	else if (m_ModeTri == m_pDoc->MODE_BLOC)
		CheckRadioButton(IDC_AN_RAD_GLOBAL ,IDC_AN_RAD_PARBLOC ,IDC_AN_RAD_PARBLOC);

}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection ordre de tri >> passage en ordre croissant
void CDlgParamsTri::OnBnClickedAnRadCroissant()
{
	m_TypeTri = m_pDoc->TYPE_CROISSANT; 
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection ordre de tri >> passage en ordre déroissant
void CDlgParamsTri::OnBnClickedAnRadDecroissant()
{
	m_TypeTri = m_pDoc->TYPE_DECROISSANT; 
	SetControls();	
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection ordre de tri >> passage retour ordre d'origine
void CDlgParamsTri::OnBnClickedAnRadAucuntri()
{
	m_TypeTri = m_pDoc->TYPE_BASE; 
	SetControls();	
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection nature du tri  >> sur toutes les lignes du tableur
void CDlgParamsTri::OnBnClickedAnRadGlobal()
{
	m_ModeTri = m_pDoc->MODE_LIGNE; 
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection nature du tri >> par blocs de critère
void CDlgParamsTri::OnBnClickedAnRadParbloc()
{
	m_ModeTri = m_pDoc->MODE_BLOC; 
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// On valide les nouvelles sélections tris
void CDlgParamsTri::OnBnClickedOk()
{
	// Valide les sélections dans le documents	
	m_pDoc->Set_TypeTri(m_TypeTri);
	m_pDoc->Set_ModeTri(m_ModeTri);

	// Ident elmt 
	JUnt32 ItemData;

	// Valide la sélection grandeur
	if (m_Lst_Grandeurs.GetCurSel() >= 0)
	{
		// Récup ident elmt grandeur sélectionné
		ItemData = m_Lst_Grandeurs.GetItemData(m_Lst_Grandeurs.GetCurSel());	

		JUnt32 Item0 = m_Lst_Grandeurs.GetItemData(0);
		JUnt32 Item1 = m_Lst_Grandeurs.GetItemData(1);
		JUnt32 Item2 = m_Lst_Grandeurs.GetItemData(2);
	}	
	else
	{
		// On sélectionne le 1er elmt
		m_Lst_Grandeurs.SetCurSel(0);
		ItemData = m_Lst_Grandeurs.GetItemData(0);
	}

	// On met à jour le doc
	RESULTTYPE ResultType = (RESULTTYPE) ItemData;
	m_pDoc->Set_GrandeurTri(ResultType);


	// Valide la sélection résultat
	if (m_Lst_Results.GetCurSel() >= 0)
	{
		// Récup ident elmt résultat sélectionné
		ItemData = m_Lst_Results.GetItemData(m_Lst_Results.GetCurSel());	
	}	
	else
	{
		// On sélectionne le 1er elmt
		m_Lst_Results.SetCurSel(0);
		ItemData = m_Lst_Results.GetItemData(0);
	}
	
	// On met à jour le doc
	RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData; 
	m_pDoc->Set_ResultTri(ResultStyle);

	OnOK();
}
// On ne tient pas compte des nouvelles sélections
void CDlgParamsTri::OnBnClickedCancel()
{
	OnCancel();
}
