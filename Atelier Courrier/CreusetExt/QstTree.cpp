// QstTree.cpp : implementation file
//

#include "stdafx.h"
#include "QstTree.h"
#include ".\qsttree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQstTree
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )


CQstTree::CQstTree()
{
	m_CATPColorsApp = CATPColors::COLORCREUSET;
}

CQstTree::~CQstTree()
{
}


BEGIN_MESSAGE_MAP(CQstTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnTvnItemexpanded)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQstTree message handlers


void CQstTree::SetData(CTerrain *pTerrain, CItemsConstruitsBase * pIConst, JList<JInt32>& selIDlist)
{
	// Remove all items
	DeleteAllItems();
	m_pTerrain = pTerrain;
	m_pIConst = pIConst;

#define NBICON 2
	HICON hIcon[NBICON];
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_CR_THEME);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_CR_QUESTION);

	m_Imagelist.DeleteImageList ();
	m_Imagelist.Create(16, 16, ILC_COLOR32 , 8, 8); // 32, 32 pour grandes icônes
	for (int n = 0; n < NBICON; n++)
		m_Imagelist.Add(hIcon[n]);
	SetImageList(&m_Imagelist, TVSIL_NORMAL);

	if(pTerrain->IsLoaded())
	{
		CString str;
		str.LoadString(IDS_CR_INDEXTHEM);
		HTREEITEM hPar = InsertItem(TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT,str,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,0,0);
		pTerrain->FillIndexThematique(*((CTreeCtrl *)this),hPar);
		FilterTree(hPar);
		AddQuestions(hPar, selIDlist);
		Expand(hPar,TVE_EXPAND);
		SelectItem(hPar);
		//SetItemState(hPar, 0, TVIS_SELECTED);
		SelectItem(NULL);

		/*ou*/
	/*	pTerrain->FillIndexThematique(*((CTreeCtrl *)this),TVI_ROOT);
		AddQuestions(TVI_ROOT, selIDlist);
		Expand(TVI_ROOT,TVE_EXPAND);*/
	}
}

void CQstTree::AddQuestions(HTREEITEM hPar, JList<JInt32>& selIDlist)
{
	HTREEITEM hTmp = GetChildItem(hPar);

	while(hTmp)
	{
		if (ItemHasChildren(hTmp))
			AddQuestions(hTmp, selIDlist);
		else
		{
			ITheme * pTheme = (ITheme *)GetItemData(hTmp);
			if(pTheme && pTheme->IsBlocQuestion())
			{
				JList<IQuestion const *> Question_List;
				m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), Question_List); 
				//m_pIConst->GetCriteresModifieByBlocID(Question_List,pTheme->GetID(),m_pTerrain);
				//m_pIConst->GetCriteresQuantileByBlocID(Question_List,pTheme->GetID(),m_pTerrain);
				//m_pIConst->GetQuestionsInduiteByBlocID(Question_List,pTheme->GetID(),m_pTerrain);
	
				for(Question_List.MoveFirst(); Question_List.IsValid(); Question_List.MoveNext())
				{
					IQuestion const * qst  = Question_List.GetItem();

					// Vérifie si c'est une question
					const CQuestion *pQu = qst->AsQuestion();
					if (pQu)
					{
						if(!qst->IsQuestion() || (qst->HasDimensions() == 1 && !qst->AsQuestion()->IsQuantiPure()))
						{
							TVINSERTSTRUCT Ins;
							Ins.hParent				=	hTmp;
							Ins.hInsertAfter		=	TVI_LAST;
							Ins.item.mask			=	TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT;
							Ins.item.state			=	INDEXTOSTATEIMAGEMASK(12);
							Ins.item.stateMask		=	TVIS_STATEIMAGEMASK|0x000F ;
							Ins.item.pszText		=	(char *)qst->GetLabel().AsJCharPtr();
							Ins.item.cchTextMax		=	255;
							Ins.item.iImage			=	1;
							Ins.item.iSelectedImage	=	1;
							Ins.item.cChildren		=	0;
							Ins.item.lParam			=	reinterpret_cast<LPARAM>(qst);	

							HTREEITEM hItem = InsertItem(&Ins);
							//ajoute l'element dans le map
							m_mapItemSel.Add(hItem, true) = false;
							
							for(selIDlist.MoveFirst(); selIDlist.IsValid(); selIDlist.MoveNext())
							{
								if(selIDlist.GetItem() == qst->GetID())
								{
									//SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
									m_mapItemSel.GetItem() = true;
									EnsureVisible(hItem);
								}
							}//for

						}//if
					}
				}//for
			}//if
		}//else
		hTmp = GetNextSiblingItem(hTmp);
	}
}

