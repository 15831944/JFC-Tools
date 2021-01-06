//
// Fichier: JMTFR03Maille.cpp
// Auteur:  Sylvain SAMMURI
// Date:    16/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03Maille.h"

// on inclut les définitions nécessaires
#include "float.h"

////////////////////
// les constructeurs

JMTFR03Maille::JKey::JKey()
{
	// on ne fait rien
}

JMTFR03Maille::JKey::JKey(const JKey & Source)
{
	// on vérifie la validité de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdTitre    = Source.m_IdTitre;
	m_IdParution = Source.m_IdParution;
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03Maille::JKey & JMTFR03Maille::JKey::operator =(const JKey & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdTitre    = Source.m_IdTitre;
		m_IdParution = Source.m_IdParution;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03Maille::JKey::operator <(const JKey & Reference) const
{
	// on compare la première composante
	if (m_IdTitre < Reference.m_IdTitre) return (true);
	if (m_IdTitre > Reference.m_IdTitre) return (false);

	// on compare la dernière composante
	return (m_IdParution < Reference.m_IdParution);
}

JBool JMTFR03Maille::JKey::operator >(const JKey & Reference) const
{
	// on compare la première composante
	if (m_IdTitre > Reference.m_IdTitre) return (true);
	if (m_IdTitre < Reference.m_IdTitre) return (false);

	// on compare la dernière composante
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
	// on vérifie la validité de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_NbInsertions = Source.m_NbInsertions;
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03Maille::JDat & JMTFR03Maille::JDat::operator =(const JDat & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_NbInsertions = Source.m_NbInsertions;
	}

	// on renvoie notre référence
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
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03Maille & JMTFR03Maille::operator =(const JMTFR03Maille & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03Maille::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JVoid JMTFR03Maille::Add(const JRef32 & IdTitre, const JRef32 & IdParution, const JRef32 & IdTuteur, JInt32 NbInsertions, JFlt64 GRPRef, JBool Move)
{
	// on crée la clé
	JKey Key;

	// on vérifie la validité de l'élément
	if (::_finite(GRPRef) == 0) throw JInvalidCall::GetInstance();

	// on vérifie la validité de l'élément
	if (!IdTitre.IsValid() || !IdParution.IsValid() || !IdTuteur.IsValid() || GRPRef < 0.0 || NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on initialise la clé
	Key.m_IdTitre = IdTitre; Key.m_IdParution = IdParution;

	// on ajoute le nouvel élément
	JDat & Dat = m_TBLItems.Add(Key, Move);

	// on initialise l'élément
	Dat.m_IdTuteur     = IdTuteur;
	Dat.m_NbInsertions = NbInsertions;
	Dat.m_GRPRef       = GRPRef;
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03Maille::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLItems.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JRef32 & JMTFR03Maille::GetIdTitre() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey().m_IdTitre);
}

const JRef32 & JMTFR03Maille::GetIdParution() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetKey().m_IdParution);
}

const JRef32 & JMTFR03Maille::GetIdTuteur() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem().m_IdTuteur);
}

const JInt32x & JMTFR03Maille::GetNbInsertions() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem().m_NbInsertions);
}

const JFlt64x & JMTFR03Maille::GetGRPRef() const
{
	// on renvoie l'élément du curseur
	return (m_TBLItems.GetItem().m_GRPRef);
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03Maille::MoveTo(const JRef32 & IdTitre, const JRef32 & IdParution, JInt32 Move) const
{
	// on crée la clé
	JKey Key;

	// on vérifie la validité de la clé
	if (!IdTitre.IsValid() || !IdParution.IsValid()) throw JInvalidCall::GetInstance();

	// on initialise la clé
	Key.m_IdTitre = IdTitre; Key.m_IdParution = IdParution;

	// on déplace le curseur
	m_TBLItems.MoveTo(Key, Move);
}

JVoid JMTFR03Maille::MoveFirst() const
{
	// on déplace le curseur
	m_TBLItems.MoveFirst();
}

JVoid JMTFR03Maille::MoveNext() const
{
	// on déplace le curseur
	m_TBLItems.MoveNext();
}

JVoid JMTFR03Maille::MovePrev() const
{
	// on déplace le curseur
	m_TBLItems.MovePrev();
}

JVoid JMTFR03Maille::MoveLast() const
{
	// on déplace le curseur
	m_TBLItems.MoveLast();
}

//////////////////////////////////////////////////////
// la fonction pour mettre à jour l'élément du curseur

JVoid JMTFR03Maille::Update(JInt32 NbInsertions)
{
	// on vérifie la validité du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on récupère l'élément du curseur
	JDat & Dat = m_TBLItems.GetItem();

	// on met à jour l'élément
	Dat.m_NbInsertions = NbInsertions;
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

JVoid JMTFR03Maille::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLItems.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03Maille::Swap(JMTFR03Maille & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03Maille::Reset()
{
	// on réinitialise les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03Maille::~JMTFR03Maille()
{
	// on ne fait rien
}
