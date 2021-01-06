
#pragma once


#include <string>

class IItemVisitor;
class IAbsolutVisitor;


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
class AFX_EXT_CLASS IBloc  
{
public:
	IBloc();
	virtual ~IBloc();

public:
	enum OPERATOR
	{
		Nop	=	0,	///< No Operator
		And =	1,	///< Operator AND
		Or	=	2,	///< Operator OR
		Not	=	3,	///< Operator NOT
		Xor =	4,	///< Operator XOR

		LT	=	20,	///< Less than
		GT	=	21,	///< Greater than
		LTE	=	22,	///< Less than or Equal to
		GTE	=	23,	///< Greater than or Equal to
		EQ	=	24,	///< Equal to

		ALL	=	40	///< All
	};

	/// Gets an Int32 value from a string
	virtual JInt32			ToIntValue(const JChar ** pstr) const;
	/// Gets an Flt32 value from a string
	virtual JFlt32			ToFltValue(const JChar ** pstr) const;
	/// Changes an Int32 value to a string
	virtual std::string		IntToString(JInt32 IValue) const;
	/// Changes a Flt32 value to a string
	virtual std::string		FltToString(JFlt32 FValue) const;
	//return an IBloc pointer
	virtual IBloc *			AsIBloc() { return this; } ;
	//return a const IBloc pointer
	virtual const IBloc *	AsIBloc() const { return this; } ;
	//Appends the operator to the string
	virtual JBool			OperatorAsString(std::string & str, OPERATOR Op) const;
	/// Sets the operator codes
	virtual JVoid			SetOperator(OPERATOR OpCode) { m_Operator = OpCode; } ;
	/// Returns true if an operator is present
	virtual JBool			HasOperator() const { return m_Operator != Nop; } ;
	/// Returns the operator codes
	virtual	OPERATOR		GetOperator() const { return m_Operator; } ;
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor ) {};
	/// Accepts a visiting Object
	virtual JVoid		Accept(IAbsolutVisitor &Visitor ) {};
	/// Build the XML opcode
	virtual JBool		OperatorAsXML(std::string & str, OPERATOR Op) const;

public:
	/// Overrides for the IBloc Interface
	/// Resets to zero this object
	virtual JBool		Reset() = 0;
	/// Is this object Empty
	virtual JBool		IsEmpty() = 0;
	/// Validate this object removing all empty sub objects
	virtual	JBool		Validate() = 0;
	/// Validate this object
	virtual	JBool		IsValid() = 0;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const = 0;
	/// Builds this object from a string
	virtual JBool		FromString(const JChar ** pstr) = 0;

	OPERATOR			m_Operator;
};
