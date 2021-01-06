//
// Fichier: JATPDATInsertion.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    24/04/2003
//

// on inclut les d�finitions n�cessaires
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
	// on recopie l'�l�ment
	*this = Source;
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment

JVoid JATPDATInsertion::Send(JStream & Stream) const
{
	// on s�rialise l'�l�ment
	Stream.Send(m_TarifP);
	Stream.Send(m_TarifN);
	Stream.Send(m_Duree);
}

JVoid JATPDATInsertion::Recv(JStream & Stream)
{
	// on s�rialise l'�l�ment
	JInt32 TarifP; Stream.Recv(TarifP);
	JInt32 TarifN; Stream.Recv(TarifN);
	JInt32 Duree;  Stream.Recv(Duree);

	// on recopie l'�l�ment
	m_TarifP = TarifP;
	m_TarifN = TarifN;
	m_Duree  = Duree;
}

JVoid JATPDATInsertion::Swap(JATPDATInsertion & Source)
{
	// on s�rialise l'�l�ment
	JInt32 TarifP	=  Source.m_TarifP;
	JInt32 TarifN	=  Source.m_TarifN;
	JInt32 Duree	=  Source.m_Duree;

	// on recopie l'�l�ment
	Source.m_TarifP	=	m_TarifP;
	m_TarifP = TarifP;
	Source.m_TarifN	=	m_TarifN;
	m_TarifN = TarifN;
	Source.m_Duree	=	m_Duree;
	m_Duree  = Duree;
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JATPDATInsertion & JATPDATInsertion::operator =(const JATPDATInsertion & Source)
{
	// on recopie l'�l�ment
	m_TarifP = Source.m_TarifP;
	m_TarifN = Source.m_TarifN;
	m_Duree  = Source.m_Duree;

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JATPDATInsertion::operator ==(const JATPDATInsertion & Reference) const
{
	// on compare la premi�re composante
	if (m_TarifP != Reference.m_TarifP) return (false);

	// on compare la deuxi�me composante
	if (m_TarifN != Reference.m_TarifN) return (false);

	// on compare la derni�re composante
	return (m_Duree == Reference.m_Duree);
}

JBool JATPDATInsertion::operator !=(const JATPDATInsertion & Reference) const
{
	// on compare la premi�re composante
	if (m_TarifP != Reference.m_TarifP) return (true);

	// on compare la deuxi�me composante
	if (m_TarifN != Reference.m_TarifN) return (true);

	// on compare la derni�re composante
	return (m_Duree != Reference.m_Duree);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JATPDATInsertion::Reset()
{
	// on r�initialise l'�l�ment
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
