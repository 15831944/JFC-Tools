// on inclut les définitions nécessaires
#include "stdafx.h"

#include "JFCMiniCalendar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on définit le libellé des jours
static const TCHAR* g_LibMonth[12] = { "Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Décembre" };

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCMiniCalendar::JFCMiniCalendar()
{

}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la période
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCMiniCalendar::~JFCMiniCalendar()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCMiniCalendar, JFCViewport)
	//{{AFX_MSG_MAP(JFCMiniCalendar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCMiniCalendar::OnBeginDrawing(CDC & dc)
{
	// on teste si on a quelque chose à dessiner
	if ((!m_Debut.IsValid()) || (!m_Fin.IsValid())) return (false);

	// on crée les stylos
	m_PenA.CreatePen(PS_SOLID, 1, CATPColors::GetColorSelect(m_CATPColorsApp));
	m_PenG.CreatePen(PS_SOLID, 1, CATPColors::GetColorLight(CATPColors::GREY_MOUSE));

	// on crée le pinceau
	m_BrushD.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));

	// on crée la fonte
	m_FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), &dc);

	// on initialise le décalage
	m_Decalage = 0;

	// on continue le dessin des éléments
	return (true);
}

void JFCMiniCalendar::OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat)
{
	// on initialise les états
	long etatp = 0;
	long etatn = 0;

	// on calcule la position du cadre de l'élément
	LONG x1 = rect.left;
	LONG x2 = rect.right  - 1;
	LONG y1 = rect.top;
	LONG y2 = rect.bottom - 1;

	// on récupère la date du jour, du lendemain et de la fin
	JDate prev = m_Debut + i - 1;
	JDate date = m_Debut + i;
	JDate next = m_Debut + i + 1;

	// on récupère les états des jours précédents et suivants
	if (prev >= m_Debut) { etatp = this->OnGetItemState(i - 1, j); }
	if (next <= m_Fin)   { etatn = this->OnGetItemState(i + 1, j); }

	// on récupère le stylo et le pinceau initial
	CPen*   pOldPen   = dc.GetCurrentPen();
	CBrush* pOldBrush = dc.GetCurrentBrush();

	// on teste l'état
	switch (etat)
	{
	case 1:
		// on sélectionne le stylo G et le pinceau D
		dc.SelectObject(&m_PenG);
		dc.SelectObject(&m_BrushD);
		// on remplit le fond de l'élément
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

void JFCMiniCalendar::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	RECT cadre;

	// le détail des dates
	long DJour, DMois, DAnnee;
	long NJour, NMois, NAnnee;

	// on récupère le stylo et la fonte initiaux
	CPen*  pOldPen   = dc.GetCurrentPen();
	CFont* pOldFont  = dc.GetCurrentFont();

	// on modifie la couleur du texte
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorBlack());

	// on modifie le mode de transparence
	LONG OldBack = dc.SetBkMode(TRANSPARENT);

	// on calcule la position du cadre de l'élément
	LONG x1 = rect.left;
	LONG x2 = rect.right  - 1;

	//LONG y1 = rect.top;
	//LONG y2 = rect.bottom - (((rect.bottom - rect.top) / 3) * 2);
	//LONG y3 = rect.bottom - (((rect.bottom - rect.top) / 3) * 1);
	//LONG y4 = rect.bottom - 1;

	LONG yH = rect.top;
	LONG yM = rect.bottom - (((rect.bottom - rect.top) / 2) );
	LONG yB = rect.bottom - 1;

	// on récupère la date du jour, du lendemain et de la fin
	JDate date = m_Debut + i;
	JDate next = m_Debut + i + 1;

	// on récupère le détail des dates
	date.GetDate(DJour, DMois, DAnnee);
	next.GetDate(NJour, NMois, NAnnee);

	// on récupère le jour de la semaine
	LONG day = date.GetDayOfWeek();

	// on selectionne le stylo A
	dc.SelectObject(&m_PenA);

	// on dessine les séparateur horizontaux
//	dc.MoveTo(x1, y2); dc.LineTo(x2 + 1, y2);
	dc.MoveTo(x1, yM); dc.LineTo(x2 + 1, yM);
	
	// on teste si on dessine le dernier jour de la période
	if (date == m_Fin)
	{
		// on trace le séparateur vertical
		dc.MoveTo(x2, yB); dc.LineTo(x2, yH - 1);
	}
	else
	{
		// on teste si on est sur le dernier jour d'un mois
		if (DMois != NMois)
		{
			// on trace le séparateur vertical
			dc.MoveTo(x2, yM); dc.LineTo(x2, yB+1);
		}

		// on teste si on est dessine le dernier jour d'une semaine
		if (day == 6)
		{
			// on trace le séparateur vertical
			dc.MoveTo(x2, yM); dc.LineTo(x2, yH-1);
		}
		//else
		//{
		//	// on sélectionne le stylo G
		//	dc.SelectObject(&m_PenG);

		//	// on trace le séparateur vertical
		//	dc.MoveTo(x2, yM); dc.LineTo(x2, yH);
		//}
	}

	// on sélectionne la fonte S
	dc.SelectObject(&m_FontS);

	// on teste si on dessine le mois courant
	if ((DJour == 1) || (m_Decalage == 0))
	{
		TCHAR Texte[16];

		// le détail de la date
		long MJour, MMois, MAnnee;

		// on calcule la largeur d'un jour
		long size = x2 - x1 + 1;

		// on calcule la largeur maximale pour écrire le mois
		long maximum = x2 - 1 + (size * (m_Fin - date));

		// on passe au mois suivant
		(date + (32 - DJour)).GetDate(MJour, MMois, MAnnee);

		// on initialise le cadre
		cadre.left   = x1 + 1 - (size * (DJour - 1));
		cadre.right  = x2 - 1 + (size * (32 - MJour - DJour));
		cadre.top    = yM + 2;
		cadre.bottom = yB - 1;

		// on formate le texte
		sprintf_s(Texte, 16, "%s %04d", _T(g_LibMonth[DMois - 1]), DAnnee);

		// on calcule la taille du texte
		CSize Size = dc.GetTextExtent(Texte);

		// on calcule la position du texte
		cadre.left += (cadre.right - cadre.left - Size.cx) / 2;

		// on corrige la largeur du cadre
		if (cadre.right > maximum) cadre.right = maximum + 1;

		// on écrit le libellé du mois
		dc.DrawText(Texte, &cadre, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

	// on teste si on dessine le jour de la semaine
	if ((day == 0) || (m_Decalage == 0))
	{
		TCHAR Texte[16];

		// le détail de la date
		long MJour, MMois, MAnnee;

		// on calcule la largeur d'un jour
		long size = x2 - x1 + 1;

		// on calcule la largeur maximale pour écrire le mois
		long maximum = x2 - 1 + (size * (m_Fin - date));

		// on initialise le cadre
		cadre.left   = x1 + 1 - (size * day);
		cadre.right  = x2 - 1 + (size * (6 - day));
		cadre.top    = yH + 2;
		cadre.bottom = yM - 1;

		// on corrige la largeur du cadre
		if (cadre.right > maximum) cadre.right = maximum + 1;

		// on récupère le détail de la date
		(date - day).GetDate(MJour, MMois, MAnnee);

		// on formate le texte
		sprintf_s(Texte, 16, "%02d", MJour);

		// on écrit le jour de la semaine
		dc.DrawText(Texte, &cadre, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

	// on restaure le mode de transparence
	dc.SetBkMode(OldBack);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure le stylo et la fonte
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);

	// on incrémente le décalage
	m_Decalage += 1;
}

void JFCMiniCalendar::OnEndDrawing(CDC & dc)
{
	// on détruit les objets graphiques
	m_PenA  .DeleteObject();
	m_PenG  .DeleteObject();
	m_FontS .DeleteObject();
	m_BrushD.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// JFCMiniCalendar message handlers



