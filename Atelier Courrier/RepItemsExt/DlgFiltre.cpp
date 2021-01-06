// DlgFiltre message handlers
// DlgFiltre.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFiltre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgFiltre dialog

IMPLEMENT_DYNAMIC(CDlgFiltre, CDialog)

CDlgFiltre::CDlgFiltre(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFiltre::IDD, pParent)
{
	// Récupère la couleur courante
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
}

CDlgFiltre::~CDlgFiltre()
{
}

void CDlgFiltre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDlgFiltre::OnInitDialog()
{
	CRect Rect;
	long WidthDlg, HeightDlg;
	CDialog::OnInitDialog();

	// Déclare les controles de la boite
	m_ButtonOK.SubclassDlgItem(IDOK, this);
	m_ButtonCancel.SubclassDlgItem(IDCANCEL , this);
	m_BoxFiltre.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_ListFiltre.SubclassDlgItem(IDC_RI_LIST_FILTRE, this);
	
	// Couleur Controles
	m_BoxFiltre.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_BoxFiltre.SetTransparent(false);
	m_BoxFiltre.SetSunken(true);

	// Couleur Selection Liste Filtres
	m_ListFiltre.	SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	/*
	m_ListFiltreModa.SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_ListFiltreModa.SetBkColor(CATPColors::GetColorWhite());
	m_ListFiltreModa.SetBackColor(CATPColors::GetColorWhite());
	*/

	// Ajout Elmt dans la boite
	m_BoxFiltre.AddTopWindow(m_ListFiltre);
	// m_BoxFiltre.AddTopWindow(m_ListFiltreModa);

	// Positionnement de la fenetre à droite de la boite dimension 1
	this->GetWindowRect(&Rect);
	WidthDlg  = abs(Rect.right - Rect.left);
	HeightDlg = abs(Rect.top-Rect.bottom);
	this->SetWindowPos(NULL, m_PosDlg.right + 16, m_PosDlg.top - 24, WidthDlg, HeightDlg, SWP_NOOWNERZORDER);

	/*
	m_ListFiltreModa.		InsertColumn(0, "");
	m_ListFiltreModa.		SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_ListFiltreModa.EnableToolTips(false);
	*/

	// Remplissage des elmts filtres avec sélection en cours
	m_ListFiltre.ResetContent();
	// m_ListFiltreModa.DeleteAllItems();
	CString StrVal;
	// int IdxItem = 0;
	for (m_MapCodeFiltre.MoveFirst(),  m_MapLibFiltre.MoveFirst(); m_MapCodeFiltre.IsValid(); m_MapCodeFiltre.MoveNext(),  m_MapLibFiltre.MoveNext())
	{
		// Code et sélection code secteur
		long Val = m_MapCodeFiltre.GetKey();
		bool Sel = m_MapCodeFiltre.GetItem();

		// Libellé secteur
		CString LibSecteur = m_MapLibFiltre.GetItem();

		// ajoute element (ancienne liste)
		int nR = m_ListFiltre.AddString(LibSecteur);
		m_ListFiltre.SetItemData(nR, Val);
		m_ListFiltre.SetSel(nR, Sel);
	
		/*
		int nR = m_ListFiltre.AddItem(LibSecteur);
		m_ListFiltre.SetItemData(nR, Val);
		m_ListFiltre.SetSel(nR, Sel);
		*/
		
		/*
		// Nouvelle liste
		// m_ListFiltreModa.InsertItem(IdxItem, LibSecteur);
		nR = m_ListFiltreModa.InsertItem(LVIF_PARAM|LVIF_TEXT, IdxItem, LibSecteur,0,0,0,(LPARAM)Val);
		m_ListFiltreModa.SetItemData(nR, Val);
		if (Sel)
			m_ListFiltreModa.SetSelectionMark(nR);
		*/

		// IdxItem++;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDlgFiltre, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgFiltre::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RAZ, &CDlgFiltre::OnBnClickedRaz)
END_MESSAGE_MAP()


// CDlgFiltre message handlers

void CDlgFiltre::OnBnClickedOk()
{
	// Mise à jour des sélections filtre dans le map
	for (int Idx = 0; Idx < m_ListFiltre.GetCount(); Idx++)
	{
		// Récup le code secteur
		JInt32 CodeSecteur = m_ListFiltre.GetItemData(Idx);

		// Sélectioné ou pas ??
		JBool SelFiltre = m_ListFiltre.GetSel(Idx);

		// mise à jour du map sélection
		m_MapCodeFiltre.MoveTo(CodeSecteur);
		if (m_MapCodeFiltre.IsValid())
		{
			m_MapCodeFiltre.GetItem() = SelFiltre;
		}
	}

	OnOK();
}

void CDlgFiltre::OnBnClickedRaz()
{
	// Ici on reset les sélections
	for (int Idx = 0; Idx < m_ListFiltre.GetCount(); Idx++)
	{
		// Récup le code secteur
		JInt32 CodeSecteur = m_ListFiltre.GetItemData(Idx);

		m_ListFiltre.SetSel(Idx, false);

		// mise à jour du map sélection
		m_MapCodeFiltre.MoveTo(CodeSecteur);
		if (m_MapCodeFiltre.IsValid())
		{
			m_MapCodeFiltre.GetItem() = false;
		}
	}
}
