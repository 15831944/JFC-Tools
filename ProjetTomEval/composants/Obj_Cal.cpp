// Obj_Cal.cpp : implementation file
//
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Cal v7.7a
Tâche		: Affichage d'un calendrier horizontal jour/semaine/mois/annee paramétrable
Type 		: Amelioration
Fichiers	: Obj_Cal.cpp, Obj_Cal.h
Auteur		: VLF
Date		: 24/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "stdafx.h"
#include "Obj_Cal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Cal

CObj_Cal::CObj_Cal()
{
//	AlloueCalend();

	m_CoupureCalend = NULL;
	m_SCalend		= NULL;
	m_nbmois		= 0;
	m_LibMois		= NULL;
	m_nbjour		= 0;
	m_LibJour		= NULL;
	m_nbcurseur		= 0;
	m_ArrayCurseur	= NULL;
	m_LeftSelect	= NULL;
	m_RightSelect	= NULL;
	
	m_pMSSansSerif	= NULL;
	m_pTimesNewRoman = NULL;
}

CObj_Cal::~CObj_Cal()
{
	LibereCalend();
}

void CObj_Cal::AlloueCalend(void)
{
	m_nbcases=0;			m_nbjour=0;				m_nbmois=0;			m_nbcurseur=0;
	m_nbcasesvisu=0;		m_posactive=0;
	m_fAlignSem=0;
	m_FormatMois=0;			m_FormatSem=0;			m_FormatJour=0;		m_FormatAnnee = 0;
	m_dxMois=0;				m_dxSem=0;				m_dxJour=0;			m_dx=0;		m_dxAnnee = 0;
	m_dyMois=0;				m_dySem=0;				m_dyJour=0;			m_dyAnnee = 0;
	m_yMois=0;				m_ySem=0;				m_yJour=0;			m_yAnnee = 0;
	m_numOrdreMois=-1;		m_numOrdreSem =-1;		m_numOrdreJour=-1;	m_numOrdreAnnee = -1;
	m_fInit=false;			m_fCapture=0;			m_fCursor=0;
	m_fSurCapture=0;		m_fSelIndep = false;	m_fLBCapture = false;
	m_fRBCapture = false;
	m_fTablier=0;			m_fTablierCreuset=0;	m_fStyle=0;			m_fSelect=0;
	m_SelectRules=0;
	
	m_colorSel=RGB(255, 255, 0);
	m_colorSursel=RGB(0, 255, 255);

	m_fClickLevel=0;
	m_CoupureCalend=NULL;	m_SCalend=NULL;			m_SCurseur=NULL;
	m_LibMois=NULL;			m_LibJour=NULL;
	m_LeftSelect = NULL;	m_RightSelect = NULL;	m_CurrSelect = NULL;

	m_pGrille=NULL;

	m_pMSSansSerif	= new CFont;
	m_pTimesNewRoman = new CFont;
	m_pMSSansSerif->CreateFont(-8, 0, 0, 0,FW_NORMAL,0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"MS Sans Serif");         /* and face name only */
	m_pTimesNewRoman->CreateFont(-12, 0, 0, 0,FW_NORMAL,0, 0, 0, 0, 0, 0, 0,(BYTE)(VARIABLE_PITCH | FF_SWISS),"Times New Roman");         /* and face name only */
	m_ArrayCurseur	= new CPtrArray;
	m_hCursorArrow	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorSizewe = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_PrntDC=NULL;
}

void CObj_Cal::LibereCalend(void)
{
	if(m_CoupureCalend)					delete m_CoupureCalend;		m_CoupureCalend = NULL;
	if(m_SCalend)						delete m_SCalend;			m_SCalend		= NULL;
	for(short i=0 ; i<m_nbmois ; i++)	delete [] m_LibMois[i];
	if(m_LibMois)						delete [] m_LibMois;		m_LibMois		= NULL;
	for(i=0 ; i<m_nbjour ; i++)			delete [] m_LibJour[i];
	if(m_LibJour)						delete [] m_LibJour;		m_LibJour		= NULL;
	for(i=m_nbcurseur-1 ; i>=0 ; i--)	DelCurseur(i);
	if(m_ArrayCurseur)					delete m_ArrayCurseur;		m_ArrayCurseur	= NULL;
	if(m_LeftSelect)					delete m_LeftSelect;		m_LeftSelect	= NULL;
	if(m_RightSelect)					delete m_RightSelect;		m_RightSelect	= NULL;
	
	if(m_pMSSansSerif)					delete m_pMSSansSerif;		m_pMSSansSerif	= NULL;
	if(m_pTimesNewRoman)				delete m_pTimesNewRoman;	m_pTimesNewRoman = NULL;

	m_PenMois.DeleteObject();
	m_PenJour.DeleteObject();
	m_PenSem.DeleteObject();

	m_fInit		= 0;
	m_nbmois	= 0;
	m_nbjour	= 0;
	m_nbcurseur = 0;

	RAZSelectionIndep();
}


BEGIN_MESSAGE_MAP(CObj_Cal, CStatic)
	//{{AFX_MSG_MAP(CObj_Cal)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Cal message handlers

void CObj_Cal::Initialise(short Style, COleDateTime JourDeb, short NbCases, CScrollBar  * ptScroll,
						CFont *ptFont, short DefPos, CRect * pR_Print)
{
	//Initialise les variables et les objets(pen, font, ..)
	AlloueCalend();

	m_Style=Style;
	m_jourdeb=JourDeb;
	m_nbcases=NbCases;
	m_ptscroll=ptScroll;
	m_posactive=DefPos;
	m_ptFont=ptFont;
	if(m_ptFont==NULL)	m_ptFont=m_pMSSansSerif;

	if(m_Style & OPTION_CAL_PRINTER)
	{
		m_PrntRect=*pR_Print;
	}
	if((m_Style & 0x0f)==STYLE_CAL_PIGE)
	{
		m_Style|=(OPTION_CAL_SPLITGRAY|OPTION_CAL_BIGTABLIER);
		
		SInitSelection Sleft, Sright;
		Sleft.ColorSel=RGB(128,255,128);
		Sleft.LevelSelect=CAL_LEVEL_JOUR|CAL_LEVEL_SEMAINE|CAL_LEVEL_MOIS;
		Sleft.ModeSel=CAL_ETENDUE_JFC;
		Sleft.ShowRect=1;
		Sleft.TypeSelect=0;
		Sright.ColorSel=RGB(255,255,0);
		Sright.LevelSelect=CAL_LEVEL_JOUR|CAL_LEVEL_SEMAINE|CAL_LEVEL_MOIS;
		Sright.ModeSel=CAL_ETENDUE_JFC;
		Sright.ShowRect=1;
		Sright.TypeSelect=1;
		InitialiseSelection(&Sleft, &Sright, CAL_CROISEE);
	}
}

void CObj_Cal::InitialiseSelection(short ModeSelect, short LevelSelect, COLORREF ColorSelect)
{
	if((m_Style & 0x0f)==STYLE_CAL_PIGE)	return;
	if(!m_LeftSelect){
		// return;
		// Modif de rémi 5/10/99 3h58 AM
		// Pour assumer le Changer période
		// Pour l'instant, il faut appeler cette 
		// Fonction.
		m_LeftSelect=new SSelection;
	}
	m_SelectRules=CAL_INDEPENDANTE;
	m_LeftSelect->Selections.SetSize(m_nbcases);
	m_LeftSelect->SelectionsRAZ.SetSize(m_nbcases);
	m_LeftSelect->SelectionsTmp.SetSize(m_nbcases);
	for(short i=0 ; i<m_nbcases ; i++)m_LeftSelect->Selections[i]=0;
	m_LeftSelect->SelectionsRAZ.Copy(m_LeftSelect->Selections);
	m_LeftSelect->SelectionsTmp.Copy(m_LeftSelect->Selections);
	m_LeftSelect->ColSelect=0;
	m_LeftSelect->debSelect=0;
	m_LeftSelect->debL=0;
	m_LeftSelect->debR=0;
	m_LeftSelect->RSelect.SetRectEmpty();
	
	m_LeftSelect->LevelSelect	= LevelSelect;
	m_LeftSelect->ColorSel		= ColorSelect;
	m_LeftSelect->TypeSelect	= 0;
	m_LeftSelect->ModeSel		= ModeSelect & 0x0FF;
	m_LeftSelect->fOption		= ModeSelect & 0xF00;
	m_LeftSelect->ShowRect		= 0;
	m_CurrSelect				= m_LeftSelect;
}

void CObj_Cal::InitialiseSelection(SInitSelection * LeftSelection, SInitSelection * RightSelection,
									short SelectRules)
{
	m_SelectRules = SelectRules;
	//initialisation des structures de select
	//selection click gauche
	if(!LeftSelection)
	{
		if(m_LeftSelect)
		{
			//RAZ de la selection
			delete m_LeftSelect;
			m_LeftSelect = NULL;
		}
	}

	//si LeftSelection est OK..
		//si ya déja une sélection -> RAZ
	if(LeftSelection)
	{
		if(m_LeftSelect)
		{
			//RAZ de la selection
			delete m_LeftSelect;
		}
		m_LeftSelect=new SSelection;
		m_LeftSelect->Selections.SetSize(m_nbcases);
		m_LeftSelect->SelectionsRAZ.SetSize(m_nbcases);
		m_LeftSelect->SelectionsTmp.SetSize(m_nbcases);
		for(short i=0 ; i<m_nbcases ; i++)m_LeftSelect->Selections[i]=0;
		m_LeftSelect->SelectionsRAZ.Copy(m_LeftSelect->Selections);
		m_LeftSelect->SelectionsTmp.Copy(m_LeftSelect->Selections);
		m_LeftSelect->ColSelect=0;
		m_LeftSelect->debSelect=0;
		m_LeftSelect->debL=0;
		m_LeftSelect->debR=0;
		m_LeftSelect->RSelect.SetRectEmpty();
	
		m_LeftSelect->LevelSelect	= LeftSelection->LevelSelect;
		m_LeftSelect->ColorSel		= LeftSelection->ColorSel;
		m_LeftSelect->TypeSelect	= LeftSelection->TypeSelect;
		m_LeftSelect->ModeSel		= LeftSelection->ModeSel & 0x0FF;
		m_LeftSelect->fOption		= LeftSelection->ModeSel & 0xF00;
		m_LeftSelect->ShowRect		= LeftSelection->ShowRect;
		m_CurrSelect				= m_LeftSelect;
	}

	//selection click droit
	//si RightSelection=NULL, on libère le m_RightSelect
	if(!RightSelection)
	{
		if(m_RightSelect)
		{
			//RAZ de la selection
			delete m_RightSelect;
			m_RightSelect = NULL;
		}
		m_fSurCapture=0;
	}
	if(RightSelection)
	{
		if(m_RightSelect)
		{
			//RAZ de la selection
			delete m_RightSelect;
		}
		m_RightSelect=new SSelection;
		m_RightSelect->Selections.SetSize(m_nbcases);
		m_RightSelect->SelectionsRAZ.SetSize(m_nbcases);
		m_RightSelect->SelectionsTmp.SetSize(m_nbcases);
		for(short i=0 ; i<m_nbcases ; i++)m_RightSelect->Selections[i]=0;
		m_RightSelect->SelectionsRAZ.Copy(m_RightSelect->Selections);
		m_RightSelect->SelectionsTmp.Copy(m_RightSelect->Selections);
		m_RightSelect->ColSelect=0;
		m_RightSelect->debSelect=0;
		m_RightSelect->debL=0;
		m_RightSelect->debR=0;
		m_RightSelect->RSelect.SetRectEmpty();

		m_RightSelect->LevelSelect	= RightSelection->LevelSelect;
		m_RightSelect->ColorSel		= RightSelection->ColorSel;
		m_RightSelect->TypeSelect	= RightSelection->TypeSelect;
		m_RightSelect->ModeSel		= RightSelection->ModeSel & 0x0FF;
		m_RightSelect->fOption		= RightSelection->ModeSel & 0xF00;
		m_RightSelect->ShowRect		= RightSelection->ShowRect;
		if(!m_LeftSelect)			m_CurrSelect=m_RightSelect;
	}
}

void CObj_Cal::InitialiseAnnee(short Format,
							   COLORREF ColorBg,
							   COLORREF ColorTxt,
							   COLORREF ColorSplit,
							   short UTVertical,
							   short UTHorizontal,
							   short NumOrdre)
{
	if(!m_fInit)
	{
		m_FormatAnnee=Format;
		m_dyAnnee=UTVertical;
		m_dxAnnee=UTHorizontal;
		m_numOrdreAnnee=NumOrdre;
		m_colorBgAnnee=ColorBg;
		m_ColorTxtAnnee=ColorTxt;
		m_colorSplitAnnee=ColorSplit;
		if(m_dxAnnee > 0 && m_Style & OPTION_CAL_SPLITGRAY && (m_Style & 0x0f)!=STYLE_CAL_PIGE)
			m_PenAnnee.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		else
			m_PenAnnee.CreatePen(PS_SOLID, 1, m_colorSplitAnnee);
		ValideCalend();
	}
}

void CObj_Cal::InitialiseMois(bool fAlignSem, short Format, COLORREF ColorBg,
							  COLORREF ColorTxt, COLORREF ColorSplit, short UTVertical,
							  short UTHorizontal, short NumOrdre)
{
	if(!m_fInit)
	{
		m_fAlignSem=fAlignSem;
		m_FormatMois=Format;
		m_dyMois=UTVertical;
		m_dxMois=UTHorizontal;
		m_numOrdreMois=NumOrdre;
		m_colorBgMois=ColorBg;
		m_ColorTxtMois=ColorTxt;
		m_colorSplitMois=ColorSplit;
		if(m_dxMois > 0 && m_Style & OPTION_CAL_SPLITGRAY && (m_Style & 0x0f)!=STYLE_CAL_PIGE)
			m_PenMois.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		else
			m_PenMois.CreatePen(PS_SOLID, 1, m_colorSplitMois);
		ValideCalend();
	}
}

void CObj_Cal::InitialiseSemaine(short JourDebSem, short Format, COLORREF ColorBg, COLORREF ColorTxt, COLORREF ColorSplit, short UTVertical, short UTHorizontal, short NumOrdre)
{
	if(!m_fInit)
	{
		m_JourDebSem=JourDebSem;
		if(m_JourDebSem>6 || m_JourDebSem<0)	m_JourDebSem=0;
		m_FormatSem=Format;
		m_dySem=UTVertical;
		m_dxSem=UTHorizontal;
		m_numOrdreSem=NumOrdre;
		m_colorBgSem=ColorBg;
		m_ColorTxtSem=ColorTxt;
		m_colorSplitSem=ColorSplit;
		if(m_dxSem > 0 && m_Style & OPTION_CAL_SPLITGRAY && (m_Style & 0x0f)!=STYLE_CAL_PIGE)
			m_PenSem.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		else
			m_PenSem.CreatePen(PS_SOLID, 1, m_colorSplitSem);
		ValideCalend();
	}
}

void CObj_Cal::InitialiseJour(short Format, COLORREF ColorBg, COLORREF ColorTxt, COLORREF ColorSplit, short UTVertical, short UTHorizontal, short NumOrdre)
{
	if(!m_fInit)
	{
		m_FormatJour=Format;
		m_dyJour=UTVertical;
		m_dxJour=UTHorizontal;
		m_numOrdreJour=NumOrdre;
		m_colorBgJour=ColorBg;
		m_ColorTxtJour=ColorTxt;
		m_colorSplitJour=ColorSplit;
		if(m_Style & OPTION_CAL_SPLITGRAY)
			m_PenJour.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		else
			m_PenJour.CreatePen(PS_SOLID, 1, m_colorSplitJour);
		ValideCalend();
	}
}

short CObj_Cal::SetPosition(short Pos, bool flag)
{
	m_posactive=Pos;
	if(flag)	InvalidateRect(NULL,0);
	return(1);
}

char * CObj_Cal::GetCoupureCalend(void)
{
	if(m_fInit)	return m_CoupureCalend;
	else return NULL;
}

