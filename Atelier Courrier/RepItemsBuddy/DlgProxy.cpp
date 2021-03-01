// DlgProxy.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "RepItemsBuddy.h"
#include "DlgProxy.h"
#include "RepItemsBuddyDlg.h"
#include ".\dlgproxy.h"
#include "ItemCible.h"
#include "Vect_Poids.h"
#include "Vect_Item.h"
#include "TerrainManager.h"
#include "JSRCTables.h"

#include "JFCConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 

// CRepItemsBuddyProxy

IMPLEMENT_DYNCREATE(CRepItemsBuddyProxy, CCmdTarget)

CRepItemsBuddyProxy::CRepItemsBuddyProxy()
{
	EnableAutomation();
	
	// Pour que l'application reste exécutée tant qu'un 
	//	objet Automation est actif, le constructeur appelle AfxOleLockApp.
	AfxOleLockApp();

	// Accéder à la boîte de dialogue par l'intermédiaire du pointeur
	//  de la fenêtre principale de l'application. Définir le pointeur interne du proxy
	//  pour qu'il pointe vers la boîte de dialogue, et définit le pointeur de retour de la boîte de dialogue pour qu'il pointe vers
	//  ce proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CRepItemsBuddyDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CRepItemsBuddyDlg)))
		{
			m_pDialog = reinterpret_cast<CRepItemsBuddyDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;


			JFCConfig::CreateInstance("C:\\Temp\\test.ini");

			m_pRepItems = new CDlgItems();
			m_pRepItems->SetShowMode(SHOWMODE_SIMPLE);
			m_pTerrain = NULL;
		}
	}
}

CRepItemsBuddyProxy::~CRepItemsBuddyProxy()
{
	// Pour quitter l'application quand tous les objets ont été créés
	// 	avec Automation, le destructeur appelle AfxOleUnlockApp.
	//  Cela aura notamment pour effet de détruire la boîte de dialogue principale
	delete m_pRepItems;
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CRepItemsBuddyProxy::OnFinalRelease()
{
	// Lorsque la dernière référence pour un objet automation est libérée
	// OnFinalRelease est appelé. La classe de base supprime automatiquement
	// l'objet. Un nettoyage supplémentaire est requis pour votre
	// objet avant d'appeler la classe de base.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CRepItemsBuddyProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRepItemsBuddyProxy, CCmdTarget)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "ShowRepItem", dispidShowRepItem, ShowRepItem, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "LoadTerrain", dispidLoadTerrain, LoadTerrain, VT_I1, VTS_I4 VTS_I4)

	DISP_PROPERTY_EX_ID(CRepItemsBuddyProxy, "Vect_Poids", dispidVect_Poids, GetVect_Poids, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "MoveFirstItem", dispidMoveFirstItem, MoveFirstItem, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "IsValidItem", dispidIsValidItem, IsValidItem, VT_I1, VTS_NONE)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "MoveNextItem", dispidMoveNextItem, MoveNextItem, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "GetItem", dispidGetItem, GetItem, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "GetVect_Item", dispidGetVect_Item, GetVect_Item, VT_DISPATCH, VTS_I4)			
	
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "GetListPoids", dispidGetListPoids, GetListPoids, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CRepItemsBuddyProxy, "SetPoids", dispidSetPoids, SetPoids, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// Remarque : La prise en charge de IID_IRepItemsBuddy a été ajoutée afin de prendre en charge la liaison de type sécurisé
//  fournie par VBA. Cet IID doit correspondre au GUID qui est associé à
//  dispinterface dans le fichier .IDL.

// {F6498CEF-C703-46A2-822E-8A9F8FE2184B}
static const IID IID_IRepItemsBuddy =
{ 0xF6498CEF, 0xC703, 0x46A2, { 0x82, 0x2E, 0x8A, 0x9F, 0x8F, 0xE2, 0x18, 0x4B } };

BEGIN_INTERFACE_MAP(CRepItemsBuddyProxy, CCmdTarget)
	INTERFACE_PART(CRepItemsBuddyProxy, IID_IRepItemsBuddy, Dispatch)
END_INTERFACE_MAP()

// La macro IMPLEMENT_OLECREATE2 est définie dans le fichier StdAfx.h de ce projet
// {FFD031BB-3C04-4866-9097-68BEFD040655}
IMPLEMENT_OLECREATE2(CRepItemsBuddyProxy, "RepItemsBuddy.Application", 0xffd031bb, 0x3c04, 0x4866, 0x90, 0x97, 0x68, 0xbe, 0xfd, 0x4, 0x6, 0x55)


// gestionnaires de messages pour CRepItemsBuddyProxy

void CRepItemsBuddyProxy::ShowRepItem() 
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	AfxGetMainWnd()->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//m_pRepItems->Show(); 
	m_pRepItems->DoModal();
}


