//
// Fichier: JATPEtude.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_ETUDE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_ETUDE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPEtude
{
public:
	// les constructeurs
	JATPEtude();
	JATPEtude(const JATPEtude & Source);

	// la fonction pour tester la validit� de l'�l�ment
	JBool IsValid() const;

	// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// l'op�rateur pour recopier l'�l�ment
	JATPEtude & operator =(const JATPEtude & Source);

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JATPEtude();

public:
	JUnt32 m_IdEtude; // l'identifiant de l'�tude
	JLabel m_Libelle; // le libell� de l'�tude
	JBool  m_Archive; // indique si l'�tude est archiv� 
	                  // (n'apparaitra plus au moment de l'ouverture d'une �tude)
};

// fin de l'inclusion conditionnelle
#endif
