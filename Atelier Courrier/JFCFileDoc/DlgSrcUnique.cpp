// DlgSrcUnique.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgSrcUnique.h"
#include "ATPColors.h"

#include <list>
#include <algorithm>

// Boîte de dialogue CDlgSrcUnique

IMPLEMENT_DYNAMIC(CDlgSrcUnique, ETSLayoutDialog)
CDlgSrcUnique::CDlgSrcUnique(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CDlgSrcUnique::IDD, pParent)
{
	// Init table sources
	m_srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Init divers
	m_pData = NULL;
	m_bGripper = false;

	// Init Warning enquête
	m_OkWarningEnqueteAEPM2006 = false;
	m_OkWarningEnqueteFCA2007  = false;
}

CDlgSrcUnique::~CDlgSrcUnique()
{
}

void CDlgSrcUnique::SetData (CDialogChoixOffre* data)
{
	m_pData = data;
}

// Récup l'offre
CDialogChoixOffre * CDlgSrcUnique::GetData()
{
	return m_pData;
}

void CDlgSrcUnique::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
}

BOOL CDlgSrcUnique::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();

	SetupControls();

	Init();

	//l'init n'a rien modifié aux données
	m_pData->m_bModif = false;
	m_pData->m_offre.m_Changes = 0;


	ResetSize();
	UpdateLayout(
		pane(VERTICAL)
			<<item(IDC_FD_STATIC_TTLSOURCE, ABSOLUTE_VERT)
			<<item(IDC_FD_COMBO_SOURCE, ABSOLUTE_VERT)
			<<(pane(HORIZONTAL)<< (pane(VERTICAL)
										<<item(IDC_FD_STATIC_TTLTERRAIN, ABSOLUTE_VERT)
										<<item(IDC_FD_LIST_TERRAIN)
									)
								<<(pane(VERTICAL)
										<<item(IDC_FD_STATIC_TTLSEGMENT, ABSOLUTE_VERT)
										<<item(IDC_FD_LIST_SEGMENT)
										<<item(IDC_FD_STATIC_TTLPOIDS, ABSOLUTE_VERT)
										<<item(IDC_FD_LIST_POIDS)
									)
				)

		);
	
	//
	//m_ttlSource.SubclassDlgItem(IDC_FD_STATIC_TTLSOURCE, this);
	//m_comboSource.SubclassDlgItem(IDC_FD_COMBO_SOURCE, this);
	//m_ttlTerrain.SubclassDlgItem(IDC_FD_STATIC_TTLTERRAIN, this);
	//m_lstTerrain.SubclassDlgItem(IDC_FD_LIST_TERRAIN, this);
	//m_ttlSegment.SubclassDlgItem(IDC_FD_STATIC_TTLSEGMENT, this);
	//m_lstSegment.SubclassDlgItem(IDC_FD_LIST_SEGMENT, this);
	//m_ttlPoids.SubclassDlgItem(IDC_FD_STATIC_TTLPOIDS, this);
	//m_lstPoids.SubclassDlgItem(IDC_FD_LIST_POIDS, this);


	return TRUE;  
}

void CDlgSrcUnique::Init()
{
	FillComboSource();
	//on selectionne la source et on rempli les terrains
	OnCbnSelchangeComboSource();
	//on charge les terrains
	int sel = m_comboSource.GetCurSel();
	if ( sel != CB_ERR )
		FillListTerrain( (JUnt32)m_comboSource.GetItemData(sel) );
	
	OnLbnSelchangeListTerrain();
}

BEGIN_MESSAGE_MAP(CDlgSrcUnique, ETSLayoutDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_FD_COMBO_SOURCE, OnCbnSelchangeComboSource)
	ON_LBN_SELCHANGE(IDC_FD_LIST_TERRAIN, OnLbnSelchangeListTerrain)
	ON_LBN_SELCHANGE(IDC_FD_LIST_SEGMENT, OnLbnSelchangeListSegment)
	ON_LBN_SELCHANGE(IDC_FD_LIST_POIDS, OnLbnSelchangeListPoids)
END_MESSAGE_MAP()

