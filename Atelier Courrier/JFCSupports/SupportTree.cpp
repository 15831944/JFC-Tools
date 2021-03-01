#include "stdafx.h"
#include "Resource.h"

#include "SupportTree.h"

#include <AllTypeOfVisitor.h>
#include <AllTypeOfIterator.h>
#include <IDLimits.h>
#include ".\supporttree.h"
#include "ATPMessages.h"


// main symbols

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CSupportTree

//Pour utilisersKindOf
IMPLEMENT_DYNAMIC( CSupportTree, CTreeCtrl )

#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

CSupportTree::CSupportTree()
{
	m_bDragging = FALSE;
	m_pimagelist = NULL;
	m_hitemDrag = NULL;
	m_bSelectionComplete = true;
	m_appColor = 0;
	m_selColor = ::GetSysColor(COLOR_HIGHLIGHT);
	m_ColorAttribue = ::GetSysColor(COLOR_WINDOWTEXT);
	m_bEditable = false;
	m_bDragDrop = false;
	m_bFamNA = true;
	m_bSupports = true;
	m_bExpandAll = false;
	m_bMultiSel = false;
	m_pchTip = NULL;
	m_pwchTip = NULL;

}

CSupportTree::~CSupportTree()
{
}

void CSupportTree::SetData( CRegroupements * pRegroup )
{
	m_pRegroupements = pRegroup;

	HICON hIcon[NBICON];
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_SU_ICON_ALL);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_SU_ICON_TITRE0);
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_SU_ICON_TITRE1);
	hIcon[3] = AfxGetApp()->LoadIcon(IDI_SU_ICON_TITRE2);

	hIcon[ICON_F] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_BGREEN);
	hIcon[ICON_F+1] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_LGREEN);
	hIcon[ICON_F+2] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_BLUE);
	hIcon[ICON_F+3] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_ORANGE);
	hIcon[ICON_F+4] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_BROWN);
	hIcon[ICON_F+5] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_GREY);
	hIcon[ICON_F+6] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_GREY);
	hIcon[ICON_F+7] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_ROSE);
	hIcon[ICON_F+8] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_BROWN);
	hIcon[ICON_F+9] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_BROWN);
	hIcon[ICON_F+10] = AfxGetApp()->LoadIcon(IDI_SU_ICON_F_ICE_BLUE);

	hIcon[ICON_C] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_BGREEN);
	hIcon[ICON_C+1] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_LGREEN);
	hIcon[ICON_C+2] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_BLUE);
	hIcon[ICON_C+3] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_ORANGE);
	hIcon[ICON_C+4] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_BROWN);
	hIcon[ICON_C+5] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_GREY);
	hIcon[ICON_C+6] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_GREY);
	hIcon[ICON_C+7] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_ROSE);
	hIcon[ICON_C+8] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_BROWN);
	hIcon[ICON_C+9] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_BROWN);
	hIcon[ICON_C+10] = AfxGetApp()->LoadIcon(IDI_SU_ICON_C_ICE_BLUE);

	m_Imagelist.DeleteImageList ();
	m_Imagelist.Create(16, 16, ILC_COLOR32 , 8, 8); // 32, 32 pour grandes icônes
	for (int n = 0; n < NBICON; n++)
		m_Imagelist.Add(hIcon[n]);

	SetImageList(&m_Imagelist, TVSIL_NORMAL);

	FillTree();
}

void CSupportTree::FillTree()
{
	DeleteAllItems();

	TV_INSERTSTRUCT     curTreeItem;
	curTreeItem.hInsertAfter = TVI_LAST;
	curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	
	curTreeItem.hParent =  NULL;
	curTreeItem.item.iImage = ICON_ALL;
	curTreeItem.item.iSelectedImage = ICON_ALL;
	CString str; str.LoadString(IDS_SU_TTFAMILLES);
	curTreeItem.item.pszText = str.GetBuffer();
	str.ReleaseBuffer();
	curTreeItem.item.lParam = (LPARAM)NULL;
	HTREEITEM hroot = InsertItem(&curTreeItem);

	for (m_pRegroupements->GetFamilles()->MoveFirst(); 
		m_pRegroupements->GetFamilles()->IsValid();
		m_pRegroupements->GetFamilles()->MoveNext())
		InsertTree( m_pRegroupements->GetFamilles()->GetItem(), 0, hroot );
	SortChildren(hroot);

	if (m_bFamNA)
	{
		TV_INSERTSTRUCT     curTreeItem;
		curTreeItem.hInsertAfter = TVI_LAST;
		curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
		
		curTreeItem.hParent =  hroot;
		curTreeItem.item.iImage = ICON_ALL;
		curTreeItem.item.iSelectedImage = ICON_ALL;
		CString str; str.LoadString(IDS_SU_TITRESNA);
		curTreeItem.item.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		curTreeItem.item.lParam = (LPARAM)NULL;
		HTREEITEM hFNA = InsertItem(&curTreeItem);
		if (m_bSupports)//on fait le test ici pour s'economiser la boucle
		for (m_pRegroupements->GetTitresNA()->MoveFirst(); 
			m_pRegroupements->GetTitresNA()->IsValid();
			m_pRegroupements->GetTitresNA()->MoveNext())
			InsertTree( m_pRegroupements->GetTitresNA()->GetItem(), 0, hFNA );
		if (m_bSupports)//on fait le test ici pour s'economiser la boucle
		for ( m_pRegroupements->GetTitresApparentes()->MoveFirst(); m_pRegroupements->GetTitresApparentes()->IsValid(); m_pRegroupements->GetTitresApparentes()->MoveNext())
		{
			CTitreApparente* titreHS = (CTitreApparente*)m_pRegroupements->GetTitresApparentes()->GetItem();
			if ( titreHS->GetParentID() == 0 )
				InsertTree( titreHS, 0, hFNA );
		}

	}

	Expand( hroot, TVE_EXPAND );


}

