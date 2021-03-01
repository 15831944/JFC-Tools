// DlgPupitreEch.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgPupitreEch.h"
#include "JATPDocChoixSupport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Boîte de dialogue CDlgPupitreEch

IMPLEMENT_DYNAMIC(CDlgPupitreEch, JFCDialog)
CDlgPupitreEch::CDlgPupitreEch(CWnd                *pParent /*=NULL*/,
							   JATPDocChoixSupport *pDoc,
							   CDlgEchelle         * pDlg)
	: JFCDialog(CDlgPupitreEch::IDD, pParent)
{
	// récupère le document
	pDocChoixSupport = pDoc;

	// récupére la fenetre de base
	pDlgEchelle = pDlg;
}

CDlgPupitreEch::~CDlgPupitreEch()
{
}

void CDlgPupitreEch::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_LABEL_GRANDEURS, m_LblGrandeurs);
	DDX_Control(pDX, IDC_CS_LIST_GRANDEURS, m_LstGrandeurs);
	DDX_Control(pDX, IDC_CS_STATIC_PUPITRE_ECH, m_CadrePupitreEch);
}


BEGIN_MESSAGE_MAP(CDlgPupitreEch, JFCDialog)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_CS_LIST_GRANDEURS, OnLbnSelchangeCsListGrandeurs)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation fenêtre Echelle
BOOL CDlgPupitreEch::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fenêtre Echelles
	SetupControls();

	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fenêtre Pupitre Echelle
