// CritereQuantile.cpp: implementation of the CCritereQuantile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CritereQuantile.h"
#include "ItemsConstruits.h"
#include "JMFCStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCritereQuantile::CCritereQuantile(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp)
:CPalettes(pItemsConstruits,ID,bDeleteTemp)
{
	// Question Bloc ID
	m_lBlocID = 0;
	// CritereQuantile label
	CString Label;
	Label.Format("CritereQuantile %d",m_lID);
	m_Label = Label;
}

CCritereQuantile::~CCritereQuantile()
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
JArchive& CCritereQuantile::Recv( JArchive& lhs, JInt32 Version )
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_lID);
			lhs.Recv( m_lOwnerID );
			lhs.Recv(m_lBlocID);
			m_Label.Recv(lhs);

			JInt32 Count;
			lhs.Recv(Count);

			for(JInt32 x = 0; x < Count ; x++)
			{
				lhs.Recv(m_Items.AddTail());
			}
		}
		break;

		default:
			TRACE("CCritereQuantile :: File Version error");
			throw JInternalError::GetInstance(); //("CCritereQuantile :: File Version error");
		break;
	}
	return lhs;
}

// Save method
JArchive& CCritereQuantile::Send( JArchive& lhs ) const
{
	lhs.Send(m_lID);
	lhs.Send( m_lOwnerID );
	lhs.Send(m_lBlocID);
	m_Label.Send(lhs);
	JInt32 Count = m_Items.GetCount();
	lhs.Send(Count);

	for( m_Items.MoveFirst(); m_Items.IsValid(); m_Items.MoveNext())
	{
		lhs.Send(m_Items.GetItem());
	}
	return lhs;
}

// Makes a deep copy of this object
CCritereQuantile  & CCritereQuantile::DeepCopy(const CCritereQuantile *rhs)
{
	// Must delete temporary objects
	m_bDeleteTemp = true;
	// Label
	m_Label		=	rhs->m_Label ;
	// Question Bloc ID
	m_lBlocID	=	rhs->m_lBlocID;

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
JBool	CCritereQuantile::SetLabel(const JLabel & Label, JBool bForce)
{
	if(Label.IsEmpty())
		return false;

	const CCritereQuantile * pItem = 0;

	if(!bForce)
	{
		pItem = m_pItemsConstruits->GetCritereQuantile(Label);

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

/// Export the object via StdioFile
JBool	CCritereQuantile::Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const
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
	File.WriteString(_T("QUANTILE\n"));				//	Write the Type Item
	File.WriteString(_T("1\n"));					//	Write the Version
	Buf.Format("%s\n",m_Label.AsJCharPtr());
	File.WriteString(Buf);							//	Write the Name
	Buf.Format("%d\n",m_lID);
	File.WriteString(Buf);							//	Write the ID
	Buf.Format("%d\n",m_lOwnerID); 
	File.WriteString(Buf);							//	Write the OwnerID
	Buf.Format("%d\n",m_lBlocID);
	File.WriteString(Buf);							//	Write the BlocID
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
JBool	CCritereQuantile::Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap)
{
	// Export this item
	CString Buf;
	File.ReadString(Buf);							//	Read the version
	JInt32 Version = atol(Buf);
	
	switch(Version)
	{
		case 1:
		{
			File.ReadString(Buf);							//	Read the Name
			m_Label = Buf;
			File.ReadString(Buf);							//	Read the ID
			m_lID = atol(Buf);
			TRACE("Quantile : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);
			File.ReadString(Buf);							//	Read the OwnerID
			m_lOwnerID = atol(Buf);
			m_lOwnerID = 0;
			File.ReadString(Buf);							//	Read the bloc id
			m_lBlocID = atol(Buf);
			File.ReadString(Buf);							//	Read the Source ID
			JInt32 NbItems = atol(Buf);
			for(int x = 0 ; x < NbItems; x++)
			{
				File.ReadString(Buf);						//	Read the Source ID
				JInt32 Item = atol(Buf);
				ImpMap.MoveTo(Item);
				if(!ImpMap.IsValid())
					return false;
				AddItem((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()));
			}

			ImpMap.Add(GetID()) = this->AsIItem();

			JInt32 Id = m_pItemsConstruits->CheckExists(this);
			if(Id != 0 && ReplaceImportBy(Id,ImpMap))
			{
				delete this;
				return true;
			}

			if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
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
				m_pItemsConstruits->RegisterCritereQuantile(this,ImpMap);
			}
		}
		break;

		default:
			return false;
	}
	return true;
}

/// Export the object via CArchive
JBool	CCritereQuantile::Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const
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
				m_pIItem->Export(Archive,ExpMap);
		}
	}

	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);
	
	// Export this item
	JLabel Text;
	CString Buf;

	//	Write the Type Item
	Text = "QUANTILE";
	Text.Send(*pstream); 

	//	Write the Version
	Text = "1";
	Text.Send(*pstream); 
	
	//	Write the Name
	Text = m_Label.AsJCharPtr();
	Text.Send(*pstream); 

	//	Write the ID
	Buf.Format("%d",m_lID);
	Text = Buf;
	Text.Send(*pstream); 
	
	//	Write the OwnerID
	Buf.Format("%d",m_lOwnerID); 
	Text = Buf;
	Text.Send(*pstream); 

	//	Write the BlocID
	Buf.Format("%d",m_lBlocID);
	Text = Buf;
	Text.Send(*pstream); 

	//	Write the Number of items
	Buf.Format("%d",m_Items.GetCount()); 
	Text = Buf;
	Text.Send(*pstream); 

	// Boucle sur les items	
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

