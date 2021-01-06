// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_F2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_F2::CATPEchelle_F2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPEchelle_F2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPEchelle_F2::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_F2::~CATPEchelle_F2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_F2, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_F2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPEchelle_F2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
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
		// CItemsConstruits *pItemsConstruits = CItemsConstruits::GetInstance(); 

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

		// on r�cup�re le tarif du Support
		JFlt64 AudienceRef = ITDATEchelle.m_AudienceRef;
		CString TxtOut;

		// Add all children
		CRegroupements * pReg = CRegroupements::GetInstance();
		// R�cup id support
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Test si c'est un couplage
		if (AudienceRef > 0.0 && CibleOk)
			TxtOut.Format("%.0f", AudienceRef / 1000.0);
		else
			TxtOut.Format("*");

		CString Fmt = _T("### ### ### ### ### ");
		CFormatStr::FormatNum(Fmt, TxtOut);
	
		// on �crit le tarif
		dc.DrawText(TxtOut, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelle_F2::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

JVoid CATPEchelle_F2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPEchelle_F2::OnUpdate()
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
// CATPEchelle_F2 message handlers

void CATPEchelle_F2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}
