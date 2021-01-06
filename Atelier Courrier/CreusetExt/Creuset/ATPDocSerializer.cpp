//
// Date:    02/03/2004
//


// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPDocSerializer.h"
#include "JATPDocCreuset.h"
#include "ItemsConstruitsManager.h"

CATPDocSerializer::CATPDocSerializer(void)
{
}
CATPDocSerializer::~CATPDocSerializer(void)
{
}

void CATPDocSerializer::Send(JStream & Stream, JATPDocCreuset * doc)
{
	// on s�rialise l'ent�te
	JInt32 Compagny = 0x0043464A; Stream.Send(Compagny);
	JInt32 Magic    = 0x4A6D2311; Stream.Send(Magic);
	JInt32 Compress = 0x00000000; Stream.Send(Compress);
	JInt32 Encrypt  = 0x00000000; Stream.Send(Encrypt);
	Stream.Send(VERSION);

	// on initialise la version
	JInt32 OldVersion = Stream.SetVersion(VERSION);

	switch(VERSION)
	{
	case 1:
		WriteV1(Stream, doc);
		break;

	case 4:
		WriteV4(Stream, doc);
		break;

	case 5:
		WriteV5(Stream, doc);
		break;
	
	// Idem Version 5, mais pour concordance offre avec Analyse de l'offre (modif Famille), on passe � la version 7
	// + sauvegarde des tarifs manuels
	case 7:
		WriteV7(Stream, doc);
		break;

	// Idem version 7, mais en sauvant en + le mode cible pond�r� actif ou pas
	case 8:
		WriteV8(Stream, doc);
		break;
	}
}

