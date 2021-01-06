#include "stdafx.h"
#include "BitArray.h"
#include <memory>
#include <cassert>

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

// The BYTE Mask Array.
/*
static const JUnt08 BYTE_MASK[] = {   0x80,
                                    0x40,
                                    0x20,
                                    0x10,
                                    0x08,
                                    0x04,
                                    0x02,
                                    0x01
                                };
*/
static const JUnt08 BYTE_MASK[] = {   0x01,
                                    0x02,
                                    0x04,
                                    0x08,
                                    0x10,
                                    0x20,
                                    0x40,
                                    0x80
                                };

CBitArray::CBitArray()
{
    m_pData = 0;
    m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CBitArray::~CBitArray()
{
    delete[] m_pData;
}

JVoid CBitArray::SetSize(JInt32 nNewBitSize, JInt32 nGrowBy)
{
    assert(nNewBitSize >= 0);

    JInt32	nHostVarNewSize = BIT_TO_HOSTVAR_SIZE(nNewBitSize);

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

    assert(m_nGrowBy == 0 || !(m_nGrowBy%HOSTVAR_TO_BIT_SIZE(1)));

    if (nNewBitSize == 0)
    {
        delete[] m_pData;
        m_pData = 0;
        m_nSize = m_nMaxSize = 0;
    }
    else if (m_pData == 0)
    {
        m_pData = new HOSTVAR[nHostVarNewSize];

        JInt32 nByteSize = HOSTVAR_TO_BYTE_SIZE(nHostVarNewSize);
        memset(m_pData, 0, nByteSize);

        m_nSize = nNewBitSize;
        m_nMaxSize = HOSTVAR_TO_BIT_SIZE(nHostVarNewSize);
    }
    else if (nNewBitSize <= m_nMaxSize)
    {
        JInt32 nOldSize = m_nSize;
        m_nSize = nNewBitSize;

        if (nNewBitSize > nOldSize)
        {
            // initialize the new elements
            SetRange(nOldSize, m_nSize-1, false);
        }
    }
    else
    {
        // otherwise, grow array
        JInt32 nHostVarGrowBy = BIT_TO_HOSTVAR_SIZE(m_nGrowBy);
        if (nHostVarGrowBy == 0)
        {
            // heuristically determine growth when nGrowBy == 0
            // (this avoids heap fragmentation in many situations)
            // Code taken from CByteArray...
            JInt32 nCurrHostVarSize = BIT_TO_HOSTVAR_SIZE(m_nSize);
            nHostVarGrowBy=__min(1024, __max(4, nCurrHostVarSize/8));
            assert(nHostVarGrowBy >= 4);
        }
        assert(nHostVarGrowBy > 0);
        JInt32 nGrowByBits = HOSTVAR_TO_BIT_SIZE(nHostVarGrowBy);

        JInt32 nNewMaxSize = 0;
        if (nNewBitSize < m_nMaxSize + nGrowByBits)
        {
            nNewMaxSize = m_nMaxSize + nGrowByBits;
        }
        else
        {
            nNewMaxSize = nNewBitSize;
        }

        assert(nNewMaxSize >= m_nMaxSize);

        JInt32 nNewHostVarSize = BIT_TO_HOSTVAR_SIZE(nNewMaxSize);

        HOSTVAR* pNewData = new HOSTVAR[nNewHostVarSize];

        // copy new data from old
        memcpy(pNewData, m_pData, BIT_TO_BYTE_SIZE(m_nSize));

        // delete old array
        delete[] m_pData;
        m_pData = pNewData;

        // set the member variables
        assert(nNewMaxSize > m_nSize);
        JInt32 nOldSize = m_nSize;
        m_nSize = nNewBitSize;
        m_nMaxSize = HOSTVAR_TO_BIT_SIZE(nNewHostVarSize);

        // init remaining elements
        SetRange(nOldSize, m_nSize-1, false);
    }
}

JInt32 CBitArray::Add(JBool bValue)
{
    SetSize(m_nSize+1);
    
    // SetSize will change m_nSize. So index is m_nSize-1
    SetAt(m_nSize-1, bValue);

    // return the current index.
    return m_nSize-1;
}

JInt32 CBitArray::Append(const CBitArray& src)
{
    assert(this != &src);   // cannot Append to itself

    JInt32 nSrcByteCount = BIT_TO_BYTE_SIZE(src.m_nSize);
    if (!nSrcByteCount)
    {
        // Nothing to copy
        return 0;
    }

    JInt32 nOldSize = m_nSize;
    SetSize(m_nSize + src.m_nSize);

    JInt32 nDestOffset = BIT_OFFSET_IN_BYTE(nOldSize);

    if (!nDestOffset)
    {
        // Cool. It's a byte boundary. Got lucky.
        memcpy((JUnt08 *)m_pData + BIT_TO_BYTE_SIZE(nOldSize),
                src.m_pData, BIT_TO_BYTE_SIZE(src.m_nSize));
    }
    else
    {
        JInt32 nDestByteIndex	= BIT_TO_BYTE_INDEX(nOldSize);
        JInt32 nSrcOffset      = BIT_OFFSET_IN_BYTE(src.m_nSize);
        JUnt08* pSrcData      = (JUnt08*)src.m_pData;
        JUnt08* pDestData     = (JUnt08*)m_pData + nDestByteIndex;
        JUnt08  byWork = 0;

        while (nSrcByteCount--)
        {
            // Assert that the bits we are going to modify
            // are initialized by SetSize to zero and that
            // we are not corrupting memory.
            assert((JUnt08)(*pDestData << nDestOffset) == 0);
            *pDestData |= *pSrcData >> nDestOffset;
            ++pDestData;
            
            if ( nSrcByteCount ||
                 !nSrcOffset || nDestOffset+nSrcOffset > 8 )
            {
                // Similar check.
                assert((JUnt08)(*pDestData >> (8-nDestOffset)) == 0);
                *pDestData = *pSrcData << (8-nDestOffset);
                ++pSrcData;
            }
        }
    }
    // Return the first Appended element.
    return nOldSize;
}

JVoid CBitArray::InsertAt(JInt32 nIndex, JBool bNewElement, JInt32 nCount)
{
    assert(nIndex >= 0);
    assert(nCount > 0);

    if (nIndex >= m_nSize)
    {
        // adding after the end of the array
        SetSize(nIndex + nCount);
    }
    else
    {
        // inserting in the middle of the array
        JInt32 nOldSize = m_nSize;
        SetSize(m_nSize + nCount);

        // For now we'll do GetAt and SetAt. Can be optimized.
        for (JInt32 nCopyIndex=nOldSize-1;nCopyIndex>=nIndex; --nCopyIndex)
        {
            SetAt(nCopyIndex+nCount, GetAt(nCopyIndex));
        }
    }

    // insert new value in the gap
    assert(nIndex + nCount <= m_nSize);
    SetRange(nIndex, nIndex + nCount - 1, bNewElement);
}

JVoid CBitArray::InsertAt(JInt32 nStartIndex, CBitArray* pNewArray)
{
    assert(pNewArray != NULL);
    assert(nStartIndex >= 0);

    if (pNewArray->GetSize() > 0)
    {
        // Call the other version of InsertAt.
        // This will expand the array and will also 
        // set the 0th element
        InsertAt(nStartIndex, pNewArray->GetAt(0),
                              pNewArray->GetSize());              
        
        // Copy the rest of the array bit-by-bit
        for (JInt32 nIndex = 1; nIndex < pNewArray->GetSize();
                             ++nIndex)
        {
            SetAt(nStartIndex + nIndex, pNewArray->GetAt(nIndex));
        }
    }
}

JVoid CBitArray::RemoveAt(JInt32 nIndex, JInt32 nCount)
{
    assert(nIndex >= 0);
    assert(nCount >= 0);
    assert(nIndex + nCount <= m_nSize);

    for (JInt32 nCopyIndex=nIndex+nCount;nCopyIndex<GetSize(); ++nCopyIndex)
    {
        SetAt(nCopyIndex-nCount, GetAt(nCopyIndex));
    }
    m_nSize -= nCount;
}

JVoid CBitArray::FreeExtra()
{
    JInt32 nNewHostVarSize = BIT_TO_HOSTVAR_SIZE(m_nSize);
    if (nNewHostVarSize != (JInt32)BIT_TO_HOSTVAR_SIZE(m_nMaxSize))
    {
        // shrink to desired size
        HOSTVAR* pNewData = 0;
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

JBool CBitArray::GetAt(JInt32 nBitIndex) const
{
    assert(nBitIndex >= 0 && nBitIndex < m_nSize);

    JInt32 nByteIndex      = BIT_TO_BYTE_INDEX(nBitIndex);
    JUnt08* pData         = (JUnt08*)m_pData + nByteIndex;
    JInt32	nOffsetInByte	= BIT_OFFSET_IN_BYTE(nBitIndex);
    JUnt08 byMask         = BYTE_MASK[nOffsetInByte];

    return (*pData & byMask) ? true : false;
}

JVoid CBitArray::SetAt(JInt32 nBitIndex, JBool bValue)
{
    assert(nBitIndex >= 0 && nBitIndex < m_nSize);

    JInt32 nByteIndex			= BIT_TO_BYTE_INDEX(nBitIndex);
    JUnt08* pData	= (JUnt08*)m_pData + nByteIndex;

    JInt32	nOffsetInByte		= BIT_OFFSET_IN_BYTE(nBitIndex);
    JUnt08 byMask	= BYTE_MASK[nOffsetInByte];

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

JVoid CBitArray::SetRange(JInt32 nStartBitIndex,
                         JInt32 nEndBitIndex, JBool bValue)
{
    assert(nEndBitIndex >= 0 && nEndBitIndex < m_nSize);
    assert(nStartBitIndex >= 0 && nStartBitIndex <= nEndBitIndex);

    JInt32 nStartByteIndex     = BIT_TO_BYTE_INDEX(nStartBitIndex);
    JInt32 nEndByteIndex       = BIT_TO_BYTE_INDEX(nEndBitIndex);
    JInt32 nStartOffsetInByte  = BIT_OFFSET_IN_BYTE(nStartBitIndex);
    JInt32 nEndOffsetInByte    = BIT_OFFSET_IN_BYTE(nEndBitIndex);
    JUnt08 byMask    = 0;
    JUnt08* pData    = (JUnt08*)m_pData + nStartByteIndex;

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

    assert (nStartByteIndex < nEndByteIndex);

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
    JInt32 nBytesInBetween = nEndByteIndex - nStartByteIndex - 1;
    if (nBytesInBetween)
    {
        byMask = (bValue) ? 0xFF : 0;
        pData++;
        memset(pData, byMask, nBytesInBetween);
    }

    // End Byte
    byMask = GetByteMaskRange(0, nEndOffsetInByte);

    pData = (JUnt08*)m_pData + nEndByteIndex;
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

JUnt08 CBitArray::GetByteMaskRange(JInt32 nStartBit, JInt32 nEndBit) const
{
    assert(nStartBit>=0 && nStartBit<=7);
    assert(nEndBit>=0 && nEndBit<=7);
    assert(nStartBit <= nEndBit);

    JUnt08    byMask = 0;

    for (JInt32 nIndex=nStartBit; nIndex <= nEndBit; ++nIndex)
    {
        byMask = byMask | BYTE_MASK[nIndex];
    }
    return byMask;
}

JVoid CBitArray::SetAtGrow(JInt32 nBitIndex, JBool bValue)
{
    assert(nBitIndex >= 0);

    if (nBitIndex >= m_nSize)
    {
        SetSize(nBitIndex+1);
        assert(nBitIndex+1 == m_nSize);
    }
    SetAt(nBitIndex, bValue);
}


CBitArray & CBitArray::operator &=(const CBitArray &src)
{
    assert(this != &src);   // cannot Append to itself
    assert(m_nSize == src.m_nSize);   // must be the same size

	for(HOSTVAR x=0;x<BIT_TO_HOSTVAR_SIZE(src.m_nSize);++x)
	{
		m_pData[x]&=src.m_pData[x];
	}
	return(*this);
}

CBitArray & CBitArray::operator |=(const CBitArray &src)
{
    assert(this != &src);   // cannot Append to itself
    assert(m_nSize == src.m_nSize);   // must be the same size

	for(HOSTVAR x=0;x<BIT_TO_HOSTVAR_SIZE(src.m_nSize);++x)
	{
		m_pData[x]|=src.m_pData[x];
	}
	return(*this);
}

CBitArray & CBitArray::operator ^=(const CBitArray &src)
{
    assert(this != &src);   // cannot Append to itself
    assert(m_nSize == src.m_nSize);   // must be the same size

	for(HOSTVAR x=0;x<BIT_TO_HOSTVAR_SIZE(src.m_nSize);++x)
	{
		m_pData[x]^=src.m_pData[x];
	}
	return(*this);
}

CBitArray::CBitArray(const CBitArray &src)
{
    m_pData = 0;
    m_nSize = m_nMaxSize = m_nGrowBy = 0;
	*this=src;
}

CBitArray & CBitArray::operator =(const CBitArray &src)
{
    assert(this != &src);   // cannot Append to itself

    SetSize(src.m_nSize);
    memcpy(m_pData, src.m_pData, BIT_TO_BYTE_SIZE(src.m_nSize));
	return *this;
}

CBitArray CBitArray::operator ~() const
{
	CBitArray Temp(*this);
	for(HOSTVAR x=0;x<BIT_TO_HOSTVAR_SIZE(m_nSize);++x)
	{
		Temp.m_pData[x]=(~m_pData[x]);
	}
	return Temp;
}

CBitArray CBitArray::operator &(const CBitArray &src) const
{
	CBitArray Temp(*this);
	Temp&=src;
	return Temp;
}

CBitArray CBitArray::operator |(const CBitArray &src) const
{
	CBitArray Temp(*this);
	Temp|=src;
	return Temp;
}

CBitArray CBitArray::operator ^(const CBitArray &src) const
{
	CBitArray Temp(*this);
	Temp^=src;
	return Temp;
}

JVoid CBitArray::SetAll(JBool bValue)
{
	JUnt08*	pData = (JUnt08*)m_pData;
	JUnt08	byMask = (bValue) ? 0xFF : 0x00;

	memset(pData, byMask,BIT_TO_BYTE_SIZE(m_nSize));
}

JArchive& CBitArray::Recv( JArchive& lhs )
{
	JUnt08 *pChar = (JUnt08 *)m_pData;
	for(JUnt32 x= 0 ; x < (JUnt32)HOSTVAR_TO_BYTE_SIZE(BIT_TO_HOSTVAR_SIZE(m_nSize)); ++x)
	{
		lhs.Recv(pChar[x]);
	}
	return lhs;
}

JArchive& CBitArray::Send( JArchive& lhs )
{
	JUnt08 *pChar = (JUnt08 *)m_pData;
	for(JUnt32 x= 0 ; x < (JUnt32)HOSTVAR_TO_BYTE_SIZE(BIT_TO_HOSTVAR_SIZE(m_nSize)); ++x)
	{
		lhs.Send(pChar[x]);
	}
	return lhs;
}

