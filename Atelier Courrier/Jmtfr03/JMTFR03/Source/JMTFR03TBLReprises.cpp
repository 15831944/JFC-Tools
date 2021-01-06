//
// Fichier: JMTFR03TBLReprises.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLReprises.h"

////////////////////
// les constructeurs

JMTFR03TBLReprises::JMTFR03TBLReprises()
{
	// on ne fait rien
}

JMTFR03TBLReprises::JMTFR03TBLReprises(const JMTFR03TBLReprises & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLReprises & JMTFR03TBLReprises::operator =(const JMTFR03TBLReprises & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLReprises::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATReprise & JMTFR03TBLReprises::Add(const JMTFR03KEYReprise & KEYReprise, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYReprise.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATReprise & DATReprise = m_TBLItems.Add(KEYReprise, Move);

	// on renvoie l'�l�ment
	return (DATReprise);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLReprises::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYReprise & JMTFR03TBLReprises::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATReprise & JMTFR03TBLReprises::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATReprise & JMTFR03TBLReprises::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLReprises::MoveTo(const JMTFR03KEYReprise & KEYReprise, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYReprise.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYReprise, Move);
}

JVoid JMTFR03TBLReprises::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLReprises::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLReprises::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLReprises::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLReprises::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLReprises::Swap(JMTFR03TBLReprises & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLReprises::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLReprises::~JMTFR03TBLReprises()
{
	// on ne fait rien
}
