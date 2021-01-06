//
// Fichier: JMTFR03TBLEchantillons.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLEchantillons.h"

////////////////////
// les constructeurs

JMTFR03TBLEchantillons::JMTFR03TBLEchantillons()
{
	// on ne fait rien
}

JMTFR03TBLEchantillons::JMTFR03TBLEchantillons(const JMTFR03TBLEchantillons & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLEchantillons & JMTFR03TBLEchantillons::operator =(const JMTFR03TBLEchantillons & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLEchantillons::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATEchantillon & JMTFR03TBLEchantillons::Add(const JMTFR03KEYEchantillon & KEYEchantillon, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYEchantillon.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	JMTFR03DATEchantillon & DATEchantillon = m_TBLItems.Add(KEYEchantillon, Move);

	// on renvoie l'élément
	return (DATEchantillon);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLEchantillons::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYEchantillon & JMTFR03TBLEchantillons::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATEchantillon & JMTFR03TBLEchantillons::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATEchantillon & JMTFR03TBLEchantillons::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLEchantillons::MoveTo(const JMTFR03KEYEchantillon & KEYEchantillon, JInt32 Move) const
{
	// on vérifie la validité de l'élément
	if (!KEYEchantillon.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(KEYEchantillon, Move);
}

JVoid JMTFR03TBLEchantillons::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLEchantillons::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLEchantillons::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLEchantillons::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03TBLEchantillons::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLEchantillons::Swap(JMTFR03TBLEchantillons & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLEchantillons::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLEchantillons::~JMTFR03TBLEchantillons()
{
	// on ne fait rien
}
