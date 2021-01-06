#include "StdAfx.h"
#include "jfcgridmulticell.h"

JFCGridMultiCell::JFCGridMultiCell(void)
{
	// Init horizontal cellules count
	m_HorzCellCount = 0;

	// Par défaut no colonne format = 1
	m_NoColFormat = 1;

	// Pas de mode jump ligne ou colonne en mode edit
	SpecialEditWithJump = false;

	// Par défaut on est en mode cellule
	m_ModeBlocEdit = false;

	// Init elmt sélection bloc
	InitZoneSelect();

	// on crée la bitmap de remplissage
	// Create a hatched bit pattern.
	WORD HatchBits[8] = { 0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88 };
	CBitmap Bitmap;
	Bitmap.CreateBitmap(8, 8, 1, 1, HatchBits);

	// on crée le pinceau pour le remplissage
	// m_BrushDragDrop.CreatePatternBrush(&Bitmap);
	// m_BrushDragDrop.CreateSolidBrush(RGB(0,255,0));
	m_BrushDragDrop.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
}


JFCGridMultiCell::~JFCGridMultiCell(void)
{
}

// Ecriture Mode édition
void JFCGridMultiCell::SetModeEdit(bool ModeBloc)
{
	m_ModeBlocEdit = ModeBloc;
}

// Lecture Mode Edition
bool JFCGridMultiCell::GetModeEdit()
{
	return m_ModeBlocEdit;
}


void JFCGridMultiCell::SetHorzCellCount(LONG nombre, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on initialise le nombre d'éléments
	m_HorzCellCount = (nombre > 0) ? nombre : 0;

	// on initialise l'intervalle des éléments
	//this->SetHorzItemRange(0, (nombre > 0) ? (nombre - 1) : 0);

	// on redessine la fenêtre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

LONG JFCGridMultiCell::GetHorzCellCount()
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie le nombre d'éléments
	return (m_HorzCellCount);
}

BEGIN_MESSAGE_MAP(JFCGridMultiCell, JFCGrid)
	//{{AFX_MSG_MAP(JFCGridMultiCell)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP

	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_DOWN,OnMsgNewEditingDown)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_UP,OnMsgNewEditingUp)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_LEFT,OnMsgNewEditingLeft)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_RIGHT,OnMsgNewEditingRight)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING,OnMsgNewEditing)

END_MESSAGE_MAP()

void JFCGridMultiCell::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Récup no cellule format
	m_NoColFormatDown = GetNoFormat(point);	

	if ((GetHorzCount() > 0) && (GetVertCount() > 0))
	{
		// on récupère l'élément sous le curseur
		LONG i,j;
		GetIdxAtPos(point.x, point.y, i, j);

		m_BeginCol = i;
		m_BeginRow = j;

		// DRAG DROG EFFECT -- Drag drop
		SetCapture();
		m_initialPoint = point;
		CRect rect(point,point);
		CDC *pDC = GetDC();
		
		pDC->DrawDragRect(&rect, CSize(1,1), NULL, CSize(1,1), NULL, NULL);
		//pDC->DrawDragRect(&rect, CSize(1,1), &m_lastRect, CSize(1,1), &m_BrushDragDrop, &m_BrushDragDrop);

		m_lastRect = rect;
		ReleaseDC(pDC);
		////////////////////////////////
	}
}


