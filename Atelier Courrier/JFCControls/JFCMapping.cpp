// JFCMapping.cpp : implementation file
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCMapping.h"
#include "JFCGraduation.h"
#include "TendCurve.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// les identificateurs des menus
const LONG JFCMapping::IdShowAllLibelle = 10400;
const LONG JFCMapping::IdHideAllLibelle = 10401;
const LONG JFCMapping::IdShowSelLibelle = 10402;
const LONG JFCMapping::IdHideSelLibelle = 10403;
const LONG JFCMapping::IdShowSelItem    = 10404;
const LONG JFCMapping::IdHideSelItem    = 10405;
const LONG JFCMapping::IdShowAllItem    = 10406;
const LONG JFCMapping::IdAllSelect      = 10407;
const LONG JFCMapping::IdInvSelect      = 10408;
const LONG JFCMapping::IdNoSelect       = 10409;
const LONG JFCMapping::IdCreateCourbeD1 = 10410;
const LONG JFCMapping::IdCreateCourbeD2 = 10411;
const LONG JFCMapping::IdHideAllCourbe	= 10412;
const LONG JFCMapping::IdHideCourbeSel	= 10413;
const LONG JFCMapping::IdShowLibCourbe	= 10414;
const LONG JFCMapping::IdHideLibCourbe	= 10415;

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCMapping::JFCMapping()
{
	// on initialise les éléments
	m_pIMapping = NULL;
	m_pICurve   = NULL;
	
	// on met à jour les grandeurs
	this->OnUpdateLongueurs(); this->OnUpdateGrandeurs();

	// on initialise le tracking
	m_Track = 0; m_TrkIdx = m_TrkPar1 = m_TrkPar2 = m_TrkPar3 = m_TrkPar4 = 0;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour initialiser les éléments
/////////////////////////////////////////////////////////////////////////////

JFCIMapping* JFCMapping::SetItems(JFCIMapping* pIMapping, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on mémorise les nouveaux éléments
	JFCIMapping* pOldIMapping = m_pIMapping; m_pIMapping = pIMapping;

	// on met à jour les longueurs et les grandeurs
	this->OnUpdateLongueurs(); this->OnUpdateGrandeurs();

	// Récup infos coordonnées courbe
	bool De1 = OnGetCourbeDegr1();
	bool De2 = OnGetCourbeDegr2();
	
	// on redessine le mapping
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);

	// on renvoie les anciens éléments
	return (pOldIMapping);
}
/////////////////////////////////////////////////////////////////////////////
// la fonction pour initialiser les courbes
/////////////////////////////////////////////////////////////////////////////

JFCICurve* JFCMapping::SetCurve(JFCICurve* pICurve, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on mémorise les nouveaux éléments
	JFCICurve* pOldICurve = m_pICurve; m_pICurve = pICurve;

	// on met à jour les longueurs et les grandeurs
	this->OnUpdateLongueurs(); this->OnUpdateGrandeurs();

	// on redessine le mapping
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);

	// on renvoie les anciens éléments
	return (pOldICurve);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour supprimer les éléments
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::Reset(BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on réinitialise les éléments
	m_pIMapping = NULL; m_pICurve = NULL; this->OnUpdateLongueurs(); this->OnUpdateGrandeurs();
	
	// on redessine le mapping
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour exporter le dessin vers un MetaFile
/////////////////////////////////////////////////////////////////////////////

BOOL JFCMapping::ExportItems(long VSize, const TCHAR* pFilename)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on vérifie la validité des éléments
	if(m_pIMapping == NULL) return FALSE;

	// on récupère le contexte de périphérique
	CDC* pDC = this->GetDC();
	if(pDC == NULL) return FALSE;

	CMetaFileDC MetaDC; // création du MetaFile
	if(!MetaDC.CreateEnhanced(pDC, pFilename, NULL, NULL)) return FALSE;

	// récupération des attributs d'origine
	MetaDC.SetAttribDC(pDC->m_hAttribDC);

	// initialisation des outils de dessin
	if(!this->OnBeginDrawing(MetaDC))
	{
		MetaDC.ReleaseAttribDC();
		MetaDC.CloseEnhanced();
		return FALSE;
	}


	double HRes = (double)MetaDC.GetDeviceCaps(HORZSIZE) * (double)MetaDC.GetDeviceCaps(VERTRES);
	double VRes = (double)MetaDC.GetDeviceCaps(VERTSIZE) * (double)MetaDC.GetDeviceCaps(HORZRES);
	double Ratio = VRes / HRes;
	long HSize = (long)(((double)VSize * Ratio) + .5);
	
	// Dessin des axes
	this->DrawAxes(MetaDC, HSize, VSize, true);

	// Dessin des courbes
	this->DrawCurves(MetaDC, HSize, VSize);
	
	// Dessin liaison courbe et libellé équation
	this->DrawTraitEquation(MetaDC, HSize, VSize);

	// Les points du mapping
	this->DrawLosanges(MetaDC, HSize, VSize);

	// Dessin lien point et libellé
	this->DrawTraitLib(MetaDC, HSize, VSize);

	// Libellés point du mapping
	this->DrawLibelles(MetaDC, HSize, VSize);

	// Libelles des equations
	this->DrawLibEquation(MetaDC, HSize, VSize);
		
	// libération des outils
	this->OnEndDrawing(MetaDC); 

	MetaDC.ReleaseAttribDC();

	// fermeture du fichier, on garde un handle pour la copie éventuelle dans le Presse-Papiers
	HENHMETAFILE hMetaFile = MetaDC.CloseEnhanced();
	if(hMetaFile == NULL) return FALSE;

	// si un nom de fichier a été spécifié on termine ici avec succès
	if(pFilename == NULL)
	{
		// ouverture du presse-papiers
		if(!OpenClipboard()) return FALSE;

		// vidange du presse-papiers
		if(!EmptyClipboard())
		{
			CloseClipboard();
			return FALSE;
		}
		// envoie les données du MetaFile dans le presse-papiers
		if(SetClipboardData(CF_ENHMETAFILE, hMetaFile) == NULL)
		{
			CloseClipboard();
			return FALSE;
		}
		CloseClipboard();
	}
	else
	{
		// on supprime le métafile
		if (FALSE == ::DeleteEnhMetaFile(hMetaFile)) return (FALSE);
	}
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCMapping::~JFCMapping()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCMapping, JFCView)
	//{{AFX_MSG_MAP(JFCMapping)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND(JFCMapping::IdShowAllLibelle, OnShowAllLibelle)
	ON_COMMAND(JFCMapping::IdHideAllLibelle, OnHideAllLibelle)
	ON_COMMAND(JFCMapping::IdShowSelLibelle, OnShowSelLibelle)
	ON_COMMAND(JFCMapping::IdHideSelLibelle, OnHideSelLibelle)
	ON_COMMAND(JFCMapping::IdShowSelItem,    OnShowSelItem)
	ON_COMMAND(JFCMapping::IdHideSelItem,    OnHideSelItem)
	ON_COMMAND(JFCMapping::IdShowAllItem,    OnShowAllItem)
	ON_COMMAND(JFCMapping::IdAllSelect,      OnAllSelect)
	ON_COMMAND(JFCMapping::IdInvSelect,      OnInvSelect)
	ON_COMMAND(JFCMapping::IdNoSelect,       OnNoSelect)
	ON_COMMAND(JFCMapping::IdNoSelect,		 OnNoSelect)
	ON_COMMAND(JFCMapping::IdCreateCourbeD1, OnCreateCourbeD1)
	ON_COMMAND(JFCMapping::IdCreateCourbeD2, OnCreateCourbeD2)
	ON_COMMAND(JFCMapping::IdHideAllCourbe,	 OnHideAllCourbe)
	ON_COMMAND(JFCMapping::IdHideCourbeSel,	 OnHideCourbeSel)
	ON_COMMAND(JFCMapping::IdShowLibCourbe,	 OnShowLibCourbe)
	ON_COMMAND(JFCMapping::IdHideLibCourbe,	 OnHideLibCourbe)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCMapping::OnBeginDrawing(CDC & dc)
{
	// on vérifie la validité des éléments
	if (m_pIMapping == NULL) return (false);
	if (m_pICurve   == NULL) return (false);
	LONG A_FAIRE;

	// on récupère le résolution de DC
	LONG Reso = dc.GetDeviceCaps(LOGPIXELSY);

	// on crée les fontes
	m_FontP.CreateFont(-MulDiv( 7, Reso, 72), 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	m_FontM.CreateFont(-MulDiv( 9, Reso, 72), 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "MS Shell Dlg");
	m_FontG.CreateFont(-MulDiv(11, Reso, 72), 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "MS Shell Dlg");

	// on crée les stylos
	m_PenG    .CreatePen(PS_SOLID, 0, RGB(128,128,128));
	m_PenB    .CreatePen(PS_SOLID, 0, RGB(  0,  0,  0));
	m_PenR    .CreatePen(PS_SOLID, 0, RGB(255,  0,  0));
	m_PenBlue .CreatePen(PS_SOLID, 0, RGB(  0,  0,192));
	m_PenGreen.CreatePen(PS_SOLID, 0, RGB(  0,192,  0));
	m_PenSky  .CreatePen(PS_SOLID, 0, RGB(  0,192,192));
	m_PenUmbre.CreatePen(PS_SOLID, 0, RGB(192,  0,192));
	m_PenOrang.CreatePen(PS_SOLID, 0, RGB(255,160,  0));

	// on crée les pinceaux
	m_BrushG    .CreateSolidBrush(RGB(192,255,192));
	m_BrushY    .CreateSolidBrush(RGB(255,255,192));
	m_BrushB    .CreateSolidBrush(RGB(  0,  0,255));
	m_BrushBlue .CreateSolidBrush(RGB(128,192,255));
	m_BrushGreen.CreateSolidBrush(RGB(192,255,192));
	m_BrushSky  .CreateSolidBrush(RGB(192,255,255));
	m_BrushUmbre.CreateSolidBrush(RGB(255,192,255));
	m_BrushOrang.CreateSolidBrush(RGB(255,192, 96));

	// on continue le dessin des éléments
	return (true);
}

void JFCMapping::OnDrawItems(CDC & dc, RECT rect)
{
	// on dessine les axes
	this->OnDrawAxes(dc, rect);

	// on dessine les courbes
	this->OnDrawCurves(dc, rect);

	// on dessine les points
	this->OnDrawLosanges(dc, rect);

	// on dessine les traits
	this->OnDrawTraitLib(dc, rect);
	this->OnDrawTraitEquation(dc, rect);

	// on dessine les libellés
	this->OnDrawLibelles(dc, rect);

	// on dessine les equations
	this->OnDrawLibEquation(dc, rect);
}

void JFCMapping::OnEndDrawing(CDC & dc)
{
	// on détruit les pinceaux
	m_BrushY    .DeleteObject();
	m_BrushG    .DeleteObject();
	m_BrushB    .DeleteObject();
	m_BrushBlue .DeleteObject();
	m_BrushGreen.DeleteObject();
	m_BrushSky  .DeleteObject();
	m_BrushUmbre.DeleteObject();
	m_BrushOrang.DeleteObject();

	// on détruit les stylos
	m_PenR    .DeleteObject();
	m_PenB    .DeleteObject();
	m_PenG    .DeleteObject();
	m_PenBlue .DeleteObject();
	m_PenGreen.DeleteObject();
	m_PenSky  .DeleteObject();
	m_PenUmbre.DeleteObject();
	m_PenOrang.DeleteObject();

	// on détruit les fontes
	m_FontG.DeleteObject();
	m_FontM.DeleteObject();
	m_FontP.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour calculer la position des éléments
/////////////////////////////////////////////////////////////////////////////

long JFCMapping::OnGetXPosition(double value, long HSize)
{
	// on teste la dimension
	if (HSize <= 0) return (0);
	
	// on vérifie la validité de l'intervalle
	if (m_XGrdMin == m_XGrdMax) return (HSize / 2);

	// on calcule la position
	long Pos = (long)floor((((value - m_XGrdMin) * (double)HSize) / (m_XGrdMax - m_XGrdMin)) + 0.5);

	// on renvoie la position
	return (Pos);
}

long JFCMapping::OnGetYPosition(double value, long VSize)
{
	// on teste la dimension
	if (VSize <= 0) return (0);
	
	// on vérifie la validité du calcul
	if (m_YGrdMin == m_YGrdMax) return (VSize / 2);

	// on calcule la position
	long Pos = (LONG)floor((((value - m_YGrdMin) * (double)VSize) / (m_YGrdMax - m_YGrdMin)) + 0.5);

	// on renvoie la position
	return (VSize - Pos - 1);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour modifier le déplacement des libellés
/////////////////////////////////////////////////////////////////////////////

float JFCMapping::OnSetDxLibelle(long value)
{
	// on récupère la dimension
	long HSize = this->GetHorzSize(); if (HSize <= 0) return (0.0);

	// on calcule le déplacement
	float Depl = (float)value / (float)HSize;

	// on renvoie le déplacement
	return (Depl);
}

float JFCMapping::OnSetDyLibelle(long value)
{
	// on récupère la dimension
	long VSize = this->GetVertSize(); if (VSize <= 0) return (0.0);

	// on calcule le déplacement
	float Depl = (float)value / (float)VSize;

	// on renvoie le déplacement
	return (Depl);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour calculer le déplacement des libellés
/////////////////////////////////////////////////////////////////////////////

long JFCMapping::OnGetDxLibelle(float value, long HSize)
{
	// on teste la dimension
	if (HSize <= 0) return (0);

	// on calcule le déplacement
	long Depl = (long)floor(((double)HSize * (double)value) + 0.5);

	// on renvoie le déplacement
	return (Depl);
}

long JFCMapping::OnGetDyLibelle(float value, long VSize)
{
	// on teste la dimension
	if (VSize <= 0) return (0);

	// on calcule le déplacement
	long Depl = (long)floor(((double)VSize * (double)value) + 0.5);

	// on renvoie le déplacement
	return (Depl);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions de recherche des losanges et des libellés
/////////////////////////////////////////////////////////////////////////////

long JFCMapping::OnGetLosangeAtPoint(LONG x, LONG y)
{
	// on initialise l'indice
	long idx = -1;

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = taille - 1; idx < 0 && indice >= 0; indice -= 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on calcule la position de l'élément
			long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(indice), this->GetHorzSize());
			long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(indice), this->GetVertSize());

			// on teste si le point est dans le losange
			if      (x == PosX - 2 && y == PosY) idx = indice;
			else if (x == PosX - 1 && y >= PosY - 1 && y <= PosY + 1) idx = indice;
			else if (x == PosX     && y >= PosY - 2 && y <= PosY + 2) idx = indice;
			else if (x == PosX + 1 && y >= PosY - 1 && y <= PosY + 1) idx = indice;
			else if (x == PosX + 2 && y == PosY) idx = indice;
		}
	}

	// on renvoie l'indice du libellé
	return (idx);
}

long JFCMapping::OnGetLibelleAtPoint(LONG x, LONG y)
{
	// on initialise l'indice
	long idx = -1;

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = taille - 1; idx < 0 && indice >= 0; indice -= 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on vérifie la visibilité du libellé
			if (m_pIMapping->GetShowLabel(indice))
			{
				// on récupère la position du libellé
				long LbDx = this->OnGetDxLibelle(m_pIMapping->GetDxLabel(indice), this->GetHorzSize());
				long LbDy = this->OnGetDyLibelle(m_pIMapping->GetDyLabel(indice), this->GetVertSize());

				// on teste si libellé visible
				if (m_LbHg > 0 && m_LbLg[indice] > 0)
				{
					// on calcule la position de l'élément
					long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(indice), this->GetHorzSize());
					long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(indice), this->GetVertSize());

					// on corrige la position initiale
					PosX -= (m_LbLg[indice] / 2) - LbDx; PosY -= (m_LbHg / 2) - LbDy;

					// on teste si le point est dans le libellé
					if (x >= PosX && y >= PosY && x <= PosX + m_LbLg[indice] && y <= PosY + m_LbHg) idx = indice;
				}
			}
		}
	}

	// on renvoie l'indice du libellé
	return (idx);
}

