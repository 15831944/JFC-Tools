/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFormat
//
//	Descriptif:
//	Repr�sentation d'un �l�ment Format
*/


#if !defined(AFX_JFORMAT_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_JFORMAT_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"


class AFX_EXT_CLASS JFormat : public IElem  
{

public:
	JFormat();
	JFormat(JUnt32 id, const JChar* lbl, const JChar* lblcourt);
	virtual ~JFormat();

	virtual const JLabel & GetLabelCourt() const;

	//pour appeler le send de JFormatSpe de facon transparente
	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;

protected:
	JLabel m_LabelCourt;

};


#endif // !defined(AFX_JFORMAT_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
