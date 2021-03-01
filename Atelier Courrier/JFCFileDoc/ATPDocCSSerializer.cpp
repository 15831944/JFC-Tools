// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPDocCSSerializer.h"
#include "JATPDocChoixSupport.h"

// Constructeur sérialisation document Choix Supports
CATPDocCSSerializer::CATPDocCSSerializer(void)
{
}

// Destructeur sérialisation document Choix Supports
CATPDocCSSerializer::~CATPDocCSSerializer(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Sérialisation sauvegarde document Choix Supports
void CATPDocCSSerializer::Send(JStream & Stream, JATPDocChoixSupport * doc)
{
	// on sérialise l'entête
	JInt32 Compagny = 0x0043464A; Stream.Send(Compagny);
	JInt32 Magic    = 0x4A6D2311; Stream.Send(Magic);
	JInt32 Compress = 0x00000000; Stream.Send(Compress);
	JInt32 Encrypt  = 0x00000000; Stream.Send(Encrypt);
	Stream.Send(VERSIONCS);

	JInt32 NoVersion = VERSIONCS;

	// on initialise la version
	JInt32 OldVersion = Stream.SetVersion(VERSIONCS);

	switch(VERSIONCS)
	{
		// Sérialisation type version 1
		case 1:
			WriteV1(Stream, doc);
			break;

		// Sérialisation type version 2
		case 2:
			WriteV2(Stream, doc);
			break;

		// Sérialisation type version 3
		case 3:
			WriteV3(Stream, doc);
			break;

		// Sérialisation type version 4
		// (offre au début / pour compatibilié avec creuset)
		case 4:
			WriteV4(Stream, doc);
			break;

		// Sérialisation type version 5
		// (gestion visibilité des supports sélectionnés)
		case 5:
			WriteV5(Stream, doc);
			break;

		// Sérialisation type version 6 (+ archivage des rangs grandeurs utilisées)
		// Sérialisation type version 7 (idem + archivage des familles sélectionnées  / via send offre)
		case 6:
		case 7:
		case 8:
			WriteV6(Stream, doc);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Sérialisation récupération document Choix Supports
void CATPDocCSSerializer::Recv(JStream & Stream, JATPDocChoixSupport * doc)
{
	// on sérialise l'entête
	JInt32 Compagny; Stream.Recv(Compagny);
	if (Compagny != 0x0043464A) 
 		throw JBadSchema::GetInstance();
	JInt32 Magic;    Stream.Recv(Magic);    if (Magic    != 0x4A6D2311) throw JBadSchema::GetInstance();
	JInt32 Compress; Stream.Recv(Compress); if (Compress != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Encrypt;  Stream.Recv(Encrypt);  if (Encrypt  != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Version;  Stream.Recv(Version);

	// on initialise la version
	JInt32 OldVersion = Stream.SetVersion(Version);

	// Attention si version < 3 il faudra resauver l'étude pour passage étude entre utilisateur
	if (Version < 3)
	{
		AfxMessageBox("Cette étude sauvegardée ne pourra pas être utilisée pour des échanges entre postes utilisateurs, sauf si vous la sauvegardez à nouveau pour être conforme au nouveau format de sauvegarde");
        doc->SetModifiedFlag(true);
	}

	switch(Version)
	{
		// Sérialisation type version 1
		case 1:
			ReadV1(Stream, doc);
			break;

		// Sérialisation type version 2
		case 2:
			ReadV2(Stream, doc);
			break;

		// Sérialisation type version 3
		case 3:
			ReadV3(Stream, doc);
			break;

		// Sérialisation type version 4
		case 4:
			ReadV4(Stream, doc);
			break;

		// Sérialisation type version 5
		case 5:
			ReadV5(Stream, doc);
			break;

		// Sérialisation type version 6 (en + récup. sélection des rangs)
		// Sérialisation type version 7 (idem + récup sélection des familles / via recv offre)
		case 6:
		case 7:
		case 8:
			ReadV6(Stream, doc);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Sérialisation récupération document Choix Supports
void CATPDocCSSerializer::RecvNoVersion(JStream & Stream, JUnt32 & NoVersion, JATPDocChoixSupport * doc)
{
	// on sérialise l'entête
	JInt32 Compagny; Stream.Recv(Compagny);
	if (Compagny != 0x0043464A) 
 		throw JBadSchema::GetInstance();
	JInt32 Magic;    Stream.Recv(Magic);    if (Magic    != 0x4A6D2311) throw JBadSchema::GetInstance();
	JInt32 Compress; Stream.Recv(Compress); if (Compress != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Encrypt;  Stream.Recv(Encrypt);  if (Encrypt  != 0x00000000) throw JBadSchema::GetInstance();
	Stream.Recv(NoVersion);
}	


//////////////////////////////////////////////////////////////////////////////////////////////////
// Sérialisation récupération uniquement l'offre du doc
JBool CATPDocCSSerializer::RecvOffre(JStream & Stream, JATPOffre &Offre)
{
	// on sérialise l'entête
	JInt32 Compagny; Stream.Recv(Compagny);
	if (Compagny != 0x0043464A) 
 		throw JBadSchema::GetInstance();
	JInt32 Magic;    Stream.Recv(Magic);    if (Magic    != 0x4A6D2311) throw JBadSchema::GetInstance();
	JInt32 Compress; Stream.Recv(Compress); if (Compress != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Encrypt;  Stream.Recv(Encrypt);  if (Encrypt  != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Version;  Stream.Recv(Version);

	// on initialise la version
	JInt32 OldVersion = Stream.SetVersion(Version);

	// Récupération Offre active à partir de la version 4
	if (Version >= 4)
	{
		// on récupére l'offre au début désormais (pour récup étude creuset <> Cs)
		JMap <JInt32,const IItem *> MapCibleOffre;
		Offre				.Recv(Stream, MapCibleOffre);
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération Offre + Plateau (supports + familles)
JBool CATPDocCSSerializer::RecvOffrePlateau(JStream & Stream, JATPOffre &Offre, 
											JList <JUnt32> &LstSuppPlateau, 
											JDate &DateEtude)
{
	// on sérialise l'entête
	JInt32 Compagny; Stream.Recv(Compagny);
	if (Compagny != 0x0043464A) throw JBadSchema::GetInstance();
	JInt32 Magic;    Stream.Recv(Magic);    if (Magic    != 0x4A6D2311) throw JBadSchema::GetInstance();
	JInt32 Compress; Stream.Recv(Compress); if (Compress != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Encrypt;  Stream.Recv(Encrypt);  if (Encrypt  != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Version;  Stream.Recv(Version);

	// on initialise la version
	JInt32 OldVersion = Stream.SetVersion(Version);

	// Récup seulement pour version sauvegardée récente
	if (Version >= 4)
	{
		// on récupére l'offre au début
		JMap <JInt32,const IItem *> MapCibleOffre;
		Offre.Recv(Stream, MapCibleOffre);

		// Récup étude (voir JATPKEYEtude de Creuset)
		JUnt32 IdProduit; Stream.Recv(IdProduit);
		JDate  DateD; DateD .Recv(Stream);
		JDate  DateF; DateF .Recv(Stream);
		DateEtude = DateD;

		JInt32 Day,Month,Year;
		DateEtude.GetDate(Day,Month,Year); 
		
		// Récup les 2 dates période Action (voir JATPKEYPeriodeAction de Creuset)
		DateD.Recv(Stream);
		DateF.Recv(Stream);

		// Récup les 2 dates période Action Hyp (voir JATPKEYPeriodeAction de Creuset)
		DateD.Recv(Stream);
		DateF.Recv(Stream);

		// Récup les 2 dates période Action Tablier (voir JATPKEYPeriodeAction de Creuset)
		DateD.Recv(Stream);
		DateF.Recv(Stream);

		// Id Source d'audience et de ciblage
		JUnt32		IdSrcAudienceSel;		Stream.Recv(IdSrcAudienceSel);
		JUnt32		IdSrcCiblageSel;		Stream.Recv(IdSrcCiblageSel);

		// Map id terrains sélectionnés
		JInt32					Count;					Stream				.Recv(Count);
		JMap<JUnt32,JUnt32>		MapIdTerrainAudienceSel;
		for(int x = 0; x < Count; x++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainAudienceSel.Add(Key) = Item;
		}

		JUnt32		IdTerrainCiblageSel;		Stream.Recv(IdTerrainCiblageSel);
		JUnt32		IdSegmentAudienceSel;		Stream.Recv(IdSegmentAudienceSel);
		JUnt32		IdSegmentCibleSel;			Stream.Recv(IdSegmentCibleSel);
		JUnt32		IdFiltreAudienceSel;		Stream.Recv(IdFiltreAudienceSel);

		// Map état des formats
														Stream				.Recv(Count);
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		for(int x = 0; x < Count; x++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Récupération éléments échelle creuset
		// nombre d'éléments échelle
		Stream.Recv(Count);

		// on boucle sur tous les éléments
		for (JInt32 Idx = 0; Idx < Count; Idx += 1)
		{
			// on sérialise chaque élément
			JUnt32 IdSupport;
			Stream.Recv(IdSupport);
		}

		// Init liste supports sélectionnés
		LstSuppPlateau.Reset();

		// Récupération éléments plateau supports creuset
		// JATPPlateauSupports		PlateauSupports;		PlateauSupports		.Recv(Stream);
		// nombre d'éléments plateau
		Stream.Recv(Count);

		// on boucle sur tous les éléments
		for (JInt32 Idx = 0; Idx < Count; Idx += 1)
		{
			// on sérialise l'élément id support et id parent support
			JUnt32 IdSupport;		Stream.Recv(IdSupport);
			JUnt32 IdParentSupport; Stream.Recv(IdParentSupport);

			// Ajoute à la liste sélection
			LstSuppPlateau.AddTail() = IdSupport;  
		}

		return true;
	}

	// Pas de récup si ancienne version
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// SERIALISATION VERSION 1 (WRITE & READ)
//
void CATPDocCSSerializer::WriteV1(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Index du module courant 
	Stream.Send(doc->GetModuleCourant());

	// on sérialise le plateau
	doc->m_KEYPlateau.Send(Stream);

	// on sérialise la date tarifaire
	doc->m_DateTarifaire.Send(Stream);

	// on sérialise le map des supports sélectionnés
	JInt32 Count = doc->m_MapIdSupportsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsSel.MoveFirst();
		doc->m_MapIdSupportsSel.IsValid();
		doc->m_MapIdSupportsSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsSel.GetKey();
		JBool  Item = doc->m_MapIdSupportsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des couleurs supports sélectionnées
	Count = doc->m_MapIdColorSupports.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdColorSupports.MoveFirst();
		doc->m_MapIdColorSupports.IsValid();
		doc->m_MapIdColorSupports.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdColorSupports.GetKey();
		JUnt32  Item = doc->m_MapIdColorSupports.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// On sérialise le mode sélection couleurs
	Stream.Send(doc->m_ModeColorGraphe);

	// on sérialise la liste des maps des cibles audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdCiblesAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdCiblesAudienceSel.MoveFirst();
		 doc->m_LstMapIdCiblesAudienceSel.IsValid();
		 doc->m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdCiblesAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise le map des Grandeurs sélectionnées pour échelle
	Count = doc->m_MapIdGrandEchSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandEchSel.MoveFirst();
		doc->m_MapIdGrandEchSel.IsValid();
		doc->m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandEchSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandEchSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Grandeurs sélectionnées pour cumulativité
	Count = doc->m_MapIdGrandCumSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandCumSel.MoveFirst();
		doc->m_MapIdGrandCumSel.IsValid();
		doc->m_MapIdGrandCumSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandCumSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandCumSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Mapping
	JInt32 IdGrand = doc->m_IdGrandMapSelAbs;
	Stream				.Send(IdGrand);

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Ordonnée
	IdGrand = doc->m_IdGrandMapSelOrd;
	Stream				.Send(IdGrand);

	// on sérialise le map des Grandeurs sélectionnées pour Duplication
	Count = doc->m_MapIdGrandDupSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandDupSel.MoveFirst();
		doc->m_MapIdGrandDupSel.IsValid();
		doc->m_MapIdGrandDupSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandDupSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandDupSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour échelle
	Count = doc->m_MapIdFormatSpecifEch.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifEch.MoveFirst();
		doc->m_MapIdFormatSpecifEch.IsValid();
		doc->m_MapIdFormatSpecifEch.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifEch.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour mapping
	Count = doc->m_MapIdFormatSpecifMap.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifMap.MoveFirst();
		doc->m_MapIdFormatSpecifMap.IsValid();
		doc->m_MapIdFormatSpecifMap.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifMap.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifMap.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour cumulativite
	Count = doc->m_MapIdFormatSpecifCum.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifCum.MoveFirst();
		doc->m_MapIdFormatSpecifCum.IsValid();
		doc->m_MapIdFormatSpecifCum.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifCum.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifCum.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour duplication
	Count = doc->m_MapIdFormatSpecifDup.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifDup.MoveFirst();
		doc->m_MapIdFormatSpecifDup.IsValid();
		doc->m_MapIdFormatSpecifDup.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifDup.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifDup.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le nbinsertions sélectionné pour calcul
	Stream.Send(doc->m_NbInsertions);

	// on sérialise liste ordonnancement grandeurs échelle à afficher
	Count = doc->m_LstOrdoGrandeurEch.GetCount();
	Stream.Send(Count);
	for (doc->m_LstOrdoGrandeurEch.MoveFirst();
		 doc->m_LstOrdoGrandeurEch.IsValid();
		 doc->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 IdGrandeur = doc->m_LstOrdoGrandeurEch.GetItem();
		Stream.Send(IdGrandeur);
	}

	// on sérialise la liste des idents src audience sel
	Count = doc->m_LstIdSrcAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcAudienceSel.MoveFirst();
		 doc->m_LstIdSrcAudienceSel.IsValid();
		 doc->m_LstIdSrcAudienceSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcAudienceSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des idents source de ciblage
	Count = doc->m_LstIdSrcCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcCiblageSel.MoveFirst();
		 doc->m_LstIdSrcCiblageSel.IsValid();
		 doc->m_LstIdSrcCiblageSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcCiblageSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des maps des terrains audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 doc->m_LstMapIdTerrainsAudienceSel.IsValid();
		 doc->m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdTerrainsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents terrains ciblage sélectionnés
	Count = doc->m_LstIdTerrainCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdTerrainCiblageSel.MoveFirst();
		 doc->m_LstIdTerrainCiblageSel.IsValid();
		 doc->m_LstIdTerrainCiblageSel.MoveNext())
	{
		JUnt32 IdTer = doc->m_LstIdTerrainCiblageSel.GetItem();
		Stream.Send(IdTer);
	}

	// on sérialise le map des formats sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdFormatsSel.MoveFirst();
		 doc->m_LstMapIdFormatsSel.IsValid();
		 doc->m_LstMapIdFormatsSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdFormatsSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item = MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents segment d'audience
	Count = doc->m_LstIdSegmentAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentAudienceSel.MoveFirst();
		 doc->m_LstIdSegmentAudienceSel.IsValid();
		 doc->m_LstIdSegmentAudienceSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentAudienceSel.GetItem(); 
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident segment pour ciblage
	Count = doc->m_LstIdSegmentCibleSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentCibleSel.MoveFirst();
		 doc->m_LstIdSegmentCibleSel.IsValid();
		 doc->m_LstIdSegmentCibleSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentCibleSel.GetItem();
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident filtre d'audience
	Count = doc->m_LstIdFiltreAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdFiltreAudienceSel.MoveFirst();
		 doc->m_LstIdFiltreAudienceSel.IsValid();
		 doc->m_LstIdFiltreAudienceSel.MoveNext())
	{
		JUnt32 IdFiltre = doc->m_LstIdFiltreAudienceSel.GetItem();
		Stream.Send(IdFiltre);
	}

	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);

	// Sauvegarde des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	Count = doc->m_OrdreEnteteEch.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_OrdreEnteteEch.GetCount(); i++)
		Stream.Send(doc->m_OrdreEnteteEch[i]);

	// Sauvegarde du tri en cours pour chaque module
	Count = doc->m_TabInfoTriCol.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_TabInfoTriCol.GetCount(); i++)
	{
		// Récup infos tri module
		CInfoTriCol InfoTriCol = doc->m_TabInfoTriCol[i];

		// Sauve no de la colonne courante du tri
		Stream.Send(InfoTriCol.NoCol);

		// Sauve mode du tri colonne courante
		Stream.Send(InfoTriCol.TriCroissant);
	}
}

void CATPDocCSSerializer::ReadV1(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Index du module courant 
	JUnt32 InxModuleCur;
	Stream				.Recv(InxModuleCur);
	doc->SetModuleCourant(InxModuleCur);

	// on récupère le plateau
	JATPKEYPlateau			KEYPlateau;				KEYPlateau			.Recv(Stream);

	// on récupère la date tarifaire
	JDate					DateTarifaire;			DateTarifaire		.Recv(Stream);

	// on récupère le map des supports sélectionnés
	JUnt32					Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsSel.Add(Key) = Item;
	}

	// on récupère le map des couleurs affectées aux supports pour les graphes
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdColorSupports;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdColorSupports.Add(Key) = Item;
	}

	// On récupère le mode sélection couleurs
	JUnt32		ModeColorGraphe;					Stream.Recv(ModeColorGraphe);

	// on récupère map des cibles audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdCiblesAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdCiblesAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdCiblesAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdCiblesAudienceSel.AddTail().Swap(MapIdCiblesAudienceSel);
	}

	// on récupére le map des Grandeurs sélectionnées pour échelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);

		// Modif Data Exo !!!!!
		// On vérifie que item grandeur existant dans les grandeurs dispos (suite à grandeurs exogènes)
		doc->m_MapAbrGrandEch.MoveTo(Item);
		if (doc->m_MapAbrGrandEch.IsValid())
			MapIdGrandEchSel.Add(Key) = Item;
	}

	// on récupére le map des Grandeurs sélectionnées pour cumulativité
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandCumSel.Add(Key) = Item;
	}

	// on récupére Ident Grandeur Abscisse sélectionnée pour Mapping
	JInt32 IdGrandMapSelAbs;
	Stream				.Recv(IdGrandMapSelAbs);

	// on récupére Ident Grandeur Ordonnée sélectionnée pour Mapping
	JInt32 IdGrandMapSelOrd;
	Stream				.Recv(IdGrandMapSelOrd);

	// on récupére le map des Grandeurs sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandDupSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Echelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatEchSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Mapping
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatMapSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatMapSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Cumulativite
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatCumSel.Add(Key) = Item;
	}
	
	// on récupére le map des formats sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatDupSel.Add(Key) = Item;
	}

	// on récupére le nbinsertions sélectionné pour calcul
	JUnt32		NbInsertions;				Stream.Recv(NbInsertions);

	// on récupére liste ordonné des grandeurs échelle à afficher
	Stream				.Recv(Count);
	JList<JUnt32>		LstOrdoGrandeurEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Item;					Stream				.Recv(Item);
		LstOrdoGrandeurEch.AddTail() = Item;
	}

	// on récupére les idents source d'audience
	JList <JUnt32> LstIdSrcAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcAudienceSel.AddTail() = Ident; 
	}

	// on récupére l'ident source de ciblage
	JList <JUnt32> LstIdSrcCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcCiblageSel.AddTail() = Ident; 
	}

	// on récupère map des terrains audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdTerrainsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdTerrainsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdTerrainsAudienceSel.AddTail().Swap(MapIdTerrainsAudienceSel);
	}

	// on récupére la liste des idents terrains ciblage
	JList <JUnt32> LstIdTerrainCiblageSel; 
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdTerrainCiblageSel.AddTail() = Ident; 
	}

	// on récupére le map des formats sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdFormatsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdFormatsSel.AddTail().Swap(MapIdFormatsSel);
	}

	// on récupére la liste des idents segment d'audience
	JList <JUnt32> LstIdSegmentAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentAudienceSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents segment pour ciblage
	JList <JUnt32> LstIdSegmentCibleSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentCibleSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents filtre d'audience
	JList <JUnt32> LstIdFiltreAudience;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdFiltreAudience.AddTail() = Ident; 
	}

	// on récupére l'offre
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	// Récupère ordre entetes échelle
	Stream				.Recv(Count);
	CArray<JUnt32> OrdreEnteteEch;
	OrdreEnteteEch.SetSize(Count);
	for (int i = 0; i < OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre;
		Stream.Recv(ValOrdre);
		OrdreEnteteEch[i] = ValOrdre;
	}		

	// Récupère informations tri sur tous les modules
	Stream				.Recv(Count);
	CArray<CInfoTriCol,CInfoTriCol&> TabOrdreTri;
	TabOrdreTri.SetSize(Count);
	for (int i = 0; i < TabOrdreTri.GetCount(); i++)
	{
		// Défini nvlle struct tri
		CInfoTriCol InfoTriCol;

		// Récup col courante
		Stream.Recv(InfoTriCol.NoCol);

		// Récup mode tri
		Stream.Recv(InfoTriCol.TriCroissant);
	}

	// Swap the elements
	doc->m_KEYPlateau					.Swap(KEYPlateau);
	doc->m_DateTarifaire				= DateTarifaire; 
	doc->m_MapIdSupportsSel				.Swap(MapIdSupportsSel);
	doc->m_MapIdColorSupports			.Swap(MapIdColorSupports); 
	doc->m_ModeColorGraphe				= ModeColorGraphe;
	doc->m_LstMapIdCiblesAudienceSel	.Swap(LstMapIdCiblesAudienceSel);

	// Les élmt grandeurs sélection pour les différents modules
	doc->m_MapIdGrandEchSel				.Swap(MapIdGrandEchSel);
	doc->m_MapIdGrandCumSel				.Swap(MapIdGrandCumSel);
	doc->m_IdGrandMapSelAbs				= IdGrandMapSelAbs;
	doc->m_IdGrandMapSelOrd				= IdGrandMapSelOrd;
	doc->m_MapIdGrandDupSel				.Swap(MapIdGrandDupSel);

	// Les élmt formats sélection pour les différents modules
	doc->m_MapIdFormatSpecifEch			.Swap(MapIdFormatEchSel);
	doc->m_MapIdFormatSpecifCum			.Swap(MapIdFormatCumSel);
	doc->m_MapIdFormatSpecifMap			.Swap(MapIdFormatMapSel);
	doc->m_MapIdFormatSpecifDup			.Swap(MapIdFormatDupSel);

	// Eléments sélections pour échelle
	doc->m_NbInsertions					= NbInsertions;
	doc->m_LstOrdoGrandeurEch			.Swap(LstOrdoGrandeurEch);

	doc->m_LstIdSrcAudienceSel			.Swap(LstIdSrcAudienceSel);
	doc->m_LstIdSrcCiblageSel			.Swap(LstIdSrcCiblageSel);
	doc->m_LstMapIdTerrainsAudienceSel	.Swap(LstMapIdTerrainsAudienceSel);
	doc->m_LstIdTerrainCiblageSel		.Swap(LstIdTerrainCiblageSel);
	doc->m_LstMapIdFormatsSel			.Swap(LstMapIdFormatsSel);
	doc->m_LstIdSegmentAudienceSel		.Swap(LstIdSegmentAudienceSel);
	doc->m_LstIdSegmentCibleSel			.Swap(LstIdSegmentCibleSel);
	doc->m_LstIdFiltreAudienceSel		.Swap(LstIdFiltreAudience);
	doc->m_Offre						.Swap(Offre);

	// Ordre des entetes échelles
	doc->m_OrdreEnteteEch.SetSize(OrdreEnteteEch.GetCount());
	for (int i=0; i<doc->m_OrdreEnteteEch.GetCount(); i++)
		doc->m_OrdreEnteteEch[i] = OrdreEnteteEch[i];

	// Tableau ordre de tris
	doc->m_TabInfoTriCol.SetSize(TabOrdreTri.GetCount());
	for (int i=0; i<doc->m_TabInfoTriCol.GetCount(); i++)
		doc->m_TabInfoTriCol[i] = TabOrdreTri[i];
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// SERIALISATION VERSION 2 (WRITE & READ)
//
void CATPDocCSSerializer::WriteV2(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Index du module courant 
	Stream.Send(doc->GetModuleCourant());

	// on sérialise le plateau
	doc->m_KEYPlateau.Send(Stream);

	// on sérialise la date tarifaire
	doc->m_DateTarifaire.Send(Stream);

	// on sérialise le map des supports sélectionnés
	JInt32 Count = doc->m_MapIdSupportsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsSel.MoveFirst();
		doc->m_MapIdSupportsSel.IsValid();
		doc->m_MapIdSupportsSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsSel.GetKey();
		JBool  Item = doc->m_MapIdSupportsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des couleurs supports sélectionnées
	Count = doc->m_MapIdColorSupports.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdColorSupports.MoveFirst();
		doc->m_MapIdColorSupports.IsValid();
		doc->m_MapIdColorSupports.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdColorSupports.GetKey();
		JUnt32  Item = doc->m_MapIdColorSupports.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// On sérialise le mode sélection couleurs
	Stream.Send(doc->m_ModeColorGraphe);

	// on sérialise la liste des maps des cibles audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdCiblesAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdCiblesAudienceSel.MoveFirst();
		 doc->m_LstMapIdCiblesAudienceSel.IsValid();
		 doc->m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdCiblesAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise le map des Grandeurs sélectionnées pour échelle
	Count = doc->m_MapIdGrandEchSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandEchSel.MoveFirst();
		doc->m_MapIdGrandEchSel.IsValid();
		doc->m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandEchSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandEchSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Grandeurs sélectionnées pour cumulativité
	Count = doc->m_MapIdGrandCumSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandCumSel.MoveFirst();
		doc->m_MapIdGrandCumSel.IsValid();
		doc->m_MapIdGrandCumSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandCumSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandCumSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Mapping
	JInt32 IdGrand = doc->m_IdGrandMapSelAbs;
	Stream				.Send(IdGrand);

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Ordonnée
	IdGrand = doc->m_IdGrandMapSelOrd;
	Stream				.Send(IdGrand);

	// on sérialise le map des Grandeurs sélectionnées pour Duplication
	Count = doc->m_MapIdGrandDupSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandDupSel.MoveFirst();
		doc->m_MapIdGrandDupSel.IsValid();
		doc->m_MapIdGrandDupSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandDupSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandDupSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour échelle
	Count = doc->m_MapIdFormatSpecifEch.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifEch.MoveFirst();
		doc->m_MapIdFormatSpecifEch.IsValid();
		doc->m_MapIdFormatSpecifEch.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifEch.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour mapping
	Count = doc->m_MapIdFormatSpecifMap.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifMap.MoveFirst();
		doc->m_MapIdFormatSpecifMap.IsValid();
		doc->m_MapIdFormatSpecifMap.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifMap.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifMap.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour cumulativite
	Count = doc->m_MapIdFormatSpecifCum.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifCum.MoveFirst();
		doc->m_MapIdFormatSpecifCum.IsValid();
		doc->m_MapIdFormatSpecifCum.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifCum.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifCum.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour duplication
	Count = doc->m_MapIdFormatSpecifDup.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifDup.MoveFirst();
		doc->m_MapIdFormatSpecifDup.IsValid();
		doc->m_MapIdFormatSpecifDup.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifDup.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifDup.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le nbinsertions sélectionné pour calcul
	Stream.Send(doc->m_NbInsertions);

	// on sérialise liste ordonnancement grandeurs échelle à afficher
	Count = doc->m_LstOrdoGrandeurEch.GetCount();
	Stream.Send(Count);
	for (doc->m_LstOrdoGrandeurEch.MoveFirst();
		 doc->m_LstOrdoGrandeurEch.IsValid();
		 doc->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 IdGrandeur = doc->m_LstOrdoGrandeurEch.GetItem();
		Stream.Send(IdGrandeur);
	}

	// on sérialise la liste des idents src audience sel
	Count = doc->m_LstIdSrcAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcAudienceSel.MoveFirst();
		 doc->m_LstIdSrcAudienceSel.IsValid();
		 doc->m_LstIdSrcAudienceSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcAudienceSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des idents source de ciblage
	Count = doc->m_LstIdSrcCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcCiblageSel.MoveFirst();
		 doc->m_LstIdSrcCiblageSel.IsValid();
		 doc->m_LstIdSrcCiblageSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcCiblageSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des maps des terrains audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 doc->m_LstMapIdTerrainsAudienceSel.IsValid();
		 doc->m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdTerrainsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents terrains ciblage sélectionnés
	Count = doc->m_LstIdTerrainCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdTerrainCiblageSel.MoveFirst();
		 doc->m_LstIdTerrainCiblageSel.IsValid();
		 doc->m_LstIdTerrainCiblageSel.MoveNext())
	{
		JUnt32 IdTer = doc->m_LstIdTerrainCiblageSel.GetItem();
		Stream.Send(IdTer);
	}

	// on sérialise le map des formats sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdFormatsSel.MoveFirst();
		 doc->m_LstMapIdFormatsSel.IsValid();
		 doc->m_LstMapIdFormatsSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdFormatsSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item = MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents segment d'audience
	Count = doc->m_LstIdSegmentAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentAudienceSel.MoveFirst();
		 doc->m_LstIdSegmentAudienceSel.IsValid();
		 doc->m_LstIdSegmentAudienceSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentAudienceSel.GetItem(); 
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident segment pour ciblage
	Count = doc->m_LstIdSegmentCibleSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentCibleSel.MoveFirst();
		 doc->m_LstIdSegmentCibleSel.IsValid();
		 doc->m_LstIdSegmentCibleSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentCibleSel.GetItem();
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident filtre d'audience
	Count = doc->m_LstIdFiltreAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdFiltreAudienceSel.MoveFirst();
		 doc->m_LstIdFiltreAudienceSel.IsValid();
		 doc->m_LstIdFiltreAudienceSel.MoveNext())
	{
		JUnt32 IdFiltre = doc->m_LstIdFiltreAudienceSel.GetItem();
		Stream.Send(IdFiltre);
	}

	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);

	// Sauvegarde des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	Count = doc->m_OrdreEnteteEch.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_OrdreEnteteEch.GetCount(); i++)
		Stream.Send(doc->m_OrdreEnteteEch[i]);

	// Sauvegarde du tri en cours pour chaque module
	Count = doc->m_TabInfoTriCol.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_TabInfoTriCol.GetCount(); i++)
	{
		// Récup infos tri module
		CInfoTriCol InfoTriCol = doc->m_TabInfoTriCol[i];

		// Sauve no de la colonne courante du tri
		Stream.Send(InfoTriCol.NoCol);

		// Sauve mode du tri colonne courante
		Stream.Send(InfoTriCol.TriCroissant);
	}

	// En + de la version 1 -- on sérialise le Map des positions mapping
	Count = doc->m_MapPointMapping.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapPointMapping.MoveFirst();
		doc->m_MapPointMapping.IsValid();
		doc->m_MapPointMapping.MoveNext())
	{
		// Récupére clé et item d'une position mapping
		CKeyInfoMapping Key  	= doc->m_MapPointMapping.GetKey();
		CPointLibelle	Item	= doc->m_MapPointMapping.GetItem();

		// Stockage de la clef positions mapping
		Stream.Send(Key.m_IdTerrain);
		Stream.Send(Key.m_IdCible);
		Stream.Send(Key.m_IdSupport);
		Stream.Send(Key.m_IdFormat);
		Stream.Send(Key.m_NbInsertions);
		Stream.Send(Key.m_IdGrandeurAbs);
		Stream.Send(Key.m_IdGrandeurOrd);
		Stream.Send(Key.m_IdFiltreAudience);
		Stream.Send(Key.m_IdContexteLecture);
		Stream.Send(Key.m_bRepriseEnMain);
		Stream.Send(Key.m_bLNMMag);
		Stream.Send(Key.m_bLDPQtd);
		Stream.Send(Key.m_bLDP30J);
	
		// Stockage de l'item X / Y position mapping
		JFlt64 X = Item.m_XLib;
		Stream.Send(X);
		JFlt64 Y = Item.m_YLib;
		Stream.Send(Y);
	}
}

