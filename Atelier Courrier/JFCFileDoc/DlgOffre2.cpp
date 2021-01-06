// DialogOffre2.cpp : fichier d'implémentation
//

#include "stdafx.h"

// includes pour les données
#include "IElem.h"
#include <AllTypeOfNode.h>
#include "JSRCTables.h"
#include "ItemsConstruitsManager.h"
#include "ItemsConstruitsBase.h"

// include pour contrôles/dialogues
#include "DlgOffre2.h"
#include "OffreTabCtrl.h"
#include "DlgSupportsInexistants.h"


using namespace std;

#ifdef _DEBUG
 #define NOTIMPLEMENTED {CString s("Not implemented : ");s.AppendFormat("(%s, %s)", THIS_FILE, __LINE__);AfxMessageBox(s);}
#else
 #define NOTIMPLEMENTED 
#endif

// Boîte de dialogue CDialogOffre2
IMPLEMENT_DYNAMIC(CDialogOffre2, CDialog)

// Gestionnaires de messages de CDialogOffre2
BEGIN_MESSAGE_MAP(CDialogOffre2, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, &CDialogOffre2::OnTcnSelchangeTabctrl)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


CDialogOffre2::CDialogOffre2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOffre2::IDD, pParent)
{
	m_appColor				= CATPColors::CREUSET;
	m_brush.CreateSolidBrush(CATPColors::GetColorLight(m_appColor));
	m_bDirtyFlag			= false;

	m_pCurPage				= 0;
	m_pTabCtrl				= new COffreTabCtrl(this);

	m_preselcible			= false;
	m_OffreTerrainManager	= COffreTerrainManager::GetInstance();
}

CDialogOffre2::~CDialogOffre2()
{
	m_OffreTerrainManager->Unload(m_offre);
}

void CDialogOffre2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCTRL,  *m_pTabCtrl);
	DDX_Control(pDX, IDOK,			m_btnOK);
}

BOOL CDialogOffre2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pTabCtrl->InsertItem(eodoPeriode,	"Périodes");
	m_pTabCtrl->InsertItem(eodoTitres,	"Titres");
	m_pTabCtrl->InsertItem(eodoCibles,	"Cibles");
	m_pTabCtrl->InsertItem(eodoFormats,	"Formats");

	m_pTabCtrl->InitDialogs();

	m_pTabCtrl->ActivateTabDialogs();

	///////////////////////////////
	// on crée les pages à onglets

	// on définit la page active
	m_pCurPage		= m_pTabCtrl->GetActiveTab();

	//switch (m_StartOngletIdx)
	//{
	//	case eodoCibles:
	//		break;
	//	case eodoTitres:
	//		break;
	//	case eodoFormats:
	//		NOTIMPLEMENTED;
	//		break;
	//	case eodoPeriode:
	//		NOTIMPLEMENTED;
	//		break;
	//}

	SetupControls();

	return TRUE;
}

void CDialogOffre2::SetColor()
{
}

void CDialogOffre2::SetupControls()
{
	// subclassdlgitem !!??
	SetColor();
}

void CDialogOffre2::SetStartupMode( eOngletsDlgOffre ongletIdx, CATPColors::APP appColor )
{
	m_pTabCtrl->SetStartTab(ongletIdx); m_appColor = appColor;
}

void CDialogOffre2::OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pTabCtrl->ActivateTabDialogs();
	*pResult = 0;
}

HBRUSH CDialogOffre2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}


void CDialogOffre2::SetOffre(JATPOffre& offre)
{
	m_lstIDTitre.clear();
	m_lstIDCouplage.clear();
	m_lstIDLot.clear();

	m_offre = offre;

	// plus aucune gestion de sources multiples
	ASSERT(m_offre.m_IdSourceCiblage == m_offre.m_IdSourceAudience);

	// Positionne les élmts supports sélectionnés
	for (m_offre.m_TBLIdSupports.MoveFirst(); m_offre.m_TBLIdSupports.IsValid(); m_offre.m_TBLIdSupports.MoveNext() )
	{
		JUnt32 id = m_offre.m_TBLIdSupports.GetItem();
		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem* ielem = pRegroupements->GetIElemByID(id);

		if (ielem == NULL)
		{
			m_offre.m_TBLIdSupports.Remove();
			continue;
		}
		if (ielem->IsCouplage())
		{
			m_lstIDCouplage.push_back(id);
			continue;
		}
		if (ielem->IsLot())
		{
			m_lstIDLot.push_back(id);
			continue;
		}
		m_lstIDTitre.push_back(id);
	}

	// Récupére les élmts familles
	m_offre.m_MapSelFamilles.MoveFirst();
	while(m_offre.m_MapSelFamilles.IsValid())
	{
		// Récup id famille
		JUnt32 id = m_offre.m_MapSelFamilles.GetKey();

		// Vérifie que c'est bien une famille
		CRegroupements * pRegroupements = CRegroupements::GetInstance();
		IElem *pElem = pRegroupements->GetIElemByID(m_offre.m_MapSelFamilles.GetKey());
		if (pElem && pElem->IsLot()) m_lstIDLot.push_back(id);
		m_offre.m_MapSelFamilles.MoveNext(); 
	}

	// Préselection de l'item ensemble
	if (m_offre.m_TBLIdItems.GetCount() == 0)
	{
		// m_offre.m_TBLIdItems.AddTail() = CItemsConstruits::GetInstance()->GetDefaultItemId();
		m_offre.m_TBLIdItems.AddTail() = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetDefaultItemId();
		m_preselcible = true;
	}
	if (m_offre.m_IdItemAffinite == 0)
	{
		// m_offre.m_IdItemAffinite = CItemsConstruits::GetInstance()->GetDefaultItemId();
		m_offre.m_IdItemAffinite = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetDefaultItemId();
		m_preselcible = true;
	}

	//on précharge les terrains s'il n'existent pas
	m_OffreTerrainManager->Preload(m_offre);
}

