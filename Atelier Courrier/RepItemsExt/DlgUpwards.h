#pragma once

#include "JFCListBox.h"
#include "label.h"
#include "BtnST.h"

#include "Resource.h"

// CDlgUpwards dialog

class CDlgUpwards : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpwards)

public:
	CDlgUpwards(const CString & Message, const CString &Titre, const JMap<JInt32, JLabel> & Upwards, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUpwards();

// Dialog Data
	enum { IDD = IDD_RI_UPWARDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CLabel      m_Box;
	JFCListBox	m_ListBox;
	CButtonST   m_OK;

	// Titre fenêtre (nom de l'item à supprimer)
	CString m_Titre;

private:
	CATPColors::APPCOLOR m_CATPColorsApp;

private:
	const JMap<JInt32, JLabel> & m_Upwards;
	const CString & m_Message;

};
