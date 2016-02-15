#if !defined(AFX_OBJ_LBOX_H__92B9C1B1_F632_11D1_B3F8_006052017E36__INCLUDED_)
#define AFX_OBJ_LBOX_H__92B9C1B1_F632_11D1_B3F8_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Obj_LBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_LBox window

//Styles de la liste-box
#define UNISTATE				0									//sélection item par click gauche seulement
#define DBLSTATE				1									//sélection item par click gauche ou droit
#define TRISTATE				2									//sélection item par click gauche et/ou droit

//format
#define SIMPLE					0									//un seul champ 
#define MULTIPLE				1									//plusieurs champs
#define ETENDUE 				2									//une vague + CTRL
#define ETENDUE_JFC				3									//plusieurs vagues

//règles de sélection
#define INDEPENDANTE			0									//les selections ne mélangent pas
#define CROISEE					1									//les sélections se superposent
#define LEFTINTORIGHT			2									//la sélection gauche dans la sélection droite
#define RIGHTINTOLEFT			3									//inversement
#define EXCLUSIVE				4									//ou l'un, ou l'autre, mais pas les deux !

class CObj_LBox : public CListBox
{
// Construction
public:
	CObj_LBox();

// Attributes
public:

// Operations
public:
	void Initialise(short Style, short ModeSel1=ETENDUE_JFC, short ModeSel2=ETENDUE_JFC,
		COLORREF ColorSel1=0, COLORREF ColorSel2=0, short SelectRules=RIGHTINTOLEFT,
		COLORREF BkColor=0, CFont * TxtFont=NULL, COLORREF ColorTxt=0);
	int GetSelCount();
	int GetSelItems(int nMaxItems, LPINT rgIndex );

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_LBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ChangeMode(short ModeSel1,short ModeSel2,bool flagRedraw=true);
	void ChangeSelectRules(short newSelectRules);
	void ChangeColorSel1(COLORREF newcolor,bool flagRedraw=true);
	void ChangeColorSel2(COLORREF newcolor,bool flagRedraw=true);
	virtual ~CObj_LBox();

protected:
	//fonction a surdéfinir
	virtual void OnItemState(short nItem, short State);

	// Generated message map functions
public:
	//variables d'initialisation
	short m_Style;										//voir liste des styles
	short m_ModeSel1,									//voir liste des formats
		  m_ModeSel2;									//
	short m_SelectRules;								//voir règles de sélection
	COLORREF m_ColorSel1,								//RGB selection 1
			 m_ColorSel2,								//RGB selection 2
			 m_BkColor,									//RGB fond
			 m_ColorTxt;								//RGB couleur texte
	CFont * m_TxtFont;									//font du texte
// private:
public:
	CBrush * m_brush1,									//brush selection 1
		   * m_brush2,									//brush selection 2
		   * m_brushBk;									//brush fond

	short m_ItemSelDebBL,								//1er item selectionné dans une multisel; click gauche
		  m_ItemSelCurrBL;								//item sélectionné; click gauche
	short m_ItemSelDebBR,								//1er item selectionné dans une multisel; click droit
		  m_ItemSelCurrBR;								//item sélectionné; click droit
	short m_LastItemSel;
	BOOL  m_fInit,										//indique si initialisation OK
		  m_fSelect,
		  m_fCapture;
	CRect m_RClient;
	CByteArray m_Selection,
				m_SelectionTmp,
				m_SelectionRAZ;

	//{{AFX_MSG(CObj_LBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void SetDataItem(short item, short etat, short data);
	void RAZAllItemState(short etat);
	void SelectItemRange(short itemDeb, short itemFin, DWORD param, short fclick);
	short IsSelection(short etat);
	BOOL IsSelectRules(short click, short item);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_LBOX_H__92B9C1B1_F632_11D1_B3F8_006052017E36__INCLUDED_)
