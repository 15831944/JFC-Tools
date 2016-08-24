// TarifAnnee.h: interface for the CTarifAnnee class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARIFANNEE_H__DC61B9D3_72CA_4C8A_A4EF_AFCDAD316934__INCLUDED_)
#define AFX_TARIFANNEE_H__DC61B9D3_72CA_4C8A_A4EF_AFCDAD316934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTarifAnnee  
{

public:
	double GetTarif(int ligne,int sem);
	bool IsModified();
	void ModifyCode(LPCSTR Code,LPCSTR NewCode);
	void ModifyTarif(LPCSTR Code,double *Tarif);
	int AddTarif(LPCSTR Code, double * Tarif,bool flagSetModify=true);
	int GetIndexForCode(LPCSTR Code);

	void RemoveTarif(LPCSTR Code);
	int LoadTarif();
	int SaveTarif();

	// nom du fichier de tarif
	CString m_NomFich;
	
	CTarifAnnee & operator= (const  CTarifAnnee & Data)
	{
		m_NomFich=Data.m_NomFich;
		m_flagModified=Data.m_flagModified;
		m_Ligne.Copy(Data.m_Ligne);
		return *this;
	}
	// neutral constructor
	CTarifAnnee()
	{
		m_flagModified=0;
	};

	CTarifAnnee(const  CTarifAnnee & Data)
	{	
		*this=Data;
	};

	virtual ~CTarifAnnee();

private:
	int m_flagModified;
	// tableau pour stocker les tarifs [index][semaine]
	class tagTarif
	{
		public:
		CString m_Code;
		double m_Tarif[53];
	};
	CArray<tagTarif,tagTarif&> m_Ligne;
};

#endif // !defined(AFX_TARIFANNEE_H__DC61B9D3_72CA_4C8A_A4EF_AFCDAD316934__INCLUDED_)