long JFCMapping::OnGetLibEquationAtPoint(LONG x, LONG y)
{
	// on initialise l'indice
	long idx = -1;

	// on récupère le nombre d'éléments
	if (m_pICurve)
	{
		long tailleMax = m_pICurve->GetNbCourbeMax();

		// on parcourt tous les éléments
		for (long indice = tailleMax - 1; idx < 0 && indice >= 0; indice -= 1)
		{
			// on vérifie la validité et la visibilité de l'élément
			if (m_pICurve->GetExistCb(indice) == true)
			{
				// on vérifie la visibilité du libellé
				if (m_pICurve->GetShowLibelle(indice))
				{
					// on récupère la position du libellé
					long LbEqDx = this->OnGetDxLibelle(m_pICurve->GetDxLibelle(indice), this->GetHorzSize());
					long LbEqDy = this->OnGetDyLibelle(m_pICurve->GetDyLibelle(indice), this->GetVertSize());

					// on teste si on a quelque chose à tester
					if (m_LbEqHg > 0 && m_LbEqLg[indice] > 0)
					{
						// on recupere l'equation de la courbe
						double a, b, c;
						m_pICurve->GetEquation(indice, a, b, c);

						// on calcule la position d'ancrage du trait sur le libelle
						long PosX = this->OnGetXPosition(m_BarycentreX[indice], this->GetHorzSize());
						long PosY = this->OnGetYPosition(m_BarycentreY[indice], this->GetVertSize());

						// on corrige la position initiale
						PosX -= (m_LbEqLg[indice] / 2) - LbEqDx; 
						PosY -= (m_LbEqHg         / 2) - LbEqDy;

						// on teste si le point est dans le libellé
						if (x >= PosX && y >= PosY && x <= PosX + m_LbEqLg[indice] && y <= PosY + m_LbEqHg) idx = indice;
					}
				}
			}
		}
	}

	// on renvoie l'indice du libellé
	return (idx);
}

long JFCMapping::OnGetCourbeAtPoint(LONG PTX, LONG PTY)
{
	// on initialise le flag
	long idx = -1;	
	
	// on cree le rectangle de confiance du point
	long X1 = PTX - 2; long X2 = PTX + 2;
	long Y1 = PTY - 2; long Y2 = PTY + 2;

	// on recupere les bornes en abscisse et en ordonnees du mapping
	long XMIN = this->OnGetXPosition(m_XMin, this->GetHorzSize()); long XMAX = this->OnGetXPosition(m_XMax, this->GetHorzSize());
	long YMAX = this->OnGetYPosition(m_YMin, this->GetVertSize()); long YMIN = this->OnGetYPosition(m_YMax, this->GetVertSize());

	// on teste si ces bornes ne sont pas confondues
	if (XMIN == XMAX || YMIN == YMAX) return (idx);

	// on determine l'abscisse et l'ordonnée reel du point et son intervalle de confiance
	double dx1 = m_XMin + (double)(X1 - XMIN) * (m_XMax - m_XMin) / (double)(XMAX - XMIN);
	double dx2 = m_XMin + (double)(X2 - XMIN) * (m_XMax - m_XMin) / (double)(XMAX - XMIN);
	double dy1 = m_YMax - (double)(Y2 - YMIN) * (m_YMax - m_YMin) / (double)(YMAX - YMIN);
	double dy2 = m_YMax - (double)(Y1 - YMIN) * (m_YMax - m_YMin) / (double)(YMAX - YMIN);

	// on récupère le nombre de courbes max
	long tailleMax = m_pICurve->GetNbCourbeMax();
	
	// on parcourt toutes les courbes
	for (long indice = tailleMax - 1; idx < 0 && indice >= 0; indice -= 1)
	{
		// on vérifie la validité de la courbe
		if (m_pICurve->GetExistCb(indice))
		{
			// on initialise les coef de la courbe
			double a, b, c;

			// on les recupere
			m_pICurve->GetEquation(indice, a, b, c);

			// on recupere l'ordonnée du point et son intervalle de confiance
			double y2 = (a * dx2 * dx2) + (b * dx2) + c;
			double y1 = (a * dx1 * dx1) + (b * dx1) + c;
			
			// on teste si la courbe n'est pas monotone
			if (a != 0.0)
			{									
				// on determine le determinant pour dy1
				double deltaY1 = (b * b) - (4.0 * a * (c - dy1));
	
				// on determine le determinant pour dy2
				double deltaY2 = (b * b) - (4.0 * a * (c - dy2));

				// on teste le resultat pour dy1
				if (deltaY1 < 0.0 || deltaY2 < 0.0)
				{
					// on teste si la courbe coupe le segment 
					if(( (dy1 <=   y1) && (  y1 <= dy2)) ||
					   ( (dy1 <=   y2) && (  y2 <= dy2)))
					{
						idx = indice;
					}
				}
				else if (deltaY1 == 0.0 || deltaY2 == 0.0)
				{
					double x = (-b) / (2.0*a);

					// on teste si la courbe coupe le segment 
					if(( (dy1 <=   y1) && (  y1 <= dy2)) ||
					   ( (dy1 <=   y2) && (  y2 <= dy2)) ||
					   ( (dx1 <=    x) && (   x <= dx2)))
					{
						idx = indice;
					}
				}
				else if (deltaY1 > 0.0)
				{
					double sqrtDY1 = ::sqrt(deltaY1);
					double xa1 = ((-b - sqrtDY1) / (2.0 * a));
					double xb1 = ((-b + sqrtDY1) / (2.0 * a));

					// on teste si la courbe coupe le segment 
					if(( (dy1 <=   y1) && (  y1 <= dy2)) ||
					   ( (dy1 <=   y2) && (  y2 <= dy2)) ||
					   ( (dx1 <=  xa1) && ( xa1 <= dx2)) ||
					   ( (dx1 <=  xb1) && ( xb1 <= dx2)))
					{	
						idx = indice;
					}
				}
				else if (deltaY2 > 0.0)
				{
					double sqrtDY2 = ::sqrt(deltaY2);
					double xa2 = ((-b - sqrtDY2) / (2.0 * a));
					double xb2 = ((-b + sqrtDY2) / (2.0 * a));

					// on teste si la courbe coupe le segment 
					if(( (dy1 <=   y1) && (  y1 <= dy2)) ||
					   ( (dy1 <=   y2) && (  y2 <= dy2)) ||
					   ( (dx1 <=  xa2) && ( xa2 <= dx2)) ||
					   ( (dx1 <=  xb2) && ( xb2 <= dx2)))
					{	
						idx = indice;
					}
				}
			}
			else
			{	
				// on determine x1 et x2
				double x1 = (dy1 - c) / b;
				double x2 = (dy2 - c) / b;

				// on teste si la courbe est dans ce rectangle de confiance
				if(((dy1 <= y1) && (y1 <=  dy2)) ||
				   ((dy1 <= y2) && (y2 <=  dy2)) ||
				   ((dx1 <= x1) && (x1 <=  dx2)) ||
				   ((dx1 <= x2) && (x2 <=  dx2)))
				{	
					idx = indice;
				}
			}
		}
	}

	// on renvoie l'indice de la courbe
	return (idx);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner le losange et le libellé de tracking
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::OnTrackLosange(long mode, long indice, long x, long y)
{
	// on initialise les positions
	LONG x1, x2, y1, y2;

	// on initialise le stylo
	CPen Pen(PS_SOLID, 0, RGB(255,255,255));

	// on récupère le DC de la fenêtre
	CDC* pDc = this->GetDC();

	// on modifie le mode de tracé
	long OldRop = pDc->SetROP2(R2_XORPEN);

	// on modifie le stylo du tracé
	CPen* pOldPen = pDc->SelectObject(&Pen);

	// on teste le mode de tracé
	if (mode == 1)
	{
		// on mémorise la position initialise
		m_TrkPar1 = m_TrkPar3 = x;
		m_TrkPar2 = m_TrkPar4 = y;

		// on récupère les positions
		this->GetTrackPos(m_TrkPar1, m_TrkPar2, x1, y1);
		this->GetTrackPos(m_TrkPar3, m_TrkPar4, x2, y2);

		// on dessine le rectangle
		pDc->MoveTo(x1, y1);
		pDc->LineTo(x2, y1); pDc->LineTo(x2, y2);
		pDc->LineTo(x1, y2); pDc->LineTo(x1, y1);
	}
	else if (mode == 2)
	{
		// on récupère les positions
		this->GetTrackPos(m_TrkPar1, m_TrkPar2, x1, y1);
		this->GetTrackPos(m_TrkPar3, m_TrkPar4, x2, y2);

		// on dessine le rectangle
		pDc->MoveTo(x1, y1);
		pDc->LineTo(x2, y1); pDc->LineTo(x2, y2);
		pDc->LineTo(x1, y2); pDc->LineTo(x1, y1);
	}
	else if (mode == 3)
	{
		// on teste si on a quelque chose à faire
		if (x != m_TrkPar3 || y != m_TrkPar4)
		{
			// on récupère les positions
			this->GetTrackPos(m_TrkPar1, m_TrkPar2, x1, y1);
			this->GetTrackPos(m_TrkPar3, m_TrkPar4, x2, y2);

			// on efface le rectangle
			pDc->MoveTo(x1, y1);
			pDc->LineTo(x2, y1); pDc->LineTo(x2, y2);
			pDc->LineTo(x1, y2); pDc->LineTo(x1, y1);

			// on mémorise la position
			m_TrkPar3 = x;	m_TrkPar4 = y;

			// on récupère les positions
			this->GetTrackPos(m_TrkPar3, m_TrkPar4, x2, y2);

			// on dessine le rectangle
			pDc->MoveTo(x1, y1);
			pDc->LineTo(x2, y1); pDc->LineTo(x2, y2);
			pDc->LineTo(x1, y2); pDc->LineTo(x1, y1);
		}
	}
	else if (mode == 4)
	{
		// on récupère les positions
		this->GetTrackPos(m_TrkPar1, m_TrkPar2, x1, y1);
		this->GetTrackPos(m_TrkPar3, m_TrkPar4, x2, y2);

		// on supprime le rectangle
		pDc->MoveTo(x1, y1);
		pDc->LineTo(x2, y1); pDc->LineTo(x2, y2);
		pDc->LineTo(x1, y2); pDc->LineTo(x1, y1);
	}

	// on restaure le stylo du tracé
	pDc->SelectObject(pOldPen);

	// on restaure le mode de tracé
	pDc->SetROP2(OldRop);

	// on libère le DC
	this->ReleaseDC(pDc);
}

void JFCMapping::OnTrackLibelle(long mode, long indice, long x, long y)
{
	// on initialise les positions
	long x1, y1, x2, y2;

	// on initialise le stylo
	CPen Pen(PS_SOLID, 0, RGB(128,128,128));

	// on récupère le DC de la fenêtre
	CDC* pDc = this->GetDC();

	// on modifie le mode de tracé
	long OldRop = pDc->SetROP2(R2_XORPEN);

	// on modifie le stylo du tracé
	CPen* pOldPen = pDc->SelectObject(&Pen);

	// on teste le mode
	if (mode == 1)
	{
		// on mémorise l'indice
		m_TrkIdx = indice;

		// on récupère la position du libellé
		m_TrkPar1 = this->OnGetDxLibelle(m_pIMapping->GetDxLabel(m_TrkIdx), this->GetHorzSize());
		m_TrkPar2 = this->OnGetDyLibelle(m_pIMapping->GetDyLabel(m_TrkIdx), this->GetVertSize());

		// on récupère la position de l'élément
		long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(m_TrkIdx), this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(m_TrkIdx), this->GetVertSize());

		// on calcule les offsets
		m_TrkPar3 = x - (PosX + m_TrkPar1);
		m_TrkPar4 = y - (PosY + m_TrkPar2);

		// on calcule la position
		x1 = PosX + m_TrkPar1 - (m_LbLg[m_TrkIdx] / 2);
		y1 = PosY + m_TrkPar2 - (m_LbHg / 2);

		// on récupère les positions
		this->GetTrackPos(x1, y1, x2, y2);

		// on dessine le rectangle
		pDc->MoveTo(x2, y2);
		pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2);
		pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2 + m_LbHg);
		pDc->LineTo(x2, y2 + m_LbHg);
		pDc->LineTo(x2, y2);
	}
	else if (mode == 2)
	{
		// on récupère la position de l'élément
		long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(m_TrkIdx), this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(m_TrkIdx), this->GetVertSize());

		// on calcule la position
		x1 = PosX + m_TrkPar1 - (m_LbLg[m_TrkIdx] / 2);
		y1 = PosY + m_TrkPar2 - (m_LbHg / 2);

		// on récupère les positions
		this->GetTrackPos(x1, y1, x2, y2);

		// on affiche le rectangle
		pDc->MoveTo(x2, y2);
		pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2);
		pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2 + m_LbHg);
		pDc->LineTo(x2, y2 + m_LbHg);
		pDc->LineTo(x2, y2);
	}
	else if (mode == 3)
	{
		// on récupère la position de l'élément
		long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(m_TrkIdx), this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(m_TrkIdx), this->GetVertSize());

		// on calcule la nouvelle position
		long TrkLbDx = x - PosX - m_TrkPar3;
		long TrkLbDy = y - PosY - m_TrkPar4;

		// on teste si on a quelque chose à faire
		if (m_TrkPar1 != TrkLbDx || m_TrkPar2 != TrkLbDy)
		{
			// on calcule la position
			x1 = PosX + m_TrkPar1 - (m_LbLg[m_TrkIdx] / 2);
			y1 = PosY + m_TrkPar2 - (m_LbHg / 2);

			// on récupère les positions
			this->GetTrackPos(x1, y1, x2, y2);

			// on supprime le rectangle
			pDc->MoveTo(x2, y2);
			pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2);
			pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2 + m_LbHg);
			pDc->LineTo(x2, y2 + m_LbHg);
			pDc->LineTo(x2, y2);

			// on modifie la position courante
			m_TrkPar1 = TrkLbDx; m_TrkPar2 = TrkLbDy;

			// on calcule la position
			x1 = PosX + m_TrkPar1 - (m_LbLg[m_TrkIdx] / 2);
			y1 = PosY + m_TrkPar2 - (m_LbHg / 2);

			// on récupère les positions
			this->GetTrackPos(x1, y1, x2, y2);

			// on dessine le rectangle
			pDc->MoveTo(x2, y2);
			pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2);
			pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2 + m_LbHg);
			pDc->LineTo(x2, y2 + m_LbHg);
			pDc->LineTo(x2, y2);
		}
	}
	else if (mode == 4)
	{
		// on récupère la position de l'élément
		long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(m_TrkIdx), this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(m_TrkIdx), this->GetVertSize());

		// on calcule la position
		x1 = PosX + m_TrkPar1 - (m_LbLg[m_TrkIdx] / 2);
		y1 = PosY + m_TrkPar2 - (m_LbHg / 2);

		// on récupère les positions
		this->GetTrackPos(x1, y1, x2, y2);

		// on supprime le rectangle
		pDc->MoveTo(x2, y2);
		pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2);
		pDc->LineTo(x2 + m_LbLg[m_TrkIdx], y2 + m_LbHg);
		pDc->LineTo(x2, y2 + m_LbHg);
		pDc->LineTo(x2, y2);
	}

	// on restaure le stylo du tracé
	pDc->SelectObject(pOldPen);

	// on restaure le mode de tracé
	pDc->SetROP2(OldRop);

	// on libère le DC
	this->ReleaseDC(pDc);
}

