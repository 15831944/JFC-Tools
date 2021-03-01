#include "StdAfx.h"
#include ".\atpexpcsvPlanCalend.h"
#include "JATPDocExport.h"
#include "Courbes.h"

// Constructeur
CATPExpCsvPlanCalend::CATPExpCsvPlanCalend(void)
{
}

// Destructeur
CATPExpCsvPlanCalend::~CATPExpCsvPlanCalend(void)
{
}

// Export Entete Plan Test
void CATPExpCsvPlanCalend::VisitBegin(JATPDocExport * pObjet)
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

// Export Pied de page Plan Test
void CATPExpCsvPlanCalend::VisitEnd  (JATPDocExport * pObjet)
{
	// Nothing for moment
	// A FAIRE
}

// Export début hypothèse
void CATPExpCsvPlanCalend::VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
	// Export Début Hypothèse
	CString Txt;
	Txt.AppendFormat("HYPOTHESE %d\n", IndexHypo);
	pObjet->m_StrExportHypo += Txt;

	// Export Libellé hypothèse
	if (pObjet->m_LabelHypo.IsEmpty())
	{
		Txt.FormatMessage(IDS_CR_HYPOTHESE, IndexHypo);
		Txt.Append("\n\n");
	}
	else
		Txt.Format("%s\n\n",pObjet->m_LabelHypo.AsJCharPtr());

	pObjet->m_StrExportHypo += Txt;

	// Export Bloc n° 2 / EXPORT BLOC 2
	Txt.Format("%s\n","BLOC 2",IndexHypo);
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("SELECTION HYPOTHESES:\n");

}

// Export hypothèse
void CATPExpCsvPlanCalend::Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
	// ------------------------------- Export Bloc Hypothèses -----------------------------------
	this->OnExportHypothese(pObjet,IndexHypo);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);

	// ------------------------------- Graphe des couverture ------------------------------------
	CString Txt;
	Txt.Format("%s\n","BLOC 3");
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("GRAPHE COUVERTURE:\n");

	this->OnExportGrapheCouverture(pObjet,IndexHypo);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);

	// ----------------------------------- Graphe des GRP ---------------------------------------
	Txt.Format("%s\n","BLOC 4");
	pObjet->m_StrExportHypo += Txt;

	// on exporte le titre
	pObjet->m_StrExportHypo +=_T("GRAPHE GRP:\n");

	this->OnExportGrapheGRP(pObjet,IndexHypo);

	// Saut de lignes
	this->OnExportSTLigne(pObjet,1);
}

