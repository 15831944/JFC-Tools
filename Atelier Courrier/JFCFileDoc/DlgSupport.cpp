// DlgSupport.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgSupport.h"
#include "ATPColors.h"
#include "SearchAlgo.h"

// Boîte de dialogue CDlgSupport

IMPLEMENT_DYNAMIC(CDlgSupport, ETSLayoutDialog)
CDlgSupport::CDlgSupport(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CDlgSupport::IDD, pParent)
{
	m_pRegroupements = CRegroupements::GetInstance();
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
	m_lstHaut.m_pDlgParent = this;
	m_lstBas.m_pDlgParent = this;
	m_treeSupport.m_pDlgParent = this;
	m_pData = NULL;
	m_bGripper = false;
}

CDlgSupport::~CDlgSupport()
{
//	KillTimer(IdTimer_Search);
}

void CDlgSupport::SetData (CDialogChoixOffre* data)
{
	m_pData = data;
}

bool CDlgSupport::IsComplete()
{
	if(m_pData->m_lstIDLot.GetCount() !=0
		|| m_pData->m_lstIDTitre.GetCount() !=0
		|| m_pData->m_lstIDCouplage.GetCount() != 0)
		return true;

	return false;
}

void CDlgSupport::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FD_EDITSEARCH, m_searchStr);
	DDX_Control(pDX, IDC_FD_TREE_BAS, m_treeSupport);
}

BOOL CDlgSupport::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_treeSupport.SetData(m_pRegroupements);
	m_treeSupport.EnableEdit(false);
	m_treeSupport.EnableDragDrop(false);
	m_treeSupport.EnableFamilleNA(false);
	m_treeSupport.ShowSupport(false);
	m_treeSupport.EnableExpandAll(false);

	// A VOIR
	m_treeSupport.EnableMultipleSel(true);

	// Permettra d'afficher les composants des familles dans la liste du bas
	m_treeSupport.EnableToolTips(true);

	// Permettra d'afficher les composants de chaque item de la liste en haut des sélectionnés
	m_lstHaut.EnableToolTips(true);

	// ALAIN - Ajout 20/12/2005 (pour afficher tooltip des couplages = composants couplages )
	m_lstBas.EnableToolTips(true);

	SetupControls();
	
	m_btnAfficheAll.SetCheck(BST_CHECKED);
	Init();
	FillPeriod();

	//l'init n'a rien modifié aux données
	m_pData->m_bModif = false;

	// Indique en haut de la fenêtre la source et terrain courant
	/*
	*/

	return 	TRUE;
}

void CDlgSupport::Init()
{
//	OnBnClickedBtnLots();
	OnBnClickedBtnTitres();
}

void CDlgSupport::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ETSLayoutDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		switch(m_internalMode)
		{
		case LOT:
			OnBnClickedBtnLots();
			break;
		case TITRE:
			OnBnClickedBtnTitres();
			break;
		case COUPLAGE:
			OnBnClickedBtnCouplages();
			break;
		}
	}
}

// Gestionnaires de messages CDlgSupport
BEGIN_MESSAGE_MAP(CDlgSupport, ETSLayoutDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_FD_BTNR1_LOTS, OnBnClickedBtnLots)
	ON_BN_CLICKED(IDC_FD_BTNR2_TITRES, OnBnClickedBtnTitres)
	ON_BN_CLICKED(IDC_FD_BTNR3_COUPLAGES, OnBnClickedBtnCouplages)
	ON_LBN_SELCHANGE(IDC_FD_LIST_BAS, OnLbnSelchangeListBas)
	ON_LBN_SELCHANGE(IDC_FD_LIST_HAUT, OnLbnSelchangeListHaut)
	ON_LBN_SELCHANGE(IDC_FD_LIST_PERIOD, OnLbnSelchangeListPeriod)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_FD_EDITSEARCH, OnEnChangeSearch)
	ON_BN_CLICKED(IDC_FD_BTN_LISTALL, OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_FD_BTN_RAZSEARCH, OnBnClickedRazSearch)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FD_TREE_BAS, OnTvnSelChangedTreeBas)
	ON_BN_CLICKED(IDC_FD_BTNT, OnBtnT)
	ON_BN_CLICKED(IDC_FD_BTNT2, OnBtnT2)