void JFCGridMultiCell::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Récup no cellule format
	m_NoColFormat = GetNoFormat(point);

	// si non utilsation de la touche CTRL >> incrément via click souris
	// if ((nFlags & MK_CONTROL) == 0)
	if ((nFlags & MK_CONTROL) != 0 || (nFlags & MK_SHIFT) != 0)
	{
		// Ferme boité édition si en cours
		this->CloseEditBox();

		if ((GetHorzCount() > 0) && (GetVertCount() > 0))
		{
			// on récupère l'élément sous le curseur
			LONG i,j;
			GetIdxAtPos(point.x, point.y, i, j);

			LONG x1,x2,y1,y2;
			GetPosAtIdx(i, j, x1, y1, x2, y2);

			double ln = x2 - x1;
			double sz = point.x-x1;

			// on effectue le traitement
			this->OnLButtonDown(i, m_NoColFormat, j);
		}
	}

	else
	{
		this->SetFocus();

		// on teste si on a des éléments
		if ((m_HorzCount > 0) && (m_VertCount > 0))
		{
			LONG i,j;

			// on récupère l'élément sous le curseur
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{

				// Si multi-cellule, on colore la zone qui prendra la valeur edit
				/*
				if (i != m_BeginCol || j != m_BeginRow || m_NoColFormat != m_NoColFormatDown)
				{
					bool OKColor = true;
					
					CDC * pDc = this->GetDC();
					
					LONG xx1,xx2,yy1,yy2;
					GetPosAtIdx(m_BeginCol, m_BeginRow, xx1, yy1, xx2, yy2);

					CRect rect1;
					rect1.top		= yy1;
					rect1.left	= xx1;

					// Attention au format en cours
					ZoneEditFormat(rect1, xx1, xx2, m_NoColFormatDown);

					GetPosAtIdx(i, j, xx1, yy1, xx2, yy2);

					CRect rect2;
					rect2.bottom	= yy2;
					rect2.top		= yy1;
					rect2.right		= xx2;
					rect2.left		= xx1;

					// Attention au format en cours
					ZoneEditFormat(rect2, xx1, xx2, m_NoColFormat);

					CRect rectint;
					rectint.top		= rect1.top;
					rectint.left	= rect1.left;
					rectint.bottom  = rect2.bottom;
					rectint.right   = rect2.right;

					// this->OnDrawCell(*pDc, rectint, m_BeginCol, m_BeginRow, m_BrushA, m_BrushB, m_BrushC, m_BrushD);

					// on récupère la zone cliente de la fenêtre
					CRect Client;
					GetClientRect(&Client);

					Client.left		= rectint.left;
					Client.right	= rectint.right;
					Client.top		= rectint.top;
					Client.bottom	= rectint.bottom;

					this->OnDrawBorder(*pDc, Client, rectint.left, rectint.top, rectint.right - rectint.left, rectint.bottom - rectint.top);

				}
				*/

				LONG x1, y1, x2, y2;

				// on teste si on doit redessiner la cellule de l'ancien curseur
				if (m_HorzCursor >= 0 && m_HorzCursor < m_HorzCount && m_VertCursor >= 0 && m_VertCursor < m_VertCount)
				{
					// on récupère la position de la cellule
					if (this->GetPosAtIdx(m_HorzCursor, m_VertCursor, x1, y1, x2, y2))
					{
						RECT Rect;

						// on initialise le rectangle
						Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

						// on invalide le curseur
						m_HorzCursor = -1;
						m_VertCursor = -1;

						// on redessine la cellule
						//this->InvalidateRect(&Rect, FALSE);
						//this->UpdateWindow();
					}
				}

				// on modifie le curseur
				m_HorzCursor = i;
				m_VertCursor = j;

				// on teste l'état de la touche CTRL
				// if ((nFlags & MK_CONTROL) != 0)
				if ((nFlags & MK_CONTROL) == 0)
				{
					// on récupère la position de la cellule
					if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
					{
						RECT Rect;

						// on initialise le rectangle (attention multi cellule)
						Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

						// on redessine la cellule
						//this->InvalidateRect(&Rect, FALSE);
						//this->UpdateWindow();

						/* AVANT
						if (m_HorzCellCount > 0)
						{
							// Calcul largeur ss-cellule
							LONG sz = (Rect.right - Rect.left) / m_HorzCellCount;

							Rect.left  = Rect.left  + (m_NoColFormat - 1) * (sz + 1) + 1;
							Rect.right = Rect.left + sz;

							// on démarre l'édition de l'élément
							SetEditBox(Rect,i,j);
						}
						*/

						if (m_BeginCol == -1 && m_BeginRow == -1)
						{
							// Ferme boité édition si en cours
							this->CloseEditBox();	
						}
						else if (i != m_BeginCol || j != m_BeginRow || m_NoColFormat != m_NoColFormatDown)
						{
							bool OKColor = true;

							CDC * pDc = this->GetDC();

							LONG xx1,xx2,yy1,yy2;
							GetPosAtIdx(m_BeginCol, m_BeginRow, xx1, yy1, xx2, yy2);

							CRect rect1;
							rect1.top	= yy1;
							rect1.left	= xx1;

							// Attention au format en cours
							ZoneEditFormat(rect1, xx1, xx2, m_NoColFormatDown);

							GetPosAtIdx(i, j, xx1, yy1, xx2, yy2);

							CRect rect2;
							rect2.bottom	= yy2;
							rect2.top		= yy1;
							rect2.right		= xx2;
							rect2.left		= xx1;

							// Attention au format en cours
							ZoneEditFormat(rect2, xx1, xx2, m_NoColFormat);

							CRect rectint;
							rectint.top		= rect1.top;
							rectint.left	= rect1.left;
							rectint.bottom  = rect2.bottom;
							rectint.right   = rect2.right;

							/*
							CString Txt;
							Txt.Format("Col %d Row %d",m_BeginCol, m_BeginRow);
							AfxMessageBox(Txt);
							*/

							// on démarre l'édition de l'élément
							SetEditBox(rectint,i,j);
						}
						else
						{
							// Calcul largeur ss-cellule
							LONG sz = (Rect.right - Rect.left) / m_HorzCellCount;

							Rect.left  = Rect.left  + (m_NoColFormat - 1) * (sz + 1) + 1;
							Rect.right = Rect.left + sz;

							// on démarre l'édition de l'élément
							SetEditBox(Rect,i,j);
						}

					}
				}
				else
				{
					// on effectue le traitement
					this->OnLButtonDown(i, j);
				}
			}
		}

		// on appelle le gestionnaire de base
		this->JFCViewport::OnLButtonDown(nFlags, point);
	}

	if (m_BeginCol == -1 || m_BeginRow == -1)
	{
		//m_RowCellEdit = -1;
		//m_ColCellEdit = -1;
	}

	// DRAG DROP EFFECT
	if (GetCapture() == this)
	{
		CDC *pDC = GetDC();
		CRect rect(0,0,0,0);

		pDC->DrawDragRect(rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
		// pDC->DrawDragRect(&rect, CSize(1,1), &m_lastRect, CSize(1,1), &m_BrushDragDrop, &m_BrushDragDrop);

		ReleaseDC(pDC);
		ReleaseCapture();
	}
	/////////////////////////////////////////

}

