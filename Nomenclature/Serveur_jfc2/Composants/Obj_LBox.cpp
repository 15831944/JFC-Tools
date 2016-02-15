// Obj_LBox.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_LBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_LBox

CObj_LBox::CObj_LBox()
{
	m_Style=0;
	m_ModeSel1=0;			m_ModeSel2=0;
	m_SelectRules=0;
	
	m_ItemSelDebBL=0;		m_ItemSelCurrBL=0;
	m_fInit=0;				m_fSelect=0;		m_fCapture=0;
	m_LastItemSel=0;
	m_ColorSel1=RGB(0, 0, 255);		//bleu
	m_ColorSel2=RGB(255, 0, 0);		//rouge
	m_BkColor=RGB(255, 255, 255);	//blanc
	m_ColorTxt=RGB(0, 0, 0);		//noir
	m_TxtFont=NULL;
	m_brush1=NULL;
	m_brush2=NULL;
	m_brushBk=NULL;
}

CObj_LBox::~CObj_LBox()
{
	if(m_brush1)	delete m_brush1;
	if(m_brush2)	delete m_brush2;
	if(m_brushBk)	delete m_brushBk;
	if(m_TxtFont)	delete m_TxtFont;
}

BEGIN_MESSAGE_MAP(CObj_LBox, CListBox)
	//{{AFX_MSG_MAP(CObj_LBox)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_LBox message handlers
void CObj_LBox::Initialise(short Style, short ModeSel1, short ModeSel2, COLORREF ColorSel1,
				COLORREF ColorSel2, short SelectRules, COLORREF BkColor, CFont * TxtFont,
				COLORREF ColorTxt)
{
	m_Style=Style;
	m_ModeSel1=ModeSel1;
	m_ModeSel2=ModeSel2;
	m_SelectRules=SelectRules;
	if(ColorSel1)	m_ColorSel1=ColorSel1;
	if(ColorSel2)	m_ColorSel2=ColorSel2;
	if(BkColor)		m_BkColor=BkColor;
	if(TxtFont)		m_TxtFont=TxtFont;
	else
	{
		m_TxtFont=new CFont;
		m_TxtFont->CreatePointFont(-80, "MS Sans Serif", NULL);         /* and face name only */
	}
	if(ColorTxt)	m_ColorTxt=ColorTxt;

	m_brush1=new CBrush(m_ColorSel1);
	m_brush2=new CBrush(m_ColorSel2);
	m_brushBk=new CBrush(m_BkColor);
//	m_brush2.CreateSolidBrush(m_ColorSel2);
//	m_brushBk.CreateSolidBrush(m_BkColor);

//	m_brush1.CreateSolidBrush(m_ColorSel1);
//	m_brush2.CreateSolidBrush(m_ColorSel2);
//	m_brushBk.CreateSolidBrush(m_BkColor);

	m_fInit=1;
}

void CObj_LBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType==ODT_LISTBOX);
	if(lpDrawItemStruct->itemAction!=ODA_DRAWENTIRE)return;
	ASSERT(lpDrawItemStruct->itemAction==ODA_DRAWENTIRE);

	if(!m_fInit)
	{
		m_brush1=new CBrush(m_ColorSel1);
		m_brush2=new CBrush(m_ColorSel2);
		m_brushBk=new CBrush(m_BkColor);
		m_fInit=1;
	}
	CDC * dc=CDC::FromHandle(lpDrawItemStruct->hDC);

	COLORREF bgColor, ColorTxt;
	CBrush * Oldbrush;
	CFont * OldFont;
	
	short etat=0;

	dc->SetBkMode(TRANSPARENT);
	OldFont=dc->SelectObject(m_TxtFont);

	CString txt;
	GetText(lpDrawItemStruct->itemID, txt);
	CRect R=lpDrawItemStruct->rcItem;
	CRect RTxt=R;
	RTxt.OffsetRect(2, 0);
