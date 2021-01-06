//
// Fichier: JMTFR03TBLLectorats.cpp
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLLectorats.h"

////////////////////
// les constructeurs

JMTFR03TBLLectorats::JMTFR03TBLLectorats()
{
	// on ne fait rien
}

JMTFR03TBLLectorats::JMTFR03TBLLectorats(const JMTFR03TBLLectorats & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLLectorats & JMTFR03TBLLectorats::operator =(const JMTFR03TBLLectorats & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLLectorats::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATLectorat & JMTFR03TBLLectorats::Add(const JMTFR03KEYLectorat & KEYLectorat, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYLectorat.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATLectorat & DATLectorat = m_TBLItems.Add(KEYLectorat, Move);

	// on renvoie l'�l�ment
	return (DATLectorat);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLLectorats::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYLectorat & JMTFR03TBLLectorats::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATLectorat & JMTFR03TBLLectorats::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATLectorat & JMTFR03TBLLectorats::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLLectorats::MoveTo(const JMTFR03KEYLectorat & KEYLectorat, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYLectorat.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYLectorat, Move);
}

JVoid JMTFR03TBLLectorats::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLLectorats::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLLectorats::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLLectorats::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLLectorats::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLLectorats::Swap(JMTFR03TBLLectorats & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLLectorats::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLLectorats::~JMTFR03TBLLectorats()
{
	// on ne fait rien
}
