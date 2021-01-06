#include "stdafx.h"

#include "Offre2SupportsSegments.h"
#include "Offre2MainDialog.h"
#include "Offre2Supports.h"

#include "JSegment.h"
#include "JATPOffre.h"

#include <vector>
#include <algorithm>

using namespace std;

//=============================================================================	
// Boîte de dialogue Offre2SupportsSegments
//=============================================================================	

IMPLEMENT_DYNAMIC(Offre2SupportsSegments, COffre2SupportsOngletBase)

//=============================================================================	
// Gestionnaires de messages de Offre2SupportsSegments
//=============================================================================	

BEGIN_MESSAGE_MAP(Offre2SupportsSegments, COffre2SupportsOngletBase)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LB_SEL_SEGMENTS, &Offre2SupportsSegments::OnLbnSelchangeLbSelSegments)
END_MESSAGE_MAP()


//=============================================================================	
Offre2SupportsSegments::Offre2SupportsSegments()
	:	COffre2SupportsOngletBase(IDD)
//=============================================================================	
{

}

//=============================================================================	
Offre2SupportsSegments::~Offre2SupportsSegments()
//=============================================================================	
{
}

//=============================================================================	
void Offre2SupportsSegments::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	COffre2SupportsOngletBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_SEL_SEGMENTS, m_SegmentsList);
}

//=============================================================================	
HBRUSH Offre2SupportsSegments::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	case CTLCOLOR_EDIT:
	case CTLCOLOR_MSGBOX:
		break;
	}

	// sinon on paint comme d'haibute
	return (CDialog::OnCtlColor(pDC, pWnd, nCtlColor));
}

//=============================================================================	
BOOL Offre2SupportsSegments::OnInitDialog()
//=============================================================================	
{
	COffre2SupportsOngletBase::OnInitDialog();
	FillSegments();
	return TRUE;
}

//=============================================================================	
// retourne true si l'état de la sélection est correct
bool Offre2SupportsSegments::IsSelectionOk() const
//=============================================================================	
{
	return (!m_pDialogOffre->GetSupportsSelection().SelectedSegments.empty());
}

//=============================================================================	
// remplit la partie de l'offre correspondant à l'onglet
void Offre2SupportsSegments::FillOffre(JATPOffre * pOffre)
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();

	// on modifie les segments
	set<JUnt32> lastSegments;
	for (pOffre->m_TBLIdSegmentsAudience.MoveFirst();pOffre->m_TBLIdSegmentsAudience.IsValid();pOffre->m_TBLIdSegmentsAudience.MoveNext())
		lastSegments.insert(pOffre->m_TBLIdSegmentsAudience.GetItem());

	Selection.SelectedSegments.sort();

	if ((lastSegments.size() != Selection.SelectedSegments.size()) // taille différentes
		|| !equal(lastSegments.begin(), lastSegments.end(), Selection.SelectedSegments.begin()))
	{
		pOffre->m_TBLIdSegmentsAudience.Reset();

		// il y a des différences, on recopie la sélection de segments dans l'offre
		list<JUnt32>::const_iterator seg_it = Selection.SelectedSegments.begin();
		while (seg_it != Selection.SelectedSegments.end())
		{
			JUnt32 IdSegment = *seg_it;
			pOffre->m_TBLIdSegmentsAudience.AddTail() = IdSegment;
			++seg_it;
		}
		pOffre->m_Changes |= pOffre->SEG_AUDIENCE;
	}
}

//=============================================================================	
void Offre2SupportsSegments::OnLbnSelchangeLbSelSegments()
//=============================================================================	
{
	// on récupère sélection et filtre
	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();
	CSupportFilter & filter = m_pOngletTitres->GetFilter();

	// on les nettoie
	Selection.SelectedSegments.clear();
	filter.segments.clear();

	// on récupère les segments sélectionnés
	vector<const JSegment *> selectedSegments;
	GetControlSelectedData(selectedSegments, m_SegmentsList);
	vector<const JSegment *>::const_iterator segment_it = selectedSegments.begin();
	while(segment_it != selectedSegments.end())
	{
		Selection.SelectedSegments.push_back((*segment_it)->m_IdSegment);
		filter.segments.push_back((*segment_it)->m_IdSegment);
		++segment_it;
	}

	// cette modif entraine une MAJ des seuils des cibles
	m_pDialogOffre->NotifySourceElementsChange(COffre2MainDialog::SEGMENTS_CHANGE);

	// modifie la liste des supports
	m_pOngletTitres->UpdateSupportsDisplay();

	// on indique au dialogue parent une modif
	m_pDialogOffre->SetModified();

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDialogOffre->EnableOK();
}

//=============================================================================	
void Offre2SupportsSegments::FillSegments()
//=============================================================================	
{
	// on vide la listbox
	m_SegmentsList.ResetContent();

	// on récupère sélection et filtre
	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();
	CSupportFilter & filter = m_pOngletTitres->GetFilter();

	filter.SetAllSegments(m_pOngletTitres->m_Segments);

	// on boucle sur ts les segments existants pour remplir la listbox
	vector<const JSegment*>::const_iterator  seg_it = m_pOngletTitres->m_Segments.begin();
	while (seg_it != m_pOngletTitres->m_Segments.end())
	{
		const JSegment * pSegment = *seg_it;
		// on ajoute le libellé du segment
		int idx = m_SegmentsList.AddLine(pSegment->m_Libelle.AsJCharPtr(), m_pDialogOffre->GetAppColorTemplate());
		m_SegmentsList.SetItemData(idx, reinterpret_cast<DWORD_PTR>(pSegment));
		++seg_it;
	}

	// on vérifie chaque id segment de la sélection
	list<JUnt32>::iterator  id_it = Selection.SelectedSegments.begin();
	while (id_it != Selection.SelectedSegments.end())
	{
		// on boucle sur tous les segments chargées
		seg_it = m_pOngletTitres->m_Segments.begin();
		while (seg_it != m_pOngletTitres->m_Segments.end())
		{
			// on teste avec l'id en cours
			if((*seg_it)->m_IdSegment == *id_it)
				break;
			++seg_it;
		}

		// on a pas trouvé l'id courant
		if (seg_it == m_pOngletTitres->m_Segments.end())
		{
			// on l'efface et on passe au suivant
			id_it = Selection.SelectedSegments.erase(id_it);
		}
		else ++id_it;
	}

	// on parcours la listbox pour sélectionner les segments
	const int count = m_SegmentsList.GetCount();
	for (int idx = 0; idx < count; ++idx)
	{
		const JSegment * pSegment = reinterpret_cast<const JSegment*>(m_SegmentsList.GetItemData(idx));
		list<JUnt32>::iterator found = find(Selection.SelectedSegments.begin(), Selection.SelectedSegments.end(), pSegment->m_IdSegment);
		if (found != Selection.SelectedSegments.end())
			m_SegmentsList.SetSel(idx);
	}	
}

//=============================================================================	
// appelé pour la mise à jour
void  Offre2SupportsSegments::UpdateContent()
//=============================================================================	
{
	FillSegments();
}
