//
// Fichier: JTimeout.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_TIMEOUT_H
#define JLIB_TIMEOUT_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JException.h"

class JTimeout : public JException
{
private:
	// le constructeur
	JTimeout();

private:
	// l'unique instance de l'exception
	static JTimeout m_Instance;

public:
	// la fonction pour r�cup�rer l'instance de l'exception
	static JTimeout* GetInstance();

public:
	// la fonction pour r�cup�rer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JTimeout();

private:
	// l'op�rateur d'affectation
	JTimeout & operator =(const JTimeout & Source);

	// le constructeur copie
	JTimeout(const JTimeout & Source);
};

// on finit l'inclusion conditionnelle
#endif
