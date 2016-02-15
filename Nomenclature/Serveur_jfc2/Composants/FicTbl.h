// FicTbl.h: interface for the CFicTbl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICTBL_H__50E7C691_A562_11D1_95EA_444553540000__INCLUDED_)
#define AFX_FICTBL_H__50E7C691_A562_11D1_95EA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define TAILLEBUFFLINE (1000)
class CFicTbl  
{
private:
	CStdioFile m_FicTbl;
	char m_fFileOpened;
	LPSTR m_BuffLine;
	CString m_NomFic;

	void SupBlancsFin(LPSTR ch);
	void SupprimeCARIAGE_LINE(LPSTR txt);
	short FormatteLine();
	short m_NbField;
	short *m_Pos;
	short m_NroLigne;
public:
	CFicTbl();
	virtual ~CFicTbl();

	short OpenFile(LPCSTR aNomFic,char aMode='r');
	short CloseFile();
	
	short FindStar();
	short FindHash();
	
	short SetLineFormat(short Nb0=0,short Nb1=0,short Nb2=0,short Nb3=0,short Nb4=0,short Nb5=0,
		short Nb6=0,short Nb7=0,short Nb8=0,short Nb9=0,short Nb10=0,short Nb11=0,short Nb12=0,
		short Nb13=0,short Nb14=0,short Nb15=0,short Nb16=0);

	short ReadLine();
	
	char GetChar(short PosField);		//lecture d'un char
	LPCSTR GetCharPtr(short PosField);	//lecture d'une chaine
	short GetShort(short PosField);		//lecture d'un short
	long GetLong(short PosField);		//lecture d'un long

	short WriteLine(LPCSTR aLine);		//ecriture d'un rec dans le fichier
	short WriteHash(short aNbRec=0);	//ecriture du #
	short WriteStar(short aNbRec=0);	//ecriture de * et nb de rec

};

#endif // !defined(AFX_FICTBL_H__50E7C691_A562_11D1_95EA_444553540000__INCLUDED_)

