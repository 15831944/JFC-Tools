// ABTerme.cpp: implementation of the CABTerme class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABTerme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABTerme::CABTerme()
{
	m_Terme.Reset();
}

CABTerme::~CABTerme()
{
	Reset();
}

// Resets to zero this object
JBool	CABTerme::Reset()
{
	m_ID = 0;

	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		delete m_Terme.GetItem();
	}
	m_Terme.Reset();
	SetOperator(IBloc::Nop);

	return true;
}
// Is this object Empty
JBool	CABTerme::IsEmpty()
{
	// Have to have at least one terme
	if(m_Terme.GetCount() || m_ID != 0)
		return false;

	return true;
}

// Validate this object removing all empty sub objects
JBool	CABTerme::Validate()
{

	JList<CABTerme *>	Terme;
	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		if(!m_Terme.GetItem()->Validate())
			delete m_Terme.GetItem();
		else
			Terme.AddTail() = m_Terme.GetItem();
	}
	m_Terme.Swap(Terme);

	return !IsEmpty();
}

// Validate this object removing all empty sub objects
JBool	CABTerme::IsValid()
{
	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		if(!m_Terme.GetItem()->IsValid())
			return false;
	}
	return !IsEmpty();
}

// Converts this object to a string
JBool	CABTerme::AsString(std::string & str) const
{
	if(HasOperator())
		OperatorAsString(str, m_Operator);

	if(m_Terme.GetCount())
	{
		for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
		{
			str.append(_T("("));
			m_Terme.GetItem()->AsString(str);
			str.append(_T(")"));
		}
	}
	else
	{
		str.append(_T("r"));
		str.append(IntToString(m_ID));
	}
	return true;
}

// Builds this object from a string
JBool	CABTerme::FromString(const JChar ** pstr)
{
	JBool NewBloc = false;

	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '&': // Logical And
			{
				if(NewBloc)
				{
					NewBloc = false;
					m_Terme.AddTail() = new CABTerme();
					m_Terme.MoveLast();
					if(!m_Terme.GetItem()->FromString(pstr))
						return false;
				}
				else
					SetOperator(IBloc::And);
			}
			break;

			case '|': // Logical Or
			{
				if(NewBloc)
				{
					NewBloc = false;
					m_Terme.AddTail() = new CABTerme();
					m_Terme.MoveLast();
					if(!m_Terme.GetItem()->FromString(pstr))
						return false;
				}
				else
					SetOperator(IBloc::Or);
			}
			break;

			case '!': // Logical Not
			{
				if(NewBloc)
				{
					NewBloc = false;
					m_Terme.AddTail() = new CABTerme();
					m_Terme.MoveLast();
					if(!m_Terme.GetItem()->FromString(pstr))
						return false;
				}
				else
					SetOperator(IBloc::Not);
			}
			break;

			case '^': // Logical Xor
			{
				if(NewBloc)
				{
					NewBloc = false;
					m_Terme.AddTail() = new CABTerme();
					m_Terme.MoveLast();
					if(!m_Terme.GetItem()->FromString(pstr))
						return false;
				}
				else
					SetOperator(IBloc::Xor);
			}
			break;

			case 'r': // Terme Bloc
			{
				NewBloc = false;
				m_Terme.AddTail() = new CABTerme();
				m_Terme.MoveLast();
				(*pstr)++;
				if(!m_Terme.GetItem()->SetID(ToIntValue(pstr)))
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
JVoid	CABTerme::Accept(IAbsolutVisitor &Visitor )
{
	Visitor.VisitStart(*this);
	
	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		m_Terme.GetItem()->Accept(Visitor);
	}

	Visitor.VisitEnd(*this);
}
