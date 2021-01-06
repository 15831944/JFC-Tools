// DlgOffre2_Cibles.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "OngletOffre2_Cibles.h"
#include "DlgOffre2.h"

// Boîte de dialogue COngletOffre2_Cibles

IMPLEMENT_DYNAMIC(COngletOffre2_Cibles, COngletOffre2)

// Gestionnaires de messages de COngletOffre2_Cibles
BEGIN_MESSAGE_MAP(COngletOffre2_Cibles, COngletOffre2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

COngletOffre2_Cibles::COngletOffre2_Cibles()
	:	COngletOffre2(IDD)
{

}

COngletOffre2_Cibles::~COngletOffre2_Cibles()
{

}

void COngletOffre2_Cibles::DoDataExchange(CDataExchange* pDX)
{
	COngletOffre2::DoDataExchange(pDX);
}

BOOL COngletOffre2_Cibles::OnInitDialog()
{
	COngletOffre2::OnInitDialog();

	return TRUE;
}

HBRUSH COngletOffre2_Cibles::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_pDlgOffre->GetBackgroundBrush();
}
