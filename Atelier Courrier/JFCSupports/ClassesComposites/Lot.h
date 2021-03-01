/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CLot
//
//	Descriptif:
//	Implémentation de ILot
*/


#if !defined(AFX_LOT_H__89C13529_6F06_4DC1_81CF_EA440AAB16B1__INCLUDED_)
#define AFX_LOT_H__89C13529_6F06_4DC1_81CF_EA440AAB16B1__INCLUDED_

#pragma once

#include "Ilot.h"


class AFX_EXT_CLASS CLot : public ILot  
{

public:
	CLot();
	CLot(JUnt32 ID, const JChar* lbl);
	virtual ~CLot();

//	virtual JBool IsLot() { return true; };
//	virtual CLot * AsLot() { return this;};

/*	virtual IElem* GetFilsByID(JUnt32 id);
	virtual IElem* GetFilsByIDAudience(JUnt32 id);
	virtual JVoid GetFils(JList<JUnt32> & lst) const;
	virtual JVoid GetFils(JList<IElem*> & lst) const;
*/

	virtual JVoid Accept (IVisiteur* v);

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

};

#endif // !defined(AFX_LOT_H__89C13529_6F06_4DC1_81CF_EA440AAB16B1__INCLUDED_)
