// DialogChoixOffre.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DialogChoixOffre.h"
#include "DlgSrcUnique.h"
#include "JMFCStream.h"

#include "DlgSupport.h"
#include "DlgCible.h"
#include "DlgSupportsInexistants.h"

#include "JATPDocChoixSupport.h"
#include "ItemsConstruitsManager.h"

#include <map>

// Boîte de dialogue CDialogChoixOffre
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDialogChoixOffre, ETSLayoutDialog)
CDialogChoixOffre::CDialogChoixOffre(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CDialogChoixOffre::IDD, pParent)
{
	m_pActivePage			= NULL;
	m_bModif				= false;
	m_preselcible			= false;
	m_pdlgSource			= NULL;
	m_pdlgSupport			= NULL;
	m_pdlgCible				= NULL;

	m_appColor				= CATPColors::COLORCREUSET;
	m_OffreTerrainManager	= COffreTerrainManager::GetInstance();

	// Par défaut la page active est la page terrain
	m_startMode				= SOURCE;

	// Définition des couleurs comme dans Creuset
	m_appColor				= CATPColors::COLORCREUSET;

	m_bGripper				= false;
}

CDialogChoixOffre::~CDialogChoixOffre()
{
	m_OffreTerrainManager->Unload();
}

void CDialogChoixOffre::OnBnClickedOk()
{
	//aucun terrain n'a été selectionné
	if (m_offre.m_TBLIdTerrainsAudience.GetCount() == 0)
	{
		AfxMessageBox( IDS_FD_STR_ERR_TERRA, MB_OK | MB_ICONSTOP);
		OnBnClickedBtnSource();	
		return;
	}
	//aucun terrain n'a été selectionné
	if (m_offre.m_IdTerrainCiblage == 0)
	{
		AfxMessageBox( IDS_FD_STR_ERR_TERRC, MB_OK | MB_ICONSTOP);
		OnBnClickedBtnSource();	
		return;
	}
	
	//si les trois listes sont vides
	if (m_lstIDTitre.GetCount()==0 && m_lstIDLot.GetCount()==0 && m_lstIDCouplage.GetCount()==0  )
	{
		AfxMessageBox( IDS_FD_STR_ERR_SUPPORT, MB_OK | MB_ICONSTOP);
		OnBnClickedBtnSupport();	
		return;
	}

	//si les trois listes sont vides
	if ( m_offre.m_TBLIdItems.GetCount()== 0 )  
	{
		AfxMessageBox( IDS_FD_STR_ERR_CIBLE, MB_OK | MB_ICONSTOP);
		OnBnClickedBtnCible();	
		return;
	}
	//pas d'item affinité
	if ( m_offre.m_IdItemAffinite == 0)
	{
		AfxMessageBox( IDS_FD_STR_ERR_IAFF, MB_OK | MB_ICONSTOP);
		OnBnClickedBtnCible();	
		return;
	}

	// Sauve les autosrisations cibles
	if (m_pdlgCible)
		m_offre.m_MapInterdictionsItems.Swap(m_pdlgCible->m_MapTerrainsInterdits);

	//si on est là c'est que tout est complet
	ETSLayoutDialog::OnOK();
}

void CDialogChoixOffre::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogChoixOffre, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_FD_BTNR1_SOURCE,		OnBnClickedBtnSource)
	ON_BN_CLICKED(IDC_FD_BTNR2_SUPPORT,		OnBnClickedBtnSupport)
	ON_BN_CLICKED(IDC_FD_BTNR3_CIBLE,		OnBnClickedBtnCible)
	ON_BN_CLICKED(IDC_FD_OK,				OnBnClickedOk)
	ON_BN_CLICKED(IDC_FD_BTN_SEUILS,		OnBnClickedBtnSeuils)
	ON_BN_CLICKED(IDC_FD_BTN_INFO_SEUILS,	OnBnClickedBtnInfoSeuils)
	ON_BN_CLICKED(IDC_FD_IMPORT,			OnBnClickedBtnImport)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDialogChoixOffre::SetStartupMode(STARTUPMODE start, CATPColors::APPCOLOR app)
{
	m_startMode = start;
	m_appColor = app;
}

bool CDialogChoixOffre::IsModified()
{
	return m_bModif;
}

