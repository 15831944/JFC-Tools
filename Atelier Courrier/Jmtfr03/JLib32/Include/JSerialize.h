//
// Fichier: JSerialize.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_SERIALIZE_H
#define JLIB_SERIALIZE_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

class JSerialize
{
protected:
	// le constructeur
	JSerialize();

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty() = 0;

	// les fonctions pour transférer les données
	virtual JVoid  Send(JUnt08 Data) = 0;
	virtual JUnt08 Recv() = 0;

	// la fonction pour vider le tampon
	virtual JVoid Flush() = 0;

	// le destructeur
	virtual ~JSerialize();

private:
	// l'opérateur d'affectation
	JSerialize & operator =(const JSerialize & Source);

	// le constructeur copie
	JSerialize(const JSerialize & Source);
};

// on finit l'inclusion conditionnelle
#endif