void CSupportTree::InsertTree(INode* nd, int proflim, HTREEITEM hroot, int prof)
{
	//si on n'affiche pas les titres
	if (nd->IsTitre() && !m_bSupports)
		return;

	TV_INSERTSTRUCT     curTreeItem;
	curTreeItem.hInsertAfter = TVI_LAST;
	curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	
	curTreeItem.hParent =  hroot;
	if ( IDFAMILLE_MIN < nd->GetID() && nd->GetID()< IDFAMILLE_MAX )
	{
		curTreeItem.item.iImage = ICON_F + m_appColor;
		curTreeItem.item.iSelectedImage = ICON_F + m_appColor;
	}
	if ( IDCENTRE_MIN < nd->GetID() && nd->GetID()< IDCENTRE_MAX )
	{
		curTreeItem.item.iImage = ICON_C + m_appColor;
		curTreeItem.item.iSelectedImage = ICON_C + m_appColor;
	}
	if ( nd->IsTitre())
	{
		curTreeItem.item.iImage = ICON_TITRE;
		curTreeItem.item.iSelectedImage = ICON_TITRE;
	}
	curTreeItem.item.pszText = (char*)nd->GetLabel().AsJCharPtr();
	curTreeItem.item.lParam = (LPARAM)nd;
	HTREEITEM hpere = InsertItem(&curTreeItem);


	IIterateur *ite = nd->CreateIterateur();
	//on ne le fait que si on a un iterateur (node!=Leaf)
	if (ite)
	{
		INode* node = ite->getHead();
		while(node != NULL && prof != proflim)
		{
			//recursivité
			InsertTree(node, proflim, hpere, prof+1);
			node = ite->getNext();
		}
		delete ite;
	}
	SortChildren(hpere);
	if (m_bExpandAll)
		Expand( hpere, TVE_EXPAND );

}

BEGIN_MESSAGE_MAP(CSupportTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_SU_CTXALL_SUPPR, OnCtxSuppr)
	ON_COMMAND(IDM_SU_CTXALL_ADDF, OnCtxFamilleAdd)

	ON_COMMAND(IDM_SU_CTXFAMILLE_SUPPR, OnCtxSuppr)
	ON_COMMAND(IDM_SU_CTXFAMILLE_RENAME, OnCtxRenommer)
	ON_COMMAND(IDM_SU_CTXFAMILLE_ADDC, OnCtxCentreAdd)

	ON_COMMAND(IDM_SU_CTXCENTRE_SUPPR, OnCtxSuppr)
	ON_COMMAND(IDM_SU_CTXCENTRE_RENAME, OnCtxRenommer)

	ON_COMMAND(IDM_SU_CTXSUPPORT_SUPPR, OnCtxSuppr)

	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT_EX(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDING, OnItemexpanding)

//Infotip
//	ON_NOTIFY_RANGE(TTN_GETDISPINFO, 0, 0xFFFF, OnToolTipText)
//ToolTip
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSupportTree message handlers

void CSupportTree::EnableDragDrop(bool bEnable)
{
	m_bDragDrop = bEnable;
}

void CSupportTree::EnableEdit(bool bEnable)
{
	m_bEditable = bEnable;
	SetNewStyle(TVS_EDITLABELS, bEnable);
}

void CSupportTree::EnableFamilleNA(bool bEnable)
{
	m_bFamNA = bEnable;
}

void CSupportTree::ShowSupport(bool bEnable)
{
	m_bSupports = bEnable;
}
void CSupportTree::EnableExpandAll(bool bEnable)
{
	m_bExpandAll = bEnable;
}

void CSupportTree::EnableMultipleSel(bool bEnable)
{
	m_bMultiSel = bEnable;
}

bool CSupportTree::IsIDSelected(JInt32 id)
{
	bool bRet = false;
	HTREEITEM hItemSelected = GetFirstSelectedItem();
	while(hItemSelected)
	{
		//si toutes familles est selectionné c OK
		if (hItemSelected == GetRootItem())
			return true;

		//si une famille est selectionnée
		INode* node = (INode*)GetItemData(hItemSelected);
		if (node)
		{
			//si l'id est dans cette famille
			if(node->IsLot() && node->AsLot()->GetFilsByIDAudience(id))
				return true;

			//sinon on regarde dans les centres
			IIterateur* it = node->CreateIterateur();
			if(it)
			{
				INode* nodefils = it->getHead();
				while (nodefils)
				{
					if(nodefils->IsLot() && nodefils->AsLot()->GetFilsByIDAudience(id))
						return true;
					nodefils = it->getNext();
				}
				delete it;
			}

		}
		else
		{
			//m_pRegroupements->GetTitresNA()->MoveTo(id);
			if (m_pRegroupements->GetTitreNAByIDAudience(id))
				return true;
		}

		hItemSelected = GetNextSelectedItem(hItemSelected);
	}
	return bRet;
}


