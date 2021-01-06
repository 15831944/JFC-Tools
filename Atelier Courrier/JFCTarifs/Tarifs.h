/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTarifs
//
//	Descriptif:
//	Point d'entrée de la Dll de gestion des Tarifs
//	Possède les méthodes de recherche des tarifs
//	DesignPattern : Singleton
*/


#pragma once

#include "TarifsManuels.h"
#include "TarifsAutos.h"
#include "Parutions.h"
#include "VersionsTarifsAutos.h"
#include "KeyTarifManuel.h"


	enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};

class AFX_EXT_CLASS CTarifs
{

friend class CVisiteurDelete;
friend class CVisiteurDeleteTA;
friend class CDlgTarif;

public:
	virtual ~CTarifs(void); 

	//Singleton
	static CTarifs* GetInstance();
	JBool SaveTarifsManuels();

	///Méthodes de lecture du tarif
	JInt32 GetTarifByDate( JUnt32 idSupport, JUnt32 idFormat, JDate dateClick, JDate dateLimite, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const;
	
	////////////
	RETTRF GetTarifByDateNew( JUnt32 idSupport, JUnt32 idFormat, JDate dateClick, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const;
	RETTRF GetNextTarifByDateNew( JUnt32 idSupport, JUnt32 idFormat, JDate dateFinInsert, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const;
	RETTRF GetStatutTarifNew( JUnt32 idSupport, JUnt32 idFormat, JDate dateToTest, JDate dateLimiteMin, JDate dateLimiteMax) const;

	//////////

	JInt32 GetStatutTarif( JUnt32 idSupport, JUnt32 idFormat, JDate dateClick, JDate dateLimite) const;

	JBool GetTarifByVersion( JUnt32 idSupport, JUnt32 idFormat, JDate date, JUnt32 & tarif) const;

	JBool GetNumParution( JUnt32 idSupport, JDate date, JUnt32 & numero) const;

	// Même méthode de récupération tarif mais avec passage des tarifs manuels utilisateurs
	JInt32 GetTarifByDate		(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateClick, JDate dateLimite, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const;
	RETTRF GetTarifByDateNew	(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateClick, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const;
	RETTRF GetNextTarifByDateNew(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateFinInsert, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const;
	JBool  GetTarifByVersion	(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate date, JUnt32 & tarif) const;

protected:
	JUnt32 GetDureePeriod(JUnt32 idSupport) const;

	CTarifsManuels m_TarifsManuels;
	CTarifsAutos m_TarifsAutos;
	CParutions m_Parutions;
	CVersionsTarifsAutos m_VersionsTarifsAutos;


private:
	CTarifs(void);
	static CTarifs m_Instance;
	

};
