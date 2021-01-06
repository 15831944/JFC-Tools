//
// Fichier: JMTFR03Exception.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_EXCEPTION_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_EXCEPTION_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03Exception : public JException
{
private:
	// le constructeur
	JMTFR03Exception();

private:
	// l'unique instance de l'exception
	static JMTFR03Exception m_Instance;

public:
	// la fonction pour récupérer l'instance de l'exception
	static JMTFR03Exception* GetInstance();

public:
	// la fonction pour récupérer le message de l'exception
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JMTFR03Exception();

private:
	// l'opérateur pour recopier l'exception
	JMTFR03Exception & operator =(const JMTFR03Exception & Source);

	// le constructeur copie
	JMTFR03Exception(const JMTFR03Exception & Source);
};

// fin de l'inclusion conditionnelle
#endif
