//
// Fichier: JMTFR03Couplage.cpp
// Auteur:  Sylvain SAMMURI
// Date:    17/02/2005
//

// on inclut les définitions nécessaires
#include "JMTFR03Couplage.h"

////////////////////
// les constructeurs

JMTFR03Couplage::JMTFR03Couplage()
{
	// on ne fait rien
}

JMTFR03Couplage::JMTFR03Couplage(const JMTFR03Couplage & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03Couplage & JMTFR03Couplage::operator =(const JMTFR03Couplage & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03Couplage::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JVoid JMTFR03Couplage::Add(const JRef32 & IdTitre, JBool Move)
{
	// on vérifie la validité de l'élément
	if (!IdTitre.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel élément
	m_TBLItems.Add(IdTitre, Move) = 0;
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03Couplage::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

//////////////////////////////////////////////////
// la fonction pour récupérer l'élément du curseur

const JRef32 & JMTFR03Couplage::GetIdTitre() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03Couplage::MoveTo(const JRef32 & IdTitre, JInt32 Move) const
{
	// on vérifie la validité de la clé
	if (!IdTitre.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLItems.MoveTo(IdTitre, Move);
}

JVoid JMTFR03Couplage::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03Couplage::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03Couplage::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03Couplage::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03Couplage::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03Couplage::Swap(JMTFR03Couplage & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03Couplage::Reset()
{
	// on réinitialise les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03Couplage::~JMTFR03Couplage()
{
	// on ne fait rien
}
