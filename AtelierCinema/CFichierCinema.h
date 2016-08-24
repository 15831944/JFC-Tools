#if !defined(AFX_CFICHIERCINEMA_H__0250D9C9_FC07_4F5D_B9C4_672F526C807C__INCLUDED_)
#define AFX_CFICHIERCINEMA_H__0250D9C9_FC07_4F5D_B9C4_672F526C807C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFichierCinema
{
//	DECLARE_DYNAMIC(CFichierCinema)

public:
	void SetDelimitor(CString delimiteur);
	void SetChemin(LPCSTR chemin);
	void Close();
	bool WriteLine(LPCSTR str);
	void SetSeparator(CString separateur);
	// Opérations
	bool FileExist(LPCSTR nomFich);
	bool OpenFileForWrite(LPCSTR nomFich);
	bool OpenFileForRead(LPCSTR NomFic,int messageErreur);
	CFichierCinema();
	virtual ~CFichierCinema();
	int ReadLine();
	int GetNbElement(){return m_TabElement.GetSize();};
	LPCSTR GetElement(int position);
private:
	CString m_separator;
	CString m_delimitor;
	CStdioFile m_File;
	CString m_Chemin;
	CStringArray m_TabElement;
};

#endif // !defined(AFX_TOMJOB_H__0250D9C9_FC07_4F5D_B9C4_672F526C807C__INCLUDED_)
