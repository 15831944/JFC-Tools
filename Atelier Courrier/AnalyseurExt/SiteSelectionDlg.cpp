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
	// Par d�faut pas de sites d�j� s�lectionn�s
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
// R�cup�ration des sites s�lectionn�s
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
// Positionnement des sites s�lectionn�s
void CSiteSelectionDlg::SetSiteSel(JList <JLabel> & LstSiteSel)
{
	// R�cup�re la liste des sites pr�c�demment s�elctionn�s
	m_LstSiteSel.Swap(LstSiteSel); 
}

/*
void CSiteSelectionDlg::SetSelectedItems(JList<CItemWrapper *> & Lst_Items)
{
	// Init liste des sites s�lectionn�s
	m_SelectedItems.Reset();

	// Attention si rien de s�lectionn�	
	if (Lst_Items.GetCount() > 0) 
	{
		// alaye tous les �lmts de la liste
		for(Lst_Items.MoveFirst(); Lst_Items.IsValid(); Lst_Items.MoveNext())
		{
			// R�cup�re ident site s�lectionn�
			CItemWrapper * pItemWrapper = Lst_Items.GetItem();
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 
			m_SelectedItems.AddTail() = pItemConstruit->GetID(); 
		}
	}

}
*/

///////////////////////////////////////////////////////////////////////////////////
// S�lection des sites 
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
		// Init Map des sites s�lectionn�s
		m_MapSiteSel.Reset(); 

		// Chargement de tous les items sites valides sur le terrain t�moin
		for(ImpMap.MoveFirst(); ImpMap.IsValid(); ImpMap.MoveNext())
		{
			// R�cup�re l'item
			CItemConstruit * pItem = (CItemConstruit *)ImpMap.GetItem()->AsItemConstruit();

			// Test si valide sur le terrain
			if (pItem->IsValid(m_pTerrainTemoin))
			{
				// Valide >> on l'ajoute au map des sites s�lectionnables
				m_ImpMap.Add(pItem->GetID()) = pItem;

				// Info map s�lection pour ce site
				m_MapSiteSel.Add(pItem->GetID()) = false;  
			}
		}
	}
	else
		// Terrain t�moin non d�fini
		AfxMessageBox("Attention aucun terrain d�fini",MB_ICONINFORMATION);

	// Affiche le nombre de sites
	CString Txt;
	Txt.LoadString(IDS_AN_SITES);
	Txt.Format("%s (%d)",Txt,m_ImpMap.GetCount()); 
	m_ttlSite.SetWindowText(Txt); 

	// Mise � jour s�lection sur la liste visualis�e
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

// V�rifie la boite de recherche, via timer
void CSiteSelectionDlg::OnTimer(UINT nIDEvent)
{
	KillTimer(IdTimer_Search);

	// Sauve les s�lections en cours
	SaveSelCur();

	// Chargement de la liste des sites
	m_List_Sites.ResetContent(); 

	// Libell� de recherche
	CString SearchTxt;
	m_EditSearch.GetWindowText(SearchTxt); 

	// D�compose libell� de recherche, en lots de texte � rechercher (on vire le ;
	JList <CString>  LstEditSearch;
	LstEditSearch.Reset();

	// Boucle sur la totalit� du texte
	JInt32 Pos;
	while (SearchTxt != "")
	{
		// Voir si s�parateur
		Pos = SearchTxt.Find(";");

		if (Pos > 0)
		{
			// R�cup�re libell� � gauche
			LstEditSearch.AddTail() = SearchTxt.Left(Pos);

			// Continue avec reste du libell� � droite
			SearchTxt = SearchTxt.Mid(Pos+1);
		}
		else
		{
			// Pas de s�parateur, on prend la dernier texte et on sort
			LstEditSearch.AddTail() = SearchTxt;
			SearchTxt = "";
		}
	}

	// Garde uniquement les sites via le crit�re alpha de recherche
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
				// R�cup chaine � tester
				CString SearchTxt = LstEditSearch.GetItem();

				// Ajoute site avec ce texte
				CString TxtSite = pItem->GetLabel().AsJCharPtr();
				if (CSearchAlgo::Contains(pItem->GetLabel().AsJCharPtr(), SearchTxt))
				{
					// V�rifie site non pr�sent dans la liste des sites � conserver (�vite les doublons)
					MapSiteOK.MoveTo(TxtSite);
					if (!MapSiteOK.IsValid())
					{
						// Nouveau site � conserver, on l'ajoute � la liste
						m_List_Sites.AddItem(pItem);

						// et on le marque, comme d�j� ajout� � la liste
						MapSiteOK.Add(TxtSite) = 0;
					}
				}
			}
		}
		else
			// Tous les sites sont � afficher
			m_List_Sites.AddItem(pItem);
	}

	// La liste des sites est compl�te, on marque les sites pr�c�demment s�lectionn�s
	MarqueSelPrevious();
	
	// Apr�s r�affichage des sites filtr�s, on affiche la pr�c�dente s�lection
	RecupSelCur();

	// Mise � jour s�lection sur la liste visualis�e
	m_List_Sites.SetSelectedID(m_SelectedItems);

	// Affichage informations s�lection sites
	AffichInfoSite();
	
	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////
