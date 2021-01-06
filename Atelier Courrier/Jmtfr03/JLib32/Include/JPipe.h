//
// Fichier: JPipe.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_PIPE_H
#define JLIB_PIPE_H

// on inclut les définitions nécessaires
#include "..\Include\JSerialize.h"

// on inclut les définitions nécessaires
#include "..\Include\JPath.h"

// on inclut les définitions nécessaires
#include "..\Include\JHost.h"

class JPipe : public JSerialize
{
private:
	// le constructeur
	JPipe();

public:
	// la fonction pour créer une instance
	static JPipe* Create(const JHost & Host, const JPath & Path);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// les fonctions pour transférer les données
	virtual JVoid  Send(JUnt08 Data);
	virtual JUnt08 Recv();

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JPipe();

private:
	// l'opérateur d'affectation
	JPipe & operator =(const JPipe & Source);

	// le constructeur copie
	JPipe(const JPipe & Source);
};

// on finit l'inclusion conditionnelle
#endif
