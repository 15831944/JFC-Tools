//
// Fichier: JMTFR03TBLEffectifs.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLEffectifs.h"

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

// on inclut les définitions nécessaires
#include "JMTResultats.h"
// #include "windows.h"
// #include "stdio.h"

////////////////////
// les constructeurs

JMTFR03TBLEffectifs::JMTFR03TBLEffectifs()
{
	// on ne fait rien
}

JMTFR03TBLEffectifs::JMTFR03TBLEffectifs(const JMTFR03TBLEffectifs & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLEffectifs & JMTFR03TBLEffectifs::operator =(const JMTFR03TBLEffectifs & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLEffectifs::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLEffectifs.GetCount());
}

//////////////////////////////////////////
// la fonction pour calculer les résultats

JBool JMTFR03TBLEffectifs::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03RequeteE & RequeteE, JResultats & Resultats) const
{
	// on initialise les résultats
	Resultats.m_NbCas    = 0;
	Resultats.m_Effectif = 0.0;

	// A VIRER
	/*
	char tab[512];
	sprintf(tab, "%s %0.2f - %0.2f - %0.2f", "Ponder 0 1 2", TblPonderations.GetPonderations(0), TblPonderations.GetPonderations(1), TblPonderations.GetPonderations(2));
	// InfoPonder.Format("%s %0.2f - %0.2f - %0.2f", "Ponder 0 1 2", TblPonderations.GetPonderations(0), TblPonderations.GetPonderations(1), TblPonderations.GetPonderations(2));
	MessageBox(0,tab,"",0);
	// FIN A VIRER	
	*/

	// on récupère la requête
	const JMTFR03RequeteE::JRequete & Requete = RequeteE.GetItems();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = Requete.m_ItemEffectif.GetCount();

	// on calcule le nombre de cas de l'item de l'effectif
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1) if (Requete.m_ItemEffectif.GetAt(IdxIndividu) && DATCible.m_TBLProbas.Item(IdxIndividu) > 0.0) Resultats.m_NbCas += 1;

	// on calcule l'effectif de référence
	JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pKEYPoids, *Requete.m_pDATPoids, *Requete.m_pTBLEchantillons, *Requete.m_pTBLHomotheties, DATCible, TblPonderations);

	// on mémorise l'effectif
	Resultats.m_Effectif = Effectif.AsJFlt64();

	// on quitte
	return (true);
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

const JMTFR03DATEffectif & JMTFR03TBLEffectifs::Add(const JMTFR03KEYEffectif & KEYEffectif, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, JMTFR03Logs* pLogs, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYEffectif.IsValid()) throw JInvalidCall::GetInstance();

	// on crée la clé de la cible
	JMTFR03KEYCible KEYCible;

	// on initialise la clé de la cible
	KEYCible.m_IdSource  = KEYEffectif.m_IdSource;
	KEYCible.m_IdTerrain = KEYEffectif.m_IdTerrain;
	KEYCible.m_IdCible   = KEYEffectif.m_IdCible;

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on récupère les pondérations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain); 

	// on crée et on construit la requête
	JMTFR03RequeteE RequeteE; RequeteE.SetItems(TBLTerrains, TBLModulations, KEYEffectif.m_IdSource, KEYEffectif.m_IdTerrain);

	// on crée et on calcule les résultats
	JResultats Resultats; if (!this->OnComputeResultats(DATCible, TblPonderations, RequeteE, Resultats)) throw JMTFR03Exception::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATEffectif & DATEffectif = m_TBLEffectifs.Add(KEYEffectif, Move);

	// on initialise l'élément
	DATEffectif.m_Effectif = Resultats.m_Effectif;
	DATEffectif.m_NbCas    = Resultats.m_NbCas;

	// on renvoie l'élément
	return (DATEffectif);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLEffectifs::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLEffectifs.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYEffectif & JMTFR03TBLEffectifs::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLEffectifs.GetKey());
}

const JMTFR03DATEffectif & JMTFR03TBLEffectifs::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLEffectifs.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLEffectifs::MoveTo(const JMTFR03KEYEffectif & KEYEffectif, JInt32 Move) const
{
	// on vérifie la validité de la clé
	if (!KEYEffectif.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLEffectifs.MoveTo(KEYEffectif, Move);
}

JVoid JMTFR03TBLEffectifs::MoveFirst() const
{
	// on déplace le curseur
	m_TBLEffectifs.MoveFirst();
}

JVoid JMTFR03TBLEffectifs::MoveNext() const
{
	// on déplace le curseur
	m_TBLEffectifs.MoveNext();
}

JVoid JMTFR03TBLEffectifs::MovePrev() const
{
	// on déplace le curseur
	m_TBLEffectifs.MovePrev();
}

JVoid JMTFR03TBLEffectifs::MoveLast() const
{
	// on déplace le curseur
	m_TBLEffectifs.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'élément courant

JVoid JMTFR03TBLEffectifs::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLEffectifs.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLEffectifs::Swap(JMTFR03TBLEffectifs & Source)
{
	// on permute les éléments
	m_TBLEffectifs.Swap(Source.m_TBLEffectifs);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLEffectifs::Reset()
{
	// on réinitialise les éléments
	m_TBLEffectifs.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLEffectifs::~JMTFR03TBLEffectifs()
{
	// on ne fait rien
}
