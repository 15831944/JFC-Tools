//
// Fichier: JExport.h
// Auteur:  Sylvain SAMMURI
// Date:    25/11/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_EXPORT_H
#define JLIB_EXPORT_H

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

// on inclut les définitions nécessaires
#include "..\Include\JLexeme.h"

class JExport
{
private:
	// le constructeur
	JExport(JStream* pStream);

public:
	// la fonction pour créer une instance
	static JExport* Create(JStream* pStream);

public:
	// la fonction pour récupérer la ligne
	virtual JInt32 GetLine() const;

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JExport();

private:
	// on définit le contexte du parseur
	class JContext
	{
	public:
		JInt32  m_Line;   // la ligne
		JInt32  m_Retain; // la retenue
		JLexeme m_Lexeme; // le lexème
	};

private:
	// la fonction pour construire le contexte
	JBool OnMakeContext();

private:
	// la fonction pour libérer le contexte
	JVoid OnRemContext();

private:
	// l'opérateur d'affectation
	JExport & operator =(const JExport & Source);

	// le constructeur copie
	JExport(const JExport & Source);

private:
	JContext* m_pContext; // le contexte
	JStream*  m_pStream;  // le flot
};

// on finit l'inclusion conditionnelle
#endif
