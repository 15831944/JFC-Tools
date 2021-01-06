/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Sylvain SAMMURI
//	Jérémy DEVILERS
//
//	Classe:
//	JSource
//
//	Descriptif:
//	Classe représentant une source de la table des sources
*/

// on inclut les définitions nécessaires
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JSOURCE_H

// on définit la macro pour éviter les inclusions multiples
#define JSOURCE_H
#pragma once



class AFX_EXT_CLASS JSource
{
public:
	/// les constructeurs
	JSource();
	JSource(const JSource & Source);

	/// la fonction pour tester l'élément
	JBool IsValid() const;

	/// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	/// l'opérateur pour recopier l'élément
	JSource & operator =(const JSource & Source);

	/// la fonction pour réinitialiser l'élément
	JVoid Reset();

	/// le destructeur
	~JSource();

public:
	JInt32 m_IdSource; ///< l'identifiant de la source
	JInt32 m_IdMedia; ///< l'identifiant du média
	JLabel m_Libelle;  ///< le libellé
};

// fin de l'inclusion conditionnelle
#endif
