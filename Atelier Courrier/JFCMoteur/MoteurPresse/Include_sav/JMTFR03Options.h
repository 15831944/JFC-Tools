//
// Fichier: JMTFR03Options.h
// Auteur:  Sylvain SAMMURI
// Date:    14/01/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_OPTIONS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_OPTIONS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03Options
{
public:
	// les constructeurs
	JMTFR03Options();
	JMTFR03Options(const JMTFR03Options & Source);

	// la fonction pour tester les éléments
	JBool IsValid() const;

	// les fonctions pour privilégier la LDP pour les quotidiens
	JVoid SetLDPQuotidiens(JBool Etat);
	JBool GetLDPQuotidiens() const;

	// les fonctions pour privilégier la LNM pour les magazines
	JVoid SetLNMMagazines(JBool Etat);
	JBool GetLNMMagazines() const;

	// les fonctions pour utiliser la LDP à 30 jours pour les bimestriels
	JVoid SetLDP30JrsBimestriels(JBool Etat);
	JBool GetLDP30JrsBimestriels() const;

	// les fonctions pour prendre en compte les reprises en mains
	JVoid SetReprises(JBool Etat);
	JBool GetReprises() const;

	// l'opérateur pour recopier les éléments
	JMTFR03Options & operator =(const JMTFR03Options & Source);

	// la fonction pour réinitialiser les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03Options();

private:
	JRef32 m_LDPQuotidiens;       // le mode des quotidiens
	JRef32 m_LNMMagazines;        // le mode des magazines
	JRef32 m_LDP30JrsBimestriels; // le mode des bimestriels
	JRef32 m_Reprises;            // le mode des reprises
};

// fin de l'inclusion conditionnelle
#endif
