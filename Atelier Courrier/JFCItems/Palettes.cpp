// Palettes.cpp: implementation of the CPalettes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Palettes.h"
#include "ItemsConstruitsBase.h"
#include "JMFCStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPalettes::CPalettes(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp)
:m_pItemsConstruits(pItemsConstruits)
{
	// Repere de la palette
	m_lID = ID;
	// Delete temporary items
	m_bDeleteTemp = bDeleteTemp;

	// Palettes label
	CString Label;
	Label.Format("Palette %d",m_lID);
	m_Label = Label;
}

CPalettes::~CPalettes()
{
	// Remove all temporary items
	if(m_bDeleteTemp)
	{
		for(m_TempItems.MoveFirst(); m_TempItems.IsValid(); m_TempItems.MoveNext())
		{
			delete m_TempItems.GetItem();
		}
		m_TempItems.Reset();
	}
}

// Load method
JArchive& CPalettes::Recv( JArchive& lhs, JInt32 Version )
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

			for(JInt32 x = 0 ; x < Count ; x++)
			{
				lhs.Recv(m_Items.AddTail());
			}
		}
		break;

		default:
			TRACE("CPalettes Version error");
			throw JInternalError::GetInstance(); //("CPalettes :: File Version error");
		break;
	}
	return lhs;
}

// Save method
JArchive& CPalettes::Send( JArchive& lhs ) const
{
	lhs.Send(m_lID);
	lhs.Send( m_lOwnerID );
	m_Label.Send(lhs);

	JInt32 Count = m_Items.GetCount();
	lhs.Send(Count);

	for (m_Items.MoveFirst(); m_Items.IsValid() ; m_Items.MoveNext())
	{
		// Récup item onstruit via l'Id item
		CItemConstruit * pItem = m_pItemsConstruits->GetItemConstruit(m_Items.GetItem());

		// save repere
		lhs.Send(m_Items.GetItem());
	}
	return lhs;
}

/// Gets the Palettes Items IDs
void	CPalettes::GetItems(JList<JInt32> &Items) const
{
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
		Items.AddTail() = m_Items.GetItem();
}

/// Gets the Palettes Item IDs
void	CPalettes::GetItems(JList<const CItemConstruit *> & Items) const
{
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(IsRegistered()) 
		{
			Items.AddTail() = m_pItemsConstruits->GetItemConstruit(m_Items.GetItem());
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_Items.GetItem());
			// return if not valid
			if(m_TempItems.IsValid())
				Items.AddTail() = m_TempItems.GetItem();
		}
	}
}

/// Gets the Palettes Item IDs
void CPalettes::GetItems(std::list<const CItemConstruit *> & Items) const
{
	Items.clear();
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		if(IsRegistered()) 
		{
			Items.push_back(m_pItemsConstruits->GetItemConstruit(m_Items.GetItem()));
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_Items.GetItem());
			// return if not valid
			if(m_TempItems.IsValid())
				Items.push_back(m_TempItems.GetItem());
		}
	}
}

/// Gets the Palettes Item IDs
void CPalettes::GetItems(std::list<const JInt32> & Items) const
{
	Items.clear();
	for(m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
		Items.push_back(m_Items.GetItem());
}

JBool	CPalettes::GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID, JInt32 Dim, JUnt32 ContingentMask)	const
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
	}
	return true;
}

JBool	CPalettes::IsValidTerme(const JVector<JInt32> & IDTerme)	const
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

JBool	CPalettes::SetModaliteOrder(const JList<JInt32> & IDOrder)
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

	// If this object is registered then make it dirty
	if(IsRegistered())
		m_pItemsConstruits->SetDirty();

	return true;
}

// Check if the object is valid with the given terrain
JBool	CPalettes::IsValid(CTerrain * pTerrain) const
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
		// Modif JUIN 2014 @@@@  pas la peine
		/*
		if(pItem)
		{
	
			CItemConstruit * pItemConstruit = (CItemConstruit *)(pItem->AsItemConstruit());
			if (pItem->GetIDSource() == 49 && pItem->GetIDTerrain() <= 514)
			{
				pItemConstruit->SetRebuildViaRepAbs(true);
			}
		}
		*/
	
		if(!pItem || !pItem->IsValid(pTerrain))
			return false;
	}
	return true;
}