BOOL CDialogChoixOffre::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	SetupControls();

	m_pdlgSource = new CDlgSrcUnique(this);
	m_pdlgSource->SetData(this);

	m_pdlgSupport = new CDlgSupport(this);
	m_pdlgSupport->SetData(this);

	m_pdlgCible = new CDlgCible(this);
	m_pdlgCible->SetData(this);

	//Création de la page source
	if(m_pdlgSource->Create(CDlgSrcUnique::IDD, this) == FALSE)
		return FALSE;

	//Création de la page support
	if(m_pdlgSupport->Create(CDlgSupport::IDD, this) == FALSE)
		return FALSE;
	
	//Création de la page cibles
	if(m_pdlgCible->Create(CDlgCible::IDD, this) == FALSE)
	{
		return FALSE;
	}	
	else
	{
		// On indique que la page cible est bien crée
		m_pdlgCible->CheckSeuils();
	}
	
	//mise en place du mode de démarrage
	switch (m_startMode)
	{
		case IMPORT:
			OnBnClickedBtnImport();

		case SOURCE:

			// Par défaut la page active est la page sélection terrains
			CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR1_SOURCE);
			m_pActivePage = m_pdlgSource;

			// Affiche la page active dans sa zone
			UpdateDlgPos(TRUE);
			break;

		case SUPPORT:
			CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR2_SUPPORT);
			m_pActivePage = m_pdlgSupport;

			// Titre Source + Terrains en titre
			AfficheTitreFenetre();

			// Affiche la page active dans sa zone
			UpdateDlgPos(TRUE);

			break;

		case CIBLE:
			// Affichage des cibles
			CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR3_CIBLE);

			// On vire le bouton controle seuil pour le moment (à priori inutile après modif seuil par terrains)
			// m_btnSeuils.ShowWindow(SW_SHOW);

			// En mode cible, bouton infos seuil
			m_btnInfoSeuils.ShowWindow(SW_SHOW);

			m_pActivePage = m_pdlgCible;

			// Titre Source + Terrains en titre
			AfficheTitreFenetre();

			// Récup id source d'audience
			((CDlgCible*)m_pActivePage)->m_IdSrcAudience = m_offre.m_IdSourceAudience;

			// Récup liste des idents terrains
			((CDlgCible*)m_pActivePage)->m_LstIdTerrains.Reset();
			for (m_offre.m_TBLIdTerrainsAudience.MoveFirst();  m_offre.m_TBLIdTerrainsAudience.IsValid(); m_offre.m_TBLIdTerrainsAudience.MoveNext())
			{	
				// Récup ident terrain
				JUnt32 TerID = m_offre.m_TBLIdTerrainsAudience.GetItem();
				((CDlgCible*)m_pActivePage)->m_LstIdTerrains.AddTail() = TerID;
			}

			// Vérification automatique des seuils 
			// ((CDlgCible*)m_pActivePage)->CheckSeuils();

			// Affiche la page active dans sa zone
			UpdateDlgPos(TRUE);

			break;
	}

	m_btnImport.ShowWindow(SW_HIDE);

	EnableOK();	

	UpdateLayout(
		pane(HORIZONTAL)
					<<item(IDC_FD_STATIC_FILLE)
					<<(pane(VERTICAL)	<<item(IDC_FD_OK, NORESIZE)
										<<item(IDCANCEL, NORESIZE)
										<<itemGrowing(VERTICAL)
										<<item(IDC_FD_IMPORT, NORESIZE)
										<<itemGrowing(VERTICAL)
										<<item(IDC_FD_BTN_SEUILS, NORESIZE)
										<<item(IDC_FD_BTN_INFO_SEUILS, NORESIZE)
										<<itemGrowing(VERTICAL)
										<<item(IDC_FD_BTNR1_SOURCE, NORESIZE)
										<<item(IDC_FD_BTNR2_SUPPORT, NORESIZE)
										<<item(IDC_FD_BTNR3_CIBLE, NORESIZE)
						)
		);

	// Affiche la page active dans sa zone
	UpdateDlgPos();
	return TRUE;  // return TRUE unless you set the focus to a control
}

