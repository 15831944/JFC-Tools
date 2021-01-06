//
// Fichier: JMTFR03TBLReprises.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLReprises.h"

////////////////////
// les constructeurs

JMTFR03TBLReprises::JMTFR03TBLReprises()
{
	// on ne fait rien
}

JMTFR03TBLReprises::JMTFR03TBLReprises(const JMTFR03TBLReprises & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLReprises & JMTFR03TBLReprises::operator =(const JMTFR03TBLReprises & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLReprises::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATReprise & JMTFR03TBLReprises::Add(const JMTFR03KEYReprise & KEYReprise, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYReprise.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATReprise & DATReprise = m_TBLItems.Add(KEYReprise, Move);

	// on renvoie l'élément
	return (DATReprise);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLReprises::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYReprise & JMTFR03TBLReprises::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATReprise & JMTFR03TBLReprises::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATReprise & JMTFR03TBLReprises::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLReprises::MoveTo(const JMTFR03KEYReprise & KEYReprise, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYReprise.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYReprise, Move);
}

JVoid JMTFR03TBLReprises::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLReprises::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLReprises::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLReprises::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLReprises::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLReprises::Swap(JMTFR03TBLReprises & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLReprises::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLReprises::~JMTFR03TBLReprises()
{
	// on ne fait rien
}
