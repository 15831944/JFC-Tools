// Boite dialogue ordonnancement des headers grille �chelle
// cr�� le 08/08/2004
// DlgOrdreGrandEch.cpp : fichier d'impl�mentation
#include "stdafx.h"
#include "DlgOrdreGrandEch.h"
#include "JATPDocChoixSupport.h"
#include ".\dlgordregrandech.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgOrdreGrandEch
IMPLEMENT_DYNAMIC(CDlgOrdreGrandEch, CDialog)
CDlgOrdreGrandEch::CDlgOrdreGrandEch(CWnd* pParent,
									 JATPDocChoixSupport *pDoc)
	: CDialog(CDlgOrdreGrandEch::IDD, pParent)
{
	// R�cup�re pointeur s�lection g�n�rale
	pDocChoixSupport = pDoc;

	// Init param�tres drag drop
	m_nDragIndex	= -1;
	m_nDropIndex	= -1;
	m_pDragWnd		= 0;
	m_pDropWnd		= 0;
	m_bDragging		= false;
	m_pDragImage	= 0;

	m_Deplace		= false;
}
	

CDlgOrdreGrandEch::~CDlgOrdreGrandEch()
{
}

void CDlgOrdreGrandEch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_BK_ORDRE_GRDECH, m_BkOrdreGrandEch);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_CS_LABEL_TITRE, m_LabelTitre);
	DDX_Control(pDX, IDC_CS_LST_GRANDEURS, m_LstGrandEch);
	DDX_Control(pDX, IDC_CS_GRD_UP, m_Btn_Grandeur_UP);
	DDX_Control(pDX, IDC_CS_GRD_DOWN, m_Btn_Grandeur_DOWN);
	DDX_Control(pDX, IDC_CHECK1, m_Chk_RangVisible);
}


BEGIN_MESSAGE_MAP(CDlgOrdreGrandEch, CDialog)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_CS_LST_GRANDEURS, OnLvnBegindragCsLstGrandeurs)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CS_GRD_UP, OnBnClickedCsGrdUp)
	ON_BN_CLICKED(IDC_CS_GRD_DOWN, OnBnClickedCsGrdDown)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CS_LST_GRANDEURS, OnLvnItemchangedCsLstGrandeurs)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CS_LST_GRANDEURS, OnLvnColumnclickCsLstGrandeurs)
	ON_NOTIFY(NM_CLICK, IDC_CS_LST_GRANDEURS, OnNMClickCsLstGrandeurs)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue tri des grandeurs �chelle
BOOL CDlgOrdreGrandEch::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Mise en place des contr�les
	SetupControls();

	// Centrage de la fen�tre
	this->CenterWindow();

	// Init liste des grandeurs et ordre courant
	InitListGrandeurs();

	// S�lection des rangs visibles en cours
	InitRangVisible();

	// Positionne le check rang visible
	SetChkRangVisible();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des contr�les
void CDlgOrdreGrandEch::SetupControls()
{
	// Mise en place des couleurs
	SetColors();

	// Bouton check rang visible
	/* A VOIR
	m_Chk_RangVisible			.DrawBorder(false); 
	*/

	// Mise en plave controle liste (nom grandeur + activation rang)
	SetListGrandeur();
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des couleurs
void CDlgOrdreGrandEch::SetColors()
{
	// Couleur du background
	m_BkOrdreGrandEch.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::MEDIUM_DEGRAD);
	m_BkOrdreGrandEch.SetSunken(true);

	// Les boutons
	m_BtnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);

	// Style Label Titre
	m_LabelTitre.SetFontName(CATPColors::GetFontLabelName());
	m_LabelTitre.SetFontSize(CATPColors::GetFontLabelSize());
	m_LabelTitre.SetBorder(false);
	m_LabelTitre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LabelTitre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Liste des grandeurs
	m_LstGrandEch.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));

	// Couleurs des boutons tris s�lection tris crois�s
	// UP & DOWN Lignes
	m_Btn_Grandeur_UP.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS, 0);
	m_Btn_Grandeur_UP.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS, 0);
	m_Btn_Grandeur_UP.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_Btn_Grandeur_UP.		SetBitmaps(IDB_CS_UP,						RGB(192,192,192)); 
	m_Btn_Grandeur_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS, 0);
	m_Btn_Grandeur_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS, 0);
	m_Btn_Grandeur_DOWN.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_Btn_Grandeur_DOWN.	SetBitmaps(IDB_CS_DOWN,						RGB(192,192,192));

	// Coloris bouton check rang visible
	/* A VOIR
	m_Chk_RangVisible.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),0);
	m_Chk_RangVisible.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));	
	m_Chk_RangVisible.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS));	
	*/
}

