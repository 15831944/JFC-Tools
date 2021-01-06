//
// Fichier: JInvalidCall.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_INVALIDCALL_H
#define JLIB_INVALIDCALL_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JException.h"

class JInvalidCall : public JException
{
private:
	// le constructeur
	JInvalidCall();

private:
	// l'unique instance de l'exception
	static JInvalidCall m_Instance;

public:
	// la fonction pour r�cup�rer l'instance de l'exception
	static JInvalidCall* GetInstance();

public:
	// la fonction pour r�cup�rer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JInvalidCall();

private:
	// l'op�rateur d'affectation
	JInvalidCall & operator =(const JInvalidCall & Source);

	// le constructeur copie
	JInvalidCall(const JInvalidCall & Source);
};

// on finit l'inclusion conditionnelle
#endif
