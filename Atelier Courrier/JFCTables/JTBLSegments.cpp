#include "StdAfx.h"
#include "jtblsegments.h"

////////////////////
// les constructeurs

JTBLSegments::JTBLSegments()
{
	// on ne fait rien
}

////////////////////////////////
//  fonction d'import
JVoid JTBLSegments::Import(CStdioFile& file)
{
	CString str;
	while (file.ReadString(str))
	{
		//ligne vide ou commentaire
		str.Trim();
		if(str.Find("//") == 0 || str.IsEmpty())
			continue;

		JSegment segment;
		CString resToken;
		int curPos= 0;

		// l'identifiant
		resToken= str.Tokenize(",",curPos);
		segment.m_IdSegment = atoi(resToken);
		
		// le libellé
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		segment.m_Libelle = resToken;

		// on ajoute l'élément
		if (segment.IsValid())
			Add(segment.m_IdSegment) = segment;
	}
}

/// la fonction pour récupérer un élément
const JSegment * JTBLSegments::GetItem(JInt32 IdSegment) const
{
	MoveTo(IdSegment);
	if (IsValid())
		return &GetItem();
	else
		return 0;
}

/// la fonction pour récupérer un élément
const JSegment & JTBLSegments::GetItem() const
{
	return JMap<JUnt32, JSegment>::GetItem();
}

/////////////////
// le destructeur
JTBLSegments::~JTBLSegments()
{
	// on ne fait rien
}
