#include "StdAfx.h"
#include "DlgProfiling.h"
// #include "DlgVectProfile.h"
#include "ProfileCrossTable.h"
#include "DlgTableauProfile.h"
#include "AnalyseurEnums.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDlgProfiling, CDlgAnalyseur)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_BN_CLICKED(IDC_AN_BUT_VECTORIELLE , OnBnClickedButVectorielle)
	ON_COMMAND(IDM_AN_TABLEAU_TRANSPOSER, OnTableauTransposer)
	ON_COMMAND(IDM_AN_TABLEAU_MODIFIER,	OnBnClickedButVectorielle)
	ON_COMMAND(IDM_AN_TABLEAU_APPLIQUER_CANEVAS, OnTableauAppliquerCanevas)
	ON_COMMAND(IDM_AN_TABLEAU_ENREGISTRER_CANEVAS, OnTableauEnregistrerCanevas)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CDlgProfiling, CDlgAnalyseur)

CDlgProfiling::CDlgProfiling(CWnd* pParent /*=NULL*/)
	: CDlgAnalyseur(pParent)
{
	// Init des seuils  (seuil modifié le 21/01/2010 voir Martine notamment pour certains sites)
	AfxGetApp()->WriteProfileInt("Moteur", "Seuil_Avertissement", 160);
	AfxGetApp()->WriteProfileInt("Moteur", "Seuil_Limite", 160);
	
	// Init paramètres par défaut surlignage
	m_Default_OpeSurlignage.Param_Grandeur	= RTYPE_EFFECTIVE_CIBLE;
	m_Default_OpeSurlignage.Param_Result	= RSTYLE_HORZ_STRUCTURE;

	// On indique q'on est en mode Sites
	m_pDlgTableau->SetShowSite(SHOW_SITES);
}

CDlgProfiling::~CDlgProfiling(void)
{
}

void CDlgProfiling::SetupCrossTable()
{
	m_pCrossTable = new CProfileCrossTable();
}

void CDlgProfiling::OnTableauTransposer()
{
	/* AVANT
	CWaitCursor Cw;
	m_pDoc->TransposeRowCol();
	UpdateResultStyles(); 
	m_pCrossTable->CalculateItems(); 
	SetTitre();

	*/

	// Wait
	CWaitCursor Cw;

	// Transposition Lignes <> Colonnes
	m_pDoc->TransposeRowCol();

	UpdateResultStyles(); 

	// On remet les paramètres de calcul en forçant le recalcul sur lignes et colonnes (car depuis moyenne calcul change, vecteur quanti uniquement en ligne)
	m_pDoc->SetCalculation();
	m_pDoc->Add_Calculation(CALC_ROWS);
	m_pDoc->Add_Calculation(CALC_COLS);

	// Idem pour les palettes ligne, colonne
	m_pDlgTableau->Transpose(CPaletteMode::Line,CPaletteMode::Column);

	m_pCrossTable->CalculateItems();

	UpdateSurlignage();
	SetTitre();
}

void CDlgProfiling::OnFileNew()
{
	// ATTENTION : cette fonction est la copie conforme de la fonction CDlgAnalyseur::OnFileNew
	//             if faut reporter les modifications (en attendant mieux)
	if(m_pDoc->IsDirty())
	{
		CString txt;
		txt.LoadString(IDS_AN_SAVETABLEAU);
		int Result = MessageBox(txt,0,MB_YESNOCANCEL);
		if(Result == IDYES)
			OnFileSave();
		else if(Result == IDCANCEL)
			return;
	}
	m_pDoc->Reset(); 

	m_pCrossTable->SetupGrid(&m_CrossTable, m_pDoc, m_ModeBase100Actif, &m_But_Ordonner);

	// Init liste des elmts moyennables
	m_List_VarMoy.ResetContent();

	CWaitCursor Cw;
	m_pCrossTable->CalculateItems(); 
	SetTitre();

	OnBnClickedButVectorielle();
}

void CDlgProfiling::OnBnClickedButVectorielle()
{
	m_pDlgTableau->SetShowSite(SHOW_SITES); 
	GoPupitre();
}

