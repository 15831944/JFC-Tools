#pragma once
#include "afxwin.h"
#include "resource.h"
// Bo�te de dialogue DlgSupportsInexistants

class DlgSupportsInexistants : public CDialog
{
	DECLARE_DYNAMIC(DlgSupportsInexistants)

public:
	DlgSupportsInexistants(const CString & sUserMsg, const CString & sListSupports);   // constructeur standard
	virtual ~DlgSupportsInexistants();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_FD_DLG_SUPPORTS_INEXISTANTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	// textbox contenant tous les supports inexistants
	CEdit m_TextSupports;
	const CString & m_sListSupports;

	// texte affich� pour l'utilisateur
	CStatic m_Static;
	const CString & m_sUserText;

	virtual BOOL OnInitDialog();
};
