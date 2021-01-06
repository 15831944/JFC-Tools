// DlgOffre2_Titres.cpp : fichier d'implémentation
//

#include "stdafx.h"

// include pour contrôles/dialogues
#include "OngletOffre2_Titres.h"
#include "DlgOffre2.h"

// includes pour les données
#include "JSRCTables.h"
#include "JSRCPresse.h"

// includes C++ STL
#include <algorithm>

using namespace std;

// Boîte de dialogue DlgOffre2_Titres

IMPLEMENT_DYNAMIC(COngletOffre2_Titres, COngletOffre2)

// Gestionnaires de messages de COngletOffre2_Titres

BEGIN_MESSAGE_MAP(COngletOffre2_Titres, COngletOffre2)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CBB_SOURCES,			&COngletOffre2_Titres::OnComboSourceSelChange)
	ON_LBN_SELCHANGE(IDC_LST_FILTRES,			&COngletOffre2_Titres::OnFilterFolderSelChange)
	ON_LBN_SELCHANGE(IDC_LST_ELT_FILTRES_MULTI,	&COngletOffre2_Titres::OnFilterElementSelChange)
	ON_LBN_SELCHANGE(IDC_LST_ELT_FILTRES_SINGLE,&COngletOffre2_Titres::OnFilterElementSelChange)
	ON_BN_CLICKED	(IDC_CK_SUPPORTS_SOURCE,	&COngletOffre2_Titres::OnChkboxSupportsSourceClicked)
	ON_BN_CLICKED	(IDC_CK_TITRES,				&COngletOffre2_Titres::UpdateCheckboxesState)
	ON_BN_CLICKED	(IDC_CK_COUPLAGES,			&COngletOffre2_Titres::UpdateCheckboxesState)
	ON_EN_CHANGE	(IDC_TITLE_SEARCH_STR,		&COngletOffre2_Titres::OnEnChangeTitleSearchStr)
END_MESSAGE_MAP()

// echange dynamique de données avec les contrôles du dialogue
void COngletOffre2_Titres::DoDataExchange(CDataExchange* pDX)
{
	COngletOffre2::DoDataExchange(pDX);
	DDX_Control	(pDX, IDC_CBB_SOURCES,				m_SourceCombo);
	DDX_Control	(pDX, IDC_LST_FILTRES,				m_FilterFoldersList);
	DDX_Control	(pDX, IDC_LST_ELT_FILTRES_MULTI,	m_FilterElementsList.Multi);
	DDX_Control	(pDX, IDC_LST_ELT_FILTRES_SINGLE,	m_FilterElementsList.Single);
	DDX_Control	(pDX, IDC_LST_TITRES,				m_TitresList);
	DDX_Control	(pDX, IDC_CK_SUPPORTS_SOURCE,		m_SupportsSourceChkbox);
	DDX_Control	(pDX, IDC_CK_TITRES,				m_TitresChkbox);
	DDX_Control	(pDX, IDC_CK_COUPLAGES,				m_CouplagesChkbox);
	DDX_Text	(pDX, IDC_TITLE_SEARCH_STR,			m_sSearchString);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Implémentation privée											//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// a evidemment laisser dans cet order (libellés des categories de filtres)
static const char * ppLibellesFolders[] = {"Terrains", "Segments", "Types de Poids", "Périodicités", '\0'}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//							 Implémentation de la classe COngletOffre2_Titres								//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

COngletOffre2_Titres::COngletOffre2_Titres()
	:	COngletOffre2(IDD)
	,	m_sSearchString(_T(""))
{
	// Init tables des sources
	m_srcTables = (JSRCTables*)& JSRCTables::GetInstance();
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

	// par défaut le folder est le folder 'terrain'
	m_eFolderIdx = effTerrains;

	// par défaut on affiche que les titres et pas les couplages
	m_eTitresCouplagesDisplay = m_eLastTitresCouplagesDisplay = CSupportFilter::etcdOnlyTitres;

	m_bSupportsDeLaSource = true;
}

COngletOffre2_Titres::~COngletOffre2_Titres()
{
}


void LectureEtCreationFiltreCourant(CSupportList & supportList)
{
	// TODO : pour l'instant en TEST mais faire ca ailleurs
	CSupportFilter & filter = supportList.GetDisplayFilter();
	filter.idSource = 27;
	filter.terrains.push_back(175);
	filter.terrains.push_back(13);
	filter.periodicites.push_back(12);
	filter.periodicites.push_back(14);
	filter.segments.push_back(14);
	filter.segments.push_back(16);
	filter.typePoids = 14;
	filter.titre_couplage = CSupportFilter::etcdOnlyTitres;
	filter.supportsFromSource = true;
	filter.strSearch.Empty();
	// TODO : fin
}

BOOL COngletOffre2_Titres::OnInitDialog()
{
	COngletOffre2::OnInitDialog();

	m_AppColor = m_pDlgOffre->GetAppColor();

	// todo  : function temporaire en attendant lecture differente de la préselection depuis fichier
	LectureEtCreationFiltreCourant(m_TitresList);

	// initialisation des listes de filtrages et de la liste de selection
	InitLists();

	// initialisation de l'etat des checkboxes
	m_SupportsSourceChkbox.SetCheck(m_bSupportsDeLaSource?TRUE:FALSE);
	m_CouplagesChkbox.SetCheck(m_eTitresCouplagesDisplay & CSupportFilter::etcdOnlyCouplages);
	m_TitresChkbox.SetCheck(m_eTitresCouplagesDisplay & CSupportFilter::etcdOnlyTitres);

	return TRUE;
}

HBRUSH COngletOffre2_Titres::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
		// on paint le dialogue lui même
		case CTLCOLOR_DLG:
			return m_pDlgOffre->GetBackgroundBrush();
		default:
			break;
	}

	// sinon on paint comme d'haibute
	return (CDialog::OnCtlColor(pDC, pWnd, nCtlColor));
}

