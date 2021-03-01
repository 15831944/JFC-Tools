// DlgApparie.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "..\Resource.h"
#include "DlgApparie.h"


// Boîte de dialogue CDlgApparie

IMPLEMENT_DYNAMIC(CDlgApparie, CDlgFCP)

CDlgApparie::CDlgApparie(CWnd* pParent /*=NULL*/)
{
	m_AfficheApparentes = false;
	m_MediaWeight = 100;
	m_IDTAEdited = 0;
	m_plstTitre = new TitreInXListBox();
	m_pTerrain = NULL;
	m_pTitresApp = NULL;
	m_selIDSource = CB_ERR;
	m_selIDTerrain = CB_ERR;
	m_selTitreHorsID = CB_ERR;
	m_pTitreTemoin = NULL;
	m_pTitreHors = NULL;
	m_srcTables = (JSRCTables*)& JSRCTables::GetInstance();
}

CDlgApparie::~CDlgApparie()
{
	delete m_plstTitre;
	if (m_pTerrain)
	{
		m_pTitresApp->Save();
		//delete m_pTerrain;
		CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);
	}
}

void CDlgApparie::DoDataExchange(CDataExchange* pDX)
{
	CDlgFCP::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RS_LISTAPPARIE, m_lstTitreApparie);
	DDX_Control(pDX, IDC_RS_LISTTITREHORS, m_lstTitreHors);
	DDX_Control(pDX, IDC_RS_COMBOSOURCE, m_ComboSource);
	DDX_Control(pDX, IDC_RS_COMBOTERRAIN,m_ComboTerrain);
	DDX_Text(pDX, IDC_RS_EDITMW, m_MediaWeight);
}


BEGIN_MESSAGE_MAP(CDlgApparie, CDlgFCP)
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_RS_CTXLST_SUPPR, OnCtxSuppr)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITRE, OnLbnSelchangeListTitre)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITREHORS, OnLbnSelchangeListTitreHors)
	ON_CBN_SELCHANGE(IDC_RS_COMBOSOURCE, OnCbnSelchangeComboSource)
	ON_CBN_SELCHANGE(IDC_RS_COMBOTERRAIN, OnCbnSelchangeComboTerrain)
	ON_BN_CLICKED(IDC_RS_BTNVALID, OnBtnValider)
	ON_BN_CLICKED(IDC_RS_BTNEFFSAS, OnBtnEffSas)
	ON_BN_CLICKED(IDC_RS_BTNGENESE, OnBtnGenese)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgCouplage

BOOL CDlgApparie::OnInitDialog()
{
	CDlgFCP::OnInitDialog();

	SetWindowRelations();
	SetupControls();
	FillComboSource();
	
	UpdateData();
	return 	TRUE;
}

void CDlgApparie::FillListTitreApparie()
{
	m_lstTitreApparie.ResetContent();
	if (m_pTerrain && m_pTitresApp)
	{
		for (m_pTitresApp->MoveFirst();m_pTitresApp->IsValid();m_pTitresApp->MoveNext())
		{
			CTitreApparie* titreapp = (CTitreApparie*)m_pTitresApp->GetItem() ;
			m_lstTitreApparie.AddItem (titreapp);
		}
	}

}

