// on inclut les définitions nécessaires
#include "stdafx.h"
#include "resource.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResult_Grandeurs.h"
#include "ATPResultHypo_A2.h"
#include "Courbes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultHypo_A2::CATPResultHypo_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPResultHypo_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
	m_ResultGrandeurs.SetDocument(m_pATPDocCreuset);
	// on met à jour le composant
	this->OnUpdate();

}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPResultHypo_A2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo_A2::~CATPResultHypo_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultHypo_A2, JFCGrid)
	//{{AFX_MSG_MAP(CATPResultHypo_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPResultHypo_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{

	// on crée le texte
	CString Text;

	// on vérifie la validité de l'hypothèse
	if (m_pATPDocCreuset->IsValid())
	{
		// Résultats généraux (Couverture, GRP, Budget etc....)
		if (!m_pATPDocCreuset->m_AFFDistributionN
			&& !m_pATPDocCreuset->m_AFFDistributionC
			&& !m_pATPDocCreuset->m_AFFVentilation.IsVisible()
			&& !m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		{
			// Libellé Budget KEuros
			if (m_ResultGrandeurs.IsBudget(j))
				Text.LoadString(IDS_CR_BUDGET);
			// Libellé Nb indivs touchés
			if (m_ResultGrandeurs.IsNbIndividus(j))
				Text.LoadString(IDS_CR_AUDIENCE);
			// Libellé Nb de contacts distribuées
			if (m_ResultGrandeurs.IsNbContacts(j))
				Text.LoadString(IDS_CR_CONTACTS);
			// Libellé Nb de contacts plus
			if (m_ResultGrandeurs.IsNbCtsPlus(j))
				Text.LoadString(IDS_CR_CONTACTSPLUS);
			// Libellé GRP
			if (m_ResultGrandeurs.IsGRP(j))
				Text.LoadString(IDS_CR_GRP);
			// Libellé X-GRP
			if (m_ResultGrandeurs.IsXGRP(j))
				Text.LoadString(IDS_CR_GRPACTIF);
			// Libellé Couverture
			if (m_ResultGrandeurs.IsCouverture(j))
				Text.LoadString(IDS_CR_COUVERTURE);
			// Libellé Répétition
			if (m_ResultGrandeurs.IsRepetition(j))
				Text.LoadString(IDS_CR_REPET);
			// Libellé Répétition [+]
			if (m_ResultGrandeurs.IsRepetPlus(j))
				Text.LoadString(IDS_CR_REPETPLUS);
			// Libellé Couverture mémorisée
			if (m_ResultGrandeurs.IsCouvMem(j))
				Text.LoadString(IDS_CR_COUVMEM);
			// Libellé Cout GRP
			if (m_ResultGrandeurs.IsCoutGRP(j))
				Text.LoadString(IDS_CR_CTGRP);
			// Libellé Cout Couverture
			if (m_ResultGrandeurs.IsCoutCouv(j))
				Text.LoadString(IDS_CR_CTCOUVERTURE);
			// Libellé Cout / 1000 Indiv
			if (m_ResultGrandeurs.IsCout1000Ind(j))
				Text.LoadString(IDS_CR_CT1000INDIVIDUS);
			// Libellé Cout GRP 1000 Contact
			if (m_ResultGrandeurs.IsCout1000Cts(j))
				Text.LoadString(IDS_CR_CT1000CONTACTS);
			
		}

		if(m_pATPDocCreuset->m_AFFDistributionC)
		{
			// Distribution de contacts
			if (j == 20)
				// Libellé des non touchés
				Text.LoadString(IDS_CR_DISTRIBNT);

			else 
			{
				// touchés au moins 1 fois (1 fois, 2 fois etc ... jusqu'à 20+)
				if (j == 0)
					Text.LoadString(IDS_CR_DISTRIB1);
				else if (j < 19) 
					Text.FormatMessage(IDS_CR_DISTRIBN,j+1);
				else
					Text.FormatMessage(IDS_CR_DISTRIBP,j+1);
			}
		}

		if(m_pATPDocCreuset->m_AFFDistributionN)
		{
			if(i == 0)
			{
				// Distribution de contacts
				if (j == 20)
					// Libellé des non touchés
					Text.LoadString(IDS_CR_DISTRIBNT);

				else 
				{
					// touchés au moins 1 fois (1 fois, 2 fois etc ... jusqu'à 20+)
					if (j == 0)
						Text.LoadString(IDS_CR_DISTRIBNETTE1);
					else if (j < 19) 
						Text.FormatMessage(IDS_CR_DISTRIBNETTEN,j+1);
					else
						Text.FormatMessage(IDS_CR_DISTRIBNETTEP,j+1);
				}
			}
			if(i == 1)
			{
				if (j != 20)
				{
					int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
					CCourbes * courbes = CCourbes::GetInstance();
					courbes->GetProba(j+1,beta);

					Text.Format(_T("%.0f"), 100.0 * courbes->GetProba(j+1, beta));
				}
			}
		}

		if(m_pATPDocCreuset->m_AFFVentilation.IsVisible())
		{
			Text = m_pATPDocCreuset->m_AFFVentilation.GetLabel(j);
			/*
			if (i==0)
			{
				if (j%6 == 0)
					Text.Format("%s",m_pATPDocCreuset->m_AFFVentilation.GetLblItem(j/6));
				//if (j%6 == 5 && j!= GetVertCount()-1)
				//	Text = "Intersection avec";
			}
			if(i==1)
			{
				if (j%6 == 0)
					if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
						Text.LoadString(IDS_CR_GRP);
					else
						Text.LoadString(IDS_CR_GRPACTIF);
				if (j%6 == 1)
					if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
						Text.LoadString(IDS_CR_INDGRP);
					else
						Text.LoadString(IDS_CR_INDGRPACTIF);
				if (j%6 == 2)
					Text.LoadString(IDS_CR_COUVERTURE);
				if (j%6 == 3)
					Text.LoadString(IDS_CR_INDCOUV);
				if (j%6 == 4)
					if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
						Text.LoadString(IDS_CR_REPET);
					else
						Text.LoadString(IDS_CR_REPETPLUS);
			}*/
		}

		if(m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		{
			// on récupère l'identifiant du Support via le plateau
			JUnt32 IdSupport;

			// %%%%%%%%%%%%%%%%%%%%
			// Nombre de supports du plateau
			JUnt32 NbSupportsPlateau = m_pATPDocCreuset->m_PlateauSupports.GetCount();
			JUnt32 NbSupports = m_pATPDocCreuset->OnNbLigPlanActif();

			// Nombre de grandeurs contribution sélectionnées
			JUnt32 NbGrandeursActives = m_pATPDocCreuset->m_AFFContribution_New.NbValue();

			// Affichage des contributions par bloc de grandeurs identiques
			if (m_pATPDocCreuset->m_AFFContribution_New.GetModeAffich() == m_pATPDocCreuset->m_AFFContribution_New.MODEAFFICH::AFFICH_BLOC_SUPPORTS)
			{
				// Indice support
				int IndSupportActif = j % (NbSupports + 1);
				int IndSupport = m_pATPDocCreuset->GetIndexSuppPlateauViaIndexSuppActif(IndSupportActif);
				Text ="";

				// Attention saut de ligne entre blocs
				if (IndSupport < NbSupportsPlateau && IndSupportActif < NbSupports)
				{
					const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(IndSupport);
					IdSupport = KeySupp.m_IdSupport; 

					Text.Format(_T("Support %09d"), IdSupport);

					// on teste si on a trouvé le Support
					if (IdSupport)
					{
						// on récupère le Support
						CRegroupements * pReg = CRegroupements::GetInstance();
						IElem * pElem= pReg->GetIElemByID(IdSupport);
						if(pElem)
						{
							if(!KeySupp.m_IdParent)
							{
								if(pElem->IsCouplage())
									// Support Enfant decalé
									Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
								else
									// on écrit le libellé du Support
									Text = pElem->GetLabel().AsJCharPtr();
							}
							else
								// Support Enfant decalé
								Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
						}
					}
				}
			}
			else
			{
				// A FAIRE autre mode affichage
				// Affichage résultat contributions en regroupant les grandeurs sélectionnés
				Text = "";

				// Cas de plusieurs grandeurs contribs sélectionnées
				int IndSupportActif = j / (NbGrandeursActives + 1);
				int IndSupport = m_pATPDocCreuset->GetIndexSuppPlateauViaIndexSuppActif(IndSupportActif);
				const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(IndSupport);
				

				IdSupport = KeySupp.m_IdSupport; 

				if (j % (NbGrandeursActives + 1) < NbGrandeursActives)
				{
					if (j % (NbGrandeursActives + 1) == 0)
					{
						// on teste si on a trouvé le Support
						if (IdSupport)
						{
							// on récupère le Support
							CRegroupements * pReg = CRegroupements::GetInstance();
							IElem * pElem= pReg->GetIElemByID(IdSupport);
							if(pElem)
							{
								if(!KeySupp.m_IdParent)
								{
									if(pElem->IsCouplage())
										// Support Enfant decalé
										Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
									else
										// on écrit le libellé du Support
										Text = pElem->GetLabel().AsJCharPtr();
								}
								else
									// Support Enfant decalé
									Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
							}
						}
					}
					else
						Text = "-";
				}
			}
		}

		// on écrit le texte
		dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE |DT_END_ELLIPSIS);	
	}
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPResultHypo_A2::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

JVoid CATPResultHypo_A2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPResultHypo_A2::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG xCol		= 0;
	LONG yGrandeur	= 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on récupère le nombre de grandeurs (y compris grandeurs distri.fréq.)
		if (!m_pATPDocCreuset->m_AFFDistributionN
			&& !m_pATPDocCreuset->m_AFFDistributionC
			&& !m_pATPDocCreuset->m_AFFVentilation.IsVisible()
			&& !m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
			//&& !m_pATPDocCreuset->m_AFFMemoDememo)
		{
			// Les résultats généraux (Couverture, GRP, Budget etc....)
			xCol		= 1;
			yGrandeur	= m_ResultGrandeurs.GetNbGrandeurs();
		}
		if (m_pATPDocCreuset->m_AFFDistributionN )
		{
			// Distribution de contacts nette
			if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
				xCol	= 1;
			else
				xCol	= 2;
			yGrandeur  += m_ResultGrandeurs.GetNbGrandeursDF();
		}
		if (m_pATPDocCreuset->m_AFFDistributionC)
		{
			// Distribution de contacts cumulée
			xCol	= 1;
			yGrandeur  += m_ResultGrandeurs.GetNbGrandeursDF();
		}
		
		if (m_pATPDocCreuset->m_AFFVentilation.IsVisible())
		{
			// Ventilation
			xCol	= 1;
			yGrandeur  = (m_pATPDocCreuset->m_AFFVentilation.NbValue()+1) * m_pATPDocCreuset->m_AFFVentilation.GetNbItem();
		}
		if (m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		{
			xCol	= 1;
			if (m_pATPDocCreuset->m_AFFOffreHypos) 
			{
				// en mode offre gloable
				yGrandeur = m_pATPDocCreuset->m_EchelleSupports.GetCount();

				// Tenir compte du nombre de grandeurs sélectionnées
				if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
				{
					// Aucune grandeur contrib sélectionnée
					yGrandeur = m_pATPDocCreuset->m_EchelleSupports.GetCount();
				}
				else
				{
					// Cas de plusieurs grandeurs contribs sélectionnées
					yGrandeur = m_pATPDocCreuset->m_EchelleSupports.GetCount() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();
				}
			}
			else
			{
				// Tenir compte du nombre de grandeurs sélectionnées
				if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
				{
					// Aucune grandeur contrib sélectionnée
					// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount();
					yGrandeur = m_pATPDocCreuset->OnNbLigPlanActif();
				}
				else
				{
					// en mode plateau
					// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();
					yGrandeur = m_pATPDocCreuset->OnNbLigPlanActif() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();

					// On ajoute les lignes espaces entre bloc de résultats
					if (m_pATPDocCreuset->m_AFFContribution_New.GetModeAffich() == m_pATPDocCreuset->m_AFFContribution_New.MODEAFFICH::AFFICH_BLOC_SUPPORTS)
					{
						// Mode bloc par supports
						yGrandeur += m_pATPDocCreuset->m_AFFContribution_New.NbValue() - 1;
					}
					else
					{
						// Mode bloc par grandeurs
						// yGrandeur += m_pATPDocCreuset->m_PlateauSupports.GetCount() - 1;
						yGrandeur += m_pATPDocCreuset->OnNbLigPlanActif() - 1;
					}
				}
			}
		}

	}

	// on met à jour le composant
	this->SetHorzCount(xCol, FALSE);
	if(xCol>0 && !m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Ax()/xCol, TRUE, FALSE);
	
	this->SetVertCount(yGrandeur, TRUE);
}

///////////////////////////////////////
// CATPResultHypo_A2 message handlers

void CATPResultHypo_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Ax(), TRUE, FALSE);
	this->SetVertItemSize(CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);
}
