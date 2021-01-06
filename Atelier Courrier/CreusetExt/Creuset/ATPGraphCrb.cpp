// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPGraphCrb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphCrb::CATPGraphCrb()
{
	// on initialise le maximum
	m_ValMax = 0.0;
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPGraphCrb::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPGraphCrb::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPGraphCrb::~CATPGraphCrb()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPGraphCrb, JFCViewport)
	//{{AFX_MSG_MAP(CATPGraphCrb)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////
// les fonctions pour dessiner les �l�ments

bool CATPGraphCrb::OnBeginDrawing(CDC & dc)
{
	// on v�rifie la validit� du document
	if (m_pATPDocCreuset == 0) return (false);

	// on continue le dessin des �l�ments
	return (true);
}

void CATPGraphCrb::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	//if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
	//	return;
	
	Graphics grf(dc);
	grf.SetSmoothingMode(SmoothingModeAntiAlias);
	
	Color clr;
	if(m_pATPDocCreuset->m_MapAffHypothese.GetNbAff()>1)
		clr.SetFromCOLORREF(CATPColors::GetColorGrph(m_pATPDocCreuset->m_IdxHypotheseCourant));
	else
        clr.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	Pen      penD(clr);
	clr.SetFromCOLORREF(CATPColors::GetColorDark(CATPColors::GREY_MOUSE));
	Pen      penM(clr);

	Color clr1;
	Color clr2;
	clr1.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	clr2.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	//clr1.SetFromCOLORREF(CATPColors::GetColorWhite());
	//clr2.SetFromCOLORREF(CATPColors::GetColorGrph(m_pATPDocCreuset->m_IdxHypotheseCourant));

	// on r�cup�re les positions verticales des histogrammes
	JInt32 y1 = rect.top    + 14;
	JInt32 y5 = rect.bottom -  6;
	if (y1 > y5)
		return;

	LinearGradientBrush linGrBrush( Point(0, y5), Point(0, rect.top), clr1, clr2);

	// on r�cup�re les positions horizontales
	JInt32 x1 = ((rect.right - rect.left) / 2) + (2 * rect.left) - rect.right;
	JInt32 x2 = rect.left;
	JInt32 x3 = ((rect.right - rect.left) / 2) + rect.left;
	JInt32 x4 = rect.right;

	// on recherche l'interrogation
	m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

	// on teste si on a trouv� l'hypoth�se
	if (!m_pATPDocCreuset->m_ITRHypotheses.IsValid())
		return;

	// on r�cup�re l'hypoth�se
	const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
	const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();

	// on v�rifie la validit� du jour
	if (i < 0 || i >= DATHypothese.m_NbContactsJour.GetCount())
		return;
	if (i < 0 || i >= DATHypothese.m_DistributionJour.GetCount())
		return;

	JBool bfill = false;
	JDate date = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() + i;
	if(m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD() <= date && date <= m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF())
		bfill = true;
	
	// on r�cup�re les couvertures � 1+ de la veille

	JFlt64 A2 = 0.0;
	JFlt64 A3 = 0.0;
	JFlt64 A4 = 0.0;

	CalcValues(DATHypoFormat, m_pATPDocCreuset->m_IdxHypotheseCourant, i, A2, A3, A4);

	// on calcule la hauteur de la courbe la veille
	JInt32 y2 = y5 - (JInt32)JMath::Round((A2 * (JFlt64)(y5 - y1)) / m_ValMax);
	JInt32 y3 = y5 - (JInt32)JMath::Round((A3 * (JFlt64)(y5 - y1)) / m_ValMax);
	JInt32 y4 = y5 - (JInt32)JMath::Round((A4 * (JFlt64)(y5 - y1)) / m_ValMax);

	// on teste le dessin � faire
	DrawSection(grf, linGrBrush, penD, x1, x2, x3, x4, y1, y2, y3, y4, y5, bfill);

	for(m_pATPDocCreuset->m_ITRHypotheses.MoveFirst();
		m_pATPDocCreuset->m_ITRHypotheses.IsValid();
		m_pATPDocCreuset->m_ITRHypotheses.MoveNext())
	{
		if(m_pATPDocCreuset->m_IdxHypotheseCourant == m_pATPDocCreuset->m_ITRHypotheses.GetKey())
			continue;
		if(! m_pATPDocCreuset->m_MapAffHypothese.GetState(m_pATPDocCreuset->m_ITRHypotheses.GetKey()))
			continue;

		const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
		const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();
		Color clr0;
		clr0.SetFromCOLORREF(CATPColors::GetColorGrph(m_pATPDocCreuset->m_ITRHypotheses.GetKey()));
		Pen      pen0(clr0);
		// on v�rifie la validit� du jour
		if (i < 0 || i >= DATHypothese.m_NbContactsJour.GetCount())
			return;
		if (i < 0 || i >= DATHypothese.m_DistributionJour.GetCount())
			return;


		// on r�cup�re les couvertures � 1+ de la veille
		JFlt64 A2 = 0.0; 
		JFlt64 A3 = 0.0; 
		JFlt64 A4 = 0.0;

		CalcValues(DATHypoFormat, m_pATPDocCreuset->m_ITRHypotheses.GetKey(),  i, A2, A3, A4);
		
		// on calcule la hauteur de la courbe la veille
		JInt32 y2 = y5 - (JInt32)JMath::Round((A2 * (JFlt64)(y5 - y1)) / m_ValMax);
		JInt32 y3 = y5 - (JInt32)JMath::Round((A3 * (JFlt64)(y5 - y1)) / m_ValMax);
		JInt32 y4 = y5 - (JInt32)JMath::Round((A4 * (JFlt64)(y5 - y1)) / m_ValMax);

		// on teste le dessin � faire
		DrawSection(grf, linGrBrush, pen0, x1, x2, x3, x4, y1, y2, y3, y4, y5, false);


	}

	// on dessine l'axe horizontal
	grf.DrawLine(&penM, Point(x2-1, y5), Point(x4 + 1, y5));


}

