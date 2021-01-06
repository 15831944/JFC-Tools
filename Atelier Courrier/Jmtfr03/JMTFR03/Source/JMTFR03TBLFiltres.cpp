//
// Fichier: JMTFR03TBLFiltres.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLFiltres.h"

////////////////////
// les constructeurs

JMTFR03TBLFiltres::JMTFR03TBLFiltres()
{
	// on ne fait rien
}

JMTFR03TBLFiltres::JMTFR03TBLFiltres(const JMTFR03TBLFiltres & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLFiltres & JMTFR03TBLFiltres::operator =(const JMTFR03TBLFiltres & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLFiltres::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATFiltre & JMTFR03TBLFiltres::Add(const JMTFR03KEYFiltre & KEYFiltre, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYFiltre.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATFiltre & DATFiltre = m_TBLItems.Add(KEYFiltre, Move);

	// on renvoie l'élément
	return (DATFiltre);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLFiltres::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYFiltre & JMTFR03TBLFiltres::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATFiltre & JMTFR03TBLFiltres::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATFiltre & JMTFR03TBLFiltres::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLFiltres::MoveTo(const JMTFR03KEYFiltre& KEYFiltre, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYFiltre.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYFiltre, Move);
}

JVoid JMTFR03TBLFiltres::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLFiltres::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLFiltres::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLFiltres::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLFiltres::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLFiltres::Swap(JMTFR03TBLFiltres & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLFiltres::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLFiltres::~JMTFR03TBLFiltres()
{
	// on ne fait rien
}
