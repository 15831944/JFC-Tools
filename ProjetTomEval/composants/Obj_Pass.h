// Obj_Pass.h: interface for the CObj_Pass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJ_PASS_H__77BCB12E_8954_11D4_BA64_C6B342CCF83D__INCLUDED_)
#define AFX_OBJ_PASS_H__77BCB12E_8954_11D4_BA64_C6B342CCF83D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CODEAPP_JFCPASS 0
#define CODEAPP_TOPCHRONO 1
#define CODEAPP_PIGEPRESSE 2

class CObj_Pass  
{
public:
	bool CheckChallenge(LPCSTR Reponse,LPCSTR NomFic);
	LPCSTR GetChallenge(int CodeApp);
	int IsLicenceValide(LPCSTR NomFic,int CodeApp);
	CObj_Pass();
	virtual ~CObj_Pass();

private:
	void PrepareCodage(int CodeApp);
	CDWordArray m_Codec;
	DWORD GetNumberToTranslate(char lecteur);
	bool WriteLicence(LPCSTR NomFic,int CodeApp,int DateJJMMAA);
	int m_NumeroaDecoder;
	int m_CodeApp;
};

#endif // !defined(AFX_OBJ_PASS_H__77BCB12E_8954_11D4_BA64_C6B342CCF83D__INCLUDED_)
