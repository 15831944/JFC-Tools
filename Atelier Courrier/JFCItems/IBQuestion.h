
#pragma once

#include "IBloc.h"
#include "IBModalite.h"
#include "IBClass.h"
#include "IBQuant.h"
#include "IBScore.h"
#include "IBNumValue.h"
#include "IBPoint.h"
#include "IItemVisitor.h"

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
class AFX_EXT_CLASS CIBQuestion : public IBloc  
{
public:
	CIBQuestion();
	virtual ~CIBQuestion();
	JInt32	GetID() { return m_ID; } ;
	JBool	SetID(JInt32 ID) { m_ID = ID; return !ID; } ;

	/// Adds a Quant to the list
	virtual	JVoid SetQuant(CIBQuant *pObject)		{ if(m_pQuant) delete m_pQuant; m_pQuant = pObject; } ;
	/// Adds a Class to the list
	virtual	JVoid SetClass(CIBClass *pObject)		{ if(m_pClass) delete m_pClass; m_pClass = pObject; } ;
	/// Detach the Class from the list
	virtual	CIBClass * DetachClass()				{ CIBClass * pClass = m_pClass; m_pClass = 0; return (pClass); } ;
	/// Adds a Score to the list
	virtual	JVoid SetScore(CIBScore *pObject)		{ if(m_pScore) delete m_pScore; m_pScore = pObject; } ;
	/// Adds a NumValue to the list
	virtual	JVoid SetNumValue(CIBNumValue *pObject) { if(m_pNumValue) delete m_pNumValue; m_pNumValue = pObject; } ;

	/// Detach the NumValue from the list
	virtual	CIBNumValue * DetachNumValue()			{ CIBNumValue * pNumValue = m_pNumValue; m_pNumValue = 0; return (pNumValue); } ;
	/// Adds a Point to the list
	virtual	JVoid SetPoint(CIBPoint *pObject)		{ if(m_pPoint) delete m_pPoint; m_pPoint = pObject; } ;
	/// Adds a Modalite to the list
	virtual JVoid AddModalite(CIBModalite *pObject) { m_Modalite.AddTail() = pObject; } ;
	/// Resets the modalities list
	virtual JVoid ResetModalites();
	/// Gets a Quant
	virtual	CIBQuant * GetQuant()					{ return m_pQuant; } ;
	/// Gets a Class
	virtual	CIBClass * GetClass()					{ return m_pClass; } ;
	/// Gets a Score
	virtual	CIBScore * GetScore()					{ return m_pScore; } ;
	/// Gets a NumValue
	virtual	CIBNumValue * GetNumValue()				{ return m_pNumValue; } ;
	/// Gets a Point
	virtual	CIBPoint * GetPoint()					{ return m_pPoint; } ;
	/// Retrieves all modalites
	virtual JVoid GetModalites(JList<CIBModalite *> &Modalites);
public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor );
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
	JInt32					m_ID;
	JList<CIBModalite *>	m_Modalite;
	CIBClass *				m_pClass;
	CIBQuant *				m_pQuant;
	CIBScore *				m_pScore;
	CIBNumValue *			m_pNumValue;
	CIBPoint *				m_pPoint;

private:
	CIBQuestion(const CIBQuestion & rhs);
	CIBQuestion & operator=(const CIBQuestion & rhs);

};
