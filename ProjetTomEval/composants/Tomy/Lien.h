// Lien.h: interface for the CLien class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIEN_H__082532F7_D8F4_41FE_A90E_0E49C5755B86__INCLUDED_)
#define AFX_LIEN_H__082532F7_D8F4_41FE_A90E_0E49C5755B86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3D9AB7D5032C
class CLien  
{
public:
	//##ModelId=3D9AB7D50336
	LPCSTR GetNomFichLien();
	//##ModelId=3D9AB7D50340
	bool GetLien(int NumeroRecherche,int & NumeroTrouve);
	//##ModelId=3D9AB7D50343
	CLien();
	//##ModelId=3D9AB7D5034A
	virtual ~CLien();
	//##ModelId=3D9AB7D5034C
	short LoadFile(LPCSTR NomFic);


private:
	//##ModelId=3D9AB7D5034E
	bool m_fLoaded;
	//##ModelId=3D9AB7D5035F
	CMap <int,int,int,int> m_Lien;
	//##ModelId=3D9AB7D50368
	CString m_Nomfich;
};

#endif // !defined(AFX_LIEN_H__082532F7_D8F4_41FE_A90E_0E49C5755B86__INCLUDED_)
