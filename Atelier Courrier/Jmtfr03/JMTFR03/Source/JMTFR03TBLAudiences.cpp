//
// Fichier: JMTFR03TBLAudiences.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLAudiences.h"

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

// on inclut les définitions nécessaires
#include "JMTResultats.h"

////////////////////
// les constructeurs

JMTFR03TBLAudiences::JMTFR03TBLAudiences()
{
	// on ne fait rien
}

JMTFR03TBLAudiences::JMTFR03TBLAudiences(const JMTFR03TBLAudiences & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLAudiences & JMTFR03TBLAudiences::operator =(const JMTFR03TBLAudiences & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLAudiences::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLAudiences.GetCount());
}

//////////////////////////////////////////
// la fonction pour calculer les résultats

JBool JMTFR03TBLAudiences::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03DATModulation & DATModulation, const JMTFR03RequeteA & RequeteA, const JMTFR03Options & Options, JResultats & Resultats, JBool IsCouplage) const
{
	// on initialise les résultats
	Resultats.m_AudienceRef   = 0.0;
	Resultats.m_AudienceProba = 0.0;
	Resultats.m_VolLecture1Jr = 0.0;
	Resultats.m_PtEtalonnage  = 0;

	// on récupère la requête
	const JMTFR03RequeteA::JRequete & Requete = RequeteA.GetItems();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = Requete.m_ItemEffectif.GetCount();

	// on calcule l'effectif de référence
	JMTResultats EffectifRef; EffectifRef.SetItem(Requete.m_ItemEffectif, *Requete.m_pKEYPoids, *Requete.m_pDATPoids, *Requete.m_pTBLEchantillons, *Requete.m_pTBLHomotheties, DATCible, TblPonderations);

	// on teste si on a un couplage
	if (IsCouplage)
	{
		// on crée les tables des audiences et des volumes de contacts
		JMTTBLAudiences TBLAudiencesRef;   TBLAudiencesRef  .SetCount(NbIndividus);
		JMTTBLAudiences TBLAudiencesProba; TBLAudiencesProba.SetCount(NbIndividus);

		// on boucle sur toutes les parutions
		for (Requete.m_TBLParutions.MoveFirst(); Requete.m_TBLParutions.IsValid(); Requete.m_TBLParutions.MoveNext())
		{
			// on récupère la parution de la requête
			const JMTFR03RequeteA::JParution & Parution = Requete.m_TBLParutions.GetItem();

			// on vérifie la validité de l'audience de référence
			if (Parution.m_pDATVariablesRef != 0)
			{
				// on incrémente l'audience de référence
				TBLAudiencesRef.AddItems(*Parution.m_pDATVariablesRef, 1);
			}

			// on vérifie la validité de l'audience probabiliste
			if (Parution.m_pDATProbabilitesRef != 0 && Parution.m_pDATPoidsProbabilitesRef != 0)
			{
				// on incrémente l'audience probabiliste
				TBLAudiencesProba.AddItems(*Parution.m_pDATProbabilitesRef, 1);
			}
		}

		// on teste la validité des poids de l'audience de référence
		if (Requete.m_pDATPoidsVariablesRef != 0)
		{
			// on calcule l'effectif
			JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pDATPoidsVariablesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesRef, DATCible, TblPonderations);

			// on calcule l'audience de référence
			JMTResultats AudienceRef; AudienceRef.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsVariablesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieVariablesRef, DATCible, TblPonderations, TBLAudiencesRef);

			// on mémorise l'audience de référence
			if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0) Resultats.m_AudienceRef = (AudienceRef.AsJFlt64() * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
		}

		// on teste la validité des poids de l'audience probabiliste
		if (Requete.m_pDATPoidsProbabilitesRef != 0)
		{
			// on calcule l'effectif
			JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations);

			// on calcule l'audience probabiliste
			JMTResultats AudienceProba; AudienceProba.SetItem(Requete.m_ItemAudience, *Requete.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Requete.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLAudiencesProba);

			// on mémorise l'audience de référence
			if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0) Resultats.m_AudienceProba = (AudienceProba.AsJFlt64() * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
		}
	}
	else
	{
		// on se place sur la parution de la requête
		Requete.m_TBLParutions.MoveFirst();

		// on teste si on a trouvé une parution
		if (Requete.m_TBLParutions.IsValid())
		{
			// on récupère la parution de la requête
			const JMTFR03RequeteA::JParution & Parution = Requete.m_TBLParutions.GetItem();

			// on vérifie la validité de l'audience de référence
			if (Parution.m_pDATVariablesRef != 0 && Parution.m_pDATPoidsVariablesRef != 0)
			{
				// on calcule l'effectif
				JMTResultats Effectif; Effectif.SetItem(Parution.m_ItemEffectif, *Parution.m_pDATPoidsVariablesRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesRef, DATCible, TblPonderations);

				// on crée et on initialise la table des audiences de référence
				JMTTBLAudiences TBLAudiencesRef; TBLAudiencesRef.SetCount(NbIndividus); TBLAudiencesRef.AddItems(*Parution.m_pDATVariablesRef, 1);

				// on calcule l'audience de référence
				JMTResultats AudienceRef; AudienceRef.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsVariablesRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesRef, DATCible, TblPonderations, TBLAudiencesRef);

				// on mémorise l'audience de référence
				if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0) Resultats.m_AudienceRef = (AudienceRef.AsJFlt64() * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
			}

			// on vérifie la validité de l'audience probabiliste
			if (Parution.m_pDATProbabilitesRef != 0 && Parution.m_pDATPoidsProbabilitesRef != 0)
			{
				// on calcule l'effectif
				JMTResultats Effectif; Effectif.SetItem(Parution.m_ItemEffectif, *Parution.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations);

				// on crée et on initialise la table des audiences probabilistes
				JMTTBLAudiences TBLAudiencesProba; TBLAudiencesProba.SetCount(NbIndividus); TBLAudiencesProba.AddItems(*Parution.m_pDATProbabilitesRef, 1);

				// on calcule l'audience probabiliste
				JMTResultats AudienceProba; AudienceProba.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsProbabilitesRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieProbabilitesRef, DATCible, TblPonderations, TBLAudiencesProba);

				// on mémorise l'audience probabiliste
				if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0) Resultats.m_AudienceProba = (AudienceProba.AsJFlt64() * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
			}

			// on teste si on doit prendre en compte les reprises en mains
			if (Options.GetReprises() && Parution.m_pDATReprises != 0 && Parution.m_pKEYPoidsReprises != 0)
			{
				// on calcule l'effectif
				JMTResultats Effectif; Effectif.SetItem(Parution.m_ItemEffectif, *Parution.m_pDATPoidsReprises, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieReprises, DATCible, TblPonderations);

				// on crée et on initialise la table des volumes de contacts avec contextes
				JMTTBLVolContacts TBLVolContacts; TBLVolContacts.SetCount(NbIndividus); TBLVolContacts.AddItems(*Parution.m_pDATReprises, 1);

				// on calcule le volume de contacts avec contextes
				JMTResultats VolContacts; VolContacts.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsReprises, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieReprises, DATCible, TblPonderations, TBLVolContacts);

				// on mémorise les volumes de contacts
				if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0) Resultats.m_VolLecture1Jr = (VolContacts.AsJFlt64() * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
			}
			else if (Parution.m_PtEtalonnageVol > 0 && Parution.m_pDATVariablesDDL_Vol != 0 && Parution.m_pKEYPoidsVariablesDDLRef)
			{
				// on calcule l'effectif
				JMTResultats Effectif; Effectif.SetItem(Parution.m_ItemEffectif, *Parution.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations);

				// on crée et on initialise la table des volumes de contacts
				JMTTBLVolContacts TBLVolContacts; TBLVolContacts.SetCount(NbIndividus); TBLVolContacts.AddItems(*Parution.m_pDATVariablesDDL_Vol, Parution.m_PtEtalonnageVol);

				// on calcule les volumes de contacts
				JMTResultats VolContacts; VolContacts.SetItem(Parution.m_ItemAudience, *Parution.m_pDATPoidsVariablesDDLRef, *Requete.m_pDATEchantillon, *Parution.m_pDATHomothetieVariablesDDLRef, DATCible, TblPonderations, TBLVolContacts);

				// on mémorise les volumes de contacts
				if (Effectif.AsJFlt64() > 0.0 && EffectifRef.AsJFlt64() > 0.0) Resultats.m_VolLecture1Jr = (VolContacts.AsJFlt64() * EffectifRef.AsJFlt64()) / Effectif.AsJFlt64();
			}

			// on récupère le point d'étalonnage du support
			Resultats.m_PtEtalonnage = Parution.m_PtEtalonnageRef;
		}
	}

	// on quitte
	return (true);
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

