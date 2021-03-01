// BagMining.h: interface for the CBagMining class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAGMINING_H__BEA3CA83_B1C8_475A_8A95_DF6B5132630C__INCLUDED_)
#define AFX_BAGMINING_H__BEA3CA83_B1C8_475A_8A95_DF6B5132630C__INCLUDED_

// on inclut les d�finitions n�cessaires
#include "JFCIMapping.h"
#include "ListPoint.h"
#include "JATPDocChoixSupport.h"
#include "JLib.h"

class CDlgMapping;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBagMining : public JFCIMapping
{
public:
	
	// le constructeur
	CBagMining();

	// Fonction de r�cup�ration document
	JVoid SetDocument(JATPDocChoixSupport* pDoc, CDlgMapping *pDlg);

	// Update points mapping
	JBool UpdatePoints(JBool InitPos);
	
	// la fonction pour r�cup�rer le nombre d'�l�ment
	virtual JInt32 GetCount();

	// la fonction pour tester la validit� de l'�l�ment
	virtual JBool IsItemValid(JInt32 Index);

	// les fonctions pour r�cup�rer les libell�s des grandeurs
	virtual const JString GetLabelXAxe(); 
	virtual const JString GetLabelYAxe();

	// les fonctions pour manipuler la visibilit� de l'�l�ment
	virtual JVoid SetShowItem(JInt32 Index, JBool Show);
	virtual JBool GetShowItem(JInt32 Index);
	virtual JVoid SetShowItemById(JUnt32 IdElmt, JBool Show);

	// la fonction pour r�cup�rer le libell� de l'�l�ment
	virtual const JString GetLabelItem(JInt32 Index);

	// les fonctions pour manipuler la s�lection de l'�l�ment
	virtual JVoid SetSelItem(JInt32 Index, JBool Selection);
	virtual JBool GetSelItem(JInt32 Index);

	// Mise � jour s�lection
	virtual JVoid MajSelSet();

	// Mise � jour libell�
	virtual JVoid MajLibelleSet();

	// la fonction pour manipuler la visibilit� du libell�
	virtual JVoid SetShowLabel(JInt32 Index, JBool Show);
	virtual JBool GetShowLabel(JInt32 Index);

	// les fonctions pour r�cup�rer la position de l'�l�ment
	virtual JFlt64 GetPosXItem(JInt32 Index);
	virtual JFlt64 GetPosYItem(JInt32 Index);

	// les fonctions pour manipuler le d�placement horizontal du libell�
	virtual JVoid  SetDxLabel(JInt32 Index, JFlt32 Dx);
	virtual JFlt32 GetDxLabel(JInt32 Index);
	
	// les fonctions pour manipuler le d�placement vertical du libell�
	virtual JVoid  SetDyLabel(JInt32 Index, JFlt32 Dy);
	virtual JFlt32 GetDyLabel(JInt32 Index);

	// fonction pour positionner en X et Y le libell�
	virtual JVoid SetPosLibelle(JInt32 indice, JFlt32 dx, JFlt32 dy);

	/*
	// fonction pour r�cup�rer position en X et Y du libell�
	virtual JFlt64 GetPosXLibelle(JInt32 indice);
	virtual JFlt64 GetPosYLibelle(JInt32 indice);
	*/

	// la fonction pour manipuler l'appartenance a la courbe
	virtual JVoid SetIsCourbeElt(JInt32 indice, JBool etat);
	virtual JBool GetIsCourbeElt(JInt32 indice);

	// les fonctions pour corriger la position des axes
	virtual JFlt64 GetPosXAxe(JFlt64 XAxePos);
	virtual JFlt64 GetPosYAxe(JFlt64 YAxePos);

	// la fonction pour mettre � jour le zoom
	virtual JVoid OnUpdateZoom(JInt32 Index);

	// la fonction pour propager les mises � jour
	virtual JVoid OnUpdate(JInt32 Update);

	// la fonction pour propager les s�lections
	virtual JVoid MajSelection();

	// Affichage ou pas de tous les libell�s points
	virtual JVoid SetShowAllLibelle(JBool etat);
		
	// r�cup�ration �tat affichage libell�s
	virtual JBool GetLibAfficher();

	// positionne �tat d'affichage des libell�s
	virtual JVoid SetLibAfficher(JBool etat);

	// r�cup�re ident �lmt
	JUnt32 GetIdent(long indice);

	// Affichage uniquement des points s�lectionn�s
	virtual JVoid ShowSelItem();

	// Affichage uniquement des points non s�lectionn�s
	virtual JVoid HideSelItem();

	// Affichage de tous les points
	virtual JVoid ShowAllItem();

	// le destructeur
	virtual ~CBagMining();

protected:
	// Liste des points � traiter
	CListPoint m_Liste;

public:

	// Pointeur sur document en cours
	// JATPDocChoixSupport *m_pDocChoixSupport;
};

#endif // !defined(AFX_BAGMINING_H__BEA3CA83_B1C8_475A_8A95_DF6B5132630C__INCLUDED_)
