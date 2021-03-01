//
// Fichier: JConnection.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_CONNECTION_H
#define JLIB_CONNECTION_H

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

// on inclut les définitions nécessaires
#include "..\Include\JSerialize.h"

// on inclut les définitions nécessaires
#include "..\Include\JPipe.h"

// on inclut les définitions nécessaires
#include "..\Include\JHost.h"
#include "..\Include\JPath.h"

class JConnection : public JStream
{
private:
	// le constructeur
	JConnection(JSerialize* pPrev, JSerialize* pNext);

public:
	// la fonction pour créer une instance
	static JConnection* Create(const JHost & Host, const JPath & Path);

	// le destructeur
	virtual ~JConnection();

private:
	// l'opérateur d'affectation
	JConnection & operator =(const JConnection & Source);

	// le constructeur copie
	JConnection(const JConnection & Source);

private:
	JPipe* m_pPipe; // le tube
};

// on finit l'inclusion conditionnelle
#endif
