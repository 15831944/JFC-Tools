// ItemConstruit.cpp: implementation of the CItemConstruit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemConstruit.h"
#include "ItemsConstruitsBase.h"
#include "IsValidVisitor.h"
#include "ToEquivalenceVisitor.h"
#include "HasItemVisitor.h"
#include "RegisterItemsVisitor.h"
#include "UnRegisterItemsVisitor.h"
#include "CalcIndivVisitor.h"
#include "ToItemVisitor.h"
#include "ToAbsolutVisitor.h"
#include "CalcQuantiValuesVisitor.h"
#include "ExportVisitor.h"
#include "importivisitor.h"
#include "importavisitor.h"
#include "ToXMLIVisitor.h"
#include "JMFCStream.h"
#include "TerrainManager.h"

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemConstruit::CItemConstruit(CItemsConstruitsBase *pItemsConstruits, JInt32 ID)
:m_pItemsConstruits(pItemsConstruits)
{
	// Repere de l'item construit
	m_lID = ID ;
	// Reference count
	m_lReferenceCnt = 0;
	// Id of the base source
	m_lSourceID = 0;
	// Id of the base terrain
	m_lTerrainID = 0;
	// Item Label
	CString Label;
	Label.Format("Item Construit %d",m_lID);
	m_Label = Label;

	// Init Categorie
	m_Categorie = 0;

	// Item Builder String format
	m_IBString;
	// Absolut Builder String format
	m_ABString;

	// PAr défaut non moyennable
	m_Moyennable = false;

	// Flag pour reconstruction d'office via repere absolu, par defaut à false (Mars 2014)
	m_RebuildViaRepAbs = false;
}

CItemConstruit::~CItemConstruit()
{
	// ne marche pas delete m_pItemsConstruits;
}

JArchive& CItemConstruit::Recv( JArchive& lhs, JInt32 Version )
{
	switch(Version)
	{
		case 1:
		{
			// Load the Item Construit
			lhs.Recv( m_lID );
			lhs.Recv( m_lOwnerID );
			lhs.Recv( m_lReferenceCnt );
			lhs.Recv( m_lSourceID );
			lhs.Recv( m_lTerrainID );
			m_Label.Recv(lhs);
			m_IBString.Recv(lhs);
			
			//m_IBString.Reset();

			m_ABString.Recv(lhs);

			// JUIN 2014 : We reset the m_IBString for the Balmetrie terrain 514 because of the change
			// from DPS1 to DPS2: all question IDs and modality IDs have changed.
			if (m_lSourceID == 49 && m_lTerrainID <= 514)
			{
				m_RebuildViaRepAbs = true;
				
				// Il nous faut systematiquement reconstruire les reperes du terrain m_IBString pour les terrains source 49 et <= 514
				CIBuilder builder;
				CTerrain * pBal2012 = CTerrainManager::GetInstance()->GetTerrain(49, 514);

				GetItemBuilder(pBal2012, builder);
				m_IBString.Reset();

				std::string str;
				str = "";
				builder.AsString(str);
				m_IBString = str.c_str();

				// m_lTerrainID--;
				// m_IBString.Reset();
			}
			//
		}
		break;

		default:
			TRACE("CItemConstruit Version error");
			throw JInternalError::GetInstance(); //("CItemConstruit :: File Version error");
		break;
	}
	return lhs;
}

JArchive& CItemConstruit::Send( JArchive& lhs ) const
{
	// Save the Item Construit
	lhs.Send( m_lID );
	lhs.Send( m_lOwnerID );
	lhs.Send( m_lReferenceCnt );
	lhs.Send( m_lSourceID );
	lhs.Send( m_lTerrainID );
	m_Label.Send(lhs);
	m_IBString.Send(lhs);
	m_ABString.Send(lhs);
	return lhs;
}

JBool	CItemConstruit::ToItemString(JStringEx &String) const
{
	JChar t[20];
	_itoa_s(m_lID,t,20,10);
	String = "I ";
	String.Append(t);
	return true;
}