void CDlgPupitreEch::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Positionnement Cadre pupitre échelle
			{IDC_CS_STATIC_PUPITRE_ECH, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contrôle label des grandeurs sélectionnables
			{IDC_CS_LABEL_GRANDEURS, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
									 {CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
								     {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								     {CResizer::eFixed, IDC_MAIN, CResizer::eTop}},			//Bottom side

			// Contrôle liste des grandeurs sélectionnables
			{IDC_CS_LIST_GRANDEURS, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
								    {CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
								    {CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
								    {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contrôles fenêtre Pupitre Echelle
void CDlgPupitreEch::SetupControls()
{
	// Propriétés du cadre pupitre Echelle
	m_CadrePupitreEch.SetTransparent(false);
	m_CadrePupitreEch.SetSunken(true);

	// Controles en avant plan
	m_CadrePupitreEch.AddTopWindow(m_LblGrandeurs);
	m_CadrePupitreEch.AddTopWindow(m_LstGrandeurs);
	
	// Style Label Grandeurs
	/*
	m_LblGrandeurs.SetFontName(CATPColors::GetFontLabelName());
	m_LblGrandeurs.SetFontSize(CATPColors::GetFontLabelSize());
	m_LblGrandeurs.SetBorder(false);
	*/
	m_LblGrandeurs.SetAppColor(CATPColors::COLORCHOIXSUPPORTS);

	// Définition des couleurs contrôles
	SetColors();

	// Fixe relations  fenêtres et position resize
	SetWindowRelations();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fenêtre Pupitre Echelle
void CDlgPupitreEch::SetColors()
{
	// Cadre pupitre échelle
	m_CadrePupitreEch.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Couleur sélection dans liste grandeurs
	m_LstGrandeurs.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
}

////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de la fenêtre pupitre Echelles
void CDlgPupitreEch::OnDestroy()
{
	JFCDialog::OnDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Resize la fenêtre pupitre Echelles
void CDlgPupitreEch::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la fenêtre Echelles
void CDlgPupitreEch::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		// Mise à jour du Grid résultat Echelles
		// AfxMessageBox("A FAIRE / GRID RESULTAT",MB_ICONINFORMATION);	
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs (attention aux grandeurs invalides pour certains types de calcul)
//
// exemple : pour moteur full binomial pas de GRP+ ni de reprise
//
bool CDlgPupitreEch::LoadGrandeurs()
{
	// Mise à jour des élmts grandeurs
	if (m_LstGrandeurs.GetCount() <= 0)
	{
		// Attention si sélection choix support non défini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch);

		// Impossible de continuer si aucune grandeur sélectionnée
		if (m_NbGrandeurs == 0) return false;

		// On balaye la liste des grandeurs ordonnées
		CString Txt;
		int IndexList = 0;
		for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
		{
			// Récup index
			JUnt32 Index = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

			// Ajoute libellé grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, Index));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((Index == pDocChoixSupport->GrdEch_XGRP)			|| 
					(Index == pDocChoixSupport->GrdEch_XRepetition)) continue;

			m_LstGrandeurs.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandeurs.SetItemData(IndexList,Index); 

			// Incrémente indice liste
			IndexList ++; 
		}
	}

	// Puis on sélectionne par défaut la 1ere grandeur
	if (m_LstGrandeurs.GetCount() > 0  && pDocChoixSupport->m_MapIdGrandEchSel.GetCount() == 0)
	{
		// Ident associé à l'élmt de la liste en position 0
		JUnt32 IdGrandeur = m_LstGrandeurs.GetItemData(0);

		// Sélection dans liste
		pDocChoixSupport->m_MapIdGrandEchSel.Add(0) = IdGrandeur;
		m_LstGrandeurs.SetSel(0,true);
	}

	// Update sélection des grandeurs
	UpdateSelGrandeurs();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs (attention aux grandeurs invalides pour certains types de calcul)
//
// exemple : pour moteur full binomial pas de GRP+ ni de reprise
//
void CDlgPupitreEch::MajGrandeurs()
{
	// Reset de la liste
	m_LstGrandeurs.ResetContent();

	// Nb grandeurs de la liste à remplir
	m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch);

	// Impossible de continuer si aucune grandeur sélectionnée
	if (m_NbGrandeurs == 0) return;

	// Chargement des grandeurs définies
	CString Txt;
	int IndexList = 0;

	// for (JUnt32 i=0; i<m_NbGrandeurs; i++)
	for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
	{
		// Récup index
		JUnt32 IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

		// Ajoute libellé grandeur
		// Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, i));
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((IdGrandeur == pDocChoixSupport->GrdEch_XGRP)		 ||
				(IdGrandeur == pDocChoixSupport->GrdEch_XRepetition)) // Indice GRP+ et Répet+ dans liste grandeur cumulativité 
			{
				// Regarde si grandeur non précédemment sélectionné
				for (pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
					 pDocChoixSupport->m_MapIdGrandEchSel.IsValid();
					 pDocChoixSupport->m_MapIdGrandEchSel.MoveNext())
				{
					JUnt32 ItemData = pDocChoixSupport->m_MapIdGrandEchSel.GetItem();
					if (ItemData == IdGrandeur)
					{
						pDocChoixSupport->m_MapIdGrandEchSel.Remove();
						break;
					}
				}
				continue;
			}
		}

		m_LstGrandeurs.AddString(Txt);

		// Ajoute ident grandeur
		m_LstGrandeurs.SetItemData(IndexList,IdGrandeur); 

		// Si élmt précédemment sélectionné on repositionne la sélection
		/*
		MapSelListe.MoveTo(i);
		if (MapSelListe.IsValid())
		{
			JUnt32 EtatSel = MapSelListe.GetItem();
			if (EtatSel > 0)
				m_LstGrandeurs.SetSel(IndexList,true);
		}
		*/
		for (pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
					 pDocChoixSupport->m_MapIdGrandEchSel.IsValid();
					 pDocChoixSupport->m_MapIdGrandEchSel.MoveNext())
		{
			JUnt32 ItemData = pDocChoixSupport->m_MapIdGrandEchSel.GetItem();
			
			// Appartient au map sélection
			if (ItemData == IdGrandeur)
			{
				m_LstGrandeurs.SetSel(IndexList,true);
				break;
			}
		}

		// Incrémente indice liste
		IndexList ++; 
	}

	UpdateSelGrandeurs();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation de la liste
void CDlgPupitreEch::InitLstGrandeurs()
{
	m_LstGrandeurs.ResetContent(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Modification sélection des grandeurs
void CDlgPupitreEch::OnLbnSelchangeCsListGrandeurs()
{
	// Si 1 seul sélection en cours , on la laisse
	if (m_LstGrandeurs.GetSelCount() > 0)
	{
		// Modif sélection grandeurs
		pDocChoixSupport->m_MapIdGrandEchSel.Reset();
		for (JUnt32 i = 0; i < m_LstGrandeurs.GetCount(); i++)
		{
			if (m_LstGrandeurs.GetSel(i) > 0)
				// Récupère identifiant type grandeur
				pDocChoixSupport->m_MapIdGrandEchSel.Add(pDocChoixSupport->m_MapIdGrandEchSel.GetCount()) = m_LstGrandeurs.GetItemData(i); 
		}

		// Mise à jour des différentes fenêtres 
		pDlgEchelle->UpdateResultEchelle(false);
	}
	else
	{
		// Récupère dernière sélection
		JUnt32 IndexGrandeur;
		pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
		IndexGrandeur = pDocChoixSupport->m_MapIdGrandEchSel.GetItem();
		for (JUnt32 i=0; i< m_LstGrandeurs.GetCount(); i++)
		{
			if (m_LstGrandeurs.GetItemData(i) == IndexGrandeur)
			{
				m_LstGrandeurs.SetSel(i,TRUE);
			}
		}

		// Mise à jour des différentes fenêtres 
		pDlgEchelle->UpdateResultEchelle(false);

	}
}

////////////////////////////////////////////////////////////////////////
// Mise à jour des sélections grandeurs
void CDlgPupitreEch::UpdateSelGrandeurs()
{
	// Déselectionne toutes les grandeurs
	for (JUnt32 i= 0; i < m_LstGrandeurs.GetCount(); i++)
	{
		m_LstGrandeurs.SetSel(i,false);
	}

	// Si au moins 1 grandeur sélectionnée
	if (pDocChoixSupport->m_MapIdGrandEchSel.GetCount() > 0)
	{
		// Boucle sur les grandeurs sélectionnées
		for (pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
			pDocChoixSupport->m_MapIdGrandEchSel.IsValid();
			pDocChoixSupport->m_MapIdGrandEchSel.MoveNext())
		{
			// Boucle sur les grandeurs existantes
			for (JUnt32 i= 0; i < m_LstGrandeurs.GetCount(); i++)
			{
				// Ident grandeur
				JUnt32 IdGrandeur = m_LstGrandeurs.GetItemData(i);

				if (IdGrandeur == pDocChoixSupport->m_MapIdGrandEchSel.GetItem())
				{
					// Sélection dans liste
					m_LstGrandeurs.SetSel(i,true);
					break;
				}
			}
		}
	}
	else
	{
		// par défaut on sélectionne le 1er élément
		m_LstGrandeurs.SetSel(0,true);
		pDocChoixSupport->m_MapIdGrandEchSel.MoveTo(0);
		if (!pDocChoixSupport->m_MapIdGrandEchSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandEchSel.Add(0) = true; 
	}
}

//////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID CDlgPupitreEch::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise à jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
	}
	///////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Mise à jour des grandeurs disponibles
		MajGrandeurs();

		// A REVOIR
		if (pDocChoixSupport->m_MoteurPresse.GetType() !=  CMoteurPresse::PROBA)
		{
			bool GRPPlusExist	= false;
			bool RepetPlusExist = false;
			for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
				 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
				 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
			{
					JUnt32 IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();
					if (IdGrandeur == pDocChoixSupport->GrdEch_XGRP)
						GRPPlusExist = true;
					if (IdGrandeur == pDocChoixSupport->GrdEch_XRepetition)
						RepetPlusExist = true;
			}
		
			if (!GRPPlusExist)
			{
				// Ajoute la grandeur à la liste des grandeurs disponibles	
				pDocChoixSupport->m_LstOrdoGrandeurEch.AddTail() = pDocChoixSupport->GrdEch_XGRP; 
			}
			if (!RepetPlusExist)
			{
				// Ajoute la grandeur à la liste des grandeurs disponibles	
				pDocChoixSupport->m_LstOrdoGrandeurEch.AddTail() = pDocChoixSupport->GrdEch_XRepetition; 
			}
		}

		AfficheListOrdoGrandeurs();
	}

	// Mise à jour suite à modification ordre grandeurs échelle
	else if (Message == UPDATE_ORDRE_GRANDEUR_ECHELLE)
	{
		// Mise à jour ordre liste des grandeurs dans panneau échelle
		AfficheListOrdoGrandeurs();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour ordre liste des grandeurs dans panneau échelle
void CDlgPupitreEch::AfficheListOrdoGrandeurs()
{
	// Réinitilaise la liste courante
	m_LstGrandeurs.ResetContent();

	// Mise à jour des élmts grandeurs
	if (m_LstGrandeurs.GetCount() <= 0)
	{
		// Boucle sur la liste ordonnée des grandeurs
		CString Txt;
		int IndexList = 0;
		for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
		{
			// Récupère ident grandeur correspondant à l'indice
			JUnt32 IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((IdGrandeur == pDocChoixSupport->GrdEch_XGRP)			|| 
					(IdGrandeur == pDocChoixSupport->GrdEch_XRepetition)) continue;

			// Ajoute libellé grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur));
			m_LstGrandeurs.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandeurs.SetItemData(IndexList,IdGrandeur); 

			// Voir si précedemment sélectionnée
			for (pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
				 pDocChoixSupport->m_MapIdGrandEchSel.IsValid();
				 pDocChoixSupport->m_MapIdGrandEchSel.MoveNext())
			{
				if (pDocChoixSupport->m_MapIdGrandEchSel.GetItem() == IdGrandeur)
				{
					// Sélection dans liste
					m_LstGrandeurs.SetSel(IndexList,true);
					break;
				}
			}

			// Incrémente indice liste
			IndexList ++; 
		}
	}
	
	// Redessine la liste
	m_LstGrandeurs.InvalidateRect(NULL,FALSE);
}