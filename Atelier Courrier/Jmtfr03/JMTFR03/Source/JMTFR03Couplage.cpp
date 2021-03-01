//
// Fichier: JMTFR03Couplage.cpp
// Auteur:  Sylvain SAMMURI
// Date:    17/02/2005
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03Couplage.h"

////////////////////
// les constructeurs

JMTFR03Couplage::JMTFR03Couplage()
{
	// on ne fait rien
}

JMTFR03Couplage::JMTFR03Couplage(const JMTFR03Couplage & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03Couplage & JMTFR03Couplage::operator =(const JMTFR03Couplage & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03Couplage::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JVoid JMTFR03Couplage::Add(const JRef32 & IdTitre, JBool Move)
{
	// on v�rifie la validit� de l'�l�ment
	if (!IdTitre.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	m_TBLItems.Add(IdTitre, Move) = 0;
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03Couplage::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer l'�l�ment du curseur

const JRef32 & JMTFR03Couplage::GetIdTitre() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03Couplage::MoveTo(const JRef32 & IdTitre, JInt32 Move) const
{
	// on v�rifie la validit� de la cl�
	if (!IdTitre.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(IdTitre, Move);
}

JVoid JMTFR03Couplage::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03Couplage::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03Couplage::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03Couplage::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03Couplage::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03Couplage::Swap(JMTFR03Couplage & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03Couplage::Reset()
{
	// on r�initialise les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03Couplage::~JMTFR03Couplage()
{
	// on ne fait rien
}