END_MESSAGE_MAP()



void CDlgSupport::OnBnClickedBtnLots()
{
	CString ttl;
	ttl.LoadString(IDS_FD_STR_LOT);
	m_ttlChoix.SetText(ttl);
	CheckRadioButton(IDC_FD_BTNR1_LOTS, IDC_FD_BTNR3_COUPLAGES, IDC_FD_BTNR1_LOTS);
	m_internalMode = LOT;
	m_searchStr = "";
	UpdateData(FALSE);

	m_btnAfficheAll.GetCheck();
	m_lstHaut.ShowWindow(SW_SHOW);
	m_lstBas.ShowWindow(SW_HIDE);
	m_btnRAZSearch.ShowWindow(SW_HIDE);
	m_btnAfficheAll.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FD_EDITSEARCH)->ShowWindow(SW_HIDE);
	m_treeSupport.ShowWindow(SW_SHOW);
	m_btnT.ShowWindow(SW_HIDE);
	m_btnT2.ShowWindow(SW_HIDE);
	m_ttlPeriod.ShowWindow(SW_HIDE);
	m_lstPeriod.ShowWindow(SW_HIDE);

	m_treeSupport.FillTree();
	FillResultListLot();

	ResetSize();
	UpdateLayout(
		//paneCtrl(IDC_FD_STATIC_TITRE, VERTICAL, GREEDY, 3, 5)
		pane(VERTICAL)
					<<item(IDC_FD_STATIC_TTLTITRE, ABSOLUTE_VERT)
					<<item(IDC_FD_LIST_HAUT)
					<<item(IDC_FD_TREE_BAS, RELATIVE_VERT,0, 75)
					<<(pane(HORIZONTAL)	<<item(IDC_FD_BTNR2_TITRES, ABSOLUTE_VERT)
										<<item(IDC_FD_BTNR1_LOTS , ABSOLUTE_VERT)
										<<item(IDC_FD_BTNR3_COUPLAGES, ABSOLUTE_VERT)
						)
		);

}

void CDlgSupport::OnBnClickedBtnTitres()
{
	CString ttl;
	ttl.LoadString(IDS_FD_STR_TITRE);
	m_ttlChoix.SetText(ttl);
	CheckRadioButton(IDC_FD_BTNR1_LOTS, IDC_FD_BTNR3_COUPLAGES, IDC_FD_BTNR2_TITRES);
	m_internalMode = TITRE;

	m_btnAfficheAll.GetCheck();
	m_lstHaut.ShowWindow(SW_HIDE);
	m_lstBas.ShowWindow(SW_SHOW);
	m_btnRAZSearch.ShowWindow(SW_SHOW);
	m_btnAfficheAll.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FD_EDITSEARCH)->ShowWindow(SW_SHOW);
	m_treeSupport.ShowWindow(SW_HIDE);

	m_btnT.ShowWindow(SW_SHOW);
	m_btnT2.ShowWindow(SW_SHOW);
	m_ttlPeriod.ShowWindow(SW_SHOW);
	m_lstPeriod.ShowWindow(SW_SHOW);

	m_searchStr = "";
	UpdateData(FALSE);
	FillListTitre();
