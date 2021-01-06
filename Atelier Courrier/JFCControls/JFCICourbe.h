#if !defined(AFX_JFCICourbe_H__94124E06_74BE_490F_8EB1_1C95C2D24181__INCLUDED_)
#define AFX_JFCICourbe_H__94124E06_74BE_490F_8EB1_1C95C2D24181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class AFX_EXT_CLASS JFCICourbe  
{
public:
	// le constructeur
	JFCICourbe();

	// R�cup�ration nombre d'�l�ments axe horizontal
	virtual JInt32 GetXElmtCount() const = 0;

	// R�cup�ration nombre total de courbes
	virtual JInt32 GetCourbeCount() const = 0;

	// R�cup�ration ident couleur de la courbe
	virtual JInt32 GetCourbeColor(JInt32 CourbeIndex) const = 0;

	// R�cup�ration couleur via index
	virtual COLORREF GetColor(JInt32 IdColor) const = 0;

	// la fonction pour r�cup�rer la validit� des �l�ments
	virtual JBool IsItemValid(JInt32 XIndex, JInt32 CourbeIndex) const = 0;

	// la fonction pour r�cup�rer la valeur des �l�ments et �tat affichage
	virtual JFlt64 GetValueItem(JInt32 XIndex, JInt32 CourbeIndex, bool &ItemMarque) const = 0;

	// la fonction pour r�cup�rer le libell� de la courbe
	virtual const JString GetLabelItem(JInt32 CourbeIndex) const = 0;

	// la fonction pour r�cup�rer le type de r�sultat horizontal
	virtual const JString GetLabelResultHorz() const = 0;

	// la fonction pour r�cup�rer le type de r�sultat vertical
	virtual const JString GetLabelResultVert() const = 0;

	// la fonction pour r�cup�rer le libell� support des courbes
	virtual const JString GetLibCourbeGraph(JUnt32 InxCourbe) const = 0;

	// la fonction pour r�cup�rer borne inf�rieure d�part courbe
	virtual JUnt32 GetDepCourbe(JUnt32 InxCourbe) const = 0;

	// le destructeur
	virtual ~JFCICourbe();
};

#endif // !defined(AFX_JFCICourbe_H__94124E06_74BE_490F_8EB1_1C95C2D24181__INCLUDED_)
