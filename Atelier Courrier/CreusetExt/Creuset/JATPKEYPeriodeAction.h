//
// Fichier: JATPKEYPeriodeAction.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_KEY_PERIODE_ACTION_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_KEY_PERIODE_ACTION_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPKEYPeriodeAction
{
public:
	// le constructeur
	JATPKEYPeriodeAction();

	// les fonctions pour manipuler la p�riode
	JVoid SetPeriode(JDate DateD, JDate DateF);
	JDate GetDateD() const;
	JDate GetDateF() const;

	// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JATPKEYPeriodeAction & Source);

	// le destructeur
	~JATPKEYPeriodeAction();

private:
	// l'op�rateur pour recopier l'�l�ment
	JATPKEYPeriodeAction & operator =(const JATPKEYPeriodeAction & Source);

	// le constructeur copie
	JATPKEYPeriodeAction(const JATPKEYPeriodeAction & Source);

private:
	JDate m_DateD; // la date de d�but
	JDate m_DateF; // la date de fin
};

// fin de l'inclusion conditionnelle
#endif
