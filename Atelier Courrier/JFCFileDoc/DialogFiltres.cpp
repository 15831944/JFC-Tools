// DialogFiltres.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DialogFiltres.h"
#include "ATPColors.h"
#include "ATPMessages.h"

// Boîte de dialogue CDialogFiltres

IMPLEMENT_DYNAMIC(CDialogFiltres, CDialog)
CDialogFiltres::CDialogFiltres(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFiltres::IDD, pParent)
{
	m_pATPDocument = 0;
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
}

CDialogFiltres::~CDialogFiltres()
{
}


void CDialogFiltres::SetDocument(JATPDocument* pATPDocument)
{
	// on mémorise le document
	m_pATPDocument = pATPDocument;

	// on met à jour le composant
	OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

void CDialogFiltres::Update()
{
	// on met à jour le composant
	OnUpdate();
}

void CDialogFiltres::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogFiltres, CDialog)
	ON_LBN_SELCHANGE(IDC_FD_LIST_BAS, OnLbnSelchangeListBas)
	ON_WM_SHOWWINDOW()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_FD_X, OnBtnX)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogFiltres

void CDialogFiltres::OnLbnSelchangeListBas()
{
	JUnt32 selID = m_lstBas.GetSelectedID();
	if(selID != LB_ERR)
	{
		//m_FilterSel.SetText(m_srcPresse->m_TBLFiltresAudience.GetLabelByID( selID ));

		// Positionne l'ident filtre sélectionné dans le document
		m_pATPDocument->SetIdFiltreAudienceSel(selID);

		// on positionne l'indicateur de modification
		m_pATPDocument->SetModifiedFlag(TRUE);

		// on met à jour les vues du document
		m_pATPDocument->UpdateAllViews(UPDATE_FILTREAUDIENCE);
		m_pATPDocument->UpdateAllViews(UPDATE_AFF_PANEL);
	}
	m_lstBas.SetCurSel(LB_ERR);
}

void CDialogFiltres::OnUpdate()
{
	// on met à jour le composant
	if (m_pATPDocument != 0)
	{
		this->ShowWindow(m_pATPDocument->m_AFFFiltreAudience ? SW_SHOW : SW_HIDE);
		FillListFiltres();
		m_FilterSel.SetText(m_srcPresse->m_TBLFiltresAudience.GetLabelByID( m_pATPDocument->GetIdFiltreAudienceSel() ));
	}

}

BOOL CDialogFiltres::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();

	SetupControls();
	m_FilterSel.SetText("");

	return TRUE;
}
void CDialogFiltres::FillListFiltres()
{
	m_lstBas.ResetContent();
	if(m_pATPDocument)
	{
		for (m_srcPresse->m_TBLFiltresAudience.MoveFirst(); m_srcPresse->m_TBLFiltresAudience.IsValid(); m_srcPresse->m_TBLFiltresAudience.MoveNext() )
		{
			JMap<JUnt32,JUnt32>* mapTerrainsSel = m_pATPDocument->GetMapIdTerrainsAudienceSel();
			if(mapTerrainsSel && mapTerrainsSel->GetCount())
			{
				CKeyTerrain keyT;
				mapTerrainsSel->MoveFirst();
				keyT.Set(m_pATPDocument->GetIdSrcAudienceSel(), mapTerrainsSel->GetKey());
				if(m_pATPDocument->m_MoteurPresse.HasFiltre(keyT, m_srcPresse->m_TBLFiltresAudience.GetItem()->GetID()))
					m_lstBas.AddItem(m_srcPresse->m_TBLFiltresAudience.GetItem());
			}
		}
	}
}

void CDialogFiltres::OnCancel() 
{
	// on vérifie la validité du document
	if (m_pATPDocument != 0)
	{
		// on met à jour le pupitre
		m_pATPDocument->m_AFFFiltreAudience = false;

		// on met à jour les vues du document
		m_pATPDocument->UpdateAllViews(UPDATE_AFF_PANEL);
	}
	else
	{
		// on met à jour le composant
		this->ShowWindow(SW_HIDE);
	}
}

void CDialogFiltres::OnOK() 
{
	// on ne fait rien
}


void CDialogFiltres::SetupControls()
{
//subclass 

	m_bkgFiltre.SubclassDlgItem(IDC_FD_STATIC_FILTRE, this);
	m_ttlFiltre.SubclassDlgItem(IDC_FD_STATIC_TTLFILTRE, this);
	m_lstBas.SubclassDlgItem(IDC_FD_LIST_BAS, this);
	m_FilterSel.SubclassDlgItem(IDC_FD_LIST_HAUT, this);
	m_btnX.SubclassDlgItem(IDC_FD_X, this);

////
	m_bkgFiltre.SetTransparent(false);
	m_bkgFiltre.SetSunken(true);
	m_bkgFiltre.AddTopWindow(m_ttlFiltre);
	m_bkgFiltre.AddTopWindow(m_FilterSel);
	m_bkgFiltre.AddTopWindow(m_lstBas);
	m_bkgFiltre.AddTopWindow(m_btnX);

////
//	m_ttlFiltre.SetFontName(CATPColors::GetFontLabelName());
	m_ttlFiltre.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlFiltre.SetBorder(false);


//	SetColors();
}

void CDialogFiltres::SetColors(CATPColors::APPCOLOR app)
{
	m_bkgFiltre.SetBkColor(CATPColors::GetColorMedium(app),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	
	m_ttlFiltre.SetBkColor(CATPColors::GetColorDark(app),app, CATPColors::DARK_DEGRAD);
	m_ttlFiltre.SetTextColor(CATPColors::GetColorSuperDark(app));
	
	m_lstBas.SetSelectionColor( NORMAL, CATPColors::GetColorWhite() );
	m_lstBas.SetSelectedTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	m_FilterSel.SetFontName(CATPColors::GetFontStaticName());
	m_FilterSel.SetFontSize(CATPColors::GetFontStaticSize());
	m_FilterSel.SetBorder(true);
	m_FilterSel.SetBkColor(CATPColors::GetColorWhite(),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);

	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app), app);
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app), app );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app), app );
}

void CDialogFiltres::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow && m_pATPDocument)
		m_FilterSel.SetText(m_srcPresse->m_TBLFiltresAudience.GetLabelByID( m_pATPDocument->GetIdFiltreAudienceSel()));
}

LRESULT CDialogFiltres::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CDialogFiltres::OnBtnX()
{
	OnCancel();
}
