// Obj_Horr.cpp : implementation file
//
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Horr v1.1
Tâche		: Découpage horaire
Fichiers	: Obj_Horr.cpp, Obj_Horr.h, Obj_Horr.rco, Obj_Horr.ico1, Obj_Horr.ico2, Obj_Horr.ico3, Obj_Horr.ico4
Auteur		: VLF
Creer le	: 14/05/98
Mise à jour : 10/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "stdafx.h"
#include "Obj_Horr.h"
#include "Obj_Horr.rch"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Obj_Horr

// coordonnées rigides des horaires
#define HOURY_TAILLELIGNE (10)		// nombre de pixel par ligne
#define HOURX_TRAITCENTRE (60)		// ligne verticale
#define HOURX_RETRAITTXTHOUR (10)	// pos gauche du texte des heures (7h00)
#define HOURX_RETRAITPIXHOUR (55)	// pos gauche du cran des heures
#define HOURX_RETRAITPIXDEMI (40)	// pos gauche du cran des demi-heures
#define HOURX_RETRAITPIXQUART (55)	// pos gauche du cran des quart-heures
#define HOURX_TAILLESELECT (140)	// pos droite d'une flèche de sélection
#define HOURY_ICOCENTREFLECHE (4)	// pos centre de la fleche de l'icone
#define HOURX_TEXTEPLAGE (65)		// pos gauche du texte plage(5h00-12h30)
#define HOURX_SIZESCROLLBAR (16)	// pos gauche du texte plage(5h00-12h30)
#define HOURY_TAILLELIGHEURE (13)	// Hauteur de la ligne de choix des heures

CObj_Horr::CObj_Horr()
{
	// les pinceaux
	m_BrushNoir.CreateSolidBrush(RGB(0,0,0));
	m_BrushGrisC.CreateSolidBrush(RGB(192,192,192));

	// les stylos
	m_PenGrisF.CreatePen(PS_SOLID,1,RGB(128,128,128));
	m_PenBlanc.CreatePen(PS_SOLID,1,RGB(255,255,255));
	m_PenNoir.CreatePen(PS_SOLID,1,RGB(0,0,0));
	m_PenGrisC.CreatePen(PS_SOLID,1,RGB(192,192,192));
	m_Pen4B.CreatePen(PS_SOLID,4,RGB(0,0,0));
	// mais que fait la police
	BOOL ret = m_Font.CreateFont( -8,0, 0, 0,FW_NORMAL, /* specify pt size    */
			0, 0, 0, 0, 0, 0, 0,(BYTE)(FIXED_PITCH | FF_MODERN),"Courier");         /* and face name only */
	if(ret==0)::AfxMessageBox("Erreur font !");
	
	// init  des icones
	m_Ico1=AfxGetApp()->LoadIcon(IDI_HORR_ICO1);
	m_Ico2=AfxGetApp()->LoadIcon(IDI_HORR_ICO2);
	m_Ico3=AfxGetApp()->LoadIcon(IDI_HORR_ICO3);
	m_Ico4=AfxGetApp()->LoadIcon(IDI_HORR_ICO4);
	m_Selections = NULL;
	m_fCapture = false;
	m_fInit = false;
	m_ScrollBar = NULL;

	m_Hdeb		= 0;
	m_Hsel		= 0;
	m_Hoffset	= 0;
	m_fCaptureH = false;
	m_pFont		= NULL;
	m_pFontB	= NULL;
	m_Style		= 0;
	m_HeureDebut= 0;
	m_QhDebut	= 0;
	m_MinDebut	= 0;
	m_NbLigne	= 0;
	m_PosAsc	= 0;
	m_PosZoom	= 0;
	m_NbHeure	= 0;
	m_RHeure.SetRectEmpty();
}

CObj_Horr::~CObj_Horr()
{
	m_BrushNoir.DeleteObject();
	m_BrushGrisC.DeleteObject();
	m_PenGrisF.DeleteObject();
	m_PenBlanc.DeleteObject();
	m_PenNoir.DeleteObject();
	m_PenGrisC.DeleteObject();
	m_Pen4B.DeleteObject();
	m_Font.DeleteObject();

	::DestroyIcon(m_Ico1);
	::DestroyIcon(m_Ico2);
	::DestroyIcon(m_Ico3);
	::DestroyIcon(m_Ico4);

	LibereObjet();
}

void CObj_Horr::LibereObjet(void )
{
	if(m_Selections)	delete [] m_Selections;
	m_Selections = NULL;
	if(m_ScrollBar)		delete m_ScrollBar;
	m_ScrollBar = NULL;
	m_fInit = false;
}

BEGIN_MESSAGE_MAP(CObj_Horr, CStatic)
	//{{AFX_MSG_MAP(CObj_Horr)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Horr message handlers


