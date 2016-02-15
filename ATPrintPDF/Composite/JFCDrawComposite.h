//==============================
// fichier: JFCDrawComposite.h
//
// date: 02/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWCOMPOSITE_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWCOMPOSITE_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawComposite : public DrawComponent
{
public:
	// le constructeur
	DrawComposite();

	// le constructeur de recopie
	DrawComposite(const DrawComposite & source);

	// la fonction pour ajouter un élément
	virtual DrawComponent *  AddChild(DrawComponent & composant);

	// la fonction pour enlever un composant
	virtual void RemoveChild(DrawComponent * composant);	
	
	// le destructeur
	virtual ~DrawComposite();
	
};
// fin du test sur la macro
#endif