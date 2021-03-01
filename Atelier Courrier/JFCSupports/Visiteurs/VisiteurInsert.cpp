// VisiteurInsert.cpp: implementation of the CVisiteurInsert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisiteurInsert.h"
#include "JTBLTitres.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVisiteurInsert::CVisiteurInsert()
{
	m_regroup = CRegroupements::GetInstance();
}

CVisiteurInsert::~CVisiteurInsert()
{
}

JVoid CVisiteurInsert::VisiteFamille(CFamille* nd)
{
	m_regroup->m_lstFamille.AddTail() = nd;
}

JVoid CVisiteurInsert::VisiteLot(CLot* nd)
{
	m_regroup->m_lstLot.AddTail() = nd;
}

JVoid CVisiteurInsert::VisiteCouplageLibre(CCouplageLibre* nd)
{
	m_regroup->m_lstCouplageLibre.AddTail() = nd;
}

JVoid CVisiteurInsert::VisiteCentre(CCentre* nd)
{
	m_regroup->m_lstCentre.AddTail() = nd;
	//insertion dans la liste des parents
	Insert(nd);
}

JVoid CVisiteurInsert::VisiteTitre(CTitre* nd)
{
	m_regroup->m_lstTitre.AddTail() = nd;
	//on le retire des NA s'il est dedans
	m_regroup->m_mapFamilleNA.MoveTo( nd->GetID() );
	if (m_regroup->m_mapFamilleNA.IsValid() )
	{
		delete (CTitre*) m_regroup->m_mapFamilleNA.GetItem();
		m_regroup->m_mapFamilleNA.Remove();
	}

	//insertion dans la liste des parents
	Insert(nd);
}


JVoid CVisiteurInsert::VisiteTitreApparente( CTitreApparente* nd )
{
	CTitreApparente* tatmp;
	tatmp = (CTitreApparente*)m_regroup->m_lstTitreApparente.GetNodeByID( nd->GetID() );
	while ( tatmp && tatmp->GetParentID()!= 0 )
	{
		tatmp = (CTitreApparente*)m_regroup->m_lstTitreApparente.GetNextNodeByID(nd->GetID());
	}
	//s'il existe un TitreApparenté avec le centre == 0 on le retire
	if (tatmp)
		m_regroup->m_lstTitreApparente.RemoveNode(tatmp);

	m_regroup->m_lstTitreApparente.AddTail() = nd;

	//insertion dans la liste des parents
	Insert(nd);
}

JVoid CVisiteurInsert::VisiteTitreLot( CTitreLot* nd )
{
	//insertion dans la liste des parents
	InsertInNode(nd);
}

JVoid CVisiteurInsert::VisiteTitreCouplageLibre( CTitreCouplageLibre* nd )
{
	//initialisation des valeurs
	IElem * titre = m_regroup->GetIElemByID(nd->GetID());
	if(titre)
	{
		nd->m_IDAudience = titre->AsITitre()->GetIDAudience();
		nd->m_Periodicite = titre->AsITitre()->GetPeriodicite();
		if (titre->IsTitreApparente())
			nd->m_MediaWeight = titre->AsTitreApparente()->GetMediaWeight();
		else
			nd->m_MediaWeight = 100;
	}
	//insertion dans la liste des parents
	InsertInNode(nd);
}

/////////////////////////////////////////////
JVoid CVisiteurInsert::Insert(INode* nd)
{
	//On recupere le pere
	IComposite * comp = (IComposite *) m_regroup->GetParentByID( nd->GetParentID() );
	//s'il existe on ajoute le fils
	if (comp)
	{
		comp->m_LstNode.AddTail() = nd;
		nd->SetParentNode(comp);
	}
}

JVoid CVisiteurInsert::InsertInNode(INode* nd)
{
	//On recupere le pere
	IComposite * comp = (IComposite *) nd->GetParentNode();
	//s'il existe on ajoute le fils
	if (comp)
		comp->m_LstNode.AddTail() = nd;
}
