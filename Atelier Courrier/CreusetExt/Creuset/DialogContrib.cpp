// DialogContrib.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DialogContrib.h"
#include ".\dialogcontrib.h"
#include "resource.h"

// Boîte de dialogue CDialogContrib

IMPLEMENT_DYNAMIC(CDialogContrib, CDialog)
CDialogContrib::CDialogContrib(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogContrib::IDD, pParent)
{
	m_pATPDocCreuset = 0;
}

CDialogContrib::~CDialogContrib()
{
}


void CDialogContrib::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

//	m_A1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);
//	m_TXTA1.SetDocument(pATPDocCreuset);
	m_Txt.SetDocument(pATPDocCreuset);

	// on met à jour le composant
	OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

void CDialogContrib::Update()
{
	// on met à jour les composants
	//m_A1.Update();
	m_A2.Update();
	m_Txt.Update();
	//m_TXTA1.Update();
	OnUpdate();
}

void CDialogContrib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CR_A1, m_A1);
	DDX_Control(pDX, IDC_CR_A2, m_Txt);
	DDX_Control(pDX, IDC_CR_A3, m_A2);
//	DDX_Control(pDX, IDC_CR_A6, m_TXTA1);
}


BEGIN_MESSAGE_MAP(CDialogContrib, CDialog)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_CR_X, OnBtnX)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogContrib

void CDialogContrib::OnUpdate()
{
	// on met à jour le composant
	if (m_pATPDocCreuset != 0) this->ShowWindow(m_pATPDocCreuset->m_AFFContribution.IsVisible() ? SW_SHOW : SW_HIDE);
}

BOOL CDialogContrib::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();
	SetupControls();
	return TRUE;
}

void CDialogContrib::OnCancel() 
{
	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on met à jour le pupitre
		m_pATPDocCreuset->m_AFFContribution.SetVisible(false);

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
	}
	else
	{
		// on met à jour le composant
		this->ShowWindow(SW_HIDE);
	}
}

void CDialogContrib::OnOK() 
{
	// on ne fait rien
}


void CDialogContrib::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_CR_BK, this);
	m_ttl.SubclassDlgItem(IDC_CR_STATIC_TTL, this);
	m_btnX.SubclassDlgItem(IDC_CR_X, this);

////
	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	//m_bkg.AddTopWindow(m_TXTA1);
	//m_bkg.AddTopWindow(m_A1);
	m_bkg.AddTopWindow(m_A2);
	m_bkg.AddTopWindow(m_Txt);
	m_bkg.AddTopWindow(m_btnX);

////
//	m_ttlFiltre.SetFontName(CATPColors::GetFontLabelName());
	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);

	//m_TXTA1.ShowWindow(SW_HIDE);
	//m_A1.ShowWindow(SW_HIDE);

	SetColors();
}

void CDialogContrib::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
}

LRESULT CDialogContrib::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CDialogContrib::OnBtnX()
{
	OnCancel();
}