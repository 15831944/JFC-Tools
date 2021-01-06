
#pragma once


#include "IBloc.h"
#include "IBSegment.h"
#include "IItemVisitor.h"
class CItemsConstruitsBase;

/**
 * \ingroup JFCITEMS
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
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CIBuilder : public IBloc  
{
public:
	CIBuilder();
	virtual ~CIBuilder();
	CIBuilder(const CIBuilder & rhs);
	virtual CIBuilder & operator=(const CIBuilder & rhs);

	JVoid			Add(CIBSegment *pSegment) { m_Segment.AddTail() = pSegment; } ;
	JInt32			GetSegmentCount() { return m_Segment.GetCount(); } ;
	CIBSegment *	GetSegment(JInt32 Index);
	JBool			RemoveSegment(CIBSegment *pSegment);
	JBool			DeleteSegment(CIBSegment *pSegment);
	JBool			HasItem(CItemsConstruitsBase *pIConst, int ID);

public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor );
	/// Resets to zero this object
	virtual JBool		Reset();
	/// Is this object Empty
	virtual JBool		IsEmpty();
	/// Validate this object removing all empty sub objects
	virtual	JBool		Validate();
	/// Validate this object
	virtual	JBool		IsValid() ;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const;
	/// Builds this object from a string
	virtual JBool		FromString(const JChar ** pstr);

protected:

	JList<CIBSegment *>		m_Segment;

};