void CObj_Horr::Initialise(int Styles,int HoraireDebut,char *Selections)
{
	RECT R;
	short decoup = 0;

	switch(Styles&0x00F)
	{
	case HOURS_STANDARD:
		m_Style = Styles;
		m_HeureDebut = HoraireDebut / 100;
		m_QhDebut = (HoraireDebut % 100) / 15;
		m_MinDebut = 0;
		m_Selections = new char[DECOUPAGE_HOUR];
		if(Selections)
			memcpy(m_Selections,Selections,DECOUPAGE_HOUR);
		else
			memset(m_Selections, 0 , DECOUPAGE_HOUR);
		GetClientRect(&R);
		if(m_Style & SCROLL_HEURE)
		{
			R.bottom -= HOURY_TAILLELIGHEURE;
			m_Hoffset = m_HeureDebut;
		}
		m_NbLigne = (R.bottom - R.top) / HOURY_TAILLELIGNE;
		m_PosAsc = 0;
		m_PosZoom = 0;

		if(m_Style & QH_ZOOM)	//zoom
		{
			m_RZoom.left	= HOURX_TRAITCENTRE - 16;
			m_RZoom.top		= m_PosZoom * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - HOURY_ICOCENTREFLECHE;
			m_RZoom.right	= m_RZoom.left + 24;
			m_RZoom.bottom	= m_RZoom.top + 24;
		}
		decoup = DECOUPAGE_HOUR;
		break;
	case MINUTES_STANDARD:
		m_Style = Styles;
		m_HeureDebut = HoraireDebut/100;
		m_QhDebut = (HoraireDebut%100)/15;
		m_MinDebut = (HoraireDebut%100);
		m_Selections = new char[DECOUPAGE_MIN];
		if(Selections)
			memcpy(m_Selections, Selections, DECOUPAGE_MIN);
		else
			memset(m_Selections, 0 , DECOUPAGE_MIN);
		GetClientRect(&R);
		if(m_Style & SCROLL_HEURE)	
		{
			R.bottom -= HOURY_TAILLELIGHEURE;
			m_Hoffset = m_HeureDebut;
		}
		m_NbLigne = (R.bottom - R.top) / HOURY_TAILLELIGNE;
		m_PosAsc = 0;
		m_PosZoom =0;

		m_RZoom.SetRectEmpty();
		decoup = DECOUPAGE_MIN;
		break;
	case HOURS_MINUTES:
		m_Style = Styles;
		m_HeureDebut = HoraireDebut / 100;
		m_QhDebut = (HoraireDebut % 100) / 15;
		m_MinDebut = (HoraireDebut % 100);
		m_Selections = new char[DECOUPAGE_MIN];
		if(Selections)
			memcpy(m_Selections, Selections, DECOUPAGE_MIN);
		else
			memset(m_Selections, 0 , DECOUPAGE_MIN);
		GetClientRect(&R);
		if(m_Style & SCROLL_HEURE)	
		{
			R.bottom -= HOURY_TAILLELIGHEURE;
			m_Hoffset = m_HeureDebut;
		}
		m_NbLigne = (R.bottom - R.top) / HOURY_TAILLELIGNE;
		m_PosAsc = 0;
		m_PosZoom = 0;

		if(m_Style & QH_ZOOM)	//zoom
		{
			m_RZoom.left	= HOURX_TRAITCENTRE - 16;
			m_RZoom.top		= m_PosZoom * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - HOURY_ICOCENTREFLECHE;
			m_RZoom.right	= m_RZoom.left+24;
			m_RZoom.bottom	= m_RZoom.top+24;
		}
		//ICI m_Decoupage vaut DECOUPAGE_HOUR pour l'ini du scrollbar
		decoup		= DECOUPAGE_HOUR;
		break;
	case TV100MINUTES:
		m_Style = Styles;
		m_HeureDebut	= HoraireDebut / 100;
		m_MinDebut		= HoraireDebut % 100;
		m_Selections	= new char[DECOUPAGE_100MIN];	//2400 minutes
		if(Selections)
			memcpy(m_Selections, Selections, DECOUPAGE_100MIN);
		else
			memset(m_Selections, 0, DECOUPAGE_100MIN);
		GetClientRect(&R);
		if(m_Style & SCROLL_HEURE)	
		{
			R.bottom -= HOURY_TAILLELIGHEURE;
			m_Hoffset = m_HeureDebut;
		}
		m_NbLigne = (R.bottom - R.top) / HOURY_TAILLELIGNE;
		m_PosAsc = 0;
		m_PosZoom = 0;
		decoup = DECOUPAGE_100MIN;
		break;
	default:break;
	}

	// construction de l'ascenceur
	if(!(m_Style & INVISIBLE_VSCROLL))	//ascenseur
	{
		R.left = R.right - HOURX_SIZESCROLLBAR;
		m_ScrollBar = new CScrollBar;
		m_ScrollBar->Create(WS_VISIBLE|SBS_VERT, R, (CWnd*)this, 0);
		// on initialise le contrôl asc.
		m_ScrollBar->SetScrollRange(0, (decoup - m_NbLigne), 0);
		m_ScrollBar->SetScrollPos(m_PosAsc, TRUE);
	}
	m_fInit = true;
}

short CObj_Horr::SetZoomQH(bool fVisible)
{
	if((m_Style & 0x00F) == HOURS_MINUTES)	//Zoom
	{
		if(fVisible)
		{
			m_Style	|= QH_ZOOM;
			m_PosZoom		= 0;
			m_RZoom.left	= HOURX_TRAITCENTRE - 16;
			m_RZoom.top		= m_PosZoom * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - HOURY_ICOCENTREFLECHE;
			m_RZoom.right	= m_RZoom.left + 24;
			m_RZoom.bottom	= m_RZoom.top + 24;
		}
		else
		{
			m_Style = m_Style & 0x1FF;
			m_PosZoom = 0;
			m_RZoom.SetRectEmpty();
		}
		InvalidateRect(NULL, 0);
		return(1);
	}
	return(0);
}

void CObj_Horr::SetSelections(char *Selections, bool fUpdate)
{
	if(!Selections)	return;
	switch(m_Style&0x00F)
	{
	case HOURS_STANDARD:
		memcpy(m_Selections, Selections, DECOUPAGE_HOUR);
		break;
	case TV100MINUTES:
		memcpy(m_Selections, Selections, DECOUPAGE_100MIN);
		break;
	case HOURS_MINUTES:
	case MINUTES_STANDARD:
		memcpy(m_Selections,Selections,DECOUPAGE_MIN);
		break;
	default:
		break;
	}
	if(fUpdate)	InvalidateRect(NULL, 0);
}

short CObj_Horr::DonneResultHour(char * Selections)
{
	if(!Selections)	return(0);
	int NbFleche,p;

	switch(m_Style & 0x00F)
	{
	case HOURS_STANDARD:
		for(NbFleche = 0, p = 0 ; p < DECOUPAGE_HOUR ; p++)
		{
			if(m_Selections[p])
			{
				NbFleche++;
			}
		}
		if(NbFleche < 2)
			return(1);
		memcpy(Selections, m_Selections, DECOUPAGE_HOUR);
		break;
	case TV100MINUTES:
		for(NbFleche = 0, p = 0 ; p < DECOUPAGE_100MIN ; p++)
		{
			if(m_Selections[p])
			{
				NbFleche++;
			}
		}
		if(NbFleche < 2)
			return(1);
		memcpy(Selections, m_Selections, DECOUPAGE_100MIN);
		break;
	case HOURS_MINUTES:
	case MINUTES_STANDARD:
		for(NbFleche = 0, p = 0 ; p < DECOUPAGE_MIN ; p++)
		{
			if(m_Selections[p])
				NbFleche++;
		}
		if(NbFleche<2)return(1);
		memcpy(Selections,m_Selections,DECOUPAGE_MIN);
		break;
	}
	return(0);
}

short CObj_Horr::DonneResultZoom()
{
	return(m_PosZoom+m_PosAsc);
}

