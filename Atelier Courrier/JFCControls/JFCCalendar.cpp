// on inclut les d�finitions n�cessaires
#include "stdafx.h"

#include "JFCCalendar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on d�finit le libell� des jours
static const TCHAR* g_LibMonth[12] = { "Janvier", "F�vrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Ao�t", "Septembre", "Octobre", "Novembre", "D�cembre" };

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCCalendar::JFCCalendar()
{
	// on initialise l'indicateur
	m_FlagSel = FALSE;
	m_FrstSelTmp = -1;
	m_FrstSel = -1;
	m_LastSelTmp = -1;
	m_LastSel = -1;

}
void JFCCalendar::Reset()
{
	m_FlagSel = FALSE;
	m_FrstSelTmp = -1;
	m_FrstSel = -1;
	m_LastSelTmp = -1;
	m_LastSel = -1;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la p�riode
/////////////////////////////////////////////////////////////////////////////

void JFCCalendar::SetRange(JDate debut, JDate fin, BOOL update)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on v�rifie la validit� de la p�riode
	if ((debut.IsValid()) && (fin.IsValid()))
	{
		// on m�morise la p�riode
		m_Debut = (debut <= fin) ? debut : fin;
		m_Fin   = (debut <= fin) ? fin : debut;

		// on initialise l'intervalle de la p�riode
		this->SetHorzItemRange(0, m_Fin - m_Debut, TRUE);
	}
	else
	{
		// on r�initialise la p�riode
		m_Debut.Reset();
		m_Fin  .Reset();

		// on initialise l'intervalle de la p�riode
		this->SetHorzItemRange(0, 0, TRUE);
	}

	// on redessine la fen�tre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

void JFCCalendar::GetRange(JDate & debut, JDate & fin)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie la p�riode
	debut = m_Debut;
	fin   = m_Fin; 
}

void JFCCalendar::SetSelRange(JDate first, JDate last, BOOL update)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on v�rifie la validit� de la p�riode
	if ((first.IsValid()) && (last.IsValid()))
	{
		// on m�morise la p�riode
		m_FrstSel = first - m_Debut;
		m_LastSel = last - m_Debut;
	}
	else
	{
		// on r�initialise la p�riode
		m_FrstSel = -1;
		m_LastSel = -1;
		m_FrstSelTmp = -1;
		m_LastSelTmp = -1;
	}

	// on redessine la fen�tre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

