//
// Fichier: JATPDATInsertion.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    24/04/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_DAT_INSERTION_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_DAT_INSERTION_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPDATInsertion
{
public:
	// les constructeurs
	JATPDATInsertion();
	JATPDATInsertion(const JATPDATInsertion & Source);

	// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);
	JVoid Swap(JATPDATInsertion & Source);

	// l'op�rateur pour recopier l'�l�ment
	JATPDATInsertion & operator =(const JATPDATInsertion & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator ==(const JATPDATInsertion & Reference) const;
	JBool operator !=(const JATPDATInsertion & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JATPDATInsertion();

public:
	JInt32 m_TarifP; // le tarif plaquette
	JInt32 m_TarifN; // le tarif n�goci�
	JInt32 m_Duree;  // la dur�e en jours
	JInt32 m_numero;

	enum InsertStatus{NOTHING, STATUT1, STATUT2};
	InsertStatus m_status;
};

// fin de l'inclusion conditionnelle
#endif