void CDlgProfiling::SetupGrandeursBase()
{
	CString Str;
	m_pDoc->GetGrandeurString(Str,RTYPE_EFFECTIVE_CIBLE);
	m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_EFFECTIVE_CIBLE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Suppression des options calcul non nécessaires
void CDlgProfiling::SuppOptionButGlobal(CMenu *pMenu)
{
	if(m_pDoc->GetRowDataType()== DTYPE_SITES  || m_pDoc->GetColDataType()== DTYPE_SITES)
	{
		// Suppression des options Indices de structure et Zscore
		pMenu->RemoveMenu(IDM_AN_GLOBAL_STRUCTURE,MF_BYCOMMAND);
		pMenu->RemoveMenu(IDM_AN_GLOBAL_ZSCORE,MF_BYCOMMAND); 
	}
	else if(m_pDoc->GetPageDataType()== DTYPE_SITES)
	{	
		// Suppression uniquement des options Indices de structure et Zscore
		pMenu->RemoveMenu(IDM_AN_GLOBAL_ZSCORE,MF_BYCOMMAND); 
	}
	else
		pMenu->RemoveMenu(IDM_AN_GLOBAL_ZSCORE,MF_BYCOMMAND); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Nouvelle version SQN_PROFILING 
// Autorise Indice Corrélation et Indice Proximité si sites en ligne ou colonne
//                + Pourcentages ou indices de structure si sites en page
void CDlgProfiling::UpdateResultStyles()
{
	// Par défaut validation de tous les boutons résultats
	m_But_Global.EnableWindow(true);
	m_But_Horiz.EnableWindow(true);
	m_But_Vert.EnableWindow(true);

	m_But_Global2.EnableWindow(true);
	m_But_Horiz2.EnableWindow(true);
	m_But_Vert2.EnableWindow(true);

	m_But_Global3.EnableWindow(true);
	m_But_Horiz3.EnableWindow(true);
	m_But_Vert3.EnableWindow(true);

	int DataType	= m_pDoc->GetRowDataType();
	DataType		= m_pDoc->GetColDataType();
	DataType		= m_pDoc->GetPageDataType();

	// Modif de Septembre 2007 / Accés Indice corrélation et Indice proximité
	// 
	if(m_pDoc->GetRowDataType()== DTYPE_SITES)
	{
		// On inhibe les résultats en vertical
		m_But_Vert.EnableWindow(false);
		m_But_Vert2.EnableWindow(false);
		m_But_Vert3.EnableWindow(false);

		// Horizontal Values
		if(m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL)
			m_pDoc->Remove_ResultStyle(RSTYLE_VERT_STRUCTURE);

		if(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
		{
			switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
			{
				case RSTYLE_GBL_STRUCTURE :
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_STRUCTURE);
				break;
				case RSTYLE_GBL_CORRELATION:
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_CORRELATION);
				break;
				case RSTYLE_GBL_PROXIMITY:
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_PROXIMITY);
				break;
			}
		}
		m_pDoc->Add_ResultStyle(RSTYLE_HORZ_STRUCTURE);
	}
	else if(m_pDoc->GetColDataType()== DTYPE_SITES)
	{
		// On inhibe les résultats en horizontal
		m_But_Horiz.EnableWindow(false);
		m_But_Horiz2.EnableWindow(false);
		m_But_Horiz3.EnableWindow(false);

		// Vertical values
		if(m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL)
			m_pDoc->Remove_ResultStyle(RSTYLE_HORZ_STRUCTURE);

		if(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
		{
			switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
			{
				case RSTYLE_GBL_STRUCTURE :
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_STRUCTURE);
				break;
				case RSTYLE_GBL_CORRELATION:
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_CORRELATION);
				break;
				case RSTYLE_GBL_PROXIMITY:
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_PROXIMITY);
				break;
			}
		}
		m_pDoc->Add_ResultStyle(RSTYLE_VERT_STRUCTURE);
	}
	else if(m_pDoc->GetPageDataType()== DTYPE_SITES)
	{
		// Global Values 
		if(m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL)
			m_pDoc->Remove_ResultStyle(RSTYLE_HORZ_STRUCTURE);
		if(m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL)
			m_pDoc->Remove_ResultStyle(RSTYLE_VERT_STRUCTURE);

		m_pDoc->Add_ResultStyle(RSTYLE_GBL_STRUCTURE);
	}
	else
	{
		m_But_Global.EnableWindow(true);
		m_But_Horiz.EnableWindow(true);
		m_But_Vert.EnableWindow(true);

		m_But_Global2.EnableWindow(true);
		m_But_Horiz2.EnableWindow(true);
		m_But_Vert2.EnableWindow(true);

		m_But_Global3.EnableWindow(true);
		m_But_Horiz3.EnableWindow(true);
		m_But_Vert3.EnableWindow(true);

		if(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
		{
			switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
			{
				case RSTYLE_GBL_STRUCTURE :
					m_pDoc->Add_ResultStyle(RSTYLE_GBL_STRUCTURE);
				break;
				case RSTYLE_GBL_CORRELATION:
					m_pDoc->Add_ResultStyle(RSTYLE_GBL_CORRELATION);
				break;
				case RSTYLE_GBL_PROXIMITY:
					m_pDoc->Add_ResultStyle(RSTYLE_GBL_PROXIMITY);
				break;
			}
		}
	}

	// Positionne les contrôles résultats (Grandeur, H%, V% etc...)
	SetResultControls();

	// Init sélection des tris
	SetupSelTri();
}

