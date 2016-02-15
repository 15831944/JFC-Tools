//===============================
// fichier: JFCDrawComponent.h
//
// date: 02/09/2001
// auteur: JB
//===============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWCOMPONENT_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWCOMPONENT_H_

// on inclus les fichiers nécessaires
#include "./../Draw/JFCDraw.h"

class DrawComponent
{
	friend class DrawComposite;
public:
	// le constructeur
	DrawComponent();

	// le constructeur de recopie
	DrawComponent(const DrawComponent & source);

	// la fonction pour ajouter un composant
	virtual DrawComponent * AddChild(DrawComponent & composant);

	// la fonction pour supprimer un composant
	virtual void RemoveChild(DrawComponent * composant);

	// la fonction pour dessiner le composant
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage) = 0;

	// les fonctions pour se déplacer dans la structure
	DrawComponent * GetRoot();
	DrawComponent * GetChild();
	DrawComponent * GetParent();
	DrawComponent * GetNext();
	DrawComponent * GetPrev();

	// la fonction de clonage
	virtual DrawComponent * Clone() const = 0;

	// le destructeur
	virtual ~DrawComponent();
protected:
	// le composite parent
	DrawComponent * m_Parent;

	// le lien avec l'enfant
	DrawComponent * m_Child;

	// le lien avec le précédent
	DrawComponent * m_Precedent;

	// le lien avec le suivant
	DrawComponent * m_Suivant;
};
// fin du test sur la macro
#endif