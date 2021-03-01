// DialogOffre2.cpp : fichier d'implémentation
//

#include "stdafx.h"

// includes pour les données
#include "IElem.h"
#include <AllTypeOfNode.h>
#include <ItemsConstruits.h>
#include "JSRCTables.h"
#include "OffreTerrainManager.h"
#include "JATPOffre.h"

// include pour contrôles/dialogues
#include "Offre2MainDialog.h"
#include "Offre2Cibles.h"
#include "Offre2Periode.h"
#include "Offre2Formats.h"
#include "Offre2OngletCtrl.h"
#include "DlgSupportsInexistants.h"

// divers
#include "JFCConfig.h"
#include "JFCException.h"

// headers stl
#include <string>

using namespace std;

// on enregistre le message indiquant un changement de source et/ou de terrains
UINT WMU_SOURCE_ELEMENTS_CHANGE = ::RegisterWindowMessage("ATELIER_PRESSE_SOURCE_TERRAINS_CHANGE");

// Boîte de dialogue COffre2MainDialog
IMPLEMENT_DYNAMIC(COffre2MainDialog, CDialog)

//=============================================================================	
// Gestionnaires de messages de COffre2MainDialog
//=============================================================================	
BEGIN_MESSAGE_MAP(COffre2MainDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL_OFFRE, &COffre2MainDialog::OnTcnSelchange)
	ON_BN_CLICKED(IDOK,							&COffre2MainDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_INFOS_SEUILS,				&COffre2MainDialog::OnBnClickedInfoSeuils)
END_MESSAGE_MAP()

//=============================================================================	
COffre2MainDialog::COffre2MainDialog(CATPColors::APPCOLOR appColor, CWnd* pParent, bool bPresenceOngletPeriode /*= true*/)
	:	CDialog(COffre2MainDialog::IDD, pParent)
	,	m_pszDlgTitle(0)
	,	m_bDisablePeriode (!bPresenceOngletPeriode)
	,	m_MaxOnglet	(eodoNumOnglets)
	,	m_appColor (appColor)
{
	m_pTabCtrl				= new COffre2OngletCtrl(this);
	m_brush.CreateSolidBrush(CATPColors::GetColorMedium(m_appColor));
	m_bDirtyFlag			= false;

	m_preselcible			= false;
	m_OffreTerrainManager	= COffreTerrainManager::GetInstance();

	// on donne une valeur invalide pour le handle template (tant que la couleur n'est pas définie)
	m_appColorXtemplate		= CXTemplateManager::InvalidHandle;		

	m_bDateDReadOnly		= false;
}

//=============================================================================	
COffre2MainDialog::~COffre2MainDialog()
{
	m_OffreTerrainManager->Unload();
	if (m_pTabCtrl)	
		delete m_pTabCtrl;
	m_pTabCtrl = 0;

	if (m_pszDlgTitle)
		delete m_pszDlgTitle;
	m_pszDlgTitle = 0;
}

//=============================================================================	
void COffre2MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCTRL_OFFRE,*m_pTabCtrl);
	DDX_Control(pDX, IDOK,				m_btnOK);
}

