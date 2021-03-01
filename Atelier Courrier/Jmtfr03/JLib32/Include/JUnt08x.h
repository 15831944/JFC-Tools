//
// Fichier: JUnt08x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_UNT08X_H
#define JLIB_UNT08X_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JUnt08x
{
public:
	// les constructeurs
	JUnt08x();
	JUnt08x(JUnt32 Source);
	JUnt08x(const JUnt08x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JUnt32 Min, JUnt32 Max) const;

	// les fonctions pour sérialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour récupérer la valeur
	JInt32 AsJInt32() const;
	JUnt32 AsJUnt32() const;

	// les opérateurs d'affectation
	JUnt08x & operator =(JUnt32 Source);
	JUnt08x & operator =(const JUnt08x & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JUnt08x & Reference) const;
	JBool operator !=(const JUnt08x & Reference) const;
	JBool operator < (const JUnt08x & Reference) const;
	JBool operator <=(const JUnt08x & Reference) const;
	JBool operator > (const JUnt08x & Reference) const;
	JBool operator >=(const JUnt08x & Reference) const;

	// la fonction pour réinitialiser la valeur
	JVoid Reset();

	// le destructeur
	~JUnt08x();

private:
	JUnt08 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
