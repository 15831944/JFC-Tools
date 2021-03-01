
#pragma once

#include "IBloc.h"
#include "IItemVisitor.h"

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
class AFX_EXT_CLASS CIBQuant : public IBloc  
{
public:
	CIBQuant();
	virtual ~CIBQuant();
	/// Gets the values into values
	virtual void GetValues(JList<JInt32> & Values);
	/// Sets the values
	virtual JVoid SetValues(JList<JInt32> &Values);
	/// Adss a value
	virtual JVoid AddValue(JInt32 Value);
public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor ) { Visitor.Visit(*this); } ;
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


protected:
	JList<JInt32>		m_Value;

private:
	CIBQuant(const CIBQuant & rhs);
	CIBQuant & operator=(const CIBQuant & rhs);

};
