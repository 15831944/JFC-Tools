//
// Fichier: JMTFR03TBLEffectifs.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLEffectifs.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Exception.h"

// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLEffectifs & JMTFR03TBLEffectifs::operator =(const JMTFR03TBLEffectifs & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLEffectifs::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLEffectifs.GetCount());
}

//////////////////////////////////////////
// la fonction pour calculer les r�sultats

JBool JMTFR03TBLEffectifs::OnComputeResultats(const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TblPonderations, const JMTFR03RequeteE & RequeteE, JResultats & Resultats) const
{
	// on initialise les r�sultats
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

	// on r�cup�re la requ�te
	const JMTFR03RequeteE::JRequete & Requete = RequeteE.GetItems();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = Requete.m_ItemEffectif.GetCount();

	// on calcule le nombre de cas de l'item de l'effectif
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1) if (Requete.m_ItemEffectif.GetAt(IdxIndividu) && DATCible.m_TBLProbas.Item(IdxIndividu) > 0.0) Resultats.m_NbCas += 1;

	// on calcule l'effectif de r�f�rence
	JMTResultats Effectif; Effectif.SetItem(Requete.m_ItemEffectif, *Requete.m_pKEYPoids, *Requete.m_pDATPoids, *Requete.m_pTBLEchantillons, *Requete.m_pTBLHomotheties, DATCible, TblPonderations);

	// on m�morise l'effectif
	Resultats.m_Effectif = Effectif.AsJFlt64();

	// on quitte
	return (true);
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

const JMTFR03DATEffectif & JMTFR03TBLEffectifs::Add(const JMTFR03KEYEffectif & KEYEffectif, const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JMTFR03TBLCibles & TBLCibles, JMTFR03Logs* pLogs, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYEffectif.IsValid()) throw JInvalidCall::GetInstance();

	// on cr�e la cl� de la cible
	JMTFR03KEYCible KEYCible;

	// on initialise la cl� de la cible
	KEYCible.m_IdSource  = KEYEffectif.m_IdSource;
	KEYCible.m_IdTerrain = KEYEffectif.m_IdTerrain;
	KEYCible.m_IdCible   = KEYEffectif.m_IdCible;

	// on recherche la cible
	TBLCibles.MoveTo(KEYCible); if (!TBLCibles.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re la cible
	const JMTFR03DATCible & DATCible = TBLCibles.GetItem();

	// on r�cup�re les pond�rations
	JMTFR03KEYTerrain KEYTerrain;
	KEYTerrain.m_IdSource  = KEYCible.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYCible.m_IdTerrain;
	const JMTFR03TBLPonderations & TblPonderations = TBLTerrains.GetTblPonderations(KEYTerrain); 

	// on cr�e et on construit la requ�te
	JMTFR03RequeteE RequeteE; RequeteE.SetItems(TBLTerrains, TBLModulations, KEYEffectif.m_IdSource, KEYEffectif.m_IdTerrain);

	// on cr�e et on calcule les r�sultats
	JResultats Resultats; if (!this->OnComputeResultats(DATCible, TblPonderations, RequeteE, Resultats)) throw JMTFR03Exception::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATEffectif & DATEffectif = m_TBLEffectifs.Add(KEYEffectif, Move);

	// on initialise l'�l�ment
	DATEffectif.m_Effectif = Resultats.m_Effectif;
	DATEffectif.m_NbCas    = Resultats.m_NbCas;

	// on renvoie l'�l�ment
	return (DATEffectif);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLEffectifs::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLEffectifs.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYEffectif & JMTFR03TBLEffectifs::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLEffectifs.GetKey());
}

const JMTFR03DATEffectif & JMTFR03TBLEffectifs::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLEffectifs.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLEffectifs::MoveTo(const JMTFR03KEYEffectif & KEYEffectif, JInt32 Move) const
{
	// on v�rifie la validit� de la cl�
	if (!KEYEffectif.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLEffectifs.MoveTo(KEYEffectif, Move);
}

JVoid JMTFR03TBLEffectifs::MoveFirst() const
{
	// on d�place le curseur
	m_TBLEffectifs.MoveFirst();
}

JVoid JMTFR03TBLEffectifs::MoveNext() const
{
	// on d�place le curseur
	m_TBLEffectifs.MoveNext();
}

JVoid JMTFR03TBLEffectifs::MovePrev() const
{
	// on d�place le curseur
	m_TBLEffectifs.MovePrev();
}

JVoid JMTFR03TBLEffectifs::MoveLast() const
{
	// on d�place le curseur
	m_TBLEffectifs.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment courant

JVoid JMTFR03TBLEffectifs::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLEffectifs.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLEffectifs::Swap(JMTFR03TBLEffectifs & Source)
{
	// on permute les �l�ments
	m_TBLEffectifs.Swap(Source.m_TBLEffectifs);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLEffectifs::Reset()
{
	// on r�initialise les �l�ments
	m_TBLEffectifs.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLEffectifs::~JMTFR03TBLEffectifs()
{
	// on ne fait rien
}
