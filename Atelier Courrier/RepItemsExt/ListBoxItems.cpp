#include "StdAfx.h"
#include ".\listboxitems.h"

BEGIN_MESSAGE_MAP(CListBoxItems,JFC_MultiListBox<IItem const *>)
	//{{AFX_MSG_MAP()
		ON_WM_LBUTTONDOWN()
		ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CListBoxItems::CListBoxItems(void)
{
	m_LeftMargin = 15;
}

CListBoxItems::~CListBoxItems(void)
{
}

void CListBoxItems::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// If there are no list box items, skip this message. 
	if (lpDrawItemStruct->itemID == -1) 
		return; 

	CDC* pDC				= CDC::FromHandle(lpDrawItemStruct->hDC);
	IItem const * lpItem	= (IItem const *) lpDrawItemStruct->itemData;
	CString sLabel			= lpItem->GetLabel().AsJCharPtr();
	JInt32 ItemID			= lpItem->GetID();

	// Save these values to restore them when done drawing.
	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor	= pDC->GetBkColor();

	// item selected
	COLORREF colorref;
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		pDC->SetTextColor(m_ColorTxtSel);
		colorref = m_ColorNORMALSel;
		pDC->SetBkColor(colorref);
	}
	else// item deselected
	{
		colorref = crOldBkColor;
		if ( IsAttribue(ItemID) )
			pDC->SetTextColor( m_ColorAttribue );
	}

	//si cet ID est sur-selectionné
	if ( ItemID == m_IDJFCSelection )
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		if( m_JFCSelectionMode == PREGNANCE )
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			colorref = m_ColorPREGNANCESel;
			pDC->SetBkColor(colorref);
		}
		if( m_JFCSelectionMode == OPPOSITION )
		{
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			colorref = m_ColorOPPOSITIONSel;
			pDC->SetBkColor(colorref);
		}
	}

	pDC->FillSolidRect(&lpDrawItemStruct->rcItem, colorref);
	RECT Rect			= lpDrawItemStruct->rcItem;
	Rect.left			+= 5;
	int		Middle		= Rect.top + ((Rect.bottom - Rect.top) / 2);
	int		Top			= Middle-5;
	int		Bottom		= Middle+5;
	CPen*	pOldPen		= 0;
	CBrush*	pOldBrush	= 0;
	CPen	Pen1;
	CBrush	Brush1;

	if(lpItem->IsItemConstruit())
	{
		// Logo item construit devant libellé
		Top		=	Middle-4;
		Bottom	=	Middle+4;
		Pen1.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
		Brush1.CreateSolidBrush(RGB(255,255,0));  
		pOldPen		= pDC->SelectObject(&Pen1);
		pOldBrush   = pDC->SelectObject(&Brush1);

		pDC->BeginPath();

		pDC->MoveTo(Rect.left+2,Bottom);
		pDC->LineTo(Rect.left+2,Top); 
		pDC->LineTo(Rect.left+8,Middle);
		pDC->LineTo(Rect.left+2,Bottom); 
		pDC->EndPath(); pDC->FillPath(); 

		pDC->BeginPath();
		pDC->MoveTo(Rect.left+6,Bottom);
		pDC->LineTo(Rect.left+6,Top); 
		pDC->LineTo(Rect.left,Middle);
		pDC->LineTo(Rect.left+6,Bottom);
		pDC->EndPath(); pDC->FillPath(); 

		pDC->MoveTo(Rect.left+2,Bottom);
		pDC->LineTo(Rect.left+2,Top); 
		pDC->LineTo(Rect.left+8,Middle);
		pDC->LineTo(Rect.left+2,Bottom); 

		pDC->MoveTo(Rect.left+6,Bottom);
		pDC->LineTo(Rect.left+6,Top); 
		pDC->LineTo(Rect.left,Middle);
		pDC->LineTo(Rect.left+6,Bottom);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	if(lpItem->IsCritereModifie())
	{
		// Logo critère modifié devant libellé
		Top		=	Middle-4;
		Bottom	=	Middle+4;
		Pen1.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
		Brush1.CreateSolidBrush(RGB(128,255,128));  
		pOldPen		= pDC->SelectObject(&Pen1);
		pOldBrush   = pDC->SelectObject(&Brush1);

		pDC->BeginPath();
		pDC->MoveTo(Rect.left,Bottom);
		pDC->LineTo(Rect.left+4,Top); 
		pDC->LineTo(Rect.left+8,Bottom);
		pDC->LineTo(Rect.left,Bottom);
		pDC->EndPath(); pDC->FillPath(); 

		pDC->MoveTo(Rect.left,Bottom);
		pDC->LineTo(Rect.left+4,Top); 
		pDC->LineTo(Rect.left+8,Bottom);
		pDC->LineTo(Rect.left,Bottom);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	if(lpItem->IsPalette())
	{
		// Logo palette devant libellé
		Pen1.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
		Brush1.CreateSolidBrush(RGB(255,128,0));  
		pOldPen		= pDC->SelectObject(&Pen1);
		pOldBrush   = pDC->SelectObject(&Brush1);

		pDC->BeginPath();
		pDC->MoveTo(Rect.left+5,Bottom);
		pDC->LineTo(Rect.left,Middle); 
		pDC->LineTo(Rect.left+5,Top);
		pDC->LineTo(Rect.left+10,Middle);
		pDC->LineTo(Rect.left+5,Bottom);
		pDC->EndPath(); pDC->FillPath(); 

		pDC->MoveTo(Rect.left+5,Bottom);
		pDC->LineTo(Rect.left,Middle); 
		pDC->LineTo(Rect.left+5,Top);
		pDC->LineTo(Rect.left+10,Middle);
		pDC->LineTo(Rect.left+5,Bottom);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	if(lpItem->IsCritereQuantile())
	{
		// Logo critère quantile devant libellé
		Top		=	Middle-4;
		Bottom	=	Middle+4;
		Pen1.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
		Brush1.CreateSolidBrush(RGB(0,128,255));  
		pOldPen		= pDC->SelectObject(&Pen1);
		pOldBrush   = pDC->SelectObject(&Brush1);

		pDC->BeginPath();
		pDC->MoveTo(Rect.left,Bottom);
		pDC->LineTo(Rect.left,Top); 
		pDC->LineTo(Rect.left+8,Top);
		pDC->LineTo(Rect.left+8,Bottom);
		pDC->LineTo(Rect.left,Bottom);
		pDC->EndPath(); pDC->FillPath(); 

		pDC->MoveTo(Rect.left,Bottom);
		pDC->LineTo(Rect.left,Top); 
		pDC->LineTo(Rect.left+8,Top);
		pDC->LineTo(Rect.left+8,Bottom);
		pDC->LineTo(Rect.left,Bottom);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	if(lpItem->IsQuestionInduite())
	{
		// Logo question induite devant libellé
		Top		=	Middle-4;
		Bottom	=	Middle+4;
		Pen1.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
		Brush1.CreateSolidBrush(RGB(255,0,0));  
		pOldPen		= pDC->SelectObject(&Pen1);
		pOldBrush   = pDC->SelectObject(&Brush1);

		pDC->BeginPath();
		pDC->Ellipse(Rect.left,Top,Rect.left+8,Bottom);
		pDC->EndPath(); pDC->FillPath(); 

		pDC->Ellipse(Rect.left,Top,Rect.left+8,Bottom);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}

	Rect.left	+=	m_LeftMargin;
	Rect.right	-=	m_RightMargin;

	pDC->DrawText( sLabel, -1, &Rect, DT_WORDBREAK | DT_NOPREFIX );

	// Reset the background color and the text color back to their
	// original values.
	pDC->SetTextColor(crOldTextColor);
	pDC->SetBkColor(crOldBkColor);
};

void CListBoxItems::OnLButtonDown(UINT nFlags, CPoint point)
{
	JFC_MultiListBox<IItem const *>::OnLButtonDown(nFlags, point);
}

void CListBoxItems::OnRButtonDown(UINT nFlags, CPoint point)
{
	BOOL Ok = FALSE;
	BOOL Outside = TRUE;
	INT IdxItem  = this->ItemFromPoint(point, Outside);
	
	if (IdxItem >= 0 && IdxItem < this->GetCount() && Outside == FALSE)
	{
		CRect Rect;
		if (LB_ERR != this->GetItemRect(IdxItem, &Rect))
		{
			Rect.left += 5;
			if (Rect.PtInRect(point))
			{
				if (LB_ERR != this->SetCaretIndex(IdxItem, TRUE))
				{
					Ok = TRUE;
					CDC* pDC = this->GetWindowDC();
					if (pDC != 0)
					{
						CFont* pOldFont = pDC->SelectObject(this->GetFont());
						IItem const* lpItem = (IItem const *)this->GetItemData(IdxItem);
						CSize Size = pDC->GetTextExtent(lpItem->GetLabel().AsJCharPtr());
						Rect.left  = Rect.left + m_LeftMargin - 2;
						Rect.right = Rect.left + Size.cx + 4;
						pDC->DrawFocusRect(&Rect);
						pDC->SelectObject(pOldFont);
						this->ReleaseDC(pDC);
					}
				}
			}
		}
	}
	if (Ok == FALSE)
		this->SetCaretIndex(LB_ERR, FALSE);

	// A VOIR (modif convention)
	// CListBox::OnRButtonDown(nFlags, point);

	// Voir si élmt clické déjà sélectionné
	int Sel;
	Sel = this->GetSel(IdxItem);

	if (Sel == 0) 
	{
		// Si non sélectionné, on lance le process comme si on avait clické sur click gauche 
		// + affichage menu popup
		OnLButtonDown(nFlags, point);
		OnLButtonUp(nFlags,point);
	}
}