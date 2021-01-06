//================================
// fichier: JMFCSerialize.h
//
// date: 21/02/2003
// auteur: JB
//--------------------------------

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JMFCSERIALIZE_H_

// on définit une macro pour identifier le fichier
#define _JMFCSERIALIZE_H_

// on inclut les fichiers nécessaires
class JMFCSerialize : public JSerialize
{
	// le constructeur
	JMFCSerialize(CArchive & ar);

public:
	// la fonction de création d'un JMFCSerialize
	static JMFCSerialize * Create(CArchive & ar);

	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// les fonctions pour transférer les données
	virtual JVoid  Send(JUnt08 Data);
	virtual JUnt08 Recv();

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JMFCSerialize();

	CArchive & GetCArchive();

private:
	// l'opérateur d'affectation
	JMFCSerialize & operator =(const JMFCSerialize & Source);

	// le constructeur copie
	JMFCSerialize(const JMFCSerialize & Source);

protected:

	// la référence sur l'archive
	CArchive & m_Archive;
	// le tampon
	JUnt08 m_Buffer;

	// le compteur
	JInt32 m_BufferCount;
};
// fin du test sur la macro
#endif