BOOL CDlgProfiling::OnInitDialog()
{
	BOOL Ret = CDlgAnalyseur::OnInitDialog();
	// Never show Grandeur in profiling
	m_But_Grandeur.EnableWindow(false); 
	m_But_Grandeur2.EnableWindow(false); 
	m_But_Grandeur3.EnableWindow(false); 
	
	// Remove the loading and saving of the parameters in profiling
	CMenu * pMenu = GetMenu();
	pMenu->DeleteMenu(IDM_AN_TABLEAU_APPLIQUER_CANEVAS,  MF_BYCOMMAND); 
	pMenu->DeleteMenu(IDM_AN_TABLEAU_ENREGISTRER_CANEVAS,MF_BYCOMMAND); 

	// Ainsi qu'un séparateur
	CMenu * pMenu1 = GetMenu()->GetSubMenu(0);
	pMenu1->DeleteMenu(5, MF_BYPOSITION );

	// Aucune grandeur sélectionnée pour SQN-Profiling
	m_pDoc->Remove_ResultStyle(RSTYLE_RESULT); 
	m_But_Grandeur.SetCheck(0);
	m_But_Grandeur2.SetCheck(0);
	m_But_Grandeur3.SetCheck(0);

	return Ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Redirection vers la construction Tris Croises = Pupitre
void CDlgProfiling::GoPupitre()
{
	// positionne les élmts déjà sélectionné
	m_pDlgTableau->SetData(m_pDoc);

	// Invalide le bouton Affichage Pupitre
	m_But_Vectorielle.EnableWindow(FALSE); 

	// Pointeur sur terrain temoin en cours
	CTerrain *pTerrainTemoin = m_pDoc->Get_TerrainTemoin();

	if (pTerrainTemoin)
	{
		// Les segments en cours
		const JList<UINT> & SegmentsId = m_pDoc->Get_SegmentsId();
		
		// L'identificateur vecteur poids
		m_pDlgTableau->SetParams(m_pDoc->Get_TypePoidsId(), SegmentsId);

		// Lance le sélecteur en mode ligne
		m_pDlgTableau->SetMode(CPaletteMode::Line); 

		// Ouverture Répertoire des items en mode Pupitre
		if(m_pDlgTableau->DoModal() == IDOK)
		{
			// Récupération des élmts sélectionnés pour tris croisés (lignes + colonnes + pages)
			InvalidateRect(NULL, FALSE);
			UpdateWindow();
			m_pDlgTableau->GetData(m_pDoc);

			// mise à jour des styles
			UpdateResultStyles();

			if((m_pDoc->Get_ResultStyle()&(RSTYLE_RESULT | RSTYLE_GLOBAL | RSTYLE_HORIZONTAL | RSTYLE_VERTICAL)) == 0)
			{
				// Force grandeur % vertical
				m_pDoc->Set_ResultStyle(RSTYLE_VERTICAL);
				SetResultControls();
			}

			CWaitCursor Cw;
			if(m_pCrossTable)
				m_pCrossTable->CalculateItems();

			// Puis on positionne le source et terrain de référence
			CTerrain * pTerrainRef = m_pDoc->Get_TerrainTemoin();
			JUnt32 IdSourceRef	= pTerrainRef->GetSourceID();
			JUnt32 IdTerrainRef = pTerrainRef->GetTerrainID();

			m_pDoc->Set_SourceIdRef(IdSourceRef);
			m_pDoc->Set_TerrainIdRef(IdTerrainRef);
			
		}
	}
	else
	{
		// Pas de terrain actif, signaler à l'utilisateur d'en sélectionner au moins 1
		AfxMessageBox("Pas de terrain en cours, veuillez en sélectionner un !!!");
	}

	// Remet le bouton actif
	m_But_Vectorielle.EnableWindow(TRUE); 
}