//Permet de changer le style du tree
//m_mytreectrl.SetNewStyle(TVS_SINGLEEXPAND, TRUE/FALSE);
void CSupportTree::SetNewStyle(long lStyleMask, BOOL bSetBits)
{
	long        lStyleOld;

	lStyleOld = GetWindowLong(m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyleMask;
	if (bSetBits)
		lStyleOld |= lStyleMask;

	SetWindowLong(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void CSupportTree::SetSelectionColor(COLORREF selColor)
{
	m_selColor = selColor;
}

//menu contextuel renommer
void CSupportTree::OnCtxRenommer() 
{
	// Passe en mode édition
	CEdit* pmyEdit = EditLabel(GetSelectedItem());
	ASSERT(pmyEdit != NULL);

	/*
	// Récup nouvel item 
	hItem = GetSelectedItem();
	CString NewItemTxt = GetItemText(hItem);

	// Vérifie qu'il n'existe pas déjà
	if (DoublonItem(hItem,NewItemTxt))
	{
		// Remet le précédent label
		SetItemText(hItem,OldItemTxt);
	}
	*/
}

// Vérifie Item non déjà présent
bool CSupportTree::DoublonItem(HTREEITEM hItem, CString NewText)
{
	// Passe le label à rechercher en majuscule
	NewText.MakeUpper(); 

	// Balaye les items même niveau d'avant (en passant les libellés en majuscule)
	HTREEITEM hItemPrevious = GetPrevSiblingItem(hItem);
	CString Txt = GetItemText(hItemPrevious);
	Txt.MakeUpper();

	while (hItemPrevious != NULL)
	{
		// Récupère texte et le passe en majuscule (pour comparaison)
		CString TxtItem = GetItemText(hItemPrevious);
		TxtItem.MakeUpper();

		if (TxtItem == NewText)
			// Texte déjà existant
			return true;

		// Passe à la branche famille précédente
		hItemPrevious = GetPrevSiblingItem(hItemPrevious);
	}

	// Balaye les items même niveau d'aprés (en les passant en majuscule)
	HTREEITEM hItemNext = GetNextSiblingItem(hItem);
	Txt = GetItemText(hItemNext);
	Txt.MakeUpper();

	while (hItemNext != NULL)
	{
		// Récupère texte (en les passant en majuscule)
		CString TxtItem = GetItemText(hItemNext);
		TxtItem.MakeUpper();

		if (TxtItem == NewText)
			// Texte déjà existant
			return true;

		// Passe à la branche famille suivante
		hItemNext = GetNextSiblingItem(hItemNext);
	}

	// Pas de doublon
	return false;
}

//Gere le renommage des items (si autorisé)
void CSupportTree::OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	TV_DISPINFO     *ptvinfo;

	ptvinfo = (TV_DISPINFO *)pnmhdr;

	// Test avant de valider le nom de l'item, s'il n'existe pas déjà
	HTREEITEM hItem = GetSelectedItem();
	if (DoublonItem(hItem,ptvinfo->item.pszText))
	{
		// Récup rand de l'item
		JUnt32 NoRang = GetRangItem(hItem);
		switch (NoRang)
		{
			case Item_Famille :
			{
				// Nom existant déjà dans la liste des familles
				CString TxtMess;
				TxtMess.LoadString(IDS_SU_FAMILLE_EXISTANTE); 
				AfxMessageBox(TxtMess);
				// AfxMessageBox("Famille déjà existante");
				break;
			}
			case Item_Centre :
			{	
				// No,m existant déjà dans la liste des centres
				CString TxtMess;
				TxtMess.LoadString(IDS_SU_CENTRE_EXISTANTE); 
				AfxMessageBox(TxtMess);
				// AfxMessageBox("Centre déjà existant");
				break;
			}

			default:
				break;
		}
	}
	
	//si le texte entré existe
	else if (ptvinfo->item.pszText != NULL )
	{
		// A FAIRE
		// Ajouter Texte dans table des rubriques
		//on renomme l'INode
		((INode*)GetItemData(ptvinfo->item.hItem))->SetLabel(ptvinfo->item.pszText);

		//on renomme l'element du tree
		ptvinfo->item.mask = TVIF_TEXT;
		SetItem(&ptvinfo->item);
	}
	
	// A VOIR
	// AfxGetApp()->m_pMainWnd->PostMessage(ATPMSG_SUPPORT_TREE_NEEEDUPD,0,0);

	*pLResult = FALSE;
}


//Teste si le 1er est fils du second
BOOL CSupportTree::IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent)
{
	do
	{
		if (hitemChild == hitemSuspectedParent)
			break;
	}
	while ((hitemChild = GetParentItem(hitemChild)) != NULL);

	return (hitemChild != NULL);
}

void CSupportTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bMultiSel)
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);
		return;
	}
	else
	{
		UINT flag;
		HTREEITEM hItem = HitTest( point, &flag );
        if( hItem && flag & TVHT_ONITEM)
		{
			
			if (hItem == GetRootItem())//si l'item toutes famille deselection de tout
			{
				ClearSelection();
				CTreeCtrl::OnLButtonDown(nFlags, point);
				return;
			}
			else
				SetItemState(GetRootItem(), 0,  TVIS_SELECTED);
			
			m_bSelectionComplete = false;
			// Toggle selection state
			UINT uNewSelState = 
				GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED ?
				0 : TVIS_SELECTED;

			// Get old selected (focus) item and state
			HTREEITEM hItemOld = GetSelectedItem();
			UINT uOldSelState  = hItemOld ?
				GetItemState(hItemOld, TVIS_SELECTED) : 0;

			// Select new item
			if( GetSelectedItem() == hItem )
				SelectItem( NULL );        // to prevent edit
			CTreeCtrl::OnLButtonDown(nFlags, point);

			// Set proper selection (highlight) state for new item
			SetItemState(hItem, uNewSelState,  TVIS_SELECTED);

			// Restore state of old selected item
			if (hItemOld && hItemOld != hItem)
				SetItemState(hItemOld, uOldSelState, TVIS_SELECTED);
			
			if (GetFirstSelectedItem() == NULL)
				SetItemState(GetRootItem(), TVIS_SELECTED,  TVIS_SELECTED);

			m_bSelectionComplete = true;
			NotifySelChanged(hItemOld, hItem);

			return;
		}
		CTreeCtrl::OnLButtonDown(nFlags, point);
		return;
	}
}


