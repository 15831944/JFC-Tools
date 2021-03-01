// DlgCouplage.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "..\Resource.h"
#include "DlgCouplage.h"


// Boîte de dialogue CDlgCouplage

IMPLEMENT_DYNAMIC(CDlgCouplage, CDlgFCP)

CDlgCouplage::CDlgCouplage(CWnd* pParent /*=NULL*/)
{
	m_IDCpl = 0;
	m_plstTitre = new TitreInXListBox();
}

CDlgCouplage::~CDlgCouplage()
{
	delete m_plstTitre;
}

void CDlgCouplage::DoDataExchange(CDataExchange* pDX)
{
	CDlgFCP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RS_LISTCOUPLAGE, m_lstCouplage);
	DDX_Control(pDX, IDC_RS_STATICGRID, m_gridTitreCpl);
}


BEGIN_MESSAGE_MAP(CDlgCouplage, CDlgFCP)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_RS_CTXLST_SUPPR, OnCtxSuppr)
	ON_COMMAND(IDM_RS_CTXLST_RENAME, OnCtxRename)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITRE, OnLbnSelchangeListTitre)
	ON_NOTIFY(GVN_SELCHANGED, IDC_RS_STATICGRID, OnGridEndSelChange)
	ON_BN_CLICKED(IDC_RS_BTNVALID, OnBtnValider)
	ON_BN_CLICKED(IDC_RS_BTNEFFSAS, OnBtnEffSas)
	ON_BN_CLICKED(IDC_RS_BTNGENESE, OnBtnGenese)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgCouplage

BOOL CDlgCouplage::OnInitDialog()
{
	CRect rect;
	GetDlgItem(IDC_RS_STATICGRID)->GetWindowRect(&rect);
	POINT pt1, pt2;
	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
	m_gridTitreCpl.Create(rect, this, IDC_RS_STATICGRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_VSCROLL);
	
	CDlgFCP::OnInitDialog();
	SetWindowRelations();
	SetupControls();
	return 	TRUE;
}

void CDlgCouplage::FillListCouplage()
{
	m_lstCouplage.ResetContent();
	m_plstTitre->m_mapIDRef.Reset();
	for (m_pRegroupements->GetCouplagesLibres()->MoveFirst();
		m_pRegroupements->GetCouplagesLibres()->IsValid();
		m_pRegroupements->GetCouplagesLibres()->MoveNext() )
	{
		CCouplageLibre* cpl = (CCouplageLibre*)m_pRegroupements->GetCouplagesLibres()->GetItem();
		//ajout du couplage
		m_lstCouplage.AddItem ( cpl );
		//initialisation du compteur de ref
		IIterateur * it = cpl->CreateIterateur();
		INode * titrecpl = it->getHead();
		while (titrecpl)
		{
			m_plstTitre->m_mapIDRef.MoveTo( titrecpl->GetID() );
			//s'il n'a pas été ajouté on l'ajoute sinon on incremente
			if ( ! m_plstTitre->m_mapIDRef.IsValid() )
				m_plstTitre->m_mapIDRef.Add(titrecpl->GetID()) = 1;
			else
				m_plstTitre->m_mapIDRef.GetItem() ++;
			titrecpl = it->getNext();
		}
		delete it;
	}

}

void CDlgCouplage::FillListTitreCouplage(int idCouplage)
{
	m_gridTitreCpl.DeleteNonFixedRows();
	if (idCouplage != -1)
	{
		CCouplageLibre* cpllibre = m_pRegroupements->GetCouplageLibreByID(idCouplage);
		IIterateur * it = cpllibre->CreateIterateur();
		INode * support;
		CTitreCouplageLibre * titrecpll = (CTitreCouplageLibre*)it->getHead();
		while (titrecpll)
		{
			support = m_pRegroupements->GetSupportByID( titrecpll->GetID() );
			int nR = m_gridTitreCpl.InsertRow( support->GetLabel().AsJCharPtr() );
			m_gridTitreCpl.SetItemData(nR, 0, (LPARAM)support );
			m_gridTitreCpl.SetItemState(nR, 0, m_gridTitreCpl.GetItemState(nR,0) | GVIS_READONLY);
			m_gridTitreCpl.SetCellType(nR, 1, RUNTIME_CLASS(CGridCellNumClick));
			m_gridTitreCpl.SetItemState(nR, 1, m_gridTitreCpl.GetItemState(nR,0) | GVIS_READONLY);
			CString txt;
			txt.Format("%d", titrecpll->GetNbInsert());
			m_gridTitreCpl.SetItemText(nR, 1, txt );

			titrecpll = (CTitreCouplageLibre*)it->getNext();
		}
		CString nbstr;
		nbstr.Format("%d", m_gridTitreCpl.GetRowCount() );
		m_statictxtNbTitre.SetText(nbstr);
		delete it;
	}
	m_gridTitreCpl.Refresh();

}

