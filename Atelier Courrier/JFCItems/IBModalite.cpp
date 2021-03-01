// IBModalite.cpp: implementation of the CIBModalite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBModalite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBModalite::CIBModalite()
{
}

CIBModalite::~CIBModalite()
{
	Reset();
}

// Resets to zero this object
JBool	CIBModalite::Reset()
{
	m_Value.Reset();
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CIBModalite::IsEmpty()
{
	if(m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CIBModalite::Validate()
{
	if(HasOperator() && m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CIBModalite::IsValid()
{
	if(HasOperator() && m_Value.GetCount())
		return true;
	return false;
}

// Converts this object to a string
JBool	CIBModalite::AsString(std::string & str) const
{
	str.append(_T("M"));
	OperatorAsString(str, m_Operator);

	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		str.append(_T("%"));
		str.append(IntToString(m_Value.GetItem()));
	}

	return true;
}

// Builds this object from a string
JBool	CIBModalite::FromString(const JChar ** pstr)
{
	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '&': // Logical And
			{
				SetOperator(IBloc::And);
				(*pstr)++;
			}
			break;

			case '|': // Logical Or
			{
				SetOperator(IBloc::Or);
				(*pstr)++;
			}
			break;

			case '!': // Logical Not
			{
				SetOperator(IBloc::Not);
				(*pstr)++;
			}
			break;

			case '^': // Logical Xor
			{
				SetOperator(IBloc::Xor);
				(*pstr)++;
			}
			break;

			case '%': // Value
			{
				(*pstr)++;
				m_Value.AddTail() = ToIntValue(pstr);
			}
			break;

			default:
				return false;

		}
	}
	return true;
}

// Sets the values
JVoid CIBModalite::SetValues(JList<JInt32> &Values)
{
	m_Value.Reset();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_Value.AddTail() = Values.GetItem();
	}
}

void CIBModalite::GetValues(JList<JInt32> & Values)
{
	Values.Reset();
	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		Values.AddTail() = m_Value.GetItem();
	}
}

// Adds a value
void CIBModalite::AddValue(JInt32 Value)
{
	m_Value.AddTail() = Value;
}
