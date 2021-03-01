/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//	Eddie GENT
//
//
//	Classe:
//	CFormatStr
//
//	Descriptif:
//	Permet de formater des chaine de caracteres
//
//
//	Classe:
//	CCompareTitre
//
//	Descriptif:
//	Permet de classer des chaines de caractères en ignorant les accents et les articles (l'équipe est classé à la lettre E)
*/

#pragma once
class JDate;
class JTime;
class AFX_EXT_CLASS CFormatStr
{
public:
	CFormatStr(void);
	virtual ~CFormatStr(void);

	//Pour faire la séparation des milliers
	static 	void FormatNum(const CString &Fmt, CString &Num, const char Decimal=',');

	static 	CString FormatCout(double value, int nbDecimales);

	// jj/mm/aaa
	static 	CString FormatDate(JDate date);

	// jour jj mois aaaa hh:mm:ss
	static 	CString FormatDateLong(JDate Date, JTime Time);

public:
	static bool SetDecimalPoint(char Decimal);
	static char GetDecimalPoint();

private:
	static char m_Decimal;
};


class AFX_EXT_CLASS CCompareTitre
{
public:
	CCompareTitre(void);
	virtual ~CCompareTitre(void);

	//
	static 	int CompareTitre(CString str1, CString str2);
private:
	static void PrepareStr(CString &str);

};