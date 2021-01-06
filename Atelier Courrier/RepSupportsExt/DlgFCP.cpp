// DlgFCP.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgFCP.h"


// Boîte de dialogue CDlgFCP

IMPLEMENT_DYNAMIC(CDlgFCP, CDialog)

CDlgFCP::CDlgFCP(CWnd* pParent /*=NULL*/)
{
	m_AfficheApparentes = true;
	m_pRegroupements = CRegroupements::GetInstance();
	m_srcpresse = (JSRCPresse*)& JSRCPresse::GetInstance();
	m_Nom = "";
	m_pfamTNA = new CFamille(0,"Titres non attribués");
}

CDlgFCP::~CDlgFCP()
{
	delete m_pfamTNA;
}

void CDlgFCP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RS_LISTPER, m_lstPeriodicite);
	DDX_Control(pDX, IDC_RS_LISTFAMILLE, m_lstFamille);
	DDX_Control(pDX, IDC_RS_LISTCENTRE, m_lstCentre);
	DDX_Text(pDX, IDC_RS_EDITNOM, m_Nom);
	DDX_Control(pDX, IDC_RS_BTNVALID, m_btnValider);
}


BEGIN_MESSAGE_MAP(CDlgFCP, CDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RS_BTNSELFAMILLE, OnBnClickedBtnTFamille)
	ON_BN_CLICKED(IDC_RS_BTNSELCENTRE, OnBnClickedBtnTCentre)
	ON_BN_CLICKED(IDC_RS_BTNSELPER, OnBnClickedBtnTPeriodicite)
	ON_BN_CLICKED(IDC_RS_BTNEFFCONTAINER, OnBtnEffContainer)
	ON_LBN_SELCHANGE(IDC_RS_LISTFAMILLE, OnLbnSelchangeListFamille)
	ON_LBN_SELCHANGE(IDC_RS_LISTCENTRE, OnLbnSelchangeListCentre)
	ON_LBN_SELCHANGE(IDC_RS_LISTPER, OnLbnSelchangeListPeriodicite)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgFCP

BOOL CDlgFCP::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillListFamille();

	for (m_srcpresse->m_TBLPeriodicites.MoveFirst();
		m_srcpresse->m_TBLPeriodicites.IsValid();
		m_srcpresse->m_TBLPeriodicites.MoveNext() )
	{
		m_lstPeriodicite.AddItem ( (JPeriodicite*)m_srcpresse->m_TBLPeriodicites.GetItem() );
	}

	SetupControls();
	SetWindowRelations();

	return 	TRUE;
}

void CDlgFCP::OnBnClickedBtnTFamille()
{
	m_lstFamille.ToggleSelection();
	OnLbnSelchangeListFamille();
}

void CDlgFCP::OnBnClickedBtnTCentre()
{
	m_lstCentre.ToggleSelection();
	OnLbnSelchangeListCentre();
}

void CDlgFCP::OnBnClickedBtnTPeriodicite()
{
	m_lstPeriodicite.ToggleSelection();
	OnLbnSelchangeListPeriodicite();
}



void CDlgFCP::OnLbnSelchangeListFamille()
{
	m_lstCentre.GetSelectedID(m_selCentreID);
	FillListCentre();
	FillListTitre();
}

void CDlgFCP::OnLbnSelchangeListCentre()
{
	m_plstTitre->GetSelectedID(m_selTitreID);
	FillListTitre();
}

void CDlgFCP::OnLbnSelchangeListPeriodicite()
{
	m_plstTitre->GetSelectedID(m_selTitreID);
	FillListTitre();
}

void CDlgFCP::RefreshList()
{
	FillListFamille();
	FillListCentre();
	m_lstPeriodicite.SetSelectedID(m_selPeriodID);
	FillListTitre();
}


void CDlgFCP::FillListFamille()
{
	//On vide le contenu de la liste
	m_lstFamille.ResetContent();

	m_lstFamille.AddItem ( m_pfamTNA );
	//puis on ajoute les eléments
	for (m_pRegroupements->GetFamilles()->MoveFirst();
		m_pRegroupements->GetFamilles()->IsValid();
		m_pRegroupements->GetFamilles()->MoveNext() )
	{
		m_lstFamille.AddItem ( (CFamille*)m_pRegroupements->GetFamilles()->GetItem() );
	}
	//On resélectionnne (pas de pb si l'id n'existe plus)
	m_lstFamille.SetSelectedID(m_selFamilleID);
}

