// FileUtils.h: interface for the CFileUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEUTILS_H__2113CC02_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_FILEUTILS_H__2113CC02_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFileUtils  
{
#define TAILLEBUFFLIGNE			1000		//soit 1000 car. max
//#define TAILLEPOS				50			//soit 30 Objets max

public:
	CFileUtils();
	virtual ~CFileUtils();
	short LireEnregistrement(short & NrLigne, CStdioFile * Fic,short NbPos);
	short RechercheLigne(short & NrLigne,CString & Buf,CStdioFile * Fic, char Symbole);
	short LectureLigneV(short & NrLigne, CStdioFile * Fic,short NbPos);
	short LectureLigne(short & NrLigne, CStdioFile * Fic,short NbPos);
	short pascal Date_010180(long date);
	long pascal Date_Decode(short coddate);
	CString GetPtrPWOFic(CString & str, char sym1, char sym2, short decal);
protected:
	short FormatteChaine(short NbPos);
	short ExtraireChaine(short NbPos);
	short ExtraireVChaine(short NbPos);
	void MetVzero(LPSTR c, short t);
	void MetZero(LPSTR c, short t);
	void SupprimeCARIAGE_LINE(LPSTR txt);
	void SupBlancsFin(LPSTR ch);
protected:
	char m_BuffLigne[TAILLEBUFFLIGNE];
	CWordArray m_Pos;
};

#endif // !defined(AFX_FILEUTILS_H__2113CC02_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