// Check if the object is valid with the given terrain
JBool	CItemConstruit::IsValid(CTerrain * pTerrain) const
{
	// Vérifie la validité pointeur terrain Oct 2006 
	// (sinon plantage via enrgt base registre, en non concordance avec désinstallation programme)
	if (pTerrain)
	{
		JInt32 IdT = pTerrain->GetTerrainID();
		
		// This test is not bonafied 
		// Modif JUIN 2014, permet à une palette de récupérer pour la source Balmétrie et les terrains <= 514 la composition des items
		// if(pTerrain->GetSourceID() == m_lSourceID &&  pTerrain->GetTerrainID() == m_lTerrainID && m_RebuildViaRepAbs == false)
		if(pTerrain->GetSourceID() == m_lSourceID &&  pTerrain->GetTerrainID() == m_lTerrainID && !(m_lSourceID == 49 && m_lTerrainID <= 514))
			if(m_IBString.GetLength() != 0)
				return true;

		if(m_ABString.GetLength()==0)
			return false;

		CABuilder Builder;
		const JChar *pstr = m_ABString.AsJCharPtr();
		if(Builder.FromString(&pstr))
		{
			CIsValidVisitor Validate;
			if(!Validate.Validate(&Builder,pTerrain,m_pItemsConstruits))
			{
				CToEquivalenceVisitor Equivalent;
				if(Equivalent.Convert(&Builder,pTerrain,m_pItemsConstruits))
				{
					return Validate.Validate(&Builder,pTerrain,m_pItemsConstruits);
				}
				return false;
			}
			return true;
		}
		return false;
	}

	return false;
}

// Registers the object to the Owner ID with the new ID
JBool	CItemConstruit::Register(JInt32 OwnerID, JInt32 ID)
{
	// Check if not already registered
	if(!IsRegistered())
	{
		if(OwnerID != 0 && ID != 0)
		{
			// Problem if reference count not zero
			if(m_lReferenceCnt)
				return false;

			if(OwnerID != CItemsConstruits::SYSTEM_ID)
			{
				if(!m_IBString.GetLength())
					return 0;

				CIBuilder Builder;
				const JChar *pstr = m_IBString.AsJCharPtr();
				if(Builder.FromString(&pstr))
				{
					CRegisterItemsVisitor RegisterItem;
					RegisterItem.RegisterItems(&Builder, m_pItemsConstruits, OwnerID, ID);
				}
				else
					return false;
			}

			m_lOwnerID	= OwnerID;
			m_lID		= ID;
		}
		else
			return false;
	}

	m_lReferenceCnt++;
	m_pItemsConstruits->SetDirty();
	return true;
}

// tests if the caller can unregister the object
JBool	CItemConstruit::CanUnRegister	(JInt32 OwnerID, JMap<JInt32, JInt32> * pMapItems)
{
	JBool Ret = true;

	if(m_lReferenceCnt <= 0)
		Ret = false;
	else
	{
		if(m_lOwnerID == OwnerID)
			Ret = (m_lReferenceCnt == 1);
		else
			Ret = (m_lReferenceCnt > 1);
	}

	if(!Ret && pMapItems)
	{
		pMapItems->MoveTo(m_lID);
		if(!pMapItems->IsValid())
			pMapItems->Add(m_lID) = 1;
		else
			pMapItems->GetItem() += 1;
	}

	return Ret;
}

JBool CItemConstruit::RefCountAZero()
{
	if(m_lReferenceCnt >= 1)
	{
		m_lReferenceCnt = 0;
		// m_lOwnerID = -1;
		return true;
	}
	return false;
}

JBool CItemConstruit::RefCountAUn()
{
	if(m_lReferenceCnt > 1)
	{
		m_lReferenceCnt = 1;
		return true;
	}
	return false;
}

// Unregisters the object
JBool	CItemConstruit::UnRegister(JInt32 OwnerID)
{
	if(m_lReferenceCnt <= 0)
		return false;

	if(m_lOwnerID == OwnerID)
	{
		if(m_lReferenceCnt != 1)
			return false;

		CIBuilder Builder;
		const JChar *pstr = m_IBString.AsJCharPtr();


		// Modif JUIN 2014 (Xavier + Alain)
		if(m_lSourceID == 49 && m_lTerrainID <= 514)
		{

		}

		if(Builder.FromString(&pstr))
		{
			CUnRegisterItemsVisitor UnRegisterItems;
			UnRegisterItems.UnRegisterItems(&Builder, m_pItemsConstruits, m_lID);
		}
		else
			return false;

		m_lOwnerID = -1;
	}

	m_lReferenceCnt--;
	m_pItemsConstruits->SetDirty();
	return true;
}

CItemConstruit & CItemConstruit::DeepCopy(const CItemConstruit * rhs)
{
	// Id of the base source
	m_lSourceID			= rhs->m_lSourceID;
	// Id of the base terrain
	m_lTerrainID		= rhs->m_lTerrainID;
	// Item Label
	m_Label				= rhs->m_Label;
	// Item Builder String
	m_IBString			= rhs->m_IBString;
	// Absolut Builder String
	m_ABString			= rhs->m_ABString;

	return *this;
}

