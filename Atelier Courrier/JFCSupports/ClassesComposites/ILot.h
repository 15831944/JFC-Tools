/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	ILot
//
//	Descriptif:
//	Lot de titre contient uniquement des titres
//	DesignPattern Classe composite
*/

#if !defined(AFX_ILOT_H__INCLUDED_)
#define AFX_ILOT_H__INCLUDED_

#pragma once

#include "Composite.h"


class AFX_EXT_CLASS ILot : public IComposite  
{

public:
	ILot();
	ILot(JUnt32 ID, const JChar* lbl, JUnt32 parentID);
	virtual ~ILot();

	virtual JBool IsLot() const {return true;};
	virtual ILot * AsLot() { return this;};

	virtual IElem* GetFilsByID(JUnt32 id);
	virtual IElem* GetFilsByIDAudience(JUnt32 id);
	virtual JVoid GetFils(JList<JUnt32> & lst) const;
	virtual JVoid GetFils(JList<IElem*> & lst) const;


//	virtual JVoid Accept (IVisiteur* v);

//	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
//	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

};

#endif // !defined(AFX_ILOT_H__INCLUDED_)
