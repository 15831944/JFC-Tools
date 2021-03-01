#include "stdafx.h"
#include ".\regroupements.h"
#include "..\AllTypeOfVisitor.h"
#include "JSRCPresse.h"
#include "JSRCTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRegroupements CRegroupements::m_Instance;

CRegroupements::CRegroupements(void)
{
}

CRegroupements::~CRegroupements(void)
{
	EmptyAll();
	for (m_mapTitresApparies.MoveFirst(); m_mapTitresApparies.IsValid(); m_mapTitresApparies.MoveNext())
		delete m_mapTitresApparies.GetItem();
}

JVoid CRegroupements::EmptyAll()
{
	if (m_bLoaded)
	{
		m_lstFamille.Empty();
		// les listes centre et titres sont déjà vidées avec lst famille
		//Méthode spéciale pour les titres apparentés
		m_lstTitreApparente.MoveLast();
		while ( m_lstTitreApparente.IsValid() )
		{
			delete m_lstTitreApparente.GetItem();
			m_lstTitreApparente.Remove(-1);
		}
		
		m_lstLot.Empty();
		m_lstCouplageLibre.Empty();
		m_bLoaded = false;
	}
	DestroyFamilleNA();
}


CRegroupements * CRegroupements::GetInstance()
{
	static JBool bLoaded = false;

	if (!bLoaded)
	{
		bLoaded = true;
		m_Instance.Load();
	}
	return & m_Instance;
}

JBool CRegroupements::IsLoaded()
{
	return m_bLoaded;
}

