// on inclut les d�finitions n�cesaires
#include "stdafx.h"
#include "ATPCreusetPlan_B3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_B3::CATPCreusetPlan_B3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetPlan_B3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetPlan_B3::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_B3::~CATPCreusetPlan_B3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_B3, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetPlan_B3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'�l�ment

JVoid CATPCreusetPlan_B3::OnDrawItem(CDC & dc, RECT rect)
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;
	JInt32 NbInsertCpl = 0;

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();

	// on v�rifie la validit� de la p�riode d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on r�cup�re l'hypoth�se courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

		// on v�rifie la validit� de l'hypoth�se
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{

			// on r�cup�re le plan de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();


			for(PLNInsertions.MoveFirst_SDF(); PLNInsertions.IsValid_SDF(); PLNInsertions.MoveNext_SDF())
			{
				m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(PLNInsertions.GetItem_SDF()->m_IdFormat);
				if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
				{
					// R�cup�re �tat du format
					JUnt32 Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
					if (Etat & 0x0003)
					{
						CInsertion *pInsertion = PLNInsertions.GetItem_SDF();
						if(pInsertion == 0)
							continue;

						if( ( DateD <= pInsertion->m_DateD && pInsertion->m_DateD <= DateF) ||
							(pInsertion->m_DateD <= DateD && DateD < pInsertion->m_DateD + pInsertion->m_Duree) ||
							(pInsertion->m_DateD <= DateF && DateF < pInsertion->m_DateD + pInsertion->m_Duree ))
						{

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
	}

	// on formate le nombre d'insertions
	CString Text;
	if (NbInsertCpl)
		Text.Format(_T("%d (%d)"), NbInsertions, NbInsertCpl);
	else
		Text.Format(_T("%d"), NbInsertions);

	// on �crit le nombre d'insertions
	dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetPlan_B3::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetPlan_B3 message handlers

void CATPCreusetPlan_B3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
