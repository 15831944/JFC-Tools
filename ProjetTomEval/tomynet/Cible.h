// Cible.h: interface for the CCible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIBLE_H__6C2A8757_0B41_4265_B15A_88BC3F697F9A__INCLUDED_)
#define AFX_CIBLE_H__6C2A8757_0B41_4265_B15A_88BC3F697F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCible  
{
public:
	void UnloadCible();
	bool LoadCible(LPCSTR CheminData,LPCSTR CodeVague,int NumeroCible);
	CCible(int NbIndivPanel);
	CCible();
	virtual ~CCible();

	int m_NrCible;
	int m_NbIndividuPanel;
	int m_Population;
	int m_NbCasCible;
	long * m_CoeffEx;
private:
	bool LoadCible(LPCSTR NomFic);

};

#endif // !defined(AFX_CIBLE_H__6C2A8757_0B41_4265_B15A_88BC3F697F9A__INCLUDED_)
