/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	IElemMap
//
//	Descriptif:
//	Classe abstraite Map de IElem*
*/


#if !defined(AFX_IELEMMAP_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
#define AFX_IELEMMAP_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )

#include "IElem.h"



class AFX_EXT_CLASS IElemMap : public JMap <JUnt32, IElem*>
{
public:
	virtual ~IElemMap();

	JChar* GetLabelByID(const JUnt32 id) const;
	IElem* GetElemByID(const JUnt32 id) const;

	virtual JVoid Empty();

	virtual JVoid Import(CStdioFile& file) PURE;
protected:
	IElemMap();

};



#endif // !defined(AFX_IELEMMAP_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
