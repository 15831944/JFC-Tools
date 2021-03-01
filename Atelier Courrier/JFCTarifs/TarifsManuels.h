/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTarifsManuels
//
//	Descriptif:
//	Liste des tarifs manuels
*/



#pragma once

#include "FileHeader.h"
#include "TarifManuel.h"



class AFX_EXT_CLASS CTarifsManuels : public JList<CTarifManuel*>
{
public:
	CTarifsManuels(void);
	virtual ~CTarifsManuels(void);

	//Pour supprimer un tarif
	JVoid RemoveTarifManuel(CTarifManuel* tarif);
	/// Load les Tarifs Manuels
	JBool Load();
	/// Save les Tarifs Manuels
	JBool Save();

	JBool GetTarifManuel( JUnt32 idSupport, JUnt32 idFormat, JDate dateIn, JUnt32 & tarif) const;

private:

	/// File Header
	CFileHeader				m_FileHeader;
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	JVoid LoadTarifsManuels(JArchive *pFile, JInt32 Version);
	/// Save the file header
	JVoid SaveFileHeader		(JArchive *pFile);
	JVoid SaveTarifsManuels(JArchive *pFile, JInt32 Version);

};
