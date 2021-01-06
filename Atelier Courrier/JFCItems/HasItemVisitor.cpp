// HasItemVisitor.cpp: implementation of the CHasItemVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HasItemVisitor.h"
#include "IBItem.h"
#include "ItemsConstruitsBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHasItemVisitor::CHasItemVisitor()
{
	m_ItemID	=	0;
	m_pItems	=	0;
	m_Valid		=	false;
}

CHasItemVisitor::~CHasItemVisitor()
{

}

JBool CHasItemVisitor::HasItem(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst,JInt32 ItemID)
{
	m_ItemID	=	ItemID;
	m_pItems	=	pItemConst;
	m_Valid		=	false;
	pBuilder->Accept(*this);
	return m_Valid ;
}

void CHasItemVisitor::VisitStart(CIBItem & Item)
{
	if(!m_Valid)
	{
		if(m_ItemID == Item.GetID())
			m_Valid = true;
		else
		{
			JList<JInt32> Values;
			Item.GetValues(Values);

			for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
			{
				if(m_ItemID == Values.GetItem())
				{
					m_Valid = true;
					break;
				}

				const CItemConstruit * pItem = m_pItems->GetItemConstruit(Values.GetItem());
				if(pItem && pItem->HasItem(m_ItemID))
				{
					m_Valid = true;
					break;
				}
			}
		}
	}
}
