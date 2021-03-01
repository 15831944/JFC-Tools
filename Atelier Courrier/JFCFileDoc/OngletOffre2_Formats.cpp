// DlgOffre2_Formats.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"

#include "OngletOffre2_Formats.h"
#include "DlgOffre2.h"


// Bo�te de dialogue COngletOffre2_Formats

IMPLEMENT_DYNAMIC(COngletOffre2_Formats, COngletOffre2)

// Gestionnaires de messages de COngletOffre2_Formats

BEGIN_MESSAGE_MAP(COngletOffre2_Formats, COngletOffre2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


COngletOffre2_Formats::COngletOffre2_Formats()
	:	COngletOffre2(IDD)
{

}

COngletOffre2_Formats::~COngletOffre2_Formats()
{
}

void COngletOffre2_Formats::DoDataExchange(CDataExchange* pDX)
{
	COngletOffre2::DoDataExchange(pDX);
}


HBRUSH COngletOffre2_Formats::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_pDlgOffre->GetBackgroundBrush();
}
