// IBQuestion.cpp: implementation of the CIBQuestion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBQuestion.h"
#include "QuestionVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBQuestion::CIBQuestion()
{
	m_ID = 0;
	m_pClass = 0;
	m_pNumValue = 0;
	m_pQuant = 0;
	m_pScore = 0;
	m_pPoint = 0;
}

CIBQuestion::~CIBQuestion()
{
	Reset();
}

// Resets to zero this object
JBool	CIBQuestion::Reset()
{
	m_ID = 0;

	for(m_Modalite.MoveFirst(); m_Modalite.IsValid(); m_Modalite.MoveNext())
	{
		delete m_Modalite.GetItem();
	}
	m_Modalite.Reset();

	if(m_pClass)
	{
		delete m_pClass;
		m_pClass = 0;
	}

	if(m_pQuant)
	{
		delete m_pQuant;
		m_pQuant = 0;
	}

	if(m_pScore)
	{
		delete m_pScore;
		m_pScore = 0;
	}
	
	if(m_pNumValue)
	{
		delete m_pNumValue;
		m_pNumValue = 0;
	}

	if(m_pPoint)
	{
		delete m_pPoint;
		m_pPoint = 0;
	}

	SetOperator(IBloc::Nop);

	return true;
}
// Is this object Empty
JBool	CIBQuestion::IsEmpty()
{
	// Have to have Question ID
	if(!m_ID)
		return true;

	// Have to have at least a modality or and element
	if(m_Modalite.GetCount() /*|| m_Element.GetCount()*/)
		return false;

	return true;
}

// Validate this object removing all empty sub objects
JBool	CIBQuestion::Validate()
{
	if(!m_ID)
		return false;

	JList<CIBModalite *>	Modalite;
	for(m_Modalite.MoveFirst(); m_Modalite.IsValid(); m_Modalite.MoveNext())
	{
		if(!m_Modalite.GetItem()->Validate())
			delete m_Modalite.GetItem();
		else
			Modalite.AddTail() = m_Modalite.GetItem();
	}
	m_Modalite.Swap(Modalite);

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

// Validate this object removing all empty sub objects
JBool	CIBQuestion::IsValid()
{
	if(!m_ID)
		return false;

	if (m_Modalite.GetCount())
	{
		for(m_Modalite.MoveFirst(); m_Modalite.IsValid(); m_Modalite.MoveNext())
		{
			if(!m_Modalite.GetItem()->IsValid())
				return false;
		}
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
JBool	CIBQuestion::AsString(std::string & str) const
{
	str.append(_T("Y"));
	OperatorAsString(str, m_Operator);
	str.append("%");
	str.append(IntToString(m_ID));

	for(m_Modalite.MoveFirst(); m_Modalite.IsValid(); m_Modalite.MoveNext())
	{
		str.append(_T("("));
		m_Modalite.GetItem()->AsString(str);
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
JBool	CIBQuestion::FromString(const JChar ** pstr)
{
//	JBool NewBloc = false;

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

			case '%': // Question ID
			{
				(*pstr)++;
				m_ID = ToIntValue(pstr);
			}
			break;

			case 'E': // Element Bloc
			case 'M': // Modality Bloc
			{
//				NewBloc = false;
				m_Modalite.AddTail() = new CIBModalite();
				m_Modalite.MoveLast();
				(*pstr)++;
				if(!m_Modalite.GetItem()->FromString(pstr))
					return false;
			}
			break;

			case '(': // Start Bloc
			{
//				NewBloc = true;
			}
			break;

			case 'C': // Classe Bloc
			{
//				NewBloc = false;
				m_pClass = new CIBClass();
				(*pstr)++;
				if(!m_pClass->FromString(pstr))
					return false;
			}
			break;

			case 'Q': // Quantile Bloc
			{
//				NewBloc = false;
				m_pQuant = new CIBQuant();
				(*pstr)++;
				if(!m_pQuant->FromString(pstr))
					return false;
			}
			break;

			case 'R': // Score Bloc
			{
//				NewBloc = false;
				m_pScore = new CIBScore();
				(*pstr)++;
				if(!m_pScore->FromString(pstr))
					return false;
			}
			break;

			case 'V': // Numeric Values Bloc
			{
//				NewBloc = false;
				m_pNumValue = new CIBNumValue();
				(*pstr)++;
				if(!m_pNumValue->FromString(pstr))
					return false;
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
		(*pstr)++;
	}
	return true;
}

// Accepts a visiting Object
JVoid	CIBQuestion::Accept(IItemVisitor &Visitor )
{
	Visitor.VisitStart(*this);
	
	for(m_Modalite.MoveFirst(); m_Modalite.IsValid(); m_Modalite.MoveNext())
	{
		m_Modalite.GetItem()->Accept(Visitor);
	}

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

/// Resets the modalities list
JVoid CIBQuestion::ResetModalites()
{
	for(m_Modalite.MoveFirst(); m_Modalite.IsValid(); m_Modalite.MoveNext())
	{
		delete m_Modalite.GetItem();
	}
	m_Modalite.Reset();
}

// Retrieves all modalites
JVoid CIBQuestion::GetModalites(JList<CIBModalite *> &Modalites)
{
	CQuestionVisitor Visitor;
	Visitor.GetModalites(this,Modalites); 
}

