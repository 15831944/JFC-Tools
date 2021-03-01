// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPeriodeAction.h"
#include ".\atpperiodeaction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPeriodeAction::CATPPeriodeAction(UINT nIDTTLboite, UINT nIDTTL, CWnd* pParent) 
:JFCDialog(CATPPeriodeAction::IDD, pParent)
{
	m_nIDTTLboite = nIDTTLboite;
	m_nIDTTL = nIDTTL;
	//{{AFX_DATA_INIT(CATPPeriodeAction)
	//}}AFX_DATA_INIT
}

///////////////////////////////////////
// la fonction pour définir le document
JVoid CATPPeriodeAction::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;
}

JVoid CATPPeriodeAction::SetHypo(JBool bHypo)
{
	m_A2.SetHypo(bHypo);
}


void CATPPeriodeAction::DoDataExchange(CDataExchange* pDX)
{
	// on appelle le gestionnaire de base
	this->JFCDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CATPPeriodeAction)
	DDX_Control(pDX, IDC_CR_A1, m_A1);
	DDX_Control(pDX, IDC_CR_HS, m_HS);
	DDX_Control(pDX, IDC_CR_A3, m_A2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPPeriodeAction, JFCDialog)
	//{{AFX_MSG_MAP(CATPPeriodeAction)
	ON_BN_CLICKED(IDC_CR_BTNT, OnBnClickedBtnT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour mettre à jour la période d'action

BOOL CATPPeriodeAction::OnUpdatePeriodeAction()
{
	// on initialise le code
	BOOL Ret = FALSE;
	
	JDate first, last;
    m_A1.GetSelRange(first, last);
	if (first.IsValid() &&  last.IsValid())
	{
		m_DateD_Action = first;
		m_DateF_Action = last;
	}
	else
		m_A1.GetRange(m_DateD_Action, m_DateF_Action);


	// on modifie le code
	Ret = TRUE;

	// on renvoie le code
	return (Ret);
}

////////////////////////////////////////
// CATPPeriodeAction message handlers

BOOL CATPPeriodeAction::OnInitDialog()
{
	// on appelle le gestionnaire de base
	this->JFCDialog::OnInitDialog();
	m_A1.m_parent = this;
	m_A2.m_parent = this;
	// on configure les ascenseurs
	this->SetScrollBar(&m_A1, &m_HS, NULL);
	this->SetScrollBar(&m_A2, &m_HS, NULL);

	// on lie les ascenseurs
	m_A1.SetHorzScroll(&m_HS, TRUE);
	m_A2.SetHorzScroll(&m_HS, TRUE);

	// on définit la période de l'étude
	m_A1.SetRange(m_DateD_Etude, m_DateF_Etude, FALSE);

	m_A1.SetSelRange(m_DateD_Action, m_DateF_Action);

	// on se place à la fin de la période
	m_A1.EnsureVisible(m_DateD_Action);

	m_A2.SetDocument(m_pATPDocCreuset);

	SetupControls();

	CString text;
	text.FormatMessage(m_nIDTTLboite);
	SetWindowText(text);
	text.FormatMessage(m_nIDTTL);
	m_Ttl.SetText(text);

	// on quitte
	return (TRUE);
}

void CATPPeriodeAction::OnBnClickedBtnT()
{
	m_A1.BtnT();
	m_A2.Invalidate();
}

void CATPPeriodeAction::OnCancel()
{
	// on appelle le gestionnaire de base
	this->JFCDialog::OnCancel();
}

void CATPPeriodeAction::OnOK()
{
	// on met à jour le périmètre
	if (FALSE == this->OnUpdatePeriodeAction()) return;

	// on appelle le gestionnaire de base
	this->JFCDialog::OnOK();
}

void CATPPeriodeAction::SetupControls()
{
	m_Backgnd.SubclassDlgItem(IDC_CR_BK, this);
	m_Ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_btnT.SubclassDlgItem(IDC_CR_BTNT, this);
	m_btnOk.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	SetColors();
}

void CATPPeriodeAction::SetColors()
{
	m_Backgnd.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Backgnd.SetSunken(true);

	m_Ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_btnT.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnT.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnT.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnOk.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
}
