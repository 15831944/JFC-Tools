//
// Fichier: JMTFR03TBLAttributs.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLAttributs.h"

////////////////////
// les constructeurs

JMTFR03TBLAttributs::JMTFR03TBLAttributs()
{
	// on ne fait rien
}

JMTFR03TBLAttributs::JMTFR03TBLAttributs(const JMTFR03TBLAttributs & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLAttributs & JMTFR03TBLAttributs::operator =(const JMTFR03TBLAttributs & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLAttributs::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATAttribut & JMTFR03TBLAttributs::Add(const JMTFR03KEYAttribut & KEYAttribut, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYAttribut.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATAttribut & DATAttribut = m_TBLItems.Add(KEYAttribut, Move);

	// on renvoie l'�l�ment
	return (DATAttribut);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLAttributs::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYAttribut & JMTFR03TBLAttributs::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATAttribut & JMTFR03TBLAttributs::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATAttribut & JMTFR03TBLAttributs::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLAttributs::MoveTo(const JMTFR03KEYAttribut & KEYAttribut, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYAttribut.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYAttribut, Move);
}

JVoid JMTFR03TBLAttributs::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLAttributs::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLAttributs::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLAttributs::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLAttributs::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLAttributs::Swap(JMTFR03TBLAttributs & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLAttributs::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLAttributs::~JMTFR03TBLAttributs()
{
	// on ne fait rien
}
