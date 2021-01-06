#include "StdAfx.h"
#include ".\abscissegrph.h"

CAbscisseGrph::CAbscisseGrph(void)
{
	m_pATPDocCreuset = NULL;
}

CAbscisseGrph::~CAbscisseGrph(void)
{
}

JVoid CAbscisseGrph::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CAbscisseGrph::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

JVoid CAbscisseGrph::OnUpdate()
{
	// on initialise la période
	JDate DateDebut;
	JDate DateFin;
	JDate DatePD	= JDate::GetMin();
	JDate DatePF	= JDate::GetMin();

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0 )
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