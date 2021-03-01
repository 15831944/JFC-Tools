// DialogVentilation.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "DialogVentilation.h"
#include "TerrainManager.h"
#include ".\dialogventilation.h"
#include "ItemsConstruitsManager.h"

#include "IBModalite.h"
#include "IBQuestion.h"


// Boîte de dialogue CDialogVentilation

IMPLEMENT_DYNAMIC(CDialogVentilation, ETSLayoutDialog)
CDialogVentilation::CDialogVentilation(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CDialogVentilation::IDD, pParent)
{
	m_pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();
	m_terrainC = NULL;
}

CDialogVentilation::~CDialogVentilation()
{
	// m_pItems->Release();
}

void CDialogVentilation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogVentilation, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_CR_BTN1QUESTION, OnBnClickedCrBtn1question)
	ON_BN_CLICKED(IDC_CR_BTN2ITEM, OnBnClickedCrBtn2item)
	ON_BN_CLICKED(IDC_CR_BTN3PALETTE, OnBnClickedCrBtn3palette)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CR_TREE, OnSelChangedTree)
	ON_LBN_SELCHANGE(IDC_CR_LIST, OnSelChangeList)
	ON_LBN_SELCHANGE(IDC_CR_LIST_HAUT, OnSelChangeResultList)
END_MESSAGE_MAP()


BOOL CDialogVentilation::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();

	//pour avoir une choli ikone!
	SetIcon(AfxGetApp()->LoadIcon(IDR_CR_ATPCREUSET), TRUE);		// Définir une grande icône
	SetIcon(AfxGetApp()->LoadIcon(IDR_CR_ATPCREUSET), FALSE);		// Définir une petite icône

	SetupControls();

	if(m_terrainC && m_pItems)
        m_tree.SetData(m_terrainC, m_pItems, m_pATPDocCreuset->m_AFFVentilation.m_listIDQST);

	for(m_pATPDocCreuset->m_AFFVentilation.m_listIDItem.MoveFirst();
		m_pATPDocCreuset->m_AFFVentilation.m_listIDItem.IsValid();
		m_pATPDocCreuset->m_AFFVentilation.m_listIDItem.MoveNext())
		m_listIDItem.AddTail() = m_pATPDocCreuset->m_AFFVentilation.m_listIDItem.GetItem();

	for(m_pATPDocCreuset->m_AFFVentilation.m_listIDPal.MoveFirst();
		m_pATPDocCreuset->m_AFFVentilation.m_listIDPal.IsValid();
		m_pATPDocCreuset->m_AFFVentilation.m_listIDPal.MoveNext())
		m_listIDPal.AddTail() = m_pATPDocCreuset->m_AFFVentilation.m_listIDPal.GetItem();
	 

	switch(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel)
	{
	case IDC_CR_BTN1QUESTION:
		OnBnClickedCrBtn1question();
		break;
	case IDC_CR_BTN2ITEM:
		OnBnClickedCrBtn2item();
		break;
	case IDC_CR_BTN3PALETTE:
		OnBnClickedCrBtn3palette();
		break;
	default:
		OnBnClickedCrBtn1question();
	}

	m_btnCheckGRP.SetCheck(1);
	m_btnCheckIndGRP.SetCheck(1);
	m_btnCheckCouv.SetCheck(1);
	m_btnCheckIndCouv.SetCheck(1);
	m_btnCheckRepet.SetCheck(1);

	m_btnCheckGRP.ShowWindow(SW_HIDE);
	m_btnCheckIndGRP.ShowWindow(SW_HIDE);
	m_btnCheckCouv.ShowWindow(SW_HIDE);
	m_btnCheckIndCouv.ShowWindow(SW_HIDE);
	m_btnCheckRepet.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

JVoid CDialogVentilation::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;
}



