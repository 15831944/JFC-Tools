//
// Fichier: JATPAFFCreuset.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_AFF_CRESUET_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_AFF_CREUSET_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"



class JAFFQuickPlan
{
public:
	//Singleton
	static JAFFQuickPlan* GetInstance();

	JVoid SetAffQuickPlan(JBool EtatAffichage);
	JBool GetAffQuickPlan() const;

private:
	//Singleton
	JBool m_AffQuickPlan;

	JAFFQuickPlan(void);
	virtual ~JAFFQuickPlan(void);
	static JAFFQuickPlan	m_Instance;
};


class JATPAFFCreuset
{
public:
	// le constructeur
	JATPAFFCreuset();

	// les fonctions pour manipuler l'affichage creuset
	JVoid SetAffPlan(JBool EtatAffichage);
	JVoid SetAffHypo(JBool EtatAffichage);
	
	// Etat affichage fen�tre plan ou fen�tre hypo
	JBool GetAffPlan() const;
	JBool GetAffHypo() const;

	JVoid SetAffQuickPlan(JBool EtatAffichage);
	JBool GetAffQuickPlan() const;

	JVoid SetEchelleQP(JBool EtatAffichage);
	JBool GetEchelleQP() const;

	// les fonctions pour manipuler l'affichage creuset
	JVoid SetPlanActif(JBool EtatAffichage);
	JVoid SetHypoActif(JBool EtatAffichage);
	
	// Etat affichage fen�tre plan ou fen�tre hypo
	JBool GetPlanActif() const;
	JBool GetHypoActif() const;

	// les fonction pour manipuler l'�chelle plan
	JVoid SetEchelleResult(JBool State);
	JBool GetEchelleResult() const;

	// Etat Creuset
	JBool GetState() const;

	// Swap et sauvegarde
	JVoid	Swap(JATPAFFCreuset & Source);

	JATPAFFCreuset & operator=(const JATPAFFCreuset & rhs);

	JVoid	Send(JStream & Stream); 
	JVoid	Recv(JStream & Stream);

	// le destructeur
	~JATPAFFCreuset();
	
private:
	JBool m_PlanVisible;
	JBool m_HypoVisible;
	JBool m_PlanActif;
	JBool m_HypoActif;
	JBool m_EchelleResult;

	JBool m_QuickPlan;
	JBool m_QuickPlanEchelle;
	
};



// fin de l'inclusion conditionnelle
#endif