CByteArray * CObj_Cal::GetSelections(void)
{
	if(m_fInit)
	{
		if(m_LeftSelect)
		{
			if(!m_LeftSelect->TypeSelect)	return (&m_LeftSelect->Selections);
			else if(m_RightSelect)			return (&m_RightSelect->Selections);
			else return NULL;
		}
		else return NULL;
	}
	else return NULL;
}

CByteArray * CObj_Cal::GetSelectionsRight(void)
{
	if(m_fInit)
	{
		if(m_RightSelect)
		{
			if(m_RightSelect->TypeSelect)
			{
				return (&m_RightSelect->Selections);
			}
			else
			{
				if(m_LeftSelect)
				{
					return (&m_LeftSelect->Selections);
				}
				else
				{
					return NULL;
				}
			}
		}
		else return NULL;
	}
	else return NULL;
}


bool CObj_Cal::ValideCalend(void)
{
	for(short i = 0 ; i < CAL_NBNIVEAU ; i++)
	{
		m_TabOrd[m_numOrdreAnnee].m_y	= 0;
		m_TabOrd[m_numOrdreAnnee].m_dy = 0;	
	}
	if(m_numOrdreAnnee >= 0)
	{
		m_TabOrd[m_numOrdreAnnee].m_dy = m_dyAnnee;
	}
	if(m_numOrdreMois >= 0)
	{
		m_TabOrd[m_numOrdreMois].m_dy = m_dyMois;
	}
	if(m_numOrdreSem >= 0)
	{
		m_TabOrd[m_numOrdreSem].m_dy = m_dySem;
	}
	if(m_numOrdreJour >= 0)
	{
		m_TabOrd[m_numOrdreJour].m_dy = m_dyJour;
	}

	m_TabOrd[0].m_y = 0;
	m_TabOrd[1].m_y = m_TabOrd[0].m_dy + 1;
	m_TabOrd[2].m_y = m_TabOrd[0].m_dy + m_TabOrd[1].m_dy + 2;
	m_TabOrd[3].m_y = m_TabOrd[0].m_dy + m_TabOrd[1].m_dy + m_TabOrd[2].m_dy + 3;

	m_yAnnee = m_TabOrd[m_numOrdreAnnee].m_y;
	m_yMois  = m_TabOrd[m_numOrdreMois].m_y;
	m_ySem   = m_TabOrd[m_numOrdreSem].m_y;
	m_yJour  = m_TabOrd[m_numOrdreJour].m_y;
	if(m_dxAnnee)	m_fInit  = true;
	if(m_dxMois)	m_fInit  = true;
	if(m_dxSem)		m_fInit  = true;
	if(m_dxJour)	m_fInit  = true;

	if(m_fInit)
	{
		if(m_Style & OPTION_CAL_PRINTER)
		{
			m_RClient = m_PrntRect;
		}
		else
		{
			GetClientRect(&m_RClient);
		}

	
		if(m_dxAnnee)	m_dx = m_dxAnnee;
		if(m_dxMois)	m_dx = m_dxMois;
		if(m_dxSem)		m_dx = m_dxSem;
		if(m_dxJour)	m_dx = m_dxJour;

		m_nbcasesvisu = m_RClient.Width() / m_dx;
		if(m_nbcasesvisu > m_nbcases)
		{
			m_nbcasesvisu = m_nbcases;
			m_RClient.right = m_RClient.left + (m_dx * m_nbcasesvisu);
		}

		m_nbCol = m_nbcasesvisu;
		if(m_ptscroll)
		{
			m_ptscroll->SetScrollRange(0, (m_nbcases - m_nbcasesvisu), 0);
			m_ptscroll->SetScrollPos(m_posactive, true);
		}

		ConstruitCalendrier();
		CalculCoupure();
	}
	return(1);
}
	

void CObj_Cal::OnPaint()
{
	if(!m_fInit)	return;
	CPaintDC dc(this); // device context for painting

	CRect R, Rdest;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	long l, h;

	// copie de l'écran en mémoire ****************
	// copie de l'écran en mémoire ****************
	if(m_Style&OPTION_CAL_PRINTER)	R=m_PrntRect;
	else	GetClientRect(&R);

	DC.CreateCompatibleDC(&dc);

	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew.CreateCompatibleBitmap(&dc, l,h);
	hBmpOld=(CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);
	// *********************************************

	DessineCalendrier(&DC,&R,m_ptFont);

	// copy de la mem sur ecran
	Rdest=dc.m_ps.rcPaint;
	dc.BitBlt(Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,&DC,Rdest.left,Rdest.top,SRCCOPY);
	dc.SelectObject(hBmpOld);
	DC.DeleteDC();
	// ************************
	// Do not call CStatic::OnPaint() for painting messages
}

void CObj_Cal::PaintToPrinter(CDC * pDC, CFont * pFont)
{
	DessineCalendrier(pDC,&m_PrntRect,pFont, 1);
}

void CObj_Cal::DessineCalendrier(CDC * pDC,CRect * R, CFont * pFont, bool fPrinter)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(pFont);
	
	//dessine en gris le fond
	if(!fPrinter)
	{
		CBrush brush(RGB(192, 192, 192));
		pDC->FillRect(R, &brush);
	}

	short OffsetY=0;
	if(fPrinter)	OffsetY=m_PrntRect.top;

	if(m_dyAnnee)	DessineAnnee(pDC, OffsetY);
	if(m_dyMois)	DessineMois(pDC,OffsetY);
	if(m_dySem)		DessineSem(pDC,OffsetY);
	if(m_dyJour)	DessineJour(pDC,OffsetY);

	if(m_fCapture)
//	if(fPrinter && m_fCapture)
	{
		SSelection * SelectTmp=m_CurrSelect;
		m_CurrSelect=m_LeftSelect;
		if(m_Style & STYLE_CAL_PIGE)
			DessineSelectionPIGE(pDC);
		else
			DessineSelection(pDC);
		m_CurrSelect=SelectTmp;
	}
	if(m_fSurCapture)
	{
		SSelection * SelectTmp=m_CurrSelect;
		m_CurrSelect=m_RightSelect;
		if(m_Style & STYLE_CAL_PIGE)
			DessineSelectionPIGE(pDC);
		else
			DessineSelection(pDC);
		m_CurrSelect=SelectTmp;
	}
	if(m_fSelIndep)
	{
		for(long i = 0 ; i < m_vectIndepSel.GetSize() ; i++)
		{
			DessineSelectionIndep(pDC, &m_vectIndepSel.GetAt(i));
		}
	}
	if(m_fTablier)	DessineTablier(pDC);
	DessineCoupures(pDC);
	DessineLibelles(pDC);

	if(m_dyAnnee)	DessineCadreAnnee(pDC);
	if(m_dyMois)	DessineCadreMois(pDC);
	if(m_dySem)		DessineCadreSem(pDC);
	if(m_dyJour)	DessineCadreJour(pDC);

	//on trace un rectangle autour du calendrier
	pDC->MoveTo(m_RClient.left, m_RClient.top);
	pDC->LineTo(m_RClient.right, m_RClient.top);
	pDC->LineTo(m_RClient.right, m_RClient.bottom);
	pDC->LineTo(m_RClient.left, m_RClient.bottom);
	pDC->LineTo(m_RClient.left, m_RClient.top);

	if(!fPrinter){
		//on trace un rectangle autour de l'objet static
		pDC->MoveTo(R->left, R->top);
		pDC->LineTo(R->right-1, R->top);
		pDC->LineTo(R->right-1, R->bottom-1);
		pDC->LineTo(R->left, R->bottom-1);
		pDC->LineTo(R->left, R->top);
	}
	if(m_nbcurseur)		DessineCurseur(pDC);
}

short CObj_Cal::DessineAnnee(CDC * dc, short OffsetY)
{
	CBrush brush(m_colorBgAnnee);

	m_RAnnee.left=m_RClient.left;
	m_RAnnee.top=m_yAnnee+OffsetY;
	m_RAnnee.right=m_RClient.right-1;
	m_RAnnee.bottom=m_yAnnee+m_dyAnnee+OffsetY;
	dc->FillRect(&m_RAnnee, &brush);
	return 1;
}

short CObj_Cal::DessineCadreAnnee(CDC * dc)
{
	dc->MoveTo(m_RAnnee.right, m_RAnnee.top);
	dc->LineTo(m_RAnnee.right, m_RAnnee.bottom);
	dc->LineTo(m_RAnnee.left, m_RAnnee.bottom);
	dc->LineTo(m_RAnnee.left, m_RAnnee.top);
	if(!m_numOrdreAnnee)
		dc->LineTo(m_RAnnee.right, m_RAnnee.top);
	return(1);
}

short CObj_Cal::DessineMois(CDC * dc, short OffsetY)
{
	CBrush brush(m_colorBgMois);

	m_RMois.left=m_RClient.left;
	m_RMois.top=m_yMois+OffsetY;
	m_RMois.right=m_RClient.right-1;
	m_RMois.bottom=m_yMois+m_dyMois+OffsetY;
	dc->FillRect(&m_RMois, &brush);
	return 1;
}

short CObj_Cal::DessineCadreMois(CDC * dc)
{
	dc->MoveTo(m_RMois.right, m_RMois.top);
	dc->LineTo(m_RMois.right, m_RMois.bottom);
	dc->LineTo(m_RMois.left, m_RMois.bottom);
	dc->LineTo(m_RMois.left, m_RMois.top);
	if(!m_numOrdreMois)
		dc->LineTo(m_RMois.right, m_RMois.top);
	return(1);
}

short CObj_Cal::DessineSem(CDC * dc, short OffsetY)
{
	CBrush brush(m_colorBgSem);

	m_RSem.left=m_RClient.left;
	m_RSem.top=m_ySem+OffsetY;
	m_RSem.right=m_RClient.right-1;
	m_RSem.bottom=m_ySem+m_dySem+OffsetY;
	dc->FillRect(&m_RSem, &brush);
	return(1);
}

short CObj_Cal::DessineCadreSem(CDC * dc)
{
	dc->MoveTo(m_RSem.right, m_RSem.top);
	dc->LineTo(m_RSem.right, m_RSem.bottom);
	dc->LineTo(m_RSem.left, m_RSem.bottom);
	dc->LineTo(m_RSem.left, m_RSem.top);
	if(!m_numOrdreSem)
		dc->LineTo(m_RSem.right, m_RSem.top);
	return(1);
}

short CObj_Cal::DessineJour(CDC * dc, short OffsetY)
{
	CBrush brush(m_colorBgJour);

	m_RJour.left=m_RClient.left;
	m_RJour.top=m_yJour+OffsetY;
	m_RJour.right=m_RClient.right-1;
	m_RJour.bottom=m_yJour+m_dyJour+OffsetY;
	dc->FillRect(&m_RJour, &brush);
	return(1);
}

short CObj_Cal::DessineCadreJour(CDC * dc)
{
	dc->MoveTo(m_RJour.right, m_RJour.top);
	dc->LineTo(m_RJour.right, m_RJour.bottom);
	dc->LineTo(m_RJour.left, m_RJour.bottom);
	dc->LineTo(m_RJour.left, m_RJour.top);
	if(!m_numOrdreJour)
		dc->LineTo(m_RJour.right, m_RJour.top);
	return(1);
}

void CObj_Cal::SetCoupuresJour(BYTE sel, CDC * dc, short i)
{
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	switch(sel)
	{
	case 0:
	case 1:
		m_PenCurr=&m_PenJour;
		if(m_dyJour)	DessineCoupureJour(dc, i);
		break;
	case 2:
		m_PenCurr=&penBlack;
		if(m_dyJour)	DessineCoupureJour(dc, i-1);
		m_PenCurr=&m_PenJour;
		if(m_dyJour)	DessineCoupureJour(dc, i);
		break;
	case 3:
		m_PenCurr=&penBlack;
		if(m_dyJour)	DessineCoupureJour(dc, i);
		break;
	case 4:
		m_PenCurr=&penBlack;
		if(m_dyJour)	DessineCoupureJour(dc, i-1);
		if(m_dyJour)	DessineCoupureJour(dc, i);
		break;
	}
}

short CObj_Cal::DessineCoupures(CDC * dc)
{
	bool flag=0;
	m_PenCurr=&m_PenMois;
	if(m_dyJour)	DessineCoupureJour(dc, m_posactive);
	if(m_dySem)		DessineCoupureSem(dc, m_posactive);
	if(m_dyMois)	DessineCoupureMois(dc, m_posactive);
	if(m_dyAnnee)	DessineCoupureAnnee(dc, m_posactive);

	for(short i=m_posactive+1 ; i<=m_nbcasesvisu+m_posactive ; i++)
	{
		if(i>m_nbcases)		break;
		m_fStyle=0;
		flag=0;
		SSelection * SelectTmp=m_LeftSelect;
		if(m_RightSelect && m_RightSelect->Selections[i-1])	SelectTmp=m_RightSelect;

		switch(m_CoupureCalend[i])
		{
		case 0:	//Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			break;
		case 1:	//Semaine + Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenSem;
			if(m_dyJour)	DessineCoupureJour(dc, i);
			if(m_dySem)		DessineCoupureSem(dc, i);
			break;
		case 2:	//Mois + Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenMois;
			if(m_dyMois)	DessineCoupureMois(dc, i);
			break;
		case 3:	//Mois + Semaine + Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenMois;
			if(m_dyJour)	DessineCoupureJour(dc, i);
			if(m_dySem)		DessineCoupureSem(dc, i);
			if(m_dyMois)	DessineCoupureMois(dc, i);
			break;
		case 4:	//Annee + jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenAnnee;
			if(m_dyJour)	DessineCoupureJour(dc, i);
			if(m_dyAnnee)	DessineCoupureAnnee(dc, i);
			break;
		case 5:	//Annee + Semaine + Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenAnnee;
			if(m_dyJour)	DessineCoupureJour(dc, i);
			if(m_dySem)		DessineCoupureSem(dc, i);
			if(m_dyAnnee)	DessineCoupureAnnee(dc, i);
			break;
		case 6:	//Annee + Mois + Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenAnnee;
			if(m_dyJour)	DessineCoupureJour(dc, i-1);
			if(m_dyMois)	DessineCoupureMois(dc, i);
			if(m_dyAnnee)	DessineCoupureAnnee(dc, i);
			break;
		case 7:	//Annee + Mois + Semaine + Jour
			if(!SelectTmp)		//no selection
				SetCoupuresJour(3, dc, i);
			else
				SetCoupuresJour(SelectTmp->Selections[i-1], dc, i);
			m_PenCurr=&m_PenAnnee;
			if(m_dyJour)	DessineCoupureJour(dc, i);
			if(m_dySem)		DessineCoupureSem(dc, i);
			if(m_dyMois)	DessineCoupureMois(dc, i);
			if(m_dyAnnee)	DessineCoupureAnnee(dc, i);
			break;
		default:
			break;
		}
	}
	return(1);
}

short CObj_Cal::DessineLibelles(CDC * dc)
{
	if(m_dyJour)	DessineLibelleJour(dc, m_posactive);
	if(m_dySem)		DessineLibelleSem(dc, m_posactive);
	if(m_dyMois)	DessineLibelleMois(dc, m_posactive);
	if(m_dyAnnee)	DessineLibelleAnnee(dc, m_posactive);

	for(short i=m_posactive+1 ; i<=m_nbcasesvisu+m_posactive ; i++)
	{
		if(i>m_nbcases)		break;
		m_fStyle=0;
		switch(m_CoupureCalend[i])
		{
		case 0:	//Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			break;
		case 1:	//Semaine + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			if(m_dySem)		DessineLibelleSem(dc, i);
			break;
		case 2:	//Mois + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			m_PenCurr=&m_PenMois;
			if(m_dyMois)	DessineLibelleMois(dc, i);
			break;
		case 3:	//Mois + Semaine + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			if(m_dySem)		DessineLibelleSem(dc, i);
			if(m_dyMois)	DessineLibelleMois(dc, i);
			break;
		case 4:	//Annee + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			if(m_dyAnnee)	DessineLibelleAnnee(dc, i);
			break;
		case 5:	//Annee + Semaine + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			if(m_dySem)		DessineLibelleSem(dc, i);
			if(m_dyAnnee)	DessineLibelleAnnee(dc, i);
			break;
		case 6:	//Annee + Mois + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			if(m_dyMois)	DessineLibelleMois(dc, i);
			if(m_dyAnnee)	DessineLibelleAnnee(dc, i);
			break;
		case 7:	//Annee + Mois + Semaine + Jour
			if(m_dyJour)	DessineLibelleJour(dc, i);
			if(m_dySem)		DessineLibelleSem(dc, i);
			if(m_dyMois)	DessineLibelleMois(dc, i);
			if(m_dyAnnee)	DessineLibelleAnnee(dc, i);
			break;
		default:
			break;
		}
	}
	return(1);
}

