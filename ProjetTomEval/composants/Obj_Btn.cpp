// Obj_Btn.cpp : implementation file
//
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Btn v1.3
Tâche		: Bouton avec icone, texte, couleur
Type 		: Nouvelles fonctions + Amelioration
Fichiers	: Obj_Btn.cpp, Obj_Btn.h
Auteur		: VLF
Date		: 10/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "stdafx.h"
#include "Obj_Btn.h"
#include "Obj_Btn.rch"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HICON * CObj_Btn::g_LoadIcons=NULL;							// Tableau des handles des icones chargés
short CObj_Btn::g_nbLoadIcons=0;

/////////////////////////////////////////////////////////////////////////////
// Obj_Btn

CObj_Btn::CObj_Btn()
{
	m_Style=STYLE_STANDARD;
	m_Param=FORMAT_STANDARD;
	m_Etat=0;

	m_BFond=new CBrush(::GetSysColor(COLOR_BTNFACE)); // RGB(192,192,192);
	m_BFondEnfonce=new CBrush(::GetSysColor(COLOR_BTNFACE)); // RGB(192,192,192);
	m_Font=new CFont;
	m_Font->CreatePointFont(-8, "MS Sans Serif", NULL);

	m_BNoir=NULL;
	m_BGrisC=NULL;

	m_PGrisF=new CPen(PS_SOLID,1,RGB(128,128,128));
	m_PBlanc=new CPen(PS_SOLID,1,RGB(255,255,255));
	m_PNoir=new CPen(PS_SOLID,1,RGB(0,0,0));
	m_PGrisC=new CPen(PS_SOLID,1,RGB(192,192,192));

	m_Action=0;
	m_fAction=0;
	m_fOnLampe=0;
	m_fCapture=0;
	m_SBitmap=NULL;
	InitTabIcons();
}

CObj_Btn::~CObj_Btn()
{
	if(m_BFond)	delete m_BFond;
	if(m_BFondEnfonce)	delete m_BFondEnfonce;
	if(m_BNoir)	delete m_BNoir;
	if(m_BGrisC)delete m_BGrisC;
	if(m_PGrisF)delete m_PGrisF;
	if(m_PBlanc)delete m_PBlanc;
	if(m_PNoir)	delete m_PNoir;
	if(m_PGrisC)delete m_PGrisC;

	if(m_Font)	delete m_Font;
	if(g_LoadIcons)
	{
		if(g_nbLoadIcons>1)	g_nbLoadIcons--;
		else
		{
/*			// Note de rémi:
			// La destruction des icones semble inutile...
			for(short i=0 ; i<NBICONES ; i++)
			{
				if(g_LoadIcons[i]==NULL)continue;
				DestroyIcon(g_LoadIcons[i]);
			}*/
			delete [] g_LoadIcons;
			g_LoadIcons=NULL;
			g_nbLoadIcons=0;
		}
	}
}


BEGIN_MESSAGE_MAP(CObj_Btn, CButton)
	//{{AFX_MSG_MAP(CObj_Btn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Btn message handlers

void CObj_Btn::InitTabIcons(void)
{
	g_nbLoadIcons++;
	if(!g_LoadIcons)
	{
		g_LoadIcons=new HICON[NBICONES];
		for(short i=0 ; i<NBICONES ; i++)	g_LoadIcons[i]=NULL;
	}

	m_TabIcons[ 0] = IDI_00LEDTRANS;
	m_TabIcons[ 1] = IDI_01LEDBLEU;
	m_TabIcons[ 2] = IDI_02LEDMARRON;
	m_TabIcons[ 3] = IDI_03LEDORANGE;
	m_TabIcons[ 4] = IDI_04LEDROUGE;
	m_TabIcons[ 5] = IDI_05LEDVERTBOUTEIL;
	m_TabIcons[ 6] = IDI_06LEDVERTE;
	m_TabIcons[ 7] = IDI_07LEDVERTESELECT;
	m_TabIcons[ 8] = IDI_08LEDVIDE;
	m_TabIcons[ 9] = IDI_09LEDVIOLET;
	m_TabIcons[10] = IDI_PRINTER;
	m_TabIcons[11] = IDI_10LEDNUM1;
	m_TabIcons[12] = IDI_11LEDVERTENUM1;
	m_TabIcons[13] = IDI_12LEDNUM2;
	m_TabIcons[14] = IDI_13LEDVERTENUM2;
	m_TabIcons[15] = IDI_14CROIXEXIT;
	m_TabIcons[16] = IDI_EXPORT;
}