//	R.DeflateRect(1, 1, 0, 0);

	if(!this->IsWindowEnabled())
	{
		COLORREF color(RGB(255, 255, 255)), colortxt(RGB(192, 192, 192));
		bgColor=dc->GetBkColor();
		Oldbrush=dc->GetCurrentBrush();
		ColorTxt=dc->GetTextColor();
		dc->SetTextColor(colortxt);

		CBrush brush(color);
		dc->SetBkColor(color);
		dc->SelectObject(&brush);
		dc->FillRect(&R, &brush);
		dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);

		dc->SetBkColor(bgColor);
		dc->SelectObject(Oldbrush);
		dc->SetTextColor(ColorTxt);

		return;
	}

	CRect R1, R2;
	if(m_Style==TRISTATE)
	{
		R1.SetRect(R.left, R.top, R.right, R.top+R.Height()/2);
		R2.SetRect(R.left, R.top+R.Height()/2, R.right, R.bottom);
	}
	switch(lpDrawItemStruct->itemAction)
	{

		case ODA_DRAWENTIRE:
			{
				bgColor=dc->GetBkColor();
				Oldbrush=dc->GetCurrentBrush();
				ColorTxt=dc->GetTextColor();
				dc->SetTextColor(m_ColorTxt);
				switch(GetItemData(lpDrawItemStruct->itemID))
				{	
				case 0:
					dc->SetBkColor(m_BkColor);
					dc->SelectObject(m_brushBk);
					dc->FillRect(&R, m_brushBk);
					dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
					break;
				case 1:
					dc->SetBkColor(m_ColorSel1);
					dc->SelectObject(m_brush1);
					dc->FillRect(&R, m_brush1);
					dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
					break;
				case 2:
					dc->SetBkColor(m_ColorSel2);
					dc->SelectObject(m_brush2);
					dc->FillRect(&R, m_brush2);
					dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
					break;
				case 3:
					dc->SetBkColor(m_ColorSel1);
					dc->SelectObject(m_brush1);
					dc->FillRect(&R1, m_brush1);
					dc->SetBkColor(m_ColorSel2);
					dc->SelectObject(m_brush2);
					dc->FillRect(&R2, m_brush2);
					dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
					break;
				case 4:
					return;
				}
				dc->SetBkColor(bgColor);
				dc->SelectObject(Oldbrush);
				dc->SetTextColor(ColorTxt);
			}
			break;
		case ODA_SELECT:
			{
			}
			break;
		case ODA_FOCUS:
			break;
	}

	if(lpDrawItemStruct->itemState==ODS_SELECTED && lpDrawItemStruct->itemAction!=ODA_DRAWENTIRE)
	{
		bgColor=dc->GetBkColor();
		Oldbrush=dc->GetCurrentBrush();
		ColorTxt=dc->GetTextColor();
		dc->SetTextColor(m_ColorTxt);
		etat=(short)GetItemData(lpDrawItemStruct->itemID);
		switch(etat)
		{	
		case 0:
			dc->SetBkColor(m_BkColor);
			dc->SelectObject(m_brushBk);
			dc->FillRect(&R, m_brushBk);
			dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
			break;
		case 1:
			dc->SetBkColor(m_ColorSel1);
			dc->SelectObject(m_brush1);
			dc->FillRect(&R, m_brush1);
			dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
			break;
		case 2:
			dc->SetBkColor(m_ColorSel2);
			dc->SelectObject(m_brush2);
			dc->FillRect(&R, m_brush2);
			dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
			break;
		case 3:
			dc->SetBkColor(m_ColorSel1);
			dc->SelectObject(m_brush1);
			dc->FillRect(&R1, m_brush1);
			dc->SetBkColor(m_ColorSel2);
			dc->SelectObject(m_brush2);
			dc->FillRect(&R2, m_brush2);
			dc->DrawText(txt, &RTxt, DT_LEFT|DT_VCENTER|DT_NOPREFIX);
			break;
		case 4:
			return;
		}
		dc->SetBkColor(bgColor);
		dc->SelectObject(Oldbrush);
		dc->SetTextColor(ColorTxt);
	}
	dc->SelectObject(OldFont);
	OnItemState(lpDrawItemStruct->itemID, etat);

}

