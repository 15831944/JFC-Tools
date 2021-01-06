//
// Fichier: JATPEtude.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_ETUDE_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_ETUDE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPEtude
{
public:
	// les constructeurs
	JATPEtude();
	JATPEtude(const JATPEtude & Source);

	// la fonction pour tester la validité de l'élément
	JBool IsValid() const;

	// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// l'opérateur pour recopier l'élément
	JATPEtude & operator =(const JATPEtude & Source);

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JATPEtude();

public:
	JUnt32 m_IdEtude; // l'identifiant de l'étude
	JLabel m_Libelle; // le libellé de l'étude
	JBool  m_Archive; // indique si l'étude est archivé 
	                  // (n'apparaitra plus au moment de l'ouverture d'une étude)
};

// fin de l'inclusion conditionnelle
#endif
