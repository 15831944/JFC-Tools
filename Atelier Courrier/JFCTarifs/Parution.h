/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CParution
//
//	Descriptif:
//	Représentation d'une parution
*/

#pragma once

class CParution
{
public:
	CParution(void);
	virtual ~CParution(void);

	JUnt32 GetVersion() const;
	JUnt32 GetNumPar() const;
	JUnt32 GetDuree() const;

	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

private:
	JUnt32 m_Version;
	JUnt32 m_NumPar;
	JUnt32 m_Duree;
};
