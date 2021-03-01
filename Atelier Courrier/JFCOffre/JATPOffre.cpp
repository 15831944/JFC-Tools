#include "stdafx.h"
#include "jatpoffre.h"
#include "JMFCStream.h"
#include "JSRCTables.h"
#include "KeyTerrain.h"
// #include "OffreTerrainManager.h"
#include "TerrainManager.h"
#include "ItemsConstruitsManager.h"

#include <afxadv.h>

/////////////////////////////////////////////////////////////
// Constructeur
JATPOffre::JATPOffre(void)
{
	// Changes
	m_Changes					= 0;
	// Source d'Audience
	m_IdSourceAudience			= 0;
	// Type de Poids
	m_IdTypePoids				= 1;
	// Terrain de Ciblage
	m_IdTerrainCiblage			= 0;
	// Cible d'Affinité
	m_IdItemAffinite			= 0;

	// Table de Terrains d'audience by ID
	//	m_MAPpTerrains.Reset();

	// Init des interdits items par terrains
	m_MapInterdictionsItems.Reset();
}

////////////////////////////////////////////////////////////////////
// Destructeur
JATPOffre::~JATPOffre(void)
{
	// Delete the loaded terrains
	//for(m_MAPpTerrains.MoveFirst(); m_MAPpTerrains.IsValid() ; m_MAPpTerrains.MoveNext())
	//	delete m_MAPpTerrains.GetItem();
}

