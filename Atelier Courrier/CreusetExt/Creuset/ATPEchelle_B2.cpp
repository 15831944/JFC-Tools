// on inclut les d�finitions n�cessaires
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
// la fonction pour d�finir le document

JVoid CATPEchelle_B2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPEchelle_B2::Update()
{
	// on met � jour le composant
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
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPEchelle_B2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on r�cup�re l'identifiant du Support
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	
	// on recherche l'�chelle
	m_pATPDocCreuset->m_ITREchelle.MoveTo(IdSupport);

	// on teste si on a trouv� l'�chelle
	if (m_pATPDocCreuset->m_ITREchelle.IsValid())
	{
		// on r�cup�re l'�chelle
		const JITPDATEchelle & ITDATEchelle = m_pATPDocCreuset->m_ITREchelle.GetItem();

		CString Text;
		// on formate la p�riodicit�
		CRegroupements * pReg = CRegroupements::GetInstance();
		// R�cup id support
		IElem *pElem = pReg->GetIElemByID(IdSupport);
		// Test si c'est un couplage
		if(pElem->IsCouplage())
		{
			Text = _T("Couplage ");
		}
		else
		{
			// on r�cup�re le p�riodicit�
			if(ITDATEchelle.m_Periodicite)
			{
				const JSRCPresse & SrcPresse = JSRCPresse::GetInstance();
				Text = SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(ITDATEchelle.m_Periodicite);
			}
		}
		// on �crit le libell� p�riodicit�
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
// la fonction pour appliquer les mises � jour

JVoid CATPEchelle_B2::OnUpdate()
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
// CATPEchelle_B2 message handlers

void CATPEchelle_B2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Bx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}
