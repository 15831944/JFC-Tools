/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTarifManuel
//
//	Descriptif:
//	Représentation d'un tarif manuel
*/


#pragma once


class AFX_EXT_CLASS CTarifManuel
{
public:
	CTarifManuel(void);
	CTarifManuel(JUnt32 idsupport, JUnt32 idformat, JFlt32 trf, JDate dc, JDate ds);
	~CTarifManuel(void);

	JUnt32 GetIDSupport() const;
	JUnt32 GetIDFormat() const;
	JFlt32 GetTarif() const;
	JDate GetDateC() const;
	JDate GetDateS() const;

//	JVoid SetIDSupport(JUnt32 id);
//	JVoid SetIDFormat(JUnt32 id);
//	JVoid SetTarif(JFlt32 trf);
//	JVoid SetDateC(JDate dc);
//	JVoid SetDateS(JDate ds);

	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);
	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;


private:
	JUnt32 m_ID_Support;
	JUnt32 m_ID_Format;
	JFlt32 m_Tarif;
	JDate m_DateC;
	JDate m_DateS;

};
