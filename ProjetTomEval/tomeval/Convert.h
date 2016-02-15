// Convert.h: interface for the CConvert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERT_H__EFC395A0_A6F0_11D2_843A_004005327F70__INCLUDED_)
#define AFX_CONVERT_H__EFC395A0_A6F0_11D2_843A_004005327F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConvert  
{
public:
	int GetNrCible(int);
	int GetNrCible(LPCSTR);
	short Load(LPCSTR Path);
	CConvert();
	virtual ~CConvert();

private:
	struct SConvert{
	public:
		char m_Libelle[20];
		int m_Numero;
		int m_Convert;
	};
	CArray<SConvert,SConvert> m_Data;

	bool m_fLoaded;
};

#endif // !defined(AFX_CONVERT_H__EFC395A0_A6F0_11D2_843A_004005327F70__INCLUDED_)
