
#pragma once

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
class AFX_EXT_CLASS ITheme  
{
public:
	ITheme();
	virtual ~ITheme();

	virtual const ITheme *	AsITheme() const	{ return this; } ;
	virtual ITheme *		AsITheme()			{ return this; } ;

	virtual JBool	IsGrandTheme()		const { return false; } ;
	virtual JBool	IsTheme()			const { return false; } ;
	virtual JBool	IsBlocQuestion()	const { return false; } ;
	virtual JBool	IsContingent()		const { return false; } ;
	virtual JBool	IsQuestion()		const { return false; } ;

	virtual const JLabel &	GetLabel()	const =0 ;
	virtual JInt32			GetID()		const =0 ;
};

