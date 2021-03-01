//
// Fichier: JMTFR03TBLSegments.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLSegments.h"

////////////////////
// les constructeurs

JMTFR03TBLSegments::JMTFR03TBLSegments()
{
	// on ne fait rien
}

JMTFR03TBLSegments::JMTFR03TBLSegments(const JMTFR03TBLSegments & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLSegments & JMTFR03TBLSegments::operator =(const JMTFR03TBLSegments & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLSegments::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATSegment & JMTFR03TBLSegments::Add(const JMTFR03KEYSegment & KEYSegment, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYSegment.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATSegment & DATSegment = m_TBLItems.Add(KEYSegment, Move);

	// on renvoie l'�l�ment
	return (DATSegment);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLSegments::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYSegment & JMTFR03TBLSegments::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATSegment & JMTFR03TBLSegments::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATSegment & JMTFR03TBLSegments::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLSegments::MoveTo(const JMTFR03KEYSegment & KEYSegment, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYSegment.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYSegment, Move);
}

JVoid JMTFR03TBLSegments::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLSegments::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLSegments::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLSegments::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLSegments::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLSegments::Swap(JMTFR03TBLSegments & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLSegments::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLSegments::~JMTFR03TBLSegments()
{
	// on ne fait rien
}
