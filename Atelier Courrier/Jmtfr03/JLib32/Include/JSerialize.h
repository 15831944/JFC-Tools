//
// Fichier: JSerialize.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_SERIALIZE_H
#define JLIB_SERIALIZE_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

class JSerialize
{
protected:
	// le constructeur
	JSerialize();

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty() = 0;

	// les fonctions pour transf�rer les donn�es
	virtual JVoid  Send(JUnt08 Data) = 0;
	virtual JUnt08 Recv() = 0;

	// la fonction pour vider le tampon
	virtual JVoid Flush() = 0;

	// le destructeur
	virtual ~JSerialize();

private:
	// l'op�rateur d'affectation
	JSerialize & operator =(const JSerialize & Source);

	// le constructeur copie
	JSerialize(const JSerialize & Source);
};

// on finit l'inclusion conditionnelle
#endif
