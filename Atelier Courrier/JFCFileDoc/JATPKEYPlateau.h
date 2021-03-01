//
// Fichier: JATPKEYPlateau.h
// Auteur:  Alain CHAMBARD
// Date:    08/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_KEY_PLATEAU_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_KEY_PLATEAU_H

// on inclut les définitions nécessaires
#include "JLib.h"

class AFX_EXT_CLASS JATPKEYPlateau
{
public:

	// le constructeur
	JATPKEYPlateau();

	// les fonctions pour manipuler le nom du plateau
	CString GetLabel() const;
	JVoid SetPath(CString path);
	CString GetPath() const;

	// les fonctions pour manipuler la date de tarification
	JVoid SetDateTar(JDate DateTar);
	JDate GetDateTar() const;	

	// Sérialisation
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les éléments
	JVoid Swap(JATPKEYPlateau & Source);

	// le destructeur
	~JATPKEYPlateau();

private:
	// l'opérateur pour recopier l'élément
	JATPKEYPlateau & operator =(const JATPKEYPlateau & Source);

	// le constructeur copie
	JATPKEYPlateau(const JATPKEYPlateau & Source);

private:
	// Nom du plateau
	CString m_NomPlateau;

	// Path situation du plateau
	CString m_path;

	// Date début tarification
	JDate  m_DateTar;
};

// fin de l'inclusion conditionnelle
#endif