void CATPDocSerializer::Recv(JStream & Stream, JATPDocCreuset * doc)
{

	// on s�rialise l'ent�te
	JInt32 Compagny; Stream.Recv(Compagny);
	if (Compagny != 0x0043464A) throw JBadSchema::GetInstance();
	JInt32 Magic;    Stream.Recv(Magic);    if (Magic    != 0x4A6D2311) throw JBadSchema::GetInstance();
	JInt32 Compress; Stream.Recv(Compress); if (Compress != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Encrypt;  Stream.Recv(Encrypt);  if (Encrypt  != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Version;  Stream.Recv(Version);

	// on initialise la version
	JInt32 OldVersion = Stream.SetVersion(Version);

	// Attention si version < 3 il faudra resauver l'�tude pour passage �tude entre utilisateur
	if (Version < 3)
	{
		AfxMessageBox("Cette �tude sauvegard�e ne pourra pas �tre utilis�e pour des �changes entre postes utilisateurs, sauf si vous la sauvegardez � nouveau pour �tre conforme au nouveau format de sauvegarde");
        doc->SetModifiedFlag(true);
	}

	switch(Version)
	{
	case 1:
		ReadV1(Stream, doc);
		break;
	case 4:
		ReadV4(Stream, doc);
		break;
	case 5:
		ReadV5(Stream, doc);
		break;

	// Idem Version 5, mais pour concordance offre avec Analyse de l'offre (modif Famille), on passe � la version 7
	// + lecture des tarifs manuels
	case 7:
		ReadV7(Stream, doc);
		break;

	// Idem Version 7, mais en r�cup�rant en + le mode cible pond�r�e (actif ou pas)
	case 8:
		ReadV8(Stream, doc);
		break;
	}
}

/*
//////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration Offre + Plateau
JBool CATPDocSerializer::RecvOffrePlateau(JStream & Stream, JATPOffre &Offre, JList <JUnt32> &LstSuppPlateau)
{
	// on s�rialise l'ent�te
	JInt32 Compagny; Stream.Recv(Compagny);
	if (Compagny != 0x0043464A) throw JBadSchema::GetInstance();
	JInt32 Magic;    Stream.Recv(Magic);    if (Magic    != 0x4A6D2311) throw JBadSchema::GetInstance();
	JInt32 Compress; Stream.Recv(Compress); if (Compress != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Encrypt;  Stream.Recv(Encrypt);  if (Encrypt  != 0x00000000) throw JBadSchema::GetInstance();
	JInt32 Version;  Stream.Recv(Version);

	// R�cup seulement pour version sauvegard�e r�cente
	if (Version >= 4)
	{
		// on r�cup�re l'offre au d�but
		JMap <JInt32,const IItem *> MapCibleOffre;
		Offre.Recv(Stream, MapCibleOffre);

		// R�cup �tude et p�riodes
		JATPKEYEtude			KEYEtude;				KEYEtude			.Recv(Stream);
		JATPKEYPeriodeAction	KEYPeriodeAction;		KEYPeriodeAction	.Recv(Stream);
		JATPKEYPeriodeAction	KEYPeriodeActionHyp;	KEYPeriodeActionHyp	.Recv(Stream);
		JATPKEYPeriodeAction	KEYPeriodeActionTablier;KEYPeriodeActionTablier	.Recv(Stream);

		// Id Source d'audience et de ciblage
		JUnt32		IdSrcAudienceSel;		Stream.Recv(IdSrcAudienceSel);
		JUnt32		IdSrcCiblageSel;		Stream.Recv(IdSrcCiblageSel);

		// Map id terrains s�lectionn�s
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

		// Map �tat des formats
														Stream				.Recv(Count);
		JMap<JUnt32,JUnt32>		MapIdFormatsSel;
		for(int x = 0; x < Count; x++)
		{
			JUnt32				Key;					Stream				.Recv(Key);
			JUnt32				Item;					Stream				.Recv(Item);
			MapIdFormatsSel.Add(Key) = Item;
		}

		// R�cup�ration des plateaux (plateau �tendu et plateau hypoth�ses)
		JATPEchelleSupports		EchelleSupports;		EchelleSupports		.Recv(Stream);
		JATPPlateauSupports		PlateauSupports;		PlateauSupports		.Recv(Stream);
		JATPPlateauSupports		PlatSuppPlanEtendu;		PlatSuppPlanEtendu	.Recv(Stream);

		// Init liste supports s�lectionn�s
		LstSuppPlateau.Reset(); 

		// R�cup�re dans liste supports
		for (JUnt32 IndexSupp = 0; IndexSupp <PlateauSupports.GetCount(); IndexSupp++)
		{
			// Clef du support s�lectionn�
			CKeySupport KeySupp = PlateauSupports.Item(IndexSupp);

			// Ajoute � la liste s�lection
			LstSuppPlateau.AddTail() = KeySupp.m_IdSupport;  

		}
		return true;
	}

	// Pas de r�cup si ancienne version
	return false;
}
*/


//////////////////////// GESTION SAUVEGARDE VERSION 1 ////////////////////
//
void CATPDocSerializer::WriteV1(JStream & Stream, JATPDocCreuset * doc)
{
	// on s�rialise l'�l�ment
	doc->m_KEYEtude.Send(Stream);
	doc->m_KEYPeriodeAction.Send(Stream);
	doc->m_KEYPeriodeActionHyp.Send(Stream);
	doc->m_KEYPeriodeActionTablier	.Send(Stream);

	Stream.Send(doc->m_IdItemSel);
	Stream.Send(doc->m_IdSrcAudienceSel);
	Stream.Send(doc->m_IdSrcCiblageSel);

	JInt32 Count = doc->m_MapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdTerrainsAudienceSel.MoveFirst();
		doc->m_MapIdTerrainsAudienceSel.IsValid();
		doc->m_MapIdTerrainsAudienceSel.MoveNext())
	{
		JUnt32 Key  = doc->m_MapIdTerrainsAudienceSel.GetKey();
		JUnt32 Item = doc->m_MapIdTerrainsAudienceSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	Stream.Send(doc->m_IdTerrainCiblageSel);

	Stream.Send(doc->m_IdSegmentAudienceSel);
	Stream.Send(doc->m_IdSegmentCibleSel);

	Stream.Send(doc->m_IdFiltreAudienceSel);

	// Map �tat des formats
	Count = doc->m_MapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatsSel.MoveFirst();
		doc->m_MapIdFormatsSel.IsValid();
		doc->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 Key = doc->m_MapIdFormatsSel.GetKey();
		JUnt32 Item = doc->m_MapIdFormatsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}


	// Sauvegarde des plateaux (plateau �tendu et plateau hypoth�ses)
	doc->m_EchelleSupports.Send(Stream);
	doc->m_PlateauSupports.Send(Stream);
	doc->m_PlateauSupportsPlanEtendu.Send(Stream);

	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);


	// Save Plan & Hypotheses
	Stream				.Send(doc->m_IdxHypotheseCourant);
	Stream				.Send(doc->m_IdxHypotheseActif);

	Count  = doc->m_TblHypotheses.GetCount();
	Stream				.Send(Count);
	for(doc->m_TblHypotheses	.MoveFirst();
		doc->m_TblHypotheses	.IsValid();
		doc->m_TblHypotheses	.MoveNext())
	{
		doc->m_TblHypotheses.GetItem().Send(Stream);
	}

	// Archive Plan Master
	doc->m_MasterPlan.Send(Stream);

	// Archive Historique Plan Master
	Count  = doc->m_TblPlanArchive.GetCount();
	Stream				.Send(Count);
	for(doc->m_TblPlanArchive.MoveFirst();
		doc->m_TblPlanArchive.IsValid();
		doc->m_TblPlanArchive.MoveNext())
	{
		doc->m_TblPlanArchive.GetItem().Send(Stream);
	}
}


