//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe permettant de définir des items pondérés
// - chaque item sera défini par rapport à un item de base, et celui ci sera associé à une liste
// - d'items avec une pondération
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ItemConstruit.h"
#include "IItem.h"

class  CItemsConstruitsBase;

#define ITEMPONDERE_BASE 7000000

class AFX_EXT_CLASS CItemPondere : public IItem //public IQuestion, 
{
public:
	// Constructeur
	virtual ~CItemPondere();

	// Récupération ID item de base de l'item pondéré 
	virtual JInt32		GetIDItemBase();

	// Récupération item de base de l'item pondéré 
	virtual void		GetItemBase(const CItemConstruit * pItemBase) const;

	/// Gets the Ponderates Item IDs
	// virtual void		GetIDItems(JMap<JInt32, JFlt64> &ItemsPonderes);
	virtual void        GetIDItems(JList<JInt32> &IDItemsCompos, JList<JFlt64> &CoeffItemsCompos);

	// Modifie la valeur pondération d'un composant
	virtual void		ModifPonder(JUnt32 IdCompo, JFlt64 NewCoeff);

	/// Gets the Ponderates Item IDs
	void		GetItems(JList<const CItemConstruit *> &Items, JList <JFlt64> &PonderItems) const;

	///	Check if the Absoloute definitions are the same
	virtual JBool		IsSimilaire(const CItemPondere *pRight) const;

protected:
	friend class CItemsConstruitsBase;

	// Constructeur avec Ident Item Pondéré
	CItemPondere(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp);

	/// Add the Item ID de base 
	virtual JBool		AddIDBase(JInt32 ID);

	/// Removes the Item ID  de base
	// virtual JBool		RemoveIDBase(JInt32 ID);

	/// Add the Composant Item ID to the item list
	virtual JBool		AddIDList(JInt32 ID, JFlt64 Ponder);

	/// Removes the item ID from the item list
	virtual JBool		RemoveIDList(JInt32 ID);

	/// Makes a deep copy of this object
	CItemPondere &		DeepCopy(const CItemPondere *rhs);

	// Reconstruction de l'item pondéré origine
	virtual JBool		ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap);

private:
	/// assignment operator
	CItemPondere & operator =(const CItemPondere & rhs);

	/// copy constructor
	CItemPondere(const CItemPondere & rhs);

protected:
	/// Repere de l'item pondéré
	JInt32							m_lID;

	/// Ponderate Item Label	
	JLabel							m_Label;

	/// Ident de l'item de base
	JInt32							m_IDItemBase;

	/// Liste des items avec pondérations  (les items utilisés pour pondérer l'item de base)
	// JMap<JInt32, JFlt64>			m_ItemsPonderes;

	// Liste des ids composants + liste des coeffs
	JList <JInt32>					m_IDItemsCompos;
	JList <JFlt64>					m_CoeffItemsCompos;

	/// const pointer to items construit
	CItemsConstruitsBase *const		m_pItemsConstruits;

	/// Map of temporary items construit
	JMap<JInt32, CItemConstruit *>	m_TempItems;

	/// Flag for deleting temporary objects
	JBool							m_bDeleteTemp;

public:
	/// Est-ce un itm pondéré ???
	virtual JBool					IsItemPondere()				const	{ return true; } ;

	// Récup item pondéré
	virtual const CItemPondere *	AsItemPondere()				const	{ return this; } ;
	virtual CItemPondere *			AsItemPondere()						{ return this; } ;

	// Récup label de l'item pondéré
	virtual const JLabel &			GetLabel()					const	{ return m_Label; } ;

	// Récup ident de l'item pondéré
	virtual JInt32					GetID()						const	{ return m_lID; } ;

	/// Gets the bloc ID
	virtual JInt32					GetBlocID()					const { return 0; } ;
	virtual JInt32 					HasDimensions()				const { return 1; } ;
	virtual JInt32			 		DimensionType(JInt32 Dim)	const { if(Dim)
																			return IQuestion::NODIM;
																		else
																			return IQuestion::MODALITE;
																      } ;
	
	virtual JBool				ToItemString(JStringEx &String)	const { String = ""; return true; };

	// Peut-on le modifier ???
	virtual JBool				CanModify() const { return true; } ;

	/// Rename an Item returns false if name is invalid
	virtual JBool				RenameItem(JLabel &Label, JInt32 ItemID);

	/// Item Pondéré are always valid 
	virtual JBool				IsInContingent(JUnt32 ContingentMask) const { return true; } ;

	/// Check if the object is valid with the given terrain
	virtual JBool				IsValid(CTerrain * pTerrain) const ;

	/// Add the given item construit to the object else returns false
	virtual JBool				AddItemBase(CItemConstruit * pItem) ;

	/// Add the given item construit to the object else returns false
	virtual JBool				AddItem(CItemConstruit * pItem, JFlt64 Ponder) ;

	/// Returns true if the item contains the given itemID
	virtual JBool				HasItem(JInt32 ID) const;

	/// Removes the given item construit from the object else returns false
	virtual JBool				RemoveItem(JInt32 ID) ;

	/// Test if the object is registered
	virtual JBool				IsRegistered() const { return m_lOwnerID >= 1; } ;

	/// Sets the Label of this object
	virtual JBool				SetLabel(const JLabel & Label, JBool bForce = 0) ;

	/// Return false if Label exists already
	virtual JBool				CheckLabel(JLabel &Label) const ;

	/// Export the object avec CStdioFile
	virtual JBool				Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects avec CStdioFile
	virtual JBool				Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap);

	/// Export the object avec CArchive
	virtual JBool				Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects  avec CArchive
	virtual JBool				Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap);

protected:

	/// Registers the object to the Owner ID with the new ID
	virtual JBool				Register(JInt32 OwnerID, JInt32 ID) ;

	/// Registers the object to the Owner ID with the new ID
	virtual JBool				Register(JInt32 OwnerID, JMap<JInt32, const IItem *> &ImpMap, JInt32 ID) ;

	/// tests if the caller can unregister the object
	virtual JBool				CanUnRegister(JInt32 OwnerID=0, JMap<JInt32, JInt32> * pMapItems = 0) ;

	/// Unregisters the object
	virtual JBool				UnRegister(JInt32 OwnerID=0) ;

	/// Load method
	virtual JArchive&			Recv( JArchive& lhs, JInt32 Version);

	/// Save method
	virtual	JArchive&			Send( JArchive& lhs ) const;
};

// Structure ensemble des items pondérés
typedef std::auto_ptr<CItemPondere> PTR_ITEMSPONDERES;
typedef JMap<JInt32, PTR_ITEMSPONDERES> ITEMPONDERMAP;

