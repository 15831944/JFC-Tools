// VisiteurRemove.cpp: implementation of the CVisiteurRemove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisiteurRemove.h"
#include "JTBLTitres.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVisiteurRemove::CVisiteurRemove()
{
	m_regroup = CRegroupements::GetInstance();
}

CVisiteurRemove::~CVisiteurRemove()
{
}


JVoid CVisiteurRemove::VisiteFamille(CFamille* nd)
{
//	Remove(nd);
}

JVoid CVisiteurRemove::VisiteCentre(CCentre* nd)
{
	m_regroup->m_lstCentre.RemoveNode(nd);
	Remove(nd);
}

JVoid CVisiteurRemove::VisiteTitre(CTitre* nd)
{
	m_regroup->m_lstTitre.RemoveNode(nd);
	Remove(nd);
}

JVoid CVisiteurRemove::VisiteCouplageLibre( CCouplageLibre* nd )
{
	Remove(nd);
}

JVoid CVisiteurRemove::VisiteLot( CLot* nd )
{
	Remove(nd);
}

JVoid CVisiteurRemove::VisiteTitreApparente( CTitreApparente* nd )
{
	//on le retire du centre
	Remove(nd);

	CTitreApparente* tatmp = NULL;
	(CTitreApparente*)m_regroup->m_lstTitreApparente.GetNodeByID(nd->GetID());
	tatmp = (CTitreApparente*)m_regroup->m_lstTitreApparente.GetNextNodeByID(nd->GetID());

	//si nd est le seul titre app de cet ID
//	if (!tatmp)
//		nd->SetParentID(0);
//	else //sinon on le retire de la liste
	{
		m_regroup->m_lstTitreApparente.RemoveNode(nd);
//		delete nd;
	}

}

JVoid CVisiteurRemove::VisiteTitreLot( CTitreLot* nd )
{
	Remove(nd);
}

JVoid CVisiteurRemove::VisiteTitreCouplageLibre( CTitreCouplageLibre* nd )
{
	Remove(nd);
}


/////////////////////////////////////////
JVoid CVisiteurRemove::Remove(INode* nd)
{
	//On recupere le pere
	//IComposite * comp = (IComposite *) m_regroup->GetNodeByID( nd->GetParentID() );
	IComposite * comp = (IComposite *) nd->GetParentNode();
	//s'il existe 
	if (comp)
		comp->m_LstNode.RemoveNode(nd);

	//RAZ de l'ID du parent
	nd->SetParentID(0);
	nd->SetParentNode(NULL);
}
