// on inclut les d�finitions n�cesaires
#include "stdafx.h"
#include "ATPCreusetPlan_Metriques.h"
#include "ATPCreusetPlan_A2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_A2::CATPCreusetPlan_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetPlan_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetPlan_A2::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_A2::~CATPCreusetPlan_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_A2, JFCGrid)
	//{{AFX_MSG_MAP(CATPCreusetPlan_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPCreusetPlan_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on r�cup�re l'identifiant du Support
	CKeySupport Support;
	Support.m_IdSupport = 0;
	if (m_pATPDocCreuset->m_AFFPlateauGrille)
	{
		if (m_pATPDocCreuset->m_AFFPlanEtendu) 
			// R�cup�re identificateur support sur le plateau plan �tendu
			Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(j);
		else
			// R�cup�re identificateur support sur le plateau hypoth�ses
			Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	}
	else
	{
		// R�cup�re index plateau
		JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(j);
		if (InxPlateau >= 0)
			if (m_pATPDocCreuset->m_AFFPlanEtendu) 
				// R�cup�re identificateur support sur le plateau plan �tendu
				Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
			else
				// R�cup�re identificateur support sur le plateau hypoth�ses
				Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
	}

	CString Text;
	Text.Format(_T("Support %09d"), Support.m_IdSupport);

	// on teste si on a trouv� le Support
	if (Support.m_IdSupport)
	{
		// on r�cup�re le Support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem * pElem= pReg->GetIElemByID(Support.m_IdSupport);
		if(pElem)
		{
			if(!Support.m_IdParent)
			{
				if(pElem->IsCouplage())
					// Support Enfant decal�
					Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
				else
					// on �crit le libell� du Support
					Text = pElem->GetLabel().AsJCharPtr();
			}
			else
				// Support Enfant decal�
				Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
		}
	}
	// on �crit le libell� du Support
	dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPCreusetPlan_A2::OnLButtonDown(LONG i, LONG j)
{
	// on r�cup�re l'identifiant du Support
	const CKeySupport & Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	m_pATPDocCreuset->m_MapIdCouplageExpanded.MoveTo(Support.m_IdSupport);
	if(m_pATPDocCreuset->m_MapIdCouplageExpanded.IsValid())
		m_pATPDocCreuset->m_MapIdCouplageExpanded.GetItem() = !m_pATPDocCreuset->m_MapIdCouplageExpanded.GetItem();
	
	m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PLATEAU); 
}

JVoid CATPCreusetPlan_A2::OnRButtonDown(LONG i, LONG j)
{
//	// R�cup�re le support
//	const CKeySupport & Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
//
//	// On s'y positionne
//	m_pATPDocCreuset->m_MapIdCouplageExpanded.MoveTo(Support.m_IdSupport);
//
//	if(m_pATPDocCreuset->m_MapIdCouplageExpanded.IsValid())
//		// Si c'est un couplage on affiche ses fils
//		m_pATPDocCreuset->m_MapIdCouplageExpanded.GetItem() = true;
//
//	// Mise � jour affichage
//	m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PLATEAU);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetPlan_A2::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG cx = 0;
	LONG cy = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on r�cup�re le nombre d'�l�ments
		cx = 1;
		if (m_pATPDocCreuset->m_AFFPlanEtendu)
		{
			// Cas du plan �tendu
			if (m_pATPDocCreuset->m_AFFPlateauGrille) 
				// nb �lmts plateau s�lectionn� en mode �tendu
				cy = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.GetCount();
			else
			{
				// nb �lmt plateau compact� (plan s�lectionn� en plan �tendu)
				cy = m_pATPDocCreuset->OnNbLigPlanPlateau(true);
			}
		}
		else
		{
			// Cas des hypoth�ses
			if (m_pATPDocCreuset->m_AFFPlateauGrille) 
				// nb �lmts plateau s�lectionn�
				cy = m_pATPDocCreuset->m_PlateauSupports.GetCount();
			else
			{
				// nb �lmt plateau compact� (plan s�lectionn�)
				cy = m_pATPDocCreuset->OnNbLigPlanPlateau(false);
			}
		}
	}

	// on met � jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPCreusetPlan_A2 message handlers

void CATPCreusetPlan_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetPlan_Metriques::CX_Ax(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetPlan_Metriques::CY_x2(), TRUE, FALSE);
}
