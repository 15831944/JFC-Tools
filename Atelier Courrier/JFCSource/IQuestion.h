
#pragma once

#include "IModalite.h"

class CQuestion;
class CPalettes;
class CQuestionInduite;
class CCritereModifie;
class CCritereQuantile;
class CCritereScore;
class CPseudo;
class CBlocQuestions;


/**
 * \ingroup JFCSOURCE EXTERNAL
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
class AFX_EXT_CLASS IQuestion  
{

public:
	typedef enum DIMTYPE
	{
		NODIM = 0,
		MODALITE = 1,
		ELEMENT	= 2
	};

	IQuestion();
	virtual ~IQuestion();

	/// Gets a constant IQuestion pointer from this object
	virtual const IQuestion *	AsIQuestion() const { return this; } ;
	/// Gets an IQuestion pointer from this object
	virtual IQuestion *			AsIQuestion()		{ return this; } ;

	/// Returns True if this object is a Pseudo
	virtual JBool	IsPseudo()			const { return false; } ;
	/// Returns True if this object is a Question
	virtual JBool	IsQuestion()		const { return false; } ;
	/// Returns True if this object is a Palette
	virtual JBool	IsPalette()			const { return false; } ;
	/// Returns True if this object is a QuestionInduite
	virtual JBool	IsQuestionInduite()		const { return false; } ;
	/// Returns True if this object is a Critere Modifie
	virtual JBool	IsCritereModifie()	const { return false; } ;
	/// Returns True if this object is a Critere Quantile
	virtual JBool	IsCritereQuantile()	const { return false; } ;
	/// Returns True if this object is a Critere Score
	virtual JBool	IsCritereScore()	const { return false; } ;
	/// Returns True if this object is a Bloc Questions
	virtual JBool	IsBlocQuestion()	const { return false; } ;

	virtual const CPseudo *				AsPseudo()			const	{ return 0; } ;
	virtual CPseudo *					AsPseudo()					{ return 0; } ;
	virtual const CQuestion *			AsQuestion()		const	{ return 0; } ;
	virtual CQuestion *					AsQuestion()				{ return 0; } ;
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
	virtual const CBlocQuestions *		AsBlocQuestions()	const	{ return 0; } ;
	virtual CBlocQuestions  *			AsBlocQuestions()			{ return 0; } ;
	/// Returns the object Label
	virtual const JLabel &	GetLabel()	const =0;
	/// Return the object ID
	virtual JInt32			GetID()		const =0;
	/// Returns the objects Bloc ID
	virtual JInt32			GetBlocID()	const =0;
	/// Returns the Objects Contingent mask
	virtual JInt32			GetContingentMask()	const { return 0xFFFF; } ;
	/// Returns True if the objects has Modalites
	virtual JInt32 			HasDimensions()	const =0 ;
	/// Returns the dimension type
	virtual JInt32			DimensionType(JInt32 Dim)	const =0 ;
	/// Gets the Modalites
	virtual JBool	GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID=0, JInt32 Dim=0, JUnt32 ContingentMask=0)	const =0 ;
	///// Returns true if the Terme is valid
	virtual JBool	IsValidTerme(const JVector<JInt32> & IDTerme)		const =0 ;
	/// Transforms the object into a string form
	virtual JBool	ToItemString(JStringEx &String)						const =0 ;
	/// Returns true if this object can be ordered
	virtual JBool	CanModify() const =0 ;
	/// Returns True if the modalites have been reordered
	virtual JBool	SetModaliteOrder(const JList<JInt32> & IDOrder) =0 ;
	/// Rename an Item returns false if name is invalid
	virtual JBool RenameItem(JLabel &Label, JInt32 ItemID) = 0;
	/// Returns True if this object is in the contingent
	virtual JBool	IsInContingent(JUnt32 ContingentMask) const = 0;

};
