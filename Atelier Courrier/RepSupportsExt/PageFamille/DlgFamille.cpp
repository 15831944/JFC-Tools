// DlgFamille.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "..\Resource.h"
#include "DlgFamille.h"
#include ".\dlgfamille.h"
#include "SearchAlgo.h"

// Boîte de dialogue CDlgFamille

IMPLEMENT_DYNAMIC(CDlgFamille, CDialog)

CDlgFamille::CDlgFamille(CWnd* pParent /*=NULL*/) : m_bCheckNA(FALSE)
{
	m_pDragImage = NULL;
	m_bDragging = FALSE;
	m_pRegroupements = CRegroupements::GetInstance();
	m_srcpresse = (JSRCPresse*)& JSRCPresse::GetInstance();
	m_treeFamille.m_pDlgParent = this;
}

CDlgFamille::~CDlgFamille()
{
	if (m_pDragImage)
		delete m_pDragImage;
}

void CDlgFamille::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RS_LISTTITRE, m_lstTitre);
	DDX_Control(pDX, IDC_RS_LISTPER, m_lstPeriodicite);
	DDX_Control(pDX, IDC_RS_TREEFAM, m_treeFamille);
	DDX_Check(pDX, IDC_RS_CHECKNA, m_bCheckNA);
	DDX_Text(pDX, IDC_RS_EDITSEARCH, m_searchStr);
}


BOOL CDlgFamille::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_treeFamille.SetData(m_pRegroupements);
	m_treeFamille.EnableEdit(true);
	m_treeFamille.EnableDragDrop(true);
	m_treeFamille.EnableFamilleNA(false);
	m_treeFamille.ShowSupport(true);
	m_treeFamille.EnableExpandAll(false);
	m_treeFamille.EnableToolTips(false);

	for (m_srcpresse->m_TBLPeriodicites.MoveFirst();
		m_srcpresse->m_TBLPeriodicites.IsValid();
		m_srcpresse->m_TBLPeriodicites.MoveNext() )
		m_lstPeriodicite.AddItem ( (JPeriodicite*)m_srcpresse->m_TBLPeriodicites.GetItem() );


	SetupControls();
	SetWindowRelations();

	return 	TRUE;
}



BEGIN_MESSAGE_MAP(CDlgFamille, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RS_CHECKNA, OnBnClickedCheckNA)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_RS_LISTTITRE, OnBeginDragListTitre)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_RS_LISTPER, OnLbnSelchangeListper)
	ON_BN_CLICKED(IDC_RS_BTNSELPER, OnBnClickedBtnselper)
	ON_BN_CLICKED(IDC_RS_BTNSEARCH, OnBnClickedBtnEffacer)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_RS_EDITSEARCH, OnEnChangeSearch)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgFamille

void CDlgFamille::OnBnClickedCheckNA()
{
	FillListTitres();	
}

void CDlgFamille::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	if (m_lstTitre.GetSafeHwnd())
		m_lstTitre.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	//Invalidate();
	UpdateWindow();
}

void CDlgFamille::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{//des titres HS ont pu etre ajoutés ou retirés
		FillListTitres();
		m_treeFamille.FillTree();
	}
}

void CDlgFamille::OnDestroy()
{
    KillTimer(IdTimer_Expand);
    KillTimer(IdTimer_Search);
	CDialog::OnDestroy();
}

