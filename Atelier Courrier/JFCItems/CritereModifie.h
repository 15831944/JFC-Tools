#pragma once

#include "ItemConstruit.h"
#include "IQuestion.h"
#include "IItem.h"
#include "CritereQuantile.h"

#define CRITEREMOD_BASE 4000000

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
class AFX_EXT_CLASS CCritereModifie : public CCritereQuantile 
{
public:

	virtual ~CCritereModifie();
	/// Combine items return true if items combined
	/// This can only be used when the object is not registered
	virtual JBool CombineItems(CTerrain *pTerrain, JLabel &Label, JList<JInt32> &ItemID);
	/// Checks if the Item is composed of one or more elements/Modalites
	virtual JBool IsComposed(CTerrain *pTerrain, JInt32 ItemID) const;
	///	Check if the Absoloute definitions are the same
	JBool	IsSimilaire(const CCritereModifie *pRight) const;

protected:

	friend class CItemsConstruitsBase;
	CCritereModifie(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp);
	/// Makes a deep copy of this object
	CCritereModifie  & DeepCopy(const CCritereModifie *rhs);
	virtual JBool ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap);

private:
	/// assignment operator
	CCritereModifie & operator =(const CCritereModifie & rhs);
	/// copy constructor
	CCritereModifie(const CCritereModifie & rhs);

public:
	/// Interface overrides
	virtual JBool						IsCritereQuantile()	const	{ return false; } ;
	virtual const CCritereQuantile *	AsCritereQuantile()	const	{ return 0; } ;
	virtual CCritereQuantile *			AsCritereQuantile()			{ return 0; } ;

	virtual JBool						IsCritereModifie()	const	{ return true; } ;
	virtual const CCritereModifie *		AsCritereModifie()	const	{ return this; } ;
	virtual CCritereModifie *			AsCritereModifie()			{ return this; } ;
	/// Sets the Label of this object
	virtual JBool	SetLabel(const JLabel & Label, JBool bForce = 0) ;

	/// Export the object via StdioFile
	virtual JBool	Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects via StdioFile
	virtual JBool	Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap);

	/// Export the object via CArchive
	virtual JBool	Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects via CArchive
	virtual JBool	Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap);

protected:
	/// Load method
	virtual JArchive &	Recv( JArchive& lhs, JInt32 Version);
	/// Save method
	virtual JArchive &	Send( JArchive& lhs ) const;
};

typedef std::auto_ptr<CCritereModifie> PTR_CRITEREMODIFIE;
typedef JMap<JInt32,PTR_CRITEREMODIFIE> CRITEREMODMAP;