void CObj_Horr::SetPosition(int Position, bool fUpdate)
{
	int maxi;
	switch(m_Style & 0x00F)
	{
	case HOURS_STANDARD:
	case HOURS_MINUTES:
		maxi = DECOUPAGE_HOUR - m_NbLigne + 1;
		if(Position >= 0 && Position <= maxi)
		{
			m_PosAsc = Position;
			if(!(m_Style & INVISIBLE_VSCROLL))	m_ScrollBar->SetScrollPos(m_PosAsc, TRUE);
		}
		m_MinDebut = 0;
		break;
	case MINUTES_STANDARD:
		maxi = DECOUPAGE_MIN - m_NbLigne + 1;
		if(Position >= 0 && Position <= maxi)
		{
			m_PosAsc = Position;
			if(!(m_Style & INVISIBLE_VSCROLL))	m_ScrollBar->SetScrollPos(m_PosAsc, TRUE);
		}
		m_PosZoom=0;
		break;
	case TV100MINUTES:
		maxi = DECOUPAGE_100MIN - m_NbLigne + 1;
		if(Position >= 0 && Position <= maxi)
		{
			m_PosAsc = Position;
			if(!(m_Style & INVISIBLE_VSCROLL))	m_ScrollBar->SetScrollPos(m_PosAsc, TRUE);
		}
		m_MinDebut = 0;
		break;
	default:break;
	}
	if(fUpdate)		InvalidateRect(NULL,0);
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void CObj_Horr::OnPaint() 
{
	if(!m_fInit)	return;
	CPaintDC dcDest(this); // device context for painting
	
	RECT R, Rdest;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	long l, h;

	// copie de l'écran en mémoire ****************
	GetClientRect(&R);
	if(!(m_Style & INVISIBLE_VSCROLL))	//ascenseur
		R.right -= HOURX_SIZESCROLLBAR;
	DC.CreateCompatibleDC(&dcDest);
	l = R.right - R.left;
	h = R.bottom - R.top;
	hBmpNew.CreateCompatibleBitmap(&dcDest, l, h);
	hBmpOld = (CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);
	// *********************************************

	// Dessin proprement dit du bouton
	DC.SelectObject(m_Font);
	DC.SetBkMode(TRANSPARENT);
	//DC.FrameRect(&R,&m_BrushNoir);
	DC.MoveTo(HOURX_TRAITCENTRE,0);
	if(m_Style & SCROLL_HEURE)	
		R.bottom -= HOURY_TAILLELIGHEURE;
	DC.LineTo(HOURX_TRAITCENTRE, R.bottom);

	DessineGraduations(&DC);
	// les libellés entre les flèches
	DessineLibelles(&DC);
	//Dessine le Zoom
	if((m_Style & QH_ZOOM) && ((m_Style & 0x00F) == HOURS_MINUTES))
		DessineZoom(&DC);
	if(m_Style & SCROLL_HEURE)
		DessineRaccourciH(&DC);

	// copy de la mem sur ecran
	Rdest = dcDest.m_ps.rcPaint;
	dcDest.BitBlt(Rdest.left, Rdest.top, Rdest.right, Rdest.bottom, &DC, Rdest.left, Rdest.top, SRCCOPY);
	dcDest.SelectObject(hBmpOld);
	DC.DeleteDC();
	// ************************
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CObj_Horr::DessineRaccourciH(CDC * DC)
{
	COLORREF	colorRed(RGB(255,0,0)),
				colorMauve(RGB(192,192,192)),
				colorBlack(RGB(0,0,0)),
				oldColor;
	CFont		*oldFont = DC->GetCurrentFont();
	CRect		R;

	GetClientRect(&R);

	long lfheight = -MulDiv(7, DC->GetDeviceCaps(LOGPIXELSY), 72);
	long lfwidth = -MulDiv(7, DC->GetDeviceCaps(LOGPIXELSX), 72);

	if(!m_pFont)
	{
		m_pFont = new CFont;
		m_pFont->CreateFont(lfheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
	}
	if(!m_pFontB)
	{	
		m_pFontB = new CFont;
		m_pFontB->CreateFont(lfheight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, 0, DEFAULT_QUALITY, DEFAULT_PITCH|FF_ROMAN, "Times New Roman");
	}

	m_RHeure		= R;
	m_RHeure.top	= m_RHeure.bottom - ((-lfheight) * 3 / 2);
	m_RHeure.right	-= HOURX_SIZESCROLLBAR;

	//cadre de la barre des heures
/*	DC->MoveTo(m_RHeure.left, m_RHeure.top);
	DC->LineTo(m_RHeure.right - 1, m_RHeure.top);
	DC->LineTo(m_RHeure.right - 1, m_RHeure.bottom - 1);
	DC->LineTo(m_RHeure.left, m_RHeure.bottom - 1);
	DC->LineTo(m_RHeure.left, m_RHeure.top);
*/
	DC->DrawEdge(&m_RHeure, EDGE_BUMP, BF_RECT);


	long	width	= 0,
			widthRef= 0;
	short	pos		= 0;
	CString str;

	//Calcul du nombre d'heure possible à afficher
	for(short h = 0 ; h < 23 + m_Hoffset; h++)
	{
		str.Format("%d", h);
		CSize txtSize = DC->GetTextExtent(str);
		width = txtSize.cx  + (-lfwidth / 2) + 2;
		if(width > widthRef)
		{
			widthRef = width;
		}
		break;
	}
	m_NbHeure = m_RHeure.Width() / widthRef;

	//Affiche les heures
	width	= 0;
	for(short i = 0 ; i < m_NbHeure ; i++)
	{
		//Tester si heure sélectionnée
		if(m_Hdeb > m_Hsel)
		{
			if((i + m_Hdeb + m_Hoffset - 24) == m_Hsel)
			{
				DC->SelectObject(m_pFontB);
				oldColor = DC->SetTextColor(colorRed);
			}
			else
			{
				DC->SelectObject(m_pFont);
				DC->SetTextColor(colorBlack);
			}			
		}
		else
		{
			if((i + m_Hdeb + m_Hoffset) == m_Hsel)
			{
				oldFont	 = (CFont *)DC->SelectObject(m_pFontB);
				oldColor = DC->SetTextColor(colorRed);
			}
			else
			{
				oldFont = (CFont *)DC->SelectObject(m_pFont);
				DC->SetTextColor(colorBlack);
			}
		}

		if((i + m_Hdeb) > 23)
		{
			str.Format("%d", m_Hoffset + i + m_Hdeb - 24);
		}
		else
		{
			str.Format("%d", m_Hoffset + i + m_Hdeb);
		}

		CSize txtSize = DC->GetTextExtent(str);
		if(i == 0)
		{
			m_RposHeure[i].SetRect(2 + m_RHeure.left,
									m_RHeure.bottom - ((-lfheight) * 3 / 2),
									2 + m_RHeure.left + widthRef,
									m_RHeure.bottom);
		}
		else
		{
			m_RposHeure[i].SetRect(m_RposHeure[i-1].right,
									m_RHeure.bottom - ((-lfheight) * 3 / 2),
									m_RposHeure[i-1].right + widthRef,
									m_RHeure.bottom);
		}
		DC->DrawText(str, m_RposHeure[i], DT_CENTER|DT_VCENTER);
	}
	//On trace des points pour séparé les heures
	CPen	pen(PS_SOLID, 2, colorMauve),
			*oldPen;
	oldPen = (CPen*)DC->SelectObject(&pen);
	for(i = 1 ; i < m_NbHeure ; i++)
	{
		int dx = m_RposHeure[i].left - m_RposHeure[i - 1].right;
		DC->MoveTo(m_RposHeure[i].left + (dx / 2), m_RHeure.top + (-lfheight) * 2 / 3);
		DC->LineTo(m_RposHeure[i].left + (dx / 2), m_RHeure.top + (-lfheight) * 2 / 3);
	}

	DC->SelectObject(oldPen);
	DC->SelectObject(oldFont);
	DC->SetTextColor(oldColor);
}


void CObj_Horr::DessineGraduations(CDC * DC)
{
	CPen * Oldpen=NULL;
	short m = 0;
	for(short i = 0 ; i < m_NbLigne ; i++)
	{
		switch(m_Style & 0x00F)
		{
		case HOURS_STANDARD:
			switch((i + m_PosAsc + m_QhDebut) % 4)
			{
				case 0: // Heure
					sprintf(m_tamponalloc,"%2dH00",m_HeureDebut+(i+m_PosAsc+m_QhDebut)/4);
					if(i==0)DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE,m_tamponalloc,strlen(m_tamponalloc));
					else if(i == m_NbLigne-1)DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE-2,m_tamponalloc,strlen(m_tamponalloc));
					else DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE-1,m_tamponalloc,strlen(m_tamponalloc));
					DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-1);
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-1);
					DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)+1);
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)+1);
				break;

				case 2: // Demi-heure
					DC->MoveTo(HOURX_RETRAITPIXDEMI,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				break;

				case 1: // Quart-heure
				case 3: // Quart-heure
					DC->MoveTo(HOURX_RETRAITPIXQUART,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				break;
			}
			// les sélections :
			switch(m_Selections[i+m_PosAsc])
			{
			case 1:	//fleche roug->qh
				DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico1);
				break;
			case 2:	//fleche bleu->minutes
				DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE);
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE);
//				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico2);
				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico1);
				break;
			case 3:	//fleche bleu ET fleche rouge
				DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico1);
				DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE);
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE);
				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico1);
