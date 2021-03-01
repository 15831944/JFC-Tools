// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExportDlg.h"
#include "ItemsConstruitsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CExportDlg dialog

IMPLEMENT_DYNAMIC(CExportDlg, CDialog)
CExportDlg::CExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportDlg::IDD, pParent)
{
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
}

CExportDlg::~CExportDlg()
{
}

void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExportDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()

BOOL CExportDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	m_Button_OK.SubclassDlgItem(IDOK, this);
	m_Button_Cancel.SubclassDlgItem(IDCANCEL , this);
	m_Label.SubclassDlgItem(IDC_RI_LABEL, this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_ListItems.SubclassDlgItem(IDC_RI_LIST_ITEMS, this);

	m_ListItems.SetSelectionColor(NORMAL,CATPColors::GetColorSelect(m_CATPColorsApp));

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label.SetBorder(false);
	m_Label.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp,CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_ListItems);

	// CItemsConstruits * pIConst = CItemsConstruits::GetInstance();
	
	// if (pIConst)
	if (CItemsConstruitsManager::GetInstance()->GetItemsInstance())
	{
		JList<IItem const *> vItems;
		m_ListItems.ResetContent(); 

		// pIConst->GetItemsIndependant(vItems);
		// pIConst->GetPalettes(vItems);
		// pIConst->GetCriteresModifieByBlocID(vItems);
		// pIConst->GetCriteresQuantileByBlocID(vItems); 
		// pIConst->GetQuestionsInduiteByBlocID(vItems); 

		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsIndependant(vItems);
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetPalettes(vItems);
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetCriteresModifieByBlocID(vItems);
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetCriteresQuantileByBlocID(vItems); 
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetQuestionsInduiteByBlocID(vItems);

		// Get only those valid for this terrain
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			m_ListItems.AddItem(vItems.GetItem());
		// Redraw
		m_ListItems.Invalidate();

	}
	return 0;
}

// CExportDlg message handlers

void CExportDlg::OnOk()
{
	JList<JInt32> ItemList;
	m_ListItems.GetSelectedID(ItemList);
	if(ItemList.GetCount())
	{
		// CItemsConstruits * pIConst = CItemsConstruits::GetInstance();
		CItemsConstruitsBase * pIConst = CItemsConstruitsManager::GetInstance()->GetItemsInstance();
		if(pIConst->Export(ItemList, this))
			OnOK();
	}
}
