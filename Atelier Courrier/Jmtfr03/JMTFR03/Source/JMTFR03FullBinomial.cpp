//
// Fichier: JMTFR03FullBinomial.cpp
// Auteur:  Sylvain SAMMURI
// Date:    08/01/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03FullBinomial.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Exception.h"

// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03FullBinomial & JMTFR03FullBinomial::operator =(const JMTFR03FullBinomial & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////
// la fonction pour calculer les r�sultats

JBool JMTFR03FullBinomial::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JBool fNoDistrib, JResultats & Resultats) const
{
	// on r�cup�re la requ�te
	const JMTFR03RequeteA::JRequete & Requete = RequeteA.GetItems();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = Requete.m_ItemEffectif.GetCount();

	// on calcule l'effectif de r�f�rence
	JMTResultats EffectifRef; EffectifRef.SetItem(Requete.m_ItemEffectif, *Requete.m_pKEYPoids, *Requete.m_pDATPoids, *Requete.m_pTBLEchantillons, *Requete.m_pTBLHomotheties, DATCible, TblPonderations);

	// on teste si on a trouv� des poids communs
	if (Requete.m_TBLParutions.GetCount() > 0 && Requete.m_pDATPoidsProbabilitesRef == 0) { JInt32 LOG_A_FAIRE; }

	// on teste si on a des parutions et des poids communs aux parutions
	if (Requete.m_TBLParutions.GetCount() > 0 && Requete.m_pDATPoidsProbabilitesRef != 0)
	{
		// on calcule l'effectif
		JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations);

		// on d�termine le nombre de classes
		JInt32 KMax = g_KMax; if (fNoDistrib) KMax = 1;

		// on cr�e et on initialise les distributions
		JMTTBLDistributions TBLDistributions; TBLDistributions.SetCount(NbIndividus, KMax);

		// on boucle sur toutes les parutions de la maille
		for (Requete.m_TBLParutions.MoveFirst(); Requete.m_TBLParutions.IsValid(); Requete.m_TBLParutions.MoveNext())
		{
			// on r�cup�re la parution de la requ�te
			const JMTFR03RequeteA::JParution & Parution = Requete.m_TBLParutions.GetItem();

			// on v�rifie la validit� des probabilit�s du support
			if (Parution.m_pDATProbabilitesRef != 0)
			{
				// on calcule le GRP de r�f�rence
				JFlt64 GRPRef = Parution.m_GRPRef * (JFlt64)Parution.m_NbInsertions;

				// on cr�e et on initialise le volume de contacts
				JMTTBLVolContacts TBLVolContacts; TBLVolContacts.SetCount(NbIndividus);

				// on cumule les probabilit�s du supports
				TBLVolContacts.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_NbInsertions);

				// on cr�e et on calcule le volume de contacts
				JMTResultats VolContacts; VolContacts.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLVolContacts);

				// on calcule le coefficient du trusquin
				JFlt64 CoefTrusquin = 0.0; if (VolContacts.AsJFlt64() > 0.0 && Effectif.AsJFlt64() > 0.0) CoefTrusquin = (GRPRef * Effectif.AsJFlt64()) / (VolContacts.AsJFlt64() * 100.0);

				// on cumule les probabilit�s du support
				TBLDistributions.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_NbInsertions, CoefTrusquin);
			}
			else
			{
				JInt32 LOG_A_FAIRE;
			}
		}

		// on cr�e et on calcule les r�sultats
		JMTTBLResultats TBLResultats; TBLResultats.SetItems(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLDistributions);

		// on alloue les r�sultats
		Resultats.m_TBLClasses.SetCount(g_KMax);

		// on v�rifie la validit� des effectifs
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
		// on alloue les r�sultats
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
// la fonction pour d�finir les �l�ments

JVoid JMTFR03FullBinomial::SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, JBool fNoDistrib, JMTFR03Logs* pLogs)
{
	// on v�rifie la validit� de la cl� et des options
	if (!KEYCible.IsValid() || !Options.IsValid()) throw JInvalidCall::GetInstance();

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on cr�e la cl� des modulations
	JMTFR03KEYModulation KEYModulation;

	// on initialise la cl� des modulations
	KEYModulation.m_IdSource  = KEYCible.m_IdSource;
	KEYModulation.m_IdTerrain = KEYCible.m_IdTerrain;

	// on r�cup�re les pond�rations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain);

	// on recherche les modulations
	TBLModulations.MoveTo(KEYModulation); if (!TBLModulations.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re les modulations
	const JMTFR03DATModulation & DATModulation = TBLModulations.GetItem();

	// on cr�e et on construit la requ�te
	JMTFR03RequeteA RequeteA; RequeteA.SetItems(TBLTerrains, TBLModulations, KEYCible.m_IdSource, KEYCible.m_IdTerrain, Options, Maille);

	// on cr�e les nouveaux �l�ments
	JArray<JResultats> TBLItems; TBLItems.SetCount(1);

	// on calcule les r�sultats
	if (!this->OnComputeResultats(DATCible, TblPonderations, DATModulation, RequeteA, Options, fNoDistrib, TBLItems.Item(0))) throw JMTFR03Exception::GetInstance();

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
}

////////////////////////////////////////////
// la fonction pour calculer la distribution

JVoid JMTFR03FullBinomial::MakeDistribution(JDistribution & Distribution) const
{
	// on v�rifie l'existence des r�sultats
	if (m_TBLItems.GetCount() > 0)
	{
		// on r�cup�re les r�sultats
		const JResultats & Resultats = m_TBLItems.Item(0);

		// on r�initialise la distribution
		for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = Resultats.m_TBLClasses.Item(Idx);
	}
	else
	{
		// on r�initialise la distribution
		for (JInt32 Idx = 0; Idx < g_KMax; Idx += 1) Distribution.m_Items[Idx] = 0.0;
	}
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03FullBinomial::Swap(JMTFR03FullBinomial & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03FullBinomial::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FullBinomial::~JMTFR03FullBinomial()
{
	// on ne fait rien
}