void JFCCalendar::GetSelRange(JDate & first, JDate & last)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie la p�riode
	if (m_FrstSel!=-1 && m_LastSel!=-1)
	{
		first = m_Debut + m_FrstSel;
		last  = m_Debut + m_LastSel; 
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour rendre visible une date
/////////////////////////////////////////////////////////////////////////////

void JFCCalendar::EnsureVisible(JDate date)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on v�rifie la validit� de la p�riode et de la date
	if ((m_Debut.IsValid()) && (m_Fin.IsValid()) && (date.IsValid()))
	{
		// on corrige la date
		if (date < m_Debut) date = m_Debut; else if (date > m_Fin) date = m_Fin;

		// on rend visible la date
		this->JFCViewport::EnsureVisible(date - m_Debut, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCCalendar::~JFCCalendar()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCCalendar, JFCViewport)
	//{{AFX_MSG_MAP(JFCCalendar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer la s�lection des �l�ments
/////////////////////////////////////////////////////////////////////////////

BOOL JFCCalendar::OnGetItemSelection(JDate date)
{
	// on renvoie la s�lection de l'�l�ment
	if(m_FrstSel != -1 && m_LastSel != -1)
	{
		if( m_Debut+m_FrstSel <= date && date <= m_Debut+m_LastSel)
			return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour calculer le span des �l�ments
/////////////////////////////////////////////////////////////////////////////

void JFCCalendar::OnGetItemSpan(LONG i, LONG y, LONG & first, LONG & last)
{
	RECT rect;

	// on r�cup�re la zone cliente de la fen�tre
	this->GetClientRect(&rect);

	// on calcule les points de s�paration
	LONG y1 = rect.bottom - (((rect.bottom - rect.top) / 3) * 2);
	LONG y2 = rect.bottom - (((rect.bottom - rect.top) / 3) * 1);

	// on teste si on est sur un jour
	if (y >= y2)
	{
		// on renvoie le span de l'�l�ment
		first = i;
		last  = i;
	}
	else
	{
		// on initialise les bornes de l'intervalle
		long min = 0;
		long max = m_Fin - m_Debut;

		// on teste si on est sur une semaine
		if (y > y1)
		{
			// on r�cup�re le jour de la semaine
			LONG Jour = (m_Debut + i).GetDayOfWeek();

			// on renvoie le span de l'�l�ment
			first = i - (Jour);
			last  = i - (Jour - 6);
		}
		else
		{
			// le d�tail de la date
			LONG DJour, DMois, DAnnee;
			LONG FJour, FMois, FAnnee;

			// on r�cup�re le d�tail de la date
			(m_Debut + i     ).GetDate(DJour, DMois, DAnnee);
			(m_Debut + i + 31).GetDate(FJour, FMois, FAnnee);

			// on renvoie le span de l'�l�ment
			first = i - (DJour -  1);
			last  = i - (FJour - 31);
		}

		// on ram�ne les indices dans les limites
		first = (first < min) ? min : (first > max) ? max : first;
		last  = (last  < min) ? min : (last  > max) ? max : last;
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer l'�tat des �l�ments
/////////////////////////////////////////////////////////////////////////////

LONG JFCCalendar::OnGetItemState(LONG i, LONG j)
{
	// on teste si a une s�lection en cours
	return OnGetItemSelection(m_Debut + i);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool JFCCalendar::OnBeginDrawing(CDC & dc)
{
	// on teste si on a quelque chose � dessiner
	if ((!m_Debut.IsValid()) || (!m_Fin.IsValid())) return (false);

	// on cr�e les stylos
	m_PenA.CreatePen(PS_SOLID, 1, CATPColors::GetColorSelect(m_CATPColorsApp));
	m_PenG.CreatePen(PS_SOLID, 1, CATPColors::GetColorLight(CATPColors::GREY_MOUSE));

	// on cr�e le pinceau
	m_BrushD.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));

	// on cr�e la fonte
	m_FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), &dc);

	// on initialise le d�calage
	m_Decalage = 0;

	// on continue le dessin des �l�ments
	return (true);
}

void JFCCalendar::OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat)
{
	// on initialise les �tats
	long etatp = 0;
	long etatn = 0;

	// on calcule la position du cadre de l'�l�ment
	LONG x1 = rect.left;
	LONG x2 = rect.right  - 1;
	LONG y1 = rect.top;
	LONG y2 = rect.bottom - 1;

	// on r�cup�re la date du jour, du lendemain et de la fin
	JDate prev = m_Debut + i - 1;
	JDate date = m_Debut + i;
	JDate next = m_Debut + i + 1;

	// on r�cup�re les �tats des jours pr�c�dents et suivants
	if (prev >= m_Debut) { etatp = this->OnGetItemState(i - 1, j); }
	if (next <= m_Fin)   { etatn = this->OnGetItemState(i + 1, j); }

	// on r�cup�re le stylo et le pinceau initial
	CPen*   pOldPen   = dc.GetCurrentPen();
	CBrush* pOldBrush = dc.GetCurrentBrush();

	// on teste l'�tat
	switch (etat)
	{
	case 1:
		// on s�lectionne le stylo G et le pinceau D
		dc.SelectObject(&m_PenG);
		dc.SelectObject(&m_BrushD);
		// on remplit le fond de l'�l�ment
		dc.BeginPath();
		dc.MoveTo(x1 - 1, y1    );
		dc.LineTo(x2 + 1, y1    ); dc.LineTo(x2 + 1, y2 + 1);
		dc.LineTo(x1 - 1, y2 + 1); dc.LineTo(x1 - 1, y1    );
		dc.EndPath(); dc.FillPath();
		// on dessine le contour
		if (etatp != 1) { dc.MoveTo(x1 - 1, y1); dc.LineTo(x1 - 1, y2 + 1); }
		if (etatn != 1) { dc.MoveTo(x2 + 0, y1); dc.LineTo(x2 + 0, y2 + 1); }
		break;
	default:
		// on ne fait rien
		break;
	}

	// on restaure le pinceau et le stylo
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}

void JFCCalendar::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	RECT cadre;

	// le d�tail des dates
	long DJour, DMois, DAnnee;
	long NJour, NMois, NAnnee;

	// on r�cup�re le stylo et la fonte initiaux
	CPen*  pOldPen   = dc.GetCurrentPen();
	CFont* pOldFont  = dc.GetCurrentFont();

	// on modifie la couleur du texte
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorBlack());

	// on modifie le mode de transparence
	LONG OldBack = dc.SetBkMode(TRANSPARENT);

	// on calcule la position du cadre de l'�l�ment
	LONG x1 = rect.left;
	LONG x2 = rect.right  - 1;
	LONG y1 = rect.top;
	LONG y2 = rect.bottom - (((rect.bottom - rect.top) / 3) * 2);
	LONG y3 = rect.bottom - (((rect.bottom - rect.top) / 3) * 1);
	LONG y4 = rect.bottom - 1;

	// on r�cup�re la date du jour, du lendemain et de la fin
	JDate date = m_Debut + i;
	JDate next = m_Debut + i + 1;

	// on r�cup�re le d�tail des dates
	date.GetDate(DJour, DMois, DAnnee);
	next.GetDate(NJour, NMois, NAnnee);

	// on r�cup�re le jour de la semaine
	LONG day = date.GetDayOfWeek();

	// on selectionne le stylo A
	dc.SelectObject(&m_PenA);

	// on dessine les s�parateur horizontaux
	dc.MoveTo(x1, y2); dc.LineTo(x2 + 1, y2);
	dc.MoveTo(x1, y3); dc.LineTo(x2 + 1, y3);
	
	// on teste si on dessine le dernier jour de la p�riode
	if (date == m_Fin)
	{
		// on trace le s�parateur vertical
		dc.MoveTo(x2, y4); dc.LineTo(x2, y1 - 1);
	}
	else
	{
		// on teste si on est sur le dernier jour d'un mois
		if (DMois != NMois)
		{
			// on trace le s�parateur vertical
			dc.MoveTo(x2, y2); dc.LineTo(x2, y1 - 1);
		}

		// on teste si on est dessine le dernier jour d'une semaine
		if (day == 6)
		{
			// on trace le s�parateur vertical
			dc.MoveTo(x2, y4); dc.LineTo(x2, y2);
		}
		else
		{
			// on s�lectionne le stylo G
			dc.SelectObject(&m_PenG);

			// on trace le s�parateur vertical
			dc.MoveTo(x2, y4); dc.LineTo(x2, y3);
		}
	}

	// on s�lectionne la fonte S
	dc.SelectObject(&m_FontS);

	// on teste si on dessine le mois courant
	if ((DJour == 1) || (m_Decalage == 0))
	{
		TCHAR Texte[16];

		// le d�tail de la date
		long MJour, MMois, MAnnee;

		// on calcule la largeur d'un jour
		long size = x2 - x1 + 1;

		// on calcule la largeur maximale pour �crire le mois
		long maximum = x2 - 1 + (size * (m_Fin - date));

		// on passe au mois suivant
		(date + (32 - DJour)).GetDate(MJour, MMois, MAnnee);

		// on initialise le cadre
		cadre.left   = x1 + 1 - (size * (DJour - 1));
		cadre.right  = x2 - 1 + (size * (32 - MJour - DJour));
		cadre.top    = y1 + 2;
		cadre.bottom = y2 - 1;

		// on formate le texte
		sprintf_s(Texte, 16, "%s %04d", _T(g_LibMonth[DMois - 1]), DAnnee);

		// on calcule la taille du texte
		CSize Size = dc.GetTextExtent(Texte);

		// on calcule la position du texte
		cadre.left += (cadre.right - cadre.left - Size.cx) / 2;

		// on corrige la largeur du cadre
		if (cadre.right > maximum) cadre.right = maximum + 1;

		// on �crit le libell� du mois
		dc.DrawText(Texte, &cadre, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

	// on teste si on dessine le jour de la semaine
	if ((day == 0) || (m_Decalage == 0))
	{
		TCHAR Texte[16];

		// le d�tail de la date
		long MJour, MMois, MAnnee;

		// on calcule la largeur d'un jour
		long size = x2 - x1 + 1;

		// on calcule la largeur maximale pour �crire le mois
		long maximum = x2 - 1 + (size * (m_Fin - date));

		// on initialise le cadre
		cadre.left   = x1 + 1 - (size * day);
		cadre.right  = x2 - 1 + (size * (6 - day));
		cadre.top    = y2 + 2;
		cadre.bottom = y3 - 1;

		// on corrige la largeur du cadre
		if (cadre.right > maximum) cadre.right = maximum + 1;

		// on r�cup�re le d�tail de la date
		(date - day).GetDate(MJour, MMois, MAnnee);

		// on formate le texte
		sprintf_s(Texte, 16, "%02d", MJour);

		// on �crit le jour de la semaine
		dc.DrawText(Texte, &cadre, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

	// on restaure le mode de transparence
	dc.SetBkMode(OldBack);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure le stylo et la fonte
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);

	// on incr�mente le d�calage
	m_Decalage += 1;
}

void JFCCalendar::OnEndDrawing(CDC & dc)
{
	// on d�truit les objets graphiques
	m_PenA  .DeleteObject();
	m_PenG  .DeleteObject();
	m_FontS .DeleteObject();
	m_BrushD.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// JFCCalendar message handlers

void JFCCalendar::PreSubclassWindow() 
{
	// on initialise la p�riode
	m_Debut.Reset();
	m_Fin  .Reset();

	// on initialise l'indicateur
	m_FlagSel = FALSE;

	// on appelle le gestionnaire de base
	this->JFCViewport::PreSubclassWindow();

	// on initialise la taille des �l�ments
	this->SetHorzItemSize(7, TRUE, FALSE);
	this->SetVertItemSize(0, TRUE, FALSE);
}

void JFCCalendar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on a quelque chose � clicker
	if ((m_Debut.IsValid()) && (m_Fin.IsValid()))
	{
		// on teste si on peut lancer une s�lection
		if ((m_FlagSel == FALSE) && (NULL == ::GetCapture()))
		{
			LONG i, j;

			// on r�cup�re l'�l�ment sous le curseur
			if (this->GetIdxAtPos(point.x, point.y, i, j))
			{
				// on r�cup�re la date du jour
				JDate date = m_Debut + i;

				// on r�cup�re l'�tat de l'�l�ment
				m_EtatSel = 1;

				// on initialise la s�lection des �l�ments
				// on capture la souris
				::SetCapture(this->m_hWnd);

				// on teste si a captur� la souris
				if (this->m_hWnd == ::GetCapture())
				{
					// on modifie l'indicateur de s�lection
					m_FlagSel = TRUE;
					
					//si aucune selection n'a �t� faite
					if(m_FrstSel == -1 && m_LastSel== -1 )
					{
						this->OnGetItemSpan(i, point.y, m_FrstSelTmp, m_LastSelTmp);
						this->OnGetItemSpan(i, point.y, m_FrstSel, m_LastSel);
						this->JFCViewport::OnLButtonDown(nFlags, point);
						SendMessage(MSG_CALENDAR_SELCHANGING);
						this->InvalidateRect(NULL, FALSE);
						// on lance un timer
						this->SetTimer(48758, 20, NULL);
						return;
					}

					
					
					//////
					//////la selection des bornes d�pend de la position de i;
					//////
					
					//on recupere la zone autour de i
					LONG first, last;
					this->OnGetItemSpan(i, point.y, first, last);
				
					//si i est en dehors de la selection et la per selectionn�e est distante
					if ((i < m_FrstSel-1  || i > m_LastSel+1)
						&&((last < m_FrstSel-1  || first > m_LastSel+1)||(first<m_FrstSel && last>m_LastSel )))
					{
						//on commence une nouvelle selection
						this->OnGetItemSpan(i, point.y, m_FrstSel, m_LastSel);

					 	//// on inverse la s�lection si n�cessaire
						if (m_LastSel < m_FrstSel) { LONG temp = m_FrstSel; m_FrstSel = m_LastSel; m_LastSel = temp; }
	
						m_FrstSelTmp = m_FrstSel;
						m_LastSelTmp = m_FrstSel;

						// on met � jour l'affichage
						SendMessage(MSG_CALENDAR_SELCHANGING);
						this->InvalidateRect(NULL, FALSE);
						// on lance un timer
						this->SetTimer(48758, 20, NULL);
						return;
					}

					//si on est au bord+1
					if ((i == m_FrstSel-1  || i == m_LastSel+1)
						|| ((first <= m_LastSel+1 || last >= m_FrstSel-1)&&(i<m_FrstSel-1 || i>m_LastSel+1)) )
					{
						
						m_FrstSel = (first <= m_FrstSel) ? first : m_FrstSel;
						m_LastSel = (last <= m_LastSel) ? m_LastSel : last;
						//// on inverse la s�lection si n�cessaire
						if (m_LastSel < m_FrstSel) { LONG temp = m_FrstSel; m_FrstSel = m_LastSel; m_LastSel = temp; }

						//cot� gauche
						if (i<=m_FrstSel)
						{
							m_FrstSelTmp = m_LastSel;
							m_LastSelTmp = m_LastSel;
						}

						//cot� droit
						if (i>=m_LastSel)
						{
							m_FrstSelTmp = m_FrstSel;
							m_LastSelTmp = m_FrstSel;
						}


						// on met � jour l'affichage
						SendMessage(MSG_CALENDAR_SELCHANGING);
						this->InvalidateRect(NULL, FALSE);
						// on lance un timer
						this->SetTimer(48758, 20, NULL);
						return;
					}


					//si la sel est la p�riode d�ja sel
					if(first == m_FrstSel && last == m_LastSel)
					{
						//RAZ sel
						m_FrstSel = -1;
						m_LastSel = -1;
						m_FrstSelTmp = -1;
						m_LastSelTmp = -1;
						m_FlagSel = FALSE;
						::ReleaseCapture();
						SendMessage(MSG_CALENDAR_SELCHANGED);
						this->InvalidateRect(NULL, FALSE);
						// on lance un timer
						//this->SetTimer(48758, 20, NULL);
						return;
					}

					if(first == m_FrstSel)
					{
						if (last+1<m_LastSel)
							m_FrstSel = last+1;
						else
						{
							m_FrstSel = -1;
							m_LastSel = -1;
							m_FrstSelTmp = -1;
							m_LastSelTmp = -1;
						}
						m_FlagSel = FALSE;
						::ReleaseCapture();
						SendMessage(MSG_CALENDAR_SELCHANGED);
						this->InvalidateRect(NULL, FALSE);
						return;
					}

					if(last == m_LastSel)
					{
						if (first-1>m_FrstSel)
							m_LastSel = first-1;
						else
						{
							m_FrstSel = -1;
							m_LastSel = -1;
							m_FrstSelTmp = -1;
							m_LastSelTmp = -1;
						}

						m_FlagSel = FALSE;
						::ReleaseCapture();
						SendMessage(MSG_CALENDAR_SELCHANGED);
						this->InvalidateRect(NULL, FALSE);
						return;
					}

					//dernier cas i est � l'interieur on revient sur le premier
					if (first <= m_FrstSel)
					{
						if(m_LastSel<last+1)
							m_FrstSel = m_LastSel = -1;
						else
							m_FrstSel = last+1;
						m_FlagSel = FALSE;
						::ReleaseCapture();
						SendMessage(MSG_CALENDAR_SELCHANGED);
						this->InvalidateRect(NULL, FALSE);
						return;
					}
					m_LastSel = first-1;
					m_FrstSelTmp = m_FrstSel;
					m_LastSelTmp = m_FrstSel;
					
					SendMessage(MSG_CALENDAR_SELCHANGING);
					this->InvalidateRect(NULL, FALSE);
					// on lance un timer
					this->SetTimer(48758, 20, NULL);
					return;


				}

			}
		}
	}
	SendMessage(MSG_CALENDAR_SELCHANGING);
	// on lance un timer
	this->SetTimer(48758, 20, NULL);
	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonDown(nFlags, point);
}

void JFCCalendar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si a une s�lection en cours
	if (m_FlagSel != FALSE)
	{
		// on lib�re la souris
		::ReleaseCapture();

		// on teste si la lib�ration s'est bien effectu�e
		if (this->m_hWnd != ::GetCapture())
		{
			// on lib�re le timer
			this->KillTimer(48758);

			// on r�initialise l'indicateur de s�lection
			m_FlagSel = FALSE;

			SendMessage(MSG_CALENDAR_SELCHANGED);
			// on redessine la fen�tre
			this->InvalidateRect(NULL, FALSE);
		}
	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonUp(nFlags, point);
}

void JFCCalendar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on teste si on a une s�lection en cours
	if (m_FlagSel != FALSE)
	{
		LONG i, j;

		// on r�cup�re l'�l�ment sous la position
		this->GetTrackIdx(point.x, point.y, i, j, FALSE);

		// on r�cup�re la s�lection courante
		LONG first, last;
		this->OnGetItemSpan(i, point.y, first, last);

		// on r�cup�re la s�lection courante
		if (m_FrstSel == -1)
		{
			m_FrstSel = first;
			m_LastSel = last;
			m_FrstSelTmp = m_FrstSel;
			m_LastSelTmp = m_FrstSel;
		}
		else
		{
			m_FrstSel = (m_FrstSelTmp <= first) ? m_FrstSelTmp : first;
			m_LastSel = (m_LastSelTmp <= last) ? last : m_LastSelTmp;
		}

		//// on inverse la s�lection si n�cessaire
		if (m_LastSel < m_FrstSel) { LONG temp = m_FrstSel; m_FrstSel = m_LastSel; m_LastSel = temp; }

		// on redessine la fen�tre
		SendMessage(MSG_CALENDAR_SELCHANGING);
		this->InvalidateRect(NULL, FALSE);


	}

	// on appelle le gestionnaire de base
	this->JFCViewport::OnMouseMove(nFlags, point);
}


void JFCCalendar::BtnT()
{
	//si on a une selection
	if(m_FrstSel != -1)
	{
		m_FrstSel = -1;
		m_LastSel = -1;
		m_FrstSelTmp = -1;
		m_LastSelTmp = -1;
	}
	else
	{
		m_FrstSel = 0;
		m_LastSel = m_Fin-m_Debut;
	}

	
	SendMessage(MSG_CALENDAR_SELCHANGED);
	this->InvalidateRect(NULL, FALSE);
}

void JFCCalendar::OnTimer(UINT nIDEvent) 
{
	// on teste si c'est le bon timer
	if (nIDEvent == 48758)
	{
		// on teste si on a une s�lection en cours
		if (m_FlagSel != FALSE)
		{
			LONG i,j;
			POINT point;

			// on r�cup�re la position du curseur
			DWORD pos = ::GetMessagePos();

			// on r�cup�re les coordonn�es du curseur
			point.x = (LONG)(SHORT)LOWORD(pos);
			point.y = (LONG)(SHORT)HIWORD(pos);

			// on convertit la position du curseur dans le rep�re de la fen�tre
			this->ScreenToClient(&point);

			// on r�cup�re l'�l�ment sous la position
			this->GetTrackIdx(point.x, point.y, i, j, TRUE);

			// on r�cup�re la s�lection courante
			LONG first, last;
			this->OnGetItemSpan(i, point.y, first, last);

	
			// on r�cup�re la s�lection courante
			if (m_FrstSel == -1)
			{
				m_FrstSel = first;
				m_LastSel = last;
				m_FrstSelTmp = m_FrstSel;
				m_LastSelTmp = m_FrstSel;
			}
			else
			{
				m_FrstSel = (m_FrstSelTmp <= first) ? m_FrstSelTmp : first;
				m_LastSel = (m_LastSelTmp <= last) ? last : m_LastSelTmp;
			}

			//// on inverse la s�lection si n�cessaire
			if (m_LastSel < m_FrstSel) { LONG temp = m_FrstSel; m_FrstSel = m_LastSel; m_LastSel = temp; }

			// on redessine la fen�tre
			SendMessage(MSG_CALENDAR_SELCHANGING);
			this->InvalidateRect(NULL, FALSE);

		}
	}
	// on appelle le gestionnaire de base
	this->JFCViewport::OnTimer(nIDEvent);
}