//debut du dragg
void CSupportTree::OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	if (m_bDragDrop)
	{
		CPoint      ptAction;
		UINT        nFlags;

		GetCursorPos(&ptAction);
		ScreenToClient(&ptAction);

		m_hitemDrag = HitTest(ptAction, &nFlags);
		m_hitemDrop = NULL;

		//si c'est une famille ou l'item racine
		if( m_hitemDrag == GetRootItem() || GetParentItem(m_hitemDrag) == GetRootItem() )
			return;

		ASSERT(!m_bDragging);
		m_bDragging = TRUE;
		ASSERT(m_pimagelist == NULL);
		CImageList* img2;
		img2 = CreateDragImage(m_hitemDrag);
		m_pimagelist = CreateDragImage(m_hitemDrag);
		

		//int gg = img2->Add(AfxGetApp()->LoadIcon(IDI_SU_PLUS));
		//
		//m_pimagelist = new CImageList();
		//	
		//m_pimagelist->Create(*img2, 0, *img2, gg, 10, 10);

		//HCURSOR	cursor_no	= AfxGetApp()->LoadCursor(IDC_SU_POINTER_COPY);

		m_pimagelist->SetDragCursorImage(0, CPoint(0, 0));
		m_pimagelist->DragShowNolock(TRUE);
		m_pimagelist->BeginDrag(0, CPoint(0,0));
		m_pimagelist->DragMove(ptAction);
		m_pimagelist->DragEnter(this, ptAction);
		
		SetCapture();
	}
	
	*pLResult = TRUE;
}


//deplacement de la souris
void CSupportTree::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM hitem;
	UINT flags;

	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragMove(point);

		//si on passe au dessus d'un element
		if ((hitem = HitTest(point, &flags)) != NULL)
		{
			m_pimagelist->DragLeave(this);
			
			//if(IsCTRLpressed())
			//{
			//	HCURSOR	cursor_no	= AfxGetApp()->LoadCursor(IDC_SU_POINTER_COPY);
			//	::SetCursor(cursor_no);
			//}
			//else
			{
				HCURSOR	cursor_normal	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
				::SetCursor(cursor_normal);
			}
	

			//			SetInsertMark(NULL);
			SelectDropTarget(hitem);
			m_hitemDrop = hitem;
//			SetInsertMark(hitem);
			m_pimagelist->DragEnter(this, point);
		}

	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

//renvoie vers la fonction de depose de l'item
void CSupportTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnButtonUp();
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

// depose d'un item
void CSupportTree::OnButtonUp()
{
	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragLeave(this);
//		SetInsertMark(NULL);
		m_pimagelist->EndDrag();
		delete m_pimagelist;
		m_pimagelist = NULL;

		bool candropp, candropp2;

		//si dragg est un titre et drop est un lot
		candropp = ((INode*)GetItemData(m_hitemDrag))->IsTitre() && ((INode*)GetItemData(m_hitemDrop))->IsLot();
		
		//si dragg est un centre(lot) alors profondeur == 1 (famille) mais pas lot
		candropp2 = ((INode*)GetItemData(m_hitemDrag))->IsLot() &&  GetProfondeur(m_hitemDrop) == 1 &&  !((INode*)GetItemData(m_hitemDrop))->IsLot();

		//int diff = GetProfondeur(m_hitemDrag) - GetProfondeur(m_hitemDrop);

		if (m_hitemDrag != m_hitemDrop	//si le draggé est diff du droppé
			&& !IsChildNodeOf(m_hitemDrop, m_hitemDrag)	//si le droppé n'est pas fils du draggé
			&& GetParentItem(m_hitemDrag) != m_hitemDrop	//si le droppé n'est pas le pere du draggé
			&& (candropp||candropp2)//respect de l'homogénéité
			)	
		{
			TransferItem(m_hitemDrag, m_hitemDrop); // fn recursive
			DeleteItem(m_hitemDrag);
		}

		ReleaseCapture();
		m_bDragging = FALSE;
		m_hitemDrag = NULL;
		SelectDropTarget(NULL);
	}
}

//Fonction recursive
//permet de faire passer l'item draggé
BOOL CSupportTree::TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop)
{
	TV_INSERTSTRUCT     tvstruct;
	HTREEITEM           hNewItem, hFirstChild;

	//déplacement dans le tree
	TVITEM itemDrag;
	itemDrag.hItem = hitemDrag;
	itemDrag.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	BOOL bWorked = GetItem(&itemDrag);

	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvstruct.hParent =  hitemDrop;
	tvstruct.item.pszText = (char*)((INode*)itemDrag.lParam)->GetLabel().AsJCharPtr();
	tvstruct.item.lParam = itemDrag.lParam;

	tvstruct.item.iImage			= itemDrag.iImage;
	tvstruct.item.iSelectedImage	= itemDrag.iSelectedImage;


	hNewItem = InsertItem(&tvstruct);

	Expand( hitemDrop, TVE_EXPAND );

	//deplacement dans les obj
	INode * nodeDragged = NULL;
	CVisiteurRemove vRemove;
	CVisiteurInsert vInsert;
	switch ( GetProfondeur(hitemDrag) )
	{
	case 0://on ne déplace pas les familles
		break;
	case 1://deplacement d'un centre
	case 2://deplacement d'un support
		nodeDragged = (INode *)GetItemData(hitemDrag);
		nodeDragged->Accept(&vRemove);
		nodeDragged->SetParentID( ((IComposite*)GetItemData(hitemDrop))->GetID() );
		nodeDragged->Accept(&vInsert);
		break;
	}

	//partie recursive de la fonction
	while ( (hFirstChild = GetChildItem(hitemDrag)) != NULL)
	{
		TransferItem(hFirstChild, hNewItem);  
		DeleteItem(hFirstChild);        // puisqu'on efface l'item on n'a pas besoin de getnext
	}
	return TRUE;
}

