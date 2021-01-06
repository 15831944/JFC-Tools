// DlgTarif.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "..\Resource.h"
#include "DlgTarif.h"
#include "DlgRename.h"
#include "SearchAlgo.h"
#include "FormatStr.h"
#include "ATPMessages.h"

// Boîte de dialogue CDlgTarif

IMPLEMENT_DYNAMIC(CDlgTarif, CDialog)

CDlgTarif::CDlgTarif(CWnd* pParent /*=NULL*/)
{
	m_pRegroupements = CRegroupements::GetInstance();
	m_srcpresse = (JSRCPresse*)& JSRCPresse::GetInstance();
	m_pTarifs = CTarifs::GetInstance();

	m_NomFormatSpe = "";
	m_IDFormatSpeEdited = 0;
	m_pTarifEdited = NULL;

	m_lstFormatStd.m_dlgParent = this;

	ResetSel();
}

CDlgTarif::~CDlgTarif()
{
}

void CDlgTarif::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RS_LISTFORMATSTD, m_lstFormatStd);
	DDX_Control(pDX, IDC_RS_LISTFORMATSPE, m_lstFormatSpe);
	DDX_Control(pDX, IDC_RS_LISTTITRE, m_lstTitre);
	DDX_Control(pDX, IDC_RS_LISTCOUPLAGE, m_lstCPL);
	DDX_Control(pDX, IDC_RS_LISTTARIF, m_lstTarif);

	DDX_Text(pDX, IDC_RS_EDITSEARCH, m_searchStr);
	DDX_Text(pDX, IDC_RS_EDITFORMATSPE, m_NomFormatSpe);
}


BEGIN_MESSAGE_MAP(CDlgTarif, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_RS_CTXLST2_SUPPR, OnCtxSupprTarif)
	ON_COMMAND(IDM_RS_CTXLST_SUPPR, OnCtxSupprFSpe)
	ON_COMMAND(IDM_RS_CTXLST_RENAME, OnCtxRename)

	ON_BN_CLICKED(IDC_RS_BTNVALIDFSPE, OnBtnValiderFSpe)
//	ON_BN_CLICKED(IDC_RS_BTNGENESEFSPE, OnBtnGeneseFSpe)

	ON_BN_CLICKED(IDC_RS_BTNVALID, OnBtnValider)
	ON_BN_CLICKED(IDC_RS_BTNEFFCONTAINER, OnBtnEffContainer)
	ON_BN_CLICKED(IDC_RS_BTNEFFSAS, OnBtnEffSas)
	ON_BN_CLICKED(IDC_RS_BTNGENESE, OnBtnGenese)

	ON_LBN_SELCHANGE(IDC_RS_LISTFORMATSTD, OnLbnSelchangeListFormatStd)
	ON_LBN_SELCHANGE(IDC_RS_LISTFORMATSPE, OnLbnSelchangeListFormatSpe)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITRE, OnLbnSelchangeListTitre)
	ON_LBN_SELCHANGE(IDC_RS_LISTTARIF, OnLbnSelchangeListTarif)
	ON_LBN_SELCHANGE(IDC_RS_LISTCOUPLAGE, OnLbnSelchangeListCPL)

	ON_BN_CLICKED(IDC_RS_BTNSEARCH, OnBnClickedBtnEffacer)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_RS_EDITSEARCH, OnEnChangeSearch)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgTarif

BOOL CDlgTarif::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillListFormatStd();
	FillListFormatSpe();
	FillListCPL();
	//+ RefreshList fait à l'affichage

	SetWindowRelations();
	SetupControls();

	COleDateTime oledate;
	oledate.SetStatus(COleDateTime::null);
	m_DateF.SetTime(oledate);

	return 	TRUE;
}


void CDlgTarif::OnBtnValiderFSpe()
{
	UpdateData();
	srand( (unsigned)time( NULL ) );
	if ( m_IDFormatSpeEdited == 0 )
		m_IDFormatSpeEdited = IDFORMATSPE_MIN + rand();
	m_srcpresse->m_TBLFormats.AddFormatSpe(m_IDFormatSpeEdited, m_NomFormatSpe, m_NomFormatSpe);
	m_NomFormatSpe = "";
	m_IDFormatSpeEdited = 0;
	SaveSel();
	FillListFormatSpe();
	//on met à jour aussi les tarifs au cas ou le format spe est affiché
	FillListTarif( GetSelectedSupport() );
	UpdateData(FALSE);

}
void CDlgTarif::OnBtnGeneseFSpe()
{
	int sel = m_lstFormatSpe.GetCurSel();
	if (sel != LB_ERR)
	{
		JFormatSpe * formatspe = (JFormatSpe*) m_lstFormatSpe.GetItemData( sel );
		m_NomFormatSpe = formatspe->GetLabel().AsJCharPtr();
		m_IDFormatSpeEdited = formatspe->GetID();
		UpdateData(FALSE);
	}
}