// Add the given item construit to the object else returns false
JBool	CPalettes::AddItem(CItemConstruit * pItem, JFlt64 Coeff)
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
JBool	CPalettes::RemoveItem(JInt32 ID)
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
JBool	CPalettes::Register(JInt32 OwnerID, JMap<JInt32, const IItem *> &ImpMap, JInt32 ID)
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
JBool	CPalettes::Register(JInt32 OwnerID, JInt32 ID)
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
					// Set new item ID
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
JBool	CPalettes::CanUnRegister(JInt32 OwnerID, JMap<JInt32, JInt32> * pMapItems)
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
JBool	CPalettes::UnRegister(JInt32 OwnerID)
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

// Add the Item ID to the item list
JBool	CPalettes::AddID(JInt32 ID)
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
JBool	CPalettes::RemoveID(JInt32 ID)
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

// Makes a deep copy of this object
CPalettes  & CPalettes::DeepCopy(const CPalettes *rhs)
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

// Sets the Label of this object
JBool	CPalettes::SetLabel(const JLabel & Label, JBool bForce)
{
	if(Label.IsEmpty())
		return false;

	const CPalettes * pItem = 0;

	if(!bForce)
	{
		// Récupére l'item
		pItem = m_pItemsConstruits->GetPalette(Label);

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
JBool	CPalettes::HasItem(JInt32 ID) const
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

// Rename an Item
JBool CPalettes::RenameItem(JLabel &Label, JInt32 ItemID)
{
	if(!CheckLabel(Label))
		return false;

	CItemConstruit * pItem = 0;

	if(IsRegistered()) 
	{
		pItem = m_pItemsConstruits->GetItemConstruit(ItemID);
	}
	else
	{
		// Get first temporary item construit
		m_TempItems.MoveTo(ItemID);
		// return if not valid
		if(m_TempItems.IsValid())
			pItem = m_TempItems.GetItem();
	}
	if(pItem)
	{
		return pItem->SetLabel(Label,true);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Return false if Label exists already
JBool CPalettes::CheckLabel(JLabel &Label) const
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
		if(!pItem || pItem->GetLabel() == Label)
			return false;
	}
	return true;
}

/// Export the object via StdioFile
JBool	CPalettes::Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const
{
	// First check if we have already been exported
	ExpMap.MoveTo(m_lID);
	if(ExpMap.IsValid ())
		return true;// Already exported

	ExpMap.Add(m_lID) = true;

	for(m_Items.MoveFirst();m_Items.IsValid(); m_Items.MoveNext())
	{
		// We must export the owner first (Palette,Class,Quantile)
		ExpMap.MoveTo(m_Items.GetItem());
		// Has the owner been exported ?
		if(!ExpMap.IsValid())
		{
			// Export the owner
			IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_Items.GetItem());
			if(m_pIItem)
				m_pIItem->Export(File,ExpMap);
		}
	}

	// Export this item
	CString Buf;
	File.WriteString(_T("PALETTE\n"));				//	Write the Type Item

	File.WriteString(_T("1\n"));					//	Write the Version

	Buf.Format("%s\n",m_Label.AsJCharPtr());
	File.WriteString(Buf);							//	Write the Name
	Buf.Format("%d\n",m_lID);
	File.WriteString(Buf);							//	Write the ID
	Buf.Format("%d\n",m_lOwnerID); 
	File.WriteString(Buf);							//	Write the OwnerID
	Buf.Format("%d\n",m_Items.GetCount()); 
	File.WriteString(Buf);							//	Write the Number of items
	for(m_Items.MoveFirst();m_Items.IsValid(); m_Items.MoveNext())
	{
		Buf.Format("%d\n",m_Items.GetItem()); 
		File.WriteString(Buf);							//	Write the Item ID
	}

	return true;
}

/// Import the objects via StdioFile
JBool	CPalettes::Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap)
{
	// Export this item
	CString Buf;
	File.ReadString(Buf);							//	Read the version
	JInt32 Version = atol(Buf);
	
	switch(Version)
	{
		case 1:
		{
			// Lecture du nom de la palette
			File.ReadString(Buf);		
			m_Label = Buf;

			// Lecture de l'ident de la palette
			File.ReadString(Buf);			
			m_lID = atol(Buf);
			TRACE("Palette : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Lecture du OwnerID
			File.ReadString(Buf);
			m_lOwnerID = atol(Buf);
			m_lOwnerID = 0;

			// Lecture du nombre items composant la palette
			File.ReadString(Buf);						
			JInt32 NbItems = atol(Buf);
			for(int x = 0 ; x < NbItems; x++)
			{
				// Lecture ident de l'item de base
				File.ReadString(Buf);			
				JInt32 Item = atol(Buf);

				// Veifie dans le map composition item???
				ImpMap.MoveTo(Item);
				if(!ImpMap.IsValid())
				{
					return false;
				}
				else
				{
					// Ajout de l'item 
					AddItem((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()));
				}
			}

			// Ident
			JInt32 IdP = GetID();

			ImpMap.Add(GetID()) = this->AsIItem();

			JInt32 Id = m_pItemsConstruits->CheckExists(this);


			// Même Ident, on essaie de le reconstruire sur la base actuelle
			if(Id != 0 && ReplaceImportBy(Id,ImpMap))
			{
				delete this;
				return true;
			}

			if(m_lOwnerID <= CItemsConstruitsBase::SYSTEM_ID)
			{
				// Check the Item Name does not exist
				int x = 2;
				JLabel nom = m_Label;
				while(!SetLabel(nom))
				{
					CString str;
					str.Format("%s (%d)",m_Label.AsJCharPtr(),x++); 
					nom = str;
				}
				m_pItemsConstruits->RegisterPalette(this,ImpMap);
			}

		}
		break;

		default:
			return false;
	}
	return true;
}

// A FAIRE -- Voir si composants palette sint des composants quantitatifs
JBool CPalettes::IsPaletteQuantitative()
{
	// A priori palette non composé de question quantitative
	JBool PaletteQuant = false;

	for(m_Items.MoveFirst();m_Items.IsValid(); m_Items.MoveNext())
	{
		// Item constituant
		IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_Items.GetItem());
	}
	return PaletteQuant;
}

