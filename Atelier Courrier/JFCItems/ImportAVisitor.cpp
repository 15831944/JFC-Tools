#include "stdafx.h"
#include ".\importavisitor.h"
#include "ABuilder.h"
#include "ABItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CImportAVisitor::CImportAVisitor(void)
{
}

CImportAVisitor::~CImportAVisitor(void)
{
}


JBool CImportAVisitor::Import(CABuilder *pBuilder, JMap<JInt32, const IItem *> *pImpMap)
{
	m_pImpMap	=	pImpMap;
	if(pBuilder && pImpMap)
		pBuilder->Accept(*this);
	else
		return false;
	return true;
}

void CImportAVisitor::VisitStart(CABItem & Item)
{
	JList<JInt32> Values;
	JList<JInt32> ValuesO;

	JInt32 ID = Item.GetID();
	Item.GetValues(Values);

	// Check if stocked item type container is Palette, Classe or Quantile
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