// move about with keyboard
/*
void JFCGridMultiCell::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	LONG i,j;
	GetIdxAtPos(point.x, point.y, i, j);

	if (!IsValid(m_idCurrentCell))
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
}
*/


///////////////////////////////////////////////////////////////////////////////////////
// Récupération du n° de la colonne format
LONG JFCGridMultiCell::GetNoFormat(CPoint point)
{
	if ((GetHorzCount() > 0) && (GetVertCount() > 0))
	{
		// on récupère l'élément sous le curseur
		LONG i,j;
		GetIdxAtPos(point.x, point.y, i, j);
		LONG x1,x2,y1,y2;
		GetPosAtIdx(i, j, x1, y1, x2, y2);

		double ln = x2 - x1;
		double sz = point.x-x1;

		int ind = (int)((sz*(double)m_HorzCellCount)/ln)+1;
		if(ind>m_HorzCellCount)
			ind = m_HorzCellCount;
		if(ind==0)
			ind = 1;

		return ind;
	}

	// Par défaut ça sera la colonne 1  (pour 1 seul format)
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////
// Calcul Zone réelle d'édition via la colonne format
void JFCGridMultiCell::ZoneEditFormat(RECT &rect_edit, LONG x1, LONG x2, LONG NoColFormat)
{
	LONG Depx = 0;
	if (m_HorzCellCount > 1)
	{
		Depx = ((NoColFormat-1) * (x2-x1) / m_HorzCellCount) + 2;
		rect_edit.left  += Depx;
		rect_edit.right = rect_edit.left + (x2-x1) / m_HorzCellCount;
	}
}

void JFCGridMultiCell::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if ((GetHorzCount() > 0) && (GetVertCount() > 0))
	{
		// on récupère l'élément sous le curseur
		LONG i,j;
		GetIdxAtPos(point.x, point.y, i, j);
		LONG x1,x2,y1,y2;
		GetPosAtIdx(i, j, x1, y1, x2, y2);

		double ln = x2 - x1;
		double sz = point.x-x1;

		int ind = (int)((sz*(double)m_HorzCellCount)/ln)+1;
		if(ind>m_HorzCellCount)
			ind = m_HorzCellCount;
		if(ind==0)
			ind = 1;
		// on effectue le traitement
		this->OnRButtonDown(i, ind, j);
	}
	
	// on appelle le gestionnaire de base
	this->JFCGrid::OnRButtonDown(nFlags, point);
}