// Affichage des informations sur la s�lection des sites
void CSiteSelectionDlg::AffichInfoSite()
{
	// Affiche le nombre de sites
	CString TxtTitre = "";
	CString Txt = "";
	Txt.LoadString(IDS_AN_SITES);

	// Nombre de s�lection
	JUnt32 NbSel = GetNbTotSel();

	// Nombre de s�lection visible
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
			// Une partie des sites avec nb s�lection
			// TxtTitre.Format("%s (%d% / %d - Sel (%d))",Txt,m_List_Sites.GetCount(), m_ImpMap.GetCount(), NbSel); 
			TxtTitre.Format("%s (%d / %d)",Txt,NbSelVisible,NbSel); 
	}

	// Affiche info nb sites et redessine la fen�tre contr�le
	m_ttlSite.SetWindowText(TxtTitre); 
	m_ttlSite.RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////////
// R�cup�ration nombre total de s�lection
JUnt32 CSiteSelectionDlg::GetNbTotSel()
{
	// Init tot s�lection
	JUnt32 NbTotSel = 0;

	// Balaye tous les sites, et v�rifie leur �tat
	for (m_MapSiteSel.MoveFirst();  m_MapSiteSel.IsValid(); m_MapSiteSel.MoveNext())
	{
		// Etat s�lection du site
		JBool &EtatSel = m_MapSiteSel.GetItem();

		// Incr�ment nn s�lection si valide
		if (EtatSel) NbTotSel++;
	}

	// Retourne le nombre de s�lection
	return NbTotSel;
}	


///////////////////////////////////////////////////////////////////////////////////
// Sauve et r�cup�re les s�lections en cours
void CSiteSelectionDlg::SaveSelCur()
{
	// Liste s�lection en cours
	m_SelectedItems.Reset();
	m_List_Sites.GetSelectedID(m_SelectedItems);

	// Mise � jour du map
	for(m_SelectedItems.MoveFirst(); m_SelectedItems.IsValid(); m_SelectedItems.MoveNext())
	{
		// Ident du site
		JInt32 IdSite = m_SelectedItems.GetItem();

		// Mise � jour flag s�lection
		m_MapSiteSel.MoveTo(IdSite);
		if (m_MapSiteSel.IsValid())
		{
			// Mise � jour s�lection
			JBool &Select = m_MapSiteSel.GetItem();  
			Select = true;
		}
		else
		{
			// On l'ajoute en indiquant qu'il est s�lectionn�
			m_MapSiteSel.Add(IdSite) = true; 
		}
	}

}

void CSiteSelectionDlg::RecupSelCur()
{
	// Init s�lection en cours
	m_SelectedItems.Reset();

	// Mise � jour du map
	for(m_MapSiteSel.MoveFirst(); m_MapSiteSel.IsValid(); m_MapSiteSel.MoveNext())
	{
		// Ident site 
		JInt32 IdSite = m_MapSiteSel.GetKey();

		// Test si site s�lectionn�
		if (m_MapSiteSel.GetItem() == true)
		{
			// Ajoute � la liste s�lection
			m_SelectedItems.AddTail() = IdSite;
		}
	}
}

