//
// Fichier: JMTFR03TBLLectorats.cpp
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLLectorats.h"

////////////////////
// les constructeurs

JMTFR03TBLLectorats::JMTFR03TBLLectorats()
{
	// on ne fait rien
}

JMTFR03TBLLectorats::JMTFR03TBLLectorats(const JMTFR03TBLLectorats & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLLectorats & JMTFR03TBLLectorats::operator =(const JMTFR03TBLLectorats & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLLectorats::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATLectorat & JMTFR03TBLLectorats::Add(const JMTFR03KEYLectorat & KEYLectorat, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYLectorat.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATLectorat & DATLectorat = m_TBLItems.Add(KEYLectorat, Move);

	// on renvoie l'élément
	return (DATLectorat);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLLectorats::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYLectorat & JMTFR03TBLLectorats::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATLectorat & JMTFR03TBLLectorats::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATLectorat & JMTFR03TBLLectorats::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLLectorats::MoveTo(const JMTFR03KEYLectorat & KEYLectorat, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYLectorat.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYLectorat, Move);
}

JVoid JMTFR03TBLLectorats::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLLectorats::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLLectorats::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLLectorats::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLLectorats::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLLectorats::Swap(JMTFR03TBLLectorats & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLLectorats::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLLectorats::~JMTFR03TBLLectorats()
{
	// on ne fait rien
}
