//
// Fichier: JPipe.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_PIPE_H
#define JLIB_PIPE_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JSerialize.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JPath.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JHost.h"

class JPipe : public JSerialize
{
private:
	// le constructeur
	JPipe();

public:
	// la fonction pour cr�er une instance
	static JPipe* Create(const JHost & Host, const JPath & Path);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// les fonctions pour transf�rer les donn�es
	virtual JVoid  Send(JUnt08 Data);
	virtual JUnt08 Recv();

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JPipe();

private:
	// l'op�rateur d'affectation
	JPipe & operator =(const JPipe & Source);

	// le constructeur copie
	JPipe(const JPipe & Source);
};

// on finit l'inclusion conditionnelle
#endif