/////////////////////////////////////////////////////////////////////////////////////////
// Lise en place de la liste grandeur
void CDlgOrdreGrandEch::SetListGrandeur()
{
	// Mode grille avec checkbox
	m_LstGrandEch.SetExtendedStyle(m_LstGrandEch.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES); 

	// Largeur de la liste
	CRect rect;
	m_LstGrandEch.GetClientRect(&rect);
	
	// Cr�ation des 2 colonnes
    CString strText;

	// Intervalle de base
	int nColInterval = rect.Width()/30;

	// Entete Rang
    strText = "Rang";
    m_LstGrandEch.InsertColumn(0, strText, LVCFMT_LEFT, 6 * nColInterval);
	
	// Entete Grandeur
	strText = "Grandeur";
    m_LstGrandEch.InsertColumn(1, strText, LVCFMT_LEFT, 25 * nColInterval);
	
	// Affect� une image liste
	m_imageState.Create(IDB_CS_BITMAP_RANG, 15, 1, RGB(255,255,255) );
	m_LstGrandEch.SetImageList( &m_imageState, TVSIL_STATE );

}

/////////////////////////////////////////////////////////////////////////////////////////
// Initialisation liste des grandeurs avec ordre affichage courant
void CDlgOrdreGrandEch::InitListGrandeurs()
{
	// Init Liste affich�e des grandeurs
	m_LstGrandEch.DeleteAllItems();

	// Boucle sur la liste des grandeurs ordonn�es courantes
	JUnt32 IndexList = 0;
	for (m_LstGrandeurs.MoveFirst();
		 m_LstGrandeurs.IsValid();
		 m_LstGrandeurs.MoveNext())
	{
		// R�cup�re ident grandeur
		JUnt32 IdGrandeur = m_LstGrandeurs.GetItem();

		if (IdGrandeur != pDocChoixSupport->GrdEch_XGRP && IdGrandeur != pDocChoixSupport->GrdEch_XRepetition)
		{

			// R�cup�re libell� grandeur
			CString LibGrandeur =  pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch,IdGrandeur);

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((IdGrandeur == pDocChoixSupport->GrdEch_XGRP)			|| 
					(IdGrandeur == pDocChoixSupport->GrdEch_XRepetition)) continue;
			
			// Ajout � la liste
			m_LstGrandEch.InsertItem(IndexList,""); // LibGrandeur); 

			// Sauve Item Index Grandeur
			m_LstGrandEch.SetItemData(IndexList,IdGrandeur); 

			// Texte grandeur
			m_LstGrandEch.SetItemText(IndexList, 1, LibGrandeur);
		
			// Set checbox pour chaque ligne
			BOOL bCheck = TRUE;
			ListView_SetItemState(m_LstGrandEch.m_hWnd, IndexList,UINT((int(bCheck) + 1) << 12), LVIS_STATEIMAGEMASK);

			// Passe � index liste suivant
			IndexList++;
		}
	}

	bool GRPPlusExist	= false;
	bool RepetPlusExist = false;
	for (m_LstGrandeurs.MoveFirst();
		    m_LstGrandeurs.IsValid();
		    m_LstGrandeurs.MoveNext())
	{
		// R�cup�re ident grandeur
		JUnt32 IdGrandeur = m_LstGrandeurs.GetItem();

		if (IdGrandeur == pDocChoixSupport->GrdEch_XGRP && pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL)
			GRPPlusExist = true;
		if (IdGrandeur == pDocChoixSupport->GrdEch_XRepetition && pDocChoixSupport->m_MoteurPresse.GetType() == CMoteurPresse::ABAQUE_ETAL)
			RepetPlusExist = true;
	}

	if (GRPPlusExist)
	{
		// R�cup�re libell� grandeur
		CString LibGrandeur =  pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch,pDocChoixSupport->GrdEch_XGRP);

		// Ajout � la liste
		m_LstGrandEch.InsertItem(IndexList,""); // LibGrandeur); 
		
		// Sauve Item Index Grandeur
		m_LstGrandEch.SetItemData(IndexList,pDocChoixSupport->GrdEch_XGRP); 

		// Texte grandeur
		m_LstGrandEch.SetItemText(IndexList, 1, LibGrandeur);

		// Passe � index liste suivant
		IndexList++;
	}

	if (RepetPlusExist)
	{
		// R�cup�re libell� grandeur
		CString LibGrandeur =  pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch,pDocChoixSupport->GrdEch_XRepetition);

		// Ajout � la liste
		m_LstGrandEch.InsertItem(IndexList,""); 

		// Sauve Item Index Grandeur
		m_LstGrandEch.SetItemData(IndexList,pDocChoixSupport->GrdEch_XRepetition); 

		// Texte grandeur
		m_LstGrandEch.SetItemText(IndexList, 1, LibGrandeur);
	}
	
	// Permet navigation colonnes
	// A FAIRE

	// Redessine la liste
	m_LstGrandEch.RedrawWindow(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion permutation des libell�s grandeurs
void CDlgOrdreGrandEch::OnLvnBegindragCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult)
{
	return;

	// Info pour drag drop
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// R�cup�re index drag item
	m_nDragIndex = pNMLV->iItem;

	// Cr�ation image drag
	POINT pt;
	int noffset = 10;
	pt.x = noffset;
	pt.y = noffset;
	m_pDragImage = m_LstGrandEch.CreateDragImage(m_nDragIndex,&pt);
	ASSERT(m_pDragImage);

	m_pDragImage->BeginDrag(0,CPoint(noffset,noffset));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);

	// Set dragging mode
	m_bDragging = true;
	m_nDropIndex = -1;
	m_pDragWnd = &m_LstGrandEch; 
	m_pDropWnd = &m_LstGrandEch; 

	SetCapture(); 

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Gestion d�placement souris
void CDlgOrdreGrandEch::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	//While the mouse is moving, this routine is called.
	//This routine will redraw the drag image at the present
	// mouse location to display the dragging.
	//Also, while over a CListCtrl, this routine will highlight
	// the item we are hovering over.

	//// If we are in a drag/drop procedure (m_bDragging is true)
	if (m_bDragging)
	{
		//// Move the drag image
		//get our current mouse coordinates
		CPoint pt(point);	

		//convert to screen coordinates
		ClientToScreen(&pt);

		//move the drag image to those coordinates
		m_pDragImage->DragMove(pt); 

		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

		//// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				JFCListCtrl* pList = (JFCListCtrl*)m_pDropWnd;
				
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				TRACE("m_nDropIndex is not -1\n");
				JFCListCtrl* pList = (JFCListCtrl*)m_pDropWnd;

				int i = 0;
				int nCount = pList->GetItemCount();
				for(i = 0; i < nCount; i++)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
		{			
			//Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			UINT uFlags;
			JFCListCtrl* pList = (JFCListCtrl*)pDropWnd;

			// Turn off hilight for previous drop target
			pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// Redraw previous item
			pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// Get the item that is below cursor
			//m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
			m_nDropIndex = ((JFCListCtrl*)pDropWnd)->HitTest(pt, &uFlags);

			// Highlight it
			pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
			// Redraw item
			pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			pList->UpdateWindow();
		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}
	*/	
	CDialog::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Gestion fin d�placement souris
void CDlgOrdreGrandEch::OnLButtonUp(UINT nFlags, CPoint point)
{
	return;
	 
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls 
		// can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; 

		//must delete it because it was created at the beginning of the drag
		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates

		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer

		// If window is CListCtrl, we perform the drop
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
		{
			CListCtrl * pDropList = (CListCtrl*)pDropWnd;
			CListCtrl * pDragList = (CListCtrl*)m_pDragWnd;
			//Set pointer to the list we are dropping on
			DropItemOnList(pDragList, pDropList); 
			//Call routine to perform the actual drop
		}
		else
		{
			CListCtrl * pDropList = 0;
			CListCtrl * pDragList = (CListCtrl*)m_pDragWnd;
			//Set pointer to the list we are dropping on
			DropItemOnList(pDragList, pDropList); 
			//Call routine to perform the actual drop
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Gestion drag&drop
void CDlgOrdreGrandEch::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
 	//This routine performs the actual drop of the item dragged.
	//It simply grabs the info from the Drag list (pDragList)
	// and puts that info into the list dropped on (pDropList).
	//Send:	pDragList = pointer to CListCtrl we dragged from,
	//		pDropList = pointer to CListCtrl we are dropping on.
	//Return: nothing.

	////Variables


	// Unhilight the drop target
	pDropList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);

	//Set up the LV_ITEM for retrieving item from pDragList and adding the new item to the pDropList
	char szLabel[256];
	LVITEM lviT;
	LVITEM* plvitem;

	ZeroMemory(&lviT, sizeof (LVITEM)); //allocate and clear memory space for LV_ITEM
	lviT.iItem		= m_nDragIndex;
	lviT.mask		= LVIF_TEXT;
	lviT.pszText		= szLabel;
	lviT.cchTextMax	= 255;

	lvItem* pItem;
	lvItem lvi;
	lvi.plvi = &lviT;
	lvi.plvi->iItem = m_nDragIndex;
	lvi.plvi->mask = LVIF_TEXT;
	lvi.plvi->pszText = szLabel;
	lvi.plvi->cchTextMax = 255;

	if(pDragList->GetSelectedCount() == 1)
	{

		// R�cup�re itemdata �lmt transf�r�
		JUnt32 ItemData = pDropList->GetItemData(m_nDragIndex);

		// Get item that was dragged
		pDragList->GetItem (lvi.plvi);
		lvi.sCol2 = pDragList->GetItemText(lvi.plvi->iItem, 1);

		// Delete the original item (for Move operation)
		// This is optional. If you want to implement a Copy operation, don't delete.
		// This works very well though for re-arranging items within a CListCtrl.
		// It is written at present such that when dragging from one list to the other
		//  the item is copied, but if dragging within one list, the item is moved.
		if(pDragList == pDropList)
		{
			pDragList->DeleteItem (m_nDragIndex);
			if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //decrement drop index to account for item
															//being deleted above it
		}

		// Insert item into pDropList
		// if m_nDropIndex == -1, iItem = GetItemCount() (inserts at end of list), else iItem = m_nDropIndex
		lvi.plvi->iItem = (m_nDropIndex == -1) ? pDropList->GetItemCount () : m_nDropIndex;
		pDropList->InsertItem (lvi.plvi);
		pDropList->SetItemText(lvi.plvi->iItem, 1, (LPCTSTR)lvi.sCol2);

		pDropList->SetItemData(lvi.plvi->iItem,ItemData); 
		
		// Select the new item we just inserted
		pDropList->SetItemState (lvi.plvi->iItem, LVIS_SELECTED, LVIS_SELECTED);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Validation nouvelle s�lection ordre affichage granduers �chelle
void CDlgOrdreGrandEch::OnBnClickedOk()
{
	// Init Liste ident grandeurs
	m_LstGrandeurs.Reset();

	// R�cup�re nouvel ordre affichage via itemdata liste
	for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
	{
		// Ajout l'ident item grandeur
		m_LstGrandeurs.AddTail() = m_LstGrandEch.GetItemData(i);  

		// Ident grandeur
		JUnt32 IdGrandeur = m_LstGrandEch.GetItemData(i);

		// Etat Rang Visible
		JBool EtatRangVisible = m_LstGrandEch.GetCheck(i); 

		// Repositionne affichage rang r�sultat
		pDocChoixSupport->SetRangVisible(pDocChoixSupport->ModuleEch, EtatRangVisible, IdGrandeur); 

	}

	// Position visibilit� des rangs 
	if(m_Chk_RangVisible.GetCheck() != BST_CHECKED)
		pDocChoixSupport->m_AFFRangResultatEch = true;
	else
		pDocChoixSupport->m_AFFRangResultatEch = false;

	// On est cens� avoir chang� les rangs s�lectionn�s
	pDocChoixSupport->SetModifiedFlag(TRUE);

	// Validation du nouveau choix ordre entete et rang visible
	OnOK();
}

// Annulation nouvelle s�lection ordre affichage granduers �chelle
void CDlgOrdreGrandEch::OnBnClickedCancel()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contr�le
	OnCancel();
}

//////////////////////////////////////////////////////////////////////////////////////////
// D�calage grandeur vers le haut
void CDlgOrdreGrandEch::OnBnClickedCsGrdUp()
{
	// N� de la colonne grandeur
	static int ColGrandeur = 1;

	// D�but Mode d�placement item
	m_Deplace = true;

    if (m_LstGrandEch.GetSelectedCount() == 1)
	{
		// Index de l'�l�ment s�lectionn�
		POSITION  Pos = m_LstGrandEch.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Voir si ce n'est pas le 1er �lmt
		if (SelIndex != 0)
		{
			// Permutation contenu item en page
			for (int i= 0; i < m_LstGrandEch.GetItemCount(); i++)
			{
				if (i == SelIndex)
				{
					// Prend le texte courant
					CString TxtCur = m_LstGrandEch.GetItemText(i, ColGrandeur);

					// Prend le texte pr�c�dent
					CString TxtPrec = m_LstGrandEch.GetItemText(i-1, ColGrandeur);
					
					// Item Data Courant
					DWORD_PTR  DataCur  = m_LstGrandEch.GetItemData(i); 
					
					// Item Data Precedent
					DWORD_PTR  DataPrec = m_LstGrandEch.GetItemData(i-1); 
					
					// Permute les datas
					m_LstGrandEch.SetItemData(i-1,DataCur);
					m_LstGrandEch.SetItemData(i,DataPrec);

					// Permut les textes
					m_LstGrandEch.SetItemText(i-1, ColGrandeur, TxtCur);
					m_LstGrandEch.SetItemText(i, ColGrandeur, TxtPrec);

					// Mise � jour S�lection sur item pr�c�dent
					m_LstGrandEch.SetItemState(i-1,LVIS_SELECTED, LVIS_SELECTED);
					m_LstGrandEch.SetItemState(i,0,LVIS_SELECTED);

					// D�cale les chekbox
					JBool ValCheck1 = m_LstGrandEch.GetCheck(i);
					JBool ValCheck2 = m_LstGrandEch.GetCheck(i-1);
					m_LstGrandEch.SetCheck(i,ValCheck2);
					m_LstGrandEch.SetCheck(i-1,ValCheck1);

					// Marque derni�re s�lection
					m_LstGrandEch.SetSelectionMark(i);
										
					// Redessine la partie liste modifi�e
					m_LstGrandEch.RedrawItems(i-1,i);

					// Stop
					break;
				}
			}
		}
	}
	else if (m_LstGrandEch.GetSelectedCount() > 1)
	{
		// Plusieurs items s�lectionn�s / Index du 1er et dernier elmt s�lectionn�
		POSITION  Pos = m_LstGrandEch.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Nombre de s�lections
		JUnt32 NbSel = m_LstGrandEch.GetSelectedCount(); 

		// Index elmt � redescendre (= juste apres le bloc s�lection)
		int IndexPrecBloc = SelIndex - 1;

		if (IndexPrecBloc >= 0)
		{
			// R�cup Texte de cet item
			CString TxtPrec = m_LstGrandEch.GetItemText(IndexPrecBloc,ColGrandeur);

			// Recup Info Item 
			DWORD_PTR	DataPrec = m_LstGrandEch.GetItemData(IndexPrecBloc); 

			// Puis on d�cale tout le bloc s�lection d'un cran vers le bas de la liste
			for (int i = SelIndex; i < SelIndex + m_LstGrandEch.GetSelectedCount() ; i++)
			{
				// Prend le texte courant
				CString TxtCur = m_LstGrandEch.GetItemText(i,ColGrandeur);

				// Item Data Courant
				DWORD_PTR  DataCur  = m_LstGrandEch.GetItemData(i); 

				// Puis on le d�cale vers la ligne pr�c�dente
				m_LstGrandEch.SetItemData(i - 1,DataCur);
				m_LstGrandEch.SetItemText(i - 1, ColGrandeur, TxtCur);

				// Mise � jour S�lection sur item pr�c�dent
				m_LstGrandEch.SetItemState(i - 1,LVIS_SELECTED, LVIS_SELECTED);
				m_LstGrandEch.SetItemState(i, ColGrandeur, LVIS_SELECTED);

				// D�cale les chekbox
				JBool ValCheck1 = m_LstGrandEch.GetCheck(i);
				JBool ValCheck2 = m_LstGrandEch.GetCheck(i-1);
				m_LstGrandEch.SetCheck(i,ValCheck2);
				m_LstGrandEch.SetCheck(i-1,ValCheck1);
				
				// Marque derni�re s�lection
				m_LstGrandEch.SetSelectionMark(i);
			}

			// Remet en fin de bloc, l'item qui �tait devant le bloc
			m_LstGrandEch.SetItemData(SelIndex + m_LstGrandEch.GetSelectedCount() - 1,DataPrec);
			m_LstGrandEch.SetItemText(SelIndex + m_LstGrandEch.GetSelectedCount() - 1, ColGrandeur, TxtPrec);
			m_LstGrandEch.SetItemState(SelIndex + m_LstGrandEch.GetSelectedCount() - 1 ,0,LVIS_SELECTED);

			// Redessine la liste
			m_LstGrandEch.RedrawItems(0,m_LstGrandEch.GetItemCount()-1);
		}
	}

	// Fin mode d�placement item
	m_Deplace = false;

	// On remet le focus sur le controle liste grandeur (sinon la s�lection en cours disparait)
	m_LstGrandEch.SetFocus();
	m_LstGrandEch.RedrawWindow(); 

}

//////////////////////////////////////////////////////////////////////////////////////////
// D�calage grandeur vers le bas
void CDlgOrdreGrandEch::OnBnClickedCsGrdDown()
{
	// N� de la colonne grandeur
	static int ColGrandeur = 1;

	// D�but mode d�placement item
	m_Deplace = true;

    if (m_LstGrandEch.GetSelectedCount() == 1)
	{
		// Index de l'�l�ment s�lectionn�
		POSITION  Pos = m_LstGrandEch.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Voir si ce n'est pas le dernier �lmt
		if (SelIndex != m_LstGrandEch.GetItemCount()-1)
		{
			// Permutation contenu item en page
			for (int i= 0; i < m_LstGrandEch.GetItemCount(); i++)
			{
				if (i == SelIndex)
				{
					// Prend le texte courant
					CString TxtCur = m_LstGrandEch.GetItemText(i, ColGrandeur);

					// Prend le texte suivant
					CString TxtSuiv = m_LstGrandEch.GetItemText(i+1, ColGrandeur);
		
					// Item Data Courant
					DWORD_PTR  DataCur  = m_LstGrandEch.GetItemData(i); 
					
					// Item Data Suivant
					DWORD_PTR  DataSuiv = m_LstGrandEch.GetItemData(i+1); 
					
					// Permute les datas
					m_LstGrandEch.SetItemData(i+1,DataCur);
					m_LstGrandEch.SetItemData(i,DataSuiv);

					// Permut les textes
					m_LstGrandEch.SetItemText(i, ColGrandeur, TxtSuiv);
					m_LstGrandEch.SetItemText(i+1, ColGrandeur, TxtCur);

					// Mise � jour S�lection sur item suivant
					m_LstGrandEch.SetItemState(i+1,LVIS_SELECTED, LVIS_SELECTED);
					m_LstGrandEch.SetItemState(i, ColGrandeur, LVIS_SELECTED);

					// D�cale les chekbox
					JBool ValCheck1 = m_LstGrandEch.GetCheck(i+1);
					JBool ValCheck2 = m_LstGrandEch.GetCheck(i);
					m_LstGrandEch.SetCheck(i+1,ValCheck2);
					m_LstGrandEch.SetCheck(i,ValCheck1);

					// Marque derni�re s�lection
					m_LstGrandEch.SetSelectionMark(i+1);

					// Redessine la partie liste modifi�e
					m_LstGrandEch.RedrawItems(i,i+1);

					// Stop
					break;
				}
			}
		}
	}
	else if (m_LstGrandEch.GetSelectedCount() > 1)
	{
		// Plusieurs items s�lectionn�s / Index du 1er et dernier elmt s�lectionn�
		POSITION  Pos = m_LstGrandEch.GetFirstSelectedItemPosition();
		int SelIndex = JUnt32(Pos) - 1;

		// Nombre de s�lections
		JUnt32 NbSel = m_LstGrandEch.GetSelectedCount(); 

		// Index elmt � remonter (= juste apres le bloc s�lection)
		int IndexSuivBloc = SelIndex + NbSel;

		if (IndexSuivBloc < m_LstGrandEch.GetItemCount())
		{
			// R�cup Texte de cet item
			CString TxtSuiv = m_LstGrandEch.GetItemText(IndexSuivBloc, ColGrandeur);

			// Recup Info Item 
			DWORD_PTR	DataSuiv = m_LstGrandEch.GetItemData(IndexSuivBloc); 

			// Puis on d�cale tout le bloc s�lection d'un cran vers le bas de la liste
			for (int i = SelIndex + NbSel - 1; i >= SelIndex; i--)
			{
				// Prend le texte courant
				CString TxtCur = m_LstGrandEch.GetItemText(i, ColGrandeur);

				// Item Data Courant
				DWORD_PTR  DataCur  = m_LstGrandEch.GetItemData(i); 

				// Puis on le d�cale vers la ligne suivante
				m_LstGrandEch.SetItemData(i + 1,DataCur);
				m_LstGrandEch.SetItemText(i+1, ColGrandeur, TxtCur);

				// Mise � jour S�lection sur item suivant
				m_LstGrandEch.SetItemState(i + 1,LVIS_SELECTED, LVIS_SELECTED);
				m_LstGrandEch.SetItemState(i, ColGrandeur, LVIS_SELECTED);

				// D�cale les chekbox
				JBool ValCheck1 = m_LstGrandEch.GetCheck(i+1);
				JBool ValCheck2 = m_LstGrandEch.GetCheck(i);
				m_LstGrandEch.SetCheck(i+1,ValCheck2);
				m_LstGrandEch.SetCheck(i,ValCheck1);

				// Marque derni�re s�lection
				m_LstGrandEch.SetSelectionMark(i+1);
			}

			// Remet en t�te de bloc, l'item qui �tait derriere le bloc
			m_LstGrandEch.SetItemData(SelIndex,DataSuiv);
			m_LstGrandEch.SetItemText(SelIndex, ColGrandeur, TxtSuiv);
			m_LstGrandEch.SetItemState(SelIndex,0,LVIS_SELECTED);

			// Redessine la liste
			m_LstGrandEch.RedrawItems(0,m_LstGrandEch.GetItemCount()-1);
		}
	}

	// Fin mode d�placement item
	m_Deplace = false;

	// On remet le focus sur le controle liste grandeur (sinon la s�lection en cours disparait)
	m_LstGrandEch.SetFocus();
	m_LstGrandEch.RedrawWindow(); 
}

/////////////////////////////////////////////////////////////////////////////////////////
// Initialisation liste des grandeurs avec s�lection rang en cours
void CDlgOrdreGrandEch::InitRangVisible()
{
	// Boucle sur la liste des grandeurs
	JUnt32 IndexList = 0;
	for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
		 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
		 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
	{
		// Ident grandeur
		JUnt32 &IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

		// Etat affichage rang r�sultat
		JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 
		
		// S�lection ou non s�lection du rang
		if (EtatAffichRang)
			m_LstGrandEch.SetCheck(IndexList,TRUE);
		else
			m_LstGrandEch.SetCheck(IndexList,FALSE);

		// Passe � index liste suivant
		IndexList++;
	}

	// Positionne �tat s�lection des rangs
	SetChkRangVisible();

	// Puis enfin positionne l'�tat rang visible ou pas
	if (pDocChoixSupport->m_AFFRangResultatEch)
		m_Chk_RangVisible.SetCheck(BST_CHECKED);
	else
		m_Chk_RangVisible.SetCheck(BST_UNCHECKED);
		
}

/////////////////////////////////////////////////////////////////////
// Positionne le checkbox rang visible
void CDlgOrdreGrandEch::SetChkRangVisible()
{
	// R�cup item s�lectionn�
	JInt32 ItemSelected = -1;
	for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
	{
		// Etat Rang Visible
		if (m_LstGrandEch.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			ItemSelected  = i;
			break;
		}
	}
	

	// Sinon on d�valide
	// A VOIR m_Chk_RangVisible.SetCheck(BST_UNCHECKED);
	// m_Chk_RangVisible.SetCheck(0,BST_UNCHECKED);

	// Compte le nombre de grandeur avec rang s�lectionn�
	JUnt32 NbRangSel = 0;
	for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
	{
		// Etat Rang Visible
		if (m_LstGrandEch.GetCheck(i))
		{
			// Rend actif la controle checkbox rang visible ssi au moins 1 rang s�lectionn�
			// m_Chk_RangVisible.SetCheck(BST_CHECKED); 
			NbRangSel++;
		}
	}		

	/*
	if (NbRangSel == m_LstGrandEch.GetItemCount())
		// Tous les rangs s�lectionn�s
		// A VOIRm_Chk_RangVisible.SetCheck(BST_CHECKED); 
		// m_Chk_RangVisible.SetCheck(0,BST_CHECKED); 
	else if (NbRangSel > 0)
		// Au moins 1 rang s�lectionn�
		// m_Chk_RangVisible.SetCheck(BST_INDETERMINATE); 
		// m_Chk_RangVisible.SetCheck(0,BST_INDETERMINATE); 

	else
		// Aucun rang s�lectionn�
		// Au moins 1 rang s�lectionn�
		// m_Chk_RangVisible.SetCheck(BST_UNCHECKED);
		// m_Chk_RangVisible.SetCheck(0,BST_UNCHECKED);
	*/

	if (NbRangSel == m_LstGrandEch.GetItemCount())
		// Tous les rangs s�lectionn�s
		m_Chk_RangVisible.SetCheck(BST_CHECKED); 

	else if (NbRangSel > 0)
		// Au moins 1 rang s�lectionn�
		m_Chk_RangVisible.SetCheck(BST_INDETERMINATE); 

	else
		// Aucun rang s�lectionn�
		// Au moins 1 rang s�lectionn�
		m_Chk_RangVisible.SetCheck(BST_UNCHECKED);

	// Remet l'ancien item s�lectionn�
	if (ItemSelected >= 0)
	{
		m_LstGrandEch.SetItemState(ItemSelected,LVIS_SELECTED,LVIS_SELECTED);
		
		// Redessine la liste
		m_LstGrandEch.RedrawItems(0,m_LstGrandEch.GetItemCount()-1);
		m_LstGrandEch.RedrawWindow(); 
	}
}

/////////////////////////////////////////////////////////////////////
// Nombre de rangs s�lectionn�s
JUnt32 CDlgOrdreGrandEch::GetNbRangSel()
{
	// Compte le nombre de grandeur avec rang s�lectionn�
	JUnt32 NbRangSel = 0;
	for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
	{
		// Etat Rang Visible
		if (m_LstGrandEch.GetCheck(i))
		{
			// Un rang de + s�lectionn�
			NbRangSel++;
		}
	}			

	// Retourne le nombre de rangs s�lectionn�s
	return NbRangSel;
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour check box visibilit� des rangs dans �chelle
void CDlgOrdreGrandEch::OnLvnItemchangedCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// Repositionne le check box rang visible via le nombre de rangs s�lectionn�s
	if (!m_Deplace) 
	{
		// Repositionne les check item, et d�valide item s�lectionn�
		SetChkRangVisible();
	}
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
// D�selection ou s�lection compl�te des rangs
void CDlgOrdreGrandEch::OnLvnColumnclickCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult)
{
	// R�cup�re info colonne s�lectionn�e
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// Repostionne le check box rang visible via le nombre de rangs s�lectionn�s
	SetChkRangVisible();

	return;

	// VOIR si on vire la suite !!!!!

	// Seul la 1er colonne est active pour s�lection et d�selection totale des rangs
	if (pNMLV->iSubItem == 0) 
	{
		if (GetNbRangSel() > 0)
		{
			// D�s�lection de toutes les s�lections rangs
			for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
			{
				// Remet Etat Rang D�slectionn�
				m_LstGrandEch.SetCheck(i, false);
			}
		}
		else
		{
			// S�lection de toutes les s�lections rangs
			for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
			{
				// Remet Etat Rang S�lectionn�
				m_LstGrandEch.SetCheck(i, true);
			}
		}
		
		// Repostionne le check box rang visible via le nombre de rangs s�lectionn�s
		SetChkRangVisible();
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// Valide ou d�valide tous les rangs
void CDlgOrdreGrandEch::OnBnClickedChkRangVisible()
{
	// R�cup item s�lectionn�
	JInt32 ItemSelected = -1;
	for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
	{
		// Etat Rang Visible
		if (m_LstGrandEch.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			ItemSelected  = i;
			break;
		}
	}

	// A VOIR if (m_Chk_RangVisible.GetCheck() == BST_CHECKED)
	/*
	if (m_Chk_RangVisible.GetCheck(0) == BST_CHECKED)
	{
		// D�s�lection de toutes les s�lections rangs
		for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
		{
			// Remet Etat Rang D�slectionn�
			m_LstGrandEch.SetCheck(i, true);
		}

		// Revalide s�lection btn principal
		// m_Chk_RangVisible.SetCheck(BST_CHECKED);
	}
	else
	{
		// S�lection de toutes les s�lections rangs
		for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
		{
			// Remet Etat Rang s�lectionn�
			m_LstGrandEch.SetCheck(i, false);
		}

		// Revalide s�lection btn principal
		// m_Chk_RangVisible.SetCheck(BST_UNCHECKED);
	}
	*/

	// Repositionne l'�tat s�lection des rangs
	SetChkRangVisible();

	// Remet l'ancien item s�lectionn�
	if (ItemSelected >= 0)
	{
		m_LstGrandEch.SetItemState(ItemSelected,LVIS_SELECTED,LVIS_SELECTED);

		// Redessine la liste
		m_LstGrandEch.RedrawItems(0,m_LstGrandEch.GetItemCount()-1);
		m_LstGrandEch.SetFocus();
		m_LstGrandEch.RedrawWindow(); 
	}
}

void CDlgOrdreGrandEch::OnNMClickCsLstGrandeurs(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Repostionne le check box rang visible via le nombre de rangs s�lectionn�s
	SetChkRangVisible();

	*pResult = 0;
}

// Valider ou d�valider tous les rangs
void CDlgOrdreGrandEch::OnBnClickedCheck1()
{
	// Si �tat unckecked ou ind�terminate
	if (m_Chk_RangVisible.GetCheck() ==  BST_UNCHECKED || m_Chk_RangVisible.GetCheck() == BST_INDETERMINATE)
	{
		// Check� tous les rangs dans la liste des grandeurs
		for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
		{
			// Remet Etat Rang S�lectionn�
			m_LstGrandEch.SetCheck(i, true);
		}
	}
	else
	{
		// Uncheck� tous les rangs dans la liste des grandeurs
		for (JUnt32 i= 0; i<m_LstGrandEch.GetItemCount(); i++)
		{
			// Remet Etat Rang D�s�lectionn�
			m_LstGrandEch.SetCheck(i, false);
		}
	}

	// Repositionne l'�tat s�lection des rangs
	SetChkRangVisible();

		
}

void CDlgOrdreGrandEch::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	this->Invalidate(); 

	// TODO : ajoutez ici le code de votre gestionnaire de messages
}