void JFCMapping::OnTrackLibEquation(long mode, long indice, long x, long y)
{
	// on initialise les positions
	long x1, y1, x2, y2;

	// on initialise le stylo
	CPen Pen(PS_SOLID, 0, RGB(128,128,128));

	// on récupère le DC de la fenêtre
	CDC* pDc = this->GetDC();

	// on modifie le mode de tracé
	long OldRop = pDc->SetROP2(R2_XORPEN);

	// on modifie le stylo du tracé
	CPen* pOldPen = pDc->SelectObject(&Pen);

	// on teste le mode
	if (mode == 1)
	{
		// on mémorise l'indice
		m_TrkIdx = indice;
	
		// on recupere les coefs de la courbe
		double a, b, c; m_pICurve->GetEquation(m_TrkIdx, a, b, c);

		// on récupère la position du libellé
		m_TrkPar1 = this->OnGetDxLibelle(m_pICurve->GetDxLibelle(m_TrkIdx), this->GetHorzSize());
		m_TrkPar2 = this->OnGetDyLibelle(m_pICurve->GetDyLibelle(m_TrkIdx), this->GetVertSize());
		
		// on calcule la position d'ancrage du trait sur le libelle
		long PosX = this->OnGetXPosition(m_BarycentreX[m_TrkIdx], this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_BarycentreY[m_TrkIdx], this->GetVertSize());

		// on calcule les offsets
		m_TrkPar3 = x - (PosX + m_TrkPar1);
		m_TrkPar4 = y - (PosY + m_TrkPar2);

		// on calcule la position
		x1 = PosX + m_TrkPar1 - (m_LbEqLg[m_TrkIdx] / 2);
		y1 = PosY + m_TrkPar2 - (m_LbEqHg / 2);
		
		// on récupère les positions
		this->GetTrackPos(x1, y1, x2, y2);

		// on dessine le rectangle
		pDc->MoveTo(x2, y2);
		pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2);
		pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2 + m_LbEqHg);
		pDc->LineTo(x2, y2 + m_LbEqHg);
		pDc->LineTo(x2, y2);
	}
	else if (mode == 2)
	{
		// on recupere les coefs de la courbe
		double a, b, c; m_pICurve->GetEquation(m_TrkIdx, a, b, c);

		// on calcule la position d'ancrage du trait sur le libelle
		long PosX = this->OnGetXPosition(m_BarycentreX[m_TrkIdx], this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_BarycentreY[m_TrkIdx], this->GetVertSize());

		// on calcule la position
		x1 = PosX + m_TrkPar1 - (m_LbEqLg[m_TrkIdx] / 2);
		y1 = PosY + m_TrkPar2 - (m_LbEqHg / 2);

		// on récupère les positions
		this->GetTrackPos(x1, y1, x2, y2);

		// on affiche le rectangle
		pDc->MoveTo(x2, y2);
		pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2);
		pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2 + m_LbEqHg);
		pDc->LineTo(x2, y2 + m_LbEqHg);
		pDc->LineTo(x2, y2);
	}
	else if (mode == 3)
	{
		// on recupere les coefs de la courbe
		double a, b, c; m_pICurve->GetEquation(m_TrkIdx, a, b, c);

		// on calcule la position d'ancrage du trait sur le libelle
		long PosX = this->OnGetXPosition(m_BarycentreX[m_TrkIdx], this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_BarycentreY[m_TrkIdx], this->GetVertSize());

		// on calcule la nouvelle position
		long TrkLbDx = x - PosX - m_TrkPar3;
		long TrkLbDy = y - PosY - m_TrkPar4;

		// on teste si on a quelque chose à faire
		if (m_TrkPar1 != TrkLbDx || m_TrkPar2 != TrkLbDy)
		{
			// on calcule la position
			x1 = PosX + m_TrkPar1 - (m_LbEqLg[m_TrkIdx] / 2);
			y1 = PosY + m_TrkPar2 - (m_LbEqHg / 2);

			// on récupère les positions
			this->GetTrackPos(x1, y1, x2, y2);

			// on supprime le rectangle
			pDc->MoveTo(x2, y2);
			pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2);
			pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2 + m_LbEqHg);
			pDc->LineTo(x2, y2 + m_LbEqHg);
			pDc->LineTo(x2, y2);

			// on modifie la position courante
			m_TrkPar1 = TrkLbDx; m_TrkPar2 = TrkLbDy;

			// on calcule la position
			x1 = PosX + m_TrkPar1 - (m_LbEqLg[m_TrkIdx] / 2);
			y1 = PosY + m_TrkPar2 - (m_LbEqHg / 2);

			// on récupère les positions
			this->GetTrackPos(x1, y1, x2, y2);

			// on dessine le rectangle
			pDc->MoveTo(x2, y2);
			pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2);
			pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2 + m_LbEqHg);
			pDc->LineTo(x2, y2 + m_LbEqHg);
			pDc->LineTo(x2, y2);
		}
	}
	else if (mode == 4)
	{
		// on recupere les coefs de la courbe
		double a, b, c; m_pICurve->GetEquation(m_TrkIdx, a, b, c);

		// on calcule la position d'ancrage du trait sur le libelle
		long PosX = this->OnGetXPosition(m_BarycentreX[m_TrkIdx], this->GetHorzSize());
		long PosY = this->OnGetYPosition(m_BarycentreY[m_TrkIdx], this->GetVertSize());

		// on calcule la position
		x1 = PosX + m_TrkPar1 - (m_LbEqLg[m_TrkIdx] / 2);
		y1 = PosY + m_TrkPar2 - (m_LbEqHg / 2);

		// on récupère les positions
		this->GetTrackPos(x1, y1, x2, y2);

		// on supprime le rectangle
		pDc->MoveTo(x2, y2);
		pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2);
		pDc->LineTo(x2 + m_LbEqLg[m_TrkIdx], y2 + m_LbEqHg);
		pDc->LineTo(x2, y2 + m_LbEqHg);
		pDc->LineTo(x2, y2);
	}

	// on restaure le stylo du tracé
	pDc->SelectObject(pOldPen);

	// on restaure le mode de tracé
	pDc->SetROP2(OldRop);

	// on libère le DC
	this->ReleaseDC(pDc);
}

void JFCMapping::OnTrackCourbe(long mode, long indice)
{	
	if (m_pICurve)
	{
		// on teste la selection de la courbe
		if (indice < 0)
		{
			// on recupere le nombre de courbes max
			long tailleMax = m_pICurve->GetNbCourbeMax();

			// on boucle sur toutes les courbes
			for (long i = 0; i < tailleMax; i += 1)
			{
				// on deselectionne toutes les courbes
				m_pICurve->SetSelectCb(i, false);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour sélectionner les losanges
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::OnEndTrackLosanges(long x1, long y1, long x2, long y2, bool reset, bool invert)
{
	// on permute les positions
	if (x1 > x2) { long temp = x1; x1 = x2; x2 = temp; }
	if (y1 > y2) { long temp = y1; y1 = y2; y2 = temp; }

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on calcule la position de l'élément
			long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(indice), this->GetHorzSize());
			long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(indice), this->GetVertSize());

			// on initialise la nouvelle sélection
			bool sel = (reset) ? m_pIMapping->GetSelItem(indice) : false;
			if (invert && (PosX + 2) >= x1 && (PosY + 2) >= y1 && (PosX - 2) <= x2 && (PosY - 2) <= y2)
			{
				sel = ! m_pIMapping->GetSelItem(indice);
				m_pIMapping->SetSelItem(indice, sel);
			}
			else
				m_pIMapping->SetSelItem(indice, ((PosX + 2) >= x1 && (PosY + 2) >= y1 && (PosX - 2) <= x2 && (PosY - 2) <= y2) ? true : sel);
		}
	}

	// Mise à jour des sélections
	LONG A_FAIRE;
//	m_pBag->MajSelection();

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(2);
}

void JFCMapping::OnEndTrackCourbes(long indice, bool reset)
{
	// on récupère le nombre d'éléments
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long i = 0; i < tailleMax; i += 1)
	{
		// on deselectionne la courbe precedente si il le faut
		if (m_pICurve->GetSelectCb(i) == true && m_pICurve->GetSelectCb(i) == true) m_pICurve->SetSelectCb(i, false);
	}

	// on teste l'etat de la courbe
	if (indice >= 0 && m_pICurve->GetExistCb(indice) == true)	
	{
		// on initialise la nouvelle sélection
		bool sel = m_pICurve->GetSelectCb(indice);

		// on modifie la sélection de la courbe
		m_pICurve->SetSelectCb(indice, !sel);

		// on recupere le nombre d'elements du mapping
		long nbElements = m_pIMapping->GetCount();

		// on deselectionne tous les points
		for (long i = 0; i< nbElements; i += 1) m_pIMapping->SetSelItem(i, false);

		// on recupere la liste d'elements de cette courbe
		JList<long>* pListe = m_pICurve->GetListePointCourbe(indice);

		// on se place au debut de la liste
		pListe->MoveFirst();

		// on boucle sur tous les elements
		while (pListe->IsValid())
		{
			// on recupere l'element
			long element = pListe->GetItem();

			// on change la selection des elements du mapping
			m_pIMapping->SetSelItem(element, true);

			// on passe au point suivant
			pListe->MoveNext();
		}
	}

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(2);
}

