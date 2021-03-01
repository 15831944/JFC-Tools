#include "StdAfx.h"
#include "DlgAdvision.h"
#include "ProfileCrossTable.h"
#include "AnalyseurEnums.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDlgAdvision, CDlgAnalyseur)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CDlgAdvision, CDlgAnalyseur)

// Constructeur
CDlgAdvision::CDlgAdvision(CWnd* pParent /*=NULL*/)
	: CDlgAnalyseur(pParent)
{
}

// Destructeur
CDlgAdvision::~CDlgAdvision(void)
{
}

// Setup Tableau
void CDlgAdvision::SetupCrossTable()
{
	m_pCrossTable = new CProfileCrossTable();
}

// Mise en place des grandeurs pour Advision
void CDlgAdvision::SetupGrandeurs()
{
	// Setup the Grandeurs list box
	m_List_Grandeurs.ResetContent();

	// Chargement des grandeurs de bases
	this->SetupGrandeursBase();

	// Chargement des grandeurs complémentaires
	CString Str;
	if(m_pDoc->Get_ResultStyle() & RSTYLE_CIBLES)
	{
		m_pDoc->GetGrandeurString(Str,RTYPE_PENETRATION);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_PENETRATION);
		m_pDoc->GetGrandeurString(Str,RTYPE_CENTRAGE);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_CENTRAGE);
		m_pDoc->GetGrandeurString(Str,RTYPE_CONCOMITENCE);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_CONCOMITENCE);
		m_pDoc->GetGrandeurString(Str,RTYPE_ZSCORE);
		m_List_Grandeurs.SetItemData(m_List_Grandeurs.AddString(Str),RTYPE_ZSCORE);
	}
	m_List_Grandeurs.Invalidate(TRUE);
	m_List_Grandeurs.UpdateWindow(); 

}

// Suppression des options calcul non nécessaires
void CDlgAdvision::SuppOptionButGlobal(CMenu *pMenu)
{
	// Suppression de toutes les options // ne plus utiliser le menu popup
	pMenu->RemoveMenu(IDM_AN_GLOBAL_STRUCTURE,MF_BYCOMMAND);
	pMenu->RemoveMenu(IDM_AN_GLOBAL_PROXIMITY,MF_BYCOMMAND); 
	pMenu->RemoveMenu(IDM_AN_GLOBAL_AFFINITE,MF_BYCOMMAND); 
	pMenu->RemoveMenu(IDM_AN_GLOBAL_ZSCORE,MF_BYCOMMAND); 
}

void CDlgAdvision::UpdateResultStyles()
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

	if(m_pDoc->GetRowDataType()== DTYPE_SITES)
	{
		m_But_Global.EnableWindow(false);
		m_But_Vert.EnableWindow(false);

		m_But_Global2.EnableWindow(false);
		m_But_Vert2.EnableWindow(false);

		m_But_Global3.EnableWindow(false);
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
				case RSTYLE_GBL_ZSCORE:
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_ZSCORE);
				break;
			}
		}
		// m_pDoc->Add_ResultStyle(RSTYLE_HORZ_STRUCTURE);
		m_pDoc->Add_ResultStyle(RSTYLE_HORZ_CORRELATION);
	}
	else if(m_pDoc->GetColDataType()== DTYPE_SITES)
	{
		m_But_Global.EnableWindow(false);
		m_But_Horiz.EnableWindow(false);

		m_But_Global2.EnableWindow(false);
		m_But_Horiz2.EnableWindow(false);

		m_But_Global3.EnableWindow(false);
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
				case RSTYLE_GBL_ZSCORE:
					m_pDoc->Remove_ResultStyle(RSTYLE_GBL_ZSCORE);
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

	SetResultControls();
}

