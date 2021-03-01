// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo_E2.h"
#include "ATPCreusetHypo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )


//////////////////
// le constructeur

CATPCreusetHypo_E2::CATPCreusetHypo_E2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo_E2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPCreusetHypo_E2::SetParent(CATPCreusetHypo* pATPCreusetHypo)
{
	m_pATPCreusetHypo = pATPCreusetHypo;
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo_E2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_E2::~CATPCreusetHypo_E2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_E2, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetHypo_E2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments

LONG CATPCreusetHypo_E2::OnGetItemState(JUnt32 IndHyp, JUnt32 IndSupport)
{
	// on initialise l'état du jour
	LONG Etat = 0;

	// on renvoie l'état du jour
	return (Etat);
}

JBool CATPCreusetHypo_E2::AjoutSupportPlateau(CKeySupport KeySup)
{
	// on teste les exceptions
	try
	{
		// on ajoute le Support au plateau
		m_pATPDocCreuset->m_PlateauSupports.Add(KeySup);
	}
	catch (JException* pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		return false;
	}
	
	// on positionne l'indicateur de modification
	m_pATPDocCreuset->SetModifiedFlag(TRUE);

	// on met à jour les vues du document
	m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PLATEAU);

	return true;

}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo_E2::OnUpdate()
{
	// Nombre d'éléments
	LONG ySupp = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie le document
		if (m_pATPDocCreuset->IsValid())
		{
			// on récupère le nombre de supports
			// via plateau
			ySupp = m_pATPDocCreuset->m_PlateauSupports.GetCount();
		}
	}

	// on met à jour le composant
	this->SetHorzCount(1, FALSE);
	this->SetVertCount(ySupp, TRUE);

}