// Gestionnaires de messages CDialogVentilation
void CDialogVentilation::ResetSize()
{
	m_bkg.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
//	m_ttl.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	m_list.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	m_listHaut.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	m_tree.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
}
void CDialogVentilation::OnBnClickedCrBtn1question()
{
	ResetSize();
	UpdateLayout(
		pane(HORIZONTAL)
					<<(paneCtrl(IDC_CR_BK, VERTICAL, GREEDY, 4,4,0,4)
						<<item(IDC_CR_TTL, ABSOLUTE_VERT)
						<<item(IDC_CR_LIST_HAUT, RELATIVE_VERT,0,25)
						<<item(IDC_CR_TREE)
						//<<item(IDC_CR_LIST)
						<<(pane(HORIZONTAL)
							<<item(IDC_CR_BTN1QUESTION, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN2ITEM, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN3PALETTE, ABSOLUTE_VERT)
							)
					)
					<<(pane(VERTICAL)	<<item(IDOK, NORESIZE)
										<<item(IDCANCEL, NORESIZE)
										<<itemGrowing(VERTICAL)
										<<item(IDC_CHECK1, NORESIZE)
										<<item(IDC_CHECK2, NORESIZE)
										<<item(IDC_CHECK3, NORESIZE)
										<<item(IDC_CHECK4, NORESIZE)
										<<item(IDC_CHECK5, NORESIZE)

						)
		);

	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN2ITEM)
	{
		m_list.GetSelectedID(m_listIDItem);
		m_list.GetSelectedItems(m_listItem);
	}
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN3PALETTE)
	{
		m_list.GetSelectedID(m_listIDPal);
		m_list.GetSelectedItems(m_listPalettes);
	}

	CString ttl;
	ttl.LoadString(IDS_CR_INDEXTHEM);
	m_ttl.SetText(ttl);
	m_list.ShowWindow(SW_HIDE);
	m_tree.ShowWindow(SW_SHOW);
	m_tree.Invalidate();
	CheckRadioButton(IDC_CR_BTN1QUESTION, IDC_CR_BTN3PALETTE, IDC_CR_BTN1QUESTION);
	m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel = IDC_CR_BTN1QUESTION;
	FillResultList();
}

void CDialogVentilation::OnBnClickedCrBtn2item()
{
	ResetSize();
	UpdateLayout(
		pane(HORIZONTAL)
					<<(paneCtrl(IDC_CR_BK, VERTICAL, GREEDY, 4,4,0,4)
						<<item(IDC_CR_TTL, ABSOLUTE_VERT)
						<<item(IDC_CR_LIST_HAUT, RELATIVE_VERT, 0, 25)
						//<<item(IDC_CR_TREE)
						<<item(IDC_CR_LIST)
						<<(pane(HORIZONTAL, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN1QUESTION, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN2ITEM, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN3PALETTE, ABSOLUTE_VERT)
							)
					)
					<<(pane(VERTICAL)	<<item(IDOK, NORESIZE)
										<<item(IDCANCEL, NORESIZE)
										<<itemGrowing(VERTICAL)
										<<item(IDC_CHECK1, NORESIZE)
										<<item(IDC_CHECK2, NORESIZE)
										<<item(IDC_CHECK3, NORESIZE)
										<<item(IDC_CHECK4, NORESIZE)
										<<item(IDC_CHECK5, NORESIZE)
						)
		);

	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN3PALETTE)
	{
		m_list.GetSelectedID(m_listIDPal);
		m_list.GetSelectedItems(m_listPalettes);
	}
	CString ttl;
	ttl.LoadString(IDS_CR_ITEMS);
	m_ttl.SetText(ttl);
	m_tree.ShowWindow(SW_HIDE);
	m_list.ShowWindow(SW_SHOW);
	FillListItem();
	CheckRadioButton(IDC_CR_BTN1QUESTION, IDC_CR_BTN3PALETTE, IDC_CR_BTN2ITEM);
	m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel = IDC_CR_BTN2ITEM;
	FillResultList();
}

