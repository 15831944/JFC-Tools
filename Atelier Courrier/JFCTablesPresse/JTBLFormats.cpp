// JTBLFormats.cpp: implementation of the JTBLFormats class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLFormats.h"
#include "..\JFCConf\JFCConfig.h"
#include "..\JFCSupports\IDLimits.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


JTBLFormats::JTBLFormats()
{
}


JTBLFormats::~JTBLFormats()
{
}


JChar* JTBLFormats::GetLabelCourtByID(JUnt32 id) const
{
	JChar * ret;
	MoveTo(id);
	if (IsValid())
		ret = (JChar*)((JFormat*)GetItem())->GetLabelCourt().AsJCharPtr();
	else
		ret = "TBL Format:GetLabelCourtByID ID inconnu";

	return ret;
}

JVoid JTBLFormats::AddFormatSpe(JUnt32 id, const JChar* lblCourt, const JChar* lblLong)
{
	MoveTo(id);
	//Si l'elem n'existe pas on l'ajoute sinon on le met à jour
	if ( !IsValid() )
		Add(id, true) = new JFormatSpe(id, lblLong, lblCourt);
	else
	{
		((JFormatSpe*)GetItem())->SetLabel(lblLong);
		((JFormatSpe*)GetItem())->SetLabelCourt(lblCourt);
	}

}

JVoid JTBLFormats::RemFormatSpe(JUnt32 id)
{
	MoveTo(id);
	if( IsValid() )
	{
		delete GetItem();
		Remove();
	}
}

JVoid JTBLFormats::RemAllFormatSpe()
{
	MoveLast();
	while( IsValid() )
	{
		if ( GetKey() > IDFORMATSPE_MIN
			&& GetKey() < IDFORMATSPE_MAX )
		{
			delete GetItem();
			Remove(-1);
		}
		else
			MovePrev();
	}
}


JInt32 JTBLFormats::GetNbFormatSpe()
{
	JInt32 nRet = 0;
	for (MoveFirst(); IsValid(); MoveNext())
	{
		if ( GetKey() > IDFORMATSPE_MIN
			&& GetKey() < IDFORMATSPE_MAX )
			nRet++;
	}
	return nRet;
}

JVoid JTBLFormats::Import(CStdioFile& file)
{
	Empty();
	CString str;
	while (file.ReadString(str))
	{
		//ligne vide ou commentaire
		str.Trim();
		if(str.Find("//") == 0 || str.IsEmpty())
			continue;

		CString resToken;
		int curPos= 0;

		// l'identifiant terrain
		resToken= str.Tokenize(",",curPos);
		JUnt32 id = atoi(resToken);
		
		// le libellé long
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		CString lbl = resToken;

		// le libellé court
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		CString lblcourt = resToken;

		// on crée un élément
		JFormat* format = new JFormat( id, lbl, lblcourt );
		// on ajoute l'élément
		Add( format->GetID() ) = format;
	}
	LoadFormatSpe();

}

JBool JTBLFormats::LoadFormatSpe()
{
	RemAllFormatSpe();

	//SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));
	SetCurrentDirectory(JFCConfig::GetInstance().GetUserPath());

	JString sName = "/FormatsSpe.1D";
	try
	{
		JInt32 nCount, i, version;
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		version = m_FileHeader.GetVersion();

		switch( version )
		{
		case 1:
			pFile->Recv(nCount);
			JFormatSpe* formatspe;
			for( i = 0; i < nCount; i++ )
			{
				formatspe = new JFormatSpe();
				formatspe->Recv(*pFile, version);
				Add( formatspe->GetID() ) = formatspe;
			}
			break;

		default:
			throw JInternalError::GetInstance(); //("File Version error");
		}
		delete pFile;
	}
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		return false;
	}
	
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return true;
}

JBool JTBLFormats::SaveFormatSpe()
{
	
	JStringEx sName;

	//SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));
	SetCurrentDirectory(JFCConfig::GetInstance().GetUserPath());

	sName.Append ("/FormatsSpe.1D");

	const JInt32 CURRENT_VERSION = 1;

	try
	{
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr(), true);

		SaveFileHeader(pFile, CURRENT_VERSION);

		//Save
		pFile->Send( GetNbFormatSpe() );
		for( MoveFirst(); IsValid(); MoveNext() )
			((JFormat*)GetItem())->Send(*pFile, CURRENT_VERSION);
		pFile->Flush();

		delete pFile;
		JPathManager::UpdateFile( sName.AsJCharPtr() );

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		return false;
	}
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return true;
}

JVoid JTBLFormats::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3481D)
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



JVoid JTBLFormats::SaveFileHeader(JArchive *pFile, JInt32 Version)
{
	// Compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Magic number
	m_FileHeader.SetMagicNumber (0x6FD3481D);
	// Compression
	m_FileHeader.SetCompression (0);
	// Cryptage
	m_FileHeader.SetCryptage(0);
	// Current version
	m_FileHeader.SetVersion(Version);

	m_FileHeader.Send(*pFile);

}