// CSrcUniqueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SrcUniqueDlg.h"
#include "ATPColors.h"
#include "TerrainManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SrcUniqueDlg dialog

IMPLEMENT_DYNAMIC(CSrcUniqueDlg, CDialog)
CSrcUniqueDlg::CSrcUniqueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrcUniqueDlg::IDD, pParent)
{
	m_srcTables		=	(JSRCTables*)& JSRCTables::GetInstance();
	m_Terrains.		Reset();
	m_SelTerrains.	Reset();
	m_SelSegments.	Reset();
	m_SourceId		=	0;
	m_TypePoidsId	=	0;
	m_TerrainId		=	0;
}

CSrcUniqueDlg::~CSrcUniqueDlg()
{
	for(m_Terrains.MoveFirst(); m_Terrains.IsValid(); m_Terrains.MoveNext())
	{
		CTerrain *pTerrain = m_Terrains.GetItem();
		if(pTerrain) CTerrainManager::GetInstance()->ReleaseTerrain(pTerrain); 
	}
}

void CSrcUniqueDlg::SetData(JUnt32 SourceId, const JList<UINT> & Terrains, const JList<UINT> & Segments, JUnt32 TypePoids)
{
	m_SourceId		=	SourceId;
	m_TypePoidsId	=	TypePoids;

	m_Terrains.		Reset();
	m_SelTerrains.	Reset();

	for(Terrains.MoveFirst(); Terrains.IsValid(); Terrains.MoveNext())
	{
		JUnt32 TerrainId = Terrains.GetItem();
		CKeyTerrain Key;
		Key.Set(SourceId,TerrainId); 
		m_Terrains.Add(Key)		= CTerrainManager::GetInstance()->GetTerrain(Key);
		m_SelTerrains.AddTail() = TerrainId;
	}

	m_SelSegments.Reset();
	for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
	{
		m_SelSegments.AddTail() = Segments.GetItem();
	}
}

void CSrcUniqueDlg::GetData(JUnt32 & SourceId, JList<UINT> & Terrains, JList<UINT> & Segments, JUnt32 & TypePoids)
{
	SourceId	=	m_SourceId;
	TypePoids	=	m_TypePoidsId;

	Terrains.Reset();
	for(m_SelTerrains.MoveFirst(); m_SelTerrains.IsValid(); m_SelTerrains.MoveNext())
		Terrains.AddTail() = m_SelTerrains.GetItem();

	Segments.Reset();
	for(m_SelSegments.MoveFirst(); m_SelSegments.IsValid(); m_SelSegments.MoveNext())
		Segments.AddTail() = m_SelSegments.GetItem();
}


void CSrcUniqueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSrcUniqueDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_AN_COMBO_SOURCE, OnCbnSelchangeComboSource)
	ON_LBN_SELCHANGE(IDC_AN_LIST_TERRAIN, OnLbnSelchangeListTerrain)
	ON_LBN_SELCHANGE(IDC_AN_LIST_SEGMENT, OnLbnSelchangeListSegment)
	ON_LBN_SELCHANGE(IDC_AN_LIST_POIDS, OnLbnSelchangeListPoids)
END_MESSAGE_MAP()


// CSrcUniqueDlg message handlers

BOOL CSrcUniqueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupControls();
	FillComboSource();
	FillListTerrain();
	FillListSegment();
	FillListPoids();
	UpdateOk();

	return TRUE;  
}

CTerrain * CSrcUniqueDlg::GetTerrain(CKeyTerrain & Key)
{
	m_Terrains.MoveTo(Key);	if (m_Terrains.IsValid()) return m_Terrains.GetItem();

	return m_Terrains.Add(Key) = CTerrainManager::GetInstance()->GetTerrain(Key);
}

void CSrcUniqueDlg::FillComboSource()
{
	JUnt32 SourceId = 0;
	m_comboSource.ResetContent();
	//on affiche les sources
	for(m_srcTables->m_TBLSources.MoveFirst(); m_srcTables->m_TBLSources.IsValid(); m_srcTables->m_TBLSources.MoveNext())
	{
		const JSource & Src =  m_srcTables->m_TBLSources.GetItem();
		int It = m_comboSource.AddString(Src.m_Libelle.AsJCharPtr());
		m_comboSource.SetItemData(It, Src.m_IdSource);
		//on resélectionne la source
		if (m_SourceId == Src.m_IdSource)
		{
			m_comboSource.SetCurSel(It);
			SourceId = Src.m_IdSource;
		}
	}
	m_SourceId = SourceId;
	if (m_SourceId == 0)
	{
		if (m_comboSource.GetCount() > 0)
		{
			m_comboSource.SetCurSel(0);
			m_SourceId = (JUnt32)m_comboSource.GetItemData(0);
		}
	}
}

