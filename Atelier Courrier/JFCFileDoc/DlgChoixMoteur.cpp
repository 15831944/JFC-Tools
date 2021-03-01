// DlgChoixMoteur.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgChoixMoteur.h"
#include "ATPMessages.h"


// Boîte de dialogue CDlgChoixMoteur

IMPLEMENT_DYNAMIC(CDlgChoixMoteur, CDialog)
CDlgChoixMoteur::CDlgChoixMoteur(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChoixMoteur::IDD, pParent)
{
	m_pATPDoc = 0;
}

CDlgChoixMoteur::~CDlgChoixMoteur()
{
}

void CDlgChoixMoteur::SetDocument(JATPDocument* pATPDoc)
{
	// on mémorise le document
	m_pATPDoc = pATPDoc;

	// on met à jour le composant
	OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

void CDlgChoixMoteur::Update()
{
	// on met à jour le composant
	OnUpdate();
}

void CDlgChoixMoteur::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgChoixMoteur, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_FD_X, OnBtnX)
	ON_BN_CLICKED(IDC_FD_BTNOPTMOT, OnBnClickedMoteur)
	ON_BN_CLICKED(IDC_FD_BTNREPRISE, OnBtnReprise)
	ON_BN_CLICKED(IDC_FD_BTNLDP, OnBtnLDP)
	ON_BN_CLICKED(IDC_FD_BTNLNM, OnBtnLNM)
	ON_BN_CLICKED(IDC_FD_BTNLDP30J, OnBtnLDP30J)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogFiltres



void CDlgChoixMoteur::OnUpdate()
{
	// on met à jour le composant
	if (m_pATPDoc != 0)
	{
		this->ShowWindow(m_pATPDoc->m_AFFOptionsMoteur ? SW_SHOW : SW_HIDE);
		CString lblMoteur;
		switch (m_pATPDoc->m_MoteurPresse.GetType())
		{
		case CMoteurPresse::ABAQUE :
			//m_btnMoteur.SetBitmaps(IDB_FD_MOT3, RGB(0,128,128));
			m_btnMoteur.SetTooltipText(IDS_FD_ABAQUE);
			lblMoteur.LoadString(IDS_FD_ABAQUE);
			m_lblMoteur.SetText(lblMoteur);
			break;
		case CMoteurPresse::ABAQUE_ETAL:
			m_btnMoteur.SetBitmaps(IDB_FD_MOT2, RGB(0,128,128));
			m_btnMoteur.SetTooltipText(IDS_FD_ABAQUE);
			lblMoteur.LoadString(IDS_FD_ABAQUE);
			m_lblMoteur.SetText(lblMoteur);
			break;
		case CMoteurPresse::PROBA:
			m_btnMoteur.SetBitmaps(IDB_FD_MOT1, RGB(0,128,128));
			m_btnMoteur.SetTooltipText(IDS_FD_PROBA);
			lblMoteur.LoadString(IDS_FD_PROBA);
			m_lblMoteur.SetText(lblMoteur);
			break;
		}
	
		m_ChkReprise.SetCheck(m_pATPDoc->m_bRepriseEnMain);
		m_ChkLDP.SetCheck(m_pATPDoc->m_bLDPQtd);
		m_ChkLNM.SetCheck(m_pATPDoc->m_bLNMMag);
		m_ChkLDP30J.SetCheck(m_pATPDoc->m_bLDP30J);
	}
}

BOOL CDlgChoixMoteur::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();

	SetupControls();
	m_ChkReprise.ShowWindow(SW_HIDE);

	m_ChkLNM.EnableWindow(FALSE);
	m_ChkLDP30J.EnableWindow(FALSE);

	return TRUE;
}


void CDlgChoixMoteur::OnCancel() 
{
	// on vérifie la validité du document
	if (m_pATPDoc != 0)
	{
		// on met à jour le pupitre
		m_pATPDoc->m_AFFOptionsMoteur = false;

		// on met à jour les vues du document
		OnUpdate();
		//m_pATPDoc->UpdateAllViews(UPDATE_AFF_PUPITRE);
	}
	else
	{
		// on met à jour le composant
		this->ShowWindow(SW_HIDE);
	}
}

void CDlgChoixMoteur::OnOK() 
{
	// on ne fait rien
}


