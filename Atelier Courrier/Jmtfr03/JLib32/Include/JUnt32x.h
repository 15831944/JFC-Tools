//
// Fichier: JUnt32x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_UNT32X_H
#define JLIB_UNT32X_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JUnt32x
{
public:
	// les constructeurs
	JUnt32x();
	JUnt32x(JUnt32 Source);
	JUnt32x(const JUnt32x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JUnt32 Min, JUnt32 Max) const;

	// les fonctions pour sérialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour récupérer la valeur
	JInt32 AsJInt32() const;
	JUnt32 AsJUnt32() const;

	// les opérateurs d'affectation
	JUnt32x & operator =(JUnt32 Source);
	JUnt32x & operator =(const JUnt32x & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JUnt32x & Reference) const;
	JBool operator !=(const JUnt32x & Reference) const;
	JBool operator < (const JUnt32x & Reference) const;
	JBool operator <=(const JUnt32x & Reference) const;
	JBool operator > (const JUnt32x & Reference) const;
	JBool operator >=(const JUnt32x & Reference) const;

	// la fonction pour réinitialiser la valeur
	JVoid Reset();

	// le destructeur
	~JUnt32x();

private:
	JUnt32 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
