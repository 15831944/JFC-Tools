
#pragma once


#include "ItemConstruit.h"

#include "IQuestion.h"
#include "IItem.h"

#include <list>

#define PALETTES_BASE 2000000

/**
 * \ingroup JFCITEMS EXTERNAL
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CPalettes : public IQuestion, public IItem
{

public:
	virtual ~CPalettes();
	/// Gets the Palettes Item IDs
	virtual void		GetItems(JList<JInt32> &Items) const;
	/// Gets the Palettes Item IDs
	virtual void		GetItems(JList<const CItemConstruit *> &Items) const;
	/// Gets the Palettes Item IDs
	virtual void		GetItems(std::list<const JInt32> & Items) const;
	/// Gets the Palettes Item IDs
	virtual void		GetItems(std::list<const CItemConstruit *> & Items) const;
	///	Check if the Absoloute definitions are the same
	virtual JBool		IsSimilaire(const CPalettes *pRight) const;

protected:
	friend class CItemsConstruitsBase;
	CPalettes(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp);
	/// Add the Item ID to the item list
	virtual JBool		AddID(JInt32 ID);
	/// Removes the item ID from the item list
	virtual JBool		RemoveID(JInt32 ID);
	/// Makes a deep copy of this object
	CPalettes & DeepCopy(const CPalettes *rhs);
	virtual JBool ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap);

private:
	/// assignment operator
	CPalettes & operator =(const CPalettes & rhs);
	/// copy constructor
	CPalettes(const CPalettes & rhs);

protected:
	/// Repere de la palette
	JInt32							m_lID;
	/// Palettes label	
	JLabel							m_Label;
	/// Items construit
	JList<JInt32>					m_Items;
	/// const pointer to items construit
	CItemsConstruitsBase * const			m_pItemsConstruits;
	/// Map of temporary items construit
	JMap<JInt32, CItemConstruit *>	m_TempItems;
	/// Flag for deleting temporary objects
	JBool							m_bDeleteTemp;

public:
	/// interface overloads
	virtual JBool				IsPalette()					const	{ return true; } ;
	virtual const CPalettes *	AsPalette()					const	{ return this; } ;
	virtual CPalettes *			AsPalette()							{ return this; } ;
	virtual const JLabel &		GetLabel()					const	{ return m_Label; } ;
	virtual JInt32				GetID()						const	{ return m_lID; } ;

	/// Gets the bloc ID
	virtual JInt32				GetBlocID()					const { return 0; } ;
	virtual JInt32 				HasDimensions()				const { return 1; } ;
	virtual JInt32			 	DimensionType(JInt32 Dim)	const { if(Dim)
																		return IQuestion::NODIM;
																	else
																		return IQuestion::MODALITE;	} ;

	virtual JBool				GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID=0, JInt32 Dim=0, JUnt32 ContingentMask=0)	const ;
	virtual JBool				IsValidTerme(const JVector<JInt32> & IDTerme)	const ;
	virtual JBool				ToItemString(JStringEx &String)	const { String = ""; return true; };
	virtual JBool				CanModify() const { return true; } ;
	virtual JBool				SetModaliteOrder(const JList<JInt32> & IDOrder) ;
	/// Rename an Item returns false if name is invalid
	virtual JBool				RenameItem(JLabel &Label, JInt32 ItemID);
	/// Palettes are always valid 
	virtual JBool				IsInContingent(JUnt32 ContingentMask) const { return true; } ;
	/// Check if the object is valid with the given terrain
	virtual JBool				IsValid(CTerrain * pTerrain) const ;
	/// Add the given item construit to the object else returns false
	virtual JBool				AddItem(CItemConstruit * pItem, JFlt64 Coeff = 0.0) ;
	/// Returns true if the item contains the given itemID
	virtual JBool				HasItem(JInt32 ID) const;
	/// Removes the given item construit from the object else returns false
	virtual JBool				RemoveItem(JInt32 ID) ;
	/// Test if the object is registered
	virtual JBool				IsRegistered() const { return m_lOwnerID >= 1; } ;
	virtual void				ResetOwnerID() { m_lOwnerID = 0; } ;

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

	// Palette formée ou non de composants quantitatifs
	virtual JBool				IsPaletteQuantitative();


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

typedef std::auto_ptr<CPalettes> PTR_PALETTES;
typedef JMap<JInt32, PTR_PALETTES> PALETTEMAP;
