// CibleSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CibleSelectionDlg.h"
#include ".\cibleselectiondlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CCibleSelectionDlg dialog

IMPLEMENT_DYNAMIC(CCibleSelectionDlg, CDialog)
CCibleSelectionDlg::CCibleSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCibleSelectionDlg::IDD, pParent)
{
}

CCibleSelectionDlg::~CCibleSelectionDlg()
{
}

void CCibleSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCibleSelectionDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_AN_LIST_SELCIBLE, OnLbnSelchangeAnListSelcible)
	ON_LBN_SELCHANGE(IDC_AN_LIST_CIBLES, OnLbnSelchangeAnListCibles)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCibleSelectionDlg message handlers

void CCibleSelectionDlg::OnLbnSelchangeAnListSelcible()
{
	int Sel = m_List_Selected.GetCurSel(); 

	if(Sel != LB_ERR)
	{
		
		// Récupére Item
		CItemConstruit *pt = (CItemConstruit *)m_List_Selected.GetItemData(Sel);

		// Label de l'élmt enlevé
		CString LabelDelete;
		LabelDelete = pt->GetLabel().AsJCharPtr();

		// Vire de la liste sélection en haut		
		m_List_Selected.DeleteString(Sel);

		// Mettre à jour le marquage sur la liste du bas
		for (int i=0; i < m_List_Cibles.GetCount(); i++)
		{
			CItemConstruit *ptOrg = (CItemConstruit *)m_List_Cibles.GetItemData(i);

			// Démarque si même libellé
			if (ptOrg->GetLabel().AsJCharPtr() == LabelDelete)
				m_List_Cibles.SetSel(i,FALSE);
		}
	}

	
}

// Changement de sélection des univers disponibles
void CCibleSelectionDlg::OnLbnSelchangeAnListCibles()
{
	// Init Liste sélection
	m_List_Selected.ResetContent();

	// Si liste sélection non vide, on met à jour les sélections sur la liste en haut
	for (int i=0; i < m_List_Cibles.GetCount(); i++)
	{
		// On ajoute à la liste des univers sélectionnés
		if (m_List_Cibles.GetSel(i))
		{
			// Récupère l'élm
			CItemConstruit *pt = (CItemConstruit *)m_List_Cibles.GetItemData(i);
			if(pt)
				m_List_Selected.SetItemData(m_List_Selected.AddString(pt->GetLabel().AsJCharPtr()),(DWORD_PTR)pt);
		}
	}

}


bool CCibleSelectionDlg::SetData(CAnalyseurDoc * pDoc)
{
	m_pDoc = pDoc;
	m_bCible = (m_pDoc->Get_ResultStyle()& RSTYLE_CIBLES)  ;
	m_pTerrain = m_pDoc->Get_TerrainTemoin();
	const JList<const CItemConstruit *> & ListItems = m_pDoc->Get_ListCibles();
	for(ListItems.MoveFirst(); ListItems.IsValid (); ListItems.MoveNext())
		m_SelectedItems.AddTail() = ListItems.GetItem();
	return true;
}

void CCibleSelectionDlg::SetupControls()
{
	m_bkgCible.			SubclassDlgItem(IDC_AN_STATIC_CIBLE, this);
	m_ttlCible.		SubclassDlgItem(IDC_AN_STATIC_TTLCIBLE, this);
	m_But_Ok. 			SubclassDlgItem(IDOK				,this);
	m_But_Cancel. 		SubclassDlgItem(IDCANCEL			,this);
	m_List_Selected.	SubclassDlgItem(IDC_AN_LIST_SELCIBLE,this);
	m_List_Cibles.		SubclassDlgItem(IDC_AN_LIST_CIBLES	,this);

	m_bkgCible.SetTransparent(false);
	m_bkgCible.SetSunken(true);
	m_bkgCible.AddTopWindow(m_ttlCible);
	m_bkgCible.AddTopWindow(m_List_Selected);
	m_bkgCible.AddTopWindow(m_List_Cibles);

	if(m_bCible)
	{
		CString Txt;
		Txt.LoadString(IDS_AN_TITRE_CHOISIR_CIBLES);
		this->SetWindowText(Txt);
		Txt.LoadString(IDS_AN_LABEL_CIBLES);
		m_ttlCible.SetWindowText(Txt); 
	}
	else
	{
		CString Txt;
		Txt.LoadString(IDS_AN_TITRE_CHOISIR_UNIVERS);
		this->SetWindowText(Txt);
		Txt.LoadString(IDS_AN_LABEL_UNIVERS);
		m_ttlCible.SetWindowText(Txt); 
	}
	m_ttlCible.SetFontName(CATPColors::GetFontLabelName());
	m_ttlCible.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlCible.SetBorder(false);

	// Set the windows colors
	SetColors();
}

