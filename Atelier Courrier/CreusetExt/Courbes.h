// Courbes.h: interface for the CCourbes class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "Courbe.h"
#include "FileHeader.h"

class  CCourbes : public JMap<int, CCourbe*>
{

public:
	virtual ~CCourbes(void);

	//Singleton
	static CCourbes* GetInstance();

	void Save();

	double GetProba(int indice, int appel) const;

	int GetNbCourbes() const;
	const JLabel &  GetLabel(int idCourbe) const;

private:
	double GetProbaByBeta(int indice, int beta) const;
	double GetProbaByCourbe(int indice, int idCourbe) const;
	CCourbes(void);
	static CCourbes m_Instance;

	//JMap<int, CCourbe*> m_Courbes;

	/// File Header
	CFileHeader				m_FileHeader;
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Save the file header
	JVoid SaveFileHeader		(JArchive *pFile);
};