//				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico2);
			}
			break;
		case MINUTES_STANDARD:
			if((i+m_PosAsc+m_MinDebut)%5)
			{
				DC->MoveTo(HOURX_RETRAITPIXQUART,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
			}
			else
			{
				sprintf(m_tamponalloc,"%2dH%02d",m_HeureDebut+(i+m_PosAsc+m_MinDebut)/60, (i+m_PosAsc+m_MinDebut)%60);
				if(i==0)DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE,m_tamponalloc,strlen(m_tamponalloc));
				else if(i==m_NbLigne-1)DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE-2,m_tamponalloc,strlen(m_tamponalloc));
				else DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE-1,m_tamponalloc,strlen(m_tamponalloc));
				DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-1);
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-1);
				DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)+1);
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)+1);
			}
			// les sélections :
			switch(m_Selections[i+m_PosAsc])
			{
			case 1:	//fleche roug->qh
				DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico1);
				break;
			case 2:	//fleche bleu->minutes
			case 3:	//fleche bleu->minutes
				DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
				DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
//				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico2);
				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico1);
				break;
			}
			break;
		case HOURS_MINUTES:
			switch((i+m_PosAsc+m_QhDebut)%4)
			{
				case 0: // Heure
					sprintf(m_tamponalloc,"%2dH00",m_HeureDebut+(i+m_PosAsc+m_QhDebut)/4);
					if(i==0)DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE,m_tamponalloc,strlen(m_tamponalloc));
					else if(i==m_NbLigne-1)DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE-2,m_tamponalloc,strlen(m_tamponalloc));
					else DC->TextOut(HOURX_RETRAITTXTHOUR,1+i*HOURY_TAILLELIGNE-1,m_tamponalloc,strlen(m_tamponalloc));
					if(m_Style&QH_ZOOM && ((m_Style&0x00F)==HOURS_MINUTES))	//Zoom
					{
						if(m_RZoom.top+3<1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2) && m_RZoom.bottom>1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2))
							Oldpen=DC->SelectObject(&m_Pen4B);
					}
					DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-1);
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-1);
					DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->MoveTo(HOURX_RETRAITPIXHOUR,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)+1);
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)+1);
					if(Oldpen)		DC->SelectObject(Oldpen);
				break;

				case 2: // Demi-heure
					if(m_Style&QH_ZOOM && ((m_Style&0x00F)==HOURS_MINUTES))	//Zoom
					{
						if(m_RZoom.top+3<1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2) && m_RZoom.bottom>1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2))
						{
							DC->MoveTo(HOURX_RETRAITPIXDEMI,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
							DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
							Oldpen=DC->SelectObject(&m_Pen4B);
							DC->MoveTo(m_RZoom.left+5,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
							DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
							DC->SelectObject(Oldpen);
						}
						else
						{
							DC->MoveTo(HOURX_RETRAITPIXDEMI,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
							DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
						}
					}
					else
					{
						DC->MoveTo(HOURX_RETRAITPIXDEMI,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
						DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					}
				break;

				case 1: // Quart-heure
				case 3: // Quart-heure
					if(m_Style&QH_ZOOM && ((m_Style&0x00F)==HOURS_MINUTES))	//Zoom
					{
						if(m_RZoom.top+3<1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2) && m_RZoom.bottom>1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2))
							Oldpen=DC->SelectObject(&m_Pen4B);
					}
					DC->MoveTo(HOURX_RETRAITPIXQUART,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					if(Oldpen)		DC->SelectObject(Oldpen);
				break;
			}
			// les sélections :
			for(m = 0 ; m < 15 ; m++)
			{
				switch(m_Selections[((i+m_PosAsc)*15)+m])
				{
				case 1:	//fleche roug->qh
					DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico1);
					break;
				case 2:	//fleche bleu->minutes
					DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE));
					DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico1);