void CObj_Btn::Initialise(short Style, short param, COLORREF bkcolor, CFont * Font)
{
	if(Style==STYLE_LAMPELUMINEUX){
		Style=STYLE_LAMPE;
		if(bkcolor)
		{
			delete m_BFondEnfonce;
			m_BFondEnfonce = new CBrush(bkcolor);
			bkcolor=0;
		}
	}

	if(Style==STYLE_LUMINEUX)
	{
		Style=STYLE_STANDARD;
		if(bkcolor)
		{
			delete m_BFondEnfonce;
			m_BFondEnfonce = new CBrush(bkcolor);
			bkcolor=0;
		}
	}

	m_Style=Style;
	m_Param=param;
	
	if(bkcolor)
	{
		delete m_BFond;
		m_BFond = new CBrush(bkcolor);
		delete m_BFondEnfonce;
		m_BFondEnfonce = new CBrush(bkcolor);
	}
	//font
	if(Font)	m_Font=Font;
	GetClientRect(m_RClient);

	// les pinceaux
	if(!m_BNoir)	m_BNoir=new CBrush(RGB(0,0,0));
	if(!m_BGrisC)	m_BGrisC=new CBrush(RGB(192,192,192));

	// init  des icones
	switch(m_Style)
	{
	case STYLE_LAMPE:
		if(!g_LoadIcons[8]){
			m_IcoLedTrans=AfxGetApp()->LoadIcon(IDI_08LEDVIDE);
			g_LoadIcons[8]=m_IcoLedTrans;
		}
		else	m_IcoLedTrans=g_LoadIcons[8];
		if(!g_LoadIcons[7]){
			m_IcoOnLed=AfxGetApp()->LoadIcon(IDI_07LEDVERTESELECT);
			g_LoadIcons[7]=m_IcoOnLed;
		}
		else	m_IcoOnLed=g_LoadIcons[7];
		if(!g_LoadIcons[6]){
			m_IcoLedSelect=AfxGetApp()->LoadIcon(IDI_06LEDVERTE);
			g_LoadIcons[6]=m_IcoLedSelect;
		}
		else	m_IcoLedSelect=g_LoadIcons[6];

		break;
	case STYLE_PRINT:
		if(!g_LoadIcons[10]){
			m_IcoStd=AfxGetApp()->LoadIcon(IDI_PRINTER);
			g_LoadIcons[10]=m_IcoStd;
		}
		else	m_IcoStd=g_LoadIcons[10];
		break;
	case STYLE_EXPORT:
		if(!g_LoadIcons[16]){
			m_IcoStd=AfxGetApp()->LoadIcon(IDI_EXPORT);
			g_LoadIcons[16]=m_IcoStd;
		}
		else	m_IcoStd=g_LoadIcons[16];
		break;
	case STYLE_EXIT:
		if(!g_LoadIcons[15]){
			m_IcoStd=AfxGetApp()->LoadIcon(IDI_14CROIXEXIT);
			g_LoadIcons[15]=m_IcoStd;
		}
		else	m_IcoStd=g_LoadIcons[15];
		break;
	}
}

BOOL CObj_Btn::SetBtnState(short etat)
{
	BOOL f=1;
	m_Etat=etat;
	switch(m_Style)
	{
	case STYLE_LAMPE:
		if(m_Etat<0 || m_Etat>(ETAT_ALLUME|ETAT_ENFONCE)){f=0;	m_Etat=0;}
		break;
	}
	if(f)	InvalidateRect(NULL, 0);
	return f;
}

short CObj_Btn::GetBtnState()
{
	return m_Etat;
}

short CObj_Btn::ChangeColorLampe(short NumLedColor)		//couleur 0 à 10
{
	if(NumLedColor<0 || NumLedColor>9)	return FALSE;
	if(!g_LoadIcons[NumLedColor]){
		m_IcoLedSelect=AfxGetApp()->LoadIcon(m_TabIcons[NumLedColor]);
		g_LoadIcons[NumLedColor]=m_IcoLedSelect;
	}
	else	m_IcoLedSelect=g_LoadIcons[NumLedColor];
	return TRUE;
}

