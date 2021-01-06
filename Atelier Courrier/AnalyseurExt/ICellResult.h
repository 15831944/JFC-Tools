#pragma once

#include "AnalyseurEnums.h"

class CCellResult;
class CCellItemMoyenResult;

class ICellResult
{
public:

	ICellResult(void);
	virtual ~ICellResult(void);

	virtual const ICellResult *				AsICellResult()			const	{ return this; } ;
	virtual ICellResult *					AsICellResult()					{ return this; } ;
	virtual const CCellResult *				AsCCellResult()			const	{ return 0; } ;
	virtual CCellResult *					AsCCellResult()					{ return 0; } ;
	virtual const CCellItemMoyenResult *	AsCCellItemMoyenResult()const	{ return 0; } ;
	virtual CCellItemMoyenResult *			AsCCellItemMoyenResult()		{ return 0; } ;

	virtual double	GetResult(RESULTTYPE Type, const JFLT32VECTOR * pQuanti = 0, bool ModeMoyenne0Exclu = 0, ITEMTYPE ElmtType = ITYPE_ITEM, bool ModeExclusif = false) = 0;
	virtual bool	IsCCellResult() { return false; }
	virtual bool	IsCCellItemMoyenResult()  { return false; }
};

typedef std::auto_ptr<ICellResult> PTR_ICELLRESULT;