// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetPlan_C1.h"
#include "ATPCreusetPlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_C1::CATPCreusetPlan_C1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan_C1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPCreusetPlan_C1::SetParent(CATPCreusetPlan * pATPCreusetPlan)
{
	m_pATPCreusetPlan = pATPCreusetPlan; 
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan_C1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_C1::~CATPCreusetPlan_C1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_C1, JFCCalendar)
	//{{AFX_MSG_MAP(CATPCreusetPlan_C1)
	ON_MESSAGE(MSG_CALENDAR_SELCHANGING, OnSelChanging)
	ON_MESSAGE(MSG_CALENDAR_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan_C1::OnUpdate()
{
	// on initialise la période
	JDate DateDebut;
	JDate DateFin;
	JDate DatePD	= JDate::GetMin();
	JDate DatePF	= JDate::GetMin();

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère la période d'action
		DateDebut = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		DateFin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
		DatePD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
		DatePF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();

		if(m_pATPDocCreuset->m_IdxHypotheseCourant == 0)
		{
			DateDebut = m_pATPDocCreuset->m_KEYEtude.GetDateD();
			DateFin = m_pATPDocCreuset->m_KEYEtude.GetDateF();
			DatePD = m_pATPDocCreuset->m_KEYPeriodeActionHyp.GetDateD();
			DatePF = m_pATPDocCreuset->m_KEYPeriodeActionHyp.GetDateF();
		}

		// on met à jour le composant
		SetRange(DateDebut, DateFin, TRUE);

		if(DatePD!=DateDebut || DatePF!=DateFin)
			SetSelRange(DatePD,	DatePF, TRUE);
		else
		{
			if(m_FlagSel==FALSE)
				Reset();
		}

	}
}


/////////////////////////////////////
// CATPCreusetPlan_C1 message handlers

LRESULT CATPCreusetPlan_C1::OnSelChanging(WPARAM, LPARAM)
{
	JDate first, last;
	GetSelRange(first, last);
	if (first.IsValid() && last.IsValid())
		m_pATPDocCreuset->m_KEYPeriodeActionTablier.SetPeriode(first, last);

	m_pATPCreusetPlan->Update();
	//Quand on aura des Pentium VI 64bits à 10Ghz, pour une mise à jour des resultats en temps réel
	//m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PERIODE_ACTION);
	return 0;
}

LRESULT CATPCreusetPlan_C1::OnSelChanged(WPARAM, LPARAM)
{
	JDate first, last;
	GetSelRange(first, last);
	if (first.IsValid() && last.IsValid())
		m_pATPDocCreuset->m_KEYPeriodeActionTablier.SetPeriode(first, last);
	else
	{
		GetRange(first, last);
		m_pATPDocCreuset->m_KEYPeriodeActionTablier.SetPeriode(first, last);
	}

	m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PERIODE_ACTION);
	return 0;
}