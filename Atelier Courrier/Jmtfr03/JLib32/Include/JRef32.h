//
// Fichier: JRef32.h
// Auteur:  Sylvain SAMMURI
// Date:    31/10/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_REF32_H
#define JLIB_REF32_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JRef32
{
public:
	// les constructeurs
	JRef32();
	JRef32(JInt32 Reference);
	JRef32(const JRef32 & Source);

	// la fonction pour tester la référence
	JBool IsValid() const;

	// la fonction pour récupérer la clé de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour sérialiser la référence
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer la référence
	JInt32 AsJInt32() const;

	// les opérateurs d'affectation
	JRef32 & operator =(JInt32 Reference);
	JRef32 & operator =(const JRef32 & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JRef32 & Reference) const;
	JBool operator !=(const JRef32 & Reference) const;
	JBool operator < (const JRef32 & Reference) const;
	JBool operator <=(const JRef32 & Reference) const;
	JBool operator > (const JRef32 & Reference) const;
	JBool operator >=(const JRef32 & Reference) const;

	// la fonction pour réinitialiser la référence
	JVoid Reset();

	// le destructeur
	~JRef32();

public:
	// les fonctions pour récupérer les bornes
	static JRef32 GetMin();
	static JRef32 GetMax();

private:
	JUnt32 m_Reference; // la référence
};

// on finit l'inclusion conditionnelle
#endif
