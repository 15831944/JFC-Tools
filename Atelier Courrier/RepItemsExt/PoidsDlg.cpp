// PoidDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\Poidsdlg.h"
#include "Poids.h"
#include "JSRCTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPoidsDlg dialog

IMPLEMENT_DYNAMIC(CPoidsDlg, CDialog)
CPoidsDlg::CPoidsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPoidsDlg::IDD, pParent)
{
	m_pTerrain = 0;
	m_IdPoids = 0;
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
}

CPoidsDlg::~CPoidsDlg()
{
}

void CPoidsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPoidsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()


bool CPoidsDlg::SetData(CTerrain * pTerrain, JUnt32 IdPoids)
{
	m_pTerrain = pTerrain;
	m_IdPoids = IdPoids;
	return true;
}

// CPoidsDlg message handlers
BOOL CPoidsDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	m_Button_OK.SubclassDlgItem(IDOK, this);
	m_Button_Cancel.SubclassDlgItem(IDCANCEL , this);
	m_Label.SubclassDlgItem(IDC_RI_LABEL, this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_ListPoids.SubclassDlgItem(IDC_RI_LIST_POIDS, this);

	m_ListPoids.SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label.SetBorder(false);
	m_Label.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 
	
	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_ListPoids);

	if(!m_pTerrain)
	{
		this->EndDialog(false);
		return false;
	}

	JVector<JInt32> Poids;
	if(m_pTerrain->GetTypesPoids(Poids))
	{
		const JSRCTables & srcTables = JSRCTables::GetInstance(); 
		m_ListPoids.ResetContent ();
		// Update the Poid list
		for(Poids.MoveFirst(); Poids.IsValid(); Poids.MoveNext())
		{
			JChar * pLibelle = srcTables.m_TBLPoids.GetLabelByID(Poids.GetItem());
			if(pLibelle)
				m_ListPoids.AddString(pLibelle);
		}
		// Set the selected Poids
		int x = 0;
		for(x=0, Poids.MoveFirst() ; Poids.IsValid(); Poids.MoveNext(),x++)
		{
			if(Poids.GetItem()==m_IdPoids)
				m_ListPoids.SetCurSel(x);
		}
	}
	return 0;
}

void CPoidsDlg::OnOk()
{
	int Item = m_ListPoids.GetCurSel(); 
	if(Item == LB_ERR)
		return;

	JVector<JInt32> Poids;
	m_pTerrain->GetTypesPoids(Poids);

	m_IdPoids = 0;
	Poids.MoveTo(Item);
	if(Poids.IsValid())
		m_IdPoids = Poids.GetItem();
	OnOK();
}