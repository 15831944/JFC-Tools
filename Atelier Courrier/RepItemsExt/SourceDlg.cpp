// SourceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SourceDlg.h"
#include "BrowserDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSourceDlg dialog

IMPLEMENT_DYNAMIC(CSourceDlg, CDialog)
CSourceDlg::CSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSourceDlg::IDD, pParent)
{
	m_Id_Source	= 0;
	m_Id_Terrain = 0;
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
}

CSourceDlg::~CSourceDlg()
{
}

BOOL CSourceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Button_OK.SubclassDlgItem(IDOK, this);
	m_Button_Cancel.SubclassDlgItem(IDCANCEL , this);
	m_Button_Property.SubclassDlgItem(IDC_RI_BTN_PROPERTY , this);
	m_Label_Source.SubclassDlgItem(IDC_RI_STATIC_SOURCE  , this);
	m_Label_Terrain.SubclassDlgItem(IDC_RI_STATIC_TERRAIN , this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_ListTerrain.SubclassDlgItem(IDC_RI_LIST_TERRAIN, this);
	m_ComboSource.SubclassDlgItem(IDC_RI_COMBO_SOURCE, this);

	m_ComboSource.SetSelectionColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_ListTerrain.SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp)); 

	m_Label_Source.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Source.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Source.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Source.SetBorder(false);
	m_Label_Source.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));  

	m_Label_Terrain.SetFontName(CATPColors::GetFontLabelName());
	m_Label_Terrain.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label_Terrain.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label_Terrain.SetBorder(false);
	m_Label_Terrain.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));  

	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label_Source);
	m_Box.AddTopWindow(m_Label_Terrain);
	m_Box.AddTopWindow(m_ListTerrain);
	m_Box.AddTopWindow(m_ComboSource);


	bool Found = false;
	const JSRCTables & Sources = JSRCTables::GetInstance ();
	for(Sources.m_TBLSources.MoveFirst(); Sources.m_TBLSources.IsValid(); Sources.m_TBLSources.MoveNext())
	{
		const JSource & Item =  Sources.m_TBLSources.GetItem();
		int It = m_ComboSource.AddString(Item.m_Libelle.AsJCharPtr());
		m_ComboSource.SetItemData(It,Item.m_IdSource);  
		if(m_Id_Source == Item.m_IdSource)
		{
			m_ComboSource.SetCurSel(It);
			Found = true;
		}
	}

	if(!Found)
	{
		int Sel = m_ComboSource.SetCurSel(0);
		if(Sel!=CB_ERR)
			m_Id_Source = static_cast<int>(m_ComboSource.GetItemData(Sel));
		else
			m_Id_Source = 0;
	}
	OnCbnSelchangeComboSource();
	return true;
}

void CSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSourceDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_RI_COMBO_SOURCE, OnCbnSelchangeComboSource)
	ON_LBN_SELCHANGE(IDC_RI_LIST_TERRAIN, OnLbnSelchangeListTerrain)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_RI_BTN_PROPERTY, OnProperty)
END_MESSAGE_MAP()


// CSourceDlg message handlers

void CSourceDlg::OnCbnSelchangeComboSource()
{
	const JSRCTables & Sources = JSRCTables::GetInstance ();
	m_ListTerrain.ResetContent();
	int Sel = m_ComboSource.GetCurSel();
	if(Sel != CB_ERR)
	{
		int SourceId = static_cast<int>(m_ComboSource.GetItemData(Sel));
		if(SourceId != m_Id_Source)
		{
			m_Id_Source	=	SourceId;
			m_Id_Terrain =	0;
		}

		bool Found = false;
		for(Sources.m_TBLTerrains.MoveFirst(); Sources.m_TBLTerrains.IsValid(); Sources.m_TBLTerrains.MoveNext())
		{
			const JTerrain &Item = Sources.m_TBLTerrains.GetItem();
			if(Item.m_IdSource  == m_Id_Source)
			{
				int It = m_ListTerrain.AddString(Item.m_Libelle.AsJCharPtr());
				m_ListTerrain.SetItemData(It,Item.m_IdTerrain);
				if(m_Id_Terrain == Item.m_IdTerrain)
				{
					m_ListTerrain.SetCurSel(It);
					Found = true;
				}
			}
		}
		if(!Found)
		{
			int Sel = m_ListTerrain.SetCurSel(0);
			if(Sel!= LB_ERR)
				m_Id_Terrain = static_cast<int>(m_ListTerrain.GetItemData(Sel));
			else
				m_Id_Terrain = 0;
		}
	}
	else
		m_Id_Source = 0;

	OnLbnSelchangeListTerrain();
}

void CSourceDlg::OnLbnSelchangeListTerrain()
{
	const JSRCTables & Sources = JSRCTables::GetInstance ();
	int Sel = m_ListTerrain.GetCurSel();

	if(Sel != LB_ERR)
	{
		m_Id_Terrain = static_cast<int>(m_ListTerrain.GetItemData(Sel));
		m_Button_Property.EnableWindow(true);
	}
	else
	{
		m_Id_Terrain = 0;
		m_Button_Property.EnableWindow(true);
	}
}

void CSourceDlg::OnOk()
{
	OnOK();
}

void CSourceDlg::OnProperty()
{
	const JSRCTables & Sources = JSRCTables::GetInstance ();
	const JSource * Src  = Sources.m_TBLSources.GetItem(m_Id_Source);
	const JTerrain * Ter = Sources.m_TBLTerrains.GetItem(m_Id_Source, m_Id_Terrain);
	if(Src && Ter)
	{
		CBrowserDlg Dlg(this);
		CString txt;

		// Set the header text
		txt = Src->m_Libelle.AsJCharPtr();
		txt += " ";
		txt += Ter->m_Libelle.AsJCharPtr();
		Dlg.m_DlgHeader = txt ;


		char buffer[1024];
		// Get the current working directory: 
		if( _getcwd( buffer, 1024 ) != NULL )
		{
			// Construction chemin fichier informations enquêtes
			CString Addr;
            Addr.Format("%s%s.html", buffer, Ter->m_Chemin.AsJCharPtr());

			// Test existance ficher / évite affichage pour rien
			FILE *fp = fopen (Addr, "r");
			if (fp == NULL)
			{
				// fichier inexistant, on prévient l'utilisateur
				MessageBox("Pas de fichier informations pour le moment pour cette enquête",txt,MB_ICONINFORMATION);
			}
			else
			{
				// fichier existant, on peut l'ouvrir via le browser
				fclose(fp);

				// Affiche les infos enquête via fichier html
				Dlg.m_AddrLink = Addr;
				Dlg.DoModal();
			}
		}
	}
}
