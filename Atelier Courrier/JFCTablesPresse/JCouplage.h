/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	ICouplage
//
//	Descriptif:
//	Interface d'un couplage (spécialisée en Couplage Fixe ou Libre)
*/


#if !defined(AFX_ICOUPLAGE_H__INCLUDED_)
#define AFX_ICOUPLAGE_H__INCLUDED_


#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"




class AFX_EXT_CLASS ICouplage
{
public:
	ICouplage(){};
	virtual ~ICouplage(){};

	virtual IElem* GetFilsByID(JUnt32 id) const PURE;
	virtual JVoid GetFils(JList<JUnt32> & lst) const PURE;
	virtual JVoid GetFils(JList<IElem*> & lst) const PURE;

	virtual JInt32 GetNbElem() const PURE;
};


#endif // !defined(AFX_ICOUPLAGE_H__INCLUDED_)
