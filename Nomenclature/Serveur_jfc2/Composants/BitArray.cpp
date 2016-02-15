#include "stdafx.h"
#include "BitArray.h"

#ifdef _DEBUG

#define ASSERT_RETURN(x)            if(!(x))           \
                                    {                  \
                                        ASSERT(FALSE); \
                                        return;        \
                                    }

#define ASSERT_RETURN_VALUE(x,v)    if(!(x))           \
                                    {                  \
                                        ASSERT(FALSE); \
                                        return (v);    \
                                    }
#else

#define ASSERT_RETURN(x)
#define ASSERT_RETURN_VALUE(x,v)

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Some helper macros.
#define BIT_TO_HOSTVAR_SIZE(nSize) (((nSize)/(8*sizeof(HOSTVAR))) \
                           + (((nSize)%(8*sizeof(HOSTVAR)))?1:0))
#define BIT_TO_BYTE_SIZE(nSize) (((nSize)/8) + (((nSize)%8)?1:0))
#define HOSTVAR_TO_BYTE_SIZE(nSize) ((nSize) * sizeof(HOSTVAR))
#define HOSTVAR_TO_BIT_SIZE(nSize)  ((nSize) * sizeof(HOSTVAR) * 8)
#define BIT_TO_BYTE_INDEX(nIndex)   ((nIndex)/8)
#define BIT_OFFSET_IN_BYTE(nIndex)  ((nIndex)%8)

IMPLEMENT_DYNAMIC(CBitArray, CObject)

// The BYTE Mask Array.
static const BYTE BYTE_MASK[] = {   0x80,
                                    0x40,
                                    0x20,
                                    0x10,
                                    0x08,
                                    0x04,
                                    0x02,
                                    0x01
                                };

