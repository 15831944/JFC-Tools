//
// Fichier: JFlt64x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_FLT64X_H
#define JLIB_FLT64X_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JFlt64x
{
public:
	// les constructeurs
	JFlt64x();
	JFlt64x(JFlt64 Source);
	JFlt64x(const JFlt64x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JFlt64 Min, JFlt64 Max) const;

	// les fonctions pour sérialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer la valeur
	JFlt64 AsJFlt64() const;

	// les opérateurs d'affectation
	JFlt64x & operator =(JFlt64 Source);
	JFlt64x & operator =(const JFlt64x & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JFlt64x & Reference) const;
	JBool operator !=(const JFlt64x & Reference) const;
	JBool operator < (const JFlt64x & Reference) const;
	JBool operator <=(const JFlt64x & Reference) const;
	JBool operator > (const JFlt64x & Reference) const;
	JBool operator >=(const JFlt64x & Reference) const;

	// la fonction pour réinitialiser la valeur
	JVoid Reset();

	// le destructeur
	~JFlt64x();

private:
	JFlt64 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
