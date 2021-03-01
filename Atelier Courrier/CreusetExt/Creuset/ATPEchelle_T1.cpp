// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_T1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_T1::CATPEchelle_T1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelle_T1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelle_T1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_T1::~CATPEchelle_T1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_T1, JFCTitle)
	//{{AFX_MSG_MAP(CATPEchelle_T1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPEchelle_T1::OnDrawItem(CDC & dc, RECT rect)
{
	CString Text = "";

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on récupère libellé filtre d'audience
		CString TxtFiltre = "";
		JSRCPresse* m_srcPresse;
		m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
		if(m_pATPDocCreuset->GetIdFiltreAudienceSel() != 1)
		{
			TxtFiltre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(m_pATPDocCreuset->GetIdFiltreAudienceSel());
			Text += TxtFiltre;
			Text += " - ";
		}


		// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

		// Attention cible ou cible pondérée
		IItem * pItem;

		if (!m_pATPDocCreuset->m_CiblesPondsActives)
			// Récup cible correspondant à l'indice
			// pItem = pItemConst->GetItemConstruit(m_pATPDocCreuset->m_IdItemSel);
			pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(m_pATPDocCreuset->m_IdItemSel);
		else
			// Récup cible correspondant à l'indice
			// pItem = pItemConst->GetItemPondere(m_pATPDocCreuset->m_IdItemSel);
			pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemPondere(m_pATPDocCreuset->m_IdItemSel);

		// Si cible trouvé, on retourne son libellé
		if(pItem)
		{
			// Recuperer libellé cible en cours
			// Text += " - ";
			Text += pItem->GetLabel().AsJCharPtr();

			// Récupère effectif cible courante
			JFlt64 EffectifCible = 0.0;
			
			// on crée la clé des effectifs
			CKeyMoteur keyMoteur;

			// on initialise la clé
			// A FAIRE JEREMY
			m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
			if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
				keyMoteur.Set(m_pATPDocCreuset->GetIdSrcAudienceSel(), m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pATPDocCreuset->m_IdItemSel);
			// on initialise l'effectif

			EffectifCible = m_pATPDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);

			// si effectif <> 0 ajoute info effectif
			if (EffectifCible > 0)
			{
				CString TxtEffectif;
				Text += " (";

				TxtEffectif.Format("%.0f",EffectifCible); 
				CString Fmt = _T("### ### ### ### ###");
				CFormatStr::FormatNum(Fmt, TxtEffectif);
				Text += TxtEffectif;

				Text += " ";
				//on ajoute le libellé du type de poids sélectionné
				const JSRCTables  & srcTables	= JSRCTables::GetInstance();
				Text += srcTables.m_TBLPoids.GetLabelByID(m_pATPDocCreuset->m_Offre.m_IdTypePoids);

				Text += ")";
			}
		}

		
		//L'affinité
		// pItem = pItemConst->GetItemConstruit(m_pATPDocCreuset->m_Offre.m_IdItemAffinite);
		pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(m_pATPDocCreuset->m_Offre.m_IdItemAffinite);

		if(pItem)
			Text.AppendFormat(" - Ref. affinité %s", pItem->GetLabel().AsJCharPtr());
		
		// Les segments
		// + nom segment (si +, seg.multiples)
		if (m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.GetCount() > 1)
			// Ici indique plusieurs segments
			Text.Append(" - Multi Segments");
		else
		{
			// Source Table
			const JSRCTables & Sources = JSRCTables::GetInstance ();

			// Id Segment
			JUnt32 IdSeg;
			m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
			IdSeg = m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.GetItem();

			// Affiche ssi segment différent de ensemble
			if (IdSeg != 1)
			{
				const JSegment* Seg = Sources.m_TBLSegments.GetItem(IdSeg);
				Text += " - Segment : ";
				Text += Seg->m_Libelle.AsJCharPtr();
			}
		}
	}

	// on écrit le titre
	if (Text=="" && !m_pATPDocCreuset->m_AFFCreuset.GetEchelleQP())
		Text.FormatMessage(IDS_CR_ECHELLE);
	if (Text=="" && m_pATPDocCreuset->m_AFFCreuset.GetEchelleQP())
		Text.FormatMessage(IDS_CR_FORMATS);


	// on calcule le rectangle
	// RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;
	
	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);
	CATPColors::FillRectGraduateWithSurround(dc, rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);

	// Font spécifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	// lf.lfHeight = -16;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	// Ecriture texte avec font courante
	dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	// Remet ancienne Font
	dc.SelectObject(pOldFont);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelle_T1::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPEchelle_T1 message handlers

void CATPEchelle_T1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