void CATPDocCSSerializer::ReadV2(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Index du module courant 
	JUnt32 InxModuleCur;
	Stream				.Recv(InxModuleCur);
	doc->SetModuleCourant(InxModuleCur);

	// on récupère le plateau
	JATPKEYPlateau			KEYPlateau;				KEYPlateau			.Recv(Stream);

	// on récupère la date tarifaire
	JDate					DateTarifaire;			DateTarifaire		.Recv(Stream);

	// on récupère le map des supports sélectionnés
	JUnt32					Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsSel.Add(Key) = Item;
	}

	// on récupère le map des couleurs affectées aux supports pour les graphes
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdColorSupports;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdColorSupports.Add(Key) = Item;
	}

	// On récupère le mode sélection couleurs
	JUnt32		ModeColorGraphe;					Stream.Recv(ModeColorGraphe);

	// on récupère map des cibles audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdCiblesAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdCiblesAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdCiblesAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdCiblesAudienceSel.AddTail().Swap(MapIdCiblesAudienceSel);
	}

	// on récupére le map des Grandeurs sélectionnées pour échelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);

		// Modif Data Exo !!!!!
		// On vérifie que item grandeur existant dans les grandeurs dispos (suite à grandeurs exogènes)
		doc->m_MapAbrGrandEch.MoveTo(Item);
		if (doc->m_MapAbrGrandEch.IsValid())
			MapIdGrandEchSel.Add(Key) = Item;
	}

	// on récupére le map des Grandeurs sélectionnées pour cumulativité
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandCumSel.Add(Key) = Item;
	}

	// on récupére Ident Grandeur Abscisse sélectionnée pour Mapping
	JInt32 IdGrandMapSelAbs;
	Stream				.Recv(IdGrandMapSelAbs);

	// on récupére Ident Grandeur Ordonnée sélectionnée pour Mapping
	JInt32 IdGrandMapSelOrd;
	Stream				.Recv(IdGrandMapSelOrd);

	// on récupére le map des Grandeurs sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandDupSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Echelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatEchSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Mapping
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatMapSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatMapSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Cumulativite
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatCumSel.Add(Key) = Item;
	}
	
	// on récupére le map des formats sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatDupSel.Add(Key) = Item;
	}

	// on récupére le nbinsertions sélectionné pour calcul
	JUnt32		NbInsertions;				Stream.Recv(NbInsertions);

	// on récupére liste ordonné des grandeurs échelle à afficher
	Stream				.Recv(Count);
	JList<JUnt32>		LstOrdoGrandeurEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Item;					Stream				.Recv(Item);
		LstOrdoGrandeurEch.AddTail() = Item;
	}

	// Attention provisoire (bidouille)
	if (Count < 16)
		LstOrdoGrandeurEch.AddTail() = 15;

	// on récupére les idents source d'audience
	JList <JUnt32> LstIdSrcAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcAudienceSel.AddTail() = Ident; 
	}

	// on récupére l'ident source de ciblage
	JList <JUnt32> LstIdSrcCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcCiblageSel.AddTail() = Ident; 
	}

	// on récupère map des terrains audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdTerrainsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdTerrainsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdTerrainsAudienceSel.AddTail().Swap(MapIdTerrainsAudienceSel);
	}

	// on récupére la liste des idents terrains ciblage
	JList <JUnt32> LstIdTerrainCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdTerrainCiblageSel.AddTail() = Ident; 
	}

	// on récupére le map des formats sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdFormatsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdFormatsSel.AddTail().Swap(MapIdFormatsSel);
	}

	// on récupére la liste des idents segment d'audience
	JList <JUnt32> LstIdSegmentAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentAudienceSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents segment pour ciblage
	JList <JUnt32> LstIdSegmentCibleSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentCibleSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents filtre d'audience
	JList <JUnt32> LstIdFiltreAudience;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdFiltreAudience.AddTail() = Ident; 
	}

	// on récupére l'offre
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	// Récupère ordre entetes échelle
	Stream				.Recv(Count);
	CArray<JUnt32> OrdreEnteteEch;
	OrdreEnteteEch.SetSize(Count);
	for (int i = 0; i < OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre;
		Stream.Recv(ValOrdre);
		OrdreEnteteEch[i] = ValOrdre;
	}		

	// Récupère informations tri sur tous les modules
	Stream				.Recv(Count);
	CArray<CInfoTriCol,CInfoTriCol&> TabOrdreTri;
	TabOrdreTri.SetSize(Count);
	for (int i = 0; i < TabOrdreTri.GetCount(); i++)
	{
		// Défini nvlle struct tri
		CInfoTriCol InfoTriCol;

		// Récup col courante
		Stream.Recv(InfoTriCol.NoCol);

		// Récup mode tri
		Stream.Recv(InfoTriCol.TriCroissant);
	}

	// on récupére le map des positions dans mapping
	Stream				.Recv(Count);

	JMap<CKeyInfoMapping,CPointLibelle>		MapPointMapping;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupération de la clef position mapping
		CKeyInfoMapping Key;
		Stream.Recv(Key.m_IdTerrain);
		Stream.Recv(Key.m_IdCible);
		Stream.Recv(Key.m_IdSupport);
		Stream.Recv(Key.m_IdFormat);
		Stream.Recv(Key.m_NbInsertions);
		Stream.Recv(Key.m_IdGrandeurAbs);
		Stream.Recv(Key.m_IdGrandeurOrd);
		Stream.Recv(Key.m_IdFiltreAudience);
		Stream.Recv(Key.m_IdContexteLecture);
		Stream.Recv(Key.m_bRepriseEnMain);
		Stream.Recv(Key.m_bLNMMag);
		Stream.Recv(Key.m_bLDPQtd);
		Stream.Recv(Key.m_bLDP30J);
	
		// Récupération de l'item X / Y position mapping
		CPointLibelle	Item;
		Stream		.Recv(Item.m_XLib);
		Stream		.Recv(Item.m_YLib);

		// Sauvegarde dans mapping
		MapPointMapping.Add(Key) = Item;
	}

	// Swap the elements
	doc->m_KEYPlateau					.Swap(KEYPlateau);
	doc->m_DateTarifaire				= DateTarifaire; 
	doc->m_MapIdSupportsSel				.Swap(MapIdSupportsSel);
	doc->m_MapIdColorSupports			.Swap(MapIdColorSupports); 
	doc->m_ModeColorGraphe				= ModeColorGraphe;
	doc->m_LstMapIdCiblesAudienceSel	.Swap(LstMapIdCiblesAudienceSel);

	// Les élmt grandeurs sélection pour les différents modules
	doc->m_MapIdGrandEchSel				.Swap(MapIdGrandEchSel);
	doc->m_MapIdGrandCumSel				.Swap(MapIdGrandCumSel);
	doc->m_IdGrandMapSelAbs				= IdGrandMapSelAbs;
	doc->m_IdGrandMapSelOrd				= IdGrandMapSelOrd;
	doc->m_MapIdGrandDupSel				.Swap(MapIdGrandDupSel);

	// Les élmt formats sélection pour les différents modules
	doc->m_MapIdFormatSpecifEch			.Swap(MapIdFormatEchSel);
	doc->m_MapIdFormatSpecifCum			.Swap(MapIdFormatCumSel);
	doc->m_MapIdFormatSpecifMap			.Swap(MapIdFormatMapSel);
	doc->m_MapIdFormatSpecifDup			.Swap(MapIdFormatDupSel);

	// Eléments sélections pour échelle
	doc->m_NbInsertions					= NbInsertions;
	doc->m_LstOrdoGrandeurEch			.Swap(LstOrdoGrandeurEch);

	doc->m_LstIdSrcAudienceSel			.Swap(LstIdSrcAudienceSel);
	doc->m_LstIdSrcCiblageSel			.Swap(LstIdSrcCiblageSel);
	doc->m_LstMapIdTerrainsAudienceSel	.Swap(LstMapIdTerrainsAudienceSel);
	doc->m_LstIdTerrainCiblageSel		.Swap(LstIdTerrainCiblageSel);
	doc->m_LstMapIdFormatsSel			.Swap(LstMapIdFormatsSel);
	doc->m_LstIdSegmentAudienceSel		.Swap(LstIdSegmentAudienceSel);
	doc->m_LstIdSegmentCibleSel			.Swap(LstIdSegmentCibleSel);
	doc->m_LstIdFiltreAudienceSel		.Swap(LstIdFiltreAudience);
	doc->m_Offre						.Swap(Offre);

	// Ordre des entetes échelles
	doc->m_OrdreEnteteEch.SetSize(OrdreEnteteEch.GetCount());
	for (int i=0; i<doc->m_OrdreEnteteEch.GetCount(); i++)
		doc->m_OrdreEnteteEch[i] = OrdreEnteteEch[i];

	// Tableau ordre de tris
	doc->m_TabInfoTriCol.SetSize(TabOrdreTri.GetCount());
	for (int i=0; i<doc->m_TabInfoTriCol.GetCount(); i++)
		doc->m_TabInfoTriCol[i] = TabOrdreTri[i];

	// Le map des positions mapping
	doc->m_MapPointMapping			.Swap(MapPointMapping);
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// SERIALISATION VERSION 3 (WRITE & READ)
//
void CATPDocCSSerializer::WriteV3(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Index du module courant 
	Stream.Send(doc->GetModuleCourant());

	// on sérialise le plateau
	doc->m_KEYPlateau.Send(Stream);

	// on sérialise la date tarifaire
	doc->m_DateTarifaire.Send(Stream);

	// on sérialise le map des supports sélectionnés
	JInt32 Count = doc->m_MapIdSupportsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsSel.MoveFirst();
		doc->m_MapIdSupportsSel.IsValid();
		doc->m_MapIdSupportsSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsSel.GetKey();
		JBool  Item = doc->m_MapIdSupportsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des supports sélectionnés
	Count = doc->m_MapIdSupportsVisible.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsVisible.MoveFirst();
		doc->m_MapIdSupportsVisible.IsValid();
		doc->m_MapIdSupportsVisible.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsVisible.GetKey();
		JBool  Item = doc->m_MapIdSupportsVisible.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des couleurs supports sélectionnées
	Count = doc->m_MapIdColorSupports.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdColorSupports.MoveFirst();
		doc->m_MapIdColorSupports.IsValid();
		doc->m_MapIdColorSupports.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdColorSupports.GetKey();
		JUnt32  Item = doc->m_MapIdColorSupports.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// On sérialise le mode sélection couleurs
	Stream.Send(doc->m_ModeColorGraphe);

	// on sérialise la liste des maps des cibles audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdCiblesAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdCiblesAudienceSel.MoveFirst();
		 doc->m_LstMapIdCiblesAudienceSel.IsValid();
		 doc->m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdCiblesAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise le map des Grandeurs sélectionnées pour échelle
	Count = doc->m_MapIdGrandEchSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandEchSel.MoveFirst();
		doc->m_MapIdGrandEchSel.IsValid();
		doc->m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandEchSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandEchSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Grandeurs sélectionnées pour cumulativité
	Count = doc->m_MapIdGrandCumSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandCumSel.MoveFirst();
		doc->m_MapIdGrandCumSel.IsValid();
		doc->m_MapIdGrandCumSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandCumSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandCumSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Mapping
	JInt32 IdGrand = doc->m_IdGrandMapSelAbs;
	Stream				.Send(IdGrand);

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Ordonnée
	IdGrand = doc->m_IdGrandMapSelOrd;
	Stream				.Send(IdGrand);

	// on sérialise le map des Grandeurs sélectionnées pour Duplication
	Count = doc->m_MapIdGrandDupSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandDupSel.MoveFirst();
		doc->m_MapIdGrandDupSel.IsValid();
		doc->m_MapIdGrandDupSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandDupSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandDupSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour échelle
	Count = doc->m_MapIdFormatSpecifEch.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifEch.MoveFirst();
		doc->m_MapIdFormatSpecifEch.IsValid();
		doc->m_MapIdFormatSpecifEch.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifEch.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour mapping
	Count = doc->m_MapIdFormatSpecifMap.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifMap.MoveFirst();
		doc->m_MapIdFormatSpecifMap.IsValid();
		doc->m_MapIdFormatSpecifMap.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifMap.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifMap.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour cumulativite
	Count = doc->m_MapIdFormatSpecifCum.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifCum.MoveFirst();
		doc->m_MapIdFormatSpecifCum.IsValid();
		doc->m_MapIdFormatSpecifCum.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifCum.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifCum.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour duplication
	Count = doc->m_MapIdFormatSpecifDup.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifDup.MoveFirst();
		doc->m_MapIdFormatSpecifDup.IsValid();
		doc->m_MapIdFormatSpecifDup.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifDup.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifDup.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le nbinsertions sélectionné pour calcul
	Stream.Send(doc->m_NbInsertions);

	// on sérialise liste ordonnancement grandeurs échelle à afficher
	Count = doc->m_LstOrdoGrandeurEch.GetCount();
	Stream.Send(Count);
	for (doc->m_LstOrdoGrandeurEch.MoveFirst();
		 doc->m_LstOrdoGrandeurEch.IsValid();
		 doc->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 IdGrandeur = doc->m_LstOrdoGrandeurEch.GetItem();
		Stream.Send(IdGrandeur);
	}

	// on sérialise la liste des idents src audience sel
	Count = doc->m_LstIdSrcAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcAudienceSel.MoveFirst();
		 doc->m_LstIdSrcAudienceSel.IsValid();
		 doc->m_LstIdSrcAudienceSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcAudienceSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des idents source de ciblage
	Count = doc->m_LstIdSrcCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcCiblageSel.MoveFirst();
		 doc->m_LstIdSrcCiblageSel.IsValid();
		 doc->m_LstIdSrcCiblageSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcCiblageSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des maps des terrains audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 doc->m_LstMapIdTerrainsAudienceSel.IsValid();
		 doc->m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdTerrainsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents terrains ciblage sélectionnés
	Count = doc->m_LstIdTerrainCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdTerrainCiblageSel.MoveFirst();
		 doc->m_LstIdTerrainCiblageSel.IsValid();
		 doc->m_LstIdTerrainCiblageSel.MoveNext())
	{
		JUnt32 IdTer = doc->m_LstIdTerrainCiblageSel.GetItem();
		Stream.Send(IdTer);
	}

	// on sérialise le map des formats sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdFormatsSel.MoveFirst();
		 doc->m_LstMapIdFormatsSel.IsValid();
		 doc->m_LstMapIdFormatsSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdFormatsSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item = MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des maps des segments audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdSegmentsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdSegmentsAudienceSel.MoveFirst();
		 doc->m_LstMapIdSegmentsAudienceSel.IsValid();
		 doc->m_LstMapIdSegmentsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdSegmentsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents segment d'audience
	Count = doc->m_LstIdSegmentAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentAudienceSel.MoveFirst();
		 doc->m_LstIdSegmentAudienceSel.IsValid();
		 doc->m_LstIdSegmentAudienceSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentAudienceSel.GetItem(); 
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident segment pour ciblage
	Count = doc->m_LstIdSegmentCibleSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentCibleSel.MoveFirst();
		 doc->m_LstIdSegmentCibleSel.IsValid();
		 doc->m_LstIdSegmentCibleSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentCibleSel.GetItem();
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident filtre d'audience
	Count = doc->m_LstIdFiltreAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdFiltreAudienceSel.MoveFirst();
		 doc->m_LstIdFiltreAudienceSel.IsValid();
		 doc->m_LstIdFiltreAudienceSel.MoveNext())
	{
		JUnt32 IdFiltre = doc->m_LstIdFiltreAudienceSel.GetItem();
		Stream.Send(IdFiltre);
	}

	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);
	
	// Sauvegarde des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	Count = doc->m_OrdreEnteteEch.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_OrdreEnteteEch.GetCount(); i++)
		Stream.Send(doc->m_OrdreEnteteEch[i]);

	// Sauvegarde du tri en cours pour chaque module
	Count = doc->m_TabInfoTriCol.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_TabInfoTriCol.GetCount(); i++)
	{
		// Récup infos tri module
		CInfoTriCol InfoTriCol = doc->m_TabInfoTriCol[i];

		// Sauve no de la colonne courante du tri
		Stream.Send(InfoTriCol.NoCol);

		// Sauve mode du tri colonne courante
		Stream.Send(InfoTriCol.TriCroissant);
	}

	// En + de la version 1 -- on sérialise le Map des positions mapping
	Count = doc->m_MapPointMapping.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapPointMapping.MoveFirst();
		doc->m_MapPointMapping.IsValid();
		doc->m_MapPointMapping.MoveNext())
	{
		// Récupére clé et item d'une position mapping
		CKeyInfoMapping Key  	= doc->m_MapPointMapping.GetKey();
		CPointLibelle	Item	= doc->m_MapPointMapping.GetItem();

		// Stockage de la clef positions mapping
		Stream.Send(Key.m_IdTerrain);
		Stream.Send(Key.m_IdCible);
		Stream.Send(Key.m_IdCibleAff);
		Stream.Send(Key.m_IdSupport);
		Stream.Send(Key.m_IdSegment);
		Stream.Send(Key.m_IdFormat);
		Stream.Send(Key.m_IdMoteur);
		Stream.Send(Key.m_NbInsertions);
		Stream.Send(Key.m_IdGrandeurAbs);
		Stream.Send(Key.m_IdGrandeurOrd);
		Stream.Send(Key.m_IdFiltreAudience);
		Stream.Send(Key.m_IdContexteLecture);
		Stream.Send(Key.m_bRepriseEnMain);
		Stream.Send(Key.m_bLNMMag);
		Stream.Send(Key.m_bLDPQtd);
		Stream.Send(Key.m_bLDP30J);
	
		// Stockage de l'item X / Y position mapping
		JFlt64 X = Item.m_XLib;
		Stream.Send(X);
		JFlt64 Y = Item.m_YLib;
		Stream.Send(Y);
	}
}

