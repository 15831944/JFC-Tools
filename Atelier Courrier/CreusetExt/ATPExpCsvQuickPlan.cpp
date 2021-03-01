#include "StdAfx.h"
#include ".\atpexpcsvquickplan.h"
#include "JATPDocExport.h"

CATPExpCsvQuickPlan::CATPExpCsvQuickPlan(void)
{
}

CATPExpCsvQuickPlan::~CATPExpCsvQuickPlan(void)
{
}

void CATPExpCsvQuickPlan::VisitBegin(JATPDocExport * pObjet)
{
	try
	{
		// Export No de version
		OnExportNoVersion(pObjet);

		// Export de l'entete
		OnExportEntete(pObjet);

		// Saut de ligne
		OnExportSTLigne(pObjet,1);

		//OnExportCourbeReponse(pObjet);

		// Saut de ligne
		//OnExportSTLigne(pObjet,1);
	}
	catch (...)
	{
		// on propage l'exception
		throw;
	}
}

void CATPExpCsvQuickPlan::VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
	// Export D�but Hypoth�se
	CString Txt;
	Txt.Format("%s %d\n","HYPOTHESE",IndexHypo); 
	pObjet->m_StrExportHypo = Txt;

	// Export Libell� hypoth�se
	Txt.Format("%s\n\n",pObjet->m_LabelHypo.AsJCharPtr());
	if (Txt == "\n\n")
		Txt = "Libell� hypoth�se non d�fini\n\n";
	pObjet->m_StrExportHypo += Txt;

	// Export Bloc n� 2 / EXPORT BLOC 2
	Txt.Format("%s\n","BLOC 2",IndexHypo);
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("SELECTION HYPOTHESES:\n");

}

void CATPExpCsvQuickPlan::Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
	// Export Bloc Hypoth�ses
	this->OnExportHypothese(pObjet,IndexHypo);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);


	// export Bloc R�sultats Hypoth�se
	// Export Bloc n� 3 / EXPORT BLOC 3 TEST PLAN
	CString Txt;
	Txt.Format("%s\n","BLOC 3");
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("RESULTATS HYPOTHESES:\n");

	OnExportResultHypothese(pObjet,IndexHypo, pDocExport);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);

	// export Bloc Distribution Cumulee Hypoth�se
	Txt.Format("%s\n","BLOC 4");
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("DISTRIBUTION DE CONTACT CUMULEE:\n");

	this->OnExportDistriCumHypothese(pObjet,IndexHypo);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);

	// export Bloc Distribution Nette Hypoth�se
	Txt.Format("%s\n","BLOC 5");
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("DISTRIBUTION DE CONTACT NETTE:\n");

	this->OnExportDistriNetteHypothese(pObjet,IndexHypo);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);

//	pObjet->m_StrExportHypo += pDocExport->m_ATPDocCreuset.m_AFFVentilation.ExportVentil(IndexHypo);

}

VOID CATPExpCsvQuickPlan::OnExportEntete(JATPDocExport * pObjet)
{
	// Export Bloc n� 1 / EXPORT BLOC 1 TEST PLAN
	pObjet->m_StrExport += "EXPORT QUICK PLAN\n\n";

	// 1er Bloc : Entete
	pObjet->m_StrExport += "BLOC 1\n";

	// on exporte le titre
	pObjet->m_StrExport +=_T("TITRE:\t");

	// Test de plans
	pObjet->m_StrExport += _T("QUICK PLAN\n");

	// Export de l'�tude
	OnExportEtude(pObjet);

	// Export du produit associ�
//	this->OnExportProduit(pObjet);

	// Export de la p�riode
	OnExportPeriode(pObjet);

	// Export de la cible
	OnExportCible(pObjet);

	// Export R�ference affinit�
	OnExportRefAffinite(pObjet);

	// Export Filtre d'audience
	//OnExportFiltreAudience(pObjet);

	// Export Source
	OnExportSource(pObjet);

	// Export Terrains
	OnExportTerrains(pObjet);

	// Export Segment
	OnExportSegment(pObjet);

	// Export Type de poids
	OnExportTypePoid(pObjet);

	// Export Type de moteur calcul
	OnExportTypeCalcul(pObjet);

	// Export liste des formats actifs
	OnExportFormats(pObjet);

	//OnExportCtxLecture(pObjet);

}

VOID CATPExpCsvQuickPlan::OnExportPeriode(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("DATE TARIFAIRE:\t");

	// on r�cup�re les dates de l'�tude
	JDate DateD = pObjet->m_DateDebPeriode;
	JDate DateF = pObjet->m_DateFinPeriode;

	// on v�rifie la validit� des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on d�compose les dates
		JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);

		// on exporte les dates
		CString Txt;
		Txt.Format("\"%02d/%02d/%04d\"\n",DayD, MonthD, YearD);
		pObjet->m_StrExport += Txt;
	}
	else
	{
		// on exporte des �toiles // quel romantisme !!!!!
		pObjet->m_StrExport += _T("\"*\"\n");
	}
}




