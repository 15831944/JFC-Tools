//
// Fichier: JATPDATInsertion.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    24/04/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JATPDATInsertion.h"

////////////////////
// les constructeurs

JATPDATInsertion::JATPDATInsertion()
{
	// on ne fait rien
	m_status = NOTHING;
}

JATPDATInsertion::JATPDATInsertion(const JATPDATInsertion & Source)
{
	// on recopie l'élément
	*this = Source;
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JATPDATInsertion::Send(JStream & Stream) const
{
	// on sérialise l'élément
	Stream.Send(m_TarifP);
	Stream.Send(m_TarifN);
	Stream.Send(m_Duree);
}

JVoid JATPDATInsertion::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JInt32 TarifP; Stream.Recv(TarifP);
	JInt32 TarifN; Stream.Recv(TarifN);
	JInt32 Duree;  Stream.Recv(Duree);

	// on recopie l'élément
	m_TarifP = TarifP;
	m_TarifN = TarifN;
	m_Duree  = Duree;
}

JVoid JATPDATInsertion::Swap(JATPDATInsertion & Source)
{
	// on sérialise l'élément
	JInt32 TarifP	=  Source.m_TarifP;
	JInt32 TarifN	=  Source.m_TarifN;
	JInt32 Duree	=  Source.m_Duree;

	// on recopie l'élément
	Source.m_TarifP	=	m_TarifP;
	m_TarifP = TarifP;
	Source.m_TarifN	=	m_TarifN;
	m_TarifN = TarifN;
	Source.m_Duree	=	m_Duree;
	m_Duree  = Duree;
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JATPDATInsertion & JATPDATInsertion::operator =(const JATPDATInsertion & Source)
{
	// on recopie l'élément
	m_TarifP = Source.m_TarifP;
	m_TarifN = Source.m_TarifN;
	m_Duree  = Source.m_Duree;

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JATPDATInsertion::operator ==(const JATPDATInsertion & Reference) const
{
	// on compare la première composante
	if (m_TarifP != Reference.m_TarifP) return (false);

	// on compare la deuxième composante
	if (m_TarifN != Reference.m_TarifN) return (false);

	// on compare la dernière composante
	return (m_Duree == Reference.m_Duree);
}

JBool JATPDATInsertion::operator !=(const JATPDATInsertion & Reference) const
{
	// on compare la première composante
	if (m_TarifP != Reference.m_TarifP) return (true);

	// on compare la deuxième composante
	if (m_TarifN != Reference.m_TarifN) return (true);

	// on compare la dernière composante
	return (m_Duree != Reference.m_Duree);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JATPDATInsertion::Reset()
{
	// on réinitialise l'élément
	m_TarifP = 0;
	m_TarifN = 0;
	m_Duree = 0;
}

/////////////////
// le destructeur

JATPDATInsertion::~JATPDATInsertion()
{
	// on ne fait rien
}
