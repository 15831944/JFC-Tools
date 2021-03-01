// on inclut les d�finitions n�cesaires
#include "stdafx.h"
#include "ATPCreusetPlan_Metriques.h"
#include "ATPCreusetPlan_B2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_B2::CATPCreusetPlan_B2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetPlan_B2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetPlan_B2::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_B2::~CATPCreusetPlan_B2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_B2, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetPlan_B2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPCreusetPlan_B2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;
	JInt32 NbInsertCpl = 0;

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();
	bool bCpl = false;

	// on v�rifie la validit� de la p�riode d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on r�cup�re l'hypoth�se courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

		// on v�rifie la validit� de l'hypoth�se
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			// on r�cup�re l'identifiant du Support
			CKeySupport Support;
			Support.m_IdSupport = 0;
			if (m_pATPDocCreuset->m_AFFPlateauGrille)
			{
				// Mode affichage plateau (tous les supports s�lectionn�es)
				if (m_pATPDocCreuset->m_AFFPlanEtendu) 
					// R�cup�re identificateur support sur le plateau plan �tendu
					Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(j);
				else
					// R�cup�re identificateur support sur le plateau hypoth�ses
					Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
			}
			else
			{
				// Mode affichage plan (tous les supports s�lectionn�es avec au moins 1 insertion)
				// R�cup�re index plateau
				JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(j);
				if (InxPlateau >= 0)

					if (m_pATPDocCreuset->m_AFFPlanEtendu) 
						// R�cup�re identificateur support sur le plateau plan �tendu
						Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
					else
						// R�cup�re identificateur support sur le plateau hypoth�se
						Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
			}

			// on r�cup�re le plan de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

			// on cr�e la cl� de l'insertion
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
						// R�cup�re �tat du format
						JUnt32 Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
						if ((Etat & 0x0003) && (List.GetItem().m_IdParent == Support.m_IdParent))
							Tmp.AddTail() = List.GetItem();
					}
				}
				// on met � jour l'affichage du zoom
				if(Tmp.GetCount())
				{
					for(Tmp.MoveFirst(); Tmp.IsValid(); Tmp.MoveNext())
					{
						CInsKey InsKey = Tmp.GetItem();
						if(InsKey.m_IdParent)
							bCpl = true;
						CInsertion *pInsertion = PLNInsertions.FindInsertion(InsKey);
						if(pInsertion == 0)
							continue;
						NbInsertions += pInsertion->m_NbInsertions;
						if(pInsertion->IsCouplage())
						{
							//NbInsertCpl += pInsertion->GetCouplageInsertions().GetCount();
							CPlanInsertions & PLNInserCpl = pInsertion->GetCouplageInsertions();
							for(PLNInserCpl.MoveFirst_SDF(); PLNInserCpl.IsValid_SDF(); PLNInserCpl.MoveNext_SDF())
							{
								CInsertion *pInsCpl = PLNInserCpl.GetItem_SDF();
								if( ( DateD <= pInsCpl->m_DateD && pInsCpl->m_DateD <= DateF) ||
									(pInsCpl->m_DateD <= DateD && DateD < pInsCpl->m_DateD + pInsCpl->m_Duree) ||
									(pInsCpl->m_DateD <= DateF && DateF <= pInsCpl->m_DateD + pInsCpl->m_Duree ))
									NbInsertCpl += pInsCpl->m_NbInsertions;
							}
						}
					}
				}
			}
		}
	}

	// on formate le nombre d'insertions
	CString Text;
	if(bCpl)
		Text.Format(_T("(%d)"), NbInsertions);
	else
	{
		if (NbInsertCpl)
			Text.Format(_T("%d (%d)"), NbInsertions, NbInsertCpl);
		else
			Text.Format(_T("%d"), NbInsertions);
	}

	// on �crit le nombre d'insertions
	dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}


////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPCreusetPlan_B2::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
	JUnt32 Valeur = 0;
}

JVoid CATPCreusetPlan_B2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetPlan_B2::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG cx = 0;
	LONG cy = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on r�cup�re le nombre d'�l�ments
		cx = 1;

		/*
		if (m_pATPDocCreuset->m_AFFPlateauGrille) 
			// nb �lmts plateau s�lectionn�
			cy = m_pATPDocCreuset->m_PlateauSupports.GetCount();
		else
		{
			// nb �lmt plateau compact� (plan s�lectionn�)
			cy = m_pATPDocCreuset->OnNbLigPlanPlateau();
		}
		*/

		// on r�cup�re le nombre d'�l�ments
		if (m_pATPDocCreuset->m_AFFPlanEtendu)
		{
			// Cas du plan �tendu
			if (m_pATPDocCreuset->m_AFFPlateauGrille) 
				// nb �lmts plateau s�lectionn� en mode �tendu
				cy = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.GetCount();
			else
			{
				// nb �lmt plateau compact� (plan s�lectionn� en plan �tendu)
				cy = m_pATPDocCreuset->OnNbLigPlanPlateau(true);
			}
		}
		else
		{
			// Cas des hypoth�ses
			if (m_pATPDocCreuset->m_AFFPlateauGrille) 
				// nb �lmts plateau s�lectionn�
				cy = m_pATPDocCreuset->m_PlateauSupports.GetCount();
			else
			{
				// nb �lmt plateau compact� (plan s�lectionn�)
				cy = m_pATPDocCreuset->OnNbLigPlanPlateau(false);
			}
		}

	}

	// on met � jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPCreusetPlan_B2 message handlers

void CATPCreusetPlan_B2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetPlan_Metriques::CX_Bx(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetPlan_Metriques::CY_x2(), TRUE, FALSE);
}
