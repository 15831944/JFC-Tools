// DlgRangResult.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "DlgRangResult.h"
#include ".\dlgrangresult.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgRangResult

IMPLEMENT_DYNAMIC(CDlgRangResult, CDialog)

//////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CDlgRangResult::CDlgRangResult(CWnd* pParent,JATPDocChoixSupport *pDoc)
	: CDialog(CDlgRangResult::IDD, pParent)
{
	// R�cup�re le document en cours
	pDocChoixSupport = pDoc;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgRangResult::~CDlgRangResult()
{
}

///////////////////////////////////////////////////////////////////////////////////////
// Initialisation boite de dialogue s�lection rangs r�sultats
BOOL CDlgRangResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Mise en place des contr�les
	SetupControls();

	// Centrage de la fen�tre
	this->CenterWindow();

	// Init liste des grandeurs et ordre courant
	InitListGrandeurs();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Initialisation liste des grandeurs avec s�lection rang en cours
void CDlgRangResult::InitListGrandeurs()
{
	// Init Liste affich�e des grandeurs
	m_LstRangGrandeur.DeleteAllItems();

	// Boucle sur la liste des grandeurs
	JUnt32 IndexList = 0;
	for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
		 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
		 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
	{
		// Ident grandeur
		JUnt32 &IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

		// Etat affichage rang r�sultat
		JBool EtatAffichRang = pDocChoixSupport->GetRangVisible(pDocChoixSupport->ModuleEch,IdGrandeur); 
		
		// R�cup�re libell� grandeur
		CString LibGrandeur =  pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch,IdGrandeur);

		// Ajout � la liste
		m_LstRangGrandeur.InsertItem(IndexList,LibGrandeur); 

		// Sauve Item Index Grandeur
		m_LstRangGrandeur.SetItemData(IndexList,IdGrandeur); 

		/*
		if (EtatAffichRang)
			m_LstRangGrandeur.SetItem(IndexList, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED,LVIS_SELECTED, 0);
		*/
		// S�lection ou non s�lection du rang
		if (EtatAffichRang)
			m_LstRangGrandeur.SetItemState (IndexList, LVIS_SELECTED, LVIS_SELECTED);

		// Passe � index liste suivant
		IndexList++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des contr�les
void CDlgRangResult::SetupControls()
{
	// Mise en place des couleurs
	SetColors();
}

////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour des couleurs
void CDlgRangResult::SetColors()
{
	// Couleur du background
	m_BkRangGrandeur.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::MEDIUM_DEGRAD);
	m_BkRangGrandeur.SetSunken(true);

	// Les boutons
	m_BtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN,				CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
	m_BtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN,			CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
	m_Btn_TousLesRangs.SetColor(CButtonST::BTNST_COLOR_BK_IN,	CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);
	m_Btn_AucunRang.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorWhite(), CATPColors::COLORCHOIXSUPPORTS);

	// Style Label Titre
	m_LabelTitre.SetFontName(CATPColors::GetFontLabelName());
	m_LabelTitre.SetFontSize(CATPColors::GetFontLabelSize());
	m_LabelTitre.SetBorder(false);
	m_LabelTitre.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LabelTitre.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Liste des grandeurs
	m_LstRangGrandeur.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
	m_LstRangGrandeur.InsertColumn(0, "");
    m_LstRangGrandeur.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CDlgRangResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_LABEL_TITRE, m_LabelTitre);
	DDX_Control(pDX, IDC_CS_LST_GRANDEURS, m_LstRangGrandeur);
	DDX_Control(pDX, IDC_CS_BK_ORDRE_GRDECH, m_BkRangGrandeur);
	DDX_Control(pDX, IDOK, m_BtnOk);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_CS_TOUS_RANG, m_Btn_TousLesRangs);
	DDX_Control(pDX, IDC_CS_AUCUN_RANG, m_Btn_AucunRang);
}


BEGIN_MESSAGE_MAP(CDlgRangResult, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CS_TOUS_RANG, OnBnClickedCsTousRang)
	ON_BN_CLICKED(IDC_CS_AUCUN_RANG, OnBnClickedCsAucunRang)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////
// Validation s�lection Rangs
void CDlgRangResult::OnBnClickedOk()
{
	// Init S�lection rang
	for (JUnt32 IndexList = 0;  IndexList < m_LstRangGrandeur.GetItemCount(); IndexList++)
	{
		// Ident grandeur
		JUnt32 IdGrandeur = m_LstRangGrandeur.GetItemData(IndexList);
	
		// Repositionne affichage rang r�sultat � faux
		pDocChoixSupport->SetRangVisible(pDocChoixSupport->ModuleEch, false, IdGrandeur); 
	}		

	// Nombre de rangs s�lectionn�s
	JUnt32 NbSelRang = m_LstRangGrandeur.GetSelectedCount();
	
	// Mise � jour des s�lections rangs
	if (NbSelRang > 0)
	{
		// Init pour 1er �lmt selectionn�
		int  nItem = -1;

		for (JUnt32 i = 0; i < NbSelRang; i++)
		{
			// R�cup�re item s�lectionn�
			nItem = m_LstRangGrandeur.GetNextItem(nItem, LVNI_SELECTED);
			
			// Ident grandeur
			JUnt32 IdGrandeur = m_LstRangGrandeur.GetItemData(nItem);
	
			// Repositionne affichage rang r�sultat
			pDocChoixSupport->SetRangVisible(pDocChoixSupport->ModuleEch, true, IdGrandeur); 
		}
	}

	// On est cens� avoir chang� les rangs s�lectionn�s
	pDocChoixSupport->SetModifiedFlag(TRUE);

	// Validation du nouveau choix ordre entete
	OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////
// Annulation s�lection Rangs
void CDlgRangResult::OnBnClickedCancel()
{
	// Annulation s�lection (on garde s�lection pr�c�dente)
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////////////
// S�lection de toutes les grandeurs pour affichage des rangs
void CDlgRangResult::OnBnClickedCsTousRang()
{
	for (int IndexList = 0;  IndexList < m_LstRangGrandeur.GetItemCount(); IndexList++)
	{
		// S�lection dans liste
		m_LstRangGrandeur.SetItemState(IndexList, LVIS_SELECTED, LVIS_SELECTED);

		// Ident grandeur
		JUnt32 IdGrandeur = static_cast<JUnt32>(m_LstRangGrandeur.GetItemData(IndexList));
	
		// Repositionne affichage rang r�sultat
		pDocChoixSupport->SetRangVisible(pDocChoixSupport->ModuleEch, true, IdGrandeur); 
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// D�s�lection de toutes les grandeurs pour affichage des rangs
void CDlgRangResult::OnBnClickedCsAucunRang()
{
	for (int IndexList = 0;  IndexList < m_LstRangGrandeur.GetItemCount(); IndexList++)
	{
		// D�selection dans liste
		m_LstRangGrandeur.SetItemState(IndexList, 0, LVIS_SELECTED);

		// Ident grandeur
		JUnt32 IdGrandeur = static_cast<JUnt32>(m_LstRangGrandeur.GetItemData(IndexList));
	
		// Repositionne affichage rang r�sultat
		pDocChoixSupport->SetRangVisible(pDocChoixSupport->ModuleEch, false, IdGrandeur); 
	}
}