void JFCMapping::OnEndTrackLibelles(long indice, long dx, long dy)
{
	// on vérifie la validité et la visibilité de l'élément
	if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
	{
		// on vérifie la visibilité du libellé
		if (m_pIMapping->GetShowLabel(indice))
		{
			// on mémorise la position du libellé
			m_pIMapping->SetPosLibelle(indice, this->OnSetDxLibelle(dx), this->OnSetDyLibelle(dy));
		}
	}

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(2);
}

void JFCMapping::OnEndTrackLibEquation(long indice, long dx, long dy, long x, long y)
{
	// on vérifie la validité et la visibilité de la courbe
	if (m_pICurve->GetExistCb(indice) == true)
	{
		// on vérifie la visibilité du libellé
		if (m_pICurve->GetShowLibelle(indice))
		{
			// on mémorise la position du libellé
			m_pICurve->SetDxLibelle(indice, this->OnSetDxLibelle(dx));
			m_pICurve->SetDyLibelle(indice, this->OnSetDyLibelle(dy));
		}
	}

	// on envoie la mise à jour
	m_pICurve->OnUpdate(2);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner les axes
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::DrawAxes(CDC & dc, long HSize, long VSize, bool bCadre)
{
	// on récupère la position de la moyenne
	long MoyX = this->OnGetXPosition(m_XMoy, HSize);
	long MoyY = this->OnGetYPosition(m_YMoy, VSize);

	// on calcule les dépassements
	LONG DepX = (HSize + 8) / 16;
	LONG DepY = (VSize + 8) / 16;

	// on modifie la transparence
	long OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = dc.SetTextColor(RGB(64,64,64));

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenG);

	// on modifie la fonte
	CFont* pOldFont = dc.SelectObject(&m_FontG);

	// on dessine les axes de la moyenne
	dc.MoveTo(MoyX, 0 - DepY); dc.LineTo(MoyX, VSize + DepY);
	dc.MoveTo(0 - DepX, MoyY); dc.LineTo(HSize + DepX, MoyY);

	// on dessine les flèches des axes
	dc.MoveTo(MoyX - 4, 8 - DepY);			dc.LineTo(MoyX - 0, 0 - DepY);
	dc.MoveTo(MoyX + 4, 8 - DepY);			dc.LineTo(MoyX - 0, 0 - DepY);
	dc.MoveTo(HSize + DepX - 8, MoyY - 4);	dc.LineTo(HSize + DepX - 0, MoyY - 0);
	dc.MoveTo(HSize + DepX - 8, MoyY + 4);	dc.LineTo(HSize + DepX - 0, MoyY - 0);

	// on teste si on a des éléments
	if (m_pIMapping->GetCount() > 0)
	{
		// on récupère les libellés des axes
		CString GrandeurX = m_pIMapping->GetLabelXAxe().AsJCharPtr();
		CString GrandeurY = m_pIMapping->GetLabelYAxe().AsJCharPtr();

		// on récupère la taille des libellés
		CSize TailleX = dc.GetTextExtent(GrandeurX);
		CSize TailleY = dc.GetTextExtent(GrandeurY);

		// on écrit les grandeurs
		dc.TextOut(4 + HSize + DepX - TailleX.cx/2, MoyY - TailleX.cy - 6, GrandeurX);
		dc.TextOut(MoyX - TailleY.cx - 8, 10 - DepY - TailleY.cy, GrandeurY);

		// on modifie la fonte
		dc.SelectObject(&m_FontP);

		// on initialise les graduations
		JFCGraduation GradX; GradX.SetRange(m_XMin, m_XMax);
		JFCGraduation GradY; GradY.SetRange(m_YMin, m_YMax);

		// on récupère le nombre de graduations
		long TGrdX = GradX.GetNbGraduation();
		long TGrdY = GradY.GetNbGraduation();

		// on boucle sur toutes les graduations horizontales
		for (long Idx = 0; Idx < TGrdX; Idx += 1)
		{
			// on récupère la position de la graduation
			long PosX = this->OnGetXPosition(GradX.GetGraduation(Idx), HSize);

			// on teste si on doit écrire la graduation
			if (PosX >= (0 - DepX) && PosX <= (HSize + DepX - 1) && (PosX < (MoyX - 6) || PosX > (MoyX + 6)))
			{
				// on récupère le libellé de la graduation
				CString LibX = GradX.GetLibGraduation(Idx);

				// Bidouille si valeur négative, on le repasse en positive (vire symbole -)
				if (atoi(LibX) < 0) LibX.Delete(0,1); 

				// on récupère la taille de la graduation
				CSize SizX = dc.GetTextExtent(LibX);

				// on dessine la graduation
				dc.MoveTo(PosX, MoyY - 2); dc.LineTo(PosX, MoyY + 3);

				// on écrit le libellé
				dc.TextOut(PosX - (SizX.cx / 2), MoyY + 5, LibX);
			}
		}

		// on boucle sur toutes les graduations verticales
		for (long Idy = 0; Idy < TGrdY; Idy += 1)
		{
			// on récupère la position de la graduation
			long PosY = this->OnGetYPosition(GradY.GetGraduation(Idy), VSize);

			// on teste si on doit écrire la graduation
			if (PosY >= (0 - DepY) && PosY <= (VSize + DepY - 1) && (PosY < (MoyY - 6) || PosY > (MoyY + 6)))
			{
				// on récupère le libellé de la graduation
				CString LibY = GradY.GetLibGraduation(Idy);

				// Bidouille si valeur négative, on le repasse en positive (vire symbole -)
				if (atoi(LibY) < 0) LibY.Delete(0,1); 

				// on récupère la taille de la graduation
				CSize SizY = dc.GetTextExtent(LibY);

				// on dessine la graduation
				dc.MoveTo(MoyX - 2, PosY); dc.LineTo(MoyX + 3, PosY);

				// on écrit le libellé
				dc.TextOut(MoyX + 5, PosY - (SizY.cy / 2), LibY);
			}
		}

		if(bCadre)	// on dessine un rectangle autour de tout le dessin
		{
			DepX *= 2;
			DepY *= 2;
			CRect cadreRect(0 - DepX, 0 - DepY, HSize + DepX, VSize + DepY);
			this->DrawCadre(dc, cadreRect);
		}
	}

	// on restaure la fonte
	dc.SelectObject(pOldFont);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

void JFCMapping::OnDrawAxes(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawAxes(dc, HSize, VSize);
}

void JFCMapping::DrawCadre(CDC & dc, CRect& rect)
{
	CPen nullPen;
	nullPen.CreatePen(PS_NULL, 0, RGB(0,0,0));
	CPen* pOldPen = dc.SelectObject(&nullPen);
	dc.MoveTo(rect.left,	rect.bottom);
	dc.LineTo(rect.right,	rect.bottom);
	dc.LineTo(rect.right,	rect.top);
	dc.LineTo(rect.left,	rect.top);
	dc.LineTo(rect.left,	rect.bottom);
	dc.SelectObject(pOldPen);
	nullPen.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les losanges et les libellés
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::DrawLosanges(CDC & dc, long HSize, long VSize)
{
	// on initialise la sélection
	long Sel = 1;
	long LargLosange = 4;

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenB);

	// on modifie le pinceau
	CBrush* pOldBrush = dc.SelectObject(&m_BrushG);

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on récupère l'état de l'élément
			bool Etat = m_pIMapping->GetSelItem(indice);

			// on calcule la position de l'élément
			long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(indice), HSize);
			long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(indice), VSize);

			// on teste la sélection de l'élément
			if (Etat)
			{
				// on modifie le stylo
				if (Sel != 2) { dc.SelectObject(&m_PenR); Sel = 2; }
			}
			else
			{
				// on modifie le stylo
				if (Sel != 1) { dc.SelectObject(&m_PenB); Sel = 1; }
			}

			// Modifie le brush si élément sélectionné
			if (m_pIMapping->GetSelItem(indice)) 
				dc.SelectObject(&m_BrushOrang);
			else
				dc.SelectObject(&m_BrushG);
				
			// on dessine l'élément
			dc.BeginPath();
			dc.MoveTo(PosX - LargLosange, PosY);
			dc.LineTo(PosX, PosY - LargLosange); dc.LineTo(PosX + LargLosange, PosY);
			dc.LineTo(PosX, PosY + LargLosange); dc.LineTo(PosX - LargLosange, PosY);
			dc.EndPath(); dc.StrokeAndFillPath();
		}
	}

	// on restaure le pinceau initial
	dc.SelectObject(pOldBrush);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);
}

void JFCMapping::OnDrawLosanges(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawLosanges(dc, HSize, VSize);
}

void JFCMapping::DrawLibelles(CDC & dc, long HSize, long VSize)
{
	// on modifie la transparence
	long OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = dc.SetTextColor(RGB(0,0,0));

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenG);

	// on modifie la fonte
	CFont* pOldFont = dc.SelectObject(&m_FontM);

	// on modifie le pinceau
	CBrush* pOldBrush = dc.SelectObject(&m_BrushY);

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on vérifie la visibilité du libellé
			if (m_pIMapping->GetShowLabel(indice))
			{
				// on récupère le libellé
				CString Libelle = m_pIMapping->GetLabelItem(indice).AsJCharPtr();

				// on corrige le libellé s'il est vide
				if (FALSE != Libelle.IsEmpty()) Libelle = " ";

				// on teste si on doit calculer les dimensions du libellé
				if (m_LbHg < 1 || m_LbLg[indice] < 1)
				{
					// on récupère la taille du texte
					CSize Size = dc.GetTextExtent(Libelle);

					// on met à jour la longueur
					if (m_LbLg[indice] < 1) m_LbLg[indice] = Size.cx + 4;

					// on met à jour la hauteur
					if (m_LbHg < 1) m_LbHg = Size.cy;
				}

				// on teste si on a quelque chose à dessiner
				if (m_LbHg > 0 && m_LbLg[indice] > 0)
				{
					// on calcule la position de l'élément
					long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(indice), HSize);
					long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(indice), VSize);

					// on récupère la position du libellé
					long LbDx = this->OnGetDxLibelle(m_pIMapping->GetDxLabel(indice), HSize);
					long LbDy = this->OnGetDyLibelle(m_pIMapping->GetDyLabel(indice), VSize);

					// on corrige la position initiale
					PosX -= (m_LbLg[indice] / 2) - LbDx; PosY -= (m_LbHg / 2) - LbDy;

					// on dessine le carré
					if (m_pIMapping->GetSelItem(indice))
                        			dc.SelectObject(&m_PenR);
					else
						dc.SelectObject(&m_PenG);

					// Modifie le brush si élément sélectionné
					if (m_pIMapping->GetSelItem(indice)) 
						dc.SelectObject(&m_BrushOrang);
					else
						dc.SelectObject(&m_BrushG);

					// on dessine le cadre du libellé
					dc.BeginPath();
					dc.MoveTo(PosX, PosY);
					dc.LineTo(PosX + m_LbLg[indice], PosY);
					dc.LineTo(PosX + m_LbLg[indice], PosY + m_LbHg);
					dc.LineTo(PosX, PosY + m_LbHg);
					dc.LineTo(PosX, PosY);
					dc.EndPath(); dc.StrokeAndFillPath();

					// on écrit le libellé
					dc.TextOut(PosX + 2, PosY + 1, Libelle);

				}
			}
		}
	}

	// on restaure le pinceau initial
	dc.SelectObject(pOldBrush);

	// on restaure la fonte
	dc.SelectObject(pOldFont);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

void JFCMapping::OnDrawLibelles(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawLibelles(dc, HSize, VSize);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner les traits des libellés
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::DrawTraitLib(CDC & dc, long HSize, long VSize)
{
	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenG);

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on vérifie la visibilité du libellé
			if (m_pIMapping->GetShowLabel(indice))
			{
				// on calcule la position de l'élément
				long PosX = this->OnGetXPosition(m_pIMapping->GetPosXItem(indice), HSize);
				long PosY = this->OnGetYPosition(m_pIMapping->GetPosYItem(indice), VSize);

				// on récupère la position du libellé
				long LbDx = this->OnGetDxLibelle(m_pIMapping->GetDxLabel(indice), HSize);
				long LbDy = this->OnGetDyLibelle(m_pIMapping->GetDyLabel(indice), VSize);

				// Calcul des limites pour dessin des libellés
				long XMin = this->OnGetXPosition(m_XMin,HSize); 
				long XMax = this->OnGetXPosition(m_XMax,HSize); 
				long YMin = this->OnGetYPosition(m_YMin,VSize); 
				long YMax = this->OnGetYPosition(m_YMax,VSize); 

				//////////////////////////////////////////////////////////////////////////////////
				// ALAIN : Modification Mars 2005
				/////////////////////////// Attention aux limites du graphe //////////////////////
				// Recadrage en Y
				RECT Rect; this->GetClientRect(&Rect);

				if ((PosY + LbDy) > YMin || (PosY + LbDy) < YMax)
				{
					// modifie position du libellé en y
					LbDy = this->OnGetDyLibelle(m_pIMapping->GetDyLabel(indice), VSize);
				}

				// Recadrage en X
				if ((PosX + LbDx) < XMin || (PosX + LbDx) > XMax)
				{
					// modifie position du libellé en y
					LbDx = this->OnGetDxLibelle(m_pIMapping->GetDxLabel(indice), HSize);
				}

				// on modifie couleur du tracé
				if (m_pIMapping->GetSelItem(indice))
					dc.SelectObject(&m_PenR);
				else
					dc.SelectObject(&m_PenG);

				// A FAIRE Recadrage si débordement lors d'une restriction -- Voir Sylvain (limite réelle du dessin)
				if (PosX + LbDx < -20 * HSize / 100 || PosX + LbDx > (120 * HSize / 100))
				{
					m_pIMapping->SetDxLabel(indice, (JFlt32)0.05);
					LbDx = this->OnGetDxLibelle(m_pIMapping->GetDxLabel(indice), VSize);
				}

				if (PosY + LbDy < -20 * VSize / 100 || PosY + LbDy > (120 * VSize / 100))
				{
					m_pIMapping->SetDyLabel(indice, (JFlt32)0.05);
					LbDy = this->OnGetDyLibelle(m_pIMapping->GetDyLabel(indice), VSize);
				}

				// on dessine le trait de liaison
				dc.MoveTo(PosX, PosY); dc.LineTo(PosX + LbDx, PosY + LbDy);
			}
		}
	}

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);
}

