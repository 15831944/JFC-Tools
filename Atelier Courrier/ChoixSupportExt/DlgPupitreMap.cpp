// DlgPupitreMap.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "ChoixSupport.h"
#include "DlgPupitreMap.h"
#include "JATPDocChoixSupport.h"
#include ".\dlgpupitremap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Bo�te de dialogue CDlgPupitreMap
IMPLEMENT_DYNAMIC(CDlgPupitreMap, JFCDialog)
CDlgPupitreMap::CDlgPupitreMap(CWnd* pParent /*=NULL*/,
							   JATPDocChoixSupport *pDoc,
							   CDlgMapping         * pDlg)
	: JFCDialog(CDlgPupitreMap::IDD, pParent)
{
	// r�cup�re le document
	pDocChoixSupport = pDoc;

	// r�cup�re la fenetre de base
	pDlgMapping = pDlg;
}

CDlgPupitreMap::~CDlgPupitreMap()
{
}

void CDlgPupitreMap::DoDataExchange(CDataExchange* pDX)
{
	JFCDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CS_STATIC_PUPITRE_MAP, m_CadrePupitreMap);
	DDX_Control(pDX, IDC_CS_LABEL_ABS_MAPPING, m_LblAbscisse);
	DDX_Control(pDX, IDC_CS_LABEL_ORD_MAPPING, m_LblOrdonnee);
	DDX_Control(pDX, IDC_LIST_GRANDABS, m_LstGrandAbs);
	DDX_Control(pDX, IDC_LIST_GRANDORD, m_LstGrandOrd);
}


