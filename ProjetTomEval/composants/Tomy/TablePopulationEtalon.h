// TablePopulationEtalon.h: interface for the CTablePopulationEtalon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEPOPULATIONETALON_H__45B25180_881A_43BA_BACE_9F50A2B42261__INCLUDED_)
#define AFX_TABLEPOPULATIONETALON_H__45B25180_881A_43BA_BACE_9F50A2B42261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTablePopulationEtalon  
{
public:
	CTablePopulationEtalon();
	virtual ~CTablePopulationEtalon();
	LPCSTR GetNomFichLien();
	bool GetPopulation(int NumeroRecherche,int & NumeroTrouve);
	short LoadFile(LPCSTR NomFic);


private:
	bool m_fLoaded;
	CMap <int,int,int,int> m_Lien;
	CString m_Nomfich;
};

#endif // !defined(AFX_TABLEPOPULATIONETALON_H__45B25180_881A_43BA_BACE_9F50A2B42261__INCLUDED_)