void CDlgApparie::FillListTitre()
{
	m_plstTitre->ResetContent();
	// si un terrain est chargé
	if (m_pTerrain)
	{
		//les titres des familles
		int nCount = m_lstFamille.GetSelCount();
		//si aucune famille n'est selectionnée
		/*
		if (nCount == 0)
		{
			for (int i=0; i < m_lstFamille.GetCount(); i++)
			{
				CFamille* famillesel = (CFamille*) m_lstFamille.GetItemData(i);
				IIterateur* it = famillesel->CreateIterateur();
				INode * filstmp = it->getHead();
				while (filstmp)
				{
					if (filstmp->IsTitre() 
						&& TestPeriodicites( filstmp->GetID() )
						&& TestApparente( filstmp->GetID() ) 
						&& 	m_pTerrain->HasSupport(filstmp->AsITitre()->GetIDAudience()) )
						m_plstTitre->AddItem ( filstmp );
					filstmp = it->getNext();
				}
				delete it;
			}
		}
		else//*/
		//sinon on n'affiche que les titres des familles sélectionnées
		{
			CArray<int,int> arayListBoxSel;
			arayListBoxSel.SetSize(nCount);
			m_lstFamille.GetSelItems(nCount, arayListBoxSel.GetData());
			for (int i=0; i < arayListBoxSel.GetCount(); i++)
			{
				CFamille* famillesel = (CFamille*) m_lstFamille.GetItemData(arayListBoxSel[i]);
				IIterateur* it = famillesel->CreateIterateur();
				INode * filstmp = it->getHead();
				while (filstmp)
				{
					if ( filstmp->IsTitre() 
						&& TestPeriodicite( filstmp->GetID() )
						&& TestApparente( filstmp->GetID() ) 
						&& m_pTerrain->HasSupport( filstmp->AsITitre()->GetIDAudience() )
						)
						m_plstTitre->AddItem ( filstmp );
					filstmp = it->getNext();
				}
				delete it;
			}
		}
		
		
		//les titres des centres
		nCount = m_lstCentre.GetSelCount();
		//si aucun n'est selectionné
		if (nCount == 0)
		{
			for (int i=0; i < m_lstCentre.GetCount(); i++)
			{
				CCentre* centresel = (CCentre*) m_lstCentre.GetItemData(i);
				IIterateur* it = centresel->CreateIterateur();
				INode * titretmp = it->getHead();
				while (titretmp)
				{
					//tester la periodicité
					if ( TestPeriodicite( titretmp->GetID() )
						&& TestApparente( titretmp->GetID() )
						&& 	m_pTerrain->HasSupport( titretmp->AsITitre()->GetIDAudience() )
						)
							m_plstTitre->AddItem ( titretmp );

					titretmp = it->getNext();
				}
				delete it;
			}
		}
		else
		{
			CArray<int,int> arayListBoxSel;
			arayListBoxSel.SetSize(nCount);
			m_lstCentre.GetSelItems(nCount, arayListBoxSel.GetData());
			for (int i=0; i < arayListBoxSel.GetCount(); i++)
			{
				CCentre* centresel = (CCentre*) m_lstCentre.GetItemData(arayListBoxSel[i]);
				IIterateur* it = centresel->CreateIterateur();
				INode * titretmp = it->getHead();
				while (titretmp)
				{
					if ( TestPeriodicite( titretmp->GetID() ) 
						&& TestApparente( titretmp->GetID() ) 
						&& 	m_pTerrain->HasSupport( titretmp->AsITitre()->GetIDAudience() )
						)
							m_plstTitre->AddItem ( titretmp );
					titretmp = it->getNext();
				}
				delete it;
			}
		}

		//si la famille titres non attribué est sélectionnée
		if( m_lstFamille.GetSel(0) )
		{
			for ( m_pRegroupements->GetTitresNA()->MoveFirst(); m_pRegroupements->GetTitresNA()->IsValid(); m_pRegroupements->GetTitresNA()->MoveNext())
			{
				CTitre* titre = m_pRegroupements->GetTitresNA()->GetItem();
				if ( TestPeriodicite( titre->GetID() )
					&& 	m_pTerrain->HasSupport( titre->GetIDAudience() ) )
						m_plstTitre->AddItem (titre);
			}
		}

		//On resélectionnne (pas de pb si l'id n'existe plus)
		m_plstTitre->SetSelectedID(m_selTitreID);
	}
	FillListTitreHors();
}

