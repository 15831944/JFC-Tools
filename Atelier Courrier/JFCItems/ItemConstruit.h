
#pragma once


#pragma warning( disable : 4786 )

#include <memory>

#include "BitArray.h"
#include "IModalite.h"
#include "IItem.h"
#include "IBuilder.h"
#include "ABuilder.h"
#include "CalcItemParams.h"

class CItemsConstruitsBase;

// #include "JMFCStream.h"

#define ITEM_BASE 1000000

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
class AFX_EXT_CLASS CItemConstruit : public IModalite, public IItem
{
public:

	virtual ~CItemConstruit();
	/// Load method
	JArchive&	Recv( JArchive& lhs, JInt32 Version );
	/// Save method
	JArchive&	Send( JArchive& lhs ) const;
	/// Set the item builder constructed upon the given terrain
	JBool		SetItemBuilder(CTerrain * pTerrain, CIBuilder & IBuilder);
	/// Set the absolut builder constructed upon the given terrain
	JBool		SetAbsolutBuilder(CTerrain * pTerrain, CABuilder & ABuilder);
	/// Gets a copy of the item builder for a given terrain
	JBool		GetItemBuilder(CTerrain * pTerrain, CIBuilder & IBuilder, bool ForceRepAbs = false) const;
	/// Gets a copy of the absolut builder for a given terrain
	JBool		GetAbsolutBuilder(CTerrain * pTerrain, CABuilder & ABuilder) const;
	/// Calculate the Item
	/// Inputs Terrain
	/// Outputs vector of individus and the EchantillonID
	JBool		Calculate(CCalcItemParams * pParams) const;
	/// Calculates the Item
	/// Inputs Terrain and StatisticID
	/// Outputs the Effectif/Tot NbCas/Tot
	JBool		CalculateEffectif(CCalcItemParams * pParams) const;
	/// Inputs Terrain and vector of values
	/// This must be used only with one segment on question
	/// Outputs the vector of values
	JBool		CalcQuantiValues(CTerrain * pTerrain, JVector<JFlt32> &Values, JInt32 & Divider) const;
	/// Check if the object is valid
	JBool		IsValid() const ;
	/// Check if the given Source and Terrain is the Souche for this Item
	JBool		IsSouche(JInt32 SourceID, JInt32 TerrainID) const { return  (m_lSourceID == 0) || (SourceID==m_lSourceID && TerrainID==m_lTerrainID); } ;
	JBool		IsSouche(JInt32 SourceID) const { return (m_lSourceID == 0) || (SourceID==m_lSourceID); } ;
	///	Check if the Absoloute definitions are the same
	JBool		IsSimilaire(const CItemConstruit *pRight) const;

	// New Juin 2014 (ne sert ps pour le moment)
	JInt32		GetIDSource()				const { return m_lSourceID;; } ;
	JInt32		GetIDTerrain()				const { return m_lTerrainID;; } ;


public:
	/// IModalite interface overrides

	/// Return a pointer to this const object
	virtual const CItemConstruit *	AsItemConstruit() const { return this; } ;

	/// Return a pointer to this object
	virtual CItemConstruit *		AsItemConstruit() { return this; } ;

	/// Test if this object is an Item Construit
	virtual JBool					IsItemConstruit()	const { return true; } ;

	/// Get the objects label
	virtual const JLabel &			GetLabel()			const { return m_Label; } ;

	/// Get the Categorie item
	virtual JInt32 					GetCategorie()		{ return m_Categorie; } ;

	/// Get the objects ID
	virtual JInt32					GetID()				const { return m_lID; } ;

	/// Convert the objects id to an Item string
	virtual JBool					ToItemString(JStringEx &String) const ;

	/// Item construits are always valid
	virtual JBool					IsInContingent(JUnt32 ContingentMask) const { return true; } ;

	/// Recupere Item Builder String format
	virtual const JStringEx	&		GetIBString()		const {return m_IBString; } ;

	/// Recupere Item Builder String format
	virtual const JStringEx	&		GetABString()		const {return m_ABString; } ;

	// Récupère l'élmément CItemsConstruits
	virtual const CItemsConstruitsBase *GetItemsConstruits() const {return m_pItemsConstruits; };

	// Set if Item Moyennable
	virtual void 					SetMoyennable(JBool Moyennable)		{ m_Moyennable = Moyennable; } ;