void CSrcUniqueDlg::FillListTerrain()
{
	m_TerrainId = 0;
	JList<UINT> SelTerrains;
	m_lstTerrain.ResetContent();
	for(m_srcTables->m_TBLTerrains.MoveFirst(); m_srcTables->m_TBLTerrains.IsValid(); m_srcTables->m_TBLTerrains.MoveNext())
	{
		const JTerrain & Terr = m_srcTables->m_TBLTerrains.GetItem();
		//on affiche uniquement les terrains de la source
		if (m_SourceId == Terr.m_IdSource)
		{
			int It = m_lstTerrain.AddString(Terr.m_Libelle.AsJCharPtr());
			m_lstTerrain.SetItemData(It, Terr.m_IdTerrain);
			//On applique la sélection
			for (m_SelTerrains.MoveFirst() ; m_SelTerrains.IsValid(); m_SelTerrains.MoveNext())
			{
				if (m_SelTerrains.GetItem() == Terr.m_IdTerrain)
				{
					m_lstTerrain.SetSel(It, true);
					SelTerrains.AddTail() = Terr.m_IdTerrain;
				}
			}
		}
	}
	m_SelTerrains.Swap(SelTerrains);
	if (m_SelTerrains.GetCount() == 0)
	{
		if (m_lstTerrain.GetCount() > 0)
		{
			m_lstTerrain.SetSel(0);
			m_SelTerrains.AddTail() = (JUnt32)m_lstTerrain.GetItemData(0);
		}
	}
	for (m_SelTerrains.MoveFirst() ; m_SelTerrains.IsValid(); m_SelTerrains.MoveNext())
	{
		if (m_TerrainId < m_SelTerrains.GetItem()) m_TerrainId = m_SelTerrains.GetItem();
	}
}

void CSrcUniqueDlg::FillListSegment()
{
	JList<UINT> SelSegments;
	m_lstSegment.ResetContent();
	if (m_SourceId == 0 || m_TerrainId == 0) { m_SelSegments.Swap(SelSegments); return; }

	//on remplit avec les segments du terrain de ciblage
	CTerrain * pterrain = NULL;
	CKeyTerrain k(m_SourceId, m_TerrainId);
	pterrain = GetTerrain(k); if (pterrain == NULL) { m_SelSegments.Swap(SelSegments); return; }

	JVector<CSegment*> vect_segment;
	if (pterrain->GetSegments(vect_segment))
	{
		// Update the segment list
		for (vect_segment.MoveFirst(); vect_segment.IsValid(); vect_segment.MoveNext())
		{
			const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(vect_segment.GetItem()->GetID());
			int idx = m_lstSegment.AddString(Seg->m_Libelle.AsJCharPtr());
			m_lstSegment.SetItemData(idx, Seg->m_IdSegment);
			for (m_SelSegments.MoveFirst(); m_SelSegments.IsValid(); m_SelSegments.MoveNext())
			{
				if (Seg->m_IdSegment == m_SelSegments.GetItem())
				{
					m_lstSegment.SetSel(idx,true);
					SelSegments.AddTail() = Seg->m_IdSegment;
				}
			}
		}
	}
	m_SelSegments.Swap(SelSegments);
	if (m_SelSegments.GetCount() == 0)
	{
		int nbsegments = m_lstSegment.GetCount();
		for (int idx = 0; idx < nbsegments; idx += 1)
		{
			m_lstSegment.SetSel(idx,true);
			m_SelSegments.AddTail() = (JUnt32)m_lstSegment.GetItemData(idx);
		}
	}
	//s'il y a plus d'un segment autoriser la selection
	if (m_lstSegment.GetCount() > 1)
		m_lstSegment.EnableWindow(TRUE);
	else
		m_lstSegment.EnableWindow(FALSE);
}

void CSrcUniqueDlg::FillListPoids()
{
	JUnt32 TypePoidsId = 0;
	m_lstPoids.ResetContent();
	if (m_SourceId == 0 || m_TerrainId == 0) { m_TypePoidsId = TypePoidsId; return; }

	//on remplit avec les poids du terrain de ciblage
	CTerrain * pterrain = NULL;
	CKeyTerrain k(m_SourceId, m_TerrainId);
	pterrain = GetTerrain(k); if (pterrain == NULL) { m_TypePoidsId = TypePoidsId; return; }

	JVector<JInt32> vect;
	if (pterrain->GetTypesPoids(vect))
	{
		// Update the poids list
		for(vect.MoveFirst(); vect.IsValid(); vect.MoveNext())
		{
			int idx = m_lstPoids.AddString( m_srcTables->m_TBLPoids.GetLabelByID(vect.GetItem()) );
			m_lstPoids.SetItemData(idx, vect.GetItem());
			// Set the selected poids
			if (m_TypePoidsId == vect.GetItem())
			{
				m_lstPoids.SetCurSel(idx);
				TypePoidsId = vect.GetItem();
			}
		}
	}
	m_TypePoidsId = TypePoidsId;
	if (m_TypePoidsId == 0)
	{
		if (m_lstPoids.GetCount() > 0)
		{
			m_lstPoids.SetCurSel(0);
			m_TypePoidsId = (JUnt32)m_lstPoids.GetItemData(0);
		}
	}
	//s'il y a plus d'un poids autoriser la selection
	if (m_lstPoids.GetCount() > 1)
		m_lstPoids.EnableWindow(TRUE);
	else
		m_lstPoids.EnableWindow(FALSE);
}

