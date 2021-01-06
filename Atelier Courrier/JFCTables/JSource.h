/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	Sylvain SAMMURI
//	J�r�my DEVILERS
//
//	Classe:
//	JSource
//
//	Descriptif:
//	Classe repr�sentant une source de la table des sources
*/

// on inclut les d�finitions n�cessaires
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JSOURCE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JSOURCE_H
#pragma once



class AFX_EXT_CLASS JSource
{
public:
	/// les constructeurs
	JSource();
	JSource(const JSource & Source);

	/// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	/// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	/// l'op�rateur pour recopier l'�l�ment
	JSource & operator =(const JSource & Source);

	/// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	/// le destructeur
	~JSource();

public:
	JInt32 m_IdSource; ///< l'identifiant de la source
	JInt32 m_IdMedia; ///< l'identifiant du m�dia
	JLabel m_Libelle;  ///< le libell�
};

// fin de l'inclusion conditionnelle
#endif