short CObj_Cal::DessineCoupureJour(CDC * dc, short pos)
{
	short indice=pos-m_posactive;
	short x=m_RJour.left+(indice*m_dx);
	CPen * OldPen;
	OldPen=(CPen*)dc->SelectObject(m_PenCurr);
	dc->MoveTo(x, m_RJour.top);
	dc->LineTo(x, m_RJour.bottom);
	dc->SelectObject(OldPen);
	return(1);
}

short CObj_Cal::DessineLibelleJour(CDC * dc, short pos)
{
	short indice=pos-m_posactive;
	if(indice==0)	return(1);
	short x=m_RJour.left+(indice*m_dx);
	//dessine le lib du jour
	CRect R;
	R.left=m_RJour.left+((indice-1)*m_dx);
	R.top=m_RJour.top;
	R.right=x;
	R.bottom=m_RJour.bottom;
	

	CString txt;
	CSize size;
	switch(m_FormatJour)
	{
	case 0:
		txt=m_LibJour[pos-1][0];
		size=dc->GetTextExtent(txt);
		break;
	case 1:
		txt=m_LibJour[pos-1][1][0];
		txt+="\n"+m_LibJour[pos-1][0];
		txt.MakeUpper();
		size=dc->GetTextExtent(m_LibJour[pos-1][0]);
		break;
	case 2:
		txt=m_LibJour[pos-1][1];
		size=dc->GetTextExtent(txt);
		break;
	default:
		txt=m_LibJour[pos-1][0];
		size=dc->GetTextExtent(txt);
		break;
	}
	long l=R.right-R.left;
	long h=R.bottom-R.top;
	COLORREF oldColor=dc->SetTextColor(m_ColorTxtJour);
	if(size.cx<=l && size.cy<=h)
		dc->DrawText(txt, &R, DT_VCENTER|DT_CENTER|DT_NOPREFIX);
	dc->SetTextColor(oldColor);
	return(1);
}

short CObj_Cal::DessineCoupureSem(CDC * dc, short pos)
{
	short indice=pos-m_posactive;
	short x=m_RSem.left+(indice*m_dx);
	CPen * OldPen;
	OldPen=(CPen*)dc->SelectObject(m_PenCurr);
	dc->MoveTo(x, m_RSem.top);
	dc->LineTo(x, m_RSem.bottom);
	dc->SelectObject(OldPen);
	return(1);
}

short CObj_Cal::DessineLibelleSem(CDC * dc, short pos)
{
	short indice=pos-m_posactive;
	short x=m_RSem.left+(indice*m_dx);
	if(pos>=m_nbcases)	return(1);
	//dessine le lib de la semaine
	short n=0;
	for(short i=pos+1 ; i<m_nbcasesvisu+pos ; i++)
	{
		if(m_CoupureCalend[i]!=1 && m_CoupureCalend[i]!=3 && m_CoupureCalend[i]!=5 && m_CoupureCalend[i]!=7)
		{
			n++;
		}
		else
		{
			break;
		}
	}

	CRect R;
	R.left = x;
	R.top = m_RSem.top;
	R.right = m_RSem.left + ((indice + n + 1) * m_dx);
	while(R.right > m_RClient.right)
		R.right = m_RSem.left + ((indice + (--n) + 1) * m_dx);
	R.bottom = m_RSem.bottom;

	while(m_CoupureCalend[pos]!=1 && m_CoupureCalend[pos]!=3 && m_CoupureCalend[pos]!=5 && m_CoupureCalend[pos]!=7)
	{
		if(pos > 0)	pos--;
		else		break;
	}

	long l = R.right - R.left;
	long h = R.bottom - R.top;
	CString txt;
	UINT nformat = DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
	switch(m_FormatSem)
	{
	case 0:
		txt.Format("%d", m_SCalend[pos].NoSem);
		nformat|=DT_CENTER;
		break;
	case 1:
		txt=m_LibJour[pos][0];
		nformat|=DT_LEFT;
		R.left+=2;
		break;
	default:
		txt.Format("%d", m_SCalend[pos].NoSem);
		nformat|=DT_CENTER;
		break;
	}
	CSize size=dc->GetTextExtent(txt);
	COLORREF oldColor=dc->SetTextColor(m_ColorTxtSem);
	if(size.cx+2<=l && size.cy<=h)
		dc->DrawText(txt, &R, nformat);
	dc->SetTextColor(oldColor);
	return(1);
}

short CObj_Cal::DessineCoupureMois(CDC * dc, short pos)
{
	short x=0;
	short indice=pos-m_posactive-1;
	if(pos>=m_nbcases)	indice++;
	if(indice>=0)
	{
		x=m_RMois.left+(indice*m_dx);
		CPen * OldPen;
		OldPen=(CPen*)dc->SelectObject(m_PenCurr);
		dc->MoveTo(x, m_RMois.top);
		dc->LineTo(x, m_RMois.bottom);
		dc->SelectObject(OldPen);
	}
	return(1);
}

short CObj_Cal::DessineLibelleMois(CDC * dc, short pos)
{
	short indice=pos-m_posactive-1;
//	short x=0;
	short x=m_RMois.left;
	if(indice>=0)	x=m_RMois.left+(indice*m_dx);

	if(pos>=m_nbcases)	return(1);
	//dessine le lib du mois
	short n=0;
	for(short i=pos+1 ; i<=m_nbcasesvisu+pos ; i++)
	{
		if(m_CoupureCalend[i] != 2 && m_CoupureCalend[i] != 3
		&& m_CoupureCalend[i] != 6 && m_CoupureCalend[i] != 7){
			n++;
		}
		else{
			break;
		}
	}
	if(indice+n>m_nbcasesvisu)	n=m_nbcasesvisu-indice-1;

	CRect R;
	R.left=x;
	R.top=m_RMois.top;
	R.right=m_RMois.left+((indice+n+1)*m_dx);
	R.bottom=m_RMois.bottom;
	for(i=0 ; i<m_nbmois ; i++)
		if(m_SCalend[pos].Mois==atoi(m_LibMois[i][0])
		&& m_SCalend[pos].Annee==atoi(m_LibMois[i][5]))
			break;
	if(m_FormatMois>4 || m_FormatMois<0)	m_FormatMois=0;
	long l=R.right-R.left;
	long h=R.bottom-R.top;
	CSize size=dc->GetTextExtent(m_LibMois[i][m_FormatMois]);
	COLORREF oldColor=dc->SetTextColor(m_ColorTxtMois);
	if(size.cx+2<=l && size.cy<=h)
		dc->DrawText(m_LibMois[i][m_FormatMois], &R, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	dc->SetTextColor(oldColor);
	return(1);
}

short CObj_Cal::DessineCoupureAnnee(CDC * dc, short pos)
{
	short x=0;
	short indice=pos-m_posactive-1;
	if(pos>=m_nbcases)	indice++;
	if(indice>=0)
	{
		x=m_RAnnee.left+(indice*m_dx);
		CPen * OldPen;
		OldPen=(CPen*)dc->SelectObject(m_PenCurr);
		dc->MoveTo(x, m_RAnnee.top);
		dc->LineTo(x, m_RAnnee.bottom);
		dc->SelectObject(OldPen);
	}
	return(1);
}

short CObj_Cal::DessineLibelleAnnee(CDC * dc, short pos)
{
	short indice=pos-m_posactive-1;
//	short x=0;
	short x=m_RAnnee.left;
	if(indice>=0)	x=m_RAnnee.left+(indice*m_dx);

	if(pos>=m_nbcases)	return(1);
	//dessine le lib du mois
	short n=0;
	for(short i=pos+1 ; i<=m_nbcasesvisu+pos ; i++)
	{
		if(	m_CoupureCalend[i]	!= 4	&& m_CoupureCalend[i] != 5 &&
			m_CoupureCalend[i]	!= 6	&& m_CoupureCalend[i] != 7){
			n++;
		}
		else{
			break;
		}
	}
	if(indice+n>m_nbcasesvisu)	n=m_nbcasesvisu-indice-1;

	CRect R;
	R.left=x;
	R.top=m_RAnnee.top;
	R.right=m_RAnnee.left + ((indice + n + 1) * m_dx);
	R.bottom=m_RAnnee.bottom;
	for(i = 0 ; i < m_nbcases ; i++)
	{
		if(m_SCalend[pos].Annee==atoi(m_LibMois[i][5])){
			break;
		}
	}
	if(m_FormatAnnee>4 || m_FormatAnnee<0)	m_FormatAnnee=0;
	long l=R.right-R.left;
	long h=R.bottom-R.top;
	CSize size=dc->GetTextExtent(m_LibMois[i][5]);
	COLORREF oldColor=dc->SetTextColor(m_ColorTxtAnnee);
	if(size.cx+2 <= l && size.cy <= h){
		dc->DrawText(m_LibMois[i][5], &R, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	}
	dc->SetTextColor(oldColor);
	return(1);
}

void CObj_Cal::DessineSelection(CDC * dc)
{
	short x=-1, xb=-1;
	CRect R;
	CBrush brushOn(m_CurrSelect->ColorSel);
	CBrush brushLevel(RGB(210, 210, 210));
	CBrush brushOff(m_colorBg);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));

	for(short i=m_posactive ; i<m_nbcasesvisu+m_posactive ; i++)
	{	
		R.left=(i-m_posactive)*m_dx;
		R.right=((i-m_posactive)*m_dx)+m_dx;
		if(m_CurrSelect->ShowRect)
		{
			if(m_dyAnnee)
			{
				R.top=m_RAnnee.top;
				R.bottom=R.top+m_dyAnnee;
				if(m_CurrSelect->Selections[i])
					dc->FillRect(&R, &brushOn);
			}
			if(m_dyMois)
			{
				R.top=m_RMois.top;
				R.bottom=R.top+m_dyMois;
				if(m_CurrSelect->Selections[i])		dc->FillRect(&R, &brushOn);
			}
			if(m_dySem)
			{
				R.top=m_RSem.top;
				R.bottom=R.top+m_dySem;
				if(m_CurrSelect->Selections[i])
					dc->FillRect(&R, &brushOn);
			}
			if(m_dyJour)
			{
				R.top=m_RJour.top;
				R.bottom=R.top+m_dyJour;
				if(m_CurrSelect->Selections[i])		dc->FillRect(&R, &brushOn);
			}
		}
		else
		{
			R.top=m_CurrSelect->RSelect.top;
			if(!R.top)	R.top+=1;
			R.bottom=m_CurrSelect->RSelect.bottom;
			if(m_CurrSelect->Selections[i])	dc->FillRect(&R, &brushOn);
		}

		if(m_CurrSelect->Selections[i]==2)		x=R.left;
		else if(m_CurrSelect->Selections[i]==3)	x=(short)R.right;
		else if(m_CurrSelect->Selections[i]==4){x=R.left;	xb=(short)R.right;}

		if(x > -1)
		{
			CPen * PenOld=dc->SelectObject(&pen);
			if(m_CurrSelect->ShowRect)
			{
				R.top=1;
				R.bottom=m_dyAnnee + m_dyMois + m_dySem + m_dyJour + R.top;
			}
			dc->MoveTo(x, R.top);
			dc->LineTo(x, R.bottom);
			if(xb > -1)
			{
				dc->MoveTo(xb, R.top);
				dc->LineTo(xb, R.bottom);
			}
			dc->SelectObject(PenOld);
		}
	}
	//surselection du niveau semaine ou mois en cours; aucune lorsque on relache le click
	dc->FillRect(&m_RLevelSel, &brushLevel);
}

void CObj_Cal::DessineSelectionIndep(CDC * dc, SSelection *IndepSelect)
{
	if(IndepSelect == NULL)
	{
		if(m_vectIndepSel.GetSize() > 0)
		{
			IndepSelect = &m_vectIndepSel.GetAt(0);
		}
		else
		{
			return;
		}
	}
	CRect R = IndepSelect->RSelect;
	CBrush brushOn(IndepSelect->ColorSel);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen * PenOld=dc->SelectObject(&pen);
	bool espion = true;
	//on compte 
	for(short s = 0 ; s < IndepSelect->Selections.GetSize() ; s++)
	{
	}
	for(short i = m_posactive ; i < m_nbcasesvisu + m_posactive ; i++)
	{	
		R.left = (i - m_posactive) * m_dx;
		R.right = ((i - m_posactive) * m_dx) + m_dx;
		if(i >= IndepSelect->Selections.GetSize()) break;
		if(IndepSelect->Selections[i])
		{
			if(m_LeftSelect)
			{
				if(m_LeftSelect->Selections[i] && IndepSelect->fOption == CAL_SEL_INDEP_HIDE)
				{
					continue;
				}
			}
			if(m_RightSelect)
			{
				if(m_RightSelect->Selections[i] && IndepSelect->fOption == CAL_SEL_INDEP_HIDE)
				{
					continue;
				}
			}
			dc->FillRect(&R, &brushOn);
			if(IndepSelect->ShowRect)
			{
				if(espion)
				{
					dc->MoveTo(R.left, R.top);
					dc->LineTo(R.left, R.bottom);
					espion = false;
				}
				dc->MoveTo(R.left, R.top);
				dc->LineTo(R.right, R.top);
				dc->MoveTo(R.left, R.bottom);
				dc->LineTo(R.right, R.bottom);
			}

		}
		else
		{
			if(i && IndepSelect->ShowRect)	//si pas premiere sélection ET cadrage permis
			{
				if(IndepSelect->Selections[i - 1])
				{
					dc->MoveTo(R.left, R.top);
					dc->LineTo(R.left, R.bottom);
				}
			}
		}
	}
	dc->SelectObject(PenOld);
}