bool CDlgSrcUnique::IsComplete()
{
	// optimisation (pas besoin de tester tt car si 1 seul vaut 0 on return false)
/* ANCIEN CODE
if (m_pData->m_offre.m_IdTerrainCiblage != 0
&& m_pData->m_offre.m_TBLIdTerrainsAudience.GetCount() != 0
&& m_pData->m_offre.m_IdTypePoids != 0
&& m_pData->m_offre.m_TBLIdSegmentsAudience.GetCount() != 0)
return true;
return false;
*/

	if (m_pData->m_offre.m_IdTerrainCiblage == 0
		|| m_pData->m_offre.m_TBLIdTerrainsAudience.GetCount() == 0
		|| m_pData->m_offre.m_IdTypePoids == 0
		|| m_pData->m_offre.m_TBLIdSegmentsAudience.GetCount() == 0)
		return false;
	return true;
}
// Gestionnaires de messages CDlgSrcUnique
void CDlgSrcUnique::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas ETSLayoutFormView::OnPaint() pour la peinture des messages
	RECT Rect;
	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	CBrush Brush(CATPColors::GetColorMedium(m_pData->m_appColor));

	// on dessine le fond
	dc.FillRect(&Rect, &Brush);
}

void CDlgSrcUnique::ResetSize()
{
	CRect rect;
	m_ttlSource.GetWindowRect(rect);

	m_ttlSource.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);

	
	m_lstTerrain.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);
	m_lstSegment.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);

	m_lstPoids.SetWindowPos(NULL,1,1,1,1,SWP_NOZORDER);

	m_ttlTerrain.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_ttlSegment.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	m_ttlPoids.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
	
	m_comboSource.GetWindowRect(rect);
	m_comboSource.SetWindowPos(NULL,1,1,1,rect.Height(),SWP_NOZORDER);
}

void CDlgSrcUnique::FillComboSource()
{
	m_comboSource.ResetContent();
	//on affiche les sources
	for(m_srcTables->m_TBLSources.MoveFirst(); m_srcTables->m_TBLSources.IsValid(); m_srcTables->m_TBLSources.MoveNext())
	{
		const JSource & Src =  m_srcTables->m_TBLSources.GetItem();
		int It = m_comboSource.AddString(Src.m_Libelle.AsJCharPtr());
		m_comboSource.SetItemData(It, Src.m_IdSource );
		//on resélectionne la source
		if ( m_pData->m_offre.m_IdSourceAudience == Src.m_IdSource )
			m_comboSource.SetCurSel(It);
	}
	if (m_comboSource.GetCurSel() == CB_ERR)
		m_comboSource.SetCurSel(0);
}

void CDlgSrcUnique::OnCbnSelchangeComboSource()
{
	int sel = m_comboSource.GetCurSel();
	if ( sel != CB_ERR )
	{
		JUnt32 IDsrctmp = (JUnt32)m_comboSource.GetItemData(sel);
		//si la source a bel et bien changée
		if (m_pData->m_offre.m_IdSourceAudience != IDsrctmp)
		{
			if(IDsrctmp == 35)
				AfxMessageBox("Attention : \r\nPour construire ou évaluer des plans mono-famille sur la BUP,\r\nAudipresse recommande de revenir aux bandes sources AEPM ou EPIC",MB_OK|MB_ICONINFORMATION);

			
			//il y a eu modif
			m_pData->m_bModif = true;
			m_pData->m_offre.m_Changes |= m_pData->m_offre.SRC_AUDIENCE;

			m_pData->m_offre.m_IdSourceAudience = IDsrctmp;
			m_pData->m_offre.m_IdTerrainCiblage = 0;
			m_pData->m_offre.m_TBLIdTerrainsAudience.Reset();
			m_pData->EnableOK();

			FillListTerrain( IDsrctmp  );
			OnLbnSelchangeListTerrain();
		}
	}
}