void CATPDocCSSerializer::ReadV3(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Index du module courant 
	JUnt32 InxModuleCur;
	Stream				.Recv(InxModuleCur);
	doc->SetModuleCourant(InxModuleCur);

	// on récupère le plateau
	JATPKEYPlateau			KEYPlateau;				KEYPlateau			.Recv(Stream);

	// on récupère la date tarifaire
	JDate					DateTarifaire;			DateTarifaire		.Recv(Stream);

	// on récupère le map des supports sélectionnés
	JUnt32					Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsSel.Add(Key) = Item;
	}

	// on récupère le map des supports sélectionnés
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsVisible;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsVisible.Add(Key) = Item;
	}

	// on récupère le map des couleurs affectées aux supports pour les graphes
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdColorSupports;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdColorSupports.Add(Key) = Item;
	}

	// On récupère le mode sélection couleurs
	JUnt32		ModeColorGraphe;					Stream.Recv(ModeColorGraphe);

	// on récupère map des cibles audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdCiblesAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdCiblesAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdCiblesAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdCiblesAudienceSel.AddTail().Swap(MapIdCiblesAudienceSel);
	}

	// on récupére le map des Grandeurs sélectionnées pour échelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);

		// Modif Data Exo !!!!!
		// On vérifie que item grandeur existant dans les grandeurs dispos (suite à grandeurs exogènes)
		doc->m_MapAbrGrandEch.MoveTo(Item);
		if (doc->m_MapAbrGrandEch.IsValid())
			MapIdGrandEchSel.Add(Key) = Item;
	}

	// on récupére le map des Grandeurs sélectionnées pour cumulativité
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandCumSel.Add(Key) = Item;
	}

	// on récupére Ident Grandeur Abscisse sélectionnée pour Mapping
	JInt32 IdGrandMapSelAbs;
	Stream				.Recv(IdGrandMapSelAbs);

	// on récupére Ident Grandeur Ordonnée sélectionnée pour Mapping
	JInt32 IdGrandMapSelOrd;
	Stream				.Recv(IdGrandMapSelOrd);

	// on récupére le map des Grandeurs sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandDupSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Echelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatEchSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Mapping
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatMapSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatMapSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Cumulativite
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatCumSel.Add(Key) = Item;
	}
	
	// on récupére le map des formats sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatDupSel.Add(Key) = Item;
	}

	// on récupére le nbinsertions sélectionné pour calcul
	JUnt32		NbInsertions;				Stream.Recv(NbInsertions);

	// on récupére liste ordonné des grandeurs échelle à afficher
	Stream				.Recv(Count);
	JList<JUnt32>		LstOrdoGrandeurEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Item;					Stream				.Recv(Item);
		LstOrdoGrandeurEch.AddTail() = Item;
	}

	// Attention provisoire (bidouille)
	if (Count < 16)
		LstOrdoGrandeurEch.AddTail() = 15;

	// on récupére les idents source d'audience
	JList <JUnt32> LstIdSrcAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcAudienceSel.AddTail() = Ident; 
	}

	// on récupére l'ident source de ciblage
	JList <JUnt32> LstIdSrcCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcCiblageSel.AddTail() = Ident; 
	}

	// on récupère map des terrains audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdTerrainsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdTerrainsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdTerrainsAudienceSel.AddTail().Swap(MapIdTerrainsAudienceSel);
	}

	// on récupére la liste des idents terrains ciblage
	JList <JUnt32> LstIdTerrainCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdTerrainCiblageSel.AddTail() = Ident; 
	}

	// on récupére le map des formats sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdFormatsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdFormatsSel.AddTail().Swap(MapIdFormatsSel);
	}

	// on récupère map des segments audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdSegmentsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdSegmentsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdSegmentsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdSegmentsAudienceSel.AddTail().Swap(MapIdSegmentsAudienceSel);
	}

	// on récupére la liste des idents segment d'audience
	JList <JUnt32> LstIdSegmentAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentAudienceSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents segment pour ciblage
	JList <JUnt32> LstIdSegmentCibleSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentCibleSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents filtre d'audience
	JList <JUnt32> LstIdFiltreAudience;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdFiltreAudience.AddTail() = Ident; 
	}

	// on récupére l'offre
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);
	
	// Récupère ordre entetes échelle
	Stream				.Recv(Count);
	CArray<JUnt32> OrdreEnteteEch;
	OrdreEnteteEch.SetSize(Count);
	for (int i = 0; i < OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre;
		Stream.Recv(ValOrdre);
		OrdreEnteteEch[i] = ValOrdre;
	}		

	// Récupère informations tri sur tous les modules
	Stream				.Recv(Count);
	CArray<CInfoTriCol,CInfoTriCol&> TabOrdreTri;
	TabOrdreTri.SetSize(Count);
	for (int i = 0; i < TabOrdreTri.GetCount(); i++)
	{
		// Défini nvlle struct tri
		CInfoTriCol InfoTriCol;

		// Récup col courante
		Stream.Recv(InfoTriCol.NoCol);

		// Récup mode tri
		Stream.Recv(InfoTriCol.TriCroissant);
	}

	// on récupére le map des positions dans mapping
	Stream				.Recv(Count);

	JMap<CKeyInfoMapping,CPointLibelle>		MapPointMapping;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupération de la clef position mapping
		CKeyInfoMapping Key;
		Stream.Recv(Key.m_IdTerrain);
		Stream.Recv(Key.m_IdCible);
		Stream.Recv(Key.m_IdCibleAff);
		Stream.Recv(Key.m_IdSupport);
		Stream.Recv(Key.m_IdSegment);
		Stream.Recv(Key.m_IdFormat);
		Stream.Recv(Key.m_IdMoteur);
		Stream.Recv(Key.m_NbInsertions);
		Stream.Recv(Key.m_IdGrandeurAbs);
		Stream.Recv(Key.m_IdGrandeurOrd);
		Stream.Recv(Key.m_IdFiltreAudience);
		Stream.Recv(Key.m_IdContexteLecture);
		Stream.Recv(Key.m_bRepriseEnMain);
		Stream.Recv(Key.m_bLNMMag);
		Stream.Recv(Key.m_bLDPQtd);
		Stream.Recv(Key.m_bLDP30J);
	
		// Récupération de l'item X / Y position mapping
		CPointLibelle	Item;
		Stream		.Recv(Item.m_XLib);
		Stream		.Recv(Item.m_YLib);

		// Sauvegarde dans mapping
		MapPointMapping.Add(Key) = Item;
	}

	// Swap the elements
	doc->m_KEYPlateau					.Swap(KEYPlateau);
	doc->m_DateTarifaire				= DateTarifaire; 
	doc->m_MapIdSupportsSel				.Swap(MapIdSupportsSel);
	doc->m_MapIdSupportsVisible			.Swap(MapIdSupportsVisible);
	doc->m_MapIdColorSupports			.Swap(MapIdColorSupports); 
	doc->m_ModeColorGraphe				= ModeColorGraphe;
	doc->m_LstMapIdCiblesAudienceSel	.Swap(LstMapIdCiblesAudienceSel);

	// Les élmt grandeurs sélection pour les différents modules
	doc->m_MapIdGrandEchSel				.Swap(MapIdGrandEchSel);
	doc->m_MapIdGrandCumSel				.Swap(MapIdGrandCumSel);
	doc->m_IdGrandMapSelAbs				= IdGrandMapSelAbs;
	doc->m_IdGrandMapSelOrd				= IdGrandMapSelOrd;
	doc->m_MapIdGrandDupSel				.Swap(MapIdGrandDupSel);

	// Les élmt formats sélection pour les différents modules
	doc->m_MapIdFormatSpecifEch			.Swap(MapIdFormatEchSel);
	doc->m_MapIdFormatSpecifCum			.Swap(MapIdFormatCumSel);
	doc->m_MapIdFormatSpecifMap			.Swap(MapIdFormatMapSel);
	doc->m_MapIdFormatSpecifDup			.Swap(MapIdFormatDupSel);

	// Eléments sélections pour échelle
	doc->m_NbInsertions					= NbInsertions;
	doc->m_LstOrdoGrandeurEch			.Swap(LstOrdoGrandeurEch);
	doc->m_LstIdSrcAudienceSel			.Swap(LstIdSrcAudienceSel);
	doc->m_LstIdSrcCiblageSel			.Swap(LstIdSrcCiblageSel);
	doc->m_LstMapIdTerrainsAudienceSel	.Swap(LstMapIdTerrainsAudienceSel);
	doc->m_LstIdTerrainCiblageSel		.Swap(LstIdTerrainCiblageSel);
	doc->m_LstMapIdFormatsSel			.Swap(LstMapIdFormatsSel);
	doc->m_LstMapIdSegmentsAudienceSel	.Swap(LstMapIdSegmentsAudienceSel);
	doc->m_LstIdSegmentAudienceSel		.Swap(LstIdSegmentAudienceSel);
	doc->m_LstIdSegmentCibleSel			.Swap(LstIdSegmentCibleSel);
	doc->m_LstIdFiltreAudienceSel		.Swap(LstIdFiltreAudience);
	doc->m_Offre						.Swap(Offre);

	// Ordre des entetes échelles
	doc->m_OrdreEnteteEch.SetSize(OrdreEnteteEch.GetCount());
	for (int i=0; i<doc->m_OrdreEnteteEch.GetCount(); i++)
		doc->m_OrdreEnteteEch[i] = OrdreEnteteEch[i];

	// Tableau ordre de tris
	doc->m_TabInfoTriCol.SetSize(TabOrdreTri.GetCount());
	for (int i=0; i<doc->m_TabInfoTriCol.GetCount(); i++)
		doc->m_TabInfoTriCol[i] = TabOrdreTri[i];

	// Le map des positions mapping
	doc->m_MapPointMapping			.Swap(MapPointMapping);
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// SERIALISATION VERSION 4 (WRITE & READ)
//
void CATPDocCSSerializer::WriteV4(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Sauvegarde de l'offre au début désormais (facilitera transfert creuset <> cs)
	doc->m_Offre						.Send(Stream);

	// Index du module courant 
	Stream.Send(doc->GetModuleCourant());

	// on sérialise le plateau
	doc->m_KEYPlateau.Send(Stream);

	// on sérialise la date tarifaire
	doc->m_DateTarifaire.Send(Stream);

	// on sérialise le map des supports sélectionnés
	JInt32 Count = doc->m_MapIdSupportsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsSel.MoveFirst();
		doc->m_MapIdSupportsSel.IsValid();
		doc->m_MapIdSupportsSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsSel.GetKey();
		JBool  Item = doc->m_MapIdSupportsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des couleurs supports sélectionnées
	Count = doc->m_MapIdColorSupports.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdColorSupports.MoveFirst();
		doc->m_MapIdColorSupports.IsValid();
		doc->m_MapIdColorSupports.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdColorSupports.GetKey();
		JUnt32  Item = doc->m_MapIdColorSupports.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// On sérialise le mode sélection couleurs
	Stream.Send(doc->m_ModeColorGraphe);

	// Reporter après écriture de l'offre
	// on sérialise la liste des maps des cibles audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdCiblesAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdCiblesAudienceSel.MoveFirst();
		 doc->m_LstMapIdCiblesAudienceSel.IsValid();
		 doc->m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdCiblesAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise le map des Grandeurs sélectionnées pour échelle
	Count = doc->m_MapIdGrandEchSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandEchSel.MoveFirst();
		doc->m_MapIdGrandEchSel.IsValid();
		doc->m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandEchSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandEchSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Grandeurs sélectionnées pour cumulativité
	Count = doc->m_MapIdGrandCumSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandCumSel.MoveFirst();
		doc->m_MapIdGrandCumSel.IsValid();
		doc->m_MapIdGrandCumSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandCumSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandCumSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Mapping
	JInt32 IdGrand = doc->m_IdGrandMapSelAbs;
	Stream				.Send(IdGrand);

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Ordonnée
	IdGrand = doc->m_IdGrandMapSelOrd;
	Stream				.Send(IdGrand);

	// on sérialise le map des Grandeurs sélectionnées pour Duplication
	Count = doc->m_MapIdGrandDupSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandDupSel.MoveFirst();
		doc->m_MapIdGrandDupSel.IsValid();
		doc->m_MapIdGrandDupSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandDupSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandDupSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour échelle
	Count = doc->m_MapIdFormatSpecifEch.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifEch.MoveFirst();
		doc->m_MapIdFormatSpecifEch.IsValid();
		doc->m_MapIdFormatSpecifEch.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifEch.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour mapping
	Count = doc->m_MapIdFormatSpecifMap.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifMap.MoveFirst();
		doc->m_MapIdFormatSpecifMap.IsValid();
		doc->m_MapIdFormatSpecifMap.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifMap.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifMap.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour cumulativite
	Count = doc->m_MapIdFormatSpecifCum.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifCum.MoveFirst();
		doc->m_MapIdFormatSpecifCum.IsValid();
		doc->m_MapIdFormatSpecifCum.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifCum.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifCum.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour duplication
	Count = doc->m_MapIdFormatSpecifDup.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifDup.MoveFirst();
		doc->m_MapIdFormatSpecifDup.IsValid();
		doc->m_MapIdFormatSpecifDup.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifDup.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifDup.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le nbinsertions sélectionné pour calcul
	Stream.Send(doc->m_NbInsertions);

	// on sérialise liste ordonnancement grandeurs échelle à afficher
	Count = doc->m_LstOrdoGrandeurEch.GetCount();
	Stream.Send(Count);
	for (doc->m_LstOrdoGrandeurEch.MoveFirst();
		 doc->m_LstOrdoGrandeurEch.IsValid();
		 doc->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 IdGrandeur = doc->m_LstOrdoGrandeurEch.GetItem();
		Stream.Send(IdGrandeur);
	}

	// on sérialise la liste des idents src audience sel
	Count = doc->m_LstIdSrcAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcAudienceSel.MoveFirst();
		 doc->m_LstIdSrcAudienceSel.IsValid();
		 doc->m_LstIdSrcAudienceSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcAudienceSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des idents source de ciblage
	Count = doc->m_LstIdSrcCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcCiblageSel.MoveFirst();
		 doc->m_LstIdSrcCiblageSel.IsValid();
		 doc->m_LstIdSrcCiblageSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcCiblageSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des maps des terrains audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 doc->m_LstMapIdTerrainsAudienceSel.IsValid();
		 doc->m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdTerrainsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents terrains ciblage sélectionnés
	Count = doc->m_LstIdTerrainCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdTerrainCiblageSel.MoveFirst();
		 doc->m_LstIdTerrainCiblageSel.IsValid();
		 doc->m_LstIdTerrainCiblageSel.MoveNext())
	{
		JUnt32 IdTer = doc->m_LstIdTerrainCiblageSel.GetItem();
		Stream.Send(IdTer);
	}

	// on sérialise le map des formats sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdFormatsSel.MoveFirst();
		 doc->m_LstMapIdFormatsSel.IsValid();
		 doc->m_LstMapIdFormatsSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdFormatsSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item = MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents segment d'audience
	Count = doc->m_LstIdSegmentAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentAudienceSel.MoveFirst();
		 doc->m_LstIdSegmentAudienceSel.IsValid();
		 doc->m_LstIdSegmentAudienceSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentAudienceSel.GetItem(); 
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident segment pour ciblage
	Count = doc->m_LstIdSegmentCibleSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentCibleSel.MoveFirst();
		 doc->m_LstIdSegmentCibleSel.IsValid();
		 doc->m_LstIdSegmentCibleSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentCibleSel.GetItem();
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident filtre d'audience
	Count = doc->m_LstIdFiltreAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdFiltreAudienceSel.MoveFirst();
		 doc->m_LstIdFiltreAudienceSel.IsValid();
		 doc->m_LstIdFiltreAudienceSel.MoveNext())
	{
		JUnt32 IdFiltre = doc->m_LstIdFiltreAudienceSel.GetItem();
		Stream.Send(IdFiltre);
	}

	/*
	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);
	*/

	// Sauvegarde des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	Count = doc->m_OrdreEnteteEch.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_OrdreEnteteEch.GetCount(); i++)
		Stream.Send(doc->m_OrdreEnteteEch[i]);

	// Sauvegarde du tri en cours pour chaque module
	Count = doc->m_TabInfoTriCol.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_TabInfoTriCol.GetCount(); i++)
	{
		// Récup infos tri module
		CInfoTriCol InfoTriCol = doc->m_TabInfoTriCol[i];

		// Sauve no de la colonne courante du tri
		Stream.Send(InfoTriCol.NoCol);

		// Sauve mode du tri colonne courante
		Stream.Send(InfoTriCol.TriCroissant);
	}

	// En + de la version 1 -- on sérialise le Map des positions mapping
	Count = doc->m_MapPointMapping.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapPointMapping.MoveFirst();
		doc->m_MapPointMapping.IsValid();
		doc->m_MapPointMapping.MoveNext())
	{
		// Récupére clé et item d'une position mapping
		CKeyInfoMapping Key  	= doc->m_MapPointMapping.GetKey();
		CPointLibelle	Item	= doc->m_MapPointMapping.GetItem();

		// Stockage de la clef positions mapping
		Stream.Send(Key.m_IdTerrain);
		Stream.Send(Key.m_IdCible);
		Stream.Send(Key.m_IdCibleAff);
		Stream.Send(Key.m_IdSupport);
		Stream.Send(Key.m_IdSegment);
		Stream.Send(Key.m_IdFormat);
		Stream.Send(Key.m_IdMoteur);
		Stream.Send(Key.m_NbInsertions);
		Stream.Send(Key.m_IdGrandeurAbs);
		Stream.Send(Key.m_IdGrandeurOrd);
		Stream.Send(Key.m_IdFiltreAudience);
		Stream.Send(Key.m_IdContexteLecture);
		Stream.Send(Key.m_bRepriseEnMain);
		Stream.Send(Key.m_bLNMMag);
		Stream.Send(Key.m_bLDPQtd);
		Stream.Send(Key.m_bLDP30J);
	
		// Stockage de l'item X / Y position mapping
		JFlt64 X = Item.m_XLib;
		Stream.Send(X);
		JFlt64 Y = Item.m_YLib;
		Stream.Send(Y);
	}
}

