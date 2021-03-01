// ABItem.cpp: implementation of the CABItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABItem::CABItem()
{
	m_ID		=	0;
	m_pPoint	=	0;
}

CABItem::~CABItem()
{
	Reset();
}

// Converts this object to a string
JBool	CABItem::AsString(std::string & str) const
{
	str.append(_T("I"));
	str.append(_T("%"));
	str.append(IntToString(m_ID));
	OperatorAsString(str, m_Operator);

	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		str.append(_T("%"));
		str.append(IntToString(m_Value.GetItem()));
	}

	if(m_pPoint)
	{
		str.append(_T("("));
		m_pPoint->AsString(str);
		str.append(_T(")"));
	}

	return true;
}

// Resets to zero this object
JBool	CABItem::Reset()
{
	m_ID = 0;
	m_Value.Reset();
	if(m_pPoint)
		delete m_pPoint;
	m_pPoint = 0;

	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CABItem::IsEmpty()
{
	if(m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CABItem::Validate()
{
	if(m_pPoint && !m_pPoint->Validate())
	{
		delete m_pPoint;
		m_pPoint = 0;
	}

	if(HasOperator() && m_Value.GetCount())
		return true;

	return false;
}

// Validate this object removing all empty sub objects
JBool	CABItem::IsValid()
{
	if(m_pPoint && !m_pPoint->IsValid())
		return false;

	if(HasOperator() && m_Value.GetCount())
		return true;
	return false;
}

// Builds this object from a string
JBool	CABItem::FromString(const JChar ** pstr)
{
	// skip past the %
	(*pstr)++;
	// get the Item container ID
	m_ID = ToIntValue(pstr);
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

			case '(': // Start Bloc
			{
				(*pstr)++;
			}
			break;

			case 'P': // Point Values Bloc
			{
				m_pPoint = new CABPoint();
				(*pstr)++;
				if(!m_pPoint->FromString(pstr))
					return false;
			}
			break;

			default:
				return false;

		}
	}
	return true;
}

JVoid CABItem::Accept(IAbsolutVisitor &Visitor ) 
{
	Visitor.VisitStart(*this);

	if(m_pPoint)
		m_pPoint->Accept(Visitor);

	Visitor.VisitEnd(*this);
}

// Sets the values
JVoid CABItem::SetValues(JList<JInt32> &Values)
{
	m_Value.Reset();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_Value.AddTail() = Values.GetItem();
	}
}

void CABItem::GetValues(JList<JInt32> & Values)
{
	Values.Reset();
	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		Values.AddTail() = m_Value.GetItem();
	}
}
