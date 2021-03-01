
#pragma once
 
#include "ITerme.h"
#include "ABTerme.h"
#include "ABClass.h"
#include "ABQuant.h"
#include "ABScore.h"
#include "ABPoint.h"
#include "ABNumValue.h"
#include "IAbsolutVisitor.h"

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
class CABQuestion : public ITerme  
{
public:
	CABQuestion();
	virtual ~CABQuestion();

	/// Adds a Quant to the list
	virtual	JVoid SetQuant(CABQuant *pObject)		{ if(m_pQuant) delete m_pQuant; m_pQuant = pObject; } ;
	/// Adds a Class to the list
	virtual	JVoid SetClass(CABClass *pObject)		{ if(m_pClass) delete m_pClass; m_pClass = pObject; } ;
	/// Adds a Score to the list
	virtual	JVoid SetScore(CABScore *pObject)		{ if(m_pScore) delete m_pScore; m_pScore = pObject; } ;
	/// Adds a NumValue to the list
	virtual	JVoid SetNumValue(CABNumValue *pObject) { if(m_pNumValue) delete m_pNumValue; m_pNumValue = pObject; } ;
	/// Adds a Point to the list
	virtual	JVoid SetPoint(CABPoint *pObject)		{ if(m_pPoint) delete m_pPoint; m_pPoint = pObject; } ;

public:
	/// Overrides for the IBloc Interface
	/// Adds a terme to the terme list
	virtual	JVoid AddTerme(ITerme *pObject) { m_Terme.AddTail() = (CABTerme *) pObject; } ;
	/// Sets the object with the given ID
	virtual JBool SetID(JInt32 ID) { return true; } ;
	/// Gets the objects ID
	virtual JInt32 GetID() { return 0; } ;

	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IAbsolutVisitor &Visitor ) ;
	/// Resets to zero this object
	virtual JBool		Reset();
	/// Is this object Empty
	virtual JBool		IsEmpty();
	/// Validate this object removing all empty sub objects
	virtual	JBool		Validate();
	/// Validate this object
	virtual	JBool		IsValid() ;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const;
	/// Builds this object from a string
	virtual JBool		FromString(const JChar ** pstr);

protected:
	JList<CABTerme *>		m_Terme;
	CABClass *				m_pClass;
	CABQuant *				m_pQuant;
	CABScore *				m_pScore;
	CABNumValue *			m_pNumValue;
	CABPoint *				m_pPoint;

private:
	CABQuestion(const CABQuestion & rhs);
	CABQuestion & operator=(const CABQuestion & rhs);
};