////////////////////////////////////////////////////////////////////
// Sérialisation de l'offre
JVoid JATPOffre::Send(JStream & Stream)
{
	JInt32 Count;

	// Changes
	Stream.Send(m_Changes);
	// Source d'Audience
	Stream.Send(m_IdSourceAudience);

	// Terrains d'Audiences
	Count = m_TBLIdTerrainsAudience.GetCount();
	Stream.Send(Count);
	for(m_TBLIdTerrainsAudience.MoveFirst();m_TBLIdTerrainsAudience.IsValid();m_TBLIdTerrainsAudience.MoveNext())
	{
		JUnt32 IdTerrain = m_TBLIdTerrainsAudience.GetItem();

		/*
		IdTerrain = 282;

		// Décomposer en octet
		JUnt08 IdTerrain4;
		JUnt08 IdTerrain3;
		JUnt08 IdTerrain2;
		JUnt08 IdTerrain1;

		IdTerrain1 = IdTerrain % 256;
		IdTerrain2 = (IdTerrain) / 256;
		IdTerrain3 = (IdTerrain - 256) / (256^2);
		IdTerrain4 = (IdTerrain - 256^2) / (256^3);

		Stream.Send(IdTerrain1);
		Stream.Send(IdTerrain2);
		Stream.Send(IdTerrain3);
		Stream.Send(IdTerrain4);
		*/

		Stream.Send(IdTerrain);
		// Stream.Send(m_TBLIdTerrainsAudience.GetItem());
	}

	// @@@@@@ EPM 2006-2007 BUG IDTERRAIN + 281
	// return;

	// Liste de Segments d'Audience
	Count = m_TBLIdSegmentsAudience.GetCount();
	Stream.Send(Count);
	for(m_TBLIdSegmentsAudience.MoveFirst();m_TBLIdSegmentsAudience.IsValid();m_TBLIdSegmentsAudience.MoveNext())
	{
		Stream.Send(m_TBLIdSegmentsAudience.GetItem());
	}

	// Type de Poids
	Stream.Send(m_IdTypePoids);

	// List de Formats
	Count = m_TBLIdFormats.GetCount();
	Stream.Send(Count);
	for(m_TBLIdFormats.MoveFirst();m_TBLIdFormats.IsValid();m_TBLIdFormats.MoveNext())
	{
		Stream.Send(m_TBLIdFormats.GetItem());
	}

	// List de Supports
	Count = m_TBLIdSupports.GetCount();
	Stream.Send(Count);
	for(m_TBLIdSupports.MoveFirst();m_TBLIdSupports.IsValid();m_TBLIdSupports.MoveNext())
	{
		Stream.Send(m_TBLIdSupports.GetItem());
	}

	// Map Sélection Familles
	Count = m_MapSelFamilles.GetCount();
	Stream.Send(Count);
	for(m_MapSelFamilles.MoveFirst(); m_MapSelFamilles.IsValid(); m_MapSelFamilles.MoveNext())
	{
		// Sauve ident famille
		JUnt32 Key				= m_MapSelFamilles.GetKey();
		Stream.Send(Key);

		// Récup info famille
		CInfoFamille &InfoFamille = m_MapSelFamilles.GetItem();

		// Sauve nom de la famille
		InfoFamille.m_NomFamille.Send(Stream);  

		// Sauve Nb composants
		Count = InfoFamille.m_LstIdSupport.GetCount();
		Stream.Send(Count);

		// Sauve liste des composants (idents supports)
		for (InfoFamille.m_LstIdSupport.MoveFirst(); 
			 InfoFamille.m_LstIdSupport.IsValid(); 
			 InfoFamille.m_LstIdSupport.MoveNext())
		{
			JUnt32 IdSupport = InfoFamille.m_LstIdSupport.GetItem(); 
			Stream.Send(IdSupport);
		}
	}

	// Source de Ciblage (inutilisée)
	const JUnt32 dummyIdSourceCiblage = 0;
	Stream.Send(dummyIdSourceCiblage);

	// Terrain de Ciblage
	Stream.Send(m_IdTerrainCiblage);

	// Liste de Segments de Ciblage (inutilisée)
	Count = 0;
	Stream.Send(Count);

	// List de Cibles (*** attention aux cibles construites)
	Count = m_TBLIdItems.GetCount();
	Stream.Send(Count);

	// List pour export des ids cibles
	JList <JInt32> ListIdItem;
	ListIdItem.Reset();

	// Boucle sur toutes les cibles sélectionnées
	for(m_TBLIdItems.MoveFirst();m_TBLIdItems.IsValid();m_TBLIdItems.MoveNext())
	{
		// stocke dans liste globale id cible
		ListIdItem.AddTail() = m_TBLIdItems.GetItem();
	
		// Sauve id courant cible
		 Stream.Send(m_TBLIdItems.GetItem());
	}

	// Cible d'Affinité (*** attention aux cibles construites)
	ListIdItem.AddTail() = m_IdItemAffinite;
	Stream.Send(m_IdItemAffinite);

	// Export Map pour définition des cibles construites
	JMap <JInt32,JBool> MapItemExp;
	MapItemExp.Reset(); 

	// Via fichier archive
	CArchive *pExportArchive = ((JMFCStream&)Stream).GetCArchive();

	// Export définitions des items
	// CItemsConstruits::GetInstance()->Export(*pExportArchive,ListIdItem,MapItemExp);
	CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Export(*pExportArchive,ListIdItem,MapItemExp);

	/* NE MARCHE PAS DANS CERTAIN CAS !!!! PROBLEME DANS JFMCSTREAM ?????
	// SYLVAIN MODIFIERA LES ROUTINES DE LECTURE ET ECRITURE ITEM 
	// (VIRE LES STDIOFILES NON CALEES AVEC STREAM
	//
	// Export Map pour définition des cibles construites
	JMap <JInt32,JBool> MapItemExp;
	MapItemExp.Reset(); 

	// Flush contenu Stream pour mise à jour CFile
	Stream.Flush(); 

	// Via fichier texte
	// CStdioFile ExportFile((FILE*)(        ((CStdioFile*)((JMFCStream&)Stream).GetCFile())->m_pStream    ));
	CStdioFile *pExportFile = (CStdioFile*)((JMFCStream&)Stream).GetCFile();

	// Flag repérage (position bloc définition des items)
	JUnt32 PosBloc = pExportFile->GetPosition(); 
    pExportFile->Write(&PosBloc,sizeof(JUnt32));
	
	// Export définitions des items
	CItemsConstruits::GetInstance()->Export(*pExportFile,ListIdItem,MapItemExp);

	// Ecriture archive et vide le stream courant
	pExportFile->Flush();
	*/

	// Table de libellés de supports
	Count = m_mapLblSupport.GetCount();
	Stream.Send(Count);
	for(m_mapLblSupport.MoveFirst();m_mapLblSupport.IsValid();m_mapLblSupport.MoveNext())
	{
		Stream.Send(m_mapLblSupport.GetKey());
		m_mapLblSupport.GetItem().Send(Stream);
	}

	//Table de libellés d'item
	Count = m_mapLblItem.GetCount();
	Stream.Send(Count);
	for(m_mapLblItem.MoveFirst();m_mapLblItem.IsValid();m_mapLblItem.MoveNext())
	{
		Stream.Send(m_mapLblItem.GetKey());
		m_mapLblItem.GetItem().Send(Stream);
	}
}

