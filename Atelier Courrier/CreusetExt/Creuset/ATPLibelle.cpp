// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPLibelle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPLibelle::CATPLibelle(JLabel TitreFen,CWnd* pParent) : CDialog(CATPLibelle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATPLibelle)
	//}}AFX_DATA_INIT

	// récupération Titre de la fenêtre dialogue
	m_TitreFen = TitreFen;
}

CATPLibelle::CATPLibelle(UINT strId, CWnd* pParent) : CDialog(CATPLibelle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATPLibelle)
	//}}AFX_DATA_INIT
	CString TitreFen;
	TitreFen.FormatMessage(strId);
	m_TitreFen = TitreFen;
}
//////////////////////////////////////////
// les fonctions pour manipuler le libellé

JVoid CATPLibelle::SetLibelle(const JLabel & Libelle)
{
	// on mémorise le libellé
	m_Libelle = Libelle;
}

JLabel CATPLibelle::GetLibelle() const
{
	// on renvoie le libellé
	return (m_Libelle);
}

void CATPLibelle::DoDataExchange(CDataExchange* pDX)
{
	// on appelle le gestionnaire de base
	this->CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CATPLibelle)
	DDX_Control(pDX, IDC_CR_LIBELLE, m_EditL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPLibelle, CDialog)
	//{{AFX_MSG_MAP(CATPLibelle)
	ON_EN_CHANGE(IDC_CR_LIBELLE, OnChangeLibelle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////
// la fonction pour mettre à jour le libellé

BOOL CATPLibelle::OnUpdateLibelle()
{
	// on initialise le code
	BOOL Ret = FALSE;

	// on teste les exceptions
	try
	{
		// on crée le buffer
		TCHAR Buf[256];

		// on récupère le libellé
		m_EditL.GetWindowText(Buf, sizeof(Buf) / sizeof(Buf[0]));

		// on met à jour le libellé
		m_Libelle = Buf;

		// on modifie le code
		Ret = TRUE;
	}
	catch (JException* pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR);
	}

	// on renvoie le code
	return (Ret);
}

//////////////////////////////////////////
// la fonction pour mettre à jour la boîte

VOID CATPLibelle::OnUpdate()
{
	// on récupère le bouton OK
	CWnd* pButton = this->GetDlgItem(IDOK);

	// on teste si on a trouvé le bouton
	if (pButton != NULL)
	{
		// on crée le buffer
		TCHAR Buf[256];

		// on initialise l'état du bouton
		BOOL State = FALSE;

		// on récupère la longueur du libellé
		INT Lng = m_EditL.GetWindowTextLength();

		// on vérifie la validité de la longueur
		if (Lng > 0 && Lng < 256)
		{
			// on initialise l'indicateur
			BOOL Valid = TRUE;
			BOOL Empty = TRUE;

			// on récupère le libellé
			m_EditL.GetWindowText(Buf, sizeof(Buf) / sizeof(Buf[0]));

			// on boucle sur tous les caractères
			for (LONG Idx = 0; Buf[Idx] != 0; Idx += 1)
			{
				// on teste le caractère courant
				if      (Buf[Idx] >=  1 && Buf[Idx] <=  8) Valid = FALSE;
				else if (Buf[Idx] >= 10 && Buf[Idx] <= 31) Valid = FALSE;
				else if (Buf[Idx] == 127)                  Valid = FALSE;
				else if (Buf[Idx] != 9  && Buf[Idx] != 32) Empty = FALSE;
			}

			// on corrige l'état du bouton
			if (Valid != FALSE && Empty == FALSE) State = TRUE;
		}

		// on met à jour le bouton
		pButton->EnableWindow(State);
	}
}

//////////////////////////////////
// CATPLibelle message handlers

BOOL CATPLibelle::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// Titre de la fenêtre dialogue
	this->SetWindowText(m_TitreFen.AsJCharPtr()); 

	// on définit les limites du libellé
	m_EditL.SetLimitText(255);

	// on met à jour le libellé
	m_EditL.SetWindowText(m_Libelle.AsJCharPtr());

	SetupControls();
	// on met à jour la boîte
	this->OnUpdate();

	// on quitte
	return (TRUE);
}

void CATPLibelle::OnChangeLibelle() 
{
	// on met à jour la boîte
	this->OnUpdate();
}

void CATPLibelle::OnCancel() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnCancel();
}

void CATPLibelle::OnOK() 
{
	// on met à jour le libellé
	if (FALSE == this->OnUpdateLibelle()) return;

	// on appelle le gestionnaire de base
	this->CDialog::OnOK();
}

void CATPLibelle::SetupControls()
{
	m_Backgnd.SubclassDlgItem(IDC_CR_BK, this);
	m_Ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_btnOk.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	SetColors();
}

void CATPLibelle::SetColors()
{
	m_Backgnd.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Backgnd.SetSunken(true);

	m_Ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_btnOk.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );

}