void CDialogVentilation::OnBnClickedCrBtn3palette()
{
	ResetSize();
	UpdateLayout(
		pane(HORIZONTAL)
					<<(paneCtrl(IDC_CR_BK, VERTICAL, GREEDY, 4,4,0,4)
						<<item(IDC_CR_TTL, ABSOLUTE_VERT)
						<<item(IDC_CR_LIST_HAUT, RELATIVE_VERT,0,25)
						//<<item(IDC_CR_TREE)
						<<item(IDC_CR_LIST)
						<<(pane(HORIZONTAL, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN1QUESTION, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN2ITEM, ABSOLUTE_VERT)
							<<item(IDC_CR_BTN3PALETTE, ABSOLUTE_VERT)
							)
					)
					<<(pane(VERTICAL)	<<item(IDOK, NORESIZE)
										<<item(IDCANCEL, NORESIZE)
										<<itemGrowing(VERTICAL)
										<<item(IDC_CHECK1, NORESIZE)
										<<item(IDC_CHECK2, NORESIZE)
										<<item(IDC_CHECK3, NORESIZE)
										<<item(IDC_CHECK4, NORESIZE)
										<<item(IDC_CHECK5, NORESIZE)
						)
		);

	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN2ITEM)
	{
		m_list.GetSelectedID(m_listIDItem);
		m_list.GetSelectedItems(m_listItem);
	}
	CString ttl;
	ttl.LoadString(IDS_CR_PALETTES);
	m_ttl.SetText(ttl);
	m_tree.ShowWindow(SW_HIDE);
	m_list.ShowWindow(SW_SHOW);
	FillListPalette();
	CheckRadioButton(IDC_CR_BTN1QUESTION, IDC_CR_BTN3PALETTE, IDC_CR_BTN3PALETTE);
	m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel = IDC_CR_BTN3PALETTE;
	FillResultList();
}

void CDialogVentilation::SetupControls()
{
	m_bkg.SubclassDlgItem(IDC_CR_BK, this);
	m_ttl.SubclassDlgItem(IDC_CR_TTL, this);

	m_tree.SubclassDlgItem(IDC_CR_TREE, this);
	m_btnQuestion.SubclassDlgItem(IDC_CR_BTN1QUESTION, this);
	m_btnItem.SubclassDlgItem(IDC_CR_BTN2ITEM, this);
	m_btnPalette.SubclassDlgItem(IDC_CR_BTN3PALETTE, this);
	m_list.SubclassDlgItem(IDC_CR_LIST, this);
	m_listHaut.SubclassDlgItem(IDC_CR_LIST_HAUT, this);

	m_btnOk.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	m_btnCheckGRP.SubclassDlgItem(IDC_CHECK1, this);;
	m_btnCheckIndGRP.SubclassDlgItem(IDC_CHECK2, this);
	m_btnCheckCouv.SubclassDlgItem(IDC_CHECK3, this);
	m_btnCheckIndCouv.SubclassDlgItem(IDC_CHECK4, this);
	m_btnCheckRepet.SubclassDlgItem(IDC_CHECK5, this);

	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	m_bkg.AddTopWindow(m_btnQuestion);
	m_bkg.AddTopWindow(m_btnItem);
	m_bkg.AddTopWindow(m_btnPalette);
	m_bkg.AddTopWindow(m_listHaut);
	m_bkg.AddTopWindow(m_list);
	m_bkg.AddTopWindow(m_tree);
	
	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);
	
	SetColors();
}

void CDialogVentilation::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);

	m_ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_listHaut.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORCREUSET) );
	m_list.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORCREUSET) );

	m_btnQuestion.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnQuestion.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnQuestion.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnItem.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnItem.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnItem.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnPalette.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnPalette.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnPalette.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnOk.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );

	
	m_btnCheckGRP.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCheckIndGRP.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCheckCouv.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCheckIndCouv.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCheckRepet.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );

	////m_btnCheckGRP.SetIcon();
	////m_btnCheckIndGRP.SetCheck(1);
	////m_btnCheckCouv.SetCheck(1);
	////m_btnCheckIndCouv.SetCheck(1);
	////m_btnCheckRepet.SetCheck(1);
}

void CDialogVentilation::SetTerrain(CKeyTerrain keyterrain)
{
	m_terrainC = CTerrainManager::GetInstance()->GetTerrainNoRef(keyterrain);
}

