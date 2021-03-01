/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Sylvain SAMMURI
//	Jérémy DEVILERS(effet de Hover)
//
//
*/

#pragma once

// on inclut les définitions nécessaires
#include "ATPColors.h"
#include "JFControl.h"
#include "JFCScroll.h"

/////////////////////////////////////////////////////////////////////////////
// JFCButtonList

class AFX_EXT_CLASS JFCButtonList : public JFControl 
{
public:
	enum STYLES {
		LED_NO_SEL			=	0x0001,
		LED_SEL				=	0x0002,
		BUT_SINGLE_SEL		=	0x0004,
		BUT_MULTI_SEL		=	0x0008,
	};

	enum STATE {
		BUT_DOWN			=	0x0001,
		LED_ON				=	0x0002,
	};

	struct BtnParams
	{
		BtnParams()
		{
			m_State			=	0;
			m_Style			=	0;
			m_Color_Border	=	GetSysColor(COLOR_3DDKSHADOW);
			m_Color_LedBorder	=	GetSysColor(COLOR_3DDKSHADOW);
			m_Color_LedOff	=	RGB(0,0,0);
			m_Color_LedOn	=	RGB(255,255,255);
			m_Color_BtnUp	=	RGB(255,255,255);
			m_Color_BtnHover = RGB(255,255,255);
			m_Color_BtnDown	=	RGB(128,128,128);;
			m_Color_Text	=	RGB(0,0,0);
			m_Design_Led    =   0;						// 0: led carré, 1: led triangle /\ 2: led triangle \/ 3: led ronde 
			m_Text_Font.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName());
			m_Text			=	"";
		}
		LONG		m_State;
		LONG		m_Style;
		COLORREF	m_Color_Border;
		COLORREF	m_Color_LedBorder;
		COLORREF	m_Color_LedOff;
		COLORREF	m_Color_LedOn;
		COLORREF	m_Color_BtnUp;
		COLORREF	m_Color_BtnHover;
		COLORREF	m_Color_BtnDown;
		COLORREF	m_Color_Text;
		LONG		m_Design_Led;
		CFont		m_Text_Font;
		CString		m_Text;
	};

	// le constructeur
	JFCButtonList();
	// destructor
	virtual ~JFCButtonList();

	// les fonctions pour définir les ascenseurs externes
	void SetHorzScroll(JFCScroll* pScroll, BOOL Master, BOOL update = TRUE);
	void SetVertScroll(JFCScroll* pScroll, BOOL Master, BOOL update = TRUE);

	// les fonctions pour récupérer les ascenseurs externes
	JFCScroll* GetHorzScroll();
	JFCScroll* GetVertScroll();

	// les fonctions pour définir la taille des éléments
	void SetHorzItemSize(LONG size, LONG Interstice = 0, LONG border = 0, BOOL update = TRUE);
public:
	void SetVertItemSize(LONG size, LONG Interstice = 0, LONG border = 0, BOOL update = TRUE);

	void SetIcons(int iconOn, int IconOff);
protected:

	// les fonctions pour définir l'intervalle des éléments
	void SetHorzItemRange(LONG first, LONG last, BOOL update = TRUE);
	void SetVertItemRange(LONG first, LONG last, BOOL update = TRUE);

protected:
	// la fonction pour rendre visible un élément
	bool EnsureVisible(LONG i, LONG j);

protected:
	// les fonctions pour convertir les indices et les positions
	bool GetIdxAtPos(LONG x, LONG y, LONG & i,  LONG & j);
	bool GetPosAtIdx(LONG i, LONG j, LONG & x1, LONG & y1, LONG & x2, LONG & y2);

protected:
	// la fonction pour convertir les positions en indices de tracking
	void GetTrackIdx(LONG x, LONG y, LONG & i, LONG & j, BOOL scroll);

protected:
	// la fonction pour récupérer les Parametres des buttons
	virtual void OnGetItemParams(LONG i, LONG j, BtnParams & Params);

protected:
	// les fonctions pour dessiner les éléments
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnDrawParam(CDC & dc, CRect rect, LONG i, LONG j, BtnParams & Params);
	virtual void OnEndDrawing(CDC & dc);
	virtual void OnButtonSel(LONG i, LONG j);
	virtual void OnLedSel(LONG i, LONG j);

private:
	// la fonction pour configurer les ascenseurs actifs
	void OnSetScrollBar(BOOL update);

	// les fonctions pour modifier la position des ascenseurs actifs
	bool OnSetHorzPosition(LONG position);
	bool OnSetVertPosition(LONG position);

	// les fonctions pour récupérer la position des ascenseurs actifs
	LONG OnGetHorzPosition();
	LONG OnGetVertPosition();

private:
	// les fonctions pour faire défiler l'affichage
	void OnHorzScrollItems(BOOL forward);
	void OnVertScrollItems(BOOL forward);

	// les fonctions pour actualiser la position d'affichage
	void OnHorzScrollPosition();
	void OnVertScrollPosition();

private:
	// les fonctions pour dessiner les éléments
	void OnDrawParams(CDC & dc, LONG x, LONG y, LONG cx, LONG cy, LONG i, LONG j, LONG ci, LONG cj);

protected:
	// les fonctions pour dessiner la fenêtre
	virtual void OnDrawWindow(CDC & dc, LONG x, LONG y, LONG dx, LONG dy);
	virtual void OnDrawBorder(CDC & dc, CRect client, LONG x, LONG y, LONG dx, LONG dy);

private:
	LONG m_HorzFirst;		// le premier élément horizontal
	LONG m_HorzLast;		// le dernier élément horizontal
	LONG m_HorzSize;		// la taille des éléments horizontaux
	LONG m_HorzBorderSize;	// l'indicateur des bordures horizontales
	LONG m_HorzInterstice;

private:
	LONG m_VertFirst;		// le premier élément vertical
	LONG m_VertLast;		// le dernier élément vertical
	LONG m_VertSize;		// la taille des éléments verticaux
	LONG m_VertBorderSize;	// l'indicateur des bordures verticales
	LONG m_VertInterstice;

private:
	BOOL m_HorzMaster;		// l'indicateur de l'ascenseur horizontal
	BOOL m_VertMaster;		// l'indicateur de l'ascenseur vertical

private:
	JFCScroll* m_pHorzScroll; // le pointeur sur l'ascenseur horizontal
	JFCScroll* m_pVertScroll; // le pointeur sur l'ascenseur vertical

protected:
	//{{AFX_VIRTUAL(JFCViewport)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCViewport)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bMouseOnButton;
	LONG m_BtnI;
	LONG m_BtnJ;
	void CancelHover();
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	int	m_iconOn;
	int m_iconOff;

};
