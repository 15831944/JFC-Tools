#pragma once

#include "resource.h"
#include "Terrain.h"
#include "ItemsConstruitsBase.h"

class CQstTree : public CTreeCtrl
{
/// Construction
public:
	CQstTree();
	virtual ~CQstTree();

	void	SetData(CTerrain *pTerrain, CItemsConstruitsBase * pIConst, JList<JInt32>& selIDlist);

	HTREEITEM GetFirstSelectedItem();
	HTREEITEM GetNextSelectedItem( HTREEITEM hItem );
	HTREEITEM GetPrevSelectedItem( HTREEITEM hItem );

	void ClearSelection();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);

private:


	void NotifySelChanged(HTREEITEM hItemFrom, HTREEITEM hItemTo);
	void AddQuestions(HTREEITEM hPar, JList<JInt32>& selIDlist);
	void FilterTree(HTREEITEM hti);

private:

	CATPColors::APPCOLOR				m_CATPColorsApp;
	CTerrain				*	m_pTerrain;
	CItemsConstruitsBase	*	m_pIConst;
	CImageList					m_Imagelist;

	JMap<HTREEITEM,bool>		m_mapItemSel;
};