void CDlgCouplage::OnContextMenu(CWnd *pWnd, CPoint point)
{
	CMenu menu;
	CPoint pt;
	pt = point;
	ScreenToClient(&pt);
	menu.LoadMenu(IDR_RS_MENUCTXTREE);

	if ( pWnd->GetSafeHwnd() == m_lstCouplage.GetSafeHwnd() )
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDlgCouplage::OnCtxSuppr()
{
	OnBtnEffSas();
	CCouplageLibre * cpll = NULL;
	int sel = m_lstCouplage.GetCurSel();
	if (sel != LB_ERR)
	{
		cpll = (CCouplageLibre*)m_lstCouplage.GetItemData( sel );
		CVisiteurDelete vDelete;
		cpll->Accept(&vDelete);
		FillListCouplage();
	}
	OnBtnEffSas();
}

void CDlgCouplage::OnCtxRename()
{
	CCouplageLibre * cpll = NULL;
	int sel = m_lstCouplage.GetCurSel();
	if (sel != LB_ERR)
	{
		cpll = (CCouplageLibre*)m_lstCouplage.GetItemData( sel );
		CDlgRename dlg;
		dlg.m_Libelle = cpll->GetLabel().AsJCharPtr();
		if (dlg.DoModal() == IDOK)
		{
			cpll->SetLabel(dlg.m_Libelle);
			FillListCouplage();
		}
	}
}

void CDlgCouplage::OnLbnSelchangeListTitre()
{
	int sel = m_plstTitre->GetCurSel();
	if (sel != LB_ERR)
	{
		INode * support = (INode*) m_plstTitre->GetItemData( sel );
		//s'il n'a pas déjà été ajouté
		bool bAdded = false;
		for (int i = 0; i < m_gridTitreCpl.GetRowCount(); i++)
			if ( ((INode*)m_gridTitreCpl.GetItemData(i, 0))->GetID() == support->GetID() )
			{
				bAdded = true;
				break;
			}
		//s'il n'a pas déjà été ajouté
		if (!bAdded)
		{
			int nR = m_gridTitreCpl.InsertRow( support->GetLabel().AsJCharPtr() );
			m_gridTitreCpl.SetItemData(nR, 0, (LPARAM)support );
			//m_gridTitreCpl.SetItemText(nR, 0, support->GetLabel().AsJCharPtr() );
			m_gridTitreCpl.SetItemState(nR, 0, m_gridTitreCpl.GetItemState(nR,0) | GVIS_READONLY);
			m_gridTitreCpl.SetCellType(nR, 1, RUNTIME_CLASS(CGridCellNumClick));
			m_gridTitreCpl.SetItemState(nR, 1, m_gridTitreCpl.GetItemState(nR,0) | GVIS_READONLY);
			m_gridTitreCpl.SetItemText(nR, 1, "1" );
			m_gridTitreCpl.RedrawRow(nR);

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
		nbstr.Format("%d", m_gridTitreCpl.GetRowCount() );
		m_statictxtNbTitre.SetText(nbstr);
	}
	m_plstTitre->SetCurSel(-1);
}

void CDlgCouplage::OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (pItem->iColumn == 0)
	{
		INode* support = (INode*)m_gridTitreCpl.GetItemData(pItem->iRow, 0);
		m_plstTitre->m_mapIDRef.MoveTo( support->GetID() );
		//on decremente
		if ( m_plstTitre->m_mapIDRef.IsValid() )
			m_plstTitre->m_mapIDRef.GetItem() --;
//		SaveSel();
//		FillListTitre();

		m_gridTitreCpl.DeleteRow(pItem->iRow);
		m_gridTitreCpl.Refresh();
	
		CString nbstr;
		nbstr.Format("%d", m_gridTitreCpl.GetRowCount() );
		m_statictxtNbTitre.SetText(nbstr);
		SaveSel();
		m_plstTitre->Invalidate();
		//FillListTitre();
	}
	else
		m_gridTitreCpl.SetFocusCell(pItem->iRow, pItem->iColumn);
}

void CDlgCouplage::OnBtnEffSas()
{
	m_statictxtNbTitre.SetText("");
	m_Nom = "";
	UpdateData(FALSE);
	for(int i = 0; i<m_gridTitreCpl.GetRowCount(); i++)
	{
		INode* support = (INode*)m_gridTitreCpl.GetItemData(i, 0);
		m_plstTitre->m_mapIDRef.MoveTo( support->GetID() );
		if (m_IDCpl==0)
		{
			//on decremente
			if ( m_plstTitre->m_mapIDRef.IsValid() )
				m_plstTitre->m_mapIDRef.GetItem() --;
		}
		else
		{
			CCouplageLibre* cpl = m_pRegroupements->GetCouplageLibreByID(m_IDCpl);
			IIterateur * it = cpl->CreateIterateur();
			if( ! it->getNodeByID(support->GetID()) )
				if ( m_plstTitre->m_mapIDRef.IsValid() )
					m_plstTitre->m_mapIDRef.GetItem() --;
			delete it;
		}
	}
	FillListTitreCouplage(-1);
	m_plstTitre->Invalidate();
	//FillListTitre();
	m_plstTitre->SetCurSel(-1);
	m_IDCpl = 0;
}

void CDlgCouplage::OnBtnValider()
{
	UpdateData();
	if ( !m_Nom.IsEmpty() )
	{
		CCouplageLibre* cpl = NULL;
		//update
		if (m_IDCpl)
		{
			cpl = m_pRegroupements->GetCouplageLibreByID(m_IDCpl);
			CVisiteurDelete vDelete;
			cpl->Accept(&vDelete);
			cpl = new CCouplageLibre( m_IDCpl, m_Nom );
		}
		else
		{
			srand( (unsigned)time( NULL ) );
			cpl = new CCouplageLibre( IDCOUPLELIBRE_MIN + rand(), m_Nom );
		}
		CVisiteurInsert vInsert;
		cpl->Accept(&vInsert);
		
		//remplissage des titres
		CTitreCouplageLibre* titrecpl = NULL;
		for (int i = 0; i < m_gridTitreCpl.GetRowCount(); i++)
		{
			INode * support = (INode*)m_gridTitreCpl.GetItemData(i, 0);
			int nbins = atoi( m_gridTitreCpl.GetItemText(i, 1) );
			
			JUnt32 mediaWeight = 100;
			if (support->IsTitreApparente())
				mediaWeight = support->AsTitreApparente()->GetMediaWeight();

			titrecpl = new CTitreCouplageLibre(	support->GetID(), nbins, support->GetParentID(), cpl->GetID(), cpl);
			titrecpl->Accept(&vInsert);
		}

		OnBtnEffContainer();
		FillListCouplage();
		m_plstTitre->Invalidate();
	}
}

void CDlgCouplage::OnBtnGenese()
{
	CCouplageLibre * cpl = NULL;
	int sel = m_lstCouplage.GetCurSel();
	if (sel != LB_ERR)
	{
		cpl = (CCouplageLibre*)m_lstCouplage.GetItemData( sel );
		//On rempli les zones d'édition correspondant au titre
		m_Nom = cpl->GetLabel().AsJCharPtr();
		UpdateData(FALSE);
		m_IDCpl = cpl->GetID();
		FillListTitreCouplage(m_IDCpl);

		ResetSel();
		IIterateur* it = cpl->CreateIterateur();
		CTitreCouplageLibre* titrecpl = (CTitreCouplageLibre*)it->getHead();

		while (titrecpl)
		{
			//recupération du centre
			int centreID = titrecpl->GetIDCentre();
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
			m_selPeriodID.AddTail() = titrecpl->GetPeriodicite();
			RefreshList();
			titrecpl = (CTitreCouplageLibre*)it->getNext();
		}
		delete it;
	}

}

void CDlgCouplage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDlgFCP::OnShowWindow(bShow, nStatus);
	if (bShow)
		FillListCouplage();
	else
		OnBtnEffSas();
}

