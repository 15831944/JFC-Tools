// JFCXListBox.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "JFCXListBox.h"


// CJFCXListBox

IMPLEMENT_DYNAMIC(CJFCXListBox, CXListBox)

CJFCXListBox::CJFCXListBox()
{
	m_idxFirst			= -1;
	m_idxPrec			= -1;
	m_bMouseMove		= false;
}

CJFCXListBox::~CJFCXListBox()
{
}


BEGIN_MESSAGE_MAP(CJFCXListBox, CXListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// Gestionnaires de messages de CJFCXListBox



void CJFCXListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bOutside;
	m_idxHaut = m_idxBas = m_idxFirst = ItemFromPoint(point, bOutside);
	if(!bOutside)
	{
		CRect Rect;
		GetItemRect(m_idxFirst, &Rect);
		if(GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) && Rect.PtInRect(point))
		{
			SetSel(m_idxFirst, !GetSel(m_idxFirst) );

			m_bFirstSel = false;
			if ( GetSel(m_idxFirst) > 0)
				m_bFirstSel = true;

			//deselection calcul des bornes
			if ( m_bFirstSel == false)
			{
				do
				--m_idxHaut;
				while ( m_idxHaut > -1 && GetSel(m_idxHaut) != GetSel(m_idxFirst) );
				do
				++m_idxBas;
				while ( m_idxBas < GetCount() && GetSel(m_idxBas) != GetSel(m_idxFirst) );
			}

			m_bMouseMove = TRUE;
			SetCapture();
		}
		else
		{
			if(Rect.PtInRect(point))
				SetCurSel(m_idxFirst);
		}
	}

	SetFocus();
	m_idxPrec		= m_idxFirst;
	SetCaretIndex(m_idxPrec, 0);
}

void CJFCXListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	NotifySelChanged();
	m_bMouseMove	= FALSE;
	m_idxFirst		= m_idxPrec = -1;
	CListBox::OnLButtonUp(nFlags, point);
}

void CJFCXListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CXListBox::OnMouseMove(nFlags, point);
	if ( m_bMouseMove && GetStyle()&(LBS_MULTIPLESEL|LBS_EXTENDEDSEL) )
	{
		BOOL bOutside = false;
		int idx = ItemFromPoint(point, bOutside);

		//scroll	
		if ( bOutside && idx == GetTopIndex() ) //on remonte
			SetCaretIndex(idx-1, 0);

		//si on a réelement bougé sinon on fait rien
		if ( idx != m_idxPrec )
		{
			SetCaretIndex(idx, 0);
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
			m_idxPrec = idx;
		}
	}
}

// notifie le parent d'un changement de sélection
void CJFCXListBox::NotifySelChanged()
{
	WPARAM wparam	= MAKEWPARAM( GetDlgCtrlID(), LBN_SELCHANGE);
	CWnd* pWnd		= GetParent();
	if (pWnd)
		pWnd->PostMessage(WM_COMMAND, wparam, (LPARAM)m_hWnd);
}