void CObj_LBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bOutside;
	short param;
	short Item;

	switch(m_Style)
	{
	case UNISTATE:
	case DBLSTATE:
	case TRISTATE:
		Item=ItemFromPoint(point, bOutside);
		if(!IsSelectRules(0, Item))	return;
		if(bOutside)	return;
		param=(short)GetItemData(Item);
		m_fSelect=0;
		if(!param || param==2)	m_fSelect=1;
		m_fCapture=1;
		switch(m_ModeSel1)
		{
		case SIMPLE:		//un item selectionnable
			if(nFlags==MK_LBUTTON)
			{
				m_ItemSelCurrBL=Item;
				RAZAllItemState(1);
				SetDataItem(Item, 1, param);
			}
			break;
		case MULTIPLE:		//plusieurs items selectionnables; un à la fois
		case ETENDUE:		//une vague selectionnable + CTRL
			break;
		case ETENDUE_JFC:	//plusieurs vagues selectionnable
			if(nFlags==MK_LBUTTON || nFlags==(MK_LBUTTON|MK_CONTROL))
			{
				SetDataItem(Item, 1, param);
				m_ItemSelDebBL=Item;
				m_ItemSelCurrBL=Item;
			}
			if(nFlags==(MK_LBUTTON|MK_SHIFT) || nFlags==(MK_LBUTTON|MK_SHIFT|MK_CONTROL))
			{
				m_ItemSelCurrBL=Item;
				DWORD param=GetItemData(m_ItemSelDebBL);
				if(param!=2)	SelectItemRange(m_ItemSelDebBL, m_ItemSelCurrBL, param, 0);
			}
			break;
		}
		break;
	}
	InvalidateRect(NULL, 0);
	CListBox::OnLButtonDown(nFlags, point);
}

void CObj_LBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CObj_LBox::OnLButtonDown(nFlags, point);
	//CListBox::OnLButtonDblClk(nFlags, point);
}

void CObj_LBox::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bOutside;
	short param;
	short Item;

	switch(m_Style)
	{
	case DBLSTATE:
	case TRISTATE:
		Item=ItemFromPoint(point, bOutside);
		if(!IsSelectRules(1, Item))	return;
		if(bOutside)	return;
		param=(short)GetItemData(Item);
		m_fSelect=0;
		if(param!=2)	m_fSelect=1;
		m_fCapture=1;
		switch(m_ModeSel2)
		{
			case SIMPLE:		//un item selectionnable
				if(nFlags==MK_RBUTTON)
				{
					m_ItemSelCurrBR=Item;
					RAZAllItemState(2);
					SetDataItem(Item, 2, param);
				}
				break;
			case MULTIPLE:		//plusieurs items selectionnables; un à la fois
			case ETENDUE:		//une vague selectionnable + CTRL
				break;
			case ETENDUE_JFC:	//plusieurs vagues selectionnable
				if(nFlags==MK_RBUTTON || nFlags==(MK_RBUTTON|MK_CONTROL))
				{
					SetDataItem(Item, 2, param);
					m_ItemSelDebBR=Item;
					m_ItemSelCurrBR=Item;
				}
				if(nFlags==(MK_RBUTTON|MK_SHIFT) || nFlags==(MK_RBUTTON|MK_SHIFT|MK_CONTROL))
				{
					m_ItemSelCurrBR=Item;
					DWORD param=GetItemData(m_ItemSelDebBR);
					if(param!=1)	SelectItemRange(m_ItemSelDebBR, m_ItemSelCurrBR, param, 1);
				}
				break;
		}
		break;
	}
	SetCapture();
	InvalidateRect(NULL, 0);
	CListBox::OnRButtonDown(nFlags, point);
}

void CObj_LBox::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnRButtonDown(nFlags, point);
	CListBox::OnRButtonDblClk(nFlags, point);
}

void CObj_LBox::OnItemState(short nItem, short State)
{
}

void CObj_LBox::RAZAllItemState(short etat)
{
	short n=0;
	for(short i=0 ; i<GetCount() ; i++)
	{
		n=(short)GetItemData(i);
		if(n==etat)	SetItemData(i, 0);
		else if(n==3)		SetItemData(i, n>>etat);
	}
}

void CObj_LBox::SelectItemRange(short itemDeb, short itemFin, DWORD param, short fclick)
{
	for(short i=min(itemDeb, itemFin) ; i<=max(itemDeb, itemFin) ; i++)
		if(IsSelectRules(fclick, i))
			SetItemData(i, param);
}

