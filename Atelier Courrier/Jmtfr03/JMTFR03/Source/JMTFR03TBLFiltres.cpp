//
// Fichier: JMTFR03TBLFiltres.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLFiltres.h"

////////////////////
// les constructeurs

JMTFR03TBLFiltres::JMTFR03TBLFiltres()
{
	// on ne fait rien
}

JMTFR03TBLFiltres::JMTFR03TBLFiltres(const JMTFR03TBLFiltres & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLFiltres & JMTFR03TBLFiltres::operator =(const JMTFR03TBLFiltres & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLFiltres::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATFiltre & JMTFR03TBLFiltres::Add(const JMTFR03KEYFiltre & KEYFiltre, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYFiltre.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATFiltre & DATFiltre = m_TBLItems.Add(KEYFiltre, Move);

	// on renvoie l'�l�ment
	return (DATFiltre);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLFiltres::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYFiltre & JMTFR03TBLFiltres::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATFiltre & JMTFR03TBLFiltres::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATFiltre & JMTFR03TBLFiltres::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLFiltres::MoveTo(const JMTFR03KEYFiltre& KEYFiltre, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYFiltre.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYFiltre, Move);
}

JVoid JMTFR03TBLFiltres::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLFiltres::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLFiltres::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLFiltres::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLFiltres::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLFiltres::Swap(JMTFR03TBLFiltres & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLFiltres::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLFiltres::~JMTFR03TBLFiltres()
{
	// on ne fait rien
}
