/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTitresApparies
//
//	Descriptif:
//	Regroupements des titres appariés pour une source et un terrain donné
*/

#pragma once

#include "..\FileHeader.h"
#include "TitreApparie.h"


class AFX_EXT_CLASS CTitresApparies : public JMap<JInt32, CTitreApparie*>
{
public:
	CTitresApparies(void);
	virtual ~CTitresApparies(void);
	/// Load les TitresApparies
	JBool Load(JStringEx &Name);
	/// Save les TitresApparies
	JBool Save();

	JVoid Add(JInt32 id, CTitreApparie* titreapp, JBool bMove = false);

	JUnt32	GetSourceID() const {return m_lSourceID;};
	JUnt32	GetTerrainID() const {return m_lTerrainID;};
	JVoid	SetSourceID(JUnt32 sourceID) {m_lSourceID = sourceID;};
	JVoid	SetTerrainID(JUnt32 terrainID) {m_lTerrainID = terrainID;};

private:

	/// File Name
	JStringEx				m_sName;
	/// File Header
	CFileHeader				m_FileHeader;
	/// Source ID
	JUnt32					m_lSourceID;
	/// Terrain ID
	JUnt32					m_lTerrainID;


private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the TitresApparies
	JVoid LoadTitresApparies	(JArchive *pFile, JInt32 Version);

	/// Save the file header
	JVoid SaveFileHeader		(JArchive *pFile);
	/// Save the enquete header
	JVoid SaveHeader			(JArchive *pFile, JInt32 Version);
	/// Save the TitresApparies
	JVoid SaveTitresApparies	(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CTitresApparies & operator =(const CTitresApparies & rhs);
	/// copy constructor
	CTitresApparies(const CTitresApparies & rhs);

};
