//
// Fichier: JInt16x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_INT16X_H
#define JLIB_INT16X_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JInt16x
{
public:
	// les constructeurs
	JInt16x();
	JInt16x(JInt32 Source);
	JInt16x(const JInt16x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JInt32 Min, JInt32 Max) const;

	// les fonctions pour s�rialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour r�cup�rer la valeur
	JInt32 AsJInt32() const;
	JUnt32 AsJUnt32() const;

	// les op�rateurs d'affectation
	JInt16x & operator =(JInt32 Source);
	JInt16x & operator =(const JInt16x & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JInt16x & Reference) const;
	JBool operator !=(const JInt16x & Reference) const;
	JBool operator < (const JInt16x & Reference) const;
	JBool operator <=(const JInt16x & Reference) const;
	JBool operator > (const JInt16x & Reference) const;
	JBool operator >=(const JInt16x & Reference) const;

	// la fonction pour r�initialiser la valeur
	JVoid Reset();

	// le destructeur
	~JInt16x();

private:
	JInt16 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