void CCibleSelectionDlg::SetColors()
{
	m_bkgCible.		SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::MEDIUM_DEGRAD);

	m_ttlCible.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR, CATPColors::DARK_DEGRAD);

	m_ttlCible.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_List_Selected.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));
	m_List_Cibles.	SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));
}

BOOL CCibleSelectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Voir si terrain valide
	if(m_pTerrain == 0)
	{
		EndDialog(0);
		return true;
	}

	// Mise en place des contrôles
	SetupControls();

	// Get the items construits
	JList<const CItemConstruit *> ListItems;
	// CItemsConstruits *pItemsConstruits = CItemsConstruits::GetInstance(); 
	CItemsConstruitsBase * pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	pItemsConstruits->GetItemsIndependant(ListItems,m_pTerrain);
	for(ListItems.MoveFirst();ListItems.IsValid();ListItems.MoveNext())
	{
		const CItemConstruit *pItem = ListItems.GetItem();
		if(pItem == pItemsConstruits->GetItemEnsemble())
			continue;

		for(m_SelectedItems.MoveFirst(); m_SelectedItems.IsValid(); m_SelectedItems.MoveNext())
			if(m_SelectedItems.GetItem() == pItem)
				break;

		// Récupère texte à afficher
		CString LibAjout;
		LibAjout = pItem->GetLabel().AsJCharPtr();

		// Ajoute à liste des items disponibles en bas
		int IndexAjout = m_List_Cibles.AddString(LibAjout);

		// Info item dans liste dispo
		m_List_Cibles.SetItemData(IndexAjout,(DWORD_PTR)pItem);

		if(m_SelectedItems.IsValid())
		{
			// Ajoute à liste sélection en haut
			m_List_Selected.SetItemData(m_List_Selected.AddString(LibAjout),(DWORD_PTR)pItem);

			// Marque dans liste du bas
			m_List_Cibles.SetSel(IndexAjout,TRUE);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCibleSelectionDlg::OnBnClickedOk()
{
	/*
	int Count = m_List_Selected.GetCount();

	m_SelectedItems.Reset();
	// m_SelectedItems.AddTail() = CItemsConstruits::GetInstance()->GetItemEnsemble();
	m_SelectedItems.AddTail() = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemEnsemble();

	for(int x = 0; x < Count; ++x)
	{
		m_SelectedItems.AddTail() = (const CItemConstruit *)m_List_Selected.GetItemData(x);
	}
	m_pDoc->Set_ListCibles(m_SelectedItems); 
	*/

	int Count = m_List_Cibles.GetCount();

	m_SelectedItems.Reset();
	m_SelectedItems.AddTail() = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemEnsemble();

	for(int x = 0; x < Count; ++x)
	{
		int InxSel = m_List_Cibles.GetSel(x);

		if (m_List_Cibles.GetSel(x) > 0)
		{
			// Récupère l'élm
			CItemConstruit *pt = (CItemConstruit *)m_List_Cibles.GetItemData(x);
			if(pt)
				m_SelectedItems.AddTail() = (const CItemConstruit *)pt;
		}
	}
	m_pDoc->Set_ListCibles(m_SelectedItems); 

	OnOK();
}

