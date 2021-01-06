// DialogFormat.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DialogFormat.h"
#include <IDLimits.h>


// Boîte de dialogue CDialogFormat

IMPLEMENT_DYNAMIC(CDialogFormat, CDialog)
CDialogFormat::CDialogFormat(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFormat::IDD, pParent)
{
	m_bModif = false;
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
	m_trf = CTarifs::GetInstance();
	m_regroup = CRegroupements::GetInstance();

	m_lstBas.m_plstIDFormats = 	&m_lstIDFormats;
	m_appColor = CATPColors::COLORCREUSET;
	m_bmonosel = false;
}



CDialogFormat::~CDialogFormat()
{
}

void CDialogFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDialogFormat::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetupControls();
	FillListFormats();
	FillListSelFormats(true);


	EnableOK();
	if(m_bmonosel)
	{
		SetWindowText("Choisir le format");
		m_ttlFormat.SetText("Format");
	}
	return 	TRUE;
}

JVoid CDialogFormat::SetMonoSel(bool bMonosel /*= true*/)
{
	m_bmonosel = bMonosel;


}

JVoid CDialogFormat::SetData(JList<JUnt32> & lstIDSupport, JDate date)
{
	m_lstIDSupport.Recopie(lstIDSupport);
	m_date = date;
}

JVoid CDialogFormat::SetFormats(JList<JUnt32> & lstIDFormat)
{
	m_lstIDFormats.Recopie(lstIDFormat);
}

JVoid CDialogFormat::GetFormats(JList<JUnt32> & lstIDFormat) const
{
	lstIDFormat.Reset();
	for (m_lstIDFormats.MoveFirst(); m_lstIDFormats.IsValid(); m_lstIDFormats.MoveNext() )
        lstIDFormat.AddTail() = m_lstIDFormats.GetItem();
}


BEGIN_MESSAGE_MAP(CDialogFormat, CDialog)
	ON_LBN_SELCHANGE(IDC_OF_LIST_HAUT, OnLbnSelchangeListHaut)
	ON_LBN_SELCHANGE(IDC_OF_LIST_BAS, OnLbnSelchangeListBas)
	ON_BN_CLICKED(IDC_OF_OK, OnBnClickedOk)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogFormat
void CDialogFormat::OnBnClickedOk()
{
	CDialog::OnOK();
}

void CDialogFormat::OnLbnSelchangeListHaut()
{
	//on suppr
	JUnt32 selID = m_lstHaut.GetSelectedID();
	if (selID != LB_ERR)
	{
		m_lstIDFormats.RemoveID(selID);
		FillListSelFormats();
		m_bModif = true;
	}
	m_lstHaut.SetCurSel(LB_ERR);
	m_lstBas.Invalidate();
	EnableOK();
}

void CDialogFormat::OnLbnSelchangeListBas()
{
	//on remonte
	//JUnt32 selID = m_lstBas.GetSelectedID();
	JList<JInt32>  selIDLst;
	m_lstBas.GetSelectedID(selIDLst);
	m_lstIDFormats.Reset();
	for(selIDLst.MoveFirst();selIDLst.IsValid();selIDLst.MoveNext())
	{
		JUnt32 selID = selIDLst.GetItem();
		if (selID != LB_ERR)
		{
			if(m_bmonosel)
				m_lstIDFormats.Reset();
			// Si sélection non existante, on l'ajoute
			m_lstIDFormats.AddTail() = selID;

			m_bModif = true;
		}
	}
	// Mise à jour de la liste sélection en haut
	FillListSelFormats();

	EnableOK();
}

void CDialogFormat::EnableOK()
{
	m_btnOK.EnableWindow(m_lstHaut.GetCount()!=0);
}