void CATPGraphCrb::DrawSection(Graphics& grf, Brush & brush, Pen &pen, JInt32 x1, JInt32 x2, JInt32 x3, JInt32 x4, JInt32 y1, JInt32 y2, JInt32 y3, JInt32 y4, JInt32 y5, bool bfill)
{
	if (y4 < y5)
	{
		// on teste le dessin � faire
		if (y3 < y5)
		{
			// on trace l'aire sous la courbe
			GraphicsPath path;
			path.AddLine(Point(x1, y3), Point(x3, y4));
			path.AddLine(Point(x3, y4), Point(x3, y5 - 1));
			path.AddLine(Point(x3, y5 - 1), Point(x1, y5 - 1));
			path.AddLine(Point(x1, y5 - 1), Point(x1, y3));
			if (bfill)
				grf.FillPath(&brush, &path);

			// on trace la courbe
			grf.DrawLine(&pen, Point(x1, y3), Point(x3, y4));
		}
		else
		{
			// on teste le dessin � faire
			if (y2 < y5)
			{
				// on trace l'aire sous la courbe
				GraphicsPath path;
				path.AddLine(Point(x2, y3), Point(x2, y2));
				path.AddLine(Point(x3, y4), Point(x3, y5 - 1));
				path.AddLine(Point(x3, y5 - 1), Point(x2, y5 - 1));
				path.AddLine(Point(x2, y5 - 1), Point(x2, y3));
				if (bfill)
					grf.FillPath(&brush,&path);

				// on trace la courbe
				grf.DrawLine(&pen, Point(x2, y3), Point(x2, y2));
				grf.DrawLine(&pen, Point(x2, y2), Point(x3, y4));
			}
			else
			{
				// on trace l'aire sous la courbe
				GraphicsPath path;
				path.AddLine(Point(x2, y2), Point(x3, y4));
				path.AddLine(Point(x3, y4), Point(x3, y5 - 1));
				path.AddLine(Point(x3, y5 - 1), Point(x2, y5 - 1));
				path.AddLine(Point(x2, y5 - 1), Point(x2, y2));
				if (bfill)
					grf.FillPath(&brush, &path);
	
				// on trace la courbe
				grf.DrawLine(&pen, Point(x2, y2), Point(x3, y4));
			}
		}

		// on teste si on doit faire le dessin
		/*if (i + 1 == DATHypothese.m_DistributionJour.GetCount())
		{

			// on trace l'aire sous la courbe
			GraphicsPath path;
			path.AddLine(Point(x3, y4), Point(x4, y4));
			path.AddLine(Point(x4, y4), Point(x4, y5 - 1));
			path.AddLine(Point(x4, y5 - 1), Point(x3, y5 - 1));
			path.AddLine(Point(x3, y5 - 1), Point(x3, y4));
			if (bfill)
				grf.FillPath(&brush, &path);

			// on trace la courbe
			grf.DrawLine(&pen, Point(x3, y4), Point(x4, y4));
			grf.DrawLine(&pen, Point(x4, y4), Point(x4, y5));
		}*/		
	}
	else
	{
		// on teste si on doit faire le dessin
		if (y3 < y5)
		{

			// on trace l'aire sous la courbe
			GraphicsPath path;
			path.AddLine(Point(x1, y3), Point(x2, y3));
			path.AddLine(Point(x2, y3), Point(x2, y4));
			path.AddLine(Point(x2, y4), Point(x2, y5 - 1));
			path.AddLine(Point(x2, y5 - 1), Point(x1, y5 - 1));
			path.AddLine(Point(x1, y5 - 1), Point(x1, y3));
			if (bfill)
				grf.FillPath(&brush, &path);

			// on trace la courbe
			grf.DrawLine(&pen, Point(x1, y3), Point(x2, y3));
			grf.DrawLine(&pen, Point(x2, y3), Point(x2, y4));
		}
	}

}


