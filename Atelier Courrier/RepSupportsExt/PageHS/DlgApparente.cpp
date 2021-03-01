// DlgApparente.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "..\Resource.h"
#include "DlgApparente.h"
#include "JFCConfig.h"

// Boîte de dialogue CDlgApparente

IMPLEMENT_DYNAMIC(CDlgApparente, CDlgFCP)

CDlgApparente::CDlgApparente(CWnd* pParent /*=NULL*/)
{
	// TODO : verifier si IDC_RS_EDITNOM est utilsé ailleurs, car ici ce n'est plus le cas, option GarsCon
	m_AfficheApparentes = false;
	m_MediaWeight = 100;
	m_IDTA = 0;
	m_plstTitre = new TitreInXListBox();
	m_pTitreTuteur = NULL;
}

CDlgApparente::~CDlgApparente()
{
	delete m_plstTitre;
}

void CDlgApparente::DoDataExchange(CDataExchange* pDX)
{
	CDlgFCP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RS_LISTAPPARENTE, m_lstApparente);
	DDX_Text(pDX, IDC_RS_EDITMW, m_MediaWeight);
}


BEGIN_MESSAGE_MAP(CDlgApparente, CDlgFCP)
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_RS_CTXLST_SUPPR, OnCtxSuppr)
	ON_COMMAND(IDM_RS_CTXLST_RENAME, OnCtxRename)
	ON_LBN_SELCHANGE(IDC_RS_LISTTITRE, OnLbnSelchangeListTitre)
	ON_BN_CLICKED(IDC_RS_BTNVALID, OnBtnValider)
	ON_BN_CLICKED(IDC_RS_BTNEFFSAS, OnBtnEffSas)
	ON_BN_CLICKED(IDC_RS_BTNGENESE, OnBtnGenese)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgApparente

BOOL CDlgApparente::OnInitDialog()
{
	CDlgFCP::OnInitDialog();

	SetupControls();
	SetWindowRelations();
	return 	TRUE;
}

void CDlgApparente::FillListTitreApparente()
{
	m_lstApparente.ResetContent();
	JMap<JInt32, JInt32> mapIDdone;
	for (m_pRegroupements->GetTitresApparentes()->MoveFirst();
		m_pRegroupements->GetTitresApparentes()->IsValid();
		m_pRegroupements->GetTitresApparentes()->MoveNext() )
	{
		CTitreApparente* titreapp =(CTitreApparente*)m_pRegroupements->GetTitresApparentes()->GetItem();
		//on elimime les redondances
		mapIDdone.MoveTo( titreapp->GetID() );
		if ( !mapIDdone.IsValid() )
		{
            m_lstApparente.AddItem ( titreapp );
			mapIDdone.Add(titreapp->GetID());
			//initialisation du compteur de ref
			m_plstTitre->m_mapIDRef.MoveTo( titreapp->GetSupportTuteur() );
			//s'il n'a pas été ajouté on l'ajoute sinon on incremente
			if ( ! m_plstTitre->m_mapIDRef.IsValid() )
				m_plstTitre->m_mapIDRef.Add(titreapp->GetSupportTuteur()) = 1;
			else
				m_plstTitre->m_mapIDRef.GetItem() ++;
		}

	}
}