// Récupération Offre : Avec modif pour Correspondance entre ItemConstruit
JVoid JATPOffre::Recv(JStream & Stream, JMap <JInt32,const IItem *> &MapItemImp)
{
	JInt32 Count;

	// No Version sauvegarde offre
	JUnt32 NoVersion = Stream.GetVersion();

	switch(Stream.GetVersion())
	{
	case 1 :
	case 2 :
	case 3 :
	case 4 :
	case 5 :
	case 6 :
	case 7 :
	case 8 :
		// JUnt32 NbALaCon;
		// Stream.Recv(NbALaCon);

        // Changes
		Stream.Recv(m_Changes);

		// Source d'Audience
		Stream.Recv(m_IdSourceAudience);

		// Terrains d'Audiences
		m_TBLIdTerrainsAudience.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			/* Problème en bva 2004-2005 uniquement ?????
			JUnt32 IdTerrain = 0;
			Stream.Recv(IdTerrain);
			m_TBLIdTerrainsAudience.AddTail() = IdTerrain;
			*/

			// @@@@@@ EPM 2006-2007 BUG IDTERRAIN + 281
			/*
			JUnt08 IdTerrain1;
			JUnt08 IdTerrain2;
			JUnt08 IdTerrain3;
			JUnt08 IdTerrain4;
			Stream.Recv(IdTerrain1);
			Stream.Recv(IdTerrain2);
			Stream.Recv(IdTerrain3);
			Stream.Recv(IdTerrain4);

			JUnt32 IdTerrain = 0;
			IdTerrain =  (JUnt32)IdTerrain1;
			IdTerrain += (JUnt32)IdTerrain2 * 256;
			IdTerrain += (JUnt32)IdTerrain3 * 256^2;
			IdTerrain += (JUnt32)IdTerrain4 * 256^3;

			// IdTerrain = (JUnt32)IdTerrain4 * (256^3) + (JUnt32)IdTerrain3 * (256^2) + (JUnt32)IdTerrain2 * 256 + (JUnt32)IdTerrain1;
			*/
			
			// Récupère ident terrain			
			JUnt32 IdTerrain = 0;
			Stream.Recv(IdTerrain);
			m_TBLIdTerrainsAudience.AddTail() = IdTerrain;
		}

		// @@@@@@ EPM 2006-2007 BUG IDTERRAIN + 281
		// return;

		// Liste de Segments d'Audience
		m_TBLIdSegmentsAudience.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			Stream.Recv(m_TBLIdSegmentsAudience.AddTail());
		}

		// Type de Poids
		Stream.Recv(m_IdTypePoids);

		// List de Formats
		m_TBLIdFormats.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			Stream.Recv(m_TBLIdFormats.AddTail());
		}

		// List de Supports
		m_TBLIdSupports.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			Stream.Recv(m_TBLIdSupports.AddTail());
		}

		// Attention récupération sélection familles (ssi no version >= 7)
		m_MapSelFamilles.Reset();
		if (NoVersion >= 7)
		{
			// Récup nb familles
			Stream				.Recv(Count);
			JMap<JUnt32,CInfoFamille> MapSelFamilles;
			for(int x = 0; x < Count; x++)
			{
				// Récup Key Info Famille
				JUnt32				Key;					Stream				.Recv(Key);
		
				// Récup infos familles (nom et composants supports
				CInfoFamille		InfoFamille;

				// Nom de la famille
				InfoFamille.m_NomFamille.Recv(Stream);

				// Nb composants
				JUnt32 NbSupports;
				Stream.Recv(NbSupports); 

				// Récup liste des composants (idents supports)
				InfoFamille.m_LstIdSupport.Reset();   
				for (JUnt32 NoSupp = 0; NoSupp < NbSupports; NoSupp++)
				{
					// Ident support à ajouter
					JUnt32 IdSupport;
					Stream.Recv(IdSupport); 

					// Ajoute à la liste
					InfoFamille.m_LstIdSupport.AddTail() = IdSupport; 
				}

				// Ajoute élmt infos famille dans le map
				MapSelFamilles.Add(Key) = InfoFamille;
			}

			// Récupère dans le map offre
			m_MapSelFamilles.Swap(MapSelFamilles); 
		}

		// Source de Ciblage (inutilisée)
		JUnt32 dummyIdSourceCiblage = 0;
		Stream.Recv(dummyIdSourceCiblage);

		// Terrain de Ciblage
		Stream.Recv(m_IdTerrainCiblage);

		// Liste de Segments de Ciblage dummy
		JList<JInt32> dummyTBLIdSegmentsCiblage;
		dummyTBLIdSegmentsCiblage.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++) Stream.Recv(dummyTBLIdSegmentsCiblage.AddTail());
		
		// List de Cibles
		m_TBLIdItems.Reset();

		// Récupère nb cibles
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			JUnt32 IdCible = 0;
			Stream.Recv(IdCible);
			m_TBLIdItems.AddTail() = IdCible;

			// Récupère l'ident item cible
			/*
			Stream.Recv(m_TBLIdItems.AddTail());
			*/
		}

		// Cible d'Affinité
		Stream.Recv(m_IdItemAffinite);

		// A Partir de la version 2, gestion des sauvegardes construction items
		int NoVers = Stream.GetVersion();

		if (Stream.GetVersion() > 3)
		{
			// Via Archive
			///////////////////////// Modif pour correspondance ItemConstruit /////////////////////
			//
			// Map de contrôles des items
			MapItemImp.Reset(); 

			// Via fichier texte
            CArchive *pImportArchive = ((JMFCStream&)Stream).GetCArchive();

			// Import des définitions items
			// CItemsConstruits::GetInstance()->Import(*pImportArchive,MapItemImp);
			CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Import(*pImportArchive,MapItemImp);

			// Balaye la liste des items cibles
			for (m_TBLIdItems.MoveFirst(); m_TBLIdItems.IsValid(); m_TBLIdItems.MoveNext())
			{
				// Ident cible
				JUnt32 IdCib = m_TBLIdItems.GetItem();

				// On se positionne sur le map import
				MapItemImp.MoveTo(m_TBLIdItems.GetItem());
				if (MapItemImp.IsValid())
				{
					// Recadrage de l'ident item
					JUnt32 IdItem = MapItemImp.GetItem()->GetID();
					m_TBLIdItems.GetItem() = MapItemImp.GetItem()->GetID();  
				}
				// else if (m_TBLIdItems.GetItem() != CItemsConstruits::GetInstance()->GetItemEnsemble()->GetID())
				else if (m_TBLIdItems.GetItem() != CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemEnsemble()->GetID())
				{
					// cible non récupérée
					m_TBLIdItems.GetItem() = 0;
				}
			}

			// Elague les cibles non valides
			for (m_TBLIdItems.MoveLast(); m_TBLIdItems.IsValid(); m_TBLIdItems.MovePrev())
			{
				if (m_TBLIdItems.GetItem() == 0) m_TBLIdItems.Remove();
			}

			// Vérifie également pour item affinité
			MapItemImp.MoveTo(m_IdItemAffinite);
			if (MapItemImp.IsValid())
			{
				// Récupère le non id
				m_IdItemAffinite = MapItemImp.GetItem()->GetID();  
			}
			else
			{
				// Par défaut on prendra la cible ensemble (tjrs présente)
				// m_IdItemAffinite = CItemsConstruits::GetInstance()->GetItemEnsemble()->GetID(); 
				CItemsConstruitsManager * pItemsConstruitsManager = CItemsConstruitsManager::GetInstance();
				m_IdItemAffinite = pItemsConstruitsManager->GetItemsInstance()->GetItemEnsemble()->GetID();  
			}
		}
			
		// Les libellés supports
		m_mapLblSupport.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			JUnt32 k;
			JLabel lbl;
			Stream.Recv(k);
			lbl.Recv(Stream);
			m_mapLblSupport.Add(k) = lbl;
		}

		// Les libellés item
		m_mapLblItem.Reset();
		Stream.Recv(Count);
		for(JInt32 x=0; x < Count ; x++)
		{
			JUnt32 k;
			JLabel lbl;
			Stream.Recv(k);
			lbl.Recv(Stream);
			m_mapLblItem.Add(k) = lbl;
		}

		// Attention ici il faut refabriquer le map des cibles autorisées par terrain
		ConstructionItemsInterdits();

		break;
	}
}

