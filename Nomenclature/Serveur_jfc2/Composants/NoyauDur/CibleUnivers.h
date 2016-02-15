// CibleUnivers.h: interface for the CCibleUnivers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIBLEUNIVERS_H__593EDCE2_5D29_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_CIBLEUNIVERS_H__593EDCE2_5D29_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include "dictionary.h"
#include "FileUtils.h"
#include "TableCible.h"

class CCibleUnivers : public CFileUtils
{
#define _INI					FichierIni
#define _INI_Local				"Localisation"
#define _INI_User				"USER"
#define _INI_Univers			"UNIVERS"

public:
	CCibleUnivers();
	CCibleUnivers(CString CheminIni, CString Marche);
	virtual ~CCibleUnivers();

	CTableCibleArray &DonneTableCibles(int Univers);
	bool Initialisation(CString CheminIni, CString Marche);
	void LibererCibles();

protected:	
	CTableCibleArray m_TableCibles;

	CString m_Marche,
			m_Univers;
	CString m_CheminEtudIni,
			m_CheminUnivers,
			m_CheminUser;
	bool m_Init;

protected:
	short ChargerCibles(char Type);
	void InitCibles(void);
};

#endif // !defined(AFX_CIBLEUNIVERS_H__593EDCE2_5D29_11D2_B3F9_006052017E36__INCLUDED_)
