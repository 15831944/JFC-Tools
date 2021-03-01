//
// Fichier: JStringEx.h
// Auteur:  Sylvain SAMMURI
// Date:    06/06/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_STRINGEX_H
#define JLIB_STRINGEX_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JStringEx
{
public:
	// les constructeurs
	JStringEx();
	JStringEx(const JChar* pSource);
	JStringEx(const JStringEx & Source);

	// la fonction pour récupérer la longueur
	JInt32 GetLength() const;

	// les fonctions pour concaténer les chaînes
	JStringEx & Append(const JChar* pSource);
	JStringEx & Append(const JStringEx & Source);

	// les fonctions pour sérialiser la chaîne
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer la chaîne
	const JChar* AsJCharPtr() const;

	// les opérateurs d'affectation
	JStringEx & operator =(const JChar* pSource);
	JStringEx & operator =(const JStringEx & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JStringEx & Reference) const;
	JBool operator !=(const JStringEx & Reference) const;
	JBool operator < (const JStringEx & Reference) const;
	JBool operator <=(const JStringEx & Reference) const;
	JBool operator > (const JStringEx & Reference) const;
	JBool operator >=(const JStringEx & Reference) const;

	// la fonction pour réinitialiser la chaîne
	JVoid Reset();

	// le destructeur
	~JStringEx();

private:
	// on définit le noeud
	class JNode
	{
	public:
		JInt32 m_Count;     // le compteur
		JInt32 m_Length;    // la longueur
		JChar  m_String[1]; // la chaîne
	};

private:
	// la fonction pour vérifier la chaîne
	JInt32 OnCheckString(const JChar* pSource) const;

private:
	// la fonction pour concaténer la chaîne
	JVoid OnAppendString(JNode* pNode, const JChar* pSource);

private:
	// les fonctions pour manipuler la chaîne
	JInt32 OnCompareString(const JChar* pSource, const JChar* pReference) const;
	const JChar* OnExtractString() const;

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddNode(JNode* & pNewNode, JInt32 Count);
	JVoid OnRepNode(JNode* pNode);
	JVoid OnSetNode(JNode* pNode);

private:
	JNode* m_pNode; // le noeud
};

// fin de l'inclusion conditionnelle
#endif
