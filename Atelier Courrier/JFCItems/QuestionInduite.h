
#pragma once

#include "ItemConstruit.h"


#include "IQuestion.h"
#include "IItem.h"
#include "CritereQuantile.h"

#define QUESTIONIND_BASE 3000000

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
class AFX_EXT_CLASS CQuestionInduite : public CCritereQuantile
{
public:

	virtual ~CQuestionInduite();
	/// Set the question bloc id
	virtual void SetQuestionBlocId(JInt32 BlocID) { m_lBlocID = BlocID; } ;
	///	Check if the Absoloute definitions are the same
	JBool	IsSimilaire(const CQuestionInduite *pRight) const;

protected:

	friend class CItemsConstruitsBase;
	CQuestionInduite(CItemsConstruitsBase *pItemsConstruits, JInt32 ID, JBool bDeleteTemp);
	/// Makes a deep copy of this object
	CQuestionInduite  & DeepCopy(const CQuestionInduite *rhs);
	virtual JBool ReplaceImportBy(JInt32 Id, JMap<JInt32,const IItem *> &ImpMap);

private:
	/// assignment operator
	CQuestionInduite & operator =(const CQuestionInduite & rhs);
	/// copy constructor
	CQuestionInduite(const CQuestionInduite & rhs);


public:
	/// Interface overrides
	virtual JBool						IsCritereQuantile()		const	{ return false; } ;
	virtual const CCritereQuantile *	AsCritereQuantile()		const	{ return 0; } ;
	virtual CCritereQuantile *			AsCritereQuantile()				{ return 0; } ;

	virtual JBool						IsQuestionInduite()	const	{ return true; } ;
	virtual const CQuestionInduite *	AsQuestionInduite()	const	{ return this; } ;
	virtual CQuestionInduite *			AsQuestionInduite()			{ return this; } ;
	virtual JInt32						GetBlocID()	const { return m_lBlocID; } ;
	/// Sets the Label of this object
	virtual JBool						SetLabel(const JLabel & Label, JBool bForce = 0) ;

	/// Export the object via StdioFile
	virtual JBool						Export(CStdioFile &File, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects via StdioFile
	virtual JBool						Import(CStdioFile &File, JMap<JInt32,const IItem *> &ImpMap);

	/// Export the object via CArchive
	virtual JBool						Export(CArchive &File, JMap<JInt32, JBool> &ExpMap) const;
	/// Import the objects via CArchive
	virtual JBool						Import(CArchive &File, JMap<JInt32,const IItem *> &ImpMap);

protected:
	/// Load method
	virtual JArchive&					Recv( JArchive& lhs, JInt32 Version);
	/// Save method
	virtual	JArchive&					Send( JArchive& lhs ) const;

};

typedef std::auto_ptr<CQuestionInduite> PTR_QUESTIONIND;
typedef JMap<JInt32,PTR_QUESTIONIND> QUESTIONINDMAP;