void JFCMapping::OnDrawTraitLib(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawTraitLib(dc, HSize, VSize);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner les courbes
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::DrawCurves(CDC & dc,  long HSize, long VSize)
{
	// on recupere le nombre de courbes max
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on boucle sur toute les courbes
	for (long indice = 0; indice < tailleMax; indice+=1)
	{	
		// on teste l'etat de la courbe
		if (m_pICurve->GetExistCb(indice) == true)
		{		
			// on initialise le pinceau
			CPen* pOldPen;

			// on teste la selection de la courbe
			if (m_pICurve->GetSelectCb(indice) == true) 
				pOldPen = dc.SelectObject(&m_PenR);
			else
			{
				// on modifie le stylo
				switch (indice)
				{
					case (0): { pOldPen = dc.SelectObject(&m_PenBlue ); break; }
					case (1): { pOldPen = dc.SelectObject(&m_PenGreen); break; }
					case (2): { pOldPen = dc.SelectObject(&m_PenSky  ); break; }
					case (3): { pOldPen = dc.SelectObject(&m_PenUmbre); break; }
					case (4): { pOldPen = dc.SelectObject(&m_PenOrang); break; }
					default : { pOldPen = dc.SelectObject(&m_PenB    ); break; } 
				}
			}

			double a, b, c;
	
			// on recupere les coefficients de l'equation
			m_pICurve->GetEquation(indice, a, b, c);

			// on calcul la valeur du debordement de la courbe par rapport aux points dessinés
			double VarX = abs(0.09 *(m_XMax - m_XMin));
			double VarY = abs(0.09 *(m_YMax - m_YMin));

			// on recupere les abscisses de droite et de gauche en pixel 
			long x1 = this->OnGetXPosition(m_XMin - VarX, HSize);
			long x2 = this->OnGetXPosition(m_XMax + VarX, HSize);
			long y1 = this->OnGetYPosition(m_YMax + VarY, VSize);
			long y2 = this->OnGetYPosition(m_YMin - VarY, VSize);
			
			// on verifie que les 2 points ne sont pas confondus
			if (x1 == x2 || y1 == y2) { dc.SelectObject(pOldPen); return; }
			
			double xmin = m_XMin - VarX; double xmax = m_XMax + VarX;
			double ymin = m_YMin - VarY; double ymax = m_YMax + VarY;
					
			// on calcule la position de l'élément
			long PsX = this->OnGetXPosition(m_BarycentreX[indice], HSize);
			long PsY = this->OnGetYPosition(m_BarycentreY[indice], VSize);

			// on récupère la position du libellé
			long LbDx = this->OnGetDxLibelle(m_pICurve->GetDxLibelle(indice),HSize);
			long LbDy = this->OnGetDyLibelle(m_pICurve->GetDyLibelle(indice),VSize);

			// on corrige la position initiale
			PsX += LbDx; PsY += LbDy;
						
			// on transforme en reel
			long xmi = this->OnGetXPosition(m_XMin, HSize);
			long xma = this->OnGetXPosition(m_XMax, HSize);
			long yma = this->OnGetYPosition(m_YMin, VSize);
			long ymi = this->OnGetYPosition(m_YMax, VSize);

			double PosX = m_XMin + (double)(PsX  - xmi) * (m_XMax - m_XMin) / (double)(xma - xmi);
			double PosY = m_YMax - (double)(PsY  - ymi) * (m_YMax - m_YMin) / (double)(yma - ymi);

			double LibX = m_XMin + (double)(LbDx - xmi) * (m_XMax - m_XMin) / (double)(xma - xmi);
			double LibY = m_YMax - (double)(LbDy - ymi) * (m_YMax - m_YMin) / (double)(yma - ymi);

			// on initialise le pas
			long pas = 1;

			// on convertit un pixel en reel
			double dx = ((m_XMax + VarX) - (m_XMin - VarX)) / (double)(x2 - x1);

			// on recupere l'abscisse du premier point du mapping en reel
			double x = m_XMin - VarX;

			// on en deduit son ordonnées en reel
			double y = a*x*x + b*x + c;
								
			// on initialise les position de la plus petite distance
			double distSave = -1.0;

			// on se place sur ce point (coordonnees en pixel)
			dc.MoveTo(this->OnGetXPosition(x,HSize), this->OnGetYPosition(y,VSize));

			// on boucle sur toutes les abscisses du mapping
			for (long idx = x1; idx <= x2; idx += pas)
			{	
				// recupere le reel en abscisse
				x += dx * (double)pas;

				// on en deduit le reel en ordonnée
				y  = a*x*x + b*x + c;

				long PixX = this->OnGetXPosition(x, HSize);
				long PixY = this->OnGetYPosition(y, VSize);

				// Test si dans les bornes
				if (y > ymax || y < ymin || x > xmax || x < xmin)
				{
					dc.MoveTo(PixX, PixY);
					continue;
				}

				// if (PixY < -50 || PixY > VSize + 50)
				//	dc.MoveTo(PixX, PixY);

				// on initialise la distance courbe - libelle
				double de;

				// on teste si la courbe et de degre 1 ou 2
				if (a == 0.0)
				{
					// on trace le segment de droite si dans le mapping
					if (xmin <=  x && x <= xmax && ymin <= y && y <= ymax) 
					{ 
						// on dessine le segment
						dc.LineTo(PixX, PixY);

						// on calcule la distance du point de la courbe au libelle 
						de = (PsX-PixX)*(PsX-PixX) + (PsY-PixY)*(PsY-PixY);

						// on sauvegarde la plus petite distance et les coordonnées du point de la courbe
						if (distSave < 0.0 || distSave > de) {distSave = de; m_AccrochageX[indice] = x; m_AccrochageY[indice] = y;}
					}
				}
				else
				{
					// on determine l'abscisse de l'optimum de la courbe
					double xopt = - b / (2*a);

					// on trace le segment de droite si dans le mapping
					if (a > 0.0 && xmin <= x && x <= xopt && ymin <= y && y <= ymax) 
					{ 
						// on dessine le segment
						dc.LineTo(PixX, PixY); 

						// on calcule la distance du point de la courbe au libelle 
						de = (PsX-PixX)*(PsX-PixX) + (PsY-PixY)*(PsY-PixY);

						// on sauvegarde la plus petite distance et les coordonnées du point de la courbe
						if (distSave < 0.0 || distSave > de) {distSave = de; m_AccrochageX[indice] = x; m_AccrochageY[indice] = y;}
					}
					if (a > 0.0 && xopt <= x && x <= xmax && ymin <= y && y <= ymax) 
					{ 
						// on dessine le segment
						dc.LineTo(PixX, PixY); 

						// on calcule la distance du point de la courbe au libelle 
						de = (PsX-PixX)*(PsX-PixX) + (PsY-PixY)*(PsY-PixY);

						// on sauvegarde la plus petite distance et les coordonnées du point de la courbe
						if (distSave < 0.0 || distSave > de) {distSave = de; m_AccrochageX[indice] = x; m_AccrochageY[indice] = y;}
					}
					if (a < 0.0 && xmin <= x && x <= xopt && ymin <= y && y <= ymax) 
					{ 
						// on dessine le segment
						dc.LineTo(PixX, PixY); 

						// on calcule la distance du point de la courbe au libelle 
						de = (PsX-PixX)*(PsX-PixX) + (PsY-PixY)*(PsY-PixY);

						// on sauvegarde la plus petite distance et les coordonnées du point de la courbe
						if (distSave < 0.0 || distSave > de) {distSave = de; m_AccrochageX[indice] = x; m_AccrochageY[indice] = y;}
					}	
					if (a < 0.0 && xopt <= x && x <= xmax && ymin <= y && y <= ymax) 
					{ 
						// on dessine le segment
						dc.LineTo(PixX, PixY); 

						// on calcule la distance du point de la courbe au libelle 
						de = (PsX-PixX)*(PsX-PixX) + (PsY-PixY)*(PsY-PixY);

						// on sauvegarde la plus petite distance et les coordonnées du point de la courbe
						if (distSave < 0.0 || distSave > de) {distSave = de; m_AccrochageX[indice] = x; m_AccrochageY[indice] = y;}
					}
				}
			}
			
			// on restaure le stylo initial
			dc.SelectObject(pOldPen);
		}
	}
}

void JFCMapping::OnDrawCurves(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawCurves(dc, HSize, VSize);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner les equations
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::DrawLibEquation(CDC & dc, long HSize, long VSize)
{
	// on modifie la transparence
	long OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = dc.SetTextColor(RGB(0,0,0));

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenG);

	// on modifie la fonte
	CFont* pOldFont = dc.SelectObject(&m_FontP);	
	
	// on modifie le pinceau
	CBrush* pOldBrush = dc.SelectObject(&m_BrushB);

	// on récupère le nombre de courbes
	long taille = m_pICurve->GetNbCourbeMax();

	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pICurve->GetExistCb(indice) == true)
		{
			bool De1 = OnGetCourbeDegr1();
			bool De2 = OnGetCourbeDegr2();
			double valxx = m_BarycentreX[indice];

			// on vérifie la visibilité du libellé
			if (m_pICurve->GetShowLibelle(indice))
			{	
				// on modifie le stylo
				switch (indice)
				{
					case (0): { pOldBrush = dc.SelectObject(&m_BrushBlue ); break; }
					case (1): { pOldBrush = dc.SelectObject(&m_BrushGreen); break; }
					case (2): { pOldBrush = dc.SelectObject(&m_BrushSky  ); break; }
					case (3): { pOldBrush = dc.SelectObject(&m_BrushUmbre); break; }
					case (4): { pOldBrush = dc.SelectObject(&m_BrushOrang); break; }
					default : { pOldBrush = dc.SelectObject(&m_BrushB    ); break; } 
				}

				// on recupere les coefficients de l'equation
				double a, b, c;	
				m_pICurve->GetEquation(indice, a, b, c);
				double r2 = m_pICurve->GetCoefCorrelation(indice);
								
				// on récupere les libellés de l'equation et de R2
				CString atext, oper1text, btext, oper2text, ctext;
				
				// on determine l'ecriture de l'equation
				if (a == 0.0) 
					atext.Format(""           );
				else
				        atext.Format("%0.2f x²", a);

				if (a != 0.0 && b > 0) 
					oper1text.Format("+");
				if (b == 0.0)
					btext.Format(""           );
				else		  
					btext.Format("%0.2f x" , b);

				if ((a != 0.0 || b != 0.0) && c > 0) 
					oper2text.Format("+");

				if (c == 0.0) 
					ctext.Format(""           );
				else		  
					ctext.Format("%0.2f"  ,  c);

				// on en deduit le texte a afficher
				CString Lib; Lib.Format(" y = %s %s %s %s %s, R2 = %0.2f", atext, oper1text, btext, oper2text, ctext, r2);			

				// on teste si on doit calculer les dimensions du libellé
				if (m_LbEqHg < 1 || m_LbEqLg[indice] < 1)
				{
					// on récupère la taille de du texte
					CSize Size = dc.GetTextExtent(Lib);

					// on met à jour la longueur
					if (m_LbEqLg[indice] < 1) m_LbEqLg[indice] = Size.cx + 4;

					// on met à jour la hauteur
					if (m_LbEqHg < 1) m_LbEqHg = Size.cy;
				}

				// on calcule la position d'ancrage du trait sur le libelle
				double valx = m_BarycentreX[indice];
				long PosX = this->OnGetXPosition(m_BarycentreX[indice], HSize);
				long PosY = this->OnGetYPosition(m_BarycentreY[indice], VSize);

				// on récupère la position du libellé
				long LbEqDx = this->OnGetDxLibelle(m_pICurve->GetDxLibelle(indice), HSize);
				long LbEqDy = this->OnGetDyLibelle(m_pICurve->GetDyLibelle(indice), VSize);

				// on corrige la position initiale
				PosX -= (m_LbEqLg[indice] / 2) - LbEqDx; 
				PosY -= (m_LbEqHg         / 2) - LbEqDy;

				// on dessine le carré
				dc.BeginPath();
				dc.MoveTo(PosX, PosY);
				dc.LineTo(PosX                   , PosY + m_LbEqHg);
				dc.LineTo(PosX + m_LbEqLg[indice], PosY + m_LbEqHg);
				dc.LineTo(PosX + m_LbEqLg[indice], PosY           );
				dc.LineTo(PosX                   , PosY           );
				dc.EndPath(); dc.StrokeAndFillPath();

				// on écrit l'equation et le R2
				dc.TextOut(PosX, PosY, Lib);
			}
		}
	}

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la fonte
	dc.SelectObject(pOldFont);

	// on restaure le pinceau initial
	dc.SelectObject(pOldBrush);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

