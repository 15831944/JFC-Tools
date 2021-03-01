/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTitreApparie
//
//	Descriptif:
//	Titre apparié (titre sans GRP) possède un médiaweight et un support tuteur dont il prend l'ID d'audiance
//	DesignPattern Classe composite Classe acceptant un visiteur.
*/

#pragma once

#include "IElem.h"
#include "ITitre.h"

class AFX_EXT_CLASS CTitreApparie : public IElem , public ITitre
{
public:
	CTitreApparie(void);
	CTitreApparie( JUnt32 id, JUnt32 idTitre, JUnt32 idCentre, JUnt32 idTitreTemoin, JUnt32 mw);
	virtual ~CTitreApparie(void);
	
	virtual JBool IsTitre()  const { return true; };
	virtual JBool IsTitreApparie()  const { return true; };
	virtual ITitre * AsITitre() { return this;};
	virtual CTitre * AsTitre() { return  (CTitre*)this;};
	virtual CTitreApparie * AsTitreApparie() { return this;};

	JArchive&	Recv( JArchive& lhs, JInt32 Version );
	JArchive&	Send( JArchive& lhs, JInt32 Version ) const;

//	JVoid SetLabel(const JChar* lbl);

	JUnt32 GetIDCentre() const  { return m_IDCentre; } ;

	JUnt32 GetTitreTemoin() const  { return m_IDTitreTemoin; } ;
	
	JUnt32 GetIDTitre() const;

	JUnt32 GetMediaWeight() const  { return m_MediaWeight; } ;

	JUnt32	GetSourceID() const {return m_sourceID;};
	JUnt32	GetTerrainID() const {return m_terrainID;};
	JVoid	SetSourceID(JUnt32 sourceID) {m_sourceID = sourceID;};
	JVoid	SetTerrainID(JUnt32 terrainID) {m_terrainID = terrainID;};

private:

	/// ID du centre d'intérêt
	JUnt32				m_IDCentre;
	/// ID du support
	JUnt32				m_IDTitre;
	/// ID du support témoin
	JUnt32				m_IDTitreTemoin;
	/// MediaWeight
	JUnt32				m_MediaWeight;
	/// Source ID
	JUnt32					m_sourceID;
	/// Terrain ID
	JUnt32					m_terrainID;

private:

	/// assignment operator
	CTitreApparie & operator =(const CTitreApparie & rhs);
	/// copy constructor
	CTitreApparie(const CTitreApparie & rhs);
};
