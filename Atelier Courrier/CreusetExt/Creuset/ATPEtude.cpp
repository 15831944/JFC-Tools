// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPeriode.h"
#include "ATPEtude.h"
#include "ATPProduit.h"
#include "DialogFormat.h"
#include "DlgDateTarif.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEtude::CATPEtude(JATPDocCreuset & ATPDocCreuset, CWnd* pParent) : m_ATPDocCreuset(ATPDocCreuset), CDialog(CATPEtude::IDD, pParent)
{
	bCompleteAnnonc =  2;
	bCompleteOffre = 0;
	//On préselectionne 1PQ
	
	m_ATPDocCreuset.m_Offre.m_TBLIdFormats.AddTail() = 4;

	if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
	{
		m_ATPDocCreuset.m_listFormatsQP.AddTail() = 4;
		m_ATPDocCreuset.m_Offre.m_TBLIdFormats.Reset();
		m_ATPDocCreuset.m_Offre.m_TBLIdFormats.AddTail() = FORMAT_VIRTUEL;
	}

	bCompleteFormat = 1;
	//{{AFX_DATA_INIT(CATPEtude)
	//}}AFX_DATA_INIT
}

void CATPEtude::DoDataExchange(CDataExchange* pDX)
{
	// on appelle le gestionnaire de base
	this->CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CATPEtude)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPEtude, CDialog)
	//{{AFX_MSG_MAP(CATPEtude)
	ON_BN_CLICKED(IDC_CR_PERIODE, OnPeriode)
	ON_BN_CLICKED(IDC_CR_PRODUIT, OnProduit)
	ON_BN_CLICKED(IDC_CR_FORMAT, OnFormat)
	ON_COMMAND(IDM_CR_OFFRECREER, OnOffre)
	ON_COMMAND(IDM_CR_OFFREIMPORT, OnOffreImport)
	
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_CR_OFFRE2CREER, &CATPEtude::OnOffre2Creer)
END_MESSAGE_MAP()

/////////////////////////////////////////
// la fonction pour mettre à jour l'étude