JBool CRegroupements::Load()
{

	m_bLoaded = false;
	//On vide la liste avant de lire un fichier
//	EmptyAll();
	CreateFamilleNA();

	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	m_sName.Reset();
	m_sName.Append("/RegTitres");
	m_sName.Append (".16");

	try
	{
		JInt32 nCount, i, version;
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		version = m_FileHeader.GetVersion();

		CVisiteurInsert vInsert;
		CFamille* familletmp;
		CCentre* centretmp;
		CTitre* titretmp;
		CTitreApparente* titresapptmp;
		CLot* lottmp;
		CCouplageLibre* couplagetmp;

		switch( version )
		{
		case 1:
			//LoadFamilles
			pFile->Recv(nCount);
			for( i = 0; i < nCount; i++ )
			{
				familletmp = new CFamille();
				familletmp->Recv(*pFile, version);
				familletmp->Accept(&vInsert);
			}
			//LoadCentres
			pFile->Recv(nCount);
			for( i = 0; i < nCount; i++ )
			{
				centretmp = new CCentre();
				centretmp->Recv(*pFile, version);
				centretmp->Accept(&vInsert);
			}

			//LoadTitres
			pFile->Recv(nCount);
			for( i = 0; i < nCount; i++ )
			{
				titretmp = new CTitre();
				titretmp->Recv(*pFile, version);

				if(titretmp->GetIDAudience() == 0)
					delete titretmp;
				else
					titretmp->Accept(&vInsert);
			}
			//LoadTitresApp
			pFile->Recv(nCount);
			for( i = 0; i < nCount; i++ )
			{
				titresapptmp = new CTitreApparente();
				titresapptmp->Recv(*pFile, version);
				if(titresapptmp->GetIDAudience() == 0)
					delete titresapptmp;
				else
					titresapptmp->Accept(&vInsert);
			}
			//LoadLots
			pFile->Recv(nCount);
			for( i = 0; i < nCount; i++ )
			{
				lottmp = new CLot();
				lottmp->Recv(*pFile, version);
				lottmp->Accept(&vInsert);
			}
			//LoadCouplagesL
			pFile->Recv(nCount);
			for( i = 0; i < nCount; i++ )
			{
				couplagetmp = new CCouplageLibre();
				couplagetmp->Recv(*pFile, version);
				couplagetmp->Accept(&vInsert);
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
		m_bLoaded = false;
		return m_bLoaded;
	}
	m_bLoaded = true;

	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return m_bLoaded;
}

JVoid CRegroupements::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35916)
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



JBool CRegroupements::Save()
{
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	m_sName.Reset();
	m_sName.Append("/RegTitres");
	m_sName.Append (".16");

	const JInt32 CURRENT_VERSION = 1;

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr(), true);

		SaveFileHeader(pFile, CURRENT_VERSION);

		//SaveFamilles
		pFile->Send( m_lstFamille.GetCount() );
		for( m_lstFamille.MoveFirst(); m_lstFamille.IsValid(); m_lstFamille.MoveNext() )
			m_lstFamille.GetItem()->Send(*pFile, CURRENT_VERSION);

		//SaveCentres
		pFile->Send( m_lstCentre.GetCount() );
		for( m_lstCentre.MoveFirst(); m_lstCentre.IsValid(); m_lstCentre.MoveNext() )
			m_lstCentre.GetItem()->Send(*pFile, CURRENT_VERSION);

		//SaveTitres
		pFile->Send( m_lstTitre.GetCount() );
		for( m_lstTitre.MoveFirst(); m_lstTitre.IsValid(); m_lstTitre.MoveNext() )
			m_lstTitre.GetItem()->Send(*pFile, CURRENT_VERSION);
	
		//SaveTitresApp
		pFile->Send( m_lstTitreApparente.GetCount() );
		for( m_lstTitreApparente.MoveFirst(); m_lstTitreApparente.IsValid(); m_lstTitreApparente.MoveNext() )
			m_lstTitreApparente.GetItem()->Send(*pFile, CURRENT_VERSION);

		//SaveLots
		pFile->Send( m_lstLot.GetCount() );
		for( m_lstLot.MoveFirst(); m_lstLot.IsValid(); m_lstLot.MoveNext() )
			m_lstLot.GetItem()->Send(*pFile, CURRENT_VERSION);

		//SaveCouplagesL
		pFile->Send( m_lstCouplageLibre.GetCount() );
		for( m_lstCouplageLibre.MoveFirst(); m_lstCouplageLibre.IsValid(); m_lstCouplageLibre.MoveNext() )
			m_lstCouplageLibre.GetItem()->Send(*pFile, CURRENT_VERSION);

		pFile->Flush();

		delete pFile;
		JPathManager::UpdateFile( m_sName.AsJCharPtr() );

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

JVoid CRegroupements::SaveFileHeader(JArchive *pFile, JInt32 Version)
{
	// Compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Magic number
	m_FileHeader.SetMagicNumber (0x6FD35916);
	// Compression
	m_FileHeader.SetCompression (0);
	// Cryptage
	m_FileHeader.SetCryptage(0);
	// Current version
	m_FileHeader.SetVersion(Version);

	m_FileHeader.Send(*pFile);

}

INode* CRegroupements::GetParentByID(JUnt32 id)
{
	INode* noderet = NULL;

	noderet = m_lstFamille.GetNodeByID(id);
	if (noderet)
		return noderet;
	noderet = m_lstCentre.GetNodeByID(id);
	if (noderet)
		return noderet;
	//noderet = m_lstTitre.GetNodeByID(id);
	//if (noderet)
	//	return noderet;
	//noderet = m_lstTitreApparente.GetNodeByID(id);
	//if (noderet)
	//	return noderet;
	noderet = m_lstLot.GetNodeByID(id);
	if (noderet)
		return noderet;
	noderet = m_lstCouplageLibre.GetNodeByID(id);
	if (noderet)
		return noderet;

	return NULL;
}

IElem* CRegroupements::GetIElemByID(JUnt32 id)
{
	IElem* elemRet = NULL;

	elemRet = m_lstFamille.GetNodeByID(id);
	if (elemRet)
		return elemRet;

	elemRet = m_lstCentre.GetNodeByID(id);
	if (elemRet)
		return elemRet;

	elemRet = m_lstTitreApparente.GetNodeByID(id);
	if (elemRet)
		return elemRet;
	//elemRet = m_lstLot.GetNodeByID(id);
	//if (elemRet)
	//	return elemRet;
	elemRet = m_lstCouplageLibre.GetNodeByID(id);
	if (elemRet)
		return elemRet;

	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();

	elemRet =  srcpresse->m_TBLTitres.GetElemByID(id);
	if (elemRet)
		return elemRet;

	//elemRet =  srcpresse->m_TBLCouplagesFixes.GetElemByID(id);
	//if (elemRet)
	//	return elemRet;
	//
	//for (srcpresse->m_TBLCouplagesFixes.MoveFirst(); srcpresse->m_TBLCouplagesFixes.IsValid(); srcpresse->m_TBLCouplagesFixes.MoveNext() )
	//{
	//	JCouplageFixe * jcplf = (JCouplageFixe*)srcpresse->m_TBLCouplagesFixes.GetItem();
	//	IElem* titre =  jcplf->GetFilsByID(id);
	//	if (titre)
	//		return titre;
	//}

	elemRet =  GetTitreApparieByID(id);
	if (elemRet)
		return elemRet;


	return NULL;
}

INode* CRegroupements::GetSupportByID(JUnt32 id)
{
	INode* noderet = NULL;

	noderet = m_lstTitre.GetNodeByID(id);
	if (noderet)
		return noderet;
	noderet = m_lstTitreApparente.GetNodeByID(id);
	if (noderet)
		return noderet;

	m_mapFamilleNA.MoveTo(id);
	if( m_mapFamilleNA.IsValid() )
		return m_mapFamilleNA.GetItem();

	return NULL;
}

JNodeList* CRegroupements::GetFamilles()
{
	return &m_lstFamille;
}
CFamille* CRegroupements::GetFamilleByID(JUnt32 id)
{
	return (CFamille*)m_lstFamille.GetNodeByID(id);
}

JNodeList* CRegroupements::GetCentres()
{
	return &m_lstCentre;
}
CCentre* CRegroupements::GetCentreByID(JUnt32 id)
{
	return (CCentre*)m_lstCentre.GetNodeByID(id);
}

JNodeList* CRegroupements::GetTitres()
{
	return &m_lstTitre;
}
CTitre* CRegroupements::GetTitreByID(JUnt32 id)
{
	return (CTitre*)m_lstTitre.GetNodeByID(id);
}

JNodeList* CRegroupements::GetLots()
{
	return &m_lstLot;
}
CLot* CRegroupements::GetLotByID(JUnt32 id)
{
//	return (CLot*)m_lstLot.GetNodeByID(id);
	CLot * node  = NULL;

	node = (CLot*)m_lstFamille.GetNodeByID(id);
	if (!node)
		node = (CLot*)m_lstCentre.GetNodeByID(id);

	return node;

}

JNodeList* CRegroupements::GetCouplagesLibres()
{
	return &m_lstCouplageLibre;
}
CCouplageLibre* CRegroupements::GetCouplageLibreByID(JUnt32 id)
{
	return (CCouplageLibre*)m_lstCouplageLibre.GetNodeByID(id);
}

JNodeList* CRegroupements::GetTitresApparentes()
{
	return &m_lstTitreApparente;
}
CTitreApparente* CRegroupements::GetTitreApparenteByID(JUnt32 id)
{
	return (CTitreApparente*)m_lstTitreApparente.GetNodeByID(id);
}

JMap<JUnt32, CTitre*>* CRegroupements::GetTitresNA()
{
	return &m_mapFamilleNA;
}

CTitre* CRegroupements::GetTitreNAByID(JUnt32 id)
{
	m_mapFamilleNA.MoveTo(id);
	if (m_mapFamilleNA.IsValid())
		return m_mapFamilleNA.GetItem();
	return NULL;
}
CTitre* CRegroupements::GetTitreNAByIDAudience(JUnt32 id)
{
	for(m_mapFamilleNA.MoveFirst(); m_mapFamilleNA.IsValid(); m_mapFamilleNA.MoveNext())
	{
		if (m_mapFamilleNA.GetItem()->GetIDAudience() == id)
			return m_mapFamilleNA.GetItem();
	}
	return NULL;
}

JVoid CRegroupements::CreateFamilleNA()
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	for (srcpresse->m_TBLTitres.MoveFirst(); srcpresse->m_TBLTitres.IsValid(); srcpresse->m_TBLTitres.MoveNext() )
	{
		JTitre * jtitre = (JTitre*)srcpresse->m_TBLTitres.GetItem();
		if(!jtitre->IsAgregat())
		{
			CTitre* titre = new CTitre(jtitre->GetID(), jtitre->GetLabel().AsJCharPtr(), 0);
			m_mapFamilleNA.Add(titre->GetID()) = titre;
		}
	}
}

