#pragma once

#include <afxtempl.h>

class AFX_EXT_CLASS CListBoxQuestions : public CListBox
{
public:
	// Constructeur
	CListBoxQuestions(void);

	// Destructeur
	virtual ~CListBoxQuestions(void);

	
// Attributes
protected:
	RECT		m_Rect;
	//Les couleurs des selections
	COLORREF	m_ColorSel;
	COLORREF	m_ColorTxtSel;
	COLORREF	m_ColorQuestion;
	COLORREF	m_ColorBloc;
	COLORREF	m_ColorQuestionNum;
	COLORREF	m_ColorMarqueSel;

	CFont		m_FontQuestion;
	CFont		m_FontBloc;
	CFont		m_FontItalique;

// Operations
public:

	// Reset de tous les elmts de la liste
	void			ResetContent();

	// Initilaisation du map sélection ("marquage")
	void			ResetMapSel();
	
	int				AddItem(const IQuestion * pData);
	int				AddItems(JList<IQuestion *> & Data);
	int				AddItems(JVector<IQuestion *> & Data);
	bool			RemoveItem(int ID);
	JInt32			GetSelectedID();
	bool			SetSelectedID(JInt32 ID);

	// Marquage et démarquage des élmts
	void SetModeMarquage	(bool MarqueSel);
	bool			MarqueSel(JInt32 ID);
	bool			DemarqueSel(JInt32 ID);

	JMap <JInt32,JInt32>   m_MapIdMarque;
	bool			m_MarqueSel;
	
	void			SetSelectionColor(COLORREF color );
	void			SetSelectedTextColor( COLORREF color );
	void			SetQuestionTextColor( COLORREF color );
	void			SetBlocTextColor( COLORREF color );
	void			SetQuestionNumTextColor( COLORREF color );

	// Item en cours de sélection, on le marque c'est tout
	void			SetMarqueColor(COLORREF color); 
	void			ResizeList();

protected:
	int				FindItemIndex(int ID);
	virtual int		CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void	MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

protected:

	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	void			NotifySelChanged();
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
