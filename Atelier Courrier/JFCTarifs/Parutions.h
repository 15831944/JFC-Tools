/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CParutions
//
//	Descriptif:
//	Liste des parutions des supports (à une clé support-date on associe une parution)
//	Possède des méthode de recherche
*/

#pragma once

#include "FileHeader.h"
#include "Parution.h"
#include "KeySupportDate.h"


class CParutions : public JMap<CKeySupportDate, CParution>
{
public:
	CParutions(void);
	virtual ~CParutions(void);

	/// Load les Parutions
	JBool Load(JStringEx &Name);

	JBool FindDateExact(JUnt32 idSupport, JDate dateIn, JDate &dateOut, JUnt32 &duree, JUnt32 &version, JInt32 sens ) const;

	JBool FindNumero(JUnt32 idSupport, JDate dateIn, JUnt32& numero) const;

private:

	/// File Name
	JStringEx				m_sName;
	/// File Header
	CFileHeader				m_FileHeader;
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	JVoid LoadParutions(JArchive *pFile, JInt32 Version);

};