void CATPDocCSSerializer::ReadV4(JStream & Stream, JATPDocChoixSupport * doc)
{
	// on récupére l'offre au début désormais (pour récup étude creuset <> Cs)
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	// Index du module courant 
	JUnt32 InxModuleCur;
	Stream				.Recv(InxModuleCur);
	doc->SetModuleCourant(InxModuleCur);

	// on récupère le plateau
	JATPKEYPlateau			KEYPlateau;				KEYPlateau			.Recv(Stream);

	// on récupère la date tarifaire
	JDate					DateTarifaire;			DateTarifaire		.Recv(Stream);

	// on récupère le map des supports sélectionnés
	JUnt32					Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsSel.Add(Key) = Item;
	}

	// on récupère le map des couleurs affectées aux supports pour les graphes
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdColorSupports;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdColorSupports.Add(Key) = Item;
	}

	// On récupère le mode sélection couleurs
	JUnt32		ModeColorGraphe;					Stream.Recv(ModeColorGraphe);

	// On récupère map des cibles audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdCiblesAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdCiblesAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdCiblesAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdCiblesAudienceSel.AddTail().Swap(MapIdCiblesAudienceSel);
	}

	// on récupére le map des Grandeurs sélectionnées pour échelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);

		// Modif Data Exo !!!!!
		// On vérifie que item grandeur existant dans les grandeurs dispos (suite à grandeurs exogènes)
		doc->m_MapAbrGrandEch.MoveTo(Item);
		if (doc->m_MapAbrGrandEch.IsValid())
			MapIdGrandEchSel.Add(Key) = Item;
	}

	// on récupére le map des Grandeurs sélectionnées pour cumulativité
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandCumSel.Add(Key) = Item;
	}

	// on récupére Ident Grandeur Abscisse sélectionnée pour Mapping
	JInt32 IdGrandMapSelAbs;
	Stream				.Recv(IdGrandMapSelAbs);

	// on récupére Ident Grandeur Ordonnée sélectionnée pour Mapping
	JInt32 IdGrandMapSelOrd;
	Stream				.Recv(IdGrandMapSelOrd);

	// on récupére le map des Grandeurs sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandDupSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Echelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatEchSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Mapping
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatMapSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatMapSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Cumulativite
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatCumSel.Add(Key) = Item;
	}
	
	// on récupére le map des formats sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatDupSel.Add(Key) = Item;
	}

	// on récupére le nbinsertions sélectionné pour calcul
	JUnt32		NbInsertions;				Stream.Recv(NbInsertions);

	// on récupére liste ordonné des grandeurs échelle à afficher
	Stream				.Recv(Count);
	JList<JUnt32>		LstOrdoGrandeurEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Item;					Stream				.Recv(Item);
		LstOrdoGrandeurEch.AddTail() = Item;
	}

	// Attention provisoire (bidouille)
	if (Count < 16)
		LstOrdoGrandeurEch.AddTail() = 15;

	// on récupére les idents source d'audience
	JList <JUnt32> LstIdSrcAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcAudienceSel.AddTail() = Ident; 
	}

	// on récupére l'ident source de ciblage
	JList <JUnt32> LstIdSrcCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcCiblageSel.AddTail() = Ident; 
	}

	// on récupère map des terrains audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdTerrainsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdTerrainsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdTerrainsAudienceSel.AddTail().Swap(MapIdTerrainsAudienceSel);
	}

	// on récupére la liste des idents terrains ciblage
	JList <JUnt32> LstIdTerrainCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdTerrainCiblageSel.AddTail() = Ident; 
	}

	// on récupére le map des formats sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdFormatsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdFormatsSel.AddTail().Swap(MapIdFormatsSel);
	}

	// on récupére la liste des idents segment d'audience
	JList <JUnt32> LstIdSegmentAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentAudienceSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents segment pour ciblage
	JList <JUnt32> LstIdSegmentCibleSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentCibleSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents filtre d'audience
	JList <JUnt32> LstIdFiltreAudience;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdFiltreAudience.AddTail() = Ident; 
	}

	/*
	// on récupére l'offre
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);
	*/
		
	// Récupère ordre entetes échelle
	Stream				.Recv(Count);
	CArray<JUnt32> OrdreEnteteEch;
	OrdreEnteteEch.SetSize(Count);
	for (int i = 0; i < OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre;
		Stream.Recv(ValOrdre);
		OrdreEnteteEch[i] = ValOrdre;
	}		

	// Récupère informations tri sur tous les modules
	Stream				.Recv(Count);
	CArray<CInfoTriCol,CInfoTriCol&> TabOrdreTri;
	TabOrdreTri.SetSize(Count);
	for (int i = 0; i < TabOrdreTri.GetCount(); i++)
	{
		// Défini nvlle struct tri
		CInfoTriCol InfoTriCol;

		// Récup col courante
		Stream.Recv(InfoTriCol.NoCol);

		// Récup mode tri
		Stream.Recv(InfoTriCol.TriCroissant);
	}

	// on récupére le map des positions dans mapping
	Stream				.Recv(Count);

	JMap<CKeyInfoMapping,CPointLibelle>		MapPointMapping;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupération de la clef position mapping
		CKeyInfoMapping Key;
		Stream.Recv(Key.m_IdTerrain);
		Stream.Recv(Key.m_IdCible);
		Stream.Recv(Key.m_IdSupport);
		Stream.Recv(Key.m_IdFormat);
		Stream.Recv(Key.m_IdMoteur);
		Stream.Recv(Key.m_NbInsertions);
		Stream.Recv(Key.m_IdGrandeurAbs);
		Stream.Recv(Key.m_IdGrandeurOrd);
		Stream.Recv(Key.m_IdFiltreAudience);
		Stream.Recv(Key.m_IdContexteLecture);
		Stream.Recv(Key.m_bRepriseEnMain);
		Stream.Recv(Key.m_bLNMMag);
		Stream.Recv(Key.m_bLDPQtd);
		Stream.Recv(Key.m_bLDP30J);
	
		// Récupération de l'item X / Y position mapping
		CPointLibelle	Item;
		Stream		.Recv(Item.m_XLib);
		Stream		.Recv(Item.m_YLib);

		// Sauvegarde dans mapping
		MapPointMapping.Add(Key) = Item;
	}

	// Swap the elements
	doc->m_KEYPlateau					.Swap(KEYPlateau);
	doc->m_DateTarifaire				= DateTarifaire; 
	doc->m_MapIdSupportsSel				.Swap(MapIdSupportsSel);
	doc->m_MapIdColorSupports			.Swap(MapIdColorSupports); 
	doc->m_ModeColorGraphe				= ModeColorGraphe;
	doc->m_LstMapIdCiblesAudienceSel	.Swap(LstMapIdCiblesAudienceSel);

	// Les élmt grandeurs sélection pour les différents modules
	doc->m_MapIdGrandEchSel				.Swap(MapIdGrandEchSel);
	doc->m_MapIdGrandCumSel				.Swap(MapIdGrandCumSel);
	doc->m_IdGrandMapSelAbs				= IdGrandMapSelAbs;
	doc->m_IdGrandMapSelOrd				= IdGrandMapSelOrd;
	doc->m_MapIdGrandDupSel				.Swap(MapIdGrandDupSel);

	// Les élmt formats sélection pour les différents modules
	doc->m_MapIdFormatSpecifEch			.Swap(MapIdFormatEchSel);
	doc->m_MapIdFormatSpecifCum			.Swap(MapIdFormatCumSel);
	doc->m_MapIdFormatSpecifMap			.Swap(MapIdFormatMapSel);
	doc->m_MapIdFormatSpecifDup			.Swap(MapIdFormatDupSel);

	// Eléments sélections pour échelle
	doc->m_NbInsertions					= NbInsertions;
	doc->m_LstOrdoGrandeurEch			.Swap(LstOrdoGrandeurEch);

	doc->m_LstIdSrcAudienceSel			.Swap(LstIdSrcAudienceSel);
	doc->m_LstIdSrcCiblageSel			.Swap(LstIdSrcCiblageSel);
	doc->m_LstMapIdTerrainsAudienceSel	.Swap(LstMapIdTerrainsAudienceSel);
	doc->m_LstIdTerrainCiblageSel		.Swap(LstIdTerrainCiblageSel);
	doc->m_LstMapIdFormatsSel			.Swap(LstMapIdFormatsSel);
	doc->m_LstIdSegmentAudienceSel		.Swap(LstIdSegmentAudienceSel);
	doc->m_LstIdSegmentCibleSel			.Swap(LstIdSegmentCibleSel);
	doc->m_LstIdFiltreAudienceSel		.Swap(LstIdFiltreAudience);
	doc->m_Offre						.Swap(Offre);

	// Ordre des entetes échelles
	doc->m_OrdreEnteteEch.SetSize(OrdreEnteteEch.GetCount());
	for (int i=0; i<doc->m_OrdreEnteteEch.GetCount(); i++)
		doc->m_OrdreEnteteEch[i] = OrdreEnteteEch[i];

	// Tableau ordre de tris
	doc->m_TabInfoTriCol.SetSize(TabOrdreTri.GetCount());
	for (int i=0; i<doc->m_TabInfoTriCol.GetCount(); i++)
		doc->m_TabInfoTriCol[i] = TabOrdreTri[i];

	// Le map des positions mapping
	doc->m_MapPointMapping			.Swap(MapPointMapping);
}


