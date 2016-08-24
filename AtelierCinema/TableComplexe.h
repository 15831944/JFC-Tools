// TableComplexe.h: interface for the CTableComplexe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLECOMPLEXE_H__D034F795_7F96_4049_8E79_AA032F2A43CF__INCLUDED_)
#define AFX_TABLECOMPLEXE_H__D034F795_7F96_4049_8E79_AA032F2A43CF__INCLUDED_

#include "CFichierCinema.h"	// Added by ClassView
#include "Table.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// On définit une structure contenant les paramètres du BAC : code et libellé
class tagComplexe : public CtagTable
{
public:
	
	// code du complexe
	CString m_Code;

	// libellé du complexe
	CString m_Libelle;

	// code du bac auxquel il est rattaché
	CString m_CodeBAC;

	// Régie du complexe
	char m_Regie;

	// part de marché du complexe
	double m_PartDeMarche;

	WORD m_indexBAC;
	tagComplexe & operator=(const  tagComplexe & Data)
	{
		m_Code=Data.m_Code;
		m_Libelle=Data.m_Libelle;
		m_Regie=Data.m_Regie;
		m_CodeBAC=Data.m_CodeBAC;
		m_indexBAC=Data.m_indexBAC;
		m_PartDeMarche = Data.m_PartDeMarche;
		return *this;
	};

	// Neutral constructor
	tagComplexe(){
		m_PartDeMarche=0.0;
	}
	// Copy constructor
	tagComplexe(const  tagComplexe & Data)
	{
		*this=Data;
	};


	LPCSTR GetCodeBAC(){return(m_CodeBAC);};
	// Particularisation des fonctions de CtagTable
	LPCSTR GetLibelle(){return(m_Libelle);};
	char GetRegie(){return(m_Regie);};
	LPCSTR GetCode(){return(m_Code);};
	double GetPartDeMarche(){return(m_PartDeMarche);}
};

typedef CTable<tagComplexe,tagComplexe&> CTableComplexeMere;

class CTableComplexe : public CTableComplexeMere  
{
public:
	void Decale(int idcomplexe);
	void Tri();
	LPCSTR GetCodeBac(int position);

	char GetRegie(int idxComplexe)
	{
	
		tagComplexe Cpx = GetAt(idxComplexe);
		return(Cpx.m_Regie);
	
	};
	
	bool SaveTable();
	CTableComplexe & operator=(const  CTableComplexe & Data)
	{
		Copy(Data);
		IsModified=Data.IsModified;
		m_NomFich=Data.m_NomFich;
		return *this;
	};
	CTableComplexe();
	CTableComplexe(const  CTableComplexe & Data);
	virtual ~CTableComplexe();
	bool LoadTable();
	int IsModif()
	{
		return IsModified; 
	};
	void SetModified(int modif)
	{
		IsModified = modif;
	};

private:
	CString m_NomFich;
	int IsModified;

};

#endif // !defined(AFX_TABLECOMPLEXE_H__D034F795_7F96_4049_8E79_AA032F2A43CF__INCLUDED_)
