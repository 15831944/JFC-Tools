
#pragma once


#include "IBModalite.h"
#include "IItemVisitor.h"
#include "IBPoint.h"

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
class AFX_EXT_CLASS CIBItem : public CIBModalite  
{
public:
	CIBItem();
	virtual ~CIBItem();

	JInt32	GetID() { return m_ID; } ;
	JBool	SetID(JInt32 ID) { m_ID = ID; return !ID; } ;

public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor );
	/// Resets to zero this object
	virtual JBool		Reset() ;
	/// Is this object Empty
	virtual JBool		IsEmpty() ;
	/// Validate this object removing all empty sub objects
	virtual	JBool		Validate() ;
	/// Validate this object
	virtual	JBool		IsValid() ;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const ;
	/// Builds this object from a string
	virtual JBool		FromString(const JChar ** pstr) ;
	/// Adds a Point to the list
	virtual	JVoid		SetPoint(CIBPoint *pObject)		{ if(m_pPoint) delete m_pPoint; m_pPoint = pObject; } ;
	/// Gets a Point
	virtual	CIBPoint * GetPoint()					{ return m_pPoint; } ;

private:
	JInt32				m_ID;
	CIBPoint *			m_pPoint;
	CIBItem(const CIBItem & rhs);
	CIBItem & operator=(const CIBItem & rhs);

};
