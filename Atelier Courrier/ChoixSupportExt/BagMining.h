// BagMining.h: interface for the CBagMining class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAGMINING_H__BEA3CA83_B1C8_475A_8A95_DF6B5132630C__INCLUDED_)
#define AFX_BAGMINING_H__BEA3CA83_B1C8_475A_8A95_DF6B5132630C__INCLUDED_

// on inclut les définitions nécessaires
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

	// Fonction de récupération document
	JVoid SetDocument(JATPDocChoixSupport* pDoc, CDlgMapping *pDlg);

	// Update points mapping
	JBool UpdatePoints(JBool InitPos);
	
	// la fonction pour récupérer le nombre d'élément
	virtual JInt32 GetCount();

	// la fonction pour tester la validité de l'élément
	virtual JBool IsItemValid(JInt32 Index);

	// les fonctions pour récupérer les libellés des grandeurs
	virtual const JString GetLabelXAxe(); 
	virtual const JString GetLabelYAxe();

	// les fonctions pour manipuler la visibilité de l'élément
	virtual JVoid SetShowItem(JInt32 Index, JBool Show);
	virtual JBool GetShowItem(JInt32 Index);
	virtual JVoid SetShowItemById(JUnt32 IdElmt, JBool Show);

	// la fonction pour récupérer le libellé de l'élément
	virtual const JString GetLabelItem(JInt32 Index);

	// les fonctions pour manipuler la sélection de l'élément
	virtual JVoid SetSelItem(JInt32 Index, JBool Selection);
	virtual JBool GetSelItem(JInt32 Index);

	// Mise à jour sélection
	virtual JVoid MajSelSet();

	// Mise à jour libellé
	virtual JVoid MajLibelleSet();

	// la fonction pour manipuler la visibilité du libellé
	virtual JVoid SetShowLabel(JInt32 Index, JBool Show);
	virtual JBool GetShowLabel(JInt32 Index);

	// les fonctions pour récupérer la position de l'élément
	virtual JFlt64 GetPosXItem(JInt32 Index);
	virtual JFlt64 GetPosYItem(JInt32 Index);

	// les fonctions pour manipuler le déplacement horizontal du libellé
	virtual JVoid  SetDxLabel(JInt32 Index, JFlt32 Dx);
	virtual JFlt32 GetDxLabel(JInt32 Index);
	
	// les fonctions pour manipuler le déplacement vertical du libellé
	virtual JVoid  SetDyLabel(JInt32 Index, JFlt32 Dy);
	virtual JFlt32 GetDyLabel(JInt32 Index);

	// fonction pour positionner en X et Y le libellé
	virtual JVoid SetPosLibelle(JInt32 indice, JFlt32 dx, JFlt32 dy);

	/*
	// fonction pour récupérer position en X et Y du libellé
	virtual JFlt64 GetPosXLibelle(JInt32 indice);
	virtual JFlt64 GetPosYLibelle(JInt32 indice);
	*/

	// la fonction pour manipuler l'appartenance a la courbe
	virtual JVoid SetIsCourbeElt(JInt32 indice, JBool etat);
	virtual JBool GetIsCourbeElt(JInt32 indice);

	// les fonctions pour corriger la position des axes
	virtual JFlt64 GetPosXAxe(JFlt64 XAxePos);
	virtual JFlt64 GetPosYAxe(JFlt64 YAxePos);

	// la fonction pour mettre à jour le zoom
	virtual JVoid OnUpdateZoom(JInt32 Index);

	// la fonction pour propager les mises à jour
	virtual JVoid OnUpdate(JInt32 Update);

	// la fonction pour propager les sélections
	virtual JVoid MajSelection();

	// Affichage ou pas de tous les libellés points
	virtual JVoid SetShowAllLibelle(JBool etat);
		
	// récupération état affichage libellés
	virtual JBool GetLibAfficher();

	// positionne état d'affichage des libellés
	virtual JVoid SetLibAfficher(JBool etat);

	// récupére ident élmt
	JUnt32 GetIdent(long indice);

	// Affichage uniquement des points sélectionnés
	virtual JVoid ShowSelItem();

	// Affichage uniquement des points non sélectionnés
	virtual JVoid HideSelItem();

	// Affichage de tous les points
	virtual JVoid ShowAllItem();

	// le destructeur
	virtual ~CBagMining();

protected:
	// Liste des points à traiter
	CListPoint m_Liste;

public:

	// Pointeur sur document en cours
	// JATPDocChoixSupport *m_pDocChoixSupport;
};

#endif // !defined(AFX_BAGMINING_H__BEA3CA83_B1C8_475A_8A95_DF6B5132630C__INCLUDED_)
