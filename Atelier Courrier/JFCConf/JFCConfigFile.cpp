// JFCConfigFile.cpp: implementation of the JFCConfigFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFCConfigFile.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JFCConfigFile::JFCConfigFile()
{
}


JFCConfigFile::~JFCConfigFile()
{
}

bool JFCConfigFile::Import(CStdioFile& file)
{
	CString read;
	file.ReadString(read);
	if(read == "1")
		m_NameTitresApp = true;
	else
		if(read == "0")
			m_NameTitresApp = false;
		else
			return false;

	file.ReadString(read);
	m_Type_moteurDef = atoi(read);
	if(m_Type_moteurDef!=0 && m_Type_moteurDef!=1 && m_Type_moteurDef!=2)
		return false;

	return true;
}