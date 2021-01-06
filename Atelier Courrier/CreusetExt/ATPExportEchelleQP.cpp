#include "StdAfx.h"
#include ".\atpexportechelleqp.h"
#include "JATPDocExport.h"


CATPExportEchelleQP::CATPExportEchelleQP(void)
{
}

CATPExportEchelleQP::~CATPExportEchelleQP(void)
{
}

VOID CATPExportEchelleQP::OnExportEntete(JATPDocExport * pObjet)
{
	// Export Bloc n° 1 / EXPORT BLOC 1 ECHELLE
	pObjet->m_StrExport += "EXPORT ECHELLE QUICK PLAN\n\n";

	// 1er Bloc : Entete
	pObjet->m_StrExport += "BLOC 1\n";

	// on exporte le titre
	pObjet->m_StrExport +=_T("TITRE:\t");

	// Echelle
	pObjet->m_StrExport += _T("QUICK PLAN\n");

	// Export de l'étude
	this->OnExportEtude(pObjet);

	// Export du produit associé
	//this->OnExportProduit(pObjet);

	// Export de la période
	this->OnExportPeriode(pObjet);

	// Export de la cible
	this->OnExportCible(pObjet);

	// Export Réference affinité
	this->OnExportRefAffinite(pObjet);

	// Export Filtre d'audience
	//this->OnExportFiltreAudience(pObjet);

	// Export Source
	this->OnExportSource(pObjet);

	// Export Terrains
	this->OnExportTerrains(pObjet);

	// Export Segment
	OnExportSegment(pObjet);

	// Export Type de poids
	OnExportTypePoid(pObjet);

	// Export Type de moteur calcul
	OnExportTypeCalcul(pObjet);

	// Export des formats actifs
	OnExportFormats(pObjet);

	// Export des contextes
	//OnExportCtxLecture(pObjet);

}	

VOID CATPExportEchelleQP::OnExportPeriode(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("DATE TARIFAIRE:\t");

	// on récupère les dates de l'étude
	JDate DateD = pObjet->m_DateDebPeriode;
	JDate DateF = pObjet->m_DateFinPeriode;

	// on vérifie la validité des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on décompose les dates
		JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);

		// on exporte les dates
		CString Txt;
		Txt.Format("\"%02d/%02d/%04d\"\n",DayD, MonthD, YearD);
		pObjet->m_StrExport += Txt;
	}
	else
	{
		// on exporte des étoiles // quel romantisme !!!!!
		pObjet->m_StrExport += _T("\"*\"\n");
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//  Export entete supports échelle
VOID CATPExportEchelleQP::OnExportEnteteSupports(JATPDocExport * pObjet)
{
	// Entete Bloc
	CString Txt;
	Txt.Format("%s\n","BLOC 2");
	pObjet->m_StrExport += Txt;

	// on exporte le titre
	pObjet->m_StrExport +=_T("SUPPORTS ECHELLE:\n");

	// Libellé des entetes info support

//	pObjet->m_StrExport += _T("Id Support\tLib Support\t");

	CString str;

	str.LoadString(IDS_CR_SUPPORTS);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_PERIODICITE);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_TARIF);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_LDPLNM);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_GRP);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_INDAFFINITE);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_PRCAFFINITE);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_CTGRP);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

		str.LoadString(IDS_CR_REPET);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_AUDIENCE_ABR);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_CONTACTS_ABR);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_APPORT_COUV);
	pObjet->m_StrExport += "%";
	pObjet->m_StrExport += str;
	// pObjet->m_StrExport += "\t";

	//retourligne
	pObjet->m_StrExport += "\n";
		
	//pObjet->m_StrExport += _T("Id Support\tLib Support\tPeriodicite\tAbr Periodicite\tTarif\tLDP/LNM\tGRP Actif\t");

//	if(pObjet->m_ATPDocCreuset.m_bRepriseEnMain)
//		pObjet->m_StrExport += _T("GRP\tInd. Affinite\t%% Affinite\tCoût GRP\tReprises\tAudience\tContact\n");
//	else
//		pObjet->m_StrExport += _T("GRP\tInd. Affinite\t%% Affinite\tCoût GRP\tNb Jour Lect\tAudience\tContact\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//  Export des supports échelles
VOID CATPExportEchelleQP::OnExportSupportEchelle(CInfoEchSupport * pObjet)
{
	// Export Support
	CString Txt;
	//Txt.Format("%d\t",pObjet->m_IdSupport); 
	//if (Txt == "\t")
	//	Txt = "*\t";
	//pObjet->m_StrExportEch += Txt;

	// Export Libellé Support
	Txt.Format("%s\t",pObjet->m_LabelSupport.AsJCharPtr());
	if (Txt == "\t")
		Txt = "*\t";
	pObjet->m_StrExportEch += Txt;

	// Périodicité Support
	//Txt.Format("%s\t",pObjet->m_LabelPeriodicite.AsJCharPtr());
	//if (Txt == "\t")
	//	Txt = "*\t";
	//pObjet->m_StrExportEch += Txt;

	// Périodicité abrégé support
	Txt.Format("%s\t",pObjet->m_LabelAbrPeriodicite.AsJCharPtr());
	if (Txt == "\t")
		Txt = "*\t";
	pObjet->m_StrExportEch += Txt;
	
	// Tarif support début période
	Txt = "*\t";
	if (pObjet->m_Tarif >= 0)
		Txt.Format("%.0f\t",pObjet->m_Tarif);
	pObjet->m_StrExportEch += Txt;
		
	// LDP/LNM Support
	Txt = "*\t";
	if (pObjet->m_LDPLNM >= 0)
		Txt.Format("%.0f\t",pObjet->m_LDPLNM);
	pObjet->m_StrExportEch += Txt;

	// GRP Support
	Txt = "*\t";
	if (pObjet->m_GRP >= 0)
		Txt.Format("%.1f\t",pObjet->m_GRP);
	pObjet->m_StrExportEch += Txt;

	// Affinite Support
	Txt = "*\t";
	if (pObjet->m_Affinite >= 0)
		Txt.Format("%.0f\t",pObjet->m_Affinite);
	pObjet->m_StrExportEch += Txt;

	// % Affinité support
	Txt = "*\t";
	if (pObjet->m_PrctAffinite >= 0)
		Txt.Format("%.1f\t",pObjet->m_PrctAffinite);
	pObjet->m_StrExportEch += Txt;

	// Coût GRP Support
	Txt = "*\t";
	if (pObjet->m_CoutGRP >= 0)
		Txt.Format("%.2f\t",pObjet->m_CoutGRP);
	pObjet->m_StrExportEch += Txt;

	// Reprises support
	Txt = "*\t";
	if (pObjet->m_Reprise >= 0)
		Txt.Format("%.2f\t",pObjet->m_Reprise);
	pObjet->m_StrExportEch += Txt;

	//// Nb Jours Lecture Support
	//Txt = "*\t";
	//if (pObjet->m_NbJourLect >= 0)
	//	Txt.Format("%f\t",pObjet->m_NbJourLect);
	//pObjet->m_StrExportEch += Txt;

	// Audience Support
	Txt = "*\t";
	if (pObjet->m_Audience >= 0)
		Txt.Format("%.0f\t",pObjet->m_Audience/1000.0);
	pObjet->m_StrExportEch += Txt;

	// Contacts Support
	Txt = "*\t";
	if (pObjet->m_Contact >= 0)
		Txt.Format("%.0f\t",pObjet->m_Contact/1000.0);
	pObjet->m_StrExportEch += Txt;
}
