/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
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

	/// la fonction pour rechercher un élément
	const JSegment * GetItem(JInt32 IdSegment) const;

	/// la fonction pour récupérer un élément
	const JSegment & GetItem() const;


};
