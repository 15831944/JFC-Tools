// on inclut les d�finitions n�cessaires
#include "stdafx.h"

// on inclut les d�finitions n�cessaires
#include "JFCGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCGrid::JFCGrid()
{
	m_tool.Create(this);	// on initialise le nombre d'�l�ments
	m_HorzCount  = m_VertCount  = 0;
	m_HorzCursor = m_VertCursor = -1;

	// Acces par d�faut non autoris� sur mode cellule
	m_AccesModeEdit = false;
}

/////////////////////////////////////////////////////////////////////////////
// Autorise ou n'autorise pas le mode acc�s �dition sur cellule (par d�faut autoris�)
void JFCGrid::SetAccesEditBox(bool OkEdit)
{
	m_AccesModeEdit = OkEdit;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nombre d'�l�ments horizontaux
/////////////////////////////////////////////////////////////////////////////

void JFCGrid::SetHorzCount(LONG nombre, BOOL update)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on initialise le nombre d'�l�ments
	m_HorzCount = (nombre > 0) ? nombre : 0;

	// on initialise l'intervalle des �l�ments
	this->SetHorzItemRange(0, (nombre > 0) ? (nombre - 1) : 0);

	// on redessine la fen�tre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

LONG JFCGrid::GetHorzCount()
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie le nombre d'�l�ments
	return (m_HorzCount);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nombre d'�l�ments verticaux
/////////////////////////////////////////////////////////////////////////////

void JFCGrid::SetVertCount(LONG nombre, BOOL update)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on initialise le nombre d'�l�ments
	m_VertCount = (nombre > 0) ? nombre : 0;

	// on initialise l'intervalle des �l�ments
	this->SetVertItemRange(0, (nombre > 0) ? (nombre - 1) : 0);

	// on redessine la fen�tre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

LONG JFCGrid::GetVertCount()
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie le nombre d'�l�ments
	return (m_VertCount);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCGrid::~JFCGrid()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCGrid, JFCViewport)
	//{{AFX_MSG_MAP(JFCGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(JFCEditGrid::JFC_CANCEL_EDITING, OnMsgCancelEditing)
	ON_MESSAGE(JFCEditGrid::JFC_END_EDITING, OnMsgEndEditing)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_DOWN,OnMsgNewEditingDown)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_UP,OnMsgNewEditingUp)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_LEFT,OnMsgNewEditingLeft)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING_RIGHT,OnMsgNewEditingRight)
	ON_MESSAGE(JFCEditGrid::JFC_BEGIN_NEW_EDITING,OnMsgNewEditing)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules
/////////////////////////////////////////////////////////////////////////////

void JFCGrid::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour �diter les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool JFCGrid::OnBeginEditing(CString &CurString ,LONG i, LONG j)
{
	// on arr�te l'�dition des �l�ments
	return (false);
}

bool JFCGrid::OnEditItem(const TCHAR* pString, LONG i, LONG j)
{
	// on arr�te l'�dition des �l�ments
	return (false);
}

void JFCGrid::OnCancelEditing()
{
	// on ne fait rien
}

void JFCGrid::OnEndEditing()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris
/////////////////////////////////////////////////////////////////////////////

void JFCGrid::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

void JFCGrid::OnLButtonUp(LONG i, LONG j)
{
	// on ne fait rien
}


void JFCGrid::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

