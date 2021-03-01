/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CRegroupements
//
//	Descriptif:
//	Point d'entr�e de la DLL JFCSupport
//	Premet de gerer les divers regroupements de titres (familles, centres, couplages) et les titre sp�ciaux (apparent�s, appari�s)
//	DesignPattern Singleton.
*/

#pragma once

#include <map>

#include "..\AllTypeOfNode.h"
#include "Composite.h"

#include "..\FileHeader.h"
#include "..\KeyUntUnt.h"

#include "TitresApparies.h"

class AFX_EXT_CLASS CRegroupements
{

friend class CVisiteurInsert;
friend class CVisiteurRemove;
friend class CVisiteurDelete;
friend class CVisiteurDeleteTA;

public:
	virtual ~CRegroupements(void);

	//Singleton
	static CRegroupements* GetInstance();

	JBool IsLoaded();

	JBool Load();
	JBool Save();

	///Interfaces dispos
	IElem* GetIElemByID(JUnt32 id);
	JVoid GetTitres(JList<JUnt32>& lst);
	JVoid GetTitresApparentes(JList<JUnt32>& lst);
//	JVoid GetTitresApparies(JList<JUnt32>& lst);
	JVoid GetLots(JList<JUnt32>& lst);
//	JVoid GetCouplages(JList<JUnt32>& lst);
	JVoid GetCouplagesLibres(JList<JUnt32>& lst);
//	JVoid GetCouplagesFixes(JList<JUnt32>& lst);

	JVoid GetLstTitres(JList<IElem*>& lst);

	// Correspond aux 'TITRES A CREER' du R�pertoire des Supports
	JVoid GetLstTitresApparentes(JList<IElem*>& lst);
	JVoid GetLstLots(JList<IElem*>& lst);
	JVoid GetLstCouplages(JList<IElem*>& lst);
	JVoid GetLstCouplagesLibres(JList<IElem*>& lst);
	JVoid GetLstCouplagesFixes(JList<IElem*>& lst);

	/// pour un usage plus pointu (RepSupports et interne � la dll)

	//ne renvoit que les familles centres lots et cpl
	INode* GetParentByID(JUnt32 id);

	//renvoit le premier support trouv� avec cet id
	INode* GetSupportByID(JUnt32 id);

	JNodeList* GetFamilles();
	CFamille* GetFamilleByID(JUnt32 id);
	
	JNodeList* GetCentres();
	CCentre* GetCentreByID(JUnt32 id);
	
	JNodeList* GetTitres();
	JMap<JUnt32, CTitre*>* GetTitresNA();
	CTitre* GetTitreNAByID(JUnt32 id);
	CTitre* GetTitreNAByIDAudience(JUnt32 id);

	CTitre* GetTitreByID(JUnt32 id);
	
	JNodeList* GetLots();
	CLot* GetLotByID(JUnt32 id);
	
	JNodeList* GetCouplagesLibres();
	CCouplageLibre* GetCouplageLibreByID(JUnt32 id);
	
	// Correspond aux 'TITRES A CREER' du R�pertoire des Supports
	JNodeList* GetTitresApparentes();
	CTitreApparente* GetTitreApparenteByID(JUnt32 id);

	// Correspond aux 'TITRES SANS GRP' du R�pertoire des Supports
	JVoid GetLstTitresApparies(JList<IElem*>& lst, JUnt32 idSource, JUnt32 idTerrain);

	CTitresApparies* GetTitresApparies(JUnt32 idSource, JUnt32 idTerrain);
	CTitreApparie* GetTitreApparieByID(JUnt32 id);

protected:
	JMap<JUnt32, CTitre*> m_mapFamilleNA;
	JNodeList m_lstFamille;
	JNodeList m_lstCentre;
	JNodeList m_lstTitre;
	JNodeList m_lstTitreApparente;
	JNodeList m_lstLot;
	JNodeList m_lstCouplageLibre;
	JMap <CKeyUntUnt, CTitresApparies*> m_mapTitresApparies;

private:
	CRegroupements(void);
	JBool m_bLoaded;
	JVoid EmptyAll();
	JVoid CreateFamilleNA();
	JVoid DestroyFamilleNA();

	JVoid LoadFileHeader		(JArchive *pFile);
	JVoid SaveFileHeader		(JArchive *pFile, JInt32 Version);

	static CRegroupements		m_Instance;
	CFileHeader m_FileHeader;
	JStringEx m_sName;
};
