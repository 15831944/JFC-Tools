// Famille.cpp: implementation of the CFamille class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Famille.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFamille::CFamille()
{
}

CFamille::CFamille(JUnt32 ID, const JChar* lbl) : ILot(ID, lbl, 0)
{
}

CFamille::~CFamille()
{
}

/*
JBool CFamille::IsLot() const
{
JBool bRet = false;
//si on a un titre c'est un lot
m_LstNode.MoveFirst();
if ( m_LstNode.IsValid() && m_LstNode.GetItem()->IsTitre() )
bRet = true;


return bRet;
}
*/

JBool CFamille::IsLot() const
{
	//si on a un titre c'est un lot
	m_LstNode.MoveFirst();
	while(m_LstNode.IsValid())
	{
		if (m_LstNode.GetItem()->IsTitre())
		{
			return true;
		}
		else if (m_LstNode.GetItem()->IsLot())
		{
			if (m_LstNode.GetItem()->AsLot()->GetNbFils() > 0)
				return true;
		}

		m_LstNode.MoveNext();
	}
	return false;
}

JVoid CFamille::Accept (IVisiteur* v)
{
	v->VisiteFamille(this);
}