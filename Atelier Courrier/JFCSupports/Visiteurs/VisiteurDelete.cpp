// VisiteurDelete.cpp: implementation of the CVisiteurDelete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisiteurDelete.h"
#include "JTBLTitres.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVisiteurDelete::CVisiteurDelete()
{
	m_regroup = CRegroupements::GetInstance();
}

CVisiteurDelete::~CVisiteurDelete()
{
}

JVoid CVisiteurDelete::VisiteFamille(CFamille* nd)
{
	//on passe le visiteur aux fils
 	DeleteFils(nd);
	//on retire la famille de la liste des familles
	int Nb = m_regroup->m_lstFamille.GetCount(); 
	m_regroup->m_lstFamille.RemoveNode(nd);
	Nb = m_regroup->m_lstFamille.GetCount(); 
	delete nd;
}

JVoid CVisiteurDelete::VisiteCentre(CCentre* nd)
{
	//on passe le visiteur aux fils
	DeleteFils(nd);
	//on le retire de la famille
	DeleteInParent(nd);
	//on retire le centre de la liste des centres
	m_regroup->m_lstCentre.RemoveNode(nd);
	//enfin on le delete
	delete nd;
}

JVoid CVisiteurDelete::VisiteCouplageLibre( CCouplageLibre* nd )
{
	//on passe le visiteur aux fils
	DeleteFils(nd);
	//on retire le CouplageLibre de la liste des Couplages Libres
	m_regroup->m_lstCouplageLibre.RemoveNode(nd);

	//il faut supprimer les tarif manuels associées au couplage
	//CTarifs* pTarifs = CTarifs::GetInstance();
	//for (pTarifs->m_TarifsManuels.MoveFirst(); pTarifs->m_TarifsManuels.IsValid(); pTarifs->m_TarifsManuels.MoveNext() )
	//{
	//	CTarifManuel* trf = pTarifs->m_TarifsManuels.GetItem();
	//	if (trf->GetIDSupport() == nd->GetID() )
	//		pTarifs->m_TarifsManuels.RemoveTarifManuel(trf);
	//}

	//enfin on le delete
	delete nd;
}

JVoid CVisiteurDelete::VisiteLot( CLot* nd )
{
	//on passe le visiteur aux fils
	DeleteFils(nd);
	//on retire le lot de la liste des lots
	m_regroup->m_lstLot.RemoveNode(nd);
	//enfin on le delete
	delete nd;
}

JVoid CVisiteurDelete::VisiteTitre(CTitre* nd)
{
	//on le retire du centre parent
	DeleteInParent(nd);
	//on le retire de la liste des titres
	m_regroup->m_lstTitre.RemoveNode(nd);

	//on verifie si on doit replacer ce titre dans la liste des non attribués ou pas
	if (! m_regroup->GetTitreByID(nd->GetID()) )
	{
		nd->SetParentID(0);
		nd->SetParentNode(NULL);
		m_regroup->m_mapFamilleNA.Add( nd->GetID() ) = nd;
	}
	else	//on le delete
		delete nd;
}

JVoid CVisiteurDelete::VisiteTitreApparente(CTitreApparente* nd)
{
	//on le retire du centre
	DeleteInParent(nd);
	CTitreApparente* tatmp = NULL;
	(CTitreApparente*)m_regroup->m_lstTitreApparente.GetNodeByID(nd->GetID());
	tatmp = (CTitreApparente*)m_regroup->m_lstTitreApparente.GetNextNodeByID(nd->GetID());

	//si nd est le dernier titre app de cet ID
	if (!tatmp)
		nd->SetParentID(0);
	else //sinon on le retire de la liste
	{
		m_regroup->m_lstTitreApparente.RemoveNode(nd);
		delete nd;
	}


}

JVoid CVisiteurDelete::VisiteTitreLot(CTitreLot* nd)
{
	//on le retire de la famille
	DeleteInParent(nd);
	///enfin on le delete
	delete nd;
}

JVoid CVisiteurDelete::VisiteTitreCouplageLibre(CTitreCouplageLibre* nd)
{
	//on le retire de la famille
	DeleteInParent(nd);
	///enfin on le delete
	delete nd;
}

//////////////////////////////////////////////
JVoid CVisiteurDelete::DeleteInParent(INode* nd)
{
	//On recupere le pere
	//IComposite * pere = (IComposite *) m_regroup->GetNodeByID( nd->GetParentID() );
	IComposite * pere = (IComposite *) nd->GetParentNode();
	//s'il existe 
	if (pere)
		pere->m_LstNode.RemoveNode(nd);

}


JVoid CVisiteurDelete::DeleteFils(IComposite* nd)
{
	//on delete les fils
	nd->m_LstNode.MoveLast();
	//tant que la liste n'est pas vide
	while ( nd->m_LstNode.IsValid() )
	{
		//on recupere le dernier elem
		INode * node = nd->m_LstNode.GetItem();
		//qu'on retire de la liste
//les fifs s'effacent tt seuls de leur pere
//		nd->m_LstNode.Remove(-1);
		//on lui retire ses fils (recursivité)
		node->Accept(this);
	}
}