void JFCGrid::OnMouseMove(LONG i, LONG j)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool JFCGrid::OnBeginDrawing(CDC & dc)
{
	// on teste si on a des �l�ments � dessiner
	if ((m_HorzCount == 0) || (m_VertCount == 0)) return (false);

	// on cr�e les stylos
	m_PenA.CreatePen(PS_SOLID, 1, CATPColors::GetColorSelect(m_CATPColorsApp));
	m_PenG.CreatePen(PS_SOLID, 1, CATPColors::GetColorLight(CATPColors::GREY_MOUSE));

	// on cr�e la fonte
	m_FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), &dc);

	
	// on cr�e les pinceaux
	m_BrushA.CreateSolidBrush(CATPColors::GetColorSelect(m_CATPColorsApp));
	m_BrushB.CreateSolidBrush(CATPColors::GetColorDark(m_CATPColorsApp));
	m_BrushC.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
	m_BrushD.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));

	//////////////////////////////////////////////////
	// TEST COULEUR DEGRADE
	/*
	CBitmap bmDegradeDark;
	bmDegradeDark.LoadBitmap(IDB_BLEU_DARK_DEGRADE);
	CBitmap bmDegradeMedium;
	bmDegradeMedium.LoadBitmap(IDB_BLEU_MEDIUM_DEGRADE);
	CBitmap bmDegradeLight;
	bmDegradeLight.LoadBitmap(IDB_BLEU_LIGHT_DEGRADE);

	m_BrushLight.CreatePatternBrush(&bmDegradeLight);		// le pinceau degrad� light
	m_BrushMedium.CreatePatternBrush(&bmDegradeMedium);		// le pinceau degrad� m�dium
	m_BrushDark.CreatePatternBrush(&bmDegradeDark);			// le pinceau d�grad� dark
	*/

		// on continue le dessin des �l�ments
	return (true);
}

void JFCGrid::OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat)
{
	// on ne fait rien
}

void JFCGrid::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	RECT cadre;

	// on r�cup�re la position des �l�ments
	LONG x1 = rect.left;
	LONG x2 = rect.right - 1;
	LONG y1 = rect.top;
	LONG y2 = rect.bottom - 1;

	// on r�cup�re le stylo, la fonte initiaux
	CPen*  pOldPen  = dc.GetCurrentPen();
	CFont* pOldFont = dc.GetCurrentFont();

	// on modifie la couleur du texte
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorBlack());

	// on modifie le mode de transparence
	LONG OldBack = dc.SetBkMode(TRANSPARENT);

	// on s�lectionne le stylo A et la fonte S
	dc.SelectObject(&m_PenA);
	dc.SelectObject(&m_FontS);

	// on initialise le cadre
	cadre.left   = x1 + 3;
	cadre.right  = x2 - 3;
	cadre.top    = y1 + 2;
	cadre.bottom = y2 - 1;

	// on v�rifie la validit� du cadre
	if ((cadre.left < cadre.right) && (cadre.top < cadre.bottom))
	{
		// on dessine l'int�rieur de la cellule
		this->OnDrawCell(dc, cadre, i, j, m_BrushA, m_BrushB, m_BrushC, m_BrushD);

		// this->OnDrawCell(dc, cadre, i, j, m_BrushA, m_BrushDark, m_BrushMedium, m_BrushLight);

		/*
		static COLORREF clr_degrad[CATPColors::m_NbGrad];
		if (ModeDegrad == CATPColors::LIGHT_DEGRAD)
			CATPColors::GetColorLightGraduation(CATPColors::COLORCREUSET, clr_degrad, NbGrad);
		else if (ModeDegrad == CATPColors::MEDIUM_DEGRAD)
			CATPColors::GetColorMediumGraduation(CATPColors::COLORCREUSET, clr_degrad, NbGrad);
		else
			CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, NbGrad);
		CATPColors::FillRectGraduate(*pDC, *pRect, clr_degrad, NbGrad);
		*/
	}

	// on teste si on est sur la derni�re ligne
	if ((j + 1) < m_VertCount)
	{
		// on s�lectionne le stylo G
		dc.SelectObject(&m_PenG);

		// on dessine le s�parateur horizontal
		dc.MoveTo(x1, y2); dc.LineTo(x2, y2);

		// on s�lectionne le stylo A si on est sur la derni�re colonne
		if ((i + 2) > m_HorzCount) dc.SelectObject(&m_PenA);

		// on dessine le s�parateur vertical
		dc.MoveTo(x2, y1); dc.LineTo(x2, y2 + 1);
	}
	else
	{
		// on s�lectionne le stylo A
		dc.SelectObject(&m_PenA);

		// on dessine le s�parateur horizontal
		dc.MoveTo(x1, y2); dc.LineTo(x2 + 1, y2);

		// on s�lectionne le stylo G si on est sur la derni�re colonne
		if ((i + 1) < m_HorzCount) dc.SelectObject(&m_PenG);

		// on dessine le s�parateur vertical
		dc.MoveTo(x2, y1); dc.LineTo(x2, y2);
	}

	// on s�lectionne le stylo A et la fonte S
	dc.SelectObject(&m_PenA);
	dc.SelectObject(&m_FontS);

	//// on initialise le cadre
	//cadre.left   = x1 + 3;
	//cadre.right  = x2 - 3;
	//cadre.top    = y1 + 2;
	//cadre.bottom = y2 - 1;

	//// on v�rifie la validit� du cadre
	//if ((cadre.left < cadre.right) && (cadre.top < cadre.bottom))
	//{
	//	// on dessine l'int�rieur de la cellule
	//	this->OnDrawCell(dc, cadre, i, j, m_BrushA, m_BrushB, m_BrushC, m_BrushD);
	//}

	// on teste si on doit dessiner le curseur
	//if (m_HorzCursor == i && m_VertCursor == j)
	//{
	//	RECT RectF;
	//	// on calcule le rectangle
	//	RectF.left = cadre.left - 1; RectF.top = cadre.top - 1; RectF.right = cadre.right + 1; RectF.bottom = cadre.bottom;
	//	// on le dessine
	//	if (RectF.left < RectF.right && RectF.top < RectF.bottom) dc.DrawFocusRect(&RectF);
	//}

	// on restaure le mode de transparence
	dc.SetBkMode(OldBack);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure le stylo et la fonte initial
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
}

