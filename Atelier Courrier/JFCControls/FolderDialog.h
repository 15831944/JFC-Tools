/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CFolderDialog
//
//	Descriptif:
//	Encapsulation de la boite de recherche de dossier
*/

#pragma once

class AFX_EXT_CLASS CFolderDialog
{
public:
	CFolderDialog(void);
	CFolderDialog(HWND hwndParent, CString& path, CString ttlDlg, bool bNoNewFolderBtn, bool bEditBox);
	virtual ~CFolderDialog(void);


	void Init(HWND hwndParent, CString& path, CString ttlDlg, bool bNoNewFolderBtn, bool bEditBox);
	
	INT_PTR DoModal();

	CString GetPath();

private:

	CString m_path;
	
	BROWSEINFO m_bi;
	

	//pour choix du dossier, code des MSDN
	static int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData);

};
