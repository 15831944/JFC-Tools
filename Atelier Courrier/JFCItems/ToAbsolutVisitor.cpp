// ToAbsolutVisitor.cpp: implementation of the CToAbsolutVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToAbsolutVisitor.h"
#include "IBuilder.h"
#include "IBQuestion.h"
#include "IBItem.h"
#include "ABItem.h"
#include "ABNumValue.h"

#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToAbsolutVisitor::CToAbsolutVisitor()
{
	m_Valid			=	false;
	m_QID			=	0;
	m_pABuilder		=	0;
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurItem		=	0;
	m_pCurTerme		=	0;
	m_pTerrain		=	0;
	m_Operators.Reset();
	m_Elements.Reset();
}

CToAbsolutVisitor::~CToAbsolutVisitor()
{

}

// Construct an Absolut Builder from an Item Builder
JBool CToAbsolutVisitor::ToAbsolut(CIBuilder *pIBuilder, CABuilder *pABuilder, CTerrain * pTerrain)
{
	m_Valid			=	false;
	m_QID			=	0;
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurItem		=	0;
	m_pCurTerme		=	0;
	m_Operators.Reset();
	m_Elements.Reset();

	m_pTerrain = pTerrain;
	pIBuilder->Accept(*this);
	if(!m_Valid)
	{
		delete m_pABuilder;
		return false;
	}
	if(m_pABuilder)
	{
		if(m_pABuilder->Validate())
		{
			*pABuilder = *m_pABuilder;
			delete m_pABuilder;
			return true;
		}
		delete m_pABuilder;
	}
	return false;
}

void CToAbsolutVisitor::VisitStart(CIBuilder & Builder)
{
	m_Valid = true;
	m_pABuilder = new CABuilder;
	m_pABuilder->SetOperator(Builder.GetOperator()); 
}

void CToAbsolutVisitor::VisitEnd(CIBuilder & Builder)
{
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurTerme		=	0;
	m_pCurItem		=	0;
	m_Operators.Reset();
	m_Elements.Reset();
}

void CToAbsolutVisitor::VisitStart(CIBSegment & Segment)
{
	m_pCurSegment = new CABSegment;
	m_pCurSegment->SetOperator(Segment.GetOperator ());
}

void CToAbsolutVisitor::VisitEnd(CIBSegment & Segment)
{
	m_pABuilder->Add(m_pCurSegment);
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurItem		=	0;
	m_pCurTerme		=	0;
	m_Operators.Reset();
	m_Elements.Reset();
}

void CToAbsolutVisitor::VisitStart(CIBQuestion & Question)
{
	m_pCurQuestion = new CABQuestion;
	m_QID = Question.GetID();
	m_pCurQuestion->SetOperator(Question.GetOperator ());
}

void CToAbsolutVisitor::VisitEnd(CIBQuestion & Question)
{
	// Return if no segment
	if(m_pCurSegment)
	{
		// Create a Values vector with the elements size
		JVector<JInt32> Values;
		Values.SetCount(m_Elements.GetCount());
		Values.MoveFirst();
		// Get the first operator
		m_Operators.MoveFirst();
		m_pCurQuestion->SetOperator(m_Operators.GetItem());
		// Step to next operator
		m_Operators.MoveNext();
		// Step to first element
		m_Elements.MoveFirst();
		// Create the termes
		if(SetTermes(m_pCurQuestion, Values, 0))
		{
			// Add the question
			m_pCurSegment->Add(m_pCurQuestion);
		}
		else
			// delete the question
			delete m_pCurQuestion;
	}
	else
		// delete the question
		delete m_pCurQuestion;

	m_QID = 0;
	m_pCurQuestion= 0;
	m_Operators.Reset();
	m_Elements.Reset();
}

