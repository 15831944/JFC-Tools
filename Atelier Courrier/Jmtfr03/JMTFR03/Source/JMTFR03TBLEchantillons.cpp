//
// Fichier: JMTFR03TBLEchantillons.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLEchantillons.h"

////////////////////
// les constructeurs

JMTFR03TBLEchantillons::JMTFR03TBLEchantillons()
{
	// on ne fait rien
}

JMTFR03TBLEchantillons::JMTFR03TBLEchantillons(const JMTFR03TBLEchantillons & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLEchantillons & JMTFR03TBLEchantillons::operator =(const JMTFR03TBLEchantillons & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLEchantillons::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATEchantillon & JMTFR03TBLEchantillons::Add(const JMTFR03KEYEchantillon & KEYEchantillon, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYEchantillon.IsValid()) throw JInvalidCall::GetInstance();

	// on ajoute le nouvel �l�ment
	JMTFR03DATEchantillon & DATEchantillon = m_TBLItems.Add(KEYEchantillon, Move);

	// on renvoie l'�l�ment
	return (DATEchantillon);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLEchantillons::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYEchantillon & JMTFR03TBLEchantillons::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey());
}

const JMTFR03DATEchantillon & JMTFR03TBLEchantillons::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATEchantillon & JMTFR03TBLEchantillons::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLEchantillons::MoveTo(const JMTFR03KEYEchantillon & KEYEchantillon, JInt32 Move) const
{
	// on v�rifie la validit� de l'�l�ment
	if (!KEYEchantillon.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLItems.MoveTo(KEYEchantillon, Move);
}

JVoid JMTFR03TBLEchantillons::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03TBLEchantillons::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03TBLEchantillons::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03TBLEchantillons::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03TBLEchantillons::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLEchantillons::Swap(JMTFR03TBLEchantillons & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLEchantillons::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLEchantillons::~JMTFR03TBLEchantillons()
{
	// on ne fait rien
}
