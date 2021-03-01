#pragma once

/////////////////////////////////////////////////////////////////////////////
// JFCEditGrid

class JFCEditGrid : public CEdit
{
public:
	// le constructeur
	JFCEditGrid();

	// le destructeur
	virtual ~JFCEditGrid();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnChange();
	afx_msg void OnDestroy();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	static const UINT JFC_CANCEL_EDITING			= WM_USER + 1;
	static const UINT JFC_END_EDITING				= WM_USER + 2;
	static const UINT JFC_BEGIN_NEW_EDITING_UP		= WM_USER + 3;
	static const UINT JFC_BEGIN_NEW_EDITING_DOWN	= WM_USER + 4;
	static const UINT JFC_BEGIN_NEW_EDITING_RIGHT	= WM_USER + 5;
	static const UINT JFC_BEGIN_NEW_EDITING_LEFT	= WM_USER + 6;
	static const UINT JFC_BEGIN_NEW_EDITING			= WM_USER + 7;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CFont m_FontS; // la fonte

protected:
	LONG m_Etat; // l'état (0 : départ, 1 : Focus, 2 ; Focus + modif)
};
