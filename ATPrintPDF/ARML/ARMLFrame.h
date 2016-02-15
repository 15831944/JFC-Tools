// ARMLFrame.h: interface for the CFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMLFRAME_H__07A7FCBE_F331_4CD6_9EA8_84AE58147F47__INCLUDED_)
#define AFX_ARMLFRAME_H__07A7FCBE_F331_4CD6_9EA8_84AE58147F47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les fichiers nécessaires
#include "AfxTempl.h"
#include "ARMLField.h"
#include "./../Composite/JFCDrawComposite.h"

class CFrame  
{
public:
	// le constructeur
	CFrame();

	// le constructeur de recopie
	CFrame(const CFrame & source);

	// l'opérateur d'affectation
	CFrame & operator = (const CFrame & operande);

	// les fonctions pour fixer les sommets du cadre
	void SetStartPosX(long startposx);
	void SetEndPosX(long endposx);
	void SetStartPosY(long startposy);
	void SetEndPosY(long endposy);
	
	// la fonction pour fixer les styles de traits
	void SetStrokeStyles(long traitG, long traitH, long traitD, long traitB);

	// la fonction pour récupérer le style du trait haut
	long GetStyleTH();

	// la fonction pour fixer la couleur de fond
	void SetGrayed(bool grayed);

	// la fonction pour ajouter un champ
	void AddField(CField & field);

	// la fonction pour créer le composite d'un cadre
	void CreateFrame(long numpage, DrawComposite * composant);

	// le destructeur
	virtual ~CFrame();
public:
	// le flag grisé ou non
	bool m_Grayed;
	// les coordonnées du cadre
	long m_StartPosX;
	long m_EndPosX;
	long m_StartPosY;
	long m_EndPosY;
	// les styles de traits
	long m_StyleTG;
	long m_StyleTH;
	long m_StyleTD;
	long m_StyleTB;
	// la valeur de couleur (gris-blanc-noir)
	long m_couleur;
	// le tableau de champs
	CArray<CField, CField&> m_FieldArray;
};

#endif // !defined(AFX_ARMLFRAME_H__07A7FCBE_F331_4CD6_9EA8_84AE58147F47__INCLUDED_)
