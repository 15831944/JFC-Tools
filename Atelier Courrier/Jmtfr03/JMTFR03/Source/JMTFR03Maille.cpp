//
// Fichier: JMTFR03Maille.cpp
// Auteur:  Sylvain SAMMURI
// Date:    16/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03Maille.h"

// on inclut les d�finitions n�cessaires
#include "float.h"

////////////////////
// les constructeurs

JMTFR03Maille::JKey::JKey()
{
	// on ne fait rien
}

JMTFR03Maille::JKey::JKey(const JKey & Source)
{
	// on v�rifie la validit� de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdTitre    = Source.m_IdTitre;
	m_IdParution = Source.m_IdParution;
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03Maille::JKey & JMTFR03Maille::JKey::operator =(const JKey & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdTitre    = Source.m_IdTitre;
		m_IdParution = Source.m_IdParution;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03Maille::JKey::operator <(const JKey & Reference) const
{
	// on compare la premi�re composante
	if (m_IdTitre < Reference.m_IdTitre) return (true);
	if (m_IdTitre > Reference.m_IdTitre) return (false);

	// on compare la derni�re composante
	return (m_IdParution < Reference.m_IdParution);
}

JBool JMTFR03Maille::JKey::operator >(const JKey & Reference) const
{
	// on compare la premi�re composante
	if (m_IdTitre > Reference.m_IdTitre) return (true);
	if (m_IdTitre < Reference.m_IdTitre) return (false);

	// on compare la derni�re composante
	return (m_IdParution > Reference.m_IdParution);
}

/////////////////
// le destructeur

JMTFR03Maille::JKey::~JKey()
{
	// on ne fait rien
}

////////////////////
// les constructeurs

JMTFR03Maille::JDat::JDat()
{
	// on ne fait rien
}

JMTFR03Maille::JDat::JDat(const JDat & Source)
{
	// on v�rifie la validit� de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_NbInsertions = Source.m_NbInsertions;
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03Maille::JDat & JMTFR03Maille::JDat::operator =(const JDat & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_NbInsertions = Source.m_NbInsertions;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

/////////////////
// le destructeur

JMTFR03Maille::JDat::~JDat()
{
	// on ne fait rien
}

////////////////////
// les constructeurs

JMTFR03Maille::JMTFR03Maille()
{
	// on ne fait rien
}

JMTFR03Maille::JMTFR03Maille(const JMTFR03Maille & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03Maille & JMTFR03Maille::operator =(const JMTFR03Maille & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03Maille::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JVoid JMTFR03Maille::Add(const JRef32 & IdTitre, const JRef32 & IdParution, const JRef32 & IdTuteur, JInt32 NbInsertions, JFlt64 GRPRef, JBool Move)
{
	// on cr�e la cl�
	JKey Key;

	// on v�rifie la validit� de l'�l�ment
	if (::_finite(GRPRef) == 0) throw JInvalidCall::GetInstance();

	// on v�rifie la validit� de l'�l�ment
	if (!IdTitre.IsValid() || !IdParution.IsValid() || !IdTuteur.IsValid() || GRPRef < 0.0 || NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on initialise la cl�
	Key.m_IdTitre = IdTitre; Key.m_IdParution = IdParution;

	// on ajoute le nouvel �l�ment
	JDat & Dat = m_TBLItems.Add(Key, Move);

	// on initialise l'�l�ment
	Dat.m_IdTuteur     = IdTuteur;
	Dat.m_NbInsertions = NbInsertions;
	Dat.m_GRPRef       = GRPRef;
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03Maille::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JRef32 & JMTFR03Maille::GetIdTitre() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey().m_IdTitre);
}

const JRef32 & JMTFR03Maille::GetIdParution() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetKey().m_IdParution);
}

const JRef32 & JMTFR03Maille::GetIdTuteur() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem().m_IdTuteur);
}

const JInt32x & JMTFR03Maille::GetNbInsertions() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem().m_NbInsertions);
}

const JFlt64x & JMTFR03Maille::GetGRPRef() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLItems.GetItem().m_GRPRef);
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03Maille::MoveTo(const JRef32 & IdTitre, const JRef32 & IdParution, JInt32 Move) const
{
	// on cr�e la cl�
	JKey Key;

	// on v�rifie la validit� de la cl�
	if (!IdTitre.IsValid() || !IdParution.IsValid()) throw JInvalidCall::GetInstance();

	// on initialise la cl�
	Key.m_IdTitre = IdTitre; Key.m_IdParution = IdParution;

	// on d�place le curseur
	m_TBLItems.MoveTo(Key, Move);
}

JVoid JMTFR03Maille::MoveFirst() const
{
	// on d�place le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03Maille::MoveNext() const
{
	// on d�place le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03Maille::MovePrev() const
{
	// on d�place le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03Maille::MoveLast() const
{
	// on d�place le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////////
// la fonction pour mettre � jour l'�l�ment du curseur

JVoid JMTFR03Maille::Update(JInt32 NbInsertions)
{
	// on v�rifie la validit� du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on r�cup�re l'�l�ment du curseur
	JDat & Dat = m_TBLItems.GetItem();

	// on met � jour l'�l�ment
	Dat.m_NbInsertions = NbInsertions;
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

JVoid JMTFR03Maille::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03Maille::Swap(JMTFR03Maille & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03Maille::Reset()
{
	// on r�initialise les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03Maille::~JMTFR03Maille()
{
	// on ne fait rien
}
