//
// Fichier: JMTFR03TBLCibles.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLCibles.h"

////////////////////
// les constructeurs

JMTFR03TBLCibles::JMTFR03TBLCibles()
{
	// on ne fait rien
}

JMTFR03TBLCibles::JMTFR03TBLCibles(const JMTFR03TBLCibles & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLCibles & JMTFR03TBLCibles::operator =(const JMTFR03TBLCibles & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLCibles::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLCibles.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

const JMTFR03DATCible & JMTFR03TBLCibles::Add(const JMTFR03KEYCible & KEYCible, const JBitArray & ItemCible, JBool Move)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on vérifie la validité de la clé
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = ItemCible.GetCount();

	// on alloue la table des probabilités
	JArray<JFlt64x> TBLProbas; TBLProbas.SetCount(NbIndividus);

	// on initialise la table des probabilités
	for (JInt32 Idx = 0; Idx < NbIndividus; Idx += 1) if (ItemCible.GetAt(Idx)) TBLProbas.Item(Idx) = 1.0;

	// on ajoute le nouvel élément
	JMTFR03DATCible & DATCible = m_TBLCibles.Add(KEYCible, Move);

	// on initialise l'élément
	DATCible.m_TBLProbas.Swap(TBLProbas);

	// on renvoie l'élément
	return (DATCible);
}

const JMTFR03DATCible & JMTFR03TBLCibles::Add(const JMTFR03KEYCible & KEYCible, const JUnt32* pItemCible, JInt32 NbIndividus, JBool Move)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on vérifie la validité de la clé
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on vérifie la validité de l'item cible
	if (NbIndividus < 0 || (NbIndividus > 0 && pItemCible == 0)) throw JInvalidCall::GetInstance();

	// on alloue la table des probabilités
	JArray<JFlt64x> TBLProbas; TBLProbas.SetCount(NbIndividus);

	// on initialise le masque
	JMsk32 Mask = 0;

	// on boucle sur tous les individus
	for (JInt32 Idx = 0; Idx < NbIndividus; Idx += 1)
	{
		// on récupère le masque des individus
		JInt32 Jdx = Idx % 32; if (Jdx == 0) { Mask = *pItemCible; pItemCible += 1; }

		// on initialise la probabilité de l'individu
		if (Mask.GetAt(Jdx)) TBLProbas.Item(Idx) = 1.0;
	}

	// on ajoute le nouvel élément
	JMTFR03DATCible & DATCible = m_TBLCibles.Add(KEYCible, Move);

	// on initialise l'élément
	DATCible.m_TBLProbas.Swap(TBLProbas);

	// on renvoie l'élément
	return (DATCible);
}

const JMTFR03DATCible & JMTFR03TBLCibles::Add(const JMTFR03KEYCible & KEYCible, const JFlt64* pItemProbas, JInt32 NbIndividus, JBool Move)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on vérifie la validité de la clé
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on vérifie la validité de l'item des probabilités
	if (NbIndividus < 0 || (NbIndividus > 0 && pItemProbas == 0)) throw JInvalidCall::GetInstance();

	// on alloue la table des probabilités
	JArray<JFlt64x> TBLProbas; TBLProbas.SetCount(NbIndividus);

	// on boucle sur tous les individus
	for (JInt32 Idx = 0; Idx < NbIndividus; Idx += 1)
	{
		// on vérifie la validité de la probabilité de l'individu courant
		if (*pItemProbas < 0.0 || *pItemProbas > 1.0) throw JInvalidCall::GetInstance();

		// on mémorise la probabilité de l'individu
		TBLProbas.Item(Idx) = *pItemProbas;

		// on passe à la probabilité suivante
		pItemProbas += 1;
	}

	// on ajoute le nouvel élément
	JMTFR03DATCible & DATCible = m_TBLCibles.Add(KEYCible, Move);

	// on initialise l'élément
	DATCible.m_TBLProbas.Swap(TBLProbas);

	// on renvoie l'élément
	return (DATCible);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLCibles::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLCibles.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYCible & JMTFR03TBLCibles::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLCibles.GetKey());
}

const JMTFR03DATCible & JMTFR03TBLCibles::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLCibles.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLCibles::MoveTo(const JMTFR03KEYCible & KEYCible, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLCibles.MoveTo(KEYCible, Move);
}

JVoid JMTFR03TBLCibles::MoveFirst() const
{
	// on déplace le curseur
	m_TBLCibles.MoveFirst();
}

JVoid JMTFR03TBLCibles::MoveNext() const
{
	// on déplace le curseur
	m_TBLCibles.MoveNext();
}

JVoid JMTFR03TBLCibles::MovePrev() const
{
	// on déplace le curseur
	m_TBLCibles.MovePrev();
}

JVoid JMTFR03TBLCibles::MoveLast() const
{
	// on déplace le curseur
	m_TBLCibles.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLCibles::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLCibles.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLCibles::Swap(JMTFR03TBLCibles & Source)
{
	// on permute les éléments
	m_TBLCibles.Swap(Source.m_TBLCibles);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLCibles::Reset()
{
	// on libère les éléments
	m_TBLCibles.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLCibles::~JMTFR03TBLCibles()
{
	// on ne fait rien
}