void CDialogVentilation::FillListItem()
{
	m_list.ResetContent();
	JList<const IItem *> lstItem;
	if (m_terrainC)
	{
		m_pItems->GetItemsIndependant(lstItem, m_terrainC);
		//pour chaque item
		for (lstItem.MoveFirst(); lstItem.IsValid(); lstItem.MoveNext())
		{
			IItem* item = (IItem*)lstItem.GetItem();

			if (item->IsRegistered())
				m_list.AddItem(item);
		}
		
		m_list.SetSelectedID(m_listIDItem);

	}
}

void CDialogVentilation::FillListPalette()
{
	m_list.ResetContent();
	JList<const IItem*> lstPalette;

	if (m_terrainC)
	{
		m_pItems->GetPalettes(lstPalette, m_terrainC);
		//pour chaque palette
		for (lstPalette.MoveFirst(); lstPalette.IsValid(); lstPalette.MoveNext())
		{
			IItem* item = (IItem*)lstPalette.GetItem();
			m_list.AddItem(item);
		}

		m_list.SetSelectedID(m_listIDPal);
	}
}

void CDialogVentilation::OnCancel()
{
	for(m_listAffItemQuestion.MoveFirst(); m_listAffItemQuestion.IsValid(); m_listAffItemQuestion.MoveNext())
		delete m_listAffItemQuestion.GetItem();

	ETSLayoutDialog::OnCancel();
}

void CDialogVentilation::OnOK()
{

	m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.Reset(); 

	CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	//on prepare le liste
	for(m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.MoveFirst(); m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.IsValid(); m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.MoveNext())
	{
		// Ident elmt ventil
		JInt32 IdElmt = m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.GetItem()->GetID();

		if(m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.GetItem()->GetID()<0)
			delete m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.GetItem();
	}

	m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.Reset();
	

	//if(GetCheckedRadioButton(IDC_CR_BTN1QUESTION, IDC_CR_BTN3PALETTE) == IDC_CR_BTN2ITEM)
	//{
	//	m_list.GetSelectedID(m_listIDItem);
	//	m_listHaut.GetSelectedItems(m_listItem);
	//}
	//if(GetCheckedRadioButton(IDC_CR_BTN1QUESTION, IDC_CR_BTN3PALETTE) == IDC_CR_BTN3PALETTE)
	//{
	//	m_list.GetSelectedID(m_listIDPal);
	//	m_listHaut.GetSelectedItems(m_listPalettes);
	//}
	
	m_pATPDocCreuset->m_AFFVentilation.m_listIDItem.Swap(m_listIDItem);
	m_pATPDocCreuset->m_AFFVentilation.m_listIDPal.Swap(m_listIDPal);

	
	//transfert
	if(m_listItemQuestion.GetCount() != 0)
	{
		for(m_listItemQuestion.MoveFirst(); m_listItemQuestion.IsValid(); m_listItemQuestion.MoveNext())
			m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.AddTail() = m_listItemQuestion.GetItem();
	}
	else
	{
		for(m_listAffItemQuestion.MoveFirst(); m_listAffItemQuestion.IsValid(); m_listAffItemQuestion.MoveNext())
			m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.AddTail() = m_listAffItemQuestion.GetItem();
	}
	
	for(m_listItem.MoveFirst(); m_listItem.IsValid(); m_listItem.MoveNext())
	{
		IItem * item = m_listItem.GetItem();
		if (item->IsItemConstruit())
			m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.AddTail() = item;
	}

	for(m_listPalettes.MoveFirst(); m_listPalettes.IsValid(); m_listPalettes.MoveNext())
	{
		IItem * item = m_listPalettes.GetItem();
		if(item->IsPalette())
		{
			CPalettes * palette = item->AsPalette();
			JList<JInt32> idList;
			palette->GetItems(idList);
			for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
			{
				CItemConstruit * itempal = pItems->GetIItemByID( idList.GetItem() )->AsItemConstruit();
				m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.AddTail() = itempal;
			}
		}
	}

	// Cas d'une cible normale sélectionnée
	if (!m_pATPDocCreuset->m_CiblesPondsActives)
	{
		m_pATPDocCreuset->m_MAPpItems.MoveTo(m_pATPDocCreuset->m_IdItemSel);
		m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.AddHead() = m_pATPDocCreuset->m_MAPpItems.GetItem()->AsItemConstruit();
	}	
	else
	{
		// Récupération de l'item pondéré
		m_pATPDocCreuset->m_MAPpItemsPonderes.MoveTo(m_pATPDocCreuset->m_IdItemSel);
		CItemPondere *pItemPondere = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->AsItemPondere();

		// Réaffecte les pondérations 
		m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst ();
		if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
		{
			m_pATPDocCreuset->m_MoteurPresse.SetPonderations(m_pATPDocCreuset->m_IdItemSel,m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
		}

		// Ajoute l'élmt item construit associé à l'item de base
		CItemsConstruitsBase * pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();
		m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.AddHead() = pItemsConstruits->GetItemConstruit(pItemPondere->GetIDItemBase()); 
	}

	ETSLayoutDialog::OnOK();

	m_pATPDocCreuset->UpdateAllViews(UPDATE_VENTIL);

}

void CDialogVentilation::OnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle

	FillResultList();

	*pResult = 0;
}

