//
// Fichier: JInt08x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_INT08X_H
#define JLIB_INT08X_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JInt08x
{
public:
	// les constructeurs
	JInt08x();
	JInt08x(JInt32 Source);
	JInt08x(const JInt08x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JInt32 Min, JInt32 Max) const;

	// les fonctions pour sérialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour récupérer la valeur
	JInt32 AsJInt32() const;
	JUnt32 AsJUnt32() const;

	// les opérateurs d'affectation
	JInt08x & operator =(JInt32 Source);
	JInt08x & operator =(const JInt08x & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JInt08x & Reference) const;
	JBool operator !=(const JInt08x & Reference) const;
	JBool operator < (const JInt08x & Reference) const;
	JBool operator <=(const JInt08x & Reference) const;
	JBool operator > (const JInt08x & Reference) const;
	JBool operator >=(const JInt08x & Reference) const;

	// la fonction pour réinitialiser la valeur
	JVoid Reset();

	// le destructeur
	~JInt08x();

private:
	JInt08 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
