// CritereScore.cpp: implementation of the CCritereScore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CritereScore.h"
#include "ItemsConstruitsBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCritereScore::CCritereScore(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp)
:m_pItemsConstruits(pItemsConstruits)
{
	// Repere de la CritereScore
	m_lID = ID ;
	// Index de rangement
	m_lIndex = 0;
	// CritereScore label
	CString Label;
	Label.Format("CritereScore %d",m_lID);
	m_Label = Label;

	m_bDeleteTemp = bDeleteTemp;
}

CCritereScore::~CCritereScore()
{
	// Remove all temporary items
	if(m_bDeleteTemp)
	{
		for(m_TempItems.MoveFirst(); m_TempItems.IsValid(); m_TempItems.MoveNext())
		{
			delete m_TempItems.GetItem();
		}
	}
}

// Load method
JArchive& CCritereScore::Recv( JArchive& lhs, JInt32 Version )
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_lID);
			lhs.Recv( m_lOwnerID );
			m_Label.Recv(lhs);

			JInt32 Count;
			lhs.Recv(Count);

			for( JInt32 x = 0 ; x < Count ; x++)
			{
				lhs.Recv(m_Items.AddTail());
			}
		}
		break;

		default:
			TRACE("CCritereScore :: File Version error");
			throw JInternalError::GetInstance(); //("CCritereScore :: File Version error");
		break;
	}
	return lhs;
}

// Save method
JArchive& CCritereScore::Send( JArchive& lhs ) const
{
	lhs.Send(m_lID);
	lhs.Send( m_lOwnerID );
	m_Label.Send(lhs);

	JInt32 Count = m_Items.GetCount();
	lhs.Send(Count);

	for( m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		lhs.Send(m_Items.GetItem());
	}
	return lhs;
}

JBool	CCritereScore::GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID, JInt32 Dim, JUnt32 ContingentMask)	const
{
	if( ModaliteID != 0 || Dim != 0 )
		return false;

	Vector.SetCount(m_Items.GetCount());
	for(m_Items.MoveFirst(), Vector.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext(), Vector.MoveNext())
	{
		if(IsRegistered()) 
		{
			Vector.GetItem() = (IModalite *) m_pItemsConstruits->GetItemConstruit(m_Items.GetItem())->AsIModalite();
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_Items.GetItem());
			// return if not valid
			if(!m_TempItems.IsValid())
				return false;
			Vector.GetItem() = m_TempItems.GetItem()->AsIModalite();
		}
	}	return true;
}

JBool	CCritereScore::IsValidTerme(const JVector<JInt32> & IDTerme)	const
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

// Check if the object is valid with the given terrain
JBool	CCritereScore::IsValid(CTerrain * pTerrain) const
{
	const CItemConstruit * pItem = 0;

	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(IsRegistered()) 
		{
			// Get the item construit
			pItem = m_pItemsConstruits->GetItemConstruit(m_Items.GetItem());
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_Items.GetItem());
			// return if not valid
			if(!m_TempItems.IsValid())
				return false;
			pItem = m_TempItems.GetItem();
		}

		// Check if item exists and that the item is valid
		if(!pItem || !pItem->IsValid(pTerrain))
			return false;
	}
	return true;
}

// Add the given item construit to the object else returns false
JBool	CCritereScore::AddItem(CItemConstruit * pItem, JFlt64 Coeff)
{
	if(IsRegistered() || pItem->IsRegistered())
		return false;

	if(!pItem->IsValid())
		return false;

	if(AddID(pItem->GetID()))
	{
		m_TempItems.Add(pItem->GetID()) = pItem;
	}
	return true;
}

// Removes the given item construit from the object else returns false
JBool	CCritereScore::RemoveItem(JInt32 ID)
{
	if(IsRegistered())
	{
		for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
		{
			if(m_Items.GetItem() == ID && m_pItemsConstruits->UnRegisterItemConstruit(ID, m_lID))
			{
				RemoveID(ID);
				return true;
			}
		}
		return false;
	}

	if(RemoveID(ID))
	{
		m_TempItems.MoveTo(ID);

		if(m_TempItems.IsValid())
		{
			if(m_bDeleteTemp)
				delete m_TempItems.GetItem();

			m_TempItems.Remove();
		}
	}
	return true;
}

/// Registers the object to the Owner ID with the new ID
JBool	CCritereScore::Register(JInt32 OwnerID, JMap<JInt32, const IItem *> &ImpMap, JInt32 ID)
{
	// Check if not already registered
	if(!IsRegistered())
	{
		if(OwnerID != 0 && ID != 0)
		{
			m_lOwnerID	= OwnerID;
			m_lID		= ID;

			for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
			{
				m_TempItems.MoveTo(m_Items.GetItem());
				if(m_TempItems.IsValid())
				{
					// Get the item construit
					CItemConstruit * pItem = m_TempItems.GetItem();
					// Register the item to this object
					const CItemConstruit * pRItem = m_pItemsConstruits->RegisterItemConstruit(pItem, ImpMap, m_lID);
					// Set new item ID
					if(pRItem)
						m_Items.GetItem() = pRItem->GetID();
					else
						m_Items.GetItem() = 0;
				}
			}
			m_TempItems.Reset();
			return true;
		}
	}
	return false;
}