	/// Get if item moyennable
	virtual const JBool 			GetMoyennable() const	{ return m_Moyennable; } ;

	virtual void					SetRebuildViaRepAbs(JBool Rebuild)		{ m_RebuildViaRepAbs = Rebuild; } ;  

public:
	/// IItem interface overloads

	/// Check if the object is valid with the given terrain
	virtual JBool	IsValid			(CTerrain * pTerrain) const ;
	/// Add the given item construit to the object else returns false
	virtual JBool	AddItem			(CItemConstruit * pItem, JFlt64 Coeff = 0.0) { return false; } ;
	/// Returns true if the item contains the given itemID
	virtual JBool	HasItem			(JInt32 ID) const;
	/// Removes the given item construit from the object else returns false
	virtual JBool	RemoveItem		(JInt32 ID) { return false; };
	/// Test if the object is registered
	virtual JBool	IsRegistered	() const { return m_lReferenceCnt >= 1; }; //;return m_lOwnerID >= 1; } ;
	/// Test if object can be edited
	virtual JBool	CanModify		() const { return (m_lOwnerID != 10); } ;
	/// Sets the Label of this object
	virtual JBool	SetLabel		(const JLabel & Label, JBool bForce = 0) ;

	/// Sets la catégorie
	virtual JBool	SetCategorie	(JInt32 Categorie) ;

	/// Export the object avec CStdioFile
	virtual JBool	Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects avec CStdioFile
	virtual JBool	Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap);
	virtual JBool	Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap, bool bTemp);

	/// Export the object avec CArchive
	virtual JBool	Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const;

	/// Import the objects avec CArchive
	virtual JBool	Import(CArchive &File, JMap<JInt32,const IItem *> &ImpMap);
	virtual JBool	Import(CArchive &File, JMap<JInt32,const IItem *> &ImpMap, bool bTemp);

	/// Gets an XML string version of the Item
	virtual JBool	AsXML(CString & strXML, CTerrain * pTerrain);

	/// est ce un item site (bidouille >= 12000001 && <= 12999999
	virtual JBool   IsItemSite() const ;

	virtual JBool	RefCountAZero();
	virtual JBool	RefCountAUn();

protected:
	/// IItem interface overloads

	/// Registers the object to the Owner ID with the new ID
	virtual JBool	Register		(JInt32 OwnerID, JInt32 ID) ;
	/// tests if the caller can unregister the object
	virtual JBool	CanUnRegister	(JInt32 OwnerID=0, JMap<JInt32, JInt32> * pMapItems = 0) ;

	/// Unregisters the object
	virtual JBool	UnRegister		(JInt32 OwnerID=0) ;

protected:

	friend class CItemsConstruitsBase ;
	friend class CRegisterItemVisitor;
	friend class CUnRegisterItemVisitor;

	CItemConstruit(CItemsConstruitsBase *pItemsConstruits, JInt32 ID);

public:
	CItemConstruit & DeepCopy(const CItemConstruit *rhs);

private:

	/// ID of this item construit
	JInt32		m_lID;
	/// Reference count
	JInt32		m_lReferenceCnt;
	/// Id of the base source
	JInt32		m_lSourceID;
	/// Id of the base terrain
	JInt32		m_lTerrainID;
	/// Item Label
	JLabel		m_Label;
	/// Item Categorie
	JInt32		m_Categorie;
	/// Item Builder String format
	JStringEx	m_IBString;
	/// Absolut Builder String format
	JStringEx	m_ABString;

	// Item moyennable ou pas
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	JBool		m_Moyennable;

	// Item à reconstruire d'office avec les reperes absolus (fait pour Atelier Courrier (Souce == 49 && Id Terrain <= 514)
	JBool		m_RebuildViaRepAbs;

	/*
	// Type item quanti (0 :QUANTI_PURE / quantité cumulable exemple nb voyages // 1: QUANTI_ASS / non cumulable exemple age)
	(CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure)
	*/

	CItemsConstruitsBase *const m_pItemsConstruits;
	
private:
	/// assignment operator
	CItemConstruit & operator =(const CItemConstruit & rhs);
	/// copy constructor
	CItemConstruit(const CItemConstruit & rhs);
};

typedef std::auto_ptr<CItemConstruit> PTR_ITEMCONSTRUIT;
typedef JMap<JInt32,PTR_ITEMCONSTRUIT> ITEMCONSTRUITMAP;

