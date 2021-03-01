// ABQuestion.cpp: implementation of the CABQuestion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABQuestion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABQuestion::CABQuestion()
{
	m_Terme.Reset();
	m_pClass	=	0;
	m_pQuant	=	0;
	m_pScore	=	0;
	m_pNumValue	=	0;
	m_pPoint	=	0;
}

CABQuestion::~CABQuestion()
{
	Reset();
}

// Resets to zero this object
JBool	CABQuestion::Reset()
{
	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		delete m_Terme.GetItem();
	}
	m_Terme.Reset();

	if(m_pClass)
		delete m_pClass;
	m_pClass = 0;

	if(m_pQuant)
		delete m_pQuant;
	m_pQuant = 0;

	if(m_pScore)
		delete m_pScore;
	m_pScore = 0;

	if(m_pNumValue)
		delete m_pNumValue;
	m_pNumValue = 0;

	if(m_pPoint)
		delete m_pPoint;
	m_pPoint = 0;

	SetOperator(IBloc::Nop);

	return true;
}
// Is this object Empty
JBool	CABQuestion::IsEmpty()
{
	// Have to have at least one terme
	if(m_Terme.GetCount())
		return false;

	return true;
}

// Validate this object removing all empty sub objects
JBool	CABQuestion::Validate()
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


	if(m_pClass && !m_pClass->Validate())
	{
		delete m_pClass;
		m_pClass = 0;
	}

	if(m_pQuant && !m_pQuant->Validate())
	{
		delete m_pQuant;
		m_pQuant = 0;
	}

	if(m_pScore && !m_pScore->Validate())
	{
		delete m_pScore;
		m_pScore = 0;
	}

	if(m_pNumValue && !m_pNumValue->Validate())
	{
		delete m_pNumValue;
		m_pNumValue = 0;
	}

	if(m_pPoint && !m_pPoint->Validate())
	{
		delete m_pPoint;
		m_pPoint = 0;
	}

	return !IsEmpty();
}

// Validate this object
JBool	CABQuestion::IsValid()
{

	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		if(!m_Terme.GetItem()->IsValid())
			return false;
	}

	if(m_pClass && !m_pClass->IsValid())
		return false;
	if(m_pQuant && !m_pQuant->IsValid())
		return false;
	if(m_pScore && !m_pScore->IsValid())
		return false;
	if(m_pNumValue && !m_pNumValue->IsValid())
		return false;
	if(m_pPoint && !m_pPoint->IsValid())
		return false;

	return !IsEmpty();
}

// Converts this object to a string
JBool	CABQuestion::AsString(std::string & str) const
{
	str.append(_T("Y"));
	OperatorAsString(str, m_Operator);

	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
	{
		str.append(_T("("));
		m_Terme.GetItem()->AsString(str);
		str.append(_T(")"));
	}

	if(m_pClass)
	{
		str.append(_T("("));
		m_pClass->AsString(str);
		str.append(_T(")"));
	}

	if(m_pQuant)
	{
		str.append(_T("("));
		m_pQuant->AsString(str);
		str.append(_T(")"));
	}

	if(m_pScore)
	{
		str.append(_T("("));
		m_pScore->AsString(str);
		str.append(_T(")"));
	}

	if(m_pNumValue)
	{
		str.append(_T("("));
		m_pNumValue->AsString(str);
		str.append(_T(")"));
	}

	if(m_pPoint)
	{
		str.append(_T("("));
		m_pPoint->AsString(str);
		str.append(_T(")"));
	}

	return true;
}

// Builds this object from a string
JBool	CABQuestion::FromString(const JChar ** pstr)
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

			case 'C': // Classe Bloc
			{
				NewBloc = false;
				m_pClass = new CABClass();
				(*pstr)++;
				if(!m_pClass->FromString(pstr))
					return false;
			}
			break;

			case 'Q': // Quantile Bloc
			{
				NewBloc = false;
				m_pQuant = new CABQuant();
				(*pstr)++;
				if(!m_pQuant->FromString(pstr))
					return false;
			}
			break;

			case 'R': // Score Bloc
			{
				NewBloc = false;
				m_pScore = new CABScore();
				(*pstr)++;
				if(!m_pScore->FromString(pstr))
					return false;
			}
			break;

			case 'V': // Numeric Values Bloc
			{
				NewBloc = false;
				m_pNumValue = new CABNumValue();
				(*pstr)++;
				if(!m_pNumValue->FromString(pstr))
					return false;
			}
			break;

			case 'P': // Point Values Bloc
			{
				NewBloc = false;
				m_pPoint = new CABPoint();
				(*pstr)++;
				if(!m_pPoint->FromString(pstr))
					return false;
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
JVoid	CABQuestion::Accept(IAbsolutVisitor &Visitor )
{
	Visitor.VisitStart(*this);
	
	for(m_Terme.MoveFirst(); m_Terme.IsValid(); m_Terme.MoveNext())
		m_Terme.GetItem()->Accept(Visitor);

	if(m_pClass)
		m_pClass->Accept(Visitor);
	if(m_pNumValue)
		m_pNumValue->Accept(Visitor);
	if(m_pQuant)
		m_pQuant->Accept(Visitor);
	if(m_pScore)
		m_pScore->Accept(Visitor);
	if(m_pPoint)
		m_pPoint->Accept(Visitor);

	Visitor.VisitEnd(*this); 
}
