//
// Fichier: JExport.h
// Auteur:  Sylvain SAMMURI
// Date:    25/11/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_EXPORT_H
#define JLIB_EXPORT_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JLexeme.h"

class JExport
{
private:
	// le constructeur
	JExport(JStream* pStream);

public:
	// la fonction pour cr�er une instance
	static JExport* Create(JStream* pStream);

public:
	// la fonction pour r�cup�rer la ligne
	virtual JInt32 GetLine() const;

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JExport();

private:
	// on d�finit le contexte du parseur
	class JContext
	{
	public:
		JInt32  m_Line;   // la ligne
		JInt32  m_Retain; // la retenue
		JLexeme m_Lexeme; // le lex�me
	};

private:
	// la fonction pour construire le contexte
	JBool OnMakeContext();

private:
	// la fonction pour lib�rer le contexte
	JVoid OnRemContext();

private:
	// l'op�rateur d'affectation
	JExport & operator =(const JExport & Source);

	// le constructeur copie
	JExport(const JExport & Source);

private:
	JContext* m_pContext; // le contexte
	JStream*  m_pStream;  // le flot
};

// on finit l'inclusion conditionnelle
#endif
