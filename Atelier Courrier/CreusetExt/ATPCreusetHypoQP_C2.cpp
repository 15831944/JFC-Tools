#include "StdAfx.h"
#include ".\atpcreusethypoqp_c2.h"
#include "ATPCreusetHypo_Metriques.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_COMBO_FORMAT 1024

CATPCreusetHypoQP_E2::CATPCreusetHypoQP_E2(void)
{
}

CATPCreusetHypoQP_E2::~CATPCreusetHypoQP_E2(void)
{
}


BEGIN_MESSAGE_MAP(CATPCreusetHypoQP_E2, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetHypoQP_E2)
	ON_CBN_SELCHANGE(ID_COMBO_FORMAT,SetSubstitutFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

JVoid CATPCreusetHypoQP_E2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
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
	if (m_pATPDocCreuset->m_AFFPlateauGrille)
	{
		// Mode affichage plateau (tous les supports sélectionnées)
		if (m_pATPDocCreuset->m_AFFPlanEtendu) 
			// Récupère identificateur support sur le plateau plan étendu
			Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(j);
		else
			// Récupère identificateur support sur le plateau hypothèses
			Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	}
	else
	{
		// Mode affichage plan (tous les supports sélectionnées avec au moins 1 insertion)
		// Récupère index plateau
		JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(j);
		if (InxPlateau >= 0)

			if (m_pATPDocCreuset->m_AFFPlanEtendu) 
				// Récupère identificateur support sur le plateau plan étendu
				Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
			else
				// Récupère identificateur support sur le plateau hypothèse
				Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
	}

	CTarifs * pTarifs	= CTarifs::GetInstance();

	JUnt32 Tarif = 0;
	// Récup id support
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem *pElem = pReg->GetIElemByID(Support.m_IdSupport);

	if(pElem && pTarifs)
	{

		JDate  dateClick(m_pATPDocCreuset->m_KEYEtude.GetDateD());
		JDate  dateLimiteMin(m_pATPDocCreuset->m_KEYEtude.GetDateD());
		JDate  dateLimiteMax(m_pATPDocCreuset->m_KEYEtude.GetDateF());
		JDate  dateExact;
		
		JUnt32 Duree;
		JUnt32 IdTitre;
		if(pElem->IsTitreApparie())
			IdTitre = pElem->AsTitreApparie()->GetIDTitre();
		else
			IdTitre = Support.m_IdSupport;

		
		JInt32 idfmt = 0;
		m_pATPDocCreuset->SubstitutFormat(IdTitre,idfmt);

		// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
		pTarifs->GetNextTarifByDateNew( IdTitre, idfmt, m_pATPDocCreuset->m_MapTarifManuel, dateClick, dateLimiteMin, dateLimiteMax, dateExact, Duree, Tarif);
	}

	if(Tarif)
	{
		Text.Format("%d",Tarif); 
		CString Fmt = _T("### ### ### ### ### € ");
		CFormatStr::FormatNum(Fmt, Text);
	}
	else
		Text = "* ";
	
	//JInt32 idfmt = 0;
	//if(m_pATPDocCreuset->SubstitutFormat(Support.m_IdSupport,idfmt))
	//	Text.AppendFormat(" %s", JSRCPresse::GetInstance().m_TBLFormats.GetLabelCourtByID(idfmt));
	
	// on écrit le tarif
	dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

}


JVoid CATPCreusetHypoQP_E2::OnLButtonDown(LONG i, LONG j)
{
	//if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
	//{
	//	LONG x1, y1, x2, y2;
	//	GetPosAtIdx(i, j, x1, y1, x2, y2);
	//	CRect rectBtn; 
	//	rectBtn.SetRect(x1, y1, x2-1, y2+100);

	//	DWORD dwStyle = 0;
	//	m_pCombo = new CInPlaceCombo(this, rectBtn, dwStyle, ID_COMBO_FORMAT);
	//	m_pCombo->SetItemHeight(-1, y2-y1-5);
	//	m_pCombo->SetSelectionColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	//	const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
	//	for(pSrcPresse->m_TBLFormats.MoveFirst();pSrcPresse->m_TBLFormats.IsValid(); pSrcPresse->m_TBLFormats.MoveNext())
	//	{
	//		JFormat * pFormat = (JFormat *)pSrcPresse->m_TBLFormats.GetItem();
	//		int nR = m_pCombo->AddString(pFormat->GetLabelCourt().AsJCharPtr());
	//		m_pCombo->SetItemData(nR, pFormat->GetID());
	//	}
	//
	//
	//	CKeySupport Support;
	//	Support.m_IdSupport = 0;
	//	if (m_pATPDocCreuset->m_AFFPlateauGrille)
	//	{
	//		// Mode affichage plateau (tous les supports sélectionnées)
	//		if (m_pATPDocCreuset->m_AFFPlanEtendu) 
	//			// Récupère identificateur support sur le plateau plan étendu
	//			Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(j);
	//		else
	//			// Récupère identificateur support sur le plateau hypothèses
	//			Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	//	}
	//	else
	//	{
	//		// Mode affichage plan (tous les supports sélectionnées avec au moins 1 insertion)
	//		// Récupère index plateau
	//		JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(j);
	//		if (InxPlateau >= 0)

	//			if (m_pATPDocCreuset->m_AFFPlanEtendu) 
	//				// Récupère identificateur support sur le plateau plan étendu
	//				Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
	//			else
	//				// Récupère identificateur support sur le plateau hypothèse
	//				Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
	//	}
	//	m_idSupport = Support.m_IdSupport;
	//}
}

//SetSubstitutFormat
void CATPCreusetHypoQP_E2::SetSubstitutFormat()
{
	int nSel = m_pCombo->GetCurSel();
	if(nSel != CB_ERR)
	{
		int idFormat = m_pCombo->GetItemData(nSel);
		m_pATPDocCreuset->SetSubstitutFormat(m_idSupport,idFormat);
		CWaitCursor wait;
		m_pATPDocCreuset->UpdateAllViews(UPDATE_TARIFS);
		Update();
	}
}

void CATPCreusetHypoQP_E2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_CxQP(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x2(), TRUE, FALSE);
}
