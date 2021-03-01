// SiteSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SiteSelectionDlg.h"
#include "SearchAlgo.h"
#include ".\siteselectiondlg.h"
#include "ItemsConstruitsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SiteSelectionDlg dialog

IMPLEMENT_DYNAMIC(CSiteSelectionDlg, CDialog)
CSiteSelectionDlg::CSiteSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSiteSelectionDlg::IDD, pParent)
{
	// Par défaut pas de sites déjà sélectionnés
	m_LstSiteSel.Reset(); 
}

CSiteSelectionDlg::~CSiteSelectionDlg()
{
	for(m_ImpMap.MoveFirst(); m_ImpMap.IsValid(); m_ImpMap.MoveNext())
	{
		CItemConstruit * pItem = m_ImpMap.GetItem();
		if(pItem && !pItem->IsRegistered())
			delete pItem;
	}
}

void CSiteSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSiteSelectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_AN_EDIT_SEARCH, OnEnChangeAnEditSearch)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_AN_ALL_SITES, OnBnClickedAnAllSites)
	ON_BN_CLICKED(IDC_AN_NOSEL_SITE, OnBnClickedAnNoSelSite)
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_AN_LIST_SITES, OnLbnSelchangeAnListSites)
END_MESSAGE_MAP()


// CSiteSelectionDlg message handlers

