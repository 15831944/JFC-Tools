#pragma once

#include "resource.h"
#include "Terrain.h"
#include "ItemsConstruits.h"
#include "ATPColors.h"

#define WM_USER_QUESTIONLISTUPDATED (WM_USER + 101)
#define WM_USER_ITEMQUANTILISTUPDATED (WM_USER + 102)
#define WM_USER_ITEMLISTUPDATED		(WM_USER + 120)

/**
 * \ingroup RepItems
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
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbidden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CThemeTree : public CTreeCtrl
{
/// Construction
public:
	CThemeTree();

/// Attributes
public:
	CATPColors::APPCOLOR		m_CATPColorsApp;

/// Implementation
public:
	virtual ~CThemeTree();
	JList<IQuestion const *> &	GetQuestionList() { return m_Question_List; } ;
	virtual void				SetData(CTerrain *pTerrain, CItemsConstruitsBase * pIConst,CString & sFilter, int iModality);
	void						SetEtatData(JMap<DWORD,JUnt32> *pMapSelTree);
	void						SetColors();
	void						RemoveSelections();
	void						UpdateQuestionList();

	// Au lieu d'ajouter les questions dans une liste intermédiaire, on complète les branches enfants du treecontol
	void						UpdateQuestionTree(JBool ModeAjout = false, bool bSendMessageNow = false);
	void						UpdateQuestionTree(HTREEITEM hti);

	
	// Positionne le mode visu en passsant aussi le mode visu (Mode items, palettes, classes,quantiles,questions induites)
	// Par défaut mode items
	void						SetModeVisu(JUnt32 IdModeVisu = 1);

	// Sélection d'une branche tree
	void						SetBlock(JInt32 BlockID);

	// Sélection des questions
	void						SetQuestionList(JList <JInt32> & ListQuestion);

	// Sélection des questions
	void						SetQuestionList(HTREEITEM hti, JList <JInt32> &ListQuestion);

	// Les grandeurs analyse disponibles pour l'échelle
	enum {HTI_Open		= 1, 
		  HTI_Select	= 2, 
		  HTI_OpenSelect= 3}; 

	// Sauve position élmt tree en ouverture
	void SetOpenTree(HTREEITEM hti);

	// Sauve sélection élmt tree theme
	void SetSelectTree(HTREEITEM hti);

	// Noeud est-il ouvert ???
	bool IsTreeOuvert(HTREEITEM hti);

	// Noeud est-il sélectionné
	bool IsTreeSel(HTREEITEM hti);

	// Reset sélection et ouvertures tree item
	void ResetMapTree();

	// Actualisation tree item via la sélection sauvegardée
	void TreeViaSauvegarde(bool WithSelection = true);
	void TreeViaSauvegarde(HTREEITEM hti, bool WithSelection);

	// Indique l'item par défaut
	void SetItemByDefault(HTREEITEM hti);

	// Récupère l'item par défaut
	HTREEITEM GetItemByDefault();

	void			ExpandAll();

	/// Generated message map functions
protected:
	void			SetBlock(JList<JInt32> & BlockID);
	void			RemoveSelections(HTREEITEM hti);
	virtual void	ToggleItemState(HTREEITEM hti, bool WithSelection = true);
	void			ToggleChildItemState(HTREEITEM hti,int iImage);
	void			ToggleParentItemState(HTREEITEM hti,int iImage);
	bool			HasInactifChildren(HTREEITEM hti);
	bool			HasActifChildren(HTREEITEM hti);
	void			UpdateQuestionList(HTREEITEM hti);
	void			FilterTree();
	void			FilterTree(HTREEITEM hti);
	HTREEITEM		FindBlockQuestion(JInt32 BlockID);
	HTREEITEM		FindBlockQuestion(HTREEITEM hti, JInt32 BlockID);
	// void			ExpandAll();
	void			ExpandAll(HTREEITEM hti);
	JUnt32			NbConnect();
	int 			GetProfondeur(const HTREEITEM hitem) const;

	CImageList				m_imageState;
	CTerrain			 *	m_pTerrain;
	CItemsConstruitsBase *	m_pIConst;

	long					m_BlockId_Sel;


	int						m_iModality;
	CString					m_sFilter;
	JMap<JInt32,bool>		m_mElements;
	JMap<JInt32,bool>		m_mModalities;

	/// Questions list
	JList<IQuestion const *>	m_Question_List;

	// Id mode visualation (items, palettes,etc... par defaut item)	
	JUnt32		m_IdModeVisu;

	// Pointeur sur map sélection et ouvertures tree
	JMap<DWORD,JUnt32> *		m_pMapSelTree;

	// Item par défaut
	HTREEITEM					m_hItemByDefault;

	//{{AFX_MSG(CThemeTree)
	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


