//
// Fichier: JStringEx.h
// Auteur:  Sylvain SAMMURI
// Date:    06/06/2003
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_STRINGEX_H
#define JLIB_STRINGEX_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JStringEx
{
public:
	// les constructeurs
	JStringEx();
	JStringEx(const JChar* pSource);
	JStringEx(const JStringEx & Source);

	// la fonction pour r�cup�rer la longueur
	JInt32 GetLength() const;

	// les fonctions pour concat�ner les cha�nes
	JStringEx & Append(const JChar* pSource);
	JStringEx & Append(const JStringEx & Source);

	// les fonctions pour s�rialiser la cha�ne
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour r�cup�rer la cha�ne
	const JChar* AsJCharPtr() const;

	// les op�rateurs d'affectation
	JStringEx & operator =(const JChar* pSource);
	JStringEx & operator =(const JStringEx & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JStringEx & Reference) const;
	JBool operator !=(const JStringEx & Reference) const;
	JBool operator < (const JStringEx & Reference) const;
	JBool operator <=(const JStringEx & Reference) const;
	JBool operator > (const JStringEx & Reference) const;
	JBool operator >=(const JStringEx & Reference) const;

	// la fonction pour r�initialiser la cha�ne
	JVoid Reset();

	// le destructeur
	~JStringEx();

private:
	// on d�finit le noeud
	class JNode
	{
	public:
		JInt32 m_Count;     // le compteur
		JInt32 m_Length;    // la longueur
		JChar  m_String[1]; // la cha�ne
	};

private:
	// la fonction pour v�rifier la cha�ne
	JInt32 OnCheckString(const JChar* pSource) const;

private:
	// la fonction pour concat�ner la cha�ne
	JVoid OnAppendString(JNode* pNode, const JChar* pSource);

private:
	// les fonctions pour manipuler la cha�ne
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
