//
// Fichier: JMTFR03TBLUnivers.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLUnivers.h"

////////////////////
// les constructeurs

JMTFR03TBLUnivers::JMTFR03TBLUnivers()
{
	// on ne fait rien
}

JMTFR03TBLUnivers::JMTFR03TBLUnivers(const JMTFR03TBLUnivers & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLUnivers & JMTFR03TBLUnivers::operator =(const JMTFR03TBLUnivers & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLUnivers::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATUnivers & JMTFR03TBLUnivers::Add(const JMTFR03KEYUnivers & KEYUnivers, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYUnivers.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATUnivers & DATUnivers = m_TBLItems.Add(KEYUnivers, Move);

	// on renvoie l'�l�ment
	return (DATUnivers);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLUnivers::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYUnivers & JMTFR03TBLUnivers::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATUnivers & JMTFR03TBLUnivers::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATUnivers & JMTFR03TBLUnivers::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLUnivers::MoveTo(const JMTFR03KEYUnivers & KEYUnivers, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYUnivers.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYUnivers, Move);
}

JVoid JMTFR03TBLUnivers::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLUnivers::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLUnivers::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLUnivers::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLUnivers::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLUnivers::Swap(JMTFR03TBLUnivers & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLUnivers::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLUnivers::~JMTFR03TBLUnivers()
{
	// on ne fait rien
}