void CDlgTarif::OnBtnEffContainer()
{
	m_lstTitre.SetCurSel(-1);
	m_lstCPL.SetCurSel(-1);
	m_lstFormatStd.SetCurSel(-1);
	//raz que si on n'edite pas un formatspe
	if (m_IDFormatSpeEdited == 0)
		m_lstFormatSpe.SetCurSel(-1);
	m_staticTxtFormat.SetText( "" );
	m_DateD.SetTime(COleDateTime::GetCurrentTime());
	m_DateF.SetTime(COleDateTime::GetCurrentTime());
	m_DateF.SetTime();
	m_Tarif.SetDouble(0.0);
	m_pTarifEdited = NULL;
}

void CDlgTarif::OnBtnEffSas()
{
	m_lstFormatStd.SetCurSel(-1);
	m_lstFormatSpe.SetCurSel(-1);
	m_staticTxtFormat.SetText( "" );
	m_DateD.SetTime(COleDateTime::GetCurrentTime());
	m_DateF.SetTime(COleDateTime::GetCurrentTime());
	m_DateF.SetTime();
	m_Tarif.SetDouble(0.0);
	m_pTarifEdited = NULL;
}

void CDlgTarif::OnBtnValider()
{
	UpdateData();
	
	int supportID = GetSelectedSupport();
	int formatID = GetSelectedFormat();
	COleDateTime dateD, dateF;
	m_DateD.GetTime(dateD);
	m_DateF.GetTime(dateF);
	//si un support et un format sont sélectionnés et la date de début est valide
	if (supportID != LB_ERR && formatID != LB_ERR 
		&& ( dateD.GetDay()!=0 && dateD.GetMonth()!=0 && dateD.GetYear()!=0 ) )
	{
		JDate dc, ds;
		dc.SetDate(dateD.GetDay(), dateD.GetMonth(), dateD.GetYear() );
		
		if ( dateF.GetStatus() == COleDateTime::valid )
		{
			if (dateF <= dateD)
				return;
			ds.SetDate(dateF.GetDay(), dateF.GetMonth(), dateF.GetYear() );
		}

		CTarifManuel * tarif = NULL;
		if (m_pTarifEdited)
		{
			tarif = m_pTarifEdited->GetTarifManuel();
			m_pTarifs->m_TarifsManuels.RemoveTarifManuel(tarif);
		}

		tarif = new CTarifManuel(supportID, formatID, (JFlt32)m_Tarif.GetDouble(), dc, ds);
		m_pTarifs->m_TarifsManuels.AddTail() = tarif;
		FillListTarif( supportID );
		FillListSupport();
		//On transmet le message
		AfxGetApp()->m_pMainWnd->PostMessage(ATPMSG_TARIF_UPDATED, 0,0);
	}
	OnBtnEffSas();

}

void CDlgTarif::OnBtnGenese()
{
	OnBtnEffSas();
	int sel = m_lstTarif.GetCurSel();
	if (sel != LB_ERR)
	{
		m_pTarifEdited = (TarifItem*)m_lstTarif.GetItemData(sel);
		CTarifManuel * tarif = m_pTarifEdited->GetTarifManuel();

		///Le support est deja selectionné
		//int idsupport = tarif->GetIDSupport();
		
		//selection du format
		int idformat = tarif->GetIDFormat();
		m_lstFormatStd.SetSelectedID(idformat);
		m_lstFormatSpe.SetSelectedID(idformat);
		m_staticTxtFormat.SetText( m_srcpresse->m_TBLFormats.GetLabelCourtByID(idformat) );
		
		//tarif
		m_Tarif.SetDouble( tarif->GetTarif() );

		JDate jdate;
		JInt32 jj, mm, aaaa;
		COleDateTime oledate;
		
		//date debut
		jdate = tarif->GetDateC();
		jdate.GetDate(jj, mm, aaaa);
		oledate.SetDate(aaaa, mm, jj);
		m_DateD.SetTime(oledate);

		//date fin
		jdate = tarif->GetDateS();
		if (jdate.IsValid())
		{
			jdate.GetDate(jj, mm, aaaa);
			oledate.SetDate(aaaa, mm, jj);
			m_DateF.SetTime(oledate);
		}
		else
		{
			m_DateF.SetTime(COleDateTime::GetCurrentTime());
			m_DateF.SetTime();
		}

	}
}

