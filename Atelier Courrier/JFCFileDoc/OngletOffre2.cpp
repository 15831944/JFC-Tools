#include <StdAfx.h>

#include "OngletOffre2.h"
#include "DlgOffre2.h"

IMPLEMENT_DYNAMIC(COngletOffre2, CDialog)

COngletOffre2::COngletOffre2(UINT nIDTemplate)
	:	CDialog(nIDTemplate, NULL)
{
	m_pDlgOffre = 0;
}

COngletOffre2::~COngletOffre2()
{
	m_pDlgOffre = 0;
}

// définit le dialogue de l'Offre 
void COngletOffre2::SetData (CDialogOffre2 * pDlgOffre2)
{
	m_pDlgOffre = pDlgOffre2;
}