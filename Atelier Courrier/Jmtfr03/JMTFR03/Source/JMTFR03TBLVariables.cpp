//
// Fichier: JMTFR03TBLVariables.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLVariables.h"

////////////////////
// les constructeurs

JMTFR03TBLVariables::JMTFR03TBLVariables()
{
	// on ne fait rien
}

JMTFR03TBLVariables::JMTFR03TBLVariables(const JMTFR03TBLVariables & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLVariables & JMTFR03TBLVariables::operator =(const JMTFR03TBLVariables & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLVariables::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATVariable & JMTFR03TBLVariables::Add(const JMTFR03KEYVariable & KEYVariable, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYVariable.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATVariable & DATVariable = m_TBLItems.Add(KEYVariable, Move);

	// on renvoie l'�l�ment
	return (DATVariable);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLVariables::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYVariable & JMTFR03TBLVariables::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATVariable & JMTFR03TBLVariables::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATVariable & JMTFR03TBLVariables::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLVariables::MoveTo(const JMTFR03KEYVariable & KEYVariable, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYVariable.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYVariable, Move);
}

JVoid JMTFR03TBLVariables::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLVariables::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLVariables::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLVariables::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLVariables::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLVariables::Swap(JMTFR03TBLVariables & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLVariables::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLVariables::~JMTFR03TBLVariables()
{
	// on ne fait rien
}