//	FillPeriod();

	ResetSize();
	UpdateLayout(
		//paneCtrl(IDC_FD_STATIC_TITRE, VERTICAL, GREEDY, 3, 5)
		pane(VERTICAL)
						<<(pane(HORIZONTAL)	<<item(IDC_FD_STATIC_TTLTITRE, ABSOLUTE_VERT)
											<<item(IDC_FD_BTNT, NORESIZE | ALIGN_RIGHT)
							)
						<<item(IDC_FD_LIST_BAS, GREEDY)
						<<(pane(HORIZONTAL)	<<item(IDC_FD_EDITSEARCH, ABSOLUTE_VERT)
											<<item(IDC_FD_BTN_RAZSEARCH, ABSOLUTE_VERT)
											<<item(IDC_FD_BTN_LISTALL, ABSOLUTE_VERT)
							)
						<<(pane(HORIZONTAL)	<<item(IDC_FD_STATIC_TTLPERIOD, ABSOLUTE_VERT)
											<<item(IDC_FD_BTNT2, NORESIZE)
							)
						<<item(IDC_FD_LIST_PERIOD, RELATIVE_VERT,0, 25)
						<<(pane(HORIZONTAL)	
											<<item(IDC_FD_BTNR2_TITRES, ABSOLUTE_VERT)
											<<item(IDC_FD_BTNR1_LOTS, ABSOLUTE_VERT)
											<<item(IDC_FD_BTNR3_COUPLAGES, ABSOLUTE_VERT)
							)
		);

}

void CDlgSupport::OnBnClickedBtnCouplages()
{
	CString ttl;
	ttl.LoadString(IDS_FD_STR_CPL);
	m_ttlChoix.SetText(ttl);
	CheckRadioButton(IDC_FD_BTNR1_LOTS, IDC_FD_BTNR3_COUPLAGES, IDC_FD_BTNR3_COUPLAGES);
	m_internalMode = COUPLAGE;

	m_lstHaut.ShowWindow(SW_SHOW);
	m_lstBas.ShowWindow(SW_SHOW);
	m_btnRAZSearch.ShowWindow(SW_SHOW);
	m_btnAfficheAll.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FD_EDITSEARCH)->ShowWindow(SW_SHOW);
	m_treeSupport.ShowWindow(SW_HIDE);
	m_btnT.ShowWindow(SW_HIDE);
	m_btnT2.ShowWindow(SW_HIDE);
	m_ttlPeriod.ShowWindow(SW_HIDE);
	m_lstPeriod.ShowWindow(SW_HIDE);

	m_searchStr = "";
	UpdateData(FALSE);

	FillListCouplage();
	FillResultListCouplage();

	ResetSize();
	UpdateLayout(
		//paneCtrl(IDC_FD_STATIC_TITRE, VERTICAL, GREEDY, 3, 5)
		pane(VERTICAL)
						<<item(IDC_FD_STATIC_TTLTITRE, ABSOLUTE_VERT)
						<<item(IDC_FD_LIST_HAUT, GREEDY)
						<<item(IDC_FD_LIST_BAS, RELATIVE_VERT,0, 75)
						<<(pane(HORIZONTAL)	<<item(IDC_FD_EDITSEARCH, ABSOLUTE_VERT)
											<<item(IDC_FD_BTN_RAZSEARCH, ABSOLUTE_VERT)
											<<item(IDC_FD_BTN_LISTALL, ABSOLUTE_VERT)
							)
						<<(pane(HORIZONTAL)	
											<<item(IDC_FD_BTNR2_TITRES, ABSOLUTE_VERT)
											<<item(IDC_FD_BTNR1_LOTS, ABSOLUTE_VERT)
											<<item(IDC_FD_BTNR3_COUPLAGES, ABSOLUTE_VERT)
							)
		);

}

void CDlgSupport::ResetSize()
{

	CRect rect;
	m_ttlChoix.GetWindowRect(rect);

	m_ttlChoix.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_ttlPeriod.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_lstHaut.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	m_lstBas.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	m_lstPeriod.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnLot.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnTitres.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnCouplages.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnRAZSearch.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnAfficheAll.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_btnT.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER|SWP_NOSIZE);
	m_btnT2.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER|SWP_NOSIZE);
	m_treeSupport.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	GetDlgItem(IDC_FD_EDITSEARCH)->SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
}


void CDlgSupport::OnBnClickedBtnAll()
{
	switch(m_internalMode)
	{
	//case LOT:
	//	FillListLot();
	//	break;
	case TITRE:
		FillListTitre();
		break;
	case COUPLAGE:
		FillListCouplage();
		break;
	}
}

