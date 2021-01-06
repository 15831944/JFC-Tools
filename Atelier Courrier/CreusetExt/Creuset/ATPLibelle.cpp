// on inclut les d�finitions n�cessaires
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

	// r�cup�ration Titre de la fen�tre dialogue
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
// les fonctions pour manipuler le libell�

JVoid CATPLibelle::SetLibelle(const JLabel & Libelle)
{
	// on m�morise le libell�
	m_Libelle = Libelle;
}

JLabel CATPLibelle::GetLibelle() const
{
	// on renvoie le libell�
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
// la fonction pour mettre � jour le libell�

BOOL CATPLibelle::OnUpdateLibelle()
{
	// on initialise le code
	BOOL Ret = FALSE;

	// on teste les exceptions
	try
	{
		// on cr�e le buffer
		TCHAR Buf[256];

		// on r�cup�re le libell�
		m_EditL.GetWindowText(Buf, sizeof(Buf) / sizeof(Buf[0]));

		// on met � jour le libell�
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
// la fonction pour mettre � jour la bo�te

VOID CATPLibelle::OnUpdate()
{
	// on r�cup�re le bouton OK
	CWnd* pButton = this->GetDlgItem(IDOK);

	// on teste si on a trouv� le bouton
	if (pButton != NULL)
	{
		// on cr�e le buffer
		TCHAR Buf[256];

		// on initialise l'�tat du bouton
		BOOL State = FALSE;

		// on r�cup�re la longueur du libell�
		INT Lng = m_EditL.GetWindowTextLength();

		// on v�rifie la validit� de la longueur
		if (Lng > 0 && Lng < 256)
		{
			// on initialise l'indicateur
			BOOL Valid = TRUE;
			BOOL Empty = TRUE;

			// on r�cup�re le libell�
			m_EditL.GetWindowText(Buf, sizeof(Buf) / sizeof(Buf[0]));

			// on boucle sur tous les caract�res
			for (LONG Idx = 0; Buf[Idx] != 0; Idx += 1)
			{
				// on teste le caract�re courant
				if      (Buf[Idx] >=  1 && Buf[Idx] <=  8) Valid = FALSE;
				else if (Buf[Idx] >= 10 && Buf[Idx] <= 31) Valid = FALSE;
				else if (Buf[Idx] == 127)                  Valid = FALSE;
				else if (Buf[Idx] != 9  && Buf[Idx] != 32) Empty = FALSE;
			}

			// on corrige l'�tat du bouton
			if (Valid != FALSE && Empty == FALSE) State = TRUE;
		}

		// on met � jour le bouton
		pButton->EnableWindow(State);
	}
}

//////////////////////////////////
// CATPLibelle message handlers

BOOL CATPLibelle::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// Titre de la fen�tre dialogue
	this->SetWindowText(m_TitreFen.AsJCharPtr()); 

	// on d�finit les limites du libell�
	m_EditL.SetLimitText(255);

	// on met � jour le libell�
	m_EditL.SetWindowText(m_Libelle.AsJCharPtr());

	SetupControls();
	// on met � jour la bo�te
	this->OnUpdate();

	// on quitte
	return (TRUE);
}

void CATPLibelle::OnChangeLibelle() 
{
	// on met � jour la bo�te
	this->OnUpdate();
}

void CATPLibelle::OnCancel() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnCancel();
}

void CATPLibelle::OnOK() 
{
	// on met � jour le libell�
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