//ouverture du menu contextuel
void CSupportTree::OnContextMenu(CWnd *pWnd, CPoint point)
{
	if(m_bEditable)
	{
		HTREEITEM   hitem;
		UINT        flags;
		CMenu menu;
		CPoint pt;
		pt = point;
		ScreenToClient(&pt);
		hitem = HitTest(pt, &flags);
		menu.LoadMenu(IDR_SU_MENUCTXTREE);
		INode * node;

		//si on click sur un item
		if (flags & TVHT_ONITEM)
		{
			SelectItem(hitem);
			//on n'a pas le mm menu suivant la profondeur
			switch ( GetProfondeur(hitem) )
			{
			case 0://root
				menu.GetSubMenu(3)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
				break;
			case 1://Famille
				menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
				break;
			case 2://Centre ou support
				node = (INode*)GetItemData(hitem);
				if (node->IsTitre())
					menu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
				else
					menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);		
				break;
			case 3://Support
				menu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
				break;
			}
		}
		else
		{
			SelectItem(NULL);
			menu.GetSubMenu(3)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
}

void CSupportTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	//doit être présente pour que le context menu fonctionne
}


//permet de connaitre la profondeur d'un item dans le tree
int CSupportTree::GetProfondeur(const HTREEITEM hitem) const
{
	int prof = -1;
	HTREEITEM hparent = hitem;
	do
	{
		hparent = GetParentItem(hparent);
		prof++;
	}while (hparent != NULL);

	return prof;
}

////////////////////////////////////////////////////////
//Menu contextuel ajout de famille
void CSupportTree::OnCtxFamilleAdd() 
{
	// Nom d'une nouvelle famille à créer
	CString TxtNewFamille;
	TxtNewFamille = GetNewNameFamille();

	//Creation de la nouvelle famille et d'un centre
	srand( (unsigned)time( NULL ) );
	CFamille * familleadd = new CFamille( IDFAMILLE_MIN + rand(), TxtNewFamille);

	// insertion dans le CRoot
	CVisiteurInsert vInsert;
	familleadd->Accept(&vInsert);

	// insertion dans le CTreeCtrl

	//Creation de l'element du tree
	TV_INSERTSTRUCT tvisFamille;
	tvisFamille.item.mask			= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvisFamille.item.pszText		= (char*)(LPCTSTR)familleadd->GetLabel().AsJCharPtr();
	tvisFamille.item.lParam			= (LPARAM)familleadd;
	tvisFamille.item.iImage			= ICON_F + m_appColor;
	tvisFamille.item.iSelectedImage	= ICON_F + m_appColor;
	tvisFamille.hInsertAfter	= TVI_LAST;
	tvisFamille.hParent		= GetRootItem();

	// insertion
	HTREEITEM hNewItem = InsertItem (&tvisFamille);

	// selection du nouvel element
	BOOL bret = SelectItem (hNewItem);
	//on lance l'edition du label
	EditLabel(hNewItem);
}

////////////////////////////////////////////////////////
// Fabrique nom nouvelle famille
CString CSupportTree::GetNewNameFamille()
{
	// Par défaut ce sera Nouvelle Famille
	CString TxtNewFamille;
	TxtNewFamille.LoadString(IDS_SU_NEW_FAMILLE); // = "Nouvelle Famille";
	JUnt32 LgTxtNewfamille = TxtNewFamille.GetLength(); 

	// N° en cours nouvelle famille
	int NoFamille = 0;

	// Recherche nom de famille dispo
	HTREEITEM hItem = GetRootItem();
	if (ItemHasChildren(hItem))
	{
		hItem = GetChildItem(hItem);
		while (hItem != NULL)
		{
			// Récupère texte branche famille
			CString TxtItem = GetItemText(hItem);

			// Vérifie qu'aucune ne s'appelle "Nouvelle Famille xxxx"
			CString TxtNewFamille;
			TxtNewFamille.LoadString(IDS_SU_NEW_FAMILLE); 
			int Pos = TxtItem.Find(TxtNewFamille,0);
			// int Pos = TxtItem.Find("Nouvelle Famille",0);
			if (Pos >= 0)
			{
				// Récup n° famille
				CString Txt = TxtItem.Mid(Pos+LgTxtNewfamille);
				if (atoi(TxtItem.Mid(Pos+LgTxtNewfamille)) >= NoFamille)
					NoFamille = atoi(TxtItem.Mid(Pos+LgTxtNewfamille)) + 1;
			}
			
			// Passe à la branche famille suivante
			hItem = GetNextSiblingItem(hItem);
		}

		// Constitue le nom de la nouvelle famille à créer
		if (NoFamille > 0)
		{
			CString Txt;
			Txt.LoadString(IDS_SU_NEW_FAMILLE); 
			TxtNewFamille.Format("%s %d",Txt,NoFamille);
			// TxtNewFamille.Format("%s %d","Nouvelle Famille",NoFamille);
		}
	}

	return TxtNewFamille;

}

////////////////////////////////////////////////////////
// Fonction donnant le rang de la branche
JUnt32 CSupportTree::GetRangItem(HTREEITEM hItem)
{
	// Handle item racine
	HTREEITEM hItemRoot = GetRootItem();

	// Niveau de base
	JUnt32 NoRang = 0;

	if (hItem != hItemRoot && hItem != 0)
	{
		// Remonte les ascendants
		while (hItem != hItemRoot)
		{
			// Récup le parent
			hItem = this->GetParentItem(hItem) ;
			NoRang++;
		}	
	}

	// Retourne le n° du rang
	return NoRang;
}