void JFCGrid::OnEndDrawing(CDC & dc)
{
	// on d�truit les objets graphiques
	m_PenA  .DeleteObject();
	m_PenG  .DeleteObject();
	m_FontS .DeleteObject();

	m_BrushA.DeleteObject();
	m_BrushB.DeleteObject();
	m_BrushC.DeleteObject();
	m_BrushD.DeleteObject();
	
	// >d�truit les brushs d�grad�es
	m_BrushLight.DeleteObject();
	m_BrushMedium.DeleteObject();
	m_BrushDark.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// JFCGrid message handlers

void JFCGrid::PreSubclassWindow() 
{
	// on initialise le nombre d'�l�ments
	m_HorzCount = m_VertCount = 0;

	// on appelle le gestionnaire de base
	this->JFCViewport::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(0, TRUE, FALSE);
	this->SetVertItemSize(0, TRUE, FALSE);
}

void JFCGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus();

	// on teste si on a des �l�ments
	if ((m_HorzCount > 0) && (m_VertCount > 0))
	{
		LONG i,j;

		// on r�cup�re l'�l�ment sous le curseur
		if (this->GetIdxAtPos(point.x, point.y, i, j))
		{
			LONG x1, y1, x2, y2;

			// on teste si on doit redessiner la cellule de l'ancien curseur
			if (m_HorzCursor >= 0 && m_HorzCursor < m_HorzCount && m_VertCursor >= 0 && m_VertCursor < m_VertCount)
			{
				// on r�cup�re la position de la cellule
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

			// on teste l'�tat de la touche CTRL
			// if ((nFlags & MK_CONTROL) != 0)

			// Saisie via �dit box, si acc�s �dition autoris�
			if ((nFlags & MK_CONTROL) == 0 && m_AccesModeEdit)
			{
				// on r�cup�re la position de la cellule
				if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
				{
					RECT Rect;

					// on initialise le rectangle
					Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

					// on redessine la cellule
					//this->InvalidateRect(&Rect, FALSE);
					//this->UpdateWindow();

					// On lance l'�dition
					SetEditBox(Rect,i,j);
				}
			}
			else
			{
				// Ferme la boite �dition si en cours....
				CloseEditBox();

				// on effectue le traitement
				this->OnLButtonDown(i, j);
			}
		}
	}
	
	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonDown(nFlags, point);
}

void JFCGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonUp(nFlags, point);
}

// move about with keyboard
void JFCGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL Ok = true;

}

