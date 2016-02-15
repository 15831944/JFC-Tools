// TabCell.h: interface for the CTabCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABCELL_H__3F204D2A_F926_4E17_BF13_86DC68478BE5__INCLUDED_)
#define AFX_TABCELL_H__3F204D2A_F926_4E17_BF13_86DC68478BE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les fichiers nécessaires

class CTable;

class CTabCell
{
	friend class CTable;
public:
	// le constructeur
	CTabCell();

	// le constructeur de recopie
	CTabCell(const CTabCell & source);

	// l'opérateur d'égalité
	CTabCell & operator=(const CTabCell & operande);

	// les fonctions pour modifier les paramètres
	void SetPosY(long posy);
	void SetHeight(long height);
	void SetPosX(long posx);
	void SetWidth(long width);
	void SetStyleTD(long styleTD);
	void SetStyleTG(long styleTG);
	void SetBkColor(long bkcolor);
	void SetJustification(char justification);
	void SetBold(bool bold);
	void SetText(CString texte);

	// le destructeur
	virtual ~CTabCell();
public:
	// la position verticale
	long m_PosY;
	// la hauteur d'une cellule
	long m_Height;
	// la position horizontale
	long m_PosX;
	// la largeur
	long m_Width;
	// le style du trait droit
	long m_StyleTD;
	// le style du trait gauche
	long m_StyleTG;
	//la couleur de fond
	long m_BkColor;
	// la justification
	long m_Justification;
	// l'épaisseur
	bool m_Bold;
	// le texte
	CString m_Texte;
};

#endif // !defined(AFX_TABCELL_H__3F204D2A_F926_4E17_BF13_86DC68478BE5__INCLUDED_)