////////////////////////////////////////////////////////
//Menu contextuel ajout de centre
void CSupportTree::OnCtxCentreAdd() 
{
	/*
	HTREEITEM hItemSelected = GetSelectedItem();

	//on verifie si on a le droit de creer un centre
	INode* nodepere = (INode*)GetItemData(hItemSelected);
	if ( nodepere->IsLot() )
		return;

	// Nom d'une nouveau centre d'intérêt à créer
	CString TxtNewCentre;
	TxtNewCentre = GetNewNameCentre();

	//Creation du nouveau centre
	srand( (unsigned)time( NULL ) );
	// CCentre * centreadd = new CCentre(IDCENTRE_MIN + rand(), "Nouveau Centre", ((IComposite*)GetItemData(hItemSelected) )->GetID() );
	CCentre * centreadd = new CCentre(IDCENTRE_MIN + rand(), TxtNewCentre, ((IComposite*)GetItemData(hItemSelected) )->GetID() );
	CVisiteurInsert vInsert;
	centreadd->Accept(&vInsert);

	//Creation de l'element du tree
	TV_INSERTSTRUCT tvis;
	tvis.item.mask				= TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	tvis.item.pszText			= (char*)(LPCTSTR)centreadd->GetLabel().AsJCharPtr();
	tvis.item.lParam			= (LPARAM)centreadd;
	tvis.item.iImage			= ICON_C + m_appColor;
	tvis.item.iSelectedImage	= ICON_C + m_appColor;

	tvis.hInsertAfter	= TVI_LAST;
	tvis.hParent		= hItemSelected;

	// insertion dans le CTreeCtrl
	HTREEITEM hNewItem = InsertItem (&tvis);
	ASSERT (hNewItem);
	// selection du nouvel element
	SelectItem (hNewItem);
	EditLabel(hNewItem);
	*/
}

////////////////////////////////////////////////////////
// Fabrique nom nouveau centre d'intérêt
CString CSupportTree::GetNewNameCentre()
{
	// Par défaut ce sera Nouveau Centre
	CString TxtNewCentre;
	TxtNewCentre.LoadString(IDS_SU_NEW_CENTRE); //"Nouveau Centre";
	JUnt32 LgTxtNewCentre = TxtNewCentre.GetLength(); 

	// N° en cours nouveau centre
	int NoCentre = 0;

	// Recherche nom de centre dispo
	HTREEITEM hItem = GetRootItem();
	if (ItemHasChildren(hItem))
	{
		HTREEITEM hItemFamille = GetChildItem(hItem);
		while (hItemFamille != NULL)
		{
			// Récupère centre
			HTREEITEM hItemCentre = GetChildItem(hItemFamille);
			
			while (hItemCentre != NULL)
			{
				// Récupère texte branche famille
				CString TxtItem = GetItemText(hItemCentre);

				// Vérifie qu'aucune ne s'appelle "Nouveau Centre xxxx"
				CString TxtNewCentre;
				TxtNewCentre.LoadString(IDS_SU_NEW_CENTRE); 
				int Pos = TxtItem.Find(TxtNewCentre,0); // TxtItem.Find("Nouveau Centre",0);
				if (Pos >= 0)
				{
					// Récup n° centre
					CString Txt = TxtItem.Mid(Pos+LgTxtNewCentre);
					if (atoi(TxtItem.Mid(Pos+LgTxtNewCentre)) >= NoCentre)
						NoCentre = atoi(TxtItem.Mid(Pos+LgTxtNewCentre)) + 1;
				}

				// Passe à la branche centre suivante
				hItemCentre = GetNextSiblingItem(hItemCentre);
			}

			// Passe à la branche famille suivante
			hItemFamille = GetNextSiblingItem(hItemFamille);
		}

		// Constitue le nom du nouveau centre d'intérêt à créer
		if (NoCentre > 0)
		{
			CString Txt;
			Txt.LoadString(IDS_SU_NEW_CENTRE); 
			TxtNewCentre.Format("%s %d",Txt,NoCentre);
			// TxtNewCentre.Format("%s %d","Nouveau Centre",NoCentre);
		}
	}
	return TxtNewCentre;
}

void CSupportTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if (m_bMultiSel)
	//{
	//	if ( (nChar==VK_UP || nChar==VK_DOWN) && GetKeyState( VK_SHIFT )&0x8000)
	//	{
	//		// Initialize the reference item if this is the first shift selection
	//		if( !m_hItemFirstSel )
	//		{
	//			m_hItemFirstSel = GetSelectedItem();
	//			ClearSelection();
	//		}

	//		// Find which item is currently selected
	//		HTREEITEM hItemPrevSel = GetSelectedItem();

	//		HTREEITEM hItemNext;
	//		if ( nChar==VK_UP )
	//			hItemNext = GetPrevVisibleItem( hItemPrevSel );
	//		else
	//			hItemNext = GetNextVisibleItem( hItemPrevSel );

	//		if ( hItemNext )
	//		{
	//			// Determine if we need to reselect previously selected item
	//			BOOL bReselect =
	//				!( GetItemState( hItemNext, TVIS_SELECTED ) & TVIS_SELECTED );

	//			// Select the next item - this will also deselect the previous item
	//			SelectItem( hItemNext );

	//			// Reselect the previously selected item
	//			if ( bReselect )
	//				SetItemState( hItemPrevSel, TVIS_SELECTED, TVIS_SELECTED );
	//		}
	//		return;
	//	}
	//	else if( nChar >= VK_SPACE )
	//	{
	//		m_hItemFirstSel = NULL;
	//		ClearSelection();
	//	}
	//}

	if (m_bEditable)
	{
		switch (nChar)
		{
		case VK_DELETE:	
			OnCtxSuppr();
			break;
		case VK_F2:	
			OnCtxRenommer();
			break;
		}
		CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
	}
}

