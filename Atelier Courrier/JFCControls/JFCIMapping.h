#if !defined(AFX_JFCIMAPPING_H__D77A3FA9_AEFB_4CDC_94FB_FAA1EE294AB3__INCLUDED_)
#define AFX_JFCIMAPPING_H__D77A3FA9_AEFB_4CDC_94FB_FAA1EE294AB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class AFX_EXT_CLASS JFCIMapping  
{
public:
	// le constructeur
	JFCIMapping();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	virtual JInt32 GetCount() = 0;

	// la fonction pour r�cup�rer la validit� des �l�ments
	virtual JBool IsItemValid(JInt32 Index) = 0;

	// les fonctions pour r�cup�rer les libell�s des grandeurs
	virtual const JString GetLabelXAxe() = 0;
	virtual const JString GetLabelYAxe() = 0;

	// les fonctions pour manipuler la visibilit� des �l�ments
	virtual JVoid SetShowItem(JInt32 Index, JBool Show) = 0;
	virtual JBool GetShowItem(JInt32 Index) = 0;

	// la fonction pour r�cup�rer le libell� des �l�ments
	virtual const JString GetLabelItem(JInt32 Index) = 0;

	// les fonctions pour manipuler la s�lection des �l�ments
	virtual JVoid SetSelItem(JInt32 Index, JBool Selection) = 0;
	virtual JBool GetSelItem(JInt32 Index) = 0;

	// Mise � jour s�lection
	virtual JVoid MajSelSet() = 0;

	// Mise � jour libell�
	virtual JVoid MajLibelleSet() = 0;

	// les fonctions pour manipuler la visibilit� des libell�s
	virtual JVoid SetShowLabel(JInt32 Index, JBool Show) = 0;
	virtual JBool GetShowLabel(JInt32 Index) = 0;

	// les fonctions pour r�cup�rer la position des �l�ments
	virtual JFlt64 GetPosXItem(JInt32 Index) = 0;
	virtual JFlt64 GetPosYItem(JInt32 Index) = 0;

	// les fonctions pour manipuler la position horizontale des libell�s
	virtual JVoid  SetDxLabel(JInt32 Index, JFlt32 Dx) = 0;
	virtual JFlt32 GetDxLabel(JInt32 Index) = 0;

	// les fonctions pour manipuler la position verticale des libell�s
	virtual JVoid  SetDyLabel(JInt32 Index, JFlt32 Dy) = 0;
	virtual JFlt32 GetDyLabel(JInt32 Index) = 0;

	// fonction pour positionner en X et Y le libell�
	virtual JVoid SetPosLibelle(JInt32 indice, JFlt32 dx, JFlt32 dy) = 0;

	/*
	// fonction pour r�cup�rer position en X et Y du libell�
	virtual JFlt64 GetPosXLibelle(JInt32 indice) = 0;
	virtual JFlt64 GetPosYLibelle(JInt32 indice) = 0;
	*/

	// la fonction pour manipuler l'appartenance a la courbe
	virtual JVoid SetIsCourbeElt(JInt32 indice, JBool etat) = 0;
	virtual JBool GetIsCourbeElt(JInt32 indice) = 0;

	// les fonctions pour corriger la position des axes
	virtual JFlt64 GetPosXAxe(JFlt64 XAxePos) = 0;
	virtual JFlt64 GetPosYAxe(JFlt64 YAxePos) = 0;

	// la fonction pour mettre � jour le zoom
	virtual JVoid OnUpdateZoom(JInt32 Index) = 0;

	// la fonction pour propager les mises � jour
	virtual JVoid OnUpdate(JInt32 Update) = 0;

	// la fonction pour propager les s�lections
	virtual JVoid MajSelection() = 0;

	// Affichage ou pas de tous les libell�s points
	virtual JVoid SetShowAllLibelle(JBool etat) = 0;
		
	// r�cup�ration �tat affichage libell�s
	virtual JBool GetLibAfficher() = 0;

	// positionne �tat d'affichage des libell�s
	virtual JVoid SetLibAfficher(JBool etat) = 0;

	// Affichage uniquement des points s�lectionn�s
	virtual JVoid ShowSelItem() = 0;

	// Affichage uniquement des points non s�lectionn�s
	virtual JVoid HideSelItem() = 0;

	// Affichage de tous les points
	virtual JVoid ShowAllItem() = 0;

	// le destructeur
	virtual ~JFCIMapping();
};

#endif // !defined(AFX_JFCIMAPPING_H__D77A3FA9_AEFB_4CDC_94FB_FAA1EE294AB3__INCLUDED_)
