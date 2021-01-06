/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLFormats
//
//	Descriptif:
//	Table des formats
//	Charge le table des formats et les formats spéciaux
*/


#if !defined(AFX_TBLFORMAT_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
#define AFX_TBLFORMAT_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )

#include "..\JFCSource\FileHeader.h"
#include "IElemMap.h"
#include "JFormat.h"
#include "JFormatSpe.h"



class AFX_EXT_CLASS JTBLFormats : public IElemMap  
{
public:
	JTBLFormats();
	virtual ~JTBLFormats();

	JChar* GetLabelCourtByID(JUnt32 id) const;

	///Permet de maj ou d'ajouter des formats
	JVoid AddFormatSpe(JUnt32 id, const JChar* lblCourt, const JChar* lblLong);

	JVoid RemFormatSpe(JUnt32 id);

	JVoid RemAllFormatSpe();

	JVoid Import(CStdioFile& file);
	JBool LoadFormatSpe();
	JBool SaveFormatSpe();

	JInt32 GetNbFormatSpe();

private:
	JVoid LoadFileHeader (JArchive *pFile);
	JVoid SaveFileHeader (JArchive *pFile, JInt32 Version);
	CFileHeader m_FileHeader;
};



#endif // !defined(AFX_TBLFORMAT_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
