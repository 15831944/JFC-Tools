// TraceFile.h: interface for the CTraceFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACEFILE_H__D1388180_FFD3_11D3_989E_004095420987__INCLUDED_)
#define AFX_TRACEFILE_H__D1388180_FFD3_11D3_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTraceFile  
{
public:
	void Close();
	void Write(LPCSTR txt);
	void Open(LPCSTR nomfic);
	CTraceFile();
	virtual ~CTraceFile();
private:
	CStdioFile m_File;
	bool m_FileOpened;
};

#endif // !defined(AFX_TRACEFILE_H__D1388180_FFD3_11D3_989E_004095420987__INCLUDED_)