CBitArray::CBitArray()
{
    m_pData = NULL;
    m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CBitArray::~CBitArray()
{
    ASSERT_VALID(this);

    delete[] m_pData;
}

void CBitArray::SetSize(int nNewBitSize, int nGrowBy)
{
    ASSERT_VALID(this);
    ASSERT(nNewBitSize >= 0);

    int	nHostVarNewSize = BIT_TO_HOSTVAR_SIZE(nNewBitSize);

    if (nGrowBy != -1)
    {
        // Make Grow by as a multiple of hostvar size.
        if ( nGrowBy%HOSTVAR_TO_BIT_SIZE(1) )
        {
            nGrowBy = BIT_TO_HOSTVAR_SIZE(nGrowBy);
            nGrowBy = HOSTVAR_TO_BIT_SIZE(nGrowBy);
        }

        m_nGrowBy = nGrowBy;
    }

    ASSERT(m_nGrowBy == 0 || !(m_nGrowBy%HOSTVAR_TO_BIT_SIZE(1)));

    if (nNewBitSize == 0)
    {
        delete[] m_pData;
        m_pData = NULL;
        m_nSize = m_nMaxSize = 0;
    }
    else if (m_pData == NULL)
    {
        m_pData = new HOSTVAR[nHostVarNewSize];

        int nByteSize = HOSTVAR_TO_BYTE_SIZE(nHostVarNewSize);
        memset(m_pData, 0, nByteSize);

        m_nSize = nNewBitSize;
        m_nMaxSize = HOSTVAR_TO_BIT_SIZE(nHostVarNewSize);
    }
    else if (nNewBitSize <= m_nMaxSize)
    {
        int nOldSize = m_nSize;
        m_nSize = nNewBitSize;

        if (nNewBitSize > nOldSize)
        {
            // initialize the new elements
            SetRange(nOldSize, m_nSize-1, FALSE);
        }
    }
    else
    {
        // otherwise, grow array
        int nHostVarGrowBy = BIT_TO_HOSTVAR_SIZE(m_nGrowBy);
        if (nHostVarGrowBy == 0)
        {
            // heuristically determine growth when nGrowBy == 0
            // (this avoids heap fragmentation in many situations)
            // Code taken from CByteArray...
            int nCurrHostVarSize = BIT_TO_HOSTVAR_SIZE(m_nSize);
            nHostVarGrowBy=min(1024, max(4, nCurrHostVarSize/8));
            ASSERT(nHostVarGrowBy >= 4);
        }
        ASSERT(nHostVarGrowBy > 0);
        int nGrowByBits = HOSTVAR_TO_BIT_SIZE(nHostVarGrowBy);

        int nNewMaxSize = 0;
        if (nNewBitSize < m_nMaxSize + nGrowByBits)
        {
            nNewMaxSize = m_nMaxSize + nGrowByBits;
        }
        else
        {
            nNewMaxSize = nNewBitSize;
        }

        ASSERT(nNewMaxSize >= m_nMaxSize);

        int nNewHostVarSize = BIT_TO_HOSTVAR_SIZE(nNewMaxSize);

        HOSTVAR* pNewData = new HOSTVAR[nNewHostVarSize];

        // copy new data from old
        memcpy(pNewData, m_pData, BIT_TO_BYTE_SIZE(m_nSize));

        // delete old array
        delete[] m_pData;
        m_pData = pNewData;

        // set the member variables
        ASSERT(nNewMaxSize > m_nSize);
        int nOldSize = m_nSize;
        m_nSize = nNewBitSize;
        m_nMaxSize = HOSTVAR_TO_BIT_SIZE(nNewHostVarSize);

        // init remaining elements
        SetRange(nOldSize, m_nSize-1, FALSE);
    }
}

int CBitArray::Add(bool bValue)
{
    ASSERT_VALID(this);

    SetSize(m_nSize+1);
    
    // SetSize will change m_nSize. So index is m_nSize-1
    SetAt(m_nSize-1, bValue);

    // return the current index.
    return m_nSize-1;
}

int CBitArray::Append(const CBitArray& src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself

    int nSrcByteCount = BIT_TO_BYTE_SIZE(src.m_nSize);
    if (!nSrcByteCount)
    {
        // Nothing to copy
        return 0;
    }

    int nOldSize = m_nSize;
    SetSize(m_nSize + src.m_nSize);

    int nDestOffset = BIT_OFFSET_IN_BYTE(nOldSize);

    if (!nDestOffset)
    {
        // Cool. It's a byte boundary. Got lucky.
        memcpy((BYTE*)m_pData + BIT_TO_BYTE_SIZE(nOldSize),
                src.m_pData, BIT_TO_BYTE_SIZE(src.m_nSize));
    }
    else
    {
        int nDestByteIndex	= BIT_TO_BYTE_INDEX(nOldSize);
        int nSrcOffset      = BIT_OFFSET_IN_BYTE(src.m_nSize);
        BYTE* pSrcData      = (BYTE*)src.m_pData;
        BYTE* pDestData     = (BYTE*)m_pData + nDestByteIndex;
        BYTE  byWork = 0;

        while (nSrcByteCount--)
        {
            // Assert that the bits we are going to modify
            // are initialized by SetSize to zero and that
            // we are not corrupting memory.
            ASSERT((BYTE)(*pDestData << nDestOffset) == 0);
            *pDestData |= *pSrcData >> nDestOffset;
            pDestData++;
            
            if ( nSrcByteCount ||
                 !nSrcOffset || nDestOffset+nSrcOffset > 8 )
            {
                // Similar check.
                ASSERT((BYTE)(*pDestData >> (8-nDestOffset)) == 0);
                *pDestData = *pSrcData << (8-nDestOffset);
                pSrcData++;
            }
        }
    }
    // Return the first appended element.
    return nOldSize;
}
/*
void CBitArray::Copy(const CBitArray& src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself

    SetSize(src.m_nSize);

    memcpy(m_pData, src.m_pData, BIT_TO_BYTE_SIZE(src.m_nSize));
}
*/
void CBitArray::InsertAt(int nIndex, bool bNewElement, int nCount)
{
    ASSERT_VALID(this);
    ASSERT(nIndex >= 0);
    ASSERT(nCount > 0);

    if (nIndex >= m_nSize)
    {
        // adding after the end of the array
        SetSize(nIndex + nCount);
    }
    else
    {
        // inserting in the middle of the array
        int nOldSize = m_nSize;
        SetSize(m_nSize + nCount);

        // For now we'll do GetAt and SetAt. Can be optimized.
        for (int nCopyIndex=nOldSize-1;nCopyIndex>=nIndex; nCopyIndex--)
        {
            SetAt(nCopyIndex+nCount, GetAt(nCopyIndex));
        }
    }

    // insert new value in the gap
    ASSERT(nIndex + nCount <= m_nSize);
    SetRange(nIndex, nIndex + nCount - 1, bNewElement);
}

void CBitArray::InsertAt(int nStartIndex, CBitArray* pNewArray)
{
    ASSERT_VALID(this);
    ASSERT(pNewArray != NULL);
    ASSERT_KINDOF(CBitArray, pNewArray);
    ASSERT_VALID(pNewArray);
    ASSERT(nStartIndex >= 0);

    if (pNewArray->GetSize() > 0)
    {
        // Call the other version of InsertAt.
        // This will expand the array and will also 
        // set the 0th element
        InsertAt(nStartIndex, pNewArray->GetAt(0),
                              pNewArray->GetSize());              
        
        // Copy the rest of the array bit-by-bit
        for (int nIndex = 1; nIndex < pNewArray->GetSize();
                             nIndex++)
        {
            SetAt(nStartIndex + nIndex, pNewArray->GetAt(nIndex));
        }
    }
}

void CBitArray::RemoveAt(int nIndex, int nCount)
{
    ASSERT_VALID(this);
    ASSERT(nIndex >= 0);
    ASSERT(nCount >= 0);
    ASSERT(nIndex + nCount <= m_nSize);

    for (int nCopyIndex=nIndex+nCount;nCopyIndex<GetSize(); nCopyIndex++)
    {
        SetAt(nCopyIndex-nCount, GetAt(nCopyIndex));
    }
    m_nSize -= nCount;
}

void CBitArray::FreeExtra()
{
    ASSERT_VALID(this);

    int nNewHostVarSize = BIT_TO_HOSTVAR_SIZE(m_nSize);
    if (nNewHostVarSize != (int)BIT_TO_HOSTVAR_SIZE(m_nMaxSize))
    {
        // shrink to desired size
        HOSTVAR* pNewData = NULL;
        if (nNewHostVarSize != 0)
        {
            pNewData = new HOSTVAR[nNewHostVarSize];

            // copy new array from old
            memcpy(pNewData, m_pData,
                   HOSTVAR_TO_BYTE_SIZE(nNewHostVarSize));
        }

        // delete old array
        delete[] m_pData;
        m_pData = pNewData;
        m_nMaxSize = HOSTVAR_TO_BIT_SIZE(nNewHostVarSize);
    }
}

bool CBitArray::GetAt(int nBitIndex) const
{
    ASSERT_VALID(this);
    ASSERT_RETURN_VALUE(nBitIndex >= 0 && nBitIndex < m_nSize, FALSE);

    int nByteIndex      = BIT_TO_BYTE_INDEX(nBitIndex);
    BYTE* pData         = (BYTE*)m_pData + nByteIndex;
    int	nOffsetInByte	= BIT_OFFSET_IN_BYTE(nBitIndex);
    BYTE byMask         = BYTE_MASK[nOffsetInByte];

    return (*pData & byMask) ? TRUE : FALSE;
}

void CBitArray::SetAt(int nBitIndex, bool bValue)
{
    ASSERT_VALID(this);
    ASSERT_RETURN(nBitIndex >= 0 && nBitIndex < m_nSize);

    int nByteIndex      = BIT_TO_BYTE_INDEX(nBitIndex);
    BYTE* pData         = (BYTE*)m_pData + nByteIndex;

    int	nOffsetInByte	= BIT_OFFSET_IN_BYTE(nBitIndex);
    BYTE byMask         = BYTE_MASK[nOffsetInByte];

    if (bValue)
    {
        *pData |= byMask;
    }
    else
    {
        byMask = ~byMask;
        *pData &= byMask;
    }
}

void CBitArray::SetRange(int nStartBitIndex,
                         int nEndBitIndex, bool bValue)
{
    ASSERT_VALID(this);
    ASSERT_RETURN(nEndBitIndex >= 0 && nEndBitIndex < m_nSize);
    ASSERT_RETURN(nStartBitIndex >= 0 && nStartBitIndex <= nEndBitIndex);

    int nStartByteIndex     = BIT_TO_BYTE_INDEX(nStartBitIndex);
    int nEndByteIndex       = BIT_TO_BYTE_INDEX(nEndBitIndex);
    int nStartOffsetInByte  = BIT_OFFSET_IN_BYTE(nStartBitIndex);
    int nEndOffsetInByte    = BIT_OFFSET_IN_BYTE(nEndBitIndex);
    BYTE byMask             = 0;
    BYTE* pData             = (BYTE*)m_pData + nStartByteIndex;

    if (nStartByteIndex == nEndByteIndex)
    {
        byMask = GetByteMaskRange(nStartOffsetInByte,
                                  nEndOffsetInByte);

        if (bValue)
        {
            *pData |= byMask;
        }
        else
        {
            byMask = ~byMask;
            *pData &= byMask;
        }
        return;
    }

    ASSERT (nStartByteIndex < nEndByteIndex);

    // Start Byte
    byMask = GetByteMaskRange(nStartOffsetInByte);

    if (bValue)
    {
        *pData |= byMask;
    }
    else
    {
        byMask = ~byMask;
        *pData &= byMask;
    }

    // Bytes inbetween.
    int nBytesInBetween = nEndByteIndex - nStartByteIndex - 1;
    if (nBytesInBetween)
    {
        byMask = (bValue) ? 0xFF : 0;
        pData++;
        memset(pData, byMask, nBytesInBetween);
    }

    // End Byte
    byMask = GetByteMaskRange(0, nEndOffsetInByte);

    pData = (BYTE*)m_pData + nEndByteIndex;
    if (bValue)
    {
        *pData |= byMask;
    }
    else
    {
        byMask = ~byMask;
        *pData &= byMask;
    }
}

BYTE CBitArray::GetByteMaskRange(int nStartBit, int nEndBit) const
{
    ASSERT_VALID(this);
    ASSERT_RETURN_VALUE(nStartBit>=0 && nStartBit<=7, 0);
    ASSERT_RETURN_VALUE(nEndBit>=0 && nEndBit<=7, 0);
    ASSERT_RETURN_VALUE(nStartBit <= nEndBit, 0);

    BYTE    byMask = 0;

    for (int nIndex=nStartBit; nIndex <= nEndBit; nIndex++)
    {
        byMask = byMask | BYTE_MASK[nIndex];
    }
    return byMask;
}

void CBitArray::SetAtGrow(int nBitIndex, bool bValue)
{
    ASSERT_VALID(this);
    ASSERT(nBitIndex >= 0);

    if (nBitIndex >= m_nSize)
    {
        SetSize(nBitIndex+1);
        ASSERT(nBitIndex+1 == m_nSize);
    }
    SetAt(nBitIndex, bValue);
}

void CBitArray::Serialize(CArchive& ar)
{
    ASSERT_VALID(this);
    CObject::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << (DWORD)m_nSize;
        ar << (DWORD)m_nGrowBy;

        ar.Write((LPVOID)m_pData,
                 (UINT)BIT_TO_BYTE_SIZE(m_nSize));
    }
    else
    {
        int	nSize = 0;
        int	nGrowBy = 0;

        ar >> (DWORD&)nSize;
        ar >> (DWORD&)nGrowBy;

        SetSize(nSize, nGrowBy);

        UINT uRet = ar.Read((LPVOID)m_pData,
                            (UINT)BIT_TO_BYTE_SIZE(m_nSize));
        ASSERT(uRet == (UINT)BIT_TO_BYTE_SIZE(m_nSize));
    }
}