void CDlgFamille::FillListTitres()
{
	//On vide la liste
	m_lstTitre.DeleteAllItems();
	m_lstTitre.DeleteColumn(0);

	//On recupere l'etat du checkbox
	UpdateData();

	//Non attribués seulement
	if (m_bCheckNA)
	{
		for (m_pRegroupements->GetTitresNA()->MoveFirst();
			m_pRegroupements->GetTitresNA()->IsValid();
			m_pRegroupements->GetTitresNA()->MoveNext() )
		{
			INode* titre = m_pRegroupements->GetTitresNA()->GetItem();
			if ( TestPeriodicite( titre->GetID() ) && CSearchAlgo::Contains(titre->GetLabel().AsJCharPtr(), m_searchStr) &&  !titre->AsITitre()->IsAgregat())
				m_lstTitre.InsertItem ( LVIF_TEXT | LVIF_PARAM, 0, titre->GetLabel().AsJCharPtr(), 0, 0, 0, (LPARAM)titre->GetID() );
		}

		//hors serie
		for ( m_pRegroupements->GetTitresApparentes()->MoveFirst();
			m_pRegroupements->GetTitresApparentes()->IsValid();
			m_pRegroupements->GetTitresApparentes()->MoveNext()
			)
		{
			CTitreApparente* tatmp = (CTitreApparente*)m_pRegroupements->GetTitresApparentes()->GetItem();
			if ( tatmp->GetParentID() == 0  && CSearchAlgo::Contains(tatmp->GetLabel().AsJCharPtr(), m_searchStr) && TestPeriodicite(tatmp->GetID()))
				m_lstTitre.InsertItem ( LVIF_TEXT | LVIF_PARAM,
					0, tatmp->GetLabel().AsJCharPtr(), 0, 0, 0, (LPARAM)tatmp->GetID() );
		}

	}
	else
	{
		//tous les titres + apparentés
		//les titres
		for ( m_srcpresse->m_TBLTitres.MoveFirst();
			m_srcpresse->m_TBLTitres.IsValid();
			m_srcpresse->m_TBLTitres.MoveNext() )
		{
			IElem* titre = m_srcpresse->m_TBLTitres.GetItem();
			if ( TestPeriodicite( titre->GetID() ) && CSearchAlgo::Contains(titre->GetLabel().AsJCharPtr(), m_searchStr) &&  !titre->AsITitre()->IsAgregat())
				m_lstTitre.InsertItem ( LVIF_TEXT | LVIF_PARAM,
					0, titre->GetLabel().AsJCharPtr(), 0, 0, 0, (LPARAM)titre->GetID() );
		}
		//les apparentés
		JMap<JInt32, JInt32> mapIDdone;
		for (m_pRegroupements->GetTitresApparentes()->MoveFirst();
			m_pRegroupements->GetTitresApparentes()->IsValid();
			m_pRegroupements->GetTitresApparentes()->MoveNext() )
		{
			CTitreApparente* titreapp =(CTitreApparente*)m_pRegroupements->GetTitresApparentes()->GetItem();
			mapIDdone.MoveTo( titreapp->GetID() );
			if ( !mapIDdone.IsValid() && CSearchAlgo::Contains(titreapp->GetLabel().AsJCharPtr(), m_searchStr) && TestPeriodicite(titreapp->GetID()))
			{
				m_lstTitre.InsertItem ( LVIF_TEXT | LVIF_PARAM,
						0, titreapp->GetLabel().AsJCharPtr(), 0, 0, 0, (LPARAM)titreapp->GetID() );
				mapIDdone.Add(titreapp->GetID());
			}
		}
	}
	m_lstTitre.InsertColumn(0, "");
	m_lstTitre.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_lstTitre.Sort();
}

bool CDlgFamille::TestPeriodicite(int ID)
{
	//si rien n'est sélectionné on considere que tt est selectionné
	if (m_lstPeriodicite.GetSelCount() == 0  || m_lstPeriodicite.GetSelCount() == m_lstPeriodicite.GetCount() )
		return true;

	//pour les apparentés
	if (IDTITREAPPARENTE_MIN<ID && ID<IDTITREAPPARENTE_MAX && m_lstPeriodicite.GetSel(m_lstPeriodicite.FindItemIndex(8)) )
		return true;

	bool bRet = false;

	//sinon on filtre selon le sélection
	JList<JInt32> lst;
	m_lstPeriodicite.GetSelectedID(lst);
	for (lst.MoveFirst(); lst.IsValid(); lst.MoveNext())
	{
		//si la selection == periodicité
		if (m_srcpresse->m_TBLTitres.GetPeriodByID(ID) == lst.GetItem() )
			bRet = true;
	}

	return bRet;
}

void CDlgFamille::OnBnClickedBtnEffacer()
{
	m_searchStr = "";
	UpdateData(FALSE);
	FillListTitres();
	GetDlgItem(IDC_RS_EDITSEARCH)->SetFocus();
}