void CDlgApparie::FillListTitreHors()
{
	m_lstTitreHors.ResetContent();
	// si un terrain est chargé
	if ( m_pTerrain )
	{
		for ( m_srcpresse->m_TBLTitres.MoveFirst(); m_srcpresse->m_TBLTitres.IsValid(); m_srcpresse->m_TBLTitres.MoveNext())
		{
			IElem* titre = m_srcpresse->m_TBLTitres.GetItem();
			if ( TestPeriodicite( titre->GetID() )
				&& ! m_pTerrain->HasSupport( titre->AsITitre()->GetIDAudience() ) )
					m_lstTitreHors.AddItem (titre);
		}

		//On resélectionnne (pas de pb si l'id n'existe plus)
		m_lstTitreHors.SetSelectedID(m_selTitreHorsID);
	}

}
void CDlgApparie::OnContextMenu(CWnd *pWnd, CPoint point)
{
	CMenu menu;
	CPoint pt;
	pt = point;
	ScreenToClient(&pt);
	menu.LoadMenu(IDR_RS_MENUCTXTREE);

	if ( pWnd->GetSafeHwnd() == m_lstTitreApparie.GetSafeHwnd() )
		menu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDlgApparie::OnCtxSuppr()
{
	CTitreApparie * titreapp = NULL;
	int sel = m_lstTitreApparie.GetCurSel();
	if (sel != LB_ERR)
	{
		titreapp = (CTitreApparie*)m_lstTitreApparie.GetItemData( sel );
	    m_pTitresApp->MoveTo(titreapp->GetID());
	    m_pTitresApp->Remove();

		m_plstTitre->m_mapIDRef.MoveTo(titreapp->GetTitreTemoin());
		if (m_plstTitre->m_mapIDRef.IsValid())
			m_plstTitre->m_mapIDRef.GetItem()--;
		m_plstTitre->Invalidate();

		m_lstTitreHors.m_mapIDRef.MoveTo(titreapp->GetIDTitre());
		if (m_lstTitreHors.m_mapIDRef.IsValid())
			m_lstTitreHors.m_mapIDRef.GetItem()--;
		m_lstTitreHors.Invalidate();

		delete titreapp;

		FillListTitreApparie();
	}
	OnBtnEffSas();
}

void CDlgApparie::FillComboSource()
{
	m_ComboSource.ResetContent();

	for(m_srcTables->m_TBLSources.MoveFirst(); m_srcTables->m_TBLSources.IsValid(); m_srcTables->m_TBLSources.MoveNext())
	{
		const JSource & Src =  m_srcTables->m_TBLSources.GetItem();
		int It = m_ComboSource.AddString(Src.m_Libelle.AsJCharPtr());
		m_ComboSource.SetItemData(It, Src.m_IdSource );
	}
	//on préselectionne l'AEPM
	m_ComboSource.SelectString(0,"AEPM");
	OnCbnSelchangeComboSource();
	//on préselectionne le terrain le + récent
	//int idMax = 0; int idx = 0;
	//for (int i=0; i<m_ComboTerrain.GetCount(); i++)
	//{
	//	int idtmp = (int)m_ComboTerrain.GetItemData(i);
	//	if (idtmp>idMax)
	//	{
	//		idMax = idtmp;
	//		idx = i;
	//	}
	//}
	//m_ComboTerrain.SetCurSel(idx);
	//OnCbnSelchangeComboTerrain();
}

void CDlgApparie::FillComboTerrain(int idSource)
{
	m_ComboTerrain.ResetContent();
	if(idSource != CB_ERR)
	{
		for(m_srcTables->m_TBLTerrains.MoveFirst(); m_srcTables->m_TBLTerrains.IsValid(); m_srcTables->m_TBLTerrains.MoveNext())
		{
			const JTerrain & Terr = m_srcTables->m_TBLTerrains.GetItem();
			if( Terr.m_IdSource == idSource )
			{
				int It = m_ComboTerrain.AddString(Terr.m_Libelle.AsJCharPtr());
				m_ComboTerrain.SetItemData(It, Terr.m_IdTerrain );
				//appliquer la sélection
				if ( Terr.m_IdTerrain ==  m_selIDTerrain)
				{
					m_ComboTerrain.SetCurSel(It);
				}
		
			}
		}
	}
}

void CDlgApparie::OnCbnSelchangeComboSource()
{
	int sel = m_ComboSource.GetCurSel();
	if (sel !=CB_ERR)
	{
		JInt32 idSource = (JInt32)m_ComboSource.GetItemData(sel);
		//si on a changé de source
		if (idSource != m_selIDSource )
		{
			if (m_pTerrain)
			{
				//sauver les TA
				m_pTitresApp->Save();
				m_pTitresApp = NULL;
				CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);
				//delete m_pTerrain;
				m_pTerrain = NULL;
				m_selIDTerrain = CB_ERR;
			}
			FillComboTerrain( idSource );
			m_selIDSource = idSource;
			FillListTitre();
			FillListTitreHors();
			FillListTitreApparie();
			OnBtnEffSas();
		}
	}
}

