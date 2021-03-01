/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CSupportTree
//
//	Descriptif:
//	TreeCtrl spécialisé dans l'affichage des familles et centres
//	Permet le Drag and drop et la selection multiple.
*/



#ifndef INC_SUPPORTTREE_H
#define INC_SUPPORTTREE_H

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSupportTree window

#include <AllTypeOfNode.h>
#include <ATPColors.h>

class AFX_EXT_CLASS CSupportTree : public CTreeCtrl
{
public:

	// Type d'item dans l'arborescence famille
	enum TYPE_ITEM
	{
		Item_Root		=	0,
		Item_Famille	=	1,
		Item_Centre		=	2,
	};

	// Constructeur
	CSupportTree();

	// Destructeur
	virtual ~CSupportTree();

	DECLARE_DYNAMIC( CSupportTree )

	// Remplissage arbre supports
	void FillTree();

	void	SetSelectionColor(COLORREF selColor);
	void	SetAttribueColor( COLORREF color );
	void	SetIconsColor(int appColor);

	void	EnableDragDrop(bool bEnable); 
	void	EnableEdit(bool bEnable); //si T alors F et T pour les 2 autres
	void	EnableFamilleNA(bool bEnable);
	void	ShowSupport(bool bEnable);
	void	EnableExpandAll(bool bEnable);
	void	EnableMultipleSel(bool bEnable);

	// Avant en private
	void ClearSelection();

#define ICON_ALL	0
#define ICON_TITRE	3
#define ICON_F		4
#define NBICON		ICON_F + 2*CATPColors::MAX_COLORS
#define ICON_C		ICON_F + CATPColors::MAX_COLORS


	void SetData( CRegroupements * pRegroup);

	bool IsIDSelected(JInt32 id);
	HTREEITEM GetFirstSelectedItem();
	HTREEITEM GetNextSelectedItem( HTREEITEM hItem );
	HTREEITEM GetPrevSelectedItem( HTREEITEM hItem );

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCtxFamilleAdd();
	afx_msg void OnCtxCentreAdd();
	afx_msg void OnCtxSuppr();
	afx_msg void OnCtxRenommer();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

//InfoTip
//	afx_msg void OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
//ToolTip
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	TCHAR* m_pchTip;
	WCHAR* m_pwchTip;

	afx_msg BOOL OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	void InsertTree(INode* nd, int proflim=0, HTREEITEM hroot=NULL, int prof=1);
	int 	GetProfondeur(const HTREEITEM hitem) const;
	
	
	BOOL SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);
	void NotifySelChanged(HTREEITEM hItemFrom, HTREEITEM hItemTo);
	void	SetNewStyle(long lStyleMask, BOOL bSetBits);
	void	OnButtonUp();
	BOOL	IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);
	BOOL	TransferItem(HTREEITEM hitem, HTREEITEM hNewParent);

	virtual bool IsAttribue(int ID) const { return FALSE; };/// redéfinition de la méthode

	// Fabrique nom nouvelle famille
	CString GetNewNameFamille();

	// Fabrique nom nouveau centre d'intérêt
	CString GetNewNameCentre();

	// Vérifie Item non déjà présent
	bool DoublonItem(HTREEITEM hItem,CString NewText);

	// Fonction donnant le rang de la branche (famille ou centre)
	JUnt32 GetRangItem(HTREEITEM hItem);

// Attributes
private:
	BOOL        m_bDragging; //Etat du drag
	HTREEITEM   m_hitemDrag; //item draggé
	HTREEITEM   m_hitemDrop; //item sur lequel on drop
	bool m_bSelectionComplete;
	CImageList  m_Imagelist;
	CImageList  *m_pimagelist;
	COLORREF	m_selColor;
	COLORREF	m_ColorAttribue;
	int			m_appColor;

	CRegroupements * m_pRegroupements;

	bool m_bEditable;
	bool m_bDragDrop;
	bool m_bFamNA;
	bool m_bSupports;
	bool m_bExpandAll;
	bool m_bMultiSel;

};

#endif //#define INC_SUPPORTTREE_H
