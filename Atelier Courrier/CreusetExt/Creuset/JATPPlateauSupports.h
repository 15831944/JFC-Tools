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
		// on s�rialise l'�l�ment
		Stream.Send(m_IdSupport);
		Stream.Send(m_IdParent);
	};

	void Recv(JStream & Stream)
	{
		// R�cup �lmt id support et id parent support
		JUnt32 IdSupport;
		Stream.Recv(IdSupport);
		JUnt32 IdParent;
		Stream.Recv(IdParent);

		// on recopie les �l�ments
		m_IdSupport       = IdSupport;
		m_IdParent        = IdParent;
	};

	CKeySupport() { m_IdSupport = 0; m_IdParent = 0; };
	JUnt32	m_IdSupport;
	JUnt32	m_IdParent;
};

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPPlateauSupports
{
public:
	// le constructeur
	JATPPlateauSupports();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JInt32 Add(CKeySupport & KeySupport);

	// la fonction pour d�finir les �l�ments
	JVoid SetKeySupports(const JList<CKeySupport> & ListKeySupport);

	// la fonction pour rechercher un �l�ment
	JInt32 FindItem(CKeySupport & KeySupport) const;

	// les fonctions pour ordonner les �l�ments
	JVoid MoveUp(JInt32 Index);
	JVoid MoveDn(JInt32 Index);

	// la fonction pour r�cup�rer les �l�ments
	const CKeySupport & Item(JInt32 Index) const;

	// les fonctions pour s�rialiser les �l�ments
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour supprimer les �l�ments
	JVoid Remove(JInt32 Index);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JATPPlateauSupports & Source);

	// le destructeur
	~JATPPlateauSupports();

private:
	// l'op�rateur pour recopier l'�l�ment
	JATPPlateauSupports & operator =(const JATPPlateauSupports & Source);

	// le constructeur copie
	JATPPlateauSupports(const JATPPlateauSupports & Source);

private:
	JArray<CKeySupport> m_KeySupports; // les Supports
};

