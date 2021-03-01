#include "StdAfx.h"
#include ".\atpexpcsvtestplan.h"
#include "JATPDocExport.h"
#include "Courbes.h"

// Constructeur
CATPExpCsvTestPlan::CATPExpCsvTestPlan(void)
{
}

// Destructeur
CATPExpCsvTestPlan::~CATPExpCsvTestPlan(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Export Entete Plan Test
void CATPExpCsvTestPlan::VisitBegin(JATPDocExport * pObjet)
{
	try
	{
		// Export No de version
		this->OnExportNoVersion(pObjet);

		// Export de l'entete
		this->OnExportEntete(pObjet);

		// Saut de ligne
		this->OnExportSTLigne(pObjet,1);

		this->OnExportCourbeReponse(pObjet);

		// Saut de ligne
		this->OnExportSTLigne(pObjet,1);
	}
	catch (...)
	{
		// on propage l'exception
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Pied de page Plan Test
void CATPExpCsvTestPlan::VisitEnd  (JATPDocExport * pObjet)
{
	// Nothing for moment
	// A FAIRE
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export d�but hypoth�se
void CATPExpCsvTestPlan::VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
	// Export D�but Hypoth�se
	CString Txt;
	Txt.Format("%s %d\n","HYPOTHESE",IndexHypo); 
	pObjet->m_StrExportHypo = Txt;

	// Export Libell� hypoth�se
	Txt.Format("%s\n\n",pObjet->m_LabelHypo.AsJCharPtr());
	if (Txt == "\n\n")
		// Txt = "Libell� hypoth�se non d�fini\n\n";
		Txt.Format("%s %d\n\n","Hypoth�se",IndexHypo);
	pObjet->m_StrExportHypo += Txt;

	// Export Bloc n� 2 / EXPORT BLOC 2
	Txt.Format("%s\n","BLOC 2",IndexHypo);
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("SELECTION HYPOTHESES:\n");

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export hypoth�se
void CATPExpCsvTestPlan::Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
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

	this->OnExportResultHypothese(pObjet,IndexHypo, pDocExport);

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

	pObjet->m_StrExportHypo += pDocExport->m_ATPDocCreuset.m_AFFVentilation.ExportVentil(IndexHypo);
	pObjet->m_StrExportHypo += pDocExport->m_ATPDocCreuset.m_AFFContribution_New.ExportContrib(IndexHypo);

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Fin hypoth�se
void CATPExpCsvTestPlan::VisitEnd(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter le n� de version de l'export Test Plan
VOID CATPExpCsvTestPlan::OnExportNoVersion(JATPDocExport * pObjet)
{
	// Ecriture n� de version dans buffer
	pObjet->m_StrExport = "VERSION 1";
	this->OnExportSTLigne(pObjet,1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter l'entete Atelier Creuset
VOID CATPExpCsvTestPlan::OnExportEntete(JATPDocExport * pObjet)
{
	// Export Bloc n� 1 / EXPORT BLOC 1 TEST PLAN
	pObjet->m_StrExport += "EXPORT TEST PLAN\n\n";

	// 1er Bloc : Entete
	pObjet->m_StrExport += "BLOC 1\n";

	// on exporte le titre
	pObjet->m_StrExport +=_T("TITRE:\t");

	// Test de plans
	pObjet->m_StrExport += _T("TEST DE PLAN\n");

	// Export de l'�tude
	this->OnExportEtude(pObjet);

	// Export du produit associ�
	this->OnExportProduit(pObjet);

	// Export de la p�riode
	this->OnExportPeriode(pObjet);

	// Export de la cible
	this->OnExportCible(pObjet);

	// Export R�ference affinit�
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

	// Export liste des formats actifs
	OnExportFormats(pObjet);

	OnExportCtxLecture(pObjet);


}	

///////////////////////////////////////////
// Export de la p�riode et p�riode action
VOID CATPExpCsvTestPlan::OnExportPeriode(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("PERIODE:\t");

	// on r�cup�re les dates de l'�tude
	JDate DateD = pObjet->m_DateDebPeriode;
	JDate DateF = pObjet->m_DateFinPeriode;

	// on v�rifie la validit� des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on d�compose les dates
		JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);
		JInt32 DayF, MonthF, YearF; DateF.GetDate(DayF, MonthF, YearF);

		// on exporte les dates
		CString Txt;
		Txt.Format("\"%02d/%02d/%04d\"\t\"%02d/%02d/%04d\"\n",DayD, MonthD, YearD, DayF, MonthF, YearF);
		pObjet->m_StrExport += Txt;
	}
	else
	{
		// on exporte des �toiles // quel romantisme !!!!!
		pObjet->m_StrExport += _T("\"*\"\t\"*\"\n");
	}

	// on exporte le titre
	pObjet->m_StrExport += _T("PERIODE ACTION:\t");

	// on r�cup�re les dates de la p�riode action
	DateD = pObjet->m_DateDebPeriodeAction;
	DateF = pObjet->m_DateFinPeriodeAction;

	// on v�rifie la validit� des dates
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on d�compose les dates
		JInt32 DayD, MonthD, YearD; DateD.GetDate(DayD, MonthD, YearD);
		JInt32 DayF, MonthF, YearF; DateF.GetDate(DayF, MonthF, YearF);

		// on exporte les dates
		CString Txt;
		Txt.Format("\"%02d/%02d/%04d\"\t\"%02d/%02d/%04d\"\n",DayD, MonthD, YearD, DayF, MonthF, YearF);
		pObjet->m_StrExport += Txt; 
	}
	else
	{
		// on exporte des �toiles / encore du romantisme !!!!
		pObjet->m_StrExport +=_T("\"*\"\t\"*\"\n");
	}

}

///////////////////////////////////////////////////
// Export de la cible
VOID CATPExpCsvTestPlan::OnExportCible(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport +=_T("CIBLE:\t");

	// on exporte les infos cibles
	CString Txt;
	Txt.Format("\"%s\"\t\"%d\"\t\"%d\"\n",pObjet->m_LabelCible.AsJCharPtr(), pObjet->m_EffectifCible,pObjet->m_EchantillonCible);
	pObjet->m_StrExport += Txt;
}
///////////////////////////////////////////////////////////////////////////
// Export R�ference affinit�
VOID CATPExpCsvTestPlan::OnExportRefAffinite(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("REFERENCE AFFINITE:\t");
	
	// on exporte l'affinit�
	CString Txt;
	Txt.Format("\"%s\"\n",pObjet->m_LabelRefAffinite.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export du filtre d'audience
VOID CATPExpCsvTestPlan::OnExportFiltreAudience(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("FILTRE AUDIENCE:\t");

	// Ecriture Libell� filtre audience dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelFiltreAudience.AsJCharPtr()); 
	pObjet->m_StrExport += Txt; 
}	


//////////////////////////////////////////////
// Export de la Source utilis�e
VOID CATPExpCsvTestPlan::OnExportSource(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("SOURCE:\t");

	// Ecriture libell� de la source dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelSource.AsJCharPtr());  
	pObjet->m_StrExport += Txt;

}

//////////////////////////////////////////////
// Export des Terrains utilis�s
VOID CATPExpCsvTestPlan::OnExportTerrains(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("TERRAIN:\t");

	// ecriture libell� des terrains dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelTerrain.AsJCharPtr());
	pObjet->m_StrExport += Txt;
	
}

//////////////////////////////////////////////
// Export Segment
VOID CATPExpCsvTestPlan::OnExportSegment(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("SEGMENT:\t");

	// �criture libell� segment dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelSegment.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export Type de poids
VOID CATPExpCsvTestPlan::OnExportTypePoid(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("TYPE POIDS:\t");

	// �criture libell� type de poids dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelTypePoids.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export Type de moteur calcul
VOID CATPExpCsvTestPlan::OnExportTypeCalcul(JATPDocExport * pObjet)
{
	// on exporte le titre
	pObjet->m_StrExport += _T("METHODE DE CALCUL DES COUVERTURES:\t");

	// Ecriture libell� moteur de calcul dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelMethodeCalcCouv.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////
// Export des Formats utilis�s
VOID CATPExpCsvTestPlan::OnExportFormats(JATPDocExport * pObjet)
{
	// Titre
	pObjet->m_StrExport += _T("FORMATS ACTIFS:\t");

	// Liste des formats actifs
	CString Txt;
	for (pObjet->m_ListFormatActif.MoveFirst();
		 pObjet->m_ListFormatActif.IsValid();
		 pObjet->m_ListFormatActif.MoveNext())
	{
		// Ecriture ident et libell� format dans buffer
		Txt.Format("%d\t",pObjet->m_ListFormatActif.GetItem());
		pObjet->m_StrExport += Txt;

		// puis les libell�s format
		JString LblCompletFormat = "*";
		JString LblAbregeFormat  = "*";
		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
		{
			// libell� long format
			LblCompletFormat = pSrcPresse->m_TBLFormats.GetLabelByID(pObjet->m_ListFormatActif.GetItem()); 

			// puis le libell� abr�g� format
			LblAbregeFormat = pSrcPresse->m_TBLFormats.GetLabelCourtByID(pObjet->m_ListFormatActif.GetItem()); 
		}

		// Ecriture libell� complet format dans buffer
		Txt.Format("%s\t",LblCompletFormat.AsJCharPtr());
		pObjet->m_StrExport += Txt;

		// Ecriture libell� abr�g� format dans buffer
		Txt.Format("%s\t",LblAbregeFormat.AsJCharPtr());
		pObjet->m_StrExport += Txt;
	}

	// saut de ligne
	this->OnExportSTLigne(pObjet,1);
}

///////////////////////////////////////////////////
// Export de l'�tude
VOID CATPExpCsvTestPlan::OnExportEtude(JATPDocExport * pObjet)
{
	// Export titre �tude
	pObjet->m_StrExport += _T("ETUDE:\t");

	// Ecriture libell� �tude dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelEtude.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

///////////////////////////////////////////////////
// Export du produit associ�
VOID CATPExpCsvTestPlan::OnExportProduit(JATPDocExport * pObjet)
{
	// Export titre groupe
	pObjet->m_StrExport += _T("GROUPE:\t");

	// Ecriture libell� groupe dans buffer
	CString Txt;
	Txt.Format("%s\n",pObjet->m_LabelGroupe.AsJCharPtr());
	pObjet->m_StrExport += Txt;

	// Export titre annonceur
	pObjet->m_StrExport += _T("ANNONCEUR:\t");

	// Ecriture libell� annonceur dans buffer
	Txt.Format("%s\n",pObjet->m_LabelAnnonceur.AsJCharPtr());
	pObjet->m_StrExport += Txt;

	// Export titre produit
	pObjet->m_StrExport += _T("PRODUIT:\t");

	// Ecriture libell� produit dans buffer
	Txt.Format("%s\n",pObjet->m_LabelProduit.AsJCharPtr());
	pObjet->m_StrExport += Txt;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 
VOID CATPExpCsvTestPlan::OnExportHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo)
{
	for (pObjet->m_TInfoSupports.MoveFirst();
		 pObjet->m_TInfoSupports.IsValid();
		 pObjet->m_TInfoSupports.MoveNext())
	{
		// Eciture info support si au moins 1 insertion
		if (pObjet->m_TInfoSupports.GetItem().m_NbInsertions > 0)
		{
			// Ecriture libell� support dans buffer
			CString Txt;
			Txt.Format("%s\t",pObjet->m_TInfoSupports.GetItem().m_LblSupport.AsJCharPtr());
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture id support dans buffer
			Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_IdSupport);
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture id parent support du support dans buffer
			Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_IdParentSupport);
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture libell� complet format du support dans buffer
			Txt.Format("%s\t",pObjet->m_TInfoSupports.GetItem().m_LblCompletFormat.AsJCharPtr());
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture libell� abr�g� format du support dans buffer
			Txt.Format("%s\t",pObjet->m_TInfoSupports.GetItem().m_LblAbregeFormat.AsJCharPtr()); 
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture id format  du support dans buffer
			Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_IdFormat); 
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture du tarif  du support dans buffer
			Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_Tarif);
			pObjet->m_StrExportHypo += Txt;	

			// Ecriture du nombre d'insertion du support dans buffer
			Txt.Format("%d\n",pObjet->m_TInfoSupports.GetItem().m_NbInsertions); 
			//Txt.Format("%f\n",pObjet->m_expDATHypoFormat->m_Effectif); 
			pObjet->m_StrExportHypo += Txt;	
		}
	}
}

VOID CATPExpCsvTestPlan::OnExportResultHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
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
		Txt.Format("%.0f\n",pObjet->m_BudgetBrut);
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
	//
	// Titre entete ligne
	
	if(pDocExport->m_ATPDocCreuset.m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		Txt.LoadString(IDS_CR_CONTACTS);
	else
		Txt.LoadString(IDS_CR_CONTACTSPLUS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Contacts actifs hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_ContactActif >= 0)
		Txt.Format("%.0f\n",pObjet->m_ContactActif/1000.0);
	pObjet->m_StrExportHypo += Txt;			
		
	//-------------------------------------------- Export GRP ---------------------------------- 
	if(pDocExport->m_GRPActifValid)
	{
		//---------------------------------------- Export GRP Actif -------------------------------- 
		// Titre entete ligne
		Txt.LoadString(IDS_CR_GRPACTIF);
		pObjet->m_StrExportHypo += _T(Txt + "\t");

		// Ecriture GRP Actif hypothese dans buffer
		Txt = "*\n";
		if (pDocExport->m_GRPActifValid && pObjet->m_GrpActif >= 0)
			Txt.Format("%.1f\n",pObjet->m_GrpActif);
		pObjet->m_StrExportHypo += Txt;			

	}

	//-------------------------------------------- Export GRP ---------------------------------- 
	// Titre entete ligne
	Txt.LoadString(IDS_CR_GRP);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture GRP hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Grp >= 0)
		Txt.Format("%.1f\n",pObjet->m_Grp);
	pObjet->m_StrExportHypo += Txt;		
	

	//----------------------------------- Export Couverture Cumul�e 1+ % -----------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_COUVERTURE);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture Couverture Cumulee � 1+ % hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Couverture > 0)
		Txt.Format("%.2f\n",pObjet->m_Couverture);
	pObjet->m_StrExportHypo += Txt;			

	/*
	//----------------------------------------- Export R�p�tition ------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_REPET);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture R�p�tition hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Repetition >= 0)
		Txt.Format("%.2f\n",pObjet->m_Repetition);
	pObjet->m_StrExportHypo += Txt;			

	//----------------------------------------- Export R�p�tition [+] ------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_REPETPLUS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture R�p�tition hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_RepetitionPlus >= 0)
		Txt.Format("%.2f\n",pObjet->m_RepetitionPlus);
	pObjet->m_StrExportHypo += Txt;			
	*/

	if(pDocExport->m_GRPActifValid)
	{
		//----------------------------------------- Export R�p�tition [+] ------------------------------
		//									
		// Titre entete ligne
		Txt.LoadString(IDS_CR_REPETPLUS);
		pObjet->m_StrExportHypo += _T(Txt + "\t");

		// Ecriture R�p�tition hypothese dans buffer
		Txt = "*\n";
		Txt.Format("%.2f\n",pObjet->m_RepetitionPlus);
		pObjet->m_StrExportHypo += Txt;			
	}

	//----------------------------------------- Export R�p�tition ------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_REPET);
	pObjet->m_StrExportHypo += _T(Txt + "\t");

	// Ecriture R�p�tition hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Repetition >= 0)
		Txt.Format("%.2f\n",pObjet->m_Repetition);
	pObjet->m_StrExportHypo += Txt;			

	//----------------------------------- Export Couverture m�moris�e %  -----------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_COUVMEM);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Couverture M�moris�e hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_CouvMemo >= 0)
		Txt.Format("%.2f\n",pObjet->m_CouvMemo);
	pObjet->m_StrExportHypo += Txt;			

	//--------------------------------------- Export Co�t/GRP ----------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CTGRP);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/GRP hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_CoutGRP >= 0)
		Txt.Format("%.2f\n",pObjet->m_CoutGRP);
	pObjet->m_StrExportHypo += Txt;			

	//--------------------------------------- Export Co�t/%couv ----------------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CTCOUVERTURE);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/%couv hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_CoutCouv >= 0)
		Txt.Format("%.2f\n",pObjet->m_CoutCouv);
	pObjet->m_StrExportHypo += Txt;			

	//----------------------------------- Export Co�t/1000 individus ----------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CT1000INDIVIDUS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/1000 individus hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Cout1000Indiv >= 0)
		Txt.Format("%.2f\n",pObjet->m_Cout1000Indiv);
	pObjet->m_StrExportHypo += Txt;			

	//------------------------------------- Export Co�t/1000 contacts ---------------------------
	//									
	// Titre entete ligne
	Txt.LoadString(IDS_CR_CT1000CONTACTS);
	pObjet->m_StrExportHypo += _T(Txt + "\t");
	
	// Ecriture Co�t/1000 contacts hypothese dans buffer
	Txt = "*\n";
	if (pObjet->m_Cout1000Contact >= 0)
		Txt.Format("%.2f\n",pObjet->m_Cout1000Contact);
	pObjet->m_StrExportHypo += Txt;			

}

VOID CATPExpCsvTestPlan::OnExportDistriCumHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo)
{
	//---------------------------------- Export de la distribution de fr�quence Cumul�e-----------------------------
	//
	CString Txt;
	for (JUnt32 InxContact = 0; InxContact<= 20; InxContact++)
	{		
		// Export n contacts
		if (InxContact == 0)
			// Les non touch�s
			pObjet->m_StrExportHypo += _T("NON TOUCHES:\t");

		else if (InxContact < 20 && InxContact > 0)
		{
			// les touch�s de 1 + 19
			Txt.Format("%s %d\t","AU MOINS",InxContact,"CONTACT:");
			pObjet->m_StrExportHypo += Txt;
		}

		else if (InxContact == 20)
		{
			// les touch�s +19 contacts
			Txt.Format("%s %d %s\t","AU MOINS",InxContact,"CONTACTS et +:");
			pObjet->m_StrExportHypo += Txt;
		}

		// Ecriture Valeur Distribution dans buffer
		Txt.Format("%.4f\n",pObjet->m_DistriCum.Item(InxContact));  
		pObjet->m_StrExportHypo += Txt; 
	}
}

VOID CATPExpCsvTestPlan::OnExportDistriNetteHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo)
{

	//---------------------------------- Export de la distribution de fr�quence Cumul�e-----------------------------
	//
	// Table des gros b�tas
	int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
	CCourbes * courbes = CCourbes::GetInstance();

	// Boucle sur les nombres de contacts
	CString Txt;
	for (JUnt32 InxContact = 0; InxContact<= 20; InxContact++)
	{		
		// Export n contacts
		if (InxContact == 0)
		{
			// Les non touch�s
			pObjet->m_StrExportHypo += _T("NON TOUCHES:\t");
		}
		else if (InxContact < 20 && InxContact > 0)
		{
			// les touch�s de 1 + 19
			Txt.Format("%s %d\t","AU MOINS",InxContact,"CONTACT:");
			pObjet->m_StrExportHypo += Txt;
		}
		else if (InxContact == 20)
		{
			// les touch�s +19 contacts
			Txt.Format("%s %d %s\t","AU MOINS",InxContact,"CONTACTS et +:");
			pObjet->m_StrExportHypo += Txt;
		}

		/* D�j� export�
		// Ecriture du b�ta associ� 
		if (InxContact > 0 && InxContact < 20)
		{
			// Calcul b�ta * 100
			JUnt32 Beta100 = courbes->GetProba(InxContact, beta) * 100;

			// Indique b�ta du nb contact
			Txt.Format("%d\t",Beta100);
			pObjet->m_StrExportHypo += Txt;
		}
		else
		{
			// Non touch�s et au del� des 20, pas de b�ta
			Txt.Format("%s\t"," ");
			pObjet->m_StrExportHypo += Txt;
		}
		*/

		// Ecriture Valeur Distribution dans buffer
		Txt.Format("%.4f\n",pObjet->m_DistriNette.Item(InxContact));  
		pObjet->m_StrExportHypo += Txt; 
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Saut de lignes
VOID CATPExpCsvTestPlan::OnExportSTLigne(JATPDocExport * pObjet,const JUnt32 NbLigne)
{
	for (JUnt32 i = 0; i < NbLigne; i++)
		// saut de ligne
		pObjet->m_StrExport += _T("\n");
}

VOID CATPExpCsvTestPlan::OnExportSTLigne(CInfoHypo * pObjet,const JUnt32 NbLigne)
{
	for (JUnt32 i = 0; i < NbLigne; i++)
		// saut de ligne
		pObjet->m_StrExportHypo += _T("\n");
}


// Export contxtes
VOID CATPExpCsvTestPlan::OnExportCtxLecture(JATPDocExport * pObjet)
{
	// Le contexte de lecture
	pObjet->m_StrExport += "\nCONTEXTE DE LECTURE \n";
	pObjet->m_StrExport += "Aucun contexte";
	pObjet->m_StrExport += "\n";
}

void CATPExpCsvTestPlan::OnExportCourbeReponse(JATPDocExport * pObjet)
{
	int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
	CCourbes* courbe = CCourbes::GetInstance();

	pObjet->m_StrExport += _T("COURBE REPONSE\n");
	if(beta==0)
		pObjet->m_StrExport+= _T("Pas de courbe\n");
	if(beta>0 && beta<100)
		pObjet->m_StrExport.AppendFormat("B�ta %d\n", beta);
	if(beta>100)
		pObjet->m_StrExport.AppendFormat("%s \n", courbe->GetLabel(beta).AsJCharPtr());

	for (int i=1; i<=20; i++)
		pObjet->m_StrExport.AppendFormat("%.0f\t", 100.0 * courbe->GetProba(i, beta));
		
	pObjet->m_StrExport += _T("\n");
}