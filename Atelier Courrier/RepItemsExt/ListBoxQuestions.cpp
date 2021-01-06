#include "StdAfx.h"
#include ".\ListBoxQuestions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CListBoxQuestions,CListBox)
	//{{AFX_MSG_MAP()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CListBoxQuestions::CListBoxQuestions(void)
{
	m_Rect.left		= 0;
	m_Rect.top		= 0;
	m_Rect.right	= 0;
	m_Rect.bottom	= 0;

	m_ColorTxtSel = ::GetSysColor(COLOR_HIGHLIGHTTEXT);//

	// Font spécifique pour les questions
	m_FontQuestion.CreatePointFont(CATPColors::GetFontSizeS(),CATPColors::GetFontName());  
	
	// Font spécifique pour les blocs
	m_FontBloc.CreatePointFont(CATPColors::GetFontSizeM(),CATPColors::GetFontLabelName());  
		
	// Font items numérisés
	LOGFONT lf;
	m_FontItalique.DeleteObject(); 
	m_FontItalique.CreatePointFont(CATPColors::GetFontSizeM(),CATPColors::GetFontLabelName());
	m_FontItalique.GetLogFont(&lf);
	
	//lf.lfWeight		= FW_BOLD;
	lf.lfItalic		= TRUE;
	lf.lfUnderline	= FALSE;

	m_FontItalique.DeleteObject();
	m_FontItalique.CreateFontIndirect(&lf);

	// Pas de marquage par défaut
	m_MarqueSel = false;

}

CListBoxQuestions::~CListBoxQuestions(void)
{
}

// Positionnement mode marquage
void CListBoxQuestions::SetModeMarquage	(bool MarqueSel)
{
	m_MarqueSel = MarqueSel;
}

// Overrides reset content
void CListBoxQuestions::ResetContent()
{
	CListBox::ResetContent();
	
};


// Initilaisation du map sélection ("marquage")
void CListBoxQuestions::ResetMapSel()
{
	m_MapIdMarque.Reset(); 
}

/// Adds a new item with the object provided
int CListBoxQuestions::AddItem(const IQuestion * pData)
{
	int Pos = AddString((LPCSTR)pData);
	return Pos;
};

/// Adds a new item with the object provided
int CListBoxQuestions::AddItems(JList<IQuestion *> & Data)
{
	for(Data.MoveFirst(); Data.IsValid(); Data.MoveNext())
	{
		AddString((LPCSTR)Data.GetItem());
	}
	return 0;
};

/// Adds a new item with the object provided
int CListBoxQuestions::AddItems(JVector<IQuestion *> & Data)
{
	for(Data.MoveFirst(); Data.IsValid(); Data.MoveNext())
	{
		AddString((LPCSTR)Data.GetItem());
	}
	return 0;
};

/// Removes an item with the ID
bool CListBoxQuestions::RemoveItem(int ID)
{
	int Index = FindItemIndex(ID);

	if(Index != LB_ERR)
	{
		DeleteString(Index);
	}
	return false;
};

JInt32 CListBoxQuestions::GetSelectedID()
{
	int CurSel = GetCurSel();

	if(CurSel != LB_ERR)
	{
		// Single selection listbox with a current selection
		IQuestion * lpItem = (IQuestion *) GetItemData(CurSel);
		return lpItem->GetID();
	}
	else
		return CurSel;
}

bool CListBoxQuestions::SetSelectedID(JInt32 ID)
{
	bool Ret = false;
	int NbSel		= GetSelCount();
	int CurSel		= GetCurSel();

	int Index = FindItemIndex(ID);
	if(Index != LB_ERR)
	{
		SetCurSel(Index);

		// Marquage sélection
		if (m_MarqueSel)
			MarqueSel(ID);

		Ret = true;
	}
	Invalidate();
	UpdateWindow(); 
	return Ret;
}

// Marquage et démarquage des élmts
bool CListBoxQuestions::MarqueSel(JInt32 ID)
{
	m_MapIdMarque.MoveTo(ID);
	if (!m_MapIdMarque.IsValid())
	{
		m_MapIdMarque.Add(ID) = 0;  
		return true;
	}
	return 
		false;
}

