// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCWave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCWave::JFCWave()
{
	// on initialise le nombre d'éléments
	m_Count = 0;
	Hsize = 8;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nombre d'éléments verticaux
/////////////////////////////////////////////////////////////////////////////

void JFCWave::SetCount(LONG nombre, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on initialise le nombre d'éléments
	m_Count = (nombre > 0) ? nombre : 0;

	// on initialise l'intervalle des éléments
	this->SetVertItemRange(0, (nombre > 0) ? (nombre - 1) : 0);

	// on redessine la fenêtre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

LONG JFCWave::GetCount()
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie le nombre d'éléments
	return (m_Count);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la période
/////////////////////////////////////////////////////////////////////////////

void JFCWave::SetRange(JDate debut, JDate fin, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on vérifie la validité de la période
	if ((debut.IsValid()) && (fin.IsValid()))
	{
		// on mémorise la période
		m_Debut = (debut <= fin) ? debut : fin;
		m_Fin   = (debut >= fin) ? debut : fin;

		// on initialise l'intervalle de la période
		this->SetHorzItemRange(0, m_Fin - m_Debut, TRUE);
	}
	else
	{
		// on réinitialise la période
		m_Debut.Reset();
		m_Fin  .Reset();

		// on initialise l'intervalle de la période
		this->SetHorzItemRange(0, 0, TRUE);
	}

	// on redessine la fenêtre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

void JFCWave::GetRange(JDate & debut, JDate & fin)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie la période
	debut = m_Debut;
	fin   = m_Fin; 
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCWave::~JFCWave()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCWave, JFCViewport)
	//{{AFX_MSG_MAP(JFCWave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCWave::OnGetItemState(LONG indice, JDate date)
{
	// on renvoie l'état de l'élément
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris
/////////////////////////////////////////////////////////////////////////////

void JFCWave::OnLButtonDown(LONG indice, JDate date)
{
	// on ne fait rien
}

void JFCWave::OnLButtonUp(LONG indice, JDate date)
{
	// on ne fait rien
}

void JFCWave::OnRButtonDown(LONG indice, JDate date)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour capter les déplacements de la souris
/////////////////////////////////////////////////////////////////////////////

void JFCWave::OnMouseMove(LONG indice, JDate date)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCWave::OnGetItemState(LONG i, LONG j)
{
	// on récupère l'état de l'élément
	LONG etat = this->OnGetItemState(j, m_Debut + i);

	// on renvoie l'état
	return (etat);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCWave::OnBeginDrawing(CDC & dc)
{
	// on teste si on a des éléments à dessiner
	if (m_Count == 0) return (false);

	// on teste si on a une période à dessiner
	if ((!m_Debut.IsValid()) || (!m_Fin.IsValid())) return (false);

	// on continue le dessin des éléments
	return (true);
}

void JFCWave::OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat)
{
	Graphics grf(dc);

	Color clr;

	clr.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::GREY_MOUSE));
	Pen PenGrisS(clr);
	SolidBrush BrushGrisS(clr);

	clr.SetFromCOLORREF(CATPColors::GetColorDark(CATPColors::COLORCREUSET));
	Pen PenClrDark(clr);
	SolidBrush BrushClrDark(clr);

	clr.SetFromCOLORREF(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
	Pen PenClrMedium(clr);
	SolidBrush BrushClrMedium(clr);

	clr.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	Pen PenClrSel(clr);
	SolidBrush BrushClrSel(clr);

	clr.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	Pen PenClrLight(clr);
	SolidBrush BrushClrLight(clr);

	clr.SetFromCOLORREF(RGB(235,235,235));
	//Pen PenGris(clr);
	SolidBrush BrushGris(clr);

	clr.SetFromCOLORREF(RGB(250,235,235));
	//Pen PenGris(clr);
	SolidBrush BrushRed(clr);
	HatchBrush BrushRed2(HatchStyleLargeCheckerBoard, clr, Color(0,255,255,255));

	clr.SetFromCOLORREF(RGB(155,155,155));
	HatchBrush BrushConfetti(HatchStyleSmallConfetti, clr, Color(0,255,255,255));
	HatchBrush BrushZigZag(HatchStyleZigZag, clr, Color(0,255,255,255));

	
	//On dessinne la ligne verticale grise ou clrSel
	JDate date = m_Debut + i;
	if ((date.GetDayOfWeek() != 6) && (date != m_Fin))
		grf.DrawLine(&PenGrisS, rect.right-1, rect.top, rect.right-1, rect.bottom-1);
	else 
		grf.DrawLine(&PenClrSel, rect.right-1, rect.top, rect.right-1, rect.bottom-1);


	//on remplie le fond
	LONG fond = etat&JFCWAVE_FOND;

	if(fond == JFCWAVE_FOND_RED2)
		grf.FillRectangle(&BrushRed2, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);

	if(fond == JFCWAVE_FOND_RED)
		grf.FillRectangle(&BrushRed, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);

	if(fond == JFCWAVE_FOND_GRIS)
		grf.FillRectangle(&BrushGris, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);

	if(fond == JFCWAVE_FOND_TRF1)
		grf.FillRectangle(&BrushConfetti, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);

	if(fond == JFCWAVE_FOND_TRF2)
		grf.FillRectangle(&BrushZigZag, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);

	//le fond sel ecrase le reste
	if(fond == JFCWAVE_FOND_SEL)
		grf.FillRectangle(&BrushClrLight, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);

	LONG type = etat&JFCWAVE_ELEM;

	if(type == JFCWAVE_ELEM_ELEM || type == JFCWAVE_ELEM_ELEM_BORD)
		DrawElem(grf, rect, etat);
	if(type == JFCWAVE_ELEM_START ||type == JFCWAVE_ELEM_START_BORD)
		DrawElemStart(grf, rect, etat);

	if(type == JFCWAVE_ELEM_STOP || type == JFCWAVE_ELEM_STOP_BORD)
		DrawElemStop(grf, rect, etat);
	if(type == JFCWAVE_ELEM_QTD || type == JFCWAVE_ELEM_QTD_BORD)
		DrawQtd(grf, rect, etat);
}

void JFCWave::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	// on calcule la position du cadre de l'élément
	Graphics grf(dc);
	Color clr;

	clr.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	Pen m_PenClrSel(clr);
	
	grf.DrawLine(&m_PenClrSel, rect.left, rect.bottom-1, rect.right-1, rect.bottom-1);
}

void JFCWave::OnEndDrawing(CDC & dc)
{
}

/////////////////////////////////////////////////////////////////////////////
// JFCWave message handlers

void JFCWave::PreSubclassWindow() 
{
	// on initialise le nombre d'éléments
	m_Count = 0;

	// on initialise la période
	m_Debut.Reset();
	m_Fin  .Reset();

	// on appelle le gestionnaire de base
	this->JFCViewport::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize( 7, TRUE, FALSE);
	//this->SetVertItemSize(22, TRUE, FALSE);
	this->SetVertItemSize(18, TRUE, FALSE);
}

void JFCWave::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{
		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			LONG i,j;

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnLButtonDown(j, m_Debut + i);
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonDown(nFlags, point);
}

void JFCWave::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{
		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			LONG i,j;

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnLButtonUp(j, m_Debut + i);
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonUp(nFlags, point);
}

void JFCWave::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{
		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			LONG i,j;

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnRButtonDown(j, m_Debut + i);
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnRButtonDown(nFlags, point);
}

void JFCWave::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on teste si on a des éléments
	if (m_Count > 0)
	{
		// on teste si on a une période
		if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
		{
			LONG i,j;

			// on récupère l'élément sous la souris
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on effectue le traitement
				this->OnMouseMove(j, m_Debut + i);
			}
			else
				this->OnMouseMove(-1, m_Debut);
		}
	}
	
	// on appelle le gestionnaire de base
	this->JFCViewport::OnMouseMove(nFlags, point);
}


