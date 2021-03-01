//
// Fichier: JMTFR03TBLHomotheties.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLHomotheties.h"

////////////////////
// les constructeurs

JMTFR03TBLHomotheties::JMTFR03TBLHomotheties()
{
	// on ne fait rien
}

JMTFR03TBLHomotheties::JMTFR03TBLHomotheties(const JMTFR03TBLHomotheties & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLHomotheties & JMTFR03TBLHomotheties::operator =(const JMTFR03TBLHomotheties & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLHomotheties::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATHomothetie & JMTFR03TBLHomotheties::Add(const JMTFR03KEYHomothetie & KEYHomothetie, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYHomothetie.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATHomothetie & DATHomothetie = m_TBLItems.Add(KEYHomothetie, Move);

	// on renvoie l'�l�ment
	return (DATHomothetie);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLHomotheties::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYHomothetie & JMTFR03TBLHomotheties::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATHomothetie & JMTFR03TBLHomotheties::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATHomothetie & JMTFR03TBLHomotheties::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLHomotheties::MoveTo(const JMTFR03KEYHomothetie & KEYHomothetie, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYHomothetie.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYHomothetie, Move);
}

JVoid JMTFR03TBLHomotheties::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLHomotheties::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLHomotheties::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLHomotheties::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLHomotheties::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLHomotheties::Swap(JMTFR03TBLHomotheties & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLHomotheties::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLHomotheties::~JMTFR03TBLHomotheties()
{
	// on ne fait rien
}
