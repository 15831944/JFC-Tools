/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CVersionsTarifsAutos
//
//	Descriptif:
//	Liste des version de tarifs
//	Association entre une cl� Support-date et un num�ro de version
*/


#pragma once

#include "FileHeader.h"
#include "KeySupportDate.h"



class CVersionsTarifsAutos : public JMap<CKeySupportDate, JUnt32>
{
public:
	CVersionsTarifsAutos(void);
	virtual ~CVersionsTarifsAutos(void);

	/// Load les Versions des TarifsAutos
	JBool Load(JStringEx &Name);

	JBool FindVersion( JUnt32 idSupport, JDate date, JUnt32& version ) const;
private:

	/// File Name
	JStringEx				m_sName;
	/// File Header
	CFileHeader				m_FileHeader;
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	JVoid LoadVersionsTarifsAutos(JArchive *pFile, JInt32 Version);

};