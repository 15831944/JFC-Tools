// FileHeader.cpp: implementation of the CFileHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <fstream>
#include "FileHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileHeader::CFileHeader()
{
	m_lCompagnie	=	0;
	m_lCompression	=	0;
	m_lCryptage		=	0;
	m_lMagicNumber	=	0;
	m_lVersion		=	0;
}

CFileHeader::~CFileHeader()
{

}

CFileHeader & CFileHeader::operator =(const CFileHeader & rhs)
{
	m_lCompagnie	=	rhs.m_lCompagnie ;
	m_lCompression	=	rhs.m_lCompression ;
	m_lCryptage		=	rhs.m_lCryptage ;
	m_lMagicNumber	=	rhs.m_lMagicNumber ;
	m_lVersion		=	rhs.m_lVersion ;

	return *this;
}

JArchive& CFileHeader::Recv( JArchive& lhs)
{
	lhs.Recv(m_lCompagnie);
	lhs.Recv(m_lMagicNumber);
	lhs.Recv(m_lCompression);
	lhs.Recv(m_lCryptage);
	lhs.Recv(m_lVersion);
	return lhs;
}

JArchive& CFileHeader::Send( JArchive& lhs)
{
	lhs.Send(m_lCompagnie);
	lhs.Send(m_lMagicNumber);
	lhs.Send(m_lCompression);
	lhs.Send(m_lCryptage);
	lhs.Send(m_lVersion);
	return lhs;
}