void CDlgApparie::OnCbnSelchangeComboTerrain()
{
	CTerrainManager* pterrainManager = CTerrainManager::GetInstance();
	int sel = m_ComboTerrain.GetCurSel();
	if (sel !=CB_ERR)
	{
		JInt32 idTerrain = (JInt32)m_ComboTerrain.GetItemData(sel);
		//si on a changé de terrain
		if( idTerrain != m_selIDTerrain)
		{
			CWaitCursor Wait;
		
			if (m_pTerrain)
			{
				//sauve les TA
				m_pTitresApp->Save();
				m_pTitresApp = NULL;
				
				pterrainManager->ReleaseTerrain(m_pTerrain);
				//m_pTerrain = NULL;
			}
			
			
			//m_pTerrain = new CTerrain();
			//JTerrain* Ter = (JTerrain*)m_srcpresse->m_TBLTerrains.GetItem(m_selIDSource, idTerrain);
			//JStringEx Name;
			//Name.Append(Ter->m_Chemin.AsJCharPtr());
			//
			//m_pTerrain->Load(Name);
			//Load titres app
			m_pTerrain = pterrainManager->GetTerrain(m_selIDSource, idTerrain);
			m_pTitresApp = m_pRegroupements->GetTitresApparies(m_selIDSource, idTerrain);

			SetAttrib();

			FillListTitre();
			FillListTitreHors();
			FillListTitreApparie();
			OnBtnEffSas();
			m_selIDTerrain = idTerrain;
		}
	}
}


void CDlgApparie::OnLbnSelchangeListTitre()
{
	int sel = m_plstTitre->GetCurSel();
	if (sel != LB_ERR)
	{
		m_pTitreTemoin = (CTitre*) m_plstTitre->GetItemData( sel );
		m_TitreTemoin.SetText( m_pTitreTemoin->GetLabel().AsJCharPtr() );
		m_lstTitreApparie.SetCurSel(-1);
		UpdateData(FALSE);
	}
	m_plstTitre->SetCurSel(LB_ERR);
}

void CDlgApparie::OnLbnSelchangeListTitreHors()
{
	int sel = m_lstTitreHors.GetCurSel();
	if (sel != LB_ERR)
	{
		if (m_IDTAEdited)
			OnBtnEffSas();
		m_pTitreHors = (CTitre*) m_lstTitreHors.GetItemData( sel );
		m_TitreApparie.SetText( m_pTitreHors->GetLabel().AsJCharPtr() );
		UpdateData(FALSE);
	}
	m_lstTitreHors.SetCurSel(LB_ERR);
}


void CDlgApparie::OnBtnEffSas()
{
	m_MediaWeight = 100;
	m_TitreTemoin.SetText("");
	m_TitreApparie.SetText("");
	UpdateData(FALSE);
	m_plstTitre->SetCurSel(-1);
	m_lstTitreHors.SetCurSel(-1);
	m_lstTitreApparie.SetCurSel(-1);
	m_IDTAEdited = 0;
	m_pTitreTemoin = NULL;
	m_pTitreHors = NULL;
}

