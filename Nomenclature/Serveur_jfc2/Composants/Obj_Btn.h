#if !defined(AFX_OBJ_BTN_H__70ED6E0F_EEFD_11D1_B3F8_006052017E36__INCLUDED_)
#define AFX_OBJ_BTN_H__70ED6E0F_EEFD_11D1_B3F8_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Obj_Btn window
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Btn v1.3
Tâche		: Bouton avec icone, texte, couleur
Type 		: Nouvelles fonctions + Amelioration
Fichiers	: Obj_Btn.cpp, Obj_Btn.h
Auteur		: VLF
Date		: 10/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#define OBJ_BTN_VER		1.3

class CObj_Btn : public CButton
{
static HICON * g_LoadIcons;					// Tableau des handles des icones chargés
static short g_nbLoadIcons;

#define NBICONES		17							//nombre d'icones possible à chargé en mémoire

// Styles de l'objet
#define STYLE_STANDARD	0							// bouton standard JFC (avec ou sans texte, avec ou sans couleur)
#define STYLE_LAMPE		1							// Reste enfoncé et une petite lampe (8*8)
#define STYLE_PRINT		2							// Bouton standard avec une petite imprimante (13*10)
#define STYLE_ACINEMA	3							// bouton qui reste enfoncé et devient vert
#define STYLE_EXIT		4							// bouton mini sans texte colorié avec icon croix noir(6*5)
#define STYLE_LAMPELUMINEUX		5					// LAMPE + le fond est coloré lorsqu'il est enfoncé
#define STYLE_EXPORT	6							// bouton standard avec petite image représantant export
#define STYLE_LUMINEUX	7							// Le fond est coloré lorsqu'il est enfoncé

#define FORMAT_STANDARD	1							// btn standard window (utilisable avec la LAMPE, dans ce cas on obtient un fonctionnement à 2 niveaux ???)
#define FORMAT_2STATES	2							// avec style standard, btn est soit leve ou enfonce
#define FORMAT_EXCLUSIF	3							// avec style STYLE_LAMPE, ETAT_ENFONCE et ETAT_ALLUME deviennent exclsifs
#define FORMAT_LIBRE	4							// avec style STYLE_LAMPE, ETAT_ENFONCE et ETAT_ALLUME sont libres

// Etat de l'objet (bit)
#define ETAT_ENFONCE	1
#define ETAT_ALLUME		2
#define ETAT_DISABLE	4

// Action effectué dans l'objet (retourné au WM_COMMAND)
#define ACTION_ENFONCE	1							// ou levé
#define ACTION_ALLUME	2							// ou éteind
#define ACTION_SHIFT	4							// touche [Shift] pressé
#define ACTION_CONTROL	8							// touche [Ctrl] pressée

// Construction
public:
	CObj_Btn();

// Attributes
public:

// Operations
public:
	void Initialise(short Style, short param=FORMAT_STANDARD, COLORREF bkcolor=0, CFont * Font=NULL);

	BOOL SetBtnState(short etat);
	short GetBtnState();
	short ChangeColorLampe(short NumLedColor);		// couleur 0 à 9

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Btn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBrushFond(COLORREF colorref,bool fUpdate=1);
	void SetBrushFondEnfonce(COLORREF colorref, bool fUpdate=1);
	virtual ~CObj_Btn();
	BOOL EnableWindow(BOOL bEnable) { InvalidateRect(NULL, 0); return(CButton::EnableWindow(bEnable));}


protected:
	short m_Style;									// style général de l'objet
	short m_Param;									// paramètres 

protected:
	short m_Etat;									// état de l'objet et/ou de la lampe
	short m_Action;									// action intervenue sur le bouton
	CRect m_RClient,								// zone du bouton
		  m_RText,									// zone du texte dans le bouton
		  m_RIcone;									// zone de l'icone dans le bouton
	BOOL m_fOnLampe,								// indique si souris sur led
		 m_fAction,									// indique si click en cours
		 m_fCapture;

	BITMAP * m_SBitmap;								// bitmap structure
	CBrush * m_BNoir,									// pinceau noir
		   * m_BGrisC,								// pinceau gris clair
		   * m_BFondEnfonce,								// pinceau du fond enfoncé
		   * m_BFond;								// pinceau du fond lor
	CPen * m_PGrisF,									// crayon gris fonce
		* m_PGrisC,									// crayon gris clair 
		* m_PBlanc,									// crayon blanc
		* m_PNoir;									// crayon noir
	HICON m_IcoOnLed,								// handle de l'icone vert en selectio
		  m_IcoLedTrans,							// handle de l'icone vide
		  m_IcoLedSelect,							// handle de l'icone vert selectionne
		  m_IcoStd;									// handle de l'icone
	CFont * m_Font;

	UINT m_TabIcons[NBICONES];						// tableaux des identifiants des icones

	//{{AFX_MSG(CObj_Btn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
	void InitTabIcons(void);

	void DessineBtn(CDC * dc, CRect * Rect);
	void DessineIconeLampe(CDC * dc);
		
	void DessineDemiRect(CDC * DC, CRect  R, CPen * hP1);
	void Dessine1Rect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2);
	void DessineRect2Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2);
	void DessineRect4Pen(CDC * DC, CRect R, CPen * hP1, CPen * hP2, CPen * hP3, CPen * hP4);
	BOOL CALLBACK EXPORT OutputFunc(HDC hDC, LPARAM lpData, int nCount );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_BTN_H__70ED6E0F_EEFD_11D1_B3F8_006052017E36__INCLUDED_)