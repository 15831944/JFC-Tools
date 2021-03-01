/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JTerrain
//
//	Descriptif:
//	Element terrain de la table des terrains
*/



// on inclut les d�finitions n�cessaires
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JTERRAIN_H

// on d�finit la macro pour �viter les inclusions multiples
#define JTERRAIN_H
#pragma once



class AFX_EXT_CLASS JTerrain
{
public:
	/// les constructeurs
	JTerrain();
	JTerrain(const JTerrain & Source);

	/// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	/// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	/// l'op�rateur pour recopier l'�l�ment
	JTerrain & operator =(const JTerrain & Source);

	/// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	/// le destructeur
	~JTerrain();

public:
	JInt32 m_IdTerrain;					// l'identifiant du terrain
	JLabel m_Libelle;					// le libell� du terrain
	JInt32 m_IdSource;					// l'identifiant de la source du terrain
	JLabel m_Chemin;					// le chemin d'acc�s du fichier terrain

	// Les seuils nb contacts terrains
	// Explication sur les seuils
	// Exemple 0
	// 
	// 300, 100   : impossible !!!!!
	//
	// Exemple 1
	// 100, 200   : en dessous de 100 on interdit utilsation de la cible, entre 100 et 200 non inclus message de pr�vention
	//
	// Exemple 2
	// 0,1		  : autorisation d'une cible avec 0 contacts, et message de pr�vention si nb contacts cible <= 0
	// 
	// Exemple 3   
	// -1, 1	  : 0 cible non utilisable, uniquement avertissement si nb contacts cible <=0
	//
	// On initialisera les seuils de la fa�on suivante : 0, 0  (pas d'interdiction, pas de message)
	// 
	JInt32 m_SeuilRedhibitoire;			// seuil au del� duquel une cible peut �tre utilis�e, sinon inutilisable si en dessous
	JInt32 m_SeuilAvertissement;		// seuil au del� duquel avertissement cible avec nb contact insuffisant, mais utilisable


};

// fin de l'inclusion conditionnelle
#endif