void JFCMapping::OnDrawLibEquation(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawLibEquation(dc, HSize, VSize);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner les traits des libellés des courbes
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::DrawTraitEquation(CDC & dc, long HSize, long VSize)
{
	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenG);

	// on récupère le nombre de courbes
	long taille = m_pICurve->GetNbCourbeMax();
						
	// on parcourt toutes les courbes
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pICurve->GetExistCb(indice) == true)
		{
			// on vérifie la visibilité du libellé
			if (m_pICurve->GetShowLibelle(indice))
			{	
				// on recupere les coefficients de l'equation
				double a, b, c;	m_pICurve->GetEquation(indice, a, b, c);

				// on calcule la position de l'élément
				long PosX = this->OnGetXPosition(m_BarycentreX[indice], HSize);
				long PosY = this->OnGetYPosition(m_BarycentreY[indice], VSize);
				
				long PixX = this->OnGetXPosition(m_AccrochageX[indice], HSize);
				long PixY = this->OnGetYPosition(m_AccrochageY[indice], VSize);

				// on récupère la position du libellé
				long LbDx = this->OnGetDxLibelle(m_pICurve->GetDxLibelle(indice), HSize);
				long LbDy = this->OnGetDyLibelle(m_pICurve->GetDyLibelle(indice), VSize);

				// on dessine le trait de liaison
				dc.MoveTo(LbDx + PosX, LbDy + PosY); dc.LineTo(PixX, PixY);
			}
		}
	}

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);
}

void JFCMapping::OnDrawTraitEquation(CDC & dc, RECT rect)
{
	// on récupère les dimensions
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	this->DrawTraitEquation(dc, HSize, VSize);
}

////////////////////////////////////////////////////////////////////////////
// les fonctions pour mettre à jour les éléments
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::OnUpdateLongueurs()
{
	// on initialise les hauteurs
	m_LbHg   = 0;
	m_LbEqHg = 0;

	// on vérifie la validité des éléments
	if (m_pIMapping != NULL)
	{
		// on récupère le nombre d'éléments
		long taille = m_pIMapping->GetCount();

		// on initialise les longueurs
		m_LbLg.SetSize(taille);	for (long indice = 0; indice < taille; indice += 1) m_LbLg[indice] = 0;
	}
	else
	{
		// on réinitialise les longueurs
		m_LbLg.RemoveAll();
	}
		
	// on vérifie la validité des courbes
	if (m_pICurve != NULL)
	{
		// on récupère le nombre d'éléments
		long tailleMax = m_pICurve->GetNbCourbeMax();

		// on initialise les longueurs
		long i = 0;
		m_LbEqLg     .SetSize(tailleMax); for (		i = 0; i < tailleMax; i += 1) m_LbEqLg[i]      =    0;
		m_BarycentreX.SetSize(tailleMax); for (     i = 0; i < tailleMax; i += 1) m_BarycentreX[i] =  0.0;
		m_BarycentreY.SetSize(tailleMax); for (     i = 0; i < tailleMax; i += 1) m_BarycentreY[i] =  0.0;
		m_AccrochageX.SetSize(tailleMax); for (     i = 0; i < tailleMax; i += 1) m_AccrochageX[i] =  0.0;
		m_AccrochageY.SetSize(tailleMax); for (     i = 0; i < tailleMax; i += 1) m_AccrochageY[i] =  0.0;
	}
	else
	{
		// on réinitialise les longueurs
		m_LbEqLg     .RemoveAll();
		m_BarycentreX.RemoveAll();;
		m_BarycentreY.RemoveAll();;
		m_AccrochageX.RemoveAll();
		m_AccrochageY.RemoveAll();
	}
}

void JFCMapping::OnUpdateGrandeurs()
{
	// on initialise les bornes
	m_XGrdMin = m_XGrdMax = 0.0;
	m_YGrdMin = m_YGrdMax = 0.0;

	// on initialise les grandeurs
	m_XMin = m_XMax = m_XMoy = 0.0;
	m_YMin = m_YMax = m_YMoy = 0.0;

	// on vérifie la validité des éléments
	if (m_pIMapping != NULL)
	{
		// on initialise le nombre d'éléments valides
		long valide = 0;

		// on récupère le nombre d'éléments
		long taille = m_pIMapping->GetCount();

		// on boucle sur les éléments restants
		for (long indice = 0; indice < taille; indice += 1)
		{
			// on vérifie la validité et la visibilité de l'élément
			if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
			{
				// on récupère la position de l'élément
				double GrdX = m_pIMapping->GetPosXItem(indice);
				double GrdY = m_pIMapping->GetPosYItem(indice);

				// on teste si on est sur le premier élément
				if (valide == 0)
				{
					// on initialise les grandeurs et les moyennes
					m_XMin = m_XMax = m_XMoy = GrdX;
					m_YMin = m_YMax = m_YMoy = GrdY;
				}
				else
				{
					// on incrémente les moyennes
					m_XMoy += GrdX; m_YMoy += GrdY;

					// on met à jour les grandeurs
					if (GrdX < m_XMin) m_XMin = GrdX;
					if (GrdX > m_XMax) m_XMax = GrdX;
					if (GrdY < m_YMin) m_YMin = GrdY;
					if (GrdY > m_YMax) m_YMax = GrdY;
				}

				// on incrémente le nombre d'éléments valides
				valide += 1;
			}
		}

		// on teste si on a des éléments
		if (valide > 0)
		{
			// on calcule les moyennes
			m_XMoy /= (double)valide;
			m_YMoy /= (double)valide;

			// on récupère la position des axes
			m_XMoy = m_pIMapping->GetPosXAxe(m_XMoy);
			m_YMoy = m_pIMapping->GetPosYAxe(m_YMoy);

			// on met à jour les grandeurs
			if (m_XMoy < m_XMin) m_XMin = m_XMoy;
			if (m_XMoy > m_XMax) m_XMax = m_XMoy;
			if (m_YMoy < m_YMin) m_YMin = m_YMoy;
			if (m_YMoy > m_YMax) m_YMax = m_YMoy;
		}

		// on initialise les graduations
		JFCGraduation GradX; GradX.SetRange(m_XMin, m_XMax);
		JFCGraduation GradY; GradY.SetRange(m_YMin, m_YMax);

		// on récupère les bornes des graduations
		double XGrdMin = GradX.GetMin(); 
		double XGrdMax = GradX.GetMax();
		double YGrdMin = GradY.GetMin(); 
		double YGrdMax = GradY.GetMax();

		// on initialise les bornes
		m_XGrdMin = (m_XMin > XGrdMin) ? XGrdMin : m_XMin;
		m_XGrdMax = (m_XMax < XGrdMax) ? XGrdMax : m_XMax;
		m_YGrdMin = (m_YMin > YGrdMin) ? YGrdMin : m_YMin;
		m_YGrdMax = (m_YMax < YGrdMax) ? YGrdMax : m_YMax;
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer l'état de l'univers
/////////////////////////////////////////////////////////////////////////////

bool JFCMapping::OnGetSelection()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on vérifie la sélection de l'élément
			if (m_pIMapping->GetSelItem(indice)) return (true);
		}
	}

	// il n'y a pas de sélection
	return (false);
}

bool JFCMapping::OnGetExistCourbe()
{
	// on récupère le nombre de courbes
	long nbCourbes = m_pICurve->GetNbCourbe();

	// on teste la presence de courbes
	if (nbCourbes != 0) return (true);

	// il n'y a pas de courbe
	return (false);
}

bool JFCMapping::OnGetInfosCourbe()
{
	// on recupere le nombre de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long indice = 0; indice < tailleMax; indice += 1)
	{
		// on verifie la validité et la selection d'une courbe
		if (m_pICurve->GetExistCb(indice) == true && m_pICurve->GetSelectCb(indice) == true)
		{
			// on teste si les infos sont deja affichées
			if (m_pICurve->GetShowLibelle(indice) == true) return (true);
		}
	}
	
	return (false);
}

bool JFCMapping::OnGetCourbeSelec()
{
	// on recupere le nombre de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long indice = 0; indice < tailleMax; indice += 1)
	{
		// on verifie la validité d'une courbe
		if (m_pICurve->GetExistCb(indice) == true)
		{
			// on teste la selection de courbes
			if (m_pICurve->GetSelectCb(indice) == true) return (true);
		}
	}
	
	// il n'y a pas de courbes selectionnées
	return (false);
}

bool JFCMapping::OnGetCourbeDegr1()
{
	if (m_pICurve)
	{
		// on recupere le nombre de courbes maximum
		long tailleMax = m_pICurve->GetNbCourbeMax();

		// on récupère le nombre de courbes
		long nbCourbes = m_pICurve->GetNbCourbe();

		// on récupère le nombre d'éléments
		long nbElmt = m_pIMapping->GetCount();

		// on initialise les extremités du rectangle
		long i=0;
		CArray <double, double &> xdte; xdte.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) xdte[i] = m_XMin - 1.0;
		CArray <double, double &> xgch; xgch.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) xgch[i] = m_XMax + 1.0;
		CArray <double, double &> ybas; ybas.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) ybas[i] = m_YMin - 1.0;
		CArray <double, double &> yhau; yhau.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) yhau[i] = m_YMax + 1.0;

		// on initialise la liste de points
		JList<long> PtSelectionnes;

		// on parcourt tous les éléments
		long point = 0;
		for (; point < nbElmt; point += 1)
		{
			// on stocke l'indice dans la liste
			if (m_pIMapping->IsItemValid(point) && m_pIMapping->GetShowItem(point) && m_pIMapping->GetSelItem(point)) PtSelectionnes.AddTail() = point;
		}

		// on recupere le nombre de points de la future courbe
		long Elmt = PtSelectionnes.GetCount();
			
		// on teste le nombre minimum de points
		if (Elmt < 2) return (false);

		// on teste le nombre de courbes
		if (nbCourbes >= tailleMax) return (false);	

		// on parcourt toutes les courbes
		for (long courbe = 0; courbe  < tailleMax; courbe  += 1)
		{			
			// on recupere a, b, c;
			double a, b, c;
			
			// on recupere les coef
			m_pICurve->GetEquation(courbe , a, b, c);
		
			if (a == 0.0 && m_pICurve->GetExistCb(courbe))
			{
				// on recupere la liste de points de la courbes
				JList<long>* PtCourbe = m_pICurve->GetListePointCourbe(courbe);
					
				// on initialise le boolean
				bool idem = true;

				if (PtSelectionnes.GetCount() == PtCourbe->GetCount())
				{					
					// on se place sur les premiers elements des 2 listes
					PtSelectionnes.MoveFirst(); PtCourbe->MoveFirst();

					// on parcourt les 2 listes
					while (PtSelectionnes.IsValid() && PtCourbe->IsValid())
					{
						// on compare les elements des 2 listes deux a deux
						if (PtSelectionnes.GetItem() != PtCourbe->GetItem()) {idem = false; break;} 
						
						// on passe aux elements suivants
						PtSelectionnes.MoveNext(); PtCourbe->MoveNext();
					}		
				}
				else idem = false;
			
				if (m_BarycentreX[courbe] == 0.0 && m_BarycentreY[courbe] == 0.0)
				{
					PtCourbe->MoveFirst();

					// on determine le barycentre des points
					while (PtCourbe->IsValid())
					{
						// on récupere l'indice du point
						point = PtCourbe->GetItem();

						// on determine les extremités du rectangle
						if (xdte[courbe] < m_XMin || xdte[courbe] > m_pIMapping->GetPosXItem(point)) xdte[courbe] = m_pIMapping->GetPosXItem(point);
						if (xgch[courbe] > m_XMax || xgch[courbe] < m_pIMapping->GetPosXItem(point)) xgch[courbe] = m_pIMapping->GetPosXItem(point);
						if (ybas[courbe] < m_YMin || ybas[courbe] > m_pIMapping->GetPosYItem(point)) ybas[courbe] = m_pIMapping->GetPosYItem(point);
						if (yhau[courbe] > m_YMax || yhau[courbe] < m_pIMapping->GetPosYItem(point)) yhau[courbe] = m_pIMapping->GetPosYItem(point);
						
						PtCourbe->MoveNext();
					}		
					
					// on calcule le barycentre du rectangle au plus proche des points
					m_BarycentreX[courbe] = (xdte[courbe] + xgch[courbe]) / 2.0;
					m_BarycentreY[courbe] = (ybas[courbe] + yhau[courbe]) / 2.0;
				
				}
						
				// les 2 courbes sont identiques
				if (idem == true) return (false);	
			}
		}

		return (true);
	}

	return false;
}