// Sets the Label of this object
JBool	CItemConstruit::SetLabel(const JLabel & Label, JBool bForce)
{
	if(Label.IsEmpty())
		return false;

	const CItemConstruit * pItem = 0;

	if(!bForce)
	{
		pItem = m_pItemsConstruits->GetItemIndependant(Label);

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

///////////////////////////////////////////////////////////////////////////////////////////
// Sets la catégorie
JBool CItemConstruit::SetCategorie	(JInt32 Categorie)
{
	if (this)
	{
		m_Categorie = Categorie;
		return true;
	}

	return false;
}

// Returns true if the item contains the given itemID
JBool	CItemConstruit::HasItem(JInt32 ID) const
{
	if(GetID()==ID)
		return true;

	if(!m_IBString.GetLength())
		return 0;

	CIBuilder Builder;
	const JChar *pstr = m_IBString.AsJCharPtr();
	if(Builder.FromString(&pstr))
	{
		CHasItemVisitor HasItem;
		return HasItem.HasItem(&Builder, m_pItemsConstruits, ID);
	}
	return false;
}

// Calculate the Item
// Inputs Terrain
// Outputs vector of individus and the EchantillonID
// TODO : Modify this call to take into account the calculation parameters
JBool	CItemConstruit::Calculate(CCalcItemParams * pParams) const
{
	ASSERT(pParams);
	if(!pParams)
		return false;

	if(!m_ABString.GetLength())
		return 0;

	if (pParams->GetItemAutorise() == false)
		return false;

	// Built the absolut item
	CABuilder Builder;
	const JChar *pstr = m_ABString.AsJCharPtr();
	if(Builder.FromString(&pstr))
	{
		// Check if valid
		CIsValidVisitor Validate;
		if(!Validate.Validate(&Builder, pParams->GetTerrain(), m_pItemsConstruits))
		{
			// Not valid, try to convert to absolut
			CToEquivalenceVisitor Equivalent;
			if(Equivalent.Convert(&Builder,pParams->GetTerrain(),m_pItemsConstruits))
			{
				// Validate absolut
				if(!Validate.Validate(&Builder,pParams->GetTerrain(),m_pItemsConstruits))
					// Not valid
					return false;
			}
			else
				// Cant convert to equivalent
				return false;
		}
		// Heres where we calculate
		CCalcIndivVisitor CalcIndiv;
		if(CalcIndiv.Calculate(&Builder, pParams, m_pItemsConstruits))
			return true;
	}
	return false;
}

// Calculates the Item
// Inputs Terrain and StatisticID
// Outputs the Effectif/Tot NbCas/Tot
// TODO : Modify this call to take into account the calculation parameters
JBool	CItemConstruit::CalculateEffectif(CCalcItemParams * pParams) const
{
	ASSERT(pParams);
	if(!pParams)
		return false;

	if(!m_ABString.GetLength())
		return 0;

	// Built the absolut item
	CABuilder Builder;
	const JChar *pstr = m_ABString.AsJCharPtr();
	if(Builder.FromString(&pstr))
	{
		// Check if valid
		CIsValidVisitor Validate;
		if(!Validate.Validate(&Builder,pParams->GetTerrain(),m_pItemsConstruits))
		{
			// Not valid, try to convert to absolut
			CToEquivalenceVisitor Equivalent;
			if(Equivalent.Convert(&Builder,pParams->GetTerrain(),m_pItemsConstruits))
			{
				// Validate absolut
				if(!Validate.Validate(&Builder,pParams->GetTerrain(),m_pItemsConstruits))
					// Not valid
					return false;
			}
			else
				// Cant convert to equivalent
				return false;
		}
		// Heres where we calculate
		CCalcIndivVisitor CalcIndiv;

		if (pParams->GetItemAutorise() == true)
		{
			if(CalcIndiv.CalculateEffectif(&Builder, pParams, m_pItemsConstruits))
			{
				Builder.Reset();
				return true;
			}
		}
		else
		{
			JBool PasDeCalcul = true;
		}				
	}
	return false;
}

// Calculates the quanti values
// Inputs Terrain and vector of values
// This must be used only with one segment on question
// Outputs the vector of values
JBool	CItemConstruit::CalcQuantiValues(CTerrain * pTerrain, JVector<JFlt32> &Values, JInt32 & Divider) const
{
	if(!m_ABString.GetLength())
		return 0;

	// Built the absolut item
	// CABuilder Builder;
	CABuilder *pBuilder = new CABuilder();
	const JChar *pstr = m_ABString.AsJCharPtr();
	// if(Builder.FromString(&pstr))
	if (pBuilder->FromString(&pstr))
	{
		// Check if valid
		CIsValidVisitor Validate;
		if(!Validate.Validate(pBuilder,pTerrain,m_pItemsConstruits)) //(&Builder,pTerrain,m_pItemsConstruits))
		{
			// Not valid, try to convert to absolut
			CToEquivalenceVisitor Equivalent;
			if(Equivalent.Convert(pBuilder,pTerrain,m_pItemsConstruits))//(&Builder,pTerrain,m_pItemsConstruits))
			{
				// Validate absolut
				if(!Validate.Validate(pBuilder,pTerrain,m_pItemsConstruits))//(&Builder,pTerrain,m_pItemsConstruits))
					// Not valid
					return false;
			}
			else
				// Cant convert to equivalent
				return false;
		}
		// Heres where we calculate
		CCalcValuesVisitor CalcValues;

		JBool RetCalculate = CalcValues.Calculate(pBuilder, Values, Divider, pTerrain);//(&Builder, Values, Divider, pTerrain);

	
		// Builder.Reset();
		delete pBuilder;
		return RetCalculate;

		// return CalcValues.Calculate(&Builder, Values, Divider, pTerrain);
	}
	return false;
}

// Set the item builder constructed upon the given terrain
JBool	CItemConstruit::SetItemBuilder(CTerrain * pTerrain, CIBuilder & IBuilder)
{
	// Other objects are using this item thus impossible to modify
	if(m_lReferenceCnt > 1)
		return false;

	// Validate the bbuilder object
	if(!IBuilder.IsValid())
		// The object was not valid
		return false;

	CABuilder ABuilder;
	CToAbsolutVisitor ToAbsolut;
	if(ToAbsolut.ToAbsolut(&IBuilder, &ABuilder, pTerrain))
	{
		// Set the source and terrain IDs
		m_lSourceID		=	pTerrain->GetSourceID();
		m_lTerrainID	=	pTerrain->GetTerrainID();

		std::string str;

		str = "";
		IBuilder.AsString(str);
		m_IBString	=	str.c_str();
		
		str = "";
		ABuilder.AsString(str);
		m_ABString	=	str.c_str();

		// If registered must set items construits dirty
		if(IsRegistered())
			m_pItemsConstruits->SetDirty();

		return true;

	}
	return false;
}

// Set the item builder constructed upon the given terrain
JBool	CItemConstruit::SetAbsolutBuilder(CTerrain * pTerrain, CABuilder & ABuilder)
{
	// Other objects are using this item thus impossible to modify
	if(m_lReferenceCnt > 1)
		return false;

	// Validate the bbuilder object
	if(!ABuilder.IsValid())
		// The object was not valid
		return false;

	CIBuilder IBuilder;
	CToItemVisitor ToItem;
	if(ToItem.ToItem(&ABuilder, &IBuilder, pTerrain))
	{
		// Set the source and terrain IDs
		m_lSourceID		=	pTerrain->GetSourceID();
		m_lTerrainID	=	pTerrain->GetTerrainID();

		std::string str;

		str = "";
		IBuilder.AsString(str);
		m_IBString	=	str.c_str();
		
		str = "";
		ABuilder.AsString(str);
		m_ABString	=	str.c_str();

		// If registered must set items construits dirty
		if(IsRegistered())
			m_pItemsConstruits->SetDirty();

		return true;

	}
	return false;
}

// Gets a copy of the item builder for a given terrain
JBool	CItemConstruit::GetItemBuilder(CTerrain * pTerrain, CIBuilder & IBuilder, bool ForceRepAbs) const
{
	// Check if this item was built on the same source & terrain
	// Mars 2014
	if (!ForceRepAbs)
	{
		if(m_lSourceID == pTerrain->GetSourceID() && m_lTerrainID == pTerrain->GetTerrainID() && !m_RebuildViaRepAbs)
		{
			if(!m_IBString.GetLength())
				return false;
			// Yes then build the string from the original Item Builder String
			const JChar * pstr = m_IBString.AsJCharPtr();
			if(IBuilder.FromString(&pstr) && IBuilder.IsValid() && !IBuilder.IsEmpty())
				return true;
		}
	}

	if(!m_ABString.GetLength())
		return false;
	// Build the absolut item
	CABuilder ABuilder;
	const JChar *pstr = m_ABString.AsJCharPtr();
	if(ABuilder.FromString(&pstr))
	{
		// Check if valid
		CIsValidVisitor Validate;
		if(!Validate.Validate(&ABuilder,pTerrain,m_pItemsConstruits))
			// Not valid with this terrain or has equivalent reperes
			return false;

		CToItemVisitor ToItem;
		if(ToItem.ToItem(&ABuilder, &IBuilder, pTerrain))
			return true;
	}
	return false;
}

// Gets a copy of the item builder for a given terrain
JBool	CItemConstruit::GetAbsolutBuilder(CTerrain * pTerrain, CABuilder & ABuilder) const
{
	if(!m_ABString.GetLength())
		return false;

	// Yes then build the string from the original Item Builder String
	const JChar * pstr = m_ABString.AsJCharPtr();
	if(ABuilder.FromString(&pstr))
	{
		// Check if valid
		CIsValidVisitor Validate;
		if(Validate.Validate(&ABuilder,pTerrain,m_pItemsConstruits))
			//valid with this terrain or has equivalent reperes
			return true;
	}

	return false;
}

// Check if the object is valid
JBool	CItemConstruit::IsValid() const
{
	if(!m_IBString.GetLength() && !m_ABString.GetLength())
		return false;
	return true;
}


/// Export the object avec StdioFile
JBool	CItemConstruit::Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const
{
	// First check if we have already been exported
	ExpMap.MoveTo(m_lID);
	if(ExpMap.IsValid () || m_lID == ITEM_BASE)
		return true;// Already exported

	// Check if this item has a custom owner (Palette, class, quantile
	if(m_lOwnerID > CItemsConstruits::SYSTEM_ID)
	{
		// We must export the owner first (Palette,Class,Quantile)
		ExpMap.MoveTo(m_lOwnerID);
		// Has the owner been exported ?
		if(!ExpMap.IsValid())
		{
			// Export the owner
			IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_lOwnerID);
			if(m_pIItem)
			{
				m_pIItem->Export(File,ExpMap);
				// Check if this item has been written
				ExpMap.MoveTo(m_lID);
				if(ExpMap.IsValid())
					return true;// Already written
			}
		}
	}

	ExpMap.Add(m_lID) = true;

	// Get the absolut string (modif JUIN 2014)
	if(!m_IBString.GetLength() && !(m_lSourceID == 49 && m_lTerrainID <= 514))
		return false;

	// Build the absolut item
	CIBuilder IBuilder;
	const JChar *pstr = m_IBString.AsJCharPtr();
	// Decode the string into a builder
	if(IBuilder.FromString(&pstr))
	{
		// Check and export any linked items
		CExportVisitor	ExportV;
		ExportV.Export(&IBuilder,m_pItemsConstruits,&File,&ExpMap);
	}

	// Export this item
	CString Buf;
	if(m_lID < ITEM_BASE)
		File.WriteString(_T("TEMPITEM\n"));			//	Write the Type Item
	else
		File.WriteString(_T("ITEM\n"));				//	Write the Type Item
	File.WriteString(_T("1\n"));					//	Write the Version
	Buf.Format("%s\n",m_Label.AsJCharPtr());
	File.WriteString(Buf);							//	Write the Name
	Buf.Format("%d\n",m_lID);
	File.WriteString(Buf);							//	Write the ID
	Buf.Format("%d\n",m_lOwnerID); 
	File.WriteString(Buf);							//	Write the OwnerID
	Buf.Format("%d\n",m_lSourceID); 
	File.WriteString(Buf);							//	Write the Source ID
	Buf.Format("%d\n",m_lTerrainID); 
	File.WriteString(Buf);							//	Write the Terrain ID
	Buf.Format("%s\n",m_IBString.AsJCharPtr());		//	Wrtie the IBString
	File.WriteString(Buf);							//	Write the Source ID
	Buf.Format("%s\n",m_ABString.AsJCharPtr());		//	Write the ABString
	File.WriteString(Buf);							//	Write the Source ID

	return true;
}

/// Import the objects avec StdioFile
JBool	CItemConstruit::Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap)
{
	// Export this item
	JInt32	OldId = GetID();
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
			TRACE("Item : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);
			File.ReadString(Buf);							//	Read the OwnerID
			m_lOwnerID = atol(Buf);
			File.ReadString(Buf);							//	Read the Source ID
			m_lSourceID = atol(Buf);
			File.ReadString(Buf);							//	Read the Terrain ID
			m_lTerrainID = atol(Buf);
			File.ReadString(Buf);							//	Read the IBString
			m_IBString = Buf;
			File.ReadString(Buf);							//	Read the ABString
			m_ABString = Buf;
		}
		break;

		default:
			return false;
	}

	// Build the absolut item
	CIBuilder IBuilder;
	CABuilder ABuilder;
	const JChar *pIstr = m_IBString.AsJCharPtr();
	const JChar *pAstr = m_ABString.AsJCharPtr();
	// Decode the string into a builder
	if(IBuilder.FromString(&pIstr) && ABuilder.FromString(&pAstr))
	{
		std::string strout;
		// Check and import any linked items
		CImportIVisitor	ImportIV;
		ImportIV.Import(&IBuilder,&ImpMap);
		IBuilder.AsString(strout);
		m_IBString = strout.c_str();
		CImportAVisitor	ImportAV;
		ImportAV.Import(&ABuilder,&ImpMap);
		strout = "";
		ABuilder.AsString(strout);
		m_ABString = strout.c_str();
	}

	// Check if this item is a system item (not a palette, class or quantile item)
	if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
	{
		JInt32 Id = m_pItemsConstruits->CheckExists(this);
		if(Id != 0)
		{
			ImpMap.Add(GetID()) = m_pItemsConstruits->GetIItemByID(Id);
			delete this;
			return true;
		}

		ImpMap.Add(GetID()) = this->AsIItem();

		// If this is a temporary item
		if(m_lID < ITEM_BASE)
			// Reinstate the old Id
			m_lID = OldId;
		else
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
			m_lOwnerID = 0;
			m_pItemsConstruits->RegisterItemConstruit(this,ImpMap); 
		}
	}
	else
		ImpMap.Add(GetID()) = this->AsIItem();

	return true;
}

