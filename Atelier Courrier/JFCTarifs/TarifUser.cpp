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
// Récupère ident support tarif
JUnt32 CTarifUser::GetIDSupport() const
{
	return 	m_IdSupport;
}

/////////////////////////////////////////////////////////////////////
// Récupère ident format tarif
JUnt32 CTarifUser::GetIDFormat() const
{
	return m_IdFormat;
}

/////////////////////////////////////////////////////////////////////
// Récupère valeur tarif
JFlt32 CTarifUser::GetTarif() const
{
	return m_Tarif;
}

/////////////////////////////////////////////////////////////////////
// Récupère date début validité tarif
JDate CTarifUser::GetDateDeb() const
{
	return m_DateValidDeb;
}

/////////////////////////////////////////////////////////////////////
// Récupère date fin validité tarif
JDate CTarifUser::GetDateFin() const
{
	return m_DateValidFin;
}
/////////////////////////////////////////////////////////////////////
// Sérialise Tarif mode Send
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

	// Erreur sérialisation
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

	// Erreur sérialisation
	default:
		TRACE("CTarifIUser   Version invalide");
		throw JInternalError::GetInstance();
	}
	return Archive;
}