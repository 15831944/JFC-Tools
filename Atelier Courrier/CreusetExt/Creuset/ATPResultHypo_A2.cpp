// on inclut les d�finitions n�cessaires
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
// la fonction pour d�finir le document

JVoid CATPResultHypo_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;
	m_ResultGrandeurs.SetDocument(m_pATPDocCreuset);
	// on met � jour le composant
	this->OnUpdate();

}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPResultHypo_A2::Update()
{
	// on met � jour le composant
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
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPResultHypo_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{

	// on cr�e le texte
	CString Text;

	// on v�rifie la validit� de l'hypoth�se
	if (m_pATPDocCreuset->IsValid())
	{
		// R�sultats g�n�raux (Couverture, GRP, Budget etc....)
		if (!m_pATPDocCreuset->m_AFFDistributionN
			&& !m_pATPDocCreuset->m_AFFDistributionC
			&& !m_pATPDocCreuset->m_AFFVentilation.IsVisible()
			&& !m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		{
			// Libell� Budget KEuros
			if (m_ResultGrandeurs.IsBudget(j))
				Text.LoadString(IDS_CR_BUDGET);
			// Libell� Nb indivs touch�s
			if (m_ResultGrandeurs.IsNbIndividus(j))
				Text.LoadString(IDS_CR_AUDIENCE);
			// Libell� Nb de contacts distribu�es
			if (m_ResultGrandeurs.IsNbContacts(j))
				Text.LoadString(IDS_CR_CONTACTS);
			// Libell� Nb de contacts plus
			if (m_ResultGrandeurs.IsNbCtsPlus(j))
				Text.LoadString(IDS_CR_CONTACTSPLUS);
			// Libell� GRP
			if (m_ResultGrandeurs.IsGRP(j))
				Text.LoadString(IDS_CR_GRP);
			// Libell� X-GRP
			if (m_ResultGrandeurs.IsXGRP(j))
				Text.LoadString(IDS_CR_GRPACTIF);
			// Libell� Couverture
			if (m_ResultGrandeurs.IsCouverture(j))
				Text.LoadString(IDS_CR_COUVERTURE);
			// Libell� R�p�tition
			if (m_ResultGrandeurs.IsRepetition(j))
				Text.LoadString(IDS_CR_REPET);
			// Libell� R�p�tition [+]
			if (m_ResultGrandeurs.IsRepetPlus(j))
				Text.LoadString(IDS_CR_REPETPLUS);
			// Libell� Couverture m�moris�e
			if (m_ResultGrandeurs.IsCouvMem(j))
				Text.LoadString(IDS_CR_COUVMEM);
			// Libell� Cout GRP
			if (m_ResultGrandeurs.IsCoutGRP(j))
				Text.LoadString(IDS_CR_CTGRP);
			// Libell� Cout Couverture
			if (m_ResultGrandeurs.IsCoutCouv(j))
				Text.LoadString(IDS_CR_CTCOUVERTURE);
			// Libell� Cout / 1000 Indiv
			if (m_ResultGrandeurs.IsCout1000Ind(j))
				Text.LoadString(IDS_CR_CT1000INDIVIDUS);
			// Libell� Cout GRP 1000 Contact
			if (m_ResultGrandeurs.IsCout1000Cts(j))
				Text.LoadString(IDS_CR_CT1000CONTACTS);
			
		}

		if(m_pATPDocCreuset->m_AFFDistributionC)
		{
			// Distribution de contacts
			if (j == 20)
				// Libell� des non touch�s
				Text.LoadString(IDS_CR_DISTRIBNT);

			else 
			{
				// touch�s au moins 1 fois (1 fois, 2 fois etc ... jusqu'� 20+)
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
					// Libell� des non touch�s
					Text.LoadString(IDS_CR_DISTRIBNT);

				else 
				{
					// touch�s au moins 1 fois (1 fois, 2 fois etc ... jusqu'� 20+)
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
			// on r�cup�re l'identifiant du Support via le plateau
			JUnt32 IdSupport;

			// %%%%%%%%%%%%%%%%%%%%
			// Nombre de supports du plateau
			JUnt32 NbSupportsPlateau = m_pATPDocCreuset->m_PlateauSupports.GetCount();
			JUnt32 NbSupports = m_pATPDocCreuset->OnNbLigPlanActif();

			// Nombre de grandeurs contribution s�lectionn�es
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

					// on teste si on a trouv� le Support
					if (IdSupport)
					{
						// on r�cup�re le Support
						CRegroupements * pReg = CRegroupements::GetInstance();
						IElem * pElem= pReg->GetIElemByID(IdSupport);
						if(pElem)
						{
							if(!KeySupp.m_IdParent)
							{
								if(pElem->IsCouplage())
									// Support Enfant decal�
									Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
								else
									// on �crit le libell� du Support
									Text = pElem->GetLabel().AsJCharPtr();
							}
							else
								// Support Enfant decal�
								Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
						}
					}
				}
			}
			else
			{
				// A FAIRE autre mode affichage
				// Affichage r�sultat contributions en regroupant les grandeurs s�lectionn�s
				Text = "";

				// Cas de plusieurs grandeurs contribs s�lectionn�es
				int IndSupportActif = j / (NbGrandeursActives + 1);
				int IndSupport = m_pATPDocCreuset->GetIndexSuppPlateauViaIndexSuppActif(IndSupportActif);
				const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(IndSupport);
				

				IdSupport = KeySupp.m_IdSupport; 

				if (j % (NbGrandeursActives + 1) < NbGrandeursActives)
				{
					if (j % (NbGrandeursActives + 1) == 0)
					{
						// on teste si on a trouv� le Support
						if (IdSupport)
						{
							// on r�cup�re le Support
							CRegroupements * pReg = CRegroupements::GetInstance();
							IElem * pElem= pReg->GetIElemByID(IdSupport);
							if(pElem)
							{
								if(!KeySupp.m_IdParent)
								{
									if(pElem->IsCouplage())
										// Support Enfant decal�
										Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
									else
										// on �crit le libell� du Support
										Text = pElem->GetLabel().AsJCharPtr();
								}
								else
									// Support Enfant decal�
									Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
							}
						}
					}
					else
						Text = "-";
				}
			}
		}

		// on �crit le texte
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
// la fonction pour appliquer les mises � jour

JVoid CATPResultHypo_A2::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG xCol		= 0;
	LONG yGrandeur	= 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on r�cup�re le nombre de grandeurs (y compris grandeurs distri.fr�q.)
		if (!m_pATPDocCreuset->m_AFFDistributionN
			&& !m_pATPDocCreuset->m_AFFDistributionC
			&& !m_pATPDocCreuset->m_AFFVentilation.IsVisible()
			&& !m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
			//&& !m_pATPDocCreuset->m_AFFMemoDememo)
		{
			// Les r�sultats g�n�raux (Couverture, GRP, Budget etc....)
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
			// Distribution de contacts cumul�e
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

				// Tenir compte du nombre de grandeurs s�lectionn�es
				if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
				{
					// Aucune grandeur contrib s�lectionn�e
					yGrandeur = m_pATPDocCreuset->m_EchelleSupports.GetCount();
				}
				else
				{
					// Cas de plusieurs grandeurs contribs s�lectionn�es
					yGrandeur = m_pATPDocCreuset->m_EchelleSupports.GetCount() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();
				}
			}
			else
			{
				// Tenir compte du nombre de grandeurs s�lectionn�es
				if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
				{
					// Aucune grandeur contrib s�lectionn�e
					// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount();
					yGrandeur = m_pATPDocCreuset->OnNbLigPlanActif();
				}
				else
				{
					// en mode plateau
					// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();
					yGrandeur = m_pATPDocCreuset->OnNbLigPlanActif() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();

					// On ajoute les lignes espaces entre bloc de r�sultats
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

	// on met � jour le composant
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
