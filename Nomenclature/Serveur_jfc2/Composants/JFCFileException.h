// JFCFileException.h: interface for the CJFCFileException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCFILEEXCEPTION_H__7DAB2021_6F1C_11D2_B3FA_006052017E36__INCLUDED_)
#define AFX_JFCFILEEXCEPTION_H__7DAB2021_6F1C_11D2_B3FA_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CJFCFileException : public CFileException
{
public:
	CJFCFileException();
	virtual ~CJFCFileException();
	void AfficheErreur(CString msg=_T(""));
	CString m_strerr;
};

#endif // !defined(AFX_JFCFILEEXCEPTION_H__7DAB2021_6F1C_11D2_B3FA_006052017E36__INCLUDED_)
