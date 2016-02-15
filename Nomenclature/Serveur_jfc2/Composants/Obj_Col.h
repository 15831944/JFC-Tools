#if !defined(AFX_OBJ_COL_H__0D8E2F22_590B_11D2_AD1C_0080C708A895__INCLUDED_)
#define AFX_OBJ_COL_H__0D8E2F22_590B_11D2_AD1C_0080C708A895__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// obj_col.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_Col window

class CObj_Col : public CStatic
{
#define STYLE_COL_DEFAUT		0
#define STYLE_COL_NO_GRILLE		1

#define OPTION_COL_PRINTER		16									//pour dessiner calendrier vers imprimante
// Construction
public:
	CObj_Col();
	virtual ~CObj_Col();

	short Initialise(short Style,short NbCelluleY,short UTVertical, short PosDefY=0, CScrollBar * ScrollY=NULL, CRect * pR_Print=NULL);

	//Position et ligne
	short SetPosition(short PosY,bool fRedraw=false);
	void SetNbLine(short NbCelluleY,bool fRedraw=false);
	
	//ascenseur
	short MAJPosScrollV(UINT nSBCode, UINT nPos, short flag);

	//printer
	void PaintToPrinter(CDC * pDC, CFont * pFont);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Col)
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
protected:
	virtual void DessineFond(CDC * dc, CRect RectObj);
	virtual void DessineCellule(CDC * dc, CRect RectCel, short Y, short PosY);

	//{{AFX_MSG(CObj_Col)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CScrollBar * m_ScrollY;										//pointeur des scrolls
	CRect m_RClient;
	short m_Style;												//style général de l'objet
	short m_dY;													//Hauteur cellule
	short m_NbCelluleY;											//Nombre de cellules vertical
	short m_PosActiveY;											//Position active vertical
	short m_NbCelluleVisuY;										//nb cellule en visu vertical et horizontal
	short m_nblines;											//nb cellules max en visu
	short m_nbLigScrollPage;												//nb cellule pour les ascenseurs
	bool m_fInit;
	CRect m_PrntRect;
	CDC * m_PrntDC;

private:
	void DessineColonne(CDC * pDC,CRect * R, CFont * pFont, bool fPrinter=false);
	void DessineGrille(CDC * dc, CRect * R, short OffsetY=0);
	void DessineContenu(CDC * dc, CRect RectObj);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_COL_H__0D8E2F22_590B_11D2_AD1C_0080C708A895__INCLUDED_)