//Menu contextuel supression
void CSupportTree::OnCtxSuppr() 
{
	HTREEITEM hItemSelected = GetSelectedItem();
	int res;
	INode* node = NULL;

	CString ttl;
	ttl.LoadString(IDS_SU_ATP);

	if (hItemSelected)
	{
		CString txt;
		//Question 
		switch ( GetProfondeur(hItemSelected) )
		{
		case 0://root
			txt.LoadString(IDS_SU_SUPPRALLREGROUP);
			res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONSTOP );
			if ( res == IDYES)
			{
				CRegroupements::GetInstance()->GetFamilles()->Empty();
				DeleteAllItems();
				GetParent()->Invalidate();
			}
			res = IDNO;
			break;
		case 1://famille
			txt.LoadString(IDS_SU_SUPPRFAMILLE);
			res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONQUESTION);
			break;
		case 2://Centre ou support
			node = (INode*)GetItemData(hItemSelected);
			if (node->IsTitre())
				txt.LoadString(IDS_SU_SUPPRSUPPORT);
			else
				txt.LoadString(IDS_SU_SUPPRCENTRE);
			res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONQUESTION);
			break;
		case 3://Support
			txt.LoadString(IDS_SU_SUPPRSUPPORT);
			res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONQUESTION);
			break;
		default:
			res = IDNO;
		}
		if ( res == IDYES)
		{
			CVisiteurDelete vDelete;
			INode * nodetmp = (INode*)GetItemData( hItemSelected );
			nodetmp->Accept(&vDelete);

			//on le retire du tree
			DeleteItem(hItemSelected);
			GetParent()->Invalidate();

			// Pas besoin d'updater le tree famille, pour éviter qu'il se referme
			// return;
		}
		// Pas besoin d'updater le tree famille, pour éviter qu'il se referme
		return;
	}
	else
	{
		CString txt;
		txt.LoadString(IDS_SU_SUPPRALLREGROUP);
		res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONSTOP );
		if ( res == IDYES)
		{
			CRegroupements::GetInstance()->GetFamilles()->Empty();
			DeleteAllItems();
			GetParent()->Invalidate();
		}
	}

	AfxGetApp()->m_pMainWnd->PostMessage(ATPMSG_SUPPORT_TREE_NEEEDUPD,0,0);
}

BOOL CSupportTree::PreTranslateMessage(MSG* pMsg) 
{
	// When an item is being edited make sure the edit control
	// receives certain important key strokes
	if (GetEditControl())
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE; // DO NOT process further
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CSupportTree::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVCUSTOMDRAW* pTVCD = reinterpret_cast<NMTVCUSTOMDRAW*>( pNMHDR );
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	switch (pTVCD->nmcd.dwDrawStage)
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
			//si selectionné
			if ((GetItemState((HTREEITEM)pTVCD->nmcd.dwItemSpec, TVIS_SELECTED) & TVIS_SELECTED)) 
			//if ( pTVCD->nmcd.uItemState & CDIS_SELECTED )
			{
				pTVCD->clrTextBk = m_selColor;
				pTVCD->clrText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			}
			else
			{
				INode* node = (INode*)GetItemData((HTREEITEM)pTVCD->nmcd.dwItemSpec);
				if ( node && IsAttribue(node->GetID()) )
					pTVCD->clrText = m_ColorAttribue;

			}
			//si en surbrillance drag&drop
			if ((GetItemState((HTREEITEM)pTVCD->nmcd.dwItemSpec, TVIS_DROPHILITED) & TVIS_DROPHILITED)) 
				pTVCD->clrTextBk = m_selColor;
			break ;
	}
}

void CSupportTree::SetIconsColor(int appColor)
{
	m_appColor = appColor;
	HTREEITEM hItem = GetRootItem();
	hItem = GetChildItem(hItem);

	while (hItem != NULL)
	{
		if (GetItemData(hItem))
			SetItemImage(hItem, ICON_F + m_appColor, ICON_F + m_appColor);
		if (ItemHasChildren(hItem))
		{
			HTREEITEM hChildItem = GetChildItem(hItem);
			while (hChildItem != NULL)
			{
				INode* node = (INode*)GetItemData(hChildItem);
				if ( !node->IsTitre())
					SetItemImage(hChildItem, ICON_C + m_appColor, ICON_C + m_appColor);
				hChildItem = GetNextSiblingItem(hChildItem);
			}
		}

		hItem = GetNextSiblingItem(hItem);
	}
	Invalidate();
}

void CSupportTree::SetAttribueColor( COLORREF color )
{
	m_ColorAttribue = color;
}

/*
//Tool tip à bas d'info tip
BOOL CSupportTree::EnableToolTips(BOOL bEnable)
{
	SetNewStyle(TVS_INFOTIP, bEnable);
	return true;
}

void CSupportTree::OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = (LPNMTVGETINFOTIP)pNMHDR;
	pGetInfoTip->hdr.hwndFrom = m_hWnd;
	INode * node = (INode*) pGetInfoTip->lParam;
	CString strTipText;
	if(node)
	{
		IIterateur* it = node->CreateIterateur();
		if (it)
		{
			INode * nd = it->getHead();
			while(nd)
			{
				strTipText.AppendFormat("%s\r\n", nd->GetLabel().AsJCharPtr());
				nd = it->getNext();
			}
			strTipText.TrimRight();
			delete it;
		}

	}
	strcpy(pGetInfoTip->pszText, strTipText);

	*pResult = 0;
}
//*/