void CSiteSelectionDlg::OnBnClickedOk()
{
	m_SelectedItems.Reset();
	m_List_Sites.GetSelectedID(m_SelectedItems);
	OnOK();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération des sites sélectionnés
void CSiteSelectionDlg::GetSelectedItems(JList<CItemWrapper *> & Items)
{
	CString Str;
	Str.LoadString(IDS_AN_SITES); 
	for(m_SelectedItems.MoveFirst(); m_SelectedItems.IsValid(); m_SelectedItems.MoveNext())
	{
		m_ImpMap.MoveTo(m_SelectedItems.GetItem());
		if(m_ImpMap.IsValid())
		{
			CItemWrapper * pWrapper = new CItemWrapper();
			pWrapper->SetItemConstruit(m_ImpMap.GetItem());
			pWrapper->SetHeader(Str);
			Items.AddTail() = pWrapper;
			m_ImpMap.Remove(); 
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Positionnement des sites sélectionnés
void CSiteSelectionDlg::SetSiteSel(JList <JLabel> & LstSiteSel)
{
	// Récupère la liste des sites précédemment séelctionnés
	m_LstSiteSel.Swap(LstSiteSel); 
}

/*
void CSiteSelectionDlg::SetSelectedItems(JList<CItemWrapper *> & Lst_Items)
{
	// Init liste des sites sélectionnés
	m_SelectedItems.Reset();

	// Attention si rien de sélectionné	
	if (Lst_Items.GetCount() > 0) 
	{
		// alaye tous les élmts de la liste
		for(Lst_Items.MoveFirst(); Lst_Items.IsValid(); Lst_Items.MoveNext())
		{
			// Récupére ident site sélectionné
			CItemWrapper * pItemWrapper = Lst_Items.GetItem();
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 
			m_SelectedItems.AddTail() = pItemConstruit->GetID(); 
		}
	}

}
*/

///////////////////////////////////////////////////////////////////////////////////
// Sélection des sites 
BOOL CSiteSelectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupControls();
	CString strPath = AfxGetApp()->GetProfileString("Data", "SourcesPath", 0);
	strPath.Append(_T("\\Sites.tbl"));

	// Test si fichier sites existe
	DWORD RetValue = GetFileAttributes(strPath);

	// Test si on est sur un tarif user
	if (RetValue == INVALID_FILE_ATTRIBUTES) return FALSE;

	CStdioFile File;
	CFileException Error;
	if(!File.Open(strPath, CFile::modeRead|CFile::typeText, &Error))
	{
		EndDialog(0);
		return true;
	}

	// Map des items sites
	JMap<JInt32, const IItem *> ImpMap;

	// Import the selected targets
	// if(!CItemsConstruits::GetInstance()->Import(File, ImpMap))
	if(!CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Import(File, ImpMap))
		return false;
	
	// Test si terrain en cours valide
	if (m_pTerrainTemoin)
	{
		// Init Map des sites sélectionnés
		m_MapSiteSel.Reset(); 

		// Chargement de tous les items sites valides sur le terrain témoin
		for(ImpMap.MoveFirst(); ImpMap.IsValid(); ImpMap.MoveNext())
		{
			// Récupère l'item
			CItemConstruit * pItem = (CItemConstruit *)ImpMap.GetItem()->AsItemConstruit();

			// Test si valide sur le terrain
			if (pItem->IsValid(m_pTerrainTemoin))
			{
				// Valide >> on l'ajoute au map des sites sélectionnables
				m_ImpMap.Add(pItem->GetID()) = pItem;

				// Info map sélection pour ce site
				m_MapSiteSel.Add(pItem->GetID()) = false;  
			}
		}
	}
	else
		// Terrain témoin non défini
		AfxMessageBox("Attention aucun terrain défini",MB_ICONINFORMATION);

	// Affiche le nombre de sites
	CString Txt;
	Txt.LoadString(IDS_AN_SITES);
	Txt.Format("%s (%d)",Txt,m_ImpMap.GetCount()); 
	m_ttlSite.SetWindowText(Txt); 

	// Mise à jour sélection sur la liste visualisée
	/*
	this->ShowWindow(SW_SHOW); 
	m_List_Sites.RedrawWindow(); 
	m_List_Sites.Invalidate(); 
	m_List_Sites.SetSelectedID(m_SelectedItems);
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSiteSelectionDlg::SetupControls()
{
	m_bkgSite.			SubclassDlgItem(IDC_AN_STATIC_SITE,		this);
	m_ttlSite.			SubclassDlgItem(IDC_AN_STATIC_TTLSITE,	this);
	m_But_Ok. 			SubclassDlgItem(IDOK,					this);
	m_But_Cancel. 		SubclassDlgItem(IDCANCEL,				this);
	m_AllSites.			SubclassDlgItem(IDC_AN_ALL_SITES,		this);
	m_NoSite.			SubclassDlgItem(IDC_AN_NOSEL_SITE,		this);

	m_List_Sites.		SubclassDlgItem(IDC_AN_LIST_SITES,		this);

	// Boite de recherche texte
	m_EditSearch.		SubclassDlgItem(IDC_AN_EDIT_SEARCH,		this); 

	m_bkgSite.SetTransparent(false);
	m_bkgSite.SetSunken(true);

	m_bkgSite.AddTopWindow(m_ttlSite);
	m_bkgSite.AddTopWindow(m_List_Sites);
	m_bkgSite.AddTopWindow(m_EditSearch);

	CString Txt;

	Txt.LoadString(IDS_AN_SITES);
	m_ttlSite.SetFontName(CATPColors::GetFontLabelName());
	m_ttlSite.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlSite.SetBorder(false);
	m_ttlSite.SetWindowText(Txt); 

	m_EditSearch.				EnableWindow(TRUE);

	// Set the windows colors
	SetColors();
}

void CSiteSelectionDlg::SetColors()
{
	m_bkgSite.		SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::MEDIUM_DEGRAD);
	m_ttlSite.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_ttlSite.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_List_Sites.	SetSelectionColor(NORMAL,CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));
}

// Vérifie la boite de recherche, via timer
void CSiteSelectionDlg::OnTimer(UINT nIDEvent)
{
	KillTimer(IdTimer_Search);

	// Sauve les sélections en cours
	SaveSelCur();

	// Chargement de la liste des sites
	m_List_Sites.ResetContent(); 

	// Libellé de recherche
	CString SearchTxt;
	m_EditSearch.GetWindowText(SearchTxt); 

	// Décompose libellé de recherche, en lots de texte à rechercher (on vire le ;
	JList <CString>  LstEditSearch;
	LstEditSearch.Reset();

	// Boucle sur la totalité du texte
	JInt32 Pos;
	while (SearchTxt != "")
	{
		// Voir si séparateur
		Pos = SearchTxt.Find(";");

		if (Pos > 0)
		{
			// Récupère libellé à gauche
			LstEditSearch.AddTail() = SearchTxt.Left(Pos);

			// Continue avec reste du libellé à droite
			SearchTxt = SearchTxt.Mid(Pos+1);
		}
		else
		{
			// Pas de séparateur, on prend la dernier texte et on sort
			LstEditSearch.AddTail() = SearchTxt;
			SearchTxt = "";
		}
	}

	// Garde uniquement les sites via le critère alpha de recherche
	JMap <CString, JUnt32> MapSiteOK;
	MapSiteOK.Reset();
	for(m_ImpMap.MoveFirst(); m_ImpMap.IsValid(); m_ImpMap.MoveNext())
	{
		CItemConstruit * pItem = m_ImpMap.GetItem();

		// si plusieurs chaines de recherche, on teste sites avec toutes ces chaines
		if (LstEditSearch.GetCount() > 0)
		{
			for (LstEditSearch.MoveFirst(); LstEditSearch.IsValid(); LstEditSearch.MoveNext())
			{
				// Récup chaine à tester
				CString SearchTxt = LstEditSearch.GetItem();

				// Ajoute site avec ce texte
				CString TxtSite = pItem->GetLabel().AsJCharPtr();
				if (CSearchAlgo::Contains(pItem->GetLabel().AsJCharPtr(), SearchTxt))
				{
					// Vérifie site non présent dans la liste des sites à conserver (évite les doublons)
					MapSiteOK.MoveTo(TxtSite);
					if (!MapSiteOK.IsValid())
					{
						// Nouveau site à conserver, on l'ajoute à la liste
						m_List_Sites.AddItem(pItem);

						// et on le marque, comme déjà ajouté à la liste
						MapSiteOK.Add(TxtSite) = 0;
					}
				}
			}
		}
		else
			// Tous les sites sont à afficher
			m_List_Sites.AddItem(pItem);
	}

	// La liste des sites est complète, on marque les sites précédemment sélectionnés
	MarqueSelPrevious();
	
	// Après réaffichage des sites filtrés, on affiche la précédente sélection
	RecupSelCur();

	// Mise à jour sélection sur la liste visualisée
	m_List_Sites.SetSelectedID(m_SelectedItems);

	// Affichage informations sélection sites
	AffichInfoSite();
	
	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
// Affichage des informations sur la sélection des sites
void CSiteSelectionDlg::AffichInfoSite()
{
	// Affiche le nombre de sites
	CString TxtTitre = "";
	CString Txt = "";
	Txt.LoadString(IDS_AN_SITES);

	// Nombre de sélection
	JUnt32 NbSel = GetNbTotSel();

	// Nombre de sélection visible
	JList<JInt32> LstSel;
	m_List_Sites.GetSelectedID(LstSel);
	JUnt32 NbSites = m_List_Sites.GetCount();
	JUnt32 NbSelVisible = LstSel.GetCount();

	if (m_List_Sites.GetCount() == m_ImpMap.GetCount())
	{
		// Tous les sites
		if (NbSel == 0)
			// TxtTitre.Format("%s (%d)",Txt, m_ImpMap.GetCount()); 
			TxtTitre.Format("%s",Txt); 
		else
			TxtTitre.Format("%s (%d / %d)",Txt,NbSelVisible,NbSel); 
	}
	else
	{	if (NbSel == 0)
			// Une partie des sites
			// TxtTitre.Format("%s (%d%s%d)",Txt,m_List_Sites.GetCount()," / ", m_ImpMap.GetCount()); 
			TxtTitre.Format("%s",Txt); 
		else
			// Une partie des sites avec nb sélection
			// TxtTitre.Format("%s (%d% / %d - Sel (%d))",Txt,m_List_Sites.GetCount(), m_ImpMap.GetCount(), NbSel); 
			TxtTitre.Format("%s (%d / %d)",Txt,NbSelVisible,NbSel); 
	}

	// Affiche info nb sites et redessine la fenêtre contrôle
	m_ttlSite.SetWindowText(TxtTitre); 
	m_ttlSite.RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////////
// Récupération nombre total de sélection
JUnt32 CSiteSelectionDlg::GetNbTotSel()
{
	// Init tot sélection
	JUnt32 NbTotSel = 0;

	// Balaye tous les sites, et vérifie leur état
	for (m_MapSiteSel.MoveFirst();  m_MapSiteSel.IsValid(); m_MapSiteSel.MoveNext())
	{
		// Etat sélection du site
		JBool &EtatSel = m_MapSiteSel.GetItem();

		// Incrément nn sélection si valide
		if (EtatSel) NbTotSel++;
	}

	// Retourne le nombre de sélection
	return NbTotSel;
}	


///////////////////////////////////////////////////////////////////////////////////
// Sauve et récupère les sélections en cours
void CSiteSelectionDlg::SaveSelCur()
{
	// Liste sélection en cours
	m_SelectedItems.Reset();
	m_List_Sites.GetSelectedID(m_SelectedItems);

	// Mise à jour du map
	for(m_SelectedItems.MoveFirst(); m_SelectedItems.IsValid(); m_SelectedItems.MoveNext())
	{
		// Ident du site
		JInt32 IdSite = m_SelectedItems.GetItem();

		// Mise à jour flag sélection
		m_MapSiteSel.MoveTo(IdSite);
		if (m_MapSiteSel.IsValid())
		{
			// Mise à jour sélection
			JBool &Select = m_MapSiteSel.GetItem();  
			Select = true;
		}
		else
		{
			// On l'ajoute en indiquant qu'il est sélectionné
			m_MapSiteSel.Add(IdSite) = true; 
		}
	}

}

void CSiteSelectionDlg::RecupSelCur()
{
	// Init sélection en cours
	m_SelectedItems.Reset();

	// Mise à jour du map
	for(m_MapSiteSel.MoveFirst(); m_MapSiteSel.IsValid(); m_MapSiteSel.MoveNext())
	{
		// Ident site 
		JInt32 IdSite = m_MapSiteSel.GetKey();

		// Test si site sélectionné
		if (m_MapSiteSel.GetItem() == true)
		{
			// Ajoute à la liste sélection
			m_SelectedItems.AddTail() = IdSite;
		}
	}
}

// Mise à jour des sites sélectionnés via précédente sélection
void CSiteSelectionDlg::MarqueSelPrevious()
{
	// Voir si liste des élmts présélectionnés non vide
	if (m_LstSiteSel.GetCount() > 0 && m_List_Sites.GetCount() > 0)
	{
		// Balaye la liste des élmts présélectionnés
		for (m_LstSiteSel.MoveFirst(); m_LstSiteSel.IsValid(); m_LstSiteSel.MoveNext())  
		{
			// Label du site présélectionné
			JLabel LabelSiteSel = m_LstSiteSel.GetItem(); 

			// Récupère les sites précédemment sélectionnés
			for (JUnt32 InxItem = 0; InxItem < m_List_Sites.GetCount(); InxItem ++)
			{
				// Récup elmt de la liste courante
				CItemConstruit *pItemConstruit = (CItemConstruit *) m_List_Sites.GetItemData(InxItem); 

				// Label item liste
				JLabel LabelSite = pItemConstruit->GetLabel();

				// Voir si sélectionné précédemment
				if (LabelSite == LabelSiteSel)
				{
					// Récup Id Site
					JInt32 IdSite = pItemConstruit->GetID(); 

					// Mise à jour flag sélection
					m_MapSiteSel.MoveTo(IdSite);
					if (m_MapSiteSel.IsValid())
					{
						// Mise à jour sélection
						JBool &Select = m_MapSiteSel.GetItem();  
						Select = true;
					}
					else
					{
						// On l'ajoute en indiquant qu'il est sélectionné
						m_MapSiteSel.Add(IdSite) = true; 
					}
				}
			}
		}

		// on vide la liste
		m_LstSiteSel.Reset();
	}
}

// Infos boite de recherche change
void CSiteSelectionDlg::OnEnChangeAnEditSearch()
{
	SetTimer(IdTimer_Search, 750, NULL);
}

/////////////////////////////////////////////////////////////////////////////////
// Sélection de tous les sites
void CSiteSelectionDlg::OnBnClickedAnAllSites()
{
	// Chargement de la liste des sites
	// m_List_Sites.ResetContent(); 

	// Libellé de recherche
	m_EditSearch.SetWindowText(""); 
	m_EditSearch.SetFocus();

}

/////////////////////////////////////////////////////////////////////////////////
// Aucun site de sélectionné  
void CSiteSelectionDlg::OnBnClickedAnNoSelSite()
{
	// Libellé de recherche impossible, virera tous les sites
	m_SelectedItems.Reset();
	m_List_Sites.SetSelectedID(m_SelectedItems);

	// Mise à jour info sléection site
	MajInfoSite();
}

void CSiteSelectionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// Init texte de recherche
	m_EditSearch.SetWindowText("");
	m_EditSearch.SetFocus();

}	

//////////////////////////////////////////////////////////////////////////////
// Positionne le terrain témoin
void CSiteSelectionDlg::SetTerrainTemoin(CTerrain *pTerrain)
{
	// Positionnement du terrain témoin courant
	m_pTerrainTemoin	=	pTerrain;
}

///////////////////////////////////////////////////////////////////////////////////
// Changement sélection sites
void CSiteSelectionDlg::OnLbnSelchangeAnListSites()
{
	// Mise à jour info sléection site
	MajInfoSite();
}

///////////////////////////////////////////////////////////////////////////////////
// Mise à jour info site
void CSiteSelectionDlg::MajInfoSite()
{
	// Balaye tous les sites de la liste en cours
	for (JUnt32 InxLst = 0; InxLst < m_List_Sites.GetCount(); InxLst++)
	{
		// Etat sélection
		int EtatSelElmt = m_List_Sites.GetSel(InxLst);

		// Récupère item
		CItemConstruit *pItemConstruit = (CItemConstruit *)m_List_Sites.GetItemDataPtr(InxLst);

		// Ident item
		JInt32 IdItem = pItemConstruit->GetID(); 

		// Mise à jour map sélection
        m_MapSiteSel.MoveTo(IdItem);

		// Etat sélection du site
		JBool &EtatSel = m_MapSiteSel.GetItem();
		if (EtatSelElmt == 0)
			EtatSel = false;
		else
			EtatSel = true;
	}

	// Réaffichage des informations sites
	AffichInfoSite();	
}