// Mise � jour des sites s�lectionn�s via pr�c�dente s�lection
void CSiteSelectionDlg::MarqueSelPrevious()
{
	// Voir si liste des �lmts pr�s�lectionn�s non vide
	if (m_LstSiteSel.GetCount() > 0 && m_List_Sites.GetCount() > 0)
	{
		// Balaye la liste des �lmts pr�s�lectionn�s
		for (m_LstSiteSel.MoveFirst(); m_LstSiteSel.IsValid(); m_LstSiteSel.MoveNext())  
		{
			// Label du site pr�s�lectionn�
			JLabel LabelSiteSel = m_LstSiteSel.GetItem(); 

			// R�cup�re les sites pr�c�demment s�lectionn�s
			for (JUnt32 InxItem = 0; InxItem < m_List_Sites.GetCount(); InxItem ++)
			{
				// R�cup elmt de la liste courante
				CItemConstruit *pItemConstruit = (CItemConstruit *) m_List_Sites.GetItemData(InxItem); 

				// Label item liste
				JLabel LabelSite = pItemConstruit->GetLabel();

				// Voir si s�lectionn� pr�c�demment
				if (LabelSite == LabelSiteSel)
				{
					// R�cup Id Site
					JInt32 IdSite = pItemConstruit->GetID(); 

					// Mise � jour flag s�lection
					m_MapSiteSel.MoveTo(IdSite);
					if (m_MapSiteSel.IsValid())
					{
						// Mise � jour s�lection
						JBool &Select = m_MapSiteSel.GetItem();  
						Select = true;
					}
					else
					{
						// On l'ajoute en indiquant qu'il est s�lectionn�
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
// S�lection de tous les sites
void CSiteSelectionDlg::OnBnClickedAnAllSites()
{
	// Chargement de la liste des sites
	// m_List_Sites.ResetContent(); 

	// Libell� de recherche
	m_EditSearch.SetWindowText(""); 
	m_EditSearch.SetFocus();

}

/////////////////////////////////////////////////////////////////////////////////
// Aucun site de s�lectionn�  
void CSiteSelectionDlg::OnBnClickedAnNoSelSite()
{
	// Libell� de recherche impossible, virera tous les sites
	m_SelectedItems.Reset();
	m_List_Sites.SetSelectedID(m_SelectedItems);

	// Mise � jour info sl�ection site
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
// Positionne le terrain t�moin
void CSiteSelectionDlg::SetTerrainTemoin(CTerrain *pTerrain)
{
	// Positionnement du terrain t�moin courant
	m_pTerrainTemoin	=	pTerrain;
}

///////////////////////////////////////////////////////////////////////////////////
// Changement s�lection sites
void CSiteSelectionDlg::OnLbnSelchangeAnListSites()
{
	// Mise � jour info sl�ection site
	MajInfoSite();
}

///////////////////////////////////////////////////////////////////////////////////
// Mise � jour info site
void CSiteSelectionDlg::MajInfoSite()
{
	// Balaye tous les sites de la liste en cours
	for (JUnt32 InxLst = 0; InxLst < m_List_Sites.GetCount(); InxLst++)
	{
		// Etat s�lection
		int EtatSelElmt = m_List_Sites.GetSel(InxLst);

		// R�cup�re item
		CItemConstruit *pItemConstruit = (CItemConstruit *)m_List_Sites.GetItemDataPtr(InxLst);

		// Ident item
		JInt32 IdItem = pItemConstruit->GetID(); 

		// Mise � jour map s�lection
        m_MapSiteSel.MoveTo(IdItem);

		// Etat s�lection du site
		JBool &EtatSel = m_MapSiteSel.GetItem();
		if (EtatSelElmt == 0)
			EtatSel = false;
		else
			EtatSel = true;
	}

	// R�affichage des informations sites
	AffichInfoSite();	
}
