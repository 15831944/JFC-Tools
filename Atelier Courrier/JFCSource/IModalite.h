
#pragma once
#include <memory>

class CModalite;
class CElement;
class CItemConstruit;

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
class AFX_EXT_CLASS IModalite  
{
public:
	IModalite();
	virtual ~IModalite();

	/// Return a pointer to this const object
	virtual const IModalite *		AsIModalite() const { return this; } ;
	/// Return a pointer to this object
	virtual IModalite *				AsIModalite() { return this; } ;
	/// Return a pointer to this const object
	virtual const CModalite *		AsModalite() const { return 0; } ;
	/// Return a pointer to this object
	virtual CModalite *				AsModalite() { return 0; } ;
	/// Return a pointer to this const object
	virtual const CElement *		AsElement() const { return 0; } ;
	/// Return a pointer to this object
	virtual CElement *				AsElement() { return 0; } ;
	/// Return a pointer to this const object
	virtual const CItemConstruit *	AsItemConstruit() const { return 0; } ;
	/// Return a pointer to this object
	virtual CItemConstruit *		AsItemConstruit() { return 0; } ;
	/// Returns true is this object is a Modalite
	virtual JBool	IsModalite()		const { return false; } ;
	/// Returns true is this object is an Element
	virtual JBool	IsElement()			const { return false; } ;
	/// Returns true is this object is a Support
	virtual JBool	IsSupport()			const { return false; } ;
	/// Returns true is this object is an ITem construit
	virtual JBool	IsItemConstruit()	const { return false; } ;
	/// Gets this objects lable
	virtual const JLabel &	GetLabel()	const =0 ;
	/// Gets this objects ID
	virtual JInt32			GetID()		const =0 ;

	/// Returns the Contingent Mask for this object
	virtual JUnt32			GetContingentMask()	const { return 0xFFFF; } ;
	// Converts this object to a string
	virtual JBool	ToItemString(JStringEx &String) const =0 ;
	/// Returns True is this object is in the Contingent
	virtual JBool	IsInContingent(JUnt32 ContingentMask) const = 0;


};

typedef std::auto_ptr<IModalite> PTR_IMODALITE;
typedef JMap<JInt32, PTR_IMODALITE> IMODALITEMAP;