void JFCWave::DrawStatus(Graphics& grf, RECT rect, LONG etat)
{
	LONG fill = etat&JFCWAVE_STATE;

	switch(fill)
	{
	case JFCWAVE_STATE_SUPERPOSE:
		DrawSuperpose(grf, rect, etat);
		break;
	case JFCWAVE_STATE_CHROMIE1:
		DrawChromieMono(grf, rect);
		break;
	case JFCWAVE_STATE_CHROMIE2:
		DrawChromie2(grf, rect);
		break;
	case JFCWAVE_STATE_CHROMIE3:
		DrawChromie3(grf, rect);
		break;
	case JFCWAVE_STATE_CHROMIE4:
		DrawChromie4(grf, rect);
		break;
	case JFCWAVE_STATE_STATUS1:
		DrawStatut1(grf, rect);
		break;
	case JFCWAVE_STATE_STATUS2:
		DrawStatut2(grf, rect);
		break;
	}

}

void JFCWave::DrawChromie4(Graphics& grf, RECT rect)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;

	//Point pts[] = {
	//	Point(rect.left, rect.top+offset), 
	//	Point(rect.right, rect.top+offset), 
	//	Point(rect.right, rect.top+offset+Hsize+1),
	//	Point(rect.left, rect.top+offset+Hsize+1)
	//				};

	//Color cols[] = {
	//	Color(255, 0, 255, 255),  // c
	//	Color(255, 255, 0, 255),  // m
	//	//Color(255, 0, 0, 0),  // black
	//	Color(255, 255, 255, 255),  // white
	//	Color(255, 255, 255, 0)   // j
	//};

	//INT ncount = 4;
	//PathGradientBrush pthGrBrush(pts, ncount);
	//pthGrBrush.SetSurroundColors(cols, &ncount);
	//pthGrBrush.SetGammaCorrection(0);
	//pthGrBrush.SetCenterColor(Color(255, 100, 100, 100));

	//grf.FillRectangle(&pthGrBrush, rect.left, rect.top+offset, rect.right-rect.left, Hsize+1);

	SolidBrush bruch1(Color(255, 255, 255, 0));         // 1
	SolidBrush bruch2(Color(255, 0, 255, 255));         // 2
	SolidBrush bruch3(Color(255, 255, 0, 255));         // 3
	SolidBrush bruch4(Color(255, 0, 0, 0));         // 4

	grf.FillRectangle(&bruch1, rect.left, rect.top+offset, (rect.right-rect.left)/2, Hsize/2);
	grf.FillRectangle(&bruch2, rect.left + (rect.right-rect.left)/2, rect.top+offset, rect.right-rect.left, Hsize/2);
	grf.FillRectangle(&bruch3, rect.left, rect.top+offset+Hsize/2+1, (rect.right-rect.left)/2, Hsize/2);
	grf.FillRectangle(&bruch4, rect.left + (rect.right-rect.left)/2, rect.top+offset+Hsize/2+1, rect.right-rect.left, Hsize/2);


	//grf.FillRectangle(&bruch1, rect.left, rect.top+offset, rect.right-rect.left, Hsize/3+1);
	//grf.FillRectangle(&bruch2, rect.left, rect.top+offset+Hsize/3+1, rect.right-rect.left, Hsize/3+1);
	//grf.FillRectangle(&bruch3, rect.left, rect.top+offset+2*Hsize/3+1, rect.right-rect.left, Hsize/3+1);
	//grf.FillRectangle(&bruch4, rect.left, rect.top+offset+2*Hsize/3+1, rect.right-rect.left, Hsize/3+1);

	Color clrPen;
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
    Pen pen(clrPen);
	grf.DrawRectangle(&pen, rect.left-1, rect.top+offset, rect.right-rect.left, Hsize);
}

