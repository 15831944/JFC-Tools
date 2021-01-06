// VisiteurDeleteTA.cpp: implementation of the CVisiteurDeleteTA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisiteurDeleteTA.h"
#include "..\Iterateurs\Iterateur.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVisiteurDeleteTA::CVisiteurDeleteTA()
{
}

CVisiteurDeleteTA::~CVisiteurDeleteTA()
{
}

JVoid CVisiteurDeleteTA::VisiteTitreApparente(CTitreApparente* nd)
{
	JUnt32 idtmp = nd->GetID();
	//on efface le titre apparenté de cet id dans tous les centres

	m_regroup->m_lstTitreApparente.MoveFirst();
	while ( m_regroup->m_lstTitreApparente.IsValid() )
	{
		if ( m_regroup->m_lstTitreApparente.GetItem()->GetID() == idtmp)
		{
			//on le retire de son centre
			DeleteInParent( m_regroup->m_lstTitreApparente.GetItem() );
			
			//le retirer également des lots et des couplages
			CVisiteurDelete vDelete;
			//lot
			JNodeList * lstLots = m_regroup->GetLots();
			for( lstLots->MoveFirst(); lstLots->IsValid(); lstLots->MoveNext() )
			{
				INode* lot = lstLots->GetItem();
				IIterateur* it = lot->CreateIterateur();
				CTitreLot* titrelot = (CTitreLot*)it->getHead();
				while(titrelot)
				{
					if (titrelot->GetID() == nd->GetID())
						titrelot->Accept(&vDelete);
					titrelot = (CTitreLot*)it->getNext();
				}
				delete it;
			}
			//couplage
			JNodeList * lstCpl = m_regroup->GetCouplagesLibres();
			for( lstCpl->MoveFirst(); lstCpl->IsValid(); lstCpl->MoveNext() )
			{
				INode* cpl = lstCpl->GetItem();
				IIterateur* it = cpl->CreateIterateur();
				CTitreCouplageLibre* titrecpl = (CTitreCouplageLibre*)it->getHead();
				while(titrecpl)
				{
					if (titrecpl->GetID() == nd->GetID())
						titrecpl->Accept(&vDelete);
					titrecpl = (CTitreCouplageLibre*)it->getNext();
				}
				delete it;
			}

			//mais aussi supprimer les tarif manuels associées au titre
			//CTarifs* pTarifs = CTarifs::GetInstance();
			//for (pTarifs->m_TarifsManuels.MoveFirst(); pTarifs->m_TarifsManuels.IsValid(); pTarifs->m_TarifsManuels.MoveNext() )
			//{
			//	CTarifManuel* trf = pTarifs->m_TarifsManuels.GetItem();
			//	if (trf->GetIDSupport() == nd->GetID() )
			//		pTarifs->m_TarifsManuels.RemoveTarifManuel(trf);
			//}

				
			//on peux enfin l'effacer
			delete m_regroup->m_lstTitreApparente.GetItem();
			m_regroup->m_lstTitreApparente.Remove(1);
		}
		else
			m_regroup->m_lstTitreApparente.MoveNext();
	}

}