void CDlgTarif::OnContextMenu(CWnd *pWnd, CPoint point)
{
	CMenu menu;
	CPoint pt;
	pt = point;
	ScreenToClient(&pt);
	menu.LoadMenu(IDR_RS_MENUCTXTREE);

	if ( pWnd->GetSafeHwnd() == m_lstTarif.GetSafeHwnd() && m_lstTarif.GetCount()  )
		menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	if ( pWnd->GetSafeHwnd() == m_lstFormatSpe.GetSafeHwnd() && m_lstFormatSpe.GetCount() )
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

}

void CDlgTarif::OnCtxSupprTarif()
{
	TarifItem * tarifitem = NULL;
	int sel = m_lstTarif.GetCurSel();
	if (sel != LB_ERR)
	{
		tarifitem = (TarifItem*)m_lstTarif.GetItemData( sel );
		CTarifManuel* tarif = tarifitem->GetTarifManuel();
		m_pTarifs->m_TarifsManuels.RemoveTarifManuel(tarif);
		FillListTarif( GetSelectedSupport() );
		FillListSupport();
		//On transmet le message
		AfxGetApp()->m_pMainWnd->PostMessage(ATPMSG_TARIF_UPDATED, 0,0);
	}
	OnBtnEffSas();
}

void CDlgTarif::OnCtxRename()
{
	int sel = m_lstFormatSpe.GetCurSel();
	if (sel != LB_ERR)
	{
		JFormatSpe * formatspe = (JFormatSpe*) m_lstFormatSpe.GetItemData( sel );
		CDlgRename dlg;
		dlg.m_Libelle = formatspe->GetLabel().AsJCharPtr();
		if (dlg.DoModal() == IDOK)
		{
			formatspe->SetLabel(dlg.m_Libelle);
			formatspe->SetLabelCourt(dlg.m_Libelle);
			FillListFormatSpe();
		}
	}
	OnBtnEffSas();


}

void CDlgTarif::OnCtxSupprFSpe()
{
	//on suprimme le format spe des formats spe mais aussi des tarif
	int sel = m_lstFormatSpe.GetCurSel();
	JFormatSpe * formatspe = (JFormatSpe*) m_lstFormatSpe.GetItemData( sel );
	
	
	//on retire les liaisons dans les tarifs
	for (m_pTarifs->m_TarifsManuels.MoveFirst();
		m_pTarifs->m_TarifsManuels.IsValid();
		m_pTarifs->m_TarifsManuels.MoveNext() )
	{
		CTarifManuel* tarif = (CTarifManuel*)m_pTarifs->m_TarifsManuels.GetItem();
		//si le tarif correspond au format selectionné
		if ( tarif->GetIDFormat() == formatspe->GetID() )
			m_pTarifs->m_TarifsManuels.RemoveTarifManuel(tarif);
	}

	m_srcpresse->m_TBLFormats.RemFormatSpe( formatspe->GetID() );
	//deja fait par RemFormatSpe	//delete formatspe;

	FillListFormatSpe();
	FillListTarif( GetSelectedSupport() );
	FillListSupport();
	OnBtnEffSas();
}

void CDlgTarif::OnLbnSelchangeListFormatStd()
{
	int sel = m_lstFormatStd.GetCurSel();
	//selection exclusive
	m_lstFormatSpe.SetCurSel(-1);
	if (sel != LB_ERR)
	{
		JFormat * format = (JFormat*) m_lstFormatStd.GetItemData( sel );
		m_staticTxtFormat.SetText( format->GetLabelCourt().AsJCharPtr() );
	}
}

void CDlgTarif::OnLbnSelchangeListFormatSpe()
{
	int sel = m_lstFormatSpe.GetCurSel();
	//selection exclusive
	m_lstFormatStd.SetCurSel(-1);
	if (sel != LB_ERR)
	{
		JFormat * format = (JFormat*) m_lstFormatSpe.GetItemData( sel );
		m_staticTxtFormat.SetText( format->GetLabelCourt().AsJCharPtr() );
	}
}

void CDlgTarif::OnLbnSelchangeListTitre()
{
	//selection exclusive
	m_lstCPL.SetCurSel(-1);
	m_lstFormatStd.Invalidate();
	FillListTarif( m_lstTitre.GetSelectedID() );
}

void CDlgTarif::OnLbnSelchangeListTarif()
{
}

void CDlgTarif::OnLbnSelchangeListCPL()
{
	//selection exclusive
	m_lstTitre.SetCurSel(-1);
	FillListTarif( m_lstCPL.GetSelectedID() );
}

void CDlgTarif::FillListSupport()
{
	SaveSel();
	FillListTitre();
	FillListCPL();
}

void CDlgTarif::RefreshList()
{
	FillListTitre();
	FillListCPL();

	m_lstTarif.SetCurSel(m_selTarif);
}