void CATPDocSerializer::ReadV1(JStream & Stream, JATPDocCreuset * doc)
{
	JATPKEYEtude			KEYEtude;				KEYEtude			.Recv(Stream);
	JATPKEYPeriodeAction	KEYPeriodeAction;		KEYPeriodeAction	.Recv(Stream);
	JATPKEYPeriodeAction	KEYPeriodeActionHyp;	KEYPeriodeActionHyp	.Recv(Stream);
	JATPKEYPeriodeAction	KEYPeriodeActionTablier;KEYPeriodeActionTablier	.Recv(Stream);


	JUnt32		IdItemSel;				Stream.Recv(IdItemSel);
	JUnt32		IdSrcAudienceSel;		Stream.Recv(IdSrcAudienceSel);
	JUnt32		IdSrcCiblageSel;		Stream.Recv(IdSrcCiblageSel);

	// Map id terrains s�lectionn�s
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

	// Map �tat des formats
													Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatsSel;
	for(int x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatsSel.Add(Key) = Item;
	}

	// R�cup�ration des plateaux (plateau �tendu et plateau hypoth�ses)
	JATPEchelleSupports		EchelleSupports;		EchelleSupports		.Recv(Stream);
	JATPPlateauSupports		PlateauSupports;		PlateauSupports		.Recv(Stream);
	JATPPlateauSupports		PlatSuppPlanEtendu;		PlatSuppPlanEtendu	.Recv(Stream);

	// on r�cup�re l'offre
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	JUnt32		IdxHypotheseCourant;		Stream.Recv(IdxHypotheseCourant);
	JUnt32		IdxHypotheseActif;			Stream.Recv(IdxHypotheseActif);

	// R�cup les hypoth�ses
	Stream					.Recv(Count);
	JList<CPlanInsertions> TblHypotheses;
	for(int x = 0; x < Count; x++)
	{
		TblHypotheses.AddTail().Recv(Stream);
	}

	// R�cup le plan master courant
	CPlanInsertions		MasterPlan;				MasterPlan.Recv(Stream);

	// R�cup historique plans masters
	Stream					.Recv(Count);
	LIST_PLANMASTERS TblPlanArchive;
	for(int x = 0; x < Count; x++)
	{
		TblPlanArchive.AddTail().Recv(Stream);
	}

///////////////////////////////////////////////


	// Swap the elements
	doc->m_KEYEtude					.Swap(KEYEtude);
	doc->m_KEYPeriodeAction			.Swap(KEYPeriodeAction);
	doc->m_KEYPeriodeActionHyp		.Swap(KEYPeriodeActionHyp);
	doc->m_KEYPeriodeActionTablier	.Swap(KEYPeriodeActionTablier);

	doc->m_IdItemSel = IdItemSel;
	doc->m_IdSrcAudienceSel = IdSrcAudienceSel;
	doc->m_IdSrcCiblageSel  = IdSrcCiblageSel;

	// Map id terrains s�lectionn�s
	doc->m_MapIdTerrainsAudienceSel.Swap(MapIdTerrainAudienceSel);

	doc->m_IdTerrainCiblageSel	= IdTerrainCiblageSel;
	doc->m_IdSegmentAudienceSel = IdSegmentAudienceSel;
	doc->m_IdSegmentCibleSel	= IdSegmentCibleSel;
	doc->m_IdFiltreAudienceSel  = IdFiltreAudienceSel;

	// Map �tat des formats
	doc->m_MapIdFormatsSel.Swap(MapIdFormatsSel);

	// R�cup�ration des plateaux (plateau �tendu et plateau hypoth�ses)
	doc->m_EchelleSupports.Swap(EchelleSupports);
	doc->m_PlateauSupports.Swap(PlateauSupports);
	doc->m_PlateauSupportsPlanEtendu.Swap(PlatSuppPlanEtendu);

	// R�cup�ration de l'offre
	doc->m_Offre.Swap(Offre);


	doc->m_IdxHypotheseCourant = IdxHypotheseCourant;
	doc->m_IdxHypotheseActif = IdxHypotheseActif;


	// R�cup les hypoth�ses
	doc->m_TblHypotheses.Swap(TblHypotheses);

	// R�cup le plan master courant
	doc->m_MasterPlan.Swap(MasterPlan);

	// R�cup historique plans masters
	doc->m_TblPlanArchive.Swap(TblPlanArchive);


}

