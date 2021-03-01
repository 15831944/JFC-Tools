/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgRename
//
//	Descriptif:
//	Boite contextuelle permettant de renommer des éléments
*/

#pragma once

#include <label.h>
#include "Resource.h"


class CDlgRename : public CDialog
{
/// Construction
public:
	CDlgRename(CWnd* pParent = NULL);   /// standard constructor
	virtual BOOL OnInitDialog();

/// Dialog Data
	enum { IDD = IDD_RS_DLGRENAME };
	CString	m_Libelle;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnNcHitTest(CPoint point);

private:
	CLabel	m_Label;
	CLabel	m_Box;
};

