// IBClass.cpp: implementation of the CIBClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBClass::CIBClass()
{
}

CIBClass::~CIBClass()
{
	Reset();
}

// Resets to zero this object
JBool	CIBClass::Reset()
{
	m_Value.Reset();
	m_Operators.Reset();
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CIBClass::IsEmpty() 
{
	if(m_Value.GetCount())
		return false;
	return true;
}
// Validate this object removing all empty sub objects
JBool	CIBClass::Validate()
{
	if(!m_Value.GetCount())
		return false;
	if(m_Value.GetCount() != m_Operators.GetCount())
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CIBClass::IsValid()
{
	if(!m_Value.GetCount())
		return false;
	if(!m_Value.GetCount() || !m_Operators.GetCount())
		return false;
	if(m_Value.GetCount() != m_Operators.GetCount())
		return false;
	return true;
}

// Converts this object to a string
JBool	CIBClass::AsString(std::string & str) const
{
	str.append(_T("C"));
	for(m_Value.MoveFirst(), m_Operators.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext(), m_Operators.MoveNext())
	{
		OperatorAsString(str,m_Operators.GetItem());
		str.append(_T("%"));
		str.append(FltToString(m_Value.GetItem()));
	}

	return true;
}

// Builds this object from a string
JBool	CIBClass::FromString(const JChar ** pstr)
{
	OPERATOR CurOp = IBloc::Nop;

	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '>': // Greater than 
			{
				CurOp = IBloc::GT;
				(*pstr)++;
			}
			break;

			case '<': // Less than or Equals
			{
				CurOp = IBloc::LT;
				(*pstr)++;// Skip past the less
			}
			break;

			case '=': // Equals
			{
				switch(CurOp)
				{
					case IBloc::GT:
					{
						CurOp = IBloc::GTE;
					}
					break;

					case IBloc::LT:
					{
						CurOp = IBloc::LTE;
					}
					break;

					case IBloc::Nop:
					{
						CurOp = IBloc::EQ;
					}
					break;

				}
				(*pstr)++;
			}

			case '%': // Value
			{
				m_Operators.AddTail() = CurOp;
				CurOp = IBloc::Nop;
				(*pstr)++;// Skip past the percent
				m_Value.AddTail() = ToFltValue(pstr);
			}
			break;

			default:
				return false;

		}
	}
	return true;
}

// Get values into values
void CIBClass::GetValues(JList<JFlt32> & Values)
{
	Values.Reset();
	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		Values.AddTail() = m_Value.GetItem();
	}
}

// Gets the Operators into Operators
void CIBClass::GetOperators(JList<OPERATOR> & Operators)
{
	Operators.Reset();
	for(m_Operators.MoveFirst(); m_Operators.IsValid(); m_Operators.MoveNext())
	{
		Operators.AddTail() = m_Operators.GetItem();
	}
}

// Sets the values
JVoid CIBClass::SetValues(JList<JFlt32> &Values)
{
	m_Value.Reset();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_Value.AddTail() = Values.GetItem();
	}
}

// Sets the values
JVoid CIBClass::SetOperators(JList<OPERATOR> &Operators)
{
	m_Operators.Reset();

	for(Operators.MoveFirst(); Operators.IsValid(); Operators.MoveNext())
	{
		m_Operators.AddTail() = Operators.GetItem();
	}
}