/// Export the object avec CArchive
JBool	CPalettes::Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const
{
	// First check if we have already been exported
	ExpMap.MoveTo(m_lID);
	if(ExpMap.IsValid ())
		return true;// Already exported

	ExpMap.Add(m_lID) = true;

	for(m_Items.MoveFirst();m_Items.IsValid(); m_Items.MoveNext())
	{
		// Composants
		JUnt32 IdItemCompo = m_Items.GetItem();

		// We must export the owner first (Palette,Class,Quantile)
		ExpMap.MoveTo(m_Items.GetItem());
		// Has the owner been exported ?
		if(!ExpMap.IsValid())
		{
			// Export the owner
			IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_Items.GetItem());
			if(m_pIItem)
				m_pIItem->Export(Archive,ExpMap);
		}
	}

	// Export this item
	JLabel Text;

	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);

	// Buffer temporaire
	CString Buf;

	// Write the Type Item
	Text = "PALETTE";
	Text.Send(*pstream);  
	
	//	Write the Version
	Text = "1";
	Text.Send(*pstream);  
		
	//	Write the Name
	Buf.Format("%s",m_Label.AsJCharPtr());
	Text = Buf;
	Text.Send(*pstream);  

	//	Write the ID
	Buf.Format("%d",m_lID);
	Text = Buf;
	Text.Send(*pstream);  

	//	Write the OwnerID
	Buf.Format("%d",m_lOwnerID); 
	Text = Buf;
	Text.Send(*pstream);  

	//	Write the Number of items
	Buf.Format("%d",m_Items.GetCount()); 
	Text = Buf;
	Text.Send(*pstream);  

	for(m_Items.MoveFirst();m_Items.IsValid(); m_Items.MoveNext())
	{
		//	Write the Item ID
		Buf.Format("%d",m_Items.GetItem()); 
		Text = Buf;
		Text.Send(*pstream);  
	}

	delete pstream;
	return true;
}

