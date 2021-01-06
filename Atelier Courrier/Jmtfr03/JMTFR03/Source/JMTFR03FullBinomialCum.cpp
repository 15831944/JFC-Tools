//
// OPTIMISATION DE LA CLASSE JMTFR03FullBinomial
// Date:    14/06/2005
//

// on inclut les définitions nécessaires
#include "JMTFR03FullBinomialCum.h"

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

// on inclut les définitions nécessaires
#include "JMTTBLDistributions.h"
#include "JMTTBLResultats.h"
#include "JMTResultats.h"

// on initialise la classe maximum des effectifs à 1 (car on calcul uniquement la distri à 1)
static const JInt32 g_KMax = 1;   //JDISTRIBUTION_MAX;

////////////////////
// les constructeurs

JMTFR03FullBinomialCum::JMTFR03FullBinomialCum()
{
	// on ne fait rien
}

JMTFR03FullBinomialCum::JMTFR03FullBinomialCum(const JMTFR03FullBinomialCum & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03FullBinomialCum & JMTFR03FullBinomialCum::operator =(const JMTFR03FullBinomialCum & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////
// la fonction pour calculer les résultats
JBool JMTFR03FullBinomialCum::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, const JUnt32 & NbInsertions, JArray<JResultats> &TBLItems) const
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

		// on crée et on initialise les distributions pour 1 contact seulement (ici g_KMax = 1)
		JMTTBLDistributions TBLDistribution; TBLDistribution.SetCount(NbIndividus, g_KMax);
		
		// Boucle sur le nombre d'insertions maximum			
		for (JUnt32 InxInsert = 0; InxInsert < NbInsertions; InxInsert++)
		{
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
					TBLDistribution.AddItems(*Parution.m_pDATProbabilitesRef, Parution.m_NbInsertions, CoefTrusquin);

				}
				else
				{
					JInt32 LOG_A_FAIRE;
				}
			}


			// RESULTAT INTERMEDIAIRE niveau NbInsert
			// 
			// on crée et on calcule les résultats intermédiaires
			JMTTBLResultats TBLResultats; TBLResultats.SetItems(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLDistribution);

			// on récupère élmt résultat et alloue les résultats
			JResultats &Resultats = TBLItems.Item(InxInsert);
			Resultats.m_TBLClasses.SetCount(g_KMax);

			// on vérifie la validité des effectifs
			if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0)
			{
				// on initialise toutes les classes
				for (JInt32 IdxClasse = 0; IdxClasse < g_KMax; IdxClasse += 1) Resultats.m_TBLClasses.Item(IdxClasse) = (TBLResultats.Item(IdxClasse + 1) * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();

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

		} // Fin boucle nombre d'insertions maximum			
	}
	else
	{
		//Boucle sur le nombre d'insertions maximum			
		for (JUnt32 InxInsert = 0; InxInsert < NbInsertions; InxInsert++)
		{
			// on récupère élmt résultat et alloue les résultats
			JResultats &Resultats = TBLItems.Item(InxInsert);

			// on alloue les résultats
			Resultats.m_TBLClasses.SetCount(g_KMax);

			// on initialise toutes les classes
			for (JInt32 IdxClasse = 0; IdxClasse < g_KMax; IdxClasse += 1) Resultats.m_TBLClasses.Item(IdxClasse) = 0.0;

			// on initialise l'effectif
			Resultats.m_Effectif = EffectifRef.AsJFlt64();
		}
	}

	// on quitte
	return (true);
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03FullBinomialCum::SetItems(const JMTFR03KEYCible & KEYCible, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Maille & Maille, const JUnt32 & NbInsertions, JMTFR03Logs* pLogs)
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
	JArray<JResultats> TBLItems; TBLItems.SetCount(NbInsertions);
	
	// on calcule les résultats
	// if (!this->OnComputeResultats(DATCible, DATModulation, RequeteA, Options, NbInsertions, TBLItems.Item(0))) throw JMTFR03Exception::GetInstance();
	if (!this->OnComputeResultats(DATCible, TblPonderations, DATModulation, RequeteA, Options, NbInsertions, TBLItems)) throw JMTFR03Exception::GetInstance();

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

/////////////////////////////////////////////////////////////////
// la fonction pour calculer la distribution niveau n insertions
JVoid JMTFR03FullBinomialCum::MakeDistribution(JDistribution & Distribution, JUnt32 & NbInsert) const
{
	// on vérifie l'existence des résultats
	if ((JUnt32)m_TBLItems.GetCount() >= NbInsert)
	{
		// on récupère les résultats
		// const JResultats & Resultats = m_TBLItems.Item(0);
		const JResultats & Resultats = m_TBLItems.Item(NbInsert-1);

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

JVoid JMTFR03FullBinomialCum::Swap(JMTFR03FullBinomialCum & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03FullBinomialCum::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FullBinomialCum::~JMTFR03FullBinomialCum()
{
	// on ne fait rien
}