JATPOffre & JATPOffre::operator =(const JATPOffre & Source)
{
	// Changes
	m_Changes					= Source.m_Changes;

	// Source d'Audience
	m_IdSourceAudience			= Source.m_IdSourceAudience;

	// Terrains d'Audiences
	m_TBLIdTerrainsAudience.Reset();
	for (Source.m_TBLIdTerrainsAudience.MoveFirst(); Source.m_TBLIdTerrainsAudience.IsValid(); Source.m_TBLIdTerrainsAudience.MoveNext())
		m_TBLIdTerrainsAudience.AddTail() = Source.m_TBLIdTerrainsAudience.GetItem();

	// Liste de Segments d'Audience
	//m_TBLIdSegmentsAudience		.Swap(Source.m_TBLIdSegmentsAudience);
	m_TBLIdSegmentsAudience.Reset();
	for (Source.m_TBLIdSegmentsAudience.MoveFirst(); Source.m_TBLIdSegmentsAudience.IsValid(); Source.m_TBLIdSegmentsAudience.MoveNext())
		m_TBLIdSegmentsAudience.AddTail() = Source.m_TBLIdSegmentsAudience.GetItem();

	// Type de Poids
	m_IdTypePoids				= Source.m_IdTypePoids;

	// List de Formats
	//m_TBLIdFormats				.Swap(Source.m_TBLIdFormats);
	m_TBLIdFormats.Reset();
	for (Source.m_TBLIdFormats.MoveFirst(); Source.m_TBLIdFormats.IsValid(); Source.m_TBLIdFormats.MoveNext())
		m_TBLIdFormats.AddTail() = Source.m_TBLIdFormats.GetItem();

	// List de Supports
	//m_TBLIdSupports				.Swap(Source.m_TBLIdSupports);
	m_TBLIdSupports.Reset();
	for (Source.m_TBLIdSupports.MoveFirst(); Source.m_TBLIdSupports.IsValid(); Source.m_TBLIdSupports.MoveNext())
		m_TBLIdSupports.AddTail() = Source.m_TBLIdSupports.GetItem();

	// Map des familles (à partir de la version 7)
	m_MapSelFamilles.Reset();
	for (Source.m_MapSelFamilles.MoveFirst(); Source.m_MapSelFamilles.IsValid(); Source.m_MapSelFamilles.MoveNext())
	{
		m_MapSelFamilles.Add(Source.m_MapSelFamilles.GetKey()) = Source.m_MapSelFamilles.GetItem();  
	}

	// Terrain de Ciblage
	m_IdTerrainCiblage			= Source.m_IdTerrainCiblage;

	// List de Cibles
	//m_TBLIdItems				.Swap(Source.m_TBLIdItems);
	m_TBLIdItems.Reset();
	for (Source.m_TBLIdItems.MoveFirst(); Source.m_TBLIdItems.IsValid(); Source.m_TBLIdItems.MoveNext())
		m_TBLIdItems.AddTail() = Source.m_TBLIdItems.GetItem();

	// Cible d'Affinité
	m_IdItemAffinite			= Source.m_IdItemAffinite;

	m_mapLblSupport.Reset();
	for (Source.m_mapLblSupport.MoveFirst(); Source.m_mapLblSupport.IsValid(); Source.m_mapLblSupport.MoveNext())
		m_mapLblSupport.Add(Source.m_mapLblSupport.GetKey()) = Source.m_mapLblSupport.GetItem();

	m_mapLblItem.Reset();
	for (Source.m_mapLblItem.MoveFirst(); Source.m_mapLblItem.IsValid(); Source.m_mapLblItem.MoveNext())
		m_mapLblItem.Add(Source.m_mapLblItem.GetKey()) = Source.m_mapLblItem.GetItem();


	// Recopie des autorisations cibles par terrains
	m_MapInterdictionsItems.Reset();
	for (Source.m_MapInterdictionsItems.MoveFirst(); Source.m_MapInterdictionsItems.IsValid(); Source.m_MapInterdictionsItems.MoveNext())
	{
		// Clef et item source associé
		const JUnt32 SrcCleCible = Source.m_MapInterdictionsItems.GetKey(); 
		const JMap <JUnt32, JBool> & SrcMapTerrainsInterdits = Source.m_MapInterdictionsItems.GetItem(); 

		// Ajoute le map général 
		JMap <JUnt32, JBool> & MapTerrainsInterdits = m_MapInterdictionsItems.Add(SrcCleCible); 
		for (SrcMapTerrainsInterdits.MoveFirst(); SrcMapTerrainsInterdits.IsValid(); SrcMapTerrainsInterdits.MoveNext())
		{
			// Mise à jour du map des terrains interdits
			MapTerrainsInterdits.Add(SrcMapTerrainsInterdits.GetKey()) = SrcMapTerrainsInterdits.GetItem();   			 
		}
	}

	//m_MAPpTerrains.Reset();
	//for (Source.m_MAPpTerrains.MoveFirst(); Source.m_MAPpTerrains.IsValid(); Source.m_MAPpTerrains.MoveNext())
	//	m_MAPpTerrains.Add(Source.m_MAPpTerrains.GetKey()) = Source.m_MAPpTerrains.GetItem();

	return (*this);
}

