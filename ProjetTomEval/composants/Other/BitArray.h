#ifndef _BITARRAY_H_
#define _BITARRAY_H_

// The bits of the bitarray is stored in HOSTVAR.
// The HOSTVAR can be BYTE or short etc.
// This can be changed to maximize speed.
#define HOSTVAR	BYTE

class CBitArray : public CObject
{
    DECLARE_DYNAMIC(CBitArray)
public:
    CBitArray();
    ~CBitArray();
// Modified by eddie 12/4/99
	CBitArray(const CBitArray &src);
	CBitArray & operator=(const CBitArray& src);
	CBitArray & operator&=(const CBitArray& src);
	CBitArray & operator|=(const CBitArray& src);
	CBitArray & operator^=(const CBitArray& src);
	CBitArray operator~();
	CBitArray operator&(const CBitArray& src);
	CBitArray operator|(const CBitArray& src);
	CBitArray operator^(const CBitArray& src);

// Attributes
    int GetSize() const
    { return m_nSize; }
    int GetUpperBound() const
    { return m_nSize-1; }
    void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
    // Clean up
    void FreeExtra();
    void RemoveAll()
    { SetSize(0); }

    // Accessing elements
    bool GetAt(int nBitIndex) const;
    void SetAt(int nBitIndex, bool bValue);
    void SetAtGrow(int nBitIndex, bool bValue);
	void SetAll(bool bValue);
    void SetRange(int nStartBitIndex,
                    int nEndBitIndex, bool bValue);

    void Serialize(CArchive&);

    // Potentially growing the array
    void SetAtGrow(int nIndex, BYTE newElement);
    int Add(bool bValue);
    int Append(const CBitArray& src);
 //   void Copy(const CBitArray& src);

    // Operations that move elements around
    void InsertAt(int nIndex, bool bNewElement, int nCount = 1);
    void RemoveAt(int nIndex, int nCount = 1);
    void InsertAt(int nStartIndex, CBitArray* pNewArray);

// Implementation
protected:
    HOSTVAR*   m_pData;
    int        m_nSize;
    int        m_nMaxSize;
    int        m_nGrowBy;

    BYTE  GetByteMaskRange(int nStartBit, int nEndBit=7) const;

#ifdef _DEBUG
public:
    int     GetByteIndex(int nBitIndex) const;
    BYTE    GetByteValue(int nByteIndex) const;
    int     GetMaxSize() const	{return m_nMaxSize;}
    int     GetGrowBy() const	{return m_nGrowBy;}
    void    Dump(CDumpContext&) const;
    void    AssertValid() const;
#endif
};

#endif