//////////////////////////////////////////////////////////////////////////////////////////
// EN ATTENTE // SERIALISATION SUITE AUX MODIFS CONCERNANT RECUPERATION DOSSIER ENTRE UTILISATEURS
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// SERIALISATION VERSION 5 (WRITE & READ)
//
void CATPDocCSSerializer::WriteV5(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Sauvegarde de l'offre au début désormais (facilitera transfert creuset <> cs)
	doc->m_Offre						.Send(Stream);

	// Index du module courant 
	Stream.Send(doc->GetModuleCourant());

	// on sérialise le plateau
	doc->m_KEYPlateau.Send(Stream);

	// on sérialise la date tarifaire
	doc->m_DateTarifaire.Send(Stream);

	// on sérialise le map des supports sélectionnés
	JInt32 Count = doc->m_MapIdSupportsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsSel.MoveFirst();
		doc->m_MapIdSupportsSel.IsValid();
		doc->m_MapIdSupportsSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsSel.GetKey();
		JBool  Item = doc->m_MapIdSupportsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// Nouveauté version 5
	// on sérialise le map des supports visibles sélectionnés
	Count = doc->m_MapIdSupportsVisible.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsVisible.MoveFirst();
		doc->m_MapIdSupportsVisible.IsValid();
		doc->m_MapIdSupportsVisible.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsVisible.GetKey();
		JBool  Item = doc->m_MapIdSupportsVisible.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des couleurs supports sélectionnées
	Count = doc->m_MapIdColorSupports.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdColorSupports.MoveFirst();
		doc->m_MapIdColorSupports.IsValid();
		doc->m_MapIdColorSupports.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdColorSupports.GetKey();
		JUnt32  Item = doc->m_MapIdColorSupports.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// On sérialise le mode sélection couleurs
	Stream.Send(doc->m_ModeColorGraphe);

	// Reporter après écriture de l'offre
	// on sérialise la liste des maps des cibles audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdCiblesAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdCiblesAudienceSel.MoveFirst();
		 doc->m_LstMapIdCiblesAudienceSel.IsValid();
		 doc->m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdCiblesAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise le map des Grandeurs sélectionnées pour échelle
	Count = doc->m_MapIdGrandEchSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandEchSel.MoveFirst();
		doc->m_MapIdGrandEchSel.IsValid();
		doc->m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandEchSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandEchSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Grandeurs sélectionnées pour cumulativité
	Count = doc->m_MapIdGrandCumSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandCumSel.MoveFirst();
		doc->m_MapIdGrandCumSel.IsValid();
		doc->m_MapIdGrandCumSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandCumSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandCumSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Mapping
	JInt32 IdGrand = doc->m_IdGrandMapSelAbs;
	Stream				.Send(IdGrand);

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Ordonnée
	IdGrand = doc->m_IdGrandMapSelOrd;
	Stream				.Send(IdGrand);

	// on sérialise le map des Grandeurs sélectionnées pour Duplication
	Count = doc->m_MapIdGrandDupSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandDupSel.MoveFirst();
		doc->m_MapIdGrandDupSel.IsValid();
		doc->m_MapIdGrandDupSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandDupSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandDupSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour échelle
	Count = doc->m_MapIdFormatSpecifEch.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifEch.MoveFirst();
		doc->m_MapIdFormatSpecifEch.IsValid();
		doc->m_MapIdFormatSpecifEch.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifEch.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour mapping
	Count = doc->m_MapIdFormatSpecifMap.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifMap.MoveFirst();
		doc->m_MapIdFormatSpecifMap.IsValid();
		doc->m_MapIdFormatSpecifMap.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifMap.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifMap.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour cumulativite
	Count = doc->m_MapIdFormatSpecifCum.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifCum.MoveFirst();
		doc->m_MapIdFormatSpecifCum.IsValid();
		doc->m_MapIdFormatSpecifCum.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifCum.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifCum.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour duplication
	Count = doc->m_MapIdFormatSpecifDup.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifDup.MoveFirst();
		doc->m_MapIdFormatSpecifDup.IsValid();
		doc->m_MapIdFormatSpecifDup.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifDup.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifDup.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le nbinsertions sélectionné pour calcul
	Stream.Send(doc->m_NbInsertions);

	// on sérialise liste ordonnancement grandeurs échelle à afficher
	Count = doc->m_LstOrdoGrandeurEch.GetCount();
	Stream.Send(Count);
	for (doc->m_LstOrdoGrandeurEch.MoveFirst();
		 doc->m_LstOrdoGrandeurEch.IsValid();
		 doc->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 IdGrandeur = doc->m_LstOrdoGrandeurEch.GetItem();
		Stream.Send(IdGrandeur);
	}

	// on sérialise la liste des idents src audience sel
	Count = doc->m_LstIdSrcAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcAudienceSel.MoveFirst();
		 doc->m_LstIdSrcAudienceSel.IsValid();
		 doc->m_LstIdSrcAudienceSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcAudienceSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des idents source de ciblage
	Count = doc->m_LstIdSrcCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcCiblageSel.MoveFirst();
		 doc->m_LstIdSrcCiblageSel.IsValid();
		 doc->m_LstIdSrcCiblageSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcCiblageSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des maps des terrains audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 doc->m_LstMapIdTerrainsAudienceSel.IsValid();
		 doc->m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdTerrainsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents terrains ciblage sélectionnés
	Count = doc->m_LstIdTerrainCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdTerrainCiblageSel.MoveFirst();
		 doc->m_LstIdTerrainCiblageSel.IsValid();
		 doc->m_LstIdTerrainCiblageSel.MoveNext())
	{
		JUnt32 IdTer = doc->m_LstIdTerrainCiblageSel.GetItem();
		Stream.Send(IdTer);
	}

	// on sérialise le map des formats sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdFormatsSel.MoveFirst();
		 doc->m_LstMapIdFormatsSel.IsValid();
		 doc->m_LstMapIdFormatsSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdFormatsSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item = MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des maps des segments audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdSegmentsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdSegmentsAudienceSel.MoveFirst();
		 doc->m_LstMapIdSegmentsAudienceSel.IsValid();
		 doc->m_LstMapIdSegmentsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdSegmentsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents segment d'audience
	Count = doc->m_LstIdSegmentAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentAudienceSel.MoveFirst();
		 doc->m_LstIdSegmentAudienceSel.IsValid();
		 doc->m_LstIdSegmentAudienceSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentAudienceSel.GetItem(); 
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident segment pour ciblage
	Count = doc->m_LstIdSegmentCibleSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentCibleSel.MoveFirst();
		 doc->m_LstIdSegmentCibleSel.IsValid();
		 doc->m_LstIdSegmentCibleSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentCibleSel.GetItem();
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident filtre d'audience
	Count = doc->m_LstIdFiltreAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdFiltreAudienceSel.MoveFirst();
		 doc->m_LstIdFiltreAudienceSel.IsValid();
		 doc->m_LstIdFiltreAudienceSel.MoveNext())
	{
		JUnt32 IdFiltre = doc->m_LstIdFiltreAudienceSel.GetItem();
		Stream.Send(IdFiltre);
	}

	/*
	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);
	*/

	// Sauvegarde des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	Count = doc->m_OrdreEnteteEch.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_OrdreEnteteEch.GetCount(); i++)
		Stream.Send(doc->m_OrdreEnteteEch[i]);

	// Sauvegarde du tri en cours pour chaque module
	Count = doc->m_TabInfoTriCol.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_TabInfoTriCol.GetCount(); i++)
	{
		// Récup infos tri module
		CInfoTriCol InfoTriCol = doc->m_TabInfoTriCol[i];

		// Sauve no de la colonne courante du tri
		Stream.Send(InfoTriCol.NoCol);

		// Sauve mode du tri colonne courante
		Stream.Send(InfoTriCol.TriCroissant);
	}

	// En + de la version 1 -- on sérialise le Map des positions mapping
	Count = doc->m_MapPointMapping.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapPointMapping.MoveFirst();
		doc->m_MapPointMapping.IsValid();
		doc->m_MapPointMapping.MoveNext())
	{
		// Récupére clé et item d'une position mapping
		CKeyInfoMapping Key  	= doc->m_MapPointMapping.GetKey();
		CPointLibelle	Item	= doc->m_MapPointMapping.GetItem();

		// Stockage de la clef positions mapping
		Stream.Send(Key.m_IdTerrain);
		Stream.Send(Key.m_IdCible);
		Stream.Send(Key.m_IdCibleAff);
		Stream.Send(Key.m_IdSupport);
		Stream.Send(Key.m_IdSegment);
		Stream.Send(Key.m_IdFormat);
		Stream.Send(Key.m_IdMoteur);
		Stream.Send(Key.m_NbInsertions);
		Stream.Send(Key.m_IdGrandeurAbs);
		Stream.Send(Key.m_IdGrandeurOrd);
		Stream.Send(Key.m_IdFiltreAudience);
		Stream.Send(Key.m_IdContexteLecture);
		Stream.Send(Key.m_bRepriseEnMain);
		Stream.Send(Key.m_bLNMMag);
		Stream.Send(Key.m_bLDPQtd);
		Stream.Send(Key.m_bLDP30J);
	
		// Stockage de l'item X / Y position mapping
		JFlt64 X = Item.m_XLib;
		Stream.Send(X);
		JFlt64 Y = Item.m_YLib;
		Stream.Send(Y);
	}
}

