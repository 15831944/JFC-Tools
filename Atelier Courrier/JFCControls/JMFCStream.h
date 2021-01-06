//============================
// fichier: JMFCStream.h
// 
// date: 21/02/2003
// auteur: JB
//----------------------------

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JMFCSTREAM_H_

// on définit une macro pour identifier le fichier
#define _JMFCSTREAM_H_

// on inclut les fichiers nécessaires
#include "JMFCSerialize.h"

class AFX_EXT_CLASS JMFCStream : public JStream
{
private:
	// le constructeur
	JMFCStream(JMFCSerialize * pPrev, JMFCSerialize * pNext);

public:
	// la fonction pour créer une instance
	static JMFCStream* Create(CArchive & ar);

	// le destructeur
	virtual ~JMFCStream();

	// Récupération élmt type CFile
	CFile * GetCFile();

	// Récupération élmt type CArchive
	CArchive * GetCArchive();

private:
	// l'opérateur d'affectation
	JMFCStream & operator =(const JMFCStream & Source);

	// le constructeur copie
	JMFCStream(const JMFCStream & Source);

private:
	JMFCSerialize* m_pSerialize; // le flot
};
// fin du test sur la macro
#endif
