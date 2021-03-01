//
// Fichier: JATPAFFCreuset.cpp
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JATPAFFCreuset.h"

////////////////////
// les constructeurs

JATPAFFCreuset::JATPAFFCreuset()
{
	// on initialise les éléments
	m_PlanVisible = false;
	m_PlanActif	=	false;

	m_HypoVisible = false;
	m_HypoActif	=	false;

	m_EchelleResult = true;

	m_QuickPlan = false;
	m_QuickPlanEchelle = true;
}

/////////////////////////////////////////////////
// les fonctions pour manipuler affichage creuset

JVoid JATPAFFCreuset::SetAffPlan(JBool EtatAffichage)
{
	// Fenetre plan visble
	m_PlanVisible = EtatAffichage;
}

JBool JATPAFFCreuset::GetAffPlan() const
{
	// Retourne visibilité fenêtre plan
	return (m_PlanVisible);
}

JVoid JATPAFFCreuset::SetAffHypo(JBool EtatAffichage)
{
	// Fenetre hypotheses (plan de test) visble
	m_HypoVisible = EtatAffichage;
}

JBool JATPAFFCreuset::GetAffHypo() const
{
	// Retourne visibilité fenêtre hypothèses (plan de test)
	return (m_HypoVisible);
}

// Etat du Creuset
JBool JATPAFFCreuset::GetState() const
{
	return (m_HypoVisible || m_PlanVisible);
}	

/////////////////
// le destructeur

JATPAFFCreuset::~JATPAFFCreuset()
{
	// on ne fait rien
}

JVoid JATPAFFCreuset::SetPlanActif(JBool EtatAffichage)
{
	// Fenetre plan visble
	m_PlanActif = EtatAffichage;
}

JBool JATPAFFCreuset::GetPlanActif() const
{
	// Retourne activation fenêtre plan
	return (m_PlanActif);
}

JVoid JATPAFFCreuset::SetHypoActif(JBool EtatAffichage)
{
	// Fenetre hypotheses (plan de test) Actif
	m_HypoActif = EtatAffichage;
}

JBool JATPAFFCreuset::GetHypoActif() const
{
	// Retourne activation fenêtre hypothèses (plan de test)
	return (m_HypoActif);
}

/////////////////////////////////////////////
// les fonction pour manipuler l'échelle result

JVoid JATPAFFCreuset::SetEchelleResult(JBool State)
{
	// on affiche l'échelle
	m_EchelleResult = State; 
}

JBool JATPAFFCreuset::GetEchelleResult() const
{
	// on renvoie l'échelle
	return m_EchelleResult;
}


JVoid JATPAFFCreuset::Send(JStream & Stream)
{
	Stream.Send(m_PlanVisible);
	Stream.Send(m_HypoVisible);
	Stream.Send(m_PlanActif);
	Stream.Send(m_HypoActif);
	Stream.Send(m_EchelleResult);
}

JVoid JATPAFFCreuset::Recv(JStream & Stream)
{
	Stream.Recv(m_PlanVisible);
	Stream.Recv(m_HypoVisible);
	Stream.Recv(m_PlanActif);
	Stream.Recv(m_HypoActif);
	Stream.Recv(m_EchelleResult);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JATPAFFCreuset::Swap(JATPAFFCreuset & Source)
{
	JBool Tmp;
	// on permute les éléments
	Tmp						=	m_PlanVisible;
	m_PlanVisible			=	Source.m_PlanVisible; 
	Source.m_PlanVisible	=	Tmp;

	Tmp						=	m_HypoVisible; 
	m_HypoVisible			=	Source.m_HypoVisible;
	Source.m_HypoVisible	=	Tmp;

	Tmp						=	m_PlanActif; 
	m_PlanActif				=	Source.m_PlanActif;
	Source.m_PlanActif		=	Tmp;
	
	Tmp						=	m_HypoActif; 
	m_HypoActif				=	Source.m_HypoActif;
	Source.m_HypoActif		=	Tmp;

	Tmp						=	m_EchelleResult; 
	m_EchelleResult			=	Source.m_EchelleResult;
	Source.m_EchelleResult	=	Tmp;

	Tmp						=	m_QuickPlan; 
	m_QuickPlan				=	Source.m_QuickPlan;
	Source.m_QuickPlan		=	Tmp;

	Tmp						=	m_QuickPlanEchelle; 
	m_QuickPlanEchelle		=	Source.m_QuickPlanEchelle;
	Source.m_QuickPlanEchelle=	Tmp;
	
}

JATPAFFCreuset & JATPAFFCreuset :: operator=(const JATPAFFCreuset & rhs)
{
	m_PlanVisible		= rhs.m_PlanVisible;
	m_HypoVisible		= rhs.m_HypoVisible;
	m_PlanActif			= rhs.m_PlanActif;
	m_HypoActif			= rhs.m_HypoActif;
	m_EchelleResult		= rhs.m_EchelleResult;
	m_QuickPlan			= rhs.m_QuickPlan;
	return *this;
}

JVoid JATPAFFCreuset::SetAffQuickPlan(JBool EtatAffichage)
{
	m_QuickPlan = EtatAffichage;
}
JBool JATPAFFCreuset::GetAffQuickPlan() const
{
	return m_QuickPlan;
}

///////////////////////////////////////////////////////////////////////////

JAFFQuickPlan JAFFQuickPlan::m_Instance;

JAFFQuickPlan::JAFFQuickPlan(void)
{
	m_AffQuickPlan = false;
}

JAFFQuickPlan::~JAFFQuickPlan(void)
{
}

//Singleton
JAFFQuickPlan * JAFFQuickPlan::GetInstance()
{
	return & m_Instance;
}

JVoid JAFFQuickPlan::SetAffQuickPlan(JBool EtatAffichage)
{
	m_AffQuickPlan = EtatAffichage;
}

JBool JAFFQuickPlan::GetAffQuickPlan() const
{
	return m_AffQuickPlan;
}

JVoid JATPAFFCreuset::SetEchelleQP(JBool EtatAffichage)
{
	m_QuickPlanEchelle = EtatAffichage;
}
JBool JATPAFFCreuset::GetEchelleQP() const
{
	return m_QuickPlanEchelle;
}