//*
//ToolTip normal
BOOL CSupportTree::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXT* pTTTA = (TOOLTIPTEXT*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = (UINT) pNMHDR->idFrom;
	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);

	// Do not process the message from built in tooltip 
	//if( nID == (UINT)m_hWnd &&
	//	(( pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND ) ||
	//	( pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND ) ) )
	//	return FALSE;

	HTREEITEM hitem = (HTREEITEM) pNMHDR->idFrom;
	INode * node = (INode*) GetItemData(hitem); //get item data
	
	if(node)
	{
		IIterateur* it = node->CreateIterateur();
		if (it)
		{
			INode * nd = it->getHead();
			while(nd)
			{
				strTipText.AppendFormat("%s\r\n", nd->GetLabel().AsJCharPtr());
				nd = it->getNext();
			}
			strTipText.TrimRight();
			delete it;
		}
	}
	strTipText.AppendChar('\0');

#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;
		
		m_pchTip = new TCHAR[strTipText.GetLength()];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;
		
		m_pwchTip = new WCHAR[strTipText.GetLength()];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif

	*pResult = 0;

	return TRUE;    
}

int CSupportTree::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	RECT rect;
	UINT nFlags;
	HTREEITEM hitem = HitTest( point, &nFlags );
	if( nFlags & TVHT_ONITEMLABEL  )
	{
		GetItemRect( hitem, &rect, TRUE );
		pTI->hwnd = m_hWnd;
		pTI->uId = reinterpret_cast<UINT_PTR>(hitem);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return (int)pTI->uId;
	}
	return -1;
}
//*/

void CSupportTree::ClearSelection()
{
	// This can be time consuming for very large trees
	// and is called every time the user does a normal selection
	// If performance is an issue, it may be better to maintain
	// a list of selected items
	for ( HTREEITEM hItem=GetRootItem(); hItem!=NULL; hItem=GetNextVisibleItem( hItem ) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			SetItemState( hItem, 0, TVIS_SELECTED );
	SelectItem(NULL);
}

// SelectItems    - Selects items from hItemFrom to hItemTo. Does not
//        - select child item if parent is collapsed. Removes
//        - selection from all other items
// hItemFrom    - item to start selecting from
// hItemTo    - item to end selection at.
BOOL CSupportTree::SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
{
	HTREEITEM hItem = GetRootItem();

	// Clear selection upto the first item
	while ( hItem && hItem!=hItemFrom && hItem!=hItemTo )
	{
		hItem = GetNextVisibleItem( hItem );
		SetItemState( hItem, 0, TVIS_SELECTED );
	}

	if ( !hItem )
		return FALSE;    // Item is not visible

	SelectItem( hItemTo );

	// Rearrange hItemFrom and hItemTo so that hItemFirst is at top
	if( hItem == hItemTo )
	{
		hItemTo = hItemFrom;
		hItemFrom = hItem;
	}


	// Go through remaining visible items
	BOOL bSelect = TRUE;
	while ( hItem )
	{
		// Select or remove selection depending on whether item
		// is still within the range.
		SetItemState( hItem, bSelect ? TVIS_SELECTED : 0, TVIS_SELECTED );

		// Do we need to start removing items from selection
		if( hItem == hItemTo )
			bSelect = FALSE;

		hItem = GetNextVisibleItem( hItem );
	}

	return TRUE;
}


HTREEITEM CSupportTree::GetFirstSelectedItem()
{
	for ( HTREEITEM hItem = GetRootItem(); hItem!=NULL; hItem = GetNextVisibleItem( hItem ) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

HTREEITEM CSupportTree::GetNextSelectedItem( HTREEITEM hItem )
{
	for ( hItem = GetNextVisibleItem( hItem ); hItem!=NULL; hItem = GetNextVisibleItem( hItem ) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

HTREEITEM CSupportTree::GetPrevSelectedItem( HTREEITEM hItem )
{
	for ( hItem = GetPrevVisibleItem( hItem ); hItem!=NULL; hItem = GetPrevVisibleItem( hItem ) )
		if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
			return hItem;

	return NULL;
}

BOOL CSupportTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Return TRUE if selection is not complete. This will prevent the 
	// notification from being sent to parent.
	return !m_bSelectionComplete;	
}

void CSupportTree::NotifySelChanged(HTREEITEM hItemFrom, HTREEITEM hItemTo)
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

BOOL CSupportTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if ( pNMTreeView->action == TVE_COLLAPSE )
	{
		HTREEITEM hItem = GetChildItem( pNMTreeView->itemNew.hItem );

		while ( hItem )
		{
			if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
				SetItemState( hItem, 0, TVIS_SELECTED );

			// Get the next node: First see if current node has a child
			HTREEITEM hNextItem = GetChildItem( hItem );
			if ( !hNextItem )
			{
				// No child: Get next sibling item
				if ( !( hNextItem = GetNextSiblingItem( hItem ) ) )
				{
					HTREEITEM hParentItem = hItem;
					while ( !hNextItem )
					{
						// No more children: Get parent
						if ( !( hParentItem = GetParentItem( hParentItem ) ) )
							break;

						// Quit when parent is the collapsed node
						// (Don't do anything to siblings of this)
						if ( hParentItem == pNMTreeView->itemNew.hItem )
							break;

						// Get next sibling to parent
						hNextItem = GetNextSiblingItem( hParentItem );
					}

					// Quit when parent is the collapsed node
					if ( hParentItem == pNMTreeView->itemNew.hItem )
						break;
				}
			}

			hItem = hNextItem;
		}
	}
	
	*pResult = 0;
	return FALSE;	// Allow parent to handle this notification as well
}