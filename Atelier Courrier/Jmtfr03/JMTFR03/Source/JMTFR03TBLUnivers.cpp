//
// Fichier: JMTFR03TBLUnivers.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLUnivers.h"

////////////////////
// les constructeurs

JMTFR03TBLUnivers::JMTFR03TBLUnivers()
{
	// on ne fait rien
}

JMTFR03TBLUnivers::JMTFR03TBLUnivers(const JMTFR03TBLUnivers & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLUnivers & JMTFR03TBLUnivers::operator =(const JMTFR03TBLUnivers & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLUnivers::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATUnivers & JMTFR03TBLUnivers::Add(const JMTFR03KEYUnivers & KEYUnivers, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYUnivers.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATUnivers & DATUnivers = m_TBLItems.Add(KEYUnivers, Move);

	// on renvoie l'élément
	return (DATUnivers);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLUnivers::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYUnivers & JMTFR03TBLUnivers::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATUnivers & JMTFR03TBLUnivers::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATUnivers & JMTFR03TBLUnivers::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLUnivers::MoveTo(const JMTFR03KEYUnivers & KEYUnivers, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYUnivers.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYUnivers, Move);
}

JVoid JMTFR03TBLUnivers::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLUnivers::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLUnivers::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLUnivers::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLUnivers::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLUnivers::Swap(JMTFR03TBLUnivers & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLUnivers::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLUnivers::~JMTFR03TBLUnivers()
{
	// on ne fait rien
}
