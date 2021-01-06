//
// Fichier: JMTFR03Exception.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2002
//

// on inclut les définitions nécessaires
#include "JMTFR03Exception.h"

///////////////////////////////////
// l'unique instance de l'exception

JMTFR03Exception JMTFR03Exception::m_Instance;

////////////////////
// les constructeurs

JMTFR03Exception::JMTFR03Exception()
{
	// on ne fait rien
}

JMTFR03Exception::JMTFR03Exception(const JMTFR03Exception & Source)
{
	// on ne fait rien
}

////////////////////////////////////////
// l'opérateur pour recopier l'exception

JMTFR03Exception & JMTFR03Exception::operator =(const JMTFR03Exception & Source)
{
	// on renvoie notre référence
	return (*this);
}

///////////////////////////////////////////////////////
// la fonction pour récupérer l'instance de l'exception

JMTFR03Exception* JMTFR03Exception::GetInstance()
{
	// on renvoie l'instance de l'exception
	return (&m_Instance);
}

///////////////////////////////////////////////////////
// la fonction pour récupérer le message de l'exception

const JChar* JMTFR03Exception::GetMessage() const
{
	// on renvoie le message de l'exception
	return ("Exception : MTFR03 Error.");
}

/////////////////
// le destructeur

JMTFR03Exception::~JMTFR03Exception()
{
	// on ne fait rien
}