//////////////////////// GESTION SAUVEGARDE VERSION 4 ////////////////////
//
void CATPDocSerializer::WriteV4(JStream & Stream, JATPDocCreuset * doc)
{
	// sauvegarde modifi�e, sauvegarde offre au d�but
	//
	// Sauvegarde de l'offre
	doc->m_Offre						.Send(Stream);

	// Sauvegarde Etude et p�riodes
	doc->m_KEYEtude.Send(Stream);
	doc->m_KEYPeriodeAction.Send(Stream);
	doc->m_KEYPeriodeActionHyp.Send(Stream);
	doc->m_KEYPeriodeActionTablier	.Send(Stream);

	// Sauvegarde id item cible reporter apr�s r�cup�ration offre 
	// (pour gestion construction de cet item)
	// Stream.Send(doc->m_IdItemSel);

	Stream.Send(doc->m_IdSrcAudienceSel);
	Stream.Send(doc->m_IdSrcCiblageSel);

	JInt32 Count = doc->m_MapIdTerrainsAudienceSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdTerrainsAudienceSel.MoveFirst();
		doc->m_MapIdTerrainsAudienceSel.IsValid();
		doc->m_MapIdTerrainsAudienceSel.MoveNext())
	{
		JUnt32 Key  = doc->m_MapIdTerrainsAudienceSel.GetKey();
		JUnt32 Item = doc->m_MapIdTerrainsAudienceSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	Stream.Send(doc->m_IdTerrainCiblageSel);

	Stream.Send(doc->m_IdSegmentAudienceSel);
	Stream.Send(doc->m_IdSegmentCibleSel);

	Stream.Send(doc->m_IdFiltreAudienceSel);

	// Map �tat des formats
	Count = doc->m_MapIdFormatsSel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapIdFormatsSel.MoveFirst();
		doc->m_MapIdFormatsSel.IsValid();
		doc->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 Key = doc->m_MapIdFormatsSel.GetKey();
		JUnt32 Item = doc->m_MapIdFormatsSel.GetItem();
		Stream.Send(Key);
		Stream.Send(Item);
	}

	// Sauvegarde des plateaux (plateau �tendu et plateau hypoth�ses)
	doc->m_EchelleSupports.Send(Stream);
	doc->m_PlateauSupports.Send(Stream);
	doc->m_PlateauSupportsPlanEtendu.Send(Stream);

	// Sauvegarde id item cible reporter apr�s r�cup�ration offre 
	// (pour gestion construction de cet item)
	Stream.Send(doc->m_IdItemSel);

	// Save Plan & Hypotheses
	Stream				.Send(doc->m_IdxHypotheseCourant);
	Stream				.Send(doc->m_IdxHypotheseActif);

	Count  = doc->m_TblHypotheses.GetCount();
	Stream				.Send(Count);
	for(doc->m_TblHypotheses	.MoveFirst();
		doc->m_TblHypotheses	.IsValid();
		doc->m_TblHypotheses	.MoveNext())
	{
		doc->m_TblHypotheses.GetItem().Send(Stream);
	}

	// Archive Plan Master
	doc->m_MasterPlan.Send(Stream);

	// Archive Historique Plan Master
	Count  = doc->m_TblPlanArchive.GetCount();
	Stream				.Send(Count);
	for(doc->m_TblPlanArchive.MoveFirst();
		doc->m_TblPlanArchive.IsValid();
		doc->m_TblPlanArchive.MoveNext())
	{
		doc->m_TblPlanArchive.GetItem().Send(Stream);
	}
}

