// DlgPupitreEch.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgPupitreEch.h"
#include "JATPDocChoixSupport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgPupitreEch

IMPLEMENT_DYNAMIC(CDlgPupitreEch, JFCDialog)
CDlgPupitreEch::CDlgPupitreEch(CWnd                *pParent /*=NULL*/,
							   JATPDocChoixSupport *pDoc,
							   CDlgEchelle         * pDlg)
	: JFCDialog(CDlgPupitreEch::IDD, pParent)
{
	// r�cup�re le document
	pDocChoixSupport = pDoc;

	// r�cup�re la fenetre de base
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
// Initialisation fen�tre Echelle
BOOL CDlgPupitreEch::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre Echelles
	SetupControls();

	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fen�tre Pupitre Echelle
void CDlgPupitreEch::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Positionnement Cadre pupitre �chelle
			{IDC_CS_STATIC_PUPITRE_ECH, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le label des grandeurs s�lectionnables
			{IDC_CS_LABEL_GRANDEURS, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
									 {CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
								     {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								     {CResizer::eFixed, IDC_MAIN, CResizer::eTop}},			//Bottom side

			// Contr�le liste des grandeurs s�lectionnables
			{IDC_CS_LIST_GRANDEURS, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},			//Left side
								    {CResizer::eFixed, IDC_MAIN, CResizer::eTop},			//Top side
								    {CResizer::eFixed, IDC_MAIN, CResizer::eRight},			//Right side
								    {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},		//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contr�les fen�tre Pupitre Echelle
void CDlgPupitreEch::SetupControls()
{
	// Propri�t�s du cadre pupitre Echelle
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

	// D�finition des couleurs contr�les
	SetColors();

	// Fixe relations  fen�tres et position resize
	SetWindowRelations();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fen�tre Pupitre Echelle
void CDlgPupitreEch::SetColors()
{
	// Cadre pupitre �chelle
	m_CadrePupitreEch.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Couleur s�lection dans liste grandeurs
	m_LstGrandeurs.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
}

////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de la fen�tre pupitre Echelles
void CDlgPupitreEch::OnDestroy()
{
	JFCDialog::OnDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Resize la fen�tre pupitre Echelles
void CDlgPupitreEch::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour de la fen�tre Echelles
void CDlgPupitreEch::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		// Mise � jour du Grid r�sultat Echelles
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
	// Mise � jour des �lmts grandeurs
	if (m_LstGrandeurs.GetCount() <= 0)
	{
		// Attention si s�lection choix support non d�fini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch);

		// Impossible de continuer si aucune grandeur s�lectionn�e
		if (m_NbGrandeurs == 0) return false;

		// On balaye la liste des grandeurs ordonn�es
		CString Txt;
		int IndexList = 0;
		for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
		{
			// R�cup index
			JUnt32 Index = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, Index));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((Index == pDocChoixSupport->GrdEch_XGRP)			|| 
					(Index == pDocChoixSupport->GrdEch_XRepetition)) continue;

			m_LstGrandeurs.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandeurs.SetItemData(IndexList,Index); 

			// Incr�mente indice liste
			IndexList ++; 
		}
	}

	// Puis on s�lectionne par d�faut la 1ere grandeur
	if (m_LstGrandeurs.GetCount() > 0  && pDocChoixSupport->m_MapIdGrandEchSel.GetCount() == 0)
	{
		// Ident associ� � l'�lmt de la liste en position 0
		JUnt32 IdGrandeur = m_LstGrandeurs.GetItemData(0);

		// S�lection dans liste
		pDocChoixSupport->m_MapIdGrandEchSel.Add(0) = IdGrandeur;
		m_LstGrandeurs.SetSel(0,true);
	}

	// Update s�lection des grandeurs
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

	// Nb grandeurs de la liste � remplir
	m_NbGrandeurs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleEch);

	// Impossible de continuer si aucune grandeur s�lectionn�e
	if (m_NbGrandeurs == 0) return;

	// Chargement des grandeurs d�finies
	CString Txt;
	int IndexList = 0;

	// for (JUnt32 i=0; i<m_NbGrandeurs; i++)
	for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
	{
		// R�cup index
		JUnt32 IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

		// Ajoute libell� grandeur
		// Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, i));
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((IdGrandeur == pDocChoixSupport->GrdEch_XGRP)		 ||
				(IdGrandeur == pDocChoixSupport->GrdEch_XRepetition)) // Indice GRP+ et R�pet+ dans liste grandeur cumulativit� 
			{
				// Regarde si grandeur non pr�c�demment s�lectionn�
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

		// Si �lmt pr�c�demment s�lectionn� on repositionne la s�lection
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
			
			// Appartient au map s�lection
			if (ItemData == IdGrandeur)
			{
				m_LstGrandeurs.SetSel(IndexList,true);
				break;
			}
		}

		// Incr�mente indice liste
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
// Modification s�lection des grandeurs
void CDlgPupitreEch::OnLbnSelchangeCsListGrandeurs()
{
	// Si 1 seul s�lection en cours , on la laisse
	if (m_LstGrandeurs.GetSelCount() > 0)
	{
		// Modif s�lection grandeurs
		pDocChoixSupport->m_MapIdGrandEchSel.Reset();
		for (JUnt32 i = 0; i < m_LstGrandeurs.GetCount(); i++)
		{
			if (m_LstGrandeurs.GetSel(i) > 0)
				// R�cup�re identifiant type grandeur
				pDocChoixSupport->m_MapIdGrandEchSel.Add(pDocChoixSupport->m_MapIdGrandEchSel.GetCount()) = m_LstGrandeurs.GetItemData(i); 
		}

		// Mise � jour des diff�rentes fen�tres 
		pDlgEchelle->UpdateResultEchelle(false);
	}
	else
	{
		// R�cup�re derni�re s�lection
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

		// Mise � jour des diff�rentes fen�tres 
		pDlgEchelle->UpdateResultEchelle(false);

	}
}

////////////////////////////////////////////////////////////////////////
// Mise � jour des s�lections grandeurs
void CDlgPupitreEch::UpdateSelGrandeurs()
{
	// D�selectionne toutes les grandeurs
	for (JUnt32 i= 0; i < m_LstGrandeurs.GetCount(); i++)
	{
		m_LstGrandeurs.SetSel(i,false);
	}

	// Si au moins 1 grandeur s�lectionn�e
	if (pDocChoixSupport->m_MapIdGrandEchSel.GetCount() > 0)
	{
		// Boucle sur les grandeurs s�lectionn�es
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
					// S�lection dans liste
					m_LstGrandeurs.SetSel(i,true);
					break;
				}
			}
		}
	}
	else
	{
		// par d�faut on s�lectionne le 1er �l�ment
		m_LstGrandeurs.SetSel(0,true);
		pDocChoixSupport->m_MapIdGrandEchSel.MoveTo(0);
		if (!pDocChoixSupport->m_MapIdGrandEchSel.IsValid()) 
			pDocChoixSupport->m_MapIdGrandEchSel.Add(0) = true; 
	}
}

//////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgPupitreEch::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise � jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
	}
	///////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Mise � jour des grandeurs disponibles
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
				// Ajoute la grandeur � la liste des grandeurs disponibles	
				pDocChoixSupport->m_LstOrdoGrandeurEch.AddTail() = pDocChoixSupport->GrdEch_XGRP; 
			}
			if (!RepetPlusExist)
			{
				// Ajoute la grandeur � la liste des grandeurs disponibles	
				pDocChoixSupport->m_LstOrdoGrandeurEch.AddTail() = pDocChoixSupport->GrdEch_XRepetition; 
			}
		}

		AfficheListOrdoGrandeurs();
	}

	// Mise � jour suite � modification ordre grandeurs �chelle
	else if (Message == UPDATE_ORDRE_GRANDEUR_ECHELLE)
	{
		// Mise � jour ordre liste des grandeurs dans panneau �chelle
		AfficheListOrdoGrandeurs();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour ordre liste des grandeurs dans panneau �chelle
void CDlgPupitreEch::AfficheListOrdoGrandeurs()
{
	// R�initilaise la liste courante
	m_LstGrandeurs.ResetContent();

	// Mise � jour des �lmts grandeurs
	if (m_LstGrandeurs.GetCount() <= 0)
	{
		// Boucle sur la liste ordonn�e des grandeurs
		CString Txt;
		int IndexList = 0;
		for (pDocChoixSupport->m_LstOrdoGrandeurEch.MoveFirst();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.IsValid();
			 pDocChoixSupport->m_LstOrdoGrandeurEch.MoveNext())
		{
			// R�cup�re ident grandeur correspondant � l'indice
			JUnt32 IdGrandeur = pDocChoixSupport->m_LstOrdoGrandeurEch.GetItem();

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((IdGrandeur == pDocChoixSupport->GrdEch_XGRP)			|| 
					(IdGrandeur == pDocChoixSupport->GrdEch_XRepetition)) continue;

			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleEch, IdGrandeur));
			m_LstGrandeurs.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandeurs.SetItemData(IndexList,IdGrandeur); 

			// Voir si pr�cedemment s�lectionn�e
			for (pDocChoixSupport->m_MapIdGrandEchSel.MoveFirst();
				 pDocChoixSupport->m_MapIdGrandEchSel.IsValid();
				 pDocChoixSupport->m_MapIdGrandEchSel.MoveNext())
			{
				if (pDocChoixSupport->m_MapIdGrandEchSel.GetItem() == IdGrandeur)
				{
					// S�lection dans liste
					m_LstGrandeurs.SetSel(IndexList,true);
					break;
				}
			}

			// Incr�mente indice liste
			IndexList ++; 
		}
	}
	
	// Redessine la liste
	m_LstGrandeurs.InvalidateRect(NULL,FALSE);
}