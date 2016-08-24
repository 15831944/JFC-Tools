// TableFormat.h: interface for the CTableFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEFORMAT_H__B08F6698_33F0_4AC7_9DC7_55613C93DE1C__INCLUDED_)
#define AFX_TABLEFORMAT_H__B08F6698_33F0_4AC7_9DC7_55613C93DE1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Table.h"

// On définit une structure contenant les paramètres du Format: libellé et coefficient
class tagFormat : public CtagTable
{
public:
	// coefficient associé au libellé
	double m_Coeff;
	// libellé du Format
	CString m_Libelle;
	tagFormat & operator=(const  tagFormat & Data)
	{

		m_Libelle=Data.m_Libelle;
		m_Coeff=Data.m_Coeff;
		return *this;
	};
	
	// Neutral constructor
	tagFormat(){
		m_Coeff=0.0;
	};
	// Copy constructor
	tagFormat(const  tagFormat & Data)
	{
		*this=Data;
	};
	double GetCoeff(){return(m_Coeff);};
	// Particularisation des fonctions de CtagTable
	LPCSTR GetLibelle(){return(m_Libelle);};
	LPCSTR GetCode(){return(0);};

	
};

// Tableau de BAC
//typedef CArray<tagFormat,tagFormat&> CTabBAC;
typedef CTable<tagFormat,tagFormat&> CTableFormatMere;

class CTableFormat : public CTableFormatMere
{
public:
	int SaveTable();
	int FindIndexFromCode(LPCSTR code);
	double GetCoefficient(LPCSTR libelle);

	CTableFormat & operator=(const  CTableFormat & Data)
	{
			Copy(Data);
			IsModified=Data.IsModified; 
			m_NomFich=Data.m_NomFich;
			return *this;
	};

	CTableFormat();
	virtual ~CTableFormat();
	// copy constructor
	CTableFormat(const CTableFormat & Data);

	// chargement de la table des formats
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

#endif // !defined(AFX_TABLEFORMAT_H__B08F6698_33F0_4AC7_9DC7_55613C93DE1C__INCLUDED_)
