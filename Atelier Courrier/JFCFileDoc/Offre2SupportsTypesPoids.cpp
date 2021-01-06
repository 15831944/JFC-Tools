#include "stdafx.h"
#include "Offre2SupportsTypesPoids.h"
#include "Offre2MainDialog.h"
#include "Offre2Supports.h"
#include "JSRCTables.h"
#include "JATPOffre.h"

#include <vector>
#include <algorithm>

using namespace std;

//=============================================================================	
// Bo�te de dialogue Offre2SupportsTypesPoids
//=============================================================================	

IMPLEMENT_DYNAMIC(Offre2SupportsTypesPoids, COffre2SupportsOngletBase)

//=============================================================================	
// Gestionnaires de messages de Offre2SupportsTypesPoids
//=============================================================================	

BEGIN_MESSAGE_MAP(Offre2SupportsTypesPoids, COffre2SupportsOngletBase)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LB_SEL_TYPESPOIDS, &Offre2SupportsTypesPoids::OnLbnSelchangeLbSelTypespoids)
END_MESSAGE_MAP()


//=============================================================================	
Offre2SupportsTypesPoids::Offre2SupportsTypesPoids()
	:	COffre2SupportsOngletBase(IDD)
//=============================================================================	
{

}

//=============================================================================	
Offre2SupportsTypesPoids::~Offre2SupportsTypesPoids()
//=============================================================================	
{
}

//=============================================================================	
void Offre2SupportsTypesPoids::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	COffre2SupportsOngletBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_SEL_TYPESPOIDS, m_TypesPoidsList);
}

//=============================================================================	
HBRUSH Offre2SupportsTypesPoids::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//=============================================================================	
{
	switch (nCtlColor)
	{
		// on paint le dialogue lui m�me
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
BOOL Offre2SupportsTypesPoids::OnInitDialog()
//=============================================================================	
{
	COffre2SupportsOngletBase::OnInitDialog();
	FillTypesPoids();
	return TRUE;
}

//=============================================================================	
// retourne true si l'�tat de la s�lection est correct
bool Offre2SupportsTypesPoids::IsSelectionOk() const
//=============================================================================	
{
	return (m_pDialogOffre->GetSupportsSelection().IdTypePoids != 0);
}

//=============================================================================	
// remplit la partie de l'offre correspondant � l'onglet
void Offre2SupportsTypesPoids::FillOffre(JATPOffre * pOffre)
//=============================================================================	
{
	// on modifie eventuellement le type de poids
	if (pOffre->m_IdTypePoids != m_pDialogOffre->GetSupportsSelection().IdTypePoids)
	{
		pOffre->m_IdTypePoids = m_pDialogOffre->GetSupportsSelection().IdTypePoids;
		pOffre->m_Changes |= pOffre->TYP_POIDS;
	}
}

//=============================================================================	
void Offre2SupportsTypesPoids::OnLbnSelchangeLbSelTypespoids()
//=============================================================================	
{
	static int last_selection = LB_ERR;

	// on r�cup�re la s�lection
	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();
	Selection.IdTypePoids = 0;

	// on emp�che de d�selectionner le type de poids courant --> il doit obligatoirement en exister au moins un!!
	int selection = m_TypesPoidsList.GetCurSel();
	if (selection != LB_ERR)
	{
		// on r�cup�re l'id du type de poids s�lectionn�
		Selection.IdTypePoids = static_cast<JUnt32>(m_TypesPoidsList.GetItemData(selection));

		// on indique au dialogue parent une modif
		m_pDialogOffre->SetModified();

		// on copie la s�lection actuelle
		last_selection = selection;
		return;
	}
	else
	{
		// pour �tre sur que l'on a forc�ment un type de poids s�lectionn�, m�me au depart
		ASSERT(last_selection != LB_ERR);
		m_TypesPoidsList.SetCurSel(last_selection);
	}

	// mise � jour du bouton OK de la boite principale de l'Offre
	m_pDialogOffre->EnableOK();

	// cette modif entraine une MAJ des seuils des cibles
	m_pDialogOffre->NotifySourceElementsChange(COffre2MainDialog::TYPEPOIDS_CHANGE);
}

//=============================================================================	
void Offre2SupportsTypesPoids::FillTypesPoids()
//=============================================================================	
{
	// on vide la listbox
	m_TypesPoidsList.ResetContent();

	// on r�cup�re s�lection et filtre
	CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();
	CSupportFilter & filter = m_pOngletTitres->GetFilter();

	// on boucle sur ts les types de poids existants pour remplir la listbox
	vector<const JInt32>::const_iterator  pds_it = m_pOngletTitres->m_TypesPoids.begin();
	while (pds_it != m_pOngletTitres->m_TypesPoids.end())
	{
		JUnt32 IdPoids = static_cast<JUnt32>(*pds_it);
		// on ajoute le libell� du type de poids
		int idx = m_TypesPoidsList.AddLine(JSRCTables::GetInstance().m_TBLPoids.GetLabelByID(IdPoids), m_pDialogOffre->GetAppColorTemplate());
		m_TypesPoidsList.SetItemData(idx, static_cast<DWORD_PTR>(IdPoids));
		++pds_it;
	}

	// on v�rifie l'id du 'type de poids' s�lectionn�
	vector<const JInt32>::const_iterator found = 
		find(m_pOngletTitres->m_TypesPoids.begin(), m_pOngletTitres->m_TypesPoids.end(), Selection.IdTypePoids);
	
	// on parcours la listbox pour s�lectionner le type de poids
	const int count = m_TypesPoidsList.GetCount();
	for (int idx = 0; idx < count; ++idx)
	{
		if (Selection.IdTypePoids == static_cast<JUnt32>(m_TypesPoidsList.GetItemData(idx)))
		{
			m_TypesPoidsList.SetCurSel(idx);
			break;
		}
	}
}

//=============================================================================	
// appel� pour la mise � jour
void Offre2SupportsTypesPoids::UpdateContent()
//=============================================================================	
{
	FillTypesPoids();
}