// Registers the object to the Owner ID with the new ID
JBool	CCritereScore::Register(JInt32 OwnerID, JInt32 ID)
{
	// Check if not already registered
	if(!IsRegistered())
	{
		if(OwnerID != 0 && ID != 0)
		{
			m_lOwnerID	= OwnerID;
			m_lID		= ID;

			for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
			{
				m_TempItems.MoveTo(m_Items.GetItem());
				if(m_TempItems.IsValid())
				{
					// Get the item construit
					CItemConstruit * pItem = m_TempItems.GetItem();
					// Register the item to this object
					const CItemConstruit * pRItem = m_pItemsConstruits->RegisterItemConstruit(pItem, m_lID);
					if(pRItem)
						m_Items.GetItem() = pRItem->GetID();
					else
						m_Items.GetItem() = 0;
					if(m_bDeleteTemp)
						delete pItem;

				}
			}
			m_TempItems.Reset();
			m_pItemsConstruits->SetDirty();
			return true;
		}
	}
	return false;
}

// tests if the caller can unregister the object
JBool	CCritereScore::CanUnRegister(JInt32 OwnerID, JMap<JInt32, JInt32> * pMapItems)
{
	if(IsRegistered())
	{
		if(m_lOwnerID == OwnerID)
		{
			JBool Ret = true;
			for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
			{
				// CanUnRegister the item from this object
				if(!m_pItemsConstruits->CanUnRegisterItemConstruit(m_Items.GetItem(), 0, m_lID))
				{
					if(pMapItems)
					{
						pMapItems->MoveTo(m_Items.GetItem());
						if(!pMapItems->IsValid())
							pMapItems->Add(m_Items.GetItem()) = 1;
						else
							pMapItems->GetItem() += 1;
					}
					Ret = false;
				}
			}
			return Ret;
		}
	}
	return false;
}

// Unregisters the object
JBool	CCritereScore::UnRegister(JInt32 OwnerID)
{
	if(IsRegistered() && CanUnRegister(OwnerID))
	{
		if(m_lOwnerID == OwnerID)
		{
			for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
			{
				// UnRegister the item from this palette
				m_pItemsConstruits->UnRegisterItemConstruit(m_Items.GetItem(), m_lID);
			}
			m_lOwnerID = -1;
			m_pItemsConstruits->SetDirty();
			return true;
		}
	}
	return false;
}

// Makes a deep copy of this object
CCritereScore  & CCritereScore::DeepCopy(const CCritereScore *rhs)
{
	// Must delete temporary objects
	m_bDeleteTemp = true;
	// Label
	m_Label		=	rhs->m_Label ;

	// Copy Items
	m_Items.Reset();
	m_TempItems.Reset();

	for(rhs->m_Items.MoveFirst(); rhs->m_Items.IsValid(); rhs->m_Items.MoveNext())
	{
		// Move to the item
		rhs->m_TempItems.MoveTo(rhs->m_Items.GetItem());

		// If the item is valid copy it
		if(rhs->m_TempItems.IsValid())
		{
			// Add the items id
			const CItemConstruit * Orig = rhs->m_TempItems.GetItem();
			CItemConstruit * pItem = m_pItemsConstruits->GetNewItemConstruit(Orig);
			if(AddID(pItem->GetID()))
			{
				m_TempItems.Add(pItem->GetID()) = pItem;
			}
		}
	}
	return *this;
}

// Add the Item ID to the item list
JBool	CCritereScore::AddID(JInt32 ID)
{
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(m_Items.GetItem() == ID)
			return false;
	}
	m_Items.AddTail() = ID;
	return true;
}

// Removes the item ID from the item list
JBool	CCritereScore::RemoveID(JInt32 ID)
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

// Sets the Label of this object
JBool	CCritereScore::SetLabel(const JLabel & Label, JBool bForce)
{
	if(Label.IsEmpty())
		return false;

	const CCritereScore * pItem = 0;

	if(!bForce)
	{
		pItem = m_pItemsConstruits->GetCritereScore(Label);

		// There is an object with the same label
		if(pItem)
		{
			// Make sure it's not me
			if(pItem == this)
				return true;
			else
				return false;
		}
	}

	m_Label = Label;

	if(IsRegistered())
		m_pItemsConstruits->SetDirty();

	return true;
}

// Returns true if the item contains the given itemID
JBool	CCritereScore::HasItem(JInt32 ID) const
{
	const CItemConstruit * pItem = 0;

	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(IsRegistered()) 
		{
			// Get the item construit
			pItem = m_pItemsConstruits->GetItemConstruit(m_Items.GetItem());
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_Items.GetItem());
			// return if not valid
			if(!m_TempItems.IsValid())
				continue;
			pItem = m_TempItems.GetItem();
		}

		// Check if item exists and that the item contains the id
		if(!pItem)
			continue;

		if(pItem->HasItem(ID))
			return true;
	}
	return false;
}