/// Import the objects avec StdioFile
JBool	CItemConstruit::Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap,bool bTemp)
{
	// Export this item
	JInt32	OldId = GetID();
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
			TRACE("Item : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);
			File.ReadString(Buf);							//	Read the OwnerID
			m_lOwnerID = atol(Buf);
			if(bTemp)
				m_lOwnerID = CItemsConstruits::SYSTEM_ID;
			File.ReadString(Buf);							//	Read the Source ID
			m_lSourceID = atol(Buf);
			File.ReadString(Buf);							//	Read the Terrain ID
			m_lTerrainID = atol(Buf);
			File.ReadString(Buf);							//	Read the IBString
			m_IBString = Buf;
			File.ReadString(Buf);							//	Read the ABString
			m_ABString = Buf;
		}
		break;

		default:
			return false;
	}

	// Build the absolut item
	CIBuilder IBuilder;
	CABuilder ABuilder;
	const JChar *pIstr = m_IBString.AsJCharPtr();
	const JChar *pAstr = m_ABString.AsJCharPtr();
	// Decode the string into a builder
	if(IBuilder.FromString(&pIstr) && ABuilder.FromString(&pAstr))
	{
		std::string strout;
		// Check and import any linked items
		CImportIVisitor	ImportIV;
		ImportIV.Import(&IBuilder,&ImpMap);
		IBuilder.AsString(strout);
		m_IBString = strout.c_str();
		CImportAVisitor	ImportAV;
		ImportAV.Import(&ABuilder,&ImpMap);
		strout = "";
		ABuilder.AsString(strout);
		m_ABString = strout.c_str();
	}

	// Check if this item is a system item (not a palette, class or quantile item)
	if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
	{
		JInt32 Id = m_pItemsConstruits->CheckExists(this);
		if(Id != 0)
		{
			ImpMap.Add(GetID()) = m_pItemsConstruits->GetIItemByID(Id);
			delete this;
			return true;
		}

		ImpMap.Add(GetID()) = this->AsIItem();

		// If this is a temporary item
		if(bTemp || m_lID < ITEM_BASE)
			// Reinstate the old Id
			m_lID = OldId;
		else
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
			m_lOwnerID = 0;
			m_pItemsConstruits->RegisterItemConstruit(this,ImpMap); 
		}
	}
	else
	{	
		ImpMap.Add(GetID()) = this->AsIItem();
	}

	return true;
}


