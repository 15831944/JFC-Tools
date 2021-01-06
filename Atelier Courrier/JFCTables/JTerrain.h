/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTerrain
//
//	Descriptif:
//	Element terrain de la table des terrains
*/



// on inclut les définitions nécessaires
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JTERRAIN_H

// on définit la macro pour éviter les inclusions multiples
#define JTERRAIN_H
#pragma once



class AFX_EXT_CLASS JTerrain
{
public:
	/// les constructeurs
	JTerrain();
	JTerrain(const JTerrain & Source);

	/// la fonction pour tester l'élément
	JBool IsValid() const;

	/// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	/// l'opérateur pour recopier l'élément
	JTerrain & operator =(const JTerrain & Source);

	/// la fonction pour réinitialiser l'élément
	JVoid Reset();

	/// le destructeur
	~JTerrain();

public:
	JInt32 m_IdTerrain;					// l'identifiant du terrain
	JLabel m_Libelle;					// le libellé du terrain
	JInt32 m_IdSource;					// l'identifiant de la source du terrain
	JLabel m_Chemin;					// le chemin d'accés du fichier terrain

	// Les seuils nb contacts terrains
	// Explication sur les seuils
	// Exemple 0
	// 
	// 300, 100   : impossible !!!!!
	//
	// Exemple 1
	// 100, 200   : en dessous de 100 on interdit utilsation de la cible, entre 100 et 200 non inclus message de prévention
	//
	// Exemple 2
	// 0,1		  : autorisation d'une cible avec 0 contacts, et message de prévention si nb contacts cible <= 0
	// 
	// Exemple 3   
	// -1, 1	  : 0 cible non utilisable, uniquement avertissement si nb contacts cible <=0
	//
	// On initialisera les seuils de la façon suivante : 0, 0  (pas d'interdiction, pas de message)
	// 
	JInt32 m_SeuilRedhibitoire;			// seuil au delà duquel une cible peut être utilisée, sinon inutilisable si en dessous
	JInt32 m_SeuilAvertissement;		// seuil au delà duquel avertissement cible avec nb contact insuffisant, mais utilisable


};

// fin de l'inclusion conditionnelle
#endif
