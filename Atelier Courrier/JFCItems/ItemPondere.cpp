//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe permettant de définir des items pondérés
// - chaque item sera défini par rapport à un item de base, et celui ci sera associé à une liste
// - d'items avec une pondération
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemPondere.h"
#include "ItemsConstruitsBase.h"
#include "JMFCStream.h"

///////////////////////////////////////////////////////////////////////////////////////////
// Construction
CItemPondere::CItemPondere(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp) : m_pItemsConstruits(pItemsConstruits)
{
	// Repere de l'item pondéré
	m_lID = ID;

	// Delete temporary items
	m_bDeleteTemp = bDeleteTemp;

	// Ponderate Item label
	CString Label;
	Label.Format("Item Pondere %d",m_lID);
	m_Label = Label;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Destruction
CItemPondere::~CItemPondere()
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

///////////////////////////////////////////////////////////////////////////////////
/// Chargement de l'item pondéré
///
///
/// Repere de l'item pondéré					: m_lID
/// Owner ID									: m_lOwnerID
/// Ponderate Item Label						: m_Label;
/// Ident de l'item de base						: m_IDItemBase;
/// Liste des items avec pondérations			: m_IDItemsPonderes;
/// Liste coeffs des items avec pondérations	: m_CoeffItemsPonderes;
///
JArchive& CItemPondere::Recv( JArchive& lhs, JInt32 Version )
{
	switch(Version)
	{
		case 1:
		{
			// Lecture Id
			lhs.Recv(m_lID);

			// Lecture Owner Id
			lhs.Recv( m_lOwnerID );

			// Lecture Label
			m_Label.Recv(lhs);

			// Lecture Id item de base
			lhs.Recv(m_IDItemBase);

			// Init listes des composants pondérés
			m_IDItemsCompos.Reset(); 
			m_CoeffItemsCompos.Reset(); 

			// Lecture nombre de composants pondérés
			JInt32 Count;
			lhs.Recv(Count);

			// Récup les composants items 
			for(JInt32 x = 0 ; x < Count ; x++)
			{
				// Récup a clé ident item
				lhs.Recv(m_IDItemsCompos.AddTail());
			}

			// Récup les coefficients
			for(JInt32 x = 0 ; x < Count ; x++)
			{
				// Récup la pondération
				lhs.Recv(m_CoeffItemsCompos.AddTail());
			}
		}
		break;

		default:
			TRACE("CItemPondere Version error");
			throw JInternalError::GetInstance();
		break;
	}
	return lhs;
}


///////////////////////////////////////////////////////////////////////////////////
/// Sauvegarde de l'item pondéré
///
///
/// Repere de l'item pondéré				: m_lID
/// Owner ID								: m_lOwnerID
/// Ponderate Item Label					: m_Label;
/// Liste des items avec pondérations			: m_IDItemsPonderes;
/// Liste coeffs des items avec pondérations	: m_CoeffItemsPonderes;
///
JArchive& CItemPondere::Send( JArchive& lhs ) const
{
	// Sauve Repere de l'item pondéré
	lhs.Send(m_lID);

	// Sauve Owner ID
	lhs.Send( m_lOwnerID );

	// Sauve Ponderate Item Label
	m_Label.Send(lhs);

	// Sauve Ident de l'item de base
	lhs.Send(m_IDItemBase);

	// Sauve le nombre de composants pondérés
	JInt32 Count = m_IDItemsCompos.GetCount();
	lhs.Send(Count);

	// Sauve les informations composants (ID)
	for (m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid() ; m_IDItemsCompos.MoveNext())
	{
		// Sauve clef ident composant
		lhs.Send(m_IDItemsCompos.GetItem());
	}

	// Sauve les informations composants (Coeff)
	for (m_CoeffItemsCompos.MoveFirst(); m_CoeffItemsCompos.IsValid() ; m_CoeffItemsCompos.MoveNext())
	{
		// Sauve le coefficient de pondération de ce composant
		lhs.Send(m_CoeffItemsCompos.GetItem()); 
	}
	
	return lhs;
}

//////////////////////////////////////////////////////////////////
// Registers the object to the Owner ID with the new ID
JBool CItemPondere::Register(JInt32 OwnerID, JInt32 ID)
{
	// Check if not already registered
	if(!IsRegistered())
	{
		if(OwnerID != 0 && ID != 0)
		{
			// Register ident objet
			m_lOwnerID	= OwnerID;
			m_lID		= ID;

			// Register l'ident item de base
			m_TempItems.MoveTo(m_IDItemBase);
			if(m_TempItems.IsValid())
			{
				// Get the item construit
				CItemConstruit * pItem = m_TempItems.GetItem();

				// Register the item to this object
				const CItemConstruit * pRItem = m_pItemsConstruits->RegisterItemConstruit(pItem, m_lID);
												

				// Set new item ID
				if(pRItem)
					m_IDItemBase = pRItem->GetID();
				else
					m_IDItemBase = 0;
				if(m_bDeleteTemp)
					delete pItem;

			}
			
			// Boucle sur les composants items
			for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
			{
				m_TempItems.MoveTo(m_IDItemsCompos.GetItem());
				if(m_TempItems.IsValid())
				{
					// Get the item construit
					CItemConstruit * pItem = m_TempItems.GetItem();

					// Register the item to this object
					const CItemConstruit * pRItem = m_pItemsConstruits->RegisterItemConstruit(pItem, m_lID);

					// Set new item ID
					if(pRItem)
						m_IDItemsCompos.GetItem() = pRItem->GetID();
					else
						m_IDItemsCompos.GetItem() = 0;

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

//////////////////////////////////////////////////////////////////////////////////////////////
/// Registers the object to the Owner ID with the new ID
JBool CItemPondere::Register(JInt32 OwnerID, JMap<JInt32, const IItem *> &ImpMap, JInt32 ID)
{
	// Check if not already registered
	if(!IsRegistered())
	{
		if(OwnerID != 0 && ID != 0)
		{
			m_lOwnerID	= OwnerID;
			m_lID		= ID;

			// Register l'ident item de base
			m_TempItems.MoveTo(m_IDItemBase);
			if(m_TempItems.IsValid())
			{
				// Get the item construit
				CItemConstruit * pItem = m_TempItems.GetItem();

				// Register the item to this object
				const CItemConstruit * pRItem = m_pItemsConstruits->RegisterItemConstruit(pItem, ImpMap, m_lID);

				// Set new item ID
				if(pRItem)
					m_IDItemBase = pRItem->GetID();
				else
					m_IDItemBase = 0;
			}	

			// Register les items composants
			for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
			{
				m_TempItems.MoveTo(m_IDItemsCompos.GetItem());
				if(m_TempItems.IsValid())
				{
					// Get the item construit
					CItemConstruit * pItem = m_TempItems.GetItem();
					// Register the item to this object
					const CItemConstruit * pRItem = m_pItemsConstruits->RegisterItemConstruit(pItem, ImpMap, m_lID);
					// Set new item ID
					if(pRItem)
						m_IDItemsCompos.GetItem() = pRItem->GetID();
					else
						m_IDItemsCompos.GetItem() = 0;
				}
			}
			m_TempItems.Reset();
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// tests if the caller can unregister the object
JBool CItemPondere::CanUnRegister(JInt32 OwnerID, JMap<JInt32, JInt32> * pMapItems)
{
	if(IsRegistered())
	{
		if(m_lOwnerID == OwnerID)
		{
			JBool Ret = true;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Id de base
			// CanUnRegister the item from this object
			if(!m_pItemsConstruits->CanUnRegisterItemConstruit(m_IDItemBase, 0, m_lID))
			{
				if(pMapItems)
				{
					pMapItems->MoveTo(m_IDItemBase);
					if(!pMapItems->IsValid())
						pMapItems->Add(m_IDItemBase) = 1;
					else
						pMapItems->GetItem() += 1;
				}
				Ret = false;
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Liste des Ids Composants
			for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
			{
				// CanUnRegister the item from this object
				if(!m_pItemsConstruits->CanUnRegisterItemConstruit(m_IDItemsCompos.GetItem(), 0, m_lID))
				{
					if(pMapItems)
					{
						pMapItems->MoveTo(m_IDItemsCompos.GetItem());
						if(!pMapItems->IsValid())
							pMapItems->Add(m_IDItemsCompos.GetItem()) = 1;
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

////////////////////////////////////////////////////////////////////////////////////
// Récupération ID item de base de l'item pondéré 
JInt32 CItemPondere::GetIDItemBase()
{
	return m_IDItemBase;
}

///////////////////////////////////////////////////////////////////////////////////
// Récupération item de base de l'item pondéré 
void CItemPondere::GetItemBase(const CItemConstruit * pItemBase) const
{
	if(IsRegistered()) 
	{
		pItemBase = m_pItemsConstruits->GetItemConstruit(m_IDItemBase);
	}
	else
	{
		// Get first temporary item construit
		m_TempItems.MoveTo(m_IDItemBase);
		// return if not valid
		if(m_TempItems.IsValid())
			pItemBase  = m_TempItems.GetItem();
	}	
}

////////////////////////////////////////////////////////////////////////////////////////
/// Gets the Ponderates Item IDs
void CItemPondere::GetIDItems(JList<JInt32> &IDItemsCompos, JList<JFlt64> &CoeffItemsCompos)
{
	// Boucle sur tous les éléments ID composants
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		IDItemsCompos.AddTail() = m_IDItemsCompos.GetItem();  
	}

	// Idem avec les coeffs
	for(m_CoeffItemsCompos.MoveFirst(); m_CoeffItemsCompos.IsValid(); m_CoeffItemsCompos.MoveNext())
	{
		CoeffItemsCompos.AddTail() = m_CoeffItemsCompos.GetItem();  
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Modifie la valeur pondération d'un composant
void CItemPondere::	ModifPonder(JUnt32 IdCompo, JFlt64 NewCoeff)
{
	// On se positionne sur le composant
	// Boucle sur la liste des composants, et en synchro sur la liste de leurs coefficients
	m_CoeffItemsCompos.MoveFirst(); 
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext(), m_CoeffItemsCompos.MoveNext())
	{
		// Récup l'item construit
		CItemConstruit *pItemConstruit = m_pItemsConstruits->GetItemConstruit(m_IDItemsCompos.GetItem());

		// Si c'est le composant à modifier
		if (pItemConstruit->GetID() == IdCompo)
		{
			// On modifie sa pondération
			m_CoeffItemsCompos.GetItem() = NewCoeff; 

			// Indique modification à prendre en compte
			if(IsRegistered())
				m_pItemsConstruits->SetDirty();

			// Et on sort
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// Gets the Ponderates Item IDs
void CItemPondere::GetItems(JList<const CItemConstruit *> &Items, JList <JFlt64> &PonderItems) const
{
	// Boucle sur la liste des composants, et en synchro sur la liste de leurs coefficients
	m_CoeffItemsCompos.MoveFirst(); 
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext(), m_CoeffItemsCompos.MoveNext())
	{
		if(IsRegistered()) 
		{
			// Récup l'item construit
			Items.AddTail() = m_pItemsConstruits->GetItemConstruit(m_IDItemsCompos.GetItem());

			// Récup le coefficient
			PonderItems.AddTail() = m_CoeffItemsCompos.GetItem(); 
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_IDItemsCompos.GetItem());
			// return if not valid
			if(m_TempItems.IsValid())
			{
				// Récup item construit
				Items.AddTail() = m_TempItems.GetItem();

				// Récup le coefficient
				PonderItems.AddTail() = m_CoeffItemsCompos.GetItem();
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
///	Check if the Absoloute definitions are the same
JBool CItemPondere::IsSimilaire(const CItemPondere *pRight) const
{
	// Les items de base
	const CItemConstruit * pItemBaseLeft;
	const CItemConstruit * pItemBaseRight;

	// Test les items de base
	GetItemBase(pItemBaseLeft);
	pRight->GetItemBase(pItemBaseRight); 

	// Test d'abord si même item de base
	if(!pItemBaseLeft->IsSimilaire(pItemBaseRight)) 
		return false;

	// Les infos items construits
	JList<const CItemConstruit *> ItemLeft;
	JList<const CItemConstruit *> ItemRight;

	// Les infos coeffs
	JList <JFlt64> PonderItemsLeft;
	JList <JFlt64> PonderItemsRight;

	GetItems(ItemLeft, PonderItemsLeft);
	pRight->GetItems(ItemRight, PonderItemsRight);

	// Test d'abord si même nombre d'items construit
	if(ItemLeft.GetCount() != ItemRight.GetCount())
		return false;

	// Test les items construits 
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

	// Test les cefficients appliqués
	int y= 1;
	for (PonderItemsLeft.MoveFirst(); PonderItemsLeft.IsValid(); PonderItemsLeft.MoveNext())
	{
		JFlt64 CoeffLeft = PonderItemsLeft.GetItem(); 
		for (PonderItemsRight.MoveFirst(); PonderItemsRight.IsValid(); PonderItemsRight.MoveNext())
		{
			if (CoeffLeft == PonderItemsRight.GetItem())
			{
				PonderItemsRight.Remove();
				break;
			}
		}
		if(PonderItemsLeft.GetCount() != (PonderItemsRight.GetCount()+x))
			return false;
	}

	return true;
}

// Unregisters the object
JBool CItemPondere::UnRegister(JInt32 OwnerID)
{
	if(IsRegistered() && CanUnRegister(OwnerID))
	{
		if(m_lOwnerID == OwnerID)
		{
			// UnRegistre item de base
			m_pItemsConstruits->UnRegisterItemConstruit(m_IDItemBase, m_lID);

			// UnRegistre les composants
			for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
			{
				// UnRegister the item from this palette
				m_pItemsConstruits->UnRegisterItemConstruit(m_IDItemsCompos.GetItem(), m_lID);
			}
			m_lOwnerID = -1;
			m_pItemsConstruits->SetDirty();
			return true;
		}
	}
	return false;
}

/// Add the Item ID de base 
JBool CItemPondere::AddIDBase(JInt32 ID)
{
	m_IDItemBase = ID;
	return true;
}

/// Add the Composant Item ID to the item list
JBool CItemPondere::AddIDList(JInt32 ID, JFlt64 Ponder)
{
	ASSERT(m_IDItemsCompos.GetCount() == m_CoeffItemsCompos.GetCount());

	// Id de base
	if(m_IDItemBase == ID)
		return false;

	// Liste des Ids Composants
	m_CoeffItemsCompos.MoveFirst();

	// Boucle sur les composants exsitants
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext(), m_CoeffItemsCompos.MoveNext())
	{
		// Test si existant
		if(m_IDItemsCompos.GetItem() == ID)
			return false;
	}

	// Sinon on le rajoute (ID + Coeff)
	m_IDItemsCompos.AddTail()		= ID;
	m_CoeffItemsCompos.AddTail()	= Ponder;

	return true;
}

/// Removes the item ID from the item list
JBool CItemPondere::RemoveIDList(JInt32 ID)
{
	// Boucle sur la liste des ids composants
	m_CoeffItemsCompos.MoveFirst(); 
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		if(m_IDItemsCompos.GetItem() == ID)
		{
			// Vire élmt Id et le coeff
			m_IDItemsCompos.Remove();
			m_CoeffItemsCompos.Remove(); 
			return true;
		}

		// Passe à élmt liste coeff suivant
		m_CoeffItemsCompos.MoveNext(); 
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Makes a deep copy of this object
/*
// Liste des ids composants + liste des coeffs
JList <JInt32>					m_IDItemsCompos;
JList <JFlt64>					m_CoeffItemsCompos;

/// const pointer to items construit
CItemsConstruits *const			m_pItemsConstruits;

/// Map of temporary items construit
JMap<JInt32, CItemConstruit *>	m_TempItems;
*/
CItemPondere & CItemPondere::DeepCopy(const CItemPondere *rhs)
{
	// Must delete temporary objects
	m_bDeleteTemp = true;

	// Recup Label
	m_Label		=	rhs->m_Label ;

	// Copy item de base
	m_IDItemBase = rhs->m_IDItemBase; 	

	// Les composants items avec leurs coeffs
	m_IDItemsCompos.Reset();
	m_CoeffItemsCompos.Reset();

	// Move to the item
	rhs->m_TempItems.MoveTo(m_IDItemBase);

	// If the item is valid copy it
	if(rhs->m_TempItems.IsValid())
	{
		// Add the items id
		const CItemConstruit * Orig = rhs->m_TempItems.GetItem();
		CItemConstruit * pItem = m_pItemsConstruits->GetNewItemConstruit(Orig);
		if(AddIDBase(pItem->GetID()))
		{
			m_TempItems.Add(pItem->GetID()) = pItem;
		}
	}

	// Les composants items avec leurs coeffs
	m_IDItemsCompos.Reset();
	m_CoeffItemsCompos.Reset();

	/*
	// Récupère les idents des composants
	for (rhs->m_IDItemsCompos.MoveFirst(); rhs->m_IDItemsCompos.IsValid(); rhs->m_IDItemsCompos.MoveNext())
	{
		m_IDItemsCompos.AddTail() = rhs->m_IDItemsCompos.GetItem(); 
	}

	// Récupère les coefficients appliqués aux composants
	for (rhs->m_CoeffItemsCompos.MoveFirst(); rhs->m_CoeffItemsCompos.IsValid(); rhs->m_CoeffItemsCompos.MoveNext())
	{
		m_CoeffItemsCompos.AddTail() = rhs->m_CoeffItemsCompos.GetItem(); 
	}
	*/

	// Moves les items composants
	rhs->m_CoeffItemsCompos.MoveFirst();
	for(rhs->m_IDItemsCompos.MoveFirst(); rhs->m_IDItemsCompos.IsValid(); rhs->m_IDItemsCompos.MoveNext(), rhs->m_CoeffItemsCompos.MoveNext())
	{
		// Move to the item
		rhs->m_TempItems.MoveTo(rhs->m_IDItemsCompos.GetItem());

		// If the item is valid copy it
		if(rhs->m_TempItems.IsValid())
		{
			// Add the items id
			const CItemConstruit * Orig = rhs->m_TempItems.GetItem();
			CItemConstruit * pItem = m_pItemsConstruits->GetNewItemConstruit(Orig);

			// Et on n'oublie pas le coeff
			if(AddIDList(pItem->GetID(), rhs->m_CoeffItemsCompos.GetItem() ))
			{
				m_TempItems.Add(pItem->GetID()) = pItem;
			}
		}
	}
	
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
// Sets the Label of this object
JBool	CItemPondere::SetLabel(const JLabel & Label, JBool bForce)
{
	if(Label.IsEmpty())
		return false;

	const CItemPondere * pItem = 0;

	if(!bForce)
	{
		// Récupére l'item
		pItem = m_pItemsConstruits->GetItemPondere(Label);

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

////////////////////////////////////////////////////////////////////////////////
// Returns true if the item contains the given itemID
JBool CItemPondere::HasItem(JInt32 ID) const
{
	const CItemConstruit * pItem = 0;

	// Sur Id item de base
	if(IsRegistered()) 
	{
		// Get the item construit
		pItem = m_pItemsConstruits->GetItemConstruit(m_IDItemBase);
	}
	else
	{
		// Get first temporary item construit
		m_TempItems.MoveTo(m_IDItemBase);
		// return if not valid
		if(m_TempItems.IsValid())
		{
			pItem = m_TempItems.GetItem();
		}

		if(pItem->HasItem(ID))
			return true;
	}

	// Sur les IDs composants
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		if(IsRegistered()) 
		{
			// Get the item construit
			pItem = m_pItemsConstruits->GetItemConstruit(m_IDItemsCompos.GetItem());
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_IDItemsCompos.GetItem());
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

/////////////////////////////////////////////////////////////////////////////////
// Rename an Item Ponderate
JBool CItemPondere::RenameItem(JLabel &Label, JInt32 ItemID)
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
JBool CItemPondere::CheckLabel(JLabel &Label) const
{
	const CItemConstruit * pItem = 0;

	// Vérifie label item de base
	if(IsRegistered()) 
	{
		pItem = m_pItemsConstruits->GetItemConstruit(m_IDItemBase);
	}
	else
	{
		// Get first temporary item construit
		m_TempItems.MoveTo(m_IDItemBase);
		// return if not valid
		if(!m_TempItems.IsValid())
			return false;
		pItem = m_TempItems.GetItem();
	}

	// Check if item exists and that the item is valid
	if(!pItem || pItem->GetLabel() == Label)
		return false;

	// Vérifie les labels des composants
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		if(IsRegistered()) 
		{
			// Get the item construit
			pItem = m_pItemsConstruits->GetItemConstruit(m_IDItemsCompos.GetItem());
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_IDItemsCompos.GetItem());
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

////////////////////////////////////////////////////////////////////////////////////////////
/// Export the object via StdioFile
JBool CItemPondere::Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const
{
	// First check if we have already been exported
	ExpMap.MoveTo(m_lID);
	if(ExpMap.IsValid ())
		return true;// Already exported

	// Export ident item pondéré
	ExpMap.Add(m_lID) = true;

	// Export Id Item de base
	ExpMap.MoveTo(m_IDItemBase);
	if(!ExpMap.IsValid())
	{
		// Export the owner
		IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_IDItemBase);
		if(m_pIItem)
			m_pIItem->Export(File,ExpMap);
	}

	// Export des composants
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		// We must export the owner first
		ExpMap.MoveTo(m_IDItemsCompos.GetItem());

		// Has the owner been exported ?
		if(!ExpMap.IsValid())
		{
			// Export the owner
			IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_IDItemsCompos.GetItem());
			if(m_pIItem)
				m_pIItem->Export(File,ExpMap);
		}
	}

	// Export this item
	CString Buf;
	File.WriteString(_T("ITEMPOND\n"));				//	Write the Type Item

	File.WriteString(_T("1\n"));					//	Write the Version

	Buf.Format("%s\n",m_Label.AsJCharPtr());
	File.WriteString(Buf);							//	Write the Name

	Buf.Format("%d\n",m_lID);
	File.WriteString(Buf);							//	Write the ID

	Buf.Format("%d\n",m_lOwnerID); 
	File.WriteString(Buf);							//	Write the OwnerID

	Buf.Format("%d\n",m_IDItemBase); 
	File.WriteString(Buf);							//	Write the m_IDItemBase

	Buf.Format("%d\n",m_IDItemsCompos.GetCount()); 
	File.WriteString(Buf);							//	Write the Number of composants items

	// Export liste des IDs composants et des coeffs
	m_CoeffItemsCompos.MoveFirst();
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		Buf.Format("%d\n", m_IDItemsCompos.GetItem()); 
		File.WriteString(Buf);						//	Write the Item ID

		Buf.Format("%0.1f\n", m_CoeffItemsCompos.GetItem()); 
		File.WriteString(Buf);						//	Write the Coeff Item ID

		// Passe à coeff suivant
		m_CoeffItemsCompos.MoveNext(); 
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// Import the objects via StdioFile
JBool	CItemPondere::Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap)
{
	// Export this item
	CString Buf;
	File.ReadString(Buf);							//	Read the version
	JInt32 Version = atol(Buf);
	
	switch(Version)
	{
		case 1:
		{
			// Lecture du nom de l'item pondére
			File.ReadString(Buf);		
			m_Label = Buf;

			// Lecture de l'ident d'item pondéré
			File.ReadString(Buf);			
			m_lID = atol(Buf);
			TRACE("Item Pondéré : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Lecture du OwnerID
			File.ReadString(Buf);
			m_lOwnerID = atol(Buf);
			m_lOwnerID = 0;

			// Lecture de l'ident de l'item de base
			File.ReadString(Buf);			
			m_IDItemBase = atol(Buf);

			// Ajout au Map
			ImpMap.MoveTo(m_IDItemBase);
			if(!ImpMap.IsValid())
				return false;

			// Ajout de l'item 
			AddItemBase((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()));

			// Init Listes des IDs items composants et liste de leur coefficient
			m_IDItemsCompos.Reset(); 
			m_CoeffItemsCompos.Reset();

			// Lecture du nombre items composants pour les coefficients
			File.ReadString(Buf);						
			JInt32 NbItemsPonds = atol(Buf);
			for(int x = 0 ; x < NbItemsPonds; x++)
			{
				// Lecture ident de l'item
				File.ReadString(Buf);			
				JInt32 IdItem = atol(Buf);

				// Lecture ident de l'item
				File.ReadString(Buf);			
				JFlt64 Coeff = atof(Buf);

				ImpMap.MoveTo(IdItem);
				if(!ImpMap.IsValid())
					return false;

				// Ajout de l'item 
				AddItem((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()), Coeff);
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
				m_pItemsConstruits->RegisterItemPondere(this,ImpMap);
			}

		}
		break;

		default:
			return false;
	}
	return true;
}

/// Export the object avec CArchive
JBool	CItemPondere::Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const
{
	// First check if we have already been exported
	ExpMap.MoveTo(m_lID);
	if(ExpMap.IsValid ())
		return true;// Already exported

	// Export ident item pondéré
	ExpMap.Add(m_lID) = true;
	
	// Export Id Item de base
	ExpMap.MoveTo(m_IDItemBase);
	if(!ExpMap.IsValid())
	{
		// Export the owner
		IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_IDItemBase);
		if(m_pIItem)
			m_pIItem->Export(Archive,ExpMap);
	}

	// Export des Ids composants
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		// We must export the owner first
		ExpMap.MoveTo(m_IDItemsCompos.GetItem());

		// Has the owner been exported ?
		if(!ExpMap.IsValid())
		{
			// Export the owner
			IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_IDItemsCompos.GetItem());
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
	Text = "ITEMPOND";
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
	
	//	Write the ID Item de base
	Buf.Format("%d",m_IDItemBase);
	Text = Buf;
	Text.Send(*pstream);  

	//	Write the Number of items composants
	Buf.Format("%d", m_IDItemsCompos.GetCount()); 
	Text = Buf;
	Text.Send(*pstream);  

	// Liste des Ids Composants et des coeffs
	m_CoeffItemsCompos.MoveFirst(); 
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		//	Write the Item ID
		Buf.Format("%d", m_IDItemsCompos.GetItem()); 
		Text = Buf;
		Text.Send(*pstream);

		// 	Write coeff à appliquer sur cet item
		Buf.Format("%0.1f", m_CoeffItemsCompos.GetItem()); 
		Text = Buf;
		Text.Send(*pstream);

		// Passe à élmt coef suivant
		m_CoeffItemsCompos.MoveNext(); 
	}

	delete pstream;
	return true;
}

/// Import the objects avec CArchive
JBool	CItemPondere::Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap)
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
			// Lecture du nom de l'item pondére
			TxtLabel.Recv(*pstream);
			m_Label = TxtLabel.AsJCharPtr();

			// Lecture de l'ident d'item pondéré
			TxtLabel.Recv(*pstream);
			m_lID = atol(TxtLabel.AsJCharPtr());

			TRACE("Item Pondéré : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Lecture du OwnerID
			TxtLabel.Recv(*pstream);
			m_lOwnerID = atol(TxtLabel.AsJCharPtr());
			m_lOwnerID = 0;
			
			// Lecture de l'ident de l'item de base
			TxtLabel.Recv(*pstream);
			m_IDItemBase = atol(TxtLabel.AsJCharPtr());

			// Ajout au Map
			ImpMap.MoveTo(m_IDItemBase);
			if(!ImpMap.IsValid())
				return false;

			// Ajout de l'item 
			AddItemBase((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()));

			// Init Liste des Ids items composants et Liste avec leur coefficient
			m_IDItemsCompos.Reset();
			m_CoeffItemsCompos.Reset();
				
			// Lecture du nombre items composants pour les coefficients
			TxtLabel.Recv(*pstream);
			JInt32 NbItemsPonds = atol(TxtLabel.AsJCharPtr());

			// Lecture des Ids items composants	
			for(int x = 0 ; x < NbItemsPonds; x++)
			{
				// Lecture ident de l'item
				TxtLabel.Recv(*pstream);
				JInt32 IdItem = atol(TxtLabel.AsJCharPtr());

				// Lecture du coefficient
				TxtLabel.Recv(*pstream);
				JFlt64 Coeff = atof(TxtLabel.AsJCharPtr());

				// Vérifie existence
				ImpMap.MoveTo(IdItem);
				if(!ImpMap.IsValid())
				{
					delete pstream;
					return false;
				}
				AddItem((CItemConstruit *)(ImpMap.GetItem()->AsItemConstruit()), Coeff);
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
				m_pItemsConstruits->RegisterItemPondere(this,ImpMap);
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

/////////////////////////////////////////////////////////////////////////////////////////////
/// Add the given item construit to the object else returns false
JBool CItemPondere::AddItemBase(CItemConstruit * pItem)
{
	if(IsRegistered())
		return false;

	/*
	if(IsRegistered() || pItem->IsRegistered())
		return false;
	*/

	if(!pItem->IsValid())
		return false;

	if(AddIDBase(pItem->GetID()))
	{
		m_TempItems.Add(pItem->GetID()) = pItem;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Add the given item construit to the object else returns false
JBool CItemPondere::AddItem(CItemConstruit * pItem, JFlt64 Ponder)
{
	if(IsRegistered())
	// if(IsRegistered() || pItem->IsRegistered())
		return false;

	if(!pItem->IsValid())
		return false;

	if(AddIDList(pItem->GetID(), Ponder))
	{
		m_TempItems.Add(pItem->GetID()) = pItem;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// Check if the object is valid with the given terrain
JBool CItemPondere::IsValid(CTerrain * pTerrain) const
{
	const CItemConstruit * pItem = 0;

	// érifie l'ident item de base
	if(IsRegistered()) 
	{
		// Get the item construit
		pItem = m_pItemsConstruits->GetItemConstruit(m_IDItemBase);
	}
	else
	{
		// Get first temporary item construit
		m_TempItems.MoveTo(m_IDItemBase);
		// return if not valid
		if(!m_TempItems.IsValid())
			return false;
		pItem = m_TempItems.GetItem();
	}

	// Check if item exists and that the item is valid
	if(!pItem || !pItem->IsValid(pTerrain))
		return false;

	// Vérifie les idents compos
	for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
	{
		if(IsRegistered()) 
		{
			// Récup ident item compo
			JUnt32 IdItemCompo = m_IDItemsCompos.GetItem();

			// Get the item construit
			pItem = m_pItemsConstruits->GetItemConstruit(m_IDItemsCompos.GetItem());
		}
		else
		{
			// Get first temporary item construit
			m_TempItems.MoveTo(m_IDItemsCompos.GetItem());
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

////////////////////////////////////////////////////////////////////////////////////////////
// Removes the given item construit from the object else returns false
JBool CItemPondere::RemoveItem(JInt32 ID)
{
	if (IsRegistered())
	{
		// Test
		for (m_TempItems.MoveFirst(); m_TempItems.IsValid(); m_TempItems.MoveNext())
		{
			JInt32 KeyId =  m_TempItems.GetKey(); 
			CItemConstruit * pItemConstruit = m_TempItems.GetItem(); 

			JBool Stop = true;
		}

		// Vire l'item des items construits temporaires
		m_TempItems.MoveTo(m_IDItemBase);
		if(m_TempItems.IsValid())
		{
			if(m_bDeleteTemp)
				delete m_TempItems.GetItem();

			m_TempItems.Remove();
		}

		/*
		// Peut-on Supprimer l'item de base
		JMap<JInt32, JLabel> MapUpwards;
		if(m_pItemsConstruits->CanUnRegisterItemConstruit(m_IDItemBase, &MapUpwards))
		{
			if(!m_pItemsConstruits->UnRegisterItemConstruit(m_IDItemBase))
			{
				return false;
			}
			else
			{
				// Vire l'item des items construits temporaires
				m_TempItems.MoveTo(m_IDItemBase);
				if(m_TempItems.IsValid())
				{
					if(m_bDeleteTemp)
						delete m_TempItems.GetItem();

					m_TempItems.Remove();
				}
			}
		}
		*/

		// Suprime les items composants
		for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
		{
			// Récup ident item composant
			JInt32 IdItemCompo = m_IDItemsCompos.GetItem(); 
			
			// Vire l'item des items construits temporaires
			m_TempItems.MoveTo(IdItemCompo);
			if(m_TempItems.IsValid())
			{
				if(m_bDeleteTemp)
					delete m_TempItems.GetItem();

				m_TempItems.Remove();
			}

			// Enleve de la liste des composants  
			RemoveIDList(IdItemCompo);
		}
		return false;

		/*
		// Suprime les items composants
		for(m_IDItemsCompos.MoveFirst(); m_IDItemsCompos.IsValid(); m_IDItemsCompos.MoveNext())
		{
			// Récup ident item composant
			JInt32 IdItemCompo = m_IDItemsCompos.GetItem(); 

			// Peut-on Supprimer l'item de base
			JMap<JInt32, JLabel> MapUpwards;
			if(m_pItemsConstruits->CanUnRegisterItemConstruit(IdItemCompo, &MapUpwards))
			{
				if(!m_pItemsConstruits->UnRegisterItemConstruit(IdItemCompo))
				{
					return false;
				}
				else
				{
					// Vire l'item des items construits temporaires
					m_TempItems.MoveTo(IdItemCompo);
					if(m_TempItems.IsValid())
					{
						if(m_bDeleteTemp)
							delete m_TempItems.GetItem();

						m_TempItems.Remove();
					}
				}

				// Enleve de la liste des composants  
				RemoveIDList(IdItemCompo);
			}
		}
		return false;
		*/
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Reconstruction de l'item pondéré origine
JBool CItemPondere::ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap)
{
	// L'item pondéré 
	const CItemPondere * pItem = m_pItemsConstruits->GetItemPondere(Id);

	if(!pItem)
		return false;

	if (pItem->GetLabel() != this->GetLabel())
		return (false);

	// Les items palette origine et les coeffs associés
	JList<const CItemConstruit *>	ItemLeft;
	JList <JFlt64>					PonderItemsLeft;
	GetItems(ItemLeft, PonderItemsLeft);

	// Les items palette reconstruite et les coeffs associés
	JList<const CItemConstruit *>	ItemRight;
	JList <JFlt64>					PonderItemsRight;
	pItem->GetItems(ItemRight, PonderItemsRight);

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