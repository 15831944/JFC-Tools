//
// Fichier: JLexeme.h
// Auteur:  Sylvain SAMMURI
// Date:    14/08/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_LEXEME_H
#define JLIB_LEXEME_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JLimits.h"

class JLexeme
{
public:
	// les constructeurs
	JLexeme();
	JLexeme(const JLexeme & Source);

	// les fonctions pour tester le lex�me
	JInt32 GetType() const;
	JInt32 GetLength() const;

	// la fonction pour remplir le lex�me
	JVoid Add(JChar Source, JInt32 Type = 0);

	// la fonction pour r�cup�rer le lex�me
	const JChar* AsJCharPtr() const;

	// l'op�rateur d'affectation
	JLexeme & operator =(const JLexeme & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JLexeme & Reference) const;
	JBool operator !=(const JLexeme & Reference) const;

	// la fonction pour r�initialiser le lex�me
	JVoid Reset();

	// le destructeur
	~JLexeme();

private:
	JChar  m_Lexeme[JLEXEME_MAX]; // le lex�me
	JInt32 m_Length;              // la longueur
	JInt32 m_Type;                // le type
};

// on finit l'inclusion conditionnelle
#endif
