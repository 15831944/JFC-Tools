//
// Fichier: JMTFR03FullBinomial.cpp
// Auteur:  Sylvain SAMMURI
// Date:    08/01/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03FullBinomial.h"

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

// on inclut les définitions nécessaires
#include "JMTTBLDistributions.h"
#include "JMTTBLResultats.h"
#include "JMTResultats.h"

// on initialise la classe maximum des effectifs
static const JInt32 g_KMax = JDISTRIBUTION_MAX;

////////////////////
// les constructeurs

JMTFR03FullBinomial::JMTFR03FullBinomial()
{
	// on ne fait rien
}

JMTFR03FullBinomial::JMTFR03FullBinomial(const JMTFR03FullBinomial & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03FullBinomial & JMTFR03FullBinomial::operator =(const JMTFR03FullBinomial & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////
// la fonction pour calculer les résultats

JBool JMTFR03FullBinomial::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fNoDistrib, JResultats & Resultats) const
{
	// on récupère la requête
	const JMTFR03RequeteA::JRequete & Requete = RequeteA.GetItems();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = Requete.m_ItemEffectif.GetCount();

	// on calcule l'effectif de référence
	JMTResultats EffectifRef; EffectifRef.SetItem(Requete.m_ItemEffectif, *Requete.m_pKEYPoids, *Requete.m_pDATPoids, *Requete.m_pTBLEchantillons, *Requete.m_pTBLHomotheties, DATCible, TblPonderations);

	// on teste si on a trouvé des poids communs
	if (Requete.m_TBLParutions.GetCount() > 0 && Requete.m_pDATPoidsProbabilitesRef == 0) { JInt32 LOG_A_FAIRE; }

	// on teste si on a des parutions et des poids communs aux parutions
	if (Requete.m_TBLParutions.GetCount() > 0 && Requete.m_pDATPoidsProbabilitesRef != 0)
	{
		// on calcule l'effectif
		JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations);

		// on détermine le nombre de classes
		JInt32 KMax = g_KMax; if (fNoDistrib) KMax = 1;

		// on crée et on initialise les distributions
		JMTTBLDistributions TBLDistributions; TBLDistributions.SetCount(NbIndividus, KMax);

		// on boucle sur toutes les parutions de la maille
		for (Requete.m_TBLParutions.MoveFirst(); Requete.m_TBLParutions.IsValid(); Requete.m_TBLParutions.MoveNext())
		{
			// on récupère la parution de la requête
			const JMTFR03RequeteA::JParution & Parution = Requete.m_TBLParutions.GetItem();

			// on vérifie la validité des probabilités du support
			if (Parution.m_pDATProbabilitesRef != 0)
			{
				// on calcule le GRP de référence
				JFlt64 GRPRef = Parution.m_GRPRef * (JFlt64)Parution.m_NbInsertions;

				// on crée et on initialise le volume de contacts
				JMTTBLVolContacts TBLVolContacts; TBLVolContacts.SetCount(NbIndividus);

				// on cumule les probabilités du supports
				TBLVolContacts.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_NbInsertions);

				// on crée et on calcule le volume de contacts
				JMTResultats VolContacts; VolContacts.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLVolContacts);

				// on calcule le coefficient du trusquin
				JFlt64 CoefTrusquin = 0.0; if (VolContacts.AsJFlt64() > 0.0 && Effectif.AsJFlt64() > 0.0) CoefTrusquin = (GRPRef * Effectif.AsJFlt64()) / (VolContacts.AsJFlt64() * 100.0);

				// on cumule les probabilités du support
				TBLDistributions.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_NbInsertions, CoefTrusquin);
			}
			else
			{
				JInt32 LOG_A_FAIRE;
			}
		}

		// on crée et on calcule les résultats
		JMTTBLResultats TBLResultats; TBLResultats.SetItems(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLDistributions);

		// on alloue les résultats
		Resultats.m_TBLClasses.SetCount(g_KMax);

		// on vérifie la validité des effectifs
		if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0)
		{
			// on initialise toutes les classes
			for (JInt32 IdxClasse = 0;    IdxClasse <   KMax; IdxClasse += 1) Resultats.m_TBLClasses.Item(IdxClasse) = (TBLResultats.Item(IdxClasse + 1) * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
			for (JInt32 JdxClasse = KMax; JdxClasse < g_KMax; JdxClasse += 1) Resultats.m_TBLClasses.Item(JdxClasse) = 0.0;

			// on initialise l'effectif
			Resultats.m_Effectif = EffectifRef.AsJFlt64();
		}
		else
		{
			// on initialise toutes les classes
			for (JInt32 IdxClasse = 0; IdxClasse < g_KMax; IdxClasse += 1) Resultats.m_TBLClasses.Item(IdxClasse) = 0.0;

			// on initialise l'effectif
			Resultats.m_Effectif = EffectifRef.AsJFlt64();
		}
	}
	else
	{
		// on alloue les résultats
		Resultats.m_TBLClasses.SetCount(g_KMax);

		// on initialise toutes les classes
		for (JInt32 IdxClasse = 0; IdxClasse < g_KMax; IdxClasse += 1) Resultats.m_TBLClasses.Item(IdxClasse) = 0.0;

		// on initialise l'effectif
		Resultats.m_Effectif = EffectifRef.AsJFlt64();
	}

	// on quitte
	return (true);
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03FullBinomial::SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fNoDistrib, JMTFR03Logs* pLogs)
{
	// on vérifie la validité de la clé et des options
	if (!KEYCible.IsValid() || !Options.IsValid()) throw JInvalidCall::GetInstance();

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on crée la clé des modulations
	JMTFR03KEYModulation KEYModulation;

	// on initialise la clé des modulations
	KEYModulation.m_IdSource  = KEYCible.m_IdSource;
	KEYModulation.m_IdTerrain = KEYCible.m_IdTerrain;

	// on récupère les pondérations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain);

	// on recherche les modulations
	TBLModulations.MoveTo(KEYModulation); if (!TBLModulations.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les modulations
	const JMTFR03DATModulation & DATModulation = TBLModulations.GetItem();

	// on crée et on construit la requête
	JMTFR03RequeteA RequeteA; RequeteA.SetItems(TBLTerrains, TBLModulations, KEYCible.m_IdSource, KEYCible.m_IdTerrain, Options, Maille);

	// on crée les nouveaux éléments
	JArray<JResultats> TBLItems; TBLItems.SetCount(1);

	// on calcule les résultats
	if (!this->OnComputeResultats(DATCible, TblPonderations, DATModulation, RequeteA, Options, fNoDistrib, TBLItems.Item(0))) throw JMTFR03Exception::GetInstance();

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

////////////////////////////////////////////
// la fonction pour calculer la distribution

JVoid JMTFR03FullBinomial::MakeDistribution(JDistribution & Distribution) const
{
	// on vérifie l'existence des résultats
	if (m_TBLItems.GetCount() > 0)
	{
		// on récupère les résultats
		const JResultats & Resultats = m_TBLItems.Item(0);

		// on réinitialise la distribution
		for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = Resultats.m_TBLClasses.Item(Idx);
	}
	else
	{
		// on réinitialise la distribution
		for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = 0.0;
	}
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03FullBinomial::Swap(JMTFR03FullBinomial & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03FullBinomial::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FullBinomial::~JMTFR03FullBinomial()
{
	// on ne fait rien
}