JVoid CRegroupements::DestroyFamilleNA()
{
	for (m_mapFamilleNA.MoveFirst(); m_mapFamilleNA.IsValid(); m_mapFamilleNA.MoveNext() )
		delete	m_mapFamilleNA.GetItem();
	m_mapFamilleNA.Reset();
}

JVoid CRegroupements::GetTitres(JList<JUnt32>& lst)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	for (srcpresse->m_TBLTitres.MoveFirst(); srcpresse->m_TBLTitres.IsValid(); srcpresse->m_TBLTitres.MoveNext() )
		lst.AddTail() = srcpresse->m_TBLTitres.GetItem()->GetID();

	for (m_lstTitreApparente.MoveFirst(); m_lstTitreApparente.IsValid(); m_lstTitreApparente.MoveNext() )
		lst.AddTail() = m_lstTitreApparente.GetItem()->GetID();

	//for (m_mapTitresApparies.MoveFirst(); m_mapTitresApparies.IsValid(); m_mapTitresApparies.MoveNext() )
	//	lst.AddTail() = m_mapTitresApparies.GetItem()->GetID();
}

JVoid CRegroupements::GetTitresApparentes(JList<JUnt32>& lst)
{
	for (m_lstTitreApparente.MoveFirst(); m_lstTitreApparente.IsValid(); m_lstTitreApparente.MoveNext() )
		lst.AddTail() = m_lstTitreApparente.GetItem()->GetID();
}

