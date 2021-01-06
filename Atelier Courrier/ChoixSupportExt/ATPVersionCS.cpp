//
// Fichier: ATPVersionCS.cpp
// Date:    09/04/2004
// Gestion Information Version Choix Supports

#include "stdafx.h"
#include "ATPVersionCS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// Boîte de dialogue CATPVersionCS

IMPLEMENT_DYNAMIC(CATPVersionCS, CDialog)
CATPVersionCS::CATPVersionCS(CWnd* pParent /*=NULL*/)
	: CDialog(CATPVersionCS::IDD, pParent)
{
}

CATPVersionCS::~CATPVersionCS()
{
}

void CATPVersionCS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CATPVersionCS, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CATPVersionCS
