
#pragma once

#pragma warning( disable : 4786 )

#define HOSTVAR	JUnt32
#include <memory>

/**
 * \ingroup JFCSOURCE EXTERNAL
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CBitArray
{
public:
    CBitArray					();
    ~CBitArray					();
	/// Copy constructor
	CBitArray					(const CBitArray &src);
	/// Assignment Operator
	CBitArray &		operator=	(const CBitArray& src);
	/// Assignment operator And
	CBitArray &		operator&=	(const CBitArray& src);
	/// Assignment operator Or
	CBitArray &		operator|=	(const CBitArray& src);
	/// Assignement operator XOr
	CBitArray &		operator^=	(const CBitArray& src);
	/// Complement Operator
	CBitArray		operator~	() const;
	/// And Operator
	CBitArray		operator&	(const CBitArray& src) const;
	/// Or operator
	CBitArray		operator|	(const CBitArray& src) const;
	/// Xor Operator
	CBitArray		operator^	(const CBitArray& src) const;
	/// Load from Archive
	JArchive& Recv( JArchive& lhs );
	/// Save to Archive
	JArchive& Send( JArchive& lhs );
	/// Gets the size in Number of bits
    JInt32	GetSize				() const { return m_nSize; } ;
	/// Gets the upper bound value
    JInt32	GetUpperBound		() const { return m_nSize-1; } ;
	/// Sets the array size
    JVoid	SetSize				(JInt32 nNewSize, JInt32 nGrowBy = -1);
    /// Remove non used memory
    JVoid	FreeExtra			();
	/// Removes all memory
    JVoid	RemoveAll			() { SetSize(0); };
	/// Gets the value at index
    JBool	GetAt				(JInt32 nBitIndex) const;
	/// Sets the value at index
    JVoid	SetAt				(JInt32 nBitIndex, JBool bValue);
	/// Sets the value at index and grows the array if necessay
    JVoid	SetAtGrow			(JInt32 nBitIndex, JBool bValue);
	/// Sets all the bits to the given value
	JVoid	SetAll				(JBool bValue);
	/// Sets a range of bits with the given value
    JVoid	SetRange			(JInt32 nStartBitIndex, JInt32 nEndBitIndex, JBool bValue);
    /// Potentially growing the array
    JVoid	SetAtGrow			(JInt32 nIndex, JUnt08 newElement);
	/// Adds a value to the array
    JInt32	Add					(JBool bValue);
	/// Appends a given array to the end of this
    JInt32	Append				(const CBitArray& src);
	/// Inserts an element at the given index
    JVoid	InsertAt			(JInt32 nIndex, JBool bNewElement, JInt32 nCount = 1);
	/// Removes an element from the given index position
    JVoid	RemoveAt			(JInt32 nIndex, JInt32 nCount = 1);
	/// Inserts another array at the given index
    JVoid	InsertAt			(JInt32 nStartIndex, CBitArray* pNewArray);

	/// Get the data
	const JUnt32* GetData() const { return m_pData; };

/// Implementation
protected:
    HOSTVAR*		m_pData;
    JInt32			m_nSize;
    JInt32			m_nMaxSize;
    JInt32			m_nGrowBy;

    JUnt08	GetByteMaskRange	(JInt32 nStartBit, JInt32 nEndBit=7) const;
};

/// A JVector of CBitArray objects
typedef JVector<CBitArray> BITARRAYVECTOR;
/// An auto pointer to a CBitArray object
typedef std::auto_ptr<CBitArray> PTR_CBITARRAY;
