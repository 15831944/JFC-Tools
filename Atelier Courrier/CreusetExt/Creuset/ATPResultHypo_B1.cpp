// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResult_Grandeurs.h"
#include "ATPResultHypo_B1.h"
#include ".\atpresulthypo_b1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultHypo_B1::CATPResultHypo_B1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
	m_prev_i = -1; 
	m_prev_j = -1;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPResultHypo_B1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPResultHypo_B1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo_B1::~CATPResultHypo_B1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultHypo_B1, JFCGrid)
	//{{AFX_MSG_MAP(CATPResultHypo_B1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPResultHypo_B1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on calcule le rectangle
	CRect rectTmp = rect;
	rectTmp.top    -= 2;
	rectTmp.bottom += 1;
	rectTmp.left   -= 3;
	rectTmp.right  += 4;

	
	// on crée le pinceau pour les insertions en arrière plan
	CBrush BrushTmp;
	LONG OldCol = -1;
	if (i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
	{
		// on crée le pinceau pour les insertions en arrière plan
		// BrushTmp.CreateSolidBrush(CATPColors::GetColorDark(CATPColors::COLORCREUSET));

		// on dessine le fond de la cellule
		// dc.FillRect(&rectTmp, &BrushTmp);

		// const int NbGrad = 10;
		static COLORREF clr_degrad[CATPColors::m_NbGrad];
		CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);

		CATPColors::FillRectGraduateWithSurround(dc, rectTmp, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);

		LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));
	}
	else
	{
		// on crée le pinceau pour les insertions en arrière plan
		// BrushTmp.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
		// dc.FillRect(&rectTmp, &BrushTmp);

		// const int NbGrad = 10;
		static COLORREF clr_degrad[CATPColors::m_NbGrad];
		CATPColors::GetColorLightGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);

		CATPColors::FillRectGraduateWithSurround(dc, rectTmp, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);

		LONG OldCol = dc.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET));
	}

	if(j == 0)
	{
		// on dessine le fond de la cellule
		CString TxtHeaderCol;
		if(m_nbFormatsActifs>1)
		{
			TxtHeaderCol.FormatMessage(IDS_CR_HYPOTHESE, i+1);
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(i+1);
			if (m_pATPDocCreuset->m_TblHypotheses.IsValid()
				&& !m_pATPDocCreuset->m_TblHypotheses.GetItem().GetLabel().IsEmpty())
			{
				TxtHeaderCol = m_pATPDocCreuset->m_TblHypotheses.GetItem().GetLabel().AsJCharPtr();
			}
		}
		else
			TxtHeaderCol.Format("H%d",i+1); 
		dc.DrawText(_T(TxtHeaderCol), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	}
	if(j == 1 && m_nbFormatsActifs)
	{

		JArray<CRect> rectTbl;
		rectTbl.SetCount(m_nbFormatsActifs);
		int id;
		LONG sz = (rectTmp.right - rectTmp.left)/m_nbFormatsActifs;
		rectTbl.Item(0) = rectTmp;
		rectTbl.Item(0).right = rectTbl.Item(0).left + sz;
		for (id = 1; id<m_nbFormatsActifs; id++)
		{
			rectTbl.Item(id) = rectTmp;
			rectTbl.Item(id).left = rectTbl.Item(id-1).right;
			rectTbl.Item(id).right = rectTbl.Item(id).left + sz;
			dc.SelectObject(&m_PenG);
			dc.MoveTo(rectTbl.Item(id).left, rectTmp.top); dc.LineTo(rectTbl.Item(id).left, rectTmp.bottom);
		}

		const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
		CString Text;

		for(id = 0, m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
		
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
			{
				//LONG OldCol01 = -1;
				//if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()==2)
				//{
				//	CBrush brushIn;
				//	brushIn.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
				//	rectTbl.Item(id).left+=1;
				//	dc.FillRect(&rectTbl.Item(id), &brushIn);
				//	OldCol01 = dc.SetTextColor(CATPColors::GetColorBlack());
				//}
				
				srcpresse.m_TBLFormats.MoveTo(m_pATPDocCreuset->m_MapIdFormatsSel.GetKey()); // Récup item correspondant à l'indice
				if(srcpresse.m_TBLFormats.IsValid())
				{
					JFormat *pElem = (JFormat *)srcpresse.m_TBLFormats.GetItem();
					if(pElem)
						Text = pElem->GetLabelCourt().AsJCharPtr();  
						dc.DrawText(Text, &rectTbl.Item(id), DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
				}
				//if(OldCol01!=-1)
				//	dc.SetTextColor(OldCol01);
				id++;
			}
		}
		
		//col total
		if(m_pATPDocCreuset->m_AFFColonneTotal)
		{
			//CBrush brushIn;
			//brushIn.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
			//rectTbl.Item(rectTbl.GetCount()-1).left+=1;
			//dc.FillRect(&rectTbl.Item(rectTbl.GetCount()-1), &brushIn);
			//LONG OldCol01 = dc.SetTextColor(CATPColors::GetColorBlack());
			
			
			//LOGFONT lf;
			//dc.GetCurrentFont()->GetLogFont(&lf);
			///*lf.lfWeight = FW_BOLD;*/lf.lfItalic = 1;
			//CFont fontBold;
			//fontBold.CreateFontIndirect(&lf);
			//CFont* pOldFont = dc.SelectObject(&fontBold);


			
			dc.DrawText("Total", &rectTbl.Item(rectTbl.GetCount()-1), DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			//dc.SetTextColor(OldCol01);

			//dc.SelectObject(pOldFont);
		}
		//col total


	}
	if(OldCol!=-1)
		dc.SetTextColor(OldCol);

	dc.SelectObject(&m_PenA);
	dc.MoveTo(rectTmp.left-1, rectTmp.top); dc.LineTo(rectTmp.left-1, rectTmp.bottom);

}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPResultHypo_B1::OnLButtonDown(LONG i, LONG j)
{
    if (m_pATPDocCreuset)
	{
		// Ident hypothèse
		JUnt32 IdHypothese = i+1;

		// Si nouvelle hypothèse active, on l'active
		if (m_pATPDocCreuset->m_IdxHypotheseActif != IdHypothese && IdHypothese <= m_pATPDocCreuset->m_TblHypotheses.GetCount() )
		{
			// On change l'hypothèse active
			m_pATPDocCreuset->m_IdxHypotheseActif	= IdHypothese;
			m_pATPDocCreuset->m_MapAffHypothese.ResetSet(IdHypothese);

			//m_pATPDocCreuset->m_MapAffHypothese.MoveTo(IdHypothese);
			//if(!m_pATPDocCreuset->m_MapAffHypothese.IsValid())
			//	m_pATPDocCreuset->m_MapAffHypothese.Add(IdHypothese) = true;
			//else
			//	m_pATPDocCreuset->m_MapAffHypothese.GetItem() = true;

			// Mise à jour via new période action
			m_pATPDocCreuset->m_IdxHypotheseCourant = m_pATPDocCreuset->m_IdxHypotheseActif;

			// Update affichage insertions
			m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
			m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_HYPOTHESE);
		}
	}
}

JVoid CATPResultHypo_B1::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPResultHypo_B1::OnUpdate()
{
	LONG cx = 0;
	LONG cy = 0;
	m_nbFormatsActifs = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on récupère le nombre d'éléments
		// cx = m_pATPDocCreuset->m_MaxHypothese;
		cx = m_pATPDocCreuset->m_TblHypotheses.GetCount()-1;
		//nbFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();
		m_nbFormatsActifs = 0;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				m_nbFormatsActifs++;
		}
		if(m_pATPDocCreuset->m_AFFColonneTotal)
			m_nbFormatsActifs++;

		if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		{
			cy = 1;
			this->SetVertItemSize( 2*CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);
		}
		else
		{
			cy = 2;
			this->SetVertItemSize( CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);
		}
	}

	if(m_nbFormatsActifs!=0)
		this->SetHorzItemSize(m_nbFormatsActifs * CATPResultHypo_Metriques::CX_Bx(), TRUE, FALSE);
	else
		this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Bx(), TRUE, FALSE);
	
	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);

}