BOOL CRepItemsBuddyProxy::LoadTerrain(LONG idSrc, LONG idTerr)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_pTerrain)
		CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);

	m_idSrc = idSrc;
	m_idTerr = idTerr;
	//m_pRepItems->SetPreloadTerrain(idSrc,idTerr,idPoids);
	//m_idTypePoids = idPoids;

	CKeyTerrain key(idSrc,idTerr);
	m_pTerrain = CTerrainManager::GetInstance()->GetTerrain(key);
	if(m_pTerrain)
	{
		m_lstItem.Reset();
		CItemsConstruits* pItems = CItemsConstruits::GetInstance();
		if(pItems->GetItemsIndependant(m_lstItem,m_pTerrain))
			return TRUE;
	}

	return FALSE;
}

BSTR CRepItemsBuddyProxy::GetListPoids()
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CString str;

	if(m_pTerrain)
	{
		JVector<JInt32> Poids;
		if(m_pTerrain->GetTypesPoids(Poids))
		{
			const JSRCTables & srcTables = JSRCTables::GetInstance(); 
			// Update the Poid list
			for(Poids.MoveFirst(); Poids.IsValid(); Poids.MoveNext())
			{
				JChar * pLibelle = srcTables.m_TBLPoids.GetLabelByID(Poids.GetItem());
				if(pLibelle)
					str.AppendFormat("%d;%s;",Poids.GetItem(),pLibelle);
			}
		}
	}
	
    return str.AllocSysString();
}


void CRepItemsBuddyProxy::SetPoids(LONG idPoids)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	m_pRepItems->SetPreloadTerrain(m_idSrc,m_idTerr,idPoids);
	m_idTypePoids = idPoids;

}

IDispatch* CRepItemsBuddyProxy::GetVect_Poids(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_pTerrain)
	{
		CVect_Poids* pVect = new CVect_Poids;
		pVect->m_vect = (JDBLVECTOR*)m_pTerrain->GetStatisticValues(m_pTerrain->GetDefaultEchantillonID(m_pTerrain->GetDefaultUnivers()),
										m_pTerrain->GetDefaultUnivers(),
										m_idTypePoids/*m_pTerrain->GetDefaultTypePoidsID()*/ );
	
		
		return pVect->GetIDispatch(TRUE);
	}
	return NULL;
}

void CRepItemsBuddyProxy::MoveFirstItem(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_lstItem.MoveFirst();
}

BOOL CRepItemsBuddyProxy::IsValidItem(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if(m_lstItem.IsValid())
		return TRUE;
	return FALSE;
}

void CRepItemsBuddyProxy::MoveNextItem(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	m_lstItem.MoveNext();
}

IDispatch* CRepItemsBuddyProxy::GetItem(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if(m_lstItem.IsValid())
	{
		const IItem * item = m_lstItem.GetItem();
		if(item)
		{
			CItemCible * itemCible = new CItemCible;
			itemCible->m_ident = item->GetID();
			itemCible->m_label = item->GetLabel().AsJCharPtr();
			return itemCible->GetIDispatch(TRUE);
		}
	}

	return NULL;
}


IDispatch* CRepItemsBuddyProxy::GetVect_Item(LONG idItem)  
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_pTerrain)
	{
		CItemsConstruits* pItems = CItemsConstruits::GetInstance();
		CItemConstruit * pItem = pItems->GetItemConstruit(idItem);
		if(pItem)
		{

			CVect_Item* pVectItem = new CVect_Item;
			CCalcItemParams Params;// = new CCalcItemParams();

			pVectItem->m_params.SetTerrain(m_pTerrain);
			pVectItem->m_params.SetEchantillonID(m_pTerrain->GetDefaultEchantillonID(m_pTerrain->GetDefaultUnivers()));
			pVectItem->m_params.SetUniversID(m_pTerrain->GetDefaultUnivers());
			pVectItem->m_params.SetTypePoidsID(m_idTypePoids/*m_pTerrain->GetDefaultTypePoidsID()*/);

			CBitArray* bArrPopSeg = new CBitArray();
			JUnt32 contingent =  0xFFFF;
			m_pTerrain->GetDefaultPopSegment(*bArrPopSeg, contingent);
			pVectItem->m_params.SetPopulationSegment(*bArrPopSeg);

			JVector<CSegment*> vectSeg;
			JList<JInt32> vectIdSeg;
			m_pTerrain->GetSegments(vectSeg);
			for(vectSeg.MoveFirst(); vectSeg.IsValid(); vectSeg.MoveNext())
				vectIdSeg.AddTail() = vectSeg.GetItem()->GetID();
			pVectItem->m_params.SetSegmentsIDs(vectIdSeg);

			if(!pItem->CalculateEffectif(&pVectItem->m_params))
				return NULL;

			if (!pItem->Calculate(&pVectItem->m_params))
				return NULL;

			return pVectItem->GetIDispatch(TRUE);
		}
	}

	return NULL;
}

void CRepItemsBuddyProxy::ListIndiv(LPCTSTR List)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// TODO : ajoutez ici le code de votre gestionnaire de dispatch


	List="totot";
}