//
// Fichier: JNetworkError.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_NETWORKERROR_H
#define JLIB_NETWORKERROR_H

// on inclut les définitions nécessaires
#include "..\Include\JException.h"

class JNetworkError : public JException
{
private:
	// le constructeur
	JNetworkError();

private:
	// l'unique instance de l'exception
	static JNetworkError m_Instance;

public:
	// la fonction pour récupérer l'instance de l'exception
	static JNetworkError* GetInstance();

public:
	// la fonction pour récupérer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JNetworkError();

private:
	// l'opérateur d'affectation
	JNetworkError & operator =(const JNetworkError & Source);

	// le constructeur copie
	JNetworkError(const JNetworkError & Source);
};

// on finit l'inclusion conditionnelle
#endif
