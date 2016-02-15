// FicProba.cpp: implementation of the CFicProba class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "probasupport.h"
#include "FicProba.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D503E2
CFicProba::CFicProba()
{
	m_NbUser=0;
}

//##ModelId=3D9AB7D60002
CFicProba::~CFicProba()
{

}

//##ModelId=3D9AB7D503E1
void CFicProba::AddOneUser()
{
	m_NbUser++;
}

//##ModelId=3D9AB7D503E0
void CFicProba::SupressOneUser()
{
	m_NbUser--;
	ASSERT(m_NbUser>=0);

}


//##ModelId=3D9AB7D503D8
int CFicProba::GetNbUser()
{
	return(m_NbUser);

}
