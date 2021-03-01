//
// Fichier: JMTFR03Exception.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_EXCEPTION_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_EXCEPTION_H

// on inclut les d�finitions n�cessaires
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
	// la fonction pour r�cup�rer l'instance de l'exception
	static JMTFR03Exception* GetInstance();

public:
	// la fonction pour r�cup�rer le message de l'exception
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JMTFR03Exception();

private:
	// l'op�rateur pour recopier l'exception
	JMTFR03Exception & operator =(const JMTFR03Exception & Source);

	// le constructeur copie
	JMTFR03Exception(const JMTFR03Exception & Source);
};

// fin de l'inclusion conditionnelle
#endif
