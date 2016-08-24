// CAlendrierHebdoCinema.cpp: implementation of the CCAlendrierHebdoCinema class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CAlendrierHebdoCinema.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCAlendrierHebdoCinema::CCAlendrierHebdoCinema()
{

}

CCAlendrierHebdoCinema::~CCAlendrierHebdoCinema()
{

}



//Deux r�gles
// 1/ puisque ce sont des semaine Mercredi/Mardi, c'est donc le Samedi (J+3)
// qui est le pivot pour d�terminer le mois et l'ann�e
// 2/ on prend toujr le num�ro de semaine du samedi en ajoutant 1 si le premier samedi de l'ann�e est � 0

int CCAlendrierHebdoCinema::GetSemaine(COleDateTime date)
{
	if(date.GetDayOfWeek()!=4)
	{
		AfxMessageBox("Erreur la date n'est pas un mercredi");
		ASSERT(0);
		return(0);
	}

	// Le num�ro de semaine d�pend du samedi
	COleDateTimeSpan span(3,0,0,0);
	date+=span;
	int week=atoi(date.Format("%W"));

	// De plus si le premier mercredi de l'Ann�e commence par z�ro il faut ajouter 1
	// (pour tester il suffit d'utiliser l'ann�e 2002)
	int an=date.GetYear();
	COleDateTime PremierJanvier(an,1,1,0,0,0);
	int nbjouravantsamedi=7-PremierJanvier.GetDayOfWeek();
	if(nbjouravantsamedi<0)nbjouravantsamedi+=7;
	span.SetDateTimeSpan(nbjouravantsamedi,0,0,0);
	COleDateTime PremierSamedi=PremierJanvier+span;
	int NrSemainePremierSamedi=atoi(PremierSamedi.Format("%W"));
	ASSERT(NrSemainePremierSamedi==0 || NrSemainePremierSamedi==1);
	if(NrSemainePremierSamedi==0)week++;

	return(week);

}
int CCAlendrierHebdoCinema::GetMois(COleDateTime date)
{
	if(date.GetDayOfWeek()!=4)
	{
		AfxMessageBox("Erreur la date n'est pas un mercredi");
		ASSERT(0);
		return(0);
	}

	// Le mois d�pend du samedi
	COleDateTimeSpan span(3,0,0,0);
	date+=span;
	int mois=date.GetMonth();
	return(mois);
}

int CCAlendrierHebdoCinema::GetAnnee(COleDateTime date)
{
	if(date.GetDayOfWeek()!=4)
	{
		AfxMessageBox("Erreur la date n'est pas un mercredi");
		ASSERT(0);
		return(0);
	}

	// L'ann�e d�pend du samedi
	COleDateTimeSpan span(3,0,0,0);
	date+=span;
	int an=date.GetYear();
	return(an);
}
