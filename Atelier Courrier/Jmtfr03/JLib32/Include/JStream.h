//
// Fichier: JStream.h
// Auteur:  Sylvain SAMMURI
// Date:    17/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_STREAM_H
#define JLIB_STREAM_H

// on inclut les définitions nécessaires
#include "..\Include\JSerialize.h"

class JStream
{
protected:
	// le constructeur
	JStream(JSerialize* pPrev, JSerialize* pNext);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// les fonctions pour manipuler les versions
	virtual JInt32 SetVersion(JInt32 Version);
	virtual JInt32 GetVersion() const;

	// les fonctions pour transférer les booléens
	virtual JVoid Send(JBool   Value);
	virtual JVoid Recv(JBool & Value);

	// les fonctions pour transférer les caractères
	virtual JVoid Send(JChar   Value);
	virtual JVoid Recv(JChar & Value);

	// les fonctions pour transférer les entiers 8 bits
	virtual JVoid Send(JInt08   Value);
	virtual JVoid Send(JUnt08   Value);
	virtual JVoid Recv(JInt08 & Value);
	virtual JVoid Recv(JUnt08 & Value);

	// les fonctions pour transférer les entiers 16 bits
	virtual JVoid Send(JInt16   Value);
	virtual JVoid Send(JUnt16   Value);
	virtual JVoid Recv(JInt16 & Value);
	virtual JVoid Recv(JUnt16 & Value);

	// les fonctions pour transférer les entiers 32 bits
	virtual JVoid Send(JInt32   Value);
	virtual JVoid Send(JUnt32   Value);
	virtual JVoid Recv(JInt32 & Value);
	virtual JVoid Recv(JUnt32 & Value);

	// les fonctions pour transférer les flottants 32 bits
	virtual JVoid Send(JFlt32   Value);
	virtual JVoid Recv(JFlt32 & Value);

	// les fonctions pour transférer les flottants 64 bits
	virtual JVoid Send(JFlt64   Value);
	virtual JVoid Recv(JFlt64 & Value);

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JStream();

private:
	// l'opérateur d'affectation
	JStream & operator =(const JStream & Source);

	// le constructeur copie
	JStream(const JStream & Source);

private:
	JSerialize* m_pPrev; // le précédent
	JSerialize* m_pNext; // le suivant

private:
	JInt32 m_Version; // la version
};

// on finit l'inclusion conditionnelle
#endif
