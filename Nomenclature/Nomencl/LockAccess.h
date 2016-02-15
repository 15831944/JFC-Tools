// LockAccess.h: interface for the CLockAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCKACCESS_H__B83E01EF_0C77_11D4_989E_004095420987__INCLUDED_)
#define AFX_LOCKACCESS_H__B83E01EF_0C77_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLockAccess  
{
public:
	void UnlockFile();
	bool LockFile(LPCSTR nomfile);
	CLockAccess();
	virtual ~CLockAccess();
private:
	bool m_fFileOpen;
	CFile m_LckFile;

};

#endif // !defined(AFX_LOCKACCESS_H__B83E01EF_0C77_11D4_989E_004095420987__INCLUDED_)
