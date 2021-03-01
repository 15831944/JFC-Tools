//
// Fichier: JLexeme.h
// Auteur:  Sylvain SAMMURI
// Date:    14/08/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_LEXEME_H
#define JLIB_LEXEME_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JLimits.h"

class JLexeme
{
public:
	// les constructeurs
	JLexeme();
	JLexeme(const JLexeme & Source);

	// les fonctions pour tester le lexème
	JInt32 GetType() const;
	JInt32 GetLength() const;

	// la fonction pour remplir le lexème
	JVoid Add(JChar Source, JInt32 Type = 0);

	// la fonction pour récupérer le lexème
	const JChar* AsJCharPtr() const;

	// l'opérateur d'affectation
	JLexeme & operator =(const JLexeme & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JLexeme & Reference) const;
	JBool operator !=(const JLexeme & Reference) const;

	// la fonction pour réinitialiser le lexème
	JVoid Reset();

	// le destructeur
	~JLexeme();

private:
	JChar  m_Lexeme[JLEXEME_MAX]; // le lexème
	JInt32 m_Length;              // la longueur
	JInt32 m_Type;                // le type
};

// on finit l'inclusion conditionnelle
#endif
