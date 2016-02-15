// TraceFile.cpp: implementation of the CTraceFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tomeval.h"
#include "TraceFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceFile::CTraceFile()
{
	m_FileOpened=0;
}

CTraceFile::~CTraceFile()
{
	if(m_FileOpened)Close();

}

void CTraceFile::Open(LPCSTR nomfic)
{

	if(!m_File.Open(nomfic,CFile::modeCreate   | CFile::modeWrite  | CFile::typeText) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",nomfic);
		AfxMessageBox(MsgErr);
		return;
	}

	m_FileOpened=1;
}

void CTraceFile::Write(LPCSTR txt)
{
	ASSERT(m_FileOpened);
	m_File.WriteString(txt);
}

void CTraceFile::Close()
{
	ASSERT(m_FileOpened);
	m_File.Close();
	m_FileOpened=0;
}