void CATPGraphCrb::OnEndDrawing(CDC & dc)
{
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPGraphCrb::OnUpdate()
{
	// on initialise la dimension
	long cx = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on initialise le Fpu
		JFpu Fpu;

		// on r�cup�re la p�riode d'action
		JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

		// on v�rifie la validit� de la p�riode d'action
		if (DateD.IsValid() && DateF.IsValid())
		{
			// on initialise le maximum
			JFlt64 Max = 0.0;

			// on calcule le nombre d'�l�ments
			cx = (DateF - DateD) + 1;
		}
	}
	this->SetHorzItemRange(0, cx);
}

////////////////////////////////////
// CATPGraphCrb message handlers

void CATPGraphCrb::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCViewport::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(7, FALSE, FALSE);
}

void CATPGraphCrb::OnPaint() 
{
	RECT Rect;

	// on appelle le gestionnaire base
	this->JFCViewport::OnPaint();

	// on r�cup�re la zone cliente de la fen�tre
	this->GetClientRect(&Rect);

	// on r�cup�re les dimensions � dessiner
	LONG Larg = Rect.right  - Rect.left;
	LONG Haut = Rect.bottom - Rect.top;

	// on v�rifie les dimensions de la fen�tre
	if (Larg > 0 && Haut > 0)
	{
		// on r�cup�re un DC sur la fen�tre
		CDC* pDc = this->GetDC();

		// on v�rifie la validit� du DC
		if (pDc != NULL)
		{
			// on cr�e le stylo
			CPen Pen(PS_SOLID, 0, CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

			// on s�lectionne le stylo
			CPen* pOldPen = pDc->SelectObject(&Pen);

			// on trace le trait horizontal
			pDc->MoveTo(Rect.left, Rect.top); pDc->LineTo(Rect.right, Rect.top);

			// on v�rifie les dimensions de la fen�tre
			if (Haut > 1)
			{
				// on trace les traits suivants
				pDc->MoveTo(Rect.left, Rect.bottom - 1); pDc->LineTo(Rect.right - 1, Rect.bottom - 1); pDc->LineTo(Rect.right - 1, Rect.top);
			}

			// on restaure le stylo
			pDc->SelectObject(pOldPen);

			// on lib�re le DC
			this->ReleaseDC(pDc);
		}
	}
}

void CATPGraphCrb::SetMax(JFlt64 max)
{
	m_ValMax = max;
	this->OnUpdate();
}

JFlt64 CATPGraphCrb::GetMax() const
{
	return m_ValMax;
}