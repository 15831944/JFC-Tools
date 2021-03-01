// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPVersion.h"
#include ".\atpversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPVersion::CATPVersion(CWnd* pParent) : CDialog(CATPVersion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATPVersion)
	//}}AFX_DATA_INIT
}

void CATPVersion::DoDataExchange(CDataExchange* pDX)
{
	// on appelle le gestionnaire de base
	this->CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CATPVersion)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPVersion, CDialog)
	//{{AFX_MSG_MAP(CATPVersion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CATPVersion::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_OK.SubclassDlgItem(IDOK, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}
