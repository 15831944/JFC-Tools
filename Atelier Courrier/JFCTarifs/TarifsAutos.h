/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTarifsAutos
//
//	Descriptif:
//	Liste des tarifs automatisées (association clé tarif auto à une valeur tarif)
*/


#pragma once

#include "FileHeader.h"
#include "KeyTarifAuto.h"



class CTarifsAutos : public JMap<CKeyTarifAuto, JUnt32>
{
public:
	CTarifsAutos(void);
	virtual ~CTarifsAutos(void);

	/// Load les Tarifs Autos
	JBool Load(JStringEx &Name);

	JBool GetTarif( JUnt32 idSupport, JUnt32 idFormat, JUnt32 version, JUnt32 & tarif) const;

private:

	/// File Name
	JStringEx				m_sName;
	/// File Header
	CFileHeader				m_FileHeader;
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	JVoid LoadTarifsAutos(JArchive *pFile, JInt32 Version);

};
