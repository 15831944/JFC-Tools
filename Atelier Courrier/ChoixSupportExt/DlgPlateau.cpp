// Boite dialogue s�lection pour plateau
// cr�� le 07/06/2004
// DlgPlateau.cpp : fichier d'impl�mentation
//
#include "stdafx.h"
#include "DlgPlateau.h"
#include "DialogFormat.h"
#include "DlgDateTarif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Bo�te de dialogue CDlgPlateau
IMPLEMENT_DYNAMIC(CDlgPlateau, CDialog)

CDlgPlateau::CDlgPlateau(JATPDocChoixSupport & ATPDocChoixSupport, CWnd* pParent) : m_ATPDocChoixSupport(ATPDocChoixSupport), CDialog(CDlgPlateau::IDD, pParent)
{
	// Offre non s�lectionn�
	bCompleteOffre = 0;

	//On pr�selectionne 1 format (Page quadri)
	m_ATPDocChoixSupport.m_Offre.m_TBLIdFormats.AddTail() = 4;
	bCompleteFormat = 1;

	//{{AFX_DATA_INIT(CATPEtude)
	//}}AFX_DATA_INIT
}

CDlgPlateau::~CDlgPlateau()
{
}

void CDlgPlateau::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_TITRE,		m_Titre);
	DDX_Control(pDX, IDC_CS_DATETAR,	m_BtnDateTarif);
	DDX_Control(pDX, IDC_CS_OFFRE,		m_BtnOffre);
	DDX_Control(pDX, IDC_CS_FORMAT,		m_BtnFormat);
	DDX_Control(pDX, IDC_CS_FOND,		m_FondDlg);
	DDX_Control(pDX, IDOK,				m_BtnOK);
	DDX_Control(pDX, IDCANCEL,			m_BtnCancel);
}


BEGIN_MESSAGE_MAP(CDlgPlateau, CDialog)
	ON_BN_CLICKED(IDC_CS_DATETAR, OnDatetar)
	ON_BN_CLICKED(IDC_CS_OFFRE, OnOffre)
	ON_BN_CLICKED(IDC_CS_FORMAT, OnFormat)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue
