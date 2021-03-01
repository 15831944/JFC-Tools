#include "StdAfx.h"
#include ".\folderdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFolderDialog::CFolderDialog(void)
{
	memset((LPVOID)&m_bi, 0, sizeof(m_bi));
	m_path = "";
}

CFolderDialog::CFolderDialog(HWND hwndParent, CString& path, CString ttlDlg, bool bNoNewFolderBtn, bool bEditBox)
{
	memset((LPVOID)&m_bi, 0, sizeof(m_bi));
	m_path = "";
	Init(hwndParent, path, ttlDlg, bNoNewFolderBtn, bEditBox);
}

CFolderDialog::~CFolderDialog(void)
{
}

void CFolderDialog::Init(HWND hwndParent, CString& path, CString ttlDlg, bool bNoNewFolderBtn, bool bEditBox)
{
	m_bi.hwndOwner = hwndParent;
	m_bi.pidlRoot = NULL;
	m_bi.pszDisplayName = 0;
	m_bi.lpszTitle = ttlDlg;
	m_bi.lParam = (LPARAM)path.GetBuffer();

	m_bi.ulFlags = BIF_NEWDIALOGSTYLE;
	if(bNoNewFolderBtn)
		m_bi.ulFlags |= BIF_NONEWFOLDERBUTTON;
	if(bEditBox)
		m_bi.ulFlags |= BIF_EDITBOX | BIF_VALIDATE;

	m_bi.lpfn = BrowseCallbackProc;
	path.ReleaseBuffer();
}
	
INT_PTR CFolderDialog::DoModal()
{

	LPITEMIDLIST pIIL = ::SHBrowseForFolder(&m_bi);


	TCHAR szInitialDir[_MAX_PATH];
	
	if ( SHGetPathFromIDList(pIIL, (char*)&szInitialDir) )
	{
		if (szInitialDir != "" )
		{
			m_path = szInitialDir;
			return IDOK;
		}
	}
	
	return IDCANCEL;
}

CString CFolderDialog::GetPath()
{
	return m_path;
}


//pour choix du dossier à partir du dossier actuel, code copié des MSDN
int __stdcall  CFolderDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) 
{
	//on initialise le dossier
	TCHAR szDir[MAX_PATH];
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		
		strcpy_s(szDir, MAX_PATH, (char*)pData);
		// WParam is TRUE since you are passing a path.
		// It would be FALSE if you were passing a pidl.
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
		break;
		
	case BFFM_SELCHANGED:
		// Set the status window to the currently selected path.
		if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir))
		{
			::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
		}
		break;
	}
	return 0;
}