//void CDlgSupport::FillListLot()
//{
//	m_lstBas.ResetContent();
//	for( m_pRegroupements->GetLots()->MoveFirst(); m_pRegroupements->GetLots()->IsValid(); m_pRegroupements->GetLots()->MoveNext() )
//	{
//		IElem* elemToInsert = m_pRegroupements->GetLots()->GetItem();
//		if ( CSearchAlgo::Contains( elemToInsert->GetLabel().AsJCharPtr() ) )
//			if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
//				&& m_lstBas.IsLotAttribue(elemToInsert->GetID()) )
//					continue;
//			else
//				m_lstBas.AddItem( elemToInsert );
//	}
//}

void CDlgSupport::FillListTitre()
{
	// on efface toute la liste
	m_lstBas.ResetContent();

	// on passe tous les titres
	for( m_srcPresse->m_TBLTitres.MoveFirst(); m_srcPresse->m_TBLTitres.IsValid(); m_srcPresse->m_TBLTitres.MoveNext() )
	{
		IElem* elemToInsert = m_srcPresse->m_TBLTitres.GetItem();

		// on ne veut pas les couplages
		if ( elemToInsert->IsCouplage())
			continue;

		if (TestPeriodicite( elemToInsert->AsITitre() ) &&  CSearchAlgo::Contains( elemToInsert->GetLabel().AsJCharPtr() , m_searchStr) )
		{
			if ( m_btnAfficheAll.GetCheck() == BST_CHECKED && m_lstBas.IsTitreAttribue(elemToInsert->GetID()))
				continue;
				
			if (m_pData->m_appColor == CATPColors::COLORCREUSET && elemToInsert->AsITitre()->IsAgregat())
				continue;
			m_lstBas.AddItem( elemToInsert );
		}
	}

	//Les titres apparentés (HS)
	for( m_pRegroupements->GetTitresApparentes()->MoveFirst(); m_pRegroupements->GetTitresApparentes()->IsValid(); m_pRegroupements->GetTitresApparentes()->MoveNext() )
	{
		IElem* elemToInsert = m_pRegroupements->GetTitresApparentes()->GetItem();
		if (TestPeriodicite( elemToInsert->AsITitre() ) && CSearchAlgo::Contains( elemToInsert->GetLabel().AsJCharPtr() , m_searchStr) )
		{
			if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
				&& m_lstBas.IsTitreAttribue(elemToInsert->GetID()) )
				continue;
			m_lstBas.AddItem( elemToInsert );
		}
	}

	//Les titres appariés
	//pour chaque terrain
	for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst();
		m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid();
		m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext() )
	{
		JUnt32 idSource = m_pData->m_offre.m_IdSourceAudience;
		JUnt32 idTerrain = m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
		CTitresApparies * titresapp = m_pRegroupements->GetTitresApparies(idSource, idTerrain);
		if (titresapp)
			for( titresapp->MoveFirst(); titresapp->IsValid(); titresapp->MoveNext() )
			{
				IElem* elemToInsert = titresapp->GetItem();
				if ( TestPeriodicite( elemToInsert->AsITitre() ) && CSearchAlgo::Contains( elemToInsert->GetLabel().AsJCharPtr(), m_searchStr ) )
				{
					if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
						&& m_lstBas.IsTitreAttribue(elemToInsert->GetID()) )
							continue;
					m_lstBas.AddItem( elemToInsert );
				}
			}
	}

	JList<JInt32> selIdlst;
	m_pData->m_lstIDTitre.Swap((JList<JUnt32>&)selIdlst);
	
	m_lstBas.SetSelectedID(selIdlst);

	m_pData->m_lstIDTitre.Swap((JList<JUnt32>&)selIdlst);

}


