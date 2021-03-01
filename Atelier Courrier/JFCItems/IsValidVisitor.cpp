// IsValidVisitor.cpp: implementation of the CIsValidVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IsValidVisitor.h"
#include "ABuilder.h"
#include "ABQuestion.h"
#include "ABItem.h"
#include "ABNumValue.h"
#include "ItemsConstruitsBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIsValidVisitor::CIsValidVisitor()
{
	m_pItems	=	0;
	m_pTerrain	=	0;
	m_Valid		=	true;
}

CIsValidVisitor::~CIsValidVisitor()
{

}

JBool CIsValidVisitor::Validate(CABuilder *pBuilder, CTerrain * pTerrain, CItemsConstruitsBase *pItemConst)
{
	m_pTerrain	=	pTerrain ;
	m_pItems	=	pItemConst ;
	m_Valid		=	true;
	pBuilder->Accept(*this);
	return m_Valid ;
}

void CIsValidVisitor::VisitStart(CABTerme & Terme)
{
	if(m_Valid)
	{
		if(!Terme.HasOperator())
		{
			// Does this repere exist for this terrain
			//if(!m_pTerrain->HasRepere(Terme.GetID()))
			if(m_pTerrain== NULL || !m_pTerrain->HasRepere(Terme.GetID()))
				m_Valid = false;
		}
	}
}

void CIsValidVisitor::VisitStart(CABItem & Item)
{
	if(m_Valid)
	{
		JList<JInt32> Values;
		Item.GetValues(Values);

		for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
		{
			const CItemConstruit * pItem = m_pItems->GetItemConstruit(Values.GetItem());
			if(!pItem || !pItem->IsValid(m_pTerrain))
			{
				m_Valid = false;
				break;
			}
		}
	}
}
