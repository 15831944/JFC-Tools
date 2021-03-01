/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JPeriodicite
//
//	Descriptif:
//	Représentation d'un élément périodicité
*/


#if !defined(AFX_JPERIODICITE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_JPERIODICITE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"



class AFX_EXT_CLASS JPeriodicite : public IElem  
{

public:
	JPeriodicite(JUnt32 id, const JChar* lbl, const JChar* lblcourt);
	virtual ~JPeriodicite();

	virtual const JChar* GetLabelCourt() const;

protected:
	JLabel m_LabelCourt;
};


#endif // !defined(AFX_JPERIODICITE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
