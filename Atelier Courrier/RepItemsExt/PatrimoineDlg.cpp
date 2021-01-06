// PatrimoineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ItemConstruit.h"
#include "ItemsConstruitsManager.h"
#include "PatrimoineDlg.h"

#include "TerrainManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPatrimoineDlg dialog

IMPLEMENT_DYNAMIC(CPatrimoineDlg, CDialog)
CPatrimoineDlg::CPatrimoineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatrimoineDlg::IDD, pParent)
{
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
}

CPatrimoineDlg::~CPatrimoineDlg()
{
}

void CPatrimoineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPatrimoineDlg, CDialog)
	ON_BN_CLICKED(IDC_RI_BUT_START, OnStart)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RI_TREE, OnNMCustomdrawTree)
END_MESSAGE_MAP()

BOOL CPatrimoineDlg::OnInitDialog ( void )
{
	CDialog::OnInitDialog(); 

	m_Button.SubclassDlgItem(IDC_RI_BUT_START, this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_Tree.SubclassDlgItem(IDC_RI_TREE,this);

	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Tree);
	return true;
}


UINT CPatrimoineDlg::LoopTerrains(LPVOID pParam)
{
	KeyItem Key;
	Key.m_SourceID = 0; 
	Key.m_ItemID = 0;

	CWaitCursor Wait;

	const JSRCTables & Sources = JSRCTables::GetInstance ();
	for(Sources.m_TBLTerrains.MoveFirst(); Sources.m_TBLTerrains.IsValid(); Sources.m_TBLTerrains.MoveNext())
	{
		const JTerrain & Ter = Sources.m_TBLTerrains.GetItem();
		const JSource * pSrc = Sources.m_TBLSources.GetItem(Ter.m_IdSource);
		////JStringEx Name;
		//Name.Append(Ter.m_Chemin.AsJCharPtr());

		//CTerrain *pTerrain = new CTerrain;
		//if(pTerrain->Load(Name))
		CTerrainManager* terrainManager = CTerrainManager::GetInstance();
		CTerrain *pTerrain = terrainManager->GetTerrain(Ter.m_IdSource, Ter.m_IdTerrain, false);
		if(pTerrain)
		{
			if(pSrc)
				LoopItems(pTerrain, pSrc->m_Libelle.AsJCharPtr() ,Ter);
			else
				LoopItems(pTerrain, 0, Ter);
			
			terrainManager->ReleaseTerrain(pTerrain);
		}
		//delete pTerrain;
	}
	return 0;
}

void CPatrimoineDlg::LoopItems(CTerrain *pTerrain,LPCSTR LibelleSrc,const JTerrain &Ter)
{
	KeyItem Key;
	HTREEITEM hParent,hChild;
	Key.m_SourceID = pTerrain->GetSourceID(); 

	// CItemsConstruits * pItems = CItemsConstruits::GetInstance();
	CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	JList<const IItem *> Items;
	// Get Items independant
	pItems->GetItemsIndependant(Items,pTerrain);
	// Get Criteres Modifie
	pItems->GetCriteresModifieByBlocID(Items,0,pTerrain);
	// Get Criteres Quantiles
	pItems->GetCriteresQuantileByBlocID(Items,0,pTerrain);
	// Get Palettes
	pItems->GetPalettes(Items,pTerrain); 

	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext ())
	{
		Key.m_ItemID = Items.GetItem()->GetID();
		m_hSource.MoveTo(Key);
		if(m_hSource.IsValid())
			hParent = m_hSource.GetItem();
		else
		{
			m_hItem.MoveTo(Key.m_ItemID);
			if(!m_hItem.IsValid())
			{
				m_hItem.Add(Key.m_ItemID) = m_Tree.InsertItem(Items.GetItem()->GetLabel().AsJCharPtr(),0,0,TVI_ROOT,TVI_SORT);
				m_hItem.MoveTo(Key.m_ItemID);
			}
			hParent = m_hItem.GetItem();
			m_hSource.Add(Key) = m_Tree.InsertItem(LibelleSrc,0,0,hParent,TVI_SORT);
			m_hSource.MoveTo(Key);
			hParent = m_hSource.GetItem();
			m_Tree.Expand(hParent,TVE_EXPAND);
		}
		hChild = m_Tree.InsertItem(Ter.m_Libelle.AsJCharPtr(),0,0,hParent,TVI_SORT);

		const CItemConstruit * pItem = Items.GetItem()->AsItemConstruit();
		if(pItem && pItem->IsSouche(Key.m_SourceID,pTerrain->GetTerrainID()))
		{
			m_Tree.SetItemData(hChild,1);
		}
		else
		{
			m_Tree.SetItemData(hChild,0);
		}
	}

	/* Test
	HTREEITEM RootItem = m_Tree.GetRootItem();
	m_Tree.Expand(RootItem, TVE_EXPAND); 
	ExpandAll(RootItem);
	*/

	UpdateWindow();

	// pItems->Release();
}

void CPatrimoineDlg::ExpandAll(HTREEITEM hti)
{
	while(hti)
	{
		// Y a t-il une branche enfant
		if (m_Tree.ItemHasChildren(hti))
		{
			m_Tree.Expand(hti,TVE_EXPAND);
			ExpandAll(m_Tree.GetChildItem(hti));
		}
		hti = m_Tree.GetNextItem(hti,TVGN_NEXT);
	}
}

void CPatrimoineDlg::OnStart()
{
	m_Button.EnableWindow(FALSE);
	m_Tree.DeleteAllItems();
	LoopTerrains(0);
	m_Button.EnableWindow(TRUE);

	// Test
	/*
	HTREEITEM RootItem = m_Tree.GetRootItem();
	m_Tree.Expand(RootItem, TVE_EXPAND); 
	ExpandAll(RootItem);
	*/
}

void CPatrimoineDlg::OnNMCustomdrawTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;
	switch (pLVCD->nmcd.dwDrawStage)
	{
		case CDDS_ITEM:
		case CDDS_POSTPAINT:
		case CDDS_PREERASE:
		case CDDS_POSTERASE:
		case CDDS_ITEMPOSTPAINT :
		case CDDS_ITEMPREERASE :
		case CDDS_ITEMPOSTERASE :
			break ;

		case CDDS_PREPAINT :
			*pResult = CDRF_NOTIFYITEMDRAW  ;
			break ;
		case CDDS_ITEMPREPAINT :
			if(pLVCD->nmcd.lItemlParam == 1) 
			{
				pLVCD->clrTextBk = CATPColors::GetColorSelect(m_CATPColorsApp) ;
				pLVCD->clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

			}
			else
			{
				pLVCD->clrTextBk = ::GetSysColor(COLOR_WINDOW);
				pLVCD->clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			}
			break ;
	}
}
