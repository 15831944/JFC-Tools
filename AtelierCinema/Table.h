// Table.h: interface for the CTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLE_H__42AF0FC2_AA06_4538_8C61_35904D6E90C4__INCLUDED_)
#define AFX_TABLE_H__42AF0FC2_AA06_4538_8C61_35904D6E90C4__INCLUDED_

#include "CFichierCinema.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CtagTable
{
	
	virtual LPCSTR GetLibelle(){return(0);};
	virtual LPCSTR GetCode(){return(0);};

	virtual int GetNbBacs(){return(0);};

	virtual void GetCompoBacs(CArray<long, long> &CompoBacs){};

	virtual int GetNbComplexes(){return(0);};

	virtual void GetCompoComplexes(CArray<int,int> &CompoComplexes){};


};

template<class TYPE, class ARG_TYPE>
class CTable  
{
public:
	int GetSize(){return(m_Table.GetSize());};
	void RemoveAll(){m_Table.RemoveAll();};
	void Copy(const CTable &src){
		m_NomTable=src.m_NomTable; 
		m_Table.Copy(src.m_Table);
		m_MapCodeToIndex.RemoveAll();
		POSITION pos;
		if((pos=src.m_MapCodeToIndex.GetStartPosition())!=NULL)
		{	
			CString rKey;
			int rValue;
			while(pos!=NULL)
			{
				// On ajoute la clé associée à la valeur dans le nouveau map
				src.m_MapCodeToIndex.GetNextAssoc(pos,rKey,rValue); 
				m_MapCodeToIndex.SetAt(rKey,rValue);
			}
		}
	};
	// définition du nom de la table pour la particularisation du message de FindIndexFromCode
	void SetName(CString name)
	{
		m_NomTable=name;
	};
	CString GetName()
	{
		return m_NomTable;
	}
		
	

	virtual LPCSTR GetCode(int position){return(m_Table.GetAt(position).GetCode());};
	virtual bool LoadTable(){return(0);};
	virtual LPCSTR GetLibelle(int position){return(m_Table.GetAt(position).GetLibelle());};

	// Pour les Groupe de Bacs
	// virtual LPCSTR GetToto(int position){return(m_Table.GetAt(position).GetToto());};
	int GetNbBacs(int position){return(m_Table.GetAt(position).GetNbBacs());}; 

	void GetCompoBacs(int position, CArray<long, long> &CompoBacs)
	{
		m_Table.GetAt(position).GetCompoBacs(CompoBacs);
	}

	/*
	void GetCompoBacs(int position, CArray<TCHAR, TCHAR> &CompoBacs)
	{
		m_Table.GetAt(position).GetCompoBacs(CompoBacs);
	}
	*/

	// Pour les groupes de Complexes
	int GetNbComplexes(int position){return(m_Table.GetAt(position).GetNbComplexes());}; 

	void GetCompoComplexes(int position, CArray<int,int> &CompoComplexes)
	{
		m_Table.GetAt(position).GetCompoComplexes(CompoComplexes);
	}


	TYPE GetAt(int position){return(m_Table.GetAt(position));};

	ARG_TYPE operator [] (int index)
	{
		return m_Table[index];
	};

	void SetAt(int index,ARG_TYPE Element){m_Table.SetAt(index,Element);};
	void RemoveAt( int nIndex, int nCount = 1 ){m_Table.RemoveAt(nIndex,nCount);};
	int Add(ARG_TYPE Element){return(m_Table.Add(Element));};
	int Append(	const CArray<TYPE,ARG_TYPE> &tab){return m_Table.Append(tab);};


	// retourne -1 si le code n'est pas trouvé
	int FindIndexFromCode(LPCSTR code,bool flagMessageErreur=1)
	{
		// recherche de l'index correspondant au code
		int index;
		if(!m_MapCodeToIndex.Lookup(code,index))
		{
			if(flagMessageErreur)
			{
				CString txt;
				if(m_NomTable=="produit.table")
				{
					txt.Format("Le code %s n'a pas été trouvé dans la table des produits",code);
				}
				else if(m_NomTable=="bac.table")
				{
					txt.Format("Le code %s n'a pas été trouvé dans la table des bacs",code);
				}
				else if(m_NomTable=="Complexe.table")
				{
					txt.Format("Le code %s n'a pas été trouvé dans la table des complexes",code);
				}
				else if(m_NomTable=="EntreeComplexe.table")
				{
					txt.Format("Le code %s n'a pas été trouvé dans la table des entrées",code);
				}
				else
				{
					txt.Format("Le code %s n'a pas été trouvé dans une table",code);
				}
				
				AfxMessageBox(txt);
			}
			//ASSERT(0);

			return(-1);	// Pas trouvé ?
		}
		
		return(index);
	};
	CTable & operator = (const  CTable & Data)
	{
		m_Table.Copy(Data.m_Table);
		m_NomTable=Data.m_NomTable; 
		return *this;
	};

	CTable(){};
	virtual ~CTable(){};
private:
	CString m_NomTable;
	CArray<TYPE,ARG_TYPE> m_Table;
protected:
	CMap<CString,LPCSTR,int,int&> m_MapCodeToIndex;

};

#endif // !defined(AFX_TABLE_H__42AF0FC2_AA06_4538_8C61_35904D6E90C4__INCLUDED_)

/*

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CArray : public CObject
{
public:
// Construction
	CArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();

	// Potentially growing the array
	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const CArray& src);
	void Copy(const CArray& src);

	// overloaded operator helpers
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CArray* pNewArray);

// Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CArray();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
AFX_INLINE int CArray<TYPE, ARG_TYPE>::GetSize() const
	{ return m_nSize; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE int CArray<TYPE, ARG_TYPE>::GetUpperBound() const
	{ return m_nSize-1; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE void CArray<TYPE, ARG_TYPE>::RemoveAll()
	{ SetSize(0, -1); }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE CArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE void CArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE const TYPE* CArray<TYPE, ARG_TYPE>::GetData() const
	{ return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE* CArray<TYPE, ARG_TYPE>::GetData()
	{ return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE CArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
AFX_INLINE TYPE& CArray<TYPE, ARG_TYPE>::operator[](int nIndex)
	{ return ElementAt(nIndex); }

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

*/