///////////////////////////////////////
// CATPResultHypo_B1 message handlers

void CATPResultHypo_B1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetVertItemSize(CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);
}


JVoid CATPResultHypo_B1::OnMouseMove(LONG i, LONG j)
{
	//on s'épargne le calcul si la souris ne change pas de cellule
	if (j == m_prev_j && i == m_prev_i )
		return;

	if (i == -1)
	{
		m_tool.Activate(FALSE);
		return;
	}
	m_tool.Activate(FALSE);
	CString strTipText;
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(i+1);
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
	{
		CPlanInsertions plan = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		if (plan.GetLabel().IsEmpty())
			strTipText.FormatMessage(IDS_CR_HYPOTHESE, i+1);
		else
			strTipText = plan.GetLabel().AsJCharPtr();
	}

	if (m_tool.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		CString sText;
		m_tool.AddTool(this, (LPCTSTR)strTipText, rectBtn, 1);
	} 

	// Set text for tooltip
	m_tool.UpdateTipText((LPCTSTR)strTipText, this, 1);
	m_tool.Activate(TRUE);
	m_prev_j = j;
	m_prev_i = i;
}

BOOL CATPResultHypo_B1::PreTranslateMessage(MSG* pMsg)
{
	InitToolTip();
	
	m_tool.RelayEvent(pMsg);

	return JFCGrid::PreTranslateMessage(pMsg);
}
