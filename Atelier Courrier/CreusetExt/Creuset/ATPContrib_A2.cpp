// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPContrib_A2.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPContrib_A2::CATPContrib_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPContrib_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPContrib_A2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPContrib_A2::~CATPContrib_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPContrib_A2, JFCGrid)
	//{{AFX_MSG_MAP(CATPContrib_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPContrib_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on initialise le texte
		CString Text;
		CString Fmt;
		// on teste la colonne
		if (i == 0)
		{
			switch(j)
			{
			case 0:
				RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;
				// on dessine le fond de la cellule
				dc.FillRect(&Rect, &BrushB);
				Text.LoadString(IDS_CR_APPORT);// = "Apport";
				break;
			case 1:
				if(m_pATPDocCreuset->m_AFFContribution.m_ContribAdd != 0.0)
				{
					if(m_pATPDocCreuset->m_AFFContribution.m_ContribAdd < 0.0)
						Text.Format(_T("- %.1f %% "), - m_pATPDocCreuset->m_AFFContribution.m_ContribAdd);
					else
						Text.Format(_T("+ %.1f %% "), m_pATPDocCreuset->m_AFFContribution.m_ContribAdd);
					Fmt.Format(_T("### ### ### ### ###,#"));
					CFormatStr::FormatNum(Fmt, Text);
				}
			break;
			case 2:
				if(m_pATPDocCreuset->m_AFFContribution.m_XGRPAdd != 0.0)
				{
					if(m_pATPDocCreuset->m_AFFContribution.m_XGRPAdd < 0.0)
						Text.Format(_T("- %.1f "), - m_pATPDocCreuset->m_AFFContribution.m_XGRPAdd);
					else
						Text.Format(_T("+ %.1f "), m_pATPDocCreuset->m_AFFContribution.m_XGRPAdd);
					Fmt.Format(_T("### ### ### ### ###,#"));
					CFormatStr::FormatNum(Fmt, Text);
				}
			break;
			case 3:
				if( int(m_pATPDocCreuset->m_AFFContribution.m_coutGRPAdd ) != 0 )
				{
					Text = CFormatStr::FormatCout(m_pATPDocCreuset->m_AFFContribution.m_coutGRPAdd, m_pATPDocCreuset->m_NbDecCout);
					if(m_pATPDocCreuset->m_AFFContribution.m_coutGRPAdd > 0.0)
					Text.Insert(0,"+ ");
			}
				break;
			case 4:
				if(m_pATPDocCreuset->m_AFFContribution.m_repriseAdd != 0.0)
				{
					if(m_pATPDocCreuset->m_AFFContribution.m_repriseAdd < 0.0)
						Text.Format(_T("- %.2f "), - m_pATPDocCreuset->m_AFFContribution.m_repriseAdd);
					else
						Text.Format(_T("+ %.2f "), m_pATPDocCreuset->m_AFFContribution.m_repriseAdd);
					Fmt.Format(_T("### ### ### ### ###,##"));
					CFormatStr::FormatNum(Fmt, Text);
				}
				break;
			}
		}	// on teste la colonne
		if (i == 1)
		{
			switch(j)
			{
			case 0:
				RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;
				// on dessine le fond de la cellule
				dc.FillRect(&Rect, &BrushB);
				Text.LoadString(IDS_CR_DEFICIT);// = "Déficit";
				break;
			case 1:
				if(m_pATPDocCreuset->m_AFFContribution.m_ContribSupp != 0.0)
				{
					if(m_pATPDocCreuset->m_AFFContribution.m_ContribSupp < 0.0)
						Text.Format(_T("- %.1f %% "), - m_pATPDocCreuset->m_AFFContribution.m_ContribSupp);
					else
						Text.Format(_T("+ %.1f %% "), m_pATPDocCreuset->m_AFFContribution.m_ContribSupp);
					Fmt.Format(_T("### ### ### ### ###,#"));
					CFormatStr::FormatNum(Fmt, Text);
				}
			break;
			case 2:
				if(m_pATPDocCreuset->m_AFFContribution.m_XGRPSupp != 0.0)
				{
					if(m_pATPDocCreuset->m_AFFContribution.m_XGRPSupp < 0.0)
						Text.Format(_T("- %.1f "), - m_pATPDocCreuset->m_AFFContribution.m_XGRPSupp);
					else
						Text.Format(_T("+ %.1f "), m_pATPDocCreuset->m_AFFContribution.m_XGRPSupp);
					Fmt.Format(_T("### ### ### ### ###,#"));
					CFormatStr::FormatNum(Fmt, Text);
				}
			break;
			case 3:
				if(int(m_pATPDocCreuset->m_AFFContribution.m_coutGRPSupp ) != 0 )
				{
					Text = CFormatStr::FormatCout(m_pATPDocCreuset->m_AFFContribution.m_coutGRPSupp, m_pATPDocCreuset->m_NbDecCout);
					if(m_pATPDocCreuset->m_AFFContribution.m_coutGRPSupp > 0.0)
						Text.Insert(0,"+ ");
				}
				break;
			case 4:
				if(m_pATPDocCreuset->m_AFFContribution.m_repriseSupp != 0.0)
				{
					if(m_pATPDocCreuset->m_AFFContribution.m_repriseSupp < 0.0)
						Text.Format(_T("- %.2f "), - m_pATPDocCreuset->m_AFFContribution.m_repriseSupp);
					else
						Text.Format(_T("+ %.2f "), m_pATPDocCreuset->m_AFFContribution.m_repriseSupp);
					Fmt.Format(_T("### ### ### ### ###,##"));
					CFormatStr::FormatNum(Fmt, Text);
				}
				break;
			}
		}
		// on écrit le texte
		if(j==0)
			dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		else
			dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	}
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPContrib_A2::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPContrib_A2 message handlers

void CATPContrib_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 2, TRUE, FALSE);
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 5, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(2, FALSE);
	this->SetVertCount(5, FALSE);
}
