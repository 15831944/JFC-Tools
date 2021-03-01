//
// Fichier: JReader.h
// Auteur:  Sylvain SAMMURI
// Date:    12/08/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_READER_H
#define JLIB_READER_H

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

// on inclut les définitions nécessaires
#include "..\Include\JLexeme.h"

class JReader
{
private:
	// le constructeur
	JReader(JStream* pStream);

public:
	// la fonction pour créer une instance
	static JReader* Create(JStream* pStream);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// la fonction pour récupérer la ligne
	virtual JInt32 GetLine() const;

	// les fonctions pour récupérer les lexèmes
	virtual JLexeme RecvString(JInt32 Count);
	virtual JLexeme RecvInteger(JInt32 Count);
	virtual JLexeme RecvFloat(JInt32 Count);
	virtual JLexeme RecvEndOfLine();

	// le destructeur
	virtual ~JReader();

private:
	// on définit le contexte du parseur
	class JContext
	{
	public:
		JInt32  m_Type;   // le type
		JInt32  m_Line;   // la ligne
		JInt32  m_Count;  // le compteur
		JInt32  m_Retain; // la retenue
		JLexeme m_Lexeme; // le lexème
	};

private:
	// la fonction pour construire le contexte
	JBool OnMakeContext();

private:
	// les fonctions pour recevoir les données
	JVoid OnFlushRetain(JStream* pStream, JContext* pContext);
	JVoid OnRecvLexeme(JStream* pStream, JContext* pContext, JInt32 Node, JInt32 Count);

private:
	// la fonction pour libérer le contexte
	JVoid OnRemContext();

private:
	// l'opérateur d'affectation
	JReader & operator =(const JReader & Source);

	// le constructeur copie
	JReader(const JReader & Source);

private:
	JContext* m_pContext; // le contexte
	JStream*  m_pStream;  // le flot
};

// on finit l'inclusion conditionnelle
#endif