CBitArray & CBitArray::operator &=(const CBitArray &src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself
    ASSERT(m_nSize == src.m_nSize);   // must be the same size

	for(int x=0;x<BIT_TO_BYTE_SIZE(src.m_nSize);x++)
	{
		m_pData[x]&=src.m_pData[x];
	}
	return(*this);
}

CBitArray & CBitArray::operator |=(const CBitArray &src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself
    ASSERT(m_nSize == src.m_nSize);   // must be the same size

	for(int x=0;x<BIT_TO_BYTE_SIZE(src.m_nSize);x++)
	{
		m_pData[x]|=src.m_pData[x];
	}
	return(*this);
}

CBitArray & CBitArray::operator ^=(const CBitArray &src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself
    ASSERT(m_nSize == src.m_nSize);   // must be the same size

	for(int x=0;x<BIT_TO_BYTE_SIZE(src.m_nSize);x++)
	{
		m_pData[x]^=src.m_pData[x];
	}
	return(*this);
}

CBitArray::CBitArray(const CBitArray &src)
{
    m_pData = NULL;
    m_nSize = m_nMaxSize = m_nGrowBy = 0;
	*this=src;
}

CBitArray & CBitArray::operator =(const CBitArray &src)
{
    ASSERT(this != &src);   // cannot append to itself

    SetSize(src.m_nSize);
    memcpy(m_pData, src.m_pData, BIT_TO_BYTE_SIZE(src.m_nSize));
	return *this;
}