BOOL  CATPEtude::OnUpdateEtude()
{
	// on initialise le code
	BOOL Ret = FALSE;

	// on teste les exceptions
	try
	{
		// on modifie la période de l'étude
		m_ATPDocCreuset.m_KEYEtude.SetDateD(m_DateD);
		m_ATPDocCreuset.m_KEYEtude.SetDateF(m_DateF);

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

//////////////////////////////////////////
// la fonction pour mettre à jour la boîte

VOID CATPEtude::OnUpdate()
{
	// on récupère le bouton OK
	CWnd* pButton = this->GetDlgItem(IDOK);

	// on teste si on a trouvé le bouton
	if (pButton != NULL)
	{
		// on récupère l'état de l'étude
		BOOL State = (m_DateD.IsValid() && m_DateF.IsValid() && bCompleteAnnonc && bCompleteOffre && bCompleteFormat) ? TRUE : FALSE;

		SetIco();
		
		// on met à jour le bouton
		pButton->EnableWindow(State);
	}
}

////////////////////////////////
// CATPEtude message handlers

BOOL CATPEtude::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	// on récupère le produit de l'étude
	m_IdProduit = m_ATPDocCreuset.m_KEYEtude.GetIdProduit();

	// on récupère les dates de l'étude
	m_DateD = m_ATPDocCreuset.m_KEYEtude.GetDateD();
	m_DateF = m_ATPDocCreuset.m_KEYEtude.GetDateF();

	SetupControls();
	if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
	{
		//m_btnPeriode.EnableWindow(FALSE);
		m_btnPeriode.SetWindowText("Date tarifaire");
	}

	// on met à jour la boîte
	this->OnUpdate();

	// on quitte	
	return (TRUE);
}

void CATPEtude::OnPeriode() 
{
	if(!m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
	{
		// on crée la boîte de dialogue
		CATPPeriode Dlg(this);

		// on définit la période
		Dlg.SetDateD(m_DateD);
		Dlg.SetDateF(m_DateF);

		// on affiche la boîte de dialogue
		if (IDOK == Dlg.DoModal())
		{
			// on récupère la période
			m_DateD = Dlg.GetDateD();
			m_DateF = Dlg.GetDateF();

			// on met à jour la boîte
			this->OnUpdate();
		}
	}
	else
	{
		CDlgDateTarif Dlg(this);
		Dlg.SetColors(CATPColors::COLORCREUSET);
		if (IDOK == Dlg.DoModal())
		{
			m_DateD = Dlg.GetDateTar();

			//m_DateF  = JDate::GetMax();
			//JInt32 j,m,a;
			//m_DateF.GetDate(j,m,a);//31/12/2997 trop de calcul
			m_DateF.SetDate(30,12,2055);

			// on met à jour la boîte
			this->OnUpdate();
		}


	}
}

void CATPEtude::OnProduit() 
{
	// on crée la boite de sélection Groupe/Annonceur/Produit
	CATPProduit	Dlg(this);

	if (IDOK == Dlg.DoModal())
	{
		// on récupére code groupe, code annonceur, code produit
		m_CodeGroupe    = Dlg.GetCodeGroupe();
		m_CodeAnnonceur = Dlg.GetCodeAnnonceur();
		m_CodeProduit   = Dlg.GetCodeProduit();

		// on met à jour la boîte
		bCompleteAnnonc = 2;
		this->OnUpdate();
	}
}

void CATPEtude::OnOffreImport()
{

	// Setup de la boite offre source
	m_DlgChoixOffre.SetStartupMode(CDialogChoixOffre::IMPORT, CATPColors::COLORCREUSET);

	// On passe l'offre et les terrains
	m_DlgChoixOffre.SetOffre(m_ATPDocCreuset.m_Offre);

	// Affichage de la fenêtre offre
	if(m_DlgChoixOffre.DoModal()== IDOK)
	{
		// Si éléments modifiés, on récupère les nouvelles infos
		//if(Dlg.IsModified())
		{
			m_DlgChoixOffre.GetOffre( m_ATPDocCreuset.m_Offre);

			bCompleteOffre = 2;
			this->OnUpdate();

			// Mise à jour globale
			//m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
		}
	}
}

void CATPEtude::OnFormat()
{
	CDialogFormat dlg;

	//Initialisation
	//if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
	//	dlg.SetMonoSel();
	dlg.SetData(m_ATPDocCreuset.m_Offre.m_TBLIdSupports, m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD() );
	if(!m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
		dlg.SetFormats(m_ATPDocCreuset.m_Offre.m_TBLIdFormats);

	dlg.SetColors(CATPColors::COLORCREUSET);

	//Ouverture Sélection des formats
	if( dlg.DoModal()== IDOK && dlg.IsModified() )
	{
		// Récuperation des données format
		
		
		if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
		{
			dlg.GetFormats(m_ATPDocCreuset.m_listFormatsQP);
			m_ATPDocCreuset.m_Offre.m_TBLIdFormats.Reset();
			m_ATPDocCreuset.m_Offre.m_TBLIdFormats.AddTail() = FORMAT_VIRTUEL;
		}
		else
			dlg.GetFormats(m_ATPDocCreuset.m_Offre.m_TBLIdFormats);

		m_ATPDocCreuset.m_Offre.m_Changes |= JATPOffre::FORMATS; 
		//m_ATPDocCreuset.UpdateAllViews(UPDATE_OFFRE);

		bCompleteFormat = 2;
		this->OnUpdate();
		// Update pupitre format en se remmettant en haut de la liste
		//m_pATPDocCreuset->m_DecalAffichFormat = 0;
		//m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
	}

}

void CATPEtude::OnCancel() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnCancel();
}

void CATPEtude::OnOK() 
{
	// on met à jour l'étude
	if (FALSE == this->OnUpdateEtude()) return;

	// on appelle le gestionnaire de base
	this->CDialog::OnOK();
}

void CATPEtude::SetupControls()
{
	m_Backgnd.SubclassDlgItem(IDC_CR_BK, this);
	m_Ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_btnPeriode.SubclassDlgItem(IDC_CR_PERIODE, this);
	m_btnProduit.SubclassDlgItem(IDC_CR_PRODUIT, this);
	m_btnOffre.SubclassDlgItem(IDC_CR_OFFRE, this);

	//m_btnOffre.SetMenu(IDR_CR_MENUOFFRE, m_hWnd, TRUE, IDR_CR_TOOLBAR);
	m_btnOffre.SetMenu(IDR_CR_MENUOFFRE, this, FALSE);

	m_btnFormat.SubclassDlgItem(IDC_CR_FORMAT, this);
	m_btnOk.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	SetColors();
}

void CATPEtude::SetColors()
{
	m_Backgnd.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Backgnd.SetSunken(true);
	
	m_Ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_btnPeriode.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnPeriode.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnPeriode.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	m_btnOffre.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnOffre.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnOffre.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	m_btnProduit.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnProduit.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnProduit.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	m_btnFormat.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnFormat.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_btnFormat.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	m_btnOk.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );

	m_btnCancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
}

void CATPEtude::SetIco()
{
	if (m_DateD.IsValid() && m_DateF.IsValid())
		m_btnPeriode.SetBitmaps(IDB_CR_LEDVERT, RGB(0,128,128));
	else
	{	
		// on récupère la date du système
		SYSTEMTIME SysTimeD;
		::GetLocalTime(&SysTimeD);

		// Attention si aucune date valide, on initialise avec date par défaut
		// Date début = date du jour, Date Fin = Date fin année courante4
		SysTimeD.wDay = 01;
		SysTimeD.wMonth = 01;
		m_DateD.SetDate(SysTimeD.wDay, SysTimeD.wMonth, SysTimeD.wYear);
		m_DateF.SetDate(31,12,SysTimeD.wYear);
		
		if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
			m_DateF.SetDate(30,12,2055);
		

		// Signal date par défaut
		// m_btnPeriode.SetBitmaps(IDB_CR_LEDROUGE, RGB(0,128,128));
		m_btnPeriode.SetBitmaps(IDB_CR_LEDORANGE, RGB(0,128,128));
	}	


	if (bCompleteAnnonc == 0)
		m_btnProduit.SetBitmaps(IDB_CR_LEDROUGE, RGB(0,128,128));
	if (bCompleteOffre == 0)
		m_btnOffre.SetBitmaps(IDB_CR_LEDROUGE, RGB(0,128,128));
	if (bCompleteFormat == 0)
		m_btnFormat.SetBitmaps(IDB_CR_LEDROUGE, RGB(0,128,128));

	if (bCompleteAnnonc == 1)
		m_btnProduit.SetBitmaps(IDB_CR_LEDORANGE, RGB(0,128,128));
	if (bCompleteOffre == 1)
		m_btnOffre.SetBitmaps(IDB_CR_LEDORANGE, RGB(0,128,128));
	if (bCompleteFormat == 1)
		m_btnFormat.SetBitmaps(IDB_CR_LEDORANGE, RGB(0,128,128));

	if (bCompleteAnnonc == 2)
		m_btnProduit.SetBitmaps(IDB_CR_LEDVERT, RGB(0,128,128));
	if (bCompleteOffre == 2)
		m_btnOffre.SetBitmaps(IDB_CR_LEDVERT, RGB(0,128,128));
	if (bCompleteFormat == 2)
		m_btnFormat.SetBitmaps(IDB_CR_LEDVERT, RGB(0,128,128));
}

// ancienne gestion de l'offre
void CATPEtude::OnOffre()
{

	// Setup de la boite offre source
	m_DlgChoixOffre.SetStartupMode(CDialogChoixOffre::SOURCE, CATPColors::COLORCREUSET);

	// On passe l'offre et les terrains
	m_DlgChoixOffre.SetOffre(m_ATPDocCreuset.m_Offre);

	// Affichage de la fenêtre offre
	if(m_DlgChoixOffre.DoModal()== IDOK)
	{
		// Si éléments modifiés, on récupère les nouvelles infos
		//if(Dlg.IsModified())
		{
			m_DlgChoixOffre.GetOffre( m_ATPDocCreuset.m_Offre);

			bCompleteOffre = 2;
			this->OnUpdate();

			// Mise à jour globale
			//m_pATPDocCreuset->UpdateAllViews(UPDATE_OFFRE);
		}
	}
}

/*
	CDialogFormat dlg;

	//Initialisation
	//if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
	//	dlg.SetMonoSel();
	dlg.SetData(m_ATPDocCreuset.m_Offre.m_TBLIdSupports, m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD() );
	if(!m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
		dlg.SetFormats(m_ATPDocCreuset.m_Offre.m_TBLIdFormats);

	dlg.SetColors(CATPColors::COLORCREUSET);

	//Ouverture Sélection des formats
	if( dlg.DoModal()== IDOK && dlg.IsModified() )
	{
		// Récuperation des données format
		
		
		if(m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan())
		{
			dlg.GetFormats(m_ATPDocCreuset.m_listFormatsQP);
			m_ATPDocCreuset.m_Offre.m_TBLIdFormats.Reset();
			m_ATPDocCreuset.m_Offre.m_TBLIdFormats.AddTail() = FORMAT_VIRTUEL;
		}
		else
			dlg.GetFormats(m_ATPDocCreuset.m_Offre.m_TBLIdFormats);

		m_ATPDocCreuset.m_Offre.m_Changes |= JATPOffre::FORMATS; 
		//m_ATPDocCreuset.UpdateAllViews(UPDATE_OFFRE);

		bCompleteFormat = 2;
		this->OnUpdate();
		// Update pupitre format en se remmettant en haut de la liste
		//m_pATPDocCreuset->m_DecalAffichFormat = 0;
		//m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
	}

*/

// nouvelle gestion de l'offre
void CATPEtude::OnOffre2Creer()
{
	COffre2MainDialog dlg(CATPColors::COLORCREUSET, this, true);

	// Setup de la boite offre source
	dlg.SetStartupMode(COffre2MainDialog::eodoTerrainsSupports);

	// On passe l'offre et les terrains
	dlg.SetOffre(m_ATPDocCreuset.m_Offre);

	// Affichage de la fenêtre offre
	if(dlg.DoModal()== IDOK)
	{
		// Si éléments modifiés, on récupère les nouvelles infos
		if (dlg.IsModified())
		{
			// on récupère le bouton OK
			CWnd* pButton = this->GetDlgItem(IDOK);


			// TODO : A VIRER CECI EST ICI POUR LES TESTS
			// on teste si on a trouvé le bouton
			if (pButton != NULL)
			{
				// on récupère l'état de l'étude
				BOOL State = (m_DateD.IsValid() && m_DateF.IsValid() /*&& bCompleteAnnonc && bCompleteOffre && bCompleteFormat*/) ? TRUE : FALSE;

				SetIco();

				// on met à jour le bouton
				pButton->EnableWindow(State);
			}

			// définition de l'offre
			dlg.GetOffre(m_ATPDocCreuset.m_Offre);

			// définition de la période
			m_DateD = dlg.GetPeriodeDebut();
			m_DateF = dlg.GetPeriodeFin();

			// définition du format (par défaut pour l'instant)
			m_ATPDocCreuset.m_Offre.m_TBLIdFormats.Reset();
			m_ATPDocCreuset.m_Offre.m_TBLIdFormats.AddTail() = 4;


			//bCompleteOffre = 2;
			//this->OnUpdate();
		}
	}
}
