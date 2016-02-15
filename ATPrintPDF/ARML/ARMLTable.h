// ARMLTable.h: interface for the CTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMLTABLE_H__21E0D24C_A41E_4DC5_A72F_80B909F9C3C3__INCLUDED_)
#define AFX_ARMLTABLE_H__21E0D24C_A41E_4DC5_A72F_80B909F9C3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AfxTempl.h"
#include "ARMLField.h"
#include "ARMLHStroke.h"
#include "ARMLTabCell.h"
#include "ARMLTabPave.h"
#include "./../Composite/JFCDrawComposite.h"


class CTable
{
public:
	// le constructeur
	CTable();

	// le constructeur de recopie
	CTable(const CTable & source);

	// l'opérateur d'affectation
	CTable & operator = (const CTable & operande);

	// la fonction pour ajouter une cellule au tableau
	void AddTabCell(CTabCell & tabcell);

	// la fonction pour ajouter une cellule au tableau
	void AddTabPave(CTabPave & tabpave);

	// la fonction pour ajouter une cellule au tableau pour la page suivante
	void AddTabPaveNext(CTabPave & tabpave);

	// la fonction pour ajouter un trait
	void AddHStroke(CHStroke & stroke);

	// la fonction pour créer les commandes du tableau
	void CreateTable(long numpage, DrawComposite * composite);

	// le destructeur
	virtual ~CTable();
public:
	// le tableau contenant les traits horizontaux du header
	CArray<CHStroke, CHStroke&> m_ArrayHStroke;

	// le tableau contenant les traits horizontaux du corps
	//CArray<CHStroke, CHStroke&> m_ArrayHStroke;

	// le tableau contenant les cellules du header
	//CArray<CTabCell, CTabCell&> m_ArrayTabCell;

	// le tableau contenant les pavés
	CArray<CTabPave, CTabPave&> m_ArrayTabPave;

	// le tableau contenant les champs
	CArray<CTabCell, CTabCell&> m_ArrayTabCell;

public:
	// la valeur de couleur (gris-blanc-noir)
	long m_couleur;

};

#endif // !defined(AFX_ARMLTABLE_H__21E0D24C_A41E_4DC5_A72F_80B909F9C3C3__INCLUDED_)
