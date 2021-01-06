// C:\Projects\Atelier Presse\AtelierPresse\Creuset\DlgVersionPlan.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgVersionPlan.h"
#include "PlanMasters.h"
#include "PlanInsertions.h"
#include ".\dlgversionplan.h"


// Boîte de dialogue CDlgVersionPlan

IMPLEMENT_DYNAMIC(CDlgVersionPlan, CDialog)
CDlgVersionPlan::CDlgVersionPlan(JBool SaveAuto,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVersionPlan::IDD, pParent)
{
	m_SaveAuto = SaveAuto;
}

CDlgVersionPlan::~CDlgVersionPlan()
{
}

void CDlgVersionPlan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CR_LISTPLAN, m_ListPlan);
}


BEGIN_MESSAGE_MAP(CDlgVersionPlan, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_CR_LISTPLAN, OnLbnDblclkCrListplan)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgVersionPlan



BOOL CDlgVersionPlan::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Init plan de récupération
	m_PlanRefArchive.Reset();

	// Remplissage de la liste des plans précédents
	LoadListPlanPrec();

	SetupControls();

	return TRUE;  
	
}

// Chargement de la liste des plans précédents 
void CDlgVersionPlan::LoadListPlanPrec()
{
	// SaveAuto = true  : les plans sauvegardés automatiquement
	// ........ = false : ..................... avec un nom   

	// Init liste plans
	m_ListPlan.ResetContent(); 

	// Index tableau plans
	JInt32 IndexPlan = m_pATPDocCreuset->m_TblPlanArchive.GetCount()-1;
	JInt32 IndexList = -1;

	// Charge les plans via creuset
	for (m_pATPDocCreuset->m_TblPlanArchive.MoveLast();
		 m_pATPDocCreuset->m_TblPlanArchive.IsValid();
		 m_pATPDocCreuset->m_TblPlanArchive.MovePrev())
	{
		// Récupère plan master
		CPlanMasters &PlanMaster = m_pATPDocCreuset->m_TblPlanArchive.GetItem();

		// Ajoute à la liste les plans sauvegardés avec nom
		IndexList = m_ListPlan.AddString(PlanMaster.m_Libelle.AsJCharPtr()); 

		// Mise à jour Item Data associé	
		m_ListPlan.SetItemData(IndexList,IndexPlan); 
		IndexPlan --;
	}
}

void CDlgVersionPlan::OnBnClickedOk()
{
	if (m_ListPlan.GetCount())
	{
		// Récup Index dans tableau plans archivés (selon mode archive)
		JInt32 IndexArchive;
		IndexArchive = (JInt32)m_ListPlan.GetItemData(m_ListPlan.GetCurSel());

		if (IndexArchive >= 0)
		{
			// Récupérer plan sélectionné 
			m_pATPDocCreuset->m_TblPlanArchive.MoveTo(IndexArchive); 
			CPlanMasters & PlanMaster = m_pATPDocCreuset->m_TblPlanArchive.GetItem(); 
		
			// Récupère le plan insertion correspondant
			m_PlanRefArchive = PlanMaster.m_Plan; 
		}
	}

	// Sortie box Dialog
	CDialog::OnOK();
}

// Sélection d'une ancienne version plan de référence
void CDlgVersionPlan::OnLbnDblclkCrListplan()
{
	if (m_ListPlan.GetCount())
	{
		// Récup Index dans tableau plans archivés (selon mode archive)
		JInt32 IndexArchive;
		IndexArchive = (JInt32)m_ListPlan.GetItemData(m_ListPlan.GetCurSel());

		if (IndexArchive >= 0)
		{
			// Récupérer plan sélectionné 
			m_pATPDocCreuset->m_TblPlanArchive.MoveTo(IndexArchive); 
			CPlanMasters & PlanMaster = m_pATPDocCreuset->m_TblPlanArchive.GetItem(); 
		
			// Récupère le plan insertion correspondant
			m_PlanRefArchive = PlanMaster.m_Plan; 
		}
	}

	// Sortie box Dialog
	CDialog::OnOK();	
}

void CDlgVersionPlan::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_CR_BK, this);
	m_ttl.SubclassDlgItem(IDC_CR_STATIC_TTL, this);
	m_btnOK.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

////
	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	m_bkg.AddTopWindow(m_ListPlan);

////
	m_ttl.SetFontName(CATPColors::GetFontLabelName());
	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);

	SetColors();
}

void CDlgVersionPlan::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);

	m_ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_ListPlan.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET, 0 );
	//m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET), 0 );
	//m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );

	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET, 0 );
	//m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET), 0 );
	//m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
}