void CDlgApparie::OnBtnValider()
{
	UpdateData();
	if ( 1>m_MediaWeight || m_MediaWeight>500)
	{
		AfxMessageBox( IDS_RS_MWOUTBOUNDS, MB_OK | MB_ICONINFORMATION);
		return;
	}

	if ( m_pTitreTemoin && m_pTitreHors )
	{
		CTitreApparie* titreapp = NULL;
		//update
		if (m_IDTAEdited)
		{
			m_pTitresApp->MoveTo(m_IDTAEdited);
			titreapp = m_pTitresApp->GetItem();
			m_pTitresApp->Remove();
			delete titreapp;
		}
		srand( (unsigned)time( NULL ) );
		titreapp = new CTitreApparie( IDTITREAPPARIE_MIN + rand(), m_pTitreHors->GetID(),
			m_pTitreTemoin->GetParentID(), m_pTitreTemoin->GetID(), m_MediaWeight );
		
		//ajout
		m_pTitresApp->Add(titreapp->GetID(), titreapp);


		m_plstTitre->m_mapIDRef.MoveTo(titreapp->GetTitreTemoin());
		if (m_plstTitre->m_mapIDRef.IsValid())
			m_plstTitre->m_mapIDRef.GetItem()++;
		else
			m_plstTitre->m_mapIDRef.Add(titreapp->GetTitreTemoin()) = 1;
		m_plstTitre->Invalidate();

		m_lstTitreHors.m_mapIDRef.MoveTo(titreapp->GetIDTitre());
		if (m_lstTitreHors.m_mapIDRef.IsValid())
			m_lstTitreHors.m_mapIDRef.GetItem()++;
		else
			m_lstTitreHors.m_mapIDRef.Add(titreapp->GetIDTitre()) = 1;
		m_lstTitreHors.Invalidate();

		FillListTitreApparie();
		OnBtnEffContainer();
	}
}

void CDlgApparie::OnBtnGenese()
{
	CTitreApparie * titreapp = NULL;
	int sel = m_lstTitreApparie.GetCurSel();
	if (sel != LB_ERR)
	{
		titreapp = (CTitreApparie*)m_lstTitreApparie.GetItemData( sel );
		//On rempli les zones d'édition correspondant au titre
		m_MediaWeight = titreapp->GetMediaWeight();
		
		m_pTitreHors = (CTitre*)m_pRegroupements->GetIElemByID(titreapp->GetIDTitre());
		UpdateData(FALSE);
		m_IDTAEdited = titreapp->GetID();

	    m_pTitreTemoin = (CTitre*)m_pRegroupements->GetSupportByID( titreapp->GetTitreTemoin() );
		m_TitreTemoin.SetText( m_pTitreTemoin->GetLabel().AsJCharPtr() );
		
		m_TitreApparie.SetText( m_srcpresse->m_TBLTitres.GetLabelByID(m_pTitreHors->GetID()) );
		//recupération du centre
	
		JUnt32 centreID = titreapp->GetIDCentre();
		INode * pere = m_pRegroupements->GetParentByID(centreID);
		if (pere)
		{
			//centre
			if (IDCENTRE_MIN < pere->GetID() && pere->GetID()< IDCENTRE_MAX)
			{
				//on sélectionne la famille
				m_selFamilleID.AddTail() = pere->GetParentID();
				//on selectionne le centre
				m_selCentreID.AddTail() = pere->GetID();
			}
			//famille
			if (IDFAMILLE_MIN < pere->GetID() && pere->GetID()< IDFAMILLE_MAX)//on sélectionne la famille
				m_selFamilleID.AddTail() = pere->GetID();
		}
		else
			m_selFamilleID.AddTail() = 0;
		//on selectionne la périodicité
		m_selPeriodID.AddTail() = m_srcpresse->m_TBLTitres.GetPeriodByID(m_pTitreTemoin->GetID());
		m_selPeriodID.AddTail() = m_srcpresse->m_TBLTitres.GetPeriodByID(m_pTitreHors->GetID());
	
		RefreshList();
	}
}