CBitArray CBitArray::operator ~()
{
    ASSERT_VALID(this);
	CBitArray Temp(*this);
	for(int x=0;x<BIT_TO_BYTE_SIZE(m_nSize);x++)
	{
		Temp.m_pData[x]=(~m_pData[x]);
	}
	return Temp;
}

CBitArray CBitArray::operator &(const CBitArray &src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself

	CBitArray Temp(*this);
	Temp&=src;
	return Temp;
}

CBitArray CBitArray::operator |(const CBitArray &src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself

	CBitArray Temp(*this);
	Temp|=src;
	return Temp;
}

CBitArray CBitArray::operator ^(const CBitArray &src)
{
    ASSERT_VALID(this);
    ASSERT(this != &src);   // cannot append to itself

	CBitArray Temp(*this);
	Temp^=src;
	return Temp;
}

void CBitArray::SetAll(bool bValue)
{
    ASSERT_VALID(this);
	BYTE*	pData = (BYTE*)m_pData;
	BYTE	byMask = (bValue) ? 0xFF : 0x00;

	memset(pData, byMask,BIT_TO_BYTE_SIZE(m_nSize));
}

////////////////////////////////////////////////////////////////
// Diagnostics
#ifdef _DEBUG
void CBitArray::Dump(CDumpContext& dc) const
{
    CObject::Dump(dc);

    dc << "with " << m_nSize << " elements";
    if (dc.GetDepth() > 0)
    {
        for (int nIndex = 0; nIndex < m_nSize; nIndex++)
        {
            dc << "\n\t[" << nIndex << "]\t= " << GetAt(nIndex);
        }
    }

    dc << "\n";
}
#endif

#ifdef _DEBUG
void CBitArray::AssertValid() const
{
    CObject::AssertValid();

    if (m_pData == NULL)
    {
        ASSERT(m_nSize == 0);
        ASSERT(m_nMaxSize == 0);
    }
    else
    {
        ASSERT(m_nSize >= 0);
        ASSERT(m_nMaxSize >= 0);
        ASSERT(m_nSize <= m_nMaxSize);
        ASSERT(AfxIsValidAddress(m_pData,BIT_TO_BYTE_SIZE(m_nMaxSize)));
    }
}
#endif

#ifdef _DEBUG
int CBitArray::GetByteIndex(int nBitIndex) const
{
    ASSERT_VALID(this);
    ASSERT_RETURN_VALUE(nBitIndex >= 0 && nBitIndex < m_nSize, FALSE);
    int nByteIndex	= BIT_TO_BYTE_INDEX(nBitIndex);
    return nByteIndex;
}
#endif

#ifdef _DEBUG
BYTE CBitArray::GetByteValue(int nByteIndex) const
{
    ASSERT_VALID(this);
    ASSERT_RETURN_VALUE(nByteIndex >= 0, 0);

    BYTE *pData	= (BYTE*)m_pData + nByteIndex;

    return *pData;
}
#endif


