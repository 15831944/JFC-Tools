//
// Fichier: JUnt16x.h
// Auteur:  Sylvain SAMMURI
// Date:    10/07/2003
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_UNT16X_H
#define JLIB_UNT16X_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JUnt16x
{
public:
	// les constructeurs
	JUnt16x();
	JUnt16x(JUnt32 Source);
	JUnt16x(const JUnt16x & Source);

	// la fonction pour tester la valeur
	JBool IsBetween(JUnt32 Min, JUnt32 Max) const;

	// les fonctions pour s�rialiser la valeur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour r�cup�rer la valeur
	JInt32 AsJInt32() const;
	JUnt32 AsJUnt32() const;

	// les op�rateurs d'affectation
	JUnt16x & operator =(JUnt32 Source);
	JUnt16x & operator =(const JUnt16x & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JUnt16x & Reference) const;
	JBool operator !=(const JUnt16x & Reference) const;
	JBool operator < (const JUnt16x & Reference) const;
	JBool operator <=(const JUnt16x & Reference) const;
	JBool operator > (const JUnt16x & Reference) const;
	JBool operator >=(const JUnt16x & Reference) const;

	// la fonction pour r�initialiser la valeur
	JVoid Reset();

	// le destructeur
	~JUnt16x();

private:
	JUnt16 m_Value; // la valeur
};

// on finit l'inclusion conditionnelle
#endif