void JFCGridMultiCell::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ((GetHorzCount() > 0) && (GetVertCount() > 0) )
	{
		// on récupère l'élément sous le curseur
		LONG i,j;
		if( GetIdxAtPos(point.x, point.y, i, j))
		{
			LONG x1,x2,y1,y2;
			GetPosAtIdx(i, j, x1, y1, x2, y2);

			double ln = x2 - x1;
			double sz = point.x-x1;

			int ind = (int)((sz*(double)m_HorzCellCount)/ln)+1;
			if(ind>m_HorzCellCount)
				ind = m_HorzCellCount;
			if(ind==0)
				ind = 1;

			// DRAG DROP EFFECT
			if (GetCapture() == this)
			{
				CRect rect(m_initialPoint, point);
				rect.NormalizeRect();

				CDC *pDC = GetDC();
				pDC->DrawDragRect(&rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
				// pDC->DrawDragRect(&rect, CSize(1,1), &m_lastRect, CSize(1,1), &m_BrushDragDrop, &m_BrushDragDrop);

				m_lastRect = rect;
				ReleaseDC(pDC);
			}
			////////////////////////////////////////////

			// on effectue le traitement
			this->OnMouseMove(i, ind, j);
		}
		else
			this->OnMouseMove(-1, -1, -1);

	}
	
	// on appelle le gestionnaire de base
	this->JFCGrid::OnMouseMove(nFlags, point);
}

void JFCGridMultiCell::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		bool Ok = true;
	}
}

void JFCGridMultiCell::OnLButtonDown(LONG i, LONG ii, LONG j)
{
	m_NoColFormat = ii;
}

void JFCGridMultiCell::OnLButtonUp(LONG i, LONG ii, LONG j)
{
	m_NoColFormat = ii;
}

void JFCGridMultiCell::OnRButtonDown(LONG i, LONG ii, LONG j)
{
}
void JFCGridMultiCell::OnMouseMove(LONG i, LONG ii, LONG j)
{
}

void JFCGridMultiCell::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	if(m_HorzCellCount!=0)
	{
		CRect rectTmp = rect;
		rectTmp.top    -= 2;
		rectTmp.bottom += 1;
		rectTmp.left   -= 3;
		rectTmp.right  += 4;
		CRect rectint;
		CRect rectintPrec;
		LONG sz = (rectTmp.right - rectTmp.left)/m_HorzCellCount;
		rectintPrec = rectTmp;
		rectintPrec.right = rectintPrec.left + sz;


		//////////////////////////////////////////////////
		// TEST COULEUR DEGRADE
		this->OnDrawCell(dc, rectintPrec, i, 1, j, BrushA, m_BrushDark, m_BrushMedium, m_BrushLight);
		// this->OnDrawCell(dc, rectintPrec, i, 1, j, BrushA, BrushB, BrushC, BrushD);
		///////////////////////////////////////////////////

		dc.SelectObject(&m_PenA);
		dc.MoveTo(rectintPrec.left-1, rectTmp.top); dc.LineTo(rectintPrec.left-1, rectTmp.bottom);
		for (int id = 1; id< m_HorzCellCount; id++)
		{
			rectint = rectTmp;
			rectint.left = rectintPrec.right;
			rectint.right = rectint.left + sz;
			
			// this->OnDrawCell(dc, rectint, i, id+1, j, BrushA, BrushB, BrushC, BrushD);
			this->OnDrawCell(dc, rectint, i, id+1, j, BrushA, m_BrushDark, m_BrushMedium, m_BrushLight);

			rectintPrec = rectint;
			dc.SelectObject(&m_PenG);
			dc.MoveTo(rectintPrec.left, rectTmp.top); dc.LineTo(rectintPrec.left, rectTmp.bottom);
		}
	}
}

