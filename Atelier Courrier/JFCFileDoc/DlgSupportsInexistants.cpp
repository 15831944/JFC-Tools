// DlgSupportsInexistants.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgSupportsInexistants.h"


// Boîte de dialogue DlgSupportsInexistants

IMPLEMENT_DYNAMIC(DlgSupportsInexistants, CDialog)

DlgSupportsInexistants::DlgSupportsInexistants(const CString & sUserMsg, const CString & sListSupports)
	: CDialog(DlgSupportsInexistants::IDD, 0)
	, m_sUserText(sUserMsg)
	, m_sListSupports(sListSupports)
{

}

DlgSupportsInexistants::~DlgSupportsInexistants()
{
}

void DlgSupportsInexistants::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPPORTS, m_TextSupports);
	DDX_Control(pDX, IDC_USERTEXT, m_Static);
}


BEGIN_MESSAGE_MAP(DlgSupportsInexistants, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages de DlgSupportsInexistants

BOOL DlgSupportsInexistants::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TextSupports.SetWindowText(m_sListSupports);

	m_Static.SetWindowText(m_sUserText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}
