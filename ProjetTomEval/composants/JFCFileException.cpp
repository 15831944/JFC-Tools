// JFCFileException.cpp: implementation of the CJFCFileException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFCFileException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D503A4
CJFCFileException::CJFCFileException()
{

}

//##ModelId=3D9AB7D503A5
CJFCFileException::~CJFCFileException()
{

}

//##ModelId=3D9AB7D503A7
void CJFCFileException::AfficheErreur(CString msg)
{
	CString str;
	switch(m_cause)
	{
	case CFileException::none:
		m_strerr="No error occurred.";
		break;
	case CFileException::generic:
		m_strerr="An unspecified error occurred.";
		break;
	case CFileException::fileNotFound:
		m_strerr="The file could not be located.";
		break;
	case CFileException::badPath:
		m_strerr="All or part of the path is invalid.";
		break;
	case CFileException::tooManyOpenFiles:
		m_strerr="The permitted number of open files was exceeded.";
		break;
	case CFileException::accessDenied:
		m_strerr="The file could not be accessed.";
		break;
	case CFileException::invalidFile:
		m_strerr="There was an attempt to use an invalid file handle.";
		break;
	case CFileException::removeCurrentDir:
		m_strerr="The current working directory cannot be removed.";
		break;
	case CFileException::directoryFull:
		m_strerr="There are no more directory entries.";
		break;
	case CFileException::badSeek:
		m_strerr="There was an error trying to set the file pointer.";
		break;
	case CFileException::hardIO:
		m_strerr="There was a hardware error.";
		break;
	case CFileException::sharingViolation:
		m_strerr="SHARE.EXE was not loaded, or a shared region was locked.";
		break;
	case CFileException::lockViolation:
		m_strerr="There was an attempt to lock a region that was already locked.";
		break;
	case CFileException::diskFull:
		m_strerr="The disk is full.";
		break;
	case CFileException::endOfFile:
		m_strerr="The end of file was reached.";
		break;
	}
	if(str.IsEmpty())	str="File exception error";
	str=msg+"\n"+m_strerr;
	AfxMessageBox(str);
	return;
}
