// TabPave.h: interface for the CTabPave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABPAVE_H__3F204D2A_F926_4E17_BF13_86DC68478BE5__INCLUDED_)
#define AFX_TABPAVE_H__3F204D2A_F926_4E17_BF13_86DC68478BE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les fichiers nécessaires

class CTable;

class CTabPave
{
	friend class CTable;
public:
	// le constructeur
	CTabPave();

	// le constructeur de recopie
	CTabPave(const CTabPave & source);

	// l'opérateur d'égalité
	CTabPave & operator=(const CTabPave & operande);

	void GetLineFields(CString texte,CStringArray & FieldArray);

	// le destructeur
	virtual ~CTabPave();
public:
	// la position verticale
	long m_PosY;
	// la hauteur d'une pave
	long m_HeightY;
	// la hauteur d'une cellule
	long m_Height;
	// la position horizontale
	long m_PosX;
	// la largeur
	long m_WidthX;
	//la couleur de fond
	long m_BkColor;
	// le texte
	CString m_Texte;
};

#endif // !defined(AFX_TABPAVE_H__3F204D2A_F926_4E17_BF13_86DC68478BE5__INCLUDED_)