/// Export the object avec CArchive
JBool	CItemConstruit::Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const
{
	// First check if we have already been exported
	ExpMap.MoveTo(m_lID);
	if(ExpMap.IsValid () || m_lID == ITEM_BASE)
		return true;// Already exported

	// Check if this item has a custom owner (Palette, class, quantile
	if(m_lOwnerID > CItemsConstruits::SYSTEM_ID)
	{
		// We must export the owner first (Palette,Class,Quantile)
		ExpMap.MoveTo(m_lOwnerID);
		// Has the owner been exported ?
		if(!ExpMap.IsValid())
		{
			// Export the owner
			IItem * m_pIItem = m_pItemsConstruits->GetIItemByID(m_lOwnerID);
			if(m_pIItem)
			{
				m_pIItem->Export(Archive,ExpMap);
				// Check if this item has been written
				ExpMap.MoveTo(m_lID);
				if(ExpMap.IsValid())
					return true;// Already written
			}
		}
	}

	ExpMap.Add(m_lID) = true;

	// Get the absolut string
	if(!m_IBString.GetLength())
		return false;
	// Build the absolut item
	CIBuilder IBuilder;
	const JChar *pstr = m_IBString.AsJCharPtr();
	// Decode the string into a builder
	if(IBuilder.FromString(&pstr))
	{
		// Check and export any linked items
		CExportVisitor	ExportV;
		ExportV.Export(&IBuilder,m_pItemsConstruits,&Archive,&ExpMap);
	}

	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);

	// Export this item
	CString Buf;
	JLabel Text;
	if(m_lID < ITEM_BASE)
	{
		//	Write the Type Item
		Text = "TEMPITEM";
		Text.Send(*pstream);					
	}	
	else
	{
		//	Write the Type Item
		Text = "ITEM";
		Text.Send(*pstream);					
	}

	// Write the Version
	Text = "1";
	Text.Send(*pstream);					

	// Write the Name
	// Buf.Format("%s",m_Label.AsJCharPtr());
	Text = m_Label;
	Text.Send(*pstream);					

	// Write the ID
	Buf.Format("%d",m_lID);
	Text = Buf;
	Text.Send(*pstream);					

	// Write the OwnerID
	Buf.Format("%d",m_lOwnerID); 
	Text = Buf;
	Text.Send(*pstream);					

	// Write the Source ID
	Buf.Format("%d",m_lSourceID); 
	Text = Buf;
	Text.Send(*pstream);					

	// Write the Terrain ID
	Buf.Format("%d",m_lTerrainID); 
	Text = Buf;
	Text.Send(*pstream);					

	// Write the IBString
	Buf.Format("%s",m_IBString.AsJCharPtr());		
	Text = Buf;
	Text.Send(*pstream);					

	// Write the ABString
	Buf.Format("%s",m_ABString.AsJCharPtr());		
	Text = Buf;
	Text.Send(*pstream);					

	delete pstream;
	return true;
}