BOOL CDlgPlateau::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// on r�cup�re la date de tarification
	m_DateTar = m_ATPDocChoixSupport.m_DateTarifaire;

	// Mise en place des contr�les
	SetupControls();

	// on met � jour la bo�te
	this->OnUpdate();

	// on quitte	
	return (TRUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour le plateau
BOOL  CDlgPlateau::OnUpdatePlateau()
{
	// on initialise le code
	BOOL Ret = FALSE;

	// on teste les exceptions
	try
	{
		// on modifie la date de tarification
		m_ATPDocChoixSupport.m_DateTarifaire		= m_DateTar;
		m_ATPDocChoixSupport.m_KEYPlateau.SetDateTar(m_DateTar);

		// on modifie le code
		Ret = TRUE;
	}
	catch (JException* pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
	}

	// on renvoie le code
	return (Ret);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Validation de la s�lection
VOID CDlgPlateau::OnUpdate()
{
	// on r�cup�re le bouton OK
	CWnd* pButton = this->GetDlgItem(IDOK);

	// on teste si on a trouv� le bouton
	if (pButton != NULL)
	{
		// on r�cup�re l'�tat de l'�tude
		BOOL State = (m_DateTar.IsValid() && bCompleteOffre && bCompleteFormat) ? TRUE : FALSE;
		
		SetIco();
		
		// on met � jour le bouton
		pButton->EnableWindow(State);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs
void CDlgPlateau::SetColors()
{
	// Couleur du fond
	m_FondDlg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	m_FondDlg.SetSunken(true);
	
	// Couleur du titre
	m_Titre.SetFontSize(CATPColors::GetFontLabelSize());
	m_Titre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_Titre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Couleur boutons s�lection date de tarification
	m_BtnDateTarif.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnDateTarif.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnDateTarif.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);

	// Couleur boutons s�lection offre
	m_BtnOffre.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnOffre.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnOffre.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);

	// Couleur boutons s�lection format
	m_BtnFormat.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnFormat.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnFormat.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS), CATPColors::COLORCHOIXSUPPORTS);

	// Bouton controle OK et Cancel
	m_BtnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des icones
void CDlgPlateau::SetIco()
{
	// Icone Bouton Date Tarification
	COleDateTime DateCur = COleDateTime::GetCurrentTime();

	// Date du jour
	JDate DateTarJour;
	DateTarJour.SetDate(DateCur.GetDay(),DateCur.GetMonth(),DateCur.GetYear());  

	if (m_DateTar.IsValid())
	{
		if (DateTarJour == m_DateTar)
			// La date en cours est celle du jour / position orange
			m_BtnDateTarif.SetBitmaps(IDB_CS_LEDORANGE, RGB(0,128,128));
		else
			// La date en cours est celle du jour / position verte
			m_BtnDateTarif.SetBitmaps(IDB_CS_LEDVERT, RGB(0,128,128));
	}
	else if (!m_DateTar.IsValid())
		// La date en cours n'est pas valide / position rouge
		m_BtnDateTarif.SetBitmaps(IDB_CS_LEDROUGE, RGB(0,128,128));


	// Icone Bouton Offre
	if (bCompleteOffre == 0)
		m_BtnOffre.SetBitmaps(IDB_CS_LEDROUGE, RGB(0,128,128));
	else if (bCompleteOffre == 1)
		m_BtnOffre.SetBitmaps(IDB_CS_LEDORANGE, RGB(0,128,128));
	else if (bCompleteOffre == 2)
		m_BtnOffre.SetBitmaps(IDB_CS_LEDVERT, RGB(0,128,128));
 
	// Icone Bouton Format
	if (bCompleteFormat == 0)
		m_BtnFormat.SetBitmaps(IDB_CS_LEDROUGE, RGB(0,128,128));
	else if (bCompleteFormat == 1)
		m_BtnFormat.SetBitmaps(IDB_CS_LEDORANGE, RGB(0,128,128));
	else if (bCompleteFormat == 2)
		m_BtnFormat.SetBitmaps(IDB_CS_LEDVERT, RGB(0,128,128));
}

//////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des contr�les
void CDlgPlateau::SetupControls()
{
	// Mise en place des couleurs
	SetColors();
}

///////////////////////////////////////////////////////////////////////////////////
// Validation nouvelle �tude
void CDlgPlateau::OnOK()
{
	// on met � jour l'�tude
	if (FALSE == this->OnUpdatePlateau()) return;

	// on appelle le gestionnaire de base
	this->CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////
// Annulation nouvelle s�lection
void CDlgPlateau::OnCancel()
{
	// on appelle le gestionnaire de base
	this->CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////////
// Modification date tarification
void CDlgPlateau::OnDatetar()
{
	// voir si document valide
	if (m_ATPDocChoixSupport.IsValid())
	{
		// La boite choix date tarifaire
		CDlgDateTarif Dlg(this);
		Dlg.SetColors(CATPColors::COLORCHOIXSUPPORTS);

		// Initialisation date
		if (m_ATPDocChoixSupport.m_DateTarifaire.IsValid()) 
			Dlg.SetDateTar(m_DateTar);

		// Ouverture S�lection Boite de dialogue Date Tarifaire
		if( Dlg.DoModal())
		{
			// R�cuperation nouvelle date tarifaire
			JDate NewDateTar; 
			NewDateTar = Dlg.GetDateTar(); 

			// Test si la date a chang�e
			if (NewDateTar != m_DateTar)
			{
				// Sauve nouvelle date
				m_DateTar = NewDateTar;
			}

			// Repositionne les diodes validit� �l�ment
			SetIco();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Modification s�lection de l'offre
void CDlgPlateau::OnOffre()
{
	CDialogChoixOffre dlgChoixOffre(this);

	// Setup de la boite offre source
	dlgChoixOffre.SetStartupMode(CDialogChoixOffre::SOURCE, CATPColors::COLORCHOIXSUPPORTS);

	// On passe l'offre et les terrains (via dernier source et terrains utilis�s)
	dlgChoixOffre.SetOffre(m_ATPDocChoixSupport.m_Offre);

	// Affichage de la fen�tre offre
	if(dlgChoixOffre.DoModal()== IDOK)
	{
		// Si �l�ments modifi�s, on r�cup�re les nouvelles infos
		//if(Dlg.IsModified())
		{
			// dlgChoixOffre.GetOffre( m_ATPDocChoixSupport.m_Offre);

			dlgChoixOffre.GetOffreRestreinte( m_ATPDocChoixSupport.m_Offre);
			bCompleteOffre = 2;
			this->OnUpdate();

			// Mise � jour globale
			// m_ATPDocChoixSupport.UpdateAllViews(JATPDocChoixSupport::UPDATE_OFFRE);

			// Repositionne les diodes validit� �l�ment
			SetIco();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Modification s�lection formats
void CDlgPlateau::OnFormat()
{
	CDialogFormat dlg(this);

	//Initialisation
	dlg.SetData(m_ATPDocChoixSupport.m_Offre.m_TBLIdSupports, m_ATPDocChoixSupport.m_DateTarifaire);
	dlg.SetFormats(m_ATPDocChoixSupport.m_Offre.m_TBLIdFormats);
	dlg.SetColors(CATPColors::COLORCHOIXSUPPORTS);

	//Ouverture S�lection des formats
	if( dlg.DoModal()== IDOK && dlg.IsModified() )
	{
		// R�cuperation des donn�es format
		dlg.GetFormats(m_ATPDocChoixSupport.m_Offre.m_TBLIdFormats);
		m_ATPDocChoixSupport.m_Offre.m_Changes |= JATPOffre::FORMATS; 

		bCompleteFormat = 2;
		this->OnUpdate();

		// Mise � jour globale
		if (m_ATPDocChoixSupport.SelectionValid())
			m_ATPDocChoixSupport.UpdateAllViews(UPDATE_SEL_FORMAT);

		// Repositionne les diodes validit� �l�ment
			SetIco();

	}
}