void JFCGridMultiCell::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
}

// les fonctions pour éditer les éléments
bool JFCGridMultiCell::OnBeginEditing(CString &CurString, LONG i, LONG j)
{
	return false;
}

bool JFCGridMultiCell::OnEditItem(const TCHAR* pString, LONG i, LONG j)
{
	return false;
}

void JFCGridMultiCell::OnCancelEditing()
{
}

// DRAG DROP EFFECT
void JFCGridMultiCell::OnCancelMode()
{
	if (GetCapture() == this)
	{
		CDC *pDC = GetDC();
		CRect rect(0,0,0,0);

		pDC->DrawDragRect(rect, CSize(1,1), &m_lastRect, CSize(1,1), NULL, NULL);
		// pDC->DrawDragRect(&rect, CSize(1,1), &m_lastRect, CSize(1,1), &m_BrushDragDrop, &m_BrushDragDrop);
		ReleaseDC(pDC);

		ReleaseCapture();
	}

	CWnd::OnCancelMode();
}

void JFCGridMultiCell::OnEndEditing()
{
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule d'en dessous
LRESULT JFCGridMultiCell::OnMsgNewEditingDown(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la précédente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on récupère l'élément sous le curseur
	LONG i,j;

	// Row & Col de la cellule courante éditée
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG x1, y1, x2, y2;
	if (this->EnsureVisible(i, j+1) && this->GetPosAtIdx(i, j+1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j+1);

		m_HorzCursor = i;
		m_VertCursor = j+1;
	}
	// Si mode sans jump ligne colonne on arrête là
	else if (!SpecialEditWithJump && this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat == m_HorzCellCount && this->EnsureVisible(i, 0) && this->GetPosAtIdx(i+1, 0, x1, y1, x2, y2))
	{
		m_NoColFormat = 1;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i+1,0);

		m_HorzCursor = i+1;
		m_VertCursor = 0;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat < m_HorzCellCount && this->EnsureVisible(i, 0) && this->GetPosAtIdx(i, 0, x1, y1, x2, y2))
	{
		m_NoColFormat++;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,0);

		m_HorzCursor = i;
		m_VertCursor = 0;
	}
	else if (this->EnsureVisible(i+1, 0) && this->GetPosAtIdx(i+1, 0, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i+1,0);

		m_HorzCursor = i+1;
		m_VertCursor = 0;
	}

	// On se remet sur la cellule initiale
	else if (this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule du dessus
LRESULT JFCGridMultiCell::OnMsgNewEditingUp(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la précédente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on récupère l'élément sous le curseur
	LONG i,j;

	// Row & Col de la cellule courante éditée
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	LONG x1, y1, x2, y2;
	LONG LastRowVisible = jmax;

	// On tente le mode édition sur la cellule du dessus
	if (this->EnsureVisible(i, j-1) && this->GetPosAtIdx(i, j-1, x1, y1, x2, y2))
	{

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j-1);

		m_HorzCursor = i;
		m_VertCursor = j-1;
	}
	// Si mode sans jump ligne colonne on arrête là
	else if (!SpecialEditWithJump && this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat == 1 && this->EnsureVisible(i-1, jmax-1) && this->GetPosAtIdx(i-1, jmax-1, x1, y1, x2, y2))
	{
		m_NoColFormat = m_HorzCellCount;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i-1, jmax-1);

		m_HorzCursor = i-1;
		m_VertCursor = jmax-1;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat > 1 && this->EnsureVisible(i, jmax-1) && this->GetPosAtIdx(i, jmax-1, x1, y1, x2, y2))
	{
		m_NoColFormat--;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,jmax-1);

		m_HorzCursor = i;
		m_VertCursor = jmax-1;
	}	
	else if (j > 0 && this->EnsureVisible(i, j-1))
	{
		RECT Rect;
		this->GetPosAtIdx(i, j-1, x1, y1, x2, y2);

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j-1);

		m_HorzCursor = i;
		m_VertCursor = j-1;
	}
	else if (this->EnsureVisible(i-1, jmax-1) && this->GetPosAtIdx(i-1, jmax-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au format en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i-1,jmax-1);

		m_HorzCursor = i-1;
		m_VertCursor = jmax-1;
	}
	// Recherche 1ere ligne en bas accessible
	else if (i>0 && GetLastRowDispo(LastRowVisible, x1, y1, x2, y2))
	{
		this->GetPosAtIdx(i-1, LastRowVisible, x1, y1, x2, y2);

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i-1,LastRowVisible);

		m_HorzCursor = i-1;
		m_VertCursor = LastRowVisible;
	}
	// Sinon on tente le mode édition sur la cellule à côté en haut à gauche
	else if (this->GetPosAtIdx(i-1, jmax-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i-1,jmax-1);

		m_HorzCursor = i-1;
		m_VertCursor = jmax-1;
	}

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