/// Import the objects avec CArchive
JBool	CItemConstruit::Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap)
{
	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);

	// Export this item
	JInt32	OldId = GetID();
	CString Buf;
	JLabel Text;

	//	Read the version
	Text.Recv(*pstream); 
	JInt32 Version = atol(Text.AsJCharPtr());
	
	switch(Version)
	{
		case 1:
		{
			// Read the Name
			Text.Recv(*pstream); 
			m_Label = Text.AsJCharPtr();

			// Read the ID
			Text.Recv(*pstream); 
			m_lID = atol(Text.AsJCharPtr());
			TRACE("Item : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Read the OwnerID
			Text.Recv(*pstream); 
			m_lOwnerID = atol(Text.AsJCharPtr());

			// Read the Source ID
			Text.Recv(*pstream); 
			m_lSourceID = atol(Text.AsJCharPtr());

			// Read the Terrain ID
			Text.Recv(*pstream); 
			m_lTerrainID = atol(Text.AsJCharPtr());

			// Read the IBString
			Text.Recv(*pstream); 
			m_IBString = Text.AsJCharPtr();

			// Read the ABString
			Text.Recv(*pstream); 
			m_ABString = Text.AsJCharPtr();
		}
		break;

		default:
			delete pstream;
			return false;
	}
	delete pstream;

	// Build the absolut item
	CIBuilder IBuilder;
	CABuilder ABuilder;
	const JChar *pIstr = m_IBString.AsJCharPtr();
	const JChar *pAstr = m_ABString.AsJCharPtr();
	// Decode the string into a builder
	if(IBuilder.FromString(&pIstr) && ABuilder.FromString(&pAstr))
	{
		std::string strout;
		// Check and import any linked items
		CImportIVisitor	ImportIV;
		ImportIV.Import(&IBuilder,&ImpMap);
		IBuilder.AsString(strout);
		m_IBString = strout.c_str();
		CImportAVisitor	ImportAV;
		ImportAV.Import(&ABuilder,&ImpMap);
		strout = "";
		ABuilder.AsString(strout);
		m_ABString = strout.c_str();
	}

	// Check if this item is a system item (not a palette, class or quantile item)
	if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
	{
		JInt32 Id = m_pItemsConstruits->CheckExists(this);
		if(Id != 0)
		{
			ImpMap.Add(GetID()) = m_pItemsConstruits->GetIItemByID(Id);
			delete this;
			return true;
		}

		ImpMap.Add(GetID()) = this->AsIItem();

		// If this is a temporary item
		if(m_lID < ITEM_BASE)
			// Reinstate the old Id
			m_lID = OldId;
		else
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
			m_lOwnerID = 0;
			m_pItemsConstruits->RegisterItemConstruit(this,ImpMap); 
		}
	}
	else
		ImpMap.Add(GetID()) = this->AsIItem();

	return true;
}