//JVoid CRegroupements::GetTitresApparies(JList<JUnt32>& lst)
//{
//	for (m_mapTitresApparies.MoveFirst(); m_mapTitresApparies.IsValid(); m_mapTitresApparies.MoveNext() )
//		lst.AddTail() = m_mapTitresApparies.GetItem()->GetID();
//}

JVoid CRegroupements::GetLots(JList<JUnt32>& lst)
{
//	for (m_lstLot.MoveFirst(); m_lstLot.IsValid(); m_lstLot.MoveNext() )
//		lst.AddTail() = m_lstLot.GetItem()->GetID();

	for (m_lstFamille.MoveFirst(); m_lstFamille.IsValid(); m_lstFamille.MoveNext() )
	{
		if (m_lstFamille.GetItem()->IsLot())
			lst.AddTail() = m_lstFamille.GetItem()->GetID();
	}

	for (m_lstCentre.MoveFirst(); m_lstCentre.IsValid(); m_lstCentre.MoveNext() )
	{
		if (m_lstCentre.GetItem()->IsLot())
			lst.AddTail() = m_lstCentre.GetItem()->GetID();
	}


}
/*
JVoid CRegroupements::GetCouplages(JList<JUnt32>& lst)
{
	for (m_lstCouplageLibre.MoveFirst(); m_lstCouplageLibre.IsValid(); m_lstCouplageLibre.MoveNext() )
		lst.AddTail() = m_lstCouplageLibre.GetItem()->GetID();
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	for (srcpresse->m_TBLCouplagesFixes.MoveFirst(); srcpresse->m_TBLCouplagesFixes.IsValid(); srcpresse->m_TBLCouplagesFixes.MoveNext() )
		lst.AddTail() = srcpresse->m_TBLCouplagesFixes.GetItem()->GetID();
}*/
JVoid CRegroupements::GetCouplagesLibres(JList<JUnt32>& lst)
{
	for (m_lstCouplageLibre.MoveFirst(); m_lstCouplageLibre.IsValid(); m_lstCouplageLibre.MoveNext() )
		lst.AddTail() = m_lstCouplageLibre.GetItem()->GetID();
}
//JVoid CRegroupements::GetCouplagesFixes(JList<JUnt32>& lst)
//{
//	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
//	for (srcpresse->m_TBLCouplagesFixes.MoveFirst(); srcpresse->m_TBLCouplagesFixes.IsValid(); srcpresse->m_TBLCouplagesFixes.MoveNext() )
//		lst.AddTail() = srcpresse->m_TBLCouplagesFixes.GetItem()->GetID();
//}



JVoid CRegroupements::GetLstTitres(JList<IElem*>& lst)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	for (srcpresse->m_TBLTitres.MoveFirst(); srcpresse->m_TBLTitres.IsValid(); srcpresse->m_TBLTitres.MoveNext() )
		lst.AddTail() = srcpresse->m_TBLTitres.GetItem();

	//les apparentes 
	for (m_lstTitreApparente.MoveFirst(); m_lstTitreApparente.IsValid(); m_lstTitreApparente.MoveNext() )
		lst.AddTail() = m_lstTitreApparente.GetItem();

	////les apparies
	//for (m_mapTitresApparies.MoveFirst(); m_mapTitresApparies.IsValid(); m_mapTitresApparies.MoveNext() )
	//	lst.AddTail() = m_mapTitresApparies.GetItem();
}

