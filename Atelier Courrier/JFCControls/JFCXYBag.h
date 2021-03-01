// JFCXYBag.h: interface for the JFCXYBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCXYBAG_H__0F478073_A0AC_42EE_9601_568B6DA7623B__INCLUDED_)
#define AFX_JFCXYBAG_H__0F478073_A0AC_42EE_9601_568B6DA7623B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS JFCXYBag
{
public:
	// le constructeur
	JFCXYBag();

	// la fonction pour récupérer le nombre d'élément
	virtual long GetNbItems() = 0;

	// la fonction pour tester la validité de l'élément
	virtual bool IsValid(long indice) = 0;

	// les fonctions pour récupérer le libellé des grandeurs
	virtual CString GetXGrandeur() = 0;
	virtual CString GetYGrandeur() = 0;

	// la fonction pour récupérer le libellé de l'élément
	virtual CString GetLibelle(long indice) = 0;

	// les fonctions pour manipuler la visibilité de l'élément
	virtual void SetShow(long indice, bool etat) = 0;
	virtual bool GetShow(long indice) = 0;

	// les fonctions pour manipuler la sélection de l'élément
	virtual void SetSelection(long indice, bool etat) = 0;
	virtual bool GetSelection(long indice) = 0;

	// la fonction pour manipuler la visibilité du libellé
	virtual void SetShowLibelle(long indice, bool etat) = 0;
	virtual bool GetShowLibelle(long indice) = 0;

	// les fonctions pour récupérer la position de l'élément
	virtual double GetXPosition(long indice) = 0;
	virtual double GetYPosition(long indice) = 0;

	// les fonctions pour manipuler le déplacement horizontal du libellé
	virtual void  SetDxLibelle(long indice, float dx) = 0;
	virtual float GetDxLibelle(long indice) = 0;
	
	// les fonctions pour manipuler le déplacement vertical du libellé
	virtual void  SetDyLibelle(long indice, float dy) = 0;
	virtual float GetDyLibelle(long indice) = 0;

	// fonction positionnement en X et Y
	virtual void  SetPosLibelle(long indice, float dx, float dy) = 0;

	// fonction récupération position en X et Y du libellé
	virtual double GetPosXLibelle(long indice) = 0;
	virtual double GetPosYLibelle(long indice) = 0;

	// la fonction pour manipuler l'appartenance a la courbe generale
	virtual void SetIsCourbeElt(long indice, bool etat) = 0;
	virtual bool GetIsCourbeElt(long indice) = 0;

	// la fonction pour mettre à jour le zoom
	virtual void OnUpdateZoom(long indice) = 0;

	// la fonction pour propager les mises à jour
	virtual void OnUpdate(long raison) = 0;

	// la fonction pour propager les sélections
	virtual void MajSelection() = 0;


	// Affichage ou pas de tous les libellés points
	virtual void SetShowAllLibelle(bool etat) = 0;
		
	// récupération état affichage libellés
	virtual bool GetLibAfficher() = 0;

	// positionne état d'affichage des libellés
	virtual void SetLibAfficher(bool etat) = 0;

	// le destructeur
	virtual ~JFCXYBag();
};

#endif // !defined(AFX_JFCXYBAG_H__0F478073_A0AC_42EE_9601_568B6DA7623B__INCLUDED_)
