#include "stdafx.h"
#include "Offre2SupportsSource.h"

#include "JATPOffre.h"
#include "JSRCTables.h"
#include "Offre2Supports.h"
#include "Offre2SupportsOngletBase.h"
#include "Offre2MainDialog.h"
#include "DlgSupportsInexistants.h"
#include "AllTypeOfNode.h"
#include "KeyTerrain.h"
#include "OffreTerrainManager.h"

using namespace std;

//=============================================================================	
// Boîte de dialogue Offre2SupportsSource
//=============================================================================	

IMPLEMENT_DYNAMIC(Offre2SupportsSource, COffre2SupportsOngletBase)

//=============================================================================	
// Gestionnaires de messages de Offre2SupportsSource
//=============================================================================	

BEGIN_MESSAGE_MAP(Offre2SupportsSource, COffre2SupportsOngletBase)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CB_SEL_SOURCE,		&Offre2SupportsSource::OnCbnSelchangeCbSelSource)
	ON_LBN_SELCHANGE(IDC_LB_SEL_TERRAINS,	&Offre2SupportsSource::OnLbnSelchangeLbSelTerrains)
END_MESSAGE_MAP()


//=============================================================================	
Offre2SupportsSource::Offre2SupportsSource()
	:	COffre2SupportsOngletBase(IDD)
	,	m_psrcTables (&JSRCTables::GetInstance())
	,	m_FirstUpdate(true)
//=============================================================================	
{
	ASSERT(m_psrcTables);
}

//=============================================================================	
Offre2SupportsSource::~Offre2SupportsSource()
//=============================================================================	
{
}

//=============================================================================	
void Offre2SupportsSource::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	COffre2SupportsOngletBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_SEL_SOURCE, m_SourcesCombo);
	DDX_Control(pDX, IDC_LB_SEL_TERRAINS, m_TerrainsList);
}

