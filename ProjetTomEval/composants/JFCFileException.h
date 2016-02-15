// JFCFileException.h: interface for the CJFCFileException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCFILEEXCEPTION_H__7DAB2021_6F1C_11D2_B3FA_006052017E36__INCLUDED_)
#define AFX_JFCFILEEXCEPTION_H__7DAB2021_6F1C_11D2_B3FA_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//##ModelId=3D9AB7D50391
class CJFCFileException : public CFileException
{
public:
	//##ModelId=3D9AB7D503A4
	CJFCFileException();
	//##ModelId=3D9AB7D503A5
	virtual ~CJFCFileException();
	//##ModelId=3D9AB7D503A7
	void AfficheErreur(CString msg=_T(""));
	//##ModelId=3D9AB7D503AF
	CString m_strerr;
};

#endif // !defined(AFX_JFCFILEEXCEPTION_H__7DAB2021_6F1C_11D2_B3FA_006052017E36__INCLUDED_)
