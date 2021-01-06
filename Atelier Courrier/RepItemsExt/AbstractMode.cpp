// AbstractMode.cpp: implementation of the CAbstractMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbstractMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbstractMode::CAbstractMode(CDlgItems *pDlgItems)
{
	m_pDlgItem	= pDlgItems;
}

CAbstractMode::~CAbstractMode()
{

}

