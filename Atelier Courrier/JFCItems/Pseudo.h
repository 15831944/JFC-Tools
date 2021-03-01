#pragma once
#include "IQuestion.h"
#include "ItemConstruit.h"

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
class AFX_EXT_CLASS CPseudo : public IQuestion  
{
public:
	virtual ~CPseudo();

	virtual	void    Reset();
	/// Add the given item construit to the object else returns false
	virtual JBool	AddItem			(JInt32 ID) ;
	virtual JBool	AddItems		(JList<JInt32> &VectID) ;
	/// Returns true if the item contains the given itemID
	virtual JBool	HasItem			(JInt32 ID) const;
	/// Returns true if the item contains at least one itemID
	virtual JBool	HasItems		() const;
	/// Removes the given item construit from the object else returns false
	virtual JBool	RemoveItem		(JInt32 ID) ;
	virtual JBool	RemoveItems		(JList<JInt32> &VectID) ;
	/// Get current items construits
	virtual void	GetItems(JList<JInt32> &VectID) const;
	/// Check if the object is valid with the given terrain
	virtual JBool	IsValid			(CTerrain * pTerrain) const;
	/// Sets the Label of this Pseudo item
	virtual void	SetLabel		(const JLabel & Label) { m_Label = Label; };
	/// Export the object
	virtual JBool	Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const { return false; };
	/// Import the objects
	virtual JBool	Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap) { return false; };

public:
	/// IQuestion overrides
	/// This is a Pseudo Question
	virtual JBool	IsPseudo()			const { return true; } ;
	/// Returns a const pointer to this object
	virtual const CPseudo *				AsPseudo()			const	{ return this; } ;
	/// return a pointer to this object
	virtual CPseudo *					AsPseudo()					{ return this; } ;

	virtual const JLabel &	GetLabel()	const { return m_Label; } ;
	virtual JInt32			GetID()		const { return 0; } ;
	virtual JInt32			GetBlocID()	const { return 0; } ;

	virtual JInt32 	DimensionType(JInt32 Dim)	const { if(Dim)
																		return IQuestion::NODIM;
																	else
																		return IQuestion::MODALITE;	} ;
	virtual JInt32 	HasDimensions()	const { return 1; } ;

	virtual JBool	GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID=0, JInt32 Dim=0, JUnt32 ContingentMask=0)	const ;

	virtual JBool	IsValidTerme(const JVector<JInt32> & IDTerme)	const ;
	virtual JBool	ToItemString(JStringEx &String)	const { String = ""; return true; };

	virtual JBool	CanModify() const { return true; } ;
	virtual JBool	SetModaliteOrder(const JList<JInt32> & IDOrder) ;
	/// Rename an Item returns false if name is invalid
	virtual JBool	RenameItem(JLabel &Label, JInt32 ItemID) { return false; };
	/// Pseudos are always valid
	virtual JBool	IsInContingent(JUnt32 ContingentMask) const { return true; } ;

protected:

	/// Pseudo question label
	JLabel				m_Label;
	/// Items construit
	JList<JInt32>		m_Items;

	CItemsConstruitsBase * const m_pItemsConstruits;

protected:

	friend class CItemsConstruitsBase;
	CPseudo(CItemsConstruitsBase * pItemsConstruits);

private:
	/// assignment operator
	CPseudo & operator =(const CPseudo & rhs);
	/// copy constructor
	CPseudo(const CPseudo & rhs);

};