//////////////////////////////////////////////////////////////////////////////////////////
// Fermeture boite �dit box
void JFCGrid::CloseEditBox()
{
	if (m_EditBox.GetSafeHwnd() != NULL)
	{
		// on d�truit la fen�tre
		m_EditBox.DestroyWindow();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Positionnement et lancement boite �dition 
void JFCGrid::SetEditBox(CRect Rect, LONG i, LONG j)
{
	// on d�marre l'�dition de l'�l�ment
	CString TxtNbInsCur;

	if (m_EditBox.GetSafeHwnd() == NULL && this->OnBeginEditing(TxtNbInsCur, i, j))
	{
		// on corrige le rectangle
		Rect.right -= 1;

		// on cr�e la boite d'�dition
		m_EditBox.Create(WS_VISIBLE | ES_CENTER, Rect, this, 0);

		/*
		CDC * pDC = m_EditBox.GetDC();
		pDC->SetBkColor(LIGHTBLUE);
		*/

		// Positionne le focus sur la boite d'�dition
		m_EditBox.SetFocus();

		// Puis on inscrit le texte par d�faut
		m_EditBox.SetWindowText(TxtNbInsCur); 

		// Recup ligne et colonne o� se trouve la boite d'�dition
		// m_EditBox_Col = i;
		// m_EditBox_Row = j;

		m_EditBox_Col = i;
		m_EditBox_Row = j;

		if (j==3)
		{
			bool stop = true;
		}

		// Puis on se met en mode overwrite
		m_EditBox.SetSel(0,-1);

	}
	else
	{
		// on arr�te l'�dition
		this->OnEndEditing();
	}
}

LRESULT JFCGrid::OnMsgCancelEditing(WPARAM wParam, LPARAM lParam)
{
	// on arr�te l'�dition de l'�l�ment
	this->OnCancelEditing();

	// on d�truit la fen�tre
	m_EditBox.DestroyWindow();

	// on quitte
	return (0);
}

LRESULT JFCGrid::OnMsgEndEditing(WPARAM wParam, LPARAM lParam)
{

	// on r�cup�re le texte
	CString Text;
	m_EditBox.GetWindowText(Text);

	// on �dite l'�l�ment
	if (!this->OnEditItem((const TCHAR*)Text, m_HorzCursor, m_VertCursor))
	{
		// on arr�te l'�dition de l'�l�ment
		this->OnCancelEditing();
	}
	else
	{
		// on termine l'�dition de l'�l�ment
		this->OnEndEditing();
	}

	// on d�truit la fen�tre
	m_EditBox.DestroyWindow();

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule d'en dessous
LRESULT JFCGrid::OnMsgNewEditingDown(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la pr�c�dente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on r�cup�re l'�l�ment sous le curseur
	LONG i,j;
	//GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	// Row & Col de la cellule courante �dit�e
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG x1, y1, x2, y2;
	if (this->EnsureVisible(i, j+1) && this->GetPosAtIdx(i, j+1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j+1);

		m_HorzCursor = i;
		m_VertCursor = j+1;
	}
	else if (this->EnsureVisible(i+1, 0) && this->GetPosAtIdx(i+1, 0, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i+1,0);

		m_HorzCursor = i+1;
		m_VertCursor = 0;
	}

	/*
	else
	{
		// on d�truit la fen�tre
		m_EditBox.DestroyWindow();
	}
	*/

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule du dessus
LRESULT JFCGrid::OnMsgNewEditingUp(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la pr�c�dente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on r�cup�re l'�l�ment sous le curseur
	LONG i,j;

	// Row & Col de la cellule courante �dit�e
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	/*
	LONG x1, y1, x2, y2;
	if (this->GetPosAtIdx(i, j-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j-1);
	}
	else if (this->GetPosAtIdx(i-1, 0, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i-1,0);
	}
	else
	{
		// on d�truit la fen�tre
		m_EditBox.DestroyWindow();
	}
	*/

	LONG x1, y1, x2, y2;
	LONG LastRowVisible = jmax;

	// On tente le mode �dition sur la cellule du dessus
	if (this->EnsureVisible(i, j-1) && this->GetPosAtIdx(i, j-1, x1, y1, x2, y2))
	{
		
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j-1);

		m_HorzCursor = i;
		m_VertCursor = j-1;
	}
	else if (j > 0 && this->EnsureVisible(i, j-1))
	{
		RECT Rect;
		this->GetPosAtIdx(i, j-1, x1, y1, x2, y2);

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j-1);

		m_HorzCursor = i;
		m_VertCursor = j-1;
	}
	// Recherche 1ere ligne en bas accessible
	else if (i>0 && GetLastRowDispo(LastRowVisible, x1, y1, x2, y2))
	{
		this->GetPosAtIdx(i-1, LastRowVisible, x1, y1, x2, y2);

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i-1,LastRowVisible);

		m_HorzCursor = i-1;
		m_VertCursor = LastRowVisible;
	}
	// Sinon on tente le mode �dition sur la cellule � c�t� en haut � gauche
	else if (this->GetPosAtIdx(i-1, jmax-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i-1,jmax-1);

		m_HorzCursor = i-1;
		m_VertCursor = jmax-1;
	}
	/*
	else
	{
		// on d�truit la fen�tre
		m_EditBox.DestroyWindow();
	}
	*/

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

////////////////////////////////////////////////////////////////////////
// Return derni�re ligne visible sur le grid
bool JFCGrid::GetLastRowDispo(LONG & LastRowVisible, LONG & x1, LONG & y1, LONG & x2, LONG & y2)
{
	LastRowVisible = this->GetVertCount();

	while (!this->GetPosAtIdx(0, LastRowVisible, x1, y1, x2, y2) && LastRowVisible >=0)
	{
		LastRowVisible--;
	}

	return (LastRowVisible >= 0);
}

////////////////////////////////////////////////////////////////////////
// Return derni�re ligne visible sur le grid

bool JFCGrid::GetLastColDispo(LONG & LastColVisible, LONG & x1, LONG & y1, LONG & x2, LONG & y2)
{
	LastColVisible = this->GetHorzCount();

	while (!this->GetPosAtIdx(LastColVisible, 0, x1, y1, x2, y2) && LastColVisible >=0)
	{
		LastColVisible--;
	}

	return (LastColVisible >= 0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule � gauche
LRESULT JFCGrid::OnMsgNewEditingLeft(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la pr�c�dente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on r�cup�re l'�l�ment sous le curseur
	LONG i,j;
	//GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	LONG LastColVisible = imax;

	// Row & Col de la cellule courante �dit�e
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG x1, y1, x2, y2;

	if (this->EnsureVisible(i-1, j) && this->GetPosAtIdx(i-1, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i-1,j);

		m_HorzCursor = i-1;
		m_VertCursor = j;
	}
	else if (j > 0 && this->EnsureVisible(imax-1, j-1) &&	this->GetPosAtIdx(imax-1, j-1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,imax,j-1);

		m_HorzCursor = imax;
		m_VertCursor = j-1;
	}
	// Recherche 1ere colonne � droite accessible
	else if (j > 0 && GetLastColDispo(LastColVisible, x1, y1, x2, y2))
	{
		this->GetPosAtIdx(LastColVisible, j-1, x1, y1, x2, y2);

		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
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

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,imax-1,j-1);

		m_HorzCursor = imax-1;
		m_VertCursor = j-1;
	}
	/*
	else
	{
		// on d�truit la fen�tre
		m_EditBox.DestroyWindow();
	}
	*/
	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

//////////////////////////////////////////////////////////////////////////////
// Passe en mode edit sur la cellule � droite
LRESULT JFCGrid::OnMsgNewEditingRight(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la pr�c�dente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on r�cup�re l'�l�ment sous le curseur
	LONG i,j;
	//GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	LONG imax,jmax;
	imax = this->GetHorzCount();
	jmax = this->GetVertCount();

	// Row & Col de la cellule courante �dit�e
	i = m_EditBox_Col;
	j = m_EditBox_Row;

	LONG x1, y1, x2, y2;
	if (this->EnsureVisible(i+1, j) && this->GetPosAtIdx(i+1, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i+1,j);

		m_HorzCursor = i+1;
		m_VertCursor = j;
	}
	else if (this->EnsureVisible(0, j+1) && this->GetPosAtIdx(0, j+1, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,0,j+1);

		m_HorzCursor = 0;
		m_VertCursor = j+1;
	}
	/*
	else
	{
		// on d�truit la fen�tre
		m_EditBox.DestroyWindow();
	}
	*/

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		OnMsgEndEditing(wParam, lParam);

		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

// Passe � la cellule suivante
LRESULT JFCGrid::OnMsgNewEditing(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la pr�c�dente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on r�cup�re l'�l�ment sous le curseur
	LONG i,j;
	
	// Row & Col de la cellule courante �dit�e
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

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j+1);

		m_HorzCursor = i;
		m_VertCursor = j+1;
	}
	/*
	else if (j < (jmax-1))
	{
		// les fonctions pour modifier la position des ascenseurs actifs
		LONG positionV = OnGetVertPosition();
		if (OnSetVertPosition(positionV+1)) 
			OnVertScrollPosition(); 
		// this->EnsureVisible(i+1, j+1);
		
		RECT Rect;
		this->GetPosAtIdx(i, j+1, x1, y1, x2, y2);

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j+1);
	}
	*/
	else if (j < (jmax-1) && this->EnsureVisible(i, j+1))
	{
		RECT Rect;
		this->GetPosAtIdx(i, j+1, x1, y1, x2, y2);

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j+1);

		m_HorzCursor = i;
		m_VertCursor = j+1;
	}
	else if (j==jmax && this->GetPosAtIdx(i+1, 0, x1, y1, x2, y2))
	{
		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
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

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j);

		m_HorzCursor = i;
		m_VertCursor = j;
	}

	// on quitte
	return (0);
}

/*
LRESULT JFCGrid::OnMsgNewEditing(WPARAM wParam, LPARAM lParam)
{
	// Rend tooltip inactif
	m_tool.Activate(FALSE);

	// Recup dernier pt de la pr�c�dente edit box
	POINT * pPoint = (POINT *)lParam;

	this->SetFocus();

	RECT Rect;
	this->GetWindowRect(&Rect);

	// on r�cup�re l'�l�ment sous le curseur
	LONG i,j;
	// GetIdxAtPos(pPoint->x - Rect.left, pPoint->y - Rect.top, i, j);

	// Row & Col de la cellule courante �dit�e
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

		// on d�marre l'�dition de l'�l�ment
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

			// on d�marre l'�dition de l'�l�ment
			SetEditBox(Rect,i,j+1);
		}
	}

	// On se remet sur la cellule initiale
	else if (this->GetPosAtIdx(i, j, x1, y1, x2, y2))
	{
		RECT Rect;

		// on initialise le rectangle (attention multi cellule)
		Rect.left = x1; Rect.top = y1; Rect.right = x2; Rect.bottom = y2;

		// on d�marre l'�dition de l'�l�ment
		SetEditBox(Rect,i,j);
	}

	// on quitte
	return (0);
}
*/

void JFCGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus();

	// on teste si on a des �l�ments
	if ((m_HorzCount > 0) && (m_VertCount > 0))
	{
		LONG i,j;

		// on r�cup�re l'�l�ment sous le curseur
		if (this->GetIdxAtPos(point.x, point.y, i, j))
		{
			// on effectue le traitement
			this->OnRButtonDown(i, j);
		}
	}
	
	// on appelle le gestionnaire de base
	this->JFCViewport::OnRButtonDown(nFlags, point);
}

void JFCGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	// on teste si on a des �l�ments
	if ((m_HorzCount > 0) && (m_VertCount > 0))
	{
		LONG i,j;

		// on r�cup�re l'�l�ment sous le curseur
		if (this->GetIdxAtPos(point.x, point.y, i, j))
		{
			// on effectue le traitement
			this->OnMouseMove(i, j);
		}
		else
			this->OnMouseMove(-1, -1);

	}
	
	// on appelle le gestionnaire de base
	this->JFCViewport::OnMouseMove(nFlags, point);
}

void JFCGrid::InitToolTip()
{
	if (m_tool.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_tool.Create(this);
		// Create inactive
		m_tool.Activate(FALSE);
		// Enable multiline
		m_tool.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		//m_ToolTip.SendMessage(TTM_SETTITLE, TTI_INFO, (LPARAM)_T("Title"));
	} // if
} // End of InitToolTip
