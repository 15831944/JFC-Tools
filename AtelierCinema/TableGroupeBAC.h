// TableGroupeBAC.h: interface for the CTableGroupeBAC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEGROUPEBAC_H__5416A9CD_6348_478C_BCCD_D7CEDBE96ACE__INCLUDED_)
#define AFX_TABLEGROUPEBAC_H__5416A9CD_6348_478C_BCCD_D7CEDBE96ACE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFichierCinema.h"
#include "Table.h"


////////////////////////////////////////////////////////// TABLE DES GROUPES DE BACS ///////////////////////////////////////////////////
//
// On définit une structure contenant les paramètres du groupe BAC : code et libellé
class tagBACGroupe : public CtagTable
{
	public:
		// libellé du groupe de BACs
		CString m_LibelleGroupe;

		// Code du groupe (pas utile)
		CString m_CodeGroupe;

		// Nombre de bacs composant le groupe (= n° du groupe)
		int m_NbBacs;

		// Codedes bacs qui composent ce groupe
		CArray<long,long> m_CompoBacs; 
		
		// Affectation
		tagBACGroupe & operator=(const  tagBACGroupe & Data)
		{
			m_LibelleGroupe = Data.m_LibelleGroupe;
			m_CodeGroupe	= Data.m_CodeGroupe;
			m_NbBacs		= Data.m_NbBacs;
			m_CompoBacs.Copy(Data.m_CompoBacs);
			return *this;
		};
	
		// Neutral constructor
		tagBACGroupe()
		{
			m_LibelleGroupe = "";
			m_NbBacs		= 0;
			m_CompoBacs.RemoveAll();
		};

		// Copy constructor
		tagBACGroupe(const  tagBACGroupe & Data)
		{
			*this=Data;
		};

		// Particularisation des fonctions de CtagTable
		LPCSTR GetLibelle(){return(m_LibelleGroupe);};
		LPCSTR GetCode(){return("");};

		int GetNbBacs(){return(m_NbBacs);};

		void GetCompoBacs(CArray<long, long> &CompoBacs)
		{
			CompoBacs.RemoveAll();
			for (int idx=0; idx < m_CompoBacs.GetSize(); idx++)
			{
				CompoBacs.Add(m_CompoBacs.GetAt(idx));
			}
		};
		
		/*
		void GetCompoBacs(CArray<CString, int> &CompoBacs)
		{
			CompoBacs.RemoveAll();
			for (int idx=0; idx < m_CompoBacs.GetSize(); idx++)
			{
				CompoBacs.Add(m_CompoBacs.GetAt(idx));
			}
		};
		*/
};

// Tableau de groupe de BACs
typedef CTable<tagBACGroupe,tagBACGroupe&> CTableBACGroupeMere;


class CTableGroupeBAC  : public CTableBACGroupeMere
{
	public:

		int index;

		// Récup bac d'après le code bac
		tagBACGroupe GetBac(CString codebac);

		// opérateur =
		CTableGroupeBAC & operator=(const  CTableGroupeBAC & Data)
		{
			Copy(Data);
			IsModified=Data.IsModified;
			m_NomFich=Data.m_NomFich;
			return *this;
		};

		// constructeur et destructeur
		CTableGroupeBAC();
		CTableGroupeBAC(const  CTableGroupeBAC & Data);
		virtual ~CTableGroupeBAC();

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
		int IsModified;

};

#endif // !defined(AFX_TABLEGROUPEBAC_H__5416A9CD_6348_478C_BCCD_D7CEDBE96ACE__INCLUDED_)