void CDlgApparie::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDlgFCP::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		FillListTitre();
		//m_TitreTemoin.SetText("");
		//int sel = m_plstTitre->GetCurSel();
		//if (sel != LB_ERR)
		//{
		//	CTitre * titre = (CTitre*) m_plstTitre->GetItemData( sel );
		//	m_TitreTemoin.SetText( titre->GetLabel().AsJCharPtr() );
		//}
		//m_TitreApparie.SetText("");
		//sel = m_lstTitreHors.GetCurSel();
		//if (sel != LB_ERR)
		//{
		//	CTitre * titre = (CTitre*) m_lstTitreHors.GetItemData( sel );
		//	m_TitreApparie.SetText( titre->GetLabel().AsJCharPtr() );
		//}
	}
	else
	{
		m_selTitreHorsID = m_lstTitreHors.GetSelectedID();
		OnBtnEffSas();
	}

}

void CDlgApparie::SetAttrib()
{
	if (m_pTitresApp)
	{
		m_plstTitre->m_mapIDRef.Reset();
		m_lstTitreHors.m_mapIDRef.Reset();
		for (m_pTitresApp->MoveFirst(); m_pTitresApp->IsValid(); m_pTitresApp->MoveNext())
		{
			CTitreApparie *titreApp = m_pTitresApp->GetItem();
			m_plstTitre->m_mapIDRef.MoveTo(titreApp->GetTitreTemoin());
			if (m_plstTitre->m_mapIDRef.IsValid())
				m_plstTitre->m_mapIDRef.GetItem()++;
			else
				m_plstTitre->m_mapIDRef.Add(titreApp->GetTitreTemoin()) = 1;

			m_lstTitreHors.m_mapIDRef.MoveTo(titreApp->GetIDTitre());
			if (m_lstTitreHors.m_mapIDRef.IsValid())
				m_lstTitreHors.m_mapIDRef.GetItem()++;
			else
				m_lstTitreHors.m_mapIDRef.Add(titreApp->GetIDTitre()) = 1;
		}

	}
}



void CDlgApparie::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
////
	{IDC_RS_STATICTITREHORS,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_RS_STATICTTLTITREHORS,	{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eTop}}, //Bottom side

			
	{IDC_RS_LISTTITREHORS,		{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICTITREHORS, CResizer::eBottom}}, //Bottom side