void CObj_LBox::SetDataItem(short item, short etat, short data)
{
	short n=0;
	switch(m_Style)
	{
	case UNISTATE:		//une couleur
		switch(data)
		{	
			case 0:	n=etat;	break;	//selection 
			case 1:	n=0;	break;	//no selection
		}
		break;
	case DBLSTATE:		//deux couleurs; l'une OU l'autre
		if(m_SelectRules==EXCLUSIVE){	
			switch(data)
			{
			case 0:	n=etat;	break;	//selection
			case 1:	n=((etat==2)?etat:0);	break;
			case 2:	n=((etat==2)?0:1);	break;
			}
		}
		else switch(data)
		{	
			case 0:	n=etat;	break;	//selection
			case 1:	n=((etat==2)?etat:0);	break;
			case 2:	n=((etat==1)?etat:1);	break;
		}
		break;
	case TRISTATE:		//deux couleurs; l'une OU/ET l'autre
		switch(data)
		{	
			case 0:	n=etat;	break;	//selection
			case 1:	n=((etat==2)?3:0);	break;
			case 2:	n=((etat==1)?3:0);	break;
			case 3:	n=((etat==2)?2:1);	break;
		}
		break;
	}
	SetItemData(item, n);
}

void CObj_LBox::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bOutside;
	short param;
	short Item;

	if((nFlags & MK_LBUTTON) && m_fCapture)
	{
		Item=ItemFromPoint(point, bOutside);
		if(!IsSelectRules(0, Item))	return;

		switch(m_Style)
		{
		case UNISTATE:
		case DBLSTATE:
		case TRISTATE:
			switch(m_ModeSel1)
			{
			case SIMPLE:
				param=(short)GetItemData(m_ItemSelCurrBL);
				if(Item!=m_ItemSelCurrBL)
				{
					RAZAllItemState(1);
					SetDataItem(Item, 1, param);
				}
				break;
			case MULTIPLE:		//plusieurs items selectionnables; un à la fois
			case ETENDUE:		//une vague selectionnable + CTRL
				break;
			case ETENDUE_JFC:	//plusieurs vagues selectionnable
				if(Item!=m_ItemSelCurrBL)
				{
					for(short i=min(m_ItemSelDebBL, m_ItemSelCurrBL) ; i<=max(m_ItemSelDebBL, m_ItemSelCurrBL) ; i++)
					{
						short n=(short)GetItemData(i);
						switch(n)
						{
						case 0:
						case 2:SetItemData(i, 1);break;
						case 1:SetItemData(i, 0);break;
						}
					}
					if(m_fSelect==1)SelectItemRange(m_ItemSelDebBL, Item, 1, 0);
					else		 SelectItemRange(m_ItemSelDebBL, Item, 0, 0);
					m_ItemSelCurrBL=Item;
				}
				break;
			}
			break;
		}
		InvalidateRect(NULL, 0);
	}
	else
	if((nFlags & MK_RBUTTON) && m_fCapture)
	{
		Item=ItemFromPoint(point, bOutside);
		if(!IsSelectRules(1, Item))	return;

		switch(m_Style)
		{
		case DBLSTATE:
		case TRISTATE:
			switch(m_ModeSel2)
			{
			case SIMPLE:
				param=(short)GetItemData(m_ItemSelCurrBR);
				if(Item!=m_ItemSelCurrBR)
				{
					RAZAllItemState(2);
					SetDataItem(Item, 2, param);
				}
				break;
			case MULTIPLE:		//plusieurs items selectionnables; un à la fois
			case ETENDUE:		//une vague selectionnable + CTRL
				break;
			case ETENDUE_JFC:		//plusieurs items selectionnables
				param=(short)GetItemData(Item);
				if(m_SelectRules!=EXCLUSIVE){
					if(param==0)	return;
				}
				if(Item!=m_ItemSelCurrBR)
				{
					for(short i=min(m_ItemSelDebBR, m_ItemSelCurrBR) ; i<=max(m_ItemSelDebBR, m_ItemSelCurrBR) ; i++)
					{
						short n=(short)GetItemData(i);
						switch(n)
						{
						case 0:break;
						case 1:SetItemData(i, 2);break;
						case 2:SetItemData(i, 1);break;
						}
					}
					if(m_fSelect)SelectItemRange(m_ItemSelDebBR, Item, 2, 1);
					else	{
						if(m_SelectRules==EXCLUSIVE)SelectItemRange(m_ItemSelDebBR, Item, 0, 1);
						else SelectItemRange(m_ItemSelDebBR, Item, 1, 1);
					}
					m_ItemSelCurrBR=Item;
				}
				break;
			}
			break;
		}
		InvalidateRect(NULL, 0);
	}
	CListBox::OnMouseMove(nFlags, point);
}