////////////////////////////////////////////////////////////////////////////////////////////
// Sélection de la source et des terrains
void CDialogChoixOffre::OnBnClickedBtnSource()
{
	CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR1_SOURCE);
	m_pActivePage->ShowWindow(SW_HIDE);

	// Cache les boutons seuils
	m_btnSeuils.ShowWindow(SW_HIDE);
	m_btnInfoSeuils.ShowWindow(SW_HIDE);

	m_pActivePage = m_pdlgSource;

	// Affichage seulement du titre vierge (en effet pas bessoin source et terrain visible sur ce panel)
	CString TxtTitreOffre;
	TxtTitreOffre.Format(IDS_FD_TITRE_OFFRE,"");
	SetWindowText(TxtTitreOffre);

	// Affiche la page active dans sa zone
	UpdateDlgPos(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Sélection des supports
void CDialogChoixOffre::OnBnClickedBtnSupport()
{
	// Teste si au moins 1 segment de sélectionner
	if (m_offre.m_TBLIdSegmentsAudience.GetCount() <= 0)
	{
		CString TxtMess;
		TxtMess.LoadString(IDS_MESS_SEG_NONSEL); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION | MB_OK);
		CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR1_SOURCE);
		return;
	}

	CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR2_SUPPORT);
	m_pActivePage->ShowWindow(SW_HIDE);

	// Cache les boutons seuils
	m_btnSeuils.ShowWindow(SW_HIDE);
	m_btnInfoSeuils.ShowWindow(SW_HIDE);

	m_pActivePage = m_pdlgSupport;

	// Affichage Sources et Terrains en cours de sélection !!!!.....
	AfficheTitreFenetre();

	// Affiche la page active dans sa zone
	UpdateDlgPos(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Sélection des cibles (items + palettes + référence affinité)
void CDialogChoixOffre::OnBnClickedBtnCible()
{

	// Teste si au moins 1 segment de sélectionner
	if (m_offre.m_TBLIdSegmentsAudience.GetCount() <= 0)
	{
		CString TxtMess;
		TxtMess.LoadString(IDS_MESS_SEG_NONSEL); 
		AfxMessageBox(TxtMess, MB_ICONEXCLAMATION | MB_OK);
		CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR1_SOURCE);
		return;
	}

	CheckRadioButton(IDC_FD_BTNR1_SOURCE, IDC_FD_BTNR3_CIBLE, IDC_FD_BTNR3_CIBLE);
	m_pActivePage->ShowWindow(SW_HIDE);
	
	// Bouton infos seuil visible 
	m_btnInfoSeuils.ShowWindow(SW_SHOW);

	m_pActivePage = m_pdlgCible;

	// Récup id source d'audience
	((CDlgCible*)m_pActivePage)->m_IdSrcAudience = m_offre.m_IdSourceAudience;

	// Récup liste des idents terrains
	((CDlgCible*)m_pActivePage)->m_LstIdTerrains.Reset();
	for (m_offre.m_TBLIdTerrainsAudience.MoveFirst();  m_offre.m_TBLIdTerrainsAudience.IsValid(); m_offre.m_TBLIdTerrainsAudience.MoveNext())
	{	
		// Récup ident terrain
		JUnt32 TerID = m_offre.m_TBLIdTerrainsAudience.GetItem();
		((CDlgCible*)m_pActivePage)->m_LstIdTerrains.AddTail() = TerID;
	}

	// Vérifie les seuils via ces terrains
	m_pdlgCible->CheckSeuils();

	// Affichage Sources et Terrains en cours de sélection !!!!.....
	AfficheTitreFenetre();

	// Affiche la page active dans sa zone
	UpdateDlgPos(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Affichage informations Source et Terrain(s) en cours
void CDialogChoixOffre::AfficheTitreFenetre()
{
	CString TxtTitreOffre;
	CString TxtSourceTerrain = "";

	// Si au moins 1 source et 1 terrain, créer texte source + terrains
	if (m_offre.m_IdSourceAudience != 0 && m_offre.m_TBLIdTerrainsAudience.GetCount() > 0)
	{
		// Récup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// Récup le source
		const JSource * ptSrc		= srcTables.m_TBLSources.GetItem(m_offre.m_IdSourceAudience);

		if (m_offre.m_TBLIdTerrainsAudience.GetCount() == 1)
		{
			// 1 seul terrain on recherche le nom du terrain
			m_offre.m_TBLIdTerrainsAudience.MoveFirst();
			if (m_offre.m_TBLIdTerrainsAudience.IsValid())
			{
				// Ident etrrain en cours
				JUnt32 TerID = m_offre.m_TBLIdTerrainsAudience.GetItem();
				const JTerrain * ptTer	= srcTables.m_TBLTerrains.GetItem(m_offre.m_IdSourceAudience,TerID);

				// Et on crée la chaine "source + terrain"
				TxtSourceTerrain.Format("- %s - %s", ptSrc->m_Libelle.AsJCharPtr() , ptTer->m_Libelle.AsJCharPtr());
			}
			else
			{
				// Et on crée la chaine "source"
				TxtSourceTerrain.Format("- %s", ptSrc->m_Libelle.AsJCharPtr());
			}
		}
		else
		{
			// Plusieurs terrains, on affiche la source en indiquant +sieurs terrains
			TxtSourceTerrain.Format("- %s (%s)", ptSrc->m_Libelle.AsJCharPtr(), "Multi-terrains");
		}
	}

	// Puis on finalise le titre et on l'affiche en haut de la fenêtre principale de dialogue
	TxtTitreOffre.Format(IDS_FD_TITRE_OFFRE,TxtSourceTerrain);
	SetWindowText(TxtTitreOffre);
}

void CDialogChoixOffre::OnSize(UINT nType, int cx, int cy)
{
	ETSLayoutDialog::OnSize(nType, cx, cy);

	// Affiche la page active dans sa zone
	UpdateDlgPos();
}

void CDialogChoixOffre::UpdateDlgPos(BOOL b_show/*=FALSE*/) 
{
	//on affiche la page active dans sa zone
	CRect rect;
	
	if(GetDlgItem(IDC_FD_STATIC_FILLE))
	{
		GetDlgItem(IDC_FD_STATIC_FILLE)->GetWindowRect(&rect);
		POINT pt1, pt2;
		pt1.x = rect.left, pt1.y = rect.top;
		pt2.x = rect.right, pt2.y = rect.bottom;
		::ScreenToClient(m_hWnd, &pt1);
		::ScreenToClient(m_hWnd, &pt2);
		rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
	 
		if (m_pActivePage)
			m_pActivePage->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_FRAMECHANGED);

		if (b_show)
			m_pActivePage->ShowWindow(SW_SHOW);
	}
}



void CDialogChoixOffre::OnDestroy()
{
	//on détruit les fenetres filles
	m_pdlgSource->DestroyWindow();
	m_pdlgSupport->DestroyWindow();
	m_pdlgCible->DestroyWindow();
	
	delete m_pdlgSource;
	delete m_pdlgSupport;
	delete m_pdlgCible;

	ETSLayoutDialog::OnDestroy();
}

void CDialogChoixOffre::SetupControls()
{
//subclass 
	m_btnOK				.SubclassDlgItem(IDC_FD_OK,					this);
	m_btnCancel			.SubclassDlgItem(IDCANCEL,					this);
	
	m_btnImport			.SubclassDlgItem(IDC_FD_IMPORT,				this);
	m_btnSeuils			.SubclassDlgItem(IDC_FD_BTN_SEUILS,			this);
	m_btnInfoSeuils		.SubclassDlgItem(IDC_FD_BTN_INFO_SEUILS,	this);

	m_btnSource			.SubclassDlgItem(IDC_FD_BTNR1_SOURCE,		this);
	m_btnSupport		.SubclassDlgItem(IDC_FD_BTNR2_SUPPORT,		this);
	m_btnCible			.SubclassDlgItem(IDC_FD_BTNR3_CIBLE,		this);

	SetColor();
}

void CDialogChoixOffre::SetColor()
{
	m_btnOK			.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnCancel		.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnImport		.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnSeuils		.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnInfoSeuils	.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnSource		.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnSupport	.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

	m_btnCible		.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),m_appColor, 0 );

}