JVoid JATPOffre::Swap(JATPOffre & Source)
{
	// Changes
	JUnt32	Tmp					= m_Changes;
	m_Changes					= Source.m_Changes;
	Source.m_Changes			= Tmp;

	// Source d'Audience
	Tmp							= m_IdSourceAudience;
	m_IdSourceAudience			= Source.m_IdSourceAudience;
	Source.m_IdSourceAudience	= Tmp;

	// Terrains d'Audiences
	m_TBLIdTerrainsAudience		.Swap(Source.m_TBLIdTerrainsAudience);

	// Liste de Segments d'Audience
	m_TBLIdSegmentsAudience		.Swap(Source.m_TBLIdSegmentsAudience);

	// Type de Poids
	Tmp							= m_IdTypePoids;
	m_IdTypePoids				= Source.m_IdTypePoids;
	Source.m_IdTypePoids		= Tmp;

	// List de Formats
	m_TBLIdFormats				.Swap(Source.m_TBLIdFormats);

	// List de Supports
	m_TBLIdSupports				.Swap(Source.m_TBLIdSupports);

	// Map des familles
	m_MapSelFamilles			.Swap(Source.m_MapSelFamilles); 

	// Terrain de Ciblage
	Tmp							= m_IdTerrainCiblage;
	m_IdTerrainCiblage			= Source.m_IdTerrainCiblage;
	Source.m_IdTerrainCiblage	= Tmp;

	// List de Cibles
	m_TBLIdItems				.Swap(Source.m_TBLIdItems);

	// Cible d'Affinité
	Tmp							= m_IdItemAffinite;
	m_IdItemAffinite			= Source.m_IdItemAffinite;
	Source.m_IdItemAffinite		= Tmp;

	m_mapLblSupport.Swap(Source.m_mapLblSupport);
	m_mapLblItem.Swap(Source.m_mapLblItem);
//	m_MAPpTerrains.Swap(Source.m_MAPpTerrains);

	// Swap des autorisations cible sur les terrains
	m_MapInterdictionsItems.Swap(Source.m_MapInterdictionsItems);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Chargement éléments source en cours pour une appli
JVoid JATPOffre::LoadParamSource(JString AbrNomAppli)
{
	// Récupère la dernière source utilisée
	CString PathInfo;
	PathInfo.Format("Options\\%s",AbrNomAppli.AsJCharPtr());  
	m_IdSourceAudience	= AfxGetApp()->GetProfileInt(PathInfo, "SourceId", 1);

	// Init tableau des terrains précedemment sélectionnés
	m_TBLIdTerrainsAudience.Reset(); 

	// Récupère le nombre de terrains derniérement utilisés
	CString Str;
	JUnt32 NbTerrain = AfxGetApp()->GetProfileInt(PathInfo, "NbTerrainId",0);
	for(JUnt32 IndexTerrain = 0; IndexTerrain < NbTerrain; IndexTerrain++)
	{
		// Récup chacun des idents terrains utilisés
		Str.Format(_T("TerrainId%d"), IndexTerrain);
		JUnt32 IdTerrain = AfxGetApp()->GetProfileInt(PathInfo, Str,0);

		// Ajoute au tableau des terrains
		m_TBLIdTerrainsAudience.AddTail() = IdTerrain;  
	}

	// Récupère le nombre de terrains derniérement utilisés
	JUnt32 NbSeg = AfxGetApp()->GetProfileInt(PathInfo, "NbSegmentsId",0);
	for(JUnt32 IndexSeg = 0; IndexSeg < NbSeg; IndexSeg++)
	{
		// Récup id segment
		Str.Format(_T("SegmentsId%d"), IndexSeg);
		JUnt32 IdSeg = AfxGetApp()->GetProfileInt(PathInfo, Str,0);

		// Ajoute dans le tableau des segments utilisés
		m_TBLIdSegmentsAudience.AddTail() = IdSeg;  
	}

	// Puis enfin le type de poids
	m_IdTypePoids = AfxGetApp()->GetProfileInt(PathInfo,"TypePoidsId",1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde éléments source en cours pour une appli
JVoid JATPOffre::SaveParamSource(JString AbrNomAppli)
{
	// Récupère la dernière source utilisée
	CString PathInfo;
	PathInfo.Format("Options\\%s",AbrNomAppli.AsJCharPtr());  

	// Sauve la dernière source utilisée
    AfxGetApp()->WriteProfileInt(PathInfo, "SourceId", m_IdSourceAudience);

	// Sauve le nombre de terrains derniérement utilisés
	CString	Str;
	JUnt32 NbTerrain = m_TBLIdTerrainsAudience.GetCount(); 

	// Ecriture nombre de terrains utilisés
	AfxGetApp()->WriteProfileInt(PathInfo, "NbTerrainId", NbTerrain);
	
	// Sauve tous les terrains
	Str = "";

	JUnt32 IndexTerrain = 0;
	for(m_TBLIdTerrainsAudience.MoveFirst(); m_TBLIdTerrainsAudience.IsValid(); m_TBLIdTerrainsAudience.MoveNext())
	{
		// Ecriture des idents terrains utilisés
		Str.Format(_T("TerrainId%d"), IndexTerrain);

		// Id terrain
		JUnt32 IdTerrain = m_TBLIdTerrainsAudience.GetItem();
	    AfxGetApp()->WriteProfileInt(PathInfo, Str,IdTerrain);

		// Passe à index terrain suivant
		IndexTerrain++;
	}

	// Vire tous les autres idents terrains inutiles
	for (IndexTerrain; IndexTerrain < 12; IndexTerrain++)
	{
		// La clef terrain
		Str.Format(_T("TerrainId%d"), IndexTerrain);
		JUnt32 IdTerrain = AfxGetApp()->GetProfileInt(PathInfo, Str,0);

		if (IdTerrain != 0)
			// on vire cette info inutile
			AfxGetApp()->WriteProfileInt(PathInfo, Str,NULL);
	}

	// Ecriture nombre de segments utilisés
	JUnt32 NbSeg = m_TBLIdSegmentsAudience.GetCount();  
	AfxGetApp()->WriteProfileInt(PathInfo, "NbSegmentsId", NbSeg);

	// Sauve tous les segments
	Str = "";
	JUnt32 IndexSeg = 0;
	for(m_TBLIdSegmentsAudience.MoveFirst(); m_TBLIdSegmentsAudience.IsValid(); m_TBLIdSegmentsAudience.MoveNext())
	{
		// Ecriture des idents segments utilisés
		Str.Format(_T("SegmentsId%d"), IndexSeg);

		// Id segment
		JUnt32 IdSeg = m_TBLIdSegmentsAudience.GetItem();
	    AfxGetApp()->WriteProfileInt(PathInfo, Str,IdSeg);

		// Passe à index segment
		IndexSeg++;
	}

	// Ecriture du type de poids utilisé
    AfxGetApp()->WriteProfileInt(PathInfo, "TypePoidsId",m_IdTypePoids);
}

////////////////////////////////////////////////////////////////////////////////////////
// Renvoi si la cible est autorisée
JBool JATPOffre::CibleAutorisee(JUnt32 IdCible, JUnt32 IdTerrain)
{
	// On se place sur les infos concernat la cible
    m_MapInterdictionsItems.MoveTo(IdCible);
	if (m_MapInterdictionsItems.IsValid())
	{	
		// Récupération infos autorisation pour la cible
		JMap <JUnt32, JBool> & MapInterditCible = m_MapInterdictionsItems.GetItem();

		// On se place sur autorisation terrain
		MapInterditCible.MoveTo(IdTerrain);
		if (MapInterditCible.IsValid())
			// Cette cible n'est pas autorisée sur ce terrain
			return false;
		else
			// Cette cible est autorisée sur ce terrain
			return true;
	}
	else
		// Cible autorisée sur ce terrain
		return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Reconstruit le map des items interdits sur les terrains
JVoid JATPOffre:: ConstructionItemsInterdits()
{
	// Initilise le map des items interdits
	m_MapInterdictionsItems.Reset();

	// Les items construits
	// CItemsConstruits * m_pItems = CItemsConstruits::GetInstance();

	// Boucle sur les infos seuils terrains
	JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Récup le source
	const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_IdSourceAudience);

	// Récup instance des terrains
	// OffreTerrainManager	= COffreTerrainManager::GetInstance();
	CTerrainManager *pTerrainManager = CTerrainManager::GetInstance();

	// Boucle sur tous les items
	for (m_TBLIdItems.MoveFirst(); m_TBLIdItems.IsValid(); m_TBLIdItems.MoveNext())
	{
		// Récup Ident item
		JUnt32 IdItem = m_TBLIdItems.GetItem();

		// Récupère l'item
		// IItem * pItem = m_pItems->GetIItemByID(IdItem);
		IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdItem);
		
		// Si item valide via les items construits, et que c'est un item simple
		if (pItem && pItem->IsItemConstruit())
		{
			// Récup item construit
			CItemConstruit * pItemConstruit = pItem->AsItemConstruit(); 

			// Boucle tous les terrains de l'offre
			for (m_TBLIdTerrainsAudience.MoveFirst(); m_TBLIdTerrainsAudience.IsValid(); m_TBLIdTerrainsAudience.MoveNext())
			{
				// Récup Ident Terrain
				JUnt32 IdTerrain = m_TBLIdTerrainsAudience.GetItem(); 

				// Récup les infos du terrain
				CKeyTerrain KeyTerrain (m_IdSourceAudience, IdTerrain);
				CTerrain * pTerrain = pTerrainManager->GetTerrain(KeyTerrain); 

				// CTerrain * pTerrain = m_OffreTerrainManager->GetTerrain(KeyTerrain);

				// Si le terrain est défini
				if(pTerrain)
				{
					// Création de la structure de calcul pour l'item
					CCalcItemParams * pParams = new CCalcItemParams();
					
					// Récup univers et échantillonnage lié au terrain
					JUnt32 UniversID		= pTerrain->GetDefaultUnivers();
					JUnt32 m_EchantillonID	= pTerrain->GetDefaultEchantillonID(UniversID);

					// Positionne les paramètres de calcul
					pParams->SetEchantillonID(m_EchantillonID);
					pParams->SetUniversID(pTerrain->GetDefaultUnivers());
					pParams->SetTerrain(pTerrain);
					pParams->SetTypePoidsID(m_IdTypePoids);

					// Tenir compte de la population segment et positionner le paramètre
					CBitArray bArrPopSeg;
					JUnt32 contingent =  0xFFFF;
					pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
					pParams->SetPopulationSegment(bArrPopSeg);

					// Set param segments
					pParams->SetSegmentsIDs(m_TBLIdSegmentsAudience);
				
					// Calcul de l'effectif pour cet item
					pItemConstruit->CalculateEffectif(pParams);

					// Test si nb cas < nb cas rédhibitoire du terrain
					if (pParams->GetNbCas() < pTerrain->GetSeuilRedhibitoire())
					{
						// Cet item n'est pas assez représentatif pour ce terrain, on l'indique de la map des items interdits par terrains
						m_MapInterdictionsItems.MoveTo(IdItem);

						// Ajoute info map item si non existant
						if (!m_MapInterdictionsItems.IsValid())
						{
							// Ajoute infos map terrains invalides 
							JMap <JUnt32, JBool> & MapTerrains = m_MapInterdictionsItems.Add(IdItem);

							// Update liste des terrains interdits
							MapTerrains.MoveTo(IdTerrain);
							if (!MapTerrains.IsValid()) 
							{
								// Ajoute le terrain invalide
								MapTerrains.Add(IdTerrain) = true;
							}
						}
						else
						// Ajout des terrains interdits au map
						{
							// Recup infos map terrains invalides 
							JMap <JUnt32, JBool> & MapTerrains = m_MapInterdictionsItems.GetItem();

							// Update liste des terrains interdits
							MapTerrains.MoveTo(IdTerrain);
							if (!MapTerrains.IsValid()) 
							{
								// Ajoute le terrain invalide
								MapTerrains.Add(IdTerrain) = true; 
							}
						}
					}
				}
			}
		}
	}
}