short CObj_LBox::IsSelection(short etat)
{
	short nb=0;
	for(short i=0 ; i<GetCount() ; i++)
	{
		short n=(short)GetItemData(i);
		if(n==etat)		nb++;
	}
	return nb;
}

void CObj_LBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_fCapture=0;
	CListBox::OnLButtonUp(nFlags, point);
}

void CObj_LBox::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_fCapture=0;
	//message selection à changé
	WPARAM wParam=MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE);
	HWND hwndListBox = CObj_LBox::m_hWnd;
	GetParent()->SendMessage(WM_COMMAND, wParam, (LPARAM)hwndListBox);
	CListBox::OnRButtonUp(nFlags, point);
}

BOOL CObj_LBox::IsSelectRules(short click, short item)
{
	if(item>GetCount() || item<0)	return FALSE;
	switch(click)
	{
	case 0:	//left
		switch(m_SelectRules)
		{
		case INDEPENDANTE:	if(GetItemData(item)==2)	return FALSE;
			return TRUE;
		case CROISEE:		return TRUE;
		case LEFTINTORIGHT:	if(GetItemData(item)==2 || GetItemData(item)==1)	return TRUE;
			return FALSE;
		case RIGHTINTOLEFT:	return TRUE;
		case EXCLUSIVE: return TRUE;
		}
		break;
	case 1:	//right
		switch(m_SelectRules)
		{
		case INDEPENDANTE:	if(GetItemData(item)==1)	return FALSE;
			return TRUE;
		case CROISEE:		return TRUE;
		case LEFTINTORIGHT:	return TRUE;
		case RIGHTINTOLEFT:	if(GetItemData(item)==1 || GetItemData(item)==2)	return TRUE;
			return FALSE;
		}
		case EXCLUSIVE: return TRUE;
		break;
	}
	return FALSE;
}

int CObj_LBox::GetSelCount()
{
	short n=0;
	for(short i=0 ; i<GetCount() ; i++)
		if(GetItemData(i))	n++;
	return n;
}

int CObj_LBox::GetSelItems(int nMaxItems, LPINT rgIndex)
{
	short n=0;
	for(short i=0 ; i<GetCount() ; i++)
	{
		if(GetItemData(i))	rgIndex[n++]=i;
		if(n==nMaxItems)	return n;
	}
	return n;
}

void CObj_LBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight=13;
}


void CObj_LBox::ChangeColorSel1(COLORREF newcolor,bool flagRedraw)
{
	m_ColorSel1=newcolor;
	delete m_brush1;
	m_brush1=new CBrush(m_ColorSel1);
	if(flagRedraw)Invalidate(1);
}

void CObj_LBox::ChangeColorSel2(COLORREF newcolor,bool flagRedraw)
{
	m_ColorSel2=newcolor;
	delete m_brush2;
	m_brush2=new CBrush(m_ColorSel2);
	if(flagRedraw)Invalidate(1);
}

void CObj_LBox::ChangeSelectRules(short newSelectRules)
{
	m_SelectRules=newSelectRules;
}

void CObj_LBox::ChangeMode(short ModeSel1,short ModeSel2,bool flagRedraw)
{
	m_ModeSel1=ModeSel1;
	m_ModeSel2=ModeSel2;

	// On vérifie simplement que si on est en SIMPLE, 
	// il ne doit y avoir qu'une seule ligne sélectionnée
	if(m_ModeSel1==SIMPLE)
	{
		short nb=0;
		for(short i=0 ; i<GetCount() ; i++)
		{
			short n=(short)GetItemData(i);
			if(n==1)nb++;
			if(nb>1)SetItemData(i,0);
		}
	}

	if(m_ModeSel2==SIMPLE)
	{
		short nb=0;
		for(short i=0 ; i<GetCount() ; i++)
		{
			short n=(short)GetItemData(i);
			if(n==2)nb++;
			if(nb>1)SetItemData(i,0);
		}
	}

	// Il faut peut-être redessiner ?
	if(flagRedraw)Invalidate();
}


