// DlgLot.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "..\Resource.h"
#include "DlgLot.h"

// Boîte de dialogue CDlgLot

IMPLEMENT_DYNAMIC(CDlgLot, CDlgFCP)

CDlgLot::CDlgLot(CWnd* pParent /*=NULL*/)
{
	m_IDLot = 0;
	m_plstTitre = new TitreInXListBox();
}

CDlgLot::~CDlgLot()
{
	delete m_plstTitre;
}

void CDlgLot::DoDataExchange(CDataExchange* pDX)
{
	CDlgFCP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RS_LISTTITRELOT, m_lstTitreLot);
	DDX_Control(pDX, IDC_RS_LISTLOT, m_lstLot);
}


BEGIN_MESSAGE_MAP(CDlgLot, CDlgFCP)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_RS_CTXLST_SUPPR, OnCtxSuppr)
	ON_COMMAND(IDM_RS_CTXLST_RENAME, OnCtxRename)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITRE, OnLbnSelchangeListTitre)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITRELOT, OnLbnSelchangeListTitreLot)
	ON_BN_CLICKED(IDC_RS_BTNVALID, OnBtnValider)
	ON_BN_CLICKED(IDC_RS_BTNEFFSAS, OnBtnEffSas)
	ON_BN_CLICKED(IDC_RS_BTNGENESE, OnBtnGenese)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgLot

BOOL CDlgLot::OnInitDialog()
{
	CDlgFCP::OnInitDialog();
	FillListLot();
	SetWindowRelations();
	SetupControls();
	return 	TRUE;
}

void CDlgLot::FillListLot()
{
	m_lstLot.ResetContent();
	m_plstTitre->m_mapIDRef.Reset();
	for (m_pRegroupements->GetLots()->MoveFirst();
		m_pRegroupements->GetLots()->IsValid();
		m_pRegroupements->GetLots()->MoveNext() )
	{
		CLot* lot = (CLot*)m_pRegroupements->GetLots()->GetItem();
		m_lstLot.AddItem ( lot );
		
		//initialisation du compteur de ref
		IIterateur * it = lot->CreateIterateur();
		INode * titrelot = it->getHead();
		while (titrelot)
		{
			m_plstTitre->m_mapIDRef.MoveTo( titrelot->GetID() );
			//s'il n'a pas été ajouté on l'ajoute sinon on incremente
			if ( ! m_plstTitre->m_mapIDRef.IsValid() )
				m_plstTitre->m_mapIDRef.Add(titrelot->GetID()) = 1;
			else
				m_plstTitre->m_mapIDRef.GetItem() ++;
			titrelot = it->getNext();
		}
		delete it;
	}
}

void CDlgLot::FillListTitreLot(int idLot)
{
	m_lstTitreLot.ResetContent();
	if (idLot != -1)
	{
		CLot* lot = m_pRegroupements->GetLotByID(idLot);
		IIterateur * it = lot->CreateIterateur();
		INode * support;
		CTitreLot * titrelot = (CTitreLot*)it->getHead();
		while (titrelot)
		{
			support = m_pRegroupements->GetSupportByID( titrelot->GetID() );
			m_lstTitreLot.AddItem ( support );
			titrelot = (CTitreLot*)it->getNext();
		}
		CString nbstr;
		nbstr.Format("%d", m_lstTitreLot.GetCount() );
		m_statictxtNbTitre.SetText(nbstr);
		delete it;
	}
}

