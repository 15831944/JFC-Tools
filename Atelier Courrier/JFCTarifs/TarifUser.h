/////////////////////////////////////////////////////////////////////////////
// Structure d'un tarif utilisateur
//
// Ces tarifs seront stock�s avec	l'ident support
//									l'ident format
//									la valeur tarif en euros
//									la date d�but prise en compte du tarif
//									la date fin de prise en compte du tarif
//
#pragma once

class AFX_EXT_CLASS CTarifUser
{
public:
	// Constructeur
	CTarifUser(void);
	CTarifUser(JUnt32 IdSupport, JUnt32 IdFormat, JFlt32 Tarif, JDate DateDeb, JDate DateFin);

	// Destructeur
	~CTarifUser(void);

	// R�cup�re ident Support
	JUnt32 GetIDSupport() const;

	// R�cup�re ident Format
	JUnt32 GetIDFormat() const;

	// R�cup�re valuer Tarif
	JFlt32 GetTarif() const;

	// R�cup�re date d�but et date de fin
	JDate  GetDateDeb() const;
	JDate  GetDateFin() const;

	// S�rialisation
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);
	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;

private:
	// Identificateur du support
	JUnt32 m_IdSupport;

	// Identificateur du format
	JUnt32 m_IdFormat;

	// Valeur tarif en euros
	JFlt32 m_Tarif;

	// Date d�but et date fin validit� tarif
	JDate m_DateValidDeb;
	JDate m_DateValidFin;
};
