// DlgOffre2_Periode.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "OngletOffre2_Periode.h"
#include "DlgOffre2.h"

// Boîte de dialogue COngletOffre2_Periode

IMPLEMENT_DYNAMIC(COngletOffre2_Periode, COngletOffre2)

// Gestionnaires de messages de COngletOffre2_Periode

BEGIN_MESSAGE_MAP(COngletOffre2_Periode, COngletOffre2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

COngletOffre2_Periode::COngletOffre2_Periode()
	:	COngletOffre2(IDD)
{

}

COngletOffre2_Periode::~COngletOffre2_Periode()
{
}

void COngletOffre2_Periode::DoDataExchange(CDataExchange* pDX)
{
	COngletOffre2::DoDataExchange(pDX);
}


HBRUSH COngletOffre2_Periode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_pDlgOffre->GetBackgroundBrush();
}
