#include "StdAfx.h"
#include "jsegment.h"


JSegment::JSegment()
{
	// on ne fait rien
}

JSegment::JSegment(const JSegment & Segment)
{
	// on recopie l'élément
	m_IdSegment = Segment.m_IdSegment;
	m_Libelle  = Segment.m_Libelle;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JSegment::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSegment!=0 && !m_Libelle.IsEmpty());
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JSegment::Send(JStream & Stream) const
{
	// on sérialise l'élément
	Stream.Send(m_IdSegment);
	m_Libelle .Send(Stream);
}

JVoid JSegment::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JInt32 IdSegment; Stream.Recv(IdSegment);
	JLabel Libelle;  Libelle .Recv(Stream);

	// on recopie l'élément
	m_IdSegment = IdSegment;
	m_Libelle  = Libelle;
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JSegment & JSegment::operator =(const JSegment & Segment)
{
	// on recopie l'élément
	m_IdSegment = Segment.m_IdSegment;
	m_Libelle  = Segment.m_Libelle;

	// on renvoie notre référence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JSegment::Reset()
{
	// on réinitialise l'élément
	m_IdSegment = 0;
	m_Libelle .Reset();
}

/////////////////
// le destructeur

JSegment::~JSegment()
{
	// on ne fait rien
}
