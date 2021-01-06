
#pragma once


class CTerrain;
class CItemConstruit;
class CPalettes;
class CQuestionInduite;
class CCritereModifie;
class CCritereQuantile;
class CCritereScore;
class CItemPondere;
class CPseudo;
class CStdioFile;

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
class AFX_EXT_CLASS IItem
{
public:
	IItem();
	virtual ~IItem();

public:
	/// IItem interface overloads
	virtual const IItem *				AsIItem()			const	{ return this; } ;
	virtual IItem *						AsIItem()					{ return this; } ;
	virtual const CItemConstruit *		AsItemConstruit()	const	{ return 0; } ;
	virtual CItemConstruit *			AsItemConstruit()			{ return 0; } ;
	virtual const CPseudo *				AsPseudo()			const	{ return 0; } ;
	virtual CPseudo *					AsPseudo()					{ return 0; } ;
	virtual const CPalettes *			AsPalette()			const	{ return 0; } ;
	virtual CPalettes *					AsPalette()					{ return 0; } ;
	virtual const CQuestionInduite *	AsQuestionInduite()		const	{ return 0; } ;
	virtual CQuestionInduite *			AsQuestionInduite()				{ return 0; } ;
	virtual const CCritereModifie *		AsCritereModifie()	const	{ return 0; } ;
	virtual CCritereModifie *			AsCritereModifie()			{ return 0; } ;
	virtual const CCritereQuantile *	AsCritereQuantile()	const	{ return 0; } ;
	virtual CCritereQuantile *			AsCritereQuantile()			{ return 0; } ;
	virtual const CCritereScore *		AsCritereScore()	const	{ return 0; } ;
	virtual CCritereScore *				AsCritereScore()			{ return 0; } ;

	virtual const CItemPondere *		AsItemPondere()		const	{ return 0; } ;
	virtual CItemPondere *				AsItemPondere()				{ return 0; } ;

	/// Check if the object is valid with the given terrain
	virtual JBool			IsValid(CTerrain * pTerrain) const = 0;
	/// Add the given item construit to the object else returns false
	virtual JBool						AddItem(CItemConstruit * pItem, JFlt64 Coeff = 0.0) = 0;

	/// Returns true if the item contains the given itemID
	virtual JBool			HasItem(JInt32 ID) const = 0;
	/// Removes the given item construit from the object else returns false
	virtual JBool			RemoveItem(JInt32 ID) = 0;
	/// Test if the object is registered
	virtual JBool			IsRegistered() const = 0;
	/// Test if object can be edited
	virtual JBool			CanModify() const = 0;
	/// Get objects label
	virtual const JLabel &	GetLabel()	const =0;
	/// Gets the objects Id
	virtual JInt32			GetID()		const =0;

	/// Sets the Label of this object
	virtual JBool			SetLabel(const JLabel & Label, JBool bForce = false) = 0;
	/// Returns True if this object is an Item Construit
	virtual JBool			IsItemConstruit()	const { return false; } ;
	/// Returns True if this object is a Pseudo
	virtual JBool			IsPseudo()			const { return false; } ;
	/// Returns True if this object is a Palette
	virtual JBool			IsPalette()			const { return false; } ;
	/// Returns True if this object is a Critere Induite
	virtual JBool			IsQuestionInduite()		const { return false; } ;
	/// Returns True if this object is a Critere Modifie
	virtual JBool			IsCritereModifie()	const { return false; } ;
	/// Returns True if this object is a Critere Quantile
	virtual JBool			IsCritereQuantile()	const { return false; } ;
	/// Returns True if this object is a Critere Score
	virtual JBool						IsCritereScore()	const { return false; } ;

	/// Returns True if this object is a Ponderate Item
	virtual JBool						IsItemPondere()	const { return false; } ;

	/// Export the object avec CStdioFile
	virtual JBool			Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const = 0;
	/// Import the object CStdioFile
	virtual JBool			Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap) = 0;

	/// Export the object avec CArchive
	virtual JBool			Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const = 0;
	/// Import the object avec CArchive 
	virtual JBool			Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap) = 0;

	/// Get the Item owner ID
	virtual JInt32			GetOwnerID() const { return m_lOwnerID; } ;

protected:
	/// IItem interface overloads

	/// Registers the object to the Owner ID with the new ID
	virtual JBool			Register(JInt32 OwnerID, JInt32 ID) = 0;
	/// tests if the caller can unregister the object
	virtual JBool			CanUnRegister(JInt32 OwnerID=0, JMap<JInt32, JInt32> * pMapItems = 0) = 0;
	/// Unregisters the object
	virtual JBool			UnRegister(JInt32 OwnerID=0) = 0;
	/// Load method
	virtual JArchive &		Recv( JArchive& lhs, JInt32 Version) = 0;
	/// Save method
	virtual JArchive &		Send( JArchive& lhs ) const = 0;

protected:

	/// The ID of the owning object
	JInt32	m_lOwnerID;

};
