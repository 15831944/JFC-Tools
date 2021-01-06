/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JCouplageFixe
//
//	Descriptif:
//	Spécialisation de JCouplage, Couplage fixe
*/


#if !defined(AFX_JCOUPLAGEFIXE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_JCOUPLAGEFIXE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"
#include "IElemMap.h"
#include "JCouplage.h"




class AFX_EXT_CLASS JCouplageFixe : public IElem ,public ICouplage
{
friend class JTBLCouplagesFixes;
friend class JTBLTitres;
//Pour le createur de couplage fixe
friend class CCplFixCreatorDlg;

public:
	JCouplageFixe(JUnt32 id, const JChar* lbl);
	virtual ~JCouplageFixe();

	IElem* GetFilsByID(JUnt32 id) const;
	JVoid GetFils(JList<JUnt32> & lst) const;
	JVoid GetFils(JList<IElem*> & lst) const;

	virtual JBool IsCouplage() const { return true; };
	virtual JBool IsCouplageFixe() const { return true; };
	virtual JCouplageFixe * AsCouplageFixe() { return this;};
	virtual ICouplage * AsCouplage() { return this;};

	JInt32 GetNbElem() const;

protected:
	IELEMMAP m_mapIElem;
	JVoid Empty();
};


#endif // !defined(AFX_JCOUPLAGEFIXE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
