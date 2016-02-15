//==============================
// fichier: JFCDrawComposite.h
//
// date: 02/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCDRAWCOMPOSITE_H_

// on d�finit une macro pour identifier le fichier
#define _JFCDRAWCOMPOSITE_H_

// on inclut les fichiers n�cessaires
#include "JFCDrawComponent.h"

class DrawComposite : public DrawComponent
{
public:
	// le constructeur
	DrawComposite();

	// le constructeur de recopie
	DrawComposite(const DrawComposite & source);

	// la fonction pour ajouter un �l�ment
	virtual DrawComponent *  AddChild(DrawComponent & composant);

	// la fonction pour enlever un composant
	virtual void RemoveChild(DrawComponent * composant);	
	
	// le destructeur
	virtual ~DrawComposite();
	
};
// fin du test sur la macro
#endif