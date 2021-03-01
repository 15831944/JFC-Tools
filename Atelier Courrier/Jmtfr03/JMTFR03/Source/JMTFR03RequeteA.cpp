	//
// Fichier: JMTFR03RequeteA.cpp
// Auteur:  Sylvain SAMMURI
// Date:    21/06/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03RequeteA.h"

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

// on initialise les types standards des titres
static const JInt32 g_IdQuotidien     = 1;
static const JInt32 g_IdMagazine      = 2;
static const JInt32 g_IdQuotidien6Jrs = 3;
static const JInt32 g_IdQuotidien5Jrs = 4;
static const JInt32 g_IdQuotidien1Jr  = 5;

////////////////////
// les constructeurs

JMTFR03RequeteA::JMTFR03RequeteA()
{
	// on ne fait rien
}

JMTFR03RequeteA::JMTFR03RequeteA(const JMTFR03RequeteA & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03RequeteA & JMTFR03RequeteA::operator =(const JMTFR03RequeteA & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////
// la fonction pour tester le filtre des modulations

JBool JMTFR03RequeteA::IsFilterActive(const JMTFR03FLTSegments & FLTSegments) const
{
	// on récupère le nombre de segments
	JInt32 NbSegments = FLTSegments.GetCount();

	// on teste la sélection de tous les segments
	for (JInt32 IdxSegment = 0; IdxSegment < NbSegments; IdxSegment += 1) if (FLTSegments.GetSelItem(IdxSegment)) return (true);

	// le filtre est inactif
	return (false);
}

///////////////////////////////////////////////////////////
// la fonction pour rechercher les univers de redressements

JVoid JMTFR03RequeteA::OnFindUnivers(const JMTFR03DATTerrain & DATTerrain, const JBitArray & ItemUnivers, const JRef32 & IdEchantillon, const JRef32 & IdPoids, const JMTFR03KEYPoids* & pKEYPoids, const JMTFR03DATPoids* & pDATPoids, const JMTFR03KEYHomothetie* & pKEYHomothetie, const JMTFR03DATHomothetie* & pDATHomothetie) const
{
	// on initialise les indicateurs
	JBool Trouve = false;
	JBool Defaut = false;

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

		// on teste s'il s'agit d'un univers compatible
		if (DATUnivers.m_ItemIndividus == ItemUnivers) { pKEYUnivers = &KEYUnivers; pDATUnivers = &DATUnivers; Trouve = true; }

		// on teste s'il s'agit de l'univers par défaut
		if (DATUnivers.m_Etat.GetAt(0)) { Defaut = true; }

		// on passe à l'univers suivant
		DATTerrain.m_TBLUnivers.MoveNext();
	}

	// on boucle sur tous les univers restants, jusqu'à l'univers par défaut
	while (!Defaut && DATTerrain.m_TBLUnivers.IsValid())
	{
		// on récupère l'univers courant
		const JMTFR03KEYUnivers & KEYUnivers = DATTerrain.m_TBLUnivers.GetKey();
		const JMTFR03DATUnivers & DATUnivers = DATTerrain.m_TBLUnivers.GetItem();

		// on teste s'il s'agit de l'univers par défaut
		if (DATUnivers.m_Etat.GetAt(0))
		{
			// on teste s'il s'agit d'un univers compatible
			if (DATUnivers.m_ItemIndividus == ItemUnivers) { pKEYUnivers = &KEYUnivers; pDATUnivers = &DATUnivers; Trouve = true; }

			// on met à jour l'indicateur
			Defaut = true;
		}

		// on passe à l'univers suivant
		DATTerrain.m_TBLUnivers.MoveNext();
	}

	// on teste si on a trouvé un univers
	if (Trouve)
	{
		// on crée les clés de l'homothétie et des poids
		JMTFR03KEYHomothetie KEYHomothetie;
		JMTFR03KEYPoids      KEYPoids;

		// on initialise les clés de l'homothétie et des poids
		KEYHomothetie.m_IdEchantillon = IdEchantillon;
		KEYHomothetie.m_IdUnivers     = pKEYUnivers->m_IdUnivers;
		KEYPoids     .m_IdPoids       = IdPoids;
		KEYPoids     .m_IdUnivers     = pKEYUnivers->m_IdUnivers;

		// on recherche l'homothétie et les poids
		DATTerrain.m_TBLHomotheties.MoveTo(KEYHomothetie);
		DATTerrain.m_TBLPoids      .MoveTo(KEYPoids);

		// on teste si on a trouvé l'homothétie et les poids
		if (DATTerrain.m_TBLHomotheties.IsValid() && DATTerrain.m_TBLPoids.IsValid())
		{
			// on renvoie les poids
			pKEYPoids = &DATTerrain.m_TBLPoids.GetKey();
			pDATPoids = &DATTerrain.m_TBLPoids.GetItem();

			// on renvoie les homothéties
			pKEYHomothetie = &DATTerrain.m_TBLHomotheties.GetKey();
			pDATHomothetie = &DATTerrain.m_TBLHomotheties.GetItem();
		}
	}
}

/////////////////////////////////////////
// la fonction pour construire la requête

JBool JMTFR03RequeteA::OnMakeRequete(const JMTFR03DATTerrain & DATTerrain, const JMTFR03DATModulation & DATModulation, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JRequete & Requete)
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

	// on alloue l'item des filtres
	JBitArray ItemFiltre; ItemFiltre.SetCount(NbIndividus);

	// on initialise l'indicateur
	JBool FgEchantillon = true;

	// on initialise l'échantillon
	Requete.m_pKEYEchantillon = 0;
	Requete.m_pDATEchantillon = 0;

	// on initialise les indicateurs
	JBool FgUnvVariablesRef    = true;
	JBool FgUnvProbabilitesRef = true;
	JBool FgUnvVariablesDDLRef = true;
	JBool FgUnvReprises        = true;

	// on initialise les poids
	Requete.m_pKEYPoidsVariablesRef    = 0;
	Requete.m_pDATPoidsVariablesRef    = 0;
	Requete.m_pKEYPoidsProbabilitesRef = 0;
	Requete.m_pDATPoidsProbabilitesRef = 0;
	Requete.m_pKEYPoidsVariablesDDLRef = 0;
	Requete.m_pDATPoidsVariablesDDLRef = 0;
	Requete.m_pKEYPoidsReprises        = 0;
	Requete.m_pDATPoidsReprises        = 0;

	// on initialise les homothéties
	Requete.m_pKEYHomothetieVariablesRef    = 0;
	Requete.m_pDATHomothetieVariablesRef    = 0;
	Requete.m_pKEYHomothetieProbabilitesRef = 0;
	Requete.m_pDATHomothetieProbabilitesRef = 0;
	Requete.m_pKEYHomothetieVariablesDDLRef = 0;
	Requete.m_pDATHomothetieVariablesDDLRef = 0;
	Requete.m_pKEYHomothetieReprises        = 0;
	Requete.m_pDATHomothetieReprises        = 0;

	// on alloue les items des filtres
	JBitArray ItemUnvVariablesRef;    ItemUnvVariablesRef   .SetCount(NbIndividus); ItemUnvVariablesRef    |= true;
	JBitArray ItemUnvProbabilitesRef; ItemUnvProbabilitesRef.SetCount(NbIndividus); ItemUnvProbabilitesRef |= true;
	JBitArray ItemUnvVariablesDDLRef; ItemUnvVariablesDDLRef.SetCount(NbIndividus); ItemUnvVariablesDDLRef |= true;
	JBitArray ItemUnvReprises;        ItemUnvReprises       .SetCount(NbIndividus); ItemUnvReprises        |= true;

	// on alloue l'item des parutions
	JBitArray ItemParution; ItemParution.SetCount(NbIndividus);

	// on récupère l'indice du filtre des modulations
	JInt32 IdxFiltre = DATModulation.m_FLTFiltres.GetSelItem(); if (IdxFiltre < 0) { JInt32 LOG_A_FAIRE; return (false); }

	// on boucle sur tous les titres de la maille
	for (Maille.MoveFirst(); Maille.IsValid(); Maille.MoveNext())
	{
		// on récupère le titre courant
		const JRef32  & IdTitre      = Maille.GetIdTitre();
		const JRef32  & IdTuteur     = Maille.GetIdTuteur();
		const JInt32x & NbInsertions = Maille.GetNbInsertions();
		const JFlt64x & GRPRef       = Maille.GetGRPRef();

		// on crée la clé des attributs 
		JMTFR03KEYAttribut KEYAttribut;

		// on initialise la clé des attributs
		KEYAttribut.m_IdTitre = IdTuteur;

		// on recherche les attributs du titre
		DATTerrain.m_TBLAttributs.MoveTo(KEYAttribut);

		// on teste si on a trouvé les attributs du titre
		if (DATTerrain.m_TBLAttributs.IsValid())
		{
			// on récupère les attributs du titre
			const JMTFR03DATAttribut & DATAttribut = DATTerrain.m_TBLAttributs.GetItem();

			// on teste le type du titre
			if (DATAttribut.m_IdType.AsJInt32() == g_IdQuotidien || DATAttribut.m_IdType.AsJInt32() == g_IdMagazine || DATAttribut.m_IdType.AsJInt32() == g_IdQuotidien6Jrs || DATAttribut.m_IdType.AsJInt32() == g_IdQuotidien5Jrs  || DATAttribut.m_IdType.AsJInt32() == g_IdQuotidien1Jr)
			{
				// on crée les clés de l'échantillon et du filtre
				JMTFR03KEYEchantillon KEYEchantillon;
				JMTFR03KEYFiltre      KEYFiltre;

				// on initialise les clés de l'échantillon et du filtre
				KEYEchantillon.m_IdEchantillon = DATAttribut.m_IdEchantillon;
				KEYFiltre     .m_IdFiltre      = DATModulation.m_FLTFiltres.Item(IdxFiltre);
				KEYFiltre     .m_IdTitre       = IdTuteur;

				// on recherche l'échantillon et le filtre
				DATTerrain.m_TBLEchantillons.MoveTo(KEYEchantillon);
				DATTerrain.m_TBLFiltres     .MoveTo(KEYFiltre);

				// on teste si on a trouvé l'échantillon et le filtre
				if (!DATTerrain.m_TBLEchantillons.IsValid()) { JInt32 LOG_A_FAIRE; }
				if (!DATTerrain.m_TBLFiltres     .IsValid()) { JInt32 LOG_A_FAIRE; }

				// on teste si on a trouvé l'échantillon et le filtre
				if (DATTerrain.m_TBLEchantillons.IsValid() && DATTerrain.m_TBLFiltres.IsValid())
				{
					// on récupère les éléments du filtre et de l'échantillon
					const JMTFR03DATFiltre      & DATFiltre      = DATTerrain.m_TBLFiltres     .GetItem();
					const JMTFR03DATEchantillon & DATEchantillon = DATTerrain.m_TBLEchantillons.GetItem();

					// on ajoute une nouvelle parution
					JParution & Parution = Requete.m_TBLParutions.AddTail(true);

					// on initialise le type du support
					Parution.m_Type            = DATAttribut.m_IdType.AsJInt32();
					Parution.m_IdTitre         = IdTitre .AsJInt32();
					Parution.m_IdTuteur        = IdTuteur.AsJInt32();
					Parution.m_NbInsertions    = NbInsertions.AsJInt32();
					Parution.m_GRPRef          = GRPRef.AsJFlt64();
					Parution.m_PtEtalonnageVol = 0;
					Parution.m_PtEtalonnageRef = DATAttribut.m_PtEtalonnage.AsJInt32();

					// on crée les clé des reprises, des variables et des lectorats
					JMTFR03KEYVariable KEYVariable;
					JMTFR03KEYLectorat KEYLectorat;
					JMTFR03KEYReprise  KEYReprise;

					// on initialise les clé des reprises, des variables et des lectorats
					KEYVariable.m_IdTitre = IdTuteur;
					KEYLectorat.m_IdTitre = IdTuteur;
					KEYReprise .m_IdTitre = IdTuteur;

					// on initialise les audiences de références
					Parution.m_pDATVariablesRef     = 0;
					Parution.m_pDATProbabilitesRef  = 0;
					Parution.m_pDATVariablesDDL_Vol = 0;
					Parution.m_pDATVariablesDDL_Ref = 0;

					// on récupère les reprises, les variables LDP et LNM, les probabilités LDP et LNM et les variable DDL
					DATTerrain.m_TBLVariablesLDP    .MoveTo(KEYVariable); if (DATTerrain.m_TBLVariablesLDP    .IsValid()) Parution.m_pDATVariablesLDP     = &DATTerrain.m_TBLVariablesLDP    .GetItem(); else Parution.m_pDATVariablesLDP     = 0;
					DATTerrain.m_TBLVariablesLNM    .MoveTo(KEYVariable); if (DATTerrain.m_TBLVariablesLNM    .IsValid()) Parution.m_pDATVariablesLNM     = &DATTerrain.m_TBLVariablesLNM    .GetItem(); else Parution.m_pDATVariablesLNM     = 0;
					DATTerrain.m_TBLProbabilitesLDP .MoveTo(KEYVariable); if (DATTerrain.m_TBLProbabilitesLDP .IsValid()) Parution.m_pDATProbabilitesLDP  = &DATTerrain.m_TBLProbabilitesLDP .GetItem(); else Parution.m_pDATProbabilitesLDP  = 0;
					DATTerrain.m_TBLProbabilitesLNM .MoveTo(KEYVariable); if (DATTerrain.m_TBLProbabilitesLNM .IsValid()) Parution.m_pDATProbabilitesLNM  = &DATTerrain.m_TBLProbabilitesLNM .GetItem(); else Parution.m_pDATProbabilitesLNM  = 0;
					DATTerrain.m_TBLVariablesDDL_001.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_001.IsValid()) Parution.m_pDATVariablesDDL_001 = &DATTerrain.m_TBLVariablesDDL_001.GetItem(); else Parution.m_pDATVariablesDDL_001 = 0;
					DATTerrain.m_TBLVariablesDDL_XX1.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_XX1.IsValid()) Parution.m_pDATVariablesDDL_XX1 = &DATTerrain.m_TBLVariablesDDL_XX1.GetItem(); else Parution.m_pDATVariablesDDL_XX1 = 0;
					DATTerrain.m_TBLVariablesDDL_002.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_002.IsValid()) Parution.m_pDATVariablesDDL_002 = &DATTerrain.m_TBLVariablesDDL_002.GetItem(); else Parution.m_pDATVariablesDDL_002 = 0;
					DATTerrain.m_TBLVariablesDDL_004.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_004.IsValid()) Parution.m_pDATVariablesDDL_004 = &DATTerrain.m_TBLVariablesDDL_004.GetItem(); else Parution.m_pDATVariablesDDL_004 = 0;
					DATTerrain.m_TBLVariablesDDL_007.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_007.IsValid()) Parution.m_pDATVariablesDDL_007 = &DATTerrain.m_TBLVariablesDDL_007.GetItem(); else Parution.m_pDATVariablesDDL_007 = 0;
					DATTerrain.m_TBLVariablesDDL_014.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_014.IsValid()) Parution.m_pDATVariablesDDL_014 = &DATTerrain.m_TBLVariablesDDL_014.GetItem(); else Parution.m_pDATVariablesDDL_014 = 0;
					DATTerrain.m_TBLVariablesDDL_030.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_030.IsValid()) Parution.m_pDATVariablesDDL_030 = &DATTerrain.m_TBLVariablesDDL_030.GetItem(); else Parution.m_pDATVariablesDDL_030 = 0;
					DATTerrain.m_TBLVariablesDDL_060.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_060.IsValid()) Parution.m_pDATVariablesDDL_060 = &DATTerrain.m_TBLVariablesDDL_060.GetItem(); else Parution.m_pDATVariablesDDL_060 = 0;
					DATTerrain.m_TBLVariablesDDL_090.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_090.IsValid()) Parution.m_pDATVariablesDDL_090 = &DATTerrain.m_TBLVariablesDDL_090.GetItem(); else Parution.m_pDATVariablesDDL_090 = 0;
					DATTerrain.m_TBLVariablesDDL_120.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_120.IsValid()) Parution.m_pDATVariablesDDL_120 = &DATTerrain.m_TBLVariablesDDL_120.GetItem(); else Parution.m_pDATVariablesDDL_120 = 0;
					DATTerrain.m_TBLVariablesDDL_180.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_180.IsValid()) Parution.m_pDATVariablesDDL_180 = &DATTerrain.m_TBLVariablesDDL_180.GetItem(); else Parution.m_pDATVariablesDDL_180 = 0;
					DATTerrain.m_TBLVariablesDDL_270.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_270.IsValid()) Parution.m_pDATVariablesDDL_270 = &DATTerrain.m_TBLVariablesDDL_270.GetItem(); else Parution.m_pDATVariablesDDL_270 = 0;
					DATTerrain.m_TBLVariablesDDL_365.MoveTo(KEYLectorat); if (DATTerrain.m_TBLVariablesDDL_365.IsValid()) Parution.m_pDATVariablesDDL_365 = &DATTerrain.m_TBLVariablesDDL_365.GetItem(); else Parution.m_pDATVariablesDDL_365 = 0;
					DATTerrain.m_TBLReprises        .MoveTo(KEYReprise);  if (DATTerrain.m_TBLReprises        .IsValid()) Parution.m_pDATReprises         = &DATTerrain.m_TBLReprises        .GetItem(); else Parution.m_pDATReprises         = 0;

					// on initialise le type d'audience de référence
					JInt32 TypeRef = 0;

					// on teste le type du titre
					if      (DATAttribut.m_IdType == g_IdQuotidien)     { if (Options.GetLDPQuotidiens()) TypeRef = 1; else TypeRef = 2; }
					else if (DATAttribut.m_IdType == g_IdMagazine)      { if (Options.GetLNMMagazines())  TypeRef = 2; else TypeRef = 1; }
					else if (DATAttribut.m_IdType == g_IdQuotidien6Jrs) { if (Options.GetLDPQuotidiens()) TypeRef = 1; else TypeRef = 2; }
					else if (DATAttribut.m_IdType == g_IdQuotidien5Jrs) { if (Options.GetLDPQuotidiens()) TypeRef = 1; else TypeRef = 2; }
					else if (DATAttribut.m_IdType == g_IdQuotidien1Jr)  { if (Options.GetLDPQuotidiens()) TypeRef = 1; else TypeRef = 2; }

					// on récupère les variables et les probabilités de référence
					if      (TypeRef == 1) { Parution.m_pDATVariablesRef = Parution.m_pDATVariablesLDP; Parution.m_pDATProbabilitesRef = Parution.m_pDATProbabilitesLDP; }
					else if (TypeRef == 2) { Parution.m_pDATVariablesRef = Parution.m_pDATVariablesLNM; Parution.m_pDATProbabilitesRef = Parution.m_pDATProbabilitesLNM; }

					// on récupère les variables DDL du titre au jour d'étalonnage DDL
					if (Parution.m_PtEtalonnageRef >=   1)                                        { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_001; Parution.m_PtEtalonnageVol =   1; }
					if (Parution.m_PtEtalonnageRef >=   2 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_002; Parution.m_PtEtalonnageVol =   2; }
					if (Parution.m_PtEtalonnageRef >=   4 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_004; Parution.m_PtEtalonnageVol =   4; }
					if (Parution.m_PtEtalonnageRef >=   7 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_007; Parution.m_PtEtalonnageVol =   7; }
					if (Parution.m_PtEtalonnageRef >=  14 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_014; Parution.m_PtEtalonnageVol =  14; }
					if (Parution.m_PtEtalonnageRef >=  30 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_030; Parution.m_PtEtalonnageVol =  30; }
					if (Parution.m_PtEtalonnageRef >=  60 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_060; Parution.m_PtEtalonnageVol =  60; }
					if (Parution.m_PtEtalonnageRef >=  90 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_090; Parution.m_PtEtalonnageVol =  90; }
					if (Parution.m_PtEtalonnageRef >= 120 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_120; Parution.m_PtEtalonnageVol = 120; }
					if (Parution.m_PtEtalonnageRef >= 180 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_180; Parution.m_PtEtalonnageVol = 180; }
					if (Parution.m_PtEtalonnageRef >= 270 && Parution.m_pDATVariablesDDL_Vol == 0) { Parution.m_pDATVariablesDDL_Vol = Parution.m_pDATVariablesDDL_270; Parution.m_PtEtalonnageVol = 270; }

					// on récupère les variables DDL du titre au point d'étalonnage
					if      (Parution.m_PtEtalonnageRef ==   1) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_001; }
					else if (Parution.m_PtEtalonnageRef ==   2) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_002; }
					else if (Parution.m_PtEtalonnageRef ==   4) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_004; }
					else if (Parution.m_PtEtalonnageRef ==   7) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_007; }
					else if (Parution.m_PtEtalonnageRef ==  14) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_014; }
					else if (Parution.m_PtEtalonnageRef ==  30) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_030; }
					else if (Parution.m_PtEtalonnageRef ==  60) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_060; }
					else if (Parution.m_PtEtalonnageRef ==  90) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_090; }
					else if (Parution.m_PtEtalonnageRef == 120) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_120; }
					else if (Parution.m_PtEtalonnageRef == 180) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_180; }
					else if (Parution.m_PtEtalonnageRef == 270) { Parution.m_pDATVariablesDDL_Ref = Parution.m_pDATVariablesDDL_270; }

					// on initialise les poids
					Parution.m_pKEYPoidsVariablesRef    = 0;
					Parution.m_pDATPoidsVariablesRef    = 0;
					Parution.m_pKEYPoidsProbabilitesRef = 0;
					Parution.m_pDATPoidsProbabilitesRef = 0;
					Parution.m_pKEYPoidsVariablesDDLRef = 0;
					Parution.m_pDATPoidsVariablesDDLRef = 0;
					Parution.m_pKEYPoidsReprises        = 0;
					Parution.m_pDATPoidsReprises        = 0;

					// on initialise les homothéties
					Parution.m_pKEYHomothetieVariablesRef    = 0;
					Parution.m_pDATHomothetieVariablesRef    = 0;
					Parution.m_pKEYHomothetieProbabilitesRef = 0;
					Parution.m_pDATHomothetieProbabilitesRef = 0;
					Parution.m_pKEYHomothetieVariablesDDLRef = 0;
					Parution.m_pDATHomothetieVariablesDDLRef = 0;
					Parution.m_pKEYHomothetieReprises        = 0;
					Parution.m_pDATHomothetieReprises        = 0;

					// on teste si la parution a des variables de référence
					if (Parution.m_pDATVariablesRef != 0)
					{
						// on crée les clés de l'univers, de l'homothétie et des poids
						JMTFR03KEYUnivers    KEYUnivers;
						JMTFR03KEYHomothetie KEYHomothetie;
						JMTFR03KEYPoids      KEYPoids;
			
						// on initialise les clés de l'univers, de l'homothétie et des poids
						KEYUnivers   .m_IdUnivers     = Parution.m_pDATVariablesRef->m_IdUnivers;
						KEYHomothetie.m_IdEchantillon = DATAttribut.m_IdEchantillon;
						KEYHomothetie.m_IdUnivers     = Parution.m_pDATVariablesRef->m_IdUnivers;
						KEYPoids     .m_IdPoids       = DATModulation.m_FLTPoids.Item(IdxPoids);
						KEYPoids     .m_IdUnivers     = Parution.m_pDATVariablesRef->m_IdUnivers;

						// on recherche l'univers, l'homothétie et les poids
						DATTerrain.m_TBLUnivers    .MoveTo(KEYUnivers);
						DATTerrain.m_TBLHomotheties.MoveTo(KEYHomothetie);
						DATTerrain.m_TBLPoids      .MoveTo(KEYPoids);

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (!DATTerrain.m_TBLUnivers    .IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLHomotheties.IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLPoids      .IsValid()) { JInt32 LOG_A_FAIRE; }

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (DATTerrain.m_TBLUnivers.IsValid() && DATTerrain.m_TBLHomotheties.IsValid() && DATTerrain.m_TBLPoids.IsValid())
						{
							// on mémorise les poids
							Parution.m_pKEYPoidsVariablesRef = &DATTerrain.m_TBLPoids.GetKey();
							Parution.m_pDATPoidsVariablesRef = &DATTerrain.m_TBLPoids.GetItem();

							// on mémorise l'homothétie
							Parution.m_pKEYHomothetieVariablesRef = &DATTerrain.m_TBLHomotheties.GetKey();
							Parution.m_pDATHomothetieVariablesRef = &DATTerrain.m_TBLHomotheties.GetItem();

							// on teste l'indicateur des univers
							if (FgUnvVariablesRef)
							{
								// on teste si on a déjà trouvé des poids
								if (Requete.m_pKEYPoidsVariablesRef == 0)
								{
									// on mémorise les poids
									Requete.m_pKEYPoidsVariablesRef = Parution.m_pKEYPoidsVariablesRef;
									Requete.m_pDATPoidsVariablesRef = Parution.m_pDATPoidsVariablesRef;

									// on mémorise l'homothétie
									Requete.m_pKEYHomothetieVariablesRef = Parution.m_pKEYHomothetieVariablesRef;
									Requete.m_pDATHomothetieVariablesRef = Parution.m_pDATHomothetieVariablesRef;
								}
								else if (Requete.m_pKEYPoidsVariablesRef != Parution.m_pKEYPoidsVariablesRef)
								{
									// on annule les poids
									Requete.m_pKEYPoidsVariablesRef = 0;
									Requete.m_pDATPoidsVariablesRef = 0;

									// on annule l'homothétie
									Requete.m_pKEYHomothetieVariablesRef = 0;
									Requete.m_pDATHomothetieVariablesRef = 0;

									// on annule l'indicateur
									FgUnvVariablesRef = false;
								}
							}

							// on met à jour l'item de l'univers
							ItemUnvVariablesRef &= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;

							// on met à jour l'item de la parution
							ItemParution |= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;
						}
					}

					// on teste si la parution a des probabilités de référence
					if (Parution.m_pDATProbabilitesRef != 0)
					{
						// on crée les clés de l'univers, de l'homothétie et des poids
						JMTFR03KEYUnivers    KEYUnivers;
						JMTFR03KEYHomothetie KEYHomothetie;
						JMTFR03KEYPoids      KEYPoids;
			
						// on initialise les clés de l'univers, de l'homothétie et des poids
						KEYUnivers   .m_IdUnivers     = Parution.m_pDATProbabilitesRef->m_IdUnivers;
						KEYHomothetie.m_IdEchantillon = DATAttribut.m_IdEchantillon;
						KEYHomothetie.m_IdUnivers     = Parution.m_pDATProbabilitesRef->m_IdUnivers;
						KEYPoids     .m_IdPoids       = DATModulation.m_FLTPoids.Item(IdxPoids);
						KEYPoids     .m_IdUnivers     = Parution.m_pDATProbabilitesRef->m_IdUnivers;

						// on recherche l'univers, l'homothétie et les poids
						DATTerrain.m_TBLUnivers    .MoveTo(KEYUnivers);
						DATTerrain.m_TBLHomotheties.MoveTo(KEYHomothetie);
						DATTerrain.m_TBLPoids      .MoveTo(KEYPoids);

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (!DATTerrain.m_TBLUnivers    .IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLHomotheties.IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLPoids      .IsValid()) { JInt32 LOG_A_FAIRE; }

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (DATTerrain.m_TBLUnivers.IsValid() && DATTerrain.m_TBLHomotheties.IsValid() && DATTerrain.m_TBLPoids.IsValid())
						{
							// on mémorise les poids
							Parution.m_pKEYPoidsProbabilitesRef = &DATTerrain.m_TBLPoids.GetKey();
							Parution.m_pDATPoidsProbabilitesRef = &DATTerrain.m_TBLPoids.GetItem();

							// on mémorise l'homothétie
							Parution.m_pKEYHomothetieProbabilitesRef = &DATTerrain.m_TBLHomotheties.GetKey();
							Parution.m_pDATHomothetieProbabilitesRef = &DATTerrain.m_TBLHomotheties.GetItem();

							// on teste l'indicateur des univers
							if (FgUnvProbabilitesRef)
							{
								// on teste si on a déjà trouvé des poids
								if (Requete.m_pKEYPoidsProbabilitesRef == 0)
								{
									// on mémorise les poids
									Requete.m_pKEYPoidsProbabilitesRef = Parution.m_pKEYPoidsProbabilitesRef;
									Requete.m_pDATPoidsProbabilitesRef = Parution.m_pDATPoidsProbabilitesRef;

									// on mémorise l'homothétie
									Requete.m_pKEYHomothetieProbabilitesRef = Parution.m_pKEYHomothetieProbabilitesRef;
									Requete.m_pDATHomothetieProbabilitesRef = Parution.m_pDATHomothetieProbabilitesRef;
								}
								else if (Requete.m_pKEYPoidsProbabilitesRef != Parution.m_pKEYPoidsProbabilitesRef)
								{
									// on annule les poids
									Requete.m_pKEYPoidsProbabilitesRef = 0;
									Requete.m_pDATPoidsProbabilitesRef = 0;

									// on annule l'homothétie
									Requete.m_pKEYHomothetieProbabilitesRef = 0;
									Requete.m_pDATHomothetieProbabilitesRef = 0;

									// on annule l'indicateur
									FgUnvProbabilitesRef = false;
								}
							}

							// on met à jour l'item de l'univers
							ItemUnvProbabilitesRef &= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;

							// on met à jour l'item de la parution
							ItemParution |= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;
						}
					}
					
					// on teste si la parution a une variable DDL de référence
					if (Parution.m_pDATVariablesDDL_Ref != 0)
					{
						// on crée les clés de l'univers, de l'homothétie et des poids
						JMTFR03KEYUnivers    KEYUnivers;
						JMTFR03KEYHomothetie KEYHomothetie;
						JMTFR03KEYPoids      KEYPoids;
			
						// on initialise les clés de l'univers, de l'homothétie et des poids
						KEYUnivers   .m_IdUnivers     = Parution.m_pDATVariablesDDL_Ref->m_IdUnivers;
						KEYHomothetie.m_IdEchantillon = DATAttribut.m_IdEchantillon;
						KEYHomothetie.m_IdUnivers     = Parution.m_pDATVariablesDDL_Ref->m_IdUnivers;
						KEYPoids     .m_IdPoids       = DATModulation.m_FLTPoids.Item(IdxPoids);
						KEYPoids     .m_IdUnivers     = Parution.m_pDATVariablesDDL_Ref->m_IdUnivers;

						// on recherche l'univers, l'homothétie et les poids
						DATTerrain.m_TBLUnivers    .MoveTo(KEYUnivers);
						DATTerrain.m_TBLHomotheties.MoveTo(KEYHomothetie);
						DATTerrain.m_TBLPoids      .MoveTo(KEYPoids);

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (!DATTerrain.m_TBLUnivers    .IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLHomotheties.IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLPoids      .IsValid()) { JInt32 LOG_A_FAIRE; }

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (DATTerrain.m_TBLUnivers.IsValid() && DATTerrain.m_TBLHomotheties.IsValid() && DATTerrain.m_TBLPoids.IsValid())
						{
							// on mémorise les poids
							Parution.m_pKEYPoidsVariablesDDLRef = &DATTerrain.m_TBLPoids.GetKey();
							Parution.m_pDATPoidsVariablesDDLRef = &DATTerrain.m_TBLPoids.GetItem();

							// on mémorise l'homothétie
							Parution.m_pKEYHomothetieVariablesDDLRef = &DATTerrain.m_TBLHomotheties.GetKey();
							Parution.m_pDATHomothetieVariablesDDLRef = &DATTerrain.m_TBLHomotheties.GetItem();

							// on teste l'indicateur des univers
							if (FgUnvVariablesDDLRef)
							{
								// on teste si on a déjà trouvé des poids
								if (Requete.m_pKEYPoidsVariablesDDLRef == 0)
								{
									// on mémorise les poids
									Requete.m_pKEYPoidsVariablesDDLRef = Parution.m_pKEYPoidsVariablesDDLRef;
									Requete.m_pDATPoidsVariablesDDLRef = Parution.m_pDATPoidsVariablesDDLRef;

									// on mémorise l'homothétie
									Requete.m_pKEYHomothetieVariablesDDLRef = Parution.m_pKEYHomothetieVariablesDDLRef;
									Requete.m_pDATHomothetieVariablesDDLRef = Parution.m_pDATHomothetieVariablesDDLRef;
								}
								else if (Requete.m_pKEYPoidsVariablesDDLRef != Parution.m_pKEYPoidsVariablesDDLRef)
								{
									// on annule les poids
									Requete.m_pKEYPoidsVariablesDDLRef = 0;
									Requete.m_pDATPoidsVariablesDDLRef = 0;

									// on annule l'homothétie
									Requete.m_pKEYHomothetieVariablesDDLRef = 0;
									Requete.m_pDATHomothetieVariablesDDLRef = 0;

									// on annule l'indicateur
									FgUnvVariablesDDLRef = false;
								}
							}

							// on met à jour l'item de l'univers
							ItemUnvVariablesDDLRef &= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;

							// on met à jour l'item de la parution
							ItemParution |= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;
						}
					}

					// on teste si la parution a des reprises
					if (Parution.m_pDATReprises != 0)
					{
						// on crée les clés de l'univers, de l'homothétie et des poids
						JMTFR03KEYUnivers    KEYUnivers;
						JMTFR03KEYHomothetie KEYHomothetie;
						JMTFR03KEYPoids      KEYPoids;
			
						// on initialise les clés de l'univers, de l'homothétie et des poids
						KEYUnivers   .m_IdUnivers     = Parution.m_pDATReprises->m_IdUnivers;
						KEYHomothetie.m_IdEchantillon = DATAttribut.m_IdEchantillon;
						KEYHomothetie.m_IdUnivers     = Parution.m_pDATReprises->m_IdUnivers;
						KEYPoids     .m_IdPoids       = DATModulation.m_FLTPoids.Item(IdxPoids);
						KEYPoids     .m_IdUnivers     = Parution.m_pDATReprises->m_IdUnivers;

						// on recherche l'univers, l'homothétie et les poids
						DATTerrain.m_TBLUnivers    .MoveTo(KEYUnivers);
						DATTerrain.m_TBLHomotheties.MoveTo(KEYHomothetie);
						DATTerrain.m_TBLPoids      .MoveTo(KEYPoids);

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (!DATTerrain.m_TBLUnivers    .IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLHomotheties.IsValid()) { JInt32 LOG_A_FAIRE; }
						if (!DATTerrain.m_TBLPoids      .IsValid()) { JInt32 LOG_A_FAIRE; }

						// on teste si on a trouvé l'univers, l'homothétie et les poids
						if (DATTerrain.m_TBLUnivers.IsValid() && DATTerrain.m_TBLHomotheties.IsValid() && DATTerrain.m_TBLPoids.IsValid())
						{
							// on mémorise les poids
							Parution.m_pKEYPoidsReprises = &DATTerrain.m_TBLPoids.GetKey();
							Parution.m_pDATPoidsReprises = &DATTerrain.m_TBLPoids.GetItem();

							// on mémorise l'homothétie
							Parution.m_pKEYHomothetieReprises = &DATTerrain.m_TBLHomotheties.GetKey();
							Parution.m_pDATHomothetieReprises = &DATTerrain.m_TBLHomotheties.GetItem();

							// on teste l'indicateur des univers
							if (FgUnvReprises)
							{
								// on teste si on a déjà trouvé des poids
								if (Requete.m_pKEYPoidsReprises == 0)
								{
									// on mémorise les poids
									Requete.m_pKEYPoidsReprises = Parution.m_pKEYPoidsReprises;
									Requete.m_pDATPoidsReprises = Parution.m_pDATPoidsReprises;

									// on mémorise l'homothétie
									Requete.m_pKEYHomothetieReprises = Parution.m_pKEYHomothetieReprises;
									Requete.m_pDATHomothetieReprises = Parution.m_pDATHomothetieReprises;
								}
								else if (Requete.m_pKEYPoidsReprises != Parution.m_pKEYPoidsReprises)
								{
									// on annule les poids
									Requete.m_pKEYPoidsReprises = 0;
									Requete.m_pDATPoidsReprises = 0;

									// on annule l'homothétie
									Requete.m_pKEYHomothetieReprises = 0;
									Requete.m_pDATHomothetieReprises = 0;

									// on annule l'indicateur
									FgUnvReprises = false;
								}
							}

							// on met à jour l'item de l'univers
							ItemUnvReprises &= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;

							// on met à jour l'item de la parution
							ItemParution |= DATTerrain.m_TBLUnivers.GetItem().m_ItemIndividus;
						}
					}

					// on teste l'indicateur des échantillons
					if (FgEchantillon)
					{
						// on teste si on a déjà trouvé un échantillon
						if (Requete.m_pKEYEchantillon == 0)
						{
							// on mémorise l'échantillon
							Requete.m_pKEYEchantillon = &DATTerrain.m_TBLEchantillons.GetKey();
							Requete.m_pDATEchantillon = &DATTerrain.m_TBLEchantillons.GetItem();
						}
						else if (Requete.m_pKEYEchantillon != &DATTerrain.m_TBLEchantillons.GetKey())
						{
							// on annule l'échantillon
							Requete.m_pKEYEchantillon = 0;
							Requete.m_pDATEchantillon = 0;

							// on annule l'indicateur
							FgEchantillon = false;
						}
					}

					// on alloue l'item des audiences et des effectifs
					Parution.m_ItemAudience.SetCount(NbIndividus);
					Parution.m_ItemEffectif.SetCount(NbIndividus);

					// on initialise l'item des effectifs
					Parution.m_ItemEffectif |= ItemParution;
					Parution.m_ItemEffectif &= DATTerrain.m_TBLEchantillons.GetItem().m_ItemIndividus;
					Parution.m_ItemEffectif &= ItemSegment;

					// on initialise l'item des audiences
					Parution.m_ItemAudience |= Parution.m_ItemEffectif;
					Parution.m_ItemAudience &= DATFiltre.m_ItemIndividus;

					// on met à jour l'item des filtres
					ItemFiltre |= DATFiltre.m_ItemIndividus;
				}
			}
		}

		// on réinitialise l'item des parutions
		ItemParution &= false;
	}

	// on teste si on a trouvé un échantillon
	if (!FgEchantillon) { JInt32 LOG_A_FAIRE; return (false); }

	// on recherche les univers compatibles avec toutes les variables d'agrégation
	if (!FgUnvVariablesRef)    this->OnFindUnivers(DATTerrain, ItemUnvVariablesRef,    Requete.m_pKEYEchantillon->m_IdEchantillon, DATModulation.m_FLTPoids.Item(IdxPoids), Requete.m_pKEYPoidsVariablesRef,    Requete.m_pDATPoidsVariablesRef,    Requete.m_pKEYHomothetieVariablesRef,    Requete.m_pDATHomothetieVariablesRef);
	if (!FgUnvProbabilitesRef) this->OnFindUnivers(DATTerrain, ItemUnvProbabilitesRef, Requete.m_pKEYEchantillon->m_IdEchantillon, DATModulation.m_FLTPoids.Item(IdxPoids), Requete.m_pKEYPoidsProbabilitesRef, Requete.m_pDATPoidsProbabilitesRef, Requete.m_pKEYHomothetieProbabilitesRef, Requete.m_pDATHomothetieProbabilitesRef);
	if (!FgUnvVariablesDDLRef) this->OnFindUnivers(DATTerrain, ItemUnvVariablesDDLRef, Requete.m_pKEYEchantillon->m_IdEchantillon, DATModulation.m_FLTPoids.Item(IdxPoids), Requete.m_pKEYPoidsVariablesDDLRef, Requete.m_pDATPoidsVariablesDDLRef, Requete.m_pKEYHomothetieVariablesDDLRef, Requete.m_pDATHomothetieVariablesDDLRef);
	if (!FgUnvReprises)        this->OnFindUnivers(DATTerrain, ItemUnvReprises,        Requete.m_pKEYEchantillon->m_IdEchantillon, DATModulation.m_FLTPoids.Item(IdxPoids), Requete.m_pKEYPoidsReprises,        Requete.m_pDATPoidsReprises,        Requete.m_pKEYHomothetieReprises,        Requete.m_pDATHomothetieReprises);

	// on alloue l'item de l'audience
	Requete.m_ItemAudience.SetCount(NbIndividus);

	// on initialise l'item de l'audience
	Requete.m_ItemAudience |= Requete.m_ItemEffectif;
	Requete.m_ItemAudience &= ItemFiltre;

	// on quitte
	return (true);
}

///////////////////////////////////////
// la fonction pour tester les éléments

JBool JMTFR03RequeteA::IsValid() const
{
	// on teste les éléments
	return (m_TBLItems.GetCount() > 0);
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03RequeteA::SetItems(const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JRef32 & IdSource, const JRef32 & IdTerrain, const JMTFR03Options & Options, const JMTFR03Maille & Maille)
{
	// on vérifie la validité des identifiants et des options
	if (!IdSource.IsValid() || !IdTerrain.IsValid() || !Options.IsValid()) throw JInvalidCall::GetInstance();

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
	if (!this->OnMakeRequete(DATTerrain, DATModulation, Options, Maille, TBLItems.Item(0))) throw JMTFR03Exception::GetInstance();

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JMTFR03RequeteA::JRequete & JMTFR03RequeteA::GetItems() const
{
	// on renvoie les éléments
	return (m_TBLItems.Item(0));
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03RequeteA::Swap(JMTFR03RequeteA & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03RequeteA::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03RequeteA::~JMTFR03RequeteA()
{
	// on ne fait rien
}
