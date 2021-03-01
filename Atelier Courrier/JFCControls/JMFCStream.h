//============================
// fichier: JMFCStream.h
// 
// date: 21/02/2003
// auteur: JB
//----------------------------

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JMFCSTREAM_H_

// on d�finit une macro pour identifier le fichier
#define _JMFCSTREAM_H_

// on inclut les fichiers n�cessaires
#include "JMFCSerialize.h"

class AFX_EXT_CLASS JMFCStream : public JStream
{
private:
	// le constructeur
	JMFCStream(JMFCSerialize * pPrev, JMFCSerialize * pNext);

public:
	// la fonction pour cr�er une instance
	static JMFCStream* Create(CArchive & ar);

	// le destructeur
	virtual ~JMFCStream();

	// R�cup�ration �lmt type CFile
	CFile * GetCFile();

	// R�cup�ration �lmt type CArchive
	CArchive * GetCArchive();

private:
	// l'op�rateur d'affectation
	JMFCStream & operator =(const JMFCStream & Source);

	// le constructeur copie
	JMFCStream(const JMFCStream & Source);

private:
	JMFCSerialize* m_pSerialize; // le flot
};
// fin du test sur la macro
#endif
