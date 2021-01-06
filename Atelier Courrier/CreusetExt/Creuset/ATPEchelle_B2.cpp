// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_B2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_B2::CATPEchelle_B2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelle_B2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelle_B2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_B2::~CATPEchelle_B2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_B2, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_B2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPEchelle_B2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on récupère l'identifiant du Support
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	
	// on recherche l'échelle
	m_pATPDocCreuset->m_ITREchelle.MoveTo(IdSupport);

	// on teste si on a trouvé l'échelle
	if (m_pATPDocCreuset->m_ITREchelle.IsValid())
	{
		// on récupère l'échelle
		const JITPDATEchelle & ITDATEchelle = m_pATPDocCreuset->m_ITREchelle.GetItem();

		CString Text;
		// on formate la périodicité
		CRegroupements * pReg = CRegroupements::GetInstance();
		// Récup id support
		IElem *pElem = pReg->GetIElemByID(IdSupport);
		// Test si c'est un couplage
		if(pElem->IsCouplage())
		{
			Text = _T("Couplage ");
		}
		else
		{
			// on récupère le périodicité
			if(ITDATEchelle.m_Periodicite)
			{
				const JSRCPresse & SrcPresse = JSRCPresse::GetInstance();
				Text = SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(ITDATEchelle.m_Periodicite);
			}
		}
		// on écrit le libellé périodicité
		dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelle_B2::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

JVoid CATPEchelle_B2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelle_B2::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG cx = 0;
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère le nombre d'éléments
		cx = 1;
		cy = m_pATPDocCreuset->m_EchelleSupports.GetCount();
	}

	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelle_B2 message handlers

void CATPEchelle_B2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Bx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}
