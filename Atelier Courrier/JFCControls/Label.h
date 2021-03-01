
#pragma once
#include "ATPColors.h"

#define	NM_LINKCLICK	WM_APP + 0x200


/**
 * \ingroup JFCControls
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-10-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CLabel : public CStatic
{
// Construction
public:


	static enum LinkStyle { LinkNone, HyperLink, MailLink };
	static enum FlashType {None, Text, Background };
	static enum Type3D { Raised, Sunken};
	static enum BackFillMode { Normal, Gradient, GradientJFC };

	CLabel();
	virtual void	Reset() { m_hWindows.RemoveAll(); }
	
	virtual CLabel& SetBkColor(COLORREF crBkgnd, CATPColors::APPCOLOR AppColor, CATPColors::MODE_DEGRAD ModeDegrad, COLORREF crBkgndHigh = 0, BackFillMode mode = Normal);
	virtual CLabel& SetBkColor2(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, BackFillMode mode = Normal);

	int GetApp(){return m_CATPColorsApp;};
	
	virtual CLabel& SetBorderColor(COLORREF crBorder);
	virtual CLabel& SetTextColor(COLORREF crText);
	virtual CLabel& SetText(const CString& strText);
	virtual CLabel& SetFontBold(BOOL bBold);
	virtual CLabel& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual CLabel& SetFontUnderline(BOOL bSet);
	virtual CLabel& SetFontItalic(BOOL bSet);
	virtual CLabel& SetFontSize(int nSize);
	virtual CLabel& SetSunken(BOOL bSet);
	virtual CLabel& SetBorder(BOOL bSet);
	virtual CLabel& SetTransparent(BOOL bSet);
	virtual CLabel& FlashText(BOOL bActivate);
	virtual CLabel& FlashBackground(BOOL bActivate);
	virtual CLabel& SetLink(BOOL bLink,BOOL bNotifyParent);
	virtual CLabel& SetLinkCursor(HCURSOR hCursor);
	virtual CLabel& SetFont3D(BOOL bSet,Type3D type=Raised);
	virtual CLabel& SetRotationAngle(UINT nAngle,BOOL bRotation);
	virtual CLabel& SetText3DHiliteColor(COLORREF cr3DHiliteColor);
	virtual CLabel& SetFont(LOGFONT lf);
	virtual CLabel& SetMailLink(BOOL bEnable, BOOL bNotifyParent);
	virtual CLabel& SetHyperLink(const CString& sLink);
	virtual void AddTopWindow(HWND hWnd);

// Attributes
public:
protected:
	void UpdateSurface();
	void ReconstructFont();
	void DrawGradientFill(CDC* pDC, CRect* pRect, COLORREF crStart, COLORREF crEnd, int nSegments);
	COLORREF		m_crText;
	COLORREF		m_cr3DHiliteColor;
	HBRUSH			m_hwndBrush;
	HBRUSH			m_hBackBrush;
	LOGFONT			m_lf;
	CFont			m_font;
	BOOL			m_bState;
	BOOL			m_bTimer;
	LinkStyle		m_Link;
	BOOL			m_bTransparent;
	BOOL			m_bFont3d;
	BOOL			m_bToolTips;
	BOOL			m_bNotifyParent;
	BOOL			m_bRotation;
	FlashType		m_Type;
	HCURSOR			m_hCursor;
	Type3D			m_3dType;
	BackFillMode	m_fillmode;
	COLORREF		m_crHiColor;
	COLORREF		m_crLoColor;
	COLORREF		m_crBorder;
	BOOL			m_crBorderSet;
	CString			m_sLink;
	CArray<HWND ,HWND > m_hWindows;

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLabel();

protected:

	CATPColors::APPCOLOR	m_CATPColorsApp;
	CATPColors::MODE_DEGRAD m_ModeDegrad;
	// BOOL					m_ColorDegrad;

	// Generated message map functions
protected:
	//{{AFX_MSG(CLabel)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
