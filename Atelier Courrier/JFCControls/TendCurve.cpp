// TendCurve.cpp: implementation of the CTendCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TendCurve.h"
#include "JFCException.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////

CTendCurve::CTendCurve()
{
	// on initialise les sommes
	m_Xi    = 0.0;
	m_Xi2   = 0.0;
	m_Xi3   = 0.0;
	m_Xi4   = 0.0;
	m_Yi    = 0.0;
	m_XiYi  = 0.0;	
	m_Xi2Yi = 0.0;
}

//////////////////////////////////////////////////////////////////////
// le constructeur de recopie:
//////////////////////////////////////////////////////////////////////

CTendCurve::CTendCurve(const CTendCurve & source)
{
	// on declenche une exception si on fait une recopie
	throw Exception();
}

//////////////////////////////////////////////////////////////////////
// l'opérateur d'affectation:
//////////////////////////////////////////////////////////////////////

CTendCurve & CTendCurve::operator = (const CTendCurve & operande)
{
	// on declenche une exception si on fait une affectation
	throw Exception();
}

/////////////////////////////////////////////////////////////////////
// Ajout de point
/////////////////////////////////////////////////////////////////////
bool CTendCurve::AddPt(double x, double y)
{
	try
	{	
		// on definit la coordonnee du point
		Point pt;
		pt.m_X = x;
		pt.m_Y = y;
		
		// on ajoute le point à la liste
		*m_ListPts.AddTail() = pt;

		// on somme avec les autres points
		m_Xi    += x;
		m_Xi2   += x*x;
		m_Xi3   += x*x*x;
		m_Xi4   += x*x*x*x;
		m_XiYi  += x*y; 	
		m_Xi2Yi += x*x*y;
		m_Yi    += y;
	}

	catch (...)
	{
		// l'ajout de point a échoué
		return (false);
	}

	return (true);
}

///////////////////////////////////////////////////////////
// calcul de l'equation de la droite de degre 1 ou 2
/////////////////////////////////////////////////////////// 

bool CTendCurve::GetEquation(long degre, double& a, double& b, double& c, double& r2)
{
	// on teste la methode
	if (degre < 1 || degre > 2) throw Exception();

	// on recupere le nombre de points de la liste
	long Ni = m_ListPts.GetSize();

	// on initialise les variables
	double aTmp, bTmp, cTmp, r2Tmp;

	// on determine le degre de l'equation
	if      (degre == 1)
	{
		// on teste le nombre 
		if (Ni < 2) return (false);

		// on teste le denominateur
		if (((double)Ni * m_Xi2) - (m_Xi * m_Xi) == 0) return (false);

		aTmp = 0.0;
		bTmp = this->GetB(Ni, m_XiYi, m_Xi, m_Yi, m_Xi2);
		cTmp = this->GetC(Ni, m_XiYi, m_Xi, m_Yi, m_Xi2);
	}
	else if (degre == 2)
	{
		// on teste le nombre 
		if (Ni < 3) return (false);

		// on calcule le determinant
		double delta = this->GetDeterm(m_Xi4, m_Xi3, m_Xi2, m_Xi3, m_Xi2, m_Xi, m_Xi2, m_Xi, Ni);

		// on en deduit a, b, c en testant le determinant
		if (delta == 0.0) return (false);

		// on utilise le calcul normal de a, b, c
		aTmp = this->GetA(  0.0,   0.0,   0.0, m_Xi3, m_Xi2, m_Xi, m_Xi2, m_Xi,  Ni, m_Xi2Yi, m_XiYi, m_Yi, delta);
		bTmp = this->GetB(m_Xi4, m_Xi3, m_Xi2,   0.0,   0.0,  0.0, m_Xi2, m_Xi,  Ni, m_Xi2Yi, m_XiYi, m_Yi, delta);
		cTmp = this->GetC(m_Xi4, m_Xi3, m_Xi2, m_Xi3, m_Xi2, m_Xi,   0.0,  0.0,   0, m_Xi2Yi, m_XiYi, m_Yi, delta);
	}

	// on calcul R2
	if (!this->CalculR2(aTmp, bTmp, cTmp, r2Tmp)) return (false);

	// on recupere les inconnues de la droite
	a = aTmp;	b = bTmp;	c = cTmp; r2 = r2Tmp;

	return (true);
}

///////////////////////////////////////////////////////////
// calcul de b et c degre 1 -> y = bx + c
///////////////////////////////////////////////////////////

double CTendCurve::GetB(long nb, double xiyi, double xi, double yi, double xi2)
{
	// on calcul b
	double b = (((double)nb * xiyi) - (xi * yi)) / (((double)nb * xi2) - (xi * xi));

	// on retourne b
	return (b);
}

double CTendCurve::GetC(long nb, double xiyi, double xi, double yi, double xi2)
{
	// on calcul c
	double c = ((yi * xi2) - (xi * xiyi)) / (((double)nb * xi2) - (xi * xi));

	// on retourne c
	return (c);
}

///////////////////////////////////////////////////////////
// calcul de a, b et c degre 2 -> y = ax2 + bx + c
///////////////////////////////////////////////////////////