/// Import the objects avec CArchive
JBool	CPalettes::Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap)
{
	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);

	// Export this item
	JLabel TxtLabel;

	// Read the version
	TxtLabel.Recv(*pstream);
	JInt32 Version = atol(TxtLabel.AsJCharPtr());
	
	switch(Version)
	{
		case 1:
		{
			// Read the Name
			TxtLabel.Recv(*pstream);
			m_Label = TxtLabel.AsJCharPtr();

			// Read the ID
			TxtLabel.Recv(*pstream);
			m_lID = atol(TxtLabel.AsJCharPtr());

			TRACE("Palette : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Read the OwnerID
			TxtLabel.Recv(*pstream);
			m_lOwnerID = atol(TxtLabel.AsJCharPtr());
			m_lOwnerID = 0;    // ????

			// Read the Source ID
			TxtLabel.Recv(*pstream);
			JInt32 NbItems = atol(TxtLabel.AsJCharPtr());

			for(int x = 0 ; x < NbItems; x++)
			{
				//	Read the item ID
				TxtLabel.Recv(*pstream);
				JInt32 Item = atol(TxtLabel.AsJCharPtr());

				// Vérifie existence
				ImpMap.MoveTo(Item);
				if(!ImpMap.IsValid())
				{
					delete pstream;
					return false;
				}
				AddItem((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()));
			}

			ImpMap.Add(GetID()) = this->AsIItem();

			JInt32 Id = m_pItemsConstruits->CheckExists(this);

			// Même Ident, on essaie de le reconstruire sur la base actuelle
			if(Id != 0 && ReplaceImportBy(Id,ImpMap))
			{
				delete pstream;
				delete this;
				return true;
			}

			if(m_lOwnerID <= CItemsConstruitsBase::SYSTEM_ID)
			{
				// Check the Item Name does not exist
				int x = 2;
				JLabel nom = m_Label;
				while(!SetLabel(nom))
				{
					CString str;
					str.Format("%s (%d)",m_Label.AsJCharPtr(),x++); 
					nom = str;
				}
				m_pItemsConstruits->RegisterPalette(this,ImpMap);
			}
		}
		break;

		default:
			delete pstream;
			return false;
	}
	delete pstream;
	return true;
}

///	Check if the Absoloute definitions are the same
JBool	CPalettes::IsSimilaire(const CPalettes *pRight) const
{
	JList<const CItemConstruit *> ItemLeft;
	JList<const CItemConstruit *> ItemRight;

	GetItems(ItemLeft);
	pRight->GetItems(ItemRight);

	if(ItemLeft.GetCount() != ItemRight.GetCount())
		return false;


	int x = 1;
	for(ItemLeft.MoveFirst(); ItemLeft.IsValid(); ItemLeft.MoveNext(), ++x)
	{
		const CItemConstruit * pLeft = ItemLeft.GetItem();
		for(ItemRight.MoveFirst(); ItemRight.IsValid(); ItemRight.MoveNext())
		{
			if(pLeft->IsSimilaire(ItemRight.GetItem()))
			{
				ItemRight.Remove();
				break;
			}
		}
		if(ItemLeft.GetCount() != (ItemRight.GetCount()+x))
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Reconstruction de la palette origine
JBool CPalettes::ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap)
{
	const CPalettes * pItem = m_pItemsConstruits->GetPalette(Id);

	if(!pItem)
		return false;

	if (pItem->GetLabel() != this->GetLabel())
		return (false);

	// Les items palette origine
	JList<const CItemConstruit *> ItemLeft;
	GetItems(ItemLeft);

	// Les items palette reconstruite
	JList<const CItemConstruit *> ItemRight;
	pItem->GetItems(ItemRight);

	int x = 1;
	for(ItemLeft.MoveFirst(); ItemLeft.IsValid(); ItemLeft.MoveNext(), ++x)
	{
		const CItemConstruit * pLeft = ItemLeft.GetItem();
		for(ItemRight.MoveFirst(); ItemRight.IsValid(); ItemRight.MoveNext())
		{
			const CItemConstruit * pRight = ItemRight.GetItem();

			// Libellé gauche et droite
			const JLabel LRight = pRight->GetLabel();
			const JLabel LLeft  = pLeft->GetLabel();

			// Test au moins le début du libellé // ???? pas top !!!!!
			if(LRight.AsJCharPtr()[0] == LLeft.AsJCharPtr()[0] && pRight->IsSimilaire(pLeft))
			{
				// Ajoute élment au map
				ImpMap.MoveTo(pLeft->GetID());
				if(ImpMap.IsValid())
				{
					// Ajoute élmt item valide
					ImpMap.GetItem() = pRight; 
				}
				else
					return false;
				ItemRight.Remove();
				break;
			}
		}
		if(ItemLeft.GetCount() != (ItemRight.GetCount()+x))
			return false;
	}

	ImpMap.MoveTo(GetID());
	if(!ImpMap.IsValid())
		return false;
	
	ImpMap.GetItem() = pItem;

	return true;
}

