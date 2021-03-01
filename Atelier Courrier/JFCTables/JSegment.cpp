#include "StdAfx.h"
#include "jsegment.h"


JSegment::JSegment()
{
	// on ne fait rien
}

JSegment::JSegment(const JSegment & Segment)
{
	// on recopie l'�l�ment
	m_IdSegment = Segment.m_IdSegment;
	m_Libelle  = Segment.m_Libelle;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JSegment::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSegment!=0 && !m_Libelle.IsEmpty());
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment

JVoid JSegment::Send(JStream & Stream) const
{
	// on s�rialise l'�l�ment
	Stream.Send(m_IdSegment);
	m_Libelle .Send(Stream);
}

JVoid JSegment::Recv(JStream & Stream)
{
	// on s�rialise l'�l�ment
	JInt32 IdSegment; Stream.Recv(IdSegment);
	JLabel Libelle;  Libelle .Recv(Stream);

	// on recopie l'�l�ment
	m_IdSegment = IdSegment;
	m_Libelle  = Libelle;
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JSegment & JSegment::operator =(const JSegment & Segment)
{
	// on recopie l'�l�ment
	m_IdSegment = Segment.m_IdSegment;
	m_Libelle  = Segment.m_Libelle;

	// on renvoie notre r�f�rence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JSegment::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSegment = 0;
	m_Libelle .Reset();
}

/////////////////
// le destructeur

JSegment::~JSegment()
{
	// on ne fait rien
}