bool CListBoxQuestions::DemarqueSel(JInt32 ID)
{
	m_MapIdMarque.MoveTo(ID);
	if (m_MapIdMarque.IsValid())
	{
		m_MapIdMarque.Remove();  
		return true;
	}
	return 
		false;
}	

int	CListBoxQuestions::FindItemIndex(int ID)
{
	int NbItems = GetCount();
	if(NbItems && NbItems != LB_ERR)
	{
		for(int x = 0 ; x < NbItems ; x++)
		{
			IQuestion * lpItem = (IQuestion *) GetItemData(x);

			if (lpItem)
			{
				if(!lpItem->IsBlocQuestion() && lpItem->GetID() == ID)
					return x;
			}
		}
	}
	return LB_ERR;
}


int CListBoxQuestions::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	IQuestion * lpItem1 = (IQuestion *)lpCompareItemStruct->itemData1;
	IQuestion * lpItem2 = (IQuestion *)lpCompareItemStruct->itemData2;

	if(lpItem1->GetLabel() < lpItem2->GetLabel())
		// return -1 = item 1 sorts before item 2
		return -1;
	
	if(lpItem1->GetLabel() > lpItem2->GetLabel())
		// return 1 = item 1 sorts after item 2
		return 1;

	// return 0 = item 1 and item 2 sort the same
	return 0;

};

