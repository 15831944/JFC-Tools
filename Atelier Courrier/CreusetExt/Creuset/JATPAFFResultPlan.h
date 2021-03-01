//
// Fichier: JATPAFFResultPlan.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_AFF_ResultPlan_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_AFF_ResultPlan_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPAFFResultPlan
{
public:
	// le constructeur
	JATPAFFResultPlan();

	// les fonctions pour manipuler le graphe des GRP
	JVoid SetGraphGRPJour();
	JBool GetGraphGRPJour() const;

	// les fonctions pour manipuler le graphe des GRP
	JVoid SetGraphGRPCumul();
	JBool GetGraphGRPCumul() const;

	// les fonctions pour manipuler la courbe des C%
	JVoid SetGraphCouvCumul();
	JBool GetGraphCouvCumul() const;

	// les fonctions pour manipuler la courbe des C%
	JVoid SetGraphCouvJour();
	JBool GetGraphCouvJour() const;

	// la fonction pour permuter les éléments
	JVoid Swap(JATPAFFResultPlan & Source);
	JVoid	Send(JStream & Stream); 
	JVoid	Recv(JStream & Stream);

	// le destructeur
	~JATPAFFResultPlan();

private:
	// l'opérateur pour recopier l'élément
	JATPAFFResultPlan & operator =(const JATPAFFResultPlan & Source);

	// le constructeur copie
	JATPAFFResultPlan(const JATPAFFResultPlan & Source);

private:
	JUnt32 m_IdResultPlan;        // le ResultPlan
};

// fin de l'inclusion conditionnelle
#endif
