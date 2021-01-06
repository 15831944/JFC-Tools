// Pseudo.cpp: implementation of the CPseudo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pseudo.h"
#include "ItemsConstruitsBase.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPseudo::CPseudo(CItemsConstruitsBase *pItemsConstruits)
: m_pItemsConstruits(pItemsConstruits) 
{
	// Pseudo label
	CString Label;
	Label = "Items Construits";
	m_Label = Label;
	m_Items.Reset();

}

CPseudo::~CPseudo()
{

}

void	CPseudo::Reset()
{
	m_Items.Reset();
}

JBool	CPseudo::GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID, JInt32 Dim, JUnt32 ContingentMask)	const
{
	if( ModaliteID != 0 || Dim != 0 )
		return false;

	Vector.SetCount(m_Items.GetCount());
	for(m_Items.MoveFirst(), Vector.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext(), Vector.MoveNext())
	{
		Vector.GetItem() = (IModalite *)m_pItemsConstruits->GetItemConstruit(m_Items.GetItem())->AsIModalite();
	}
	return true;
}


JBool	CPseudo::IsValidTerme(const JVector<JInt32> & IDTerme)	const
{
	if(IDTerme.GetCount() != 1)
		return false;

	IDTerme.MoveFirst();
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(m_Items.GetItem() == IDTerme.GetItem())
			return true;
	}
	return false;
}

JBool	CPseudo::SetModaliteOrder(const JList<JInt32> & IDOrder)
{
	if(IDOrder.GetCount() != m_Items.GetCount())
		return false;

	JList<JInt32> Temp;

	for(IDOrder.MoveFirst(); IDOrder.IsValid(); IDOrder.MoveNext())
	{
		JBool Found = false;
		for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
		{
			if(IDOrder.GetItem() == m_Items.GetItem())
			{
				Temp.AddTail() = IDOrder.GetItem();
				Found = true;
				break;
			}
		}
		if(!Found)
			return false;
	}
	m_Items.Swap(Temp);

	return true;
}

// Check if the object is valid with the given terrain
JBool	CPseudo::IsValid(CTerrain * pTerrain) const
{
	const CItemConstruit * pItem = 0;

	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		// Get the item construit
		pItem = m_pItemsConstruits->GetItemConstruit(m_Items.GetItem());

		// Check if item exists and that the item is valid
		if(!pItem || !pItem->IsValid(pTerrain))
			return false;
	}
	return true;
}

JBool	CPseudo::AddItems(JList<JInt32> &VectID)
{
	for(VectID.MoveFirst(); VectID.IsValid(); VectID.MoveNext())
	{
		if(!AddItem(VectID.GetItem()))
			return false;
	}
	return true;
}

// Add the given item construit to the object else returns false
JBool	CPseudo::AddItem(JInt32 ID)
{
	const CItemConstruit * pItem = 0;
	// Get the item construit
	pItem = m_pItemsConstruits->GetItemConstruit(ID);

	if(pItem)
	{
		for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
		{
			if(m_Items.GetItem() == ID)
				return true;
		}
		m_Items.AddTail() = ID;
		return true;
	}
	return false;
}

JBool	CPseudo::RemoveItems(JList<JInt32> &VectID)
{
	for(VectID.MoveFirst(); VectID.IsValid(); VectID.MoveNext())
	{
		if(!RemoveItem(VectID.GetItem()))
			return false;
	}
	return true;
}

// Removes the given item construit from the object else returns false
JBool	CPseudo::RemoveItem(JInt32 ID)
{
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(m_Items.GetItem() == ID)
		{
			m_Items.Remove();
			return true;
		}
	}
	return false;
}

// Get current items construits
void	CPseudo::GetItems(JList<JInt32> &VectID) const
{
	VectID.Reset();
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
		VectID.AddTail() = m_Items.GetItem();
}

// Returns true if the item contains the given itemID
JBool	CPseudo::HasItem(JInt32 ID) const
{
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(ID == m_Items.GetItem())
			return true;
	}
	return false;
}

// Returns true if the item contains at least one itemID
JBool	CPseudo::HasItems() const
{
	return (m_Items.GetCount() > 0);
}