JBool CToAbsolutVisitor::SetTermes(ITerme *pTerme,JVector<JInt32> &Values ,JInt32 Index)
{
	// Get the list of ID's
	JList<JInt32> &Ids = m_Elements.GetItem();

	// Go through the Id's
	for(Ids.MoveFirst(); Ids.IsValid(); Ids.MoveNext())
	{
		// Move to the index of our value to change
		Values.MoveTo(Index);
		// Set the Id value
		Values.GetItem() = Ids.GetItem();
		// Create a new terme
		CABTerme * pT = new CABTerme;
		// Are there more elements
		if(m_Operators.IsValid())
		{
			// More elements

			// Set the operator for this terme
			pT->SetOperator(m_Operators.GetItem());

			// Step to next element
			m_Elements.MoveNext();
			// Step to next operator
			m_Operators.MoveNext();
			// Lets rock around the elements
			if(SetTermes(pT,Values,Index+1))
			{
				// Add the new terme
				pTerme->AddTerme(pT);
			}
			else
			{
				// terme invalid so delete
				delete pT;
				m_Valid = false;
				return false;
			}

			// move Back element
			m_Elements.MovePrev();
			
			// move Back operator
			if(!m_Operators.IsValid())
				m_Operators.MoveLast();
			else
				m_Operators.MovePrev();
		}
		else
		{
			// Add this terme
			JInt32 AbsID = m_pTerrain->GetRepAbsByTermes(m_QID,Values);
			// If the repere absolut is valid
			if(AbsID)
			{
				// Add the terme
				pT->SetID(AbsID);
				pTerme->AddTerme(pT);
			}
			else
			{
				// terme invalid so delete
				delete pT;
				m_Valid = false;
				return false;
			}
		}
	}
	return true;
}

void CToAbsolutVisitor::Visit(CIBModalite & Modalite)
{
	// Add the operator to the list
	m_Operators.AddTail() = Modalite.GetOperator();
	// Get the modalities and add them to the modalities list
	JList<JInt32> &Elements = m_Elements.AddTail();
	Modalite.GetValues(Elements);
}

void CToAbsolutVisitor::Visit(CIBNumValue & NumValue)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CABNumValue * pObject = new CABNumValue();
		// Get the operator
		pObject->SetOperator (NumValue.GetOperator());
		// Copy the values
		JList<JFlt32> Values;
		NumValue.GetValues(Values);
		pObject->SetValues(Values);
		// Add the object to the question
		m_pCurQuestion->SetNumValue(pObject);
	}
}

void CToAbsolutVisitor::Visit(CIBClass & Class)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CABClass * pObject = new CABClass();
		// Get the operator
		pObject->SetOperator(Class.GetOperator());
		// Copy the values
		JList<IBloc::OPERATOR> Operators;
		Class.GetOperators(Operators);
		pObject->SetOperators(Operators);
		// Copy the values
		JList<JFlt32> Values;
		Class.GetValues(Values);
		pObject->SetValues(Values);
		// Add the object to the question
		m_pCurQuestion->SetClass(pObject);
	}
}

void CToAbsolutVisitor::Visit(CIBPoint & Point)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CABPoint * pObject = new CABPoint();
		// Get the operator
		pObject->SetOperator(Point.GetOperator());
		// Copy the value
		pObject->SetValue(Point.GetValue());
		// Add the object to the question
		m_pCurQuestion->SetPoint(pObject);
	}
	else if(m_pCurItem)
	{
		// Create new object
		CABPoint * pObject = new CABPoint();
		// Get the operator
		pObject->SetOperator(Point.GetOperator());
		// Copy the value
		pObject->SetValue(Point.GetValue());
		// Add the object to the question
		m_pCurItem->SetPoint(pObject);
	}
}

void CToAbsolutVisitor::Visit(CIBScore & Score)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CABScore * pObject = new CABScore();
		// Get the operator
		pObject->SetOperator(Score.GetOperator());
		// Copy the values
		JList<JInt32> Values;
		Score.GetValues(Values);
		pObject->SetValues(Values);
		// Add the object to the question
		m_pCurQuestion->SetScore(pObject);
	}
}

void CToAbsolutVisitor::Visit(CIBQuant & Quant)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CABQuant * pObject = new CABQuant();
		// Get the operator
		pObject->SetOperator(Quant.GetOperator());
		// Copy the values
		JList<JInt32> Values;
		Quant.GetValues(Values);
		pObject->SetValues(Values);
		// Add the object to the question
		m_pCurQuestion->SetQuant(pObject);
	}
}

void CToAbsolutVisitor::VisitStart(CIBItem & Item)
{
	m_pCurItem = 0;
	if(m_pCurSegment)
	{
		// Create new object
		m_pCurItem = new CABItem();
		m_pCurItem->SetID(Item.GetID());
		// Get the operator
		m_pCurItem->SetOperator(Item.GetOperator());
		// Copy the values
		JList<JInt32> Values;
		Item.GetValues(Values);
		m_pCurItem->SetValues(Values);
	}
}

void CToAbsolutVisitor::VisitEnd(CIBItem & Item)
{
	if(m_pCurSegment && m_pCurItem)
		// Add the object to the question
		m_pCurSegment->Add(m_pCurItem);

	m_pCurItem = 0;
}