//					DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico4);
					break;
				case 3:	//fleche bleu ET fleche rouge
					DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2));
					DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico1);
					DC->MoveTo(HOURX_TAILLESELECT,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE));
					DC->LineTo(HOURX_TRAITCENTRE,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE));
					DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico1);
//					DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+HOURY_TAILLELIGNE-HOURY_ICOCENTREFLECHE,m_Ico4);
				}
			}
			break;
		case TV100MINUTES:
			if((i + m_PosAsc + m_MinDebut) % 5)
			{
				DC->MoveTo(HOURX_RETRAITPIXQUART, 1 + (i * HOURY_TAILLELIGNE) + (HOURY_TAILLELIGNE / 2));
				DC->LineTo(HOURX_TRAITCENTRE, 1 + (i * HOURY_TAILLELIGNE) + (HOURY_TAILLELIGNE / 2));
			}
			else
			{
				sprintf(m_tamponalloc,"%2dH%02d", m_HeureDebut + (i + m_PosAsc + m_MinDebut) / 100, (i + m_PosAsc + m_MinDebut) % 100);
				if(i == 0)
				{
					DC->TextOut(HOURX_RETRAITTXTHOUR, 1 + i * HOURY_TAILLELIGNE, m_tamponalloc, strlen(m_tamponalloc));
				}
				else
				{
					if(i == m_NbLigne-1)
					{
						DC->TextOut(HOURX_RETRAITTXTHOUR, 1 + i * HOURY_TAILLELIGNE - 2, m_tamponalloc, strlen(m_tamponalloc));
					}
					else
					{
						DC->TextOut(HOURX_RETRAITTXTHOUR, 1 + i * HOURY_TAILLELIGNE - 1, m_tamponalloc, strlen(m_tamponalloc));
					}
				}
				DC->MoveTo(HOURX_RETRAITPIXHOUR, 1 + i * HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE / 2) - 1);
				DC->LineTo(HOURX_TRAITCENTRE, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - 1);
				DC->MoveTo(HOURX_RETRAITPIXHOUR, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2));
				DC->LineTo(HOURX_TRAITCENTRE, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2));
				DC->MoveTo(HOURX_RETRAITPIXHOUR, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) + 1);
				DC->LineTo(HOURX_TRAITCENTRE, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) + 1);
			}
			// les sélections :
			switch(m_Selections[i + m_PosAsc])
			{
			case 1:	//fleche roug->qh
				DC->MoveTo(HOURX_TAILLESELECT,1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2));
				DC->LineTo(HOURX_TRAITCENTRE, 1 + i * HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE / 2));
				DC->DrawIcon(HOURX_TRAITCENTRE + 1,1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - HOURY_ICOCENTREFLECHE,m_Ico1);
				break;
			case 2:	//fleche bleu->minutes
			case 3:	//fleche bleu->minutes
				DC->MoveTo(HOURX_TAILLESELECT, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2));
				DC->LineTo(HOURX_TRAITCENTRE, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2));
//				DC->DrawIcon(HOURX_TRAITCENTRE+1,1+i*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE,m_Ico2);
				DC->DrawIcon(HOURX_TRAITCENTRE + 1, 1 + i * HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - HOURY_ICOCENTREFLECHE, m_Ico1);
				break;
			}
			break;
		}
	}
}