bool JFCMapping::OnGetCourbeDegr2()
{
	if (m_pICurve)
	{
		// on recupere le nombre de courbes maximum
		long tailleMax = m_pICurve->GetNbCourbeMax();

		// on récupère le nombre de courbes
		long nbCourbes = m_pICurve->GetNbCourbe();

		// on récupère le nombre d'éléments
		long nbElmt = m_pIMapping->GetCount();

		// on initialise les extremités du rectangle
		long i=0;
		CArray <double, double &> xdte; xdte.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) xdte[i] = m_XMin - 1.0;
		CArray <double, double &> xgch; xgch.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) xgch[i] = m_XMax + 1.0;
		CArray <double, double &> ybas; ybas.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) ybas[i] = m_YMin - 1.0;
		CArray <double, double &> yhau; yhau.SetSize(tailleMax); for ( i = 0; i < tailleMax; i += 1) yhau[i] = m_YMax + 1.0;

		// on initialise la liste de points
		JList<long> PtSelectionnes;

		// on parcourt tous les éléments
		long point = 0;
		for (; point < nbElmt; point += 1)
		{
			// on stocke l'indice dans la liste
			if (m_pIMapping->IsItemValid(point) && m_pIMapping->GetShowItem(point) && m_pIMapping->GetSelItem(point)) PtSelectionnes.AddTail() = point;
		}

		// on recupere le nombre de points de la future courbe
		long Elmt = PtSelectionnes.GetCount();
			
		// on teste le nombre minimum de points
		if (Elmt < 3) return (false);

		// on teste le nombre de courbes
		if (nbCourbes >= tailleMax) return (false);	

		// on parcourt toutes les courbes
		for (long courbe = 0; courbe  < tailleMax; courbe  += 1)
		{	
			// on recupere a, b, c;
			double a, b, c;

			// on recupere les coefs
			m_pICurve->GetEquation(courbe, a, b, c);
			
			if (a != 0.0 && m_pICurve->GetExistCb(courbe))
			{
				// on recupere la liste de points de la courbes
				JList<long>* PtCourbe = m_pICurve->GetListePointCourbe(courbe );
					
				// on initialise le boolean
				bool idem = true;

				if (PtSelectionnes.GetCount() == PtCourbe->GetCount())
				{		
					// on se place sur les premiers elements des 2 listes
					PtSelectionnes.MoveFirst(); PtCourbe->MoveFirst();

					// on parcourt les 2 listes
					while (PtSelectionnes.IsValid() && PtCourbe->IsValid())
					{
						// on compare les elements des 2 listes deux a deux
						if (PtSelectionnes.GetItem() != PtCourbe->GetItem()) {idem = false; break;} 
						
						// on passe aux elements suivants
						PtSelectionnes.MoveNext(); PtCourbe->MoveNext();
					}		
				}
				else idem = false;

				if (m_BarycentreX[courbe] == 0.0 && m_BarycentreY[courbe] == 0.0)
				{
					PtCourbe->MoveFirst();

					// on determine le barycentre des points
					while (PtCourbe->IsValid())
					{
						// on récupere l'indice du point
						point = PtCourbe->GetItem();

						// on determine les extremités du rectangle
						if (xdte[courbe] < m_XMin || xdte[courbe] > m_pIMapping->GetPosXItem(point)) xdte[courbe] = m_pIMapping->GetPosXItem(point);
						if (xgch[courbe] > m_XMax || xgch[courbe] < m_pIMapping->GetPosXItem(point)) xgch[courbe] = m_pIMapping->GetPosXItem(point);
						if (ybas[courbe] < m_YMin || ybas[courbe] > m_pIMapping->GetPosYItem(point)) ybas[courbe] = m_pIMapping->GetPosYItem(point);
						if (yhau[courbe] > m_YMax || yhau[courbe] < m_pIMapping->GetPosYItem(point)) yhau[courbe] = m_pIMapping->GetPosYItem(point);

						PtCourbe->MoveNext();
					}					
						
					// on calcule le barycentre du rectangle au plus proche des points
					m_BarycentreX[courbe] = (xdte[courbe] + xgch[courbe]) / 2.0;
					m_BarycentreY[courbe] = (ybas[courbe] + yhau[courbe]) / 2.0;


					double testx = m_BarycentreX[courbe];
					double test = testx;
				}
						
				// les 2 courbes sont identiques
				if (idem == true) return (false);
			}
		}
		return (true);
	}

	return false;
}

bool JFCMapping::OnGetFiltrage()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité de l'élément
		if (m_pIMapping->IsItemValid(indice))
		{
			// on vérifie la visibilité de l'élément
			if (!m_pIMapping->GetShowItem(indice)) 
				return (true);
		}
	}

	// il n'y a pas de filtrage
	return (false);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour gérer le menu contextuel
/////////////////////////////////////////////////////////////////////////////

void JFCMapping::SetShowAllLib(bool etat)
{
	// Positionne état show pour tous les libellés
	m_pIMapping->SetShowAllLibelle(etat);
	
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(3);

	// Sauvegarde état visibilité des libellés
	m_pIMapping->SetLibAfficher(etat);
}

void JFCMapping::OnShowAllLibelle()
{
	// Tous les libellés visibles
	SetShowAllLib(true);
}

void JFCMapping::OnHideAllLibelle()
{
	// Tous les libellés invisibles
	SetShowAllLib(false);
}

void JFCMapping::OnShowSelLibelle()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on teste la sélection de l'élément
			if (m_pIMapping->GetSelItem(indice))
			{
				// on montre le libellé
				m_pIMapping->SetShowLabel(indice, true);

				// on supprime la sélection
				// m_pIMapping->SetSelItem(indice, false);
			}
		}
	}

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour des libellés
	m_pIMapping->OnUpdate(3);
}

void JFCMapping::OnHideSelLibelle()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on teste la sélection de l'élément
			if (m_pIMapping->GetSelItem(indice))
			{
				// on cache le libellé
				//m_pIMapping->SetShowItem(indice, false);  ????
				m_pIMapping->SetShowLabel(indice,false);

				// on supprime la sélection ( A VOIR)
				// m_pIMapping->SetSelItem(indice, false);
			}
		}
	}


	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour des libellés
	m_pIMapping->OnUpdate(3);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Affichage uniquement des items sélectionnés
void JFCMapping::OnShowSelItem()
{
	// Simplification, on passera par fonction document
	m_pIMapping->ShowSelItem();

	/*
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on recupere le nombre de courbes max
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on teste la sélection de l'élément
			if (m_pIMapping->GetSelItem(indice))
			{
				// on déselectionne l'élément
				// Alain Oct 2004 on garde désormais la sélection effectuée
				// m_pIMapping->SetSelection(indice, false);
			}
			else
			{
				// on cache l'élément
				m_pIMapping->SetShowItem(indice, false);

				// on boucle sur toutes les courbes
				for (long courbe = 0; courbe < tailleMax;courbe += 1)
				{
					// on teste si la courbe existe
					if (m_pICurve->GetExistCb(courbe) == true)
					{
						// on recupere la liste d'elements
						JList<long>* pListe = m_pICurve->GetListePointCourbe(courbe);

						// on se place sur le premier element de la courbe
						pListe->MoveFirst();

						// on boucle sur tous les elements de la courbe
						while(pListe->IsValid())
						{
							// on teste si l'element a cacher est dans la liste
							if (pListe->GetItem() == indice)
							{
								// on deselectionne la courbe
								m_pICurve->ResetCourbe(courbe);

								m_LbEqLg[courbe]      =   0;
								m_BarycentreX[courbe] = 0.0;
								m_BarycentreY[courbe] = 0.0;
								m_AccrochageX[courbe] = 0.0;
								m_AccrochageY[courbe] = 0.0;
							}

							// on passe a l'element suivant
							pListe->MoveNext();
						}
					}
				}
			}
		}
	}

	// on met à jour les grandeurs
	this->OnUpdateGrandeurs();

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(3);
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////
// Cache tous les points sélectionés
void JFCMapping::OnHideSelItem()
{
	// Simplification, on passera par fonction document
	m_pIMapping->HideSelItem();

	/*
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on recupere le nombre de courbes max
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on teste la sélection de l'élément
			if (m_pIMapping->GetSelItem(indice))
			{
				// on cache l'élément
				m_pIMapping->SetShowItem(indice, false);

				// on boucle sur toutes les courbes
				for (long courbe = 0; courbe < tailleMax; courbe += 1)
				{
					// on teste si la courbe existe
					if (m_pICurve->GetExistCb(courbe) == true)
					{
						// on recupere la liste d'elements
						JList<long>* pListe = m_pICurve->GetListePointCourbe(courbe);

						// on se place sur le premier element de la courbe
						pListe->MoveFirst();

						// on boucle sur tous les elements de la courbe
						while(pListe->IsValid())
						{
							// on teste si l'element a cacher est dans la liste
							if (pListe->GetItem() == indice)
							{				
								// on deselectionne la courbe
								m_pICurve->ResetCourbe(courbe);
								m_LbEqLg[courbe]      =   0;
								m_BarycentreX[courbe] = 0.0;
								m_BarycentreY[courbe] = 0.0;
								m_AccrochageX[courbe] = 0.0;
								m_AccrochageY[courbe] = 0.0;
							}

							// on passe a l'element suivant
							pListe->MoveNext();
						}
					}
				}

				// on déselectionne l'élément
				// Alain Oct 2004 on garde l'état sélection 
				// m_pIMapping->SetSelItem(indice, false);
			}
		}
	}

	// on met à jour les grandeurs
	this->OnUpdateGrandeurs();

		// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(3);
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////
// Affichag de tous les items
void JFCMapping::OnShowAllItem()
{
	// Simplification, on passera par fonction document
	m_pIMapping->ShowAllItem();

	/*
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité de l'élément
		if (m_pIMapping->IsItemValid(indice))
		{
			// on montre l'élément
			m_pIMapping->SetShowItem(indice, true);

			// Alain Oct 2004 on déselectionne l'élément
			// on garde l'état de la sélection plateau
			// m_pIMapping->SetSelItem(indice, false);
		}
	}

	// on met à jour les grandeurs
	this->OnUpdateGrandeurs();

		// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(3);
	*/
}

void JFCMapping::OnAllSelect()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on sélectionne l'élément
			m_pIMapping->SetSelItem(indice, true);
		}
	}

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Inversion des points sélectionnés du mapping ((donc aussi les courbes))
void JFCMapping::OnInvSelect()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on inverse la sélection de l'élément
			m_pIMapping->SetSelItem(indice, !m_pIMapping->GetSelItem(indice));
		}
	}

	// Vire toutes les courbes
	OnHideAllCourbe();

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Désélection de tous les points sélectionnés du mapping (donc aussi les courbes)
void JFCMapping::OnNoSelect()
{
	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice))
		{
			// on désélectionne l'élément
			m_pIMapping->SetSelItem(indice, false);
		}
	}

	// Vire toutes les courbes
	OnHideAllCourbe();

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pIMapping->OnUpdate(1);
}

/////////////////////////////////////////////////////////////////////////////
// JFCMapping message handlers

void JFCMapping::PreSubclassWindow() 
{
	// on initialise les éléments
	m_pIMapping = NULL;

	// on initialise les longueurs et les grandeurs
	this->OnUpdateLongueurs(); this->OnUpdateGrandeurs();

	// on initialise le tracking
	m_Track = 0; m_TrkIdx = m_TrkPar1 = m_TrkPar2 = m_TrkPar3 = m_TrkPar4 = 0;

	// on appelle le gestionnaire de base
	this->JFCView::PreSubclassWindow();
}

void JFCMapping::OnPaint() 
{
	// on appelle le gestionnaire de base
	this->JFCView::OnPaint();

	// on dessine la sélection
	if      (m_Track == 1) this->OnTrackLosange    (1, 0, 0, 0);
	else if (m_Track == 2) this->OnTrackLibelle    (1, 0, 0, 0);
	else if (m_Track == 4) this->OnTrackLibEquation(1, 0, 0, 0);
}

void JFCMapping::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on vérifie la validité des éléments
	if (m_pIMapping != NULL)
	{
		// on teste si on peut lancer un tracking
		if (m_Track == 0 && NULL == ::GetCapture())
		{
			// on capture la souris
			::SetCapture(this->m_hWnd);

			// on teste si a capturé la souris
			if (this->m_hWnd == ::GetCapture())
			{
				// on initialise la position
				long x, y;

				// on récupère la position réelle
				this->GetTrackIdx(point.x, point.y, x, y);

				// on recherche le libellé
				long indice = this->OnGetLibelleAtPoint(x, y);

				// on recherche le libellé de l'equation
				long indLibEq   = this->OnGetLibEquationAtPoint(x, y);
				
				// on recherche le point
				long indpoint = this->OnGetLosangeAtPoint(x, y);

				// on recherche la courbe
				long index    = this->OnGetCourbeAtPoint(x, y);

				// on initialise le tracking
				if (indLibEq < 0)
				{
					if (indice < 0)
					{	
						this->OnTrackCourbe(1, index); 

						if (indpoint <  0) 
						{ 
							m_Track = 1; 
							if (index >= 0)
							{
								m_Track = 3;
								this->OnEndTrackCourbes(index, 0);
								this->InvalidateRect(NULL, FALSE);
							}
							this->OnTrackLosange(1, 0, x, y); 
						}
						else 
						{ 
							m_Track = 1; 
							this->OnTrackLosange(1, 0, x, y); 
						}
					}
					else 
					{ 
						m_Track = 2;
						this->OnTrackLibelle(1, indice, x, y); 
					}
				}
				else 
				{ 
					m_Track = 4; 
					this->OnTrackLibEquation(1, indLibEq, x, y); 
				}
			}
		}
	}

	// on appelle le gestionaire de base
	this->JFCView::OnLButtonDown(nFlags, point);
}

