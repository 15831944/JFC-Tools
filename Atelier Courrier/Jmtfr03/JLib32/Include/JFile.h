//
// Fichier: JFile.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_FILE_H
#define JLIB_FILE_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JSerialize.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JPath.h"

class JFile : public JSerialize
{
private:
	// le constructeur
	JFile();

public:
	// la fonction pour cr�er une instance
	static JFile* Create(const JPath & Path, JBool Update = false);

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// les fonctions pour transf�rer les donn�es
	virtual JVoid  Send(JUnt08 Data);
	virtual JUnt08 Recv();

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JFile();

private:
	// on d�finit les noeuds de la liste
	class JNode
	{
	public:
		JVoid* m_pHandle;           // le handle du fichier
		JInt32 m_FPointer;          // le pointeur sur le premier �l�ment
		JInt32 m_LPointer;          // le pointeur sur le dernier �l�ment
		JUnt08 m_Buffer[JFILE_MAX]; // le tampon du noeud
	};

private:
	// la fonction pour construire les tampons
	JBool OnMakeNodes();

	// la fonction pour ouvrir les fichiers
	JBool OnOpenFiles(const JChar* pFilename, JBool Update);

	// les fonctions pour manipuler les tampons
	JBool OnSendNode();
	JBool OnRecvNode();

	// la fonction pour lib�rer les tampons
	JVoid OnRemNodes();

private:
	// l'op�rateur d'affectation
	JFile & operator =(const JFile & Source);

	// le constructeur copie
	JFile(const JFile & Source);

private:
	JNode* m_pRNode; // le noeud de lecture
	JNode* m_pWNode; // le noeud d'�criture
};

// on finit l'inclusion conditionnelle
#endif
