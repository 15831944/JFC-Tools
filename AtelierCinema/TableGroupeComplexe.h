// TableGroupeComplexe.h: interface for the CTableGroupeComplexe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEGROUPECOMPLEXE_H__5416A9CD_6348_478C_BCCD_D7CEDBE96ACE__INCLUDED_)
#define AFX_TABLEGROUPECOMPLEXE_H__5416A9CD_6348_478C_BCCD_D7CEDBE96ACE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFichierCinema.h"
#include "Table.h"


////////////////////////////////////////////////////////// TABLE DES GROUPES DE COMPLEXESS ///////////////////////////////////////////////////
//
// On définit une structure contenant les paramètres du groupe BAC : code et libellé
class tagComplexeGroupe : public CtagTable
{
public:
	// libellé du groupe de complexes
	CString m_LibelleGroupe;

	// code du groupe (= n° du groupe)
	CString m_CodeGroupe;

	// Nombre de complexes composant le groupe (= n° du groupe)
	int m_NbComplexes;

	// Code complexes qui composent ce groupe
	CArray<int,int> m_CompoComplexes; 

	// Affectation
	tagComplexeGroupe & operator=(const  tagComplexeGroupe & Data)
	{
		m_LibelleGroupe	= Data.m_LibelleGroupe;
		m_CodeGroupe	= Data.m_CodeGroupe;
		m_NbComplexes		= Data.m_NbComplexes;
		m_CompoComplexes.Copy(Data.m_CompoComplexes);
		
		return *this;
	};

	// Neutral constructor
	tagComplexeGroupe()
	{
		m_LibelleGroupe = "";
		m_NbComplexes	= 0;
		m_CodeGroupe			= "";
		m_CompoComplexes.RemoveAll();
	}
	// Copy constructor
	tagComplexeGroupe(const  tagComplexeGroupe & Data)
	{
		*this=Data;
	};


	LPCSTR GetLibelle(){return(m_LibelleGroupe);};
	LPCSTR GetCode(){return("");};

	int GetNbComplexes(){return(m_NbComplexes);};

	void GetCompoComplexes(CArray<int,int> &CompoComplexes)
	{
		CompoComplexes.RemoveAll();
		for (int idx=0; idx < m_CompoComplexes.GetSize(); idx++)
		{
			CompoComplexes.Add(m_CompoComplexes.GetAt(idx));
		}
	};
		
};

// Tableau de groupe de Complexes
typedef CTable<tagComplexeGroupe,tagComplexeGroupe&> CTableComplexeGroupeMere;


class CTableGroupeComplexe  : public CTableComplexeGroupeMere
{
	public:

		int index;

		// Récup bac d'après le code bac
		tagComplexeGroupe GetComplexe(CString codecomplexe);

		// opérateur =
		CTableGroupeComplexe & operator=(const  CTableGroupeComplexe & Data)
		{
			Copy(Data);
			IsModified=Data.IsModified;
			m_NomFich=Data.m_NomFich;
			return *this;
		};

		// constructeur et destructeur
		CTableGroupeComplexe();

		CTableGroupeComplexe(const  CTableGroupeComplexe & Data);
		virtual ~CTableGroupeComplexe();

		// Initialisation
		bool LoadTable(CString CheminData);

		// void Tri();
		bool SaveTable();

		// définition de m_MapCodeToIndex
		void MapCodeToIndex(CString code,int index);

		// Suppression des éléments
		void RemoveMap();

		bool CodeExist(CString code);
	
		void SetModified(int modif)
		{
			IsModified = modif;
		};

		int IsModif()
		{
			return IsModified; 
		};



	private:
		CString m_NomFich;
		int		IsModified;

};

#endif // !defined(AFX_TABLEGROUPECOMPLEXE_H__5416A9CD_6348_478C_BCCD_D7CEDBE96ACE__INCLUDED_)