void CListBoxQuestions::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// If there are no list box items, skip this message. 
	if (lpDrawItemStruct->itemID == -1) 
		return; 

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	IQuestion * lpItem = (IQuestion *) lpDrawItemStruct->itemData;

	if (lpItem)
	{
		CString sLabel = lpItem->GetLabel().AsJCharPtr();
		JInt32 ItemID = lpItem->GetID();

		// Save these values to restore them when done drawing.
		COLORREF crOldTextColor = pDC->GetTextColor();
		COLORREF crOldBkColor = pDC->GetBkColor();

		if(lpItem->IsBlocQuestion())
			// Couleur normal texte
			pDC->SetTextColor(m_ColorBloc);
		else
			// Couleur titre bloc
			pDC->SetTextColor(m_ColorQuestion);

		// Eventuellement couleur pour items numérisés
		CQuestion *pQuestion = lpItem->AsQuestion();
		if (pQuestion)
		{
			if (pQuestion->IsQuantitative())
			{
				// Couleur des items numérisés  / pour le moment même couleur que les items
				pDC->SetTextColor(m_ColorQuestionNum);
			}
		}

		// item selected
		COLORREF colorref;
		if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			pDC->SetTextColor(m_ColorTxtSel);
			colorref = m_ColorSel;
			pDC->SetBkColor(colorref);
		}
		else// item deselected
		{
			// Voir si elmt marqué
			m_MapIdMarque.MoveTo(ItemID);
			if (m_MapIdMarque.IsValid())
			{
				colorref = m_ColorMarqueSel;
				pDC->SetBkColor(colorref);
			}
			else
			{	
				colorref = crOldBkColor;
				pDC->SetBkColor(colorref);
			}
		}

		

		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, colorref);

		RECT Rect = lpDrawItemStruct->rcItem;
		if(lpItem->IsBlocQuestion())
		{
			// Les items questions
			pDC->SelectObject(&m_FontBloc); 
			Rect.left += 5;
		}
		else
		{ 
			// Les items bloc avec ou sans info numérisés
			CQuestion *pQuestion				= lpItem->AsQuestion();
			CPalettes *pPalette					= lpItem->AsPalette();
			CQuestionInduite *pQuestionInduite  = lpItem->AsQuestionInduite(); 

			if (pQuestion)
			{
				if (pQuestion->IsQuantitative())
				{
					// les items numérisés
					pDC->SelectObject(&m_FontItalique); 
				}
				else
				{
					// les items bloc
					pDC	->SelectObject(&m_FontQuestion); 	
				}
			}
			else if (pPalette)
			{
				// les items palettes sans question quantitative
				pDC	->SelectObject(&m_FontQuestion);

				if (pPalette->IsPaletteQuantitative())
				{
				}
			}
			else if (pQuestionInduite)
			{
				// les items questions induites sans question quantitative
				pDC	->SelectObject(&m_FontQuestion); 
			}
			else
			{
				// les items bloc
				pDC	->SelectObject(&m_FontQuestion); 
			}

			// Ne pas dessiner les items indésirables
			Rect.left+=5;

			int			Middle	=	Rect.top + ((Rect.bottom - Rect.top) / 2);
			int			Top		=	Middle-5;
			int			Bottom	=	Middle+5;
			CPen*		pOldPen		= 0;
			CBrush*		pOldBrush	= 0;
			CPen		Pen1;
			CBrush		Brush1;

			// Dessin logo critère modifié
			if(lpItem->IsCritereModifie())
			{
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

			// Dessin logo item construit
			if (lpItem->IsPseudo())
			{
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

			// Dessin logo palette
			if(lpItem->IsPalette())
			{
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

			// Dessin logo quantile
			if(lpItem->IsCritereQuantile())
			{
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

			// Dessin logo question induite
			if(lpItem->IsQuestionInduite())
			{
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

			Rect.left+=15;
		}

		Rect.right-=5;
		pDC->DrawText( sLabel, -1, &Rect, DT_WORDBREAK | DT_NOPREFIX );

		// Reset the background color and the text color back to their
		// original values.
		pDC->SetTextColor(crOldTextColor);
		pDC->SetBkColor(crOldBkColor);
	}
};

void CListBoxQuestions::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	IQuestion * lpItem = (IQuestion *) lpMeasureItemStruct->itemData;

	int nItem = lpMeasureItemStruct->itemID;
	CPaintDC dc(this);

	// ??? Bug 2008, pointeur non testé
	CString sLabel;
	if (lpItem)
	{
		sLabel = lpItem->GetLabel().AsJCharPtr();

		CRect rcLabel;
		GetItemRect(nItem, rcLabel);

		// Make left margin of 5 pixels
		if(lpItem->IsBlocQuestion())
		{
			// les items questions
			dc.SelectObject(&m_FontBloc); 
			rcLabel.left += 5;
		}
		else
		{
			CQuestion *pQuestion = lpItem->AsQuestion();
			if (pQuestion)
			{
				if (pQuestion->IsQuantitative())
				{
					// les items numérisés
					dc.SelectObject(&m_FontQuestion); 
				}	
			}
			else
				// les items appartenant à un bloc de questions
				dc.SelectObject(&m_FontQuestion); 

			rcLabel.left += 15;
		}
	
		// Make right margin 5 pixels
		rcLabel.right-=5;
		// Calculate the required rectangle for the text and set the item height for this 
		// specific item based on the return value (new height).
		lpMeasureItemStruct->itemHeight = dc.DrawText( sLabel, -1, rcLabel, 
												DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );
	}
};

void CListBoxQuestions::OnSize(UINT nType, int cx, int cy)
{
	CListBox::OnSize(nType, cx, cy);
	RECT Rect;
	GetClientRect(&Rect);
	
	// If the width is the same then nothing to do
	if(m_Rect.right == cx)
		return;

	m_Rect = Rect;

	CPaintDC dc(this);
	CRect rcLabel;
	for(int x=0; x  < GetCount(); x++)
	{
		IQuestion * lpItem = (IQuestion *)	GetItemData(x);

		if (lpItem)
		{
			CString sLabel = lpItem->GetLabel().AsJCharPtr();
			GetItemRect(x, rcLabel);

			// Make left margin of 5 pixels
			if(lpItem->IsBlocQuestion())
			{
				// les items questions
				dc.SelectObject(&m_FontBloc); 
				Rect.left+=5;
			}
			else 
			{
				CQuestion *pQuestion = lpItem->AsQuestion();
				if (pQuestion)
				{		
					if (pQuestion->IsQuantitative())
					{
						// les items numérisés
						dc.SelectObject(&m_FontItalique); 
					}
					else
						// les items appartenant à un bloc de questions
						dc.SelectObject(&m_FontQuestion); 
				}
				else
					// les items appartenant à un bloc de questions
					dc.SelectObject(&m_FontQuestion); 
				Rect.left+=15;
			}
			// Make right margin 5 pixels
			rcLabel.right-=5;
			// Calculate the required rectangle for the text and set the item height for this 
			// specific item based on the return value (new height).
			int itemHeight = dc.DrawText( sLabel, -1, rcLabel, 
													DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );
			SetItemHeight(x,itemHeight);
		}
	}
}
////////////////////////////////////////////////////////////////


void CListBoxQuestions::ResizeList()
{
	int cx, cy;
	RECT Rect;
	GetClientRect(&Rect);

	cx = Rect.right;
	cy = Rect.bottom;
	CListBox::OnSize(0, cx, cy);

	m_Rect = Rect;

	CPaintDC dc(this);
	CRect rcLabel;
	for(int x=0; x  < GetCount(); x++)
	{
		IQuestion * lpItem = (IQuestion *)	GetItemData(x);

		if (lpItem)
		{
			CString sLabel = lpItem->GetLabel().AsJCharPtr();
			GetItemRect(x, rcLabel);

			// Make left margin of 5 pixels
			if(lpItem->IsBlocQuestion())
			{
				// les items questions
				dc.SelectObject(&m_FontBloc); 
				Rect.left+=5;
			}
			else 
			{
				CQuestion *pQuestion = lpItem->AsQuestion();
				if (pQuestion)
				{		
					if (pQuestion->IsQuantitative())
					{
						// les items numérisés
						dc.SelectObject(&m_FontItalique); 
					}
					else
						// les items appartenant à un bloc de questions
						dc.SelectObject(&m_FontQuestion); 
				}
				else
					// les items appartenant à un bloc de questions
					dc.SelectObject(&m_FontQuestion); 
				Rect.left+=15;
			}
			// Make right margin 5 pixels
			rcLabel.right-=5;
			// Calculate the required rectangle for the text and set the item height for this 
			// specific item based on the return value (new height).
			int itemHeight = dc.DrawText( sLabel, -1, rcLabel, 
				DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );
			SetItemHeight(x,itemHeight);
		}
	}

	this->RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}

// Selectionne ou deselectionne un item sauf s'il est sur-selectionné
void CListBoxQuestions::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(GetCount() > 0)
	{
		BOOL bOutside;
		UINT idx = ItemFromPoint(point, bOutside);
		if(!bOutside && idx >= 0 && idx < GetCount())
		{
			CRect Rect;
			GetItemRect(idx, &Rect);
			if(Rect.PtInRect(point))
			{
				if(GetCurSel() == idx)
				{
					JInt32 SelID = GetSelectedID();
					SetCurSel(LB_ERR);

					// Demarquage sélection
					if (m_MarqueSel)
						DemarqueSel(SelID);
				}
				else
				{
					IQuestion * lpItem = (IQuestion *)	GetItemData(idx);
					if(!lpItem->IsBlocQuestion())
					{
						SetCurSel(idx);
						JInt32 SelID = GetSelectedID();

						// Marquage sélection
						if (m_MarqueSel)
							MarqueSel(SelID);
					}
				}
				NotifySelChanged();
			}
		}
		SetFocus();
	}
}


//Notify parent que la selection à changée
void CListBoxQuestions::NotifySelChanged()
{
	WPARAM wparam = MAKEWPARAM( GetDlgCtrlID(), LBN_SELCHANGE);
	CWnd* wnd = GetParent();
	if (wnd)
		wnd->PostMessage(WM_COMMAND, wparam, (LPARAM)m_hWnd);
}


//Sur-selection que si on a une liste à sel multiple
void CListBoxQuestions::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnRButtonDown(nFlags, point);
}

//Permet de choisir les couleurs d'affichage des différents types de selection
void CListBoxQuestions::SetSelectionColor(COLORREF color )
{
	m_ColorSel = color;
}

//Permet de choisir les couleurs d'affichage des différents types de marquage
void CListBoxQuestions::SetMarqueColor(COLORREF color )
{
	m_ColorMarqueSel = color;
}

void CListBoxQuestions::SetSelectedTextColor( COLORREF color )
{
	m_ColorTxtSel = color;
}

void CListBoxQuestions::SetQuestionTextColor( COLORREF color )
{
	m_ColorQuestion = color;
}

void CListBoxQuestions::SetBlocTextColor( COLORREF color )
{
	m_ColorBloc = color;
}

void CListBoxQuestions::SetQuestionNumTextColor( COLORREF color )
{
	m_ColorQuestionNum = color;
}