void CDlgApparente::OnContextMenu(CWnd *pWnd, CPoint point)
{
	if ( pWnd->GetSafeHwnd() == m_lstApparente.GetSafeHwnd() )
	{
		CMenu menu;
		CPoint pt;
		pt = point;
		ScreenToClient(&pt);

		menu.LoadMenu(IDR_RS_MENUCTXTREE);
		menu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CDlgApparente::OnCtxSuppr()
{
	CTitreApparente * titreapparente = NULL;
	int sel = m_lstApparente.GetCurSel();
	if (sel != LB_ERR)
	{
		titreapparente = (CTitreApparente*)m_lstApparente.GetItemData( sel );
		m_plstTitre->m_mapIDRef.MoveTo( titreapparente->GetSupportTuteur() );
		//on decremente
		if ( m_plstTitre->m_mapIDRef.IsValid() )
			m_plstTitre->m_mapIDRef.GetItem() --;
		SaveSel();
		m_plstTitre->Invalidate();
		//FillListTitre();
		
		CVisiteurDeleteTA vDelTA;
		titreapparente->Accept(&vDelTA);
		FillListTitreApparente();
	}
	OnBtnEffSas();
}

void CDlgApparente::OnCtxRename()
{
	CTitreApparente * titreapparente = NULL;
	int sel = m_lstApparente.GetCurSel();
	if (sel != LB_ERR)
	{
		titreapparente = (CTitreApparente*)m_lstApparente.GetItemData( sel );
		CDlgRename dlg;
		dlg.m_Libelle = titreapparente->GetLabelForEdit();
		if (dlg.DoModal() == IDOK)
		{
			titreapparente->SetLabel(dlg.m_Libelle);
			FillListTitreApparente();
		}
	}
}

void CDlgApparente::OnLbnSelchangeListTitre()
{
	int sel = m_plstTitre->GetCurSel();
	if (sel != LB_ERR)
	{
		m_pTitreTuteur = (CTitre*) m_plstTitre->GetItemData( sel );
		m_TitreTuteur.SetText( m_pTitreTuteur->GetLabel().AsJCharPtr() );
		m_Nom.Empty();
		m_MediaWeight = 100;
		m_IDTA = 0;
		m_lstApparente.SetCurSel(-1);
		m_plstTitre->SetCurSel(-1);
		UpdateData(FALSE);
	}
}


void CDlgApparente::OnBtnEffSas()
{
	m_MediaWeight = 100;
	m_TitreTuteur.SetText("");
	m_Nom = "";
	UpdateData(FALSE);
	m_IDTA = 0;
	m_pTitreTuteur = NULL;
}

void CDlgApparente::OnBtnValider()
{
	UpdateData();
	///validation des données
	//if (m_Nom.IsEmpty())
	//	return;
	if ( 1>m_MediaWeight || m_MediaWeight>500)
	{
		AfxMessageBox( IDS_RS_MWOUTBOUNDS, MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (m_pTitreTuteur)
	{
		CTitreApparente* titreapp = NULL;
		if (m_IDTA)
		{
			//update, il faut mettre à jour tous les TA
			JNodeList* lstTA = m_pRegroupements->GetTitresApparentes();
			titreapp = (CTitreApparente*)lstTA->GetNodeByID(m_IDTA);
			while (titreapp)
			{
				titreapp->SetMediaWeight(m_MediaWeight);
				titreapp->SetLabel(m_Nom);
				titreapp = (CTitreApparente*)lstTA->GetNextNodeByID(m_IDTA);
			}
		}
		else
		{
			srand( (unsigned)time( NULL ) );
			titreapp = new CTitreApparente(
				IDTITREAPPARENTE_MIN + rand(), m_Nom, m_pTitreTuteur->GetID(), m_MediaWeight, m_pTitreTuteur->GetParentID() );
			CVisiteurInsert vInsert;
			titreapp->Accept(&vInsert);
		}
		FillListTitreApparente();
		OnBtnEffContainer();
		m_IDTA = 0;
		AfxMessageBox(IDS_RS_TARIFS, MB_ICONINFORMATION);
	}
}

void CDlgApparente::OnBtnGenese()
{
	CTitreApparente * titreapparente = NULL;
	int sel = m_lstApparente.GetCurSel();
	if (sel != LB_ERR)
	{
		titreapparente = (CTitreApparente*)m_lstApparente.GetItemData( sel );
		//On rempli les zones d'édition correspondant au titre
		m_MediaWeight = titreapparente->GetMediaWeight();
		m_IDTA = titreapparente->GetID();
		int idSupport = titreapparente->GetSupportTuteur();
		m_TitreTuteur.SetText( m_srcpresse->m_TBLTitres.GetLabelByID(idSupport) );
		//maj
		UpdateData(FALSE);
	
		//recupération du père
		INode * pere = (INode*)titreapparente->GetParentNode();
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
		m_selPeriodID.AddTail() = m_srcpresse->m_TBLTitres.GetPeriodByID(idSupport);

		//on selectionne le titre
		RefreshList();
		m_plstTitre->SetSelectedID(idSupport);
		int sel = m_plstTitre->GetCurSel();
		if (sel != LB_ERR)
			m_pTitreTuteur = (CTitre*) m_plstTitre->GetItemData( sel );
	}
}

void CDlgApparente::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDlgFCP::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		FillListTitreApparente();
		m_TitreTuteur.SetText("");
		int sel = m_plstTitre->GetCurSel();
		if (sel != LB_ERR)
		{
			m_pTitreTuteur = (CTitre*) m_plstTitre->GetItemData( sel );
			m_TitreTuteur.SetText( m_pTitreTuteur->GetLabel().AsJCharPtr() );
		}
	}
	else
		OnBtnEffSas();

}

void CDlgApparente::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {
	
	
	{IDC_RS_STATICTTLTTUTEUR,{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_STATICTTLCUSTOM,	{CResizer::eFixed, IDC_RS_STATICTTLTTUTEUR, CResizer::eRight}, //Left side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
						{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	
	{IDC_RS_STATICTXTTITRETUTEUR,	{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
								{CResizer::eProportional, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
								{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_EDITNOM,			{CResizer::eFixed, IDC_RS_STATICTXTTITRETUTEUR, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side

	{IDC_RS_EDITMW,			{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eProportional, IDC_RS_STATICTXTNBMW, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop}}, //Bottom side
	
	{IDC_RS_LISTAPPARENTE,		{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eLeft}, //Left side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eTop},  //Top side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eRight}, //Right side
							{CResizer::eFixed, IDC_RS_STATICCUSTOM, CResizer::eBottom}}, //Bottom side

	
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Add(s_bi, nSize);
}

void CDlgApparente::SetupControls()
{
	m_plstTitre->SubclassDlgItem(IDC_RS_LISTTITRE, this);

	m_lblTtlTTuteur.SubclassDlgItem(IDC_RS_STATICTTLTTUTEUR, this);
	m_TitreTuteur.SubclassDlgItem(IDC_RS_STATICTXTTITRETUTEUR, this);

	m_staticCustom.AddTopWindow(m_lblTtlTTuteur);
	m_staticCustom.AddTopWindow(m_lstApparente);
	m_staticCustom.AddTopWindow(m_TitreTuteur);
    m_staticCustom.AddTopWindow(GetDlgItem(IDC_RS_EDITMW)->GetSafeHwnd());

	m_lblTtlTTuteur.SetFontName(CATPColors::GetFontLabelName());
	m_lblTtlTTuteur.SetFontSize(CATPColors::GetFontLabelSize());
	m_lblTtlTTuteur.SetBorder(false);

	m_TitreTuteur.SetFontName(CATPColors::GetFontStaticName());
	m_TitreTuteur.SetFontSize(CATPColors::GetFontStaticSize());
	m_TitreTuteur.SetBorder(true);
	m_TitreTuteur.SetText("");

	SetColors();
}

void CDlgApparente::SetColors()
{
	CDlgFCP::SetColors();
	m_plstTitre->SetSelectionColor(NORMAL, CATPColors::GetColorWhite() );
	m_plstTitre->SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_plstTitre->SetAttribueColor(CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS));
	m_lstApparente.SetSelectionColor(NORMAL, CATPColors::GetColorSelect(CATPColors::COLORREPSUPPORTS) );

	m_lblTtlTTuteur.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS), CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblTtlTTuteur.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));
	m_lblTtlTTuteur.SetFontBold(true);

	m_TitreTuteur.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
}
