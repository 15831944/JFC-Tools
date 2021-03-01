// BagCurve.h: interface for the BagCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAGCURVE_H__5A67E37F_13C7_4D0F_9314_EB9CBBDEC0B4__INCLUDED_)
#define AFX_BAGCURVE_H__5A67E37F_13C7_4D0F_9314_EB9CBBDEC0B4__INCLUDED_

#include "JFCICurve.h"
#include "TendCurve.h"
#include "JFCArray.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// le nombre de courbe par default
static const int NB_COURBES = 5;
 
class CBagCurve  : public JFCICurve
{
protected:
	class InfosCourbe
	{
	public:		
		double		m_A;		// le degré 2
		double		m_B;		// le degré 1
		double		m_C;		// le degré 0 
		double		m_R2;		// le coef de correlation
		JList<long>	m_ListIdx;  // la liste de points	
		bool		m_Existe;	// l'existence de la courbe
		bool		m_Select;	// la selection de la courbe
		bool		m_Lib;		// la visibilité du libellé
		double		m_dX;		// la position X du libelle
		double		m_dY;		// la position Y du libelle
		double		m_PosX;		// l'ancrage X a la courbe
		double		m_PosY;		// l'ancrage Y a la courbe
	};

public:
	// constructeur
	CBagCurve();

	// recuperation du nombre de courbes maximum
	long GetNbCourbeMax();

	// recuperation du nombre de courbes 
	long GetNbCourbe();
	
	// ajout d'un point
	void AddPoint(long indice, long point);

	// coefficient de correlation
	void   SetCoefCorrelation(long indice, double r2);
	double GetCoefCorrelation(long indice);

	// equation de la courbe
	bool SetEquation(long indice, double& a, double& b, double& c);
	bool GetEquation(long indice, double& a, double& b, double& c);

	// le nombre de points d'une courbe
	long GetNbPointsCourbe(long indice);

	// l'indice d'un point d'une courbe
	JList<long>* GetListePointCourbe(long indice);
	
	// la suppression d'une courbe
	void ResetCourbe(long indice);

	// l'existence des courbes
	void SetExistCb(long indice, bool etat);
	bool GetExistCb(long indice);
	
	// la selection des courbes
	void SetSelectCb(long indice, bool etat);
	bool GetSelectCb(long indice);
	
	// la visibilité des equations
	void SetShowLibelle(long indice, bool etat);
	bool GetShowLibelle(long indice);

	// les fonctions pour récupérer la position de l'élément
	double GetXPosition(long indice);
	double GetYPosition(long indice);

	// les fonctions pour manipuler le déplacement horizontal du libellé
	void  SetDxLibelle(long indice, float dx);
	float GetDxLibelle(long indice);
	
	// les fonctions pour manipuler le déplacement vertical du libellé
	void  SetDyLibelle(long indice, float dy);
	float GetDyLibelle(long indice);

	// la fonction pour propager les mises à jour
	virtual void OnUpdate(long raison);

	// destructeur
	~CBagCurve();

protected:
	Array<InfosCourbe>	m_TabCurve;		// le tableau de courbes
};

#endif // !defined(AFX_BAGCURVE_H__5A67E37F_13C7_4D0F_9314_EB9CBBDEC0B4__INCLUDED_)
