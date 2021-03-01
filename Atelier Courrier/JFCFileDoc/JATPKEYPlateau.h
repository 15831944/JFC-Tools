//
// Fichier: JATPKEYPlateau.h
// Auteur:  Alain CHAMBARD
// Date:    08/04/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_KEY_PLATEAU_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_KEY_PLATEAU_H

// on inclut les d�finitions n�cessaires
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

	// S�rialisation
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JATPKEYPlateau & Source);

	// le destructeur
	~JATPKEYPlateau();

private:
	// l'op�rateur pour recopier l'�l�ment
	JATPKEYPlateau & operator =(const JATPKEYPlateau & Source);

	// le constructeur copie
	JATPKEYPlateau(const JATPKEYPlateau & Source);

private:
	// Nom du plateau
	CString m_NomPlateau;

	// Path situation du plateau
	CString m_path;

	// Date d�but tarification
	JDate  m_DateTar;
};

// fin de l'inclusion conditionnelle
#endif
