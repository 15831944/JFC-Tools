//
// Fichier: JMTFR03RequeteE.cpp
// Auteur:  Sylvain SAMMURI
// Date:    21/06/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03RequeteE.h"

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

////////////////////
// les constructeurs

JMTFR03RequeteE::JMTFR03RequeteE()
{
	// on ne fait rien
}

JMTFR03RequeteE::JMTFR03RequeteE(const JMTFR03RequeteE & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03RequeteE & JMTFR03RequeteE::operator =(const JMTFR03RequeteE & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////
// la fonction pour tester le filtre des modulations

JBool JMTFR03RequeteE::IsFilterActive(const JMTFR03FLTSegments & FLTSegments) const
{
	// on récupère le nombre de segments
	JInt32 NbSegments = FLTSegments.GetCount();

	// on teste la sélection de tous les segments
	for (JInt32 IdxSegment = 0; IdxSegment < NbSegments; IdxSegment += 1) if (FLTSegments.GetSelItem(IdxSegment)) return (true);

	// le filtre est inactif
	return (false);
}

/////////////////////////////////////////
// la fonction pour construire la requête

JBool JMTFR03RequeteE::OnMakeRequete(const JMTFR03DATTerrain & DATTerrain, const JMTFR03DATModulation & DATModulation, JRequete & Requete)
{
	// on initialise l'indicateur
	JBool Trouve = false;

	// on initialise les éléments de l'univers
	const JMTFR03KEYUnivers* pKEYUnivers = 0;
	const JMTFR03DATUnivers* pDATUnivers = 0;

	// on se place sur le premier univers
	DATTerrain.m_TBLUnivers.MoveFirst();

	// on boucle sur tous les univers
	while (!Trouve && DATTerrain.m_TBLUnivers.IsValid())
	{
		// on récupère l'univers courant
		const JMTFR03KEYUnivers & KEYUnivers = DATTerrain.m_TBLUnivers.GetKey();
		const JMTFR03DATUnivers & DATUnivers = DATTerrain.m_TBLUnivers.GetItem();

		// on teste s'il s'agit de l'univers de référence
		if (DATUnivers.m_Etat.GetAt(0)) { pKEYUnivers = &KEYUnivers; pDATUnivers = &DATUnivers; Trouve = true; }

		// on passe à l'univers suivant
		DATTerrain.m_TBLUnivers.MoveNext();
	}

	// on teste si on a trouvé l'univers de référence
	if (!Trouve) { JInt32 LOG_A_FAIRE; return (false); }

	// on récupère le nombre d'individus
	JInt32 NbIndividus = pDATUnivers->m_ItemIndividus.GetCount();

	// on alloue l'item de l'effectif
	Requete.m_ItemEffectif.SetCount(NbIndividus);

	// on récupère la table des échantillons et des homothéties
	Requete.m_pTBLEchantillons = &DATTerrain.m_TBLEchantillons;
	Requete.m_pTBLHomotheties  = &DATTerrain.m_TBLHomotheties;

	// on crée la clé des poids
	JMTFR03KEYPoids KEYPoids;

	// on récupère l'indice du type de poids des modulations
	JInt32 IdxPoids = DATModulation.m_FLTPoids.GetSelItem(); if (IdxPoids < 0) { JInt32 LOG_A_FAIRE; return (false); }

	// on initialise la clé des poids
	KEYPoids.m_IdPoids   = DATModulation.m_FLTPoids.Item(IdxPoids);
	KEYPoids.m_IdUnivers = pKEYUnivers->m_IdUnivers;

	// on recherche les poids
	DATTerrain.m_TBLPoids.MoveTo(KEYPoids); if (!DATTerrain.m_TBLPoids.IsValid()) { JInt32 LOG_A_FAIRE; return (false); }

	// on récupère les poids
	Requete.m_pKEYPoids = &DATTerrain.m_TBLPoids.GetKey();
	Requete.m_pDATPoids = &DATTerrain.m_TBLPoids.GetItem();

	// on alloue l'item des échantillons
	JBitArray ItemEchantillon; ItemEchantillon.SetCount(NbIndividus);

	// on boucle sur tous les échantillons
	for (DATTerrain.m_TBLEchantillons.MoveFirst(); DATTerrain.m_TBLEchantillons.IsValid(); DATTerrain.m_TBLEchantillons.MoveNext())
	{
		// on récupère l'échantillon courant
		const JMTFR03KEYEchantillon & KEYEchantillon = DATTerrain.m_TBLEchantillons.GetKey();
		const JMTFR03DATEchantillon & DATEchantillon = DATTerrain.m_TBLEchantillons.GetItem();

		// on met à jour l'item des échantillons
		ItemEchantillon |= DATEchantillon.m_ItemIndividus;
	}

	// on initialise le filtre des segments
	JBool FSegment = this->IsFilterActive(DATModulation.m_FLTSegments);

	// on alloue l'item des segments
	JBitArray ItemSegment; ItemSegment.SetCount(NbIndividus);

	// on boucle sur tous les segments
	for (DATTerrain.m_TBLSegments.MoveFirst(); DATTerrain.m_TBLSegments.IsValid(); DATTerrain.m_TBLSegments.MoveNext())
	{
		// on récupère le segment courant
		const JMTFR03KEYSegment & KEYSegment = DATTerrain.m_TBLSegments.GetKey();
		const JMTFR03DATSegment & DATSegment = DATTerrain.m_TBLSegments.GetItem();

		// on recherche l'indice du segment dans les modulations
		JInt32 IdxSegment = DATModulation.m_FLTSegments.FindItem(KEYSegment.m_IdSegment); if (IdxSegment < 0) { JInt32 LOG_A_FAIRE; return (false); }

		// on met à jour l'item des segments
		if (!FSegment || DATModulation.m_FLTSegments.GetSelItem(IdxSegment)) ItemSegment |= DATSegment.m_ItemIndividus;
	}

	// on initialise l'item des effectifs
	Requete.m_ItemEffectif |= pDATUnivers->m_ItemIndividus;
	Requete.m_ItemEffectif &= ItemEchantillon;
	Requete.m_ItemEffectif &= ItemSegment;

	// on quitte
	return (true);
}

///////////////////////////////////////
// la fonction pour tester les éléments

JBool JMTFR03RequeteE::IsValid() const
{
	// on teste les éléments
	return (m_TBLItems.GetCount() > 0);
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03RequeteE::SetItems(const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JRef32 & IdSource, const JRef32 & IdTerrain)
{
	// on vérifie la validité des identifiants
	if (!IdSource.IsValid() || !IdTerrain.IsValid()) throw JInvalidCall::GetInstance();

	// on crée la clé du terrain
	JMTFR03KEYTerrain KEYTerrain;

	// on initialise la clé du terrain
	KEYTerrain.m_IdSource  = IdSource;
	KEYTerrain.m_IdTerrain = IdTerrain;

	// on recherche les éléments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les éléments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on crée la clé des modulations
	JMTFR03KEYModulation KEYModulation;

	// on initialise la clé des modulations
	KEYModulation.m_IdSource  = IdSource;
	KEYModulation.m_IdTerrain = IdTerrain;

	// on recherche les modulations
	TBLModulations.MoveTo(KEYModulation); if (!TBLModulations.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les modulations
	const JMTFR03DATModulation & DATModulation = TBLModulations.GetItem();

	// on crée les nouveaux éléments
	JArray<JRequete> TBLItems; TBLItems.SetCount(1);

	// on construit la requête
	if (!this->OnMakeRequete(DATTerrain, DATModulation, TBLItems.Item(0))) throw JMTFR03Exception::GetInstance();

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JMTFR03RequeteE::JRequete & JMTFR03RequeteE::GetItems() const
{
	// on renvoie les éléments
	return (m_TBLItems.Item(0));
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03RequeteE::Swap(JMTFR03RequeteE & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03RequeteE::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03RequeteE::~JMTFR03RequeteE()
{
	// on ne fait rien
}