void CDialogVentilation::OnSelChangeList()
{
	FillResultList();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage de la liste des sélections à ventiler
void CDialogVentilation::FillResultList()
{
	// Initialistaion de la liste
	m_listHaut.ResetContent();

	// Est-ce une question ???
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN1QUESTION)
	{
		/*
		for(m_listAffItemQuestion.MoveFirst(); m_listAffItemQuestion.IsValid(); m_listAffItemQuestion.MoveNext())
			delete m_listAffItemQuestion.GetItem();

		m_listAffItemQuestion.Reset();
		*/

		// Init liste items questions
		m_listAffItemQuestion.Reset();
		
		// Récupère les élmts items construits
		// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

		// Init liste des élmts à ventiler 
		m_pATPDocCreuset->m_AFFVentilation.m_listIDQST.Reset();

		// Balayage arbre des items possibles
		for(HTREEITEM treeItem = m_tree.GetFirstSelectedItem(); treeItem!=NULL;  treeItem = m_tree.GetNextSelectedItem(treeItem))
		{
			// Récupère la question
			IQuestion * qst = (IQuestion *)m_tree.GetItemData(treeItem);
			if(qst->IsQuestion())
			{
				JInt32 IdQu = qst->GetID();

				// Récupère ident de la question
				m_pATPDocCreuset->m_AFFVentilation.m_listIDQST.AddTail() = qst->GetID();
				
				// Boucle sur les modalités de la question
				JVector<const IModalite*> vect;
				qst->GetIModalites(vect,0,0,1);
				for(vect.MoveFirst(); vect.IsValid(); vect.MoveNext())
				{
						
					const IModalite * pModalite = vect.GetItem();
					
					// Builder object
					CIBuilder Builder;

					// Get a new item construit
					// CItemConstruit *pItem = pItems->GetNewItemConstruit();
					CItemConstruit *pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetNewItemConstruit();
					
					// Set segment operator
					Builder.SetOperator(IBloc::Or);

					// Create a new segment
					CIBSegment *pISegment = new CIBSegment();
					// Set the question operator
					pISegment->SetOperator(IBloc::And);

					// Create a new question
					CIBQuestion *pIQuestion = new CIBQuestion;
					// Set the question id
					pIQuestion->SetID(qst->GetID());

					// Add the modality
					if(qst->HasDimensions() && pModalite)
					{
						JInt32 IdModa = pModalite->GetID();

						CIBModalite * pIModalite = new CIBModalite;
						pIModalite->AddValue(pModalite->GetID()); 
						pIModalite->SetOperator(IBloc::Or);
						pIQuestion->AddModalite(pIModalite);
						pItem->SetLabel(pModalite->GetLabel(),true); 
					}

					pISegment->Add(pIQuestion);
					Builder.Add(pISegment);

					// Set the builder object
					pItem->SetItemBuilder(m_terrainC, Builder);
					m_listAffItemQuestion.AddTail() = pItem;

				}
			}
		}

	}

	// Est-ce un item ???
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN2ITEM)
	{
		m_list.GetSelectedID(m_listIDItem);
		m_list.GetSelectedItems(m_listItem);
	}

	// Est-ce une palette ???
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN3PALETTE)
	{
		m_list.GetSelectedID(m_listIDPal);
		m_list.GetSelectedItems(m_listPalettes);
	}

	// Ajoute les élmts questions
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN1QUESTION)
		m_listHaut.AddItems(m_listAffItemQuestion);

	// Ajoute les élmts items
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN2ITEM)
		m_listHaut.AddItems(m_listItem);

	// Ajoute les élmts palettes
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN3PALETTE)
		m_listHaut.AddItems(m_listPalettes);

	JUnt32 NbElmt = m_listHaut.GetCount();
	NbElmt = NbElmt;



	///////////////////////////////////////// Remet les élmts sélectionnés en cours sur la liste du haut /////////////////////////////
	// Resélectionne les sélections précédentes

	JMap <JLabel,bool> MapLabelSel;

	for (m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.MoveFirst();
		 m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.IsValid();
		 m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.MoveNext())
	{
		// Get a new item construit
		IItem * pIItem = m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.GetItem();
		
		// Id item sélectionné
		JInt32 IdItem = m_pATPDocCreuset->m_AFFVentilation.m_lstItemVentil.GetItem()->GetID();

		// Recherche index dans la liste
		// JInt32 IndexList = m_listHaut.FindItemIndex(IdItem);

		MapLabelSel.Add(pIItem->GetLabel()) = true; 
	}

	// Bidouille car les items ne conservent pas leur ident d'origine ?????
	m_listItemQuestion.Reset();
	for (JUnt32 InxLst = 0; InxLst < m_listHaut.GetCount(); InxLst++)
	{
		// Récupère item
		IItem * pIItem = (IItem *)m_listHaut.GetItemDataPtr(InxLst);

		// Test par label, car les idents changent  (A REVOIR cette merde !!!!)
		MapLabelSel.MoveTo(pIItem->GetLabel());
		if (MapLabelSel.IsValid())
		{
			// Cet item à été sélectionné auparavant
			m_listHaut.SetSel(InxLst, TRUE); 

			m_listItemQuestion.AddTail() =  pIItem;
		}
	}
}

