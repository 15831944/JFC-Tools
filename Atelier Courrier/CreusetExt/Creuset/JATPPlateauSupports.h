//
// Fichier: JATPPlateauSupports.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

#pragma once

class CKeySupport
{
public:

	bool operator<(const CKeySupport & Rhs) const
	{
		if(m_IdSupport < Rhs.m_IdSupport)
			return true;
		if(m_IdSupport == Rhs.m_IdSupport && m_IdParent < Rhs.m_IdParent)
			return true;
		return false;
	};

	bool operator>(const CKeySupport & Rhs) const
	{
		if(m_IdSupport > Rhs.m_IdSupport)
			return true;
		if(m_IdSupport == Rhs.m_IdSupport && m_IdParent > Rhs.m_IdParent)
			return true;
		return false;
	};

	bool operator==(const CKeySupport & Rhs) const
	{
		return (m_IdSupport == Rhs.m_IdSupport && m_IdParent == Rhs.m_IdParent);
	};

	CKeySupport & operator=(const CKeySupport & Rhs)
	{
		m_IdSupport = Rhs.m_IdSupport;
		m_IdParent  = Rhs.m_IdParent;
		return *this;
	};

	void Send(JStream & Stream) const
	{
		// on sérialise l'élément
		Stream.Send(m_IdSupport);
		Stream.Send(m_IdParent);
	};

	void Recv(JStream & Stream)
	{
		// Récup élmt id support et id parent support
		JUnt32 IdSupport;
		Stream.Recv(IdSupport);
		JUnt32 IdParent;
		Stream.Recv(IdParent);

		// on recopie les éléments
		m_IdSupport       = IdSupport;
		m_IdParent        = IdParent;
	};

	CKeySupport() { m_IdSupport = 0; m_IdParent = 0; };
	JUnt32	m_IdSupport;
	JUnt32	m_IdParent;
};

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPPlateauSupports
{
public:
	// le constructeur
	JATPPlateauSupports();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JInt32 Add(CKeySupport & KeySupport);

	// la fonction pour définir les éléments
	JVoid SetKeySupports(const JList<CKeySupport> & ListKeySupport);

	// la fonction pour rechercher un élément
	JInt32 FindItem(CKeySupport & KeySupport) const;

	// les fonctions pour ordonner les éléments
	JVoid MoveUp(JInt32 Index);
	JVoid MoveDn(JInt32 Index);

	// la fonction pour récupérer les éléments
	const CKeySupport & Item(JInt32 Index) const;

	// les fonctions pour sérialiser les éléments
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour supprimer les éléments
	JVoid Remove(JInt32 Index);

	// la fonction pour permuter les éléments
	JVoid Swap(JATPPlateauSupports & Source);

	// le destructeur
	~JATPPlateauSupports();

private:
	// l'opérateur pour recopier l'élément
	JATPPlateauSupports & operator =(const JATPPlateauSupports & Source);

	// le constructeur copie
	JATPPlateauSupports(const JATPPlateauSupports & Source);

private:
	JArray<CKeySupport> m_KeySupports; // les Supports
};