void CDialogChoixOffre::SetOffre(JATPOffre& offre)
{
	m_lstIDTitre.Reset();
	m_lstIDCouplage.Reset();
	m_lstIDLot.Reset();

	m_offre = offre;

	// Positionne les élmts supports sélectionnés
	for (m_offre.m_TBLIdSupports.MoveFirst(); m_offre.m_TBLIdSupports.IsValid(); m_offre.m_TBLIdSupports.MoveNext() )
	{
		JUnt32 id = m_offre.m_TBLIdSupports.GetItem();
		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem* ielem = pRegroupements->GetIElemByID(id);
		
		if (ielem == NULL)
		{
			m_offre.m_TBLIdSupports.Remove();
			continue;
		}

		
		if (ielem->IsCouplage())
		{
			m_lstIDCouplage.AddTail() = id;
			continue;
		}
		if (ielem->IsLot())
		{
			m_lstIDLot.AddTail() = id;
			continue;
		}
		//il s'agit d'un titre
		m_lstIDTitre.AddTail() = id;
	}

	// Récupére les élmts familles
	m_offre.m_MapSelFamilles.MoveFirst();
	while(m_offre.m_MapSelFamilles.IsValid())
	{
		// Récup id famille
		JUnt32 id = m_offre.m_MapSelFamilles.GetKey();

		// Vérifie que c'est bien une famille
		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem *pElem = pRegroupements->GetIElemByID(m_offre.m_MapSelFamilles.GetKey());
		if (pElem)
		{
			if (pElem->IsLot())
			{
				m_lstIDLot.AddTail() = id;
			}
		}
		m_offre.m_MapSelFamilles.MoveNext(); 
	}

	//Préselection de l'item ensemble
	if (m_offre.m_TBLIdItems.GetCount() == 0)
	{
		// m_offre.m_TBLIdItems.AddTail() = CItemsConstruits::GetInstance()->GetDefaultItemId();
		m_offre.m_TBLIdItems.AddTail() = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetDefaultItemId();
		m_preselcible = true;
	}
	if (m_offre.m_IdItemAffinite == 0)
	{
		// m_offre.m_IdItemAffinite = CItemsConstruits::GetInstance()->GetDefaultItemId();
		m_offre.m_IdItemAffinite = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetDefaultItemId();
		m_preselcible = true;
	}

	//on précharge les terrains s'il n'existent pas
	m_OffreTerrainManager->Preload(m_offre);

}

void CDialogChoixOffre::GetOffre(JATPOffre& offre)
{
	//on recupere tous les différents types de supports
	m_offre.m_TBLIdSupports.Reset();

	for (m_lstIDLot.MoveFirst(); m_lstIDLot.IsValid(); m_lstIDLot.MoveNext() )
	{
		// Attention aux doublons
		JUnt32 IdSupp = m_lstIDLot.GetItem();
		if (NewSupportTblSupp(m_offre.m_TBLIdSupports, IdSupp))
			m_offre.m_TBLIdSupports.AddTail() = m_lstIDLot.GetItem();
	}

	for (m_lstIDTitre.MoveFirst(); m_lstIDTitre.IsValid(); m_lstIDTitre.MoveNext() )
	{	
		// Attention aux doublons
		JUnt32 IdSupp = m_lstIDTitre.GetItem();
		if (NewSupportTblSupp(m_offre.m_TBLIdSupports, IdSupp))
			m_offre.m_TBLIdSupports.AddTail() = m_lstIDTitre.GetItem();
	}

	for (m_lstIDCouplage.MoveFirst(); m_lstIDCouplage.IsValid(); m_lstIDCouplage.MoveNext() )
	{
		// Attention aux doublons
		JUnt32 IdSupp = m_lstIDCouplage.GetItem();
		if (NewSupportTblSupp(m_offre.m_TBLIdSupports, IdSupp))
			m_offre.m_TBLIdSupports.AddTail() = m_lstIDCouplage.GetItem();
	}

	//on construit la table des libellés de supports
	m_offre.m_mapLblSupport.Reset();
	for (m_offre.m_TBLIdSupports.MoveFirst(); m_offre.m_TBLIdSupports.IsValid(); m_offre.m_TBLIdSupports.MoveNext())
	{
		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem* ielem = pRegroupements->GetIElemByID(m_offre.m_TBLIdSupports.GetItem());
		m_offre.m_mapLblSupport.Add(ielem->GetID()) = ielem->GetLabel();
	}
	//on construit la table des libellés d'items
	m_offre.m_mapLblItem.Reset();
	for (m_offre.m_TBLIdItems.MoveFirst(); m_offre.m_TBLIdItems.IsValid(); m_offre.m_TBLIdItems.MoveNext())
	{
		// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

		// Récup item correspondant à l'indice
		// IItem * pItem = pItemConst->GetIItemByID(m_offre.m_TBLIdItems.GetItem());
		IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_offre.m_TBLIdItems.GetItem());
		m_offre.m_mapLblItem.Add(pItem->GetID()) = pItem->GetLabel();
	}
	
	//enfin on rend l'offre
	offre = m_offre;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère offre restreinte aux terrains (sélection - Red Sélection)