void CDlgCouplage::OnSize(UINT nType, int cx, int cy) 
{
	CDlgFCP::OnSize(nType, cx, cy);

	if (m_gridTitreCpl.GetSafeHwnd())
	{
		CRect rect;
		GetDlgItem(IDC_RS_STATICGRID)->GetWindowRect(&rect);
		POINT pt1, pt2;
		pt1.x = rect.left, pt1.y = rect.top;
		pt2.x = rect.right, pt2.y = rect.bottom;
		::ScreenToClient(m_hWnd, &pt1);
		::ScreenToClient(m_hWnd, &pt2);
		rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
		m_gridTitreCpl.SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER );
		ComputeColumnWidth();
	}
}

void CDlgCouplage::ComputeColumnWidth()
{
	if ( m_gridTitreCpl.GetSafeHwnd() )
	{
		m_gridTitreCpl.ExpandColumnsToFit(); 
		long width = m_gridTitreCpl.GetVirtualWidth();
		CRect rect;
		GetDlgItem(IDC_RS_STATICTTLTCOUPLAGE)->GetWindowRect(&rect);
		m_gridTitreCpl.SetColumnWidth(0, rect.Width() );
		GetDlgItem(IDC_RS_STATICTTLNBINSERT)->GetWindowRect(&rect);
		m_gridTitreCpl.SetColumnWidth(1, rect.Width() );
		m_gridTitreCpl.ExpandColumnsToFit(); 
	}
}