void CSrcUniqueDlg::UpdateOk()
{
	CWnd* pButtonOk = GetDlgItem(IDOK);
	if (pButtonOk != 0)
	{
		if (m_SourceId > 0 && m_SelTerrains.GetCount() > 0 && m_SelSegments.GetCount() > 0 && m_TypePoidsId > 0)
			pButtonOk->EnableWindow(TRUE);
		else
			pButtonOk->EnableWindow(FALSE);
	}
}

void CSrcUniqueDlg::OnCbnSelchangeComboSource()
{
	CWaitCursor Wait;
	m_lstPoids  .ResetContent(); m_lstPoids  .UpdateWindow();
	m_lstSegment.ResetContent(); m_lstSegment.UpdateWindow();
	int sel = m_comboSource.GetCurSel();
	if (sel != CB_ERR)
		m_SourceId = (JUnt32)m_comboSource.GetItemData(sel);
	else
		m_SourceId = 0;
	FillListTerrain();
	FillListSegment();
	FillListPoids();
	UpdateOk();
}

void CSrcUniqueDlg::OnLbnSelchangeListTerrain()
{
	CWaitCursor Wait;

	// Init Annee terrain sélection
	JUnt32 DernAnneeTerrain  = 0;
	JBool  MessageConsoJuniorOk = false;

	// Instance des tables
	const JSRCTables  & srcTables	= JSRCTables::GetInstance();

	m_TerrainId  = 0;
	m_SelTerrains.Reset();
	for (int i = 0; i < m_lstTerrain.GetCount() ; i++)
	{
		//si le terrain est sélectionné
		if (m_lstTerrain.GetSel(i))
		{
			JUnt32 IDTerrain = (JUnt32)m_lstTerrain.GetItemData(i);

			// On choisit le terrain le + recent comme terrain cible
			//
			// Attention, dans le cas d'enquete avec segments de population (doivent être identiques de terrain en terrain)
			// -->> voir problème ConsoJunior 2008 (4 segments) et avant (3 segments)
			//
			if (IDTerrain > m_TerrainId)
			{
				const JTerrain * pTerrain		= srcTables.m_TBLTerrains.GetItem(m_SourceId, IDTerrain);

				// Ajoute libellé dans liste		 
				DernAnneeTerrain  = atoi(pTerrain->m_Libelle.AsJCharPtr());
			}

			/* !!!! Retour au mode précédent - Tous les Conso Junior avec le même nombre de segments
			///////////////////////////////////////////////// Spécial CONSO-JUNIOR /////////////////////////////////////////////////
			// Attention au problème ConsoJunior (Grosse bidouille en attendant une politique de gestion terrain plus efficace !!!!)
			const JTerrain * pTerrain		= srcTables.m_TBLTerrains.GetItem(m_SourceId , IDTerrain);
			if (m_SourceId == 7)
			{
				if (DernAnneeTerrain >= 2008 && atoi(pTerrain->m_Libelle.AsJCharPtr()) < 2008)
				{
					// Ici les nouveaux terrains consojunior ne sont pas compatibles avec le 1er terrain conso junior >= 2008
					m_lstTerrain.SetSel(i, false);

					// Message de prévention indiquant la non compatibilité des terrains Conso-Junior
					if  (!MessageConsoJuniorOk)
						AfxMessageBox("Attention pour Conso-Junior le mode de segmentation a changé (à partir de 2008, 4 segments au lieu de 3), \nce qui rend incompatible l'utilisation de certains terrains entre eux", MB_ICONINFORMATION);

					// Message conso junior effectué
					MessageConsoJuniorOk = true;

					continue;
				}
				else if (DernAnneeTerrain < 2008 && atoi(pTerrain->m_Libelle.AsJCharPtr()) >= 2008 && !MessageConsoJuniorOk)
				{
					// Ici les nouveaux terrains consojunior ne sont pas compatibles avec le 1er terrain conso junior < 2008
					m_lstTerrain.SetSel(i, false);

					// Message de prévention indiquant la non compatibilité des terrains Conso-Junior
					if  (!MessageConsoJuniorOk)
						AfxMessageBox("Attention pour Conso-Junior le mode de segmentation a changé (à partir de 2008, 4 segments au lieu de 3), \n ce qui rend incompatible l'utilisation de certains terrains entre eux", MB_ICONINFORMATION);

					// Message conso junior effectué
					MessageConsoJuniorOk = true;

					continue;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			*/


			m_SelTerrains.AddTail() = IDTerrain;
			if(IDTerrain > m_TerrainId) m_TerrainId = IDTerrain;
			//on charge le terrain
			CKeyTerrain k(m_SourceId, IDTerrain);
			GetTerrain(k);
		}
	}

	FillListSegment();
	FillListPoids();
	UpdateOk();
}