void CATPExpCsvPlanCalend::VisitEnd(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter le n° de version de l'export Plan Calendaire
VOID CATPExpCsvPlanCalend::OnExportNoVersion(JATPDocExport * pObjet)
{
	// Ecriture n° de version dans buffer
	pObjet->m_StrExport = "VERSION 1";
	this->OnExportSTLigne(pObjet,1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter l'entete Atelier Creuset
VOID CATPExpCsvPlanCalend::OnExportEntete(JATPDocExport * pObjet)
{
	// Export Bloc n° 1 / EXPORT BLOC 1 PLAN CALENDAIRE
	pObjet->m_StrExport += "EXPORT PLAN CALENDAIRE\n\n";

	// 1er Bloc : Entete
	pObjet->m_StrExport += "BLOC 1\n";

	// on exporte le titre
	pObjet->m_StrExport +=_T("TITRE:\t");

	// Test de plans
	pObjet->m_StrExport += _T("PLAN CALENDAIRE\n");

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

	// Export liste des formats actifs
	OnExportFormats(pObjet);

	OnExportCtxLecture(pObjet);

}	

///////////////////////////////////////////
// Export de la période et période action
VOID CATPExpCsvPlanCalend::OnExportPeriode(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportCible(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportRefAffinite(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportFiltreAudience(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportSource(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportTerrains(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportSegment(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportTypePoid(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportTypeCalcul(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportFormats(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportEtude(JATPDocExport * pObjet)
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
VOID CATPExpCsvPlanCalend::OnExportProduit(JATPDocExport * pObjet)
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
// Export Hypothèses
VOID CATPExpCsvPlanCalend::OnExportHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo)
{
	// Boucle sur les supports de l'hypothèse
	CString Txt;

	Txt.LoadString(IDS_CR_SUPPORTS);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt = "IDSUPPORT";
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt = "IDPARENTSUPPORT";
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_FORMAT);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_FORMAT);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt = "IDFORMAT";
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";


	Txt.LoadString(IDS_CR_TARIF);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	//Txt.LoadString(IDS_CR_SUPPORTS);
	Txt = "Date début";
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";


	//Txt.LoadString(IDS_CR_SUPPORTS);
	Txt = "Durée";
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_TARIF);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_CONTACTS);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_GRP);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_CTGRP);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	Txt.LoadString(IDS_CR_CONTRIBCOUV);
	pObjet->m_StrExportHypo += Txt;
	pObjet->m_StrExportHypo += "\t";

	//Txt.LoadString(IDS_CR_CONTRIBCOUV);
	pObjet->m_StrExportHypo += "NbInsertion";

	pObjet->m_StrExportHypo += "\n";

	for (pObjet->m_TInfoSupports.MoveFirst();
		 pObjet->m_TInfoSupports.IsValid();
		 pObjet->m_TInfoSupports.MoveNext())
	{
		
		CInfoSupport &InfoSupport = pObjet->m_TInfoSupports.GetItem();  
		if (InfoSupport.m_NbInsertions > 0)
		{
			// Boucle sur les insertions du support
			for (InfoSupport.m_TInfoInsertions.MoveFirst();
				 InfoSupport.m_TInfoInsertions.IsValid();
				 InfoSupport.m_TInfoInsertions.MoveNext())
			{

				// Ecriture libellé support dans buffer
				Txt.Format("%s\t",pObjet->m_TInfoSupports.GetItem().m_LblSupport.AsJCharPtr());
				pObjet->m_StrExportHypo += Txt;	

				// Ecriture id support dans buffer
				Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_IdSupport);
				pObjet->m_StrExportHypo += Txt;	

				// Ecriture id parent support du support dans buffer
				Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_IdParentSupport);
				pObjet->m_StrExportHypo += Txt;	

				// Ecriture libellé complet format du support dans buffer
				Txt.Format("%s\t",pObjet->m_TInfoSupports.GetItem().m_LblCompletFormat.AsJCharPtr());
				pObjet->m_StrExportHypo += Txt;	

				// Ecriture libellé abrégé format du support dans buffer
				Txt.Format("%s\t",pObjet->m_TInfoSupports.GetItem().m_LblAbregeFormat.AsJCharPtr()); 
				pObjet->m_StrExportHypo += Txt;	

				// Ecriture id format  du support dans buffer
				Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_IdFormat); 
				pObjet->m_StrExportHypo += Txt;	

				// Ecriture du tarif  du support dans buffer
				Txt.Format("%d\t",pObjet->m_TInfoSupports.GetItem().m_Tarif);
				pObjet->m_StrExportHypo += Txt;	

				// Info Insertion
				CInfoInsertion &InfoInsertion = InfoSupport.m_TInfoInsertions.GetItem();

				// Ecriture début insertion dans buffer
				if (InfoInsertion.m_DateParution.IsValid())
				{
					// on décompose les dates
					JInt32 DayD, MonthD, YearD;
					InfoInsertion.m_DateParution.GetDate(DayD, MonthD, YearD);
		
					// on exporte les dates
					Txt.Format("%02d/%02d/%04d\t",DayD, MonthD, YearD);
					pObjet->m_StrExportHypo += Txt;
				}
				else
					// Date parution non définie
					pObjet->m_StrExportHypo += "*\t";

				// Ecriture durée insertion dans buffer
				Txt = "*\t";
				if (InfoInsertion.m_Duree > 0)
					Txt.Format("%d\t",InfoInsertion.m_Duree);
				pObjet->m_StrExportHypo += Txt;
		
				// Ecriture tarif insertion dans buffer
				Txt = "*\t";
				if (InfoInsertion.m_Tarif >= 0)
					Txt.Format("%d\t",InfoInsertion.m_Tarif);
				pObjet->m_StrExportHypo += Txt;

				// Ecriture nb contact insertion dans buffer
				Txt = "*\t";
				if (InfoInsertion.m_NbContact >= 0)
					Txt.Format("%.0f\t",InfoInsertion.m_NbContact);
				pObjet->m_StrExportHypo += Txt;

				// Ecriture GRP insertion dans buffer
				Txt = "*\t";
				if (InfoInsertion.m_GRP >= 0)
					Txt.Format("%.1f\t",InfoInsertion.m_GRP);
				pObjet->m_StrExportHypo += Txt;

				// Ecriture Coût GRP insertion dans buffer
				Txt = "*\t";
				if (InfoInsertion.m_CoutGRP >= 0)
					Txt.Format("%.2f\t",InfoInsertion.m_CoutGRP);
				pObjet->m_StrExportHypo += Txt;

				// Ecriture Coût GRP insertion dans buffer
				Txt = "*\t";
				if (InfoInsertion.m_Contribution >= 0)
					Txt.Format("%.2f\t",InfoInsertion.m_Contribution);
				pObjet->m_StrExportHypo += Txt;

				// Ecriture NbInsertion dans buffer
				Txt = "*\n";
				Txt.Format("%d\n", InfoInsertion.m_NbInsertion);
				pObjet->m_StrExportHypo += Txt;

			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Graphe Couverture
VOID CATPExpCsvPlanCalend::OnExportGrapheCouverture(CInfoHypo *pObjet, JUnt32 IndexHypo)
{
	CString Txt;

	// Boucle sur tous les jours
	for (pObjet->m_ValCouv.MoveFirst();
		 pObjet->m_ValCouv.IsValid();
		 pObjet->m_ValCouv.MoveNext())
	{
		// Récupère la valeur couverture du jour
		Txt.Format("%.2f\n",pObjet->m_ValCouv.GetItem()); 
		pObjet->m_StrExportHypo += Txt;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Graphe GRP
VOID CATPExpCsvPlanCalend::OnExportGrapheGRP(CInfoHypo *pObjet, JUnt32 IndexHypo)
{
	CString Txt;

	// Boucle sur tous les jours
	for (pObjet->m_ValGRP.MoveFirst();
		 pObjet->m_ValGRP.IsValid();
		 pObjet->m_ValGRP.MoveNext())
	{
		// Récupère la valeur GRP du jour
		Txt.Format("%.2f\n",pObjet->m_ValGRP.GetItem()); 
		pObjet->m_StrExportHypo += Txt;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Export Saut de lignes
VOID CATPExpCsvPlanCalend::OnExportSTLigne(JATPDocExport * pObjet,const JUnt32 NbLigne)
{
	for (JUnt32 i = 0; i < NbLigne; i++)
		// saut de ligne
		pObjet->m_StrExport += _T("\n");
}

VOID CATPExpCsvPlanCalend::OnExportSTLigne(CInfoHypo * pObjet,const JUnt32 NbLigne)
{
	for (JUnt32 i = 0; i < NbLigne; i++)
		// saut de ligne
		pObjet->m_StrExportHypo += _T("\n");
}

void CATPExpCsvPlanCalend::OnExportCourbeReponse(JATPDocExport * pObjet)
{
	int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
	CCourbes* courbe = CCourbes::GetInstance();

	pObjet->m_StrExport += _T("COURBE REPONSE\n");
	if(beta==0)
		pObjet->m_StrExport+= _T("Pas de courbe\n");
	if(beta>0 && beta<100)
		pObjet->m_StrExport.AppendFormat("Bêta %d\n", beta);
	if(beta>100)
		pObjet->m_StrExport.AppendFormat("%s \n", courbe->GetLabel(beta).AsJCharPtr());

	for (int i=1; i<=20; i++)
		pObjet->m_StrExport.AppendFormat("%.0f\t", 100.0 * courbe->GetProba(i, beta));
		
	pObjet->m_StrExport += _T("\n");
}

// Export contxtes
VOID CATPExpCsvPlanCalend::OnExportCtxLecture(JATPDocExport * pObjet)
{
	// Le contexte de lecture
	pObjet->m_StrExport += "\nCONTEXTE DE LECTURE \n";
	pObjet->m_StrExport += "Aucun contexte";
	pObjet->m_StrExport += "\n";
}
