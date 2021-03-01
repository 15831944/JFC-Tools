#pragma once
#include "afxwin.h"
#include "Resource.h"
#include <Label.h>
#include <BtnST.h>
#include "JFCXListBox.h"
#include "RedListBox.h"

// CDlgMessage dialog

class CDlgMessage : public CDialog
{
	DECLARE_DYNAMIC(CDlgMessage)

public:
	CDlgMessage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMessage();

// Dialog Data
	enum { IDD = IDD_FD_DLG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Liste contenant tous les messages
	CListBox m_Lst_Message;
	// CJFCXListBox m_Lst_Message;
	// CRedListBox m_Lst_Message;

	CButtonST	m_Btn_Ok;
	CButtonST	m_Btn_Cancel;

	// Texte question message
	CStatic		m_TxtQuMessage;

public:
	// Affichage du titre boite message 
	void SetTitre(CString TxtTitre);

	// Affichage de la question finale
	void SetTexteQuestion(CString TxtQuestion);

	// Ajout message dans la liste
	void AddMessage(CString Message);

private:
	// String de remplissage des elmts boite dialogue message
	CString			m_Titre;
	CString			m_Question;
	JList <CString> m_Ligne_Message;

public:
	virtual BOOL OnInitDialog();

};
