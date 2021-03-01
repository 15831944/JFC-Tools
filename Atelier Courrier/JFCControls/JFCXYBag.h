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

	// la fonction pour r�cup�rer le nombre d'�l�ment
	virtual long GetNbItems() = 0;

	// la fonction pour tester la validit� de l'�l�ment
	virtual bool IsValid(long indice) = 0;

	// les fonctions pour r�cup�rer le libell� des grandeurs
	virtual CString GetXGrandeur() = 0;
	virtual CString GetYGrandeur() = 0;

	// la fonction pour r�cup�rer le libell� de l'�l�ment
	virtual CString GetLibelle(long indice) = 0;

	// les fonctions pour manipuler la visibilit� de l'�l�ment
	virtual void SetShow(long indice, bool etat) = 0;
	virtual bool GetShow(long indice) = 0;

	// les fonctions pour manipuler la s�lection de l'�l�ment
	virtual void SetSelection(long indice, bool etat) = 0;
	virtual bool GetSelection(long indice) = 0;

	// la fonction pour manipuler la visibilit� du libell�
	virtual void SetShowLibelle(long indice, bool etat) = 0;
	virtual bool GetShowLibelle(long indice) = 0;

	// les fonctions pour r�cup�rer la position de l'�l�ment
	virtual double GetXPosition(long indice) = 0;
	virtual double GetYPosition(long indice) = 0;

	// les fonctions pour manipuler le d�placement horizontal du libell�
	virtual void  SetDxLibelle(long indice, float dx) = 0;
	virtual float GetDxLibelle(long indice) = 0;
	
	// les fonctions pour manipuler le d�placement vertical du libell�
	virtual void  SetDyLibelle(long indice, float dy) = 0;
	virtual float GetDyLibelle(long indice) = 0;

	// fonction positionnement en X et Y
	virtual void  SetPosLibelle(long indice, float dx, float dy) = 0;

	// fonction r�cup�ration position en X et Y du libell�
	virtual double GetPosXLibelle(long indice) = 0;
	virtual double GetPosYLibelle(long indice) = 0;

	// la fonction pour manipuler l'appartenance a la courbe generale
	virtual void SetIsCourbeElt(long indice, bool etat) = 0;
	virtual bool GetIsCourbeElt(long indice) = 0;

	// la fonction pour mettre � jour le zoom
	virtual void OnUpdateZoom(long indice) = 0;

	// la fonction pour propager les mises � jour
	virtual void OnUpdate(long raison) = 0;

	// la fonction pour propager les s�lections
	virtual void MajSelection() = 0;


	// Affichage ou pas de tous les libell�s points
	virtual void SetShowAllLibelle(bool etat) = 0;
		
	// r�cup�ration �tat affichage libell�s
	virtual bool GetLibAfficher() = 0;

	// positionne �tat d'affichage des libell�s
	virtual void SetLibAfficher(bool etat) = 0;

	// le destructeur
	virtual ~JFCXYBag();
};

#endif // !defined(AFX_JFCXYBAG_H__0F478073_A0AC_42EE_9601_568B6DA7623B__INCLUDED_)