////////////////////////////////////////////////////////////////////////////////

void CDlgFamille::OnBeginDragListTitre(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nDragIndex = ((NM_LISTVIEW*)pNMHDR)->iItem;

	//on donne le focus à la liste sinon pb qd un item du tree est en edition
	m_lstTitre.SetFocus();

	//On prend la coordonée du point
	CPoint ptAction = ((NM_LISTVIEW*)pNMHDR)->ptAction;
//	GetCursorPos(&ptAction);
//	ScreenToClient(&ptAction);

	//on delete l'imagelist si elle a déjà été créée
	if (m_pDragImage)
		delete m_pDragImage;
	
	// get the image list for dragging
	m_lstTitre.SetItemState(m_nDragIndex,0,LVIF_STATE);
	//m_pDragImage = m_lstTitre.CreateDragImage(m_nDragIndex, &ptAction);  
	
	if(m_lstTitre.GetSelectedCount() > 1) //more than 1 item in list is selected
	{
		m_pDragImage = new CImageList();
		m_pDragImage->Create(IDB_RS_MULTISEL,89,0,RGB(255,255,255));
	}
	else
		m_pDragImage = m_lstTitre.CreateDragImage(m_nDragIndex, &ptAction);  
	
	m_pDragImage->BeginDrag(0, CPoint(0,5));
	m_pDragImage->DragEnter(GetDesktopWindow(), ptAction);
	
	//la fenetre actuelle dans laquelle on a le curseur
	m_pDropWnd = this;
	//l'item sur lequel on drop
	m_hDropItem = NULL;
	
	//On dragge
	m_bDragging = TRUE;

	//comme d'hab pour OnMouseMove
	SetCapture();

	*pResult = TRUE;
}

