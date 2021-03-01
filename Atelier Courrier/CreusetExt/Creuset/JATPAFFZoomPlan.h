//
// Fichier: JATPAFFZoomPlan.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

#pragma once

// on inclut les définitions nécessaires
#include "JLib.h"
#include "PlanInsertions.h"

class JATPAFFZoomPlan
{
public:
	// le constructeur
	JATPAFFZoomPlan();

	// les fonctions pour manipuler l'affichage du ZoomPlan
	JVoid	SetAffZoomPlan(JUnt32 IdxHypothese, const JList<CInsKey> & List);
	JVoid	SetAffZoomPlan(JUnt32 IdxHypothese, const CInsKey & Key);
	JUnt32	GetIdxHypothese() const;
	const JList<CInsKey> & GetKeyList() const;

	void SetVisible(JBool bVisible);
	JBool IsVisible();

	// la fonction pour permuter les éléments
	JVoid Swap(JATPAFFZoomPlan & Source);

	// la fonction pour réinitialiser les éléments
	JVoid Reset();

	// le destructeur
	~JATPAFFZoomPlan();

private:
	// l'opérateur pour recopier l'élément
	JATPAFFZoomPlan & operator =(const JATPAFFZoomPlan & Source);

	// le constructeur copie
	JATPAFFZoomPlan(const JATPAFFZoomPlan & Source);

private:
	JUnt32			m_IdxHypothese;     // l'identifiant de l'hypothèse
	JList<CInsKey>	m_KeyList;			// Insertion Key
	JBool m_bVisible;
};
