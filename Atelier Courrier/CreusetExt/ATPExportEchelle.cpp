#include "StdAfx.h"
#include ".\atpexportechelle.h"
#include "JATPDocExport.h"
#include "resource.h"

// Constructeur
CATPExportEchelle::CATPExportEchelle(void)
{
}

// Destructeur
CATPExportEchelle::~CATPExportEchelle(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Export Entete Echelle
void CATPExportEchelle::VisitBegin(JATPDocExport * pObjet)
{
	try
	{
		// Export No de version
		this->OnExportNoVersion(pObjet);

		// Export de l'entete
		this->OnExportEntete(pObjet);

		// Saut de ligne
		this->OnExportSTLigne(pObjet,1);

		// Export Entete Bloc Supports
		this->OnExportEnteteSupports(pObjet);
	}
	catch (...)
	{
		// on propage l'exception
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Pied de page echelle
void CATPExportEchelle::VisitEnd  (JATPDocExport * pObjet)
{
	// Nothing for moment
	// A FAIRE
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Debut Export des supports
void CATPExportEchelle::VisitBegin(CInfoEchSupport * pObjet)
{
}

// Export des supports
void CATPExportEchelle::Visit(CInfoEchSupport * pObjet)
{
	// ------------------------------- Export des supports échelles -----------------------------------
	this->OnExportSupportEchelle(pObjet);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);
}	

// Fin Export des supports
void CATPExportEchelle::VisitEnd(CInfoEchSupport * pObjet)
{
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter le n° de version de l'export Test Plan
VOID CATPExportEchelle::OnExportNoVersion(JATPDocExport * pObjet)
{
	// Ecriture n° de version dans buffer
	pObjet->m_StrExport = "VERSION 1";
	this->OnExportSTLigne(pObjet,1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter l'entete Atelier Creuset
VOID CATPExportEchelle::OnExportEntete(JATPDocExport * pObjet)
{
	// Export Bloc n° 1 / EXPORT BLOC 1 ECHELLE
	pObjet->m_StrExport += "EXPORT ECHELLE\n\n";

	// 1er Bloc : Entete
	pObjet->m_StrExport += "BLOC 1\n";

	// on exporte le titre
	pObjet->m_StrExport +=_T("TITRE:\t");

	// Echelle
	pObjet->m_StrExport += _T("ECHELLE\n");

	// Export de l'étude
	this->OnExportEtude(pObjet);

	// Export du produit associé
	this->OnExportProduit(pObjet);

	// Export de la période
	this->OnExportPeriode(pObjet);

	// Export de la cible
	this->OnExportCible(pObjet);

	// Export Réference affinité
	this->OnExportRefAffinite(pObjet);

	// Export Filtre d'audience
	this->OnExportFiltreAudience(pObjet);

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
	OnExportCtxLecture(pObjet);

}	

///////////////////////////////////////////
// Export de la période et période action
VOID CATPExportEchelle::OnExportPeriode(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("PERIODE:\t");

	// on récupère les dates de l'étude
	JDate DateD = pObjet->m_DateDebPeriode;
	JDate DateF = pObjet->m_DateFinPeriode;

	// on vérifie la validité des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on décompose les dates
		JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);
		JInt32 DayF, MonthF, YearF; DateF.GetDate(DayF, MonthF, YearF);

		// on exporte les dates
		CString Txt;
		Txt.Format("\"%02d/%02d/%04d\"\t\"%02d/%02d/%04d\"\n",DayD, MonthD, YearD, DayF, MonthF, YearF);
		pObjet->m_StrExport += Txt;
	}
	else
	{
		// on exporte des étoiles // quel romantisme !!!!!
		pObjet->m_StrExport += _T("\"*\"\t\"*\"\n");
	}

	// on exporte le titre
	pObjet->m_StrExport += _T("PERIODE ACTION:\t");

	// on récupère les dates de la période action
	DateD = pObjet->m_DateDebPeriodeAction;
	DateF = pObjet->m_DateFinPeriodeAction;

	// on vérifie la validité des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on décompose les dates
		JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);
		JInt32 DayF, MonthF, YearF; DateF.GetDate(DayF, MonthF, YearF);

		// on exporte les dates
		CString Txt;
		Txt.Format("\"%02d/%02d/%04d\"\t\"%02d/%02d/%04d\"\n",DayD, MonthD, YearD, DayF, MonthF, YearF);
		pObjet->m_StrExport += Txt; 
	}
	else
	{
		// on exporte des étoiles / encore du romantisme !!!!
		pObjet->m_StrExport +=_T("\"*\"\t\"*\"\n");
	}

}

///////////////////////////////////////////////////
// Export de la cible
VOID CATPExportEchelle::OnExportCible(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport +=_T("CIBLE:\t");

	// on exporte les infos cibles
	CString Txt;
	Txt.Format("\"%s\"\t\"%d\"\t\"%d\"\n",pObjet->m_LabelCible.AsJCharPtr(), pObjet->m_EffectifCible,pObjet->m_EchantillonCible);
	pObjet->m_StrExport += Txt;
}
///////////////////////////////////////////////////////////////////////////
// Export Réference affinité
VOID CATPExportEchelle::OnExportRefAffinite(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("REFERENCE AFFINITE:\t");
	
	// on exporte l'affinité
	CString Txt;
	Txt.Format("\"%s\"\n",pObjet->m_LabelRefAffinite.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export du filtre d'audience
VOID CATPExportEchelle::OnExportFiltreAudience(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("FILTRE AUDIENCE:\t");

	// Ecriture Libellé filtre audience dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelFiltreAudience.AsJCharPtr()); 
	pObjet->m_StrExport += Txt; 
}	


//////////////////////////////////////////////
// Export de la Source utilisée
VOID CATPExportEchelle::OnExportSource(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("SOURCE:\t");

	// Ecriture libellé de la source dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelSource.AsJCharPtr());  
	pObjet->m_StrExport += Txt;

}

//////////////////////////////////////////////
// Export des Terrains utilisés
VOID CATPExportEchelle::OnExportTerrains(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("TERRAIN:\t");

	// ecriture libellé des terrains dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelTerrain.AsJCharPtr());
	pObjet->m_StrExport += Txt;
	
}

//////////////////////////////////////////////
// Export Segment
VOID CATPExportEchelle::OnExportSegment(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("SEGMENT:\t");

	// écriture libellé segment dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelSegment.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export Type de poids
VOID CATPExportEchelle::OnExportTypePoid(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("TYPE POIDS:\t");

	// écriture libellé type de poids dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelTypePoids.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export Type de moteur calcul
VOID CATPExportEchelle::OnExportTypeCalcul(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("METHODE DE CALCUL DES COUVERTURES:\t");

	// Ecriture libellé moteur de calcul dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelMethodeCalcCouv.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export des Formats utilisés
VOID CATPExportEchelle::OnExportFormats(JATPDocExport * pObjet)
{
	// Titre
	pObjet->m_StrExport += _T("FORMATS ACTIFS:\t");

	// Liste des formats actifs
	CString Txt;
	for (pObjet->m_ListFormatActif.MoveFirst();
		 pObjet->m_ListFormatActif.IsValid();
		 pObjet->m_ListFormatActif.MoveNext())
	{
		// Ecriture ident et libellé format dans buffer
		Txt.Format("%d\t",pObjet->m_ListFormatActif.GetItem());
		pObjet->m_StrExport += Txt;

		// puis les libellés format
		JString LblCompletFormat = "*";
		JString LblAbregeFormat  = "*";
		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
		{
			// libellé long format
			LblCompletFormat = pSrcPresse->m_TBLFormats.GetLabelByID(pObjet->m_ListFormatActif.GetItem()); 

			// puis le libellé abrégé format
			LblAbregeFormat = pSrcPresse->m_TBLFormats.GetLabelCourtByID(pObjet->m_ListFormatActif.GetItem()); 
		}

		// Ecriture libellé complet format dans buffer
		Txt.Format("%s\t",LblCompletFormat.AsJCharPtr());
		pObjet->m_StrExport += Txt;

		// Ecriture libellé abrégé format dans buffer
		Txt.Format("%s\t",LblAbregeFormat.AsJCharPtr());
		pObjet->m_StrExport += Txt;
	}

	// saut de ligne
	this->OnExportSTLigne(pObjet,1);
}

///////////////////////////////////////////////////
// Export de l'étude
VOID CATPExportEchelle::OnExportEtude(JATPDocExport * pObjet)
{
	// Export titre étude
	pObjet->m_StrExport += _T("ETUDE:\t");

	// Ecriture libellé étude dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelEtude.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

///////////////////////////////////////////////////
// Export du produit associé
VOID CATPExportEchelle::OnExportProduit(JATPDocExport * pObjet)
{
	// Export titre groupe
	pObjet->m_StrExport += _T("GROUPE:\t");

	// Ecriture libellé groupe dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelGroupe.AsJCharPtr());
	pObjet->m_StrExport += Txt;

	// Export titre annonceur
	pObjet->m_StrExport += _T("ANNONCEUR:\t");

	// Ecriture libellé annonceur dans buffer
	Txt.Format("%s\n",pObjet->m_LabelAnnonceur.AsJCharPtr());
	pObjet->m_StrExport += Txt;

	// Export titre produit
	pObjet->m_StrExport += _T("PRODUIT:\t");

	// Ecriture libellé produit dans buffer
	Txt.Format("%s\n",pObjet->m_LabelProduit.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//  Export entete supports échelle
VOID CATPExportEchelle::OnExportEnteteSupports(JATPDocExport * pObjet)
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

	// Header Text Libellé Support
	str.LoadString(IDS_CR_SUPPORTS);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// Header Text Périodicité Support
	str.LoadString(IDS_CR_PERIODICITE);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// Header Text Tarif Support
	str.LoadString(IDS_CR_TARIF);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// // Header Text LDP/LNM Support
	str.LoadString(IDS_CR_LDPLNM);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	if(pObjet->m_GRPActifValid)
	{
		// Header Text GRP Actif (ssi moteur macro-probabiliste)
		str.LoadString(IDS_CR_GRPACTIF);
		pObjet->m_StrExport += str;
		pObjet->m_StrExport += "\t";
	}

	// Header Text GRP Support
	str.LoadString(IDS_CR_GRP);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// Header Text Indice Affinité Support
	str.LoadString(IDS_CR_INDAFFINITE);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// Header Text % Affinité Support
	str.LoadString(IDS_CR_PRCAFFINITE);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// Header Text Coût GRP Support
	str.LoadString(IDS_CR_CTGRP);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	// Header Text Répétition + Support (seulement si moteur macro-probabiliste)
	if(pObjet->m_GRPActifValid)
		str.LoadString(IDS_CR_REPETPLUS);
	else
		str.LoadString(IDS_CR_REPET);
		

	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_AUDIENCE_ABR);
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	if(pObjet->m_GRPActifValid)
		str.LoadString(IDS_CR_CONTACTSPLUS_ABR);
	else
		str.LoadString(IDS_CR_CONTACTS_ABR);
		
	pObjet->m_StrExport += str;
	pObjet->m_StrExport += "\t";

	str.LoadString(IDS_CR_APPORT_COUV);
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
VOID CATPExportEchelle::OnExportSupportEchelle(CInfoEchSupport * pObjet)
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
		Txt.Format("%.0f\t",(JFlt64)pObjet->m_Tarif);
	pObjet->m_StrExportEch += Txt;
		
	// LDP/LNM Support
	Txt = "*\t";
	if (pObjet->m_LDPLNM >= 0)
		Txt.Format("%.0f\t",pObjet->m_LDPLNM);
	pObjet->m_StrExportEch += Txt;

	/*
	// GRP Actif Support
	Txt = "*\t";
	if (pObjet->m_GRPActifValid && pObjet->m_GRPActif >= 0)
		Txt.Format("%.1f\t",pObjet->m_GRPActif);
	pObjet->m_StrExportEch += Txt;

	// GRP Support
	Txt = "*\t";
	if (pObjet->m_GRP >= 0)
		Txt.Format("%.1f\t",pObjet->m_GRP);
	pObjet->m_StrExportEch += Txt;
	*/

	// GRP Actif + Support (seulement si moteur macro-probabiliste)
	if(pObjet->m_GRPActifValid)
	{
		// GRP Actif Support
		Txt = "*\t";
		if (pObjet->m_GRPActifValid && pObjet->m_GRPActif >= 0)
			Txt.Format("%.1f\t",pObjet->m_GRPActif);
		pObjet->m_StrExportEch += Txt;
	}

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

	// Apport Support
	Txt = "*\t";
	if (pObjet->m_Apport >= 0)
		Txt.Format("%.1f\t",pObjet->m_Apport);
	pObjet->m_StrExportEch += Txt;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Saut de lignes
VOID CATPExportEchelle::OnExportSTLigne(JATPDocExport * pObjet,const JUnt32 NbLigne)
{
	for (JUnt32 i = 0; i < NbLigne; i++)
		// saut de ligne
		pObjet->m_StrExport += _T("\n");
}

VOID CATPExportEchelle::OnExportSTLigne(CInfoEchSupport * pObjet,const JUnt32 NbLigne)
{
	for (JUnt32 i = 0; i < NbLigne; i++)
		// saut de ligne
		pObjet->m_StrExportEch += _T("\n");
}

// Export contxtes
VOID CATPExportEchelle::OnExportCtxLecture(JATPDocExport * pObjet)
{
	// Le contexte de lecture
	pObjet->m_StrExport += "\nCONTEXTE DE LECTURE \n";
	pObjet->m_StrExport += "Aucun contexte";
	pObjet->m_StrExport += "\n";

}

