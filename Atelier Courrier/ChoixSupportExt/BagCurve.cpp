// BagCurve.cpp: implementation of the BagCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MAPPY.h"
#include "BagCurve.h"
#include "JFCException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBagCurve::CBagCurve()
{
	// on initialise la taille du tableau par default
	m_TabCurve    .SetSize(NB_COURBES);

	// on définit par default les courbes
	for (long indice = 0; indice < NB_COURBES; indice += 1) this->ResetCourbe(indice);
}

CBagCurve::~CBagCurve()
{
	// on ne fait rien
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// ajout d'un point
//////////////////////////////////////////////////////////////////////

void CBagCurve::AddPoint(long indice, long point)
{
	// on sauvegarde l'indice du points
	m_TabCurve[indice].m_ListIdx.AddTail() = point;
}

//////////////////////////////////////////////////////////////////////
// la suppression d'une courbe
//////////////////////////////////////////////////////////////////////
void CBagCurve::ResetCourbe(long indice)
{
	m_TabCurve[indice].m_ListIdx.Reset();
	m_TabCurve[indice].m_Existe = false;
	m_TabCurve[indice].m_Select = false;
	m_TabCurve[indice].m_Lib    = false;
	m_TabCurve[indice].m_dX		= 0.00;
	m_TabCurve[indice].m_dY		= 0.00;
}

//////////////////////////////////////////////////////////////////////
// coefficient de correlation
//////////////////////////////////////////////////////////////////////

void CBagCurve::SetCoefCorrelation(long indice, double r2)
{
	// on stocke le coef de correlation
	m_TabCurve[indice].m_R2 = r2;
}

double CBagCurve::GetCoefCorrelation(long indice)
{

	// on renvoie le coef de correlation
	return (m_TabCurve[indice].m_R2);
}

//////////////////////////////////////////////////////////////////////
// equation de la courbe
//////////////////////////////////////////////////////////////////////

bool CBagCurve::SetEquation(long indice, double& a, double& b, double& c)
{
	// on stocke les coefficients
	m_TabCurve[indice].m_A = a;
	m_TabCurve[indice].m_B = b;
	m_TabCurve[indice].m_C = c;

	return (true);
}

bool CBagCurve::GetEquation(long indice, double& a, double& b, double& c)
{
	// on recupere les coefficients
	a = m_TabCurve[indice].m_A;
	b = m_TabCurve[indice].m_B;
	c = m_TabCurve[indice].m_C;

	return (true);
}

//////////////////////////////////////////////////////////////////////
// recuperation du nombre de courbes maximum
//////////////////////////////////////////////////////////////////////

long CBagCurve::GetNbCourbeMax()
{
	// on recupere la taille du tableau
	return (m_TabCurve.GetSize());
}

//////////////////////////////////////////////////////////////////////
// recuperation du nombre de courbes 
//////////////////////////////////////////////////////////////////////

long CBagCurve::GetNbCourbe()
{
	// on recupere la taille du tableau
	long taille = m_TabCurve.GetSize();

	long nombre = 0;

	// on determine le nombre de courbes
	for (long i = 0; i < taille; i+= 1) if ( m_TabCurve[i].m_Existe == true) nombre += 1;
	
	return (nombre);
}

//////////////////////////////////////////////////////////////////////
// la visibilité des courbes
//////////////////////////////////////////////////////////////////////

void CBagCurve::SetExistCb(long indice, bool etat)
{
	m_TabCurve[indice].m_Existe = etat;
}

bool CBagCurve::GetExistCb(long indice)
{
	// on teste le numero de la courbe
	if (indice < 0 || indice > NB_COURBES) throw Exception();
	
	return (m_TabCurve[indice].m_Existe);
}

//////////////////////////////////////////////////////////////////////
// la selection des courbes
//////////////////////////////////////////////////////////////////////

void CBagCurve::SetSelectCb(long indice, bool etat)
{
	m_TabCurve[indice].m_Select = etat;
}


bool CBagCurve::GetSelectCb(long indice)
{
	// on teste le numero de la courbe
	if (indice < 0 || indice > NB_COURBES) throw Exception();
	
	return (m_TabCurve[indice].m_Select);
}

//////////////////////////////////////////////////////////////////////
// la visibilité des libelles
//////////////////////////////////////////////////////////////////////

void CBagCurve::SetShowLibelle(long indice, bool etat)
{
	m_TabCurve[indice].m_Lib = etat;
}


bool CBagCurve::GetShowLibelle(long indice)
{
	// on teste le numero de la courbe
	if (indice < 0 || indice > NB_COURBES) throw Exception();
	
	return (m_TabCurve[indice].m_Lib);
}


//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le déplacement horizontal du libellé
//////////////////////////////////////////////////////////////////////

void CBagCurve::SetDxLibelle(long indice, float dx)
{
	m_TabCurve[indice].m_dX = (double)dx;
}

float CBagCurve::GetDxLibelle(long indice)
{
	// on renvoie le déplacement
	return ((float)m_TabCurve[indice].m_dX);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le déplacement vertical du libellé
//////////////////////////////////////////////////////////////////////

void CBagCurve::SetDyLibelle(long indice, float dy)
{
	m_TabCurve[indice].m_dY = (double)dy;
}

float CBagCurve::GetDyLibelle(long indice)
{
	// on renvoie le déplacement
	return ((float)m_TabCurve[indice].m_dY);
}

//////////////////////////////////////////////////////////////////////
// le nombre de points d'une courbe
//////////////////////////////////////////////////////////////////////

long CBagCurve::GetNbPointsCourbe(long indice)
{
	return (m_TabCurve[indice].m_ListIdx.GetCount());
}

//////////////////////////////////////////////////////////////////////
// le nombre de points d'une courbe
//////////////////////////////////////////////////////////////////////
JList<long>* CBagCurve::GetListePointCourbe(long indice)
{
	return(&m_TabCurve[indice].m_ListIdx);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer la position de l'élément
//////////////////////////////////////////////////////////////////////

double CBagCurve::GetXPosition(long indice)
{
	return (m_TabCurve[indice].m_PosX);
}

double CBagCurve::GetYPosition(long indice)
{
	return (m_TabCurve[indice].m_PosY);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour propager les mises à jour
//////////////////////////////////////////////////////////////////////

void CBagCurve::OnUpdate(long raison)
{

}