void JFCMapping::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// on vérifie la validité des éléments
	if (m_pIMapping != NULL)
	{
		// on initialise le menu
		CMenu Menu;

		// on crée le menu contextuel
		if (FALSE != Menu.CreatePopupMenu())
		{
			// on convertit la position en coordonnées écran
			this->ClientToScreen(&point);

			// on récupère l'état du filtrage et des sélections
			bool Flt = this->OnGetFiltrage();
			bool Sel = this->OnGetSelection();
			bool De1 = this->OnGetCourbeDegr1();
			bool De2 = this->OnGetCourbeDegr2();
			bool Cur = this->OnGetCourbeSelec();
			bool Exi = this->OnGetExistCourbe();

			// on initialise les indicateurs
			UINT Static  = MF_STRING | MF_ENABLED;
			UINT DynaFlt = (Flt) ? (MF_STRING | MF_ENABLED) : (MF_STRING | MF_GRAYED);
			UINT DynaSel = (Sel) ? (MF_STRING | MF_ENABLED) : (MF_STRING | MF_GRAYED);
			UINT DynaDe1 = (De1) ? (MF_STRING | MF_ENABLED) : (MF_STRING | MF_GRAYED);
			UINT DynaDe2 = (De2) ? (MF_STRING | MF_ENABLED) : (MF_STRING | MF_GRAYED);
			UINT DynaCur = (Cur) ? (MF_STRING | MF_ENABLED) : (MF_STRING | MF_GRAYED);
			UINT DynaExi = (Exi) ? (MF_STRING | MF_ENABLED) : (MF_STRING | MF_GRAYED);

			// on ajoute les éléments au menu
			Menu.AppendMenu(Static,  JFCMapping::IdShowAllLibelle, "Afficher tous les libellés");
			Menu.AppendMenu(Static,  JFCMapping::IdHideAllLibelle, "Masquer tous les libellés");
			Menu.AppendMenu(MF_SEPARATOR, 0);
			Menu.AppendMenu(DynaSel, JFCMapping::IdShowSelLibelle, "Afficher les libellés des points sélectionnés");
			Menu.AppendMenu(DynaSel, JFCMapping::IdHideSelLibelle, "Masquer les libellés des points sélectionnés");

			// SsMenu filtrage support plus nécessaire
			Menu.AppendMenu(MF_SEPARATOR, 0);
			Menu.AppendMenu(DynaSel, JFCMapping::IdShowSelItem,    "Restreindre l'analyse aux points sélectionnés");
			Menu.AppendMenu(DynaSel, JFCMapping::IdHideSelItem,    "Exclure de l'analyse les points sélectionnés");
			Menu.AppendMenu(DynaFlt, JFCMapping::IdShowAllItem,    "Restaurer l'univers d'analyse");
			
			Menu.AppendMenu(MF_SEPARATOR, 0);
			Menu.AppendMenu(Static,  JFCMapping::IdAllSelect,      "Sélectionner tous les points");
			Menu.AppendMenu(Static,  JFCMapping::IdInvSelect,      "Inverser la sélection");
			Menu.AppendMenu(Static,  JFCMapping::IdNoSelect,       "Désélectionner");
			Menu.AppendMenu(MF_SEPARATOR, 0);
			Menu.AppendMenu(DynaDe1, JFCMapping::IdCreateCourbeD1, "Calculer la courbe de tendance (Degré 1)");
			Menu.AppendMenu(DynaDe2, JFCMapping::IdCreateCourbeD2, "Calculer la courbe de tendance (Degré 2)");
			Menu.AppendMenu(DynaCur, JFCMapping::IdHideCourbeSel,  "Effacer la courbe sélectionnée");
			Menu.AppendMenu(DynaExi, JFCMapping::IdHideAllCourbe,  "Effacer toutes les courbes");
			Menu.AppendMenu(MF_SEPARATOR, 0);
			Menu.AppendMenu(DynaExi,  JFCMapping::IdShowLibCourbe, "Afficher les libellés des courbes");
			Menu.AppendMenu(DynaExi,  JFCMapping::IdHideLibCourbe, "Masquer les libellés des courbes");
			// on affiche le menu
			Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
		}
	}

	// on appelle le gestionnaire de base
	this->JFCView::OnRButtonDown(nFlags, point);
}

void JFCMapping::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on vérifie la validité des éléments
	if (m_pIMapping != NULL)
	{
		// on teste si on a un tracking en cours
		if (m_Track > 0 && this->m_hWnd == ::GetCapture())
		{
			// on libère la souris
			::ReleaseCapture();

			// on teste si la libération s'est bien effectuée
			if (this->m_hWnd != ::GetCapture())
			{
				// Test si point non en dehors du cadre mapping
				RECT Rect; this->GetClientRect(&Rect);
				if (point.x > 0 && point.y > 0 && point.x < Rect.right  && point.y < Rect.bottom)
				{
					// on modifie l'indicateur de tracking
					long Track = m_Track; m_Track = 0;

					// on initialise le flag de réinitialisation
					bool reset = ((MK_CONTROL | MK_SHIFT) & nFlags) ? true : false;
					bool invert = ((MK_CONTROL) & nFlags) ? true : false;

					// on efface le tracking
					if      (Track == 1) 
					{ 
						this->OnTrackLosange(4, 0, 0, 0); 
						this->OnEndTrackLosanges(m_TrkPar1, m_TrkPar2, m_TrkPar3, m_TrkPar4, reset, invert); 
					}
					else if (Track == 2) 
					{ 
						this->OnTrackLibelle(4, 0, 0, 0); 
						this->OnEndTrackLibelles(m_TrkIdx, m_TrkPar1, m_TrkPar2); 
					}			
					else if (Track == 4) 
					{ 
						this->OnTrackLibEquation(4, 0, 0, 0); 
						this->OnEndTrackLibEquation(m_TrkIdx, m_TrkPar1, m_TrkPar2, m_TrkPar3, m_TrkPar4); 
					}			

					// on redessine le mapping
					this->InvalidateRect(NULL, FALSE);
				}
				else
					m_Track = 0;
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFCView::OnLButtonUp(nFlags, point);
}

void JFCMapping::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on vérifie la validité des éléments
	if (m_pIMapping != NULL)
	{
		// on initialise la position
		long x, y;

		// on récupère la position réelle
		this->GetTrackIdx(point.x, point.y, x, y);

		// on teste si on a un tracking en cours
		if (m_Track > 0 && this->m_hWnd == ::GetCapture())
		{
			// on déplace le tracking
			if      (m_Track == 1) this->OnTrackLosange    (3, 0, x, y);
			else if (m_Track == 2) this->OnTrackLibelle    (3, 0, x, y);
			else if (m_Track == 4) this->OnTrackLibEquation(3, 0, x, y);
		}
		else
		{
			// on récupère le libellé sur le point
			long indice = this->OnGetLibelleAtPoint(x, y);

			// si on a pas de libellé, on récupère le losange sur le point
			if (indice < 0) indice = this->OnGetLosangeAtPoint(x, y);

			// on met à jour le zoom
			m_pIMapping->OnUpdateZoom(indice);
		}
	}

	// on appelle le gestionnaire de base
	this->JFCView::OnMouseMove(nFlags, point);
}

void JFCMapping::OnDestroy() 
{
	// on appelle le gestionnaire de base
	this->JFCView::OnDestroy();

	// on réinitialise les éléments
	m_pIMapping = NULL;
	m_pICurve   = NULL;

	// on met à jour les longueurs et les grandeurs
	this->OnUpdateLongueurs(); this->OnUpdateGrandeurs();
}

////////////////////////////////////////////////////////////////////
// Calcul Courbe de tendance
void JFCMapping::OnCreateCourbeD1()
{
	CalculCrbTendance1();
}

////////////////////////////////////////////////////////////////////
// Calcul courbe de tendance degré 1
void JFCMapping::CalculCrbTendance1()
{
	// on initialise la courbe
	CTendCurve courbe;

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on recupere le nombre possible de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on initialise l'emplacement de la courbe
	long emplt = 0;

	// on determine l'emplacement de la courbe dans le tableau
	for (emplt; emplt < tailleMax; emplt += 1)
		if (m_pICurve->GetExistCb(emplt) == false) break;

	// on teste si l'emplacement est bon
	if (emplt == tailleMax) return;
	
	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité, la visibilité et la selection de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice) && m_pIMapping->GetSelItem(indice))
		{
			// on lie un point a sa courbe generale de degre 1
			m_pIMapping->SetIsCourbeElt(indice, true);
		
			// on récupere la position de l'élément
			double PosX = m_pIMapping->GetPosXItem(indice);
			double PosY = m_pIMapping->GetPosYItem(indice);

			// on ajoute les coordonnées du point a la courbe
			if (!courbe.AddPt(PosX, PosY)) return;

			// on sauvegarde l'indice du point
			m_pICurve->AddPoint(emplt, indice);
		}
	}

	// on initialise les coefficients de la courbe et de correlation
	double a, b, c, r2;

	// on calcule l'equation de la courbe
	if (courbe.GetEquation(1, a, b, c, r2))
	{
		// on sauvegarde les differents coefficients
		m_pICurve->SetEquation(emplt, a, b, c);
		m_pICurve->SetCoefCorrelation(emplt, r2);

		// on modifie la visibilité de la courbe
		m_pICurve->SetExistCb(emplt, true);

		// on redessine la fenêtre
		this->InvalidateRect(NULL, FALSE);

		// on envoie la mise à jour
		m_pICurve->OnUpdate(2);
	}
}

////////////////////////////////////////////////////////////////////
// Calcul courbe de tendance degré 2
void JFCMapping::OnCreateCourbeD2()
{
	CalculCrbTendance2();
}

////////////////////////////////////////////////////////////////////
// Calcul courbe de tendance de gré 2
void JFCMapping::CalculCrbTendance2()
{
	// on initialise la courbe
	CTendCurve courbe;

	// on récupère le nombre d'éléments
	long taille = m_pIMapping->GetCount();

	// on recupere le nombre possible de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on initialise l'emplacement de la courbe
	long emplt = 0;

	// on determine l'emplacement de la courbe dans le tableau
	for (emplt; emplt < tailleMax; emplt += 1) 
		if (m_pICurve->GetExistCb(emplt) == false) break;

	// on teste si l'emplacement est bon
	if (emplt == tailleMax) return;
		
	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité, la visibilité et la selection de l'élément
		if (m_pIMapping->IsItemValid(indice) && m_pIMapping->GetShowItem(indice) && m_pIMapping->GetSelItem(indice))
		{
			// on lie un point a sa courbe generale de degre 2
			m_pIMapping->SetIsCourbeElt(indice, true);
		
			// on récupere la position de l'élément
			double PosX = m_pIMapping->GetPosXItem(indice);
			double PosY = m_pIMapping->GetPosYItem(indice);

			// on ajoute les coordonnées du point a la courbe
			if (!courbe.AddPt(PosX, PosY)) return;

			// on sauvegarde l'indice du point
			m_pICurve->AddPoint(emplt, indice);
		}
	}

	// on initialise les coefficients de la courbe et de correlation
	double a, b, c, r2;

	// on calcule l'equation de la courbe
	if (courbe.GetEquation(2, a, b, c, r2))
	{
		// on sauvegarde les differents coefficients
		m_pICurve->SetEquation(emplt, a, b, c);
		m_pICurve->SetCoefCorrelation(emplt, r2);

		// on modifie la visibilité de la courbe
		m_pICurve->SetExistCb(emplt, true);

		// on redessine la fenêtre
		this->InvalidateRect(NULL, FALSE);

		// on envoie la mise à jour
		m_pICurve->OnUpdate(2);
	}
}
/////////////////////////////////////////////////////////////////
// Cahce toutes les courbes
void JFCMapping::OnHideAllCourbe()
{
	// on récupère le nombre de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long courbe = 0; courbe < tailleMax; courbe += 1)
	{
		// on modifie l'existence de la courbe
		if (m_pICurve->GetExistCb(courbe) == true) 	
		{
			// on supprime l'existence
			m_pICurve->ResetCourbe(courbe);
			m_LbEqLg[courbe]      =	0;
			m_BarycentreX[courbe] =	0.0;
			m_BarycentreY[courbe] =	0.0;
			m_AccrochageX[courbe] = 0.0;
			m_AccrochageY[courbe] = 0.0;	
		}
	}

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pICurve->OnUpdate(2);
}

////////////////////////////////////////////////////////////
// Cache sélection de courbes
void JFCMapping::OnHideCourbeSel()
{
	// on récupère le nombre de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long courbe = 0; courbe < tailleMax; courbe += 1)
	{
		// on modifie l'existence de la courbe
		if (m_pICurve->GetExistCb(courbe) == true && m_pICurve->GetSelectCb(courbe) == true)	
		{
			// on supprime l'existence
			m_pICurve->ResetCourbe(courbe);
			m_LbEqLg[courbe]      =   0;
			m_BarycentreX[courbe] = 0.0;
			m_BarycentreY[courbe] = 0.0;
			m_AccrochageX[courbe] = 0.0;
			m_AccrochageY[courbe] = 0.0;
		}
	}

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pICurve->OnUpdate(2);
}

////////////////////////////////////////////////////////////////////////
// Affiche les libellés courbes
void JFCMapping::OnShowLibCourbe()
{
	// on récupère le nombre de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long i = 0; i < tailleMax; i += 1)
	{
		// on modifie l'existence de la courbe
		if (m_pICurve->GetExistCb(i) == true) 	
		{
			// on supprime l'equation et le r2
			m_pICurve->SetShowLibelle(i, true);
		}
	}

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pICurve->OnUpdate(1);
}

////////////////////////////////////////////////////////////////////////
// Cache les libellés courbes
void JFCMapping::OnHideLibCourbe()
{
	// on récupère le nombre de courbes
	long tailleMax = m_pICurve->GetNbCourbeMax();

	// on parcourt toutes les courbes
	for (long i = 0; i < tailleMax; i += 1)
	{
		// on modifie l'existence de la courbe
		if (m_pICurve->GetExistCb(i) == true) 	
		{
			// on supprime l'equation et le r2
			m_pICurve->SetShowLibelle(i, false);
		}
	}

	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);

	// on envoie la mise à jour
	m_pICurve->OnUpdate(1);
}
