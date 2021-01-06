/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCListCtrl
//
//	Descriptif:
//	ListCtrl avec couleur de sélection personnalisable et affichage d'elements dans une couleur différente.
*/

#pragma once



class AFX_EXT_CLASS JFCListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(JFCListCtrl)

public:
	JFCListCtrl();
	virtual ~JFCListCtrl();

	void SetTextColor(COLORREF Text) { m_ColorText = Text; Invalidate(true); }
	void SetBackColor(COLORREF Back) { m_ColorBk = Back; Invalidate(true); }
	void SetSelectColor(COLORREF Sel) { m_ColorSel = Sel; Invalidate(true); }
	void SetSelectedTextColor(COLORREF Sel) { m_ColorSelText = Sel; Invalidate(true); }
	void SetAttribueColor( COLORREF Sel ) { m_ColorAttribue = Sel; Invalidate(true); }

protected:
	COLORREF m_ColorBk;
	COLORREF m_ColorSel;
	COLORREF m_ColorText;
	COLORREF m_ColorSelText;
	COLORREF m_ColorAttribue;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	/*
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	*/

private:
	virtual bool IsAttribue(int ID) const {return false;};

/*
private:
	bool m_bMouseMove;
	int m_idxPrec, m_idxFirst, m_idxHaut, m_idxBas;
	bool m_bFirstSel;
*/
};


