// Courbes.cpp: implementation of the CCourbes class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Courbes.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCourbes CCourbes::m_Instance;

CCourbes::CCourbes(void)
{
}

CCourbes::~CCourbes(void)
{
	for(MoveFirst(); IsValid(); MoveNext() )
		delete GetItem();
}

void CCourbes::Save()
{
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	JStringEx sName = "/Courbes.1A";

	try
	{
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr(), true);
		SaveFileHeader(pFile);
		pFile->Send(GetCount());
		for(MoveFirst(); IsValid(); MoveNext() )
			GetItem()->Send(*pFile, m_FileHeader.GetVersion() );
		
		pFile->Flush();
		delete pFile;
		JPathManager::UpdateFile( sName.AsJCharPtr() );

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	}
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
}

CCourbes* CCourbes::GetInstance()
{
	static JBool bLoaded = false;

	if (!bLoaded)
	{

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

		JStringEx sName = "/Courbes.1A";

		try
		{
			JArchive *pFile = JArchive::Create(sName.AsJCharPtr());

			m_Instance.LoadFileHeader(pFile);
			JInt32 nb = 0;
			pFile->Recv(nb);
			for(JInt32 i = 0; i<nb; i++ )
			{
				CCourbe* courbe = new CCourbe();
				courbe->Recv(*pFile, m_Instance.m_FileHeader.GetVersion ());
				m_Instance.Add(courbe->GetId()) = courbe;
			}
			
			delete pFile;

		}
		catch(JException *pE)
		{
			pE->GetMessage();
			SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
			//return NULL;
		}
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		bLoaded = true;
	}
	
	return & m_Instance;
}

JVoid CCourbes::SaveFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	// Check compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Verify the magic number
	m_FileHeader.SetMagicNumber (0x6FD3481A);
	// No compression
	m_FileHeader.SetCompression (0);
	// No cryptage
	m_FileHeader.SetCryptage(0);
	// Must not be greater than my current version
	m_FileHeader.SetVersion(CURRENT_VERSION);

	m_FileHeader.Send(*pFile);

}

JVoid CCourbes::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3481A)
		throw JInternalError::GetInstance(); //("File Header, Magic Number field error");

	// No compression
	if(m_FileHeader.GetCompression () != 0)
		throw JInternalError::GetInstance(); //("File Header, Compression field error");

	// No cryptage
	if(m_FileHeader.GetCryptage () != 0)
		throw JInternalError::GetInstance(); //("File Header, Cryptage field error");

	// Must not be greater than my current version
	if(m_FileHeader.GetVersion () > CURRENT_VERSION)
		throw JInternalError::GetInstance(); //("File Header, Version error");
}

///////////////////////////////////////////////////////////////


double CCourbes::GetProba(int indice, int appel) const
{
	if (appel == 0)
		return 1.0;
	if(appel <= 100)
		return GetProbaByBeta(indice, appel);
	else
		return GetProbaByCourbe(indice, appel);
}

double CCourbes::GetProbaByBeta(int indice, int beta) const
{
	return (1.0 - pow( (1.0- beta/100.0 ), indice ));
}

double CCourbes::GetProbaByCourbe(int indice, int idCourbe) const
{
	double nRet = 100.0;
	MoveTo(idCourbe);
	if(IsValid())
		nRet = GetItem()->Item(indice-1) / 100.0;

	return nRet;
}

const JLabel &  CCourbes::GetLabel(int idCourbe) const
{
	MoveTo(idCourbe);
	//if(m_Courbes.IsValid())
	return GetItem()->GetLabel();
}

int CCourbes::GetNbCourbes() const
{
	return GetCount();
}