void CDlgSupport::FillListCouplage()
{
	m_lstBas.ResetContent();
	//les couplages libres
	JNodeList * lst = m_pRegroupements->GetCouplagesLibres();
	for( lst->MoveFirst(); lst->IsValid(); lst->MoveNext() )
	{
		IElem* elemToInsert = lst->GetItem();
		if ( CSearchAlgo::Contains( elemToInsert->GetLabel().AsJCharPtr(), m_searchStr ) )
		{
			if (m_btnAfficheAll.GetCheck() == BST_CHECKED  && m_lstBas.IsCouplageAttribue(elemToInsert))
				continue;
			m_lstBas.AddItem( elemToInsert );
		}
	}

	//les couplages fixes
	for( m_srcPresse->m_TBLTitres.MoveFirst(); m_srcPresse->m_TBLTitres.IsValid(); m_srcPresse->m_TBLTitres.MoveNext() )
	{
		IElem* elemToInsert = m_srcPresse->m_TBLTitres.GetItem();
		if (! elemToInsert->IsCouplage())
			continue;
		if (TestPeriodicite( elemToInsert->AsITitre() ) &&  CSearchAlgo::Contains( elemToInsert->GetLabel().AsJCharPtr() , m_searchStr) )
		{

			if ( m_btnAfficheAll.GetCheck() == BST_CHECKED && m_lstBas.IsCouplageAttribue(elemToInsert))
				continue;
				
			if (m_pData->m_appColor == CATPColors::COLORCREUSET && elemToInsert->AsITitre()->IsAgregat())
				continue;
			m_lstBas.AddItem( elemToInsert );
		}
	}
}


void CDlgSupport::FillResultListLot()
{
	m_lstHaut.ResetContent();
	CLot * lot = NULL;
	for( m_pData->m_lstIDLot.MoveFirst(); m_pData->m_lstIDLot.IsValid(); m_pData->m_lstIDLot.MoveNext() )
	{
		lot = m_pRegroupements->GetLotByID( m_pData->m_lstIDLot.GetItem() );
		if (lot)
			m_lstHaut.AddItem(lot);
	}

	int NbElmt = m_lstHaut.GetCount();
	NbElmt = 0;
}

void CDlgSupport::FillPeriod()
{
//	m_lstHaut.ResetContent();
	for (m_srcPresse->m_TBLPeriodicites.MoveFirst();
		m_srcPresse->m_TBLPeriodicites.IsValid();
		m_srcPresse->m_TBLPeriodicites.MoveNext() )
		m_lstPeriod.AddItem ( m_srcPresse->m_TBLPeriodicites.GetItem() );


/*
	for( m_pData->m_lstIDTitre.MoveFirst(); m_pData->m_lstIDTitre.IsValid(); m_pData->m_lstIDTitre.MoveNext() )
	{
		//C'est un titre
		JTitre * titre = (JTitre*)m_srcPresse->m_TBLTitres.GetElemByID( m_pData->m_lstIDTitre.GetItem() );
		if (titre)
		{
			m_lstHaut.AddItem(titre);
			continue;
		}
		//nan en fait c'est un HS
		CTitreApparente *titreHS = m_pRegroupements->GetTitreApparenteByID( m_pData->m_lstIDTitre.GetItem() );
		if (titreHS)
		{
			m_lstHaut.AddItem(titreHS);
			continue;
		}
		//Pas du tout c un apparié
		CTitreApparie *titreapp = m_pRegroupements->GetTitreApparieByID( m_pData->m_lstIDTitre.GetItem() );
		if (titreapp)
		{
			m_lstHaut.AddItem(titreapp);
			continue;
		}
	}
*/
}

