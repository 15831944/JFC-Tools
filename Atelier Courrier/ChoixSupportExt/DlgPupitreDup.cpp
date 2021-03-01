// DlgPupitreDup.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgPupitreDup.h"
#include "JATPDocChoixSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgPupitreDup

IMPLEMENT_DYNAMIC(CDlgPupitreDup, JFCDialog)
CDlgPupitreDup::CDlgPupitreDup(CWnd* pParent /*=NULL*/,
							   JATPDocChoixSupport *pDoc,
							   CDlgDuplication     *pDlg)
	: JFCDialog(CDlgPupitreDup::IDD, pParent)
{
	// R�cup�re le document
	pDocChoixSupport = pDoc;

	// R�cup�re la fen�tre de base
	pDlgDup     	 = pDlg;
}

CDlgPupitreDup::~CDlgPupitreDup()
{
}

void CDlgPupitreDup::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_LABEL_GRANDDUP, m_LblGrandDup);
	DDX_Control(pDX, IDC_CS_LIST_GRANDDUP, m_LstGrandDup);
	DDX_Control(pDX, IDC_CS_STATIC_PUPITRE_DUP, m_CadrePupitreDup);
}

BEGIN_MESSAGE_MAP(CDlgPupitreDup, JFCDialog)
	ON_LBN_SELCHANGE(IDC_CS_LIST_GRANDDUP, OnLbnSelchangeCsListGranddup)
	ON_WM_SIZE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation fen�tre Pupitre Cumualtivit�
BOOL CDlgPupitreDup::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre Echelles
	SetupControls();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contr�les fen�tre Pupitre Duplication
void CDlgPupitreDup::SetupControls()
{
	// Propri�t�s du cadre pupitre Duplication
	m_CadrePupitreDup.SetTransparent(false);
	m_CadrePupitreDup.SetSunken(true);

	// Controles en avant plan
	m_CadrePupitreDup.AddTopWindow(m_LblGrandDup);
	m_CadrePupitreDup.AddTopWindow(m_LstGrandDup);
	
	// Style Label Grandeurs
	m_LblGrandDup.SetFontName(CATPColors::GetFontLabelName());
	m_LblGrandDup.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblGrandDup.SetBorder(false);

	// D�finition des couleurs contr�les
	SetColors();

	// Fixe relations  fen�tres et position resize
	SetWindowRelations();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fen�tre Pupitre Cumulativit�
void CDlgPupitreDup::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Positionnement Cadre pupitre Duplication
			{IDC_CS_STATIC_PUPITRE_DUP, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le label des grandeurs s�lectionnables
			{IDC_CS_LABEL_GRANDDUP,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop}},		//Bottom side

			// Contr�le liste des grandeurs s�lectionnables
			{IDC_CS_LIST_GRANDDUP,		{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fen�tre Pupitre Duplication
void CDlgPupitreDup::SetColors()
{
	// Cadre pupitre duplication
	m_CadrePupitreDup.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Label grandeur
	m_LblGrandDup.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblGrandDup.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Couleur s�lection dans liste grandeurs
	m_LstGrandDup.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
}

/////////////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgPupitreDup::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise � jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
		SetColors();
	}
}


/////////////////////////////////////////////////////////////////////////////////////
// Resize dz la fen�tre pupitre duplication
void CDlgPupitreDup::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs
bool CDlgPupitreDup::LoadGrandeurs()
{
	// Mise � jour des �lmts grandeurs
	if (m_LstGrandDup.GetCount() <= 0)
	{
		// Attention si s�lection choix support non d�fini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleDup);

		// Impossible de continuer si aucune grandeur s�lectionn�e
		if (m_NbGrandeurs == 0) return false;

		// Chargement des grandeurs d�finies
		CString Txt;
		JUnt32 InxList = 0;
		for (JUnt32 i=0; i<m_NbGrandeurs; i++)
		{
			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleDup, i));

			/* A VOIR
			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((i == GrdDup_XGRP)) || (i == GrdDup_XRepetition)) continue;
			*/

			m_LstGrandDup.AddString(Txt);

			// Ajoute ident grandeur
			// m_LstGrandCum.SetItemData(i,i); 
			m_LstGrandDup.SetItemData(InxList,i); 

			// Incr�mente indice liste r�el
			InxList++;
		}
	}

	// Puis on s�lectionne par d�faut la 1ere grandeur
	if (m_LstGrandDup.GetCount() > 0 )
	{
		JUnt32 IdGrandeur = m_LstGrandDup.GetItemData(0);

		// Voir si on l'ajoute au map s�lection
		pDocChoixSupport->m_MapIdGrandDupSel.MoveTo(IdGrandeur);
		if (!pDocChoixSupport->m_MapIdGrandDupSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandDupSel.Add(0) = IdGrandeur;

		// S�lection dans liste
		m_LstGrandDup.SetCurSel(0);
	}

	UpdateSelGrandeurs();

	return true;
}

////////////////////////////////////////////////////////////////////////
// Mise � jour des s�lections grandeurs
void CDlgPupitreDup::UpdateSelGrandeurs()
{
	// D�selectionne toutes les grandeurs
	for (JUnt32 i= 0; i < m_LstGrandDup.GetCount(); i++)
	{
		m_LstGrandDup.SetSel(i,false);
	}

	// Si au moins 1 grandeur s�lectionn�e
	if (pDocChoixSupport->m_MapIdGrandDupSel.GetCount() > 0)
	{
		// Boucle sur les grandeurs s�lectionn�es
		for (pDocChoixSupport->m_MapIdGrandDupSel.MoveFirst();
			 pDocChoixSupport->m_MapIdGrandDupSel.IsValid();
			 pDocChoixSupport->m_MapIdGrandDupSel.MoveNext())
		{
			// Boucle sur les grandeurs existantes
			for (JUnt32 i= 0; i < m_LstGrandDup.GetCount(); i++)
			{
				// Ident grandeur
				JUnt32 IdGrandeur = m_LstGrandDup.GetItemData(i);

				if (IdGrandeur == pDocChoixSupport->m_MapIdGrandDupSel.GetItem())
				{
					// S�lection dans liste
					m_LstGrandDup.SetCurSel(i);
					break;
				}
			}
		}
	}
	else
		// par d�faut on s�lectionne le 1er �l�ment
		m_LstGrandDup.SetCurSel(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation de la liste
void CDlgPupitreDup::InitLstGrandeurs()
{
	m_LstGrandDup.ResetContent(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Change grandeur � afficher
void CDlgPupitreDup::OnLbnSelchangeCsListGranddup()
{
	// Modif s�lection grandeurs
	pDocChoixSupport->m_MapIdGrandDupSel.Reset();
	for (JUnt32 i = 0; i < m_LstGrandDup.GetCount(); i++)
	{
		if (m_LstGrandDup.GetSel(i) > 0)
		{
			// R�cup�re identifiant type grandeur
			JUnt32 IdGrandeur = m_LstGrandDup.GetItemData(i);
			pDocChoixSupport->m_MapIdGrandDupSel.Add(pDocChoixSupport->m_MapIdGrandDupSel.GetCount()) = m_LstGrandDup.GetItemData(i); 
			break;
		}
	}

	// Mise � jour des diff�rentes fen�tres 
	pDlgDup->UpdateResultDup(false);
	
}
