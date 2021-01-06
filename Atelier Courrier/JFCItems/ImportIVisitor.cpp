#include "stdafx.h"
#include ".\importivisitor.h"
#include "IBuilder.h"
#include "IBItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CImportIVisitor::CImportIVisitor(void)
{
	m_pImpMap = 0;
}

CImportIVisitor::~CImportIVisitor(void)
{
}

JBool CImportIVisitor::Import(CIBuilder *pBuilder, JMap<JInt32, const IItem *> *pImpMap)
{
	m_pImpMap	=	pImpMap;
	if(pBuilder && pImpMap)
		pBuilder->Accept(*this);
	else
		return false;
	return true;
}

void CImportIVisitor::VisitStart(CIBItem & Item)
{
	JList<JInt32> Values;
	JList<JInt32> ValuesO;

	JInt32 ID = Item.GetID();
	Item.GetValues(Values);

	if(ID != 0)
	{
		m_pImpMap->MoveTo(ID);
		if(m_pImpMap->IsValid())
		{
			const IItem * pItem = m_pImpMap->GetItem();
			if(pItem)
				Item.SetID(pItem->GetID());
		}
	}

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		ID = Values.GetItem();

		m_pImpMap->MoveTo(ID);
		if(m_pImpMap->IsValid())
		{
			const IItem * pItem = m_pImpMap->GetItem();
			if(pItem)
				ValuesO.AddTail() = pItem->GetID();
			else
				ValuesO.AddTail() = Values.GetItem();
		}
		else
			ValuesO.AddTail() = Values.GetItem();
	}
	Item.SetValues (ValuesO);
}

