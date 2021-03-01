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


// Bo�te de dialogue CDlgParamsTri

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
// R�cup�ration param�tres en cours pour tri analyseur
void CDlgParamsTri::SetParams(CAnalyseurDoc *pDoc, CPoint Pt)
{	
	// R�cup�ration du document g�n�ral analyseur
	m_pDoc			= pDoc;

	// Position de la fen�tre
	m_PosFen = Pt;
		
}

///////////////////////////////////////////////////////////////////////////////
// R�cup�ration des �lmts tri en cours
void CDlgParamsTri::SetParamsTri()
{
	if (m_pDoc)
	{
		// Les s�lections en cours
		m_TypeTri		= m_pDoc->Get_TypeTri();
		m_ModeTri		= m_pDoc->Get_ModeTri(); 

		//////////////////////////////////////////// Les grandeurs (Effectif, Nb cas etc ...) dispos pour le tri
		// Les s�lections dispos grandeur(et chacune des s�lection courante)
		m_Lst_Grandeurs.ResetContent(); 
		for (m_pDoc->m_MapGrandeursDispos.MoveFirst(); m_pDoc->m_MapGrandeursDispos.IsValid(); m_pDoc->m_MapGrandeursDispos.MoveNext())
		{
			// R�cup�ration de l'item data (attention x 1000 pour avoir m�me ordre que dans analyseur)
			JUnt32 ItemData = m_pDoc->m_MapGrandeursDispos.GetKey() % 10000;

			// R�cup�ration du libell� grandeur
			CString LibGrandeur = m_pDoc->m_MapGrandeursDispos.GetItem();

			// Ajoute � la liste param�tres tris dispo
			JInt32 InxLst = m_Lst_Grandeurs.AddString(LibGrandeur); 

			// Info ident result
			m_Lst_Grandeurs.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne la grandeur tri s�lection en cours
		if (m_Lst_Grandeurs.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Lst_Grandeurs.GetCount(); InxLst++)	
			{
				// R�cup item data de l'�lmt
				JInt32 ItemData = m_Lst_Grandeurs.GetItemData(InxLst);

				// Si c'est celui courant, on le s�lectionne
				if (ItemData == m_pDoc->Get_GrandeurTri())
				{
					m_Lst_Grandeurs.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune s�lection, on se positionne sur le 1er elmt de la liste des grandeurs dispos
			if (m_Lst_Grandeurs.GetCurSel() < 0)
			{
				m_Lst_Grandeurs.SetCurSel(0);
				JInt32 ItemData = m_Lst_Grandeurs.GetItemData(0);
				
				RESULTTYPE ResultType = (RESULTTYPE) ItemData;
				m_pDoc->Set_GrandeurTri(ResultType); 
			}
		}

		//////////////////////////////////////////// Les modes affichage r�sultats= (Grandeur, H%, V%, et autres) dispos pour le tri
		// Les s�lections dispos r�sultats (et chacune des s�lection courante)
		m_Lst_Results.ResetContent();
		for (m_pDoc->m_MapResultCellDispo.MoveFirst(); m_pDoc->m_MapResultCellDispo.IsValid(); m_pDoc->m_MapResultCellDispo.MoveNext())
		{
			// R�cup�ration de l'item data 
			JUnt32 ItemData = m_pDoc->m_MapResultCellDispo.GetKey();

			// R�cup�ration du libell� r�sultat
			CString LibResult = m_pDoc->m_MapResultCellDispo.GetItem();

			// Attention si c'est la grandeur global
			if (LibResult == "Global")
			{
				// Mode calcul global standard
				if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
				{
					// LibResult = "IA";  Indice affinit�
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

			// Ajoute � la liste param�tres tris dispo
			JInt32 InxLst = m_Lst_Results.AddString(LibResult); 

			// Info ident result
			m_Lst_Results.SetItemData(InxLst, ItemData); 
		}

		// puis on positionne le resultat tri s�lection en cours
		if (m_Lst_Results.GetCount() > 0)
		{
			for (JInt32 InxLst = 0; InxLst < m_Lst_Results.GetCount(); InxLst++)	
			{
				// R�cup item data de l'�lmt
				JInt32 ItemData = m_Lst_Results.GetItemData(InxLst);

				// Si c'est celui courant, on le s�lectionne
				if (ItemData == m_pDoc->Get_ResultTri())
				{
					m_Lst_Results.SetCurSel(InxLst); 
					break;
				}
			}

			// Si aucune s�lection, on se positionne sur le 1er elmt de la liste des r�sultats dispos
			if (m_Lst_Results.GetCurSel() < 0)
			{
				// On s�lectionne le 1er elmt
				m_Lst_Results.SetCurSel(0);
				JInt32 ItemData = m_Lst_Results.GetItemData(0);
				
				// On met � jour le doc
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

	// On d�finit les caract�ristqies des diff�rents controles
	SetupControls();

	// Affiche les elmts liste grandeurs et r�sultats dispo
	SetParamsTri();

	// On positionne l'�tat des contr�les
	SetControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}

void CDlgParamsTri::SetupControls()
{
	// Les boutons de sortie
	m_Btn_OK			.SubclassDlgItem(IDOK,							this);
	m_Btn_CANCEL		.SubclassDlgItem(IDCANCEL,						this);
	
	// Boite de s�lection ordre tri
	m_Box_OrdreTri		.SubclassDlgItem(IDC_AN_STATIC_ORDRE_TRI,		this);
	m_Ttl_OrdreTri		.SubclassDlgItem(IDC_AN_STATIC_TTL_ORDRETRI,	this);
	m_Rad_TriCroissant	.SubclassDlgItem(IDC_AN_RAD_CROISSANT,			this);
	m_Rad_TriDecroissant.SubclassDlgItem(IDC_AN_RAD_DECROISSANT,		this);
	m_Rad_AucunTri		.SubclassDlgItem(IDC_AN_RAD_AUCUNTRI,			this);

	// Boite de s�lection grandeur tri
	m_Box_GrandeursTri	.SubclassDlgItem(IDC_AN_STATIC_GRANDEUR_TRI,	this);
	m_Ttl_GrandeurTri	.SubclassDlgItem(IDC_AN_STATIC_TTL_GRANDEURTRI,	this);
	m_Lst_Grandeurs		.SubclassDlgItem(IDC_AN_LIST_TRI_GRANDEURS,		this);
	
	// Boite de s�lection r�sultat cellule tri
	m_Box_ResultsTri	.SubclassDlgItem(IDC_AN_STATIC_RESULT_TRI,		this);
	m_Ttl_ResultTri		.SubclassDlgItem(IDC_AN_STATIC_TTL_RESULTTRI,	this);
	m_Lst_Results		.SubclassDlgItem(IDC_AN_LIST_TRI_RESULTATS,		this);

	// Boite de s�lection nature tri
	m_Box_NatureTri		.SubclassDlgItem(IDC_AN_STATIC_NATURE_TRI,		this);
	m_Ttl_NatureTri		.SubclassDlgItem(IDC_AN_STATIC_TTL_NATURETRI,	this);
	m_Rad_Global		.SubclassDlgItem(IDC_AN_RAD_GLOBAL,				this);
	m_Rad_ParBloc		.SubclassDlgItem(IDC_AN_RAD_PARBLOC,			this);
	
	// D�finition boite s�lection ordre tri
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
	
	// D�finition boite s�lection r�sultat cellule tri
	m_Box_ResultsTri.Reset();
	m_Box_ResultsTri.SetTransparent(false);
	m_Box_ResultsTri.SetSunken(true);
	m_Box_ResultsTri.AddTopWindow(m_Ttl_ResultTri);
	m_Box_ResultsTri.AddTopWindow(m_Lst_Results);

	// D�finition boite s�lection r�sultat cellule tri
	m_Box_GrandeursTri.Reset();
	m_Box_GrandeursTri.SetTransparent(false);
	m_Box_GrandeursTri.SetSunken(true);
	m_Box_GrandeursTri.AddTopWindow(m_Ttl_GrandeurTri);
	m_Box_GrandeursTri.AddTopWindow(m_Lst_Grandeurs);

	// D�finition boite s�lection nature tri
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

	// D�finition des couleurs
	SetColors();

	// Positionnement de la fen�tre
	CRect Rect;
	GetWindowRect(Rect);
	SetWindowPos(NULL, m_PosFen.x, m_PosFen.y, Rect.Width(), Rect.Height(), NULL);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// D�finition des couleurs controles
void CDlgParamsTri::SetColors()
{
	// Couleur fond des boites de s�lection
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
// Positionnement �tat des controles
void CDlgParamsTri::SetControls()
{
	// Check s�lection ordre de tri en cours
	if(m_TypeTri == m_pDoc->TYPE_CROISSANT)
		CheckRadioButton(IDC_AN_RAD_CROISSANT ,IDC_AN_RAD_AUCUNTRI ,IDC_AN_RAD_CROISSANT);

	else if (m_TypeTri == m_pDoc->TYPE_DECROISSANT)
		CheckRadioButton(IDC_AN_RAD_CROISSANT ,IDC_AN_RAD_AUCUNTRI ,IDC_AN_RAD_DECROISSANT);

	else if (m_TypeTri == m_pDoc->TYPE_BASE)
		CheckRadioButton(IDC_AN_RAD_CROISSANT ,IDC_AN_RAD_AUCUNTRI ,IDC_AN_RAD_AUCUNTRI);

	// Check s�lection nature du tri
	if(m_ModeTri == m_pDoc->MODE_LIGNE) 
		CheckRadioButton(IDC_AN_RAD_GLOBAL ,IDC_AN_RAD_PARBLOC ,IDC_AN_RAD_GLOBAL);
	else if (m_ModeTri == m_pDoc->MODE_BLOC)
		CheckRadioButton(IDC_AN_RAD_GLOBAL ,IDC_AN_RAD_PARBLOC ,IDC_AN_RAD_PARBLOC);

}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection ordre de tri >> passage en ordre croissant
void CDlgParamsTri::OnBnClickedAnRadCroissant()
{
	m_TypeTri = m_pDoc->TYPE_CROISSANT; 
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection ordre de tri >> passage en ordre d�roissant
void CDlgParamsTri::OnBnClickedAnRadDecroissant()
{
	m_TypeTri = m_pDoc->TYPE_DECROISSANT; 
	SetControls();	
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection ordre de tri >> passage retour ordre d'origine
void CDlgParamsTri::OnBnClickedAnRadAucuntri()
{
	m_TypeTri = m_pDoc->TYPE_BASE; 
	SetControls();	
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection nature du tri  >> sur toutes les lignes du tableur
void CDlgParamsTri::OnBnClickedAnRadGlobal()
{
	m_ModeTri = m_pDoc->MODE_LIGNE; 
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Changement s�lection nature du tri >> par blocs de crit�re
void CDlgParamsTri::OnBnClickedAnRadParbloc()
{
	m_ModeTri = m_pDoc->MODE_BLOC; 
	SetControls();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// On valide les nouvelles s�lections tris
void CDlgParamsTri::OnBnClickedOk()
{
	// Valide les s�lections dans le documents	
	m_pDoc->Set_TypeTri(m_TypeTri);
	m_pDoc->Set_ModeTri(m_ModeTri);

	// Ident elmt 
	JUnt32 ItemData;

	// Valide la s�lection grandeur
	if (m_Lst_Grandeurs.GetCurSel() >= 0)
	{
		// R�cup ident elmt grandeur s�lectionn�
		ItemData = m_Lst_Grandeurs.GetItemData(m_Lst_Grandeurs.GetCurSel());	

		JUnt32 Item0 = m_Lst_Grandeurs.GetItemData(0);
		JUnt32 Item1 = m_Lst_Grandeurs.GetItemData(1);
		JUnt32 Item2 = m_Lst_Grandeurs.GetItemData(2);
	}	
	else
	{
		// On s�lectionne le 1er elmt
		m_Lst_Grandeurs.SetCurSel(0);
		ItemData = m_Lst_Grandeurs.GetItemData(0);
	}

	// On met � jour le doc
	RESULTTYPE ResultType = (RESULTTYPE) ItemData;
	m_pDoc->Set_GrandeurTri(ResultType);


	// Valide la s�lection r�sultat
	if (m_Lst_Results.GetCurSel() >= 0)
	{
		// R�cup ident elmt r�sultat s�lectionn�
		ItemData = m_Lst_Results.GetItemData(m_Lst_Results.GetCurSel());	
	}	
	else
	{
		// On s�lectionne le 1er elmt
		m_Lst_Results.SetCurSel(0);
		ItemData = m_Lst_Results.GetItemData(0);
	}
	
	// On met � jour le doc
	RESULTSTYLE ResultStyle = (RESULTSTYLE) ItemData; 
	m_pDoc->Set_ResultTri(ResultStyle);

	OnOK();
}
// On ne tient pas compte des nouvelles s�lections
void CDlgParamsTri::OnBnClickedCancel()
{
	OnCancel();
}