void JFCWave::DrawChromie3(Graphics& grf, RECT rect)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;

	//Color colors[] = {
 //     Color(255, 255, 0, 0),   // red
 //     Color(255, 0, 255, 0),   // green
 //     Color(255, 0, 0, 255)};  // blue

 //  REAL positions[] = {
 //     0.0f,   // red at the left edge
 //     0.5f,   // blue at 50 percent of the distance from left edge 
 //             // to right edge
 //     1.0f};  // green at the right edge

	//LinearGradientBrush linGrBrush(
 //     Point(rect.left, rect.top+offset), 
 //     Point(rect.left, rect.top+offset+Hsize+1),
 //     Color(255, 0, 0, 0),         // black
 //     Color(255, 255, 255, 255));  // white

	//linGrBrush.SetInterpolationColors(colors, positions, 3);

	//grf.FillRectangle(&linGrBrush, rect.left, rect.top+offset, rect.right-rect.left, Hsize+1);

	SolidBrush bruch1(Color(255, 255, 0, 0));         // 1
	SolidBrush bruch2(Color(255, 0, 255, 0));         // 2
	SolidBrush bruch3(Color(255, 0, 0, 255));         // 3

	grf.FillRectangle(&bruch1, rect.left, rect.top+offset, rect.right-rect.left, Hsize/3+1);
	grf.FillRectangle(&bruch2, rect.left, rect.top+offset+Hsize/3+1, rect.right-rect.left, Hsize/3+1);
	grf.FillRectangle(&bruch3, rect.left, rect.top+offset+2*Hsize/3+1, rect.right-rect.left, Hsize/3+1);


	Color clrPen;
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
    Pen pen(clrPen);
	grf.DrawRectangle(&pen, rect.left-1, rect.top+offset, rect.right-rect.left, Hsize);
}