BEGIN_MESSAGE_MAP(CDlgPupitreMap, JFCDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST_GRANDORD, OnLbnSelChangeListGrandOrd)
	ON_LBN_SELCHANGE(IDC_LIST_GRANDABS, OnLbnSelChangeListGrandAbs)
	
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation fen�tre pupitre Mapping
BOOL CDlgPupitreMap::OnInitDialog()
{
	JFCDialog::OnInitDialog();

	// SetUp des controles fen�tre pupitre Mapping
	SetupControls();

	return 	TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des relations entre objets de la fen�tre Pupitre Mapping
void CDlgPupitreMap::SetWindowRelations()
{
	static CResizer::CBorderInfo s_bi[] = {

			// Positionnement Cadre pupitre
			{IDC_CS_STATIC_PUPITRE_MAP, {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le label Abscisse
			{IDC_CS_LABEL_ABS_MAPPING,  {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
									    {CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eTop}},		//Bottom side

			// Contr�le liste des grandeurs dispo en abscisse
			{IDC_LIST_GRANDABS,			{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
										{CResizer::eFixed, IDC_MAIN, CResizer::eTop},		//Top side
										{CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le label Ordonn�e
			{IDC_CS_LABEL_ORD_MAPPING,  {CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
									    {CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
										{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side

			// Contr�le liste des grandeurs dispo en abscisse
			{IDC_LIST_GRANDORD,			{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},		//Left side
									    {CResizer::eFixed, IDC_MAIN, CResizer::eBottom},	//Top side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eRight},		//Right side
								        {CResizer::eFixed, IDC_MAIN, CResizer::eBottom}},	//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Set up de tous les contr�les fen�tre Pupitre Mapping
void CDlgPupitreMap::SetupControls()
{
	// Prori�t�s du cadre pupitre Mapping
	m_CadrePupitreMap.SetTransparent(false);
	m_CadrePupitreMap.SetSunken(true);

	// Controles en avant plan
	m_CadrePupitreMap.AddTopWindow(m_LblAbscisse);
	m_CadrePupitreMap.AddTopWindow(m_LblOrdonnee);
	m_CadrePupitreMap.AddTopWindow(m_LstGrandAbs);
	m_CadrePupitreMap.AddTopWindow(m_LstGrandOrd);
	/*
	m_CadrePupitreMap.AddTopWindow(m_BtnMapFamilles);
	m_CadrePupitreMap.AddTopWindow(m_BtnMapCentres);
	m_CadrePupitreMap.AddTopWindow(m_BtnMapSupports);
	*/

	// D�finition des couleurs contr�les
	SetColors();

	// Fixe relations  fen�tres et position resize
	SetWindowRelations();

}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des couleurs pour les objets fen�tre Pupitre Mapping
void CDlgPupitreMap::SetColors()
{
	// Cadre pupitre Mapping
	m_CadrePupitreMap.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Controles label abscisse et ordonn�e
	m_LblAbscisse.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblAbscisse.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	m_LblOrdonnee.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCHOIXSUPPORTS),CATPColors::COLORCHOIXSUPPORTS,CATPColors::DARK_DEGRAD);
	m_LblOrdonnee.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCHOIXSUPPORTS));

	// Couleur s�lection dans liste abscisse et ordonn�e
	m_LstGrandAbs.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
	m_LstGrandOrd.SetSelectColor(CATPColors::GetColorSelect(CATPColors::COLORCHOIXSUPPORTS));
}

////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de la fen�tre pupitre Mapping
void CDlgPupitreMap::OnDestroy()
{
	JFCDialog::OnDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Resize la fen�tre pupitre Mapping
void CDlgPupitreMap::OnSize(UINT nType, int cx, int cy)
{
	JFCDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour de la fen�tre pupitre Mapping
void CDlgPupitreMap::OnShowWindow(BOOL bShow, UINT nStatus)
{
	JFCDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		// Mise � jour du Grid r�sultat Echelles
		// AfxMessageBox("A FAIRE / GRID RESULTAT",MB_ICONINFORMATION);	
	}
}

//////////////////////////////////////////
// la fonction pour mettre � jour les vues
VOID CDlgPupitreMap::UpdateAllViews(const JFCUpdate & Message)
{
	//////////////////////////////////////////////////////////////////////////
	// Mise � jour couleur Interface Choix Supports
	if(Message == UPDATE_COLOR)
	{
	}
	///////////////////////////////////////// CHANGEMENT TYPE MOTEUR ////////////////////////////////
	//
	else if (Message == UPDATE_TYPEMOTEUR)
	{
		// Mise � jour des grandeurs disponibles
		MajGrandeurs();
	}

	// Mise � jour des listes grandeurs
	m_LstGrandAbs.Invalidate();
	m_LstGrandOrd.Invalidate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs pour abscisse et ordonn�e mapping
bool CDlgPupitreMap::LoadGrandeurs()
{
	// Chargement des grandeurs en abscisse
	if (!LoadGrandeursAbs()) return false;

	// Chargement des grandeurs en ordonn�e
	if (!LoadGrandeursOrd()) return false;

	// Update s�lection des grandeurs
	UpdateSelGrandeurs();
	return true;
}

////////////////////////////////////////////////////////////////////////
// Mise � jour des s�lections grandeurs
void CDlgPupitreMap::UpdateSelGrandeurs()
{
	// D�selectionne toutes les grandeurs abscisse
	for (JUnt32 i= 0; i < m_LstGrandAbs.GetCount(); i++)
	{
		m_LstGrandAbs.SetSel(i,false);
	}

	// Si au moins 1 grandeur s�lectionn�e
	if (pDocChoixSupport->m_IdGrandMapSelAbs > 0)
	{
		// Boucle sur les grandeurs existantes
		for (JUnt32 i= 0; i < m_LstGrandAbs.GetCount(); i++)
		{
			// Ident grandeur
			JUnt32 IdGrandeur = m_LstGrandAbs.GetItemData(i);

			if (IdGrandeur == pDocChoixSupport->m_IdGrandMapSelAbs)
			{
				// S�lection dans liste
				m_LstGrandAbs.SetCurSel(i);
				break;
			}
		}
	}
	else
	{
		// par d�faut on s�lectionne le 1er �l�ment (LDP/LNM)
		m_LstGrandAbs.SetCurSel(0);
		pDocChoixSupport->m_IdGrandMapSelAbs = 0; 
	}

	// D�selectionne toutes les grandeurs ordonn�e
	for (JUnt32 i= 0; i < m_LstGrandOrd.GetCount(); i++)
	{
		m_LstGrandOrd.SetSel(i,false);
	}

	// Si au moins 1 grandeur s�lectionn�e
	if (pDocChoixSupport->m_IdGrandMapSelOrd > 0)
	{
		// Boucle sur les grandeurs existantes
		for (JUnt32 i= 0; i < m_LstGrandOrd.GetCount(); i++)
		{
			// Ident grandeur
			JUnt32 IdGrandeur = m_LstGrandOrd.GetItemData(i);

			if (IdGrandeur == pDocChoixSupport->m_IdGrandMapSelOrd)
			{
				// S�lection dans liste
				m_LstGrandOrd.SetCurSel(i);
				break;
			}
		}
	}
	else
	{
		// par d�faut on s�lectionne l'�l�ment Co�t GRP
		m_LstGrandOrd.SetCurSel(pDocChoixSupport->GrdMap_CoutGRP);
		pDocChoixSupport->m_IdGrandMapSelOrd = pDocChoixSupport->GrdMap_CoutGRP; 
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs pour abscisse mapping
bool CDlgPupitreMap::LoadGrandeursAbs()
{
	/////////////////////////////////// Grandeurs ABSCISSE /////////////////////////////////
	// Mise � jour des �lmts grandeurs abscisse mapping
	if (m_LstGrandAbs.GetCount() <= 0)
	{
		// Attention si s�lection choix support non d�fini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandAbs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleMap);

		// Impossible de continuer si aucune grandeur s�lectionn�e
		if (m_NbGrandAbs == 0) return false;

		// Chargement des grandeurs d�finies
		/*
		CString Txt;
		int IndexList = 0;
		for (JUnt32 i=0; i<m_NbGrandAbs; i++)
		{
			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, i));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((i == pDocChoixSupport->GrdMap_XGRP)			|| 
					(i == pDocChoixSupport->GrdMap_XRepetition)) continue;

			// On ne prend plus l'indice cumulativit� pour le moment (nb insertions invariant � 1)
			if (i==pDocChoixSupport->GrdMap_Cumulativite) continue;

			m_LstGrandAbs.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandAbs.SetItemData(IndexList,i); 

			// Incr�mente indice liste
			IndexList ++; 
		}
		*/

		CString Txt;
		int IndexList = 0;
		for (pDocChoixSupport->m_MapAbrGrandMap.MoveFirst(); 
			 pDocChoixSupport->m_MapAbrGrandMap.IsValid(); 
			 pDocChoixSupport->m_MapAbrGrandMap.MoveNext())
		{
			// R�cup Id Grandeur
			JUnt32 IdGrandeur = pDocChoixSupport->m_MapAbrGrandMap.GetKey();

			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, IdGrandeur));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((IdGrandeur == pDocChoixSupport->GrdMap_XGRP)			|| 
					(IdGrandeur == pDocChoixSupport->GrdMap_XRepetition)) continue;

			// On ne prend plus l'indice cumulativit� pour le moment (nb insertions invariant � 1)
			if (IdGrandeur == pDocChoixSupport->GrdMap_Cumulativite) continue;

			// Ajoute dans la liste des grandeurs abscisse
			m_LstGrandAbs.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandAbs.SetItemData(IndexList,IdGrandeur); 

			// Incr�mente indice liste
			IndexList ++; 
		}
	}

	// Puis on s�lectionne par d�faut la 1ere grandeur
	if (m_LstGrandAbs.GetCount() > 0  && pDocChoixSupport->m_IdGrandMapSelAbs <= 0)
	{
		// Ident associ� � l'�lmt de la liste en position 0
		JUnt32 IdGrandeur = m_LstGrandAbs.GetItemData(0);

		// S�lection dans liste
		pDocChoixSupport->m_IdGrandMapSelAbs = IdGrandeur;
		m_LstGrandAbs.SetCurSel(0);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage liste grandeurs pour ordonn�e mapping
bool CDlgPupitreMap::LoadGrandeursOrd()
{
	/////////////////////////////////// Grandeurs ORDONNEE /////////////////////////////////
	// Mise � jour des �lmts grandeurs ordonn�e mapping
	if (m_LstGrandOrd.GetCount() <= 0)
	{
		// Attention si s�lection choix support non d�fini
		if (pDocChoixSupport == NULL || !pDocChoixSupport->IsValid()) return false;

		// Nombre de grandeurs utilisables pour les calculs
		m_NbGrandOrd = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleMap);

		// Impossible de continuer si aucune grandeur s�lectionn�e
		if (m_NbGrandOrd == 0) return false;

		// Chargement des grandeurs d�finies
		/*
		CString Txt;
		int IndexList = 0;
		for (JUnt32 i=0; i<m_NbGrandOrd; i++)
		{
			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, i));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
			{
				if ((i == pDocChoixSupport->GrdMap_XGRP)	   ||
					(i == pDocChoixSupport->GrdMap_XRepetition)) continue;
			}

			// On ne prend plus l'indice cumulativit� pour le moment (nb insertions invariant � 1)
			if (i==pDocChoixSupport->GrdMap_Cumulativite) continue;

			m_LstGrandOrd.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandOrd.SetItemData(IndexList,i); 

			// Incr�mente indice liste
			IndexList ++; 
		}
		*/

		CString Txt;
		int IndexList = 0;
		for (pDocChoixSupport->m_MapAbrGrandMap.MoveFirst(); 
			 pDocChoixSupport->m_MapAbrGrandMap.IsValid(); 
			 pDocChoixSupport->m_MapAbrGrandMap.MoveNext())
		{
			// R�cup Id Grandeur
			JUnt32 IdGrandeur = pDocChoixSupport->m_MapAbrGrandMap.GetKey();

			// Ajoute libell� grandeur
			Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, IdGrandeur));

			// Pas de GRP + ni de reprise en moteur proba
			if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
				if ((IdGrandeur == pDocChoixSupport->GrdMap_XGRP)			|| 
					(IdGrandeur == pDocChoixSupport->GrdMap_XRepetition)) continue;

			// On ne prend plus l'indice cumulativit� pour le moment (nb insertions invariant � 1)
			if (IdGrandeur == pDocChoixSupport->GrdMap_Cumulativite) continue;

			// Ajoute dans la liste des grandeurs ordonn�e
			m_LstGrandOrd.AddString(Txt);

			// Ajoute ident grandeur
			m_LstGrandOrd.SetItemData(IndexList,IdGrandeur); 

			// Incr�mente indice liste
			IndexList ++; 
		}
	}

	// Puis on s�lectionne par d�faut la 1ere grandeur
	if (m_LstGrandOrd.GetCount() > 0  && pDocChoixSupport->m_IdGrandMapSelOrd <= 0)
	{
		// Ident associ� � l'�lmt de la liste en position 0
		JUnt32 IdGrandeur = m_LstGrandOrd.GetItemData(0);

		// S�lection dans liste
		pDocChoixSupport->m_IdGrandMapSelOrd = IdGrandeur;
		m_LstGrandOrd.SetCurSel(0);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour liste grandeurs abscisse et ordonn�e
void CDlgPupitreMap::MajGrandeurs()
{
	// Indique calcul � refaire
	pDocChoixSupport->m_ParamMapModif = true;

	// Mise � jour des grandeurs
	MajGrandeurAbs();
	MajGrandeurOrd();
	UpdateSelGrandeurs();

	// Mise � jour des diff�rentes fen�tres 
	pDlgMapping->UpdateResultMapping(false);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour liste grandeurs abscisse
void CDlgPupitreMap::MajGrandeurAbs()
{
	// Reset de la liste des grandeurs pour abscisse
	m_LstGrandAbs.ResetContent();

	// Nb grandeurs de la liste � remplir
	m_NbGrandAbs = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleMap);

	// Impossible de continuer si aucune grandeur s�lectionn�e
	if (m_NbGrandAbs == 0) return;

	// Chargement des grandeurs d�finies
	/*
	CString Txt;
	int IndexList = 0;
	for (JUnt32 i=0; i<m_NbGrandAbs; i++)
	{
		// Ajoute libell� grandeur
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, i));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((i == pDocChoixSupport->GrdMap_XGRP)	   || 
				(i == pDocChoixSupport->GrdMap_XRepetition)) 
			{
				// Regarde si grandeur non pr�c�demment s�lectionn�
				if (pDocChoixSupport->m_IdGrandMapSelAbs == i)
				{
					pDocChoixSupport->m_IdGrandMapSelAbs = -1;
				}
				continue;
			}
		}

		// Indice cumulativit� non utilis� pour le moment
		if (i==pDocChoixSupport->GrdMap_Cumulativite) continue;

		m_LstGrandAbs.AddString(Txt);

		// Ajoute ident grandeur
		m_LstGrandAbs.SetItemData(IndexList,i); 

		if (pDocChoixSupport->m_IdGrandMapSelAbs == i)
		{
			m_LstGrandAbs.SetCurSel(IndexList);
		}

		// Incr�mente indice liste
		IndexList ++; 
	}
	*/

	CString Txt;
	int IndexList = 0;
	for (pDocChoixSupport->m_MapAbrGrandMap.MoveFirst(); 
		 pDocChoixSupport->m_MapAbrGrandMap.IsValid(); 
		 pDocChoixSupport->m_MapAbrGrandMap.MoveNext())
	{
		// R�cup Id Grandeur
		JUnt32 IdGrandeur = pDocChoixSupport->m_MapAbrGrandMap.GetKey();

		// Ajoute libell� grandeur
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, IdGrandeur));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((IdGrandeur == pDocChoixSupport->GrdMap_XGRP)	   || 
				(IdGrandeur == pDocChoixSupport->GrdMap_XRepetition)) 
			{
				// Regarde si grandeur non pr�c�demment s�lectionn�
				if (pDocChoixSupport->m_IdGrandMapSelAbs == IdGrandeur)
				{
					pDocChoixSupport->m_IdGrandMapSelAbs = -1;
				}
				continue;
			}
		}

		// Indice cumulativit� non utilis� pour le moment
		if (IdGrandeur == pDocChoixSupport->GrdMap_Cumulativite) continue;

		m_LstGrandAbs.AddString(Txt);

		// Ajoute ident grandeur
		m_LstGrandAbs.SetItemData(IndexList,IdGrandeur); 

		if (pDocChoixSupport->m_IdGrandMapSelAbs == IdGrandeur)
		{
			m_LstGrandAbs.SetCurSel(IndexList);
		}

		// Incr�mente indice liste
		IndexList ++; 
	}

	// si aucun indicateur s�lectionn�
	if (pDocChoixSupport->m_IdGrandMapSelAbs == -1)
	{
		m_LstGrandAbs.SetCurSel(0);
		pDocChoixSupport->m_IdGrandMapSelAbs = 0;
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour liste grandeurs ordonnee
void CDlgPupitreMap::MajGrandeurOrd()
{
	// Reset de la liste des grandeurs pour abscisse
	m_LstGrandOrd.ResetContent();

	// Nb grandeurs de la liste � remplir
	m_NbGrandOrd = pDocChoixSupport->GetNbGrandeurs(pDocChoixSupport->ModuleMap);

	// Impossible de continuer si aucune grandeur s�lectionn�e
	if (m_NbGrandOrd == 0) return;

	// Chargement des grandeurs d�finies
	/*
	CString Txt;
	int IndexList = 0;
	for (JUnt32 i=0; i<m_NbGrandOrd; i++)
	{
		// Ajoute libell� grandeur
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, i));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((i == pDocChoixSupport->GrdMap_XGRP)	   || 
				(i == pDocChoixSupport->GrdMap_XRepetition)) 
			{
				// Regarde si grandeur non pr�c�demment s�lectionn�
				if (i == pDocChoixSupport->m_IdGrandMapSelOrd)
				{
					pDocChoixSupport->m_IdGrandMapSelOrd = -1;
				}
				continue;
			}
		}

		// Indice cumulativit� non utilis� pour le moment
		if (i==pDocChoixSupport->GrdMap_Cumulativite) continue;

		m_LstGrandOrd.AddString(Txt);

		// Ajoute ident grandeur
		m_LstGrandOrd.SetItemData(IndexList,i); 

		if (pDocChoixSupport->m_IdGrandMapSelOrd == i)
		{
			m_LstGrandOrd.SetCurSel(IndexList);
		}

		// Incr�mente indice liste
		IndexList ++; 
	}
	*/

	CString Txt;
	int IndexList = 0;
	for (pDocChoixSupport->m_MapAbrGrandMap.MoveFirst(); 
		 pDocChoixSupport->m_MapAbrGrandMap.IsValid(); 
		 pDocChoixSupport->m_MapAbrGrandMap.MoveNext())
	{
		// R�cup Id Grandeur
		JUnt32 IdGrandeur = pDocChoixSupport->m_MapAbrGrandMap.GetKey();

		// Ajoute libell� grandeur
		Txt.Format("%s",pDocChoixSupport->GetGrandeur(pDocChoixSupport->ModuleMap, IdGrandeur));

		if (pDocChoixSupport->m_MoteurPresse.GetType() ==  CMoteurPresse::PROBA)
		{
			if ((IdGrandeur == pDocChoixSupport->GrdMap_XGRP)	   || 
				(IdGrandeur == pDocChoixSupport->GrdMap_XRepetition)) 
			{
				// Regarde si grandeur non pr�c�demment s�lectionn�
				if (IdGrandeur == pDocChoixSupport->m_IdGrandMapSelOrd)
				{
					pDocChoixSupport->m_IdGrandMapSelOrd = -1;
				}
				continue;
			}
		}

		// Indice cumulativit� non utilis� pour le moment
		if (IdGrandeur == pDocChoixSupport->GrdMap_Cumulativite) continue;

		m_LstGrandOrd.AddString(Txt);

		// Ajoute ident grandeur
		m_LstGrandOrd.SetItemData(IndexList, IdGrandeur); 

		if (pDocChoixSupport->m_IdGrandMapSelOrd == IdGrandeur)
		{
			m_LstGrandOrd.SetCurSel(IndexList);
		}

		// Incr�mente indice liste
		IndexList ++; 
	}


	// si aucun indicateur s�lectionn�
	if (pDocChoixSupport->m_IdGrandMapSelOrd == -1)
	{
		m_LstGrandOrd.SetCurSel(0);
		pDocChoixSupport->m_IdGrandMapSelOrd = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Initialisation de la liste
void CDlgPupitreMap::InitLstGrandeurs()
{
	m_LstGrandAbs.ResetContent(); 
	m_LstGrandOrd.ResetContent(); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Change grandeur � afficher en abscisse
void CDlgPupitreMap::OnLbnSelChangeListGrandAbs()
{
	// Modif s�lection grandeurs
	pDocChoixSupport->m_IdGrandMapSelAbs = -1;
	for (JUnt32 i = 0; i < m_LstGrandAbs.GetCount(); i++)
	{
		if (m_LstGrandAbs.GetSel(i) > 0)
		{
			// R�cup�re identifiant type grandeur
			JUnt32 IdGrandeur = m_LstGrandAbs.GetItemData(i);
			pDocChoixSupport->m_IdGrandMapSelAbs = m_LstGrandAbs.GetItemData(i); 
			break;
		}
	}

	// Mise � jour des diff�rentes fen�tres 
	pDlgMapping->UpdateResultMapping(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Change grandeur � afficher en ordonn�e
void CDlgPupitreMap::OnLbnSelChangeListGrandOrd()
{
	// Modif s�lection grandeurs
	pDocChoixSupport->m_IdGrandMapSelOrd = -1;
	for (JUnt32 i = 0; i < m_LstGrandOrd.GetCount(); i++)
	{
		if (m_LstGrandOrd.GetSel(i) > 0)
		{
			// R�cup�re identifiant type grandeur
			JUnt32 IdGrandeur = m_LstGrandOrd.GetItemData(i);
			pDocChoixSupport->m_IdGrandMapSelOrd = m_LstGrandOrd.GetItemData(i); 
			break;
		}
	}

	// Mise � jour des diff�rentes fen�tres 
	pDlgMapping->UpdateResultMapping(false);	
}


