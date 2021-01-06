// DlgReplaceLib.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgReplaceLib.h"
#include ".\dlgreplacelib.h"

// Boîte de dialogue CDlgReplaceLib

/////////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
IMPLEMENT_DYNAMIC(CDlgReplaceLib, CDialog)
CDlgReplaceLib::CDlgReplaceLib(CWnd* pParent /*=NULL*/)	: CDialog(CDlgReplaceLib::IDD, pParent)
{
	// Init liste des idents items texte à remplacer
	m_pListIDs = NULL;

	// Init chaine édition
	//{{AFX_DATA_INIT(CDlgReplaceLib)
	m_EditBoxRemplace = _T("");
	m_EditBoxRemplacePar = _T("");
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CDlgReplaceLib::~CDlgReplaceLib()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Controles boites édition
void CDlgReplaceLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgReplaceLib)
	DDX_Text(pDX, IDC_RI_LIBELLE_REMPLACE, m_EditBoxRemplace);
	DDX_Text(pDX, IDC_RI_LIBELLE_REMPLACEPAR, m_EditBoxRemplacePar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReplaceLib, CDialog)
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgReplaceLib

/////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation contrôles boite de dialogue remplacement
BOOL CDlgReplaceLib::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Les boutons Ok et Cancel
	m_Button_OK.SubclassDlgItem(IDOK, this);
	m_Button_Cancel.SubclassDlgItem(IDCANCEL , this);
	
	// Les contrôles libellés
	m_LabelRemplace.SubclassDlgItem(IDC_RI_LABEL_REMPLACE,this);
	m_LabelRemplacePar.SubclassDlgItem(IDC_RI_LABEL_REMPLACEPAR,this);
	
	// Fond de la boite
	m_FondBox.SubclassDlgItem(IDC_RI_STATIC_BOX_REMPLACE,this);

	// Parametrage Libellé entete "Remplace :"
	m_LabelRemplace.SetFontName(CATPColors::GetFontLabelName());
	m_LabelRemplace.SetFontSize(CATPColors::GetFontLabelSize());
	m_LabelRemplace.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_LabelRemplace.SetBorder(false);
	m_LabelRemplace.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));
	
	// Parametrage Libellé entete "Par :"
	m_LabelRemplacePar.SetFontName(CATPColors::GetFontLabelName());
	m_LabelRemplacePar.SetFontSize(CATPColors::GetFontLabelSize());
	m_LabelRemplacePar.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_LabelRemplacePar.SetBorder(false);
	m_LabelRemplacePar.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));

	// Coloration et style fond de boite
	m_FondBox.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp, CATPColors::MEDIUM_DEGRAD);
	m_FondBox.SetTransparent(false);
	m_FondBox.SetSunken(true);
	m_FondBox.AddTopWindow(m_LabelRemplace);
	m_FondBox.AddTopWindow(m_LabelRemplacePar);

	// Les boites édition
	m_FondBox.AddTopWindow(*GetDlgItem(IDC_RI_LIBELLE_REMPLACE));
	m_FondBox.AddTopWindow(*GetDlgItem(IDC_RI_LIBELLE_REMPLACEPAR));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Validation remplacement chaine de caractères
void CDlgReplaceLib::OnOK()
{
	this->UpdateData(TRUE);

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Annulation remplacement
void CDlgReplaceLib::OnCancel()
{
	// Focus sur boite Remplace
	*GetDlgItem(IDC_RI_LIBELLE_REMPLACE)->SetFocus(); 

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Focus sur boite édition élmt à remplacer
void CDlgReplaceLib::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// Focus sur boite Remplace
	*GetDlgItem(IDC_RI_LIBELLE_REMPLACE)->SetFocus(); 
}
