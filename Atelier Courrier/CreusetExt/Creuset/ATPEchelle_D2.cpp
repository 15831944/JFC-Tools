// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_Grandeurs.h"
#include "ATPEchelle_D2.h"
#include "ItemsConstruitsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_D2::CATPEchelle_D2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPEchelle_D2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;
	m_grandeurs.SetDocument(pATPDocCreuset); 
	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPEchelle_D2::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_D2::~CATPEchelle_D2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_D2, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_D2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPEchelle_D2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on cr�e le texte
	CString Text;

	// on r�cup�re l'identifiant du Support
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);

	// Attention si la cible est active
	JBool CibleOk = true;

	// Ident du terrain en cours
	m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
	if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
	{
		JUnt32 IdTerrain = m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey();

		// Attention si c'est une cible pond�r�e
		// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance(); 

		// R�cup l'item d�fini par l'ident cible
		// IItem * pIItem = pItemsConstruits->GetIItemByID(m_pATPDocCreuset->m_IdItemSel);
		IItem * pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_pATPDocCreuset->m_IdItemSel);

		// Ce n'est pas un item pond�r�
		if (!pIItem->IsItemPondere())
		{
			// Calcul de l'effectif
			if (!m_pATPDocCreuset->m_Offre.CibleAutorisee(m_pATPDocCreuset->m_IdItemSel, IdTerrain))
			{
				// Cible non valide pour ce terrain, seuil r�dhibitoite non atteint
				CibleOk = false;
			}
		}
		else
		{	
			// Map avec 1 seul terrain
			JMap <JUnt32, JUnt32> MapTerrain;
			MapTerrain.Add(IdTerrain) = 0;

			// Calcul de l'effectif
			if (!m_pATPDocCreuset->m_MoteurPresse.CiblePondAutorise(m_pATPDocCreuset->m_IdItemSel, m_pATPDocCreuset->m_Offre.m_IdSourceAudience, &MapTerrain))
			{
				// Cible non valide pour ce terrain, seuil r�dhibitoite non atteint
				CibleOk = false;
			}
		}
	}
	else
		// Terrain non valide donc cible non active
		CibleOk = false;

	// on recherche l'�chelle
	m_pATPDocCreuset->m_ITREchelle.MoveTo(IdSupport);

	// on teste si on a trouv� l'�chelle
	if (m_pATPDocCreuset->m_ITREchelle.IsValid())
	{
		// on r�cup�re l'�chelle
		const JITPDATEchelle & ITDATEchelle = m_pATPDocCreuset->m_ITREchelle.GetItem();

		// on teste la grandeur � afficher
		if (m_grandeurs.IsXGRP(i) != FALSE)
		{
			// on r�cup�re le nombre de contacts et l'effectif
			JFlt64 VolLecture = ITDATEchelle.m_VolLecture;
			JFlt64 Effectif   = ITDATEchelle.m_Effectif;

			// on teste si on peut calculer le X-GRP
			if (Effectif > 0.0 && VolLecture > 0.0 && CibleOk)
			{
				// on formate le X-GRP
				Text.Format(_T("%.1f "), (100.0 * VolLecture) / Effectif);
			}
			else
			{
				// on formate une �toile
				Text.Format(_T("* "));
			}
			CString Fmt = _T("### ### ### ### ###,#");
			CFormatStr::FormatNum(Fmt, Text);

		}
		else if (m_grandeurs.IsGRP(i) != FALSE)
		{
			// on r�cup�re la couverture cumul�e � 1+
			JFlt64 AudienceProba   = ITDATEchelle.m_AudienceProba;
			JFlt64 Effectif     = ITDATEchelle.m_Effectif;

			// on formate la couverture	
			if (Effectif > 0.0 && AudienceProba > 0.0 && CibleOk)
			{
				// on formate l'audience LDP
				Text.Format(_T("%.1f "), 100.0 * AudienceProba / Effectif);
			}
			else
			{
				// on formate une �toile
				Text.Format(_T("* "));
			}
			CString Fmt = _T("### ### ### ### ###,#");
			CFormatStr::FormatNum(Fmt, Text);

		}
		else if (m_grandeurs.IsCoutGRP(i) != FALSE)
		{
			// on r�cup�re le budget, nombre de contacts et l'effectif
			JFlt32 Tarif        = ITDATEchelle.m_Tarif;
			JFlt64 AudienceProba   = ITDATEchelle.m_AudienceProba;
			JFlt64 Effectif     = ITDATEchelle.m_Effectif;

			// on teste si on peut calculer le co�t / GRP
			if (AudienceProba > 0.0 && Effectif > 0.0 && Tarif >0.0 && CibleOk)
			{
				// on formate le co�t / GRP
				Text = CFormatStr::FormatCout(((JFlt64)Tarif * Effectif) / (100.0 * AudienceProba), m_pATPDocCreuset->m_NbDecCout);
			}
			else
			{
				// on formate une �toile
				Text.Format(_T("*"));
			}
		}
		else if (m_grandeurs.IsRepetition(i) != FALSE)
		{
			// on r�cup�re le nombre de contacts et l'effectif
			JFlt64 VolLecture = ITDATEchelle.m_VolLecture;
			JFlt64 Couv       = ITDATEchelle.m_Distribution.Item(1).AsJFlt64();

			// on teste si on peut calculer la r�p�tition
			if (VolLecture > 0.0 && Couv > 0.0  && CibleOk)
			{
				// on formate la r�p�tition	
				Text.Format(_T("%.2f "),  VolLecture / Couv);
			}
			else
			{
				// on formate une �toile
				Text.Format(_T("* "));
			}

			CString Fmt = _T("### ### ### ### ###,##");
			CFormatStr::FormatNum(Fmt, Text);
		}
		else if (m_grandeurs.IsNbIndividus(i) != FALSE)
		{
			// on r�cup�re l'effectif
			JFlt64 Couv     = ITDATEchelle.m_Distribution.Item(1).AsJFlt64();

			// on formate la couverture	
			if(Couv > 0.0  && CibleOk)
				Text.Format(_T("%.0f"), Couv/1000);
			else
				Text.Format(_T("*"));

			CString Fmt = _T("### ### ### ### ### ");
			CFormatStr::FormatNum(Fmt, Text);
		}
		else if (m_grandeurs.IsNbContacts(i) != FALSE)
		{
			// on r�cup�re le nombre de contacts
			JFlt64 VolLecture = ITDATEchelle.m_VolLecture;

			// on formate le nombre de contacts
			if(VolLecture > 0.0  && CibleOk)
				Text.Format(_T("%.0f"), VolLecture/1000);
			else
				Text.Format(_T("*"));


			CString Fmt = _T("### ### ### ### ### ");
			CFormatStr::FormatNum(Fmt, Text);

		}
		else if (m_grandeurs.IsIndAffinite(i) != FALSE)
		{
			JFlt64 IndAffinite = ITDATEchelle.m_IndAffinite;
			

			// on teste si on afficher
			if (IndAffinite > 0.0  && CibleOk)
				Text.Format(_T("%.0f "), IndAffinite);
			else
				// on formate une �toile
				Text.Format(_T("* "));
			
		}
		else if (m_grandeurs.IsPrcAffinite(i) != FALSE)
		{
			JFlt64 PrcAffinite = ITDATEchelle.m_PrcAffinite;
			

			// on teste si on afficher
			if (PrcAffinite > 0.0  && CibleOk)
				Text.Format(_T("%.1f %% "), PrcAffinite);
			else
				// on formate une �toile
				Text.Format(_T("* "));
			
		}
		// Ecriture des apports
		else if (m_grandeurs.IsApport(i) != FALSE)
		{
			// Calcul des apports
			JFlt64 Apport = -1;

			// R�cup�re apport
			if (ITDATEchelle.m_Effectif > 0.0) 
				Apport = (ITDATEchelle.m_Apport / ITDATEchelle.m_Effectif) * 100.0;

			if (Apport > 0.0 && CibleOk)
			{
				Text.Format(_T("%.1f %% "), Apport);
			}
			else
			{
				// on formate une �toile
				Text.Format(_T("* "));
			}
		}

		// on �crit le texte
		if (m_grandeurs.IsCoutGRP(i) != FALSE && ITDATEchelle.m_TarifManuel)
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
			dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

			// Remet Font de base  et Couleur de base 
			dc.SelectObject(pOldFont);
			dc.SetTextColor(OldColor);
		}
		else
			dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelle_D2::OnLButtonDown(LONG i, LONG j)
{

}

JVoid CATPEchelle_D2::OnRButtonDown(LONG i, LONG j)
{
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPEchelle_D2::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG cx = 0;
	LONG cy = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{

		// on r�cup�re le nombre d'�l�ments
		cx = m_grandeurs.GetNbGrandeurs();
		cy = m_pATPDocCreuset->m_EchelleSupports.GetCount();
	}

	// on met � jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelle_D2 message handlers

void CATPEchelle_D2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Dx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}