//=============================================================================	
BOOL COffre2MainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// définition du titre
	if (m_pszDlgTitle)
		SetWindowText(m_pszDlgTitle);

	// on l'ajoute l'onglet 'Période' si nécessaire
	if (!m_bDisablePeriode)
		m_pTabCtrl->AddTab(eodoPeriode,		"Période");
	m_pTabCtrl->AddTab(eodoTerrainsSupports,"Supports");
	m_pTabCtrl->AddTab(eodoCibles,			"Cibles");
	m_pTabCtrl->AddTab(eodoFormats,			"Formats");

	// onglet periode	
	////////////////////////

	if (!m_bDisablePeriode)
	{
		COffre2Periode * pTabPeriode = static_cast<COffre2Periode*>(m_pTabCtrl->GetTab(eodoPeriode));
		ASSERT(pTabPeriode);

		// on définit les dates dans l'onglet 'Période'
		if (m_dStart.IsValid() && m_dEnd.IsValid())
		{
			pTabPeriode->SetDateD(m_dStart);
			pTabPeriode->SetDateF(m_dEnd);
		}
		pTabPeriode->SetDateDReadOnly(m_bDateDReadOnly);
	}

	m_pTabCtrl->InitDialogs();
	m_pTabCtrl->ActivateTabDialogs();

	if (m_StartupMode == eodoCibles)
		// on force le chargement des terrains si on affiche directement l'onglet 'Cibles'
		static_cast<COffre2Cibles*>(m_pTabCtrl->GetTab(eodoCibles))->NotifySelectionChange(true);

	///////////////////////////////
	// on crée les pages à onglets

	// config des onglets

	// onglet format
	if (m_SelFormatsFromOffre.empty())
		// on a pas de selection de formats défini, on met par défaut 'Page Quadri'
		m_SelFormatsFromOffre.push_back(4);

	// on donne à l'onglet sa sélection par defaut
	static_cast<COffre2Formats*>(m_pTabCtrl->GetTab(eodoFormats))->SetDefaultSelection(m_SelFormatsFromOffre);

	EnableOK();

	return TRUE;
}

//=============================================================================	
void COffre2MainDialog::SetTitle(const char * pszDlgTitle)
{
	if (pszDlgTitle && strlen(pszDlgTitle) > 0)
	{
		const size_t size_ = (strlen(pszDlgTitle) > 511) ? 512 : strlen(pszDlgTitle) + 1;
		m_pszDlgTitle = new char[size_];
		m_pszDlgTitle[size_ - 1] = '\0';
		strncpy(m_pszDlgTitle, pszDlgTitle, strlen(pszDlgTitle));
	}
}

//=============================================================================	
void COffre2MainDialog::SetPeriode(const JDate & dStart, const JDate & dEnd)
{
	m_dStart	= dStart;
	m_dEnd		= dEnd;
}

//=============================================================================	
void COffre2MainDialog::SetStartupMode(/*CATPColors::APPCOLOR appColor , */eOngletsDlgOffre ongletIdx /*= eodoTerrainsSupports*/)
{
	m_StartupMode = ongletIdx;
	m_pTabCtrl->SetStartTab(ongletIdx);

	CXTemplateManager* mgr = CXTemplateManager::GetInstance();

	if (mgr)
	{
		const COLORREF & blk	=	CATPColors::GetColorBlack();
		const COLORREF & white	=	CATPColors::GetColorWhite();
		const COLORREF & selcol = CATPColors::GetColorSelect(m_appColor);

		// on crée le template (style) pour les listbox
		// m_appColorXtemplate = mgr->CreateTemplate(blk, white, white, white);
		m_appColorXtemplate = mgr->CreateTemplate(blk, white, white, selcol);

		// on crée le template (style) pour les listbox
		/*
		m_appColorXtemplate = CXTemplateManager::GetInstance()->CreateTemplate(CATPColors::GetColorBlack(), CATPColors::GetColorWhite(),
			CATPColors::GetColorWhite(), CATPColors::GetColorSelect(m_appColor));
		*/
	}	
}