void CObj_Cal::DessineSelectionPIGE(CDC * dc)
{
	short x = -1, xb = -1;
	CRect R;
	CBrush brushLeft(m_LeftSelect->ColorSel);
	CBrush brushRight(m_RightSelect->ColorSel);
	CBrush brushLevel(RGB(210, 210, 210));
	CBrush brushOff(m_colorBg);
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));

	for(short i=m_posactive ; i<m_nbcasesvisu+m_posactive ; i++)
	{	
		R.left=(i-m_posactive)*m_dx;
		R.right=((i-m_posactive)*m_dx)+m_dx;
		if(m_LeftSelect->Selections[i] || m_RightSelect->Selections[i])
		{
			if(m_dyAnnee && m_LeftSelect->Selections[i] ||
			(!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]))
			{
				R.top=m_RAnnee.top;
				R.bottom=R.top+m_dyAnnee;
				if(m_LeftSelect->Selections[i])
				{
					dc->FillRect(&R, &brushLeft);
				}
				else
				{
					if((!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]))
					{
						dc->FillRect(&R, &brushRight);
					}
				}
			}
			if(m_dyMois	&& m_LeftSelect->Selections[i] ||
			(!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]))
			{
				R.top=m_RMois.top;
				R.bottom=R.top+m_dyMois;
				if(m_LeftSelect->Selections[i])
				{
					dc->FillRect(&R, &brushLeft);
				}
				else
				{
					if((!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]))
					{
						dc->FillRect(&R, &brushRight);
					}
				}
			}
			if(m_dySem && m_LeftSelect->Selections[i] ||
			(!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]))
			{
				R.top=m_RSem.top;
				R.bottom=R.top+m_dySem;
				if(m_LeftSelect->Selections[i])
				{
					dc->FillRect(&R, &brushLeft);
				}
				else
				{
					if((!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]))
					{
						dc->FillRect(&R, &brushRight);
					}
				}
			}
			if(m_dyJour)
			{
				R.top=m_RJour.top;
				R.bottom=R.top+m_dyJour;
				if((m_LeftSelect->Selections[i] && !m_RightSelect->Selections[i]))
				{
					dc->FillRect(&R, &brushLeft);
				}
				else
				{
					if(m_RightSelect->Selections[i])
					{
						dc->FillRect(&R, &brushRight);
					}
				}
			}
		}

		if(m_LeftSelect->Selections[i]==2 || (!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]==2))
			x=R.left;
		else if(m_LeftSelect->Selections[i]==3 || (!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]==3))
			x=(short)R.right;
		else if(m_LeftSelect->Selections[i]==4 || (!m_LeftSelect->Selections[i] && m_RightSelect->Selections[i]==4)){
			x=R.left;
			xb=(short)R.right;
		}

		if(x > -1)
		{
			CPen * PenOld=dc->SelectObject(&pen);
			if(m_LeftSelect->Selections[i] || m_RightSelect->Selections[i])
			{
				R.top=1;
				R.bottom=m_dyAnnee + m_dyMois + m_dySem + m_dyJour + R.top;
			}
			dc->MoveTo(x, R.top);
			dc->LineTo(x, R.bottom);
			if(xb > -1)
			{
				dc->MoveTo(xb, R.top);
				dc->LineTo(xb, R.bottom);
			}
			dc->SelectObject(PenOld);
		}
	}
	//surselection du niveau semaine ou mois en cours; aucune lorsque on relache le click
	dc->FillRect(&m_RLevelSel, &brushLevel);
}

void CObj_Cal::DessineCurseur(CDC * dc)
{
	for(short i=0 ; i<m_nbcurseur ; i++)
	{
		SCurseur * curseur;
		curseur = (SCurseur *)m_ArrayCurseur->GetAt(i);
		if(curseur->Pos>=m_posactive && curseur->Pos<=m_posactive+m_nbcasesvisu)
		{
			short width=0;
			if(curseur->Select)	width=3;
			else				width=2;
				
			CPen pen(PS_SOLID, width, curseur->Color);
			CPen * PenOld=dc->SelectObject(&pen);
			if(!m_fCursor || !curseur->Select)
			{
				curseur->R.left = m_dx * (curseur->Pos - m_posactive);
				curseur->R.right=curseur->R.left + width;
				if(m_Style&OPTION_CAL_BIGTABLIER)
				{
					curseur->R.top=2;
					curseur->R.bottom=m_dyAnnee + m_dyMois + m_dySem + m_dyJour + curseur->R.top - 1;
				}
				else
				{
					if(m_dxAnnee){curseur->R.top=m_yAnnee;	curseur->R.bottom=curseur->R.top+m_dyAnnee;}					
					if(m_dxMois){curseur->R.top=m_yMois;	curseur->R.bottom=curseur->R.top+m_dyMois;}
					if(m_dxSem)	{curseur->R.top=m_ySem;		curseur->R.bottom=curseur->R.top+m_dySem;}
					if(m_dxJour){curseur->R.top=m_yJour;	curseur->R.bottom=curseur->R.top+m_dyJour;}
				}
			}
			dc->MoveTo(curseur->R.left, curseur->R.top);
			dc->LineTo(curseur->R.left, curseur->R.bottom);
			dc->SelectObject(PenOld);
		}
	}
}

void CObj_Cal::DessineTablier(CDC * dc)
{
	short curs1, curs2, curs;
	CRect R, RSelect;
	CBrush brushOff(m_ColorTablier);

	curs1=GetPosCurseur(m_PosTablier1);
	curs2=GetPosCurseur(m_PosTablier2);
	if(curs2<curs1)
	{
		curs=curs1;
		curs1=curs2;
		curs2=curs;
	}
	
	if(m_dxJour)	RSelect=m_RJour;
	if(m_dxSem)		RSelect=m_RSem;
	if(m_dxMois)	RSelect=m_RMois;
	if(m_dxAnnee)	RSelect=m_RAnnee;

	for (short i=m_posactive ; i<=m_nbcasesvisu+m_posactive ; i++)
	{
		R.left=(i-m_posactive)*m_dx;
		R.right=((i-m_posactive)*m_dx)+m_dx;
		if(m_Style&OPTION_CAL_BIGTABLIER)
		{	
			if(m_dyAnnee)
			{
				R.top=m_RAnnee.top;
				R.bottom=R.top+m_dyAnnee;
			}

			if(m_dyMois)
			{
				R.top=m_RMois.top;
				R.bottom=R.top+m_dyMois;
			}
			if(m_dySem)
			{
				R.top=m_RSem.top;
				R.bottom=R.top+m_dySem;
			}
			if(m_dyJour)
			{
				R.top=m_RJour.top;
				R.bottom=R.top+m_dyJour;
			}
			if(i < curs1 || i >= curs2)
			{
				dc->FillRect(&R, &brushOff);
			}
		}
		else
		{
			R.top=RSelect.top;
			if(!RSelect.top)	RSelect.top+=1;
			R.bottom=RSelect.bottom;
			if(i<curs1 || i>=curs2)	dc->FillRect(&R, &brushOff);
		}
	}
}

short CObj_Cal::ConstruitCalendrier(void)
{
	bool M=false;
	short sem=0;
	long jour;
//	ASSERT(AfxCheckMemory());

	///////////////////////////////////////Ajout pour ChangePeriode(...)
	//si ya changement de période alors on efface et on recommence
	if(m_LibJour)
	{
		for(short i=0 ; i<m_nbjour ; i++)
		{
			if(m_LibJour[i]) delete [] m_LibJour[i];
		}
		delete [] m_LibJour;
	}
	if(m_LibMois)
	{
		for(short i=0 ; i<m_nbmois ; i++)
		{
			if(m_LibMois[i])delete [] m_LibMois[i];
		}
		delete [] m_LibMois;
	}
	if(m_SCalend) delete [] m_SCalend;
	///////////////////////////////////////

	m_nbjour=m_nbcases;
	m_LibJour=new CString*[m_nbjour];
	for(short i=0 ; i<m_nbjour ; i++)
		m_LibJour[i]=new CString[3];

	m_SCalend=new SDataJour[m_nbcases];
	COleDateTime jourdeb=m_jourdeb;

	if(m_dxSem)	jour=7;
	else		jour=1;
	COleDateTimeSpan t(jour, 0, 0, 0);

	short mois=0;
	m_nbmois=0;
	for(i=0 ; i<m_nbcases ; i++)
	{
		m_SCalend[i].JourMois=m_jourdeb.GetDay();
		m_SCalend[i].JourSem=m_jourdeb.GetDayOfWeek();
		m_SCalend[i].Mois=m_jourdeb.GetMonth();
		m_SCalend[i].Annee=m_jourdeb.GetYear();
		m_SCalend[i].NoSem=atoi(m_jourdeb.Format("%W"));
//		if(m_SCalend[i].NoSem==53)	m_SCalend[i].NoSem=1;

		m_LibJour[i][0]=m_jourdeb.Format("%d");	//jour du mois
		m_LibJour[i][1]=m_jourdeb.Format("%a");	//jour de sem abrege
		m_LibJour[i][2]=m_jourdeb.Format("%A");	//jour de sem

		if(mois!=m_jourdeb.GetMonth())
		{
			m_nbmois++;
			mois=m_jourdeb.GetMonth();
			M=true;
		}
		else M=false;
		if(m_dxMois && !M)	i--;
		m_jourdeb+=t;
	}
		
	m_LibMois=new CString*[m_nbmois];
	for(i=0 ; i<m_nbmois ; i++)
		m_LibMois[i]=new CString[6];

	mois=0;
	m_nbmois=0;
	m_jourdeb=jourdeb;
	for(i=0 ; i<m_nbcases ; i++)
	{
		if(mois!=m_jourdeb.GetMonth())
		{
			m_LibMois[m_nbmois][0]=m_jourdeb.Format("%m");	//n° mois
			m_LibMois[m_nbmois][1]=m_jourdeb.Format("%b");	//lib du mois abrege
			m_LibMois[m_nbmois][1].SetAt(0, toupper(m_LibMois[m_nbmois][1][0]));
			m_LibMois[m_nbmois][2]=m_jourdeb.Format("%B");	//lib du mois
			m_LibMois[m_nbmois][2].SetAt(0, toupper(m_LibMois[m_nbmois][2][0]));
			m_LibMois[m_nbmois][3]=m_LibMois[m_nbmois][0]+"/"+m_jourdeb.Format("%y");	//annee sans le siecle
			m_LibMois[m_nbmois][4]=m_LibMois[m_nbmois][2]+" "+m_jourdeb.Format("%Y");	//annee avec le siecle
			m_LibMois[m_nbmois++][5]=m_jourdeb.Format("%Y");							//annee avec le siecle
			mois=m_jourdeb.GetMonth();
		}
		else if(m_dxMois)	i--;
		m_jourdeb+=t;
	}
	m_jourdeb=jourdeb;
//	ASSERT(AfxCheckMemory());
	return(1);
}

short CObj_Cal::CalculCoupure()
{
	bool M=false;
	short mois=0, m=0, s=0, aa = 0, annee = 0;
	long jour;
	COleDateTime jourdeb=m_jourdeb;

	if(m_dxSem)	jour=7;
	else		jour=1;
	COleDateTimeSpan t(jour, 0, 0, 0);

	if(m_CoupureCalend)	delete [] m_CoupureCalend;
	m_CoupureCalend=new char[m_nbcases+2];
	memset(m_CoupureCalend, 0, m_nbcases+2);
	m_jourdeb-=t;
	for(short i=0 ; i<=m_nbcases ; i++)
	{
		//Jour
		m_CoupureCalend[i]=0;

		//Semaine
		s=m_jourdeb.GetDayOfWeek();
		if(!m_dxSem)
		{
			if(--s==m_JourDebSem)
				m_CoupureCalend[i]=1;
		}
		else	m_CoupureCalend[i]=1;

		//Mois
		m=m_jourdeb.GetMonth();
		if(mois!=m)
		{
			m_CoupureCalend[i]+=2;
			mois=m;
			M=true;
		}
		else M=false;
		if(m_dxMois && !M)	i--;

		//Annee
		aa = m_jourdeb.GetYear();
		if(annee != aa)
		{
			m_CoupureCalend[i] += 4;
			annee = aa;
		}

		m_jourdeb+=t;
	}
 	m_CoupureCalend[0]=7;
	m_CoupureCalend[m_nbcases+1]=7;
	m_jourdeb=jourdeb;
	return(1);
}

void CObj_Cal::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_fInit){
		return;
	}
	m_fLBCapture = true; //La capture appartient à l'objet cal
