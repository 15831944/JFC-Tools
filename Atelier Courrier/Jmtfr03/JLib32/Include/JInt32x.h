//
// Fichier: JInt32x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_INT32X_H
#define JLIB_INT32X_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JInt32x
{
public:
	// les constructeurs
	JInt32x();
	JInt32x(JInt32 Source);
	JInt32x(const JInt32x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JInt32 Min, JInt32 Max) const;

	// les fonctions pour sérialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour récupérer la valeur
	JInt32 AsJInt32() const;
	JUnt32 AsJUnt32() const;

	// les opérateurs d'affectation
	JInt32x & operator =(JInt32 Source);
	JInt32x & operator =(const JInt32x & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JInt32x & Reference) const;
	JBool operator !=(const JInt32x & Reference) const;
	JBool operator < (const JInt32x & Reference) const;
	JBool operator <=(const JInt32x & Reference) const;
	JBool operator > (const JInt32x & Reference) const;
	JBool operator >=(const JInt32x & Reference) const;

	// la fonction pour réinitialiser la valeur
	JVoid Reset();

	// le destructeur
	~JInt32x();

private:
	JInt32 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