void CATPDocCSSerializer::ReadV5(JStream & Stream, JATPDocChoixSupport * doc)
{
	// on récupére l'offre au début désormais (pour récup étude creuset <> Cs)
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	// Index du module courant 
	JUnt32 InxModuleCur;
	Stream				.Recv(InxModuleCur);
	doc->SetModuleCourant(InxModuleCur);

	// on récupère le plateau
	JATPKEYPlateau			KEYPlateau;				KEYPlateau			.Recv(Stream);

	// on récupère la date tarifaire
	JDate					DateTarifaire;			DateTarifaire		.Recv(Stream);

	// on récupère le map des supports sélectionnés
	JUnt32					Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsSel.Add(Key) = Item;
	}

	// on récupère le map des supports sélectionnés
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsVisible;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsVisible.Add(Key) = Item;
	}

	// on récupère le map des couleurs affectées aux supports pour les graphes
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdColorSupports;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdColorSupports.Add(Key) = Item;
	}

	// On récupère le mode sélection couleurs
	JUnt32		ModeColorGraphe;					Stream.Recv(ModeColorGraphe);

	// On récupère map des cibles audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdCiblesAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdCiblesAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdCiblesAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdCiblesAudienceSel.AddTail().Swap(MapIdCiblesAudienceSel);
	}

	// on récupére le map des Grandeurs sélectionnées pour échelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);

		// Modif Data Exo !!!!!
		// On vérifie que item grandeur existant dans les grandeurs dispos (suite à grandeurs exogènes)
		doc->m_MapAbrGrandEch.MoveTo(Item);
		if (doc->m_MapAbrGrandEch.IsValid())
			MapIdGrandEchSel.Add(Key) = Item;
	}

	// on récupére le map des Grandeurs sélectionnées pour cumulativité
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandCumSel.Add(Key) = Item;
	}

	// on récupére Ident Grandeur Abscisse sélectionnée pour Mapping
	JInt32 IdGrandMapSelAbs;
	Stream				.Recv(IdGrandMapSelAbs);

	// on récupére Ident Grandeur Ordonnée sélectionnée pour Mapping
	JInt32 IdGrandMapSelOrd;
	Stream				.Recv(IdGrandMapSelOrd);

	// on récupére le map des Grandeurs sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandDupSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Echelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatEchSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Mapping
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatMapSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatMapSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Cumulativite
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatCumSel.Add(Key) = Item;
	}
	
	// on récupére le map des formats sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatDupSel.Add(Key) = Item;
	}

	// on récupére le nbinsertions sélectionné pour calcul
	JUnt32		NbInsertions;				Stream.Recv(NbInsertions);

	// on récupére liste ordonné des grandeurs échelle à afficher
	Stream				.Recv(Count);
	JList<JUnt32>		LstOrdoGrandeurEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Item;					Stream				.Recv(Item);
		LstOrdoGrandeurEch.AddTail() = Item;
	}

	// Attention provisoire (bidouille)
	if (Count < 16)
		LstOrdoGrandeurEch.AddTail() = 15;

	// on récupére les idents source d'audience
	JList <JUnt32> LstIdSrcAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcAudienceSel.AddTail() = Ident; 
	}

	// on récupére l'ident source de ciblage
	JList <JUnt32> LstIdSrcCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcCiblageSel.AddTail() = Ident; 
	}

	// on récupère map des terrains audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdTerrainsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdTerrainsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdTerrainsAudienceSel.AddTail().Swap(MapIdTerrainsAudienceSel);
	}

	// on récupére la liste des idents terrains ciblage
	JList <JUnt32> LstIdTerrainCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdTerrainCiblageSel.AddTail() = Ident; 
	}

	// on récupére le map des formats sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdFormatsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdFormatsSel.AddTail().Swap(MapIdFormatsSel);
	}

	// on récupère map des segments audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdSegmentsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdSegmentsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdSegmentsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdSegmentsAudienceSel.AddTail().Swap(MapIdSegmentsAudienceSel);
	}

	// on récupére la liste des idents segment d'audience
	JList <JUnt32> LstIdSegmentAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentAudienceSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents segment pour ciblage
	JList <JUnt32> LstIdSegmentCibleSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentCibleSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents filtre d'audience
	JList <JUnt32> LstIdFiltreAudience;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdFiltreAudience.AddTail() = Ident; 
	}

	// Récupère ordre entetes échelle
	Stream				.Recv(Count);
	CArray<JUnt32> OrdreEnteteEch;
	OrdreEnteteEch.SetSize(Count);
	for (int i = 0; i < OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre;
		Stream.Recv(ValOrdre);
		OrdreEnteteEch[i] = ValOrdre;
	}		

	// Récupère informations tri sur tous les modules
	Stream				.Recv(Count);
	CArray<CInfoTriCol,CInfoTriCol&> TabOrdreTri;
	TabOrdreTri.SetSize(Count);
	for (int i = 0; i < TabOrdreTri.GetCount(); i++)
	{
		// Défini nvlle struct tri
		CInfoTriCol InfoTriCol;

		// Récup col courante
		Stream.Recv(InfoTriCol.NoCol);

		// Récup mode tri
		Stream.Recv(InfoTriCol.TriCroissant);
	}

	// on récupére le map des positions dans mapping
	Stream				.Recv(Count);

	JMap<CKeyInfoMapping,CPointLibelle>		MapPointMapping;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupération de la clef position mapping
		CKeyInfoMapping Key;
		Stream.Recv(Key.m_IdTerrain);
		Stream.Recv(Key.m_IdCible);
		Stream.Recv(Key.m_IdSupport);
		Stream.Recv(Key.m_IdFormat);
		Stream.Recv(Key.m_IdMoteur);
		Stream.Recv(Key.m_NbInsertions);
		Stream.Recv(Key.m_IdGrandeurAbs);
		Stream.Recv(Key.m_IdGrandeurOrd);
		Stream.Recv(Key.m_IdFiltreAudience);
		Stream.Recv(Key.m_IdContexteLecture);
		Stream.Recv(Key.m_bRepriseEnMain);
		Stream.Recv(Key.m_bLNMMag);
		Stream.Recv(Key.m_bLDPQtd);
		Stream.Recv(Key.m_bLDP30J);
	
		// Récupération de l'item X / Y position mapping
		CPointLibelle	Item;
		Stream		.Recv(Item.m_XLib);
		Stream		.Recv(Item.m_YLib);

		// Sauvegarde dans mapping
		MapPointMapping.MoveTo(Key);
		if (!MapPointMapping.IsValid())
			MapPointMapping.Add(Key) = Item;
	}

	// Swap the elements
	doc->m_KEYPlateau					.Swap(KEYPlateau);
	doc->m_DateTarifaire				= DateTarifaire; 
	doc->m_MapIdSupportsSel				.Swap(MapIdSupportsSel);
	doc->m_MapIdSupportsVisible			.Swap(MapIdSupportsVisible); 
	doc->m_MapIdColorSupports			.Swap(MapIdColorSupports); 
	doc->m_ModeColorGraphe				= ModeColorGraphe;
	doc->m_LstMapIdCiblesAudienceSel	.Swap(LstMapIdCiblesAudienceSel);

	// Les élmt grandeurs sélection pour les différents modules
	doc->m_MapIdGrandEchSel				.Swap(MapIdGrandEchSel);
	doc->m_MapIdGrandCumSel				.Swap(MapIdGrandCumSel);
	doc->m_IdGrandMapSelAbs				= IdGrandMapSelAbs;
	doc->m_IdGrandMapSelOrd				= IdGrandMapSelOrd;
	doc->m_MapIdGrandDupSel				.Swap(MapIdGrandDupSel);

	// Les élmt formats sélection pour les différents modules
	doc->m_MapIdFormatSpecifEch			.Swap(MapIdFormatEchSel);
	doc->m_MapIdFormatSpecifCum			.Swap(MapIdFormatCumSel);
	doc->m_MapIdFormatSpecifMap			.Swap(MapIdFormatMapSel);
	doc->m_MapIdFormatSpecifDup			.Swap(MapIdFormatDupSel);

	// Eléments sélections pour échelle
	doc->m_NbInsertions					= NbInsertions;
	doc->m_LstOrdoGrandeurEch			.Swap(LstOrdoGrandeurEch);
	doc->m_LstIdSrcAudienceSel			.Swap(LstIdSrcAudienceSel);
	doc->m_LstIdSrcCiblageSel			.Swap(LstIdSrcCiblageSel);
	doc->m_LstMapIdTerrainsAudienceSel	.Swap(LstMapIdTerrainsAudienceSel);
	doc->m_LstIdTerrainCiblageSel		.Swap(LstIdTerrainCiblageSel);
	doc->m_LstMapIdFormatsSel			.Swap(LstMapIdFormatsSel);
	doc->m_LstMapIdSegmentsAudienceSel  .Swap(LstMapIdSegmentsAudienceSel); 
	doc->m_LstIdSegmentAudienceSel		.Swap(LstIdSegmentAudienceSel);
	doc->m_LstIdSegmentCibleSel			.Swap(LstIdSegmentCibleSel);
	doc->m_LstIdFiltreAudienceSel		.Swap(LstIdFiltreAudience);
	doc->m_Offre						.Swap(Offre);

	// Ordre des entetes échelles
	doc->m_OrdreEnteteEch.SetSize(OrdreEnteteEch.GetCount());
	for (int i=0; i<doc->m_OrdreEnteteEch.GetCount(); i++)
		doc->m_OrdreEnteteEch[i] = OrdreEnteteEch[i];

	// Tableau ordre de tris
	doc->m_TabInfoTriCol.SetSize(TabOrdreTri.GetCount());
	for (int i=0; i<doc->m_TabInfoTriCol.GetCount(); i++)
		doc->m_TabInfoTriCol[i] = TabOrdreTri[i];

	// Le map des positions mapping
	doc->m_MapPointMapping			.Swap(MapPointMapping);
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// SERIALISATION VERSION 6 (WRITE & READ)
//
void CATPDocCSSerializer::WriteV6(JStream & Stream, JATPDocChoixSupport * doc)
{
	// Sauvegarde de l'offre au début désormais (facilitera transfert creuset <> cs)
	doc->m_Offre						.Send(Stream);

	// Index du module courant 
	Stream.Send(doc->GetModuleCourant());

	// on sérialise le plateau
	doc->m_KEYPlateau.Send(Stream);

	// on sérialise la date tarifaire
	doc->m_DateTarifaire.Send(Stream);

	// on sérialise le map des supports sélectionnés
	JInt32 Count = doc->m_MapIdSupportsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsSel.MoveFirst();
		doc->m_MapIdSupportsSel.IsValid();
		doc->m_MapIdSupportsSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsSel.GetKey();
		JBool  Item = doc->m_MapIdSupportsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// Nouveauté version 5
	// on sérialise le map des supports visibles sélectionnés
	Count = doc->m_MapIdSupportsVisible.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdSupportsVisible.MoveFirst();
		doc->m_MapIdSupportsVisible.IsValid();
		doc->m_MapIdSupportsVisible.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdSupportsVisible.GetKey();
		JBool  Item = doc->m_MapIdSupportsVisible.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des couleurs supports sélectionnées
	Count = doc->m_MapIdColorSupports.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdColorSupports.MoveFirst();
		doc->m_MapIdColorSupports.IsValid();
		doc->m_MapIdColorSupports.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdColorSupports.GetKey();
		JUnt32  Item = doc->m_MapIdColorSupports.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// On sérialise le mode sélection couleurs
	Stream.Send(doc->m_ModeColorGraphe);

	// Reporter après écriture de l'offre
	// on sérialise la liste des maps des cibles audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdCiblesAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdCiblesAudienceSel.MoveFirst();
		 doc->m_LstMapIdCiblesAudienceSel.IsValid();
		 doc->m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdCiblesAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise le map des Grandeurs sélectionnées pour échelle
	Count = doc->m_MapIdGrandEchSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandEchSel.MoveFirst();
		doc->m_MapIdGrandEchSel.IsValid();
		doc->m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandEchSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandEchSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Grandeurs sélectionnées pour cumulativité
	Count = doc->m_MapIdGrandCumSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandCumSel.MoveFirst();
		doc->m_MapIdGrandCumSel.IsValid();
		doc->m_MapIdGrandCumSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandCumSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandCumSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Mapping
	JInt32 IdGrand = doc->m_IdGrandMapSelAbs;
	Stream				.Send(IdGrand);

	// on sérialise l'ident Grandeur sélectionnée pour Abscisse Ordonnée
	IdGrand = doc->m_IdGrandMapSelOrd;
	Stream				.Send(IdGrand);

	// on sérialise le map des Grandeurs sélectionnées pour Duplication
	Count = doc->m_MapIdGrandDupSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdGrandDupSel.MoveFirst();
		doc->m_MapIdGrandDupSel.IsValid();
		doc->m_MapIdGrandDupSel.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdGrandDupSel.GetKey();
		JUnt32 Item	= doc->m_MapIdGrandDupSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour échelle
	Count = doc->m_MapIdFormatSpecifEch.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifEch.MoveFirst();
		doc->m_MapIdFormatSpecifEch.IsValid();
		doc->m_MapIdFormatSpecifEch.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifEch.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour mapping
	Count = doc->m_MapIdFormatSpecifMap.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifMap.MoveFirst();
		doc->m_MapIdFormatSpecifMap.IsValid();
		doc->m_MapIdFormatSpecifMap.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifMap.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifMap.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour cumulativite
	Count = doc->m_MapIdFormatSpecifCum.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifCum.MoveFirst();
		doc->m_MapIdFormatSpecifCum.IsValid();
		doc->m_MapIdFormatSpecifCum.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifCum.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifCum.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le map des Formats sélectionnés pour duplication
	Count = doc->m_MapIdFormatSpecifDup.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatSpecifDup.MoveFirst();
		doc->m_MapIdFormatSpecifDup.IsValid();
		doc->m_MapIdFormatSpecifDup.MoveNext())
	{
		JUnt32 Key	= doc->m_MapIdFormatSpecifDup.GetKey();
		JUnt32 Item	= doc->m_MapIdFormatSpecifDup.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// on sérialise le nbinsertions sélectionné pour calcul
	Stream.Send(doc->m_NbInsertions);

	// on sérialise liste ordonnancement grandeurs échelle à afficher
	Count = doc->m_LstOrdoGrandeurEch.GetCount();
	Stream.Send(Count);
	for (doc->m_LstOrdoGrandeurEch.MoveFirst();
		 doc->m_LstOrdoGrandeurEch.IsValid();
		 doc->m_LstOrdoGrandeurEch.MoveNext())
	{
		JUnt32 IdGrandeur = doc->m_LstOrdoGrandeurEch.GetItem();
		Stream.Send(IdGrandeur);
	}

	// on sérialise la liste des idents src audience sel
	Count = doc->m_LstIdSrcAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcAudienceSel.MoveFirst();
		 doc->m_LstIdSrcAudienceSel.IsValid();
		 doc->m_LstIdSrcAudienceSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcAudienceSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des idents source de ciblage
	Count = doc->m_LstIdSrcCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSrcCiblageSel.MoveFirst();
		 doc->m_LstIdSrcCiblageSel.IsValid();
		 doc->m_LstIdSrcCiblageSel.MoveNext())
	{
		JUnt32 IdSrc = doc->m_LstIdSrcCiblageSel.GetItem();
		Stream.Send(IdSrc);
	}

	// on sérialise la liste des maps des terrains audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 doc->m_LstMapIdTerrainsAudienceSel.IsValid();
		 doc->m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdTerrainsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents terrains ciblage sélectionnés
	Count = doc->m_LstIdTerrainCiblageSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdTerrainCiblageSel.MoveFirst();
		 doc->m_LstIdTerrainCiblageSel.IsValid();
		 doc->m_LstIdTerrainCiblageSel.MoveNext())
	{
		JUnt32 IdTer = doc->m_LstIdTerrainCiblageSel.GetItem();
		Stream.Send(IdTer);
	}

	// on sérialise le map des formats sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdFormatsSel.MoveFirst();
		 doc->m_LstMapIdFormatsSel.IsValid();
		 doc->m_LstMapIdFormatsSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdFormatsSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item = MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des maps des segments audience sélectionnés
	// Nombre d'émt map = nombre de module choix support
	Count = doc->m_LstMapIdSegmentsAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstMapIdSegmentsAudienceSel.MoveFirst();
		 doc->m_LstMapIdSegmentsAudienceSel.IsValid();
		 doc->m_LstMapIdSegmentsAudienceSel.MoveNext())
	{
		// Sérialise chacun des maps
		JMap <JUnt32,JUnt32> &MapId = doc->m_LstMapIdSegmentsAudienceSel.GetItem();
		Count = MapId.GetCount();
		Stream				.Send(Count);
		for(MapId.MoveFirst(); MapId.IsValid(); MapId.MoveNext())
		{
			JUnt32 Key	= MapId.GetKey();
			JUnt32 Item	= MapId.GetItem();
			Stream.Send(Key);
			Stream.Send(Item);
		}
	}

	// on sérialise la liste des idents segment d'audience
	Count = doc->m_LstIdSegmentAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentAudienceSel.MoveFirst();
		 doc->m_LstIdSegmentAudienceSel.IsValid();
		 doc->m_LstIdSegmentAudienceSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentAudienceSel.GetItem(); 
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident segment pour ciblage
	Count = doc->m_LstIdSegmentCibleSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdSegmentCibleSel.MoveFirst();
		 doc->m_LstIdSegmentCibleSel.IsValid();
		 doc->m_LstIdSegmentCibleSel.MoveNext())
	{
		JUnt32 IdSeg = doc->m_LstIdSegmentCibleSel.GetItem();
		Stream.Send(IdSeg);
	}

	// on sérialise la liste des ident filtre d'audience
	Count = doc->m_LstIdFiltreAudienceSel.GetCount();
	Stream				.Send(Count);
	for (doc->m_LstIdFiltreAudienceSel.MoveFirst();
		 doc->m_LstIdFiltreAudienceSel.IsValid();
		 doc->m_LstIdFiltreAudienceSel.MoveNext())
	{
		JUnt32 IdFiltre = doc->m_LstIdFiltreAudienceSel.GetItem();
		Stream.Send(IdFiltre);
	}

	// Sauvegarde des headers (1 / Terrain 2/ Cibles 3/Grandeurs
	Count = doc->m_OrdreEnteteEch.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_OrdreEnteteEch.GetCount(); i++)
		Stream.Send(doc->m_OrdreEnteteEch[i]);

	// Sauvegarde du tri en cours pour chaque module
	Count = doc->m_TabInfoTriCol.GetCount();
	Stream				.Send(Count);
	for (int i = 0; i < doc->m_TabInfoTriCol.GetCount(); i++)
	{
		// Récup infos tri module
		CInfoTriCol InfoTriCol = doc->m_TabInfoTriCol[i];

		// Sauve no de la colonne courante du tri
		Stream.Send(InfoTriCol.NoCol);

		// Sauve mode du tri colonne courante
		Stream.Send(InfoTriCol.TriCroissant);
	}

	// En + de la version 1 -- on sérialise le Map des positions mapping
	Count = doc->m_MapPointMapping.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapPointMapping.MoveFirst();
		doc->m_MapPointMapping.IsValid();
		doc->m_MapPointMapping.MoveNext())
	{
		// Récupére clé et item d'une position mapping
		CKeyInfoMapping Key  	= doc->m_MapPointMapping.GetKey();
		CPointLibelle	Item	= doc->m_MapPointMapping.GetItem();

		// Stockage de la clef positions mapping
		Stream.Send(Key.m_IdTerrain);
		Stream.Send(Key.m_IdCible);
		Stream.Send(Key.m_IdCibleAff);
		Stream.Send(Key.m_IdSupport);
		Stream.Send(Key.m_IdSegment);
		Stream.Send(Key.m_IdFormat);
		Stream.Send(Key.m_IdMoteur);
		Stream.Send(Key.m_NbInsertions);
		Stream.Send(Key.m_IdGrandeurAbs);
		Stream.Send(Key.m_IdGrandeurOrd);
		Stream.Send(Key.m_IdFiltreAudience);
		Stream.Send(Key.m_IdContexteLecture);
		Stream.Send(Key.m_bRepriseEnMain);
		Stream.Send(Key.m_bLNMMag);
		Stream.Send(Key.m_bLDPQtd);
		Stream.Send(Key.m_bLDP30J);
	
		// Stockage de l'item X / Y position mapping
		JFlt64 X = Item.m_XLib;
		Stream.Send(X);
		JFlt64 Y = Item.m_YLib;
		Stream.Send(Y);
	}

	// en + de la version 5, sérialisation du map des rangs sélectionnés
	// Nombre d'émt map = nombre de grandeurs
	Count = doc->m_MapEtatRangResultEch.GetCount();
	Stream				.Send(Count);
	for (doc->m_MapEtatRangResultEch.MoveFirst();
		 doc->m_MapEtatRangResultEch.IsValid();
		 doc->m_MapEtatRangResultEch.MoveNext())
	{
		// Sérialise chacun des élmts du map
		JUnt32 Key	= doc->m_MapEtatRangResultEch.GetKey();
		JBool  Item = doc->m_MapEtatRangResultEch.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}
}