void CDlgTarif::FillListFormatStd()
{
	m_lstFormatStd.ResetContent();
	for (m_srcpresse->m_TBLFormats.MoveFirst();
		m_srcpresse->m_TBLFormats.IsValid();
		m_srcpresse->m_TBLFormats.MoveNext() )
	{
		int ID = m_srcpresse->m_TBLFormats.GetItem()->GetID();
		if ( ID < IDFORMATSPE_MIN )
			m_lstFormatStd.AddItem ( (JFormat*)m_srcpresse->m_TBLFormats.GetItem() );
	}
	m_lstFormatStd.SetSelectedID(m_selFormatID);
}

void CDlgTarif::FillListFormatSpe()
{
	m_lstFormatSpe.ResetContent();
	for (m_srcpresse->m_TBLFormats.MoveFirst();
		m_srcpresse->m_TBLFormats.IsValid();
		m_srcpresse->m_TBLFormats.MoveNext() )
	{
		int ID = m_srcpresse->m_TBLFormats.GetItem()->GetID();
		if ( ID > IDFORMATSPE_MIN && ID < IDFORMATSPE_MAX )
			m_lstFormatSpe.AddItem ( (JFormatSpe*)m_srcpresse->m_TBLFormats.GetItem() );
	}
	m_lstFormatSpe.SetSelectedID(m_selFormatID);
}

void CDlgTarif::FillListTitre()
{
	//On vide la liste
	m_lstTitre.ResetContent();
	m_lstTitre.m_mapIDRef.Reset();
	//tous les titres + apparentés
	//les titres
	for (m_srcpresse->m_TBLTitres.MoveFirst();
		m_srcpresse->m_TBLTitres.IsValid();
		m_srcpresse->m_TBLTitres.MoveNext() )
	{
		IElem * titre = m_srcpresse->m_TBLTitres.GetItem();
		if (CSearchAlgo::Contains(titre->GetLabel().AsJCharPtr(), m_searchStr) && !titre->AsITitre()->IsAgregat())
		{
			m_lstTitre.AddItem ( titre );
			m_lstTitre.m_mapIDRef.Add(titre->GetID(), true) = 0;
			
			for (m_pTarifs->m_TarifsManuels.MoveFirst();
				m_pTarifs->m_TarifsManuels.IsValid();
				m_pTarifs->m_TarifsManuels.MoveNext() )
			{
				CTarifManuel* trf = (CTarifManuel*)m_pTarifs->m_TarifsManuels.GetItem();
				//si le tarif correspond au support selectionné
				if ( trf->GetIDSupport() == titre->GetID() )
					m_lstTitre.m_mapIDRef.GetItem() ++;
			}
		}

	}

	//les apparentés
	JMap<JInt32, JInt32> mapIDdone;
	for (m_pRegroupements->GetTitresApparentes()->MoveFirst();
		m_pRegroupements->GetTitresApparentes()->IsValid();
		m_pRegroupements->GetTitresApparentes()->MoveNext() )
	{
		CTitreApparente* titreapp =(CTitreApparente*)m_pRegroupements->GetTitresApparentes()->GetItem();
		if (CSearchAlgo::Contains(titreapp->GetLabel().AsJCharPtr(), m_searchStr))
		{
			mapIDdone.MoveTo( titreapp->GetID() );
			if ( !mapIDdone.IsValid() )
			{
				m_lstTitre.AddItem ( titreapp );
				mapIDdone.Add(titreapp->GetID());
				
				m_lstTitre.m_mapIDRef.Add(titreapp->GetID(), true) = 0;
				for (m_pTarifs->m_TarifsManuels.MoveFirst();
					m_pTarifs->m_TarifsManuels.IsValid();
					m_pTarifs->m_TarifsManuels.MoveNext() )
				{
					CTarifManuel* trf = (CTarifManuel*)m_pTarifs->m_TarifsManuels.GetItem();
					//si le tarif correspond au support selectionné
					if ( trf->GetIDSupport() == titreapp->GetID() )
						m_lstTitre.m_mapIDRef.GetItem() ++;
				}
			}
		}
	}
	m_lstTitre.SetSelectedID(m_selSupportID);
}