const JMTFR03DATAudience & JMTFR03TBLAudiences::Add(const JMTFR03KEYAudience & KEYAudience, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, JMTFR03Logs* pLogs, JBool Move)
{
	// on vérifie la validité de la clé et des options
	if (!KEYAudience.IsValid() || !Options.IsValid()) throw JInvalidCall::GetInstance();

	// on crée la clé de la cible
	JMTFR03KEYCible KEYCible;

	// on initialise la clé de la cible
	KEYCible.m_IdSource  = KEYAudience.m_IdSource;
	KEYCible.m_IdTerrain = KEYAudience.m_IdTerrain;
	KEYCible.m_IdCible   = KEYAudience.m_IdCible;

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on récupère les pondérations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain); 

	// on crée la clé des modulations
	JMTFR03KEYModulation KEYModulation;

	// on initialise la clé des modulations
	KEYModulation.m_IdSource  = KEYAudience.m_IdSource;
	KEYModulation.m_IdTerrain = KEYAudience.m_IdTerrain;

	// on recherche les modulations
	TBLModulations.MoveTo(KEYModulation); if (!TBLModulations.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les modulations
	const JMTFR03DATModulation & DATModulation = TBLModulations.GetItem();

	// on crée une maille avec le titre
	JMTFR03Maille Maille; Maille.Add(KEYAudience.m_IdTitre, (JRef32)1, KEYAudience.m_IdTitre, 1, 0.0);

	// on crée et on construit la requête
	JMTFR03RequeteA RequeteA; RequeteA.SetItems(TBLTerrains, TBLModulations, KEYAudience.m_IdSource, KEYAudience.m_IdTerrain, Options, Maille);

	// on crée et on calcule les résultats
	JResultats Resultats; if (!this->OnComputeResultats(DATCible, TblPonderations, DATModulation, RequeteA, Options, Resultats, false)) throw JMTFR03Exception::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATAudience & DATAudience = m_TBLAudiences.Add(KEYAudience, Move);

	// on initialise l'élément
	DATAudience.m_AudienceRef   = Resultats.m_AudienceRef;
	DATAudience.m_AudienceProba = Resultats.m_AudienceProba;
	DATAudience.m_VolLecture1Jr = Resultats.m_VolLecture1Jr;
	DATAudience.m_PtEtalonnage  = Resultats.m_PtEtalonnage;

	// on renvoie l'élément
	return (DATAudience);
}