void CDlgFamille::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		//Convertion des coordonnées du point actuel
		CPoint pt (point);
		ClientToScreen (&pt);

		// on déplace l'item draggé
		m_pDragImage->DragMove (pt);
		m_pDragImage->DragShowNolock (FALSE);

		//On recupere le CWnd* de la fenetre sous le pointeur
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd);
		// la nouvelle fenetre au dessus de laquelle le pointeur se trouve
		pDropWnd->ScreenToClient (&pt);

		// si on est en dehors de la liste et si on a qqch à dropper
		if (pDropWnd != m_pDropWnd && m_hDropItem)
		{
			// désactivation de l'élément sélectionné dans le TreeCtrl s'il existe
			((CTreeCtrl*)m_pDropWnd)->SelectDropTarget (NULL);
			m_hDropItem = NULL;
		}
		m_pDropWnd = pDropWnd;
		
		// on teste si la fenetre est un TreeCtrl
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CSupportTree)))
		{			
			//On recupere et selectionne l'item et on l'ouvre
			m_hDropItem = ((CSupportTree*)pDropWnd)->HitTest (pt);
	        //L'expand a lieu au bout de 250 ms
			SetTimer(IdTimer_Expand, 250, NULL);
			if (m_hDropItem)
			{
				INode* nodepere = (INode*)m_treeFamille.GetItemData(m_hDropItem);
				//on select le drop target
				if (  nodepere
					&& (nodepere->IsLot() || nodepere->GetNbFils() == 0 )
					)//&& !nodepere->IsTitre() )
					((CSupportTree*)pDropWnd)->SelectDropTarget (m_hDropItem);
			}
		}

		// lock window updates
		m_pDragImage->DragShowNolock (TRUE);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgFamille::OnTimer(UINT nIDEvent)
{
    switch (nIDEvent)
	{
	case IdTimer_Expand:
		{
			KillTimer(IdTimer_Expand);
			CPoint pt(GetMessagePos());
			CWnd* pDropWnd = WindowFromPoint (pt);
			((CTreeCtrl*)pDropWnd)->Expand( m_hDropItem, TVE_EXPAND );
			pDropWnd->Invalidate();
		}
		break;
	
	case IdTimer_Search:
		KillTimer(IdTimer_Search);
		FillListTitres();
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgFamille::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		// on libere la souris et le dragg
		ReleaseCapture ();
		m_bDragging = FALSE;
		m_pDragImage->DragLeave ( GetDesktopWindow() );
		m_pDragImage->EndDrag ();

		CPoint pt (point);
		ClientToScreen (&pt);
		//On recupere le CWnd* de la fenetre sous le pointeur
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd);

		// on teste si la fenetre est le MyTreeCtrl dans ce cas on insere
		if ( pDropWnd->IsKindOf (RUNTIME_CLASS (CSupportTree)) && m_hDropItem)
		{
			m_treeFamille.SelectDropTarget (NULL);

			INode* nodepere = (INode*)m_treeFamille.GetItemData(m_hDropItem);
			int idnodetoinsert;// = (int)m_lstTitre.GetItemData(m_nDragIndex);
			//si le node est un titre on prend le lot le contenant
			if (nodepere && nodepere->IsTitre())
			{
				m_hDropItem = m_treeFamille.GetParentItem(m_hDropItem);
				nodepere = nodepere->GetParentNode();
			}
			//verifier que node pere est un lot et qu'il ne contient pas l'elem
			if (  nodepere
				&& (nodepere->IsLot() || nodepere->GetNbFils() == 0 )
				&& !nodepere->IsTitre() )
			//	&& nodepere->AsLot()->GetFilsByID(idnodetoinsert) == NULL )
			{
				
				POSITION pos = m_lstTitre.GetFirstSelectedItemPosition(); //iterator for the CListCtrl
				while(pos)
				{
					m_nDragIndex = m_lstTitre.GetNextSelectedItem(pos);
					idnodetoinsert = (int)m_lstTitre.GetItemData(m_nDragIndex);
					//si le titre n'est pas dans le lot
					if (nodepere->AsLot()->GetFilsByID(idnodetoinsert) == NULL)
					{
						//insert dans la liste
						INode * titre;
						if (idnodetoinsert > IDTITREAPPARENTE_MIN 
							&& idnodetoinsert < IDTITREAPPARENTE_MAX)
						{
							CTitreApparente* tatmp = m_pRegroupements->GetTitreApparenteByID(idnodetoinsert);
							titre = new CTitreApparente(idnodetoinsert, tatmp->GetLabel().AsJCharPtr(), tatmp->GetSupportTuteur(), tatmp->GetMediaWeight() , nodepere->GetID() );

						}
						else
							titre = new CTitre(idnodetoinsert, m_srcpresse->m_TBLTitres.GetLabelByID(idnodetoinsert), nodepere->GetID() );

						CVisiteurInsert vInsert;
						titre->Accept(&vInsert);
			
						//insert dans le tree
						TV_INSERTSTRUCT     curTreeItem;
						curTreeItem.hInsertAfter = TVI_LAST;
						curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
						curTreeItem.hParent =  m_hDropItem;
						curTreeItem.item.iImage = ICON_TITRE;
						curTreeItem.item.iSelectedImage = ICON_TITRE;
						curTreeItem.item.pszText = (char*)titre->GetLabel().AsJCharPtr();
						curTreeItem.item.lParam = (LPARAM)titre;
						HTREEITEM hinsert = m_treeFamille.InsertItem(&curTreeItem);
						m_treeFamille.SelectItem(hinsert);
					}
				}
				//on met à jour les titres
				m_lstTitre.Invalidate();
			}
		}

	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgFamille::OnLbnSelchangeListper()
{
	FillListTitres();
	m_treeFamille.Invalidate();
}

void CDlgFamille::OnBnClickedBtnselper()
{
	m_lstPeriodicite.ToggleSelection();
	OnLbnSelchangeListper();
}

void CDlgFamille::OnEnChangeSearch()
{
	SetTimer(IdTimer_Search, 750, NULL);
}

void CDlgFamille::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
			{IDC_RS_STATICTITRE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

			{IDC_RS_STATICTREE,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}}, //Bottom side
	

			{IDC_RS_STATICTTLFAM,	{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eTop}}, //Bottom side
	
			{IDC_RS_TREEFAM,		{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTREE, CResizer::eBottom}}, //Bottom side
					
			{IDC_RS_STATICPER,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
								{CResizer::eProportional, IDC_RS_STATICTITRE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}}, //Bottom side

			{IDC_RS_STATICTTLTITRE,{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop}}, //Bottom side

			{IDC_RS_LISTTITRE,		{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eBottom}}, //Bottom side
	
			{IDC_RS_CHECKNA,		{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop}}, //Bottom side
			
			{IDC_RS_EDITSEARCH,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom}}, //Bottom side

			{IDC_RS_BTNSEARCH,		{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom}}, //Bottom side
			
			{IDC_RS_LISTPER,		{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eBottom}}, //Bottom side			{IDC_RS_LISTPER,		{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eLeft}, //Left side
			
			{IDC_RS_STATICTTLPER,	{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop}}, //Bottom side

			{IDC_RS_BTNSELPER,		{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop},  //Top side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop}}, //Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

void CDlgFamille::SetupControls()
{
	m_staticFamille.SubclassDlgItem(IDC_RS_STATICTREE, this);
	m_staticPeriod.SubclassDlgItem(IDC_RS_STATICPER, this);
	m_staticTitre.SubclassDlgItem(IDC_RS_STATICTITRE, this);
	m_btnSelPeriodicite.SubclassDlgItem(IDC_RS_BTNSELPER, this);
	m_checkNA.SubclassDlgItem(IDC_RS_CHECKNA, this);
	m_lblTitre.SubclassDlgItem(IDC_RS_STATICTTLTITRE, this);
	m_lblPeriod.SubclassDlgItem(IDC_RS_STATICTTLPER, this);
	m_lblFamille.SubclassDlgItem(IDC_RS_STATICTTLFAM, this);
	m_btnEffacer.SubclassDlgItem(IDC_RS_BTNSEARCH, this);


	m_staticFamille.SetTransparent(false);
	m_staticFamille.SetSunken(true);
	m_staticFamille.AddTopWindow(m_lblFamille);
	m_staticFamille.AddTopWindow(m_treeFamille);

	m_staticPeriod.SetTransparent(false);
	m_staticPeriod.SetSunken(true);
	m_staticPeriod.AddTopWindow(m_lblPeriod);
	m_staticPeriod.AddTopWindow(m_lstPeriodicite);
	m_staticPeriod.AddTopWindow(m_btnSelPeriodicite);

	m_staticTitre.SetTransparent(false);
	m_staticTitre.SetSunken(true);
	m_staticTitre.AddTopWindow(m_lblTitre);
	m_staticTitre.AddTopWindow(m_lstTitre);
	m_staticTitre.AddTopWindow(m_checkNA);
	m_staticTitre.AddTopWindow(m_btnEffacer);
	m_staticTitre.AddTopWindow(GetDlgItem(IDC_RS_EDITSEARCH)->GetSafeHwnd());

	//m_checkNA.SetCheck(1, 1);

	m_lblTitre.SetFontName(CATPColors::GetFontLabelName());
	m_lblTitre.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTitre.SetBorder(false);

	m_lblPeriod.SetFontName(CATPColors::GetFontLabelName());
	m_lblPeriod.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblPeriod.SetBorder(false);

	m_lblFamille.SetFontName(CATPColors::GetFontLabelName());
	m_lblFamille.SetFontSize(10);
	m_lblFamille.SetBorder(false);

	SetColors();
}

void CDlgFamille::SetColors()
{
	m_lstTitre.SetSelectColor( CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTitre.SetAttribueColor( CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstPeriodicite.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_treeFamille.SetSelectionColor( CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_treeFamille.SetIconsColor( CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) );
	m_treeFamille.SetAttribueColor( CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );

	m_staticFamille.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticPeriod.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticTitre.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);

	m_btnSelPeriodicite.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelPeriodicite.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelPeriodicite.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_checkNA.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_checkNA.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_checkNA.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnEffacer.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffacer.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffacer.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_lblTitre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTitre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTitre.SetFontBold(true);
	
	m_lblPeriod.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblPeriod.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblPeriod.SetFontBold(true);
	
	m_lblFamille.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblFamille.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS)); 
	m_lblFamille.SetFontBold(true);
}

void CDlgFamille::RefillTree()
{
	m_treeFamille.FillTree();
}



