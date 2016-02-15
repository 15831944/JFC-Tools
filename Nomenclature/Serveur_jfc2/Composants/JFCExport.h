// JFCExport.h: interface for the CJFCExport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCEXPORT_H__BF365AA0_4388_11D3_A1B9_0050048B3CF9__INCLUDED_)
#define AFX_JFCEXPORT_H__BF365AA0_4388_11D3_A1B9_0050048B3CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>

class CJFCExport
{
#define JFCEX_TEXT			1
#define JFCEX_SEPARATOR		0x3B		//point virgule
#define JFCEX_LF			0x0A		//line feed
#define JFCEX_CR			0XOD		//carriage return

public:
	CJFCExport();
	virtual ~CJFCExport();

private:
	int m_Type;								//Type de l'export (text, html, xls, rtf, ...)
	CString m_File;							//Fichier de destination
	CFile m_pFile;
	bool m_fFile;
	TCHAR m_Separator;

public:
	virtual int DoJFCExport(void);
	virtual int DoJFCExport(int Type,			//Type de l'export (text, html, xls, rtf, ...)
							LPCSTR File,		//Fichier de destination
							bool Erase=true);	//Supprime (true) ou ajoute (false) les informations au fichier
public:
	bool OpenJFCExport(void);
	bool CloseJFCExport(void);

	bool SetStr(LPCTSTR String, TCHAR Separator=JFCEX_SEPARATOR);
	bool SetChar(const TCHAR Caracteres, TCHAR Separator=JFCEX_SEPARATOR);
	bool SetInt(const int Number, TCHAR Separator=JFCEX_SEPARATOR, TCHAR Sign=0, LPCTSTR Type=NULL);
	bool SetLong(const long Number, TCHAR Separator=JFCEX_SEPARATOR, TCHAR Sign=0, LPCTSTR Type=NULL);
	bool SetDouble(const double Number, const short Precision=2, TCHAR Separator=JFCEX_SEPARATOR, TCHAR Sign=0, LPCTSTR Type=NULL);
	bool SetDateTime(const struct tm * Time, TCHAR Separator=JFCEX_SEPARATOR);
	bool SetDateTime(const COleDateTime * Time, TCHAR Separator=JFCEX_SEPARATOR);
	//insère 
	bool SetSeparator(const short nbSeparator=1, TCHAR Separator=JFCEX_SEPARATOR);
	bool SetLineFeed(const short nbLineFeed=1);

private:
	LPCTSTR SetSign(LPCTSTR String, TCHAR Sign);
	LPCTSTR SetType(LPCTSTR String, LPCTSTR Type);
	short GetStrSize(LPCTSTR String);
};

#endif // !defined(AFX_JFCEXPORT_H__BF365AA0_4388_11D3_A1B9_0050048B3CF9__INCLUDED_)
