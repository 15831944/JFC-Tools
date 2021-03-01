//
// Fichier: JMTFR03TBLPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLPoids.h"

////////////////////
// les constructeurs

JMTFR03TBLPoids::JMTFR03TBLPoids()
{
	// on ne fait rien
}

JMTFR03TBLPoids::JMTFR03TBLPoids(const JMTFR03TBLPoids & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLPoids & JMTFR03TBLPoids::operator =(const JMTFR03TBLPoids & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLPoids::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATPoids & JMTFR03TBLPoids::Add(const JMTFR03KEYPoids & KEYPoids, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYPoids.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATPoids & DATPoids = m_TBLItems.Add(KEYPoids, Move);

	// on renvoie l'élément
	return (DATPoids);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLPoids::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYPoids & JMTFR03TBLPoids ::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATPoids & JMTFR03TBLPoids::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATPoids & JMTFR03TBLPoids::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLPoids::MoveTo(const JMTFR03KEYPoids & KEYPoids, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYPoids.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYPoids, Move);
}

JVoid JMTFR03TBLPoids::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLPoids::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLPoids::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLPoids::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLPoids::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLPoids::Swap(JMTFR03TBLPoids & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLPoids::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLPoids::~JMTFR03TBLPoids()
{
	// on ne fait rien
}
