//
// Fichier: JLabel.h
// Auteur:  Sylvain SAMMURI
// Date:    13/01/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_LABEL_H
#define JLIB_LABEL_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JLabel
{
public:
	// les constructeurs
	JLabel();
	JLabel(const JChar* pSource);
	JLabel(const JLabel & Source);

	// la fonction pour tester le libellé
	JBool IsEmpty() const;

	// la fonction pour récupérer la clé de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour sérialiser le libellé
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer le libellé
	const JChar* AsJCharPtr() const;

	// les opérateurs d'affectation
	JLabel & operator =(const JChar* pSource);
	JLabel & operator =(const JLabel & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JLabel & Reference) const;
	JBool operator !=(const JLabel & Reference) const;
	JBool operator < (const JLabel & Reference) const;
	JBool operator <=(const JLabel & Reference) const;
	JBool operator > (const JLabel & Reference) const;
	JBool operator >=(const JLabel & Reference) const;

	// la fonction pour réinitialiser le libellé
	JVoid Reset();

	// le destructeur
	~JLabel();

private:
	// on définit le noeud
	class JNode
	{
	public:
		JInt32 m_Count;    // le compteur
		JChar  m_Label[1]; // le libellé
	};

private:
	// les fonctions pour formater le libellé
	JInt32 OnTrimLeft(const JChar* pSource) const;
	JInt32 OnTrimRight(const JChar* pSource, JInt32 Start) const;

private:
	// la fonction pour remplacer le libellé
	JVoid OnReplaceLabel(JNode* pNode, const JChar* pSource, JInt32 Start, JInt32 Count);

private:
	// les fonctions pour manipuler le libellé
	JInt32 OnCompareLabel(const JChar* pSource, const JChar* pReference) const;
	const JChar* OnExtractLabel() const;

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddNode(JNode* & pNewNode, JInt32 Count);
	JVoid OnRepNode(JNode* pNode);
	JVoid OnSetNode(JNode* pNode);

private:
	JNode* m_pNode; // le noeud
};

// on finit l'inclusion conditionnelle
#endif
