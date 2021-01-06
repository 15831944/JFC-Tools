	
//
// Fichier: JATPEchelleSupports.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_KEY_ECHELLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_KEY_ECHELLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPEchelleSupports
{
public:
	// le constructeur
	JATPEchelleSupports();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour d�finir les �l�ments
	JVoid SetIdSupports(const JList<JUnt32> & ListIdSupports, JBool KeepOrder = true);

	// la fonction pour rechercher un �l�ment
	JInt32 FindItem(JUnt32 IdSupport) const;

	// la fonction pour ordonner les �l�ments
	JVoid SwapItems(JInt32 Index1, JInt32 Index2);

	// la fonction pour r�cup�rer les �l�ments
	const JUnt32 & Item(JInt32 Index) const;

	// les fonctions pour s�rialiser les �l�ments
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JATPEchelleSupports & Source);

	// le destructeur
	~JATPEchelleSupports();

private:
	// l'op�rateur pour recopier l'�l�ment
	JATPEchelleSupports & operator =(const JATPEchelleSupports & Source);

	// le constructeur copie
	JATPEchelleSupports(const JATPEchelleSupports & Source);

private:
	JArray<JUnt32> m_IdSupports; // les Supportx
};

// fin de l'inclusion conditionnelle
#endif
