//
// Fichier: JConnection.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_CONNECTION_H
#define JLIB_CONNECTION_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JSerialize.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JPipe.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JHost.h"
#include "..\Include\JPath.h"

class JConnection : public JStream
{
private:
	// le constructeur
	JConnection(JSerialize* pPrev, JSerialize* pNext);

public:
	// la fonction pour cr�er une instance
	static JConnection* Create(const JHost & Host, const JPath & Path);

	// le destructeur
	virtual ~JConnection();

private:
	// l'op�rateur d'affectation
	JConnection & operator =(const JConnection & Source);

	// le constructeur copie
	JConnection(const JConnection & Source);

private:
	JPipe* m_pPipe; // le tube
};

// on finit l'inclusion conditionnelle
#endif