//	ASSERT(AfxCheckMemory());
	m_CurrSelect = NULL;
	if(!m_LeftSelect){
		return;
	}
	m_nFlags = nFlags;
	m_CurrSelect = m_LeftSelect;
	m_fCursor = IsMouseOnCursor(point);
	if(!m_fCursor)
	{	
		if(!GetClickInfo(point)){
			return;
		}
		m_CurrSelect->ColSelect = (point.x / m_dx) + m_posactive;
		if(!IsSelectRules(0, m_CurrSelect->ColSelect))
		{
			return;//left selection
		}
		if(nFlags != (MK_LBUTTON | MK_SHIFT)){
			m_CurrSelect->debR = m_CurrSelect->ColSelect;
			m_CurrSelect->debL = m_CurrSelect->ColSelect;
		}
		m_fCapture = 0;
		bool f = 0;
		if(m_fTablier)
		{
			short curs1 = GetPosCurseur(m_PosTablier1);
			short curs2 = GetPosCurseur(m_PosTablier2);
			if(curs2<curs1)
			{
				short curs = curs1;
				curs1 = curs2;
				curs2 = curs;
			}
			if(m_Style&STYLE_CAL_CREUSET)
			{
				m_fTablierCreuset = 1;
				f = 1;
			}
			else
			{
				m_fTablierCreuset = 0;
				if((m_CurrSelect->ColSelect >= curs1 && m_CurrSelect->ColSelect < curs2)){
					f = 1;
				}
			}
		}
		if(m_CurrSelect->ColSelect<m_nbcases && (!m_fTablier || (m_fTablier && f)))
		{
			CRect RSelect = m_CurrSelect->RSelect;
			m_fCapture = 1;
			short ModeSel = m_CurrSelect->ModeSel;
			switch(ModeSel)
			{
			case CAL_SIMPLE:	//une cellule
				m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
				SetClickSelections(0);
				m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
				break;
			case CAL_MULTIPLE:
				m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsTmp);
				if(m_CurrSelect->Selections[m_CurrSelect->ColSelect])
				{
					m_fModeSelect = 0;
					m_CurrSelect->Selections[m_CurrSelect->ColSelect] = 0;
				}
				else
				{
					m_fModeSelect = 1;
					m_CurrSelect->Selections[m_CurrSelect->ColSelect] = 1;
				}
				break;
			case CAL_ETENDUE:
				if(nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON | MK_CONTROL))
				{
					m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
					m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
					SetClickSelections(0);	//0 signifie le début (initialisation) d'une selection
				}
				if(nFlags == (MK_LBUTTON|MK_SHIFT))
				{
					SetClickSelections(1);
					for(short i = min(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i<=max(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i++)
					{
						SetCellsSelection(i);
					}
					m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
				}
				break;
			case CAL_ETENDUE_JFC:
				if(nFlags == MK_LBUTTON)
				{
					m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
					SetClickSelections(0);
				}
				if(nFlags == (MK_LBUTTON | MK_CONTROL))
				{
					m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
					SetClickSelections(0);
				}
				if(nFlags == (MK_LBUTTON | MK_SHIFT))
				{
					SetClickSelections(1);	//1 signifie la suite d'une selection
					for(short i = min(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i <= max(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i++)
					{
						SetCellsSelection(i);
					}
					m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
				}
				m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
				break;
			}
		}
		MAJSelections();
	}
	SetCapture();
	InvalidateRect(NULL,0);
	SetTimer(1, 100, NULL);
//	ASSERT(AfxCheckMemory());
//	CStatic::OnLButtonDown(nFlags, point);
}

void CObj_Cal::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_fInit){
		return;
	}
	m_fRBCapture = true; //La capture appartient à l'objet cal
	m_CurrSelect = NULL;
	if(!m_RightSelect){
		return;
	}
	m_CurrSelect = m_RightSelect;
	m_nFlags = nFlags;
	if(!GetClickInfo(point)){
		return;
	}
	m_CurrSelect->ColSelect = (point.x / m_dx) + m_posactive;
	if(!IsSelectRules(1, m_CurrSelect->ColSelect))
	{
		return;//right selection
	}
	if(nFlags != (MK_RBUTTON | MK_SHIFT)){
		m_CurrSelect->debR = m_CurrSelect->ColSelect;
		m_CurrSelect->debL = m_CurrSelect->ColSelect;
	}
	m_fSurCapture = 0;
	bool f = 0;
	if(m_fTablier)
	{
		short curs1 = GetPosCurseur(m_PosTablier1);
		short curs2 = GetPosCurseur(m_PosTablier2);
		if(curs2 < curs1)
		{
			short curs = curs1;
			curs1 = curs2;
			curs2 = curs;
		}
		if(m_Style & STYLE_CAL_CREUSET)
		{
			m_fTablierCreuset = 1;
			f = 1;
		}
		else
		{
			m_fTablierCreuset = 0;
			if((m_CurrSelect->ColSelect >= curs1 && m_CurrSelect->ColSelect < curs2)){
				f = 1;
			}
		}
	}
	if(m_CurrSelect->ColSelect < m_nbcases && (!m_fTablier || (m_fTablier && f)))
	{
		//util pour vérifier si click dans la meme zone
		CRect RSurSelect = m_CurrSelect->RSelect;
		m_fSurCapture = 1;
		switch(m_CurrSelect->ModeSel)
		{
		case CAL_SIMPLE:
			m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
			m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
			if(m_CurrSelect->Selections[m_CurrSelect->ColSelect])
			{
				m_fModeSelect = 0;
				m_CurrSelect->Selections[m_CurrSelect->ColSelect] = 0;
			}
			else
			{
				m_fModeSelect = 1;
				m_CurrSelect->Selections[m_CurrSelect->ColSelect] = 1;
			}
			break;
		case CAL_MULTIPLE:
			m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
			m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsTmp);
			if(m_CurrSelect->Selections[m_CurrSelect->ColSelect])
			{
				m_fModeSelect = 0;
				m_CurrSelect->Selections[m_CurrSelect->ColSelect] = 0;
			}
			else
			{
				m_fModeSelect = 1;
				m_CurrSelect->Selections[m_CurrSelect->ColSelect] = 1;
			}
			break;
		case CAL_ETENDUE:
			if(nFlags == MK_RBUTTON || nFlags == (MK_RBUTTON | MK_CONTROL))
			{
				m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
				SetClickSelections(0);	//0 signifie le début (initialisation) d'une selection
			}
			if(nFlags == (MK_RBUTTON | MK_SHIFT))
			{
				SetClickSelections(1);
				for(short i = min(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i <= max(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i++)
				{
					SetCellsSelection(i);
				}
				m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
			}
			break;
		case CAL_ETENDUE_JFC:
			if(nFlags == MK_RBUTTON){
				m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
				SetClickSelections(0);
			}
			if(nFlags == (MK_RBUTTON | MK_CONTROL)){
				m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
				SetClickSelections(0);
			}
			if(nFlags == (MK_RBUTTON | MK_SHIFT))
			{
				SetClickSelections(1);	//1 signifie la suite d'une selection
				for(short i = min(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i <= max(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i++)
				{
					SetCellsSelection(i);
				}
				m_CurrSelect->debSelect = m_CurrSelect->ColSelect;
			}
			m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
			break;
		}
		MAJSelections();
		SetCapture();
		InvalidateRect(NULL, 0);
		SetTimer(1, 100, NULL);
	}
}

void CObj_Cal::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_fInit)	return;
	if(!m_fLBCapture && !m_fRBCapture) return;//Est ce que la capture appartient à l'objet cal???
	short i=0;
//	ASSERT(AfxCheckMemory());
	if(!m_CurrSelect)	return;

	CRect cR(m_RClient);
	m_fAscHor=0;
	if(!cR.PtInRect(point))
	{
		CPoint pt=cR.BottomRight();
		if(point.x<0 && m_posactive)
		{
			m_posactive--;
			m_CurrSelect->ColSelect=m_posactive;
			m_fAscHor=1;
		}
		if(point.x>pt.x  && m_posactive+m_nbcasesvisu<m_nbcases){
			m_posactive++;
			m_CurrSelect->ColSelect=m_posactive+m_nbcasesvisu-1;
			m_fAscHor=1;
		}
	}
	else m_CurrSelect->ColSelect=(point.x/m_dx)+m_posactive;
	
	if(nFlags&MK_LBUTTON)
	{
		if(!IsSelectRules(0, m_CurrSelect->ColSelect))
		{
			MAJSelections();
			InvalidateRect(NULL,0);
			return;
		}
	}
	if(nFlags&MK_RBUTTON)
	{
		if(!IsSelectRules(1, m_CurrSelect->ColSelect))
		{
			MAJSelections();
			InvalidateRect(NULL,0);
			return;
		}
	}
	if(m_fAscHor)
	{
		WPARAM wParam=MAKEWPARAM(SB_THUMBPOSITION, m_posactive);
		GetParent()->SendMessage(WM_HSCROLL, wParam, (LPARAM)m_ptscroll->m_hWnd);
	}
	
	bool fCursor=0;
	if(!(nFlags & (MK_RBUTTON)))	if(!m_fCursor)	IsMouseOnCursor(point);
	if(!(nFlags & (MK_LBUTTON|MK_RBUTTON))) return;

	bool f=0;
	if(m_fTablier)
	{
		short curs1=GetPosCurseur(m_PosTablier1);
		short curs2=GetPosCurseur(m_PosTablier2);
		if(curs2<curs1)
		{
			short curs=curs1;
			curs1=curs2;
			curs2=curs;
		}

		if(m_Style&STYLE_CAL_CREUSET && (nFlags&(MK_LBUTTON)))
		{
			m_fTablierCreuset=1;
			f=1;
		}
		else
		{
			m_fTablierCreuset=0;
			if((m_CurrSelect->ColSelect>=curs1 && m_CurrSelect->ColSelect<curs2))	f=1;
		}
	}
//	ASSERT(AfxCheckMemory());
	if(m_CurrSelect->ColSelect<m_nbcases  && nFlags&(MK_LBUTTON|MK_RBUTTON))
	{
		short flag=m_fClickLevel;
		switch(GetClickInfo(point))
		{
		case 0:
			if(m_CurrSelect->LevelSelect==(CAL_LEVEL_JOUR|CAL_LEVEL_SEMAINE|CAL_LEVEL_MOIS|CAL_LEVEL_ANNEE))break;
			if(m_fAscHor || m_fCursor)	break;
			return;
		case 1:
		case 2:
		case 3:
			break;
		}

		if(m_fCursor && (nFlags & (MK_LBUTTON)))
		{
			if(m_SCurseur->Pos>m_CurrSelect->ColSelect)
				OnCurseurMoved(m_SCurseur->NumCurseur, m_CurrSelect->ColSelect);
			else
				if(m_SCurseur->Pos<m_CurrSelect->ColSelect)
					OnCurseurMoved(m_SCurseur->NumCurseur, m_CurrSelect->ColSelect+1);
			m_SCurseur->Pos=m_CurrSelect->ColSelect;
			if(m_SCurseur->Pos<0)	m_SCurseur->Pos=0;
			if(point.x<=0)
				m_SCurseur->R.left=m_RClient.left;
			else if(point.x>=m_RClient.right)
				m_SCurseur->R.left=m_RClient.right-1;
			else 
				m_SCurseur->R.left=point.x;
			m_SCurseur->R.right=m_SCurseur->R.left+2;
		}
		else if((!m_fTablier || (m_fTablier && f)) && !m_fCursor)
		{
			if(m_hCursorArrow)	::SetCursor(m_hCursorArrow);

			m_fSelect=0;
			short ModeSel=m_CurrSelect->ModeSel;
			switch(ModeSel)
			{
			case CAL_SIMPLE:
				m_CurrSelect->debSelect=m_CurrSelect->ColSelect;
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
				SetClickSelections(1);
				break;
			case CAL_MULTIPLE:
				m_CurrSelect->debSelect=m_CurrSelect->ColSelect;
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsTmp);
				SetCellsSelection(m_CurrSelect->ColSelect);
				break;
			case CAL_ETENDUE:
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
				SetClickSelections(1);
				for(i=min(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i<=max(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i++)
				{
					SetCellsSelection(i);
				}
				break;
			case CAL_ETENDUE_JFC:
				m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsTmp);
				SetClickSelections(1);
				for(short i=min(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i<=max(m_CurrSelect->debSelect, m_CurrSelect->ColSelect) ; i++)
				{
					SetCellsSelection(i);
				}
				break;
			}
//			OnSelectionChanged(m_CurrSelect->Selections);
		}
	}
	//verif si pointeur au dessus d'un curseur
//	ASSERT(AfxCheckMemory());
	MAJSelections();
//	ASSERT(AfxCheckMemory());
	InvalidateRect(NULL,0);
	CStatic::OnMouseMove(nFlags, point);
}

void CObj_Cal::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CObj_Cal::OnLButtonDown(nFlags, point);
	CStatic::OnLButtonDblClk(nFlags, point);
}

void CObj_Cal::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CObj_Cal::OnRButtonDown(nFlags, point);
	CStatic::OnRButtonDblClk(nFlags, point);
}

void CObj_Cal::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_fInit)	return;
	if(!m_fLBCapture) return;//Est ce que la capture appartient à l'objet cal???
	m_fLBCapture = false;
	if(!m_LeftSelect)	return;
//	ASSERT(AfxCheckMemory());
	if(m_fCursor)
	{
		short x, x1, x2, center;
		x1=m_dx*(m_SCurseur->Pos-m_posactive);
		x2=m_dx*(m_SCurseur->Pos+1-m_posactive);
		center=x1+((x2-x1)/2);

		if(point.x>center || point.x>m_RClient.right){
			x=x2;
			m_SCurseur->Pos+=1;
		}
		else	x=x1;

		m_SCurseur->R.left=x;
		m_SCurseur->R.right=m_SCurseur->R.left+2;
		m_fCursor=0;
		OnCurseurMoved(m_SCurseur->NumCurseur, m_SCurseur->Pos);
	}
	else
	{
		if(m_fSelect)
			m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
		short ModeSel=m_CurrSelect->ModeSel;
		if(m_Style & STYLE_CAL_CREUSET)
		{
			//on dispose le tablier aux extrémités de la sélection
			for(short i=0 ; i<m_nbcases ; i++)
				if(m_CurrSelect->Selections[i]){
					SetPosCurseur(m_PosTablier1, i);
					OnCurseurMoved(m_PosTablier1, i);					
					break;
				}
			for(i=m_nbcases-1 ; i>0 ; i--)
				if(m_CurrSelect->Selections[i]){
					SetPosCurseur(m_PosTablier2, i+1);
					OnCurseurMoved(m_PosTablier2, i+1);
					break;
				}
			m_fCapture=0;
		}
		else if(ModeSel==CAL_MULTIPLE)
				m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
//				memcpy(m_CurrSelect->SelectionsTmp, m_CurrSelect->Selections, m_nbcases);
	}
	if(m_Style & STYLE_CAL_CREUSET)	m_fTablierCreuset=0;
	m_RLevelSel.SetRectEmpty();

	if(!m_CurrSelect)	m_LeftSelect=m_CurrSelect;
	MAJSelections();
//	OnSelectionChanged(&m_LeftSelect->Selections, &m_RightSelect->Selections);
	OnSelectionChanged(&m_LeftSelect->Selections, NULL);
	if(m_pGrille)	m_pGrille->SetSelectionCalend(&m_LeftSelect->Selections);
	InvalidateRect(NULL,0);
	ReleaseCapture();
	KillTimer(1);
	CStatic::OnLButtonUp(nFlags, point);
//	ASSERT(AfxCheckMemory());
}

void CObj_Cal::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_fInit)	return;
	if(!m_fRBCapture) return;	//Est ce que la capture appartient à l'objet cal???
	m_fRBCapture = false;
	if(!m_RightSelect)	return;
	if(m_fSelect)	m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
	if(m_CurrSelect->ModeSel==CAL_MULTIPLE)
		m_CurrSelect->SelectionsTmp.Copy(m_CurrSelect->Selections);
	
	m_fSelect=0;
	m_RLevelSel.SetRectEmpty();	
	if(!m_CurrSelect)	m_RightSelect=m_CurrSelect;
	MAJSelections();
	OnSelectionChanged(NULL, &m_RightSelect->Selections);
	InvalidateRect(NULL,0);
	ReleaseCapture();
	KillTimer(1);
	CStatic::OnRButtonUp(nFlags, point);
}

void CObj_Cal::OnTimer(UINT nIDEvent) 
{
	CRect CR(m_RClient);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if(!CR.PtInRect(pt))
		CObj_Cal::OnMouseMove(m_nFlags, pt);
	CStatic::OnTimer(nIDEvent);
}

short CObj_Cal::SetCurseur(short NumCurs, short Pos, COLORREF Color)
{
	SCurseur * curseur=new SCurseur;
	curseur->Pos=Pos;
	if(curseur->Pos>m_nbcases)		curseur->Pos=m_nbcases;
	if(curseur->Pos<0)				curseur->Pos=0;
	curseur->Color=Color;
	curseur->NumCurseur=NumCurs;
	curseur->Select=0;
	curseur->R. SetRectEmpty();
	m_nbcurseur++;
	m_ArrayCurseur->Add(curseur);
	return m_nbcurseur;
}

short CObj_Cal::DelCurseur(short NumCursor)
{
	SCurseur * curseur;
	if((curseur = (SCurseur *)m_ArrayCurseur->GetAt(NumCursor)) != NULL)
	{
		m_ArrayCurseur->RemoveAt(NumCursor);
		delete curseur;
		m_ArrayCurseur->FreeExtra();
	}
	return 1;
}

bool CObj_Cal::IsMouseOnCursor(CPoint pt)
{
	for(short i=0 ; i<m_nbcurseur ; i++)
	{
		if((m_SCurseur = (SCurseur *)m_ArrayCurseur->GetAt(i)) != NULL)
		{
			m_SCurseur->Select=0;
			if(m_SCurseur->R.PtInRect(pt))
			{
				m_SCurseur->Select=1;
				if(m_hCursorSizewe)	::SetCursor(m_hCursorSizewe);
				return 1;
			}
		}
	}
	return 0;
}

short CObj_Cal::SetTablier(short Curseur1, short Curseur2, COLORREF colorTablier)
{
	m_fTablier=1;

	m_PosTablier1=Curseur1;
	m_PosTablier2=Curseur2;
	m_ColorTablier=colorTablier;

	InvalidateRect(NULL,0);
	return 1;
}

short CObj_Cal::SetPosCurseur(short ncurs, short NewPos)
{
	SCurseur * curseur;
	for(short i=0 ; i<m_nbcurseur ; i++)
	{
		if((curseur = (SCurseur *)m_ArrayCurseur->GetAt(i)) != NULL)
		{
			if(curseur->NumCurseur==ncurs)
			{
				curseur->Pos=NewPos;
				if(curseur->Pos<0)	curseur->Pos=0;
				if(curseur->Pos>m_nbcases)	curseur->Pos=m_nbcases;
				InvalidateRect(NULL, 0);
				return 1;
			}
		}
	}
	if(m_pGrille)m_pGrille->SetPosCurseur(ncurs, NewPos);
	return 0;
}

short CObj_Cal::GetPosCurseur(short ncurs)
{
	SCurseur * curseur;
	for(short i=0 ; i<m_nbcurseur ; i++)
	{
		if((curseur = (SCurseur *)m_ArrayCurseur->GetAt(i)) != NULL)
		{
			if(curseur->NumCurseur==ncurs)	return curseur->Pos;
		}
	}
	return -1;
}