//=============================================================================	
BOOL Offre2SupportsSource::OnInitDialog()
//=============================================================================	
{
	COffre2SupportsOngletBase::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//=============================================================================	
void Offre2SupportsSource::OnCbnSelchangeCbSelSource()
//=============================================================================	
{
	int idx = m_SourcesCombo.GetCurSel();
	ASSERT(idx != CB_ERR); ASSERT(idx < static_cast<int>(m_Sources.size()));
	const JSource * Src = m_Sources[idx];

	// check : enquête BUP?
	if(Src->m_IdSource == 35)
	{
		AfxMessageBox(	"Attention : \r\nPour construire ou évaluer des plans mono-famille sur la BUP,\n"
						"Audipresse recommande de revenir aux bandes sources AEPM ou EPIC", MB_OK | MB_ICONINFORMATION);
	}

	CSupportsSelection & NewSelection = m_pDialogOffre->GetSupportsSelection();

	if (m_FirstUpdate)
	{
		m_FirstUpdate = false;
	}
	else
	{
		// on reset la sélection après un changement de source
		NewSelection.Reset();
	}

	NewSelection.IdSource = Src->m_IdSource;

	m_pOngletTitres->GetFilter().idSource = NewSelection.IdSource;

	if (m_pOngletTitres->ValidateSelection(true))
		m_pDialogOffre->SetModified(true);

	// on reconstruit les onglets de sélection
	m_pOngletTitres->ConstructSelectionTabs();

	// on remplit et sélectionne les terrains
	FillTerrains();

	// on indique un changement de terrains/source
	m_pDialogOffre->NotifySourceElementsChange(COffre2MainDialog::SOURCE_CHANGE | COffre2MainDialog::TERRAINS_CHANGE);

	// annulation des filtres péridoicités et titres-couplages-familles
	m_pOngletTitres->GetFilter().periodicites.clear();
	m_pOngletTitres->GetFilter().titre_coupl_famil = 0;

	m_pOngletTitres->OnLbnSelchangeLstFiltres();
	m_pOngletTitres->UpdateSupportsDisplay();

	// on indique au dialogue parent une modif
	m_pDialogOffre->SetModified();

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDialogOffre->EnableOK();
}

//=============================================================================	
void Offre2SupportsSource::FillTerrains()
//=============================================================================	
{
	m_TerrainsList.ResetContent();

	// on boucle sur ts les terrains de la liste
	vector<const JTerrain*>::const_iterator ter_it = m_pOngletTitres->m_Terrains.begin();
	while (ter_it != m_pOngletTitres->m_Terrains.end())
	{
		const JTerrain * pTerrain = *ter_it;

		// on garde uniquement ceux de la source en cours
		if(pTerrain->m_IdSource == m_pDialogOffre->GetSupportsSelection().IdSource)
		{
			// ajout du terrain à la listbox
			int idx = m_TerrainsList.AddLine(pTerrain->m_Libelle.AsJCharPtr(), m_pDialogOffre->GetAppColorTemplate());
			m_TerrainsList.SetItemData(idx, reinterpret_cast<DWORD_PTR>(pTerrain));
		}	
		++ter_it;
	}

	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();

	// on vérifie chaque id terrain de la sélection
	list<JUnt32>::iterator  selid_it = Selection.SelectedTerrains.begin();
	while (selid_it != Selection.SelectedTerrains.end())
	{
		// on boucle sur tous les terrains chargés
		ter_it = m_pOngletTitres->m_Terrains.begin();
		while (ter_it != m_pOngletTitres->m_Terrains.end())
		{
			// on teste avec l'id en cours
			if((*ter_it)->m_IdTerrain == *selid_it)
				break;
			++ter_it;
		}

		// on a pas trouvé l'id courant
		if (ter_it == m_pOngletTitres->m_Terrains.end())
		{
			// on l'efface et on passe au suivant
			selid_it = Selection.SelectedTerrains.erase(selid_it);
			m_pDialogOffre->SetModified();			
		}
		else ++selid_it;
	}

	// on sélectionne les terrains
	const int count = m_TerrainsList.GetCount();
	for (int idx = 0; idx < count; ++idx)
	{
		if (find(Selection.SelectedTerrains.begin(), Selection.SelectedTerrains.end(),
			(reinterpret_cast<const JTerrain*>(m_TerrainsList.GetItemData(idx)))->m_IdTerrain) != Selection.SelectedTerrains.end())
			m_TerrainsList.SetSel(idx);
	}

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDialogOffre->EnableOK();
}

//=============================================================================	
// pour la gestion des messages pr les cas particuliers dans la sélection des terrains et autre
// return true si continuer sélection
bool Offre2SupportsSource::AffichageMsgCasParticuliersTerrains(const int IdxSelTerrains)
//=============================================================================	
{
	// Init Annee terrain sélection
	JUnt32 DernAnneeTerrain  = 0;
	JBool  MessageConsoJuniorOk = false;

	const JTerrain * pTerrain = reinterpret_cast<const JTerrain*>(m_TerrainsList.GetItemData(IdxSelTerrains));

	// Instance des tables
	const JSRCTables  & srcTables = JSRCTables::GetInstance();

	// instances statiques des indicateurs de msg pr les cas particuliers
	static bool sbOkWarningEnqueteAEPM2006	= false;
	static bool sbOkWarningEnqueteFCA2007	= false;

	static CString TxtMess; TxtMess.Empty();

	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();

	////////////////////////////////////////////////////////////////////////////////////////
	//								Message Spécial AEPM 2006
	// Attention provisoire message prévention autre méthodoligie de calcul
	// pour Aepm Janvier-Décembre 2006  (pas très propre mais comment faire avec !!!!)
	if (Selection.IdSource == 1 && pTerrain->m_IdTerrain == 251 && !sbOkWarningEnqueteAEPM2006)
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
	if (Selection.IdSource == 3 && pTerrain->m_IdTerrain == 272 && !sbOkWarningEnqueteFCA2007)
	{
		TxtMess.LoadString(IDS_FD_MESS_FCA_2007); 
		sbOkWarningEnqueteFCA2007 = true;
	}

	if (!TxtMess.IsEmpty()) AfxMessageBox(TxtMess,MB_OK|MB_ICONINFORMATION);

	return true;
}

//=============================================================================	
bool Offre2SupportsSource::CheckOneSupport (JUnt32 IdSupport, const list<JUnt32> & lAddedTerrains, CString &txt)
//=============================================================================	
{
	const CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();

	CKeyTerrain key;
	key.SetSource(Selection.IdSource);

	//on teste le support pour chaque terrain, si invalide on sort de la boucle
	IElem * elem = CRegroupements::GetInstance()->GetIElemByID(IdSupport);

	if (elem != NULL)
	{
		std::list<JUnt32>::const_iterator ter_id = lAddedTerrains.begin();
		while (ter_id != lAddedTerrains.end())
		{
			key.SetTerrain(*ter_id);
			CTerrain * terrain = m_pDialogOffre->GetOffreTerrainManager()->GetTerrain(key);

			if (!terrain->HasSupport( elem->AsITitre()->GetIDAudience()) )
			{
				txt.Format("%s", elem->GetLabel().AsJCharPtr());
				return false;
			}
			++ter_id;
		}
	}

	return true;
}

//=============================================================================	
// TODO : remplacer par des for_each les while avec les iterateurs dans cette fonction
void Offre2SupportsSource::CheckAllSupports(std::list<JUnt32> & lAddedTerrains) 
//=============================================================================	
{
	static list<JUnt32> lLastTerrainsSel;

	// si on vient d'ajouter un terrain à la sélection
	if (lAddedTerrains.empty() == false)
	{
		CString sName;
		std::map<JUnt32, CString> mapSupports;

		const CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();

		list<JUnt32>::const_iterator it_idtitre = lLastTerrainsSel.begin();
		while(it_idtitre != lLastTerrainsSel.end())
		{
			if (CheckOneSupport(*it_idtitre, lAddedTerrains, sName) == false)
			{
				// on ajoute à la map tous les couples 'id support'/'nom support'
				mapSupports.insert(std::make_pair(*it_idtitre, sName));
			}
			++it_idtitre;
		}

		// on a retiré au moins 1 item
		if (mapSupports.empty() == false)
		{
			// on cherche le nom de la source
			const JSource * ptSrc = JSRCTables::GetInstance().m_TBLSources.GetItem(Selection.IdSource);

			// Et on crée la chaine "source + terrain"
			CString sSrcTerrains;
			std::list<JUnt32>::iterator ter_it = lAddedTerrains.begin();
			while (ter_it != lAddedTerrains.end())
			{
				if (ter_it != lAddedTerrains.begin()) sSrcTerrains.Append(", ");

				// on cherche le nom du terrain
				const JTerrain * ptTer	= JSRCTables::GetInstance().m_TBLTerrains.GetItem(Selection.IdSource, *ter_it);
				sSrcTerrains.AppendFormat("%s - %s", ptSrc->m_Libelle.AsJCharPtr() , ptTer->m_Libelle.AsJCharPtr());
				++ter_it;
			}

			// on crée la chaine contenant tous les supports
			CString sSupports;
			std::map<JUnt32, CString>::iterator map_it = mapSupports.begin();
			while (map_it != mapSupports.end())
			{
				sSupports.AppendFormat("%s\r\n", map_it->second);
				++map_it;
			}

			CString sUserMsg;
			if (lAddedTerrains.size() == 1)
				sUserMsg.Format("Les supports suivants n'existent pas dans le dernier terrain sélectionné (%s):\r\n", sSrcTerrains);
			else
				sUserMsg.Format("Les supports suivants n'existent pas dans les derniers terrains sélectionnés (%s):\r\n", sSrcTerrains);

			DlgSupportsInexistants dlg(sUserMsg, sSupports);
			dlg.DoModal();
		}
	}
}

//=============================================================================	
void Offre2SupportsSource::OnLbnSelchangeLbSelTerrains()
//=============================================================================	
{
	// on copie la sélection actuelle du filtre (avant le click)
	static list<JUnt32> lastTerrainSelection;

	vector<int> IdxSelection;

	// on reset les terrains du filtre et de la sélection
	CSupportFilter & filter = m_pOngletTitres->GetFilter();
	filter.terrains.clear();

	// selection non vide
	if (GetControlSelection(IdxSelection, m_TerrainsList))
	{
		// on récupère la table des sources
		const JSRCTables  & srcTables = JSRCTables::GetInstance();

		// Boucle sur les terrains actuellement sélectionnés
		vector<int>::iterator idx_it = IdxSelection.begin();
		while(idx_it != IdxSelection.end())
		{
			// traitement et user messages au sujet des cas particuliers terrains
			if (!AffichageMsgCasParticuliersTerrains(*idx_it))
			{
				++idx_it;
				continue;
			}

			// on récupère l'id du terrain sélectionné courant
			JUnt32 IdTerrain = reinterpret_cast<const JTerrain*>(m_TerrainsList.GetItemData(*idx_it))->m_IdTerrain;
			filter.terrains.push_back(IdTerrain);
			++idx_it;
		}

		// on cherche le(s) dernier(s) terrain(s) selectionné(s)
		static list<JUnt32> lNewlySelected;
		lNewlySelected.clear();
		lastTerrainSelection.sort(); filter.terrains.sort();
		std::set_difference(filter.terrains.begin(), filter.terrains.end(), lastTerrainSelection.begin(), lastTerrainSelection.end(), back_inserter(lNewlySelected));

		CheckAllSupports(lNewlySelected);

		// on indique au dialogue parent une modif
		m_pDialogOffre->SetModified();
	}

	// utilisai-t-on les segments avant d'avoir changé le filtre terrains?
	bool bSegmentsUsedBefore = filter.GetUseSegments();

	// mise à jour de la sélection es terrains
	std::list<JUnt32> & SelectedTerrains = m_pDialogOffre->GetSupportsSelection().SelectedTerrains;
	SelectedTerrains.clear();
	copy(filter.terrains.begin(), filter.terrains.end(), back_inserter(SelectedTerrains));

	// on met à jour le terrain de ciblage
	m_pDialogOffre->GetSupportsSelection().SetTerrainCiblage();

	if (m_pOngletTitres->ValidateSelection())
		m_pDialogOffre->SetModified(true);

	m_pOngletTitres->ConstructSelectionTabs();

	// on synchronise selection et filtre de terrains
	lastTerrainSelection.clear();
	copy(filter.terrains.begin(), filter.terrains.end(), back_inserter(lastTerrainSelection));

	// on indique un changement de terrains/source
	m_pDialogOffre->NotifySourceElementsChange(COffre2MainDialog::TERRAINS_CHANGE);

	// modifie la liste des supports
	m_pOngletTitres->UpdateSupportsDisplay();

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDialogOffre->EnableOK();
}

//=============================================================================	
HBRUSH Offre2SupportsSource::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//=============================================================================	
{
	switch (nCtlColor)
	{
		// on paint le dialogue lui même
	case CTLCOLOR_DLG:
		return m_pDialogOffre->GetBackgroundBrush();

	case CTLCOLOR_BTN:
	case CTLCOLOR_STATIC:
		// on 'paint' tout le reste en transparent
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
		break;		
		// pour l'editbox du combobox
	case CTLCOLOR_EDIT:
		switch(pWnd->GetDlgCtrlID())
		{
		case IDC_CB_SEL_SOURCE:
			// on modifie le pinceau et la couleur de fond
			pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
			return ::GetSysColorBrush(COLOR_WINDOW);
		}
		break;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

//=============================================================================	
// retourne true si l'état de la sélection est correct
bool Offre2SupportsSource::IsSelectionOk() const
//=============================================================================	
{
	return (!m_pDialogOffre->GetSupportsSelection().SelectedTerrains.empty());
}

//=============================================================================	
// remplit la partie de l'offre correspondant à l'onglet
void Offre2SupportsSource::FillOffre(JATPOffre * pOffre)
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();
	const CSupportFilter & filter = m_pOngletTitres->GetFilter();

	// on modifie la source si nécessaire
	if (pOffre->m_IdSourceAudience != filter.idSource)
	{
		pOffre->m_IdSourceAudience = filter.idSource;
		pOffre->m_Changes |= pOffre->SRC_AUDIENCE;
	}

	// on modifie les terrains
	set<JUnt32> lastTerrains;
	for (pOffre->m_TBLIdTerrainsAudience.MoveFirst();pOffre->m_TBLIdTerrainsAudience.IsValid();pOffre->m_TBLIdTerrainsAudience.MoveNext())
		lastTerrains.insert(pOffre->m_TBLIdTerrainsAudience.GetItem());

	// on s'assure de l'unicité de la selection de terrains, et qu'elle est triée
	Selection.SelectedTerrains.sort();Selection.SelectedTerrains.unique();

	// on compare les terrains précedemment sélectionnés et la sélection actuelle
	if ((Selection.IdTerrainCiblage != pOffre->m_IdTerrainCiblage)		// terrains de ciblage différents
		|| (Selection.SelectedTerrains.size() != lastTerrains.size())	// ou pas le même nbr de terrains
		|| (!equal(lastTerrains.begin(), lastTerrains.end(),			// ou terrains sélectionnés sont différents
				Selection.SelectedTerrains.begin()))
		)	
	{
		pOffre->m_TBLIdTerrainsAudience.Reset();
		pOffre->m_IdTerrainCiblage = Selection.IdTerrainCiblage;

		// il y a des différences, on recopie la sélection de terrains dans l'offre
		list<JUnt32>::const_iterator ter_it = Selection.SelectedTerrains.begin();
		while (ter_it != Selection.SelectedTerrains.end())
		{
			JUnt32 IdTerrain = *ter_it;
			pOffre->m_TBLIdTerrainsAudience.AddTail() = IdTerrain;
			++ter_it;
		}
		pOffre->m_Changes |= (pOffre->TER_AUDIENCE | pOffre->TER_CIBLAGE);
	}
}

//=============================================================================	
void Offre2SupportsSource::FillSources()
//=============================================================================	
{
	// on remplit la combobox des sources
	m_SourcesCombo.ResetContent();

	// on boucle sur toutes les sources
	m_psrcTables->m_TBLSources.MoveFirst();
	while (m_psrcTables->m_TBLSources.IsValid())
	{
		// on ajoute le libellé comme elt du combo et la source comme data
		const JSource & Src =  m_psrcTables->m_TBLSources.GetItem();
		int It = m_SourcesCombo.AddString(Src.m_Libelle.AsJCharPtr());
		m_SourcesCombo.SetItemData(It, reinterpret_cast<DWORD_PTR>(&Src));
		m_psrcTables->m_TBLSources.MoveNext();
	}

	const JUnt32 SelSourceId = m_pDialogOffre->GetSupportsSelection().IdSource;
	int selidx = CB_ERR;

	// on boucle sur les elts du control
	const int nmax = m_SourcesCombo.GetCount();
	for (int idx = 0; idx < nmax; ++idx)
	{
		const JSource * pSource = reinterpret_cast<const JSource*>(m_SourcesCombo.GetItemData(idx));
		if (pSource->m_IdSource == SelSourceId)
			selidx = idx;
		m_Sources.push_back(pSource);
	}

	if (selidx == CB_ERR)
	{
		ASSERT(!m_Sources.empty());
		selidx = 0;
		m_pDialogOffre->GetSupportsSelection().IdSource = m_Sources[0]->m_IdSource;
	}
	m_SourcesCombo.SetCurSel(selidx);

	// on provoque le rechargement des terrains
	OnCbnSelchangeCbSelSource();
}

//=============================================================================	
// appelé pour la mise à jour
void Offre2SupportsSource::UpdateContent()
//=============================================================================	
{
}
