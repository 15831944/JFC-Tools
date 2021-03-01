// IBSegment.cpp: implementation of the CIBSegment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBSegment.h"
#include "IBItem.h"
#include "IBQuestion.h"
#include "IBModalite.h"
#include "QuestionVisitor.h"
#include "ItemVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBSegment::CIBSegment()
{

}

CIBSegment::~CIBSegment()
{
	Reset();
}

// Resets to zero this object
JBool	CIBSegment::Reset()
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
JBool	CIBSegment::IsEmpty()
{
	if(m_lBloc.GetCount())
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CIBSegment::Validate()
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

// Validate this object removing all empty sub objects
JBool	CIBSegment::IsValid()
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
JBool	CIBSegment::AsString(std::string & str) const
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
JBool	CIBSegment::FromString(const JChar ** pstr)
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
				m_lBloc.AddTail() = new CIBItem();
				m_lBloc.MoveLast();
				(*pstr)++;
				if(!m_lBloc.GetItem()->FromString(pstr))
					return false;
			}
			break;

			case 'Y': // Question Bloc
			{
				NewBloc = false;
				m_lBloc.AddTail() = new CIBQuestion();
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
JVoid	CIBSegment::Accept(IItemVisitor &Visitor )
{
	Visitor.VisitStart(*this);
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		m_lBloc.GetItem()->Accept(Visitor);
	}
	Visitor.VisitEnd(*this); 
}

JVoid CIBSegment::GetQuestionsID(JList<JInt32> & Items)
{
	Items.Reset();
	CQuestionVisitor Visitor;
	Visitor.GetQuestionsID(this,Items); 
}

JVoid CIBSegment::GetItemsID(JList<JInt32> & Items)
{
	Items.Reset();
	CItemVisitor Visitor;
	Visitor.GetItemsID(this,Items); 
}

CIBQuestion * CIBSegment::FindQuestion(JInt32 Quid)
{
	CQuestionVisitor Visitor;
	return Visitor.FindQuestion(this,Quid);
}

JBool	CIBSegment::DeleteQuestion(CIBQuestion *pQuestion)
{
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		if(pQuestion == m_lBloc.GetItem())
		{
			delete m_lBloc.GetItem();
			m_lBloc.Remove();
			return true;
		}
	}
	return false;
}

JBool	CIBSegment::DeleteItem(CIBItem *pItem)
{
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		if(pItem == m_lBloc.GetItem())
		{
			delete m_lBloc.GetItem();
			m_lBloc.Remove();
			return true;
		}
	}
	return false;
}

JBool	CIBSegment::RemoveQuestion(CIBQuestion *pQuestion)
{
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		if(pQuestion == m_lBloc.GetItem())
		{
			m_lBloc.Remove();
			return true;
		}
	}
	return false;
}

JBool	CIBSegment::RemoveItem(CIBItem *pItem)
{
	for(m_lBloc.MoveFirst(); m_lBloc.IsValid(); m_lBloc.MoveNext())
	{
		if(pItem == m_lBloc.GetItem())
		{
			m_lBloc.Remove();
			return true;
		}
	}
	return false;
}

JBool	CIBSegment::HasItems()
{
	CItemVisitor Visitor;
	return Visitor.HasItems(this);
}

JBool	CIBSegment::HasQuestions()
{
	CQuestionVisitor Visitor;
	return Visitor.HasQuestions(this);
}

CIBItem * 	CIBSegment::FindItem(JInt32 Itid)

{
	CItemVisitor Visitor;
	return Visitor.FindItem(this,Itid);
}
