//
// Fichier: JMTFR03TBLAttributs.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLAttributs.h"

////////////////////
// les constructeurs

JMTFR03TBLAttributs::JMTFR03TBLAttributs()
{
	// on ne fait rien
}

JMTFR03TBLAttributs::JMTFR03TBLAttributs(const JMTFR03TBLAttributs & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLAttributs & JMTFR03TBLAttributs::operator =(const JMTFR03TBLAttributs & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLAttributs::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATAttribut & JMTFR03TBLAttributs::Add(const JMTFR03KEYAttribut & KEYAttribut, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYAttribut.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATAttribut & DATAttribut = m_TBLItems.Add(KEYAttribut, Move);

	// on renvoie l'élément
	return (DATAttribut);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLAttributs::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYAttribut & JMTFR03TBLAttributs::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATAttribut & JMTFR03TBLAttributs::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATAttribut & JMTFR03TBLAttributs::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLAttributs::MoveTo(const JMTFR03KEYAttribut & KEYAttribut, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYAttribut.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYAttribut, Move);
}

JVoid JMTFR03TBLAttributs::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLAttributs::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLAttributs::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLAttributs::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLAttributs::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLAttributs::Swap(JMTFR03TBLAttributs & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLAttributs::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLAttributs::~JMTFR03TBLAttributs()
{
	// on ne fait rien
}
