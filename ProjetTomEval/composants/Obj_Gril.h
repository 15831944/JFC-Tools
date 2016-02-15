#if !defined(AFX_OBJ_GRIL_H__3F4F26AF_F483_11D1_B3F8_006052017E36__INCLUDED_)
#define AFX_OBJ_GRIL_H__3F4F26AF_F483_11D1_B3F8_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Obj_Gril.h : header file
//
//disable le warning 4244
//warning C4244: '=' : conversion from 'int' to 'short', possible loss of data
#pragma warning(disable : 4244)
/////////////////////////////////////////////////////////////////////////////
// CObj_Gril window

class CObj_Gril : public CStatic
{
#define STYLE_GRIL_DEFAUT		0
#define STYLE_GRIL_PIGE			1

#define OPTION_GRIL_PRINTER		16
#define OPTION_GRIL_NO_LIGNE	32
#define OPTION_GRIL_NO_COLONNE	64
#define OPTION_GRIL_NO_CONTOUR	128
#define OPTION_GRIL_RETAILLABLE	256
#define OPTION_GRIL_TRAITSGRIS	512
// avec OPTION_GRIL_RETAILLABLE, crée UNE cellule à taille variable (en largeur)
#define OPTION_GRIL_ALLWIDTHSIZE	1024

// Construction
public:
	CObj_Gril();

	short Initialise(short Style, short NbCelluleX, short NbCelluleY, short UTHorizontal, short UTVertical, short PosDefX=0, short PosDefY=0, CRect * pR_Print=NULL);
	short InitScrollBar(CScrollBar * ScrollX=NULL, CScrollBar * ScrollY=NULL);

	short SetPosition(short PosX, bool flag=false);
	short SetPositionV(short PosY, bool flag=false);

	//curseurs
	short SetPosCurseur(short NumCurs, short Pos);
	short SetCurseur(short NumCurs, short Pos, COLORREF Color);

	short SetNbLines(short NbLines, bool fUpdate=false);
	short SetNbRows(short NbRows, bool fUpdate=false);
	void SetCoupureCalend(char * TabCoupures);
	void SetSelectionCalend(CByteArray * Selections);

	short MAJPosScrollH(UINT nSBCode, UINT nPos, short flag);
	short MAJPosScrollV(UINT nSBCode, UINT nPos, short flag);

	//printer
	void PaintToPrinter(CDC * pDC, CFont * pFont);

	// Taille
	void SetSize(int largeur,int hauteur);

protected:
	//fonctions de dessin à surdéfinir
	virtual void DessineFond(CDC * dc, CRect RectObj);
	virtual void DessineFondInutilise(CDC * dc, CRect RectObj);
	virtual void DessineContenu(CDC * dc, CRect RectObj);
	virtual void DessineCellule(CDC * dc, CRect RectCel, short X, short Y, short PosX, short PosY);

	//fonctions souris à surdéfinir
	virtual bool OnLeftClick(short X, short Y, short PosX, short PosY,CPoint pos);
	virtual bool OnRightClick(short X, short Y, short PosX, short PosY,CPoint pos);
	virtual bool OnMousePosition(short X, short Y, short PosX, short PosY,CPoint pos);

	virtual bool OnCurseurMoved(short NumCurs, short Pos);
	virtual bool OnSelectionChanged(char * Selections);
	// Operations
public:
// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Gril)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObj_Gril();
	// Generated message map functions

private:
	//variable d'initialisation
	short m_Style;												//style général de l'objet
	short m_dX, m_dY;											//largeur d'une cellule X, et Y
	short m_NbCelluleX, m_NbCelluleY;							//Nombre de cellules horizontal et vertical
	short m_PosActiveX, m_PosActiveY;							//Position active vertical et horizontal
	CScrollBar * m_ScrollX, * m_ScrollY;						//pointeur des scrolls

private:
	//variable curseurs
	struct SCurseur
	{
		short NumCurseur;										//random
		COLORREF Color;											//RGB
		short Pos;												//0-m_nbcases 
		CRect R;												//zone curseur
		bool Select;											//flag 1=select 0=non select
	}* m_SCurseur;
	short m_NbCurseurs;
	CPtrArray * m_ACurseur;										//[m_nbcurseur]
	
	//pour le style PIGE
	char * m_CoupureCalend;										//[nbcase du calendrier]	//1=sem, 2=mois, 3=sem&mois
	CByteArray m_Selections;									//[nbcase du calendrier]	//0=no sel, 1=sel, 2=sel trait noir à gauche,
																//3=sel trait noir à droite, 4=sel trait noir des 2 cotés
	//variables 
	bool m_fInit,												//indique si initialisation OK
		m_fCoupures,											//indique si les coupures sont initialisées
		m_fSelection;											//indique si la sélection est initialisée
	CRect m_RClient;											//rectangle dans lequel est dessiné la grille
	short m_NbCelluleVisuX, m_NbCelluleVisuY;					//nb cellule en visu vertical et horizontal
	short m_nbCol, m_nbLig;										//nb cellule pour les ascenseurs

	CPen m_PenGray,			//pen style Pige (inter-semaine)
		 m_Pen2Black;		//pen style Pige (selection)

	CRect m_PrntRect;
	CDC * m_PrntDC;


	//{{AFX_MSG(CObj_Gril)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	//initialisation du rectangle client et des coordonnées
	void InitialiseCellule(void);
	
	//fonction de dessin
	void DessineCadrillage(CDC * dc, CRect * R, short OffsetX=0, short OffsetY=0);
	void DessineCurseur(CDC * dc);
	void DessineGrille(CDC * pDC,CRect * R, CFont * pFont, bool fPrinter=false);

	short InvalideCellule(short PosX, short PosY);
	short DelCurseur(short NumCursor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_GRIL_H__3F4F26AF_F483_11D1_B3F8_006052017E36__INCLUDED_)