/// Import the objects avec CArchive
JBool	CItemConstruit::Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap,bool bTemp)
{
	// Stream associé à l'élmt Archive
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(Archive);

	// Export this item
	JInt32	OldId = GetID();
	CString Buf;
	JLabel Text;

	// Read the version
	Text.Recv(*pstream);							
	JInt32 Version = atol(Text.AsJCharPtr());
	
	switch(Version)
	{
		case 1:
		{
			// Read the Name
			Text.Recv(*pstream); 
			m_Label = Text.AsJCharPtr();

			// Read the ID
			Text.Recv(*pstream); 
			m_lID = atol(Text.AsJCharPtr());
			TRACE("Item : %s ,%d\n",m_Label.AsJCharPtr() ,m_lID);

			// Read the OwnerID
			Text.Recv(*pstream); 
			m_lOwnerID = atol(Text.AsJCharPtr());
			if(bTemp)
				m_lOwnerID = CItemsConstruits::SYSTEM_ID;

			// Read the Source ID
			Text.Recv(*pstream); 
			m_lSourceID = atol(Text.AsJCharPtr());

			// Read the Terrain ID
			Text.Recv(*pstream); 
			m_lTerrainID = atol(Text.AsJCharPtr());

			// Read the IBString
			Text.Recv(*pstream); 
			m_IBString = Text.AsJCharPtr();

			// Read the ABString
			Text.Recv(*pstream); 
			m_ABString = Text.AsJCharPtr();
		}
		break;

		default:
			delete pstream;
			return false;
	}
	delete pstream;

	// Build the absolut item
	CIBuilder IBuilder;
	CABuilder ABuilder;
	const JChar *pIstr = m_IBString.AsJCharPtr();
	const JChar *pAstr = m_ABString.AsJCharPtr();
	// Decode the string into a builder
	if(IBuilder.FromString(&pIstr) && ABuilder.FromString(&pAstr))
	{
		std::string strout;
		// Check and import any linked items
		CImportIVisitor	ImportIV;
		ImportIV.Import(&IBuilder,&ImpMap);
		IBuilder.AsString(strout);
		m_IBString = strout.c_str();
		CImportAVisitor	ImportAV;
		ImportAV.Import(&ABuilder,&ImpMap);
		strout = "";
		ABuilder.AsString(strout);
		m_ABString = strout.c_str();
	}

	// Check if this item is a system item (not a palette, class or quantile item)
	if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
	{
		JInt32 Id = m_pItemsConstruits->CheckExists(this);
		if(Id != 0)
		{
			ImpMap.Add(GetID()) = m_pItemsConstruits->GetIItemByID(Id);
			delete this;
			return true;
		}

		ImpMap.Add(GetID()) = this->AsIItem();

		// If this is a temporary item
		if(bTemp || m_lID < ITEM_BASE)
			// Reinstate the old Id
			m_lID = OldId;
		else
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
			m_lOwnerID = 0;
			m_pItemsConstruits->RegisterItemConstruit(this,ImpMap); 
		}
	}
	else
		ImpMap.Add(GetID()) = this->AsIItem();

	return true;
}

