//
// Fichier: JMTFR03TBLVariables.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLVariables.h"

////////////////////
// les constructeurs

JMTFR03TBLVariables::JMTFR03TBLVariables()
{
	// on ne fait rien
}

JMTFR03TBLVariables::JMTFR03TBLVariables(const JMTFR03TBLVariables & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLVariables & JMTFR03TBLVariables::operator =(const JMTFR03TBLVariables & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLVariables::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATVariable & JMTFR03TBLVariables::Add(const JMTFR03KEYVariable & KEYVariable, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYVariable.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATVariable & DATVariable = m_TBLItems.Add(KEYVariable, Move);

	// on renvoie l'élément
	return (DATVariable);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLVariables::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYVariable & JMTFR03TBLVariables::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATVariable & JMTFR03TBLVariables::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATVariable & JMTFR03TBLVariables::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLVariables::MoveTo(const JMTFR03KEYVariable & KEYVariable, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYVariable.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYVariable, Move);
}

JVoid JMTFR03TBLVariables::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLVariables::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLVariables::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLVariables::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLVariables::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLVariables::Swap(JMTFR03TBLVariables & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLVariables::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLVariables::~JMTFR03TBLVariables()
{
	// on ne fait rien
}
