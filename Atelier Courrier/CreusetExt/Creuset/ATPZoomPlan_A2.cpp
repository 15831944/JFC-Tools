// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPZoomPlan_A2.h"
#include "ATPZoomPlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPZoomPlan_A2::CATPZoomPlan_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPZoomPlan_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

void CATPZoomPlan_A2::SetParent(CATPZoomPlan* ATPZoomPlan)
{
	m_pZoom = ATPZoomPlan;
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPZoomPlan_A2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPZoomPlan_A2::~CATPZoomPlan_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPZoomPlan_A2, JFCGrid)
	//{{AFX_MSG_MAP(CATPZoomPlan_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPZoomPlan_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le texte
	CString Text;

	// on teste la colonne
	if (i == 0)
	{
		// on calcule le rectangle
		RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

		// on dessine le fond de la cellule
		dc.FillRect(&Rect, &BrushB);

		// on teste la ligne
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		for(JInt32 inc = 0; inc < j ; inc++)
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

			
		const JSRCPresse  & srcPresse = JSRCPresse::GetInstance();
		if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
			Text.Format(_T("%s"), srcPresse.m_TBLFormats.GetLabelCourtByID(m_pATPDocCreuset->m_MapIdFormatsSel.GetKey()));

		// on écrit le texte
		dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);		
	}
	else if (i == 1)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on récupère l'indice de l'hypothèse
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_AFFZoomPlan.GetIdxHypothese());

			// on vérifie la validité des identifiants et de la date
			if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
			{
				// on récupère le plan de l'hypothèse courante
				CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();
				
				// on récupère l'insertion du ZoomPlan
				const JList<CInsKey> & List = m_pATPDocCreuset->m_AFFZoomPlan.GetKeyList();
				CString Fmt;
				if (List.GetCount())
				{
					List.MoveTo(0);
					CInsKey InsKey;
					if (List.IsValid())
					{
						InsKey = List.GetItem();
						m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
						for(JInt32 inc = 0; inc < j ; inc++)
							m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

						//m_pATPDocCreuset->m_TBLIdFormats.MoveTo(j-7);
						const JSRCPresse  & srcPresse = JSRCPresse::GetInstance();
						InsKey.m_IdFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
						CInsertion * pInsertion = PLNInsertions.FindInsertion(InsKey);
						if (pInsertion)
							Text.Format(_T("%d "), pInsertion->m_NbInsertions);
					}
				}

                  
				// on écrit le texte
				dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			}
		}
	}
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPZoomPlan_A2::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPZoomPlan_A2 message handlers

void CATPZoomPlan_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 1, TRUE, FALSE);
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 2, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetVertCount(1, FALSE);
	this->SetHorzCount(2, FALSE);
}