void CDlgFCP::FillListCentre()
{
	m_lstCentre.ResetContent();
	int nCount = m_lstFamille.GetSelCount();
	//si aucune famille n'est selectionnée
/*	if (nCount == 0)
	{
		for (int i=0; i < m_lstFamille.GetCount(); i++)
		{
			CFamille* famillesel = (CFamille*) m_lstFamille.GetItemData(i);
			IIterateur* it = famillesel->CreateIterateur();
			INode * filstmp = it->getHead();
			while (filstmp)
			{
				if (!filstmp->IsTitre())
					m_lstCentre.AddItem ( (CCentre*)filstmp );
				filstmp = it->getNext();
			}
			delete it;
		}
	}
	else
*/	//sinon on n'affiche que les centres des familles sélectionnées
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
				if (!filstmp->IsTitre())
					m_lstCentre.AddItem( (CCentre*)filstmp );
				filstmp = it->getNext();
			}
			delete it;
		}
	}
	//On resélectionnne (pas de pb si l'id n'existe plus)
	m_lstCentre.SetSelectedID(m_selCentreID);

}

void CDlgFCP::FillListTitre()
{
	m_plstTitre->ResetContent();
	JMap<JUnt32,JUnt32> mapID;
	
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
					&& TestPeriodicite( filstmp->GetID() )
					&& TestApparente( filstmp->GetID() ) )
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
				mapID.MoveTo(filstmp->GetID());
				if (filstmp->IsTitre() 
					&& TestPeriodicite( filstmp->GetID() )
					&& TestApparente( filstmp->GetID() ) 
					&& !mapID.IsValid())
				{
					m_plstTitre->AddItem ( filstmp );
					mapID.Add(filstmp->GetID());
				}
				filstmp = it->getNext();
			}
			delete it;
		}
	}
	
	
	// les titres des centres

	//si aucun centre n'est selectionné
	nCount = m_lstCentre.GetSelCount();
	if (nCount == 0)
	{
		//on ajoute tous leurs titres
		for (int i=0; i < m_lstCentre.GetCount(); i++)
		{
			CCentre* centresel = (CCentre*) m_lstCentre.GetItemData(i);
			IIterateur* it = centresel->CreateIterateur();
			INode * titretmp = it->getHead();
			while (titretmp)
			{
				mapID.MoveTo(titretmp->GetID());
				if (TestPeriodicite( titretmp->GetID() )
					&& TestApparente( titretmp->GetID() ) 
					&& !mapID.IsValid())
				{
					m_plstTitre->AddItem ( titretmp );
					mapID.Add(titretmp->GetID());
				}
				titretmp = it->getNext();
			}
			delete it;
		}
	}
	else
	{
		//on ajoute uniquement les titres des centres sélectionnés
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
				mapID.MoveTo(titretmp->GetID());
				if (TestPeriodicite( titretmp->GetID() )
					&& TestApparente( titretmp->GetID() ) 
					&& !mapID.IsValid())
				{
					m_plstTitre->AddItem ( titretmp );
					mapID.Add(titretmp->GetID());
				}
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
			mapID.MoveTo(titre->GetID());
			if ( TestPeriodicite( titre->GetID() ) && !mapID.IsValid() )
			{
				m_plstTitre->AddItem (titre);
				mapID.Add(titre->GetID());
			}
		}
		if (m_AfficheApparentes)
		{
			for ( m_pRegroupements->GetTitresApparentes()->MoveFirst(); m_pRegroupements->GetTitresApparentes()->IsValid(); m_pRegroupements->GetTitresApparentes()->MoveNext())
			{
				CTitreApparente* titreHS = (CTitreApparente*)m_pRegroupements->GetTitresApparentes()->GetItem();
				mapID.MoveTo(titreHS->GetID());
				if ( titreHS->GetParentID() == 0 && TestPeriodicite( titreHS->GetID() )  && !mapID.IsValid() )
				{
					m_plstTitre->AddItem (titreHS);
					mapID.Add(titreHS->GetID());
				}
			}
		}
	}

	//On resélectionnne (pas de pb si l'id n'existe plus)
	m_plstTitre->SetSelectedID(m_selTitreID);

}

bool CDlgFCP::TestApparente(int ID)
{
	//si on affiche les apparentés on sort
	if (m_AfficheApparentes)
		return true;

	//sinon on teste si c un apparenté
	if ( ID > IDTITREAPPARENTE_MIN && ID < IDTITREAPPARENTE_MAX )
		return false;
	
	//c'en etait pas un 
	return true;
}