void CDlgChoixMoteur::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_FD_BK, this);
	m_ttl.SubclassDlgItem(IDC_FD_STATIC_TTL, this);
	m_btnX.SubclassDlgItem(IDC_FD_X, this);
	
	m_lblMoteur.SubclassDlgItem(IDC_FD_TXT, this);

	m_btnMoteur.SubclassDlgItem(IDC_FD_BTNOPTMOT, this);
	m_btnMoteur.EnableToolTips();
	m_btnMoteur.SetCheck(1);

	m_ChkReprise.SubclassDlgItem(IDC_FD_BTNREPRISE, this);
	m_ChkLDP.SubclassDlgItem(IDC_FD_BTNLDP, this);
	m_ChkLNM.SubclassDlgItem(IDC_FD_BTNLNM, this);
	m_ChkLDP30J.SubclassDlgItem(IDC_FD_BTNLDP30J, this);

	////
	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	m_bkg.AddTopWindow(m_btnX);
	m_bkg.AddTopWindow(m_lblMoteur);
	m_bkg.AddTopWindow(m_btnMoteur);
	m_bkg.AddTopWindow(m_ChkReprise);
	m_bkg.AddTopWindow(m_ChkLDP);
	m_bkg.AddTopWindow(m_ChkLNM);
	m_bkg.AddTopWindow(m_ChkLDP30J);

////
//	m_ttlFiltre.SetFontName(CATPColors::GetFontLabelName());
	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);


//	SetColors();
}

void CDlgChoixMoteur::SetColors(CATPColors::APPCOLOR app)
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(app),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	
	m_ttl.SetBkColor(CATPColors::GetColorDark(app),app, CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(app));

	m_lblMoteur.SetBkColor(CATPColors::GetColorMedium(app),app, CATPColors::MEDIUM_DEGRAD);
	m_lblMoteur.SetTextColor(CATPColors::GetColorBlack());
	
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app),app );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app),app );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app),app );

	m_btnMoteur.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app),app );
	m_btnMoteur.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app),app );
	m_btnMoteur.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app),app );

	m_ChkReprise.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app),app );
	m_ChkReprise.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app),app );
	m_ChkReprise.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app),app );
	m_ChkReprise.SetIcon(IDI_FD_CHECK, IDI_FD_NOCHECK);
	
	m_ChkLDP.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app),app );
	m_ChkLDP.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app),app );
	m_ChkLDP.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app),app );
	m_ChkLDP.SetIcon(IDI_FD_CHECK, IDI_FD_NOCHECK);

	m_ChkLNM.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app),app );
	m_ChkLNM.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app),app );
	m_ChkLNM.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app),app );
	m_ChkLNM.SetIcon(IDI_FD_CHECK, IDI_FD_NOCHECK);

	m_ChkLDP30J.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(app),app );
	m_ChkLDP30J.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(app),app );
	m_ChkLDP30J.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(app),app );
	m_ChkLDP30J.SetIcon(IDI_FD_CHECK, IDI_FD_NOCHECK);

}

void CDlgChoixMoteur::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


	if (bShow && m_pATPDoc)
		Update();
}

LRESULT CDlgChoixMoteur::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CDlgChoixMoteur::OnBtnX()
{
	OnCancel();
}


void CDlgChoixMoteur::OnBnClickedMoteur()
{
	switch (m_pATPDoc->m_MoteurPresse.GetType())
	{
	//case CMoteurPresse::ABAQUE :
	//	m_pATPDoc->m_MoteurPresse.SetType(CMoteurPresse::PROBA);
	//	break;
	case CMoteurPresse::ABAQUE_ETAL:
		m_pATPDoc->m_MoteurPresse.SetType(CMoteurPresse::PROBA);
		break;
	case CMoteurPresse::PROBA:
		m_pATPDoc->m_MoteurPresse.SetType(CMoteurPresse::ABAQUE_ETAL);
		break;
	}
	m_pATPDoc->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
}
void CDlgChoixMoteur::OnBtnReprise()
{
	m_pATPDoc->m_bRepriseEnMain = !m_pATPDoc->m_bRepriseEnMain;
	m_pATPDoc->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
}

void CDlgChoixMoteur::OnBtnLDP()
{
	m_pATPDoc->m_bLDPQtd = !m_pATPDoc->m_bLDPQtd;
	m_pATPDoc->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
}

void CDlgChoixMoteur::OnBtnLNM()
{
	m_pATPDoc->m_bLNMMag = !m_pATPDoc->m_bLNMMag;
	m_pATPDoc->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
}

void CDlgChoixMoteur::OnBtnLDP30J()
{
	m_pATPDoc->m_bLDP30J = !m_pATPDoc->m_bLDP30J;
	m_pATPDoc->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
}