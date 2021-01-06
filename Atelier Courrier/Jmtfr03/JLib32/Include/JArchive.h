//
// Fichier: JArchive.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_ARCHIVE_H
#define JLIB_ARCHIVE_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JSerialize.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JFile.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JPath.h"

class JArchive : public JStream
{
private:
	// le constructeur
	JArchive(JSerialize* pPrev, JSerialize* pNext);

public:
	// la fonction pour cr�er une instance
	static JArchive* Create(const JPath & Path, JBool Update = false);

	// le destructeur
	virtual ~JArchive();

private:
	// l'op�rateur d'affectation
	JArchive & operator =(const JArchive & Source);

	// le constructeur copie
	JArchive(const JArchive & Source);

private:
	JFile* m_pFile; // le fichier
};

// on finit l'inclusion conditionnelle
#endif
