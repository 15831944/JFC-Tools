// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JFCGraduation.h"
#include "ATPGraphHisto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphHisto::CATPGraphHisto()
{
	// on initialise les maximums
	m_ValMax = m_GRDMax = 0.0;

	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPGraphHisto::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPGraphHisto::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPGraphHisto::~CATPGraphHisto()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPGraphHisto, JFCViewport)
	//{{AFX_MSG_MAP(CATPGraphHisto)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les �l�ments
/////////////////////////////////////////////////////////////////////////////

bool CATPGraphHisto::OnBeginDrawing(CDC & dc)
{
	// on v�rifie la validit� du document
	if (m_pATPDocCreuset == 0) return (false);

	// on continue le dessin des �l�ments
	return (true);
}

void CATPGraphHisto::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	// on r�cup�re la hauteur maximum des histogrammes
	JInt32 Min = rect.top    + 14;
	JInt32 Max = rect.bottom -  6; if (Min > Max) return;

	Graphics grf(dc);
	Color clr1;
	Color clr2;
	clr1.SetFromCOLORREF(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	clr2.SetFromCOLORREF(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

	
	//clr1.SetFromCOLORREF(CATPColors::GetColorWhite());
	//clr2.SetFromCOLORREF(CATPColors::GetColorGrph(m_pATPDocCreuset->m_IdxHypotheseCourant));

	LinearGradientBrush linGrBrush( Point(0, Max), Point(0, rect.top), clr1, clr2);

	m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);
	if(m_pATPDocCreuset->m_ITRHypotheses.IsValid())
	{
		const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
		const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();
		Color clr;
		clr.SetFromCOLORREF(CATPColors::GetColorDark(CATPColors::GREY_MOUSE));
		Pen      pen(clr);

		if (i < 0 || i >= DATHypothese.m_NbContactsJour.GetCount()) return;

		// on r�cup�re l'effectif et le nombre de contacts
		JDate date = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() + i;

		JFlt64 Value = 0.0;
		CalcValues(DATHypoFormat, i, Value);
		// on calcule la hauteur de l'histogramme
		JInt32 HautG = Max - (JInt32)JMath::Round((Value * (JFlt64)(Max - Min)) / m_GRDMax);

		Rect gdrect(rect.left, HautG, rect.right-rect.left, Max-HautG);
	
		

		
		// on colorie les histogrammes
		if(m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD() <= date && date <= m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF())
			grf.FillRectangle(&linGrBrush, gdrect);


		// on recherche l'interrogation
		for(m_pATPDocCreuset->m_ITRHypotheses.MoveFirst();
			m_pATPDocCreuset->m_ITRHypotheses.IsValid();
			m_pATPDocCreuset->m_ITRHypotheses.MoveNext())
		{
			if(m_pATPDocCreuset->m_IdxHypotheseCourant == m_pATPDocCreuset->m_ITRHypotheses.GetKey())
				continue;

			//m_pATPDocCreuset->m_MapAffHypothese.MoveTo();
			//on teste l'affichage de l'hypothese
			if(! m_pATPDocCreuset->m_MapAffHypothese.GetState(m_pATPDocCreuset->m_ITRHypotheses.GetKey()))
				continue;

			
			// on r�cup�re l'hypoth�se
			const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
			const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();
			//const JITPDATHypothese & DATHypothese = m_pATPDocCreuset->m_ITRHypotheses.GetItem();

			// on v�rifie la validit� du jour
			if (i < 0 || i >= DATHypothese.m_NbContactsJour.GetCount()) return;

			// on r�cup�re l'effectif et le nombre de contacts
			JDate date = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() + i;

			JFlt64 Value = 0.0;
			CalcValues(DATHypoFormat, i, Value);

			// on calcule la hauteur de l'histogramme
			JInt32 HautG = Max - (JInt32)JMath::Round((Value * (JFlt64)(Max - Min)) / m_GRDMax);

			// on colorie les histogrammes
			Rect gdrect0(rect.left, HautG, rect.right-rect.left, Max-HautG);
		
			Color clr0;
			clr0.SetFromCOLORREF(CATPColors::GetColorGrph(m_pATPDocCreuset->m_ITRHypotheses.GetKey()));
			Pen      pen0(clr0);
			grf.DrawRectangle(&pen0, gdrect0);
		}

		grf.DrawRectangle(&pen, gdrect);
		if(m_pATPDocCreuset->m_MapAffHypothese.GetNbAff()>1)
		{
			Color clr3;
			clr3.SetFromCOLORREF(CATPColors::GetColorGrph(m_pATPDocCreuset->m_IdxHypotheseCourant));
			Pen      pen3(clr3);
			grf.DrawLine(&pen3, rect.left, HautG, rect.right, HautG);
		}
		grf.DrawLine(&pen, Point(rect.left, Max), Point(rect.right, Max));
	}
}

void CATPGraphHisto::OnEndDrawing(CDC & dc)
{
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPGraphHisto::OnUpdate()
{
	// on initialise la dimension
	long cx = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on r�cup�re la p�riode d'action
		JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

		// on v�rifie la validit� de la p�riode d'action
		if (DateD.IsValid() && DateF.IsValid())
		{
			// on calcule le nombre d'�l�ments
			cx = (DateF - DateD) + 1;
		}
	}

	// on construit les graduations
	JFCGraduation Grad; Grad.SetRange(0.0, m_ValMax);

	// on m�morise les valeurs
	m_GRDMax = Grad.GetMax();
	// on met � jour le nombre d'histogrammes
	this->SetHorzItemRange(0, cx);
}

////////////////////////////////////
// CATPGraphHisto message handlers

void CATPGraphHisto::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCViewport::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(7, FALSE, FALSE);
}

void CATPGraphHisto::OnPaint() 
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

void CATPGraphHisto::SetMax(JFlt64 max)
{
	m_ValMax = max;
	this->OnUpdate();
}

JFlt64 CATPGraphHisto::GetMax() const
{
	return m_ValMax;
}
