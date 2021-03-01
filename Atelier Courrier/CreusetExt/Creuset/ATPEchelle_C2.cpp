/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													COLONNE TARIFICATION DES SUPPORTS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_C2.h"
#include "DlgTarifManuel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_C2::CATPEchelle_C2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPEchelle_C2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// Ce grid n'utilisera pas les �ditbox
	this->SetAccesEditBox(false);

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPEchelle_C2::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_C2::~CATPEchelle_C2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_C2, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_C2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPEchelle_C2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// Flag de position si tarif manuel
	JBool TarifManuel = false;

	// on r�cup�re l'identifiant du Support
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);

	
	
	// on recherche l'�chelle
	m_pATPDocCreuset->m_ITREchelle.MoveTo(IdSupport);

	// on teste si on a trouv� l'�chelle
	if (m_pATPDocCreuset->m_ITREchelle.IsValid())
	{
		// Stockage tarif support
		JFlt32 Tarif = 0;

		// On r�cup�re info de l'�chelle pour r�cup�ration tarif source
		const JITPDATEchelle & ITDATEchelle = m_pATPDocCreuset->m_ITREchelle.GetItem();

		// on r�cup�re le tarif du Support
		Tarif = ITDATEchelle.m_Tarif;

		CString Txt;
		if (Tarif > 0)
		{
			Txt.Format("%.0f",Tarif); 
			CString Fmt = _T("### ### ### ### ### � ");
			CFormatStr::FormatNum(Fmt, Txt);
		}
		else
			Txt.Format("* ");

		// Affichage texte dans cellule tarif
		// if (!TarifManuel)
		if (!ITDATEchelle.m_TarifManuel) 
		{
			// Ecriture du tarif de base
			dc.DrawText(Txt, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		}	
		else
		{
			// Font sp�cifique tarif manuel
			LOGFONT lf;
			dc.GetCurrentFont()->GetLogFont(&lf);
			lf.lfItalic = FALSE;
			CFont fontTM;
			fontTM.CreateFontIndirect(&lf);

			// Couleur diff�rente pour les tarifs manuels
			LONG OldColor = dc.SetTextColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET));

			// Sauve ancienne font
			CFont* pOldFont = dc.SelectObject(&fontTM);

			// Ecriture du tarif manuel
			dc.DrawText(Txt, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

			// Remet Font de base  et Couleur de base 
			dc.SelectObject(pOldFont);
			dc.SetTextColor(OldColor);
		}	
	}
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris
JVoid CATPEchelle_C2::OnLButtonDown(LONG i, LONG j)
{
	// Affichage de la fen�tre dialog saisie tarif manuel
	CDlgTarifManuel DlgTarifManuel(this,m_pATPDocCreuset);

	// Passe l'ident support tarif � modifier
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	DlgTarifManuel.m_IdSupport = IdSupport;

	// Position param�tres couleur et dates
	DlgTarifManuel.SetColors(CATPColors::COLORCREUSET);
	DlgTarifManuel.SetDateTarif(m_pATPDocCreuset->m_KEYEtude.GetDateD(),m_pATPDocCreuset->m_KEYEtude.GetDateF());  

	// Et les formats dispo pour le support, ainsi que le format en cours sur le support
	JUnt32 IdFormatSel = 0;
	for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() >=2)
		{
			// On a trouv� au moins 1 format en cours sur ce support
			IdFormatSel = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
		}
	}
	
	// On ouvre la boite modification tarif si au moins 1 format valide s�lectionn�
	if (m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() > 0 && IdFormatSel!= 0)
	{
		DlgTarifManuel.SetFormatSupport(&m_pATPDocCreuset->m_MapIdFormatsSel, IdFormatSel);

		// Ouverture de la boite
		if(DlgTarifManuel.DoModal()== IDOK)
		{
			// Mise � jour des tarifs �chelle
			OnUpdate();
			m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);

			// Mise � jour de tous les tarifs sur les hypoth�ses courantes
			m_pATPDocCreuset->UpdateAllViews(UPDATE_TARIFS);

			// Mise � jour des diff�rents r�sultats
			// ... de la ventilation
			m_pATPDocCreuset->UpdateAllViews(UPDATE_VENTIL);
			// ... des r�sultats hypotheses
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_HYPO);

			/*
			// ... de la contribution 
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIB);
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIBNEW);
			*/
		}
	}
}

/* //Saisie tarif via edit box
JVoid CATPEchelle_C2::OnLButtonDown(LONG i, LONG j)
{
	
	// Edit box pour saisie manuel tarif
	CEdit* pEditTarif = new CEdit;
	pEditTarif->SetParent(this); 
	
	// Capture coordonn�es cellule pour �dition tarif
	CRect RectEdit;
	LONG Xdeb,Xfin,Ydeb,Yfin;
	this->GetPosAtIdx(i,j,Xdeb,Ydeb,Xfin,Yfin);
	RectEdit.left	= Xdeb;
	RectEdit.top	= Ydeb;
	RectEdit.right	= Xfin;
	RectEdit.bottom = Yfin;

	// Style boite �dition tarif
	DWORD StyleEditTarif = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NUMBER | ES_RIGHT;

	// Cr�ation de la boite saisie tarif
	pEditTarif->Create(StyleEditTarif,RectEdit, this, 1);

	// Font pour saisie tarif
	LOGFONT lf;
	CFont FontSaisieTarif;
	FontSaisieTarif.CreatePointFont(CATPColors::GetFontSizeS(),CATPColors::GetFontLabelName());
	FontSaisieTarif.GetLogFont(&lf);
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = TRUE; 
	pEditTarif->SetFont(&FontSaisieTarif,TRUE);

	// Focus sur la boite �dition tarif
	pEditTarif->SetFocus(); 

	// R�actualise zone �dit
	pEditTarif->InvalidateRect(&RectEdit); 
}
*/

JVoid CATPEchelle_C2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPEchelle_C2::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG cx = 0;
	LONG cy = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on r�cup�re le nombre d'�l�ments
		cx = 1;
		cy = m_pATPDocCreuset->m_EchelleSupports.GetCount();
	}

	// on met � jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelle_C2 message handlers

void CATPEchelle_C2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}