void CDialogFormat::FillListFormats()
{
	m_lstBas.ResetContent();
	bool bAdd = false;
	JFormat* frm;
	JUnt32 idFrm;
	//Les formats
	for( m_srcPresse->m_TBLFormats.MoveFirst(); m_srcPresse->m_TBLFormats.IsValid(); m_srcPresse->m_TBLFormats.MoveNext() )
	{
		frm = (JFormat*)m_srcPresse->m_TBLFormats.GetItem();
		idFrm = frm->GetID();
		bAdd = false;
		m_lstBas.AddItem( m_srcPresse->m_TBLFormats.GetItem() );
	}

}
void CDialogFormat::FillListSelFormats(JBool Select)
{
	// Liste des idents formats sélectionnés
	JList <JInt32> selIdlst;

	// Réinit la liste sélection en haut
	m_lstHaut.ResetContent();

	// Les formats
	for( m_lstIDFormats.MoveFirst(); m_lstIDFormats.IsValid(); m_lstIDFormats.MoveNext() )
	{
		// Ajout élmt dans liste sélection en haut
		JUnt32 IdElmt = m_lstIDFormats.GetItem();
		if(m_srcPresse->m_TBLFormats.GetElemByID(IdElmt))
		{
			m_lstHaut.AddItem( m_srcPresse->m_TBLFormats.GetElemByID(IdElmt));

			// Ajout à la liste sélection
			selIdlst.AddTail() = IdElmt;
		}
	}

	// Mise à jour sélection en bas
	// m_lstBas.SetSelectedID(selIdlst); 
	if (Select)
		m_lstBas.SetSelectedID(selIdlst); 

	/*
			// Valide sélection dans liste du bas
		for (int i=0; i<m_lstBas.GetCount(); i++)
		{
			// Récup élmt format
			JUnt32 IdList = m_lstBas.SetSelectedID( .GetSelectedID( .GetSelectedID();

			// Marque ou pas l'élmt
			if (IdElmt == frm->GetID())
				m_lstBas.SetSel(i, TRUE);
			else
				m_lstBas.SetSel(i, FALSE);
		}

	*/

	
}

void CDialogFormat::SetupControls()
{
//subclass 

	m_bkgFormat.SubclassDlgItem(IDC_OF_STATIC_FORMAT, this);
	m_ttlFormat.SubclassDlgItem(IDC_OF_STATIC_TTLFORMAT, this);
	m_lstHaut.SubclassDlgItem(IDC_OF_LIST_HAUT, this);
	m_lstBas.SubclassDlgItem(IDC_OF_LIST_BAS, this);
	m_btnOK.SubclassDlgItem(IDC_OF_OK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

////
	m_bkgFormat.SetTransparent(false);
	m_bkgFormat.SetSunken(true);
	m_bkgFormat.AddTopWindow(m_ttlFormat);
	m_bkgFormat.AddTopWindow(m_lstHaut);
	m_bkgFormat.AddTopWindow(m_lstBas);

////
	m_ttlFormat.SetFontName(CATPColors::GetFontLabelName());
	m_ttlFormat.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlFormat.SetBorder(false);

	SetColors();
}

void CDialogFormat::SetColors(CATPColors::APPCOLOR app)
{
	m_appColor = app;
}

void CDialogFormat::SetColors()
{
	m_bkgFormat.SetBkColor(CATPColors::GetColorMedium(m_appColor), CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	
	m_ttlFormat.SetBkColor(CATPColors::GetColorDark(m_appColor), m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlFormat.SetTextColor(CATPColors::GetColorSuperDark(m_appColor));

   	m_lstHaut.SetSelectionColor( NORMAL, CATPColors::GetColorWhite());
	m_lstBas.SetSelectionColor( NORMAL, CATPColors::GetColorSelect(m_appColor));

	m_lstHaut.SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), m_appColor, 0 );
	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), m_appColor, 0 );
}


///////////////////////////////////////////////////////////////
//	class CFormatAttribLB
//////////////////////////////////////////////////////////////
CFormatAttribLB::CFormatAttribLB()
{
	// m_ColorAttribue = CATPColors::GetColorRed();
}

CFormatAttribLB::~CFormatAttribLB()
{
}

bool CFormatAttribLB::IsAttribue(int ID) const
{
	if (m_plstIDFormats->IDExist(ID))
		return true;
	return false;
}
