/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CParutions
//
//	Descriptif:
//	Liste des parutions des supports (� une cl� support-date on associe une parution)
//	Poss�de des m�thode de recherche
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