void CObj_Horr::DessineLibelles(CDC * DC)
{
	int i,iAvant,p,pAvant,NbFleche;
	switch(m_Style & 0x00F)
	{
	case HOURS_STANDARD:
		for(NbFleche=0,p=0;p<DECOUPAGE_HOUR;p++)if(m_Selections[p])NbFleche++;
		if(NbFleche>1)
		{
			iAvant=-1;
			for(i=0;i<m_NbLigne;i++)
			{
				if(m_Selections[i+m_PosAsc])
				{
					if(iAvant+1>=i){
						iAvant=i;
						continue; // pas de place
					}
					pAvant=-1;
					for(p=0;p<DECOUPAGE_HOUR;p++)
					{
						if(!m_Selections[p])continue;
						if(pAvant>=0 && p==i+m_PosAsc)break;
						pAvant=p;
					}
					sprintf(m_tamponalloc,"%dh%02d-%dh%02d",
						m_HeureDebut+((m_QhDebut+pAvant)*15)/60,
						((m_QhDebut+pAvant)*15)%60,
						m_HeureDebut+((i+m_QhDebut+m_PosAsc)*15)/60,
						((i+m_QhDebut+m_PosAsc)*15)%60);
					DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE*(iAvant+1)+(HOURY_TAILLELIGNE*(i-iAvant-2))/2,m_tamponalloc,strlen(m_tamponalloc));
					iAvant=i;
				}
			}
			// le dernier libellé
			if(iAvant+1<i)
			{
				pAvant=-1;
				for(p=0;p<DECOUPAGE_HOUR;p++){
					if(m_Selections[p])pAvant=p;
					if(pAvant>=0 && p>=(i+m_PosAsc-1))break;
				}
//				p++;
				p=i+m_PosAsc;
				while(!m_Selections[p]){
					p++;
					if(p>=DECOUPAGE_HOUR)p=0;
				}
				sprintf(m_tamponalloc,"%dH%02d-%dH%02d",
					m_HeureDebut+((m_QhDebut+pAvant)*15)/60,
					((m_QhDebut+pAvant)*15)%60,
					m_HeureDebut+((m_QhDebut+p)*15)/60,
					((m_QhDebut+p)*15)%60);
				DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE*(iAvant+1)+(HOURY_TAILLELIGNE*(i-iAvant-2))/2,m_tamponalloc,strlen(m_tamponalloc));
			}
		}
		break;
	case MINUTES_STANDARD:
		for(NbFleche=0,p=0;p<DECOUPAGE_MIN;p++)if(m_Selections[p])NbFleche++;
		if(NbFleche>1)
		{
			iAvant=-1;
			for(i=0;i<m_NbLigne;i++)
			{
				if(m_Selections[i+m_PosAsc])
				{
					if(iAvant+1>=i){
						iAvant=i;
						continue; // pas de place
					}
					pAvant=-1;
					for(p=0;p<DECOUPAGE_MIN;p++)
					{
						if(!m_Selections[p])continue;
						if(pAvant>=0 && p==i+m_PosAsc)break;
						pAvant=p;
					}
					sprintf(m_tamponalloc,"%dh%02d-%dh%02d",
						m_HeureDebut+((m_MinDebut+pAvant))/60,
						((m_MinDebut+pAvant))%60,
						m_HeureDebut+((i+m_MinDebut+m_PosAsc))/60,
						((i+m_MinDebut+m_PosAsc))%60);
					DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE*(iAvant+1)+(HOURY_TAILLELIGNE*(i-iAvant-2))/2,m_tamponalloc,strlen(m_tamponalloc));
					iAvant=i;
				}
			}
			// le dernier libellé
			if(iAvant+1<i)
			{
				pAvant=-1;
				for(p=0;p<DECOUPAGE_MIN;p++){
					if(m_Selections[p])pAvant=p;
					if(pAvant>=0 && p>=(i+m_PosAsc-1))break;
				}
				p++;
				while(!m_Selections[p]){
					p++;
					if(p>=DECOUPAGE_MIN)p=0;
				}
				sprintf(m_tamponalloc,"%dH%02d-%dH%02d",
					m_HeureDebut+((m_MinDebut+pAvant))/60,
					((m_MinDebut+pAvant))%60,
					m_HeureDebut+((m_MinDebut+p))/60,
					((m_MinDebut+p))%60);
				DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE*(iAvant+1)+(HOURY_TAILLELIGNE*(i-iAvant-2))/2,m_tamponalloc,strlen(m_tamponalloc));
			}
		}
		break;
	case HOURS_MINUTES:
		for(NbFleche=0,p=0;p<DECOUPAGE_MIN;p++)if(m_Selections[p])NbFleche++;
		if(NbFleche>1)
		{
			iAvant=-1;
			for(i=0;i<m_NbLigne;i++)
			{
				for(short m=0 ; m<15 ; m++)
				{
					if((((i+m_PosAsc)*15)+m)<DECOUPAGE_MIN)
					{
						if(m_Selections[((i+m_PosAsc)*15)+m])
						{
							if(iAvant+1>=i){
								iAvant=i;
								continue; // pas de place
							}
							pAvant=-1;
							for(p=0;p<DECOUPAGE_MIN;p++)
							{
								if(!m_Selections[p])continue;
								if(pAvant>=0 && p==((i+m_PosAsc)*15)+m)break;
								pAvant=p;
							}
							sprintf(m_tamponalloc,"%dh%02d-%dh%02d",
								m_HeureDebut+((m_MinDebut+pAvant)/60),
								((m_MinDebut+pAvant)%60),
								m_HeureDebut+(m_MinDebut+((i+m_PosAsc)*15)+m)/60,
								(m_MinDebut+((i+m_PosAsc)*15)+m)%60);
							DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE*(iAvant+1)+(HOURY_TAILLELIGNE*(i-iAvant-2))/2,m_tamponalloc,strlen(m_tamponalloc));
							iAvant=i;
						}
					}
				}
			}

			// le dernier libellé
			if(iAvant+1<i)
			{
				pAvant=-1;
				for(short m=0 ; m<15 ; m++)
				{
					if((((i+m_PosAsc-1)*15)+m)<DECOUPAGE_MIN)
					{
						for(p=0;p<DECOUPAGE_MIN;p++){
							if(m_Selections[p])pAvant=p;
							if(pAvant>=0 && p>=((i+m_PosAsc-1)*15)+m)break;
						}
						while(!m_Selections[p]){
							p++;
							if(p>=DECOUPAGE_MIN)p=0;
						}
						sprintf(m_tamponalloc,"%dH%02d-%dH%02d",
							m_HeureDebut+(m_MinDebut+pAvant)/60,
							(m_MinDebut+pAvant)%60,
							m_HeureDebut+(m_MinDebut+p)/60,
							(m_MinDebut+p)%60);
						DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE*(iAvant+1)+(HOURY_TAILLELIGNE*(i-iAvant-2))/2,m_tamponalloc,strlen(m_tamponalloc));
					}
				}
			}
		}
		break;
	case TV100MINUTES:
		for(NbFleche = 0, p = 0 ; p < DECOUPAGE_100MIN ; p++)
		{
			if(m_Selections[p])
				NbFleche++;
		}
		if(NbFleche > 1)
		{
			iAvant = -1;
			for(i = 0 ; i < m_NbLigne ; i++)
			{
				if(m_Selections[i + m_PosAsc])
				{
					if(iAvant + 1 >= i)
					{
						iAvant = i;
						continue; // pas de place
					}
					pAvant = -1;
					for(p = 0 ; p < DECOUPAGE_100MIN ; p++)
					{
						if(!m_Selections[p])
							continue;
						if(pAvant >= 0 && p == i + m_PosAsc)
							break;
						pAvant = p;
					}
					sprintf(m_tamponalloc,"%dh%02d-%dh%02d",
						m_HeureDebut + ((m_MinDebut + pAvant))/100,
						((m_MinDebut + pAvant)) % 100,
						m_HeureDebut + ((i + m_MinDebut + m_PosAsc))/100,
						((i + m_MinDebut + m_PosAsc)) % 100);
					DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE * (iAvant + 1) + (HOURY_TAILLELIGNE * (i - iAvant - 2)) / 2, m_tamponalloc, strlen(m_tamponalloc));
					iAvant = i;
				}
			}
			// le dernier libellé
			if(iAvant + 1 < i)
			{
				pAvant = -1;
				for(p = 0 ; p < DECOUPAGE_100MIN ; p++)
				{
					if(m_Selections[p])
						pAvant = p;
					if(pAvant >= 0 && p >= (i + m_PosAsc - 1))
						break;
				}
				p++;
				while(!m_Selections[p])
				{
					p++;
					if(p >= DECOUPAGE_100MIN)
						p = 0;
				}
				sprintf(m_tamponalloc,"%dH%02d-%dH%02d",
					m_HeureDebut + ((m_MinDebut + pAvant)) / 100,
					((m_MinDebut + pAvant)) % 100,
					m_HeureDebut + ((m_MinDebut + p)) / 100,
					((m_MinDebut + p)) % 100);
				DC->TextOut(HOURX_TEXTEPLAGE,HOURY_TAILLELIGNE * (iAvant + 1) + (HOURY_TAILLELIGNE * (i - iAvant - 2)) / 2, m_tamponalloc,strlen(m_tamponalloc));
			}
		}
		break;
	}
}

