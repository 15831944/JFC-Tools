//
// Fichier: JATPKEYPeriodeAction.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_KEY_PERIODE_ACTION_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_KEY_PERIODE_ACTION_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPKEYPeriodeAction
{
public:
	// le constructeur
	JATPKEYPeriodeAction();

	// les fonctions pour manipuler la période
	JVoid SetPeriode(JDate DateD, JDate DateF);
	JDate GetDateD() const;
	JDate GetDateF() const;

	// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les éléments
	JVoid Swap(JATPKEYPeriodeAction & Source);

	// le destructeur
	~JATPKEYPeriodeAction();

private:
	// l'opérateur pour recopier l'élément
	JATPKEYPeriodeAction & operator =(const JATPKEYPeriodeAction & Source);

	// le constructeur copie
	JATPKEYPeriodeAction(const JATPKEYPeriodeAction & Source);

private:
	JDate m_DateD; // la date de début
	JDate m_DateF; // la date de fin
};

// fin de l'inclusion conditionnelle
#endif