void CObj_Cal::MAJSelections(void)
{
	bool f=0;
	if(m_LeftSelect)
	{
		for(short i=0 ; i<m_nbcases ; i++)
		{
			if(m_LeftSelect->Selections[i] && !f)
			{
				if(IsSelectRules(0, i))
				{
					f=1;
					m_LeftSelect->Selections[i]=2;
					m_LeftSelect->debL=i;
				}
				else m_LeftSelect->Selections[i]=0;
			}
			else
			{
				if(m_LeftSelect->Selections[i] && f)
				{
					if(IsSelectRules(0, i))	m_LeftSelect->Selections[i]=1;
					else 					m_LeftSelect->Selections[i]=0;
				}
				else
				{
					if(!m_LeftSelect->Selections[i] && f)
					{
						if(IsSelectRules(0, i-1))
						{
							f=0;
							if(m_LeftSelect->Selections[i-1]==2)	m_LeftSelect->Selections[i-1]=4;
							else									m_LeftSelect->Selections[i-1]=3;
							m_LeftSelect->debR=i-1;
						}
						else 	m_LeftSelect->Selections[i-1]=0;
					}
					else m_LeftSelect->Selections[i]=0;
				}
			}
		}
	}
	f=0;
	if(m_RightSelect)
	{
		for(short i=0 ; i<m_nbcases ; i++)
		{
			if(m_RightSelect->Selections[i] && !f)
			{
				if(IsSelectRules(1, i))
				{
					f=1;
					m_RightSelect->Selections[i]=2;
				}
				else 	m_RightSelect->Selections[i]=0;
			}
			else
				if(m_RightSelect->Selections[i] && f)
				{
					if(IsSelectRules(1, i))	m_RightSelect->Selections[i]=1;
					else 	m_RightSelect->Selections[i]=0;
				}
				else
					if(!m_RightSelect->Selections[i] && f)
					{
						if(IsSelectRules(1, i-1))
						{
							f=0;
							if(m_RightSelect->Selections[i-1]==2)	m_RightSelect->Selections[i-1]=4;
							else	m_RightSelect->Selections[i-1]=3;
						}
						else 	m_RightSelect->Selections[i-1]=0;
					}
					else 	m_RightSelect->Selections[i]=0;
		}
	}
	if(m_fTablier  && !m_fCursor)
	{
		short curs1=GetPosCurseur(m_PosTablier1);
		short curs2=GetPosCurseur(m_PosTablier2);
		if(m_LeftSelect)
		{
			for(short i=0 ; i<min(curs1, curs2) ; i++)		m_LeftSelect->Selections[i]=0;
			for(i=max(curs1, curs2) ; i<m_nbcases ; i++)	m_LeftSelect->Selections[i]=0;
		}
		if(m_RightSelect)
		{
			for(short i=0 ; i<min(curs1, curs2) ; i++)		m_RightSelect->Selections[i]=0;
			for(i=max(curs1, curs2) ; i<m_nbcases ; i++)	m_RightSelect->Selections[i]=0;
		}
	}
}

short CObj_Cal::GetClickInfo(CPoint point)
{
//Selection en fonction du click sur les jours, ou semaines ou mois
	m_fClickLevel = 0;
	if(m_dyJour && m_RJour.PtInRect(point))
	{
		OnLevelSelect(CAL_LEVEL_JOUR, m_nFlags);
		if(m_CurrSelect->LevelSelect & (CAL_LEVEL_JOUR))
		{
			m_fClickLevel = 1;
			m_CurrSelect->RSelect = m_RJour;
			m_colorBg = m_colorBgJour;
		}
	}
	if(m_dySem && m_RSem.PtInRect(point))
	{
		OnLevelSelect(CAL_LEVEL_SEMAINE, m_nFlags);
		if(m_CurrSelect->LevelSelect & (CAL_LEVEL_SEMAINE))
		{
			m_fClickLevel = 2;
			if(m_CurrSelect->TypeSelect)	//click droit STYLE PIGE
			{
				m_CurrSelect->RSelect = m_RSem;
				m_colorBg = m_colorBgSem;
			}
			else
			{
				if(m_dxJour)
				{
					m_CurrSelect->RSelect = m_RJour;
					m_colorBg = m_colorBgJour;
				}
				else
				{
					if(m_dxSem)
					{
						m_CurrSelect->RSelect = m_RSem;
						m_colorBg = m_colorBgSem;
					}
				}
			}
		}
	}
	if(m_dyMois && m_RMois.PtInRect(point))
	{
		OnLevelSelect(CAL_LEVEL_MOIS, m_nFlags);
		if(m_CurrSelect->LevelSelect & (CAL_LEVEL_MOIS))
		{
			m_fClickLevel = 3;
			if(m_CurrSelect->TypeSelect)	//click droit STYLE PIGE
			{
				m_CurrSelect->RSelect = m_RMois;
				m_colorBg = m_colorBgMois;
			}
			else
			{
				if(m_dxJour)
				{
					m_CurrSelect->RSelect = m_RJour;
					m_colorBg = m_colorBgJour;
				}
				else
				{
					if(m_dxSem)
					{
						m_CurrSelect->RSelect = m_RSem;
						m_colorBg = m_colorBgSem;
					}
					else
					{
						if(m_dxMois)
						{
							m_CurrSelect->RSelect = m_RMois;
							m_colorBg = m_colorBgMois;
						}
					}
				}
			}
		}
	}
	if(m_dyAnnee && m_RAnnee.PtInRect(point))
	{
		OnLevelSelect(CAL_LEVEL_ANNEE, m_nFlags);
		if(m_CurrSelect->LevelSelect & (CAL_LEVEL_ANNEE))
		{
			m_fClickLevel = 4;
			if(m_CurrSelect->TypeSelect)	//click droit STYLE PIGE
			{
				m_CurrSelect->RSelect = m_RAnnee;
				m_colorBg = m_colorBgAnnee;
			}
			else
			{
				if(m_dxJour)
				{
					m_CurrSelect->RSelect = m_RJour;
					m_colorBg = m_colorBgJour;
				}
				else
				{
					if(m_dxSem)
					{
						m_CurrSelect->RSelect = m_RSem;
						m_colorBg = m_colorBgSem;
					}
					else
					{
						if(m_dxMois){
							m_CurrSelect->RSelect = m_RMois;
							m_colorBg = m_colorBgMois;
						}
						else{
							m_CurrSelect->RSelect = m_RAnnee;
							m_colorBg = m_colorBgAnnee;
						}
					}
				}
			}
		}
	}

	return m_fClickLevel;
}
///////////////////////////////////////////////////////////////////

bool CObj_Cal::SetClickSelections(short f)
{
	long a1, a2, m1, m2, s1, s2, s3, s4;
	
	
	m_RLevelSel.SetRectEmpty();
	//Met la valeur à la place suivant la config
	//Replace la valeur initiale du niveau de sélection
	switch(m_fClickLevel)
	{
	case 0:
		return 0;
		break;
	case 1:		//jour
		if(f == 0)	//nouvelle selection
		{
			DetectModeSelect();
		}

		SetCellsSelection(m_CurrSelect->ColSelect);

		//selection pige : selection définie les bornes du tablier
		if(m_fTablierCreuset)
		{
			if(m_CurrSelect->debSelect > m_CurrSelect->ColSelect)
			{
				SetPosCurseur(m_PosTablier1, m_CurrSelect->ColSelect);
				SetPosCurseur(m_PosTablier2, m_CurrSelect->debSelect + 1);
			}
			else
			{
				SetPosCurseur(m_PosTablier1, m_CurrSelect->debSelect);
				SetPosCurseur(m_PosTablier2, m_CurrSelect->ColSelect + 1);
			}
		}
		break;
	case 2:		//semaine
		{
			//_________________________
			//|_______|____*__|_______|
			//		  s1  sel s2	  

			//obtient les bornes du tablier
			short curs1, curs2;
			short s1 = m_CurrSelect->ColSelect;
			if(m_fTablier)
			{
				curs1 = GetPosCurseur(m_PosTablier1);
				curs2 = GetPosCurseur(m_PosTablier2);
				if(curs2 < curs1)
				{
					short curs = curs1;
					curs1 = curs2;
					curs2 = curs;
				}
			}
			else
			{
				curs1 = 0;
			}
			while((	m_CoupureCalend[s1] != 1
					&& m_CoupureCalend[s1] != 3
					&& m_CoupureCalend[s1] != 5
					&& m_CoupureCalend[s1] != 7)
					&& (s1 > curs1)
					&& IsSelectRules(m_CurrSelect->TypeSelect, s1))
			{
				s1--;
			}

			//on surligne le niveau de selection semaine en changeant la couleur de la semaine selectionné
			//on doit connaitre les bords
			short s2 = s1 + 1;
			if(!m_fTablier)
			{
				curs2 = m_nbcases;
			}
			while((	m_CoupureCalend[s2] != 1
					&& m_CoupureCalend[s2] != 3
					&& m_CoupureCalend[s2] != 5
					&& m_CoupureCalend[s2] != 7)
					&& (s2 < curs2)
					&& IsSelectRules(m_CurrSelect->TypeSelect, s2))
			{
				s2++;
			}
			//on connait les bords : i et k...maintenant le rectangle

			if(f == 0)	//selection si f=1
			{
				DetectModeSelect(s1, s2, curs2);
			}
			//on connait les bords :i et k...maintenant le rectangle
			if(m_Style&STYLE_CAL_PIGE)
			{
				m_RLevelSel.SetRectEmpty();
			}
			else
			{
				m_RLevelSel.SetRect((s1 - m_posactive) * m_dx, (m_ySem == 0 ? m_ySem + 1 : m_ySem), (s2 - m_posactive) * m_dx, m_ySem + m_dySem);
				if(m_fModeSelect) m_RLevelSel.right -= 1;	//on repositionne bien le trait du curseur droit
			}

			//selection creuset : selection définie les bornes du tablier
			if(m_fTablierCreuset)
			{
				SetPosCurseur(m_PosTablier1, min(s1, m_CurrSelect->debL));
			}
			
			//on place les selections
			for(long j = s1 ; j < s2 - 1 ; j++)
			{
				if((j + 1 < curs2) && IsSelectRules(m_CurrSelect->TypeSelect, j + 1))
				{
					SetCellsSelection(j);
					if(j > m_nbcases)
					{
						break;
					}
				}
			}
			SetCellsSelection(j);
			if(m_fTablierCreuset)
			{
				SetPosCurseur(m_PosTablier2, max(j + 1, m_CurrSelect->debR + 1));
			}
			if(f == 0)	//selection si f=1
			{
				m_CurrSelect->debL = s1;
				m_CurrSelect->debR = j;
			}
			break;
		}
	case 3:		//mois
		{
			//_________________________________________________
			//|__________|___________mois__*_____________|____|
			//|_______|__sem1_|_sem2__|_sem3__|__sem4_|_sem5__|
			//		     m1	    		  sel		     m2   

			//obtient les bornes du tablier
			short curs1, curs2;
			short sel = m_CurrSelect->ColSelect + 1;
			if(m_fTablier)
			{
				curs1 = GetPosCurseur(m_PosTablier1);
				curs2 = GetPosCurseur(m_PosTablier2);
				if(curs2 < curs1)
				{
					short curs = curs1;
					curs1 = curs2;
					curs2 = curs;
				}
			}
			else
			{
				curs1 = 0;
			}

			//Le mois c'est 2, 3, 6 ou 7
			m1 = sel;
			while((	m_CoupureCalend[m1] != 2
					&& m_CoupureCalend[m1] != 3
					&& m_CoupureCalend[m1] != 6
					&& m_CoupureCalend[m1] != 7)
					&& (m1 - 1 > curs1)
					&& IsSelectRules(m_CurrSelect->TypeSelect, m1 - 1))
			{
				m1--;
			}
			m2 = m1;

			//Si mode HEBDO
			//recherche des bornes des semaines à cheval avec la borne m1
			//_________________________________________________
			//|__________|___________mois________________|____|
			//|_______|__sem1_|_sem2__|_sem3__|__sem4_|_sem5__|
			//		  s1 m1	  s2					  s3 m2   s4


			//on doit connaitre les bords s1 et s2
			//si s1m1 < m1s2, on agrandi la sélection jusqu'à s1, sinon on diminue jsuqu'à s2
			//de même pour l'autre limite m2

			if(m_CurrSelect->fOption == CAL_MOIS_HEBDOMADAIRE)
			{
				s1 = m1;
				s2 = m1 + 1;
				while((	m_CoupureCalend[s1] != 1
						&& m_CoupureCalend[s1] != 3
						&& m_CoupureCalend[s1] != 5
						&& m_CoupureCalend[s1] != 7)
						&& (s1 > curs1)
						&& IsSelectRules(m_CurrSelect->TypeSelect, s1))
				{
					s1--;
				}
				while((	m_CoupureCalend[s2] != 1
						&& m_CoupureCalend[s2] != 3
						&& m_CoupureCalend[s2] != 5
						&& m_CoupureCalend[s2] != 7)
						&& (s2 > curs1)
						&& IsSelectRules(m_CurrSelect->TypeSelect, s2))
				{
					s2++;
				}
				if(m1 - s1 <= s2 - m1 + 1)
				{
					m1 = s1;
				}
				else
				{
					m1 = s2;
				}
				m1++;
			}
			
			if(!m_fTablier)
			{
				curs2 = m_nbcases;
			}
			while((	m_CoupureCalend[m2 + 1] != 2
					&& m_CoupureCalend[m2 + 1] != 3
					&& m_CoupureCalend[m2 + 1] != 6
					&& m_CoupureCalend[m2 + 1] != 7)
					&& (m2 < curs2)
					&& IsSelectRules(m_CurrSelect->TypeSelect, m2))
			{
				m2++;
			}

			//on doit connaitre les bords s3 et s4
			//si s3m2 < m2s4, on agrandi la sélection jusqu'à s3, sinon on diminue jsuqu'à s4
			//de même pour l'autre limite m2

			if(m_CurrSelect->fOption == CAL_MOIS_HEBDOMADAIRE)
			{
				s3 = m2;
				s4 = m2+1;
				while((	m_CoupureCalend[s3] != 1
						&& m_CoupureCalend[s3] != 3
						&& m_CoupureCalend[s3] != 5
						&& m_CoupureCalend[s3] != 7)
						&& (s3 < curs2)
						&& IsSelectRules(m_CurrSelect->TypeSelect, s3))
				{
					s3--;
				}
				while((	m_CoupureCalend[s4] != 1
						&& m_CoupureCalend[s4] != 3
						&& m_CoupureCalend[s4] != 5
						&& m_CoupureCalend[s4] != 7)
						&& (s4 < curs2)
						&& IsSelectRules(m_CurrSelect->TypeSelect, s4))
				{
					s4++;
				}
				if(m2 - s3 < s4 - m2)
				{
					m2 = s3;
				}
				else
				{
					m2 = s4;
				}
			}

			//sélection ou deselection ?
			if(f == 0)
			{
				DetectModeSelect(m1, m2, curs2);
			}

			//on connait les bords :m1 et m2...maintenant le rectangle pour l'affichage
			if(m_Style&STYLE_CAL_PIGE)
			{
				m_RLevelSel.SetRectEmpty();
			}
			else
			{
				if(m_CurrSelect->fOption == CAL_MOIS_HEBDOMADAIRE)
				{
					m_RLevelSel.SetRect((m1 - m_posactive - 1) * m_dx, (m_ySem == 0 ? m_ySem + 1 : m_ySem), (m2 - m_posactive) * m_dx, m_ySem + m_dySem);
					if(m_fModeSelect) m_RLevelSel.right -= 1;	//on repositionne bien le trait du curseur droit
				}
				else
				{
					m_RLevelSel.SetRect((m1 - m_posactive - 1) * m_dx, (m_yMois == 0 ? m_yMois + 1 : m_yMois), (m2 - m_posactive) * m_dx, m_yMois + m_dyMois);
					if(m_fModeSelect) m_RLevelSel.right -= 1;	//on repositionne bien le trait du curseur droit
				}
			}

			//(de)selectionne la première cellule
			switch(m_fModeSelect)
			{
			case 0:		//deselection
				if(m1)	m_CurrSelect->Selections[m1 - 1] = 0;
				else	m_CurrSelect->Selections[m1] = 0;
				break;
			case 1:		//selection
				if(m1)	m_CurrSelect->Selections[m1 - 1] = 1;
				else	m_CurrSelect->Selections[m1] = 1;
				break;
			case 2:		//selection gauche et deselection droite
				if(m1)
				{
					m_LeftSelect->Selections[m1 - 1]		= 1;
					m_RightSelect->Selections[m1 - 1]		= 0;
				}
				else
				{
					m_LeftSelect->Selections[m1]		= 1;
					m_RightSelect->Selections[m1]		= 0;
				}
				break;
			case 3:		//selection droite et deselection gauche
				if(m1)
				{
					m_LeftSelect->Selections[m1 - 1]		= 0;
					m_RightSelect->Selections[m1 - 1]		= 1;
				}
				else
				{
					m_LeftSelect->Selections[m1]		= 0;
					m_RightSelect->Selections[m1]		= 1;
				}
				break;
			default:
				break;
			}
			//???ce code doit être nécessaire...mais deja fait plus haut ... (???)
			if(m_fModeSelect)
			{
				if(m1)
				{
					if(m_fTablierCreuset)
					{
						SetPosCurseur(m_PosTablier1, min(m1 - 1, m_CurrSelect->debL));
					}
				}
				else
				{
					if(m_fTablierCreuset)
					{
						SetPosCurseur(m_PosTablier1, min(m1, m_CurrSelect->debL));
					}
				}
			}
			///???
			
			//(de)selection
			for(long j = m1 ; j < m2 ; j++)
			{
				if((j < curs2) && IsSelectRules(m_CurrSelect->TypeSelect, j)
					&& j<m_nbcases)
				{
					SetCellsSelection(j);
				}
			}
			
			//2eme curseur
			if(m_fTablierCreuset)
			{
				SetPosCurseur(m_PosTablier2, max(j, m_CurrSelect->debR + 1));
			}

			//sauvegarde les extremites pour plus tard
			if(f == 0)
			{
				if(m1)
				{
					m_CurrSelect->debL = m1 - 1;
				}
				else
				{
					m_CurrSelect->debL = 0;
				}
				m_CurrSelect->debR = j - 1;
			}
			break;
		}
	case 4:		//Annee
		{

			//_________________________________________________________...
			//|_____________|________annee________*____________________...
			//|__________|___________mois_______________|______________...
			//|_______|__sem1_|_sem2__|_sem3__|__sem4_|_sem5__|_sem6__|...
			//		     m1	a1    				 sel    m2   

			//CODE A AJOUTER ICI
			//////////////////////////////////////////
			//obtient les bornes du tablier
			short curs1, curs2;
			a1 = m_CurrSelect->ColSelect + 1;
			if(m_fTablier)
			{
				curs1 = GetPosCurseur(m_PosTablier1);
				curs2 = GetPosCurseur(m_PosTablier2);
				if(curs2 < curs1)
				{
					short curs = curs1;
					curs1 = curs2;
					curs2 = curs;
				}
			}
			else
			{
				curs1 = 0;
			}
			while((	m_CoupureCalend[a1] != 4
					&& m_CoupureCalend[a1] != 5
					&& m_CoupureCalend[a1] != 6
					&& m_CoupureCalend[a1] != 7)
					&& (a1 > curs1)
					&& IsSelectRules(m_CurrSelect->TypeSelect, a1))
			{
				a1--;
			}

			//on surligne le niveau de selection semaine en changeant la couleur de la semaine selectionné
			//on doit connaitre les bords

			a2 = a1;
			if(!m_fTablier)
			{
				curs2 = m_nbcases;
			}
			while((	m_CoupureCalend[a2 + 1] != 4
					&& m_CoupureCalend[a2 + 1] != 5
					&& m_CoupureCalend[a2 + 1] != 6
					&& m_CoupureCalend[a2 + 1] != 7)
					&& (a2 < curs2)
					&& IsSelectRules(m_CurrSelect->TypeSelect, a2))
			{
				a2++;
			}
			//La sélection maintenant
			if(f == 0)
			{
				DetectModeSelect(a1, a2, curs2);
			}
			//on connait les bords :i et k...maintenant le rectangle
			if(m_Style&STYLE_CAL_PIGE)
			{
				m_RLevelSel.SetRectEmpty();
			}
			else
			{
				m_RLevelSel.SetRect((a1 - m_posactive - 1) * m_dx, (m_yAnnee == 0 ? m_yAnnee + 1 : m_yAnnee), (a2 - m_posactive) * m_dx, m_yAnnee + m_dyAnnee);
				if(m_fModeSelect) m_RLevelSel.right -= 1;	//on repositionne bien le trait du curseur droit
			}

			switch(m_fModeSelect)
			{
			case 0:		//deselection
				if(a1)	m_CurrSelect->Selections[a1 - 1] = 0;
				else	m_CurrSelect->Selections[a1] = 0;
				break;
			case 1:		//selection
				if(a1)	m_CurrSelect->Selections[a1 - 1] = 1;
				else	m_CurrSelect->Selections[a1] = 1;
				break;
			case 2:		//selection gauche et deselection droite
				if(a1)
				{
					m_LeftSelect->Selections[a1 - 1]		= 1;
					m_RightSelect->Selections[a1 - 1]		= 0;
				}
				else
				{
					m_LeftSelect->Selections[a1]		= 1;
					m_RightSelect->Selections[a1]		= 0;
				}
				break;
			case 3:		//selection droite et deselection gauche
				if(a1)
				{
					m_LeftSelect->Selections[a1 - 1]		= 0;
					m_RightSelect->Selections[a1 - 1]		= 1;
				}
				else
				{
					m_LeftSelect->Selections[a1]		= 0;
					m_RightSelect->Selections[a1]		= 1;
				}
				break;
			default:
				break;
			}

			//Suite....et fin
			if(m_fModeSelect)
			{
				if(a1)
				{
					if(m_fTablierCreuset)
					{
						SetPosCurseur(m_PosTablier1, min(a1 - 1, m_CurrSelect->debL));
					}
				}
				else
				{
					if(m_fTablierCreuset)
					{
						SetPosCurseur(m_PosTablier1, min(a1, m_CurrSelect->debL));
					}
				}
			}
			for(long j = a1 ; j < a2 ; j++)
			{
				if((j < curs2) && IsSelectRules(m_CurrSelect->TypeSelect, j)
					&& j<m_nbcases)
				{
					SetCellsSelection(j);
				}
			}

			if(m_fTablierCreuset)
			{
				SetPosCurseur(m_PosTablier2, max(j, m_CurrSelect->debR + 1));
			}
			if(f == 0)
			{
				if(a1)
				{
					m_CurrSelect->debL = a1 - 1;
				}
				else
				{
					m_CurrSelect->debL = 0;
				}
				m_CurrSelect->debR = j - 1;
			}

		break;
		}
	default:
		return 0;
	}
	return 1;
}

