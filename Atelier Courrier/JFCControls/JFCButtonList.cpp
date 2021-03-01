// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JFCButtonList.h"
#include "BtnST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(JFCButtonList, JFControl)
	//{{AFX_MSG_MAP(JFCButtonList)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCButtonList::JFCButtonList()
{
	// on initialise les bordures
	m_HorzBorderSize = m_VertBorderSize = 0;

	// on initialise les ascenseurs
	m_HorzMaster  = m_VertMaster  = FALSE;
	m_pHorzScroll = m_pVertScroll = NULL;

	// on initialise les �l�ments
	m_HorzFirst = m_HorzLast = m_HorzSize = 0;
	m_VertFirst = m_VertLast = m_VertSize = 0;

	m_bMouseOnButton = FALSE;
	m_BtnI = m_BtnJ = -1;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour d�finir les ascenseurs externes
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::SetHorzScroll(JFCScroll* pScroll, BOOL Master, BOOL update)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(m_hWnd));

	// on r�initialise l'ascenseur courant
	if ((m_HorzMaster != FALSE) && (m_pHorzScroll != NULL))	m_pHorzScroll->SetRange(0, 0, 0);

	// on m�morise l'indicateur de ma�tre
	m_HorzMaster = ((Master != FALSE) && (pScroll != NULL)) ? TRUE : FALSE;

	// on m�morise l'ascenseur
	m_pHorzScroll = pScroll;

	// on met � jour les ascenseurs
	if (update != FALSE) OnSetScrollBar(TRUE);
}

void JFCButtonList::SetVertScroll(JFCScroll* pScroll, BOOL Master, BOOL update)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(m_hWnd));

	// on r�initialise l'ascenseur courant
	if ((m_VertMaster != FALSE) && (m_pVertScroll != NULL)) m_pVertScroll->SetRange(0, 0, 0);

	// on m�morise l'indicateur de ma�tre
	m_VertMaster = ((Master != FALSE) && (pScroll != NULL)) ? TRUE : FALSE;

	// on m�morise l'ascenseur
	m_pVertScroll = pScroll;

	// on met � jour les ascenseurs
	if (update != FALSE) OnSetScrollBar(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour r�cup�rer les ascenseurs externes
/////////////////////////////////////////////////////////////////////////////

JFCScroll* JFCButtonList::GetHorzScroll()
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(m_hWnd));

	// on renvoie l'ascenseur externe
	return (m_pHorzScroll);
}

JFCScroll* JFCButtonList::GetVertScroll()
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(m_hWnd));

	// on renvoie l'ascenseur externe
	return (m_pVertScroll);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCButtonList::~JFCButtonList()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour d�finir la taille des �l�ments
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::SetHorzItemSize(LONG size, LONG Interstice, LONG border, BOOL update)
{
	// on m�morise la taille des �l�ments
	m_HorzSize = (size > 0) ? size : 0;
	//m_HorzSize += Interstice;
	m_HorzInterstice = Interstice;
	// on m�morise l'indicateur des bordures
	m_HorzBorderSize = border;

	// on met � jour les ascenseurs
	if (update != FALSE) OnSetScrollBar(TRUE);
}