void CObj_Horr::DessineZoom(CDC * DC)
{
	CPen * Oldpen;
	Oldpen=DC->SelectObject(&m_Pen4B);
	DC->MoveTo(HOURX_TRAITCENTRE,m_RZoom.top+4);
	DC->LineTo(HOURX_TRAITCENTRE,m_RZoom.bottom-4);
	DC->SelectObject(Oldpen);
	DC->DrawIcon(m_RZoom.left,m_RZoom.top,m_Ico3);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void CObj_Horr::ClickInminiHeures(CPoint pt)
{
	for(short i = 0 ; i < m_NbHeure ; i++)
	{
		if(m_RposHeure[i].PtInRect(pt))
		{
			m_Hsel = i + m_Hdeb + m_Hoffset;
			if(m_Hsel > 23 + m_Hoffset)
			{
				m_Hsel = (i + m_Hdeb + m_Hoffset) - 24;
			}
			SetCapture();
			m_fCaptureH = true;
			break;
		}
	}
	InvalidateRect(NULL,0);
	return;
}

void CObj_Horr::OnLButtonDown(UINT nFlags, CPoint pt) 
{
	if(!m_fInit)	return;
	short ligne;
	CRect R, r;

	GetClientRect(&R);

	if(!R.PtInRect(pt))
	{
		return;
	}
	if(m_Style & SCROLL_HEURE)
	{
		if(m_RHeure.PtInRect(pt))
		{
			ClickInminiHeures(pt);
			return;
		}
	}
	//On teste si click dans la partie inactive (bas de l'ascenseur);
	if(m_Style & SCROLL_HEURE && ((m_Style & INVISIBLE_VSCROLL) == 0))
	{
		if(pt.x >= m_RHeure.right || (pt.y + 3) >= m_RHeure.top)
			return;
	}

	m_fCapture = false;
	if((m_Style & QH_ZOOM) && ((m_Style & 0x00F) == HOURS_MINUTES))	//Zoom
	{
		if(m_RZoom.PtInRect(pt))
		{
			m_fCapture = true;
			SetCapture();
			InvalidateRect(NULL,0);
			return;
		}
	}

	ligne = m_PosAsc + ((pt.y - 2) / HOURY_TAILLELIGNE);
	switch(m_Style & 0x00F)
	{
	case HOURS_STANDARD:
		ligne=ligne % 96;
		if((m_Style & COUPURE_OBLIGATOIRE) && ligne==0)break;
		switch(m_Selections[ligne])
		{
		case 0:	//aucune
			m_Selections[ligne]=1;
			if(ligne==0)m_Selections[96]=1;
			break;
		case 1:	//rouge
			m_Selections[ligne]=0;
			if(ligne==0)m_Selections[96]=0;
			break;
		case 2:	//bleu
			m_Selections[ligne]^=1;
			if(ligne==0)m_Selections[96]^=1;
			break;
		case 3:	//bleu et rouge
			m_Selections[ligne]^=1;
			if(ligne==0)m_Selections[96]^=1;
			break;
		}
		break;
	case MINUTES_STANDARD:
		ligne=ligne % 1440;
		if((m_Style & COUPURE_OBLIGATOIRE) && ligne==0)break;
		switch(m_Selections[ligne])
		{
		case 0:	//aucune
			m_Selections[ligne]=2;
			if(ligne==0)m_Selections[1440]=2;
			break;
		case 1:	//rouge
			m_Selections[ligne]^=2;
			if(ligne==0)m_Selections[1440]^=2;
			break;
		case 2:	//bleu
			m_Selections[ligne]=0;
			if(ligne==0)m_Selections[1440]=0;
			break;
		case 3:	//bleu et rouge
			m_Selections[ligne]^=2;
			if(ligne==0)m_Selections[1440]^=2;
			break;
		}
		break;
	case HOURS_MINUTES:
		ligne=ligne % 96;
		if((m_Style & COUPURE_OBLIGATOIRE) && ligne==0)break;
		switch(m_Selections[ligne*15])
		{
		case 0:	//aucune
			m_Selections[ligne*15]=1;
			if(ligne==0)m_Selections[1440]=1;
			break;
		case 1:	//rouge
			m_Selections[ligne*15]=0;
			if(ligne==0)m_Selections[1440]=0;
			break;
		case 2:	//bleu
			m_Selections[ligne*15]^=1;
			if(ligne==0)m_Selections[1440]^=1;
			break;
		case 3:	//bleu et rouge
			m_Selections[ligne*15]^=1;
			if(ligne==0)m_Selections[1440]^=1;
			break;
		}
		break;
	case TV100MINUTES:
		ligne = ligne % 2400;
		if((m_Style & COUPURE_OBLIGATOIRE) && ligne==0)break;
		switch(m_Selections[ligne])
		{
		case 0:	//aucune
			m_Selections[ligne] = 1;
			if(ligne == 0)m_Selections[2400] = 1;
			break;
		case 1:	//rouge
			m_Selections[ligne] = 0;
			if(ligne == 0)m_Selections[2400] = 0;
			break;
		case 2:	//bleu
			break;
		case 3:	//bleu et rouge
			break;
		}
		break;
	}
	OnSelectionChanged(m_Selections);
	InvalidateRect(NULL,0);
	CStatic::OnLButtonDown(nFlags, pt);
}

void CObj_Horr::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point) ;
	CStatic::OnLButtonDblClk(nFlags, point);
}


