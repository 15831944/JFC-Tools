// Selection.h: interface for the CSelection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTION_H__6B345225_44E0_42DD_87D5_9299654B7100__INCLUDED_)
#define AFX_SELECTION_H__6B345225_44E0_42DD_87D5_9299654B7100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class tagSelection
{
public:
	void SetComplexeUsed(int idxComplexe)
	{
		m_MapComplexeLigne.SetAt(idxComplexe,m_MapComplexeLigne.GetCount());
		m_ComplexeLigne.Add(idxComplexe);
	};
	bool IsComplexeActif(int idxComplexe)
	{
		// On cherche l'index complexe s'il existe on retourne -1.
		WORD index;
		if(m_MapComplexeLigne.Lookup(idxComplexe,index))return (1);
		else return (0);
	};
	// Nb de complexe 
	int GetNbComplexe(){return m_ComplexeLigne.GetSize();};
	// Index complexe
	int GetIndexComplexe(int index)
	{
		return m_ComplexeLigne[index];
	};
	// code du produit,Bac,complexe
	CString m_Code;

	int m_Index;

	// type de la sélection: produit 0, bac 1, complexe 2
	// énumération pour le type de sélection
	enum TYPECODE {PRODUIT , BAC , COMPLEXE , PRODUITFIGE };
	TYPECODE m_Type;
	// CALCUL FRED
	// liste des complexes d'une ligne: tableau indexé sur les complexes

	tagSelection & operator = (const tagSelection & Data)
	{
		m_Code = Data.m_Code;
		m_Index = Data.m_Index;
		m_Type = Data.m_Type;	
		m_ComplexeLigne.Copy(Data.m_ComplexeLigne); 

		m_MapComplexeLigne.RemoveAll();
		POSITION pos;
		if((pos=Data.m_MapComplexeLigne.GetStartPosition())!=NULL)
		{	
			WORD rKey;
			WORD rValue;
			while(pos!=NULL)
			{
				// On ajoute la clé associée à la valeur dans le nouveau map
				Data.m_MapComplexeLigne.GetNextAssoc(pos,rKey,rValue); 
				m_MapComplexeLigne.SetAt(rKey,rValue);
			}
		}

		return *this;
	}

	// Neutral constructor
	tagSelection()
	{
		m_Index=0;

	};

	// Copy constructor
	tagSelection(const  tagSelection & Data)
	{
		*this=Data;
	};

public:
	CWordArray m_ComplexeLigne;
	CMap <WORD,WORD,WORD,WORD> m_MapComplexeLigne;

};

typedef CArray<tagSelection,tagSelection&> CSelectionArray;
typedef CArray<int,int> CIntArray;

class CSelection  
{
public:
	CIntArray TabPassageValide;

	void GetAllCode(CStringArray & codes);
	void PrepareMasqueNational();
	CByteArray SelectBinome;
	CByteArray m_MasqueRegional;
	bool PrepareMasqueRegional();
	bool ConstruitListeBacComplexeActifs();
	bool BindMap();

	CByteArray m_ListBacActif;
	CByteArray m_ListComplexeActif;
	CWordArray m_TypeParLigne;
	// Accélérateur pour l'intersection des lignes
	int ** m_MapIntersection;

	inline bool IsIntersection(int & lignea, int & ligneb);
	int GetIndex(int position);
	void Serialize(CArchive& ar);
	void RemoveAll(){m_Select.RemoveAll();};
	int AddCode(LPCSTR code);
	LPCSTR GetLibelle(int index);
	int GetType(int position);
	LPCSTR GetCode(int position);
	LPCSTR GetCodeFromParc(int position);
	void FreeMemory();
	void InitTab();
	bool BindAccelerateur();
	int GetSize();
	CSelection();
	virtual ~CSelection();
	int GetNbParc(int position){return(2);};
	// accès aux données membres
	WORD GetNrDirectParc(int ligne,int park)
	{
		ASSERT(ligne<GetSize());
		return(m_NrDirectParc[ligne][park]);
	};
	WORD GetNrDirectBinome(int park){return(m_NrDirectBinome[park]);};
	int GetNbParcTotal(){return(m_NbParcTotal);};

	CSelectionArray m_Select;

private:
	int m_NbParcTotal;
	CWordArray * m_NrDirectParc; // par [binome][parc] donne acces à parc
	CWordArray m_NrDirectBinome; // par [parc]
	// CSelectionArray m_Select;
};

#endif // !defined(AFX_SELECTION_H__6B345225_44E0_42DD_87D5_9299654B7100__INCLUDED_)