void JFCWave::DrawChromie2(Graphics& grf, RECT rect)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;

	//LinearGradientBrush linGrBrush(
 //     Point(rect.left-1, rect.top+offset), 
 //     Point(rect.right, rect.top+offset+Hsize),
 //     Color(255, 255, 0, 0),         //1
 //     Color(255, 0, 255, 255));  // 2

	//grf.FillRectangle(&linGrBrush, rect.left, rect.top+offset, rect.right-rect.left, Hsize+1);

	SolidBrush bruch1(Color(255, 255, 0, 0));         // 1
	SolidBrush bruch2(Color(255, 0, 255, 255));         // 2

	grf.FillRectangle(&bruch1, rect.left, rect.top+offset, (rect.right-rect.left)/2, Hsize+1);
	grf.FillRectangle(&bruch2, rect.left + (rect.right-rect.left)/2, rect.top+offset, rect.right-rect.left, Hsize+1);

	//GraphicsPath pathu(
	//grf.FillPath(

	Color clrPen;
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
    Pen pen(clrPen);
	grf.DrawRectangle(&pen, rect.left-1, rect.top+offset, rect.right-rect.left, Hsize);
}

void JFCWave::DrawChromieMono(Graphics& grf, RECT rect)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;

	SolidBrush bruchBlack(Color(255, 0, 0, 0));         // black
	SolidBrush bruchWhite(Color(255, 255, 255, 255));         // white

	
	//LinearGradientBrush linGrBrush(
 //     Point(rect.left-1, rect.top+offset), 
 //     Point(rect.right, rect.top+offset),
 //     Color(255, 0, 0, 0),         // black
 //     Color(255, 255, 255, 255));  // white
	//grf.FillRectangle(&linGrBrush, rect.left, rect.top+offset, rect.right-rect.left, Hsize+1);


	
	grf.FillRectangle(&bruchBlack, rect.left, rect.top+offset, (rect.right-rect.left)/2, Hsize+1);
	grf.FillRectangle(&bruchWhite, rect.left + (rect.right-rect.left)/2, rect.top+offset, rect.right-rect.left, Hsize+1);

	Color clrPen;
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
    Pen pen(clrPen);
	grf.DrawRectangle(&pen, rect.left-1, rect.top+offset, rect.right-rect.left, Hsize);
}

void JFCWave::DrawStatut1(Graphics& grf, RECT rect)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_CR_LOCK);
	Bitmap bmp(hIcon);
	
	grf.DrawImage(&bmp, rect.left, rect.top+offset, rect.right-rect.left-1, Hsize+1); 
}

void JFCWave::DrawStatut2(Graphics& grf, RECT rect)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_CR_STATE2);
	Bitmap bmp(hIcon);
	
	grf.DrawImage(&bmp, rect.left, rect.top+offset, rect.right-rect.left-1, Hsize+1); 
}

void JFCWave::DrawSuperpose(Graphics& grf, RECT rect, LONG etat)
{
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;
	LONG offset2 = Hsize/3;

	Color clrBrush;
	clrBrush.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	SolidBrush brush(clrBrush);

	LONG fill = etat&JFCWAVE_ELEM;
	if(fill == JFCWAVE_ELEM_QTD)
		grf.FillRectangle(&brush, rect.left+1, rect.top+offset+offset2, rect.right-rect.left-3, Hsize-offset2-1);
	else
		grf.FillRectangle(&brush, rect.left, rect.top+offset+offset2, rect.right-rect.left, Hsize-offset2-1);
}

