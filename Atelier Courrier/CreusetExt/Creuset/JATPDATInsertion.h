//
// Fichier: JATPDATInsertion.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    24/04/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_DAT_INSERTION_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_DAT_INSERTION_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPDATInsertion
{
public:
	// les constructeurs
	JATPDATInsertion();
	JATPDATInsertion(const JATPDATInsertion & Source);

	// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);
	JVoid Swap(JATPDATInsertion & Source);

	// l'opérateur pour recopier l'élément
	JATPDATInsertion & operator =(const JATPDATInsertion & Source);

	// les opérateurs pour comparer les éléments
	JBool operator ==(const JATPDATInsertion & Reference) const;
	JBool operator !=(const JATPDATInsertion & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JATPDATInsertion();

public:
	JInt32 m_TarifP; // le tarif plaquette
	JInt32 m_TarifN; // le tarif négocié
	JInt32 m_Duree;  // la durée en jours
	JInt32 m_numero;

	enum InsertStatus{NOTHING, STATUT1, STATUT2};
	InsertStatus m_status;
};

// fin de l'inclusion conditionnelle
#endif