void JFCButtonList::SetVertItemSize(LONG size, LONG Interstice, LONG border, BOOL update)
{
	// on m�morise la taille des �l�ments
	m_VertSize = (size > 0) ? size : 0;
	//m_VertSize += Interstice;
	m_VertInterstice = Interstice;

	// on m�morise l'indicateur des bordures
	m_VertBorderSize = border;

	// on met � jour les ascenseurs
	if (update != FALSE) OnSetScrollBar(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour d�finir l'intervalle des �l�ments
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::SetHorzItemRange(LONG first, LONG last, BOOL update)
{
	// on m�morise l'intervalle
	m_HorzFirst = (first <= last) ? first : last;
	m_HorzLast  = (first >= last) ? first : last;

	// on met � jour les ascenseurs
	if (update != FALSE) OnSetScrollBar(TRUE);
}

void JFCButtonList::SetVertItemRange(LONG first, LONG last, BOOL update)
{
	// on m�morise l'intervalle
	m_VertFirst = (first <= last) ? first : last;
	m_VertLast  = (first >= last) ? first : last;

	// on met � jour les ascenseurs
	if (update != FALSE) OnSetScrollBar(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour rendre visible un �l�ment
/////////////////////////////////////////////////////////////////////////////

bool JFCButtonList::EnsureVisible(LONG i, LONG j)
{
	CRect rect;

	// on r�cup�re la zone cliente de la fen�tre
	GetClientRect(&rect);

	// on v�rifie la validit� des indices
	if ((i < m_HorzFirst) || (i > m_HorzLast)) return (false);
	if ((j < m_VertFirst) || (j > m_VertLast)) return (false);

	// on r�cup�re la taille de la fen�tre
	LONG hwnd = rect.Width();
	LONG vwnd = rect.Height();

	// on r�cup�re la taille r�elle des �l�ments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > m_HorzBorderSize) ? (hwnd - (2 * m_HorzBorderSize)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > m_VertBorderSize) ? (vwnd - (2 * m_VertBorderSize)) : 0;

	// on r�cup�re la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : OnGetVertPosition();

	// on r�cup�re le nombre d'�l�ments visibles
	LONG hdta = ((hwnd > m_HorzBorderSize) && (hsize > 0)) ? ((hwnd - (2 * m_HorzBorderSize)) / hsize) : 0;
	LONG vdta = ((vwnd > m_VertBorderSize) && (vsize > 0)) ? ((vwnd - (2 * m_VertBorderSize)) / vsize) : 0;

	// on calcule les nouvelles positions des ascenseurs
	LONG nhps = (hdta > 0) ? (((hpos + hdta) <= i) ? (i - hdta + 1) : (hpos <= i) ? hpos : i) : i;
	LONG nvps = (vdta > 0) ? (((vpos + vdta) <= j) ? (j - vdta + 1) : (vpos <= j) ? vpos : j) : j;

	// on modifie les positions d'affichage
	if (OnSetHorzPosition(nhps)) OnHorzScrollPosition(); else return (false);
	if (OnSetVertPosition(nvps)) OnVertScrollPosition(); else return (false);

	// on renvoie l'indicateur de r�ussite
	return (true);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour convertir les indices et les positions
/////////////////////////////////////////////////////////////////////////////

bool JFCButtonList::GetIdxAtPos(LONG x, LONG y, LONG & i, LONG & j)
{
	CRect rect;

	// on r�cup�re la zone cliente de la fen�tre
	GetClientRect(&rect);

	// on v�rifie la validit� de la position
	if ((x < rect.left) || ((x + 1) > rect.right )) return (false);
	if ((y < rect.top ) || ((y + 1) > rect.bottom)) return (false);

	// on r�cup�re la taille de la fen�tre
	LONG hwnd = rect.Width();
	LONG vwnd = rect.Height();

	// on r�cup�re la taille r�elle des �l�ments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > m_HorzBorderSize) ? (hwnd - (2 * m_HorzBorderSize)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > m_VertBorderSize) ? (vwnd - (2 * m_VertBorderSize)) : 0;

	// on r�cup�re la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : OnGetVertPosition();

	// on calcule les indices de l'�l�ment
	LONG hidx = hpos + (((x > m_HorzBorderSize) && (hsize > 0)) ? ((x - m_HorzBorderSize) / hsize) : (0));
	LONG vidx = vpos + (((y > m_VertBorderSize) && (vsize > 0)) ? ((y - m_VertBorderSize) / vsize) : (0));

	// cas particulier : l'�l�ment est cach� par la bordure droite ou basse
	if (((x + m_HorzBorderSize + 1) > rect.right ) && (x == ((hidx - hpos) * hsize + m_HorzBorderSize))) return (false);
	if (((y + m_VertBorderSize + 1) > rect.bottom) && (y == ((vidx - vpos) * vsize + m_VertBorderSize))) return (false);

	// on v�rifie la validit� des indices
	if ((hidx < m_HorzFirst) || (hidx > m_HorzLast)) return (false);
	if ((vidx < m_VertFirst) || (vidx > m_VertLast)) return (false);

	// on renvoie les indices
	i = hidx;
	j = vidx;

	// on renvoie l'indicateur de r�ussite
	return (true);
}

bool JFCButtonList::GetPosAtIdx(LONG i, LONG j, LONG & x1, LONG & y1, LONG & x2, LONG & y2)
{
	CRect rect;

	// on r�cup�re la zone cliente de la fen�tre
	GetClientRect(&rect);

	// on v�rifie la validit� des indices
	if ((i < m_HorzFirst) || (i > m_HorzLast)) return (false);
	if ((j < m_VertFirst) || (j > m_VertLast)) return (false);

	// on r�cup�re la taille de la fen�tre
	LONG hwnd = rect.Width();
	LONG vwnd = rect.Height();

	// on r�cup�re la taille r�elle des �l�ments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > m_HorzBorderSize) ? (hwnd - (2 * m_HorzBorderSize)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > m_VertBorderSize) ? (vwnd - (2 * m_VertBorderSize)) : 0;

	// on r�cup�re la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : OnGetVertPosition();

	// on calcule la position de l'�l�ment
	LONG hpos1 = (i - hpos) * hsize + m_HorzBorderSize; LONG hpos2 = hpos1 + hsize - 1;
	LONG vpos1 = (j - vpos) * vsize + m_VertBorderSize; LONG vpos2 = vpos1 + vsize - 1;

	// on v�rifie la validit� des positions
	if ((hpos1 > hpos2) || ((hpos1 + 1) > rect.right )) return (false);
	if ((vpos1 > vpos2) || ((vpos1 + 1) > rect.bottom)) return (false);

	// on renvoie la position
	x1 = hpos1; y1 = vpos1;
	x2 = hpos2; y2 = vpos2;

	// on renvoie l'indicateur de r�ussite
	return (true);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour convertir les positions en indices de tracking
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::GetTrackIdx(LONG x, LONG y, LONG & i,  LONG & j, BOOL scroll)
{
	CRect rect;

	// on r�cup�re la zone cliente de la fen�tre
	GetClientRect(&rect);

	// on ram�ne la position � l'int�rieur de la zone cliente
	if ((x)     < rect.left  ) { x = rect.left;       if (scroll != FALSE) OnHorzScrollItems(FALSE); }
	if ((y)     < rect.top   ) { y = rect.top;        if (scroll != FALSE) OnVertScrollItems(FALSE); }
	if ((x + 1) > rect.right ) { x = rect.right  - 1; if (scroll != FALSE) OnHorzScrollItems(TRUE ); }
	if ((y + 1) > rect.bottom) { y = rect.bottom - 1; if (scroll != FALSE) OnVertScrollItems(TRUE ); }

	// on r�cup�re la taille de la fen�tre
	LONG hwnd = rect.Width();
	LONG vwnd = rect.Height();

	// on r�cup�re la taille r�elle des �l�ments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > m_HorzBorderSize) ? (hwnd - (2 * m_HorzBorderSize)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > m_VertBorderSize) ? (vwnd - (2 * m_VertBorderSize)) : 0;

	// on r�cup�re la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : OnGetVertPosition();

	// on calcule les indices de l'�l�ment
	LONG hidx = hpos + (((x > m_HorzBorderSize) && (hsize > 0)) ? ((x - m_HorzBorderSize) / hsize) : (0));
	LONG vidx = vpos + (((y > m_VertBorderSize) && (vsize > 0)) ? ((y - m_VertBorderSize) / vsize) : (0));

	// on renvoie les indices
	i = (hidx < m_HorzFirst) ? m_HorzFirst : (hidx > m_HorzLast) ? m_HorzLast : hidx;
	j = (vidx < m_VertFirst) ? m_VertFirst : (vidx > m_VertLast) ? m_VertLast : vidx;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer l'�tat des �l�ments
/////////////////////////////////////////////////////////////////////////////
void JFCButtonList::OnGetItemParams(LONG i, LONG j, BtnParams & Params)
{
	// on renvoie l'�tat de l'�l�ment
	return ;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool JFCButtonList::OnBeginDrawing(CDC & dc)
{
	return (false);
}

void JFCButtonList::OnDrawParam(CDC & dc, CRect rect, LONG i, LONG j, BtnParams & Params)
{
	CPen	Pen1;
	CBrush	Brush1;
	CPen	Pen2;
	CBrush	Brush2;
	CRect	RectText(rect);

	// on s�lectionne le stylo B et le pinceau A
	CPen*		pOldPen		= 0;
	CBrush*		pOldBrush	= 0;

	Pen1.CreatePen(PS_SOLID, 1, Params.m_Color_Border); 
	pOldPen   = dc.SelectObject(&Pen1);



	// Draw Button
	//if(Params.m_State&BUT_DOWN)
	//{
	//	Brush1.CreateSolidBrush(Params.m_Color_BtnDown);  
	//	pOldBrush = dc.SelectObject(&Brush1);
	//}
	//else
	//{
	//	Brush1.CreateSolidBrush(Params.m_Color_BtnUp);  
	//	pOldBrush = dc.SelectObject(&Brush1);
	//}
	if(Params.m_State&BUT_DOWN)
		Brush1.CreateSolidBrush(Params.m_Color_BtnDown);  
	else
	{
		if(m_bMouseOnButton && m_BtnI == i && m_BtnJ == j)
			Brush1.CreateSolidBrush(Params.m_Color_BtnHover);  
		else
			Brush1.CreateSolidBrush(Params.m_Color_BtnUp);  
	}
	pOldBrush = dc.SelectObject(&Brush1);

	dc.BeginPath();
	dc.MoveTo(rect.TopLeft());		dc.LineTo(rect.right,rect.top);
	dc.LineTo(rect.BottomRight());	dc.LineTo(rect.left,rect.bottom);
	dc.LineTo(rect.TopLeft());
	dc.EndPath(); dc.FillPath();

	// Draw Frame
	dc.MoveTo(rect.TopLeft());		dc.LineTo(rect.right,rect.top);
	dc.LineTo(rect.BottomRight());	dc.LineTo(rect.left,rect.bottom);
	dc.LineTo(rect.TopLeft());

	// on restaure le pinceau et le stylo
	dc.SelectObject(pOldPen);
	Pen1.DeleteObject(); 




	// red�finit couleur bordure LED
	//if(Params.m_Style&(LED_NO_SEL|LED_SEL))
	//{
	//	if(Params.m_State&LED_ON)
	//	{
	//		// avant si contour diff�rent / en blanc par exemple
	//		Pen1.CreatePen(PS_SOLID, 1, RGB(255,255,255)); 
	//		// Pen1.CreatePen(PS_SOLID, 1, Params.m_Color_Border); 
	//		pOldPen   = dc.SelectObject(&Pen1);
	//	}
	//	else
	//	{
	//		// avant si contour diff�rent / en blanc par exemple
	//		Pen1.CreatePen(PS_SOLID, 1, Params.m_Color_Border); 
	//		pOldPen   = dc.SelectObject(&Pen1);
	//	}
	//}
	//else
	//{
	//	Pen1.CreatePen(PS_SOLID, 1, Params.m_Color_Border); 
	//	pOldPen   = dc.SelectObject(&Pen1);
	//}

	Pen1.CreatePen(PS_SOLID , 1, Params.m_Color_LedBorder); 
	pOldPen   = dc.SelectObject(&Pen1);


	// Draw Led
	if(Params.m_Style&(LED_NO_SEL|LED_SEL))
	{
		if(Params.m_State&LED_ON)
			Brush2.CreateSolidBrush(Params.m_Color_LedOn);
		else
			Brush2.CreateSolidBrush(Params.m_Color_LedOff);
		dc.SelectObject(&Brush2);

		// Calcul zone dessin led
		CRect RectLed;
		if(rect.Width()>rect.Height())
		{
			RectLed = CRect(rect.left,rect.top,rect.left+rect.Height(),rect.bottom);
		}
		else
		{
			RectLed = CRect(rect.left,rect.top,rect.left+rect.Width()/4,rect.bottom);
		}
		RectText.left = RectLed.right;

		// Retrait pour dessin led
		RectLed.DeflateRect(5,5);
		RectLed.NormalizeRect();
		if (Params.m_Design_Led == 0)
		{
			//// dessin led (forme carr�)
			dc.BeginPath();
			dc.MoveTo(RectLed.TopLeft());		dc.LineTo(RectLed.right,RectLed.top);
			dc.LineTo(RectLed.BottomRight());	dc.LineTo(RectLed.left,RectLed.bottom);
			dc.LineTo(RectLed.TopLeft());
			dc.EndPath(); dc.FillPath();

			// le cadre carr�
			dc.MoveTo(RectLed.TopLeft());		dc.LineTo(RectLed.right,RectLed.top);
			dc.LineTo(RectLed.BottomRight());	dc.LineTo(RectLed.left,RectLed.bottom);
			dc.LineTo(RectLed.TopLeft());

		}
		else if (Params.m_Design_Led == 1)	
		{	
			RectLed = CRect(rect.left,rect.top,rect.right,rect.bottom);
			RectLed.DeflateRect(5,5);
			// dessin led triangle vers le haut
			dc.BeginPath();
			dc.MoveTo(RectLed.left + (RectLed.right-RectLed.left)/2,RectLed.top);
			dc.LineTo(RectLed.BottomRight());	dc.LineTo(RectLed.left,RectLed.bottom);
			dc.LineTo(RectLed.left + (RectLed.right-RectLed.left)/2,RectLed.top);
			dc.EndPath(); dc.FillPath();

			// le cadre triangulaire
			dc.MoveTo(RectLed.left + (RectLed.right-RectLed.left)/2,RectLed.top);
			dc.LineTo(RectLed.BottomRight());	dc.LineTo(RectLed.left,RectLed.bottom);
			dc.LineTo(RectLed.left + (RectLed.right-RectLed.left)/2,RectLed.top);
		}

		else if (Params.m_Design_Led == 2)
		{
			RectLed = CRect(rect.left,rect.top,rect.right,rect.bottom);
			RectLed.DeflateRect(5,5);
			// dessin led triangle vers le bas
			dc.BeginPath();
			dc.MoveTo(RectLed.TopLeft());		dc.LineTo(RectLed.right,RectLed.top);
			dc.LineTo(RectLed.left + (RectLed.right-RectLed.left)/2,RectLed.bottom);
			dc.LineTo(RectLed.TopLeft());
			dc.EndPath(); dc.FillPath();

			// le cadre trinagle
			dc.MoveTo(RectLed.TopLeft());		dc.LineTo(RectLed.right,RectLed.top);
			dc.LineTo(RectLed.left + (RectLed.right-RectLed.left)/2,RectLed.bottom);
			dc.LineTo(RectLed.TopLeft());
		}
		else if (Params.m_Design_Led == 3)
		{
			//// dessin led (forme ronde)
			RectLed.InflateRect(1,1);
			dc.Ellipse(RectLed);

		}
		else if (Params.m_Design_Led == 4)
		{
			//// dessin led (avec icone)
			RectLed.InflateRect(3,3);
			HICON hIcon = NULL;
			HINSTANCE	hInstResource	= NULL;

			if(Params.m_State&LED_ON)
			{
				hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(m_iconOn), RT_GROUP_ICON);
				hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(m_iconOn), IMAGE_ICON, 0, 0, 0);
			}
			else
			{
				hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(m_iconOff), RT_GROUP_ICON);
				hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(m_iconOff), IMAGE_ICON, 0, 0, 0);
			}

			//dc.DrawIcon(RectLed.TopLeft(), hIcon);
			dc.DrawState(	RectLed.TopLeft(),
							RectLed.Size(), 
							hIcon,
							DSS_NORMAL, 
							(CBrush*)NULL);


		}
	}

	// Draw the text s'il existe
	if (Params.m_Text != "")
	{
		int		OldBkMode	= dc.SetBkMode(TRANSPARENT); 
		CFont*	pOldFont	= dc.SelectObject(&Params.m_Text_Font);

		COLORREF oldColTxt;
		oldColTxt = dc.SetTextColor(Params.m_Color_Text);
		if(Params.m_State&BUT_DOWN)
			oldColTxt = dc.SetTextColor(Params.m_Color_Text);
		else
		{
			if(m_bMouseOnButton && m_BtnI == i && m_BtnJ == j)
				oldColTxt = dc.SetTextColor(::GetSysColor(COLOR_BTNHIGHLIGHT));
		}

		dc.SelectObject(&Pen2);
		CRect rct;
		rct = RectText;
		dc.DrawText(Params.m_Text,&rct,DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE | DT_CALCRECT);
		if( rct.Width() > RectText.Width())
			RectText.DeflateRect(5,0);

		dc.DrawText(Params.m_Text,&RectText,DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE | DT_END_ELLIPSIS);

		// on restaure le pinceau et le stylo
		dc.SetBkMode(OldBkMode); 
		dc.SelectObject(pOldFont);
		dc.SetTextColor(oldColTxt);
	}
	
	// on restaure le pinceau et le stylo
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	
}

void JFCButtonList::SetIcons(int iconOn, int iconOff)
{
	m_iconOn = iconOn;
	m_iconOff = iconOff;
}

void JFCButtonList::OnEndDrawing(CDC & dc)
{
	// on ne fait rien
}

////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les ascenseurs actifs
////////////////////////////////////////////////////////////////////////////

void JFCButtonList::OnSetScrollBar(BOOL update)
{
	CRect rect;

	// on met � jour l'affichage des ascenseurs
	ShowScrollBar(SB_HORZ, FALSE);
	ShowScrollBar(SB_VERT, FALSE);

	// on r�cup�re la zone cliente de la fen�tre
	GetClientRect(&rect);

	// on teste si on doit configurer l'ascenseur horizontal
	if ((m_pHorzScroll != NULL) && (m_HorzMaster != FALSE))
	{
		// on r�cup�re la largeur de la fen�tre
		LONG wnd = rect.Width();

		// on r�cup�re la largeur r�elle des �l�ments
		LONG size = (m_HorzSize > 0) ? m_HorzSize : (wnd > m_HorzBorderSize) ? (wnd - (2 * m_HorzBorderSize)) : 0;

		// on r�cup�re la taille de la page
		LONG page = ((size > 0) && (size < wnd)) ? (wnd / size) : 0;

		// on configure l'ascenseur externe
		m_pHorzScroll->SetRange(m_HorzFirst, m_HorzLast, (page > 0) ? page : 1);
	}

	// on teste si on doit configurer l'ascenseur vertical
	if ((m_pVertScroll != NULL) && (m_VertMaster != FALSE))
	{
		// on r�cup�re la hauteur de la fen�tre
		LONG wnd = rect.Height();

		// on r�cup�re la hauteur r�elle des �l�ments
		LONG size = (m_VertSize > 0) ? m_VertSize : (wnd > m_VertBorderSize) ? (wnd - (2 * m_VertBorderSize)) : 0;

		// on calcule la taille de la page
		LONG page = ((size > 0) && (size < wnd)) ? (wnd / size) : 0;

		// on configure l'ascenseur externe
		m_pVertScroll->SetRange(m_VertFirst, m_VertLast, (page > 0) ? page : 1);
	}

	// on met � jour la fen�tre
	if (update != FALSE) InvalidateRect(NULL, FALSE);
}

////////////////////////////////////////////////////////////////////////////
// les fonctions pour modifier la position des ascenseurs actifs
////////////////////////////////////////////////////////////////////////////

bool JFCButtonList::OnSetHorzPosition(LONG position)
{
	// on v�rifie la validit� et la maitrise de l'ascenseur
	if ((m_pHorzScroll == NULL) || (m_HorzMaster == FALSE)) return (false);

	// on modifie la position de l'ascenseur
	m_pHorzScroll->SetPosition(position);

	// on renvoie l'indicateur de r�ussite
	return (true);
}

bool JFCButtonList::OnSetVertPosition(LONG position)
{
	// on v�rifie la validit� et la maitrise de l'ascenseur
	if ((m_pVertScroll == NULL) || (m_VertMaster == FALSE)) return (false);

	// on modifie la position de l'ascenseur
	m_pVertScroll->SetPosition(position);

	// on renvoie l'indicateur de r�ussite
	return (true);
}

////////////////////////////////////////////////////////////////////////////
// les fonctions pour r�cup�rer la position des ascenseurs actifs
////////////////////////////////////////////////////////////////////////////

LONG JFCButtonList::OnGetHorzPosition()
{
	// on initialise la position
	LONG position = m_HorzFirst;

	// on r�cup�re la position de l'ascenseur
	if (m_pHorzScroll != NULL) position = m_pHorzScroll->GetPosition();

	// on renvoie la position
	return (position);
}

LONG JFCButtonList::OnGetVertPosition()
{
	// on initialise la position
	LONG position = m_VertFirst;

	// on r�cup�re la position de l'ascenseur
	if (m_pVertScroll != NULL) position = m_pVertScroll->GetPosition();

	// on renvoie la position
	return (position);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour faire d�filer l'affichage
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::OnHorzScrollItems(BOOL forward)
{
	// on v�rifie la validit� de l'ascenceur
	if (m_pHorzScroll != NULL)
	{
		// on r�cup�re la destination du message
		CWnd* pDest = GetParent();

		// on teste la validit� du destinataire
		if (pDest != NULL)
		{
			// on r�cup�re la source du message
			HWND hSrc = m_pHorzScroll->m_hWnd;

			// on r�cup�re le param�tre du message
			LONG Param = MAKELONG((forward != FALSE) ? SB_LINERIGHT : SB_LINELEFT, 0);

			// on envoie le message
			pDest->SendMessage(WM_HSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

void JFCButtonList::OnVertScrollItems(BOOL forward)
{
	// on v�rifie la validit� de l'ascenceur
	if (m_pVertScroll != NULL)
	{
		// on r�cup�re la destination du message
		CWnd* pDest = GetParent();

		// on teste la validit� du destinataire
		if (pDest != NULL)
		{
			// on r�cup�re la source du message
			HWND hSrc = m_pVertScroll->m_hWnd;

			// on r�cup�re le param�tre du message
			LONG Param = MAKELONG((forward != FALSE) ? SB_LINEDOWN : SB_LINEUP, 0);

			// on envoie le message
			pDest->SendMessage(WM_VSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour actualiser la position d'affichage
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::OnHorzScrollPosition()
{
	// on v�rifie la validit� de l'ascenceur
	if (m_pHorzScroll != NULL)
	{
		// on r�cup�re la destination du message
		CWnd* pDest = GetParent();

		// on teste la validit� du destinataire
		if (pDest != NULL)
		{
			// on r�cup�re la source du message
			HWND hSrc = m_pHorzScroll->m_hWnd;

			// on r�cup�re le param�tre du message
			LONG Param = MAKELONG(SB_ENDSCROLL, 0);

			// on envoie le message
			pDest->SendMessage(WM_HSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

void JFCButtonList::OnVertScrollPosition()
{
	// on v�rifie la validit� de l'ascenceur
	if (m_pVertScroll != NULL)
	{
		// on r�cup�re la destination du message
		CWnd* pDest = GetParent();

		// on teste la validit� du destinataire
		if (pDest != NULL)
		{
			// on r�cup�re la source du message
			HWND hSrc = m_pVertScroll->m_hWnd;

			// on r�cup�re le param�tre du message
			LONG Param = MAKELONG(SB_ENDSCROLL, 0);

			// on envoie le message
			pDest->SendMessage(WM_VSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les cellules des �l�ments
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::OnDrawParams(CDC & dc, LONG x, LONG y, LONG cx, LONG cy, LONG i, LONG j, LONG ci, LONG cj)
{
	CRect rect;

	// on initialise les coordonn�es verticales
	rect.top = y; rect.bottom = y + cy;

	// on parcourt tous les �l�ments verticaux � dessiner
	for (LONG jdx = 0; jdx < cj; jdx += 1)
	{
		// on initialise les coordonn�es horizontales
		rect.left = x; rect.right = x + cx;

		// on parcourt tous les �l�ments horizontaux � dessiner
		for (LONG idx = 0; idx < ci; idx += 1)
		{
			// on r�cup�re l'�tat de l'�l�ment
			BtnParams Params;
			OnGetItemParams(i + idx, j + jdx, Params);

			// on dessine l'�l�ment courant
			rect.DeflateRect(m_HorzInterstice,m_VertInterstice,m_HorzInterstice,m_VertInterstice);
			OnDrawParam(dc, rect, i + idx, j + jdx, Params);
			rect.InflateRect(m_HorzInterstice,m_VertInterstice,m_HorzInterstice,m_VertInterstice);

			// on met � jour les coordonn�es horizontales
			rect.left += cx; rect.right += cx;
		}

		// on met � jour les coordonn�es verticales
		rect.top += cy; rect.bottom += cy;
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner la fen�tre
/////////////////////////////////////////////////////////////////////////////

void JFCButtonList::OnDrawWindow(CDC & dc, LONG x, LONG y, LONG dx, LONG dy)
{


	CBrush	Brush1(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));

	CRect Rect;

	// on r�cup�re la zone cliente
	GetClientRect(&Rect);
	// on s�lectionne le brush pour le fond
	CBrush*		pOldBrush	= dc.SelectObject(&Brush1);
	dc.FillRect(&Rect,&Brush1);
	dc.SelectObject(pOldBrush);

	// on teste si on a quelque chose � dessiner
	if ((dx > 0) && (dy > 0))
	{
		LONG i1, j1, i2, j2;
		LONG x1, y1, x2, y2;

		// on r�cup�re les indices des �l�ments � dessiner
		GetTrackIdx(x,          y,          i1, j1, FALSE);
		GetTrackIdx(x + dx - 1, y + dy - 1, i2, j2, FALSE);

		// on r�cup�re la position du premier �l�ment
		if (GetPosAtIdx(i1, j1, x1, y1, x2, y2))
		{
			// on calcule la position
			LONG x3 = x1 - x;
			LONG y3 = y1 - y;

			// on calcule le nombre de cellules
			LONG ci = i2 - i1 + 1;
			LONG cj = j2 - j1 + 1;

			// on calcule la taille des cellules
			LONG cx = x2 - x1 + 1;
			LONG cy = y2 - y1 + 1;

			// on pr�pare le dessin des �l�ments
			if (OnBeginDrawing(dc))
			{
				// on dessine le fond des �l�ments
				OnDrawParams(dc, x3, y3, cx, cy, i1, j1, ci, cj);
			}

			// on termine le dessin des �l�ments
			OnEndDrawing(dc);
		}
	}
}

void JFCButtonList::OnDrawBorder(CDC & dc, CRect client, LONG x, LONG y, LONG dx, LONG dy)
{
	// on teste si on a des bordures a dessiner
	if (m_HorzBorderSize || m_VertBorderSize)
	{
		// on teste si on doit dessiner le cadre
		if ((x <= client.left) || (y <= client.top) || ((x + dx) >= client.right) || ((y + dy) >= client.bottom))
		{
			// on cr�e le stylo A
			CPen PenA(PS_SOLID, 1, CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

			// on s�lectionne le stylo A
			CPen* pOldPen = dc.SelectObject(&PenA);

			// on r�cup�re la position de la bordure
			LONG x1 = client.left   - x;
			LONG x2 = client.right  - x - m_HorzBorderSize +1;
			LONG y1 = client.top    - y;
			LONG y2 = client.bottom - y - m_VertBorderSize +1;

			// on teste si on doit tracer les bordures horizontales
			if (m_HorzBorderSize)
			{
				//if( !Params.m_State&BUT_DOWN && m_bMouseOnButton)
				//{
				//	CPen penTmp;
				//	penTmp.CreatePen(PS_SOLID, 1, CATPColors::GetColorWhite()); 
				//}
				// on trace les bordures horizontales
				dc.MoveTo(x1, y1); dc.LineTo(x2 + m_HorzBorderSize, y1);
				dc.MoveTo(x1, y2); dc.LineTo(x2 + m_HorzBorderSize, y2);
			}

			// on teste si on doit tracer les bordures verticales
			if (m_VertBorderSize)
			{
				// on trace les bordures verticales
				dc.MoveTo(x1, y1); dc.LineTo(x1, y2 + m_VertBorderSize);
				dc.MoveTo(x2, y1); dc.LineTo(x2, y2 + m_VertBorderSize);
			}

			// on restaure le stylo
			dc.SelectObject(pOldPen);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCButtonList message handlers

void JFCButtonList::PreSubclassWindow() 
{
	// on initialise les bordures
	m_HorzBorderSize = m_VertBorderSize = 2;

	// on initialise les ascenseurs
	m_HorzMaster  = m_VertMaster  = FALSE;
	m_pHorzScroll = m_pVertScroll = NULL;

	// on initialise les �l�ments
	m_HorzFirst = m_HorzLast = m_HorzSize = 0;
	m_VertFirst = m_VertLast = m_VertSize = 0;

	// on appelle le gestionnaire de base
	JFControl::PreSubclassWindow();

	// on configure les ascenseurs
	OnSetScrollBar(FALSE);
}

void JFCButtonList::OnPaint() 
{
	CDC DcComp;
	CRect Client;
	CBitmap BmpNew;

	// on cr�e un contexte de p�riph�rique
	CPaintDC Dc(this);

	// on r�cup�re la zone cliente de la fen�tre
	GetClientRect(&Client);

	// on r�cup�re la position de la zone � dessiner
	LONG x = Dc.m_ps.rcPaint.left;
	LONG y = Dc.m_ps.rcPaint.top;

	// on calcule la taille de la zone � dessiner
	LONG dx = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG dy = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on cr�e un contexte de p�riph�rique compatible
	if (FALSE != DcComp.CreateCompatibleDC(&Dc))
	{
		// on cr�e une bitmap compatible avec ce contexte de p�riph�rique
		if (FALSE != BmpNew.CreateCompatibleBitmap(&Dc, dx, dy))
		{
			// on s�lectionne la bitmap dans le contexte de p�riph�rique
			CBitmap* pBmpOld = (CBitmap*)DcComp.SelectObject(&BmpNew);

			// on vide la bitmap
			DcComp.BitBlt(0, 0, dx, dy, NULL, 0, 0, WHITENESS);

			// on dessine la fen�tre
			OnDrawWindow(DcComp, x, y, dx, dy);

			// on dessine les bordures
			OnDrawBorder(DcComp, Client, x, y, dx, dy);

			// on copie la bitmap dans la zone � dessiner de la fen�tre
			Dc.BitBlt(x, y, dx, dy, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			DcComp.SelectObject(pBmpOld);
		}

		// on d�truit le contexte de p�riph�rique compatible
		DcComp.DeleteDC();
	}
}

void JFCButtonList::OnSize(UINT nType, INT cx, INT cy) 
{
	// on appelle le gestionnaire de base
	JFControl::OnSize(nType, cx, cy);
	
	// on met � jour les ascenseurs
	OnSetScrollBar(FALSE);
}

void JFCButtonList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on a une p�riode
	LONG i,j;

	// on r�cup�re l'�l�ment sous la souris
	if (GetIdxAtPos(point.x, point.y, i, j))
	{
		BtnParams Params;
		// R�cup params button
		OnGetItemParams(i,j,Params);
		CRect rect;
        GetPosAtIdx(i,j,rect.left,rect.top,rect.right,rect.bottom);
		rect.DeflateRect (m_HorzInterstice,m_VertInterstice,m_HorzInterstice,m_VertInterstice);
		if(rect.PtInRect(point))
		{
			if(Params.m_Style&LED_SEL)
			{
				if(rect.Width()>rect.Height())
					rect.right = rect.left+rect.Height();
				else
					rect.right = rect.left+(rect.Width()/4);

				// Type bouton avec gestion LED
				if (rect.PtInRect(point))
					// Click dans zone LED
					OnLedSel(i,j);
				else
					// Click hors zone LED
					OnButtonSel(i,j);
			}
			else
				// Type bouton sans gestion LED
				OnButtonSel(i,j);
		}
	}

	// on appelle le gestionnaire de base
	JFControl::OnLButtonDown(nFlags, point);
}

void JFCButtonList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// on appelle le gestionnaire de base
	JFControl::OnRButtonDown(nFlags, point);
}

//void JFCButtonList::OnMouseMove(UINT nFlags, CPoint point) 
//{
//	// on appelle le gestionnaire de base
//	JFControl::OnMouseMove(nFlags, point);
//}

void JFCButtonList::OnButtonSel(LONG i, LONG j)
{
}

void JFCButtonList::OnLedSel(LONG i, LONG j)
{
}




void JFCButtonList::CancelHover()
{
	// Only for flat buttons
	if (m_bMouseOnButton)
	{
		m_bMouseOnButton = FALSE;
		m_BtnI = m_BtnJ = -1;
		Invalidate();
	} // if
} // End of CancelHover

void JFCButtonList::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;

	JFControl::OnMouseMove(nFlags, point);

	CPoint pointTmp = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE) return;


	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if(!GetIdxAtPos(pointTmp.x, pointTmp.y, m_BtnI, m_BtnJ))
			CancelHover();
		else
		{
		if(!m_bMouseOnButton)
		{
			m_bMouseOnButton = TRUE;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		} // if
		Invalidate();
		}
	}
	else
		CancelHover();
} // End of OnMouseMove

// Handler for WM_MOUSELEAVE
LRESULT JFCButtonList::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	CancelHover();
	return 0;
} // End of OnMouseLeave
