#include "StdAfx.h"
#include ".\columnmode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColumnMode::CColumnMode(CDlgItems *pDlgItems) : CLineMode(pDlgItems)
{
}

CColumnMode::~CColumnMode(void)
{
}

void CColumnMode::StartMode()
{
	CLineMode::StartMode();
}

// returns the current modes label
void CColumnMode::SetModeTitle(void)
{
	CString Label;
	Label.LoadString(IDS_RI_COLUMN);
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
CString CColumnMode::GetDimensionModeString(int Mode)
{
	CString St;
	St.LoadString(IDS_RI_COLUMN_DIM);
	return St;
}