void CDlgCouplage::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

	{IDC_RS_STATICCOUPLAGE,	{CResizer::eFixed, IDC_RS_STATICCENTRE, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eBottom},  //Top side
						{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	//					{CResizer::eProportional, IDC_MAIN, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTTLTCOUPLAGE,	{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCOUPLAGE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop}}, //Bottom side
	{IDC_RS_STATICTTLNBINSERT,	{CResizer::eFixed, IDC_RS_STATICTTLTCOUPLAGE, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCOUPLAGE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop}}, //Bottom side

			
	{IDC_RS_STATICGRID,		{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eBottom}}, //Bottom side

///	
	{IDC_RS_STATICTTLTTEMOIN,{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTTLCUSTOM,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	

	{IDC_RS_EDITNOM,			{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTXTNBTITRECPL,{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	
	{IDC_RS_LISTCOUPLAGE,		{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
			{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eBottom}}, //Bottom side

};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	CDlgFCP::m_Resizer.Add(s_bi, nSize);
}


void CDlgCouplage::SetupControls()
{
	m_plstTitre->SubclassDlgItem(IDC_RS_LISTTITRE, this);
	m_statictxtNbTitre.SubclassDlgItem(IDC_RS_STATICTXTNBTITRECPL, this);
	m_lblTtlTitreCouplage.SubclassDlgItem(IDC_RS_STATICTTLTCOUPLAGE, this);
	m_lblTtlNbInsert.SubclassDlgItem(IDC_RS_STATICTTLNBINSERT, this);
	m_staticCouplage.SubclassDlgItem(IDC_RS_STATICCOUPLAGE, this);

	//m_gridTitreCpl.SubclassDlgItem(IDC_RS_STATICGRID, this);
	m_gridTitreCpl.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	m_gridTitreCpl.SetColumnCount(2);
	//m_gridTitreCpl.SetListMode(); 
	//m_gridTitreCpl.SetSingleRowSelection(); 
	ComputeColumnWidth();

	m_staticCustom.AddTopWindow(m_lstCouplage);
	m_staticCustom.AddTopWindow(m_statictxtNbTitre);

	m_staticCouplage.SetTransparent(false);
	m_staticCouplage.SetSunken(true);
	m_staticCouplage.AddTopWindow(m_lblTtlTitreCouplage);
	m_staticCouplage.AddTopWindow(m_gridTitreCpl);
	m_staticCouplage.AddTopWindow(m_lblTtlNbInsert);

	m_lblTtlTitreCouplage.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTitreCouplage.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTitreCouplage.SetBorder(false);

	m_lblTtlNbInsert.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlNbInsert.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlNbInsert.SetBorder(false);


	m_statictxtNbTitre.SetFontName(CATPColors::GetFontStaticName());
	m_statictxtNbTitre.SetFontSize(CATPColors::GetFontStaticSize());
	m_statictxtNbTitre.SetBorder(true);
	m_statictxtNbTitre.SetText("");

	SetColors();
}

void CDlgCouplage::SetColors()
{
	CDlgFCP::SetColors();
	m_gridTitreCpl.GetDefaultCell(FALSE, FALSE)->SetSelClr(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS));
	m_gridTitreCpl.SetBkColor(CATPColors::GetColorWhite());
	m_plstTitre->SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_plstTitre->SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_plstTitre->SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS));
	m_lstCouplage.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );

	m_staticCouplage.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	
	m_lblTtlTitreCouplage.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTitreCouplage.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTitreCouplage.SetFontBold(true);
	
	m_lblTtlNbInsert.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlNbInsert.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlNbInsert.SetFontBold(true);

	m_statictxtNbTitre.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
}