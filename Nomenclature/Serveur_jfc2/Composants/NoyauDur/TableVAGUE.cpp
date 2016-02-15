// TableVAGUE.cpp: implementation of the CTableVAGUE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableVAGUE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableVAGUE::CTableVAGUE()
{

}

CTableVAGUE::~CTableVAGUE()
{

}

CTableVAGUE::CTableVAGUE(const CTableVAGUE &Data)
{
	*this=Data;
}

CTableVAGUE & CTableVAGUE::operator=(const CTableVAGUE &Data)
{
	m_Num=Data.m_Num;
	m_SourceT=Data.m_SourceT;
	m_NbInd=Data.m_NbInd;
	m_NbIndS=Data.m_NbIndS;
	m_Nom=Data.m_Nom;
	m_Titre=Data.m_Titre;
	m_MaxCriteres=Data.m_MaxCriteres;
	m_Occupe=Data.m_Occupe;
	return *this;
}