const JMTFR03DATAudience & JMTFR03TBLAudiences::Add(const JMTFR03KEYAudience & KEYAudience, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, const JMTFR03Options & Options, const JMTFR03Couplage & Couplage, JMTFR03Logs* pLogs, JBool Move)
{
	// on vérifie la validité de la clé et des options
	if (!KEYAudience.IsValid() || !Options.IsValid()) throw JInvalidCall::GetInstance();

	// on crée la clé de la cible
	JMTFR03KEYCible KEYCible;

	// on initialise la clé de la cible
	KEYCible.m_IdSource  = KEYAudience.m_IdSource;
	KEYCible.m_IdTerrain = KEYAudience.m_IdTerrain;
	KEYCible.m_IdCible   = KEYAudience.m_IdCible;

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on récupère les pondérations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain); 

	// on crée la clé des modulations
	JMTFR03KEYModulation KEYModulation;

	// on initialise la clé des modulations
	KEYModulation.m_IdSource  = KEYAudience.m_IdSource;
	KEYModulation.m_IdTerrain = KEYAudience.m_IdTerrain;

	// on recherche les modulations
	TBLModulations.MoveTo(KEYModulation); if (!TBLModulations.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les modulations
	const JMTFR03DATModulation & DATModulation = TBLModulations.GetItem();

	// on crée une maille avec le titre
	JMTFR03Maille Maille; for (Couplage.MoveFirst(); Couplage.IsValid(); Couplage.MoveNext()) Maille.Add(Couplage.GetIdTitre(), (JRef32)1, Couplage.GetIdTitre(), 1, 0.0);

	// on crée et on construit la requête
	JMTFR03RequeteA RequeteA; RequeteA.SetItems(TBLTerrains, TBLModulations, KEYAudience.m_IdSource, KEYAudience.m_IdTerrain, Options, Maille);

	// on crée et on calcule les résultats
	JResultats Resultats; if (!this->OnComputeResultats(DATCible, TblPonderations, DATModulation, RequeteA, Options, Resultats, true)) throw JMTFR03Exception::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATAudience & DATAudience = m_TBLAudiences.Add(KEYAudience, Move);

	// on initialise l'élément
	DATAudience.m_AudienceRef   = Resultats.m_AudienceRef;
	DATAudience.m_AudienceProba = Resultats.m_AudienceProba;
	DATAudience.m_VolLecture1Jr = Resultats.m_VolLecture1Jr;
	DATAudience.m_PtEtalonnage  = Resultats.m_PtEtalonnage;

	// on renvoie l'élément
	return (DATAudience);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLAudiences::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLAudiences.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYAudience & JMTFR03TBLAudiences::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLAudiences.GetKey());
}

const JMTFR03DATAudience & JMTFR03TBLAudiences::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLAudiences.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLAudiences::MoveTo(const JMTFR03KEYAudience & KEYAudience, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYAudience.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLAudiences.MoveTo(KEYAudience, Move);
}

JVoid JMTFR03TBLAudiences::MoveFirst() const
{
	// on déplace le curseur
	m_TBLAudiences.MoveFirst();
}

JVoid JMTFR03TBLAudiences::MoveNext() const
{
	// on déplace le curseur
	m_TBLAudiences.MoveNext();
}

JVoid JMTFR03TBLAudiences::MovePrev() const
{
	// on déplace le curseur
	m_TBLAudiences.MovePrev();
}

JVoid JMTFR03TBLAudiences::MoveLast() const
{
	// on déplace le curseur
	m_TBLAudiences.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'élément courant

JVoid JMTFR03TBLAudiences::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLAudiences.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLAudiences::Swap(JMTFR03TBLAudiences & Source)
{
	// on permute les éléments
	m_TBLAudiences.Swap(Source.m_TBLAudiences);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLAudiences::Reset()
{
	// on réinitialise les éléments
	m_TBLAudiences.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLAudiences::~JMTFR03TBLAudiences()
{
	// on ne fait rien
}