void CDialogChoixOffre::GetOffreRestreinte(JATPOffre& offre)
{
	//on recupere tous les différents types de supports
	m_offre.m_TBLIdSupports.Reset();

	// Init map des familles utilisées
	m_offre.m_MapSelFamilles.Reset();

	for (m_lstIDLot.MoveFirst(); m_lstIDLot.IsValid(); m_lstIDLot.MoveNext() )
	{
		// Attention aux doublons
		JUnt32 IdSupp = m_lstIDLot.GetItem();
		if (NewSupportTblSupp(m_offre.m_TBLIdSupports, IdSupp))
		{
			// Ajoute id lot au tableau des supports
			m_offre.m_TBLIdSupports.AddTail() = m_lstIDLot.GetItem();

			// Puis ajoute au map des familles (lots)
			m_offre.m_MapSelFamilles.MoveTo(IdSupp);
			if (!m_offre.m_MapSelFamilles.IsValid())
			{	
				// Récupère les élmts regroupement
				CRegroupements * pRegroupements = CRegroupements::GetInstance();
				IElem* pelem = pRegroupements->GetIElemByID(IdSupp);

				if (pelem)
				{
					// Récup nom du regroupement
					const JLabel Label = pelem->GetLabel(); 

					// Créer les infos archive famille
					CInfoFamille InfoFamille;

					// Récupère les composants supports
					InfoFamille.m_LstIdSupport.Reset();
					JList <IElem*> ListSup;
					ILot * pLot = pelem->AsLot();
					if (pLot)
					{
						pLot->GetFils(ListSup);

						// On ajoute les supports valides uniquement
						for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
						{
							// Ident composant famille
							JUnt32 IdSupp = ListSup.GetItem()->GetID();

							/*
							// Test si composant valide sur couple <Source,Terrain>
							if (SupportValid(ListSup.GetItem()))
							{
								// Titre présent sur les terrains sélectionnés
								InfoFamille.m_LstIdSupport.AddTail()= ListSup.GetItem()->GetID();  
							}
							*/

							// De préférence, on ajoute tous les composants, même si non valide sur Source, Terrain
							InfoFamille.m_LstIdSupport.AddTail()= IdSupp;
						}
					}

					// Récupère nom du regroupement
					InfoFamille.m_NomFamille = Label.AsJCharPtr();  

					// Et on sauvegarde la famille dans l'offre
					m_offre.m_MapSelFamilles.Add(IdSupp) = InfoFamille;    
				}
			}
		}
	}

	for (m_lstIDTitre.MoveFirst(); m_lstIDTitre.IsValid(); m_lstIDTitre.MoveNext() )
	{
		// Attention aux doublons
		JUnt32 IdSupp = m_lstIDTitre.GetItem();
		if (NewSupportTblSupp(m_offre.m_TBLIdSupports, IdSupp))
			m_offre.m_TBLIdSupports.AddTail() = m_lstIDTitre.GetItem();
	}

	for (m_lstIDCouplage.MoveFirst(); m_lstIDCouplage.IsValid(); m_lstIDCouplage.MoveNext() )
	{
		// Attention aux doublons
		JUnt32 IdSupp = m_lstIDCouplage.GetItem();
		if (NewSupportTblSupp(m_offre.m_TBLIdSupports, IdSupp))
			m_offre.m_TBLIdSupports.AddTail() = m_lstIDCouplage.GetItem();
	}

	//on construit la table des libellés de supports
	m_offre.m_mapLblSupport.Reset();
	for (m_offre.m_TBLIdSupports.MoveFirst(); m_offre.m_TBLIdSupports.IsValid(); m_offre.m_TBLIdSupports.MoveNext())
	{
		// Récupération élmt associé au support
		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem* ielem = pRegroupements->GetIElemByID(m_offre.m_TBLIdSupports.GetItem());

		// Libellé du support
		m_offre.m_mapLblSupport.MoveTo(ielem->GetID());

		// Verif si item non déjà existant Alain
		if (!m_offre.m_mapLblSupport.IsValid())
			m_offre.m_mapLblSupport.Add(ielem->GetID()) = ielem->GetLabel();
	}
	//on construit la table des libellés d'items
	m_offre.m_mapLblItem.Reset();
	for (m_offre.m_TBLIdItems.MoveFirst(); m_offre.m_TBLIdItems.IsValid(); m_offre.m_TBLIdItems.MoveNext())
	{
		// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

		// Récup item correspondant à l'indice
		// IItem * pItem = pItemConst->GetIItemByID(m_offre.m_TBLIdItems.GetItem());
		IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_offre.m_TBLIdItems.GetItem());

		m_offre.m_mapLblItem.MoveTo(pItem->GetID());
		if (!m_offre.m_mapLblItem.IsValid())
		// Verif si item non déjà existant Alain
			m_offre.m_mapLblItem.Add(pItem->GetID()) = pItem->GetLabel();
	}

	//enfin on rend l'offre
	offre = m_offre;

	// on élague les supports n'appartenant pas aux terrains sélectionnés
	offre.m_TBLIdSupports.Reset() ; 
	for (m_offre.m_TBLIdSupports.MoveFirst();
		 m_offre.m_TBLIdSupports.IsValid();
		 m_offre.m_TBLIdSupports.MoveNext())
	{
		// Récupère Id support
		JUnt32 IdSupport = m_offre.m_TBLIdSupports.GetItem();

		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem* pelem = pRegroupements->GetIElemByID(IdSupport);

		// Traitement couplage
		if (pelem->IsCouplage())
		{	
			// Par défaut couplage valide sur la sélection des terrains
			JBool CouplageValid = true;

			//pour les couplages fixes
			if (pelem->IsCouplageFixe())
			{
				// Décoposition couplage en supports simples
				JCouplageFixe * cplfixe = pelem->AsCouplageFixe();
				JList<IElem*> lstID;
				cplfixe->GetFils(lstID);

				// Balayage des supports composant le couplage
				for (lstID.MoveFirst(); lstID.IsValid(); lstID.MoveNext() )
				{
					if (!SupportValid(lstID.GetItem()))
					{
						// Un des composant support non défini
						CouplageValid = false;

						// Pas la peine de continuer
						break;
					}
				}
			}

			// VOIR avec Jeremy si ça marche
			// pour les couplages libres
			else if (pelem->IsCouplageLibre())
			{
				CCouplageLibre * cpllibre = pelem->AsCouplageLibre();

				JList<IElem*> lstID;
				cpllibre->GetFils(lstID);

				// Balayage des supports composant le couplage
				for (lstID.MoveFirst(); lstID.IsValid(); lstID.MoveNext() )
				{
					if (!SupportValid(lstID.GetItem()))
					{
						// Un des composant support non défini
						CouplageValid = false;

						// Pas la peine de continuer
						break;
					}
				}
				/*
				IIterateur* it = cpllibre->CreateIterateur();
				CTitreCouplageLibre* tcpllibre = (CTitreCouplageLibre*)it->getHead();
				while (tcpllibre)
				{
					if (!SupportValid(tcpllibre->GetID()))
					{
						// Un des composant support non défini
						CouplageValid = false;

						// Pas la peine de continuer
						break;
					}

					// Passe au composant support suivant
					tcpllibre = (CTitreCouplageLibre*)it->getNext();
				}
				*/
			}

			// Si couplage défini sur tous les terrains, on l'ajoute
			if (CouplageValid)
			{
				// Attention aux doublons
				JUnt32 IdSupp = m_offre.m_TBLIdSupports.GetItem();
				if (NewSupportTblSupp(offre.m_TBLIdSupports, IdSupp))
					offre.m_TBLIdSupports.AddTail() = m_offre.m_TBLIdSupports.GetItem();  
			}
		}

		// Traitement des lots
		else if (pelem->IsLot())
		{
			// Crée liste des supports lot
			JList <IElem*> ListSup;
			ILot * pLot = pelem->AsLot();
			if (pLot)
			{
				pLot->GetFils(ListSup);

				// On ajoute les supports valides uniquement
				for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				{
					if (SupportValid(ListSup.GetItem()))
					{
						// Titre présent sur les terrains sélectionnés
						// Et attention aux doublons
						JUnt32 IdSupp = ListSup.GetItem()->GetID();
						if (NewSupportTblSupp(offre.m_TBLIdSupports,IdSupp))
							offre.m_TBLIdSupports.AddTail() = ListSup.GetItem()->GetID();  
					}
				}
			}
			
		}

		// Traitement support simple
		else
		{
			// Test si support défini sur les terrains
			if (SupportValid(pelem))
			{
				// Attention aux doublons -- Octobre 2006
				JUnt32 IdSupp = m_offre.m_TBLIdSupports.GetItem();
				if (NewSupportTblSupp(offre.m_TBLIdSupports,IdSupp))
					offre.m_TBLIdSupports.AddTail() = m_offre.m_TBLIdSupports.GetItem();  
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Vérifie que c'est bien un nouveau support à ajouter dans la table global des supports
JBool CDialogChoixOffre::NewSupportTblSupp(JList<JUnt32> &TblSupports, JUnt32 IdSupp)
{
	// Boucle sur tous les ids supports du tableau en cours
	for (TblSupports.MoveFirst(); TblSupports.IsValid(); TblSupports.MoveNext())  
	{
		// Récupère l'ident support courant
		JUnt32 IdSuppCur = TblSupports.GetItem();
		if (IdSupp == IdSuppCur)
			return false;
	}

	// Pas trouvé, c'est un nouveau support
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Validité support via les terrains sélectionnés
JBool CDialogChoixOffre::SupportValid(IElem* pelem)
{
	// Element support non défini, on ne le prend pas 
	if (pelem == NULL) return false;

	// Audience support
	int IDAudience = 0;
	if (pelem->IsTitre())
	{
		IDAudience = pelem->AsITitre()->GetIDAudience();

		// Vérifie appartenance aux terrains sélectionnés
		CKeyTerrain k;
		k.m_IDSource = m_offre.m_IdSourceAudience;

		// Boucle sur les terrains
		for (m_offre.m_TBLIdTerrainsAudience.MoveFirst();
				m_offre.m_TBLIdTerrainsAudience.IsValid();
				m_offre.m_TBLIdTerrainsAudience.MoveNext() )
		{
			k.m_IDTerrain = m_offre.m_TBLIdTerrainsAudience.GetItem();

			CTerrain * pTerrain = NULL;
			pTerrain = m_OffreTerrainManager->GetTerrain(k);
			
			// Ajoute le support dans liste des supports de l'offre en cours
			if (pTerrain != NULL)
			{
				for(m_offre.m_TBLIdSegmentsAudience.MoveFirst();
					m_offre.m_TBLIdSegmentsAudience.IsValid();
					m_offre.m_TBLIdSegmentsAudience.MoveNext())
				{
					JInt32 idSegment = m_offre.m_TBLIdSegmentsAudience.GetItem();
					if (pTerrain->SupportBelongsToSegment( IDAudience, idSegment ))
						return true;
				}
			}
		}
		// Support non défini
		return false;
	}
	else
		// pour tous les autres cas
		return true;
}

void CDialogChoixOffre::OnCancel()
{
	ETSLayoutDialog::OnCancel();
}

void CDialogChoixOffre::EnableOK()
{
	SetIco();
	if (m_pdlgSource->IsComplete() && m_pdlgSupport->IsComplete() && m_pdlgCible->IsComplete())
		m_btnOK.EnableWindow();	
	else
		m_btnOK.EnableWindow(false);	
}

void CDialogChoixOffre::SetIco()
{
	if (m_pdlgSource->IsComplete())
		m_btnSource.SetBitmaps(IDB_FD_LEDVERT, RGB(0,128,128));
	else
		m_btnSource.SetBitmaps(IDB_FD_LEDROUGE, RGB(0,128,128));

	if (m_pdlgSupport->IsComplete())
		m_btnSupport.SetBitmaps(IDB_FD_LEDVERT, RGB(0,128,128));
	else
		m_btnSupport.SetBitmaps(IDB_FD_LEDROUGE, RGB(0,128,128));

	if (m_pdlgCible->IsComplete())
		m_btnCible.SetBitmaps(IDB_FD_LEDVERT, RGB(0,128,128));
	else
		m_btnCible.SetBitmaps(IDB_FD_LEDROUGE, RGB(0,128,128));
	//presel
	if(m_preselcible)
		m_btnCible.SetBitmaps(IDB_FD_LEDORANGE, RGB(0,128,128));
}

///////////////////////////////////////////////////////////////////////////////////
// Identification des élmts limite (via les seuils / coloré dans la liste)
void CDialogChoixOffre::OnBnClickedBtnSeuils()
{
	((CDlgCible*)m_pActivePage)->CheckSeuils();
	m_btnSeuils.EnableWindow(false);	
}

///////////////////////////////////////////////////////////////////////////////////
// Affichage des seuils des enquêtes sélectionnés
void CDialogChoixOffre::OnBnClickedBtnInfoSeuils()
{
	// Via module cible, affiche les informations seuils enqu^tes
	((CDlgCible*)m_pActivePage)->AffichSeuils();
}

void CDialogChoixOffre::RazSeuils()
{
	m_pdlgCible->ResetMapParams();
	m_btnSeuils.EnableWindow();
}

void CDialogChoixOffre::CheckItems()
{
	//contrôle des items
	bool bRem = false;
	CString msg;
	m_offre.m_TBLIdItems.MoveFirst();

	while (m_offre.m_TBLIdItems.IsValid())
	{
		// Récupère le Item de l'offre
		bRem = TestItem(m_offre.m_TBLIdItems.GetItem(), msg);
		if(bRem)
		{
			m_offre.m_TBLIdItems.Remove(-1);
			m_offre.m_TBLIdItems.MoveFirst();
		}
		else
			m_offre.m_TBLIdItems.MoveNext();
	}

	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

	// No Items loaded, on reselectionne l'item ensemble
	if(!m_offre.m_TBLIdItems.GetCount())
		// m_offre.m_TBLIdItems.AddTail() = pItemConst->GetDefaultItemId();
		m_offre.m_TBLIdItems.AddTail() = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetDefaultItemId();

	//on teste l'affinité
	if(TestItem(m_offre.m_IdItemAffinite, msg))
	{
		bRem = true;
		//on reselectionne l'item ensemble
		// m_offre.m_IdItemAffinite = pItemConst->GetDefaultItemId();
		m_offre.m_IdItemAffinite = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetDefaultItemId();
	}

	//on a retiré au moins 1 item
	if (bRem)
	{
		msg.FormatMessage(IDS_FD_ERRITEM, msg);
		::AfxMessageBox(msg, MB_ICONEXCLAMATION | MB_OK);
	}

	// pItemConst->Release();

	// Mise à jour validité des cibles via les terrains sélectionnés
	CWnd *pWnd = m_pdlgCible->GetWnd();
	if (pWnd->m_hWnd != NULL)
	{
		m_pdlgCible->CheckSeuils(); 
	}
}

bool CDialogChoixOffre::TestItem(JUnt32 IdItem, CString &txt)
{
	// Récup Table des items
	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

	// Récup item correspondant à l'indice
	// IItem * pItem = pItemConst->GetIItemByID(IdItem);
	IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdItem);

	bool bRem = false;
	if(pItem)//on ne soucie pas de palette ou item construit (générique)
	{
		CKeyTerrain key;
		key.SetSource(m_offre.m_IdSourceAudience);
		//on teste l'item pour chaque terrain, invalide on sort de la boucle
		for(m_offre.m_TBLIdTerrainsAudience.MoveFirst(); m_offre.m_TBLIdTerrainsAudience.IsValid() && bRem == false; m_offre.m_TBLIdTerrainsAudience.MoveNext())
		{
			key.SetTerrain(m_offre.m_TBLIdTerrainsAudience.GetItem());
			CTerrain * terrain = m_OffreTerrainManager->GetTerrain(key);
			if( ! pItem->IsValid(terrain))
			{
				//ajout du libellé de l'item pour le message d'erreur
				txt.Append(pItem->GetLabel().AsJCharPtr());
				txt += "\r\n";
				bRem = true;
			}
		}
		//test sur la source de ciblage
	}

	// pItemConst->Release();
	
	return bRem;
}

void CDialogChoixOffre::CheckSupports( std::list<JUnt32> & lAddedTerrains ) const 
{
	// si on vient d'ajouter un terrain à la selection
	if (lAddedTerrains.empty() == false)
	{
		CString sName;
		std::map<JUnt32, CString> mapSupports;
		for(m_lstIDTitre.MoveFirst();
			m_lstIDTitre.IsValid();
			m_lstIDTitre.MoveNext())
		{
			if (TestSupport(m_lstIDTitre.GetItem(), lAddedTerrains, sName) == false)
			{
				// on ajoute à la map tous les couples 'id support'/'nom support'
				mapSupports.insert(std::make_pair(m_lstIDTitre.GetItem(), sName));
			}
		}

		// on a retiré au moins 1 item
		if (mapSupports.empty() == false)
		{
			// on cherche le nom de la source
			const JSource * ptSrc	= JSRCTables::GetInstance().m_TBLSources.GetItem(m_offre.m_IdSourceAudience);

			// Et on crée la chaine "source + terrain"
			CString sSrcTerrains;
			std::list<JUnt32>::iterator ter_it = lAddedTerrains.begin();
			while (ter_it != lAddedTerrains.end())
			{
				if (ter_it != lAddedTerrains.begin()) sSrcTerrains.Append(", ");

				// on cherche le nom du terrain
				const JTerrain * ptTer	= JSRCTables::GetInstance().m_TBLTerrains.GetItem(m_offre.m_IdSourceAudience, *ter_it);
				sSrcTerrains.AppendFormat("%s - %s", ptSrc->m_Libelle.AsJCharPtr() , ptTer->m_Libelle.AsJCharPtr());
				++ter_it;
			}

			// on crée la chaine contenant tous les supports
			CString sSupports;
			std::map<JUnt32, CString>::iterator map_it = mapSupports.begin();
			while (map_it != mapSupports.end())
			{
				sSupports.AppendFormat("%s\r\n", map_it->second);
				++map_it;
			}

			CString sUserMsg;
			if (lAddedTerrains.size() == 1)
				sUserMsg.Format("Les supports suivants n'existent pas dans le dernier terrain sélectionné (%s):\r\n", sSrcTerrains);
			else
				sUserMsg.Format("Les supports suivants n'existent pas dans les derniers terrains sélectionnés (%s):\r\n", sSrcTerrains);

			DlgSupportsInexistants dlg(sUserMsg, sSupports);
			dlg.DoModal();
		}
	}
}
bool CDialogChoixOffre::TestSupport( JUnt32 IdSupport, const std::list<JUnt32> & lAddedTerrains, CString &txt ) const
{
	CKeyTerrain key;
	key.SetSource(m_offre.m_IdSourceAudience);

	//on teste le support pour chaque terrain, si invalide on sort de la boucle
	CRegroupements*	pRegroupements = CRegroupements::GetInstance();
	IElem * elem = pRegroupements->GetIElemByID(IdSupport);

	if (elem != NULL)
	{
		std::list<JUnt32>::const_iterator ter_id;
		for (ter_id = lAddedTerrains.begin(); ter_id != lAddedTerrains.end(); ++ter_id)
		{
			key.SetTerrain(*ter_id);	
			CTerrain * terrain = m_OffreTerrainManager->GetTerrain(key);

			if (!terrain->HasSupport( elem->AsITitre()->GetIDAudience()) )
			{
				txt.Format("%s", elem->GetLabel().AsJCharPtr());
				return false;
			}
		}
	}

	return true;
}

void CDialogChoixOffre::OnBnClickedBtnImport()
{
	JMFCStream* stream = NULL;

	CString szFilters;
	szFilters.LoadString(IDS_FD_FILTER);
	DWORD flags = OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ;
	CFileDialog fileDlg(TRUE, "ofr", 0, flags, szFilters, this);

	// Affichage de la liste des plateaux existants
	JATPDocChoixSupport ATPDocChoixSupport;
	CString path;
	if( fileDlg.DoModal() == IDOK )
		path = fileDlg.GetPathName();
	else
		return;

	// On se positionne sur le répertoire User
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));

	try
	{
		// Récupération n° de version
		JUnt32 NoVersion = 0;
		CFile filetmp(path, CFile::modeRead);

		// Ouvre le fichier en mode sérialize
		CArchive ar(&filetmp, CArchive::load);
		stream = JMFCStream::Create(ar);

		// Récup n° de version
		ATPDocChoixSupport.RecvNoVersion(*stream, NoVersion);

		// Ferme l'archive
		ar.Close();

		// Ferme le fichier
		filetmp.Close(); 

		// Selon le N° de version on ouvre en CFile ou StdioFile (pour récupérer les anciens fichiers)
		JMFCStream* stream = NULL;

		if (NoVersion <= 7)
		{
			CStdioFile file(path, CFile::modeRead);
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);
			ATPDocChoixSupport.Recv(*stream);
		}
		else
		{
			// Attention sinon problème sur certains codes (type idterrain > = 282)
			CFile file(path, CFile::modeRead);

			// Le reste reste identique
			CArchive ar(&file, CArchive::load);
			stream = JMFCStream::Create(ar);
			ATPDocChoixSupport.Recv(*stream);
		}

	}
	catch(...)
	{
	}


	if (stream)
		delete stream;

	if (ATPDocChoixSupport.IsValid())
	{
		JATPOffre offre;
		offre = ATPDocChoixSupport.m_Offre;

		offre.m_TBLIdSupports.Reset();
		offre.m_MapSelFamilles.Reset();

		for(ATPDocChoixSupport.m_MapIdSupportsSel.MoveFirst();
			ATPDocChoixSupport.m_MapIdSupportsSel.IsValid();
			ATPDocChoixSupport.m_MapIdSupportsSel.MoveNext())
		{
			if(ATPDocChoixSupport.m_MapIdSupportsSel.GetItem() == true)
			{
				// Attention aux doublons
				JUnt32 IdSupp = ATPDocChoixSupport.m_MapIdSupportsSel.GetKey();

				// Attention si c'est une famille
				if (IdSupp < 100000)
				{
					if (NewSupportTblSupp(offre.m_TBLIdSupports, IdSupp))
						offre.m_TBLIdSupports.AddTail() = ATPDocChoixSupport.m_MapIdSupportsSel.GetKey();
				}
				else
				{
					ATPDocChoixSupport.m_Offre.m_MapSelFamilles.MoveTo(IdSupp);   
					// if (!ATPDocChoixSupport.m_Offre.m_MapSelFamilles.IsValid())
					if (ATPDocChoixSupport.m_Offre.m_MapSelFamilles.IsValid())
					{
						// Récupère les élmts regroupement
						CRegroupements * pRegroupements = CRegroupements::GetInstance();
						IElem* pelem = pRegroupements->GetIElemByID(IdSupp);

						if (pelem)
						{
							// Nom famille
							CInfoFamille InfoFamille;
							InfoFamille.m_NomFamille = pelem->GetLabel().AsJCharPtr(); 

							// Récupère les composants supports
							InfoFamille.m_LstIdSupport.Reset();
							JList <IElem*> ListSup;
							ILot * pLot = pelem->AsLot();
							if (pLot)
							{
								pLot->GetFils(ListSup);

								// On ajoute les supports valides uniquement
								for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
								{
									// Ident composant famille
									JUnt32 IdSuppFam = ListSup.GetItem()->GetID();

									// De préférence, on ajoute tous les composants, même si non valide sur Source, Terrain
									InfoFamille.m_LstIdSupport.AddTail()= IdSuppFam;
								}
							}

							// Ajoute la famille
							offre.m_MapSelFamilles.Add(IdSupp) = InfoFamille;
						}
					}
				}
			}
		}

		// Positionne l'offre récupérée
		SetOffre(offre);

		m_pdlgSource->Init();
		m_pdlgSupport->Init();
		m_pdlgCible->Init();
		m_preselcible = false;
		EnableOK();
	}
}