void CATPDocSerializer::ReadV4(JStream & Stream, JATPDocCreuset * doc)
{
	// on r�cup�re l'offre au d�but
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);

	// R�cup �tude et p�riodes
	JATPKEYEtude			KEYEtude;				KEYEtude			.Recv(Stream);
	JATPKEYPeriodeAction	KEYPeriodeAction;		KEYPeriodeAction	.Recv(Stream);
	JATPKEYPeriodeAction	KEYPeriodeActionHyp;	KEYPeriodeActionHyp	.Recv(Stream);
	JATPKEYPeriodeAction	KEYPeriodeActionTablier;KEYPeriodeActionTablier	.Recv(Stream);

	// R�cup�ration id item cible reporter apr�s r�cup�ration offre 
	// (pour gestion construction de cet item)
	// JUnt32		IdItemSel;				Stream.Recv(IdItemSel);

	JUnt32		IdSrcAudienceSel;		Stream.Recv(IdSrcAudienceSel);
	JUnt32		IdSrcCiblageSel;		Stream.Recv(IdSrcCiblageSel);

	// Map id terrains s�lectionn�s
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

	// Map �tat des formats
													Stream				.Recv(Count);
	JMap<JUnt32,JUnt32>		MapIdFormatsSel;
	for(int x = 0; x < Count; x++)
	{
		JUnt32				Key;					Stream				.Recv(Key);
		JUnt32				Item;					Stream				.Recv(Item);
		MapIdFormatsSel.Add(Key) = Item;
	}

	// R�cup�ration des plateaux (plateau �tendu et plateau hypoth�ses)
	JATPEchelleSupports		EchelleSupports;		EchelleSupports		.Recv(Stream);
	JATPPlateauSupports		PlateauSupports;		PlateauSupports		.Recv(Stream);
	JATPPlateauSupports		PlatSuppPlanEtendu;		PlatSuppPlanEtendu	.Recv(Stream);

	/*
	// on r�cup�re l'offre
	JMap <JInt32,const IItem *> MapCibleOffre;
	JATPOffre				Offre;					Offre				.Recv(Stream, MapCibleOffre);
	*/

	// R�cup�ration id item cible reporter apr�s r�cup�ration offre 
	JUnt32		IdItemSel;				Stream.Recv(IdItemSel);

	// On v�rifie qu'elle se trouve bien dans le map des cibles s�lectionn�es
	MapCibleOffre.MoveTo(IdItemSel);
	if (MapCibleOffre.IsValid()) 
	{
		// Item cible trouv�e 
		IdItemSel = MapCibleOffre.GetItem()->AsItemConstruit()->GetID();
	}
	else
	{
		// Voir si c'est une cible pond�r�e
		// CItemPondere * pItemPondere = CItemsConstruits::GetInstance()->GetItemPondere(IdItemSel);
		CItemPondere * pItemPondere = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemPondere(IdItemSel);
		if (pItemPondere)
		{
			IdItemSel = IdItemSel;
		}
		else
			// sinon on prend la cible ensemble
			IdItemSel = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemEnsemble()->GetID();
	}

	JUnt32		IdxHypotheseCourant;		Stream.Recv(IdxHypotheseCourant);
	JUnt32		IdxHypotheseActif;			Stream.Recv(IdxHypotheseActif);

	// R�cup les hypoth�ses
	Stream					.Recv(Count);
	JList<CPlanInsertions> TblHypotheses;
	for(int x = 0; x < Count; x++)
	{
		TblHypotheses.AddTail().Recv(Stream);
	}

	// R�cup le plan master courant
	CPlanInsertions		MasterPlan;				MasterPlan.Recv(Stream);

	// R�cup historique plans masters
	Stream					.Recv(Count);
	LIST_PLANMASTERS TblPlanArchive;
	for(int x = 0; x < Count; x++)
	{
		TblPlanArchive.AddTail().Recv(Stream);
	}

	// Swap the elements
	doc->m_KEYEtude					.Swap(KEYEtude);
	doc->m_KEYPeriodeAction			.Swap(KEYPeriodeAction);
	doc->m_KEYPeriodeActionHyp		.Swap(KEYPeriodeActionHyp);
	doc->m_KEYPeriodeActionTablier	.Swap(KEYPeriodeActionTablier);

	doc->m_IdItemSel = IdItemSel;
	doc->m_IdSrcAudienceSel = IdSrcAudienceSel;
	doc->m_IdSrcCiblageSel  = IdSrcCiblageSel;

	// Map id terrains s�lectionn�s
	doc->m_MapIdTerrainsAudienceSel.Swap(MapIdTerrainAudienceSel);

	doc->m_IdTerrainCiblageSel	= IdTerrainCiblageSel;
	doc->m_IdSegmentAudienceSel = IdSegmentAudienceSel;
	doc->m_IdSegmentCibleSel	= IdSegmentCibleSel;
	doc->m_IdFiltreAudienceSel  = IdFiltreAudienceSel;

	// Map �tat des formats
	doc->m_MapIdFormatsSel.Swap(MapIdFormatsSel);

	// R�cup�ration des plateaux (plateau �tendu et plateau hypoth�ses)
	doc->m_EchelleSupports.Swap(EchelleSupports);
	doc->m_PlateauSupports.Swap(PlateauSupports);
	doc->m_PlateauSupportsPlanEtendu.Swap(PlatSuppPlanEtendu);

	// R�cup�ration de l'offre
	doc->m_Offre.Swap(Offre);

	doc->m_IdxHypotheseCourant = IdxHypotheseCourant;
	doc->m_IdxHypotheseActif = IdxHypotheseActif;

	// R�cup les hypoth�ses
	doc->m_TblHypotheses.Swap(TblHypotheses);

	// R�cup le plan master courant
	doc->m_MasterPlan.Swap(MasterPlan);

	// R�cup historique plans masters
	doc->m_TblPlanArchive.Swap(TblPlanArchive);

	doc->m_mapSubstFormat.Reset();

	doc->m_listFormatsQP.Reset();
}