void CDlgTarif::FillListCPL()
{
	m_lstCPL.ResetContent();
	m_lstCPL.m_mapIDRef.Reset();
	//for (m_srcpresse->m_TBLCouplagesFixes.MoveFirst();
	//	m_srcpresse->m_TBLCouplagesFixes.IsValid();
	//	m_srcpresse->m_TBLCouplagesFixes.MoveNext() )
	//{
	//	JCouplageFixe* cFixe = (JCouplageFixe*)m_srcpresse->m_TBLCouplagesFixes.GetItem();
	//	m_lstCPL.AddItem (cFixe);

	//	m_lstCPL.m_mapIDRef.Add(cFixe->GetID(), true) = 0;
	//	for (m_pTarifs->m_TarifsManuels.MoveFirst();
	//		m_pTarifs->m_TarifsManuels.IsValid();
	//		m_pTarifs->m_TarifsManuels.MoveNext() )
	//	{
	//		CTarifManuel* trf = (CTarifManuel*)m_pTarifs->m_TarifsManuels.GetItem();
	//		//si le tarif correspond au support selectionné
	//		if ( trf->GetIDSupport() == cFixe->GetID() )
	//			m_lstCPL.m_mapIDRef.GetItem() ++;
	//	}
	//}
/*	m_lstCFixe.SetSelectedID(m_selSupportID);
}

void CDlgTarif::FillListCLibre()
{

	m_lstCLibre.ResetContent();
	m_lstCLibre.m_mapIDRef.Reset();
*/	
	for (m_pRegroupements->GetCouplagesLibres()->MoveFirst();
		m_pRegroupements->GetCouplagesLibres()->IsValid();
		m_pRegroupements->GetCouplagesLibres()->MoveNext() )
	{
		CCouplageLibre* cLibre = (CCouplageLibre*)m_pRegroupements->GetCouplagesLibres()->GetItem();
		m_lstCPL.AddItem (cLibre);

		m_lstCPL.m_mapIDRef.Add(cLibre->GetID(), true) = 0;
		for (m_pTarifs->m_TarifsManuels.MoveFirst();
			m_pTarifs->m_TarifsManuels.IsValid();
			m_pTarifs->m_TarifsManuels.MoveNext() )
		{
			CTarifManuel* trf = (CTarifManuel*)m_pTarifs->m_TarifsManuels.GetItem();
			//si le tarif correspond au support selectionné
			if ( trf->GetIDSupport() == cLibre->GetID() )
				m_lstCPL.m_mapIDRef.GetItem() ++;
		}
	}
	m_lstCPL.SetSelectedID(m_selSupportID);
}

void CDlgTarif::FillListTarif(int ID)
{
	//OnBtnEffSas();
	//on vide la liste
	for (int i=0; i<m_lstTarif.GetCount(); i++)
		delete (TarifItem*)m_lstTarif.GetItemData(i);
	m_lstTarif.ResetContent();

	//pour la remplir
	for (m_pTarifs->m_TarifsManuels.MoveFirst();
		m_pTarifs->m_TarifsManuels.IsValid();
		m_pTarifs->m_TarifsManuels.MoveNext() )
	{
		CTarifManuel* trf = (CTarifManuel*)m_pTarifs->m_TarifsManuels.GetItem();
		//si le tarif correspond au support selectionné
		if ( trf->GetIDSupport() == ID )
		{
			CString lbl;
			if (trf->GetDateS().IsValid())
				lbl.FormatMessage(IDS_RS_TRFDTOD, m_srcpresse->m_TBLFormats.GetLabelByID(trf->GetIDFormat()), (int)trf->GetTarif(), CFormatStr::FormatDate(trf->GetDateC()), CFormatStr::FormatDate(trf->GetDateS()));
			else
				lbl.FormatMessage(IDS_RS_TRFD, m_srcpresse->m_TBLFormats.GetLabelByID(trf->GetIDFormat()), (int)trf->GetTarif(), CFormatStr::FormatDate(trf->GetDateC()));
			TarifItem* tItem = new TarifItem(m_pTarifs->m_TarifsManuels.GetCount()+1, lbl, trf);
			m_lstTarif.AddItem ( tItem );
		}
	}

}

int CDlgTarif::GetSelectedSupport()
{
	int supportID = m_lstTitre.GetSelectedID();
	if (supportID == LB_ERR)
		supportID = m_lstCPL.GetSelectedID();
	//if (supportID == LB_ERR)
	//	supportID = m_lstCFixe.GetSelectedID();
	return supportID;
}

int CDlgTarif::GetSelectedFormat()
{
	int formatID = m_lstFormatStd.GetSelectedID();
	if (formatID == LB_ERR)
		formatID = m_lstFormatSpe.GetSelectedID();
	return formatID;
}


void CDlgTarif::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
		RefreshList();
	else
	{
		SaveSel();
		OnBtnEffSas();
	}

}

void CDlgTarif::SaveSel()
{
	m_selSupportID = GetSelectedSupport();
	m_selFormatID = GetSelectedFormat();
	m_selTarif = m_lstTarif.GetCurSel();
}

void CDlgTarif::ResetSel()
{
	m_selSupportID = -1;
	m_selFormatID = -1;
	m_selTarif = -1;
}

