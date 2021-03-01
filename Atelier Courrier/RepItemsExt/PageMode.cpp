#include "StdAfx.h"
#include ".\pagemode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPageMode::CPageMode(CDlgItems *pDlgItems) : CLineMode(pDlgItems)
{
}

CPageMode::~CPageMode(void)
{
}

void CPageMode::StartMode()
{
	CLineMode::StartMode();
}

// returns the current modes label
void CPageMode::SetModeTitle(void)
{
	CString Label;
	Label.LoadString(IDS_RI_PAGES);
	if(m_pDlgItem->m_iModality == 4)
	{
		CString Filtre;
		Filtre.LoadString(IDS_RI_FILTRES);
		Label.Append(Filtre);
	}
	// m_pDlgItem->m_Mode_Title.SetText(Label);
	Label.LoadString(IDS_RI_TITLE_PUPITRE);
	m_pDlgItem->m_Mode_Title.SetText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();
}

/// Returns a string to the current dimension mode
CString CPageMode::GetDimensionModeString(int Mode)
{
	CString St;
	St.LoadString(IDS_RI_PAGE_DIM);
	return St;
}