void CSrcUniqueDlg::OnLbnSelchangeListSegment()
{
	//retenir la sel
	m_SelSegments.Reset();
	for (int i = 0; i < m_lstSegment.GetCount() ; i++)
	{
		//si le segment est sélectionné
		if ( m_lstSegment.GetSel(i) )
		{
			JUnt32 IDSegment = (JUnt32)m_lstSegment.GetItemData(i);
			m_SelSegments.AddTail() = IDSegment;
		}
	}
	UpdateOk();
}

void CSrcUniqueDlg::OnLbnSelchangeListPoids()
{
	int sel = m_lstPoids.GetCurSel();
	if ( sel != LB_ERR )
		m_TypePoidsId = (JUnt32)m_lstPoids.GetItemData(sel);
	else
		m_TypePoidsId = 0;
	UpdateOk();
}

void CSrcUniqueDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
	{
		FillComboSource();
		FillListTerrain();
		FillListSegment();
		FillListPoids();
		UpdateOk();
	}
}

void CSrcUniqueDlg::SetupControls()
{
//subclass 

	m_bkgSrc.		SubclassDlgItem(IDC_AN_STATIC_SOURCE, this);
	m_ttlSource.	SubclassDlgItem(IDC_AN_STATIC_TTLSOURCE, this);
	m_comboSource.	SubclassDlgItem(IDC_AN_COMBO_SOURCE, this);
	m_ttlTerrain.	SubclassDlgItem(IDC_AN_STATIC_TTLTERRAIN, this);
	m_lstTerrain.	SubclassDlgItem(IDC_AN_LIST_TERRAIN, this);
	m_ttlSegment.	SubclassDlgItem(IDC_AN_STATIC_TTLSEGMENT, this);
	m_lstSegment.	SubclassDlgItem(IDC_AN_LIST_SEGMENT, this);
	m_ttlPoids.		SubclassDlgItem(IDC_AN_STATIC_TTLPOIDS, this);
	m_lstPoids.		SubclassDlgItem(IDC_AN_LIST_POIDS, this);
	m_But_Ok. 		SubclassDlgItem(IDOK				,this);
	m_But_Cancel. 	SubclassDlgItem(IDCANCEL			,this);

////
	m_bkgSrc.SetTransparent(false);
	m_bkgSrc.SetSunken(true);
	m_bkgSrc.AddTopWindow(m_ttlSource);
	m_bkgSrc.AddTopWindow(m_comboSource);
	m_bkgSrc.AddTopWindow(m_ttlTerrain);
	m_bkgSrc.AddTopWindow(m_lstTerrain);
	m_bkgSrc.AddTopWindow(m_ttlSegment);
	m_bkgSrc.AddTopWindow(m_lstSegment);
	m_bkgSrc.AddTopWindow(m_ttlPoids);
	m_bkgSrc.AddTopWindow(m_lstPoids);

////
	m_ttlSource.SetFontName(CATPColors::GetFontLabelName());
	m_ttlSource.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlSource.SetBorder(false);

	m_ttlTerrain.SetFontName(CATPColors::GetFontLabelName());
	m_ttlTerrain.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlTerrain.SetBorder(false);

	m_ttlSegment.SetFontName(CATPColors::GetFontLabelName());
	m_ttlSegment.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlSegment.SetBorder(false);

	m_ttlPoids.SetFontName(CATPColors::GetFontLabelName());
	m_ttlPoids.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlPoids.SetBorder(false);

	SetColors();
}

void CSrcUniqueDlg::SetColors()
{
	m_bkgSrc.		SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::MEDIUM_DEGRAD);
	m_ttlSource.	SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_ttlSource.	SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_ttlTerrain.	SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_ttlTerrain.	SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_ttlSegment.	SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_ttlSegment.	SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));
	m_ttlPoids.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_ttlPoids.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR));

	m_comboSource.	SetSelectionColor( CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR) );

	m_lstTerrain.	SetSelectColor( CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR) );
	m_lstSegment.	SetSelectColor( CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR) );
	m_lstPoids.		SetSelectColor( CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR) );

}
