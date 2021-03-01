/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JTBLTitres
//
//	Descriptif:
//	Table des titres et des couplages fixes (il sont avant tout consud�r� comme des titres)
*/


#if !defined(AFX_JTBLTITRE_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
#define AFX_JTBLTITRE_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )

#include "IElemMap.h"
#include "JTitre.h"



class AFX_EXT_CLASS JTBLTitres : public IElemMap
{
public:
	JTBLTitres();
	virtual ~JTBLTitres();

	JUnt32 GetPeriodByID(JUnt32 id) const;
	JUnt32 GetIDAudienceByID(JUnt32 id) const;
	JVoid Import(CStdioFile& file);
	JVoid ImportCouplagesFixes(CStdioFile& file);

};



#endif // !defined(AFX_JTBLTITRE_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