void CDlgTarif::OnDestroy()
{
	//on vide la liste des tarifs
	for (int i=0; i<m_lstTarif.GetCount(); i++)
		delete (TarifItem*)m_lstTarif.GetItemData(i);

	CDialog::OnDestroy();
}

void CDlgTarif::OnEnChangeSearch()
{
	SetTimer(IdTimer_Search, 750, NULL);
}

void CDlgTarif::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == IdTimer_Search)
	{
		KillTimer(IdTimer_Search);
		UpdateData();
		FillListTitre();
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgTarif::OnBnClickedBtnEffacer()
{
	m_searchStr = "";
	UpdateData(FALSE);
	FillListTitre();
	GetDlgItem(IDC_RS_EDITSEARCH)->SetFocus();
}

void CDlgTarif::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
}

void CDlgTarif::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
			{IDC_RS_STATICTITRE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

			{IDC_RS_STATICFORMAT,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_MAIN, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side
	
			{IDC_RS_STATICCUSTOM,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side
	
					
			{IDC_RS_STATICCOUPLAGE,	{CResizer::eFixed, IDC_MAIN, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side

	/*		{IDC_RS_STATICCLIBRE,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCFIXE, CResizer::eBottom},  //Top side
								{CResizer::eFixed, IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_MAIN, CResizer::eBottom}}, //Bottom side
	*/
///	
			{IDC_RS_STATICTTLFORMATSTD,	{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eLeft}, //Left side
										{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eTop},  //Top side
										{CResizer::eProportional, IDC_RS_STATICFORMAT, CResizer::eRight}, //Right side
										{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eTop}}, //Bottom side
			{IDC_RS_LISTFORMATSTD,	{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTTLFORMATSTD, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICFORMAT, CResizer::eRight}, //Right side
								{CResizer::eProportional, IDC_RS_STATICFORMAT, CResizer::eBottom}}, //Bottom side
////
			{IDC_RS_STATICTTLFORMATSPE,	{CResizer::eFixed, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Left side
										{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eTop},  //Top side
										{CResizer::eProportional, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Right side
										{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eTop}}, //Bottom side
			{IDC_RS_EDITFORMATSPE,	{CResizer::eFixed, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTTLFORMATSPE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTTLFORMATSPE, CResizer::eBottom}}, //Bottom side
			{IDC_RS_BTNVALIDFSPE,	{CResizer::eFixed, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_EDITFORMATSPE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_EDITFORMATSPE, CResizer::eBottom}}, //Bottom side
			{IDC_RS_LISTFORMATSPE,	{CResizer::eFixed, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Left side
								{CResizer::eProportional, IDC_RS_BTNVALIDFSPE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICTTLFORMATSTD, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICFORMAT, CResizer::eBottom}}, //Bottom side
			//{IDC_RS_BTNGENESEFSPE,	{CResizer::eFixed, IDC_RS_BTNVALIDFSPE, CResizer::eRight}, //Left side
			//					{CResizer::eFixed, IDC_RS_EDITFORMATSPE, CResizer::eBottom},  //Top side
			//					{CResizer::eProportional, IDC_RS_STATICFORMAT, CResizer::eRight}, //Right side
			//					{CResizer::eFixed, IDC_RS_EDITFORMATSPE, CResizer::eBottom}}, //Bottom side
////
			{IDC_RS_STATICTTLTITRE,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eTop}}, //Bottom side
			{IDC_RS_LISTTITRE,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTTLTITRE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eBottom}}, //Bottom side
			
			{IDC_RS_EDITSEARCH,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom}}, //Bottom side

			{IDC_RS_BTNSEARCH,	{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom},  //Top side
								{CResizer::eFixed, IDC_RS_STATICTITRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_LISTTITRE, CResizer::eBottom}}, //Bottom side
////
////
			{IDC_RS_STATICTTLCOUPLAGE,	{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCOUPLAGE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eTop}}, //Bottom side
			{IDC_RS_LISTCOUPLAGE,	{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTTLCOUPLAGE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCOUPLAGE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCOUPLAGE, CResizer::eBottom}}, //Bottom side
////
////
	/*		{IDC_RS_STATICTTLCLIBRE,	{CResizer::eFixed, IDC_RS_STATICCLIBRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCLIBRE, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCLIBRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCLIBRE, CResizer::eTop}}, //Bottom side
			{IDC_RS_LISTCLIBRE,	{CResizer::eFixed, IDC_RS_STATICCLIBRE, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICTTLCLIBRE, CResizer::eBottom},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCLIBRE, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCLIBRE, CResizer::eBottom}}, //Bottom side

								*/
////
			{IDC_RS_STATICTTLFORMAT,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_STATICTTLTARIF,	{CResizer::eFixed, IDC_RS_STATICTTLFORMAT, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_STATICTTLDATED,	{CResizer::eFixed, IDC_RS_STATICTTLTARIF, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_STATICTTLDATEF,	{CResizer::eFixed, IDC_RS_STATICTTLDATED, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_STATICTXTFORMAT,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_EDITTARIF,	{CResizer::eFixed, IDC_RS_STATICTXTFORMAT, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_EDITDATED,	{CResizer::eFixed, IDC_RS_EDITTARIF, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_EDITDATEF,	{CResizer::eFixed, IDC_RS_EDITDATED, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_BTNVALID,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_BTNEFFCONTAINER,	{CResizer::eFixed, IDC_RS_BTNVALID, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_BTNEFFSAS,	{CResizer::eFixed, IDC_RS_BTNEFFCONTAINER, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_BTNGENESE,	{CResizer::eFixed, IDC_RS_BTNEFFSAS, CResizer::eRight}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
			{IDC_RS_LISTTARIF,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eBottom}}, //Bottom side
//*/
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);

}

void CDlgTarif::SetupControls()
{
	//subclass 
	m_DateD.SubclassDlgItem(IDC_RS_EDITDATED, this);
	m_DateF.SubclassDlgItem(IDC_RS_EDITDATEF, this);
	m_Tarif.SubclassDlgItem(IDC_RS_EDITTARIF, this);

	m_staticCustom.SubclassDlgItem(IDC_RS_STATICCUSTOM, this);
	m_staticFormat.SubclassDlgItem(IDC_RS_STATICFORMAT, this);
	m_staticTitre.SubclassDlgItem(IDC_RS_STATICTITRE, this);
	m_staticCPL.SubclassDlgItem(IDC_RS_STATICCOUPLAGE, this);
//	m_staticCLibre.SubclassDlgItem(IDC_RS_STATICCLIBRE, this);

	m_lblTtlFormatStd.SubclassDlgItem(IDC_RS_STATICTTLFORMATSTD, this);
	m_lblTtlFormatSpe.SubclassDlgItem(IDC_RS_STATICTTLFORMATSPE, this);
	m_lblTtlTitre.SubclassDlgItem(IDC_RS_STATICTTLTITRE, this);
	m_lblTtlCPL.SubclassDlgItem(IDC_RS_STATICTTLCOUPLAGE, this);
//	m_lblTtlCLibre.SubclassDlgItem(IDC_RS_STATICTTLCLIBRE, this);
	m_lblTtlFormat.SubclassDlgItem(IDC_RS_STATICTTLFORMAT, this);
	m_lblTtlTarif.SubclassDlgItem(IDC_RS_STATICTTLTARIF, this);
	m_lblTtlDateD.SubclassDlgItem(IDC_RS_STATICTTLDATED, this);
	m_lblTtlDateF.SubclassDlgItem(IDC_RS_STATICTTLDATEF, this);

	m_staticTxtFormat.SubclassDlgItem(IDC_RS_STATICTXTFORMAT, this);

	m_btnValiderFSpe.SubclassDlgItem(IDC_RS_BTNVALIDFSPE, this);
//	m_btnGeneseFSpe.SubclassDlgItem(IDC_RS_BTNGENESEFSPE, this);
	
	m_btnValider.SubclassDlgItem(IDC_RS_BTNVALID, this);
	m_btnEffContainer.SubclassDlgItem(IDC_RS_BTNEFFCONTAINER, this);
	m_btnEffSas.SubclassDlgItem(IDC_RS_BTNEFFSAS, this);
	m_btnGenese.SubclassDlgItem(IDC_RS_BTNGENESE, this);
	m_btnEffSearch.SubclassDlgItem(IDC_RS_BTNSEARCH, this);

	m_staticCustom.SetTransparent(false);
	m_staticCustom.SetSunken(true);
	m_staticCustom.AddTopWindow(m_lblTtlFormat);
	m_staticCustom.AddTopWindow(m_staticTxtFormat);
	m_staticCustom.AddTopWindow(m_lblTtlTarif);
	m_staticCustom.AddTopWindow(m_Tarif);
	m_staticCustom.AddTopWindow(m_lblTtlDateD);
	m_staticCustom.AddTopWindow(m_DateD);
	m_staticCustom.AddTopWindow(m_lblTtlDateF);
	m_staticCustom.AddTopWindow(m_DateF);
	m_staticCustom.AddTopWindow(m_btnValider);
	m_staticCustom.AddTopWindow(m_btnEffContainer);
	m_staticCustom.AddTopWindow(m_btnEffSas);
	m_staticCustom.AddTopWindow(m_btnGenese);
	m_staticCustom.AddTopWindow(m_lstTarif);

	m_staticFormat.SetTransparent(false);
	m_staticFormat.SetSunken(true);
	m_staticFormat.AddTopWindow(m_lblTtlFormatStd);
	m_staticFormat.AddTopWindow(m_lstFormatStd);
	m_staticFormat.AddTopWindow(m_lblTtlFormatSpe);
	m_staticFormat.AddTopWindow(m_lstFormatSpe);
	m_staticFormat.AddTopWindow(m_btnValiderFSpe);
//	m_staticFormat.AddTopWindow(m_btnGeneseFSpe);
	m_staticFormat.AddTopWindow( GetDlgItem(IDC_RS_EDITFORMATSPE)->GetSafeHwnd() );

	m_staticTitre.SetTransparent(false);
	m_staticTitre.SetSunken(true);
	m_staticTitre.AddTopWindow(m_lblTtlTitre);
	m_staticTitre.AddTopWindow(m_lstTitre);
	m_staticTitre.AddTopWindow(GetDlgItem(IDC_RS_EDITSEARCH)->GetSafeHwnd());
	m_staticTitre.AddTopWindow(m_btnEffSearch);

	m_staticCPL.SetTransparent(false);
	m_staticCPL.SetSunken(true);
	m_staticCPL.AddTopWindow(m_lblTtlCPL);
	m_staticCPL.AddTopWindow(m_lstCPL);
	
	//m_staticCLibre.SetTransparent(false);
	//m_staticCLibre.SetSunken(true);
	//m_staticCLibre.AddTopWindow(m_lblTtlCLibre);
	//m_staticCLibre.AddTopWindow(m_lstCLibre);


	m_lblTtlFormatStd.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlFormatStd.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlFormatStd.SetBorder(false);

	m_lblTtlFormatSpe.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlFormatSpe.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlFormatSpe.SetBorder(false);

	m_lblTtlTitre.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTitre.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTitre.SetBorder(false);

	m_lblTtlCPL.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlCPL.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlCPL.SetBorder(false);

	//m_lblTtlCLibre.SetFontName(CATPColors::GetFontLabelName());
	//m_lblTtlCLibre.SetFontSize(CATPColors::GetFontLabelSize());
	//m_lblTtlCLibre.SetBorder(false);

	m_lblTtlFormat.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlFormat.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlFormat.SetBorder(false);

	m_lblTtlTarif.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTarif.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTarif.SetBorder(false);

	m_lblTtlDateD.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlDateD.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlDateD.SetBorder(false);

	m_lblTtlDateF.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlDateF.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlDateF.SetBorder(false);

	m_staticTxtFormat.SetFontName(CATPColors::GetFontStaticName());
	m_staticTxtFormat.SetFontSize(CATPColors::GetFontStaticSize());
	m_staticTxtFormat.SetBorder(true);
	m_staticTxtFormat.SetText("");

	SetColors();
}

void CDlgTarif::SetColors()
{
	m_lstFormatStd.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstFormatStd.SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstFormatSpe.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTitre.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTitre.SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstCPL.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstCPL.SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );
	m_lstTarif.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );

	m_staticCustom.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticFormat.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticTitre.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticCPL.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);

	m_lblTtlFormatStd.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlFormatStd.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlFormatStd.SetFontBold(true);

	m_lblTtlFormatSpe.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlFormatSpe.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlFormatSpe.SetFontBold(true);

	m_lblTtlTitre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTitre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTitre.SetFontBold(true);

	m_lblTtlCPL.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlCPL.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlCPL.SetFontBold(true);

	m_lblTtlFormat.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlFormat.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlFormat.SetFontBold(true);

	m_lblTtlTarif.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTarif.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTarif.SetFontBold(true);

	m_lblTtlDateD.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlDateD.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlDateD.SetFontBold(true);

	m_lblTtlDateF.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlDateF.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlDateF.SetFontBold(true);

	m_staticTxtFormat.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);

	m_DateD.SetMonthCalColor(MCSC_TITLEBK, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS));
	m_DateF.SetMonthCalColor(MCSC_TITLEBK, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS));

	m_btnValider.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnValider.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnValider.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
						 
	m_btnEffContainer.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffContainer.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffContainer.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnEffSas.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffSas.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffSas.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
						 
	m_btnGenese.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnGenese.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnGenese.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
						
	m_btnValiderFSpe.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnValiderFSpe.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnValiderFSpe.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
							 
	m_btnEffSearch.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffSearch.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnEffSearch.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	//2 chiffres après le virgule et pas de négatif
	m_Tarif.SetMaxDecimalPlaces(2);
	m_Tarif.AllowNegative(false);
}

