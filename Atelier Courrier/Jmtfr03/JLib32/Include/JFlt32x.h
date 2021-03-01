//
// Fichier: JFlt32x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_FLT32X_H
#define JLIB_FLT32X_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JFlt32x
{
public:
	// les constructeurs
	JFlt32x();
	JFlt32x(JFlt64 Source);
	JFlt32x(const JFlt32x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JFlt64 Min, JFlt64 Max) const;

	// les fonctions pour sérialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer la valeur
	JFlt64 AsJFlt64() const;

	// les opérateurs d'affectation
	JFlt32x & operator =(JFlt64 Source);
	JFlt32x & operator =(const JFlt32x & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JFlt32x & Reference) const;
	JBool operator !=(const JFlt32x & Reference) const;
	JBool operator < (const JFlt32x & Reference) const;
	JBool operator <=(const JFlt32x & Reference) const;
	JBool operator > (const JFlt32x & Reference) const;
	JBool operator >=(const JFlt32x & Reference) const;

	// la fonction pour réinitialiser la valeur
	JVoid Reset();

	// le destructeur
	~JFlt32x();

private:
	JFlt32 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
