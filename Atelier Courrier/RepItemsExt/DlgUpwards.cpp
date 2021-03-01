// DlgUpwards.cpp : implementation file
//

#include "stdafx.h"
#include "DlgUpwards.h"
#include "DlgItems.h"

IMPLEMENT_DYNAMIC(CDlgUpwards, CDialog)
CDlgUpwards::CDlgUpwards(const CString & Message, const CString &Titre, const JMap<JInt32, JLabel> & Upwards, CWnd* pParent) : m_Message(Message), m_Upwards(Upwards), CDialog(CDlgUpwards::IDD, pParent)
{
	// Récupération du titre
	m_Titre = Titre;

	// Couleur de la boite selon qu'il vient de Tris Croises ou du Répertoire Items
	// if (((CDlgItems*)pParent)->ValCurMode() < 4)
	CString QuelAppli = ((CDlgItems*)pParent)->GetTitleAppli();
	if (QuelAppli == "Pupitre")
		m_CATPColorsApp = CATPColors::COLORANALYSEUR;
	else
		m_CATPColorsApp = CATPColors::COLORREPITEMS; 

	/* Ne marche pas
	if (pParent->IsKindOf(RUNTIME_CLASS(CDlgItems)))
		int Toto = 0;
	if (pParent->IsKindOf(RUNTIME_CLASS(CDlgTableau)))
 		int Toto = 0;
	*/
}

CDlgUpwards::~CDlgUpwards()
{
}

void CDlgUpwards::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgUpwards, CDialog)
END_MESSAGE_MAP()

// CDlgUpwards message handlers

BOOL CDlgUpwards::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Les contrôles boite Upwards
	m_OK     .SubclassDlgItem(IDOK,                this);
	m_Box    .SubclassDlgItem(IDC_RI_STATIC_BOX,   this);
	m_ListBox.SubclassDlgItem(IDC_RI_LIST_UPWARDS, this);

	// spécifités boite contenu
	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp), m_CATPColorsApp, CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_ListBox);

	// Affichage titre de la fenêtre
	CString TxtTitreFen;
	TxtTitreFen.Format("%s - %s","Ascendance",m_Titre);
	SetWindowText(TxtTitreFen);

	// Affichage des items utilisant cet item
	for (m_Upwards.MoveLast(); m_Upwards.IsValid(); m_Upwards.MovePrev())
		m_ListBox.SetItemData(m_ListBox.AddString(m_Upwards.GetItem().AsJCharPtr()), m_Upwards.GetKey());

	return TRUE;
}
