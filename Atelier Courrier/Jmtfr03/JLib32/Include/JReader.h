//
// Fichier: JReader.h
// Auteur:  Sylvain SAMMURI
// Date:    12/08/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_READER_H
#define JLIB_READER_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JLexeme.h"

class JReader
{
private:
	// le constructeur
	JReader(JStream* pStream);

public:
	// la fonction pour cr�er une instance
	static JReader* Create(JStream* pStream);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// la fonction pour r�cup�rer la ligne
	virtual JInt32 GetLine() const;

	// les fonctions pour r�cup�rer les lex�mes
	virtual JLexeme RecvString(JInt32 Count);
	virtual JLexeme RecvInteger(JInt32 Count);
	virtual JLexeme RecvFloat(JInt32 Count);
	virtual JLexeme RecvEndOfLine();

	// le destructeur
	virtual ~JReader();

private:
	// on d�finit le contexte du parseur
	class JContext
	{
	public:
		JInt32  m_Type;   // le type
		JInt32  m_Line;   // la ligne
		JInt32  m_Count;  // le compteur
		JInt32  m_Retain; // la retenue
		JLexeme m_Lexeme; // le lex�me
	};

private:
	// la fonction pour construire le contexte
	JBool OnMakeContext();

private:
	// les fonctions pour recevoir les donn�es
	JVoid OnFlushRetain(JStream* pStream, JContext* pContext);
	JVoid OnRecvLexeme(JStream* pStream, JContext* pContext, JInt32 Node, JInt32 Count);

private:
	// la fonction pour lib�rer le contexte
	JVoid OnRemContext();

private:
	// l'op�rateur d'affectation
	JReader & operator =(const JReader & Source);

	// le constructeur copie
	JReader(const JReader & Source);

private:
	JContext* m_pContext; // le contexte
	JStream*  m_pStream;  // le flot
};

// on finit l'inclusion conditionnelle
#endif
