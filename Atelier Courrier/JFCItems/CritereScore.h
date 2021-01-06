
#pragma once


#include "ItemConstruit.h"

class  CItemsConstruitsBase;

#include "IQuestion.h"
#include "IItem.h"

#define CRITERESCORE_BASE 6000000

/**
 * \ingroup JFCITEMS
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
class AFX_EXT_CLASS CCritereScore : public IQuestion, public IItem
{
public:

	virtual ~CCritereScore();

public:

	virtual JBool	IsCritereScore()	const { return true; } ;
	virtual const CCritereScore *		AsCritereScore()	const	{ return this; } ;
	virtual CCritereScore *				AsCritereScore()			{ return this; } ;

	virtual const JLabel &	GetLabel()	const { return m_Label; } ;
	virtual JInt32			GetID()		const { return m_lID; } ;
	virtual JInt32			GetBlocID()	const { return 0; } ;

	virtual JInt32 	DimensionType(JInt32 Dim)	const { if(Dim)
																		return IQuestion::NODIM;
																	else
																		return IQuestion::MODALITE;	} ;
	virtual JInt32 	HasDimensions()	const { return 1; } ;

	virtual JBool	GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID=0, JInt32 Dim=0, JUnt32 ContingentMask=0)	const ;

	virtual JBool	IsValidTerme(const JVector<JInt32> & IDTerme)	const ;
	virtual JBool	ToItemString(JStringEx &String)	const { String = ""; return true; };

	virtual JBool	CanModify() const { return false; } ;
	virtual JBool	SetModaliteOrder(const JList<JInt32> & IDOrder) { return false; } ;
	/// Rename an Item returns false if name is invalid
	virtual JBool RenameItem(JLabel &Label, JInt32 ItemID) { return false; } ;
	/// Critere scores are always valid
	virtual JBool	IsInContingent(JUnt32 ContingentMask) const { return true; } ;

public:
	/// IItem interface overloads

	/// Check if the object is valid with the given terrain
	virtual JBool	IsValid			(CTerrain * pTerrain) const ;
	/// Add the given item construit to the object else returns false
	virtual JBool	AddItem			(CItemConstruit * pItem, JFlt64 Coeff = 0.0) ;
	/// Returns true if the item contains the given itemID
	virtual JBool	HasItem			(JInt32 ID) const;
	/// Removes the given item construit from the object else returns false
	virtual JBool	RemoveItem		(JInt32 ID) ;
	/// Test if the object is registered
	virtual JBool	IsRegistered	() const { return m_lOwnerID >= 1; } ;
	/// Sets the Label of this object
	virtual JBool	SetLabel		(const JLabel & Label, JBool bForce = 0) ;

	/// Export the object avec CStdioFile
	virtual JBool	Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const { return false; };
	/// Import the objects avec CStdioFile
	virtual JBool	Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap) { return false; };

	/// Export the object avec CArchive
	virtual JBool	Export(CArchive &File, JMap<JInt32, JBool> &ExpMap) const { return false; };
	/// Import the objects  avec CArchive
	virtual JBool	Import(CArchive &File, JMap<JInt32,const IItem *> &ImpMap) { return false; };

protected:
	/// IItem interface overloads

	/// Registers the object to the Owner ID with the new ID
	virtual JBool	Register		(JInt32 OwnerID, JInt32 ID) ;
	/// Registers the object to the Owner ID with the new ID
	virtual JBool	Register		(JInt32 OwnerID, JMap<JInt32, const IItem *> &ImpMap, JInt32 ID) ;
	/// tests if the caller can unregister the object
	virtual JBool	CanUnRegister	(JInt32 OwnerID=0, JMap<JInt32, JInt32> * pMapItems = 0) ;
	/// Unregisters the object
	virtual JBool	UnRegister		(JInt32 OwnerID=0) ;

public:

	/// Load method
	JArchive&	Recv( JArchive& lhs, JInt32 Version);
	/// Save method
	JArchive&	Send( JArchive& lhs ) const;

protected:

	/// Repere de la CritereScore
	JInt32				m_lID;
	/// Index de rangement
	JInt32				m_lIndex;
	/// CritereScore label
	JLabel				m_Label;
	/// Items construit
	JList<JInt32>		m_Items;

	CItemsConstruitsBase *const m_pItemsConstruits;

	/// Map of temporary items construit
	JMap<JInt32, CItemConstruit *>	m_TempItems;

	/// Flag for deleting temporary objects
	JBool				m_bDeleteTemp;

protected:

	friend class CItemsConstruitsBase;
	CCritereScore(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp);

	/// Add the Item ID to the item list
	JBool	AddID(JInt32 ID);
	/// Removes the item ID from the item list
	JBool	RemoveID(JInt32 ID);
	/// Makes a deep copy of this object
	CCritereScore  & DeepCopy(const CCritereScore *rhs);

private:
	/// assignment operator
	CCritereScore & operator =(const CCritereScore & rhs);
	/// copy constructor
	CCritereScore(const CCritereScore & rhs);
};

typedef std::auto_ptr<CCritereScore> PTR_CRITERESCORE;
typedef JMap<JInt32,PTR_CRITERESCORE> CRITERESCOREMAP;