//=============================================================================	
void COffre2MainDialog::SetOffre(const JATPOffre& offre)
{
	// on remplit la sélection en fonction des elements de l'offre
	////////////////////////////

	// copie des elements de sources
	////////////////////////////

	m_SupportsSelection.IdSource			= offre.m_IdSourceAudience;
	m_SupportsSelection.IdTerrainCiblage	= offre.m_IdTerrainCiblage;
	m_SupportsSelection.IdTypePoids			= offre.m_IdTypePoids;

	// copie des segments
	m_SupportsSelection.SelectedSegments.clear();
	offre.m_TBLIdSegmentsAudience.MoveFirst();
	while (offre.m_TBLIdSegmentsAudience.IsValid())
	{
		m_SupportsSelection.SelectedSegments.push_back(offre.m_TBLIdSegmentsAudience.GetItem());
		offre.m_TBLIdSegmentsAudience.MoveNext();
	}

	// copie des supports (titres)
	m_SupportsSelection.SelectedSupports.clear();
	offre.m_TBLIdSupports.MoveFirst();
	while (offre.m_TBLIdSupports.IsValid())
	{
		m_SupportsSelection.SelectedSupports.push_back(offre.m_TBLIdSupports.GetItem());
		offre.m_TBLIdSupports.MoveNext();
	}

	// copie des supports (familles)
	offre.m_MapSelFamilles.MoveFirst();
	while (offre.m_MapSelFamilles.IsValid())
	{
		m_SupportsSelection.SelectedSupports.push_back(offre.m_MapSelFamilles.GetKey());
		offre.m_MapSelFamilles.MoveNext();
	}

	// copie des terrains
	m_SupportsSelection.SelectedTerrains.clear();
	offre.m_TBLIdTerrainsAudience.MoveFirst();
	while (offre.m_TBLIdTerrainsAudience.IsValid())
	{
		m_SupportsSelection.SelectedTerrains.push_back(offre.m_TBLIdTerrainsAudience.GetItem());
		offre.m_TBLIdTerrainsAudience.MoveNext();
	}

	m_SelFormatsFromOffre.clear();
	for (offre.m_TBLIdFormats.MoveFirst(); offre.m_TBLIdFormats.IsValid(); offre.m_TBLIdFormats.MoveNext())
		m_SelFormatsFromOffre.push_back(offre.m_TBLIdFormats.GetItem());

	// copie des elements de cibles
	////////////////////////////

	m_CiblesSelection.Reset();

	// Préselection de l'item ensemble si aucune cible dans l'offre actuelle
	if (offre.m_TBLIdItems.GetCount() == 0)
	{
		const JInt32 IdEnsemble = CItemsConstruits::GetInstance()->GetDefaultItemId();
		const IItem * pItemEnsemble = CItemsConstruits::GetInstance()->GetIItemByID(IdEnsemble);
		ASSERT(pItemEnsemble);
		m_CiblesSelection.SelectedTargets.push_back(pItemEnsemble);
	}
	else
	{
		std::list<JUnt32> listIdTargets;
		std::list<const IItem*>::const_iterator item_it = m_CiblesSelection.SelectedTargets.begin();
		while(item_it != m_CiblesSelection.SelectedTargets.end())
		{
			listIdTargets.push_back((*item_it)->GetID());			
			++item_it;
		}

		// on copie toutes les cibles
		offre.m_TBLIdItems.MoveFirst();
		while(offre.m_TBLIdItems.IsValid())
		{
			const IItem * pItem = CItemsConstruits::GetInstance()->GetIItemByID(offre.m_TBLIdItems.GetItem());
			if (pItem)
				m_CiblesSelection.SelectedTargets.push_back(pItem);
			offre.m_TBLIdItems.MoveNext();
		}
	}

	// assignation d'Ensemble comme reference affinité si il n'y en a pas dans l'offre acctuelle
	if (offre.m_IdItemAffinite == 0)
		m_CiblesSelection.ReferenceAffinite = CItemsConstruits::GetInstance()->GetDefaultItemId();
	else
		m_CiblesSelection.ReferenceAffinite = offre.m_IdItemAffinite;

	// on précharge les terrains s'il n'existent pas
	m_OffreTerrainManager->Preload(offre);
}