void CDlgSupport::FillResultListCouplage()
{
	// Mis à jour sélection sur liste en haut
	m_lstHaut.ResetContent();
	for( m_pData->m_lstIDCouplage.MoveFirst(); m_pData->m_lstIDCouplage.IsValid(); m_pData->m_lstIDCouplage.MoveNext() )
	{
		//les couplages libres
		CCouplageLibre * cplLibre = m_pRegroupements->GetCouplageLibreByID( m_pData->m_lstIDCouplage.GetItem() );
        if (cplLibre)
		{
			m_lstHaut.AddItem( cplLibre );
			continue;
		}
		
		//les couplages fixes
		IElem * elem = m_pRegroupements->GetIElemByID( m_pData->m_lstIDCouplage.GetItem() );
		if (elem)
		{
			m_lstHaut.AddItem( elem );
			continue;
		}
		//JCouplageFixe * cplFixe = (JCouplageFixe*)m_srcPresse->m_TBLCouplagesFixes.GetElemByID( m_pData->m_lstIDCouplage.GetItem() );
		//if (cplFixe)
		//	m_lstHaut.AddItem( cplFixe );
	}

	// Coordination sélection avec liste de tous les couplages
	JList<JInt32> selIdlst;
	for(m_pData->m_lstIDCouplage.MoveFirst(); m_pData->m_lstIDCouplage.IsValid(); m_pData->m_lstIDCouplage.MoveNext() )
	{
		// Récupére ident des couplages sélection en cours
		JInt32 IdCouplage = m_pData->m_lstIDCouplage.GetItem();
		selIdlst.AddTail() =  IdCouplage;
	}

	// Mis à jour sélection liste du bas
	m_lstBas.SetSelectedID(selIdlst);
}

void CDlgSupport::OnEnChangeSearch()
{
	SetTimer(IdTimer_Search, 750, NULL);
}

void CDlgSupport::OnBnClickedRazSearch()
{
	m_searchStr = "";
	UpdateData(FALSE);
	switch(m_internalMode)
	{
	//case LOT:
	//	FillListLot();
	//	break;
	case TITRE:
		FillListTitre();
		break;
	case COUPLAGE:
		FillListCouplage();
		break;
	}
	GetDlgItem(IDC_FD_EDITSEARCH)->SetFocus();
}

void CDlgSupport::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == IdTimer_Search)
	{
		KillTimer(IdTimer_Search);
		UpdateData();
		switch(m_internalMode)
		{
		//case LOT:
		//	FillListLot();
		//	break;
		case TITRE:
			FillListTitre();
			break;
		case COUPLAGE:
			FillListCouplage();
			break;
		}
	}
	ETSLayoutDialog::OnTimer(nIDEvent);
}

void CDlgSupport::OnLbnSelchangeListBas()
{
	//on remonte
	JList<JInt32> selIdlst;
	JUnt32 selID;
	m_lstBas.GetSelectedID(selIdlst);
	m_pData->m_bModif = true;
	m_pData->m_offre.m_Changes |= m_pData->m_offre.SUPPORTS;
	switch(m_internalMode)
	{
	case TITRE:
//		m_lstBas.
		for(int i = 0; i< m_lstBas.GetCount(); i++)
		{
			IElem* elem = (IElem*)m_lstBas.GetItemData(i);
			m_pData->m_lstIDTitre.RemoveID(elem->GetID());
		}
		for(selIdlst.MoveFirst(); selIdlst.IsValid(); selIdlst.MoveNext())
		{
			//on ajoute les nouveaux supports
			if(!m_pData->m_lstIDTitre.IDExist(selIdlst.GetItem()))
				m_pData->m_lstIDTitre.AddTail() = selIdlst.GetItem();
		}
		break;

	// Gestion des couplages
	case COUPLAGE:
		/*
		for (selIdlst.MoveFirst(); selIdlst.IsValid(); selIdlst.MoveNext())
		{
			selID = selIdlst.GetItem();
			if (!m_pData->m_lstIDCouplage.IDExist(selID) )
				m_pData->m_lstIDCouplage.AddTail() = selID;
		}
		*/
		m_pData->m_lstIDCouplage.Reset();
		for (selIdlst.MoveFirst(); selIdlst.IsValid(); selIdlst.MoveNext())
		{
			selID = selIdlst.GetItem();
			m_pData->m_lstIDCouplage.AddTail() = selID;
		}
		break;

	default:
		break;
	}


	//maj de la liste
	switch(m_internalMode)
	{
	case TITRE:
		//FillPeriod();
		//m_lstHaut.SetTopIndex(m_lstHaut.GetCount()-1);
		break;
	case COUPLAGE:
		FillResultListCouplage();
		//on retire la sel
		// m_lstBas.SetSel(LB_ERR, FALSE);
		break;
	default:
		break;
	}
	m_pData->EnableOK();
}



