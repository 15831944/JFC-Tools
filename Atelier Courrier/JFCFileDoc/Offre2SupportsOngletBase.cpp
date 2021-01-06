#include "stdafx.h"
#include "ATPColors.h"

// headers Offre
#include "Offre2SupportsOngletBase.h"
#include "Offre2MainDialog.h"

using namespace std;

//=============================================================================	
BEGIN_MESSAGE_MAP(COffre2SupportsOngletBase, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//=============================================================================	
// Interface COffre2SupportsOngletBase
//=============================================================================	

IMPLEMENT_DYNAMIC(COffre2SupportsOngletBase, CDialog)

COffre2SupportsOngletBase::COffre2SupportsOngletBase(UINT nIDTemplate)
	:	CDialog(nIDTemplate, 0)
	,	m_pOngletTitres(0)
	,	m_pDialogOffre(0)
{

}

COffre2SupportsOngletBase::~COffre2SupportsOngletBase()
{
}

void COffre2SupportsOngletBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void COffre2SupportsOngletBase::SetData(COffre2Supports * pOngletTitres, COffre2MainDialog * pDialogOffre)
{
	m_pOngletTitres = pOngletTitres;
	m_pDialogOffre = pDialogOffre;
	ASSERT(m_pOngletTitres);
	ASSERT(m_pDialogOffre);
}


HBRUSH COffre2SupportsOngletBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(CATPColors::GetColorLight(m_pDialogOffre->GetAppColor()));
	return hbr; 
}
void COffre2SupportsOngletBase::OnOK()
{
	// on ne fait rien pour ne pas fermer la page courante de l'onglet
}

void COffre2SupportsOngletBase::OnCancel()
{
	// on ne fait rien pour ne pas fermer la page courante de l'onglet
}