void CDialogVentilation::OnSelChangeResultList()
{
	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN1QUESTION)
	{
		if(m_listHaut.GetSelCount() > 0)
		{
			// m_listItemQuestion.Reset();
			m_listHaut.GetSelectedItems(m_listItemQuestion);
		}
		else
		{
			m_listItemQuestion.Reset();
			for(m_listAffItemQuestion.MoveFirst();m_listAffItemQuestion.IsValid();m_listAffItemQuestion.MoveNext())
				m_listItemQuestion.AddTail() = m_listAffItemQuestion.GetItem();
		}

		return;
	}


	JInt32 selID = m_listHaut.GetSelectedID();
	m_listHaut.RemoveItem(selID);

	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN2ITEM)
	{
		for(m_listIDItem.MoveFirst(); m_listIDItem.IsValid(); m_listIDItem.MoveNext())
		{
			if(m_listIDItem.GetItem() == selID)
			{
				m_listIDItem.Remove();
				break;
			}
		}

		m_list.SetSelectedID(m_listIDItem);
		m_list.GetSelectedItems(m_listItem);
	}

	if(m_pATPDocCreuset->m_AFFVentilation.m_lastBtnSel == IDC_CR_BTN3PALETTE)
	{
		for(m_listIDPal.MoveFirst(); m_listIDPal.IsValid(); m_listIDPal.MoveNext())
		{
			if(m_listIDPal.GetItem() == selID)
			{
				m_listIDPal.Remove();
				break;
			}
		}
		m_list.SetSelectedID(m_listIDPal);
		m_list.GetSelectedItems(m_listPalettes);
	}


}