void CObj_Btn::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect R, Rdest;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	long l, h;

	// copie de l'écran en mémoire ****************
	GetClientRect(&R);
	DC.CreateCompatibleDC(&dc);
	l=R.Width();
	h=R.Height();

	hBmpNew.CreateCompatibleBitmap(&dc, l,h);
	hBmpOld=(CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);
	/**********************************************/

	DC.SetBkMode(TRANSPARENT);
	DC.SelectObject(m_Font);

	//dessin du bouton
	if(m_Etat&ETAT_ENFONCE)DC.FillRect(&R, m_BFondEnfonce);
	else DC.FillRect(&R, m_BFond);
	switch(m_Style)
	{
	case STYLE_LAMPE:
		{
		DessineBtn(&DC, &R);
		//calcul des rectangles icones et textes
		short x=R.left+4;
		short y=R.top+((R.Height()-8)/2)+1;
		// Ajustement avec -1 des libellés et de l'icone...
		// moi pas comprendre ce qu'a voulu faire vince.lf..
		// il me semble que dans la version que j'avais fais autre chose...
		m_RIcone.SetRect(x, y-1, x+8, y+R.Height()-1);
		m_RText.SetRect(x+5, R.top, R.right, R.bottom-1);
		DessineIconeLampe(&DC);
		}
		break;
	case STYLE_EXIT:
		{
		if(!m_fAction)	DessineRect4Pen(&DC, &R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
		else{
			DessineRect4Pen(&DC, &R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
			R.OffsetRect(1, 1);
		}
		short x=(R.Width()-6)/2+R.left;
		short y=(R.Height()-5)/2+R.top;
		m_RIcone.SetRect(x, y, x+R.Width(), y+R.Height());
		m_RText=R;
		DC.DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoStd);
		}
		break;
	case STYLE_PRINT:
		{
		if(!m_fAction)	DessineRect4Pen(&DC, &R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
		else{
			DessineRect4Pen(&DC, &R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
			R.OffsetRect(1, 1);
		}
		short x=R.left+4;
		short y=R.top+((R.Height()-10)/2);
		m_RIcone.SetRect(x, y, x+13, y+9);
		m_RText.SetRect(x+13, R.top, R.right, R.bottom);
		DC.DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoStd);
		}
		break;
	case STYLE_EXPORT:
		{
		if(!m_fAction)	DessineRect4Pen(&DC, &R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
		else{
			DessineRect4Pen(&DC, &R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
			R.OffsetRect(1, 1);
		}
		short x=R.left+4;
		short y=R.top+((R.Height()-12)/2);
		m_RIcone.SetRect(x, y, x+13, y+12);
		m_RText.SetRect(x+13, R.top, R.right, R.bottom);
		DC.DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoStd);
		}
		break;
	case STYLE_ACINEMA:
		{
			if(!m_fAction)
			{
				if(!m_Etat)
				{
					DC.FillRect(&R, m_BGrisC);
					DessineRect4Pen(&DC, &R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
				}
				else
				{
					DessineRect4Pen(&DC, &R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
					R.OffsetRect(1, 1);
				}
			}
			else
			{
				DC.FrameRect(R, m_BNoir);
				R.OffsetRect(1, 1);
			}
			m_RText=R;
		}
		break;
	case STYLE_STANDARD:
		if(m_Param & FORMAT_STANDARD)
		{
			if(!m_fAction)
			{
				DessineRect4Pen(&DC, &R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
			}
			else
			{
				DessineRect4Pen(&DC, &R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
				R.OffsetRect(1, 1);
			}
		}
		else
		if(m_Param & FORMAT_2STATES)	DessineBtn(&DC, &R);
		m_RText=R;
	}
	//dessin du texte
	CString txt;
	GetWindowText(txt);
	if(!txt.IsEmpty())
	{
		if(IsWindowEnabled()) DC.DrawText(txt, &m_RText, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		else
		{
//			DC.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
//			int i=DC.SetTextAlign(TA_CENTER);
			DC.DrawText(txt, &m_RText, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			CRect Rect=m_RText;
			DC.DrawText(txt,&Rect,DT_CALCRECT|DT_CENTER|DT_VCENTER);
			CSize sz1=m_RText.Size();
			CSize sz2=Rect.Size();
			sz1-=sz2;
			sz1.cx/=2;
			sz1.cy/=2;
			CPoint pt=m_RText.TopLeft();
			pt.x+=sz1.cx;
			pt.y+=sz1.cy;
			CSize sz=m_RText.Size();//DC.GetTextExtent(txt);
//			DC.DrawState(pt,sz,hBmpNew,DST_BITMAP|DSS_DISABLED,(HBRUSH)NULL);
			DC.DrawState(pt,sz,(DRAWSTATEPROC)NULL,(LPARAM)(LPCSTR)txt,DST_TEXT|DSS_DISABLED,(HBRUSH)NULL);
//			DC.SetTextAlign(i);
		}
	}

	// copy de la mem sur ecran
	Rdest=dc.m_ps.rcPaint;
	dc.BitBlt(Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,&DC,Rdest.left,Rdest.top,SRCCOPY);
	dc.SelectObject(hBmpOld);
	DC.DeleteDC();
	// ************************
}

void CObj_Btn::DessineBtn(CDC * dc, CRect * R)
{
	short Action=m_Action, Etat=m_Etat;
	if(Action & ACTION_SHIFT)	Action^=ACTION_SHIFT;
	if(Action & ACTION_CONTROL)	Action^=ACTION_CONTROL;
	if(Etat & ACTION_SHIFT)		Etat^=ACTION_SHIFT;
	if(Etat & ACTION_CONTROL)	Etat^=ACTION_CONTROL;

	if(!m_fAction)	//pas de changement d'état en cours
	{
		switch(Etat)
		{
		case 0:				//éteind et levé
			DessineRect4Pen(dc, R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
			break;
		case ETAT_ENFONCE:	//éteind et enfoncé
			DessineRect4Pen(dc, R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
			R->OffsetRect(1, 1);
			break;
		case ETAT_ALLUME:	//allumé et levé
			DessineRect4Pen(dc, R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
			break;
		case ETAT_ENFONCE|ETAT_ALLUME:	//allumé et enfoncé
			if(m_Style==STYLE_LAMPE)
			{
				DessineRect4Pen(dc, R, m_PNoir, m_PGrisF, m_PGrisC, m_PBlanc);
				R->OffsetRect(1, 1);
			}
			break;
		}
	}
	else
	{
		switch(Action)
		{
		case 0:				//éteind et levé
			if(Etat!=ETAT_ALLUME)
			{
				dc->FillRect(R, m_BFond);
				dc->FrameRect(R, m_BNoir);
				R->OffsetRect(1, 1);
			}
			else
			{
				DessineRect4Pen(dc, R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
			}
			break;
		case ACTION_ALLUME:	//allumé et levé
			if(Etat==ETAT_ENFONCE)
			{
				dc->FillRect(R, m_BFond);
				dc->FrameRect(R, m_BNoir);
				R->OffsetRect(1, 1);
			}
			else
			{
				DessineRect4Pen(dc, R, m_PBlanc, m_PGrisC, m_PGrisF, m_PNoir);
			}
			break;
		case ACTION_ENFONCE:	//éteind et enfoncé
			dc->FillRect(R, m_BFond);
			dc->FrameRect(R, m_BNoir);
			R->OffsetRect(1, 1);
			break;
		case ACTION_ENFONCE|ACTION_ALLUME:			//allumé et enfonce
			if(m_Style==STYLE_LAMPE)
			{
				dc->FillRect(R, m_BFond);
				dc->FrameRect(R, m_BNoir);
				R->OffsetRect(1, 1);
			}
			break;
		}
	}
}

void CObj_Btn::DessineIconeLampe(CDC * dc)
{
	short Action=m_Action, Etat=m_Etat;
	if(Action & ACTION_SHIFT)	Action^=ACTION_SHIFT;
	if(Action & ACTION_CONTROL)	Action^=ACTION_CONTROL;
	if(Etat & ACTION_SHIFT)		Etat^=ACTION_SHIFT;
	if(Etat & ACTION_CONTROL)	Etat^=ACTION_CONTROL;

	//dessin de l'îcone
	if(!m_fAction)
	{
		switch(Etat&ETAT_ALLUME)
		{
		case 0:				//éteind
			dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoLedTrans);
			break;
		case ETAT_ALLUME:	//allumé
			dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoLedSelect);
			break;
		}
	}
	else
	{
		switch(Action)
		{
		case 0:				//éteind et levé
			if(m_Etat & ETAT_ALLUME)dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoOnLed);
			else					dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoLedTrans);
			break;
		case ACTION_ALLUME:	//allumé et levé
		case ACTION_ENFONCE:	//éteind et enfoncé
			dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoOnLed);
			break;
		case ACTION_ENFONCE|ACTION_ALLUME:			//allumé et enfoncé	(PAS DANS PIGE)
			if(Etat==ETAT_ALLUME)dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoLedSelect);
			else					dc->DrawIcon(m_RIcone.left, m_RIcone.top, m_IcoOnLed);
			break;
		}
	}
}

void CObj_Btn::OnLButtonDblClk(UINT nFlags, CPoint point)
{
//	CObj_Btn::OnLButtonDown(nFlags, point);
}

void CObj_Btn::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_fAction=1;
	m_fCapture=1;
	m_Action=m_Etat;
	short Action=m_Action, Etat=m_Etat;
	if(Action & ACTION_SHIFT)	Action^=ACTION_SHIFT;
	if(Action & ACTION_CONTROL)	Action^=ACTION_CONTROL;
	if(Etat & ACTION_SHIFT)		Etat^=ACTION_SHIFT;
	if(Etat & ACTION_CONTROL)	Etat^=ACTION_CONTROL;

	switch(m_Style)
	{
	case STYLE_LAMPE:

		switch (m_Param)
		{
			case FORMAT_LIBRE:
				if(m_RIcone.PtInRect(point))	//click sur lampe
				{
					m_Action=Etat^ETAT_ALLUME;
					m_fOnLampe=1;
				}
				else
				{
					m_Action=Etat^ACTION_ENFONCE;
					m_fOnLampe=0;
				}
				if(nFlags & MK_SHIFT)	m_Action|=MK_SHIFT;
				if(nFlags & MK_CONTROL)	m_Action|=MK_CONTROL;
				break;
			break;
			case FORMAT_EXCLUSIF:
				if(m_RIcone.PtInRect(point))	//click sur lampe
				{
					switch(Etat&ACTION_ALLUME)
					{
					case 0:						//éteind et levé
						m_Action=ACTION_ALLUME;
						break;
					case ETAT_ALLUME:			//allumé et levé
						m_Action=0;
						break;
					}
					m_fOnLampe=1;
				}
				else
				{

					switch(Etat&ETAT_ENFONCE)
					{
					case 0:							//éteind et levé
						m_Action=ETAT_ENFONCE;
						break;
					case ETAT_ENFONCE:				//éteind et enfoncé
						m_Action=0;
						break;
					}
					m_fOnLampe=0;
				}
				if(nFlags & MK_SHIFT)	m_Action|=MK_SHIFT;
				if(nFlags & MK_CONTROL)	m_Action|=MK_CONTROL;
			break; // FORMAT_EXCLUSIF
			default:
				if(m_RIcone.PtInRect(point))	//click sur lampe
				{
					switch(Etat)
					{
					case 0:						//éteind et levé
						m_Action=ACTION_ALLUME;
						break;
					case ETAT_ALLUME:			//allumé et levé
						m_Action=0;
						break;
					}
					m_fOnLampe=1;
				}
				else
				{

					switch(Etat)
					{
					case 0:							//éteind et levé
					case ETAT_ALLUME:				//allumé et levé
						m_Action=ACTION_ENFONCE|ACTION_ALLUME;
						break;
					case ETAT_ENFONCE:				//éteind et enfoncé
					case ETAT_ENFONCE|ETAT_ALLUME:	//allumé et enfoncé	(PAS DANS PIGE)
						m_Action=0;
						break;
					}
					m_fOnLampe=0;
				}
				if(nFlags & MK_SHIFT)	m_Action|=MK_SHIFT;
				if(nFlags & MK_CONTROL)	m_Action|=MK_CONTROL;
			break; // default
		}
		break;

	case STYLE_ACINEMA:
	case STYLE_STANDARD:
		switch(Etat)
		{
		case 0:						//levé
			m_Action=ACTION_ENFONCE;
			break;
		case ETAT_ENFONCE:			//enfoncé
			m_Action=0;
			break;
		}
		break;
	}
	InvalidateRect(NULL, 0);
	CButton::OnLButtonDown(nFlags, point);
}

void CObj_Btn::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch(m_Style)
	{
	case STYLE_LAMPE:
		if(m_fAction)	m_Etat=m_Action;
		m_fOnLampe=0;
		break;
	case STYLE_ACINEMA:
	case STYLE_STANDARD:
		if(m_fAction)	m_Etat=m_Action;
		break;
	}
	m_fAction=0;
	m_fCapture=0;
	InvalidateRect(NULL, 0);
	CButton::OnLButtonUp(nFlags, point);
}

void CObj_Btn::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_fCapture)	return;
	if(nFlags & MK_LBUTTON)
	{
		switch(m_Style)
		{
		case STYLE_LAMPE:
			if(m_RClient.PtInRect(point))		//si la souris est dans la zone cliquable
			{
				if(!m_RIcone.PtInRect(point))
				{
					if(m_fOnLampe)	m_fAction=0;
					else			m_fAction=1;
				}
				else	m_fAction=1;
			}
			else	m_fAction=0;
			break;
		default:
//		case STYLE_PRINT:
			if(!m_RClient.PtInRect(point))		//si la souris est en dehors de la zone cliquable
				m_fAction=0;
			else
				m_fAction=1;
			break;
		}
		InvalidateRect(NULL, 0);
	}
	CButton::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// un demicadre, 1 trait
void CObj_Btn::DessineDemiRect(CDC * DC, CRect  R, CPen * hP1)
{
	CPen * OldPen;
	OldPen=DC->SelectObject(hP1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.right,R.top);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.left,R.bottom);
	DC->SelectObject(OldPen);
}

// c'est un double cadre de deux couleurs
void CObj_Btn::DessineRect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2)
{
	CPen * OldPen;
	// pour faire comme un FrameRect
	R.right--;
	R.bottom--;

	// Dessin...
	OldPen=DC->SelectObject(hP1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.right,R.top);
	DC->MoveTo(R.left,R.top+1);
	DC->LineTo(R.right-1,R.top+1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.left,R.bottom);
	DC->MoveTo(R.left+1,R.top);
	DC->LineTo(R.left+1,R.bottom-1);

	DC->SelectObject(hP2);
	DC->MoveTo(R.right,R.top);
	DC->LineTo(R.right,R.bottom+1);
	DC->MoveTo(R.right-1,R.top+1);
	DC->LineTo(R.right-1,R.bottom+1);
	DC->MoveTo(R.left,R.bottom);
	DC->LineTo(R.right+1,R.bottom);
	DC->MoveTo(R.left+1,R.bottom-1);
	DC->LineTo(R.right+1,R.bottom-1);
	DC->SelectObject(OldPen);
}

// c'est un double cadre de 4 couleurs
void CObj_Btn::DessineRect4Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2, CPen * hP3, CPen * hP4)
{
	CPen * OldPen;
	// pour faire comme un FrameRect
	R.right--;
	R.bottom--;
	// Dessin...
	OldPen=DC->SelectObject(hP1);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.right,R.top);
	DC->MoveTo(R.left,R.top);
	DC->LineTo(R.left,R.bottom+1);

	DC->SelectObject(hP2);
	DC->MoveTo(R.left+1,R.top+1);
	DC->LineTo(R.right,R.top+1);
	DC->MoveTo(R.left+1,R.top+1);
	DC->LineTo(R.left+1,R.bottom);

	DC->SelectObject(hP3);
	DC->MoveTo(R.left+1,R.bottom-1);
	DC->LineTo(R.right-1,R.bottom-1);
	DC->MoveTo(R.right-1,R.bottom-1);
	DC->LineTo(R.right-1,R.top);

	DC->SelectObject(hP4);
	DC->MoveTo(R.left,R.bottom);
	DC->LineTo(R.right,R.bottom);
//	DC->MoveTo(R.right,R.bottom);
	DC->LineTo(R.right,R.top-1);
	DC->SelectObject(OldPen);
}

void CObj_Btn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//ne fait rien...le dessin est fait au niveau du Paint!
}

void CObj_Btn::SetBrushFond(COLORREF colorref, bool fUpdate)
{
	delete m_BFond;
	m_BFond= new CBrush(colorref);
	if(fUpdate)
	{
		Invalidate();
		UpdateWindow();
	}
}

void CObj_Btn::SetBrushFondEnfonce(COLORREF colorref, bool fUpdate)
{
	delete m_BFondEnfonce;
	m_BFondEnfonce = new CBrush(colorref);
	if(fUpdate)
	{
		Invalidate();
		UpdateWindow();
	}
}
