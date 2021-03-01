#include "StdAfx.h"
#include "JFCNumericCell.h"

#if !defined(BUILD_FOR_ATELIER_INTERNET)

////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(JFCNumericCell, CGXEditControl)
	ON_WM_CHAR()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////
JFCNumericCell::JFCNumericCell(CGXGridCore* pGrid, UINT nID)
	:	CGXEditControl(pGrid, nID)
	,	INumericCell (pGrid)
{
}

////////////////////////////////////////////////////////////
JFCNumericCell::~JFCNumericCell(void)
{
}

////////////////////////////////////////////////////////////
void JFCNumericCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	bool bCallBase = false;

	CString Text; GetWindowText(Text);
	int start_pos, end_pos;	
	GetSel(start_pos, end_pos);

	do 
	{
		// test des caractères numériques
		if	( (nChar >= '0' && nChar <= '9')	// caractères numériques
			||(nChar == '.' || nChar == ',')	// caractères séparateurs : virgule
			||(nChar == '-'))					// caractère '-'
		{
			if (ValidateValue(nChar, Text, start_pos, end_pos))
			{
				// on appelle le gestionnaire de base
				bCallBase = true;
			}

			// on sort de la boucle
			break;
		}
		// test du moins
		else if ( nChar == '-')
		{
			if (ValidateValue(nChar, Text, start_pos, end_pos))
			{
				// on appelle le gestionnaire de base
				bCallBase = true;
			}

			// on sort de la boucle
			break;
		}
		// tests des caractères spéciaux
		else if (  nChar == VK_BACK
			|| nChar == VK_ESCAPE
			|| nChar == VK_TAB
			|| nChar == VK_RETURN
			|| nChar == VK_DELETE)
		{
			// on appelle le gestionnaire de base
			bCallBase = true;

			// on sort de la boucle
			break;
		}
	}
	while (false);

	if (bCallBase)
	{
		//CString msg; msg.Format(" CGXSpinEditCell::OnChar(nChar : %d)\n", nChar);
		//OutputDebugString(msg);

		// on appelle le gestionnaire de base
		CGXEditControl::OnChar(nChar, nRepCnt, nFlags);
	}
}

////////////////////////////////////////////////////////////
BOOL JFCNumericCell::OnEndEditing()
{
	m_pParentGrid->m_pGridWnd->SendMessage(SPINEDITCELL_END_EDIT, 0, 0);
	return TRUE;
}

#endif //!!defined(BUILD_FOR_ATELIER_INTERNET)