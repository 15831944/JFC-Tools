#pragma once

#include "Terrain.h"
#include "JSRCTables.h"
#include "label.h"
#include "BtnST.h"

/// CPatrimoineDlg dialog
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
 * \date 07-15-2003
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
class CPatrimoineDlg : public CDialog
{
	DECLARE_DYNAMIC(CPatrimoineDlg)

public:
	CPatrimoineDlg(CWnd* pParent = NULL);   ///< standard constructor
	virtual ~CPatrimoineDlg();
	/// Initialise the dialog
	virtual BOOL OnInitDialog ( void );
	/// Dialog Data
	enum { IDD = IDD_RI_DLG_PATRIMOINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///< DDX/DDV support

	void LoopItems(CTerrain *pTerrain, LPCSTR LibelleSrc, const JTerrain &Ter);
	UINT LoopTerrains(LPVOID pParam);

	void ExpandAll(HTREEITEM hti);

	DECLARE_MESSAGE_MAP()

private:
	struct KeyItem
	{
		bool operator < (const KeyItem &rhs) const {
			if(m_ItemID < rhs.m_ItemID)
				return true;
			if(m_ItemID == rhs.m_ItemID)
			{
				if(m_SourceID < rhs.m_SourceID)
					return true;
			}
			return false;
		}
		bool operator > (const KeyItem &rhs) const {
			if(m_ItemID > rhs.m_ItemID)
				return true;
			if(m_ItemID == rhs.m_ItemID)
			{
				if(m_SourceID > rhs.m_SourceID)
					return true;
			}
			return false;
		}
		int			m_ItemID;
		int			m_SourceID;
	};

public:
	CATPColors::APPCOLOR			m_CATPColorsApp;
	JMap<int,HTREEITEM>		m_hItem;
	JMap<KeyItem,HTREEITEM>	m_hSource;
	CLabel					m_Box;
	CButtonST				m_Button;
	CTreeCtrl				m_Tree;

public:
	afx_msg void OnStart();
	afx_msg void OnNMCustomdrawTree(NMHDR *pNMHDR, LRESULT *pResult);
};
