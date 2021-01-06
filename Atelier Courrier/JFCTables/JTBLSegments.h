/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JTBLSegments
//
//	Descriptif:
//	Table des segments
*/

#pragma once
#include "JSegment.h"


class AFX_EXT_CLASS JTBLSegments : public JMap<JUnt32, JSegment>
{
public:
	/// le constructeur
	JTBLSegments();
	~JTBLSegments();

	JVoid Import(CStdioFile& file);

	/// la fonction pour rechercher un �l�ment
	const JSegment * GetItem(JInt32 IdSegment) const;

	/// la fonction pour r�cup�rer un �l�ment
	const JSegment & GetItem() const;


};