JVoid CATPCreusetHypo_E2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	//if ((j+1 == m_pATPCreusetHypo->m_NoLigSupportActif))
	//{
	//	// on crée le pinceau pour les insertions en arrière plan
	//	CBrush BrushB;
	//
	//	// les autres cellules insertions
	//	BrushB.CreateSolidBrush(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	//	rect.top    -= 2;
	//	rect.bottom += 1;
	//	rect.left   -= 3;
	//	rect.right  += 3;

	//	// on dessine le fond de la cellule
	//	dc.FillRect(&rect, &BrushB);
	//}
	
	CString Text;
	// on récupère l'identifiant du Support
	CKeySupport Support;
	Support.m_IdSupport = 0;
	//if (m_pATPDocCreuset->m_AFFPlateauGrille)
	//{
	//	// Mode affichage plateau (tous les supports sélectionnées)
	//	if (m_pATPDocCreuset->m_AFFPlanEtendu) 
	//		// Récupère identificateur support sur le plateau plan étendu
	//		Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(j);
	//	else
			// Récupère identificateur support sur le plateau hypothèses
			Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	//}
	//else
	//{
	//	// Mode affichage plan (tous les supports sélectionnées avec au moins 1 insertion)
	//	// Récupère index plateau
	//	JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(j);
	//	if (InxPlateau >= 0)

	//		if (m_pATPDocCreuset->m_AFFPlanEtendu) 
	//			// Récupère identificateur support sur le plateau plan étendu
	//			Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
	//		else
	//			// Récupère identificateur support sur le plateau hypothèse
	//			Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
	//}

	if(!m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
	{
		// on initialise le nombre d'insertions
		JInt32 NbInsertions = 0;
		JInt32 NbInsertCpl = 0;

		// on récupère la période d'action de l'étude
		JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
		JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();
		bool bCpl = false;

		// on vérifie la validité de la période d'action
		if (DateD.IsValid() && DateF.IsValid())
		{
			// on récupère l'hypothèse courante
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(i);

			// on vérifie la validité de l'hypothèse
			if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
			{

				// on récupère le plan de l'hypothèse courante
				CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();
				PLNInsertions.GetNbInsertSupport(Support, DateD, DateF, NbInsertions, NbInsertCpl, bCpl);
				
				// on formate le nombre d'insertions
				if(NbInsertions)
				{
					if (bCpl)
						Text.Format(_T("(%d)"), NbInsertions);
					else
					{
						if (NbInsertCpl)
							Text.Format(_T("%d (%d)"), NbInsertions, NbInsertCpl);
						else
							Text.Format(_T("%d"), NbInsertions);
					}
				}
			}
		}
		// on écrit le nombre d'insertions
		dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else
	{
	//	CTarifs * pTarifs	= CTarifs::GetInstance();

	//	JUnt32 Tarif = 0;
	//for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	//		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
	//		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	//	{
	//		if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() >=2)
	//		{
	//			JInt32 IdFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
	//			
	//			m_pATPDocCreuset->SubstitutFormat(Support.m_IdSupport,IdFormat);

	//			pTarifs->GetTarifByVersion(Support.m_IdSupport, IdFormat, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(), Tarif);
	//		}
	//	}
	//	if(Tarif)
	//	{
	//		Text.Format("%d",Tarif); 
	//		CString Fmt = _T("### ### ### ### ### € ");
	//		CFormatStr::FormatNum(Fmt, Text);
	//	}
	//	else
	//		Text = "* ";
	//	
	//	JInt32 idfmt = 0;
	//	if(m_pATPDocCreuset->SubstitutFormat(Support.m_IdSupport,idfmt))
	//		Text.AppendFormat(" %s", JSRCPresse::GetInstance().m_TBLFormats.GetLabelCourtByID(idfmt));
	//	
		Text = "erreur";
		// on écrit le tarif
		dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

}




JInt32 CATPCreusetHypo_E2::OnGetNbInsertions(JUnt32 IdSupport,JUnt32 IdHypothese,JBool &SuppressionPossible) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// à priori on pourra supprimer ces insertions
	SuppressionPossible = false;

	// on récupère la période d'action de l'étude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();

	// on vérifie la validité de la période d'action
	if (DateD.IsValid() && DateF.IsValid())
	{

		// Va sur l'hypothese 
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_MasterPlan;

		// Récup Index Support dans plateau
		JUnt32 j;
		JInt32 InxSuppPlateau = -1;
		for (j = 0;j < m_pATPDocCreuset->m_PlateauSupports.GetCount();j++)
			if (m_pATPDocCreuset->m_PlateauSupports.Item(j).m_IdSupport == IdSupport)
			{
				// Ce support fait partie du plateau, en index j
				InxSuppPlateau = j;
				break;
			}

		if (InxSuppPlateau >= 0)
		{
			// Le support fait partie du plateau courant, donc insertion(s) existante(s) possible(s)
			const CKeySupport & Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);

			// on crée la clé de l'insertion
			CInsKey Key;
			Key.m_IdSupport		=	Support.m_IdSupport;
			Key.m_IdParent		=	Support.m_IdParent; 
			Key.m_DateD			=	DateD;
			Key.m_IdFormat		=	0;

			JList<CInsKey> List;

			if(PLNInsertions.FindInsertions(Key,DateF,List))
			{
				// Filtre avec les formats actifs
				JList<CInsKey> Tmp;
				for(List.MoveFirst(); List.IsValid(); List.MoveNext())
				{
					m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(List.GetItem().m_IdFormat);
					if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
					{
						// Récupére état du format
						JUnt32 Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
						if (Etat & 0x0003)
							Tmp.AddTail() = List.GetItem();
					}
				}
				// on met à jour l'affichage du zoom
				if(Tmp.GetCount())
				{
					for(Tmp.MoveFirst(); Tmp.IsValid(); Tmp.MoveNext())
					{
						CInsKey Key = Tmp.GetItem();
						CInsertion *pInsertion = PLNInsertions.FindInsertion(Key);
						if(pInsertion == 0)
							continue;
						NbInsertions += pInsertion->m_NbInsertions;

						// Test si suppression possible (date début inclue dans intervalle date période action)
						if 	(pInsertion->m_DateD >= DateD && pInsertion->m_DateD <= DateF)
							SuppressionPossible = true;
					}
				}
			}
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}

/////////////////////////////////////
// CATPCreusetHypo_E2 message handlers

void CATPCreusetHypo_E2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x2(), TRUE, FALSE);
}
