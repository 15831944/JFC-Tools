/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CRedListBox
//
//	Descriptif:
//	Boite fille de DialogChoisOffre
//	Redéfinition de JFC_MultiListBox pour afficher l'état attribué d'un support et classer les supports
*/

#pragma once

//JFCTables
#include <IElem.h>
//JFCControl
#include <JFC_MultiListBox.h>

class CDlgSupport;

class CRedListBox : public JFC_MultiListBox<IElem const *>
{
public:
	CRedListBox(void);
	virtual ~CRedListBox(void);

	CDlgSupport * m_pDlgParent;

	bool IsLotAttribue(int ID) const;
	bool IsCouplageAttribue(int ID) const;
	bool IsCouplageAttribue(IElem* elem) const;
	bool IsTitreAttribue(int ID) const;

	virtual int CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct);

protected:
	DECLARE_MESSAGE_MAP()

	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	TCHAR* m_pchTip;
	WCHAR* m_pwchTip;


private:
	virtual bool IsAttribue(int ID) const;/// redéfinition de la méthode

};
