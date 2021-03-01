// ToItemVisitor.cpp: implementation of the CToItemVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToItemVisitor.h"
#include "ABuilder.h"
#include "ABQuestion.h"
#include "ABItem.h"
#include "ABNumValue.h"
#include "IBItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToItemVisitor::CToItemVisitor()
{
	m_pTerrain		=	0;
	m_pCurSegment	=	0;
	m_pCurItem		=	0;
	m_pCurQuestion	=	0;
	m_pIBuilder		=	0;
	m_Operators.Reset();
	m_Elements.Reset();
	m_bError = true;
}

CToItemVisitor::~CToItemVisitor()
{

}

// Construct an Item Builder from an Absolut Builder
JBool CToItemVisitor::ToItem(CABuilder *pABuilder, CIBuilder * pIBuilder, CTerrain * pTerrain)
{
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurItem		=	0;
	m_pIBuilder		=	0;
	m_bError		=	true;
	m_Operators.Reset();
	m_Elements.Reset();
	m_pTerrain = pTerrain;

	pABuilder->Accept(*this);
	if(m_pIBuilder)
	{
		if(m_bError && m_pIBuilder->Validate())
		{
			*pIBuilder = *m_pIBuilder;
			delete m_pIBuilder;
			return true;
		}
		delete m_pIBuilder;
	}
	return false;
}

void CToItemVisitor::VisitStart(CABuilder & Builder)
{
	m_pIBuilder = new CIBuilder;
	m_pIBuilder->SetOperator(Builder.GetOperator()); 
}

void CToItemVisitor::VisitEnd(CABuilder & Builder)
{
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurItem		=	0;
	m_Operators.Reset();
	m_Elements.Reset();
}

void CToItemVisitor::VisitStart(CABSegment & Segment)
{
	m_pCurSegment = new CIBSegment;
	m_pCurSegment->SetOperator(Segment.GetOperator ());
}

void CToItemVisitor::VisitEnd(CABSegment & Segment)
{
	m_pIBuilder->Add(m_pCurSegment);
	m_pCurSegment	=	0;
	m_pCurQuestion	=	0;
	m_pCurItem		=	0;
}

void CToItemVisitor::VisitStart(CABQuestion & Question)
{
	m_pCurQuestion = new CIBQuestion;
	m_Operators.AddTail() = Question.GetOperator();
	m_Elements.AddTail();
	m_OperatorLevel = 1;
	m_QID = 0;
}

void CToItemVisitor::VisitEnd(CABQuestion & Question)
{
	// Return if no segment
	if(m_pCurSegment && m_QID)
	{
		// Sets the Question ID
		m_pCurQuestion->SetID(m_QID);
		// Get the question
		const CQuestion  *	pQuestion = m_pTerrain->GetQuestionByID(m_QID);
		if(pQuestion)
		{
			m_Elements.MoveFirst();
			m_Operators.MoveFirst();

			JInt32 NbDim = pQuestion->HasDimensions();

			for(JInt32 x = 0; x < NbDim, m_Elements.IsValid(),m_Operators.IsValid(); x++, m_Elements.MoveNext(), m_Operators.MoveNext())
			{
				CIBModalite * pObject = new CIBModalite;
				JList<JInt32> Values;
				JMap<JInt32,JBool> &map = m_Elements.GetItem();
				for(map.MoveFirst(); map.IsValid(); map.MoveNext())
				{
					Values.AddTail() = map.GetKey();
				}
				pObject->SetValues(Values);
				pObject->SetOperator(m_Operators.GetItem()); 
				m_pCurQuestion->AddModalite(pObject); 
			}

			m_pCurSegment->Add(m_pCurQuestion); 
		}
		else
			delete m_pCurQuestion;
	}
	else
		// delete the question
		delete m_pCurQuestion;

	m_QID = 0;
	m_pCurQuestion	= 0;
	m_Operators.Reset();
	m_Elements.Reset();
}

void CToItemVisitor::VisitStart(CABTerme & Terme)
{
	if(Terme.HasOperator())
	{
		if(m_OperatorLevel == m_Operators.GetCount())
		{
			m_Operators.AddTail() = Terme.GetOperator();
			m_Elements.AddTail();
		}
		m_OperatorLevel++;
	}
	else
	{
		// Gets the termes and the question from the repere ID
		JVector<JInt32> Termes;
		JInt32 QID = m_pTerrain->GetTermesByRepAbs(Terme.GetID(),Termes);
		if(m_QID == 0 || m_QID == QID)
		{
			m_QID = QID;
			// Adds all the termes to the element maps
			if(m_Elements.GetCount() == Termes.GetCount())
			{
				for(m_Elements.MoveFirst(), Termes.MoveFirst(); m_Elements.IsValid(); m_Elements.MoveNext(), Termes.MoveNext())
				{
					JMap<JInt32,JBool> &map = m_Elements.GetItem();
					map.MoveTo(Termes.GetItem());
					if(!map.IsValid())
						map.Add(Termes.GetItem()) = true;
				}
			}
			else
				m_bError = false;
		}
		else
			m_bError = false;
	}
}

void CToItemVisitor::VisitEnd(CABTerme & Terme)
{
	if(Terme.HasOperator())
	{
		m_OperatorLevel--;
	}
}

void CToItemVisitor::Visit(CABNumValue & NumValue)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CIBNumValue * pObject = new CIBNumValue();
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

void CToItemVisitor::Visit(CABClass & Class)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CIBClass * pObject = new CIBClass();
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

void CToItemVisitor::Visit(CABPoint & Point)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CIBPoint * pObject = new CIBPoint();
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
		CIBPoint * pObject = new CIBPoint();
		// Get the operator
		pObject->SetOperator(Point.GetOperator());
		// Copy the value
		pObject->SetValue(Point.GetValue());
		// Add the object to the question
		m_pCurItem->SetPoint(pObject);
	}
}

void CToItemVisitor::Visit(CABScore & Score)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CIBScore * pObject = new CIBScore();
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

void CToItemVisitor::Visit(CABQuant & Quant)
{
	// Return if not question
	if(m_pCurQuestion)
	{
		// Create new object
		CIBQuant * pObject = new CIBQuant();
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

void CToItemVisitor::VisitStart(CABItem & Item)
{
	m_pCurItem = 0;
	if(m_pCurSegment)
	{
		// Create new object
		m_pCurItem = new CIBItem();
		m_pCurItem->SetID(Item.GetID());
		// Get the operator
		m_pCurItem->SetOperator(Item.GetOperator());
		// Copy the values
		JList<JInt32> Values;
		Item.GetValues(Values);
		m_pCurItem->SetValues(Values);
	}
}

void CToItemVisitor::VisitEnd(CABItem & Item)
{
	if(m_pCurSegment && m_pCurItem)
	{
		// Add the object to the question
		m_pCurSegment->Add(m_pCurItem);
	}
	m_pCurItem = 0;
}