void CATPDocSerializer::WriteV5(JStream & Stream, JATPDocCreuset * doc)
{
	WriteV4(Stream, doc);
		
	JInt32 Count  = doc->m_mapSubstFormat.GetCount();
	Stream				.Send(Count);
	for(doc->m_mapSubstFormat	.MoveFirst();
		doc->m_mapSubstFormat	.IsValid();
		doc->m_mapSubstFormat	.MoveNext())
	{
		Stream				.Send(doc->m_mapSubstFormat.GetKey());
		Stream				.Send(doc->m_mapSubstFormat.GetItem());
	}

	Count  = doc->m_listFormatsQP.GetCount();
	Stream				.Send(Count);
	for(doc->m_listFormatsQP	.MoveFirst();
		doc->m_listFormatsQP	.IsValid();
		doc->m_listFormatsQP	.MoveNext())
	{
		Stream				.Send(doc->m_listFormatsQP.GetItem());
	}

}

void CATPDocSerializer::ReadV5(JStream & Stream, JATPDocCreuset * doc)
{
	ReadV4(Stream, doc);

	JInt32 Count;
	Stream					.Recv(Count);
	for(int x = 0; x < Count; x++)
	{
		JInt32 key, item;
		Stream					.Recv(key);
		Stream					.Recv(item);

		doc->m_mapSubstFormat.Add(key) = item;
	}

	Stream					.Recv(Count);
	for(int x = 0; x < Count; x++)
	{
		JInt32 read;
		Stream					.Recv(read);
		doc->m_listFormatsQP.AddTail() = read;
	}

	//JMap<JInt32,JInt32> m_mapSubstFormat;

	//JList<JUnt32> m_listFormatsQP;

}