void CDlgSrcUnique::FillListTerrain(int idSource)
{
	m_lstTerrain.ResetContent();
	for(m_srcTables->m_TBLTerrains.MoveFirst(); m_srcTables->m_TBLTerrains.IsValid(); m_srcTables->m_TBLTerrains.MoveNext())
	{
		const JTerrain & Terr = m_srcTables->m_TBLTerrains.GetItem();
		//on affiche uniquement les terrains de la source
		if( Terr.m_IdSource == idSource )
		{
			int It = m_lstTerrain.AddString(Terr.m_Libelle.AsJCharPtr());
			m_lstTerrain.SetItemData(It, Terr.m_IdTerrain );
			//On applique la sélection
			for ( m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst() ; m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid(); m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext() )
			{
				if ( m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem() == Terr.m_IdTerrain )
					m_lstTerrain.SetSel(It, true);
			}
		}
	}

	if (m_lstTerrain.GetSelCount() == 0)
	{
		//on préselectionne le premier terrain
		m_lstTerrain.SetSel(0);
	}

	FillListSegment();
	FillListPoids();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Sélection nouveau terrain
void CDlgSrcUnique::OnLbnSelchangeListTerrain()
{
	CWaitCursor Wait;
	m_pData->m_bModif = true;
	m_pData->m_offre.m_Changes |= m_pData->m_offre.TER_AUDIENCE;
	m_pData->m_offre.m_Changes |= m_pData->m_offre.TER_CIBLAGE;

	// Init Terrain de ciblage et table des terrains d'audience
	// on crée une liste avec les terrains actuellement selectionnés (avant ce click)
	std::list<JUnt32> lSelectedTerrains;
	for (	m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst();
			m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid();
			m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		lSelectedTerrains.push_back(m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem());
	}

	m_pData->m_offre.m_IdTerrainCiblage = 0;
	m_pData->m_offre.m_TBLIdTerrainsAudience.Reset();

	// Init Annee terrain sélection
	JUnt32 DernAnneeTerrain  = 0;
	JBool  MessageConsoJuniorOk = false;

	// Instance des tables
	const JSRCTables  & srcTables	= JSRCTables::GetInstance();

	// Boucle sur les terrains sélectionnés
	for (int i = 0; i < m_lstTerrain.GetCount() ; i++)
	{
		//si le terrain est sélectionné
		if ( m_lstTerrain.GetSel(i) )
		{
			JUnt32 IDTerrain = (JUnt32)m_lstTerrain.GetItemData(i);

			// On choisit le terrain le + recent comme terrain cible
			//
			// Attention, dans le cas d'enquete avec segments de population (doivent être identiques de terrain en terrain)
			// -->> voir problème ConsoJunior 2008 (4 segments) et avant (3 segments)
			//
			if ( IDTerrain > m_pData->m_offre.m_IdTerrainCiblage )
			{
				// Positionne le terrain de ciblage
				m_pData->m_offre.m_IdTerrainCiblage = IDTerrain;

				const JTerrain * pTerrain		= srcTables.m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience , IDTerrain);

				// Ajoute libellé dans liste		 
				DernAnneeTerrain  = atoi(pTerrain->m_Libelle.AsJCharPtr());
			}

			/* !!!! Retour au mode précédent - Tous les Conso Junior avec le même nombre de segments
			///////////////////////////////////////////////// Spécial CONSO-JUNIOR /////////////////////////////////////////////////
			// Attention au problème ConsoJunior (Grosse bidouille en attendant une politique de gestion terrain plus efficace !!!!)
			const JTerrain * pTerrain		= srcTables.m_TBLTerrains.GetItem(m_pData->m_offre.m_IdSourceAudience , IDTerrain);
			if (m_pData->m_offre.m_IdSourceAudience == 7)
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
			
			m_pData->m_offre.m_TBLIdTerrainsAudience.AddTail() = IDTerrain;

			////////////////////////////////////////////////////////////////////////////////////////
			//								Message Spécial AEPM 2006
			// Attention provisoire message prévention autre méthodoligie de calcul
			// pour Aepm Janvier-Décembre 2006  (pas très propre mais comment faire avec !!!!)
			if (m_pData->m_offre.m_IdSourceAudience == 1 && IDTerrain == 251 && !m_OkWarningEnqueteAEPM2006)
			{
				CString TxtMess;
				TxtMess.LoadString(IDS_FD_MESS_AEPM_JANV_DEC_2006); 
				// AfxMessageBox("Les changements introduits dans l'enquête en 2006 étant \nimportants, les écarts d'audience existant entre 2005 et 2006 peuvent \ntout autant provenir de la méthodologie que d'une évolution réelle de \nl'audience. En conséquence, Audipresse s'interdit tout traitement \ncomparatif des résultats de 2006 avec ceux des années antérieures et \ndemande aux utilisateurs d'en faire autant",MB_OK|MB_ICONINFORMATION);
				AfxMessageBox(TxtMess,MB_OK|MB_ICONINFORMATION);

				m_OkWarningEnqueteAEPM2006 = true;
			}

			////////////////////////////////////////////////////////////////////////////////////////
			//								Message Spécial FCA 2007
			// Attention provisoire message prévention autre méthodoligie de calcul
			// pour France Cadres Actifs 2007
			// Rappel : les résultats de la France des Cadres Actifs 2007 ne doivent pas être comparés à ceux de l’édition 2006.
			// Cette décision est liée au redressement de l’enquête qui pour la première fois depuis 2003, a pu être actualisé au niveau le plus fin.
			if (m_pData->m_offre.m_IdSourceAudience == 3 && IDTerrain == 272 && !m_OkWarningEnqueteFCA2007)
			{
				CString TxtMess;
				TxtMess.LoadString(IDS_FD_MESS_FCA_2007); 
				AfxMessageBox(TxtMess,MB_OK|MB_ICONINFORMATION);

				m_OkWarningEnqueteFCA2007 = true;
			}

			//on charge le terrain
			CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, IDTerrain);
			m_pData->m_OffreTerrainManager->GetTerrain(k);
		}
	}

	////////////////////////////////////////////
	// on cherche le dernier terrain selectionné

	std::list<JUnt32> lAddedTerrains;	// liste des terrains ajoutés
	for (	m_pData->m_offre.m_TBLIdTerrainsAudience.MoveFirst();
			m_pData->m_offre.m_TBLIdTerrainsAudience.IsValid();
			m_pData->m_offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		// le terrain courant etait-il dans la selection précedente
		if (lSelectedTerrains.end() == std::find(lSelectedTerrains.begin(), lSelectedTerrains.end(), m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem()))
		{
			lAddedTerrains.push_back(m_pData->m_offre.m_TBLIdTerrainsAudience.GetItem());
		}
	}
	
	FillListSegment();
	OnLbnSelchangeListSegment();
	FillListPoids();
	OnLbnSelchangeListPoids();
	m_pData->EnableOK();
	m_pData->RazSeuils();
	m_pData->CheckItems();
	m_pData->CheckSupports(lAddedTerrains);
}

void CDlgSrcUnique::FillListSegment()
{
	m_lstSegment.ResetContent();
	//on remplit avec les segments du terrain de ciblage
	CTerrain * pterrain = NULL;
	CKeyTerrain k( m_pData->m_offre.m_IdSourceAudience, m_pData->m_offre.m_IdTerrainCiblage);
	pterrain = m_pData->m_OffreTerrainManager->GetTerrain(k);
	if (pterrain == NULL)
		return;

	JVector<CSegment*> vect_segment;
	if( pterrain->GetSegments(vect_segment) )
	{
		// Update the segment list
		for(vect_segment.MoveFirst(); vect_segment.IsValid(); vect_segment.MoveNext())
		{
			const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(vect_segment.GetItem()->GetID());

			// Libellé segment
			CString LibSegment = Seg->m_Libelle.AsJCharPtr();
			int idx = m_lstSegment.AddString(LibSegment);
			m_lstSegment.SetItemData(idx, Seg->m_IdSegment);

			// Set the selected segments
			for(m_pData->m_offre.m_TBLIdSegmentsAudience.MoveFirst(); m_pData->m_offre.m_TBLIdSegmentsAudience.IsValid(); m_pData->m_offre.m_TBLIdSegmentsAudience.MoveNext())
			{
				if(Seg->m_IdSegment == m_pData->m_offre.m_TBLIdSegmentsAudience.GetItem() )
					m_lstSegment.SetSel(idx,true);
			}
		}

		//s'il n'y a qu'un seul segment le selectionner
		if (m_lstSegment.GetCount() == 1)
			m_lstSegment.SetSel(0,true);
		//s'il y a plus d'un segment autoriser la selection
		if (m_lstSegment.GetCount() > 1)
			m_lstSegment.EnableWindow(TRUE);
		else
			m_lstSegment.EnableWindow(FALSE);
	}
}

void CDlgSrcUnique::FillListPoids()
{
	m_lstPoids.ResetContent();
	//on remplit avec les poids du terrain de ciblage
	CTerrain * pterrain = NULL;
	CKeyTerrain k(m_pData->m_offre.m_IdSourceAudience, m_pData->m_offre.m_IdTerrainCiblage);
	pterrain = m_pData->m_OffreTerrainManager->GetTerrain(k);

	if (pterrain == NULL)
		return;

	JVector<JInt32> vect;
	pterrain->GetTypesPoids(vect);
	// Update the poids list
	for(vect.MoveFirst(); vect.IsValid(); vect.MoveNext())
	{
		int idx = m_lstPoids.AddString( m_srcTables->m_TBLPoids.GetLabelByID(vect.GetItem()) );
		m_lstPoids.SetItemData(idx, vect.GetItem());
		// Set the selected poids
		if (m_pData->m_offre.m_IdTypePoids == vect.GetItem())
			m_lstPoids.SetCurSel(idx);
	}

	//s'il n'y a qu'un seul poids le selectionner
	if (m_lstPoids.GetCount() == 1)
		m_lstPoids.SetCurSel(0);
	//s'il y a plus d'un poids autoriser la selection
	if (m_lstPoids.GetCount() > 1)
		m_lstPoids.EnableWindow(TRUE);
	else
		m_lstPoids.EnableWindow(FALSE);
}

void CDlgSrcUnique::OnLbnSelchangeListSegment()
{
	m_pData->m_bModif = true;
	m_pData->m_offre.m_Changes |= m_pData->m_offre.SEG_AUDIENCE;
	//retenir la sel
	m_pData->m_offre.m_TBLIdSegmentsAudience.Reset();
	for (int i = 0; i < m_lstSegment.GetCount() ; i++)
	{
		//si le segment est sélectionné
		if ( m_lstSegment.GetSel(i) )
		{
			JUnt32 IDSegment = m_lstSegment.GetItemData(i);
			m_pData->m_offre.m_TBLIdSegmentsAudience.AddTail() = IDSegment;
			m_pData->EnableOK();
		}
	}

	// Reste à traiter la déselection de tous les segments, sinon diode reste verte
	if (m_lstSegment.GetSelCount() == 0)
	{
		m_pData->EnableOK();
	}

	m_pData->RazSeuils();	
}

void CDlgSrcUnique::OnLbnSelchangeListPoids()
{
	int sel = m_lstPoids.GetCurSel();
	if ( sel != LB_ERR )
	{
		int poids = m_lstPoids.GetItemData(sel);
		if (poids != m_pData->m_offre.m_IdTypePoids)
		{
			m_pData->m_bModif = true;
			m_pData->m_offre.m_Changes |= m_pData->m_offre.TYP_POIDS;
			m_pData->m_offre.m_IdTypePoids = poids;
			m_pData->EnableOK();
		}
	}
	else
		m_pData->m_offre.m_IdTypePoids = 0;

	m_pData->RazSeuils();	
}

void CDlgSrcUnique::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ETSLayoutDialog::OnShowWindow(bShow, nStatus);
	ResetSize();
	UpdateLayout();
	if(bShow)
	{
		FillComboSource();
		int sel = m_comboSource.GetCurSel();
		if ( sel != CB_ERR )
			FillListTerrain( (JUnt32)m_comboSource.GetItemData(sel) );
	}
}

void CDlgSrcUnique::SetupControls()
{
//subclass 

	m_ttlSource.SubclassDlgItem(IDC_FD_STATIC_TTLSOURCE, this);
	m_comboSource.SubclassDlgItem(IDC_FD_COMBO_SOURCE, this);
	m_ttlTerrain.SubclassDlgItem(IDC_FD_STATIC_TTLTERRAIN, this);
	m_lstTerrain.SubclassDlgItem(IDC_FD_LIST_TERRAIN, this);
	m_ttlSegment.SubclassDlgItem(IDC_FD_STATIC_TTLSEGMENT, this);
	m_lstSegment.SubclassDlgItem(IDC_FD_LIST_SEGMENT, this);
	m_ttlPoids.SubclassDlgItem(IDC_FD_STATIC_TTLPOIDS, this);
	m_lstPoids.SubclassDlgItem(IDC_FD_LIST_POIDS, this);

////

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

void CDlgSrcUnique::SetColors()
{
	m_ttlSource.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlSource.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));
	
	m_ttlTerrain.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlTerrain.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));
	
	m_ttlSegment.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlSegment.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));
	
	m_ttlPoids.SetBkColor(CATPColors::GetColorDark(m_pData->m_appColor),m_pData->m_appColor, CATPColors::DARK_DEGRAD);
	m_ttlPoids.SetTextColor(CATPColors::GetColorSuperDark(m_pData->m_appColor));
	
	m_comboSource.SetSelectionColor( CATPColors::GetColorSelect(m_pData->m_appColor) );

	m_lstTerrain.SetSelectColor( CATPColors::GetColorSelect(m_pData->m_appColor) );
	m_lstSegment.SetSelectColor( CATPColors::GetColorSelect(m_pData->m_appColor) );
	m_lstPoids.SetSelectColor( CATPColors::GetColorSelect(m_pData->m_appColor) );

}

