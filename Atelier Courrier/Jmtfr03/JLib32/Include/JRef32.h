//
// Fichier: JRef32.h
// Auteur:  Sylvain SAMMURI
// Date:    31/10/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_REF32_H
#define JLIB_REF32_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JRef32
{
public:
	// les constructeurs
	JRef32();
	JRef32(JInt32 Reference);
	JRef32(const JRef32 & Source);

	// la fonction pour tester la r�f�rence
	JBool IsValid() const;

	// la fonction pour r�cup�rer la cl� de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour s�rialiser la r�f�rence
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour r�cup�rer la r�f�rence
	JInt32 AsJInt32() const;

	// les op�rateurs d'affectation
	JRef32 & operator =(JInt32 Reference);
	JRef32 & operator =(const JRef32 & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JRef32 & Reference) const;
	JBool operator !=(const JRef32 & Reference) const;
	JBool operator < (const JRef32 & Reference) const;
	JBool operator <=(const JRef32 & Reference) const;
	JBool operator > (const JRef32 & Reference) const;
	JBool operator >=(const JRef32 & Reference) const;

	// la fonction pour r�initialiser la r�f�rence
	JVoid Reset();

	// le destructeur
	~JRef32();

public:
	// les fonctions pour r�cup�rer les bornes
	static JRef32 GetMin();
	static JRef32 GetMax();

private:
	JUnt32 m_Reference; // la r�f�rence
};

// on finit l'inclusion conditionnelle
#endif