void CDlgSupport::OnLbnSelchangeListHaut()
{
	//on suppr
	JUnt32 selID = m_lstHaut.GetSelectedID();
	if (selID != LB_ERR)
	{
		switch(m_internalMode)
		{
		case LOT:
			m_pData->m_lstIDLot.RemoveID(selID);
			FillResultListLot();
			break;
		case TITRE:
			//m_pData->m_lstIDTitre.RemoveID(selID);
			//FillPeriod();
			break;
		case COUPLAGE:
			m_pData->m_lstIDCouplage.RemoveID(selID);
			FillResultListCouplage();
			break;
		default:
			break;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		// Mise à jour sélection dans tree en bas
		/////////////////////////////////////////////////////////////////////////////////////////
		int NbItem = m_treeSupport.GetCount();
		
		m_treeSupport.ClearSelection();
		for( m_pData->m_lstIDLot.MoveFirst(); m_pData->m_lstIDLot.IsValid(); m_pData->m_lstIDLot.MoveNext() )
		{
			// Ident item sélectionné
			JUnt32 Idlot = m_pData->m_lstIDLot.GetItem();

			for (HTREEITEM hItem = m_treeSupport.GetRootItem(); hItem!=NULL; hItem = m_treeSupport.GetNextVisibleItem( hItem ))
			{
				// Récup item data
				INode * inode = (INode*) m_treeSupport.GetItemData(hItem); 

				if (inode)
				{
					// Ident Item Courant
					JUnt32 IdCurItem = inode->GetID();
					if (inode && inode->IsLot() &&  IdCurItem == Idlot)
					{
						m_treeSupport.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
					}
				}
			}
		}
		
		m_pData->m_bModif = true;
		m_pData->m_offre.m_Changes |= m_pData->m_offre.SUPPORTS;
	}
	m_lstHaut.SetCurSel(LB_ERR);
	m_pData->EnableOK();
}

void CDlgSupport::OnTvnSelChangedTreeBas(NMHDR *pNMHDR, LRESULT *pResult)
{
	// if (m_FlagEnabledTree == true) return;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	TVITEM selItem = pNMTreeView->itemNew;

	//INode* inode = (INode*)selItem.lParam;
	INode * inode = reinterpret_cast<INode*>(selItem.lParam);
	if (inode)
	{
		int Etat = selItem.state;
		JUnt32 IdItem = inode->GetID();

		if (!inode->IsLot())
		{
			AfxMessageBox("Sélection impossible, Famille vide");
			m_treeSupport.SelectItem(NULL);
		}

		if (!m_pData->m_lstIDLot.IDExist(inode->GetID()))
			bool ilnexistepas = true;

		if (inode->IsLot())
		{
			if (!m_pData->m_lstIDLot.IDExist(inode->GetID()))
			{
				// Ajoute à la liste des sélections 
				m_pData->m_lstIDLot.AddTail() = inode->GetID();
				FillResultListLot();
				m_pData->m_bModif = true;
				m_pData->m_offre.m_Changes |= m_pData->m_offre.SUPPORTS;
			}
			else
			{
				// Retire de la liste des sélections en haut
				m_pData->m_lstIDLot.RemoveID(inode->GetID());
				FillResultListLot();
				m_pData->m_bModif = true;
				m_pData->m_offre.m_Changes |= m_pData->m_offre.SUPPORTS;
			}
		}
	}
	else
	{
		// Ne pas marquer
		m_treeSupport.SelectItem(NULL);
	}

	*pResult = 0;
	m_pData->EnableOK();
	m_lstHaut.SetTopIndex(m_lstHaut.GetCount()-1);

}

void CDlgSupport::OnBtnT()
{
	m_lstBas.ToggleSelection();
	OnLbnSelchangeListBas();
}

void CDlgSupport::OnBtnT2()
{
	m_lstPeriod.ToggleSelection();
}

void CDlgSupport::OnLbnSelchangeListPeriod()
{
	FillListTitre();
}

bool CDlgSupport::TestPeriodicite(ITitre* iTitre)
{
	//si rien n'est sélectionné on considere que tt est selectionné
	if (m_lstPeriod.GetSelCount() == 0  || m_lstPeriod.GetSelCount() == m_lstPeriod.GetCount() )
		return true;

	//pour les apparentés
	//if (IDTITREAPPARENTE_MIN<ID && ID<IDTITREAPPARENTE_MAX && m_lstPeriod.GetSel(m_lstPeriod.FindItemIndex(8)) )
	//	return true;

	bool bRet = false;

	//sinon on filtre selon le sélection
	JList<JInt32> lst;
	m_lstPeriod.GetSelectedID(lst);
	for (lst.MoveFirst(); lst.IsValid(); lst.MoveNext())
	{
		//si la selection == periodicité
		if (iTitre->GetPeriodicite() == lst.GetItem() )
			bRet = true;
	}

	return bRet;
}

void CDlgSupport::SetupControls()
{
//subclass 
	m_ttlChoix.SubclassDlgItem(IDC_FD_STATIC_TTLTITRE, this);
	m_lstHaut.SubclassDlgItem(IDC_FD_LIST_HAUT, this);
	m_lstBas.SubclassDlgItem(IDC_FD_LIST_BAS, this);
	m_btnLot.SubclassDlgItem(IDC_FD_BTNR1_LOTS, this);
	m_btnTitres.SubclassDlgItem(IDC_FD_BTNR2_TITRES, this);
	m_btnCouplages.SubclassDlgItem(IDC_FD_BTNR3_COUPLAGES, this);
	m_btnRAZSearch.SubclassDlgItem(IDC_FD_BTN_RAZSEARCH, this);
	m_btnAfficheAll.SubclassDlgItem(IDC_FD_BTN_LISTALL, this);
	m_ttlPeriod.SubclassDlgItem(IDC_FD_STATIC_TTLPERIOD, this);
	m_btnT.SubclassDlgItem(IDC_FD_BTNT, this);
	m_btnT2.SubclassDlgItem(IDC_FD_BTNT2, this);
	m_lstPeriod.SubclassDlgItem(IDC_FD_LIST_PERIOD, this);

////

	m_ttlChoix.SetFontName(CATPColors::GetFontLabelName());
	m_ttlChoix.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlChoix.SetBorder(false);
	m_ttlChoix.SetText("");

	m_ttlPeriod.SetFontName(CATPColors::GetFontLabelName());
	m_ttlPeriod.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlPeriod.SetBorder(false);

	SetColors();
}

void CDlgSupport::SetColors()
{
	m_ttlChoix.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlChoix.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));

	m_ttlPeriod.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlPeriod.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));

	m_lstHaut.SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_lstHaut.SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_lstHaut.SetAttribueColor(CATPColors::GetColorRed());
	m_lstBas.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(m_pData->m_appColor));
	m_lstBas.SetAttribueColor(CATPColors::GetColorRed());
	m_lstPeriod.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(m_pData->m_appColor));

	m_treeSupport.SetSelectionColor( CATPColors::GetColorSelect(m_pData->m_appColor) );
	m_treeSupport.SetIconsColor( CATPColors::GetAppColor(m_pData->m_appColor) );
	m_treeSupport.SetAttribueColor(CATPColors::GetColorRed());


	m_btnLot.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnLot.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnLot.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnTitres.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnTitres.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnTitres.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnCouplages.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnCouplages.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnCouplages.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnRAZSearch.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnRAZSearch.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnRAZSearch.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnAfficheAll.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(m_pData->m_appColor),m_pData->m_appColor);
	m_btnAfficheAll.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnAfficheAll.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnT.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnT.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnT.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);

	m_btnT2.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnT2.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(m_pData->m_appColor),m_pData->m_appColor);
	m_btnT2.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor);
}

void CDlgSupport::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas ETSLayoutFormView::OnPaint() pour la peinture des messages
	RECT Rect;
	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	CBrush Brush(CATPColors::GetColorMedium(m_pData->m_appColor));

	// on dessine le fond
	dc.FillRect(&Rect, &Brush);
}