void CDlgLot::OnContextMenu(CWnd *pWnd, CPoint point)
{
	CMenu menu;
	CPoint pt;
	pt = point;
	ScreenToClient(&pt);
	menu.LoadMenu(IDR_RS_MENUCTXTREE);

	if ( pWnd->GetSafeHwnd() == m_lstLot.GetSafeHwnd() )
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDlgLot::OnCtxSuppr()
{
	OnBtnEffSas();
	CLot * lot = NULL;
	int sel = m_lstLot.GetCurSel();
	if (sel != LB_ERR)
	{
		lot = (CLot*)m_lstLot.GetItemData( sel );
		CVisiteurDelete vDelete;
		lot->Accept(&vDelete);
		FillListLot();
	}
	OnBtnEffSas();
}

void CDlgLot::OnCtxRename()
{
	CLot * lot = NULL;
	int sel = m_lstLot.GetCurSel();
	if (sel != LB_ERR)
	{
		lot = (CLot*)m_lstLot.GetItemData( sel );
		CDlgRename dlg;
		dlg.m_Libelle = lot->GetLabel().AsJCharPtr();
		if (dlg.DoModal() == IDOK)
		{
			lot->SetLabel(dlg.m_Libelle);
			FillListLot();
		}
	}
//	OnBtnEffSas();//OnBtnEffContainer();
}

void CDlgLot::OnLbnSelchangeListTitre()
{
	int sel = m_plstTitre->GetCurSel();
	if (sel != LB_ERR)
	{
		INode * support = (INode*) m_plstTitre->GetItemData( sel );
		//s'il n'a pas dejà ete ajouté
		if ( m_lstTitreLot.FindItemIndex( support->GetID() ) == LB_ERR )
		{
			m_lstTitreLot.AddItem ( support );

			m_plstTitre->m_mapIDRef.MoveTo( support->GetID() );
			//s'il n'a pas été ajouté on l'ajoute sinon on incremente
			if ( !m_plstTitre->m_mapIDRef.IsValid() )
				m_plstTitre->m_mapIDRef.Add(support->GetID()) = 1;
			else
				m_plstTitre->m_mapIDRef.GetItem() ++;
			SaveSel();
			m_plstTitre->Invalidate();
			//FillListTitre();

		}
		CString nbstr;
		nbstr.Format("%d", m_lstTitreLot.GetCount() );
		m_statictxtNbTitre.SetText(nbstr);
	}
	m_plstTitre->SetCurSel(-1);
}

void CDlgLot::OnLbnSelchangeListTitreLot()
{
	int sel = m_lstTitreLot.GetCurSel();
	if (sel != LB_ERR)
	{
		INode* support = (INode*) m_lstTitreLot.GetItemData(sel);
		m_plstTitre->m_mapIDRef.MoveTo( support->GetID() );
		//on decremente
		if ( m_plstTitre->m_mapIDRef.IsValid() )
			m_plstTitre->m_mapIDRef.GetItem() --;
		SaveSel();
		m_plstTitre->Invalidate();
		//FillListTitre();

		m_lstTitreLot.DeleteString(sel);
		CString nbstr;
		nbstr.Format("%d", m_lstTitreLot.GetCount() );
		m_statictxtNbTitre.SetText(nbstr);
	}
}

void CDlgLot::OnBtnEffSas()
{
	m_statictxtNbTitre.SetText("0");
	m_Nom = "";
	UpdateData(FALSE);
	for(int i = 0; i<m_lstTitreLot.GetCount(); i++)
	{
		INode* support = (INode*) m_lstTitreLot.GetItemData(i);
		m_plstTitre->m_mapIDRef.MoveTo( support->GetID() );
		if (m_IDLot==0)
		{
			//on decremente
			if ( m_plstTitre->m_mapIDRef.IsValid() )
				m_plstTitre->m_mapIDRef.GetItem() --;
		}
		else
		{
			CLot* lot = m_pRegroupements->GetLotByID(m_IDLot);
			IIterateur * it = lot->CreateIterateur();
			if( ! it->getNodeByID(support->GetID()) )
				if ( m_plstTitre->m_mapIDRef.IsValid() )
					m_plstTitre->m_mapIDRef.GetItem() --;
			delete it;
		}
	}
	m_lstTitreLot.ResetContent();
	m_plstTitre->Invalidate();
//	m_lstLot.SetCurSel(-1);
	m_IDLot = 0;
}

void CDlgLot::OnBtnValider()
{
	UpdateData();
	if ( !m_Nom.IsEmpty() )
	{
		CLot* lot = NULL;
		//update
		if (m_IDLot)
		{
			lot = m_pRegroupements->GetLotByID(m_IDLot);
			
			CVisiteurDelete vDelete;
			lot->Accept(&vDelete);
			lot = new CLot(	m_IDLot, m_Nom );
		}
		else
		{
			srand( (unsigned)time( NULL ) );
			lot = new CLot(	IDLOT_MIN + rand(), m_Nom );
		}
		CVisiteurInsert vInsert;
		lot->Accept(&vInsert);
		
		//remplissage des titres
		CTitreLot* titrelot = NULL;
		for (int i = 0; i<m_lstTitreLot.GetCount(); i++)
		{
			INode* support = (INode*) m_lstTitreLot.GetItemData(i);
			titrelot = new CTitreLot(support->GetID(), support->GetParentID(), lot->GetID(), lot );
			titrelot->Accept(&vInsert);
		}

		FillListLot();
		m_lstTitreLot.ResetContent();
		OnBtnEffContainer();
	}
	SetFocus();
}

void CDlgLot::OnBtnGenese()
{
	CLot * lot = NULL;
	int sel = m_lstLot.GetCurSel();
	if (sel != LB_ERR)
	{
		lot = (CLot*)m_lstLot.GetItemData( sel );
		//On rempli les zones d'édition correspondant au titre
		m_Nom = lot->GetLabel().AsJCharPtr();
		UpdateData(FALSE);
		m_IDLot = lot->GetID();
		FillListTitreLot(m_IDLot);

		ResetSel();
		IIterateur* it = lot->CreateIterateur();
		CTitreLot* titrelot = (CTitreLot*)it->getHead();
		while (titrelot)
		{
			//recupération du centre
			int centreID = titrelot->GetIDCentre();
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
			m_selPeriodID.AddTail() = titrelot->GetPeriodicite();

			RefreshList();
			titrelot = (CTitreLot*)it->getNext();
		}
		delete it;
	}

}

void CDlgLot::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
	{IDC_RS_STATICTITRELOT,{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_RS_STATICTTLTITRELOT,	{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eTop},  //Top side
						{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eTop}}, //Bottom side
	
			
	{IDC_RS_LISTTITRELOT,		{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eLeft}, //Left side
					{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eTop},  //Top side
					{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eRight}, //Right side
					{CResizer::eFixed, IDC_RS_STATICTITRELOT, CResizer::eBottom}}, //Bottom side
	//
	{IDC_RS_STATICTTLCUSTOM,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	{IDC_RS_EDITNOM,			{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTXTNBTITRELOT,{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_LISTLOT,			{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eBottom}}, //Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Add(s_bi, nSize);
}

void CDlgLot::SetupControls()
{
	m_plstTitre->SubclassDlgItem(IDC_RS_LISTTITRE, this);
	
	m_statictxtNbTitre.SubclassDlgItem(IDC_RS_STATICTXTNBTITRELOT, this);
	m_lblTtlTitreLot.SubclassDlgItem(IDC_RS_STATICTTLTITRELOT, this);
	m_staticTitreLot.SubclassDlgItem(IDC_RS_STATICTITRELOT, this);

	m_staticCustom.AddTopWindow(m_lstLot);
	m_staticCustom.AddTopWindow(m_statictxtNbTitre);

	m_staticTitreLot.SetTransparent(false);
	m_staticTitreLot.SetSunken(true);
	m_staticTitreLot.AddTopWindow(m_lblTtlTitreLot);
	m_staticTitreLot.AddTopWindow(m_lstTitreLot);

	m_lblTtlTitreLot.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTitreLot.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTitreLot.SetBorder(false);

	m_statictxtNbTitre.SetFontName(CATPColors::GetFontStaticName());
	m_statictxtNbTitre.SetFontSize(CATPColors::GetFontStaticSize());
	m_statictxtNbTitre.SetBorder(true);
	m_statictxtNbTitre.SetText("");

	SetColors();
}

void CDlgLot::SetColors()
{
	CDlgFCP::SetColors();
	m_plstTitre->SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_plstTitre->SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_plstTitre->SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS));
	m_lstLot.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTitreLot.SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_lstTitreLot.SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	m_staticTitreLot.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	
	m_lblTtlTitreLot.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTitreLot.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTitreLot.SetFontBold(true);
	
	m_statictxtNbTitre.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
}