////////////////////////////////////////////////////////////////////////
// Return dernière ligne visible sur le grid
bool JFCGridMultiCell::GetLastRowDispo(LONG & LastRowVisible, LONG & x1, LONG & y1, LONG & x2, LONG & y2)
{
	LastRowVisible = this->GetVertCount();

	while (!this->GetPosAtIdx(0, LastRowVisible, x1, y1, x2, y2) && LastRowVisible >=0)
	{
		LastRowVisible--;
	}

	return (LastRowVisible >= 0);
}

////////////////////////////////////////////////////////////////////////
// Return dernière ligne visible sur le grid

bool JFCGridMultiCell::GetLastColDispo(LONG & LastColVisible, LONG & x1, LONG & y1, LONG & x2, LONG & y2)
{
	LastColVisible = this->GetHorzCount();

	while (!this->GetPosAtIdx(LastColVisible, 0, x1, y1, x2, y2) && LastColVisible >=0)
	{
		LastColVisible--;
	}

	return (LastColVisible >= 0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule à gauche
LRESULT JFCGridMultiCell::OnMsgNewEditingLeft(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la précédente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on récupère l'élément sous le curseur
	LONG i,j;
	//GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	LONG LastColVisible = imax;

	// Row & Col de la cellule courante éditée
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG x1, y1, x2, y2;


	if (m_HorzCellCount == 1 && this->EnsureVisible(i-1, j) && this->GetPosAtIdx(i-1, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i-1,j);

		m_HorzCursor = i-1;
		m_VertCursor = j;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat > 1 && this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		m_NoColFormat--;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat == 1 && this->EnsureVisible(i-1, j) && this->GetPosAtIdx(i-1, j, x1, y1, x2, y2))
	{
		m_NoColFormat = m_HorzCellCount;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i-1,j);

		m_HorzCursor = i-1;
		m_VertCursor = j;
	}
	// Si mode sans jump ligne colonne on arrête là
	else if (!SpecialEditWithJump && this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}
	else if (j > 0 && this->EnsureVisible(imax-1, j-1) &&	this->GetPosAtIdx(imax-1, j-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,imax,j-1);

		m_HorzCursor = imax;
		m_VertCursor = j-1;
	}
	// Recherche 1ere colonne à droite accessible
	else if (j > 0 && GetLastColDispo(LastColVisible, x1, y1, x2, y2))
	{
		this->GetPosAtIdx(LastColVisible, j-1, x1, y1, x2, y2);

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,LastColVisible,j-1);

		m_HorzCursor = LastColVisible;
		m_VertCursor = j-1;
	}
	else if (this->GetPosAtIdx(imax-1, j-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,imax-1,j-1);

		m_HorzCursor = imax-1;
		m_VertCursor = j-1;
	}
	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule à droite
LRESULT JFCGridMultiCell::OnMsgNewEditingRight(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la précédente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on récupère l'élément sous le curseur
	LONG i,j;
	//GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	// Row & Col de la cellule courante éditée
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG x1, y1, x2, y2;
	if (m_HorzCellCount == 1 && this->EnsureVisible(i+1, j) && this->GetPosAtIdx(i+1, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i+1,j);

		m_HorzCursor = i+1;
		m_VertCursor = j;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat < m_HorzCellCount && this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		m_NoColFormat++;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}
	else if (m_HorzCellCount > 1 && m_NoColFormat == m_HorzCellCount && this->EnsureVisible(i+1, j) && this->GetPosAtIdx(i+1, j, x1, y1, x2, y2))
	{
		m_NoColFormat = 1;

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i+1,j);

		m_HorzCursor = i+1;
		m_VertCursor = j;
	}
	// Si mode sans jump ligne colonne on arrête là
	else if (!SpecialEditWithJump && this->EnsureVisible(i, j) && this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}
	else if (this->EnsureVisible(0, j+1) && this->GetPosAtIdx(0, j+1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,0,j+1);

		m_HorzCursor = 0;
		m_VertCursor = j+1;
	}

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

// Passe à la cellule suivante
LRESULT JFCGridMultiCell::OnMsgNewEditing(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Pas d'édition sur cellule suivante si on est en mode bloc selection
	if (m_ModeBlocEdit == true) 
		return 0;

	// Recup dernier pt de la précédente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on récupère l'élément sous le curseur
	LONG i,j;
	// GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	// Row & Col de la cellule courante éditée
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	LONG x1, y1, x2, y2;
	if (this->GetPosAtIdx(i, j+1, x1, y1, x2, y2))
	{
		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j+1);

		m_HorzCursor = i;
		m_VertCursor = j+1;
	}
	else if (j < (jmax-1) && this->EnsureVisible(i, j+1))
	{
		RECT Rect;
		this->GetPosAtIdx(i, j+1, x1, y1, x2, y2);

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j+1);

		m_HorzCursor = i;
		m_VertCursor = j+1;
	}
	else if (j==jmax && this->GetPosAtIdx(i+1, 0, x1, y1, x2, y2))
	{
		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i+1,0);

		m_HorzCursor = i+1;
		m_VertCursor = 0;
	}

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// Attention au forat en cours
		ZoneEditFormat(Rect, x1, x2, m_NoColFormat);

		// on démarre l'édition de l'élément
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération des infos zone sélectionnées
void JFCGridMultiCell::GetRowColBeginSelect(LONG &Row1, LONG &Col1, LONG &NoFormat1)
{
	// Col & Row début sélection
	Row1 = m_BeginRow;
	Col1 = m_BeginCol;

	// Le format début du bloc sélectionné
	NoFormat1 = m_NoColFormatDown;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Init des infos zone sélectionnées
void JFCGridMultiCell::InitZoneSelect()
{
	// Init Col & Row début sélection
	m_BeginRow = -1;
	m_BeginCol = -1;

	// Init format début du bloc sélectionné
	m_NoColFormatDown = -1;
}


/*
LRESULT JFCGridMultiCell::OnMsgNewEditing(WPARAM wParam, LPARAM lParam)
{
// Rend tooltip inactif
m_tool.Activate(FALSE);

// Recup dernier pt de la précédente edit box
POINT * pPoint = (POINT *)lParam;

this->SetFocus();

RECT Rect;
this->GetWindowRect(&Rect);

// on récupère l'élément sous le curseur
LONG i,j;
// GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

// Row & Col de la cellule courante éditée
i = m_EditBox_Col;
j = m_EditBox_Row;

LONG imax,jmax;
imax = this->GetHorzCount();
jmax = this->GetVertCount();

LONG x1, y1, x2, y2;
if (this->GetPosAtIdx(i, j+1, x1, y1, x2, y2))
{
RECT Rect;

// on initialise le rectangle (attention multi cellule)
Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

// on démarre l'édition de l'élément
SetEditBox(Rect,i,j+1);
}

// Tente de descendre le grid via scroll vertical
else if (j < jmax)
{
CRect rect;
GetClientRect(rect);
this->ScrollWindow(0,10,rect);
rect.top = rect.bottom = +10;
this->InvalidateRect(rect);

if (this->GetPosAtIdx(i, j+1, x1, y1, x2, y2))
{
RECT Rect;

// on initialise le rectangle (attention multi cellule)
Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

// on démarre l'édition de l'élément
SetEditBox(Rect,i,j+1);
}
}

// On se remet sur la cellule initiale
else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
{
RECT Rect;

// on initialise le rectangle (attention multi cellule)
Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

// on démarre l'édition de l'élément
SetEditBox(Rect,i,j);
}

// on quitte
return (0);
}
*/