short CObj_Cal::MAJPosScrollH(UINT nSBCode, UINT nPos, short flag)
{
	int i,mini,maxi;

	i=m_ptscroll->GetScrollPos();
	m_ptscroll->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_LEFT:		m_ptscroll->SetScrollPos(mini,1); break;
		case SB_ENDSCROLL:break;
		case SB_LINELEFT:	m_ptscroll->SetScrollPos(max(i-1, mini),1); break;
		case SB_LINERIGHT:	m_ptscroll->SetScrollPos(min(i+1, maxi),1); break;
		case SB_PAGELEFT:	m_ptscroll->SetScrollPos(max((i-m_nbCol-1), mini),1); break;
		case SB_PAGERIGHT:	m_ptscroll->SetScrollPos(min((i+m_nbCol-1), maxi),1); break;
		case SB_RIGHT:		m_ptscroll->SetScrollPos(maxi,1); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_ptscroll->SetScrollPos(nPos,1); break;
	}

	short pos=m_ptscroll->GetScrollPos();
	if(flag)
	{
		SetPosition(pos, 0);
		InvalidateRect(NULL,0);
		if(m_pGrille){
			m_pGrille->SetPosition(pos, 0);
			m_pGrille->InvalidateRect(NULL,0);
		}
	}
	return(pos);
}

void CObj_Cal::SetLevelSelect(short Select, bool fRAZSel)
{
	if(m_Style & STYLE_CAL_PIGE)	return;
	switch(m_SelectRules)
	{
	case CAL_INDEPENDANTE:
	case CAL_CROISEE:
	case CAL_LEFTINTORIGHT:
		if(m_LeftSelect)	m_CurrSelect=m_LeftSelect;
		else return;
		break;
	case CAL_RIGHTINTOLEFT:
		if(m_RightSelect)	m_CurrSelect=m_RightSelect;
		else return;
		break;
	}
	if(m_CurrSelect->LevelSelect==Select)	return;
	m_CurrSelect->LevelSelect=Select;
	if(fRAZSel)	//remise à zero de la sélection si changement de niveau
	{	
		m_CurrSelect->Selections.Copy(m_CurrSelect->SelectionsRAZ);
		InvalidateRect(NULL, 0);
		return;
	}
	if(m_CurrSelect->LevelSelect&(CAL_LEVEL_JOUR))
	{
		m_CurrSelect->RSelect=m_RJour;
		InvalidateRect(NULL, 0);
		return;
	}
	//Mise a jour de la selection
	//passage de la selection jour->semaine ou semaine->mois ou jour->mois
	for(short i=0 ; i<m_nbcases ; i++)
	{
		if(m_CurrSelect->Selections[i])
		{
			if(m_CurrSelect->LevelSelect & (CAL_LEVEL_SEMAINE))
			{
				short j1 = i;
				while(m_CoupureCalend[j1] != 1 && m_CoupureCalend[j1] != 3 && m_CoupureCalend[j1] != 5 && m_CoupureCalend[j1] != 7){
					j1--;
				}
				short j2 = i;
				//ajout de +1 pour tester la bonne coupure
				while(m_CoupureCalend[j2 + 1] != 1 && m_CoupureCalend[j2 + 1] != 3 && m_CoupureCalend[j2 + 1] != 5 && m_CoupureCalend[j2 + 1] != 7){
					j2++;
				}
				m_CurrSelect->Selections[j1] = 2;
				for(short k = j1 + 1 ; k <= j2 ; k++){
					m_CurrSelect->Selections[k] = 1;
				}
				m_CurrSelect->Selections[j2] = 3;
				i = j2;
				m_CurrSelect->debL = j1;
				m_CurrSelect->debR = j2;
				if(m_CurrSelect->TypeSelect){
					m_CurrSelect->RSelect = m_RSem;
				}
			}
			else
			{
				if(m_CurrSelect->LevelSelect & (CAL_LEVEL_MOIS))
				{
					short j1 = i;
					while(m_CoupureCalend[j1 + 1] != 2 && m_CoupureCalend[j1 + 1] != 3 && m_CoupureCalend[j1 + 1] != 6 && m_CoupureCalend[j1 + 1] != 7){
						j1--;
					}
					if(j1 < 0){
						j1 = 0;
					}
					short j2 = i + 1;
					//ajout de +1 pour teste la bonne coupure
					while(m_CoupureCalend[j2 + 1] != 2 && m_CoupureCalend[j2 + 1] != 3 && m_CoupureCalend[j2 + 1] != 6 && m_CoupureCalend[j2 + 1] != 7){
						j2++;
					}
					m_CurrSelect->Selections[j1] = 2;
					for(short k = j1 + 1 ; k < j2 - 1 ; k++){
						m_CurrSelect->Selections[k] = 1;
					}
					m_CurrSelect->Selections[j2 - 1] = 3;
					i = j2 - 1;
					m_CurrSelect->debL = j1;
					m_CurrSelect->debR = j2;
					if(m_CurrSelect->TypeSelect){
						m_CurrSelect->RSelect = m_RMois;
					}
				}
				else
				{
					//CODE A AJOUTER ICI
					//////////////////////////////////////////
					if(m_CurrSelect->LevelSelect & (CAL_LEVEL_ANNEE))
					{
						short j1 = i;
						while(m_CoupureCalend[j1 + 1] != 4 && m_CoupureCalend[j1 + 1] != 5 && m_CoupureCalend[j1 + 1] != 6 && m_CoupureCalend[j1 + 1] != 7)
						{
							j1--;
						}
						if(j1 < 0)
						{
							j1 = 0;
						}
						short j2 = i + 1;
						//ajout de +1 pour teste la bonne coupure
						while(m_CoupureCalend[j2 + 1] != 4 && m_CoupureCalend[j2 + 1] != 5 && m_CoupureCalend[j2 + 1] != 6 && m_CoupureCalend[j2 + 1] != 7)
						{
							j2++;
						}
						m_CurrSelect->Selections[j1] = 2;
						for(short k = j1 + 1 ; k < j2 - 1 ; k++)
						{
							m_CurrSelect->Selections[k] = 1;
						}
						m_CurrSelect->Selections[j2 - 1] = 3;
						i = j2 - 1;
						m_CurrSelect->debL = j1;
						m_CurrSelect->debR = j2;
						if(m_CurrSelect->TypeSelect)
						{
							m_CurrSelect->RSelect = m_RAnnee;
						}
					}
				}
			}
		}
	}
	MAJSelections();
	InvalidateRect(NULL, 0);
}

bool CObj_Cal::IsSelectRules(short click, short pos)
{
	if(pos > m_nbcases || pos < 0)	return false;
	switch(click)
	{
	case 0:	//left
		if(!m_RightSelect)	return true;
		switch(m_SelectRules)
		{
		case CAL_INDEPENDANTE:	if(m_RightSelect->Selections[pos])	return false;
			return true;
		case CAL_CROISEE:		return true;
		case CAL_LEFTINTORIGHT:	if(m_RightSelect->Selections[pos])	return true;
			return false;
		case CAL_RIGHTINTOLEFT:	return true;
		case CAL_PREGNANCE:		return true;
		case CAL_OPPOSITION:	return true;
		}
		break;
	case 1:	//right
		if(!m_LeftSelect)	return true;
		switch(m_SelectRules)
		{
		case CAL_INDEPENDANTE:	if(m_LeftSelect->Selections[pos])	return false;
			return true;
		case CAL_CROISEE:		return true;
		case CAL_LEFTINTORIGHT:	return true;
		case CAL_RIGHTINTOLEFT:	if(m_LeftSelect->Selections[pos])	return true;
			return false;
		case CAL_PREGNANCE:		return true;
		case CAL_OPPOSITION:	return true;
		}
		break;
	}
	return false;
}

void CObj_Cal::SetSelections(CByteArray * Selections, CByteArray * SurSelections)
{
	if(Selections && m_LeftSelect)
	{
		m_LeftSelect->Selections.Copy(*Selections);
		m_fCapture=1;
		if(m_pGrille)	m_pGrille->SetSelectionCalend(Selections);
	}
	if(SurSelections && m_RightSelect)
	{
		m_RightSelect->Selections.Copy(*SurSelections);
		m_fSurCapture=1;
	}
	if(m_fCapture || m_fSurCapture)	MAJSelections();
	InvalidateRect(NULL, 0);
}

bool CObj_Cal::OnSelectionChanged(CByteArray * Selection, CByteArray * SurSelection)
{
	return true;
}

bool CObj_Cal::OnCurseurMoved(short NumCurs, short Pos)
{
	return true;
}

bool CObj_Cal::OnLevelSelect(short Level, UINT nFlags)
{
	return true;
}

void CObj_Cal::ChangeSelection(SSelection * SelFrom)
{
	short nbcases=0;
	SSelection SelTo;
	
	SelTo.Selections.SetSize(m_nbcases);
	SelTo.SelectionsRAZ.SetSize(m_nbcases);
	SelTo.SelectionsTmp.SetSize(m_nbcases);
	if(m_nbcases<SelFrom->Selections.GetSize())	nbcases=m_nbcases;
	else nbcases=SelFrom->Selections.GetSize();
	for(short i=0 ; i<nbcases ; i++){
		SelTo.Selections[i]=SelFrom->Selections[i];
		SelTo.SelectionsRAZ[i]=SelFrom->SelectionsRAZ[i];
		SelTo.SelectionsTmp[i]=SelFrom->SelectionsTmp[i];
	}
	SelFrom->Selections.RemoveAll();
	SelFrom->Selections.Copy(SelTo.Selections);
	SelFrom->SelectionsRAZ.RemoveAll();
	SelFrom->SelectionsRAZ.Copy(SelTo.SelectionsRAZ);
	SelFrom->SelectionsTmp.RemoveAll();
	SelFrom->SelectionsTmp.Copy(SelTo.SelectionsTmp);
}