void CATPDocCSSerializer::ReadV6(JStream & Stream, JATPDocChoixSupport * doc)
{
	// on récupére l'offre au début désormais (pour récup étude creuset <> Cs)
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	// Index du module courant 
	JUnt32 InxModuleCur;
	Stream				.Recv(InxModuleCur);
	doc->SetModuleCourant(InxModuleCur);

	// on récupère le plateau
	JATPKEYPlateau			KEYPlateau;				KEYPlateau			.Recv(Stream);

	// on récupère la date tarifaire
	JDate					DateTarifaire;			DateTarifaire		.Recv(Stream);

	// on récupère le map des supports sélectionnés
	JUnt32					Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsSel.Add(Key) = Item;
	}

	// on récupère le map des supports sélectionnés
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JBool>		MapIdSupportsVisible;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapIdSupportsVisible.Add(Key) = Item;
	}

	// on récupère le map des couleurs affectées aux supports pour les graphes
							Count;					Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdColorSupports;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdColorSupports.Add(Key) = Item;
	}

	// On récupère le mode sélection couleurs
	JUnt32		ModeColorGraphe;					Stream.Recv(ModeColorGraphe);

	// On récupère map des cibles audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdCiblesAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdCiblesAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdCiblesAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdCiblesAudienceSel.AddTail().Swap(MapIdCiblesAudienceSel);
	}

	// on récupére le map des Grandeurs sélectionnées pour échelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);

		// Modif Data Exo !!!!!
		// On vérifie que item grandeur existant dans les grandeurs dispos (suite à grandeurs exogènes)
		doc->m_MapAbrGrandEch.MoveTo(Item);
		if (doc->m_MapAbrGrandEch.IsValid())
			MapIdGrandEchSel.Add(Key) = Item;
	}

	// on récupére le map des Grandeurs sélectionnées pour cumulativité
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandCumSel.Add(Key) = Item;
	}

	// on récupére Ident Grandeur Abscisse sélectionnée pour Mapping
	JInt32 IdGrandMapSelAbs;
	Stream				.Recv(IdGrandMapSelAbs);

	// on récupére Ident Grandeur Ordonnée sélectionnée pour Mapping
	JInt32 IdGrandMapSelOrd;
	Stream				.Recv(IdGrandMapSelOrd);

	// on récupére le map des Grandeurs sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdGrandDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdGrandDupSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Echelle
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatEchSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatEchSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Mapping
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatMapSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatMapSel.Add(Key) = Item;
	}

	// on récupére le map des formats sélectionnées pour Cumulativite
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatCumSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatCumSel.Add(Key) = Item;
	}
	
	// on récupére le map des formats sélectionnées pour Duplication
	Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatDupSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatDupSel.Add(Key) = Item;
	}

	// on récupére le nbinsertions sélectionné pour calcul
	JUnt32		NbInsertions;				Stream.Recv(NbInsertions);

	// on récupére liste ordonné des grandeurs échelle à afficher
	Stream				.Recv(Count);
	JList<JUnt32>		LstOrdoGrandeurEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Item;					Stream				.Recv(Item);
		LstOrdoGrandeurEch.AddTail() = Item;
	}

	// Attention provisoire (bidouille)
	if (Count < 16)
		LstOrdoGrandeurEch.AddTail() = 15;

	// on récupére les idents source d'audience
	JList <JUnt32> LstIdSrcAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcAudienceSel.AddTail() = Ident; 
	}

	// on récupére l'ident source de ciblage
	JList <JUnt32> LstIdSrcCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSrcCiblageSel.AddTail() = Ident; 
	}

	// on récupère map des terrains audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdTerrainsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdTerrainsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdTerrainsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdTerrainsAudienceSel.AddTail().Swap(MapIdTerrainsAudienceSel);
	}

	// on récupére la liste des idents terrains ciblage
	JList <JUnt32> LstIdTerrainCiblageSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdTerrainCiblageSel.AddTail() = Ident; 
	}

	// on récupére le map des formats sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdFormatsSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdFormatsSel.AddTail().Swap(MapIdFormatsSel);
	}

	// on récupère map des segments audience sélectionnés
	// Récup nb module choix support
	Stream				.Recv(Count);
	JList <JMap <JUnt32,JUnt32> > LstMapIdSegmentsAudienceSel;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupère chaque map
		JMap<JUnt32,JUnt32>		MapIdSegmentsAudienceSel;
		JUnt32 CountMap;
		Stream			.Recv(CountMap);
		for(JUnt32 y = 0; y < CountMap; y++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdSegmentsAudienceSel.Add(Key) = Item;
		}

		// Ajoute à la liste
		LstMapIdSegmentsAudienceSel.AddTail().Swap(MapIdSegmentsAudienceSel);
	}

	// on récupére la liste des idents segment d'audience
	JList <JUnt32> LstIdSegmentAudienceSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentAudienceSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents segment pour ciblage
	JList <JUnt32> LstIdSegmentCibleSel;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdSegmentCibleSel.AddTail() = Ident; 
	}

	// on récupére la liste des idents filtre d'audience
	JList <JUnt32> LstIdFiltreAudience;
	Stream				.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32 Ident;								Stream				.Recv(Ident);
		LstIdFiltreAudience.AddTail() = Ident; 
	}

	// Récupère ordre entetes échelle
	Stream				.Recv(Count);
	CArray<JUnt32> OrdreEnteteEch;
	OrdreEnteteEch.SetSize(Count);
	for (int i = 0; i < OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 ValOrdre;
		Stream.Recv(ValOrdre);
		OrdreEnteteEch[i] = ValOrdre;
	}		

	// Récupère informations tri sur tous les modules
	Stream				.Recv(Count);
	CArray<CInfoTriCol,CInfoTriCol&> TabOrdreTri;
	TabOrdreTri.SetSize(Count);
	for (int i = 0; i < TabOrdreTri.GetCount(); i++)
	{
		// Défini nvlle struct tri
		CInfoTriCol InfoTriCol;

		// Récup col courante
		Stream.Recv(InfoTriCol.NoCol);

		// Récup mode tri
		Stream.Recv(InfoTriCol.TriCroissant);
	}

	// on récupére le map des positions dans mapping
	Stream				.Recv(Count);

	JMap<CKeyInfoMapping,CPointLibelle>		MapPointMapping;
	for(JUnt32 x = 0; x < Count; x++)
	{
		// Récupération de la clef position mapping
		CKeyInfoMapping Key;
		Stream.Recv(Key.m_IdTerrain);
		Stream.Recv(Key.m_IdCible);
		Stream.Recv(Key.m_IdCibleAff);
		Stream.Recv(Key.m_IdSupport);
		Stream.Recv(Key.m_IdSegment);
		Stream.Recv(Key.m_IdFormat);
		Stream.Recv(Key.m_IdMoteur);
		Stream.Recv(Key.m_NbInsertions);
		Stream.Recv(Key.m_IdGrandeurAbs);
		Stream.Recv(Key.m_IdGrandeurOrd);
		Stream.Recv(Key.m_IdFiltreAudience);
		Stream.Recv(Key.m_IdContexteLecture);
		Stream.Recv(Key.m_bRepriseEnMain);
		Stream.Recv(Key.m_bLNMMag);
		Stream.Recv(Key.m_bLDPQtd);
		Stream.Recv(Key.m_bLDP30J);
	
		// Récupération de l'item X / Y position mapping
		CPointLibelle	Item;
		Stream		.Recv(Item.m_XLib);
		Stream		.Recv(Item.m_YLib);

		// Sauvegarde dans mapping
		MapPointMapping.MoveTo(Key);
		if (!MapPointMapping.IsValid())
			MapPointMapping.Add(Key) = Item;
	}

	// en + de la version 5, on récupère la sérialisation du map des rangs sélectionnés
	// Récup nombre d'émt map = nombre de grandeurs
	Stream				.Recv(Count);
	JMap<JUnt32,JBool>	 MapEtatRangResultEch;
	for(JUnt32 x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JBool				Item;					Stream				.Recv(Item);
		MapEtatRangResultEch.Add(Key) = Item;
	}

	// Swap the elements
	doc->m_KEYPlateau					.Swap(KEYPlateau);
	doc->m_DateTarifaire				= DateTarifaire; 
	doc->m_MapIdSupportsSel				.Swap(MapIdSupportsSel);
	doc->m_MapIdSupportsVisible			.Swap(MapIdSupportsVisible); 
	doc->m_MapIdColorSupports			.Swap(MapIdColorSupports); 
	doc->m_ModeColorGraphe				= ModeColorGraphe;
	doc->m_LstMapIdCiblesAudienceSel	.Swap(LstMapIdCiblesAudienceSel);

	// Les élmt grandeurs sélection pour les différents modules
	doc->m_MapIdGrandEchSel				.Swap(MapIdGrandEchSel);
	doc->m_MapIdGrandCumSel				.Swap(MapIdGrandCumSel);
	doc->m_IdGrandMapSelAbs				= IdGrandMapSelAbs;
	doc->m_IdGrandMapSelOrd				= IdGrandMapSelOrd;
	doc->m_MapIdGrandDupSel				.Swap(MapIdGrandDupSel);

	// Les élmt formats sélection pour les différents modules
	doc->m_MapIdFormatSpecifEch			.Swap(MapIdFormatEchSel);
	doc->m_MapIdFormatSpecifCum			.Swap(MapIdFormatCumSel);
	doc->m_MapIdFormatSpecifMap			.Swap(MapIdFormatMapSel);
	doc->m_MapIdFormatSpecifDup			.Swap(MapIdFormatDupSel);

	// Eléments sélections pour échelle
	doc->m_NbInsertions					= NbInsertions;
	doc->m_LstOrdoGrandeurEch			.Swap(LstOrdoGrandeurEch);
	doc->m_LstIdSrcAudienceSel			.Swap(LstIdSrcAudienceSel);
	doc->m_LstIdSrcCiblageSel			.Swap(LstIdSrcCiblageSel);
	doc->m_LstMapIdTerrainsAudienceSel	.Swap(LstMapIdTerrainsAudienceSel);
	doc->m_LstIdTerrainCiblageSel		.Swap(LstIdTerrainCiblageSel);
	doc->m_LstMapIdFormatsSel			.Swap(LstMapIdFormatsSel);
	doc->m_LstMapIdSegmentsAudienceSel  .Swap(LstMapIdSegmentsAudienceSel); 
	doc->m_LstIdSegmentAudienceSel		.Swap(LstIdSegmentAudienceSel);
	doc->m_LstIdSegmentCibleSel			.Swap(LstIdSegmentCibleSel);
	doc->m_LstIdFiltreAudienceSel		.Swap(LstIdFiltreAudience);
	doc->m_Offre						.Swap(Offre);

	// Ordre des entetes échelles
	doc->m_OrdreEnteteEch.SetSize(OrdreEnteteEch.GetCount());
	for (int i=0; i<doc->m_OrdreEnteteEch.GetCount(); i++)
		doc->m_OrdreEnteteEch[i] = OrdreEnteteEch[i];

	// Tableau ordre de tris
	doc->m_TabInfoTriCol.SetSize(TabOrdreTri.GetCount());
	for (int i=0; i<doc->m_TabInfoTriCol.GetCount(); i++)
		doc->m_TabInfoTriCol[i] = TabOrdreTri[i];

	// Le map des positions mapping
	doc->m_MapPointMapping			.Swap(MapPointMapping);

	// Le map des sélections rang échelle
	doc->m_MapEtatRangResultEch		.Swap(MapEtatRangResultEch);  
}