// Setup spécifique des controles
void CDlgAdvision::SetResultControls()
{
	if(m_pDoc->Get_ResultStyle()&RSTYLE_RESULT)
	{
		m_But_Grandeur. SetCheck(1); 
		m_But_Grandeur2.SetCheck(1); 
		m_But_Grandeur3.SetCheck(1); 
	}
	else
	{
		m_But_Grandeur. SetCheck(0);
		m_But_Grandeur2.SetCheck(0);
		m_But_Grandeur3.SetCheck(0);
	}

	if(m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL)
	{
		m_But_Horiz.SetCheck(1); 
		m_But_Horiz2.SetCheck(1); 
		m_But_Horiz3.SetCheck(1); 
	}
	else
	{
		m_But_Horiz.SetCheck(0);
		m_But_Horiz2.SetCheck(0);
		m_But_Horiz3.SetCheck(0);
	}

	if(m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL)
	{
		m_But_Vert.SetCheck(1); 
		m_But_Vert2.SetCheck(1);
		m_But_Vert3.SetCheck(1);
	}
	else
	{
		m_But_Vert.SetCheck(0);
		m_But_Vert2.SetCheck(0);
		m_But_Vert3.SetCheck(0);
	}

	// Affichage tooltip sur les boutons grandeurs
	//
	m_But_Grandeur.SetTooltipText("Grandeur");
	m_But_Grandeur2.SetTooltipText("Grandeur"); 
	m_But_Grandeur3.SetTooltipText("Grandeur"); 

	// Tooltip info grandeur % en horizontal
	m_But_Horiz.SetTooltipText("% en horizontal");
	m_But_Horiz2.SetTooltipText("% en horizontal");
	m_But_Horiz3.SetTooltipText("% en horizontal");

	// Tooltip info grandeur % en vertical
	m_But_Vert.SetTooltipText("% en vertical");
	m_But_Vert2.SetTooltipText("% en vertical");
	m_But_Vert3.SetTooltipText("% en vertical");

	// Mise à jour texte bouton indice
	if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_CORRELATION)
	{
		// Indice d'affinité
		m_But_Global.SetTooltipText(IDS_AN_INDAFFINITE);
		m_But_Global2.SetTooltipText(IDS_AN_INDAFFINITE);
		m_But_Global3.SetTooltipText(IDS_AN_INDAFFINITE);
	}
	else if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_PROXIMITY)
	{
		// Indice de proximité
		m_But_Global.SetTooltipText(IDS_AN_PROXIMITY);
		m_But_Global2.SetTooltipText(IDS_AN_PROXIMITY);
		m_But_Global3.SetTooltipText(IDS_AN_PROXIMITY);
	}
	else if(m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_ZSCORE)
	{
		// ZScore
		m_But_Global.SetTooltipText(IDS_AN_ABR_ZSCORE);
		m_But_Global2.SetTooltipText(IDS_AN_ABR_ZSCORE);
		m_But_Global3.SetTooltipText(IDS_AN_ABR_ZSCORE);
	}
	else if (m_pDoc->m_ModeIndiceGlobal == RSTYLE_GBL_STRUCTURE)
	{
		// Global
		m_But_Global.SetTooltipText(IDS_AN_INDGLOBAL);
		m_But_Global2.SetTooltipText(IDS_AN_INDGLOBAL);
		m_But_Global3.SetTooltipText(IDS_AN_INDGLOBAL);
	}

	CString GblStr;
	GblStr.LoadString(IDS_AN_ABR_ZSCORE);
	if(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
	{
		m_But_Global.SetCheck(1);
		m_But_Global2.SetCheck(1);
		m_But_Global3.SetCheck(1);
		switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
		{
			case RSTYLE_GBL_ZSCORE:
				GblStr.LoadString(IDS_AN_ABR_ZSCORE);
			break;
		}
	}
	else
	{
		m_But_Global.SetCheck(0);
		m_But_Global2.SetCheck(0);
		m_But_Global3.SetCheck(0);

		// Mise à jour texte bouton indice
		switch(m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL)
		{
			case RSTYLE_GBL_STRUCTURE:
				GblStr.LoadString(IDS_AN_ABR_GLOBAL);
				break;
			case RSTYLE_GBL_CORRELATION:
				//				GblStr.LoadString(IDS_AN_ABR_CORRELATION);
				break;
			case RSTYLE_GBL_PROXIMITY:
				GblStr.LoadString(IDS_AN_ABR_PROXIMITY);
				break;
			case RSTYLE_GBL_ZSCORE:
				GblStr.LoadString(IDS_AN_ABR_ZSCORE);
				break;
		}
	}

	m_But_Global.SetWindowText(GblStr); 
	m_But_Global2.SetWindowText(GblStr); 
	m_But_Global3.SetWindowText(GblStr); 

	if(m_pDoc->Get_TableStyle()&TSTYLE_EXCLUSIVE)
		m_But_Exclusifs.SetCheck(1); 
	else
		m_But_Exclusifs.SetCheck(0);

	if(m_pDoc->Get_TableStyle()&TSTYLE_DUPLIQUE)
		m_But_Dupliquants.SetCheck(1); 
	else
		m_But_Dupliquants.SetCheck(0);
}

// Traitement spécifique bouton H% >> devient toujours Indice Affinité
void CDlgAdvision::ClickedAnButHoriz()
{
	// Selon état bouton horriz
	/*
	if(m_But_Horiz.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_GBL_CORRELATION);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_GBL_CORRELATION);
	*/
	if(m_But_Horiz.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_HORIZONTAL);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_HORIZONTAL);
	
	// Recalcul
	SetTitre();
	m_pCrossTable->CalculateItems(); 
}

// Gestion Bouton Global dans Advision / uniquement ZScore 
void CDlgAdvision::ClickedAnButGlobal()
{
	if(m_But_Global.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_GBL_ZSCORE);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_GBL_ZSCORE);

	SetTitre();
	m_pCrossTable->CalculateItems();
}


// Gestion Bouton Global dans Advision / uniquement ZScore (click gauche)
void CDlgAdvision::LClickedAnButGlobal()
{
	if(m_But_Global.GetCheck() != 0)
		m_pDoc->Add_ResultStyle(RSTYLE_GBL_ZSCORE);
	else
		m_pDoc->Remove_ResultStyle(RSTYLE_GBL_ZSCORE);

	SetTitre();
	m_pCrossTable->CalculateItems();
}

// Gestion Bouton Global click droit / pas gérer dans Advision (menu contextuel)
void CDlgAdvision::RClickedAnButGlobal()
{
}

// Initialisation fenêtre dialogue
BOOL CDlgAdvision::OnInitDialog()
{
	BOOL Ret = CDlgAnalyseur::OnInitDialog();

	// Modifie libellé bouton Global (d'office ZScore)
	CString txt;
	txt.LoadString(IDS_AN_ZSCORE);
	m_But_Global.SetWindowText(txt);

	// Modifie le libellé H% >> Indice Corrélation (ou Indice Affinité / pour Advision)
	txt.LoadString(IDS_AN_ABR_INDAFFINITE);
	m_But_Horiz.SetWindowText(txt);

	return Ret;
}