#if !defined(AFX_OBJ_TAB_H__558D4000_D70E_11D2_989D_004095420987__INCLUDED_)
#define AFX_OBJ_TAB_H__558D4000_D70E_11D2_989D_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Obj_Tab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_Tab window

class CObj_Tab : public CStatic
{
//style
#define STYLE_TAB_DEFAUT			1							//visualisation
#define STYLE_TAB_SELECTION			2							//sélection col/lig/cellule
#define STYLE_TAB_EDIT				4							//édition des cellules

//options pour les styles
#define OPT_TAB_NOGRIL_COL			16							//pas de séparateur col		00 0000 0001 xxxx
#define OPT_TAB_NOGRIL_LIG			32							//pas de séparateur lig		00 0000 0010 xxxx
#define OPT_TAB_SELECT_COL			64							//sélection d'une col		00 0000 0100 xxxx
#define OPT_TAB_SELECT_LIG			128							//sélection d'une lig		00 0000 1000 xxxx
#define OPT_TAB_SELECT_CEL			256							//sélection d'une cellule	00 0001 0000 xxxx
#define OPT_TAB_EDIT_CEL			512							//édition d'une cellule		00 0010 0000 xxxx
#define OPT_TAB_TRI_COL				1024						//tri par col				00 0100 0000 xxxx
#define OPT_TAB_TRI_LIG				2048						//tri par lig				00 1000 0000 xxxx
#define OPT_TAB_NOGRIL_OUT			4096						//pas de séparateur en dehors des limites lig/col	01 0000 0000 xxxx

//format 
#define FORMAT_TAB_BTN				1							//titre style bouton Win98
	
//type des sections
#define TITRE_LIG					0
#define TITRE_COL					1
#define CORPS						2

//orientation
#define GAUCHE						1
#define DROITE						2
#define HAUT						4
#define BAS							8
#define CENTRE						16

//Marge libelle
#define MARGE_TAB_X					10
#define MARGE_TAB_Y					4

//Selection
#define CLR_TAB_SELECTION			RGB(128,128,255)

// Construction
public:
	CObj_Tab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Tab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObj_Tab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Tab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// début de mon code...

public:
	void Initialise(short Style,
					short Format,
					short DataType,
					short dxCel,
					short dyCel,
					short nLigVisu,
					short nColVisu,
					CScrollBar * ScrollH,
					CScrollBar * ScrollV,
					CFont * pFont=NULL,
					COLORREF TabColorBg=0);
	void InitialiseTitre(	short TypeTitre,
							short dxTitre,
							short dyTitre,
							CFont * pFont=NULL,
							COLORREF TitreColorBg=0);

	void SetTitre(short TypeTitre, CStringArray * TabTitre);
	void SetTableau(short NbLig, CStringArray * TabElements);
	void SetTableau(short NbLig, short NbCol, short ** TabElements);
	void SetTableau(short NbLig, short NbCol, long ** TabElements);
	void SetTableau(short NbLig, short NbCol, char ** TabElements);
	
	void SetFormatTitre(short TypeTitre,
						short NroTitre,
						short dxTitre,
						short dyTitre,
						UINT Orientation,
						CFont * pFont=NULL,
						COLORREF ColorCel=0,
						bool flag=false);
	void SetFormatCellule(	short CelX,
							short CelY,
							short Format,
							short DataType,
							UINT Orientation,
							CFont * pFont=NULL,
							COLORREF ColorCel=0,
							bool flag=false);
	void SelectTitre(short TypeTitre, short NroTitre, bool flag=true);
	
	virtual void OnSort(short TypeTitre, short NroTitre);
	virtual void OnLeftClick(long Lig, long Col);
	virtual void OnLeftDblClick(long Lig, long Col);

	void SetSelection(	short TypeTitre,
						short CelX,
						short CelY,
						COLORREF SelColor=RGB(0,0,255),
						bool fUpdate=false);
	short * GetSelection(void);
	short GetNbSelection(void);

	void SetPosition(	short TypeTitre,
						short CelX,
						short CelY,
						bool fUpdate=false);

	void RAZSelection(short TypeTitre, short CelX, short CelY, bool fUpdate=false);

	short MAJPosScrollV(UINT nSBCode, UINT nPos, short flag);
	short MAJPosScrollH(UINT nSBCode, UINT nPos, short flag);

	void PutMsgInToTab(CString Msg, CFont * pFont=NULL, COLORREF TxtColor=RGB(0,0,0), bool flag=true);

	void LibereObjet();
	
	void ChangeBgColor(COLORREF ColorBg);

private:
	void SetDimensionsTab(CDC * dc, CRect * pR);
	void InitAscenseurs(short nbColVisu, short nbLigVisu);
	void DessineTitre(short Titre, short Nb, CDC * dc);
	void DessineTableau(CDC * dc);
	void Dessine2Rect4Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2, CPen * hP3, CPen * hP4);
	void Dessine2Rect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2);
	void DessineRect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2);

protected:
	short m_nbLigVisu, m_nbColVisu;

private:
	short m_Style;
	short m_Format;
	short m_NbLig,m_NbCol;
	short m_PosActiveCol, m_PosActiveLig;
	CScrollBar *m_pScrollH, *m_pScrollV;
	CFont *	m_pMSSansSerif,	* m_pTimesNewRoman;
	CPen * m_PGrisF,									// crayon gris fonce
		* m_PGrisC,										// crayon gris clair
		* m_PBlanc,										// crayon blanc
		* m_PNoir;										// crayon noir
	CRect m_RectTab;
	bool m_fCapture;

	//message qui apparait dans le fond du tableau, centré
	CString m_Message;
	CFont * m_pMsgFont;
	COLORREF m_MsgColor;

private:
	//structures
	struct SInfoCellule
	{
		short m_Format;					// %, griser, vide
		short m_dx, m_dy;
		CRect m_RCellule;
		CFont * m_pFont;
		COLORREF m_RGBCel;
		CString m_strCel;				//contenu
		UINT m_Orientation;				//voir nFormat de DrawText
		short m_DataType;				//string, int, long...
		bool m_fSelect;
	};
	typedef CArray<SInfoCellule, SInfoCellule> CInfoCellule;

	struct SInfoTab
	{
		COLORREF m_ColorBg;
		CFont * m_pFont;
		short m_dx;
		short m_dy;
		short m_DataType;
		CInfoCellule * m_InfoCel;
		bool m_fInit;
		CRect m_RSection;
	};
	typedef CArray<SInfoTab, SInfoTab> CInfoTab;
	CInfoTab m_itTableau;							//initialiser à 3:titre col, titre lig, corps

	struct SInfoSelection
	{
		short m_TypeTitre;
		short m_X;
		short m_Y;
		COLORREF m_Color;
	};
	typedef CArray<SInfoSelection, SInfoSelection> CSelection;
	CSelection m_sSelection;
};	

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_TAB_H__558D4000_D70E_11D2_989D_004095420987__INCLUDED_)
