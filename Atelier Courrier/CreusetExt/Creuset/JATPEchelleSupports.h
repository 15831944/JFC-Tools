	
//
// Fichier: JATPEchelleSupports.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_KEY_ECHELLE_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_KEY_ECHELLE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPEchelleSupports
{
public:
	// le constructeur
	JATPEchelleSupports();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour définir les éléments
	JVoid SetIdSupports(const JList<JUnt32> & ListIdSupports, JBool KeepOrder = true);

	// la fonction pour rechercher un élément
	JInt32 FindItem(JUnt32 IdSupport) const;

	// la fonction pour ordonner les éléments
	JVoid SwapItems(JInt32 Index1, JInt32 Index2);

	// la fonction pour récupérer les éléments
	const JUnt32 & Item(JInt32 Index) const;

	// les fonctions pour sérialiser les éléments
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les éléments
	JVoid Swap(JATPEchelleSupports & Source);

	// le destructeur
	~JATPEchelleSupports();

private:
	// l'opérateur pour recopier l'élément
	JATPEchelleSupports & operator =(const JATPEchelleSupports & Source);

	// le constructeur copie
	JATPEchelleSupports(const JATPEchelleSupports & Source);

private:
	JArray<JUnt32> m_IdSupports; // les Supportx
};

// fin de l'inclusion conditionnelle
#endif