double CTendCurve::GetA(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2, double d0, double d1, double d2, double delta)
{
	// on calcul a
	double a = (d0*(b1*(double)c2 - c1*b2) - b0*(d1*(double)c2 - c1*d2) + c0*(d1*b2 - b1*d2)) / delta;
	
	// on retourne a
	return (a);
}

double CTendCurve::GetB(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2, double d0, double d1, double d2, double delta)
{
	// on calcul b
	double b = (a0*(d1*(double)c2 - c1*d2) - d0*(a1*(double)c2 - c1*a2) + c0*(a1*d2 - d1*a2)) / delta;

	// on retourne b
	return (b);
}

double CTendCurve::GetC(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2, double d0, double d1, double d2, double delta)
{
	// on calcul c
	double c = (a0*(b1*d2 - d1*b2) - b0*(a1*d2 - d1*a2) + d0*(a1*b2 - b1*a2)) / delta;

	// on retourne c
	return (c);
}

////////////////////////////////////////////////////////////
// calcul du determinant degre 2
///////////////////////////////////////////////////////////

double CTendCurve::GetDeterm(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2)
{
	// on calcul le determinant
	double delta = a0*(b1*(double)c2 - c1*b2) - b0*(a1*(double)c2 - c1*a2) + c0*(a1*b2 - b1*a2);

	// on retourne le determinant
	return (delta);
}

//////////////////////////////////////////////////////////
// Calcul du coefficient de correlation
//////////////////////////////////////////////////////////

bool CTendCurve::CalculR2(double a, double b, double c, double& r2)
{
	double moyYpt, moyYcb;

	// on calcul les moyennes
	if (!this->GetMoyenne(a, b, c, moyYpt, moyYcb)) return (false);

	// on initialise variance et covariance
	double covarYptYcb, varYpt, varYcb;
	covarYptYcb = varYpt = varYcb = 0.0;

	// on se place sur le premier point
	m_ListPts.MoveFirst();
	
	// on boucle sur tous les points
	while (m_ListPts.IsValid())
	{
		// on calcul ensuite la covariance
		covarYptYcb = covarYptYcb + (m_ListPts.GetItem()->m_Y - moyYpt) * (a * m_ListPts.GetItem()->m_X * m_ListPts.GetItem()->m_X + b * m_ListPts.GetItem()->m_X + c - moyYcb);

		// et les variances
		varYpt = varYpt + (m_ListPts.GetItem()->m_Y - moyYpt)*(m_ListPts.GetItem()->m_Y - moyYpt);
		varYcb = varYcb + (a * m_ListPts.GetItem()->m_X * m_ListPts.GetItem()->m_X + b * m_ListPts.GetItem()->m_X + c - moyYcb)*(a * m_ListPts.GetItem()->m_X * m_ListPts.GetItem()->m_X + b * m_ListPts.GetItem()->m_X + c - moyYcb);

		// on passe a la position suivante
		m_ListPts.MoveNext();
	}

	// on calcul le produit des variances
	double var = varYpt * varYcb;

	// on teste si exclusivement l'une des variances est negative
	if (var <= 0.0) return (false);

	// on en deduit le coef de correlation
	r2 = covarYptYcb / sqrt(var);

	return (true);
}

/////////////////////////////////////////////////////////
// Calcul de la moyenne
/////////////////////////////////////////////////////////

bool CTendCurve::GetMoyenne(double a, double b, double c, double& moyYpt, double& moyYcb)
{
	// on teste la liste
	if (m_ListPts.GetSize() == 0) return (false);

	// on initialise les moyennes et les sommes
	double sumYpt, sumYcb;
	
	sumYpt = sumYcb = 0.0;
	moyYpt = moyYcb = 0.0;

	// on se place sur le premier point
	m_ListPts.MoveFirst();
	
	// on boucle sur tous les points
	while (m_ListPts.IsValid())
	{
		// on somme les ordonnées des points
		sumYpt = sumYpt + (double)m_ListPts.GetItem()->m_Y;

		// on somme les ordonnes de la courbe
		sumYcb = sumYcb + (a * (double)m_ListPts.GetItem()->m_X * (double)m_ListPts.GetItem()->m_X + b * (double)m_ListPts.GetItem()->m_X + c);
		
		// on passe a la position suivante
		m_ListPts.MoveNext();
	}

	// on determine les moyennes
	moyYpt = sumYpt / (double)m_ListPts.GetSize();
	moyYcb = sumYcb / (double)m_ListPts.GetSize();

	return (true);
}

//////////////////////////////////////////////////////////////////////
// la suppression d'une courbe
//////////////////////////////////////////////////////////////////////

void CTendCurve::Reset()
{
	// on reinitialise la liste de points
	m_ListPts.Reset();

	// on reinitialise les infos de la courbes
	m_Xi = m_Xi2 = m_Xi3 = m_Xi4 = m_XiYi = m_Xi2Yi = m_Yi = 0.0;
}

//////////////////////////////////////////////////////////////////////
//Destruction
//////////////////////////////////////////////////////////////////////

CTendCurve::~CTendCurve()
{
	// on ne fait rien
}