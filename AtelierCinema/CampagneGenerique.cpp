// CampagneGenerique.cpp: implementation of the CCampagneGenerique class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "CampagneGenerique.h"
#include "CAlendrierHebdoCinema.h"
#include "grille.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCampagneGenerique::CCampagneGenerique()
{
	// Base de Calcul régionale : activée par défaut
	BaseCalcul=0; // 0:Régional 2:National

	// Init des accélérateurs graphiques
	m_hBmpOldspeed=0;
	m_hDCspeed=0;
	m_hBmpNewspeed=0;

	// Init nom campagne
	m_NomCampagne = "";

	// Init Vague Campagne
	m_NomVagueCampagne = "";

	m_Beta=70;
	fCurCalend=0;
	PosXCurCalend=0;
	
	m_LargColone=LARGCOLONE_ACCOGRIL;
	m_HautLigne=HAUTLIGNE_ACCOGRIL;

}

CCampagneGenerique::~CCampagneGenerique()
{

}

void CCampagneGenerique::MajInfoCalendACCOGRIL()
{
	int i;
	int debut,fin,z;

	// on commence par vérifier !
	debut = m_DebutCalend;
	fin   = m_FinCalend;

	if(debut<0) debut=0;
	if(debut>   m_NbSemaine-1) debut=m_NbSemaine-1;
	if(fin<0)   fin=0;
	if(fin>     m_NbSemaine-1) fin=m_NbSemaine-1;
	if (debut>fin){
		z     = debut;
		debut = fin;
		fin   = z;
	}
	m_DebutCalend=debut;
	m_FinCalend=fin;


	m_NbSemaineCalend = m_FinCalend - m_DebutCalend + 1;
	for(i=0;i<m_NbSemaineCalcul;i++){
		if(i>=m_DebutCalend && i<=m_FinCalend)SelectCalend[i]=1;
		else SelectCalend[i]=0;
	}

	PosXCurCalend=m_NrSemaineX;
}
void CCampagneGenerique::BindVecteurCalendrier()
{
	m_NrSemaine.RemoveAll();
	m_NrMois.RemoveAll();
	m_NrAnnee.RemoveAll();

	// En fonction de la date de début,
	// on doit construire le calendrier
	COleDateTime date;
	COleDateTimeSpan span;

	// Objet Calendrier cinema
	CCAlendrierHebdoCinema cal;

	for(int i=0;i<m_NbSemaineCalcul;i++)
	{
		span.SetDateTimeSpan(7*i,0,0,0);
		date = m_DateDebut+span;

		// CORRECTION BUG 20/02/2002:mauvaise correspondance entre mois et numéros de semaine 
		int year=cal.GetAnnee(date);
		int month=cal.GetMois(date);
		int week=cal.GetNumSemaine(date);		

		m_NrSemaine.Add(week);
		m_NrMois.Add(month);
		m_NrAnnee.Add(year);
	}
	SelectCalend.SetSize(m_NbSemaineCalcul);
	MajInfoCalendACCOGRIL();
}

void CCampagneGenerique::CalculCourbeReponse()
{
	double b=m_Beta/100.0;
	for(int c=0;c<NBCLASSE;c++)
	{
		ValBeta[c]=1.0-pow((1.0-b),(1+c));
	}
}


