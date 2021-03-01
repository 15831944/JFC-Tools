// IBItem.cpp: implementation of the CIBItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBItem::CIBItem()
{
	m_ID = 0;
	m_pPoint = 0;
}

CIBItem::~CIBItem()
{
	Reset();
}

// Converts this object to a string
JBool	CIBItem::AsString(std::string & str) const
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
JBool	CIBItem::Reset()
{
	m_ID = 0;
	m_Value.Reset();
	if(m_pPoint)
	{
		delete m_pPoint;
		m_pPoint = 0;
	}
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CIBItem::IsEmpty()
{
	if(m_Value.GetCount())
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CIBItem::Validate()
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
JBool	CIBItem::IsValid()
{
	if(m_pPoint && !m_pPoint->IsValid())
		return false;

	if(HasOperator() && m_Value.GetCount())
		return true;

	return false;
}

// Builds this object from a string
JBool	CIBItem::FromString(const JChar ** pstr)
{
	// Skip past the %
	(*pstr)++;
	// Set the item container ID
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
//				NewBloc = true;
			}
			break;

			case 'P': // Point Values Bloc
			{
//				NewBloc = false;
				m_pPoint = new CIBPoint();
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

JVoid	CIBItem::Accept(IItemVisitor &Visitor)
{
	Visitor.VisitStart(*this);
	if(m_pPoint)
		m_pPoint->Accept(Visitor);
	Visitor.VisitEnd(*this);
}