bool CDlgFCP::TestPeriodicite(int ID)
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

void CDlgFCP::OnBtnEffContainer()
{
	ResetSel();
	RefreshList();
	OnBtnEffSas();
}

void CDlgFCP::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	//quand on affiche cette fenetre on recharge les listes(car elles ont pu changer)
	if (bShow)
		RefreshList();
	else
	{
		SaveSel();
		OnBtnEffSas();
	}

	// Et on valide toutes les familles, pour lister tous les supports à droite
	m_lstFamille.SelItemRange(true, 0, m_lstFamille.GetCount()-1);
	OnLbnSelchangeListFamille();

}

void CDlgFCP::SaveSel()
{
	m_lstFamille.GetSelectedID(m_selFamilleID);
	m_lstCentre.GetSelectedID(m_selCentreID);
	m_plstTitre->GetSelectedID(m_selTitreID);
	m_lstPeriodicite.GetSelectedID(m_selPeriodID);
}

void CDlgFCP::ResetSel()
{
	m_selFamilleID.Reset();
	m_selCentreID.Reset();
	m_selTitreID.Reset();
	m_selPeriodID.Reset();
}

void CDlgFCP::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
}

void CDlgFCP::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
	{IDC_RS_STATICFAMILLE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_RS_STATICTTLFAMILLE,	{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eTop}}, //Bottom side

	{IDC_RS_BTNSELFAMILLE,		{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eTop}}, //Bottom side
			
	{IDC_RS_LISTFAMILLE,		{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eBottom}}, //Bottom side
///
	{IDC_RS_STATICCENTRE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eBottom},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_RS_STATICTTLCENTRE,	{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eTop}}, //Bottom side

	{IDC_RS_BTNSELCENTRE,		{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eTop}}, //Bottom side
			
	{IDC_RS_LISTCENTRE,		{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eBottom}}, //Bottom side
///
	{IDC_RS_STATICPER,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eBottom},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_RS_STATICTTLPER,	{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop}}, //Bottom side

	{IDC_RS_BTNSELPER,		{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop}}, //Bottom side
			
	{IDC_RS_LISTPER,		{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eBottom}}, //Bottom side
////
	{IDC_RS_STATICTITRE,	{CResizer::eFixed, IDC_RS_STATICFAMILLE, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_RS_STATICTTLTITRE,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop}}, //Bottom side

			
	{IDC_RS_LISTTITRE,		{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eBottom}}, //Bottom side
