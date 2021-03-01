//
// Fichier: JParser.h
// Auteur:  Sylvain SAMMURI
// Date:    12/08/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_PARSER_H
#define JLIB_PARSER_H

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

// on inclut les définitions nécessaires
#include "..\Include\JLexeme.h"

class JParser
{
private:
	// le constructeur
	JParser(JStream* pStream);

public:
	// la fonction pour créer une instance
	static JParser* Create(JStream* pStream);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// la fonction pour récupérer la ligne
	virtual JInt32 GetLine() const;

	// les fonctions pour ajouter des éléments
	virtual JVoid AddKeyword(const JChar* pKeyword, JInt32 Type);
	virtual JVoid AddOperator(const JChar* pOperator, JInt32 Type);

	// la fonction pour récupérer les lexèmes
	virtual JLexeme Recv();

	// le destructeur
	virtual ~JParser();

private:
	// on définit les noeuds du parseur
	class JNode
	{
	public:
		JInt32 m_Type;       // le type
		JInt32 m_State;      // l'état
		JNode* m_pNext;      // le suivant
		JNode* m_pLink[256]; // les liens
		JInt08 m_Mode[256];  // les modes
	};

private:
	// on définit le contexte du parseur
	class JContext
	{
	public:
		JInt32   m_Count;    // le compteur
		JNode*   m_pNode;    // le noeud courant
		JLexeme* m_pCLexeme; // le lexème courant
		JLexeme* m_pSLexeme; // le lexème en stock
		JLexeme  m_FLexeme;  // le premier lexème
		JLexeme  m_LLexeme;  // le dernier lexème
		JInt32   m_Store;    // le nombre de lexèmes
		JInt32   m_Error;    // le nombre d'erreurs
		JInt32   m_Line;     // le numéro de ligne
	};

private:
	// les fonctions pour fabriquer le parseur
	JBool OnMakeParser();
	JBool OnMakeContext();

private:
	// les fonctions pour tester les éléments
	JInt32 OnCheckKeyword(const JChar* pKeyword) const;
	JInt32 OnCheckOperator(const JChar* pOperator) const;

private:
	// les fonctions pour mettre à jour les noeuds des éléments
	JVoid OnUpdateKeywordNode(JNode** pNodes, JNode* pPrev, JNode* pNode, JInt32 State, JInt32 Code, JInt32 Type, JBool Last, JBool New);
	JVoid OnUpdateOperatorNode(JNode** pNodes, JNode* pPrev, JNode* pNode, JInt32 State, JInt32 Code, JInt32 Type, JBool Last, JBool New);

private:
	// les fonctions pour ajouter les éléments
	JBool OnMakeKeyword(const JChar* pKeyword, JInt32 Size, JInt32 Type);
	JBool OnMakeOperator(const JChar* pOperator, JInt32 Size, JInt32 Type);

private:
	// les fonctions pour permuter les lexèmes
	JVoid OnResetLexeme(JContext* pContext);
	JVoid OnValidateLexeme(JContext* pContext);

private:
	// les fonctions pour manipuler les lexèmes
	JVoid OnFillLexeme(JContext* pContext, JChar Value);
	JVoid OnSetLexeme(JContext* pContext);

private:
	// les fonctions pour recevoir les lexèmes
	JVoid OnUpdateContext(JContext* pContext, JChar Value);
	JVoid OnRecvLexeme(JStream* pStream, JContext* pContext);

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddNodes(JInt32 Count, JInt32 State, JNode* & pNode);
	JVoid OnRemNodes();

private:
	// l'opérateur d'affectation
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
