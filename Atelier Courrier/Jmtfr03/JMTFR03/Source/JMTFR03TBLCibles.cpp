//
// Fichier: JMTFR03TBLCibles.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLCibles.h"

////////////////////
// les constructeurs

JMTFR03TBLCibles::JMTFR03TBLCibles()
{
	// on ne fait rien
}

JMTFR03TBLCibles::JMTFR03TBLCibles(const JMTFR03TBLCibles & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLCibles & JMTFR03TBLCibles::operator =(const JMTFR03TBLCibles & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLCibles::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLCibles.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

const JMTFR03DATCible & JMTFR03TBLCibles::Add(const JMTFR03KEYCible & KEYCible, const JBitArray & ItemCible, JBool Move)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on v�rifie la validit� de la cl�
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = ItemCible.GetCount();

	// on alloue la table des probabilit�s
	JArray<JFlt64x> TBLProbas; TBLProbas.SetCount(NbIndividus);

	// on initialise la table des probabilit�s
	for (JInt32 Idx = 0; Idx < NbIndividus; Idx += 1) if (ItemCible.GetAt(Idx)) TBLProbas.Item(Idx) = 1.0;

	// on ajoute le nouvel �l�ment
	JMTFR03DATCible & DATCible = m_TBLCibles.Add(KEYCible, Move);

	// on initialise l'�l�ment
	DATCible.m_TBLProbas.Swap(TBLProbas);

	// on renvoie l'�l�ment
	return (DATCible);
}

const JMTFR03DATCible & JMTFR03TBLCibles::Add(const JMTFR03KEYCible & KEYCible, const JUnt32* pItemCible, JInt32 NbIndividus, JBool Move)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on v�rifie la validit� de la cl�
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on v�rifie la validit� de l'item cible
	if (NbIndividus < 0 || (NbIndividus > 0 && pItemCible == 0)) throw JInvalidCall::GetInstance();

	// on alloue la table des probabilit�s
	JArray<JFlt64x> TBLProbas; TBLProbas.SetCount(NbIndividus);

	// on initialise le masque
	JMsk32 Mask = 0;

	// on boucle sur tous les individus
	for (JInt32 Idx = 0; Idx < NbIndividus; Idx += 1)
	{
		// on r�cup�re le masque des individus
		JInt32 Jdx = Idx % 32; if (Jdx == 0) { Mask = *pItemCible; pItemCible += 1; }

		// on initialise la probabilit� de l'individu
		if (Mask.GetAt(Jdx)) TBLProbas.Item(Idx) = 1.0;
	}

	// on ajoute le nouvel �l�ment
	JMTFR03DATCible & DATCible = m_TBLCibles.Add(KEYCible, Move);

	// on initialise l'�l�ment
	DATCible.m_TBLProbas.Swap(TBLProbas);

	// on renvoie l'�l�ment
	return (DATCible);
}

const JMTFR03DATCible & JMTFR03TBLCibles::Add(const JMTFR03KEYCible & KEYCible, const JFlt64* pItemProbas, JInt32 NbIndividus, JBool Move)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on v�rifie la validit� de la cl�
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on v�rifie la validit� de l'item des probabilit�s
	if (NbIndividus < 0 || (NbIndividus > 0 && pItemProbas == 0)) throw JInvalidCall::GetInstance();

	// on alloue la table des probabilit�s
	JArray<JFlt64x> TBLProbas; TBLProbas.SetCount(NbIndividus);

	// on boucle sur tous les individus
	for (JInt32 Idx = 0; Idx < NbIndividus; Idx += 1)
	{
		// on v�rifie la validit� de la probabilit� de l'individu courant
		if (*pItemProbas < 0.0 || *pItemProbas > 1.0) throw JInvalidCall::GetInstance();

		// on m�morise la probabilit� de l'individu
		TBLProbas.Item(Idx) = *pItemProbas;

		// on passe � la probabilit� suivante
		pItemProbas += 1;
	}

	// on ajoute le nouvel �l�ment
	JMTFR03DATCible & DATCible = m_TBLCibles.Add(KEYCible, Move);

	// on initialise l'�l�ment
	DATCible.m_TBLProbas.Swap(TBLProbas);

	// on renvoie l'�l�ment
	return (DATCible);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLCibles::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLCibles.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYCible & JMTFR03TBLCibles::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLCibles.GetKey());
}

const JMTFR03DATCible & JMTFR03TBLCibles::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLCibles.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLCibles::MoveTo(const JMTFR03KEYCible & KEYCible, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYCible.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLCibles.MoveTo(KEYCible, Move);
}

JVoid JMTFR03TBLCibles::MoveFirst() const
{
	// on d�place le curseur
	m_TBLCibles.MoveFirst();
}

JVoid JMTFR03TBLCibles::MoveNext() const
{
	// on d�place le curseur
	m_TBLCibles.MoveNext();
}

JVoid JMTFR03TBLCibles::MovePrev() const
{
	// on d�place le curseur
	m_TBLCibles.MovePrev();
}

JVoid JMTFR03TBLCibles::MoveLast() const
{
	// on d�place le curseur
	m_TBLCibles.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLCibles::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLCibles.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLCibles::Swap(JMTFR03TBLCibles & Source)
{
	// on permute les �l�ments
	m_TBLCibles.Swap(Source.m_TBLCibles);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLCibles::Reset()
{
	// on lib�re les �l�ments
	m_TBLCibles.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLCibles::~JMTFR03TBLCibles()
{
	// on ne fait rien
}