//=============================================================================	
void COffre2MainDialog::GetOffre(JATPOffre& offre)
{
	// on récupère dans chaque onglet la partie de l'offre lui correspondant
	//////////////

	// onglets supports-terrains
	m_pTabCtrl->GetTab(eodoTerrainsSupports)->FillOffre(&offre);

	// onglets formats
	m_pTabCtrl->GetTab(eodoFormats)->FillOffre(&offre);

	// onglets cibles
	m_pTabCtrl->GetTab(eodoCibles)->FillOffre(&offre);
}

//=============================================================================	
void COffre2MainDialog::EnableOK()
{
	// itération sur les onglet existants et valides
	COffre2OngletBase * pOnglet = 0;
	m_pTabCtrl->GoFirstTab();
	while(m_pTabCtrl->IsTabValid())
	{
		pOnglet = m_pTabCtrl->GetTab();
		ASSERT(pOnglet);

		if (pOnglet->IsSelectionOk() == false)
		{
			m_btnOK.EnableWindow(FALSE);
			return;
		}

		m_pTabCtrl->GoNextTab();
	}

	m_btnOK.EnableWindow();
}

//=============================================================================	
void COffre2MainDialog::OnBnClickedOk()
{
	// itération sur les onglet existants et valides
	COffre2OngletBase * pOnglet = 0;
	m_pTabCtrl->GoFirstTab();
	while(m_pTabCtrl->IsTabValid())
	{
		pOnglet = m_pTabCtrl->GetTab();
		ASSERT(pOnglet);
		m_pTabCtrl->GoNextTab();
	}

	// on réalise les clotures et récuperation nécessaires
	// onglet période
	if (!m_bDisablePeriode)
	{
		COffre2Periode * pTabPeriode = static_cast<COffre2Periode*>(m_pTabCtrl->GetTab(eodoPeriode));
		ASSERT(pTabPeriode);
		if (pTabPeriode)
		{
			m_dStart = pTabPeriode->GetNewDateD();
			m_dEnd = pTabPeriode->GetNewDateF();
		}
		else
		{
			// on génère une exception
			throw JInvalidCall::GetInstance();
		}	
	}

	OnOK();
}

//=============================================================================	
void COffre2MainDialog::OnBnClickedInfoSeuils()
//=============================================================================	
{
	// on récupère la source courante
	const JSource * pSource = JSRCTables::GetInstance().m_TBLSources.GetItem(m_SupportsSelection.IdSource);
	ASSERT(pSource);

	// Init du message
	CString sTxtSeuil;

	const JTerrain * pTerrain = 0;

	// Boucle sur les terrains
	std::list<JUnt32>::const_iterator terid_it = m_SupportsSelection.SelectedTerrains.begin();				
	while(terid_it != m_SupportsSelection.SelectedTerrains.end())
	{
		// terrain courant
		pTerrain = JSRCTables::GetInstance().m_TBLTerrains.GetItem(m_SupportsSelection.IdSource, *terid_it);

		// Et on crée la chaine "source + terrain" et les seuils
		sTxtSeuil.AppendFormat("%s %s\n\t - Seuil rédhibitoire = %d ; Seuil d'avertissement = %d \n",
			pSource->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr(),
			pTerrain->m_SeuilRedhibitoire, pTerrain->m_SeuilAvertissement);

		++terid_it;
	}	

	// Affichage du message
	MessageBox(sTxtSeuil, "INFORMATION SEUILS ENQUETES", MB_ICONINFORMATION | MB_OK);
}

//=============================================================================	
void COffre2MainDialog::NotifySourceElementsChange(const int eltChangeMask)
//=============================================================================	
{
	WPARAM elementMask = static_cast<WPARAM>(eltChangeMask);

	// on envoie à l'onglet Cibles le message afin qu'il sache qu'il aura à recharger les seuils
	// des cibles/palettes la prochaine fois qu'il aura à les afficher
	m_pTabCtrl->GetTab(eodoCibles)->SendMessage(WMU_SOURCE_ELEMENTS_CHANGE, elementMask, 0);

	// on envoie le message à l'onglet Supports pour qu'il recharge les titres appariés
	m_pTabCtrl->GetTab(eodoTerrainsSupports)->SendMessage(WMU_SOURCE_ELEMENTS_CHANGE, elementMask, 0);
}

