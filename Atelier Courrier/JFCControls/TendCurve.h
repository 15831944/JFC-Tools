// TendCurve.h:
// 
// 16/09/2002
//
// jérémie
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TENDCURVE_H__9D7A2BFB_90BE_4009_8D42_397FDBDD200A__INCLUDED_)
#define AFX_TENDCURVE_H__9D7A2BFB_90BE_4009_8D42_397FDBDD200A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxtempl.h"
#include "JFCList.h"

class CTendCurve  
{
private:
	// la classe des coordonnees de points
	class Point
	{
	public:
		double m_X;
		double m_Y;
	};

public:
	// le constructeur
	CTendCurve();

	// l'ajout de point
	bool AddPt(double x, double y);

	// la récupération de l'equation
	bool GetEquation(long degre, double& a, double& b, double& c, double& r2); // degré egal 1 ou 2
																		
	// la suppression d'une courbe
	void Reset();

	// le destructeur
	~CTendCurve();

private:
	// les fonctions servant au calcul de l'equation degre 1
	// -----------------------------------------------------
	double GetB(long nb, double xiyi, double xi, double yi, double xi2);
	double GetC(long nb, double xiyi, double xi, double yi, double xi2);

	// les fonctions servant au calcul de l'equation degre 2
	// -----------------------------------------------------
	double GetDeterm(double a0, double b0, double c0, double a1, double b1, double c1, double a2, double b2, long c2);
	double GetA(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2, double d0, double d1, double d2, double delta);
	double GetB(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2, double d0, double d1, double d2, double delta);
	double GetC(double a0, double a1, double a2, double b0, double b1, double b2, double c0, double c1, long c2, double d0, double d1, double d2, double delta);

	// les fonctions servant au calcul du coefficient
	// ---------------------------------------------
	bool   CalculR2(double a, double b, double c, double& r2); 
	bool   GetMoyenne(double a, double b, double c, double& moyYpt, double& moyYcb);

private:
	// le constructeur de recopie
	CTendCurve(const CTendCurve& source);
	
	// l'opérateur d'affectation
	CTendCurve & operator = (const CTendCurve & operande);

private:
	List<Point> m_ListPts; // la liste de points
	double		m_Xi;	   // Sum Xi
	double		m_Xi2;	   // Sum Xi^2
	double		m_Xi3;	   // Sum Xi^3
	double		m_Xi4;	   // Sum Xi^4
	double		m_Yi;	   // Sum yi
	double		m_XiYi;	   // Sum XiYi
	double		m_Xi2Yi;   // Sum Xi^2Yi
};

#endif // !defined(AFX_TENDCURVE_H__9D7A2BFB_90BE_4009_8D42_397FDBDD200A__INCLUDED_)
