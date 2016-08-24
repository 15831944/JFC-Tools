// Saisonnalite.h: interface for the CSaisonnalite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAISONNALITE_H__D5A0D668_38FD_444B_8B1F_F518293BF061__INCLUDED_)
#define AFX_SAISONNALITE_H__D5A0D668_38FD_444B_8B1F_F518293BF061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaisonnalite  
{
public:
	void SetModified();
	int IsModif();
	int SaveSaisonnalite();
	void SetSaisonnalite(int saisonnalite,int pos);
	int LoadSaison();
	CString m_NomFich;
	bool InitialiseExercice(COleDateTime date, int nbsemaines );
	double GetSaisonnalite(int semaine);
	bool ImportFile();
	CSaisonnalite();
	CSaisonnalite(const CSaisonnalite & data);
	virtual ~CSaisonnalite();
	CSaisonnalite & operator=(const CSaisonnalite & data)
	{
		m_NomFich=data.m_NomFich; 
		m_IsModified=data.m_IsModified; 
		m_Duree=data.m_Duree;
		m_Exercice = data.m_Exercice;
		m_CoeffSaisonnalite.Copy(data.m_CoeffSaisonnalite);
		return *this;
	}
private:
	int m_Duree;
	int m_IsModified;
	COleDateTime m_Exercice;
	CWordArray m_CoeffSaisonnalite;
};

#endif // !defined(AFX_SAISONNALITE_H__D5A0D668_38FD_444B_8B1F_F518293BF061__INCLUDED_)
