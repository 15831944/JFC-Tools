//
// Fichier: JParser.h
// Auteur:  Sylvain SAMMURI
// Date:    12/08/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_PARSER_H
#define JLIB_PARSER_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JLexeme.h"

class JParser
{
private:
	// le constructeur
	JParser(JStream* pStream);

public:
	// la fonction pour cr�er une instance
	static JParser* Create(JStream* pStream);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// la fonction pour r�cup�rer la ligne
	virtual JInt32 GetLine() const;

	// les fonctions pour ajouter des �l�ments
	virtual JVoid AddKeyword(const JChar* pKeyword, JInt32 Type);
	virtual JVoid AddOperator(const JChar* pOperator, JInt32 Type);

	// la fonction pour r�cup�rer les lex�mes
	virtual JLexeme Recv();

	// le destructeur
	virtual ~JParser();

private:
	// on d�finit les noeuds du parseur
	class JNode
	{
	public:
		JInt32 m_Type;       // le type
		JInt32 m_State;      // l'�tat
		JNode* m_pNext;      // le suivant
		JNode* m_pLink[256]; // les liens
		JInt08 m_Mode[256];  // les modes
	};

private:
	// on d�finit le contexte du parseur
	class JContext
	{
	public:
		JInt32   m_Count;    // le compteur
		JNode*   m_pNode;    // le noeud courant
		JLexeme* m_pCLexeme; // le lex�me courant
		JLexeme* m_pSLexeme; // le lex�me en stock
		JLexeme  m_FLexeme;  // le premier lex�me
		JLexeme  m_LLexeme;  // le dernier lex�me
		JInt32   m_Store;    // le nombre de lex�mes
		JInt32   m_Error;    // le nombre d'erreurs
		JInt32   m_Line;     // le num�ro de ligne
	};

private:
	// les fonctions pour fabriquer le parseur
	JBool OnMakeParser();
	JBool OnMakeContext();

private:
	// les fonctions pour tester les �l�ments
	JInt32 OnCheckKeyword(const JChar* pKeyword) const;
	JInt32 OnCheckOperator(const JChar* pOperator) const;

private:
	// les fonctions pour mettre � jour les noeuds des �l�ments
	JVoid OnUpdateKeywordNode(JNode** pNodes, JNode* pPrev, JNode* pNode, JInt32 State, JInt32 Code, JInt32 Type, JBool Last, JBool New);
	JVoid OnUpdateOperatorNode(JNode** pNodes, JNode* pPrev, JNode* pNode, JInt32 State, JInt32 Code, JInt32 Type, JBool Last, JBool New);

private:
	// les fonctions pour ajouter les �l�ments
	JBool OnMakeKeyword(const JChar* pKeyword, JInt32 Size, JInt32 Type);
	JBool OnMakeOperator(const JChar* pOperator, JInt32 Size, JInt32 Type);

private:
	// les fonctions pour permuter les lex�mes
	JVoid OnResetLexeme(JContext* pContext);
	JVoid OnValidateLexeme(JContext* pContext);

private:
	// les fonctions pour manipuler les lex�mes
	JVoid OnFillLexeme(JContext* pContext, JChar Value);
	JVoid OnSetLexeme(JContext* pContext);

private:
	// les fonctions pour recevoir les lex�mes
	JVoid OnUpdateContext(JContext* pContext, JChar Value);
	JVoid OnRecvLexeme(JStream* pStream, JContext* pContext);

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddNodes(JInt32 Count, JInt32 State, JNode* & pNode);
	JVoid OnRemNodes();

private:
	// l'op�rateur d'affectation
	JParser & operator =(const JParser & Source);

	// le constructeur copie
	JParser(const JParser & Source);

private:
	JContext* m_pContext; // le contexte
	JStream*  m_pStream;  // le flot

private:
	JNode* m_pFNode; // le premier noeud
	JNode* m_pLNode; // le dernier noeud
};

// on finit l'inclusion conditionnelle
#endif
