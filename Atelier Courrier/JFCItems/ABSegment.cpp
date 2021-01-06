// ABSegment.cpp: implementation of the CABSegment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABSegment.h"
#include "ABItem.h"
#include "ABQuestion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABSegment::CABSegment()
{

}

CABSegment::~CABSegment()
{
	Reset();
}

// Resets to zero this object
JBool	CABSegment::Reset()
{
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		delete m_lBloc.GetItem();
	}
	m_lBloc.Reset();
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CABSegment::IsEmpty()
{
	if(m_lBloc.GetCount())
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CABSegment::Validate()
{
	if(!HasOperator())
		return false;

	JList<IBloc *>	Bloc;
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		if(!m_lBloc.GetItem()->Validate())
			delete m_lBloc.GetItem();
		else
			Bloc.AddTail() = m_lBloc.GetItem();
	}
	m_lBloc.Swap(Bloc);

	return !IsEmpty();
}

// Validate this object 
JBool	CABSegment::IsValid()
{
	if(!HasOperator())
		return false;

	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		if(!m_lBloc.GetItem()->IsValid())
			return false;
	}

	return !IsEmpty();
}

// Converts this object to a string
JBool	CABSegment::AsString(std::string & str) const
{
	str.append(_T("S"));
	OperatorAsString(str, m_Operator);

	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		str.append(_T("("));
		m_lBloc.GetItem()->AsString(str);
		str.append(_T(")"));
	}
	return true;
}

// Builds this object from a string
JBool	CABSegment::FromString(const JChar ** pstr)
{
	JBool NewBloc = false;

	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '&': // Logical And
			{
				SetOperator(IBloc::And);
			}
			break;

			case '|': // Logical Or
			{
				SetOperator(IBloc::Or);
			}
			break;

			case '!': // Logical Not
			{
				SetOperator(IBloc::Not);
			}
			break;

			case '^': // Logical Xor
			{
				SetOperator(IBloc::Xor);
			}
			break;

			case 'I': // Item Bloc
			{
				NewBloc = false;
				m_lBloc.AddTail() = new CABItem();
				m_lBloc.MoveLast();
				(*pstr)++;
				if(!m_lBloc.GetItem()->FromString(pstr))
					return false;
			}
			break;

			case 'Y': // Question Bloc
			{
				NewBloc = false;
				m_lBloc.AddTail() = new CABQuestion();
				m_lBloc.MoveLast();
				(*pstr)++;
				if(!m_lBloc.GetItem()->FromString(pstr))
					return false;
			}
			break;

			case '(': // Start Bloc
			{
				NewBloc = true;
			}
			break;

			default:
				return false;

		}
		(*pstr)++;
	}
	return true;
}

// Accepts a visiting Object
JVoid	CABSegment::Accept(IAbsolutVisitor &Visitor )
{
	Visitor.VisitStart(*this);
	
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		m_lBloc.GetItem()->Accept(Visitor);
	}

	Visitor.VisitEnd(*this); 
}