///////////////////////////////////////////////////////////////////////////////////////
// Idem Version 5, mais pour concordance offre avec Analyse de l'offre (modif Famille)
// on passe � la version 7
void CATPDocSerializer::WriteV7(JStream & Stream, JATPDocCreuset * doc)
{
	// Idem sauvegarde version 5
	WriteV5(Stream, doc);

	// en + sauvegarde info tarif manuel
	JInt32 Count  = doc->m_MapTarifManuel.GetCount();
	Stream				.Send(Count);
	for(doc->m_MapTarifManuel	.MoveFirst();
		doc->m_MapTarifManuel	.IsValid();
		doc->m_MapTarifManuel	.MoveNext())
	{

		// Sauve Cl� Tarif Manuel
		CKeyTarifManuel KeyTarifManuel = doc->m_MapTarifManuel.GetKey();
		Stream				.Send(KeyTarifManuel.m_IdSupport);
		Stream				.Send(KeyTarifManuel.m_IdFormat);

		// Sauve tarif manuel
		Stream				.Send(doc->m_MapTarifManuel.GetItem());
	}
}

void CATPDocSerializer::ReadV7(JStream & Stream, JATPDocCreuset * doc)
{
	// Idem lecture version 5
	ReadV5(Stream, doc);

	// en + lecture des tarifs manuels
	JInt32 Count;
	Stream					.Recv(Count);
	for(int x = 0; x < Count; x++)
	{
		CKeyTarifManuel	KeyTarifManuel;
		JFlt32			Tarif;

		// R�cup cl� tarif
		Stream					.Recv(KeyTarifManuel.m_IdSupport);
		Stream					.Recv(KeyTarifManuel.m_IdFormat);

		// R�cup Tarif
		Stream					.Recv(Tarif);	

		doc->m_MapTarifManuel.Add(KeyTarifManuel) = Tarif;
	}
}	

///////////////////////////////////////////////////////////////////////////////////////
// Idem Version 5, mais pour concordance offre avec Analyse de l'offre (modif Famille)
// on passe � la version 8
void CATPDocSerializer::WriteV8(JStream & Stream, JATPDocCreuset * doc)
{
	// Idem sauvegarde version 7
	WriteV7(Stream, doc);

	// en + sauvegarde mode cible pondere activ� ou pas
	Stream.Send(doc->m_CiblesPondsActives);
}

void CATPDocSerializer::ReadV8(JStream & Stream, JATPDocCreuset * doc)
{
	// Idem lecture version 7, avec en plus lecture mode cible pond�r�
	ReadV7(Stream, doc);

	// en + lecture mode cible pond�r�
	Stream					.Recv(doc->m_CiblesPondsActives);
}