// IBuilder.cpp: implementation of the CIBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBuilder.h"
#include "IBSegment.h"
#include "HasItemVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBuilder::CIBuilder()
{
}

CIBuilder::CIBuilder(const CIBuilder & rhs)
{
	*this = rhs;
}

CIBuilder & CIBuilder::operator=(const CIBuilder & rhs)
{
	std::string Def;
	rhs.AsString(Def);
	const JChar * def = Def.c_str();
	FromString((const JChar **)&def);
	return *this;
}

CIBuilder::~CIBuilder()
{
	Reset();
}

// Resets to zero this object
JBool	CIBuilder::Reset()
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
JBool	CIBuilder::IsEmpty()
{
	if(m_Segment.GetCount())
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CIBuilder::Validate()
{
	if(!HasOperator())
		return false;

	JList<CIBSegment *>		Segment;
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
JBool	CIBuilder::IsValid()
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
JBool	CIBuilder::AsString(std::string & str) const
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
JBool	CIBuilder::FromString(const JChar ** pstr)
{
	Reset();
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
				m_Segment.AddTail() = new CIBSegment();
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
JVoid	CIBuilder::Accept(IItemVisitor &Visitor )
{
	Visitor.VisitStart(*this);
	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		m_Segment.GetItem()->Accept(Visitor);
	}
	Visitor.VisitEnd(*this); 
}


// returns the segment with the index 
CIBSegment * CIBuilder::GetSegment(JInt32 Index)
{
	if(Index < m_Segment.GetCount())
	{
		JInt32 x = 0;
		for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext(), x++)
		{
			if(x == Index)
				return m_Segment.GetItem();
		}
	}
	return 0;
}

JBool	CIBuilder::RemoveSegment(CIBSegment *pSegment)
{
	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		if(pSegment == m_Segment.GetItem())
		{
			m_Segment.Remove();
			return true;
		}
	}
	return false;
}

JBool	CIBuilder::DeleteSegment(CIBSegment *pSegment)
{
	for(m_Segment.MoveFirst(); m_Segment.IsValid(); m_Segment.MoveNext())
	{
		if(pSegment == m_Segment.GetItem())
		{
			delete m_Segment.GetItem();
			m_Segment.Remove();
			return true;
		}
	}
	return false;
}

JBool	CIBuilder::HasItem(CItemsConstruitsBase *pIConst, int ID)
{
	CHasItemVisitor Visitor;

	return Visitor.HasItem(this,pIConst,ID); 
}
