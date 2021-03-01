//
// Fichier: JMTFR03TBLPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLPoids.h"

////////////////////
// les constructeurs

JMTFR03TBLPoids::JMTFR03TBLPoids()
{
	// on ne fait rien
}

JMTFR03TBLPoids::JMTFR03TBLPoids(const JMTFR03TBLPoids & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLPoids & JMTFR03TBLPoids::operator =(const JMTFR03TBLPoids & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLPoids::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATPoids & JMTFR03TBLPoids::Add(const JMTFR03KEYPoids & KEYPoids, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYPoids.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATPoids & DATPoids = m_TBLItems.Add(KEYPoids, Move);

	// on renvoie l'�l�ment
	return (DATPoids);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLPoids::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYPoids & JMTFR03TBLPoids ::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATPoids & JMTFR03TBLPoids::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATPoids & JMTFR03TBLPoids::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLPoids::MoveTo(const JMTFR03KEYPoids & KEYPoids, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYPoids.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYPoids, Move);
}

JVoid JMTFR03TBLPoids::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLPoids::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLPoids::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLPoids::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLPoids::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLPoids::Swap(JMTFR03TBLPoids & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLPoids::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLPoids::~JMTFR03TBLPoids()
{
	// on ne fait rien
}
