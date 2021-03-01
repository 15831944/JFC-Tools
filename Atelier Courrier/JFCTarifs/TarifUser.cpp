#include "StdAfx.h"
#include ".\tarifuser.h"

/////////////////////////////////////////////////////////////////////
// Constructeur	
CTarifUser::CTarifUser(void)
{
}

/////////////////////////////////////////////////////////////////////
// Destructeur
CTarifUser::~CTarifUser(void)
{
}

/////////////////////////////////////////////////////////////////////
// Construction copie tarif
CTarifUser::CTarifUser(JUnt32 IdSupport, JUnt32 IdFormat, JFlt32 Tarif, JDate DateDeb, JDate DateFin)
{
	m_IdSupport		= IdSupport;
	m_IdFormat		= IdFormat;
	m_Tarif			= Tarif;
	m_DateValidDeb	= DateDeb;
	m_DateValidFin	= DateFin;
}

/////////////////////////////////////////////////////////////////////
// R�cup�re ident support tarif
JUnt32 CTarifUser::GetIDSupport() const
{
	return 	m_IdSupport;
}

/////////////////////////////////////////////////////////////////////
// R�cup�re ident format tarif
JUnt32 CTarifUser::GetIDFormat() const
{
	return m_IdFormat;
}

/////////////////////////////////////////////////////////////////////
// R�cup�re valeur tarif
JFlt32 CTarifUser::GetTarif() const
{
	return m_Tarif;
}

/////////////////////////////////////////////////////////////////////
// R�cup�re date d�but validit� tarif
JDate CTarifUser::GetDateDeb() const
{
	return m_DateValidDeb;
}

/////////////////////////////////////////////////////////////////////
// R�cup�re date fin validit� tarif
JDate CTarifUser::GetDateFin() const
{
	return m_DateValidFin;
}
/////////////////////////////////////////////////////////////////////
// S�rialise Tarif mode Send
JArchive& CTarifUser::Send(JArchive & Archive, JInt32 Version) const
{
	switch(Version)
	{
		case 1:
			Archive.Send( m_IdSupport );
			Archive.Send( m_IdFormat );
			Archive.Send( m_Tarif );
			m_DateValidDeb.Send(Archive);
			m_DateValidFin.Send(Archive);
			break;

	// Erreur s�rialisation
	default:
		TRACE("CTarifIUser   Version invalide");
		throw JInternalError::GetInstance();
	}
	return Archive;
}

JArchive& CTarifUser::Recv(JArchive & Archive, JInt32 Version)
{
	switch(Version)
	{
		case 1:
			Archive.Recv( m_IdSupport );
			Archive.Recv( m_IdFormat );
			Archive.Recv( m_Tarif );
			m_DateValidDeb.Recv(Archive);
			m_DateValidFin.Recv(Archive);
			break;

	// Erreur s�rialisation
	default:
		TRACE("CTarifIUser   Version invalide");
		throw JInternalError::GetInstance();
	}
	return Archive;
}