JATPOffre& CDialogOffre2::GetOffre()
{
	//on recupere tous les différents types de supports
	m_offre.m_TBLIdSupports.Reset();

	// on ajoute tous les id de lots
	list<JUnt32>::iterator id_it = m_lstIDLot.begin();
	while (id_it != m_lstIDLot.end())
	{
		m_offre.m_TBLIdSupports.MoveTo(static_cast<JInt32>(*id_it));
		if (m_offre.m_TBLIdSupports.IsValid()) continue;
		m_offre.m_TBLIdSupports.AddTail() = static_cast<JInt32>(*id_it);
		++id_it;
	}

	// on ajoute tous les id de titres
	id_it = m_lstIDTitre.begin();
	while (id_it != m_lstIDTitre.end())
	{
		m_offre.m_TBLIdSupports.MoveTo(static_cast<JInt32>(*id_it));
		if (m_offre.m_TBLIdSupports.IsValid()) continue;
		m_offre.m_TBLIdSupports.AddTail() = static_cast<JInt32>(*id_it);
		++id_it;
	}

	// on ajoute tous les id de couplages
	id_it = m_lstIDCouplage.begin();
	while (id_it != m_lstIDCouplage.end())
	{
		m_offre.m_TBLIdSupports.MoveTo(static_cast<JInt32>(*id_it));
		if (m_offre.m_TBLIdSupports.IsValid()) continue;
		m_offre.m_TBLIdSupports.AddTail() = static_cast<JInt32>(*id_it);
		++id_it;
	}

	//on construit la table des libellés de supports
	m_offre.m_mapLblSupport.Reset();
	for (m_offre.m_TBLIdSupports.MoveFirst(); m_offre.m_TBLIdSupports.IsValid(); m_offre.m_TBLIdSupports.MoveNext())
	{
		IElem* ielem = CRegroupements::GetInstance()->GetIElemByID(m_offre.m_TBLIdSupports.GetItem());
		m_offre.m_mapLblSupport.Add(ielem->GetID()) = ielem->GetLabel();
	}

	//on construit la table des libellés d'items
	m_offre.m_mapLblItem.Reset();
	for (m_offre.m_TBLIdItems.MoveFirst(); m_offre.m_TBLIdItems.IsValid(); m_offre.m_TBLIdItems.MoveNext())
	{
		// IItem * pItem = CItemsConstruits::GetInstance()->GetIItemByID(m_offre.m_TBLIdItems.GetItem());
		IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_offre.m_TBLIdItems.GetItem());
		m_offre.m_mapLblItem.Add(pItem->GetID()) = pItem->GetLabel();
	}

	//enfin on rend l'offre
	return m_offre;
}

void CDialogOffre2::EnableOK()
{
	for (int onglet = 0; onglet < static_cast<int>(eodoNumOnglets); ++onglet)
	{
		if (m_pTabCtrl->GetTab(static_cast<eOngletsDlgOffre>(onglet))->IsSelectionOk() == false)
		{
			m_btnOK.EnableWindow(FALSE);
			return;
		}
	}
	m_btnOK.EnableWindow();
}


void CDialogOffre2::CheckAllSupports( std::list<JUnt32> & lAddedTerrains ) const 
{
	// si on vient d'ajouter un terrain à la selection
	if (lAddedTerrains.empty() == false)
	{
		CString sName;
		std::map<JUnt32, CString> mapSupports;

		list<JUnt32>::const_iterator it_idtitre = m_lstIDTitre.begin();
		while(it_idtitre != m_lstIDTitre.end())
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
			const JSource * ptSrc	= JSRCTables::GetInstance().m_TBLSources.GetItem(m_offre.m_IdSourceAudience);

			// Et on crée la chaine "source + terrain"
			CString sSrcTerrains;
			std::list<JUnt32>::iterator ter_it = lAddedTerrains.begin();
			while (ter_it != lAddedTerrains.end())
			{
				if (ter_it != lAddedTerrains.begin()) sSrcTerrains.Append(", ");

				// on cherche le nom du terrain
				const JTerrain * ptTer	= JSRCTables::GetInstance().m_TBLTerrains.GetItem(m_offre.m_IdSourceAudience, *ter_it);
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
bool CDialogOffre2::CheckOneSupport( JUnt32 IdSupport, const std::list<JUnt32> & lAddedTerrains, CString &txt ) const
{
	CKeyTerrain key;
	key.SetSource(m_offre.m_IdSourceAudience);

	//on teste le support pour chaque terrain, si invalide on sort de la boucle
	CRegroupements*	pRegroupements = CRegroupements::GetInstance();
	IElem * elem = pRegroupements->GetIElemByID(IdSupport);

	if (elem != NULL)
	{
		std::list<JUnt32>::const_iterator ter_id;
		for (ter_id = lAddedTerrains.begin(); ter_id != lAddedTerrains.end(); ++ter_id)
		{
			key.SetTerrain(*ter_id);	
			CTerrain * terrain = m_OffreTerrainManager->GetTerrain(key);

			if (!terrain->HasSupport( elem->AsITitre()->GetIDAudience()) )
			{
				txt.Format("%s", elem->GetLabel().AsJCharPtr());
				return false;
			}
		}
	}

	return true;
}