void COngletOffre2_Titres::OnComboSourceSelChange()
{
	// on récupère le nouvel id source
	int idx = m_SourceCombo.GetCurSel(); ASSERT(idx != CB_ERR);
	JUnt32 IdSource = ((JUnt32)m_SourceCombo.GetItemData(idx));

	// check : enquête BUP?
	if(IdSource == 35)
	{
		AfxMessageBox(	"Attention : \r\nPour construire ou évaluer des plans mono-famille sur la BUP,\n"
						"Audipresse recommande de revenir aux bandes sources AEPM ou EPIC",MB_OK|MB_ICONINFORMATION);
	}

	// on modifie la source dans le filtre
	m_TitresList.GetDisplayFilter().idSource = IdSource;
	m_TitresList.GetDisplayFilter().SetLastModif(CSupportFilter::efm_SOURCE);

	// on force la remise à jour de la liste des folders de filtres
	OnFilterFolderSelChange();
}

void COngletOffre2_Titres::OnFilterFolderSelChange()
{
	// on modifie la sélection
	m_eFolderIdx = static_cast<eFilterFolder>(m_FilterFoldersList.GetCurSel());

	if (m_eFolderIdx == CB_ERR) m_FilterFoldersList.SetCurSel(0);

	// on peuple la liste des elements filtres
	PopulateFilterElements();
}

