#include <StdAfx.h>

#include "Offre2OngletBase.h"
#include "Offre2MainDialog.h"

IMPLEMENT_DYNAMIC(COffre2OngletBase, CDialog)

COffre2OngletBase::COffre2OngletBase(UINT nIDTemplate, COffre2MainDialog * pDialogOffre)
	:	CDialog(nIDTemplate, NULL)
	,	m_pDlgOffre(pDialogOffre)
{
}

COffre2OngletBase::~COffre2OngletBase()
{
}

void COffre2OngletBase::OnOK()
{
	// on ne fait rien pour ne pas fermer la page courante de l'onglet
}

void COffre2OngletBase::OnCancel()
{
	// on ne fait rien pour ne pas fermer la page courante de l'onglet
}