void JFCWave::DrawQtd(Graphics& grf, RECT rect, LONG etat)
{
	Color clrBrush;
	Color clrPen;

	clrBrush.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	LONG fill = etat&JFCWAVE_ELEM;
	if(fill == JFCWAVE_ELEM_QTD_BORD)
	{
		clrBrush.SetFromCOLORREF(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
		clrPen.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::GREY_MOUSE));
	}

    SolidBrush brush(clrBrush);
    Pen pen(clrPen);
	
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;
	
	grf.FillRectangle(&brush, rect.left, rect.top+offset, rect.right-rect.left-1, Hsize);
	grf.DrawRectangle(&pen, rect.left, rect.top+offset, rect.right-rect.left-2, Hsize);

	//fill = etat&JFCWAVE_STATE;
	//if(fill == JFCWAVE_STATE_SUPERPOSE)
	//{
	//	clrBrush.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	//	LONG offset2 = Hsize/3;
	//	brush.SetColor(clrBrush);
	//	grf.FillRectangle(&brush, rect.left+1, rect.top+offset+offset2, rect.right-rect.left-3, Hsize-offset2-1);
	//}
	
	DrawStatus(grf, rect, etat);
}

void JFCWave::DrawElem(Graphics& grf, RECT rect, LONG etat)
{
	Color clrBrush;
	Color clrPen;

	clrBrush.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	LONG fill = etat&JFCWAVE_ELEM;
	if(fill == JFCWAVE_ELEM_ELEM_BORD)
	{
		clrBrush.SetFromCOLORREF(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
		clrPen.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::GREY_MOUSE));
	}


    SolidBrush brush(clrBrush);
    Pen pen(clrPen);
	
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;
	
	grf.FillRectangle(&brush, rect.left, rect.top+offset+1, rect.right-rect.left, Hsize-1);
	grf.DrawLine(&pen, rect.left, rect.top+offset, rect.right-1, rect.top+offset);
	grf.DrawLine(&pen, rect.left, rect.top+offset+Hsize, rect.right-1, rect.top+offset+Hsize);

	DrawStatus(grf, rect, etat);

}

void JFCWave::DrawElemStart(Graphics& grf, RECT rect, LONG etat)
{
	Color clrBrush;
	Color clrPen;

	clrBrush.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	LONG fill = etat&JFCWAVE_ELEM;
	if(fill == JFCWAVE_ELEM_START_BORD)
	{
		clrBrush.SetFromCOLORREF(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
		clrPen.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::GREY_MOUSE));
	}

    SolidBrush brush(clrBrush);
    Pen pen(clrPen);
	
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;
	LONG center = (rect.bottom-rect.top-1)/2 ;

	GraphicsPath path;
	path.AddLine(rect.left, rect.top+center, rect.right, rect.top+offset);
	path.AddLine(rect.right, rect.top+offset, rect.right, rect.top+offset+Hsize);
	path.AddLine(rect.right, rect.top+offset+Hsize, rect.left, rect.top+center);
	grf.FillPath(&brush,&path);
	
	grf.SetSmoothingMode(SmoothingModeAntiAlias);

	grf.DrawLine(&pen, rect.left, rect.top+center, rect.right-1, rect.top+offset);
	grf.DrawLine(&pen, rect.right-1, rect.top+offset+Hsize, rect.left, rect.top+center);
}

void JFCWave::DrawElemStop(Graphics& grf, RECT rect, LONG etat)
{
	Color clrBrush;
	Color clrPen;

	clrBrush.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	clrPen.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	LONG fill = etat&JFCWAVE_ELEM;
	if(fill == JFCWAVE_ELEM_STOP_BORD)
	{
		clrBrush.SetFromCOLORREF(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
		clrPen.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::GREY_MOUSE));
	}

    SolidBrush brush(clrBrush);
    Pen pen(clrPen);
	
	LONG offset = (rect.bottom-rect.top-1 - Hsize)/2;
	LONG center = (rect.bottom-rect.top-1)/2;

	GraphicsPath path;
	path.AddLine(rect.right-1, rect.top+center, rect.left, rect.top+offset);
	path.AddLine(rect.left, rect.top+offset, rect.left, rect.top+offset+Hsize);
	path.AddLine(rect.left, rect.top+offset+Hsize, rect.right-1, rect.top+center);
	grf.FillPath(&brush,&path);
	
	grf.SetSmoothingMode(SmoothingModeAntiAlias);

	grf.DrawLine(&pen, rect.right-1, rect.top+center, rect.left, rect.top+offset);
	grf.DrawLine(&pen, rect.left, rect.top+offset+Hsize, rect.right-1, rect.top+center);
}


