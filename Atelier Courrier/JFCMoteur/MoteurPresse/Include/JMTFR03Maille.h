//
// Fichier: JMTFR03Maille.h
// Auteur:  Sylvain SAMMURI
// Date:    16/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_MAILLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_MAILLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03Maille
{
public:
	// le constructeur
	JMTFR03Maille();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JVoid Add(const JRef32 & IdTitre, const JRef32 & IdParution, const JRef32 & IdTuteur, JInt32 NbInsertions, JFlt64 GRPRef, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JRef32  & GetIdTitre() const;
	const JRef32  & GetIdParution() const;
	const JRef32  & GetIdTuteur() const;
	const JInt32x & GetNbInsertions() const;
	const JFlt64x & GetGRPRef() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JRef32 & IdTitre, const JRef32 & IdParution, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour mettre � jour l'�l�ment du curseur
	JVoid Update(JInt32 NbInsertions);

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03Maille & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03Maille();

private:
	// on d�finit la cl�
	class JKey
	{
	public:
		// les constructeurs
		JKey();
		JKey(const JKey & Source);

		// l'op�rateur pour recopier les �l�ments
		JKey & operator =(const JKey & Source);

		// les op�rateurs pour comparer les �l�ments
		JBool operator <(const JKey & Reference) const;
		JBool operator >(const JKey & Reference) const;

		// le destructeur
		~JKey();

	public:
		JRef32 m_IdTitre;    // l'identifiant du titre
		JRef32 m_IdParution; // l'identifiant de la parution
	};

private:
	// on d�finit les donn�es
	class JDat
	{
	public:
		// les constructeurs
		JDat();
		JDat(const JDat & Source);

		// l'op�rateur pour recopier les �l�ments
		JDat & operator =(const JDat & Source);

		// le destructeur
		~JDat();

	public:
		JRef32  m_IdTuteur;     // l'identifiant du tuteur
		JInt32x m_NbInsertions; // le nombre d'insertions
		JFlt64x m_GRPRef;       // le GRP de r�f�rence
	};

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03Maille & operator =(const JMTFR03Maille & Source);

	// le constructeur copie
	JMTFR03Maille(const JMTFR03Maille & Source);

private:
	JMap<JKey, JDat> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
