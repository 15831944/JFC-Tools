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

	// on active le contr�le
	SetActive(TRUE);

	// on r�cup�re la valeur
	CString strText; GetWindowText(strText);

	// on applique eventuellement un style
	NeedStyle();

	// si le contr�le contient aucune valeur
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

			// l'utilisateur souhaite incr�menter la valeur
			if (pChild == m_pUpArrow)
			{
				// on incr�mente
				if (dValue < m_dMaxValue)
					dValue += 1.0;
			}

			// l'utilisateur souhaite d�cr�menter la valeur
			else
			{
				// on d�cremente
				if (dValue > m_dMinValue)
					dValue -= 1.0;
			}

			// on formate la nouvelle valeur en chaine de caract�res
			sz.Format(_T("%.*f"), m_NumDecimals, dValue);

			// on modifie le texte du contr�le
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
		// test des caract�res num�riques
		if	( (nChar >= '0' && nChar <= '9')	// caract�res num�riques
			||(nChar == '.' || nChar == ',')	// caract�res s�parateurs : virgule
			||(nChar == '-'))					// caract�re '-'
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
		// tests des caract�res sp�ciaux
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