void CObj_Horr::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_fInit)	return;
	CRect R;
	GetClientRect(&R);

	if(!R.PtInRect(point))	return;
	if((m_Style & QH_ZOOM) && ((m_Style & 0x00F) == HOURS_MINUTES))	//Zoom
	{
		if(m_fCapture)
		{
			//limite haut et bas
			if(point.y < R.top + (HOURY_TAILLELIGNE - 1)
			|| point.y > R.bottom - (HOURY_TAILLELIGNE + 1))
			{
				return;
			}
			m_PosZoom=(point.y - 2) / HOURY_TAILLELIGNE;
			if(point.y < m_RZoom.top)	//on monte
			{
				long y = (m_RZoom.top - point.y) + HOURY_TAILLELIGNE + (HOURY_TAILLELIGNE / 2) - HOURY_ICOCENTREFLECHE;
				m_RZoom.top -= y;
				m_RZoom.bottom = m_RZoom.top + 24;
			}
			else	//on descend
			{
				long y=(point.y-m_RZoom.top)-HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE;
				m_RZoom.top+=y;
				m_RZoom.bottom=m_RZoom.top+24;
			}
			OnZoom(m_PosZoom+m_PosAsc);
			InvalidateRect(NULL,0);
		}
	}
	if(m_Style & SCROLL_HEURE)	//on bouge le scroll des heures
	{
		if(!m_RHeure.PtInRect(point))	return;
		if(m_fCaptureH)
		{
			short pos = -1;
			//Quelle heure est pointé actuellement?
			if(m_Hdeb <= m_Hsel)
			{
				pos = m_Hsel - m_Hdeb - m_Hoffset;
			}
			else
			{
				pos = 24 - m_Hdeb + m_Hsel - m_Hoffset;
			}

			//on va à gauche ou à droite
			if(point.x + 3 <= m_RposHeure[pos].left)	//"on gauche"
			{
				if(!m_RposHeure[pos].PtInRect(point))
				{
					//ya t'il qqchose après?
					if(m_Hdeb >= 23 + m_Hoffset)
					{
						m_Hdeb = m_Hoffset;
					}
					else
					{
						m_Hdeb++;
					}
				}
			}
			else
			{
				if(point.x - 3 >= m_RposHeure[pos].right)	//"on droite"
				{
					if(!m_RposHeure[pos].PtInRect(point))
					{
						//ya t'il qqchose après?
						if(m_Hdeb <= 0)
						{
							m_Hdeb = 23 + m_Hoffset;
						}
						else
						{
							m_Hdeb--;
						}
					}
				}
			}
			InvalidateRect(NULL,0);
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CObj_Horr::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_fInit)	return;
	CRect R;
	GetClientRect(&R);

	if(!R.PtInRect(point) && m_fCapture)
	{
		if((m_Style & QH_ZOOM) && ((m_Style & 0x00F) == HOURS_MINUTES))	//Zoom
		{
			m_RZoom.top=m_PosZoom*HOURY_TAILLELIGNE-1;
			if(m_RZoom.top>R.bottom-(HOURY_TAILLELIGNE+1))
				m_RZoom.top=(m_PosZoom-1)*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE;
			m_RZoom.bottom=m_RZoom.top+24;
		}
	}
	else
	{
		if((m_Style & QH_ZOOM) && ((m_Style & 0x00F) == HOURS_MINUTES))	//Zoom
		{
			//on place le zoom entre deux graduation
			m_PosZoom=(point.y-2)/HOURY_TAILLELIGNE;
			m_RZoom.top=m_PosZoom*HOURY_TAILLELIGNE-1;
			if(m_RZoom.top>R.bottom-(HOURY_TAILLELIGNE+1))
				m_RZoom.top=(m_PosZoom-1)*HOURY_TAILLELIGNE+(HOURY_TAILLELIGNE/2)-HOURY_ICOCENTREFLECHE;
			m_RZoom.bottom=m_RZoom.top+24;
			OnZoom(m_PosZoom+m_PosAsc);
		}
	}
	if((m_Style & SCROLL_HEURE)  && m_fCaptureH)
	{
		int maxi= 0;
		int pos	= 0;

		switch(m_Style & 0x00F)
		{
		case HOURS_STANDARD:
		case HOURS_MINUTES:
			maxi = DECOUPAGE_HOUR - m_NbLigne + 1;
			pos = (m_Hsel - m_Hoffset) * 4;
			break;
		case MINUTES_STANDARD:
			maxi = DECOUPAGE_MIN - m_NbLigne + 1;
			pos = (m_Hsel - m_Hoffset) * 60;
			break;
		case TV100MINUTES:
			maxi = DECOUPAGE_100MIN - m_NbLigne + 1;
			pos = (m_Hsel - m_Hoffset) * 100;
			break;
		}
		SetPosition(pos <= maxi ? pos : maxi, true);
	}
	m_fCapture=false;
	m_fCaptureH=false;
	ReleaseCapture();
	InvalidateRect(NULL, 0);
	CStatic::OnLButtonUp(nFlags, point);
}

void CObj_Horr::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{		
	if(m_Style & INVISIBLE_VSCROLL)	return;//pas d'ascenseur
	int i,mini,maxi;

	i=m_ScrollBar->GetScrollPos();
	m_ScrollBar->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_BOTTOM: 	m_ScrollBar->SetScrollPos(maxi,1); break;
		case SB_ENDSCROLL: 	break;
		case SB_LINEDOWN: 	m_ScrollBar->SetScrollPos(min(i+1,maxi),1); break;
		case SB_LINEUP:		m_ScrollBar->SetScrollPos(max(i-1,mini),1); break;
		case SB_PAGEDOWN:	m_ScrollBar->SetScrollPos(min(i+(m_NbLigne-1),maxi),1); break;
		case SB_PAGEUP:		m_ScrollBar->SetScrollPos(max(i-(m_NbLigne-1),mini),1); break;
		case SB_TOP:		m_ScrollBar->SetScrollPos(mini,1); break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_ScrollBar->SetScrollPos(nPos,1); break;
	}

	// au besoin on redessine le bouton à qui il est associé
	if((m_PosAsc = m_ScrollBar->GetScrollPos()) == i)
		return;
	if(m_Style & SCROLL_HEURE)
	{
		switch(m_Style & 0x00F)
		{
		case HOURS_STANDARD:
			m_Hdeb = (m_PosAsc / 4) - (m_NbHeure / 2);
			if(m_Hdeb < 0)	m_Hdeb += 24;
			m_Hsel = m_Hdeb + m_Hoffset + (m_NbHeure / 2);
			break;
		case MINUTES_STANDARD:
		case HOURS_MINUTES:
			m_Hdeb = (m_PosAsc / 60) - (m_NbHeure / 2);
			if(m_Hdeb < 0)	m_Hdeb += 24;
			m_Hsel = m_Hdeb + m_Hoffset + (m_NbHeure / 2);
			break;
		case TV100MINUTES:
			m_Hdeb = (m_PosAsc / 100) - (m_NbHeure / 2);
			if(m_Hdeb < 0)	m_Hdeb += 24;
			m_Hsel = m_Hdeb + m_Hoffset + (m_NbHeure / 2);
			break;
		}
	}
	if((m_Style & QH_ZOOM) && ((m_Style & 0x00F) == HOURS_MINUTES))
		OnZoom(m_PosZoom+m_PosAsc);
	InvalidateRect(NULL,0);
	CStatic::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CObj_Horr::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(!(m_Style & INVISIBLE_VSCROLL))
	{
		if(zDelta>0)OnVScroll(SB_LINEDOWN,0,m_ScrollBar);
		if(zDelta<0)OnVScroll(SB_LINEUP,0,m_ScrollBar);
	}	
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

void CObj_Horr::OnZoom(int QH)
{
}

void CObj_Horr::OnSelectionChanged(char * Selections)
{
}