JVoid CRegroupements::GetLstTitresApparentes(JList<IElem*>& lst)
{
	for (m_lstTitreApparente.MoveFirst(); m_lstTitreApparente.IsValid(); m_lstTitreApparente.MoveNext() )
		lst.AddTail() = m_lstTitreApparente.GetItem();
}

JVoid CRegroupements::GetLstCouplages(JList<IElem*>& lst)
{
	for (m_lstCouplageLibre.MoveFirst(); m_lstCouplageLibre.IsValid(); m_lstCouplageLibre.MoveNext() )
		lst.AddTail() = m_lstCouplageLibre.GetItem();

	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	for (srcpresse->m_TBLTitres.MoveFirst(); srcpresse->m_TBLTitres.IsValid(); srcpresse->m_TBLTitres.MoveNext() )
		lst.AddTail() = srcpresse->m_TBLTitres.GetItem();
}

JVoid CRegroupements::GetLstCouplagesLibres(JList<IElem*>& lst)
{
	for (m_lstCouplageLibre.MoveFirst(); m_lstCouplageLibre.IsValid(); m_lstCouplageLibre.MoveNext() )
		lst.AddTail() = m_lstCouplageLibre.GetItem();
}

JVoid CRegroupements::GetLstCouplagesFixes(JList<IElem*>& lst)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	for (srcpresse->m_TBLTitres.MoveFirst(); srcpresse->m_TBLTitres.IsValid(); srcpresse->m_TBLTitres.MoveNext() )
		lst.AddTail() = srcpresse->m_TBLTitres.GetItem();
}


JVoid CRegroupements::GetLstTitresApparies(JList<IElem*>& lst, JUnt32 idSource, JUnt32 idTerrain)
{
	CKeyUntUnt key;
	key.m_Id1 = idSource;
	key.m_Id2 = idTerrain;
	m_mapTitresApparies.MoveTo(key);
	if (m_mapTitresApparies.IsValid())
	{
		CTitresApparies * mapTitresApp = m_mapTitresApparies.GetItem();
		for (mapTitresApp->MoveFirst(); mapTitresApp->IsValid(); mapTitresApp->MoveNext() )
			lst.AddTail() = mapTitresApp->GetItem();

	}
}

CTitresApparies* CRegroupements::GetTitresApparies(JUnt32 idSource, JUnt32 idTerrain)
{
	CKeyUntUnt key;
	key.m_Id1 = idSource;
	key.m_Id2 = idTerrain;
	m_mapTitresApparies.MoveTo(key);
	if (m_mapTitresApparies.IsValid())
		return m_mapTitresApparies.GetItem();
	else
	{
		CTitresApparies * mapTitresApp = new CTitresApparies();
		JSRCTables*  srcTables = (JSRCTables*)&JSRCTables::GetInstance();
		JSource* Src = (JSource*)srcTables->m_TBLSources.GetItem(idSource);
		JTerrain* Ter = (JTerrain*)srcTables->m_TBLTerrains.GetItem(idSource, idTerrain);
		JStringEx Name;
		Name.Append("/");
		Name.Append(Src->m_Libelle.AsJCharPtr());
		Name.Append(Ter->m_Libelle.AsJCharPtr());
		mapTitresApp->SetSourceID(idSource);
		mapTitresApp->SetTerrainID(idTerrain);
		mapTitresApp->Load(Name);

		m_mapTitresApparies.Add(key) = mapTitresApp;
		return mapTitresApp;
	}
}

//ne regarde que dans les ta chargés
CTitreApparie* CRegroupements::GetTitreApparieByID(JUnt32 id)
{
	CTitresApparies * mapTitresApp = NULL;
	for (m_mapTitresApparies.MoveFirst(); m_mapTitresApparies.IsValid(); m_mapTitresApparies.MoveNext())
	{
		mapTitresApp = m_mapTitresApparies.GetItem();
		mapTitresApp->MoveTo(id);
		if (mapTitresApp->IsValid())
			return mapTitresApp->GetItem();
	}
	//on n'a rien trouvé
	return NULL;
}