#pragma once
#include "afxwin.h"

#include <grid\gxwnd.h>
#include "IItemTable.h"


class CAnCrossTable :
	public CGXGridWnd
{

public:
	CAnCrossTable(void);
	virtual ~CAnCrossTable(void);

	// Récup informations ligne/col + infos si valeur cellule définie (pour les moyennes)
	virtual BOOL GetStyleRowCol(ROWCOL nRow,
                                ROWCOL nCol, 
								CGXStyle& style,
                                GXModifyType mt, 
								int nType);

	// Idem fonction GetStyleRowCol, mais en globalité OPTIM 2.7
	virtual BOOL GetTotStyleRowCol(ROWCOL nRowBegin, ROWCOL nRowEnd,
							    ROWCOL nColBegin, ROWCOL nColEnd, 
								CGXStyle& style,
								GXModifyType mt, 
								int nType);

	virtual bool SetIItemTable(IItemTable * pTable);
	virtual void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

	void SetTooltipText(int nText, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);

	void ActivateTooltip(BOOL bEnable = TRUE);
	DWORD EnableBalloonTooltip();

	// Affichage en mode tooltip info cellule
	void AfficheInfoCellule(ROWCOL nRow, ROWCOL nCol, CString ValReelle, bool AffichOk);

	// Renvoi coordonnées basses via ligne
	void CalcLimitTopBottomRow(ROWCOL nRow, int &Top, int &Bottom);
	
	// Update dessin de toutes les lignes et colonnes
	void UpdateAllRowsCols();

	// Update taille de toutes les lignes et colonnes
	void UpdateAllRowsColsSize();

	// Indique s'il faut rendre invisible certaines lignes moyennes
	bool SomeRowToHide();


	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAnCrossTable)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	IItemTable * m_pItemTable;
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	//virtual BOOL LButtonDown(UINT nFlags, CPoint point, UINT nHitState);
	//virtual BOOL OnLButtonHitRowCol    (ROWCOL nHitRow, ROWCOL nHitCol, ROWCOL nDragRow, ROWCOL nDragCol, CPoint point, UINT flags, WORD nHitState);

	virtual BOOL OnTrackColWidth(ROWCOL nCol);
	virtual BOOL OnTrackRowHeight(ROWCOL nRow);
	virtual void OnTimer(UINT nIDEvent);
	virtual BOOL OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL OnGridKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnChangedSelection(const CGXRange* pRange, BOOL bIsDragging, BOOL bKey);
	virtual BOOL Copy();
	virtual BOOL Paste( ) { return false; } ;

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI);

private:
	void InitToolTip();
	CToolTipCtrl m_ToolTip;
	DWORD		m_dwToolTipStyle;	///< Style of tooltip control

	// VAriables pour gestion affichage des zones à surligner
	ROWCOL							m_CurRow;
	ROWCOL							m_CurCol;
	JBool							m_RowColChange;

	// Compte les elmts grandeurs affichés
	unsigned int					m_ItemColCount;
	unsigned int					m_ItemRowCount;

	// Dernier time
	LONG m_LastTimeUpdate;
	JBool m_InvalidateAllOk;

public:
	// Map pour chaque ligne spécifiant s'il faut afficher la moyenne (moyenne significative)
	JMap <JInt32, bool> m_MapRowMoyenneAffich;

};