/// Gets an XML string version of the Item
JBool	CItemConstruit::AsXML(CString & strXML, CTerrain * pTerrain)
{
	CIBuilder IBuilder;

	if(!GetItemBuilder(pTerrain,IBuilder))
		return false;

	CToXMLIVisitor	ToXMLIV;
	return ToXMLIV.ToXML(&IBuilder,m_pItemsConstruits,pTerrain,&strXML);
}

///	Check if the ITems are system owned and the Absoloute definitions are the same
JBool	CItemConstruit::IsSimilaire(const CItemConstruit *pRight) const
{
	// If the item is deleted no test is needed
	if(m_lReferenceCnt ==0)
		return false;

	if(m_lOwnerID <= CItemsConstruits::SYSTEM_ID)
	{
		if(pRight->GetOwnerID() > CItemsConstruits::SYSTEM_ID)
			return false;
	}
	else
	{
		if(pRight->GetOwnerID() <= CItemsConstruits::SYSTEM_ID)
			return false;
	}

	return (m_ABString == pRight->m_ABString);
}

/////////////////////////////////////////////////////////////////////////////////
/// est ce un item site (bidouille >= 12000001 && <= 12999999
JBool   CItemConstruit::IsItemSite() const
{
	// Décomposition de la chaine composition item
	CString Str = this->GetIBString().AsJCharPtr();
	CString ResToken;
	int curPos= 0;

	// Info ident site après 2ème %
	ResToken = Str.Tokenize("%",curPos); 
	ResToken = Str.Tokenize("%",curPos); 
	ResToken = Str.Tokenize(")",curPos); 
	if (atoi(ResToken) >= 12000001 && atoi(ResToken) <= 12999999)
		return true;
	else
		return false;
}
