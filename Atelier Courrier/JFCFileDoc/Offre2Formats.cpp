// DlgOffre2_Formats.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "Offre2Formats.h"
#include "Offre2MainDialog.h"

#include "JATPOffre.h"
#include "JSRCPresse.h"
#include "AllTypeOfNode.h"
#include "Tarifs.h"

#include <set>
#include <iterator>
#include <algorithm>

//=============================================================================	
// Boîte de dialogue COffre2Formats
//=============================================================================	

IMPLEMENT_DYNAMIC(COffre2Formats, COffre2OngletBase)

//=============================================================================	
// Gestionnaires de messages de COffre2Formats
//=============================================================================	

BEGIN_MESSAGE_MAP(COffre2Formats, COffre2OngletBase)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LST_FORMATS, &COffre2Formats::OnLbnSelchangeLstFormats)
END_MESSAGE_MAP()

//=============================================================================	
COffre2Formats::COffre2Formats(COffre2MainDialog * pDialogOffre)
	:	COffre2OngletBase(IDD, pDialogOffre)
//=============================================================================	
{
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
}

//=============================================================================	
COffre2Formats::~COffre2Formats()
//=============================================================================	
{
}

//=============================================================================	
void COffre2Formats::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	COffre2OngletBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_FORMATS,			m_FormatsList);
	//DDX_Control(pDX, IDC_RAZFORMATS,			m_RAZSelect);
	DDX_Control(pDX, IDC_FD_STATIC_TTLFORMATS,	m_ttlFormats);
}

//=============================================================================	
HBRUSH COffre2Formats::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//=============================================================================	
{
	switch (nCtlColor)
	{
		// on paint le dialogue lui même
	case CTLCOLOR_DLG:
		return m_pDlgOffre->GetBackgroundBrush();

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
// pour définir la sélection par défaut
void COffre2Formats::SetDefaultSelection(const std::list<JUnt32> & defaultSel)
//=============================================================================	
{
	// on copie les formats localement
	m_selectedFormats.clear();
	copy(defaultSel.begin(), defaultSel.end(), std::back_inserter(m_selectedFormats));

	// si l'onglet n'est pas encore crée, on quitte
	if (::IsWindow(m_hWnd))
	{
		// l'onglet est déjà crée, on sélectionne les formats passés, on déslectionne les autres
		m_FormatsList.SelItemRange(FALSE, 0, m_FormatsList.GetCount());
		int idx = 0;
		const int nMax = m_FormatsList.GetCount();
		JFormat * pFormat = 0;
		while (idx != nMax)
		{
			pFormat = reinterpret_cast<JFormat*>(m_FormatsList.GetItemData(idx));
			if (std::find(m_selectedFormats.begin(), m_selectedFormats.end(), pFormat->GetID())
				!= m_selectedFormats.end())
			{
				m_FormatsList.SetSel(idx, TRUE);
			}
			++idx;
		}			
	}
}

//=============================================================================	
BOOL COffre2Formats::OnInitDialog()
//=============================================================================	
{
	COffre2OngletBase::OnInitDialog();

	// configuration du label 'filtres'
	m_ttlFormats.SetFontName(CATPColors::GetFontLabelName());
	m_ttlFormats.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlFormats.SetBorder(false);
	m_ttlFormats.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlFormats.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	// configuration du bouton Tous
	//const char pszTooltip [] = "Permet de sélectionner/déselectionner tous les formats";
	//m_RAZSelect.SetTooltipText(pszTooltip);

	//m_RAZSelect.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()),0);
	//m_RAZSelect.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pDlgOffre->GetAppColor()),0);
	//m_RAZSelect.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()));

	// on remplit la liste de formats
	FillListFormats();

	return TRUE;
}

void COffre2Formats::GetFormats(JList<JUnt32> & lstIDFormat) const
{
	lstIDFormat.Reset();
	std::list<JUnt32>::const_iterator fmt_it = m_allFormats.begin();
	while(fmt_it != m_allFormats.end())
	{
		lstIDFormat.AddTail() = *fmt_it;
		++fmt_it;
	}
}

//=============================================================================	
// remplit la listbox des formats
void COffre2Formats::FillListFormats()
//=============================================================================	
{
	const HXTemplateHandle hTemplate = m_pDlgOffre->GetAppColorTemplate();
	m_FormatsList.ResetContent();
	JFormat* pFormat = 0;
	int idx = LB_ERR;

	m_allFormats.clear();

	// on charge tous les formats existants
	for( m_srcPresse->m_TBLFormats.MoveFirst(); m_srcPresse->m_TBLFormats.IsValid(); m_srcPresse->m_TBLFormats.MoveNext() )
	{
		pFormat = static_cast<JFormat*>(m_srcPresse->m_TBLFormats.GetItem());
		ASSERT(pFormat);

		m_allFormats.push_back(pFormat->GetID());

		// on ajoute leur label comme chaine et le pointeur vers le format comme data
		idx = m_FormatsList.AddLine(m_srcPresse->m_TBLFormats.GetItem()->GetLabel().AsJCharPtr(), hTemplate);
		ASSERT(idx != LB_ERR);

		m_FormatsList.SetItemData(idx, reinterpret_cast<DWORD_PTR>(pFormat));
	}

	m_FormatsList.SetTopIndex(0);
}

//=============================================================================	
void COffre2Formats::OnLbnSelchangeLstFormats()
//=============================================================================	
{
	// on récupère tous les JFormats sélectionnés
	std::set<JFormat*> jformats;
	GetControlSelectedData(jformats, m_FormatsList);

	m_selectedFormats.clear();		
	if (!jformats.empty())
	{
		std::set<JFormat*>::const_iterator jfmt_it = jformats.begin();
		while(jfmt_it != jformats.end())
		{
			m_selectedFormats.push_back((*jfmt_it)->GetID());
			++jfmt_it;
		}
	}
	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDlgOffre->EnableOK();
}

//=============================================================================	
// retourne true si l'état de la sélection est correct
bool COffre2Formats::IsSelectionOk() const
//=============================================================================	
{
	return (!m_selectedFormats.empty());
}

//=============================================================================	
// remplit la partie de l'offre correspondant à l'onglet (Formats)
void COffre2Formats::FillOffre(JATPOffre * pOffre)
//=============================================================================	
{
	std::list<JUnt32> off_curFmt;

	// on recopie les id formats de l'offre courante
	for(pOffre->m_TBLIdFormats.MoveFirst(); pOffre->m_TBLIdFormats.IsValid(); pOffre->m_TBLIdFormats.MoveNext())
		off_curFmt.push_back(pOffre->m_TBLIdFormats.GetItem());

	off_curFmt.sort();off_curFmt.unique();
	m_selectedFormats.sort();m_selectedFormats.unique();

	if (m_selectedFormats.size() != static_cast<size_t>(pOffre->m_TBLIdFormats.GetCount())
		|| ((m_selectedFormats.size() == static_cast<size_t>(pOffre->m_TBLIdFormats.GetCount()))
			&& !std::equal(off_curFmt.begin(), off_curFmt.end(), m_selectedFormats.begin())))
	{
		// l'offre a été modifiée
		pOffre->m_TBLIdFormats.Reset();
		
		std::list<JUnt32>::const_iterator fmt_it = m_selectedFormats.begin();
		while (fmt_it != m_selectedFormats.end())
		{
			pOffre->m_TBLIdFormats.AddTail() = *fmt_it;
			pOffre->m_Changes |= JATPOffre::FORMATS;			
			++fmt_it;
		}
	}
}