void COngletOffre2_Titres::OnFilterElementSelChange()
{
	// on check la categorie de filtre ayant été modifiée
	switch (m_eFolderIdx)
	{
		//////////////////////
		// catégorie : Terrains
		case effTerrains:

		// on récupère le(s) terrain(s) sélectionné(s), on met à jour le filtre
		{
			CWaitCursor Wait;
			static int lastAloneTerrainIdx = LB_ERR;

			// on oblige au moins 1 terrain sélectionné
			if (m_FilterElementsList.Multi.GetSelCount() == 0)
			{
				// on resélectionne le terrain précedemment sélectionné	
				ASSERT(lastAloneTerrainIdx != LB_ERR);
				m_FilterElementsList.Multi.SetSel(lastAloneTerrainIdx);
			}
			// on récupère le filtre, les tables
			CSupportFilter & Filter = m_TitresList.GetDisplayFilter();
			const JSRCTables  & srcTables	= JSRCTables::GetInstance();

			// on copie la sélection actuelle du filtre (avant le click)
			std::list<JUnt32> lBeforeChangeSel;
			std::copy(Filter.terrains.begin(), Filter.terrains.end(), std::back_inserter(lBeforeChangeSel));

			// on reset les terrains du filtre
			Filter.terrains.clear();
			Filter.SetLastModif(CSupportFilter::efm_TERRAINS);

			// Boucle sur les terrains actuellement sélectionnés
			const int nMax = m_FilterElementsList.Multi.GetSelCount();

			int * arraySel = new int[nMax]; ASSERT(arraySel != 0);
			m_FilterElementsList.Multi.GetSelItems(nMax, arraySel); 
			Filter.terrains.clear();
			for (int arrIdx = 0; arrIdx < nMax; ++arrIdx)
			{
				// traitement et user messages au sujet des cas particuliers terrains
				if (!AffichageMsgCasParticuliersTerrains(arraySel[arrIdx]))
					continue;

				// on ajoute le nouveau terrain au filtre
				JUnt32 IdTerrain = (JUnt32)m_FilterElementsList.Multi.GetItemData(arraySel[arrIdx]);
				Filter.terrains.push_back(IdTerrain);
			}
			if (arraySel) delete[] arraySel; arraySel = 0;

			// on cherche le(s) dernier(s) terrain(s) selectionné(s)
			std::list<JUnt32> lNewlySelected;
			lBeforeChangeSel.sort(); Filter.terrains.sort();
			std::set_difference(lBeforeChangeSel.begin(), lBeforeChangeSel.end(),Filter.terrains.begin(), Filter.terrains.end(), back_inserter(lNewlySelected));


			// TODO : voir si cet ancien code a tjs lieu d'être ICI
			//FillListSegment();
			//OnLbnSelchangeListSegment();
			//FillListPoids();
			//OnLbnSelchangeListPoids();
			//m_pDlgOffre->EnableOK();
			//m_pDlgOffre->RazSeuils();
			//m_pDlgOffre->CheckItems();

			// TODO : reimplementer cette methode ici, et en utilisant la liste des supports du filtre 
			// TODO : pour vérifier les supports nouvellement sélectionnés, et pas comme c'est aujourd'hui
			// TODO : en checkant avec les supports de l'offre, car l'offre n'est pa mise à jour
			m_pDlgOffre->CheckAllSupports(lNewlySelected);

			// si 1 seul terrain sélectionné, on sauve son idx
			if (m_FilterElementsList.Multi.GetSelCount() == 1)
			{
				m_FilterElementsList.Multi.GetSelItems(1, &lastAloneTerrainIdx); 
				ASSERT(lastAloneTerrainIdx >= 0);
				ASSERT(lastAloneTerrainIdx < m_FilterElementsList.Multi.GetCount());
			}

			// modifie le cache de la liste des supports
			m_TitresList.LoadAuxiliarySupports();

		}break;

		//////////////////////
		case effSegments:

		// on récupère le(les) segment(s) sélectionné(s) et on met à jour l'offre et le filtre
		{
			static int lastAloneSegmentIdx = LB_ERR;

			// on oblige au moins 1 segment sélectionné
			if (m_FilterElementsList.Multi.GetSelCount() < 1)
			{
				// on doit forcément avoir un terrain précedemment sélectionné	
				ASSERT(lastAloneSegmentIdx != LB_ERR);
				m_FilterElementsList.Multi.SetSel(lastAloneSegmentIdx);
			}

			m_pDlgOffre->SetModified();
			m_pDlgOffre->GetOffre().m_Changes |= m_pDlgOffre->GetOffre().SEG_AUDIENCE;
			m_pDlgOffre->GetOffre().m_Changes |= m_pDlgOffre->GetOffre().SEG_CIBLAGE;

			// retenir la sel
			m_pDlgOffre->GetOffre().m_TBLIdSegmentsAudience.Reset();
			m_pDlgOffre->GetOffre().m_TBLIdSegmentsCiblage.Reset();

			// on met à jour le filtre
			CSupportFilter & Filter = m_TitresList.GetDisplayFilter();
			Filter.segments.clear();
			Filter.SetLastModif(CSupportFilter::efm_SEGMENTS);

			for (int i = 0; i < m_FilterElementsList.Multi.GetCount() ; i++)
			{
				// si le segment est sélectionné
				if (m_FilterElementsList.Multi.GetSel(i))
				{
					JUnt32 IDSegment = static_cast<JUnt32>(m_FilterElementsList.Multi.GetItemData(i));
					m_pDlgOffre->GetOffre().m_TBLIdSegmentsAudience.AddTail() = IDSegment;
					m_pDlgOffre->GetOffre().m_TBLIdSegmentsCiblage.AddTail()  = IDSegment;
					m_pDlgOffre->EnableOK();

					// on met à jour le filtre
					Filter.segments.push_back(IDSegment);
				}
			}

			// Reste à traiter la déselection de tous les segments, sinon diode reste verte
			if (m_FilterElementsList.Multi.GetSelCount() == 0)
			{
				m_pDlgOffre->EnableOK();
			}

			// TODO : décommenter et voir utilité d RazSeuils
			//m_pDlgOffre->RazSeuils();

			// si 1 seul segment sélectionné, on récupère son idx
			if (m_FilterElementsList.Multi.GetSelCount() == 1)
				for (int k=0;k < m_FilterElementsList.Multi.GetCount();k++)
					if (m_FilterElementsList.Multi.GetSel(k))
					{lastAloneSegmentIdx = k; break;}
		}break;

		//////////////////////
		case effTypesDePoids:

		// on récupère le type de poids sélectionné et on met à jour l'offre et le filtre
		{
			// on oblige 1 type de poids sélectionné
			static int lastAlonePdsIdx = LB_ERR;		

			int sel = m_FilterElementsList.Single.GetCurSel();
			if (sel != LB_ERR)
			{
				// on met à jour le filtre
				CSupportFilter & Filter = m_TitresList.GetDisplayFilter();

				JUnt32 poids = static_cast<JUnt32>(m_FilterElementsList.Single.GetItemData(sel));
				if (poids != m_pDlgOffre->GetOffre().m_IdTypePoids)
				{
					m_pDlgOffre->SetModified();
					m_pDlgOffre->GetOffre().m_Changes |= m_pDlgOffre->GetOffre().TYP_POIDS;
					m_pDlgOffre->GetOffre().m_IdTypePoids = poids;
					m_pDlgOffre->EnableOK();

					// on met à jour le filtre
					Filter.typePoids = poids;
					Filter.SetLastModif(CSupportFilter::efm_TYPE_POIDS);
				}
			}
			else
			{
				// on doit forcément avoir un type de poids sélectionné	
				ASSERT(lastAlonePdsIdx != LB_ERR);
				m_FilterElementsList.Single.SetCurSel(lastAlonePdsIdx);

				m_pDlgOffre->GetOffre().m_IdTypePoids = sel = 0;
				m_FilterElementsList.Single.SetCurSel(0);					
			}
			// TODO : décommenter et voir utilité de RazSeuils
			//m_pDlgOffre->RazSeuils();

			// on sauve l'idx courant
			lastAlonePdsIdx = sel;
		}break;

		//////////////////////
		case effPeriodicite:

		// on récupère la(les) péridiocité(s) sélectionnée(s) et on met à jour l'offre et le filtre
		{
			static int lastAlonePeriodIdx = LB_ERR;

			// on oblige au moins 1 pérdiocité sélectionné
			if (m_FilterElementsList.Multi.GetSelCount() < 1)
			{
				// on doit forcément avoir un terrain précedemment sélectionné	
				ASSERT(lastAlonePeriodIdx != LB_ERR);
				m_FilterElementsList.Multi.SetSel(lastAlonePeriodIdx);
			}

			// on met à jour le filtre
			CSupportFilter & Filter = m_TitresList.GetDisplayFilter();
			Filter.periodicites.clear();
			Filter.SetLastModif(CSupportFilter::efm_PERIODICITES);

			for (int i = 0; i < m_FilterElementsList.Multi.GetCount() ; i++)
			{
				// si la périoicité est sélectionné
				if (m_FilterElementsList.Multi.GetSel(i))
				{
					const IElem * pElem = reinterpret_cast<IElem*>(m_FilterElementsList.Multi.GetItemData(i));

					// on met à jour le filtre
					Filter.periodicites.push_back(pElem->GetID());
				}
			}

			// si 1 seul segment sélectionné, on récupère son idx
			if (m_FilterElementsList.Multi.GetSelCount() == 1)
				for (int k=0;k < m_FilterElementsList.Multi.GetCount();k++)
					if (m_FilterElementsList.Multi.GetSel(k))
					{
						lastAlonePeriodIdx = k;
						break;
					}
		}break;
	}

	// on met à jour la liste de supports car un filtre a été modifié
	m_TitresList.UpdateDisplay();
}

