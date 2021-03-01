
#pragma once

#include "ItemConstruit.h"


#include "IQuestion.h"
#include "IItem.h"
#include "Palettes.h"

#define CRITEREQUANT_BASE 5000000

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
class AFX_EXT_CLASS CCritereQuantile : public CPalettes
{
public:

	virtual ~CCritereQuantile();
	/// Set the question bloc id
	virtual void SetQuestionBlocId(JInt32 BlocID) { m_lBlocID = BlocID; } ;
	///	Check if the Absoloute definitions are the same
	JBool	IsSimilaire(const CCritereQuantile *pRight) const;

protected:

	friend class CItemsConstruitsBase;
	CCritereQuantile(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp);
	/// Makes a deep copy of this object
	CCritereQuantile  & DeepCopy(const CCritereQuantile *rhs);
	virtual JBool ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap);

private:
	/// assignment operator
	CCritereQuantile & operator =(const CCritereQuantile & rhs);
	/// copy constructor
	CCritereQuantile(const CCritereQuantile & rhs);


public:
	/// Interface overrides
	virtual JBool						IsPalette()		const	{ return false; } ;
	virtual const CPalettes *			AsPalette()		const	{ return 0; } ;
	virtual CPalettes *					AsPalette()				{ return 0; } ;

	virtual JBool						IsCritereQuantile()	const	{ return true; } ;
	virtual const CCritereQuantile *	AsCritereQuantile()	const	{ return this; } ;
	virtual CCritereQuantile *			AsCritereQuantile()			{ return this; } ;
	virtual JInt32						GetBlocID()	const { return m_lBlocID; } ;
	/// Sets the Label of this object
	virtual JBool						SetLabel(const JLabel & Label, JBool bForce = 0) ;

    /// Export the object via StdioFile
	virtual JBool						Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects via StdioFile
	virtual JBool						Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap);

	/// Export the object via Acrhive
	virtual JBool						Export(CArchive &Archive, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects via Archive
	virtual JBool						Import(CArchive &Archive, JMap<JInt32,const IItem *> &ImpMap);

protected:
	/// Load method
	virtual JArchive&					Recv( JArchive& lhs, JInt32 Version);
	/// Save method
	virtual	JArchive&					Send( JArchive& lhs ) const;

protected:
	/// Question Bloc ID
	JInt32				m_lBlocID;
};

typedef std::auto_ptr<CCritereQuantile> PTR_CRITEREQUANTILE;
typedef JMap<JInt32,PTR_CRITEREQUANTILE> CRITEREQUANTMAP;

