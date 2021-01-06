#if !defined(AFX_JFCICURVE_H__9C4B21AD_C856_4451_BA65_EB88E2483C63__INCLUDED_)
#define AFX_JFCICURVE_H__9C4B21AD_C856_4451_BA65_EB88E2483C63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JLib.h"

class AFX_EXT_CLASS JFCICurve  
{
public:
	// le constructeur
	JFCICurve();

	// ajout d'un point
	virtual JVoid AddPoint(JInt32 numCb, JInt32 indice) = 0;

	// la suppression d'une courbe
	virtual JVoid ResetCourbe(JInt32 numCb) = 0;

	// coefficient de correlation
	virtual JVoid  SetCoefCorrelation(JInt32 numCb, JFlt64 r2) = 0;
	virtual JFlt64 GetCoefCorrelation(JInt32 numCb) = 0;

	// equation de la courbe
	virtual JBool SetEquation(JInt32 numCb, JFlt64 & a, JFlt64 & b, JFlt64 & c) = 0;
	virtual JBool GetEquation(JInt32 numCb, JFlt64 & a, JFlt64 & b, JFlt64 & c) = 0;

	// nombre de courbes maximum
	virtual JInt32 GetNbCourbeMax() = 0;

	// nombre de courbes
	virtual JInt32 GetNbCourbe() = 0;

	// le nombre de points d'une courbe
	virtual JInt32 GetNbPointsCourbe(JInt32 numCb) = 0;
	
	// la liste d'indices
	virtual JList<JInt32>* GetListePointCourbe(JInt32 numCb) = 0;

	// la visibilité des courbes
	virtual JVoid SetExistCb(JInt32 numCb, JBool etat) = 0;
	virtual JBool GetExistCb(JInt32 numCb) = 0;

	// la selection des courbes
	virtual JVoid SetSelectCb(JInt32 numCb, JBool etat) = 0;
	virtual JBool GetSelectCb(JInt32 numCb) = 0;

	// la visibilité des equations
	virtual JVoid SetShowLibelle(JInt32 numCb, JBool etat) = 0;
	virtual JBool GetShowLibelle(JInt32 numCb) = 0;

	// les fonctions pour récupérer la position de l'élément
	virtual JFlt64 GetXPosition(JInt32 indice) = 0;
	virtual JFlt64 GetYPosition(JInt32 indice) = 0;

	// les fonctions pour manipuler le déplacement horizontal du libellé
	virtual JVoid  SetDxLibelle(JInt32 indice, JFlt32 dx) = 0;
	virtual JFlt32 GetDxLibelle(JInt32 indice) = 0;
	
	// les fonctions pour manipuler le déplacement vertical du libellé
	virtual JVoid  SetDyLibelle(JInt32 indice, JFlt32 dy) = 0;
	virtual JFlt32 GetDyLibelle(JInt32 indice) = 0;
	
	// la fonction pour propager les mises à jour
	virtual JVoid OnUpdate(JInt32 raison) = 0;

	// le destructeur
	virtual ~JFCICurve();
};

#endif // !defined(AFX_JFCICURVE_H__9C4B21AD_C856_4451_BA65_EB88E2483C63__INCLUDED_)