// pour la gestion des messages pr les cas particuliers dans la sélection des terrains et autre
// return true si continuer sélection
bool COngletOffre2_Titres::AffichageMsgCasParticuliersTerrains(const int IdxListTerrains)
{
	// Init Annee terrain sélection
	JUnt32 DernAnneeTerrain  = 0;
	JBool  MessageConsoJuniorOk = false;
	JUnt32 IdTerrain = (JUnt32)m_FilterElementsList.Multi.GetItemData(IdxListTerrains);

	CSupportFilter & filter = m_TitresList.GetDisplayFilter();

	// Instance des tables
	const JSRCTables  & srcTables	= JSRCTables::GetInstance();

	// instances statiques des indicateurs de msg pr les cas particuliers
	static bool sbOkWarningEnqueteAEPM2006	= false;
	static bool sbOkWarningEnqueteFCA2007	= false;

	static CString TxtMess; TxtMess.Empty();

	// On choisit le terrain le + recent comme terrain cible
	//
	// Attention, dans le cas d'enquete avec segments de population (doivent être identiques de terrain en terrain)
	// -->> voir problème ConsoJunior 2008 (4 segments) et avant (3 segments)
	//
	if (IdTerrain > m_pDlgOffre->GetOffre().m_IdTerrainCiblage)
	{
		// Positionne le terrain de ciblage
		m_pDlgOffre->GetOffre().m_IdTerrainCiblage = IdTerrain;
		const JTerrain * pTerrain = srcTables.m_TBLTerrains.GetItem(filter.idSource , IdTerrain);

		// Ajoute libellé dans liste		 
		DernAnneeTerrain  = atoi(pTerrain->m_Libelle.AsJCharPtr());
	}

	///////////////////////////////////////////////// Spécial CONSO-JUNIOR /////////////////////////////////////////////////
	// Attention au problème ConsoJunior (Grosse bidouille en attendant une politique de gestion terrain plus efficace !!!!)
	const JTerrain * pTerrain		= srcTables.m_TBLTerrains.GetItem(filter.idSource , IdTerrain);
	if (filter.idSource == 7)
	{
		if (DernAnneeTerrain >= 2008 && atoi(pTerrain->m_Libelle.AsJCharPtr()) < 2008)
		{
			// Ici les nouveaux terrains consojunior ne sont pas compatibles avec le 1er terrain conso junior >= 2008
			m_FilterElementsList.Multi.SetSel(IdxListTerrains, false);

			// Message de prévention indiquant la non compatibilité des terrains Conso-Junior
			if  (!MessageConsoJuniorOk)
				AfxMessageBox("Attention pour Conso-Junior le mode de segmentation a changé (à partir de 2008, 4 segments au lieu de 3), \nce qui rend incompatible l'utilisation de certains terrains entre eux", MB_ICONINFORMATION);

			// Message conso junior effectué
			MessageConsoJuniorOk = true;
			return false;
		}
		else if (DernAnneeTerrain < 2008 && atoi(pTerrain->m_Libelle.AsJCharPtr()) >= 2008 && !MessageConsoJuniorOk)
		{
			// Ici les nouveaux terrains consojunior ne sont pas compatibles avec le 1er terrain conso junior < 2008
			m_FilterElementsList.Multi.SetSel(IdxListTerrains, false);

			// Message de prévention indiquant la non compatibilité des terrains Conso-Junior
			if  (!MessageConsoJuniorOk)
				AfxMessageBox("Attention pour Conso-Junior le mode de segmentation a changé (à partir de 2008, 4 segments au lieu de 3), \n ce qui rend incompatible l'utilisation de certains terrains entre eux", MB_ICONINFORMATION);

			// Message conso junior effectué
			MessageConsoJuniorOk = true;
			return false;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//								Message Spécial AEPM 2006
	// Attention provisoire message prévention autre méthodoligie de calcul
	// pour Aepm Janvier-Décembre 2006  (pas très propre mais comment faire avec !!!!)
	if (filter.idSource == 1 && IdTerrain == 251 && !sbOkWarningEnqueteAEPM2006)
	{
		TxtMess.LoadString(IDS_FD_MESS_AEPM_JANV_DEC_2006); 
		sbOkWarningEnqueteAEPM2006 = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//								Message Spécial FCA 2007
	// Attention provisoire message prévention autre méthodoligie de calcul
	// pour France Cadres Actifs 2007
	// Rappel : les résultats de la France des Cadres Actifs 2007 ne doivent pas être comparés à ceux de l’édition 2006.
	// Cette décision est liée au redressement de l’enquête qui pour la première fois depuis 2003, a pu être actualisé au niveau le plus fin.
	if (filter.idSource == 3 && IdTerrain == 272 && !sbOkWarningEnqueteFCA2007)
	{
		TxtMess.LoadString(IDS_FD_MESS_FCA_2007); 
		sbOkWarningEnqueteFCA2007 = true;
	}

	if (!TxtMess.IsEmpty()) AfxMessageBox(TxtMess,MB_OK|MB_ICONINFORMATION);

	return true;
}

// methodes pour initialiser/reinitialiser les listes
void COngletOffre2_Titres::InitLists()
{
	m_TitresList.SetCallingApp(m_AppColor);

	//////////////////////////////////////////
	//		remplit le combo de sources		//
	//////////////////////////////////////////
	m_SourceCombo.ResetContent();

	CSupportFilter & filter = m_TitresList.GetDisplayFilter();

	// on boucle sur toutes les sources
	m_srcTables->m_TBLSources.MoveFirst();
	while (m_srcTables->m_TBLSources.IsValid())
	{
		// on ajoute le libellé comme elt du combo et l'id source comme data
		const JSource & Src =  m_srcTables->m_TBLSources.GetItem();
		int It = m_SourceCombo.AddString(Src.m_Libelle.AsJCharPtr());
		m_SourceCombo.SetItemData(It, Src.m_IdSource);
		// on resélectionne la source
		if (filter.idSource == Src.m_IdSource)
			m_SourceCombo.SetCurSel(It);

		m_srcTables->m_TBLSources.MoveNext();
	}
	
	// on valide la source sélectionnée
	if (m_SourceCombo.GetCurSel() == CB_ERR) m_SourceCombo.SetCurSel(0);
	filter.idSource = static_cast<JUnt32>(m_SourceCombo.GetItemData(m_SourceCombo.GetCurSel()));

	/////////////////////////////////////////////////
	// remplit la liste des 'catégories' de filtre //
	/////////////////////////////////////////////////
	m_FilterFoldersList.ResetContent();	
	int idx = CB_ERR;

	// on parcourt les libelles des categories de filtres
	const char ** ppLibelle = ppLibellesFolders;
	while(*ppLibelle)
	{
		// on ajoute le libellé, et on définit sa couleur de fond
		idx = m_FilterFoldersList.AddLine(CXListBox::Black,  CXListBox::White, *ppLibelle);
		ASSERT(idx != CB_ERR);
		ppLibelle++;
	}
	ASSERT(m_FilterFoldersList.GetCount() == effNumFolders);

	// on sélectionne le folder terrains pour le 1er affichage de l'onglet
	m_FilterFoldersList.SetCurSel(effTerrains);

	// on remplit avec les terrains
	PopulateWithTerrains();

	// on force la mise à jour
	OnFilterElementSelChange();




	// on crée la liste complète des supports dans notre liste de sélection
	m_TitresList.LoadStableSupports();

	////on selectionne la source
	//OnComboSourceSelChange();






	// TODO : NE PAS FAIRE MENTIR CE COMMENTAIRE : CE DOIT ETRE VRAI!!!!
	// Todo : de toute manière il va falloir ajouter un système (à préciser lequel)
	//		: pour sauvegarder les précedentes selections pour qu'elles deviennent les valeurs par défaut
	//		: du prochaine chargement d'AteliePresse/ de la prochaine création d'offre.
	// comme désormais une sélection est forcément faite

	//on charge les terrains
	int sel = m_SourceCombo.GetCurSel();
	ASSERT (sel != CB_ERR);
	
	//FillListTerrain( (JUnt32)m_comboSource.GetItemData(sel) );

	//OnLbnSelchangeListTerrain();
}

///////////////////////////////////////////
// remplit la liste des elements à filtrer

void COngletOffre2_Titres::PopulateFilterElements()
{
	switch (m_eFolderIdx)
	{
		// multi select
		case effTerrains:
			m_FilterElementsList.Multi.ResetContent();
			PopulateWithTerrains();
			m_FilterElementsList.ShowMulti();
			break;
		// multi select
		case effSegments:
			m_FilterElementsList.Multi.ResetContent();
			PopulateWithSegments();
			m_FilterElementsList.ShowMulti();
			break;
		// single select
		case effTypesDePoids:
			m_FilterElementsList.Single.ResetContent();
			PopulateWithTypesDePoids();
			m_FilterElementsList.ShowSingle();
			break;
		// multi select
		case effPeriodicite:
			m_FilterElementsList.Multi.ResetContent();
			PopulateWithPeriodicites();
			m_FilterElementsList.ShowMulti();
			break;
		default:
			// a priori, on devrait pas se trouver ici!!
			ASSERT(false);
	}

	if (m_FilterElementsList.IsMulti())
	{
		if (m_FilterElementsList.Multi.GetSelCount() == 0)
			// on préselectionne le premier filtre si rien de selectionné
			m_FilterElementsList.Multi.SetSel(0);
		else if (m_FilterElementsList.Multi.GetCount() == 1)
			// 1 seul filtre : on le selectionne
			m_FilterElementsList.Multi.SetSel(0);

		// on active la sélection en fonction du nbr de filtres possible
		m_FilterElementsList.Multi.EnableWindow((m_FilterElementsList.Multi.GetCount() > 1)?TRUE:FALSE);
	}
	else
	{
		if (m_FilterElementsList.Single.GetCurSel() == LB_ERR)
			// on préselectionne le premier filtre si rien de selectionné
			m_FilterElementsList.Single.SetCurSel(0);
		else if (m_FilterElementsList.Single.GetCount() == 1)
			// 1 seul filtre : on le selectionne
			m_FilterElementsList.Single.SetCurSel(0);

		// on active la sélection en fonction du nbr de filtres possible
		m_FilterElementsList.Single.EnableWindow((m_FilterElementsList.Single.GetCount() > 1)?TRUE:FALSE);
	}
}

void COngletOffre2_Titres::PopulateWithTerrains()
{
	CSupportFilter & filter = m_TitresList.GetDisplayFilter();
	for(m_srcTables->m_TBLTerrains.MoveFirst(); m_srcTables->m_TBLTerrains.IsValid(); m_srcTables->m_TBLTerrains.MoveNext())
	{
		const JTerrain & Terr = m_srcTables->m_TBLTerrains.GetItem();
		// on affiche uniquement les terrains de la source
		if( Terr.m_IdSource == filter.idSource)
		{
			int It = m_FilterElementsList.Multi.AddString(Terr.m_Libelle.AsJCharPtr());
			m_FilterElementsList.Multi.SetItemData(It, Terr.m_IdTerrain );

			list<JUnt32>::iterator ter_it = filter.terrains.begin();
			while(ter_it != filter.terrains.end())
			{
				if (*ter_it == Terr.m_IdTerrain)
				{
					m_FilterElementsList.Multi.SetSel(It);
					break;
				}
				++ter_it;
			}
		}
	}
	
	// on force la validation à un element si il n'y en a pas
	const int nMax = m_FilterElementsList.Multi.GetSelCount();
	if (nMax == 0) m_FilterElementsList.Multi.SetSel(0);

	// on valide la liste des terrains du filtre avec les terrains réellement ajoutés
	int * arraySel = new int[nMax];
	m_FilterElementsList.Multi.GetSelItems(nMax, arraySel); 
	filter.terrains.clear();
	for (int arrIdx = 0; arrIdx < nMax; ++arrIdx)
		filter.terrains.push_back(static_cast<JUnt32>(m_FilterElementsList.Multi.GetItemData(arraySel[arrIdx])));
	if (arraySel) delete arraySel; arraySel = 0;
}

void COngletOffre2_Titres::PopulateWithSegments()
{
	// on remplit avec les segments du terrain de ciblage
	CTerrain * pterrain = NULL;
	//JATPOffre & Offre = m_pDlgOffre->GetOffre();
	CSupportFilter & filter = m_TitresList.GetDisplayFilter();
	CKeyTerrain k(filter.idSource, m_pDlgOffre->GetOffre().m_IdTerrainCiblage);

	if ((pterrain = m_pDlgOffre->GetOffreTerrainManager()->GetTerrain(k)) == NULL)
		return;

	JVector<CSegment*> vect_segment;
	if( pterrain->GetSegments(vect_segment))
	{
		// Update the segment list
		for(vect_segment.MoveFirst(); vect_segment.IsValid(); vect_segment.MoveNext())
		{
			const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(vect_segment.GetItem()->GetID());

			#ifdef _DEBUG
				ASSERT(Seg);
				CString LibSegment = Seg->m_Libelle.AsJCharPtr();
			#endif

			// on ajoute le Libellé du segment (avecson id en list-data)
			int idx = m_FilterElementsList.Multi.AddString(Seg->m_Libelle.AsJCharPtr());
			m_FilterElementsList.Multi.SetItemData(idx, Seg->m_IdSegment);

			// selectionne les segments contenus dans le filtre
			if (std::find(filter.segments.begin(), filter.segments.end(), Seg->m_IdSegment) != filter.segments.end())
					m_FilterElementsList.Multi.SetSel(idx,true);
		}
	}

	// verifier qu'on a au moins 1 segment
	const int nMax = m_FilterElementsList.Multi.GetSelCount();
	if ( nMax == 0) m_FilterElementsList.Multi.SetSel(0);

	// TODO : valider la sélection : 1 seul segment sélectionné MAXIMUM
	// TODO : valider la sélection : 1 seul segment sélectionné MAXIMUM
	// TODO : valider la sélection : 1 seul segment sélectionné MAXIMUM
	// TODO : si l'user vient de déselectionner le seul segment, le remettre!
	// TODO : si l'user vient de déselectionner le seul segment, le remettre!
	// TODO : si l'user vient de déselectionner le seul segment, le remettre!


	// on valide la liste des terrains du filtre avec les terrains réellement ajoutés
	int * arraySel = new int[nMax];
	m_FilterElementsList.Multi.GetSelItems(nMax, arraySel); 
	filter.terrains.clear();
	for (int arrIdx = 0; arrIdx < nMax; ++arrIdx)
		filter.terrains.push_back(static_cast<JUnt32>(m_FilterElementsList.Multi.GetItemData(arraySel[arrIdx])));
	if (arraySel) delete arraySel; arraySel = 0;
}

void COngletOffre2_Titres::PopulateWithTypesDePoids()
{
	// on remplit avec les poids du terrain de ciblage
	CTerrain * pterrain = NULL;
	JATPOffre & Offre = m_pDlgOffre->GetOffre();
	CKeyTerrain k(Offre.m_IdSourceCiblage, Offre.m_IdTerrainCiblage);
	pterrain = m_pDlgOffre->GetOffreTerrainManager()->GetTerrain(k);

	if (pterrain == NULL)
		return;

	JVector<JInt32> vect;
	pterrain->GetTypesPoids(vect);
	// Update the poids list
	for(vect.MoveFirst(); vect.IsValid(); vect.MoveNext())
	{
		int idx = m_FilterElementsList.Single.AddString( m_srcTables->m_TBLPoids.GetLabelByID(vect.GetItem()) );
		m_FilterElementsList.Single.SetItemData(idx, vect.GetItem());
		// Set the selected poids
		if (Offre.m_IdTypePoids == vect.GetItem())
			m_FilterElementsList.Single.SetCurSel(idx);
	}
}

void COngletOffre2_Titres::PopulateWithPeriodicites()
{
	for (	m_srcPresse->m_TBLPeriodicites.MoveFirst();
			m_srcPresse->m_TBLPeriodicites.IsValid();
			m_srcPresse->m_TBLPeriodicites.MoveNext() )
	{
		const IElem * pElem = m_srcPresse->m_TBLPeriodicites.GetItem();
#ifdef _DEBUG
		const JChar* szLabel = pElem->GetLabel().AsJCharPtr();
#endif
		int idx = m_FilterElementsList.Multi.AddString(pElem->GetLabel().AsJCharPtr());
		m_FilterElementsList.Multi.SetItemData(idx, reinterpret_cast<DWORD_PTR>(pElem));
	}
}


// remplit la liste de titres
void COngletOffre2_Titres::PopulateTitres()
{

/*
										<**> <**> <**> <**> <**> 
											<**> <**> <**> <**> <**> 
												<**> <**> <**> <**> <**> 
													<**> <**> <**> <**> <**> 
<**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> 
<**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**>  CONTINUER ICI
<**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> <**> 
													<**> <**> <**> <**> <**> 
												<**> <**> <**> <**> <**> 
											<**> <**> <**> <**> <**> 
										<**> <**> <**> <**> <**> 
*/
/*	voir CDlgSupport::FillListCouplage() pour affichage des COUPLAGES
	voir CDlgSupport::FillListTitre() pour affichage et récup TITRES

	faire le nécessaire pour définir un code couleur dans les différents types d'items
	ajoutables dans la liste et pour leurs différents états :
	
	<**> sélectionnés/non sélectionnés
	<**> dans source/pas dans la source
	<**> etc.
*/


	// on reset le contenu de la liste

	// on récupère les filtres

	// o crée notre foncteur de filtrage

	// on filtre la liste de tous les titres avec notre foncteur

	// on ajoute les titres sélectionnés

	// on trie le tout

	// on affiche la liste










/*
	m_TitresList.ResetContent();

	// Les titres
	for( m_srcPresse->m_TBLTitres.MoveFirst(); m_srcPresse->m_TBLTitres.IsValid(); m_srcPresse->m_TBLTitres.MoveNext() )
	{
		IElem* elemToInsert = m_srcPresse->m_TBLTitres.GetItem();
		if ( elemToInsert->IsCouplage())
			continue;
		if (TestPeriodicite( elemToInsert->AsITitre() ) &&  CSearchAlgo::IsSearched( elemToInsert->GetLabel().AsJCharPtr() , m_searchStr) )
		{

			//if ( elemToInsert->IsCouplage() && m_btnAfficheAll.GetCheck() == BST_CHECKED && m_lstBas.IsCouplageAttribue(elemToInsert))
			//	continue;

			//if ( !elemToInsert->IsCouplage() && m_btnAfficheAll.GetCheck() == BST_CHECKED && m_lstBas.IsTitreAttribue(elemToInsert->GetID()))
			if ( m_btnAfficheAll.GetCheck() == BST_CHECKED && m_lstBas.IsTitreAttribue(elemToInsert->GetID()))
				continue;

			if (m_pData->m_appColor == CATPColors::CREUSET && elemToInsert->AsITitre()->IsAgregat())
				continue;
			m_TitresList.AddItem( elemToInsert );
		}
	}

	//Les titres apparentés (HS)
	for( m_pRegroupements->GetTitresApparentes()->MoveFirst(); m_pRegroupements->GetTitresApparentes()->IsValid(); m_pRegroupements->GetTitresApparentes()->MoveNext() )
	{
		IElem* elemToInsert = m_pRegroupements->GetTitresApparentes()->GetItem();
		if (TestPeriodicite( elemToInsert->AsITitre() ) && CSearchAlgo::IsSearched( elemToInsert->GetLabel().AsJCharPtr() , m_searchStr) )
		{
			if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
				&& m_lstBas.IsTitreAttribue(elemToInsert->GetID()) )
				continue;
			m_lstBas.AddItem( elemToInsert );
		}
	}

	//Les titres appariés
	//pour chaque terrain
	for (m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst();
		m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid();
		m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext() )
	{
		JUnt32 idSource = m_pData->m_offre.m_IdSourceAudience;
		JUnt32 idTerrain = m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem();
		CTitresApparies * titresapp = m_pRegroupements->GetTitresApparies(idSource, idTerrain);
		if (titresapp)
			for( titresapp->MoveFirst(); titresapp->IsValid(); titresapp->MoveNext() )
			{
				IElem* elemToInsert = titresapp->GetItem();
				if ( TestPeriodicite( elemToInsert->AsITitre() ) && CSearchAlgo::IsSearched( elemToInsert->GetLabel().AsJCharPtr(), m_searchStr ) )
				{
					if ( m_btnAfficheAll.GetCheck() == BST_CHECKED 
						&& m_lstBas.IsTitreAttribue(elemToInsert->GetID()) )
						continue;
					m_lstBas.AddItem( elemToInsert );
				}
			}
	}

	JList<JInt32> selIdlst;
	m_pData->m_lstIDTitre.Swap((JList<JUnt32>&)selIdlst);

	m_lstBas.SetSelectedID(selIdlst);

	m_pData->m_lstIDTitre.Swap((JList<JUnt32>&)selIdlst);
*/
}


// messages de notification de click

void COngletOffre2_Titres::OnChkboxSupportsSourceClicked()
{
	m_bSupportsDeLaSource = (m_SupportsSourceChkbox.GetCheck() == TRUE)?true:false;
	m_TitresList.GetDisplayFilter().supportsFromSource = m_bSupportsDeLaSource;
	m_TitresList.GetDisplayFilter().SetLastModif(CSupportFilter::efm_SUPPORTS_SOURCE);
}

// mise à jour (cohérence de l'état des checkboxes titres-couplages)
void COngletOffre2_Titres::UpdateCheckboxesState()
{
	if (m_TitresChkbox.GetCheck())
	{		
		if (m_CouplagesChkbox.GetCheck())	m_eTitresCouplagesDisplay = CSupportFilter::etcdTitres_plus_Couplages;
		else								m_eTitresCouplagesDisplay = CSupportFilter::etcdOnlyTitres;
	}
	else 
	{
		if (m_CouplagesChkbox.GetCheck())
		{
			m_eTitresCouplagesDisplay = CSupportFilter::etcdOnlyCouplages;
		}
		else
		{
			// l'utilisateur vient de tenter de déselectionner la dernière checkbox (si bien qu'il n'y a plus rien!!...)
			// vite, on l'en empêche en recheckant celle qui vient de dechecker
			switch (m_eLastTitresCouplagesDisplay)
			{
				case CSupportFilter::etcdOnlyCouplages:
					// on rechecke 'Couplages'
					m_CouplagesChkbox.SetCheck(TRUE);
					break;
				case CSupportFilter::etcdOnlyTitres:
					// on rechecke 'Titres'
					m_TitresChkbox.SetCheck(TRUE);
					break;
				default:
					 // ça ne devrait jamais arriver, comment changer 2 checkbox en 1 click avec 1 souris??? bonne chance!!!
					ASSERT(false);
			}
		}
	}

	// on sauvegarde l'etat et on set le filtre
	m_eLastTitresCouplagesDisplay = m_eTitresCouplagesDisplay;
	m_TitresList.GetDisplayFilter().titre_couplage = m_eLastTitresCouplagesDisplay;	
	m_TitresList.GetDisplayFilter().SetLastModif(CSupportFilter::efm_TITRES_COUPLAGES);
}
void COngletOffre2_Titres::OnEnChangeTitleSearchStr()
{
	// TODO:  Ajoutez ici le code de votre gestionnaire de notification de contrôle
}

void COngletOffre2_Titres::ValidateFilterElementsWithCurrentSource()
{
	PopulateWithTerrains();
	PopulateWithSegments();
	PopulateWithTypesDePoids();
	PopulateWithPeriodicites();
}