//=============================================================================	
// callback appelé lorsqu'on change de page
void COffre2MainDialog::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// on indique un changement de page à onglet au tabctrl
	m_pTabCtrl->ActivateTabDialogs();

	// on récupère l'onglet sélectionné
	COffre2OngletBase * pSelOnglet = m_pTabCtrl->GetTabFromIndex(m_pTabCtrl->GetCurSel());
	
	// itération sur les onglet existants et valides
	COffre2OngletBase * pOnglet = 0;
	m_pTabCtrl->GoFirstTab();
	while(m_pTabCtrl->IsTabValid())
	{
		pOnglet = m_pTabCtrl->GetTab();
		ASSERT(pOnglet);

		// on notifie les onglets de la sélection/déselection
		pOnglet->NotifySelectionChange(pOnglet == pSelOnglet);

		m_pTabCtrl->GoNextTab();
	}

	*pResult = 0;
}

//=============================================================================	
const JDate & COffre2MainDialog::GetPeriodeDebut() const
//=============================================================================	
{
	return m_dStart;
}

//=============================================================================	
const JDate & COffre2MainDialog::GetPeriodeFin() const
//=============================================================================	
{
	return m_dEnd;
}

//=============================================================================	
bool OpenOffreDialog(	LPCTSTR lpszDlgTitle, CATPColors::APPCOLOR appColor,
						JATPOffre & atpOffre, COffre2MainDialog::eOngletsDlgOffre eStartOnglet,
						JDate & dateDeb, JDate & dateFin,
						bool & bDateChanged, bool & bOffreModified, const bool bDateDebReadOnly,
						CWnd * pParent, const bool bOngletPeriodePresent /*= true*/)
//=============================================================================	
{
	// on vérifie la cohérence de l'index de l'onglet
	if (eStartOnglet == COffre2MainDialog::eodoNumOnglets) 
	{
		// l'index de l'onglet n'est pa valide, en DEBUG onl'indique
		ASSERT(FALSE);

		// en release, on se place sur l'onglet 'supports'
		eStartOnglet = COffre2MainDialog::eodoTerrainsSupports;
	}

	// on annule les indicateurs de modification
	bDateChanged	= false;
	bOffreModified	= false;

	COffre2MainDialog Dlg(appColor, pParent, bOngletPeriodePresent);
	Dlg.SetTitle(lpszDlgTitle);

	// Setup de la boite offre source
	Dlg.SetStartupMode(eStartOnglet);

	// On passe l'offre et les terrains
	Dlg.SetOffre(atpOffre);

	// on empeche la modification de la date de deb ut de periode
	if (bDateDebReadOnly)
		Dlg.SetPeriodeDateDReadOnly();

	// on passe les bornes de la periode
	Dlg.SetPeriode(dateDeb, dateFin);

	// on sauve les dates pour comparaison ultérieure
	const JDate dateDebcopy = dateDeb;
	const JDate dateFincopy = dateFin;

	// on affiche la boîte de dialogue
	if (IDOK == Dlg.DoModal())
	{
		// on récupère la nouvelle offre sélectionnée par l'utilisateur
		Dlg.GetOffre(atpOffre);

		// verification modification de la période
		if (dateDebcopy != Dlg.GetPeriodeDebut() || dateFincopy != Dlg.GetPeriodeFin())
		{
			// on attribue les nouvelles dates bornes de la période
			dateDeb = Dlg.GetPeriodeDebut();
			dateFin = Dlg.GetPeriodeFin();

			bDateChanged = true;
		}

		// on indique une eventuelle modification de l'offre
		bOffreModified = Dlg.IsModified();
		return true;
	}
	return false;
}
