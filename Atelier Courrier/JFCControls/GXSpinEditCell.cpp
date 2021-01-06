#include "StdAfx.h"
#include "GXSpinEditCell.h"

#if !defined(BUILD_FOR_ATELIER_INTERNET)

////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGXSpinEditCell, CGXSpinEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////
CGXSpinEditCell::~CGXSpinEditCell()
{

}

////////////////////////////////////////////////////////////
// la fonction des gestion du click sur les boutons du SpinEdit
void CGXSpinEditCell::OnClickedButton(CGXChild* pChild)
{
	BOOL bActive = IsActive();

	if (!bActive && !OnStartEditing())
		return;

	// on active le contrôle
	SetActive(TRUE);

	// on récupère la valeur
	CString strText; GetWindowText(strText);

	// on applique eventuellement un style
	NeedStyle();

	// si le contrôle contient aucune valeur
	if (!strText.IsEmpty())
	{
		// on est en lecture seule
		if (!IsReadOnly())
		{
			double dValue;

			// style
			CString sz;

			// on convertit en double
			dValue = atof(strText);

			// l'utilisateur souhaite incrémenter la valeur
			if (pChild == m_pUpArrow)
			{
				// on incrémente
				if (dValue < m_dMaxValue)
					dValue += 1.0;
			}

			// l'utilisateur souhaite décrémenter la valeur
			else
			{
				// on décremente
				if (dValue > m_dMinValue)
					dValue -= 1.0;
			}

			// on formate la nouvelle valeur en chaine de caractères
			sz.Format(_T("%.*f"), m_NumDecimals, dValue);

			// on modifie le texte du contrôle
			SetWindowText(sz); SetModify(TRUE); OnModifyCell();
			SetSel(0, -1);
		}

	}

	// eventuellement, on rafraichit
	if (!bActive)
		Refresh();
	else
		UpdateEditStyle();

	// on appelle le gestionnaire de base
	CGXControl::OnClickedButton(pChild);
}


void CGXSpinEditCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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
		CGXSpinEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}


////////////////////////////////////////////////////////////
CGXSpinEditCell::CGXSpinEditCell(CGXGridCore* pGrid, UINT nID)
	:	CGXSpinEdit(pGrid, nID)
	,	INumericCell (pGrid)
{

}


////////////////////////////////////////////////////////////
BOOL CGXSpinEditCell::OnEndEditing()
{
	m_pParentGrid->m_pGridWnd->SendMessage(SPINEDITCELL_END_EDIT, 0, 0);
	return TRUE;
}

#endif //!!defined(BUILD_FOR_ATELIER_INTERNET)