/// Import the objects via CArchive
JBool	CCritereQuantile::Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap)
{
	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);

	// Export this item
	JLabel Text;
	CString Buf;

	// Read the version
	Text.Recv(*pstream);							
	JInt32 Version = atol(Text.AsJCharPtr());
	
	switch(Version)
	{
		case 1:
		{
			// Read the Name
			Text.Recv(*pstream);
			m_Label = Text;

			// Read the ID
			Text.Recv(*pstream);
			m_lID = atol(Text.AsJCharPtr());
			TRACE("Quantile : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Read the OwnerID
			Text.Recv(*pstream);
			m_lOwnerID = atol(Text.AsJCharPtr());
			m_lOwnerID = 0;

			// Read the bloc id
			Text.Recv(*pstream);
			m_lBlocID = atol(Text.AsJCharPtr());

			// Read the Source ID
			Text.Recv(*pstream);
			JInt32 NbItems = atol(Text.AsJCharPtr());

			// Boucle sur les items
			for(int x = 0 ; x < NbItems; x++)
			{
				// Read the Source ID
				Text.Recv(*pstream);
				JInt32 Item = atol(Text.AsJCharPtr());
				ImpMap.MoveTo(Item);
				if(!ImpMap.IsValid())
				{
					delete pstream;
					return false;
				}

				// Ajout item
				AddItem((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()));
			}

			ImpMap.Add(GetID()) = this->AsIItem();

			JInt32 Id = m_pItemsConstruits->CheckExists(this);
			if(Id != 0 && ReplaceImportBy(Id,ImpMap))
			{
				delete pstream;
				delete this;
				return true;
			}

			if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
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
				m_pItemsConstruits->RegisterCritereQuantile(this,ImpMap);
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
JBool	CCritereQuantile::IsSimilaire(const CCritereQuantile *pRight) const
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

JBool CCritereQuantile::ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap)
{
	const CCritereQuantile * pItem = m_pItemsConstruits->GetCritereQuantile(Id);

	if(!pItem)
		return false;

	if (pItem->GetLabel() != this->GetLabel())
		return (false);

	JList<const CItemConstruit *> ItemLeft;
	JList<const CItemConstruit *> ItemRight;
	GetItems(ItemLeft);
	pItem->GetItems(ItemRight);

	int x = 1;
	for(ItemLeft.MoveFirst(); ItemLeft.IsValid(); ItemLeft.MoveNext(), ++x)
	{
		const CItemConstruit * pLeft = ItemLeft.GetItem();
		for(ItemRight.MoveFirst(); ItemRight.IsValid(); ItemRight.MoveNext())
		{
			const CItemConstruit * pRight = ItemRight.GetItem();
			if(pRight->GetLabel() == pLeft->GetLabel() && pRight->IsSimilaire(pLeft))
			{
				ImpMap.MoveTo(pLeft->GetID());
				if(ImpMap.IsValid())
					ImpMap.GetItem() = pRight;
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

