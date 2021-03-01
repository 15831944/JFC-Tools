//
// Fichier: JMTFR03TBLHomotheties.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLHomotheties.h"

////////////////////
// les constructeurs

JMTFR03TBLHomotheties::JMTFR03TBLHomotheties()
{
	// on ne fait rien
}

JMTFR03TBLHomotheties::JMTFR03TBLHomotheties(const JMTFR03TBLHomotheties & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLHomotheties & JMTFR03TBLHomotheties::operator =(const JMTFR03TBLHomotheties & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLHomotheties::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATHomothetie & JMTFR03TBLHomotheties::Add(const JMTFR03KEYHomothetie & KEYHomothetie, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYHomothetie.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATHomothetie & DATHomothetie = m_TBLItems.Add(KEYHomothetie, Move);

	// on renvoie l'élément
	return (DATHomothetie);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLHomotheties::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYHomothetie & JMTFR03TBLHomotheties::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATHomothetie & JMTFR03TBLHomotheties::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATHomothetie & JMTFR03TBLHomotheties::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLHomotheties::MoveTo(const JMTFR03KEYHomothetie & KEYHomothetie, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYHomothetie.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYHomothetie, Move);
}

JVoid JMTFR03TBLHomotheties::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLHomotheties::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLHomotheties::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLHomotheties::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLHomotheties::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLHomotheties::Swap(JMTFR03TBLHomotheties & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLHomotheties::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLHomotheties::~JMTFR03TBLHomotheties()
{
	// on ne fait rien
}
