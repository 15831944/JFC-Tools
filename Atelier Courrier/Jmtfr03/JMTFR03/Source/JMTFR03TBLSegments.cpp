//
// Fichier: JMTFR03TBLSegments.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLSegments.h"

////////////////////
// les constructeurs

JMTFR03TBLSegments::JMTFR03TBLSegments()
{
	// on ne fait rien
}

JMTFR03TBLSegments::JMTFR03TBLSegments(const JMTFR03TBLSegments & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLSegments & JMTFR03TBLSegments::operator =(const JMTFR03TBLSegments & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLSegments::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATSegment & JMTFR03TBLSegments::Add(const JMTFR03KEYSegment & KEYSegment, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYSegment.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATSegment & DATSegment = m_TBLItems.Add(KEYSegment, Move);

	// on renvoie l'élément
	return (DATSegment);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLSegments::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYSegment & JMTFR03TBLSegments::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATSegment & JMTFR03TBLSegments::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATSegment & JMTFR03TBLSegments::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLSegments::MoveTo(const JMTFR03KEYSegment & KEYSegment, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYSegment.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYSegment, Move);
}

JVoid JMTFR03TBLSegments::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLSegments::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLSegments::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLSegments::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLSegments::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLSegments::Swap(JMTFR03TBLSegments & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLSegments::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLSegments::~JMTFR03TBLSegments()
{
	// on ne fait rien
}
