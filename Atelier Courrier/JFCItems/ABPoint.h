
#pragma once


#include "IBloc.h"
#include "IAbsolutVisitor.h"

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
class CABPoint : public IBloc  
{
public:
	CABPoint();
	virtual ~CABPoint();
	/// Sets the value
	virtual JVoid SetValue(JInt32 Value) { m_Value = Value; };
	/// Get the value
	virtual JInt32 GetValue() { return m_Value;};

public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IAbsolutVisitor &Visitor ) { Visitor.Visit(*this); } ;
	/// Resets to zero this object
	virtual JBool		Reset() ;
	/// Is this object Empty
	virtual JBool		IsEmpty()  { return !IsValid();};
	/// Validate this object removing all empty sub objects
	virtual	JBool		Validate() { return IsValid();};
	/// Validate this object
	virtual	JBool		IsValid() ;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const ;
	/// Builds this object from a string
	virtual JBool		FromString(const JChar ** pstr) ;


protected:
	JInt32				m_Value;

private:
	CABPoint(const CABPoint & rhs);
	CABPoint & operator=(const CABPoint & rhs);

};