void CQstTree::FilterTree(HTREEITEM hti)
{
	//filtrage des questions en fonction des droits d'accès (voir mm méthode dans CThemeTree de RepItemsExt)
	while(hti)
	{
		HTREEITEM DelItem = 0;

		if (ItemHasChildren(hti))
		{
			FilterTree(GetChildItem(hti));
			if(GetChildItem(hti) == 0)
				DelItem = hti;
		}
		else
		{
			ITheme * pTheme = (ITheme *)GetItemData(hti);
			if(pTheme->IsBlocQuestion() && !m_pTerrain->HasQuestionsByBlocID(pTheme->GetID()))
				DelItem = hti;
		}
		hti = GetNextItem(hti,TVGN_NEXT);
		if(DelItem != 0)
			DeleteItem(DelItem);
	}
}

void CQstTree::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	// Take the default processing unless we set this to something else below.
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
			HTREEITEM hItem = (HTREEITEM)pLVCD->nmcd.dwItemSpec;
			m_mapItemSel.MoveTo(hItem);
			if(m_mapItemSel.IsValid())
			{
				if (m_mapItemSel.GetItem() == true)
				{
					pLVCD->clrTextBk = CATPColors::GetColorSelect(m_CATPColorsApp) ;
					pLVCD->clrText = CATPColors::GetColorWhite() ;
				}
			}
			//if ( pLVCD->nmcd.uItemState&CDIS_SELECTED )
			//{
			//	pLVCD->clrTextBk = CATPColors::GetColorSelect(m_CATPColorsApp) ;
			//}
			break ;
	}
}

void CQstTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT flag;
	HTREEITEM hItem = HitTest( point, &flag );
    if( hItem && flag & TVHT_ONITEM)
	{

		m_mapItemSel.MoveTo(hItem);
		if(m_mapItemSel.IsValid())
			m_mapItemSel.GetItem() = !m_mapItemSel.GetItem();
		
		HTREEITEM hItemOld = GetSelectedItem();
		NotifySelChanged(hItemOld, hItem);
		Invalidate();
		return;
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CQstTree::NotifySelChanged(HTREEITEM hItemFrom, HTREEITEM hItemTo)
{
	NM_TREEVIEW tv;
	memset(&tv.itemOld, 0, sizeof(tv.itemOld));

	if ( hItemFrom )
	{
		tv.itemOld.hItem = hItemFrom;
		tv.itemOld.state = GetItemState( hItemFrom, 0xffffffff );
		tv.itemOld.lParam = GetItemData( hItemFrom );
		tv.itemOld.mask = TVIF_HANDLE|TVIF_STATE|TVIF_PARAM;
	}

	tv.hdr.hwndFrom = GetSafeHwnd();
	tv.hdr.idFrom = GetWindowLong( GetSafeHwnd(), GWL_ID );
	tv.hdr.code = TVN_SELCHANGED;

	tv.itemNew.hItem = hItemTo;
	tv.itemNew.state = GetItemState( hItemTo, 0xffffffff );
	tv.itemNew.lParam = GetItemData( hItemTo );
	tv.itemNew.mask = TVIF_HANDLE|TVIF_STATE|TVIF_PARAM;

	tv.action = TVC_BYMOUSE;

	CWnd* pWnd = GetParent();
	if ( pWnd )
		pWnd->SendMessage( WM_NOTIFY, tv.hdr.idFrom, (LPARAM)&tv );

}


void CQstTree::ClearSelection()
{
	for(m_mapItemSel.MoveFirst();m_mapItemSel.IsValid();m_mapItemSel.MoveNext())
		m_mapItemSel.GetItem() = false;

	SelectItem(NULL);

}


HTREEITEM CQstTree::GetFirstSelectedItem()
{
	m_mapItemSel.MoveFirst();
	while(m_mapItemSel.IsValid() && m_mapItemSel.GetItem() == false)
		m_mapItemSel.MoveNext();

	if(m_mapItemSel.IsValid())
		return m_mapItemSel.GetKey();
	return NULL;
	
}

HTREEITEM CQstTree::GetNextSelectedItem( HTREEITEM hItem )
{
	m_mapItemSel.MoveTo(hItem);
	m_mapItemSel.MoveNext();
	while(m_mapItemSel.IsValid() && m_mapItemSel.GetItem() == false)
		m_mapItemSel.MoveNext();

	if(m_mapItemSel.IsValid())
		return m_mapItemSel.GetKey();

	return NULL;

	

}


HTREEITEM CQstTree::GetPrevSelectedItem( HTREEITEM hItem )
{
	m_mapItemSel.MoveTo(hItem);
	m_mapItemSel.MovePrev();
	while(m_mapItemSel.IsValid() && m_mapItemSel.GetItem() == false)
		m_mapItemSel.MovePrev();

	if(m_mapItemSel.IsValid())
		return m_mapItemSel.GetKey();

	return NULL;
}




void CQstTree::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	if ( pNMTreeView->action == TVE_COLLAPSE )
	{
		SetItemState( pNMTreeView->itemNew.hItem, 0, TVIS_SELECTED );
	}

	*pResult = 0;
}


