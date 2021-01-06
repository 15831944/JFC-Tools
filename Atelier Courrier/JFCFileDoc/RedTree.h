/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CRedTree
//
//	Descriptif:
//	TreeCtrl affichant l'attribution des noeuds dans un terrain.
*/

#pragma once

//JFCControl
#include "SupportTree.h"

class CDlgSupport;

class CRedTree : public CSupportTree
{
public:
	CRedTree(void);
	virtual ~CRedTree(void);

	CDlgSupport * m_pDlgParent;

	bool IsTitreAttribue(int ID) const;

protected:
	DECLARE_MESSAGE_MAP()

	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	TCHAR* m_pchTip;
	WCHAR* m_pwchTip;

protected:
	virtual bool IsAttribue(int ID) const;/// redéfinition de la méthode


};
