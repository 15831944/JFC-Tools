/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CCouplageLibre
//
//	Descriptif:
//	Couplage libre classe composite d�rivant de JCouplage
*/


#if !defined(AFX_COUPLAGELIBRE_H__5AAEF048_625E_40C5_ADE0_B1649889B123__INCLUDED_)
#define AFX_COUPLAGELIBRE_H__5AAEF048_625E_40C5_ADE0_B1649889B123__INCLUDED_

#pragma once

#include "Composite.h"
#include "JCouplage.h"


class AFX_EXT_CLASS CCouplageLibre : public IComposite, public ICouplage  
{

public:
	CCouplageLibre();
	CCouplageLibre(JUnt32 ID, const JChar* lbl);
	virtual ~CCouplageLibre();

	virtual JBool IsCouplage() const		{ return true; };
	virtual JBool IsCouplageLibre()const	{ return true; };
	virtual CCouplageLibre * AsCouplageLibre() { return this;};
	virtual ICouplage * AsCouplage() { return this;};

	virtual JVoid Accept (IVisiteur* v);

	virtual IElem* GetFilsByID(JUnt32 id) const;
	virtual JVoid GetFils(JList<JUnt32> & lst) const;
	virtual JVoid GetFils(JList<IElem*> & lst) const;

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

	virtual JInt32 GetNbElem() const;

};

#endif // !defined(AFX_COUPLAGELIBRE_H__5AAEF048_625E_40C5_ADE0_B1649889B123__INCLUDED_)
