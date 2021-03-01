//
// Fichier: JMTFR03Maille.h
// Auteur:  Sylvain SAMMURI
// Date:    16/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_MAILLE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_MAILLE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03Maille
{
public:
	// le constructeur
	JMTFR03Maille();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JVoid Add(const JRef32 & IdTitre, const JRef32 & IdParution, const JRef32 & IdTuteur, JInt32 NbInsertions, JFlt64 GRPRef, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JRef32  & GetIdTitre() const;
	const JRef32  & GetIdParution() const;
	const JRef32  & GetIdTuteur() const;
	const JInt32x & GetNbInsertions() const;
	const JFlt64x & GetGRPRef() const;

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JRef32 & IdTitre, const JRef32 & IdParution, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour mettre à jour l'élément du curseur
	JVoid Update(JInt32 NbInsertions);

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03Maille & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03Maille();

private:
	// on définit la clé
	class JKey
	{
	public:
		// les constructeurs
		JKey();
		JKey(const JKey & Source);

		// l'opérateur pour recopier les éléments
		JKey & operator =(const JKey & Source);

		// les opérateurs pour comparer les éléments
		JBool operator <(const JKey & Reference) const;
		JBool operator >(const JKey & Reference) const;

		// le destructeur
		~JKey();

	public:
		JRef32 m_IdTitre;    // l'identifiant du titre
		JRef32 m_IdParution; // l'identifiant de la parution
	};

private:
	// on définit les données
	class JDat
	{
	public:
		// les constructeurs
		JDat();
		JDat(const JDat & Source);

		// l'opérateur pour recopier les éléments
		JDat & operator =(const JDat & Source);

		// le destructeur
		~JDat();

	public:
		JRef32  m_IdTuteur;     // l'identifiant du tuteur
		JInt32x m_NbInsertions; // le nombre d'insertions
		JFlt64x m_GRPRef;       // le GRP de référence
	};

private:
	// l'opérateur pour recopier les éléments
	JMTFR03Maille & operator =(const JMTFR03Maille & Source);

	// le constructeur copie
	JMTFR03Maille(const JMTFR03Maille & Source);

private:
	JMap<JKey, JDat> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
