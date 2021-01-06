

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
class CABClass : public IBloc  
{
public:
	CABClass();
	virtual ~CABClass();
	/// Sets the values
	virtual JVoid SetValues(JList<JFlt32> &Values);
	/// Sets the values
	virtual JVoid SetOperators(JList<OPERATOR> &Operators);
	/// Gets the values into values
	virtual void GetValues(JList<JFlt32> & Values);
	/// Gets the Operators into Operators
	virtual void GetOperators(JList<OPERATOR> & Operators);

public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IAbsolutVisitor &Visitor ) { Visitor.Visit(*this); } ;
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
	JList<JFlt32>	m_Value;
	JList<OPERATOR>	m_Operators;

private:
	CABClass(const CABClass & rhs);
	CABClass & operator=(const CABClass & rhs);

};