void CObj_Cal::ChangePeriode(COleDateTime JourDeb, short NbCases, bool fUpdate)
{
	m_jourdeb=JourDeb;
	m_nbcases=NbCases;
	//on remet la bonne initialisations du nombre de cases
	if(m_Style&OPTION_CAL_PRINTER)	m_RClient=m_PrntRect;
	else	GetClientRect(&m_RClient);

	m_nbcasesvisu=m_RClient.Width()/m_dx;
	if(m_nbcasesvisu>m_nbcases)
	{
		m_nbcasesvisu=m_nbcases;
		m_RClient.right=m_RClient.left+m_dx*m_nbcasesvisu;
	}

	//on réinitialise le scroll 
	m_nbCol=m_nbcasesvisu;
	if(m_ptscroll){
		m_ptscroll->SetScrollRange(0, (m_nbcases-m_nbcasesvisu),0);
		m_ptscroll->SetScrollPos(m_posactive,true);
	}

	ConstruitCalendrier();
	CalculCoupure();
	//Remise à jour de la sélection
	if(m_LeftSelect)	ChangeSelection(m_LeftSelect);
	if(m_RightSelect)	ChangeSelection(m_RightSelect);
	if(fUpdate)	InvalidateRect(NULL, 0);
}

void CObj_Cal::ChangeLeftSelectionColor(COLORREF color, bool flagRedraw)
{
	if(!m_LeftSelect)return;
	m_LeftSelect->ColorSel=color;
	if(flagRedraw)Invalidate();
}

void CObj_Cal::ChangeRightSelectionColor(COLORREF color, bool flagRedraw)
{
	if(!m_RightSelect)return;
	m_RightSelect->ColorSel=color;
	if(flagRedraw)Invalidate();
}

void CObj_Cal::SetSelectionIndep(CByteArray * Selections, short UTVerticalDebut, short UTVerticalFin, bool fClose, COLORREF Color, long fOption, long nIndepSel)
{
	if(!Selections)				return;
	if(!Selections->GetSize())	return;
	if(m_vectIndepSel.GetSize() <= 0)	return;

	if(nIndepSel > m_vectIndepSel.GetSize() || nIndepSel < 0)	return;

	SSelection *IndepSelect = &m_vectIndepSel.GetAt(nIndepSel);

	IndepSelect->Selections.Copy(*Selections);
	IndepSelect->RSelect.SetRectEmpty();
	if(UTVerticalDebut >= 0)
	{
		//La plage commence par le bas
		IndepSelect->RSelect.top = m_dyAnnee + m_dyMois + m_dySem + m_dyJour - UTVerticalFin;
		IndepSelect->RSelect.bottom = m_dyAnnee + m_dyMois + m_dySem + m_dyJour - UTVerticalDebut;
	}
	else
	{
		if(UTVerticalDebut == -1)
		{
			IndepSelect->RSelect.top = 3;
			IndepSelect->RSelect.bottom = m_dyAnnee + m_dyMois + m_dySem + m_dyJour;
		}
	}
	IndepSelect->ColorSel = Color;
	IndepSelect->ShowRect = (short)fClose;
	IndepSelect->fOption	= fOption;
	m_fSelIndep = true;
	m_vectIndepSel.SetAt(nIndepSel, *IndepSelect);
}

void CObj_Cal::AddSelectionIndep(CByteArray * Selections, short UTVerticalDebut, short UTVerticalFin, bool fClose, COLORREF Color, long fOption)
{
	if(!Selections)				return;
	if(!Selections->GetSize())	return;

	SSelection IndepSelect;
	IndepSelect.Selections.Copy(*Selections);
	IndepSelect.RSelect.SetRectEmpty();
	if(UTVerticalDebut >= 0)
	{
		//La plage commence par le bas
		IndepSelect.RSelect.top = m_dyAnnee + m_dyMois + m_dySem + m_dyJour - UTVerticalFin;
		IndepSelect.RSelect.bottom = m_dyAnnee + m_dyMois + m_dySem + m_dyJour - UTVerticalDebut;
	}
	else
	{
		if(UTVerticalDebut == -1)
		{
			IndepSelect.RSelect.top = 3;
			IndepSelect.RSelect.bottom = m_dyAnnee + m_dyMois + m_dySem + m_dyJour;
		}
	}
	IndepSelect.ColorSel = Color;
	IndepSelect.ShowRect = (short)fClose;
	IndepSelect.fOption	= fOption;
	m_fSelIndep = true;
	m_vectIndepSel.Add(IndepSelect);
}


CByteArray * CObj_Cal::GetSelectionIndep(long nIndepSel)
{
	if(nIndepSel > m_vectIndepSel.GetSize() || nIndepSel < 0)	return 0;	
	return &(m_vectIndepSel.GetAt(nIndepSel)).Selections;
}

void CObj_Cal::RAZSelectionIndep(long nIndepSel)
{
	if(nIndepSel > m_vectIndepSel.GetSize() || nIndepSel < -1)	return;
	//On enlève tout
	if(nIndepSel == -1)
	{
		for(long i = 0 ; i < m_vectIndepSel.GetSize() ; i++)
		{
			SSelection IndepSelect = m_vectIndepSel.GetAt(i);
			IndepSelect.Selections.RemoveAll();
		}
		m_fSelIndep = false;
		m_vectIndepSel.RemoveAll();
	}
	else
	{
		SSelection IndepSelect = m_vectIndepSel.GetAt(nIndepSel);
		IndepSelect.Selections.RemoveAll();
		m_vectIndepSel.RemoveAt(nIndepSel);
	}
	if(m_vectIndepSel.GetSize() <= 0)	m_fSelIndep = false;
}

void CObj_Cal::ChangeModeSelection(long MouseKey, long ModeSelect, long SelectRules, bool fRAZSelect)
{
	if((m_Style & 0x0f)==STYLE_CAL_PIGE)	return;
	switch(SelectRules)
	{
	case CAL_INDEPENDANTE:
	case CAL_CROISEE:
	case CAL_LEFTINTORIGHT:
	case CAL_RIGHTINTOLEFT:
		m_SelectRules = SelectRules;
		break;
	default:	//on ne fait rien car mauvaise initialisation
		break;
	}
	
	if(MouseKey & MK_LBUTTON)
	{
		//si LeftSelection est OK..
		if(m_LeftSelect)
		{
			if(fRAZSelect)
			{
				for(short i=0 ; i<m_nbcases ; i++)
				{
					m_LeftSelect->Selections[i] = 0;
				}
				m_LeftSelect->SelectionsRAZ.Copy(m_LeftSelect->Selections);
				m_LeftSelect->SelectionsTmp.Copy(m_LeftSelect->Selections);
				m_LeftSelect->ColSelect	= 0;
				m_LeftSelect->debSelect	= 0;
				m_LeftSelect->debL		= 0;
				m_LeftSelect->debR		= 0;
				m_LeftSelect->RSelect.SetRectEmpty();
			}
		
			m_LeftSelect->ModeSel		= ModeSelect & 0x0FF;
			m_LeftSelect->fOption		= ModeSelect & 0xF00;
		}
	}
	if(MouseKey & MK_RBUTTON)
	{
		if(m_RightSelect)
		{
			if(fRAZSelect)
			{
				for(short i=0 ; i<m_nbcases ; i++)
				{
					m_RightSelect->Selections[i] = 0;
				}
				m_RightSelect->SelectionsRAZ.Copy(m_RightSelect->Selections);
				m_RightSelect->SelectionsTmp.Copy(m_RightSelect->Selections);
				m_RightSelect->ColSelect	= 0;
				m_RightSelect->debSelect	= 0;
				m_RightSelect->debL			= 0;
				m_RightSelect->debR			= 0;
				m_RightSelect->RSelect.SetRectEmpty();
			}
		
			m_RightSelect->ModeSel		= ModeSelect & 0x0FF;
			m_RightSelect->fOption		= ModeSelect & 0xF00;
		}
	}
}

void CObj_Cal::DetectModeSelect(long borne1, long borne2, long curs2)
{
	long etat;

	//Déterminer l'état actuel selon le mode de selection
	etat = GetEtatSelection(borne1, borne2, curs2);

	switch(m_SelectRules)
	{
	case CAL_PREGNANCE:
		//puis déterminer en fct de l'action du resultat
		//selection click gauche
		if(m_fLBCapture)
		{
			switch(etat)
			{
			case 0:	//pas de selection
				m_fModeSelect = 1;		//donc on est en train de selectionner
				break;
			case 1:	//selection gauche uniquement
				m_fModeSelect = 0;		//donc on est en train de deselectionner
				break;
			case 2:	//selection droite uniquement
				m_fModeSelect = 2;		//donc on est en train de selectionner
				break;
			case 3:	//selection gauche ET droite
				m_fModeSelect = 2;		//on est en train de selectionner la gauche et deselectionner la droite
				break;
			default:
				m_fModeSelect = 0;
				break;
			}
		}
		//selection click droit
		if(m_fRBCapture)
		{
			switch(etat)
			{
			case 0:	//pas de selection
				m_fModeSelect = 1;
				break;
			case 1:	//selection gauche uniquement
				m_fModeSelect = 1;
				break;
			case 2:	//selection droite uniquement
				m_fModeSelect = 2;
				break;
			case 3:	//selection gauche ET droite
				m_fModeSelect = 2;	//on est en train de selectionner la gauche et deselectionner la droite
				break;
			default:
				m_fModeSelect = 0;
				break;
			}
		}
		break;
	case CAL_OPPOSITION:
		//selection click gauche
		if(m_fLBCapture)
		{
			switch(etat)
			{
			case 0:	//pas de selection
				m_fModeSelect = 1;		//donc on est en train de selectionner
				break;
			case 1:	//selection gauche uniquement
				m_fModeSelect = 0;		//donc on est en train de deselectionner
				break;
			case 2:	//selection droite uniquement
				m_fModeSelect = 2;		//donc on est en train de selectionner
				break;
			case 3:	//selection gauche ET droite
				m_fModeSelect = 2;		//on est en train de selectionner la gauche et deselectionner la droite
				break;
			default:
				m_fModeSelect = 0;
				break;
			}
		}
		//selection click droit
		if(m_fRBCapture)
		{
			switch(etat)
			{
			case 0:	//pas de selection
				m_fModeSelect = 1;
				break;
			case 1:	//selection gauche uniquement
				m_fModeSelect = 1;
				break;
			case 2:	//selection droite uniquement
				m_fModeSelect = 0;
				break;
			case 3:	//selection gauche ET droite
				m_fModeSelect = 4;	//on est en train de selectionner la gauche et deselectionner la droite
				break;
			default:
				m_fModeSelect = 0;
				break;
			}
		}
		break;
	case CAL_INDEPENDANTE:
	case CAL_CROISEE:
	case CAL_LEFTINTORIGHT:
	case CAL_RIGHTINTOLEFT:
		//selection click gauche
		if(m_fLBCapture)
		{
			switch(etat)
			{
			case 0:	//pas de selection
				m_fModeSelect = 1;		//donc on est en train de selectionner
				break;
			case 1:	//selection gauche uniquement
				m_fModeSelect = 0;		//donc on est en train de deselectionner
				break;
			case 2:	//selection droite uniquement
				m_fModeSelect = 1;		//donc on est en train de selectionner
				break;
			case 3:	//selection gauche ET droite
				m_fModeSelect = 0;		//on est en train de selectionner la gauche et deselectionner la droite
				break;
			default:
				m_fModeSelect = 0;
				break;
			}
		}
		//selection click droit
		if(m_fRBCapture)
		{
			switch(etat)
			{
			case 0:	//pas de selection
				m_fModeSelect = 1;
				break;
			case 1:	//selection gauche uniquement
				m_fModeSelect = 1;
				break;
			case 2:	//selection droite uniquement
				m_fModeSelect = 0;
				break;
			case 3:	//selection gauche ET droite
				m_fModeSelect = 0;	//on est en train de selectionner la gauche et deselectionner la droite
				break;
			default:
				m_fModeSelect = 0;
				break;
			}
		}
		break;
	default:
		break;
	}
}

void CObj_Cal::SetCellsSelection(long Cell)
{
	switch(m_fModeSelect)
	{
	case 0:		//deselection
		m_CurrSelect->Selections[Cell] = 0;
		break;
	case 1:		//selection
		m_CurrSelect->Selections[Cell] = 1;
		break;
	case 2:		//selection gauche et deselection droite
		m_LeftSelect->Selections[Cell]		= 1;
		m_RightSelect->Selections[Cell]		= 0;
		break;
	case 3:		//selection droite et deselection gauche
		m_LeftSelect->Selections[Cell]		= 0;
		m_RightSelect->Selections[Cell]		= 1;
		break;
	case 4:	//deselection droite et gauche
		m_LeftSelect->Selections[Cell]		= 0;
		m_RightSelect->Selections[Cell]		= 0;
		break;
	default:
		break;
	}
}

long CObj_Cal::GetEtatSelection(long borne1, long borne2, long curs2)
{
	long i;
	long etat = 0;
	if(m_LeftSelect)
	{
		switch(m_fClickLevel)
		{
		case 1:	//click jour
			if(m_LeftSelect->Selections[m_CurrSelect->ColSelect])
				etat = 1;
			break;
		case 2:	//click semaine
			if(m_dxSem)	//le niveau semaine représente la base calendaire
			{
				if(m_LeftSelect->Selections[borne1])
				{
					etat = 1;
				}
			}
			else
			{
				for(long i = borne1 + 1 ; i < borne2 ; i++)
				{
					if((i < curs2) && IsSelectRules(m_LeftSelect->TypeSelect, i))
					{
						if(m_LeftSelect->Selections[i])
						{
							etat = 1;
							break;
						}
					}
				}
			}
			break;
		case 3:	//click mois
			for(i = borne1 ; i < borne2 ; i++)
			{
				if((i < curs2) && IsSelectRules(m_LeftSelect->TypeSelect, i))
				{
					if(m_LeftSelect->Selections[i])
					{
						etat = 1;
						break;
					}
				}
			}
			break;
		case 4:	//click annee
			for(i = borne1 + 1 ; i < borne2 ; i++)
			{
				if((i < curs2) && IsSelectRules(m_LeftSelect->TypeSelect, i))
				{
					if(m_LeftSelect->Selections[i])
					{
						etat = 1;
						break;
					}
				}
			}
			break;
		}
	}
	if(m_RightSelect)
	{
		switch(m_fClickLevel)
		{
		case 1:	//click jour
			if(m_RightSelect->Selections[m_CurrSelect->ColSelect])
				etat |= 2;
			break;	
		case 2:	//click semaine
			if(m_dxSem)	//le niveau semaine représente la base calendaire
			{
				if(m_RightSelect->Selections[borne1])
				{
					etat |= 2;
				}
			}
			else
			{
				for(long i = borne1 + 1 ; i < borne2 ; i++)
				{
					if((i < curs2) && IsSelectRules(m_RightSelect->TypeSelect, i))
					{
						if(m_RightSelect->Selections[i])
						{
							etat |= 2;
							break;
						}
					}
				}
			}
			break;
		case 3:	//click mois
			for(i = borne1 ; i < borne2 ; i++)
			{
				if((i < curs2) && IsSelectRules(m_RightSelect->TypeSelect, i))
				{
					if(m_RightSelect->Selections[i])
					{
						etat |= 2;
						break;
					}
				}
			}
			break;
		case 4:	//click annee
			for(i = borne1 + 1 ; i < borne2 ; i++)
			{
				if((i < curs2) && IsSelectRules(m_RightSelect->TypeSelect, i))
				{
					if(m_RightSelect->Selections[i])
					{
						etat |= 2;
						break;
					}
				}
			}
			break;
		}
	}
	return etat;
}

void CObj_Cal::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	//on remet la bonne initialisations du nombre de cases
	if(m_Style&OPTION_CAL_PRINTER)
	{
		m_RClient=m_PrntRect;
	}
	else
	{
		GetClientRect(&m_RClient);
	}

	m_nbcasesvisu=m_RClient.Width()/m_dx;
	if(m_nbcasesvisu>m_nbcases)
	{
		m_nbcasesvisu=m_nbcases;
		m_RClient.right=m_RClient.left+m_dx*m_nbcasesvisu;
	}

	//on réinitialise le scroll 
	m_nbCol=m_nbcasesvisu;
	if(m_ptscroll)
	{
		m_ptscroll->SetScrollRange(0, (m_nbcases-m_nbcasesvisu),0);
		m_ptscroll->SetScrollPos(m_posactive,true);
	}
	InvalidateRect(NULL, 0);
}