// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo_E3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetHypo_E3::CATPCreusetHypo_E3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo_E3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo_E3::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_E3::~CATPCreusetHypo_E3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_E3, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetHypo_E3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo_E3::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG xHyp = 1; 
	LONG yLigne = 1;

	// on met à jour le composant
	this->SetHorzCount(xHyp, FALSE);
	this->SetVertCount(yLigne, TRUE);
}


// la fonction pour dessiner l'intérieur des cellules
JVoid CATPCreusetHypo_E3::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
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

			int id = 0;
			CString Text;

			PLNInsertions.GetNbInsert(DateD,DateF, NbInsertions, NbInsertCpl, bCpl);

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


			// on écrit le nombre d'insertions
			dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		}
	}
}


/////////////////////////////////////
// CATPCreusetHypo_E3 message handlers

void CATPCreusetHypo_E3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x3(), TRUE, FALSE);
}
