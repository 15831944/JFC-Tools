// ABuilder.cpp: implementation of the CABuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABuilder::CABuilder()
{

}

CABuilder::CABuilder(const CABuilder & rhs)
{
	*this = rhs;
}

CABuilder & CABuilder::operator=(const CABuilder & rhs)
{
	std::string Def;
	rhs.AsString(Def);
	const JChar * def = Def.c_str();
	FromString((const JChar **)&def);
	return *this;
}

CABuilder::~CABuilder()
{
	Reset();
}

// Resets to zero this object
JBool	CABuilder::Reset()
{
	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		delete m_Segment.GetItem();
	}
	m_Segment.Reset();
	SetOperator(IBloc::Nop);

	return true;
}

// Is this object Empty
JBool	CABuilder::IsEmpty()
{
	if(m_Segment.GetCount())
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CABuilder::Validate()
{
	if(!HasOperator())
		return false;

	JList<CABSegment *>		Segment;
	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		if(!m_Segment.GetItem()->Validate())
			delete m_Segment.GetItem();
		else
			Segment.AddTail() = m_Segment.GetItem();
	}
	m_Segment.Swap(Segment);

	return !IsEmpty();
}

// Validate this object removing all empty sub objects
JBool	CABuilder::IsValid()
{
	if(!HasOperator())
		return false;

	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		if(!m_Segment.GetItem()->IsValid())
			return false;
	}
	return !IsEmpty();
}

// Converts this object to a string
JBool	CABuilder::AsString(std::string & str) const
{
	OperatorAsString(str, m_Operator);

	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		str.append(_T("("));
		m_Segment.GetItem()->AsString(str);
		str.append(_T(")"));
	}
	str.append(_T(";"));
	return true;
}

// Builds this object from a string
JBool	CABuilder::FromString(const JChar ** pstr)
{
	JBool NewBloc = false;

	while(**pstr != ';')
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

			case 'S': // Segment Bloc
			{
				NewBloc = false;
				m_Segment.AddTail() = new CABSegment();
				m_Segment.MoveLast();
				(*pstr)++;
				if(!m_Segment.GetItem()->FromString(pstr))
					return false;
			}
			break;

			case '(': // Start Bloc
			{
				NewBloc = true;
			}
			break;

			case ')': // End Bloc
			{
				NewBloc = false;
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
JVoid	CABuilder::Accept(IAbsolutVisitor &Visitor )
{
	Visitor.VisitStart(*this); 
	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		m_Segment.GetItem()->Accept(Visitor);
	}
	Visitor.VisitEnd(*this); 
}