VOID CATPExpCsvQuickPlan::OnExportResultHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
	//------------------------------------- Export Titre Total Insertions ---------------------------------- 
	//
	// Titre entete ligne
	CString Txt;
	Txt.LoadString(IDS_CR_TOTINS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture nb insertions hypoth�se dans buffer
	Txt.Format("%d\n",pObjet->m_TotInsertions);
	pObjet->m_StrExportHypo += Txt;			

	//------------------------------------- Export Titre Budget Brut ---------------------------------- 
	//
	// Titre entete ligne
	Txt.LoadString(IDS_CR_BUDGET);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
		
	// Ecriture budget brut hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_BudgetBrut >= 0)
		Txt.Format("%f\n",pObjet->m_BudgetBrut);
	pObjet->m_StrExportHypo += Txt;			

	// -------------------------------------- Export Audience (Nb Individus touch�s) -------------------
	//
	// Titre entete ligne
	Txt.LoadString(IDS_CR_AUDIENCE);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture audience brut hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Audience > 0)
		Txt.Format("%.0f\n",pObjet->m_Audience/1000.0);
	pObjet->m_StrExportHypo += Txt;			

	// -------------------------------------- Export Contacts actifs ---------------------------
	//
	// Titre entete ligne
	
	Txt.LoadString(IDS_CR_CONTACTS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Contacts actifs hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_ContactActif >= 0)
		Txt.Format("%.0f\n",pObjet->m_ContactActif/1000.0);
	pObjet->m_StrExportHypo += Txt;			

	//-------------------------------------------- Export GRP ---------------------------------- 
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_GRP);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture GRP hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Grp >= 0)
		Txt.Format("%f\n",pObjet->m_Grp);
	pObjet->m_StrExportHypo += Txt;			

	//---------------------------------------- Export GRP Actif -------------------------------- 
	//									
	// Titre entete ligne
	//Txt.LoadString(IDS_CR_GRPACTIF);
	//pObjet->m_StrExportHypo += _T(Txt + "\t");

	//// Ecriture GRP Actif hypothese dans buffer
	//Txt = "*\n";
	//if (pObjet->m_GrpActif >= 0)
	//	Txt.Format("%f\n",pObjet->m_GrpActif);
	//pObjet->m_StrExportHypo += Txt;			

	//----------------------------------- Export Couverture Cumul�e 1+ % -----------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_COUVERTURE);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture Couverture Cumulee � 1+ % hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Couverture > 0)
		Txt.Format("%f\n",pObjet->m_Couverture);
	pObjet->m_StrExportHypo += Txt;			

	//----------------------------------------- Export R�p�tition ------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_REPET);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture R�p�tition hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Repetition >= 0)
		Txt.Format("%f\n",pObjet->m_Repetition);
	pObjet->m_StrExportHypo += Txt;			

	//----------------------------------------- Export R�p�tition [+] ------------------------------
	//									
	// Titre entete ligne
	//Txt.LoadString(IDS_CR_REPETPLUS);
	//pObjet->m_StrExportHypo += _T(Txt + "\t");

	//// Ecriture R�p�tition hypothese dans buffer
	//Txt = "*\n";
	//if (pObjet->m_RepetitionPlus >= 0)
	//	Txt.Format("%f\n",pObjet->m_RepetitionPlus);
	//pObjet->m_StrExportHypo += Txt;			

	//----------------------------------- Export Couverture m�moris�e %  -----------------------
	//									
	// Titre entete ligne
	//Txt.LoadString(IDS_CR_COUVMEM);
	//pObjet->m_StrExportHypo += _T(Txt + "\t");
	//
	//// Ecriture Couverture M�moris�e hypothese dans buffer
	//Txt = "*\n";
	//if (pObjet->m_CouvMemo >= 0)
	//	Txt.Format("%f\n",pObjet->m_CouvMemo);
	//pObjet->m_StrExportHypo += Txt;			

	//--------------------------------------- Export Co�t/GRP ----------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CTGRP);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/GRP hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_CoutGRP >= 0)
		Txt.Format("%f\n",pObjet->m_CoutGRP);
	pObjet->m_StrExportHypo += Txt;			

	//--------------------------------------- Export Co�t/%couv ----------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CTCOUVERTURE);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/%couv hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_CoutCouv >= 0)
		Txt.Format("%f\n",pObjet->m_CoutCouv);
	pObjet->m_StrExportHypo += Txt;			

	//----------------------------------- Export Co�t/1000 individus ----------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CT1000INDIVIDUS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/1000 individus hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Cout1000Indiv >= 0)
		Txt.Format("%f\n",pObjet->m_Cout1000Indiv);
	pObjet->m_StrExportHypo += Txt;			

	//------------------------------------- Export Co�t/1000 contacts ---------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CT1000CONTACTS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/1000 contacts hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Cout1000Contact >= 0)
		Txt.Format("%f\n",pObjet->m_Cout1000Contact);
	pObjet->m_StrExportHypo += Txt;			

}