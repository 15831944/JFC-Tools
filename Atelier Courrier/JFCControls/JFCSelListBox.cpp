#include "StdAfx.h"
#include ".\JFCSelListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JFCSelListBox::JFCSelListBox(void)
{
	m_bMouseMove = false;
	m_idxFirst = m_idxPrec = -1;
}

JFCSelListBox::~JFCSelListBox(void)
{
}

BEGIN_MESSAGE_MAP(JFCSelListBox, JFCListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void JFCSelListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bOutside;
	m_idxHaut = m_idxBas = m_idxFirst = ItemFromPoint(point, bOutside);

	if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) && !bOutside  )
	{
		SetSel(m_idxFirst, !GetSel(m_idxFirst) );
		
		m_bFirstSel = false;
		if ( GetSel(m_idxFirst) > 0)
			m_bFirstSel = true;
		
		//deselection calcul des bornes
		if ( m_bFirstSel == false)
		{
			do
				m_idxHaut--;
			while ( m_idxHaut > -1 && GetSel(m_idxHaut) != GetSel(m_idxFirst) );
			do
				m_idxBas++;
			while ( m_idxBas < GetCount() && GetSel(m_idxBas) != GetSel(m_idxFirst) );
		}
		SetFocus();
		m_bMouseMove = TRUE;
		m_idxPrec = m_idxFirst;
		SetCapture();
		//Puisqu'on redéfinie totalement la selection on ne peux pas y faire appel
		//CListBox::OnLButtonDown(nFlags, point);
	}
	else
		CListBox::OnLButtonDown(nFlags, point);

	
}

// Selectionne ou deselectionne un item sauf s'il est sur-selectionné
void JFCSelListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove(nFlags, point);
	if ( m_bMouseMove && GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) )
	{

		BOOL bOutside;
		int idx = ItemFromPoint(point, bOutside);
		
		//scroll	
		if ( bOutside&& idx == GetTopIndex() ) //on remonte
				SetCaretIndex(idx-1);

		if ( idx !=  m_idxPrec )
		{
			if (m_bFirstSel)
			{
				//deselection
				SelectItemRange( false, m_idxFirst, m_idxPrec );
				//selection de la nouvelle zone
				SelectItemRange( true,  m_idxFirst, idx );
			}
			else
			{
				//on deselectionne tout
				SelItemRange( true, m_idxHaut+1, m_idxBas-1 );
				//on est au dessus
				if (idx > m_idxFirst)
					SelItemRange( false,  m_idxFirst , min(idx, m_idxBas) );
				//on est au dessous
				if (idx < m_idxFirst)
					SelItemRange( false, max(idx, m_idxHaut), m_idxFirst );
				//on est revenu au point de départ
				if (idx == m_idxFirst)
					SetSel( m_idxFirst,  false );
			}

			SetCaretIndex(idx);

			//On reselectionne l'item surSelec
			//if (m_IDJFCSelection != -1)
			//	SetSel( FindItemIndex(m_IDJFCSelection), TRUE );
	
			m_idxPrec = idx;

		}
	}

}

void JFCSelListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( m_bMouseMove && GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) )
	{
		ReleaseCapture();
		NotifySelChanged();
		m_bMouseMove = FALSE;
		m_idxFirst = m_idxPrec = -1;
	}
	CListBox::OnLButtonUp(nFlags, point);
}

//Notify parent que la selection à changée
void JFCSelListBox::NotifySelChanged()
{
	WPARAM wparam = MAKEWPARAM( GetDlgCtrlID(), LBN_SELCHANGE);
	CWnd* wnd = GetParent();
	if (wnd)
		wnd->PostMessage(WM_COMMAND, wparam, (LPARAM)m_hWnd);
}

void JFCSelListBox::SelectItemRange(bool bSel, int idxDeb, int idxFin)
{
	if (idxDeb == idxFin)
		SetSel(idxDeb, bSel);
	else
		SelItemRange( bSel, min(idxDeb, idxFin), max(idxDeb, idxFin) );
}

BOOL JFCSelListBox::RecreateListBox(LPVOID lpParam/*=NULL*/)
{
	return RecreateListBox(this, lpParam);
}

BOOL JFCSelListBox::RecreateListBox(CListBox* pList, LPVOID lpParam/*=NULL*/)
{
	if (pList == NULL)
		return FALSE;
	if (pList->GetSafeHwnd() == NULL)
		return FALSE;

	CWnd* pParent = pList->GetParent();
	if (pParent == NULL)
		return FALSE;

	// get current attributes
	DWORD dwStyle = pList->GetStyle();
	DWORD dwStyleEx = pList->GetExStyle();
	CRect rc;
	pList->GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);	// map to client co-ords
	UINT nID = pList->GetDlgCtrlID();
	CFont* pFont = pList->GetFont();
	CWnd* pWndAfter = pList->GetNextWindow(GW_HWNDPREV);

	// create the new list box and copy the old list box items 
	// into a new listbox along with each item's data, and selection state
	JFCSelListBox listNew;
	if (! listNew.CreateEx(dwStyleEx, _T("LISTBOX"), _T(""), dwStyle, 
                                rc, pParent, nID, lpParam))
	  return FALSE;
	listNew.SetFont(pFont);
	int nNumItems = pList->GetCount();
	BOOL bMultiSel = (dwStyle & LBS_MULTIPLESEL || dwStyle & LBS_EXTENDEDSEL);
	for (int n = 0; n < nNumItems; n++)
	{
		CString sText;
		pList->GetText(n, sText);
		int nNewIndex = listNew.AddString(sText);
		listNew.SetItemData(nNewIndex, pList->GetItemData(n));
		if (bMultiSel && pList->GetSel(n))
			listNew.SetSel(nNewIndex);
	}
	if (! bMultiSel&& nNumItems)
	{
		int nCurSel = pList->GetCurSel();
		if (nCurSel != -1)
		{
			CString sSelText;
			// get the selection in the old list
			pList->GetText(nCurSel, sSelText);
			// now find and select it in the new list
			listNew.SetCurSel(listNew.FindStringExact(-1, sSelText));
		}
	}
	// destroy the existing window, then attach the new one
	pList->DestroyWindow();
	HWND hwnd = listNew.Detach();
	pList->Attach(hwnd);

	// position correctly in z-order
	pList->SetWindowPos(pWndAfter == NULL ? &CWnd::wndBottom
                                 : pWndAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}