/////

	{IDC_RS_STATICCUSTOM,	{CResizer::eFixed, IDC_RS_STATICPER, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eBottom},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side


	{IDC_RS_STATICTXTNBMW,		{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	//
	//{IDC_RS_STATICTTLCUSTOM,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
	//					{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
	//					{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
	//					{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	//{IDC_RS_EDITNOM,			{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
	//						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
	//						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
	//						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			
	{IDC_RS_BTNVALID,			{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_BTNEFFCONTAINER,		{CResizer::eFixed, IDC_RS_BTNVALID, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_BTNEFFSAS,		{CResizer::eFixed, IDC_RS_BTNEFFCONTAINER, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_BTNGENESE,			{CResizer::eFixed, IDC_RS_BTNEFFSAS, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side


	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

void CDlgFCP::SetupControls()
{
//subclass 
	m_staticFamille.SubclassDlgItem(IDC_RS_STATICFAMILLE, this);
	m_staticCentre.SubclassDlgItem(IDC_RS_STATICCENTRE, this);
	m_staticPeriod.SubclassDlgItem(IDC_RS_STATICPER, this);
	m_staticTitre.SubclassDlgItem(IDC_RS_STATICTITRE, this);
	m_staticCustom.SubclassDlgItem(IDC_RS_STATICCUSTOM, this);

	m_lblTtlFamille.SubclassDlgItem(IDC_RS_STATICTTLFAMILLE, this);
	m_lblTtlCentre.SubclassDlgItem(IDC_RS_STATICTTLCENTRE, this);
	m_lblTtlPeriod.SubclassDlgItem(IDC_RS_STATICTTLPER, this);
	m_lblTtlTitre.SubclassDlgItem(IDC_RS_STATICTTLTITRE, this);

	m_btnSelFamille.SubclassDlgItem(IDC_RS_BTNSELFAMILLE, this);
	m_btnSelCentre.SubclassDlgItem(IDC_RS_BTNSELCENTRE, this);
	m_btnSelPeriodicite.SubclassDlgItem(IDC_RS_BTNSELPER, this);

//	m_btnValider.SubclassDlgItem(IDC_RS_BTNVALID, this);
	m_btnEffContainer.SubclassDlgItem(IDC_RS_BTNEFFCONTAINER, this);
	m_btnEffSas.SubclassDlgItem(IDC_RS_BTNEFFSAS, this);
	m_btnGenese.SubclassDlgItem(IDC_RS_BTNGENESE, this);
	m_staticTtlCustom.SubclassDlgItem(IDC_RS_STATICTTLCUSTOM, this);
	m_staticNBMW.SubclassDlgItem(IDC_RS_STATICTXTNBMW, this);

/////
	m_staticCustom.SetTransparent(false);
	m_staticCustom.SetSunken(true);
	m_staticCustom.AddTopWindow(m_btnValider);
	m_staticCustom.AddTopWindow(m_btnEffContainer);
	m_staticCustom.AddTopWindow(m_btnEffSas);
	m_staticCustom.AddTopWindow(m_btnGenese);
	m_staticCustom.AddTopWindow(m_staticNBMW);
	m_staticCustom.AddTopWindow(m_staticTtlCustom);
	m_staticCustom.AddTopWindow( GetDlgItem(IDC_RS_EDITNOM)->GetSafeHwnd() );


	m_staticFamille.SetTransparent(false);
	m_staticFamille.SetSunken(true);
	m_staticFamille.AddTopWindow(m_lblTtlFamille);
	m_staticFamille.AddTopWindow(m_lstFamille);
	m_staticFamille.AddTopWindow(m_btnSelFamille);

	m_staticCentre.SetTransparent(false);
	m_staticCentre.SetSunken(true);
	m_staticCentre.AddTopWindow(m_lblTtlCentre);
	m_staticCentre.AddTopWindow(m_lstCentre);
	m_staticCentre.AddTopWindow(m_btnSelCentre);

	m_staticPeriod.SetTransparent(false);
	m_staticPeriod.SetSunken(true);
	m_staticPeriod.AddTopWindow(m_lblTtlPeriod);
	m_staticPeriod.AddTopWindow(m_lstPeriodicite);
	m_staticPeriod.AddTopWindow(m_btnSelPeriodicite);

	m_staticTitre.SetTransparent(false);
	m_staticTitre.SetSunken(true);
	m_staticTitre.AddTopWindow(m_lblTtlTitre);
	m_staticTitre.AddTopWindow(GetDlgItem(IDC_RS_LISTTITRE)->GetSafeHwnd());

////
	m_lblTtlFamille.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlFamille.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlFamille.SetBorder(false);

	m_lblTtlCentre.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlCentre.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlCentre.SetBorder(false);

	m_lblTtlPeriod.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlPeriod.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlPeriod.SetBorder(false);

	m_lblTtlTitre.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTitre.SetFontSize(10);
	m_lblTtlTitre.SetBorder(false);

	m_staticNBMW.SetFontName(CATPColors::GetFontStaticName());
	m_staticNBMW.SetFontSize(CATPColors::GetFontStaticSize());
	m_staticNBMW.SetBorder(false);

	m_staticTtlCustom.SetFontName(CATPColors::GetFontLabelName());
	m_staticTtlCustom.SetFontSize(CATPColors::GetFontLabelSize());
	m_staticTtlCustom.SetBorder(false);

}

void CDlgFCP::SetColors()
{
	m_staticCustom.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticFamille.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticCentre.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticPeriod.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticTitre.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	
	m_staticTtlCustom.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_staticTtlCustom.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_staticTtlCustom.SetFontBold(true);

	m_lblTtlFamille.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlFamille.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlFamille.SetFontBold(true);

	m_lblTtlCentre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlCentre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlCentre.SetFontBold(true);

	m_lblTtlPeriod.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlPeriod.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS)); 
	m_lblTtlPeriod.SetFontBold(true);

	m_lblTtlTitre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTitre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS)); 
	m_lblTtlTitre.SetFontBold(true);

	m_staticNBMW.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);

	m_lstFamille.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstCentre.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstPeriodicite.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );

	m_btnSelFamille.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS);
	m_btnSelFamille.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelFamille.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnSelCentre.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelCentre.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelCentre.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnSelPeriodicite.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelPeriodicite.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnSelPeriodicite.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnValider.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnValider.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnValider.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnEffContainer.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS);
	m_btnEffContainer.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS);
	m_btnEffContainer.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnEffSas.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS);
	m_btnEffSas.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffSas.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnGenese.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnGenese.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnGenese.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
}