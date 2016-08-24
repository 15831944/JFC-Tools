// TableBAC.h: interface for the CTableBAC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEBAC_H__D5823CFF_D4B2_4819_9F32_09512ACAB4BB__INCLUDED_)
#define AFX_TABLEBAC_H__D5823CFF_D4B2_4819_9F32_09512ACAB4BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CFichierCinema.h"
#include "Table.h"




////////////////////////////////////////////////////////// TABLE DES BACS ///////////////////////////////////////////////////
//
// On d�finit une structure contenant les param�tres du BAC : code et libell�
class tagBAC : public CtagTable
{
public:
	CWordArray m_CompoComplexe; // Index dex complexes qui composent ce BAC
	// code de base du BAC (probabilis�)
	CString m_Code;
	// code du BAC Regie (CODEBAC[B75999]+[A](R�gie A) ou [M](M�diavision) ou [P] pour le bac probabilis�)
	CString m_CodeComposite;
	// libell� du BAC
	CString m_Libelle;
	// Nombre d'entr�es du BAC
	double m_NbEntree;
	// Quota (ou part de voix: 1 pour un bac probabilis�).
	double m_Quota;

	tagBAC & operator=(const  tagBAC & Data)
	{
		m_Libelle=Data.m_Libelle;
		m_Code=Data.m_Code;
		m_CodeComposite=Data.m_CodeComposite; 
		m_CompoComplexe.Copy(Data.m_CompoComplexe);
		m_NbEntree=Data.m_NbEntree;
		m_Quota=Data.m_Quota;
		return *this;
	};
	
	// Neutral constructor
	tagBAC()
	{
		// m_Libelle="";
		// m_Code="";
		// m_CompoComplexe.RemoveAll();
		m_NbEntree=+0.0;
		m_Quota=1;
	};

	// Copy constructor
	tagBAC(const  tagBAC & Data)
	{
		*this=Data;
	};

	// Particularisation des fonctions de CtagTable
	LPCSTR GetLibelle(){return(m_Libelle);};
	LPCSTR GetCode(){return(m_Code);};
    LPCSTR GetCodeComposite(){return(m_CodeComposite);};


	
};



// Tableau de BAC
typedef CTable<tagBAC,tagBAC&> CTableBACMere;

class CTableBAC : public CTableBACMere
{
public:
	void Tri();
	bool SaveTable();

	// r�cup�ration du code composite
	CString GetCodeComposite(int index){return GetAt(index).m_CodeComposite;};
	CString GetCodeUtilePourCalcul(int index)
	{
		/*
		C'est le code composite (B75999A par exemple) pour les BAC Regie
		sauf pour un bac probabilis� ou c'est directement le BAC (le composite est avec un P en plus du code 6 caract�res)
		*/

		/* Correction ALAIN Noel 2006
		   Sinon lecture incorrecte des codes bacs, toujours fig�e sur 1er code bac trouv�
		*/
		// static CString txt=GetAt(index).m_CodeComposite;

		CString txt=GetAt(index).m_CodeComposite;
		if (txt.GetLength() > 6)
		{			
			if(txt[6]=='P')
				txt=txt.Left(6);
		}	
		return txt;
	};

	// r�cup�ration de l'index � partir du code composite
	//int GetIndexFromCodeComposite(CString codecomposite);

	// d�finition de m_MapCodeToIndex
	void MapCodeToIndex(CString code,int index){m_MapCodeToIndex.SetAt(code,index);};

	// Suppression des �l�ments
	void RemoveMap(){m_MapCodeToIndex.RemoveAll();};

	// cr�ation des bacs r�gie
	void CreateBacRegie();

	// R�cup bac d'apr�s le code bac
	tagBAC GetBac(CString codebac);

	// op�rateur =
	CTableBAC & operator=(const  CTableBAC & Data)
	{
		Copy(Data);
		IsModified=Data.IsModified;
		m_NomFich=Data.m_NomFich;
		return *this;
	};

	// constructeur et destructeur
	CTableBAC();
	CTableBAC(const  CTableBAC & Data);
	virtual ~CTableBAC();

	// Initialisation
	bool LoadTable();

	void SetModified(int modif)
	{
		IsModified = modif;
	};
	int IsModif()
	{
		return IsModified; 
	};

private:
	void Decale(int idbac);
	CString m_NomFich;
	int IsModified;

};

#endif // !defined(AFX_TABLEBAC_H__D5823CFF_D4B2_4819_9F32_09512ACAB4BB__INCLUDED_)