///	
	{IDC_RS_STATICTTLTTEMOIN,{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTTLCUSTOM,	{CResizer::eFixed, IDC_RS_STATICTTLTTEMOIN, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	
	{IDC_RS_STATICTXTTITRE,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTXTNOM,	{CResizer::eFixed, IDC_RS_STATICTXTTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_EDITMW,			{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	
	{IDC_RS_LISTAPPARIE,		{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eBottom}}, //Bottom side
////
	{IDC_RS_STATICCOMBOS,{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eRight}, //Left side
					{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
					{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
					{CResizer::eFixed, IDC_MAIN, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTTLSOURCE,{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eLeft}, //Left side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eTop},  //Top side
					{CResizer::eProportional, IDC_RS_STATICCOMBOS, CResizer::eRight}, //Right side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eTop}}, //Bottom side
	{IDC_RS_STATICTTLTERRAIN,{CResizer::eFixed, IDC_RS_STATICTTLSOURCE, CResizer::eRight}, //Left side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eTop},  //Top side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eRight}, //Right side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eTop}}, //Bottom side

	{IDC_RS_COMBOSOURCE,{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eLeft}, //Left side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eTop},  //Top side
					{CResizer::eProportional, IDC_RS_STATICCOMBOS, CResizer::eRight}, //Right side
					{CResizer::eHeight, IDC_RS_STATICCOMBOS, CResizer::eTop}}, //Bottom side
	{IDC_RS_COMBOTERRAIN,{CResizer::eFixed, IDC_RS_COMBOSOURCE, CResizer::eRight}, //Left side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eTop},  //Top side
					{CResizer::eFixed, IDC_RS_STATICCOMBOS, CResizer::eRight}, //Right side
					{CResizer::eHeight, IDC_RS_STATICCOMBOS, CResizer::eTop}}, //Bottom side

};
	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Add(s_bi, nSize);
}

void CDlgApparie::SetupControls()
{
	m_plstTitre->SubclassDlgItem(IDC_RS_LISTTITRE, this);

	m_staticTitreHors.SubclassDlgItem(IDC_RS_STATICTITREHORS, this);
	m_lblTtlTitreHors.SubclassDlgItem(IDC_RS_STATICTTLTITREHORS, this);
	m_lblTtlTTemoin.SubclassDlgItem(IDC_RS_STATICTTLTTEMOIN, this);
	m_TitreTemoin.SubclassDlgItem(IDC_RS_STATICTXTTITRE, this);
	m_TitreApparie.SubclassDlgItem(IDC_RS_STATICTXTNOM, this);

	m_staticCombo.SubclassDlgItem(IDC_RS_STATICCOMBOS, this);
	m_lblTtlSource.SubclassDlgItem(IDC_RS_STATICTTLSOURCE, this);
	m_lblTtlTerrain.SubclassDlgItem(IDC_RS_STATICTTLTERRAIN, this);

	m_staticCustom.AddTopWindow(m_lblTtlTTemoin);
	m_staticCustom.AddTopWindow(m_TitreTemoin);
	m_staticCustom.AddTopWindow(m_TitreApparie);
	m_staticCustom.AddTopWindow(GetDlgItem(IDC_RS_EDITMW)->GetSafeHwnd());
	m_staticCustom.AddTopWindow(m_lstTitreApparie);

	m_staticTitreHors.SetTransparent(false);
	m_staticTitreHors.SetSunken(true);
	m_staticTitreHors.AddTopWindow(m_lblTtlTitreHors);
	m_staticTitreHors.AddTopWindow(m_lstTitreHors);

	m_staticCombo.SetTransparent(false);
	m_staticCombo.SetSunken(true);
	m_staticCombo.AddTopWindow(m_lblTtlSource);
	m_staticCombo.AddTopWindow(m_lblTtlTerrain);
	
	m_staticCombo.AddTopWindow(m_ComboSource);
	m_staticCombo.AddTopWindow(m_ComboTerrain);

	m_lblTtlTitreHors.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTitreHors.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTitreHors.SetBorder(false);

	m_lblTtlSource.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlSource.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlSource.SetBorder(false);

	m_lblTtlTerrain.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTerrain.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTerrain.SetBorder(false);

	m_lblTtlTTemoin.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTTemoin.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTTemoin.SetBorder(false);

	m_TitreTemoin.SetFontName(CATPColors::GetFontStaticName());
	m_TitreTemoin.SetFontSize(CATPColors::GetFontStaticSize());
	m_TitreTemoin.SetBorder(true);
	m_TitreTemoin.SetText("");

	m_TitreApparie.SetFontName(CATPColors::GetFontStaticName());
	m_TitreApparie.SetFontSize(CATPColors::GetFontStaticSize());
	m_TitreApparie.SetBorder(true);
	m_TitreApparie.SetText("");

	SetColors();
}

void CDlgApparie::SetColors()
{
	CDlgFCP::SetColors();
	m_plstTitre->SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_plstTitre->SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_plstTitre->SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS));
	m_ComboSource.SetSelectionColor( CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_ComboTerrain.SetSelectionColor( CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTitreApparie.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTitreHors.SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_lstTitreHors.SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_lstTitreHors.SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS));

	m_staticTitreHors.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticCombo.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);

	m_lblTtlTitreHors.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTitreHors.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTitreHors.SetFontBold(true);

	m_lblTtlSource.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlSource.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlSource.SetFontBold(true);

	m_lblTtlTerrain.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTerrain.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTerrain.SetFontBold(true);

	m_lblTtlTTemoin.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTTemoin.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTTemoin.SetFontBold(true);

	m_TitreTemoin.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	m_TitreApparie.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
}