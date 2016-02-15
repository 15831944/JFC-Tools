// Dictionary.h: interface for the CDictionary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICTIONARY_H__77DF6E4F_A7AC_11D1_9125_004005327F6C__INCLUDED_)
#define AFX_DICTIONARY_H__77DF6E4F_A7AC_11D1_9125_004005327F6C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDictionary  
{

#define SIZE_ENTETE 8
#define SIZE_BODY 24
#define SIZE_ETRANGER 20
#define SIZE_END 4
#define	ERR_DEFINE "ErrDicoDefine"
#define	ERR_INDICE "ErrDicoIndice"
#define	ERR_DICO "ErrDicoEmpty"

public:
	CDictionary();
	CDictionary(LPCSTR FileName);
	virtual ~CDictionary();
	void NewDico(LPCSTR FileName);
	LPCSTR GetDico(LPCSTR Define);
	LPCSTR GetDico(WORD Indice);
	int	GetNbElements();
	CString GetElement(int Element);


protected:
	void LoadDico(LPCSTR FileName);	
	CStringArray m_DicoText;
	CWordArray m_DicoIndice;
	CStringArray m_DicoDefine;
};

#endif // !defined(AFX_DICTIONARY_H__77DF6E4F_A7AC_11D1_9125_004005327F6C__INCLUDED_)
