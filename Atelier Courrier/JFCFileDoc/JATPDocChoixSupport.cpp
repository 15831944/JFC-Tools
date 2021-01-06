//
// Fichier: JATPDocChoixSupport.cpp
// Date:    06/04/2004
// Gestion document s�lection Choix Support
//
#include "StdAfx.h"
#include "JATPDocChoixSupport.h"
#include "ATPDocCSSerializer.h"
#include "MoteurPresse.h"
#include "JSRCTables.h"
#include "ATPMessages.h"
#include "Tarifs.h"
#include "resource.h"
#include "FormatStr.h"
#include "ItemsConstruitsManager.h"

#include <math.h>

// les op�rateurs pour comparer les �l�ments r�sultats �chelle
JBool 	CKeyResultEch::operator <(const CKeyResultEch & rhs) const
{
	if (m_IdTerrain < rhs.m_IdTerrain)	return true;
	if (m_IdTerrain > rhs.m_IdTerrain)	return false;
	if (m_IdCible   < rhs.m_IdCible)	return true;
	if (m_IdCible   > rhs.m_IdCible)	return false;
	if (m_IdCibleAff< rhs.m_IdCibleAff)	return true;
	if (m_IdCibleAff> rhs.m_IdCibleAff)	return false;
	if (m_IdSegment < rhs.m_IdSegment)  return true;
	if (m_IdSegment > rhs.m_IdSegment)  return false;
	if (m_IdSupport < rhs.m_IdSupport)  return true;
	if (m_IdSupport > rhs.m_IdSupport)  return false;
	if (m_IdMoteur  < rhs.m_IdMoteur)   return true;

	return false;
}

JBool 	CKeyResultEch::operator >(const CKeyResultEch & rhs) const
{
	if (m_IdTerrain > rhs.m_IdTerrain)	return true;
	if (m_IdTerrain < rhs.m_IdTerrain)	return false;
	if (m_IdCible   > rhs.m_IdCible)	return true;
	if (m_IdCible   < rhs.m_IdCible)	return false;
	if (m_IdCibleAff> rhs.m_IdCibleAff)	return true;
	if (m_IdCibleAff< rhs.m_IdCibleAff)	return false;
	if (m_IdSegment > rhs.m_IdSegment)  return true;
	if (m_IdSegment < rhs.m_IdSegment)  return false;
	if (m_IdSupport > rhs.m_IdSupport)  return true;
	if (m_IdSupport < rhs.m_IdSupport)  return false;
	if (m_IdMoteur  > rhs.m_IdMoteur)   return true;
	
	return false;
}

// les op�rateurs pour comparer les �l�ments r�sultats �chelle
JBool 	CKeyResultMap::operator <(const CKeyResultMap & rhs) const
{
	if (m_IdTerrain < rhs.m_IdTerrain)	return true;
	if (m_IdTerrain > rhs.m_IdTerrain)	return false;
	if (m_IdCible   < rhs.m_IdCible)	return true;
	if (m_IdCible   > rhs.m_IdCible)	return false;
	if (m_IdCibleAff< rhs.m_IdCibleAff)	return true;
	if (m_IdCibleAff> rhs.m_IdCibleAff)	return false;
	if (m_IdSegment < rhs.m_IdSegment)  return true;
	if (m_IdSegment > rhs.m_IdSegment)  return false;
	if (m_IdSupport < rhs.m_IdSupport)  return true;
	if (m_IdSupport > rhs.m_IdSupport)  return false;
	if (m_IdMoteur  < rhs.m_IdMoteur)   return true;
	
	return false;
}

JBool 	CKeyResultMap::operator >(const CKeyResultMap & rhs) const
{
	if (m_IdTerrain > rhs.m_IdTerrain)	return true;
	if (m_IdTerrain < rhs.m_IdTerrain)	return false;
	if (m_IdCible   > rhs.m_IdCible)	return true;
	if (m_IdCible   < rhs.m_IdCible)	return false;
	if (m_IdCibleAff> rhs.m_IdCibleAff)	return true;
	if (m_IdCibleAff< rhs.m_IdCibleAff)	return false;
	if (m_IdSegment > rhs.m_IdSegment)  return true;
	if (m_IdSegment < rhs.m_IdSegment)  return false;
	if (m_IdSupport > rhs.m_IdSupport)  return true;
	if (m_IdSupport < rhs.m_IdSupport)  return false;
	if (m_IdMoteur  > rhs.m_IdMoteur)   return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Operateur < de comparaison de clef positions mapping
JBool 	CKeyInfoMapping::operator <(const CKeyInfoMapping & rhs) const
{
	if (m_IdTerrain			< rhs.m_IdTerrain)			return true;
	if (m_IdTerrain			> rhs.m_IdTerrain)			return false;

	if (m_IdCible			< rhs.m_IdCible)			return true;
	if (m_IdCible			> rhs.m_IdCible)			return false;

	if (m_IdCibleAff		< rhs.m_IdCibleAff)			return true;
	if (m_IdCibleAff		> rhs.m_IdCibleAff)			return false;

	if (m_IdSegment			< rhs.m_IdSegment)			return true;
	if (m_IdSegment			> rhs.m_IdSegment)			return false;

	if (m_IdSupport			< rhs.m_IdSupport)			return true;
	if (m_IdSupport			> rhs.m_IdSupport)			return false;

	if (m_IdFormat			< rhs.m_IdFormat)			return true;
	if (m_IdFormat			> rhs.m_IdFormat)			return false;

	if (m_IdMoteur			< rhs.m_IdMoteur)			return true;
	if (m_IdMoteur			> rhs.m_IdMoteur)			return false;

	if (m_NbInsertions		< rhs.m_NbInsertions)		return true;
	if (m_NbInsertions		> rhs.m_NbInsertions)		return false;

	if (m_IdGrandeurAbs		< rhs.m_IdGrandeurAbs)		return true;
	if (m_IdGrandeurAbs		> rhs.m_IdGrandeurAbs)		return false;

	if (m_IdGrandeurOrd		< rhs.m_IdGrandeurOrd)		return true;
	if (m_IdGrandeurOrd		> rhs.m_IdGrandeurOrd)		return false;

	if (m_IdFiltreAudience	< rhs.m_IdFiltreAudience)	return true;
	if (m_IdFiltreAudience	> rhs.m_IdFiltreAudience)	return false;

	if (m_IdContexteLecture < rhs.m_IdContexteLecture)	return true;
	if (m_IdContexteLecture > rhs.m_IdContexteLecture)	return false;

	if (m_bRepriseEnMain	< rhs.m_bRepriseEnMain)		return true;
	if (m_bRepriseEnMain	> rhs.m_bRepriseEnMain)		return false;

	if (m_bLNMMag			< rhs.m_bLNMMag)			return true;
	if (m_bLNMMag			> rhs.m_bLNMMag)			return false;

	if (m_bLDPQtd			< rhs.m_bLDPQtd)			return true;
	if (m_bLDPQtd			> rhs.m_bLDPQtd)			return false;

	if (m_bLDP30J			< rhs.m_bLDP30J)			return true;
	if (m_bLDP30J			> rhs.m_bLDP30J)			return false;

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Operateur > de comparaison de clef positions mapping
JBool 	CKeyInfoMapping::operator >(const CKeyInfoMapping & rhs) const
{
	if (m_IdTerrain			> rhs.m_IdTerrain)			return true;
	if (m_IdTerrain			< rhs.m_IdTerrain)			return false;

	if (m_IdCible			> rhs.m_IdCible)			return true;
	if (m_IdCible			< rhs.m_IdCible)			return false;

	if (m_IdCibleAff		> rhs.m_IdCibleAff)			return true;
	if (m_IdCibleAff		< rhs.m_IdCibleAff)			return false;

	if (m_IdSegment			> rhs.m_IdSegment)			return true;
	if (m_IdSegment			< rhs.m_IdSegment)			return false;

	if (m_IdSupport			> rhs.m_IdSupport)			return true;
	if (m_IdSupport			< rhs.m_IdSupport)			return false;

	if (m_IdFormat			> rhs.m_IdFormat)			return true;
	if (m_IdFormat			< rhs.m_IdFormat)			return false;

	if (m_IdMoteur			> rhs.m_IdMoteur)			return true;
	if (m_IdMoteur			< rhs.m_IdMoteur)			return false;

	if (m_NbInsertions		> rhs.m_NbInsertions)		return true;
	if (m_NbInsertions		< rhs.m_NbInsertions)		return false;

	if (m_IdGrandeurAbs		> rhs.m_IdGrandeurAbs)		return true;
	if (m_IdGrandeurAbs		< rhs.m_IdGrandeurAbs)		return false;

	if (m_IdGrandeurOrd		> rhs.m_IdGrandeurOrd)		return true;
	if (m_IdGrandeurOrd		< rhs.m_IdGrandeurOrd)		return false;

	if (m_IdFiltreAudience  > rhs.m_IdFiltreAudience)	return true;
	if (m_IdFiltreAudience  < rhs.m_IdFiltreAudience)	return false;

	if (m_IdContexteLecture > rhs.m_IdContexteLecture)	return true;
	if (m_IdContexteLecture < rhs.m_IdContexteLecture)	return false;

	if (m_bRepriseEnMain	> rhs.m_bRepriseEnMain)		return true;
	if (m_bRepriseEnMain	< rhs.m_bRepriseEnMain)		return false;

	if (m_bLNMMag			> rhs.m_bLNMMag)			return true;
	if (m_bLNMMag			< rhs.m_bLNMMag)			return false;

	if (m_bLDPQtd			> rhs.m_bLDPQtd)			return true;
	if (m_bLDPQtd			< rhs.m_bLDPQtd)			return false;

	if (m_bLDP30J			> rhs.m_bLDP30J)			return true;
	if (m_bLDP30J			< rhs.m_bLDP30J)			return false;

	return false;
}

// les op�rateurs pour comparer les �l�ments r�sultats cumulativit�
JBool 	CKeyResultCum::operator <(const CKeyResultCum & rhs) const
{
	if (m_IdTerrain < rhs.m_IdTerrain)	return true;
	if (m_IdTerrain > rhs.m_IdTerrain)	return false;
	if (m_IdCible   < rhs.m_IdCible)	return true;
	if (m_IdCible   > rhs.m_IdCible)	return false;
	if (m_IdSegment < rhs.m_IdSegment)  return true;
	if (m_IdSegment > rhs.m_IdSegment)	return false;
	if (m_IdSupport < rhs.m_IdSupport)  return true;
	if (m_IdSupport > rhs.m_IdSupport)  return false;
	if (m_IdMoteur  < rhs.m_IdMoteur)   return true;
	if (m_IdMoteur  > rhs.m_IdMoteur)   return false;
	if (m_NbInsert  < rhs.m_NbInsert)   return true;
	return false;
}

JBool 	CKeyResultCum::operator >(const CKeyResultCum & rhs) const
{
	if (m_IdTerrain > rhs.m_IdTerrain)	return true;
	if (m_IdTerrain < rhs.m_IdTerrain)	return false;
	if (m_IdCible   > rhs.m_IdCible)	return true;
	if (m_IdCible   < rhs.m_IdCible)	return false;
	if (m_IdSegment > rhs.m_IdSegment)  return true;
	if (m_IdSegment < rhs.m_IdSegment)	return false;
	if (m_IdSupport > rhs.m_IdSupport)  return true;
	if (m_IdSupport < rhs.m_IdSupport)  return false;
	if (m_IdMoteur  > rhs.m_IdMoteur)   return true;
	if (m_IdMoteur  < rhs.m_IdMoteur)   return false;
	if (m_NbInsert  > rhs.m_NbInsert)   return true;
	return false;
}

// les op�rateurs pour comparer les �l�ments r�sultats duplicationcumulativit�
JBool 	CKeyResultDup::operator <(const CKeyResultDup & rhs) const
{
	if (m_IdTerrain    < rhs.m_IdTerrain)    return true;
	if (m_IdTerrain    > rhs.m_IdTerrain)    return false;
	if (m_IdCible      < rhs.m_IdCible)	     return true;
	if (m_IdCible      > rhs.m_IdCible)	     return false;
	if (m_IdSegment    < rhs.m_IdSegment)    return true;
	if (m_IdSegment    > rhs.m_IdSegment)	 return false;
	if (m_IdSupportRow < rhs.m_IdSupportRow) return true;
	if (m_IdSupportRow > rhs.m_IdSupportRow) return false;
	if (m_IdSupportCol < rhs.m_IdSupportCol) return true;
	if (m_IdSupportCol > rhs.m_IdSupportCol) return false;
	if (m_IdMoteur     < rhs.m_IdMoteur)		 return true;
	return false;
}

JBool 	CKeyResultDup::operator >(const CKeyResultDup & rhs) const
{
	if (m_IdTerrain > rhs.m_IdTerrain)	     return true;
	if (m_IdTerrain < rhs.m_IdTerrain)	     return false;
	if (m_IdCible   > rhs.m_IdCible)	     return true;
	if (m_IdCible   < rhs.m_IdCible)	     return false;
	if (m_IdSegment > rhs.m_IdSegment)	     return true;
	if (m_IdSegment < rhs.m_IdSegment)	     return false;
	if (m_IdSupportRow > rhs.m_IdSupportRow) return true;
	if (m_IdSupportRow < rhs.m_IdSupportRow) return false;
	if (m_IdSupportCol > rhs.m_IdSupportCol) return true;
	if (m_IdSupportCol < rhs.m_IdSupportCol) return false;
	if (m_IdMoteur	   > rhs.m_IdMoteur)	 return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
JATPDocChoixSupport::JATPDocChoixSupport(void)
{
	//***************************** INIT ELMT SPECIF CHOIX SUPPORTS ****************************
	//
	// Index du module courant / par d�faut �chelle
	m_InxModuleCur = ModuleEch;

	// Date Tarifaire (par d�faut date du jour)
	COleDateTime DateCur = COleDateTime::GetCurrentTime();
	m_DateTarifaire.SetDate(DateCur.GetDay(),DateCur.GetMonth(),DateCur.GetYear());  

	// Init les infos plateau
	m_KEYPlateau.SetDateTar(m_DateTarifaire);

	// les supports s�lectionn�es avec leur �tat
	m_MapIdSupportsSel.Reset();

	// les supports �tat visible ou non
	m_MapIdSupportsVisible.Reset();

	// les couleurs affect�es aux supports pour dessin graphe
	m_MapIdColorSupports.Reset();

	// Par d�faut on choisit le mode couleurs s�par�es
	m_ModeColorGraphe = 0;

	// Init liste des map terrains, cibles, formats , filtre d'audience par module
	for (int InxModule = ModuleEch; InxModule <= ModuleDup; InxModule++)
	{
		// Liste des sources d'audience
		m_LstIdSrcAudienceSel.AddTail() = 0;

		// Liste des map cibles audience sel
		m_LstMapIdCiblesAudienceSel.AddTail();  

		// Liste des map terrains audience sel
		m_LstMapIdTerrainsAudienceSel.AddTail();

		// Init map segments audience sel associ� au module
		m_LstMapIdSegmentsAudienceSel.AddTail();
		m_LstMapIdSegmentsAudienceSel.MoveLast();

		// Par d�faut on prend l'ident segment ensemble	
		JMap <JUnt32,JUnt32> &MapId = m_LstMapIdSegmentsAudienceSel.GetItem();
		MapId.Add(1) ; 

		// Liste des map formats audience sel
		m_LstMapIdFormatsSel.AddTail();

		// Filtre d'audience / par d�faut ident = 1
		m_LstIdFiltreAudienceSel.AddTail() = 1;
	}
	
	// Liste des Grandeurs s�lectionn�es pour tous les modules
	m_MapIdGrandEchSel.Reset();
	m_IdGrandMapSelAbs = -1;
	m_IdGrandMapSelOrd = -1;
	m_MapIdGrandCumSel.Reset();
	m_MapIdGrandDupSel.Reset();

	// Liste des Formats s�lectionn�s pour tous les modules
	m_MapIdFormatSpecifEch.Reset();
	m_MapIdFormatSpecifMap.Reset();
	m_MapIdFormatSpecifCum.Reset();
	m_MapIdFormatSpecifDup.Reset();

	// Nb insertions s�lectionn� pour calcul
	m_NbInsertions = 1;

	// Nb insertions s�lectionn� pour calcul en mode cumulativit�
	// m_NbInsertionsCum = 12;
	m_NbInsertionsCum = 6;

	// Map des r�sultats en cours
	ResetAllResultat();

	// Etat Param�tre des diff�rentes appl
	m_ParamEchModif = true;
	m_ParamMapModif = true;
	m_ParamCumModif = true;
	m_ParamDupModif = true;

	// Init Ordre entete Echelle
	m_OrdreEnteteEch.SetSize(3);
	for (INT_PTR i=0; i<m_OrdreEnteteEch.GetCount();i++)
		m_OrdreEnteteEch[i] = i+1;

	// Init couleur courbe en cours.....
	m_IdColorCum = 0;

	// Init Tab info tri colonnes
	CInfoTriCol InfoTriCol;
	m_TabInfoTriCol.SetSize(ModuleDup+1);
	for (int InxModule = ModuleEch; InxModule <= ModuleDup; InxModule++)
		m_TabInfoTriCol.SetAt(InxModule,InfoTriCol);

	// **************************  LISTE DES ELMTS DE TRAVAIL *********************
	//
	// Init tableau des grandeurs disponibles pour �chelle (les index liste seront les idents grandeurs)
	m_MapGrandEch.Reset();
	m_MapGrandEch.Add(0) = "LDP/LNM (000)";
	m_MapGrandEch.Add(1) = "Audience";
	m_MapGrandEch.Add(2) = "Audience (000)";
	m_MapGrandEch.Add(3) = "Nb contacts (000)|Nb contacts[+](000)";
	m_MapGrandEch.Add(4) = "GRP";
	m_MapGrandEch.Add(5) = "GRP [+]";
	m_MapGrandEch.Add(6) = "C% cumul�e";
	m_MapGrandEch.Add(7) = "R�p�tition"; // "Reprises en main|R�p�tition");
	m_MapGrandEch.Add(8) = "R�p�tition [+]";
	m_MapGrandEch.Add(9) = "Indice d'affinit�";
	m_MapGrandEch.Add(10)= "% Affinit�";
	m_MapGrandEch.Add(11)= "Indice Cumulativit�";
	m_MapGrandEch.Add(12)= "Co�t/GRP �";
	m_MapGrandEch.Add(13)= "Co�t/%Couverture �";
	m_MapGrandEch.Add(14)= "Co�t/1000 Individus �";
	m_MapGrandEch.Add(15)= "Co�t/1000 Cts �|Co�t/1000 Cts[+]�";

	// Ajout des grandeurs associ�es aux donn�es exog�nes (OJD etc...)
	const JSRCDataExo		& DataExo = JSRCDataExo::GetInstance();
	const JList <CString>	&LstData  = DataExo.GetListNomDataExo(); 

	// Balaye toutes les donn�es exog�nes
	for (LstData.MoveFirst(); LstData.IsValid(); LstData.MoveNext())
	{
		// Recherche du libell�
		CString LabelData = LstData.GetItem(); 

		// Recherche du 1er ident pour cette data exog�ne
		JUnt32 IdDepGrandeur;
		if (DataExo.GetIdDepGrandeur(LabelData, IdDepGrandeur))
		{
			// Test si nouvelle grandeur exog�ne
			m_MapGrandEch.MoveTo(IdDepGrandeur);
			if (!m_MapGrandEch.IsValid())
			{
				// Ajout nouvelle grandeur
				m_MapGrandEch.Add(IdDepGrandeur) = LabelData;
			}
			else
				// Erreur, id d�j� existant
				throw JInternalError::GetInstance();
		}
	}

	// Init tableau abr�g� des grandeurs disponibles pour �chelle
	m_MapAbrGrandEch.Reset();
	m_MapAbrGrandEch.Add(0) = "LDP/LNM ";
	m_MapAbrGrandEch.Add(1) = "Audience ";
	m_MapAbrGrandEch.Add(2) = "Aud.(000) ";
	m_MapAbrGrandEch.Add(3) = "Cts.(000) |Cts[+](000) ";
	m_MapAbrGrandEch.Add(4) = "GRP ";
	m_MapAbrGrandEch.Add(5) = "GRP[+] ";
	m_MapAbrGrandEch.Add(6) = "C% cumul�e ";
	m_MapAbrGrandEch.Add(7) = "R�pet. "; // "Reprise |R�pet. ");
	m_MapAbrGrandEch.Add(8) = "R�pet[+] ";
	m_MapAbrGrandEch.Add(9) = "Ind.aff ";
	m_MapAbrGrandEch.Add(10)= "%Affinit� ";
	m_MapAbrGrandEch.Add(11)= "Ind.Cumul. ";
	m_MapAbrGrandEch.Add(12)= "Co�t/GRP � ";
	m_MapAbrGrandEch.Add(13)= "Co�t/C% � ";
	m_MapAbrGrandEch.Add(14)= "Co�t/000 Ind. � ";
	m_MapAbrGrandEch.Add(15)= "Co�t/000 Cts �|Co�t/000 Cts[+]� ";

	// Balaye toutes les donn�es exog�nes
	for (LstData.MoveFirst(); LstData.IsValid(); LstData.MoveNext())
	{
		// Recherche du libell�
		CString LabelData = LstData.GetItem(); 

		// Type de donn�es (chiffre, % , euros etc ....) >> par d�faut chiffre
		JUnt32 TypeData = JSRCDataExo::Type_Chiffre; 
		JUnt32 NbDec;
		JBool InfoOk = DataExo.GetTypeValeur(LabelData,TypeData,NbDec);

		// Recherche du 1er ident pour cette data exog�ne
		JUnt32 IdDepGrandeur;
		if (DataExo.GetIdDepGrandeur(LabelData, IdDepGrandeur))
		{
			// Test si nouvelle grandeur exog�ne
			m_MapAbrGrandEch.MoveTo(IdDepGrandeur);
			if (!m_MapAbrGrandEch.IsValid())
			{
				// Prend le libell� court / par d�faut le m�me
				CString AbrLabelData = LabelData; 

				// Extension info data (si <> data chiffre)
				CString ExtType = "";
				if (TypeData == JSRCDataExo::Type_Euro)
					ExtType = " � ";
				else if (TypeData == JSRCDataExo::Type_Prct)
					ExtType = " % ";

				if (DataExo.GetAbrLib(LabelData,AbrLabelData)) 
				{
					// Ajout nouvelle grandeur
					m_MapAbrGrandEch.Add(IdDepGrandeur) = AbrLabelData + ExtType; // + "(*)";
				}
				else
				{
					// par d�faut le m�me
					m_MapAbrGrandEch.Add(IdDepGrandeur) = LabelData + ExtType; // + "(*)";
				}
			}
			else
				// Erreur, id d�j� existant
				throw JInternalError::GetInstance();
		}
	}

	// R�cup�ration des param�tres canevas par d�faut
	if (RecupParamDef() == false)
	{
		// Sinon on prend ordre grandeurs �chelle application
		m_LstOrdoGrandeurEch.Reset();
		for (m_MapAbrGrandEch.MoveFirst();
			 m_MapAbrGrandEch.IsValid();
			 m_MapAbrGrandEch.MoveNext())
		{
			// R�cup�re ident grandeur
			m_LstOrdoGrandeurEch.AddTail() = m_MapAbrGrandEch.GetKey();  
		}
	}

	// **************************  LISTE DES ELMTS DE TRAVAIL *********************
	//
	// Init Map des grandeurs disponibles pour mapping (les index liste seront les idents grandeurs)
	m_MapGrandMap.Reset();
	m_MapGrandMap.Add(0) = "LDP/LNM (000)";
	m_MapGrandMap.Add(1) = "Audience";
	m_MapGrandMap.Add(2) = "Audience (000)";
	m_MapGrandMap.Add(3) = "Nb contacts (000)|Nb contacts[+](000)";
	m_MapGrandMap.Add(4) = "GRP";
	m_MapGrandMap.Add(5) = "GRP [+]";
	m_MapGrandMap.Add(6) = "C% cumul�e";
	m_MapGrandMap.Add(7) = "R�p�tition";
	m_MapGrandMap.Add(8) = "R�p�tition[+]";
	m_MapGrandMap.Add(9) = "Indice d'affinit�";
	m_MapGrandMap.Add(10) = "% Affinit�";
	m_MapGrandMap.Add(11) = "Indice Cumulativit�";
	m_MapGrandMap.Add(12) = "Co�t/GRP �";
	m_MapGrandMap.Add(13) = "Co�t/%Couverture �";
	m_MapGrandMap.Add(14) = "Co�t/1000 Individus �";
	m_MapGrandMap.Add(15) = "Co�t/1000 Cts �|Co�t/1000 Cts[+]�";

	// Ajoute les grandeurs exog�nes
	for (LstData.MoveFirst(); LstData.IsValid(); LstData.MoveNext())
	{
		// Recherche du libell�
		CString LabelData = LstData.GetItem(); 

		// Recherche du 1er ident pour cette data exog�ne
		JUnt32 IdDepGrandeur;
		if (DataExo.GetIdDepGrandeur(LabelData, IdDepGrandeur))
		{
			// Test si nouvelle grandeur exog�ne
			m_MapGrandMap.MoveTo(IdDepGrandeur);
			if (!m_MapGrandMap.IsValid())
			{
				// Ajout nouvelle grandeur
				m_MapGrandMap.Add(IdDepGrandeur) = LabelData;
			}
			else
				// Erreur, id d�j� existant
				throw JInternalError::GetInstance();
		}
	}

	// Init tableau abr�g� des grandeurs disponibles pour mapping
	m_MapAbrGrandMap.Reset();
	m_MapAbrGrandMap.Add(0)  = "LDP/LNM ";
	m_MapAbrGrandMap.Add(1)  = "Audience ";
	m_MapAbrGrandMap.Add(2)  = "Aud.(000) ";
	m_MapAbrGrandMap.Add(3)  = "Cts.(000) |Cts[+](000) ";
	m_MapAbrGrandMap.Add(4)  = "GRP ";
	m_MapAbrGrandMap.Add(5)  = "GRP[+] ";
	m_MapAbrGrandMap.Add(6)  = "C% cumul�e ";
	m_MapAbrGrandMap.Add(7)  = "R�pet";
	m_MapAbrGrandMap.Add(8)  = "R�pet[+]";
	m_MapAbrGrandMap.Add(9)  = "Ind.aff ";
	m_MapAbrGrandMap.Add(10) = "%Affinit� ";
	m_MapAbrGrandMap.Add(11) = "Ind.Cumul. ";
	m_MapAbrGrandMap.Add(12) = "Co�t/GRP � ";
	m_MapAbrGrandMap.Add(13) = "Co�t/C% � ";
	m_MapAbrGrandMap.Add(14) = "Co�t/000 Ind. � ";
	m_MapAbrGrandMap.Add(15) = "Co�t/000 Cts �|Co�t/000 Cts[+]� ";

	// Balaye toutes les donn�es exog�nes
	for (LstData.MoveFirst(); LstData.IsValid(); LstData.MoveNext())
	{
		// Recherche du libell�
		CString LabelData = LstData.GetItem(); 

		// Type de donn�es (chiffre, % , euros etc ....) >> par d�faut chiffre
		JUnt32 TypeData = JSRCDataExo::Type_Chiffre; 
		JUnt32 NbDec;
		JBool InfoOk = DataExo.GetTypeValeur(LabelData,TypeData,NbDec);

		// Recherche du 1er ident pour cette data exog�ne
		JUnt32 IdDepGrandeur;
		if (DataExo.GetIdDepGrandeur(LabelData, IdDepGrandeur))
		{
			// Test si nouvelle grandeur exog�ne
			m_MapAbrGrandMap.MoveTo(IdDepGrandeur);
			if (!m_MapAbrGrandMap.IsValid())
			{
				// Prend le libell� court / par d�faut le m�me
				CString AbrLabelData = LabelData; 

				// Extension info data (si <> data chiffre)
				CString ExtType = "";
				if (TypeData == JSRCDataExo::Type_Euro)
					ExtType = " � ";
				else if (TypeData == JSRCDataExo::Type_Prct)
					ExtType = " % ";

				if (DataExo.GetAbrLib(LabelData,AbrLabelData)) 
				{
					// Ajout nouvelle grandeur
					m_MapAbrGrandMap.Add(IdDepGrandeur) = AbrLabelData + ExtType; // + "(*)";
				}
				else
				{
					// par d�faut le m�me
					m_MapAbrGrandMap.Add(IdDepGrandeur) = LabelData + ExtType; // + "(*)";
				}
			}
			else
				// Erreur, id d�j� existant
				throw JInternalError::GetInstance();
		}
	}

	// Init tableau des grandeurs disponibles pour cumulativit�
	m_TabGrandCum.SetSize(10);
	m_TabGrandCum.SetAt(0,  "Audience");
	m_TabGrandCum.SetAt(1,  "Audience (000)");
	m_TabGrandCum.SetAt(2,  "Nb Contacts (000)|Nb contacts[+](000)");
	m_TabGrandCum.SetAt(3,  "GRP");
	m_TabGrandCum.SetAt(4,  "GRP [+]");
	m_TabGrandCum.SetAt(5,  "C% cumul�e");
	m_TabGrandCum.SetAt(6,  "R�p�tition");
	m_TabGrandCum.SetAt(7,  "R�p�tition[+]");
	m_TabGrandCum.SetAt(8,  "Indice Cumulativit�");
	m_TabGrandCum.SetAt(9,  "Co�t/%Couverture �");
	
	// Init tableau abr�g� des grandeurs disponibles pour cumulativit�
	m_TabAbrGrandCum.SetSize(10);
	m_TabAbrGrandCum.SetAt(0, "Audience ");
	m_TabAbrGrandCum.SetAt(1, "Aud.(000) ");
	m_TabAbrGrandCum.SetAt(2, "Cts.(000) |Cts[+](000) ");
	m_TabAbrGrandCum.SetAt(3, "GRP ");
	m_TabAbrGrandCum.SetAt(4, "GRP[+] ");
	m_TabAbrGrandCum.SetAt(5, "C% cumul�e ");
	m_TabAbrGrandCum.SetAt(6, "R�p�t");
	m_TabAbrGrandCum.SetAt(7, "R�p�t[+]");
	m_TabAbrGrandCum.SetAt(8, "Ind.Cumul. ");
	m_TabAbrGrandCum.SetAt(9, "Co�t/C% � ");

	// Init tableau des grandeurs disponibles pour duplication
	m_TabGrandDup.SetSize(6);
	m_TabGrandDup.SetAt(0, "LDP/LNM");
	m_TabGrandDup.SetAt(1, "LDP/LNM (000)");
	m_TabGrandDup.SetAt(2, "P�n. % cible");
	m_TabGrandDup.SetAt(3, "H%");
	m_TabGrandDup.SetAt(4, "V%");
	m_TabGrandDup.SetAt(5, "Indice proximit�");   // = Coefficient de Yule

	// Init tableau abr�g� grandeurs disponibles pour duplication
	m_TabAbrGrandDup.SetSize(6);
	m_TabAbrGrandDup.SetAt(0, "LDP/LNM");
	m_TabAbrGrandDup.SetAt(1, "LDP/LNM (000)");
	m_TabAbrGrandDup.SetAt(2, "P�n.% cib");
	m_TabAbrGrandDup.SetAt(3, "H%");
	m_TabAbrGrandDup.SetAt(4, "V%");
	m_TabAbrGrandDup.SetAt(5, "Ind. proxim.");
	
	// Sp�cifit�s des �lmts des grilles
	// Les fonts
	m_SizeFontHeader	= 8;
	m_SizeFontCell		= 8;

	// Les styles
	m_BoldFontHeader	= false;
	m_BoldFontCell		= false;

	m_ItalicResultIncomplet = true;
	m_ItalicResultComplet   = false;

	// Initialisation Map R�sultats Courbes
	m_MapResultCrbCum.Reset();

	// Initialisation Map positionnement des supports dans Mapping
	m_MapPointMapping.Reset(); 

	// Param�trage d'affichage nombre de d�cimal pour les co�ts
	// m_NbDecCout = 0;

	// Param�trage d'affichage des rangs r�sultats
	// m_AFFRangResultatEch = false;
	m_AFFRangResultatEch = true;

	// Param�trage d'affichage de la moyenne r�sultats
	m_AFFMoyenneResultatEch = true;   // false;

	// Init Liste Grandeurs destin�es aux calculs des moyenne
	m_TblIdGrandeurMoyenne.SetCount(ModuleDup+1);
	for (int i = ModuleEch; i <= ModuleDup; i++)
		m_TblIdGrandeurMoyenne.Item(i) = -1; 

	// Init �tat des rangs � afficher dans Echelle
	SetRangVisible(ModuleEch,false);
	// SetRangVisible(ModuleEch,true);

	// ******************************** OPTION CALCUL ****************************
	//
	// Prise en compte des reprises en main
	m_bRepriseEnMain			= AfxGetApp()->GetProfileInt("Moteur\\CS", "Reprise", 1) != 0?true:false;

	// Privil�gier l'audience LNM pour les magazines
	m_bLNMMag					= AfxGetApp()->GetProfileInt("Moteur\\CS", "LNM_Mag", 0) != 0?true:false;

	// Privil�gier l'audience LDP pour les quotidiens
	m_bLDPQtd					= AfxGetApp()->GetProfileInt("Moteur\\CS", "LDP_Qtd", 0) != 0?true:false;	

	// Privil�gier l'audience LDP 30 jours pour les bimestriels
	m_bLDP30J					= AfxGetApp()->GetProfileInt("Moteur\\CS", "LDP_30J", 0) != 0?true:false;

	// R�cup�ration mode de calcul pour les familles
	UINT Val = AfxGetApp()->GetProfileInt("Moteur\\CS", "CumulFam", 0);
	m_CumulFamille				= AfxGetApp()->GetProfileInt("Moteur\\CS", "CumulFam", 0);	

	int type = AfxGetApp()->GetProfileInt("Moteur\\CS", "Type", CMoteurPresse::ABAQUE_ETAL);
	m_MoteurPresse.SetType((CMoteurPresse::TYPEMOTEUR)type);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
JATPDocChoixSupport::~JATPDocChoixSupport(void)
{
	// Choix du moteur
	AfxGetApp()->WriteProfileInt("Moteur\\CS", "Type", m_MoteurPresse.GetType());

	// Prise en compte des reprises en main
	AfxGetApp()->WriteProfileInt("Moteur\\CS", "Reprise", m_bRepriseEnMain);

	// Privil�gier l'audience LNM pour les magazines
	AfxGetApp()->WriteProfileInt("Moteur\\CS", "LNM_Mag", m_bLNMMag);

	// Privil�gier l'audience LDP pour les quotidiens
	AfxGetApp()->WriteProfileInt("Moteur\\CS", "LDP_Qtd", m_bLDPQtd);

	// Privil�gier l'audience LDP 30 jours pour les bimestriels
	AfxGetApp()->WriteProfileInt("Moteur\\CS", "LDP_30J", m_bLDP30J);

	// Sauvegarde mode de calcul pour les familles
	AfxGetApp()->WriteProfileInt("Moteur\\CS", "CumulFam", m_CumulFamille);	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup index module courant
JBool JATPDocChoixSupport::RecupParamDef()
{
	// D�finition du fichier de sauvegarde (type canevas)
	CString path = AfxGetApp()->GetProfileString("Data", "UserPath", 0);
	CStdioFile FileDefaultCanevas;
	CString StrFile = path + "\\CSEch000.par";

	// Chargement liste et ordre des grandeurs �chelle par d�faut
	try
	{
		if (FileDefaultCanevas.Open(StrFile, CFile::modeRead | CFile::typeText))
		{
			// Lecture info param
			CString Txt;
			FileDefaultCanevas.ReadString(Txt);
			
			// Nb Grandeurs
			int NbGrandeur = -1;
			FileDefaultCanevas.ReadString(Txt);
			NbGrandeur = atoi(Txt);

			// Init Liste Ordo des grandeurs
			m_LstOrdoGrandeurEch.Reset();
			for (int i=0; i < NbGrandeur; i++)
			{
				// R�cup id grandeur
				JUnt32 IdGrandeur = -1;
				FileDefaultCanevas.ReadString(Txt);
				IdGrandeur = atoi(Txt);

				// V�rifie id grandeur correct
				m_MapAbrGrandEch.MoveTo(IdGrandeur);
				if (m_MapAbrGrandEch.IsValid())
				{
					if (IdGrandeur >= 0 && IdGrandeur < 1000)
						m_LstOrdoGrandeurEch.AddTail() = IdGrandeur;
					else
						return false;
				}
			}

			// Attention s'il manque des grandeurs
			if (NbGrandeur <= m_MapAbrGrandEch.GetCount())
			{
				// Boucle sur tous les identifiants grandeurs disponibles
				for (m_MapAbrGrandEch.MoveFirst();
					 m_MapAbrGrandEch.IsValid();
					 m_MapAbrGrandEch.MoveNext())
				{
					// On regarde si ident non d�j� pr�sent / Balaye la liste
					JBool NewIdent = true;
					for (m_LstOrdoGrandeurEch.MoveFirst(); 
						 m_LstOrdoGrandeurEch.IsValid(); 
						 m_LstOrdoGrandeurEch.MoveNext())
					{
						// R�cup�re valeur ident dans la liste
						JUnt32 IdGrandeur = m_LstOrdoGrandeurEch.GetItem();
						
						// Test si d�j� pr�sent
						if (m_MapAbrGrandEch.GetKey() == IdGrandeur)
						{
							NewIdent = false;
							break;
						}
					}

					// Ajoute si non pr�sent
					if (NewIdent)
					{
						m_LstOrdoGrandeurEch.AddTail() = m_MapAbrGrandEch.GetKey();
					}
				}
			}

			FileDefaultCanevas.Close(); 
		}
		else
			return false;
	}
	
	catch(...)
	{
	}

	// Pas de probl�me de chargement param�trages
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup index module courant
void JATPDocChoixSupport::SetModuleCourant(JUnt32 InxModule)
{
	m_InxModuleCur = InxModule;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Positionne index module courant
JUnt32 JATPDocChoixSupport::GetModuleCourant()
{
	return m_InxModuleCur;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Init Liste d'identicateur
void JATPDocChoixSupport::InitListFen(JList <JUnt32> &Lst, JUnt32 ValInit)
{
	Lst.Reset(); 
	for (int i = 0; i <= ModuleDup; i++)
		Lst.AddTail() = ValInit; 			
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Source d'Audience Courante
JUnt32 JATPDocChoixSupport::GetIdSrcAudienceSel()
{
	// R�cup�re ident source audience pour le module courant
	m_LstIdSrcAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSrcAudienceSel.IsValid()) 
		return m_LstIdSrcAudienceSel.GetItem();
	else
		return 0;
}
void JATPDocChoixSupport::SetIdSrcAudienceSel(JUnt32 Id)
{
	// On se positionne sur l'�lmt ident sources audience pour le module courant
	m_LstIdSrcAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSrcAudienceSel.IsValid()) 
		m_LstIdSrcAudienceSel.GetItem() = Id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup Source de Ciblage Courante
JUnt32 JATPDocChoixSupport::GetIdSrcCiblageSel()
{
	// R�cup�re ident sources ciblage pour le module courant
	m_LstIdSrcCiblageSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSrcCiblageSel.IsValid()) 
		return m_LstIdSrcCiblageSel.GetItem();
	else
		return 0;
}
// Set Source de Ciblage Courante
void JATPDocChoixSupport::SetIdSrcCiblageSel(JUnt32 Id)
{
	// On se positionne sur l'�lmt ident sources ciblage pour le module courant
	m_LstIdSrcCiblageSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSrcCiblageSel.IsValid()) 
		m_LstIdSrcCiblageSel.GetItem() = Id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Terrain Ciblage Selectionn�
JUnt32 JATPDocChoixSupport::GetIdTerrainCiblageSel()
{
	// R�cup�re ident terrain ciblage pour pour le module courant
	m_LstIdTerrainCiblageSel.MoveTo(m_InxModuleCur);
	if (m_LstIdTerrainCiblageSel.IsValid()) 
		return m_LstIdTerrainCiblageSel.GetItem();
	else
		return 0;
}
// Set Terrain de ciblage courant
void JATPDocChoixSupport::SetIdTerrainCiblageSel(JUnt32 Id)
{
	// On se positionne sur l'�lmt ident terrain ciblage pour le module courant
	m_LstIdTerrainCiblageSel.MoveTo(m_InxModuleCur);
	if (m_LstIdTerrainCiblageSel.IsValid()) 
		m_LstIdTerrainCiblageSel.GetItem() = Id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment d'Audience Selectionn�
JUnt32 JATPDocChoixSupport::GetIdSegmentAudienceSel()
{
	// R�cup�re ident segment audience s�l. pour le module courant
	m_LstIdSegmentAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSegmentAudienceSel.IsValid()) 
		return m_LstIdSegmentAudienceSel.GetItem();
	else
		return 0;
}
// Set Segment d'Audience Selectionn�
void JATPDocChoixSupport::SetIdSegmentAudienceSel(JUnt32 Id)
{
	// On se positionne sur l'�lmt segment d'audience s�lectionn� pour le module courant
	m_LstIdSegmentAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSegmentAudienceSel.IsValid()) 
		m_LstIdSegmentAudienceSel.GetItem() = Id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment de Ciblage Selectionn�
JUnt32 JATPDocChoixSupport::GetIdSegmentCibleSel()
{
	// R�cup�re ident segment cible s�l. pour le module courant
	m_LstIdSegmentCibleSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSegmentCibleSel.IsValid()) 
		return m_LstIdSegmentCibleSel.GetItem();
	else
		return 0;
}
// Set Segment de ciblage Selectionn�
void JATPDocChoixSupport::SetIdSegmentCibleSel(JUnt32 Id)
{
	// On se positionne sur l'�lmt segment de ciblage s�lectionn� pour le module courant
	m_LstIdSegmentCibleSel.MoveTo(m_InxModuleCur);
	if (m_LstIdSegmentCibleSel.IsValid()) 
		m_LstIdSegmentCibleSel.GetItem() = Id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Le filtre d'audience 
JUnt32 JATPDocChoixSupport::GetIdFiltreAudienceSel()
{
	// R�cup�re ident filtre d'audience pour le module courant
	m_LstIdFiltreAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstIdFiltreAudienceSel.IsValid()) 
	{
		return m_LstIdFiltreAudienceSel.GetItem();
	}
	else
		return 0;
}
// Set filtre d'audience
void JATPDocChoixSupport::SetIdFiltreAudienceSel(JUnt32 Id)
{
	// On se positionne sur l'�lmt filtre d'audience pour le module courant
	m_LstIdFiltreAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstIdFiltreAudienceSel.IsValid()) 
		m_LstIdFiltreAudienceSel.GetItem() = Id;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup map des terrains audience s�lectionn�s pour le module courant
JMap<JUnt32,JUnt32> * JATPDocChoixSupport::GetMapIdTerrainsAudienceSel()
{
	// R�cup�re map de terrains audience s�l.pour le module courant
	m_LstMapIdTerrainsAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdTerrainsAudienceSel.IsValid()) 
		return &m_LstMapIdTerrainsAudienceSel.GetItem();
	else
		// Ce cas ne doit jamais arriv� puisque la liste des maps doit �tre initialis�
		throw JInternalError::GetInstance();
}
// Sauve map des terrains audience s�lectionn�s pour le module courant
void JATPDocChoixSupport::SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId)
{
	JMap <JUnt32,JUnt32> MapInit;
	MapInit.Swap(MapId);

	// R�cup�re map de terrains audience s�l.pour le module courant
	m_LstMapIdTerrainsAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdTerrainsAudienceSel.IsValid()) 
		m_LstMapIdTerrainsAudienceSel.GetItem().Swap(MapInit);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup map des cibles audience s�lectionn�s pour le module courant
JMap<JUnt32,JUnt32> * JATPDocChoixSupport::GetMapIdCiblesAudienceSel()
{
	// R�cup�re map des cibles audience s�l.pour le module courant
	m_LstMapIdCiblesAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdCiblesAudienceSel.IsValid()) 
		return &m_LstMapIdCiblesAudienceSel.GetItem();
	else
		// Ce cas ne doit jamais arriv� puisque la liste des maps doit �tre initialis�
		throw JInternalError::GetInstance();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup map des segments audience s�lectionn�s pour le module courant
JMap<JUnt32,JUnt32> * JATPDocChoixSupport::GetMapIdSegmentsAudienceSel()
{
	// R�cup�re map des cibles audience s�l.pour le module courant
	// m_LstMapIdSegmentsAudienceSel.MoveTo(m_InxModuleCur);

	// Pour le moment toujours module �chelle (base s�lection segment)
	m_LstMapIdSegmentsAudienceSel.MoveTo(ModuleEch);
	if (m_LstMapIdSegmentsAudienceSel.IsValid()) 
		return &m_LstMapIdSegmentsAudienceSel.GetItem();
	else
		// Ce cas ne doit jamais arriv� puisque la liste des maps doit �tre initialis�
		throw JInternalError::GetInstance();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup map des segments audience s�lectionn�s pour le module indiqu�
JMap<JUnt32,JUnt32> * JATPDocChoixSupport::GetMapIdSegmentsAudienceSel(JUnt32 IndexModule)
{
	// R�cup�re map des cibles audience s�l.pour le module courant
	m_LstMapIdSegmentsAudienceSel.MoveTo(IndexModule);
	if (m_LstMapIdSegmentsAudienceSel.IsValid()) 
		return &m_LstMapIdSegmentsAudienceSel.GetItem();
	else
		// Ce cas ne doit jamais arriv� puisque la liste des maps doit �tre initialis�
		throw JInternalError::GetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////
// Indique si support appartient au(x) segment(x) s�lectionn�s
JVoid JATPDocChoixSupport::GetMapSupportValideSegTerrain(JMap<JUnt32,JUnt32> &MapSuppSegTerrain)
{
	// Init Map des supports valides
	MapSuppSegTerrain.Reset();

	// Instance regroupements items
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// Map des segments en cours pour le module
	JMap<JUnt32,JUnt32> *pMapIdSegmentsSel = GetMapIdSegmentsAudienceSel();

	// Map des terrains en cours pour le module
	JMap <JUnt32,JUnt32> *pMapIdTerrainsSel = GetMapIdTerrainsAudienceSel();

	// Boucle sur tous les terrains
	for (pMapIdTerrainsSel->MoveFirst(); pMapIdTerrainsSel->IsValid(); pMapIdTerrainsSel->MoveNext())  
	{
		// R�cup�re IdTerrain
		JUnt32 IdTerr = pMapIdTerrainsSel->GetKey();

		// Boucle sur tous les segments
		for (pMapIdSegmentsSel->MoveFirst(); pMapIdSegmentsSel->IsValid(); pMapIdSegmentsSel->MoveNext())   		
		{
			// R�cup�re Id Segment
			JUnt32 IdSeg = pMapIdSegmentsSel->GetKey();

			// Map des supports s�lectionn�s = m_MapIdSupportsSel
			for (m_MapIdSupportsSel.MoveFirst(); m_MapIdSupportsSel.IsValid(); m_MapIdSupportsSel.MoveNext())
			{
				// Id support
				JUnt32 IdSupport = m_MapIdSupportsSel.GetKey();

				// Identifie le support
				IElem* pelem = pRegroupements->GetIElemByID(IdSupport);

				// D�finit le support
				if (SupportValid(pelem, IdTerr, IdSeg))
				{
					// V�rifie qu'il n'est pas d�j� dans le map des supports valides
					MapSuppSegTerrain.MoveTo(IdSupport);
					if (!MapSuppSegTerrain.IsValid()) 
						MapSuppSegTerrain.Add(IdSupport);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Validit� support via les terrains s�lectionn�s et segments
JBool JATPDocChoixSupport::SupportValid(IElem* pelem, JUnt32 &IdTerrain, JUnt32 &IdSegment)
{
	// Element support non d�fini, on ne le prend pas 
	if (pelem == NULL) return false;

	// Audience support
	int IDAudience = 0;

	// V�rifie appartenance aux terrains s�lectionn�s
	CKeyTerrain KeyTerrain;
	KeyTerrain.m_IDSource = GetIdSrcAudienceSel();

	// R�cup Id Terrain
	KeyTerrain.m_IDTerrain = IdTerrain;
			
	// Voir validit� terrain
	CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(KeyTerrain);
			
	if (pTerrain != NULL)
	{
		// On teste d'abord si c'est un couplage (car couplage fixe, � la fois couplage et titre)
		if (pelem->IsCouplage())
		{
			// Cr�e liste des supports lot composant le couplage
			JList<IElem*> ListSup;
			pelem->AsCouplage()->GetFils(ListSup);

			// On ajoute les supports enfants au map supports
			for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				// R�cup�re Id Audience du composant couplage
				IDAudience = ListSup.GetItem()->AsITitre()->GetIDAudience();

				// V�rifie validit� id support sur ce couple terrain/segment
				if (!pTerrain->SupportBelongsToSegment( IDAudience, IdSegment))
					return false;
			}

			// Tous les composants couplage sont valides dans ce couple (terrain,segment)
			return true;
		}

		// Titre support simple
		else if (pelem->IsTitre())
		{
			// Ident audience support
			IDAudience = pelem->AsITitre()->GetIDAudience();

			// V�rifie validit� id support sur ce couple terrain/segment
			if (pTerrain->SupportBelongsToSegment( IDAudience, IdSegment))
				return true;
		}

		// NON une famille ayant ceratins supports non valides reste valide !!!!
		// Famille de supports (AJOUT NOVEMBRE 2006)
		else if (pelem->IsLot())
		{
			/*
			// Cr�e liste des supports lot composant la famille
			JList<IElem*> ListSup;
			pelem->AsLot()->GetFils(ListSup);

			// On ajoute les supports enfants au map supports
			for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				// R�cup�re Id Audience du composant couplage
				IDAudience = ListSup.GetItem()->AsITitre()->GetIDAudience();

				// V�rifie validit� id support sur ce couple terrain/segment
				if (!pTerrain->SupportBelongsToSegment( IDAudience, IdSegment))
					return false;
			}
			*/

			// Tous les composants famille sont valides dans ce couple (terrain,segment)
			return true;
		}

		// Support non d�termin�
		return false;
	}
	
	// Terrain non valide
	return false;
}

// Sauve map des segments audience s�lectionn�s pour le module courant
void JATPDocChoixSupport::SetMapIdSegmentsAudienceSel(JMap<JUnt32,JUnt32> &MapId)
{
	JMap <JUnt32,JUnt32> MapInit;
	MapInit.Swap(MapId);

	// R�cup�re map de segments audience s�l.pour le module courant
	m_LstMapIdSegmentsAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdSegmentsAudienceSel.IsValid()) 
		m_LstMapIdSegmentsAudienceSel.GetItem().Swap(MapInit);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Ident 1er cible s�lectionn�e
JUnt32 JATPDocChoixSupport::GetIdCibleSel()
{
	// R�cup�re map des cibles audience s�l.pour le module courant
	m_LstMapIdCiblesAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdCiblesAudienceSel.IsValid()) 
	{
		// On se positionne sur le 1er �lmt
		m_LstMapIdCiblesAudienceSel.GetItem().MoveFirst();

		// Renvoi l'ident de la cible
		if (m_LstMapIdCiblesAudienceSel.GetItem().IsValid())
			return m_LstMapIdCiblesAudienceSel.GetItem().GetKey();
		else
			throw JInternalError::GetInstance();
	}
	else
		// Ce cas ne doit jamais arriv� puisque la liste des maps doit �tre initialis�
		throw JInternalError::GetInstance();
}

// Sauve map des cibles audience s�lectionn�s pour le module courant
void JATPDocChoixSupport::SetMapIdCiblesAudienceSel(JMap<JUnt32,JUnt32> &MapId)
{
	JMap <JUnt32,JUnt32> MapInit;
	MapInit.Swap(MapId); 

	// R�cup�re map de terrains audience s�l.pour le module courant
	m_LstMapIdCiblesAudienceSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdCiblesAudienceSel.IsValid()) 
		m_LstMapIdCiblesAudienceSel.GetItem().Swap(MapInit);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Recup map des formats Actifs  pour le module courant
JMap<JUnt32,JUnt32> * JATPDocChoixSupport::GetMapIdFormatsSel()	
{
	// R�cup�re map de fomats actif pour le module courant
	m_LstMapIdFormatsSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdFormatsSel.IsValid()) 
		return &m_LstMapIdFormatsSel.GetItem();
	else
		// Ce cas ne doit jamais arriv� puisque la liste des maps doit �tre initialis�
		throw JInternalError::GetInstance();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Sauve map des formats Actifs  pour le module courant
void JATPDocChoixSupport::SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId)	
{
	JMap <JUnt32,JUnt32> MapInit;
	MapInit.Swap(MapId); 

	// R�cup�re map de fomats actif pour le module courant
	m_LstMapIdFormatsSel.MoveTo(m_InxModuleCur);
	if (m_LstMapIdFormatsSel.IsValid()) 
		m_LstMapIdFormatsSel.GetItem().Swap(MapInit);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion Map Visibilit� supports
//
// Positionne �tat visibilit� d'un support
void JATPDocChoixSupport::SetSupportVisible(JUnt32 &IdSupport, JBool Visible)
{
	// V�rifie au moins 2 supports restants
	if (GetNbSupportsVisible() > 1 || Visible == true) 
	{
		// On se place dans le map sur le support
		m_MapIdSupportsVisible.MoveTo(IdSupport);

		// V�rifie existence support
		if (m_MapIdSupportsVisible.IsValid())
		{
			// R�cup�re ancien �tat visibili�
			JBool &EtatVisible = m_MapIdSupportsVisible.GetItem(); 

			// Repositionne �tat visibilit�
			EtatVisible = Visible;
		}
	}
}

// Rend visible ou invisible tous les supports
void JATPDocChoixSupport::SetAllSupportVisible(JBool Visible)
{
	// Balaye tous les supports existants
	for (m_MapIdSupportsVisible.MoveFirst();
		 m_MapIdSupportsVisible.IsValid();
		 m_MapIdSupportsVisible.MoveNext())
	{
		// R�cup�re info visible
		JBool &EtatVisible = m_MapIdSupportsVisible.GetItem(); 

		// Positionne �tat visibilit� de chaque support
		EtatVisible = true;
	}

	// Attention si �tat visible = false, laisser au moins le 1er support visible
	if (Visible == false)
	{
		m_MapIdSupportsVisible.MoveFirst();
		JBool &EtatVisible = m_MapIdSupportsVisible.GetItem(); 
		EtatVisible = true;
	}

	// Filtre � la fin par les supports possibles dans le couple (terrain(s), segment(s))
	// FiltreSupportsSegments();
	SetSupportsSegmentsVisible();
}

// Rend visible ou invisible tous les supports s�lectionn�s
void JATPDocChoixSupport::SetAllSupportSelVisible(JBool Visible)
{
	// Index 1er support s�lectionn�
	JUnt32 Id1erSupportSel = -1;

	// Filtre � la fin par les supports possibles dans le couple (terrain(s), segment(s))
	// FiltreSupportsSegments();
	SetSupportsSegmentsVisible();

	// Balaye tous les supports existants

	for (m_MapIdSupportsSel.MoveLast();
		 m_MapIdSupportsSel.IsValid();
		 m_MapIdSupportsSel.MovePrev())
	{
		// R�cup�re info visible
		JUnt32 IdSupport = m_MapIdSupportsSel.GetKey(); 

		// Est ce un support ??
		if (IdSupport > 0)
		{
			// Voir si c'est un support s�lectionn�
			if (m_MapIdSupportsSel.GetItem() == true)
			{
				// Positionne �tat visibilit�
				SetSupportVisible(IdSupport, Visible);
				Id1erSupportSel = IdSupport;

				// Nb support visible
				JUnt32 NbVisible = GetNbSuppVisible();
				int stop = 1;
			}
			else
			{
				// Positionne �tat visibilit� de chaque support
				SetSupportVisible(IdSupport, !Visible);
			}
		}	
	}

	
	/*
	// Il faut au moins 1 support visible
	if (GetNbSupportsVisible() == 0) 
	{
		// Positionne 1er support s�lectionn� visible
		SetSupportVisible(Id1erSupportSel, true);
	}
	*/

	/*
	for (m_MapIdSupportsSel.MoveLast();
		 m_MapIdSupportsSel.IsValid();
		 m_MapIdSupportsSel.MovePrev())
	{
		// R�cup�re info visible
		JUnt32 IdSupport = m_MapIdSupportsSel.GetKey(); 
	}
	*/
}

// Rend visible ou invisible tous les supports non s�lectionn�s
void JATPDocChoixSupport::SetAllSupportNonSelVisible(JBool Visible)
{
	// Index 1er support non s�lectionn�
	JUnt32 Id1erSupportSel = -1;

	// Filtre � la fin par les supports possibles dans le couple (terrain(s), segment(s))
	// FiltreSupportsSegments();
	SetSupportsSegmentsVisible();

	// Balaye tous les supports existants
	for (m_MapIdSupportsSel.MoveLast();
		 m_MapIdSupportsSel.IsValid();
		 m_MapIdSupportsSel.MovePrev())
	{
		// R�cup�re info visible
		JUnt32 IdSupport = m_MapIdSupportsSel.GetKey(); 

		// Est ce un support ??
		if (IdSupport > 0)
		{
			// Voir si c'est un support s�lectionn�
			if (m_MapIdSupportsSel.GetItem() == false)
			{
				// Positionne �tat visibilit�
				SetSupportVisible(IdSupport, Visible);
				Id1erSupportSel = IdSupport;

				// Nb support visible
				JUnt32 NbVisible = GetNbSuppVisible();
				int stop = 1;
			}
			else
			{
				// Positionne �tat visibilit� de chaque support
				SetSupportVisible(IdSupport, !Visible);
			}
		}	
	}

	/*
	// Il faut au moins 1 support visible
	if (GetNbSupportsVisible() == 0) 
	{
		// Positionne 1er support s�lectionn� visible
		SetSupportVisible(Id1erSupportSel, true);
	}
	*/
}

// R�cup�re �tat visibilit� d'un support
JBool JATPDocChoixSupport::GetSupportVisible(JUnt32 &IdSupport)
{
	// On se place dans le map sur le support
	m_MapIdSupportsVisible.MoveTo(IdSupport);

	// V�rifie existence support
	if (m_MapIdSupportsVisible.IsValid())
	{
		// Renvoi �tat visibilit�
		return (m_MapIdSupportsVisible.GetItem()); 
	}

	// Dans tous les autres cas, par d�faut pas visible
	return false;
}

// Nombre de supports visibles
JUnt32 JATPDocChoixSupport::GetNbSuppVisible()
{
	// Init Nb supports visibles
	JUnt32 NbSuppVisible = 0;

	// Balaye tous les supports existants
	for (m_MapIdSupportsVisible.MoveFirst();
		 m_MapIdSupportsVisible.IsValid();
		 m_MapIdSupportsVisible.MoveNext())
	{
		// Supports visibles
		NbSuppVisible += (m_MapIdSupportsVisible.GetItem() == true); 
	}

	// Retourne le nombre de support encore visible
	return NbSuppVisible;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// S�lection ou d�selection d'un support (ou famille)
void JATPDocChoixSupport::SetSelSupport(JUnt32 &IdSupport, JBool Selection)
{
	// V�rifie que le support a bien �t� choisi
	m_MapIdSupportsSel.MoveTo(IdSupport);

	// V�rifie que le support a bien �t� choisi
	if (m_MapIdSupportsSel.IsValid())
	{
		// Repositionne �tat du support
		JBool &EtatSupport = m_MapIdSupportsSel.GetItem();
		EtatSupport = Selection;
	}
	else
	{
		// Ajoute nouvel �lmt
		m_MapIdSupportsSel.Add(IdSupport) = Selection; 
	}

}

////////////////////////////////////////////////////////////////////////////////////////////
// Filtre des supports via segment(s) s�lectionn�s
void JATPDocChoixSupport::FiltreSupportsSegments()
{
	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	GetMapSupportValideSegTerrain(MapSuppSegTerr);

	// Boucle sur les supports de l'offre
	for (m_MapIdSupportsSel.MoveFirst();m_MapIdSupportsSel.IsValid();m_MapIdSupportsSel.MoveNext())
	{
		// R�cup�re id support
		JUnt32 IdSupport = m_MapIdSupportsSel.GetKey(); 

		// V�rifie que ce support est valide sur le couple (terrain(s) + segment(s))
		MapSuppSegTerr.MoveTo(IdSupport);
		if (!MapSuppSegTerr.IsValid() && GetSupportVisible(IdSupport) == true)
		{
			// Ce support ne peut �tre visible
    		SetSupportVisible(IdSupport,false);
		}

		else if (MapSuppSegTerr.IsValid() && GetSupportVisible(IdSupport) == false)
		{
				// Ce support n'�tait pas visible, on le rend visible
    		SetSupportVisible(IdSupport,true);
		}
	}

	// Mise � jour de la grille �chelle
	UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Filtre des supports via segment(s) s�lectionn�s
void JATPDocChoixSupport::SetSupportsSegmentsVisible()
{
	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	GetMapSupportValideSegTerrain(MapSuppSegTerr);

	// Boucle sur les supports de l'offre
	for (m_MapIdSupportsSel.MoveFirst();m_MapIdSupportsSel.IsValid();m_MapIdSupportsSel.MoveNext())
	{
		// R�cup�re id support
		JUnt32 IdSupport = m_MapIdSupportsSel.GetKey(); 

		// V�rifie que ce support est valide sur le couple (terrain(s) + segment(s))
		MapSuppSegTerr.MoveTo(IdSupport);
		if (!MapSuppSegTerr.IsValid())
		{
			// Ce support ne peut �tre visible
    		SetSupportVisible(IdSupport,false);
		}
	}

	// A VOIR Octobre 2006 Mise � jour des grilles
	/*
	if (m_NbTotSegDispo > 1)
		UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);
	*/	
	// Pas la peine de savoir si + ou - 1 segment, on le fait d'office
	UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);
}

////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration validation support sur environnement terrain/segment s�lectionn�
JBool JATPDocChoixSupport::ValideSupportSegments(JUnt32 IdSupport)
{
	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	GetMapSupportValideSegTerrain(MapSuppSegTerr);

	// V�rifie que ce support est valide sur le couple (terrain(s) + segment(s))
	MapSuppSegTerr.MoveTo(IdSupport);
	if (!MapSuppSegTerr.IsValid())
	{
		// Ce support n'est pas valide sur terrain(s) et segment(s) en cours d'utilisation
    	return false;
	}
	else
	{
		// Ce support est bien valide sur terrain(s) et segment(s) en cours d'utilisation
    	return true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Visibilit� des �lmts supports ou familles
JBool JATPDocChoixSupport::VisibleSupportMode(JUnt32 IdSupport)
{
	if (IdSupport != 0)
	{
		// R�cup id support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		// Si support et mode support actif
		if ((pElem->IsTitre() || pElem->IsCouplage()) && m_AFFModeSupports == TRUE)
			return true;
		
		// Si famille et mode famille actif
		if (pElem->IsLot() && m_AFFModeFamilles == TRUE)
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul de la clef segment
JUnt32	JATPDocChoixSupport::ClefSegment(JUnt32 InxModule)
{
	// Init Clef segment
	JUnt32 CleSeg = 0;

	// R�cup�re le map segments du module
	JMap <JUnt32,JUnt32> *pMapIdSegmentsSel = GetMapIdSegmentsAudienceSel(); 

	// Cr�e la clef en bouclant sur tous les segments s�lectionn�es
	for (pMapIdSegmentsSel->MoveFirst();
		 pMapIdSegmentsSel->IsValid();
		 pMapIdSegmentsSel->MoveNext())
	{
		// R�cup�re id segment (par convention toujours < 32)
		JUnt32 IdSeg = pMapIdSegmentsSel->GetKey();

		// Ajoute puissance de 2 ad�quate
		CleSeg += (int)pow((float)2.0,(float)IdSeg-1);
	}

	// Renvoi valeur clef trouv�e
	return CleSeg;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re nb grandeurs disponibles	
JUnt32 JATPDocChoixSupport::GetNbGrandeurs(JUnt32 TypeModule)
{
	switch (TypeModule)
	{
		// Nombre de grandeurs calcul �chelle
		case ModuleEch :
			return (static_cast<JUnt32>(m_MapGrandEch.GetCount())); 
			break;

		// Nombre de grandeurs calcul mapping (idem que pour �chelle)
		case ModuleMap :
			return (static_cast<JUnt32>(m_MapGrandMap.GetCount())); 
			break;

		case ModuleCum :
			return (static_cast<JUnt32>(m_TabGrandCum.GetCount()));
			break;

		case ModuleDup :
			return (static_cast<JUnt32>(m_TabGrandDup.GetCount()));
			break;

		default :
			return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re libell� grandeur
CString JATPDocChoixSupport::GetGrandeur(JUnt32 TypeModule, JUnt32 IndexGrandeur)
{
	switch (TypeModule)
	{
		// Echelle
		case ModuleEch :
		{
			// V�rifie la validit� du code grandeur
			m_MapGrandEch.MoveTo(IndexGrandeur);

			// Si non valide, on continue en pr�venant l'utilisateur
			if (!m_MapGrandEch.IsValid())
			{
				// Cr�ation message utilisateur
				CString TxtMess = "Grandeur sp�cifique ";
				TxtMess.Format("%s %d %s", "Grandeur sp�cifique code ",IndexGrandeur," non trouv�e");
				AfxMessageBox(TxtMess);
				break;
			}

			//ASSERT(m_MapTabGrandEch.IsValid());
			CString TxtGrandeur = m_MapGrandEch.GetItem();

			// Attention dans certains cas le libell� change
			int SpeCar = TxtGrandeur.FindOneOf("|");
			if (SpeCar < 0)
				// Toute la chaine � �crire
				return (TxtGrandeur); 
			else
			{
				// 2 chaines distinctes
				CString LTxtGrandeur = TxtGrandeur.Left(SpeCar);
				CString RTxtGrandeur = TxtGrandeur.Mid(SpeCar+1);

				// Cas sp�cifique nb contacts ([+] en macro-proba....)
				if (IndexGrandeur == GrdEch_NbContact000)
				{
					if (m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si moteur autre que Full Binomial
						return (RTxtGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtGrandeur); 
				}
				else
				{
					// Distinction nb insertions et type moteur
					if (m_NbInsertions > 1 || m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si plus d'une insertion partie droite (GRP+, Cts+ etc...)
						return (RTxtGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtGrandeur); 
				}
			}
		}
		break;

		// Mapping
		case ModuleMap :
		{
			/*
			// Test si index valide
			ASSERT(IndexGrandeur <m_TabGrandMap.GetCount());

			// Renvoi libell� grandeur pour cet index
			CString TxtGrandeur = m_TabGrandMap.GetAt(IndexGrandeur);
			*/
			// V�rifie la validit� du code grandeur
			m_MapGrandMap.MoveTo(IndexGrandeur);

			// Si non valide, on continue en pr�venant l'utilisateur
			if (!m_MapGrandMap.IsValid())
			{
				// Cr�ation message utilisateur
				CString TxtMess = "Grandeur sp�cifique ";
				TxtMess.Format("%s %d %s", "Grandeur sp�cifique code ",IndexGrandeur," non trouv�e");
				AfxMessageBox(TxtMess);
				break;
			}

			// R�cup�re le libell�
			CString TxtGrandeur = m_MapGrandMap.GetItem();

			// Attention dans certains cas le libell� change
			int SpeCar = TxtGrandeur.FindOneOf("|");
			if (SpeCar < 0)
				// Toute la chaine � �crire
				return (TxtGrandeur); 
			else
			{
				// 2 chaines distinctes
				CString LTxtGrandeur = TxtGrandeur.Left(SpeCar);
				CString RTxtGrandeur = TxtGrandeur.Mid(SpeCar+1);

				// Cas sp�cifique nb contacts ([+] en macro-proba....)
				if (IndexGrandeur == GrdMap_NbContact000)
				{
					if (m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si moteur autre que Full Binomial
						return (RTxtGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtGrandeur); 
				}
				else
				{
					// Distinction nb insertions et type moteur
					if (m_NbInsertions > 1 || m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si plus d'une insertion partie droite (GRP+, Cts+ etc...)
						return (RTxtGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtGrandeur); 
				}
			}
		}
		break;

		case ModuleCum :
		{
			// Test si index valide
			ASSERT(static_cast<INT_PTR>(IndexGrandeur) < m_TabGrandCum.GetCount());

			// Renvoi libell� grandeur pour cet index
			CString TxtGrandeur = m_TabGrandCum.GetAt(IndexGrandeur);

			// Attention dans certains cas le libell� change
			int SpeCar = TxtGrandeur.FindOneOf("|");
			if (SpeCar < 0)
				// Toute la chaine � �crire
				return (TxtGrandeur); 
			else
			{
				// 2 chaines distinctes
				CString LTxtGrandeur = TxtGrandeur.Left(SpeCar);
				CString RTxtGrandeur = TxtGrandeur.Mid(SpeCar+1);

				// Cas sp�cifique nb contacts ([+] en macro-proba....)
				if (IndexGrandeur == GrdCum_NbContact000)
				{
					if (m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si moteur autre que Full Binomial
						return (RTxtGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtGrandeur); 
				}
				else
				{
					// Distinction nb insertions et type moteur
					if (m_NbInsertions > 1 || m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si plus d'une insertion partie droite (GRP+, Cts+ etc...)
						return (RTxtGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtGrandeur); 
				}
			}

			// Renvoi libell� grandeur pour cet index
			// return (m_TabGrandCum.GetAt(IndexGrandeur)); 
		}
		break;

		case ModuleDup :
		{
			// Test si index valide
			ASSERT(static_cast<INT_PTR>(IndexGrandeur) < m_TabGrandDup.GetCount());
			return (m_TabGrandDup.GetAt(IndexGrandeur));
		}
		break;
		
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re libell� abr�g� grandeur
CString JATPDocChoixSupport::GetAbrGrandeur(JUnt32 TypeModule, JUnt32 IndexGrandeur)
{
	switch (TypeModule)
	{
		// Echelle
		case ModuleEch :
		{
			// Recherche libell�
			m_MapAbrGrandEch.MoveTo(IndexGrandeur);
			ASSERT(m_MapAbrGrandEch.IsValid());
			CString TxtAbrGrandeur = m_MapAbrGrandEch.GetItem();

			// Attention dans certains cas le libell� change
			int SpeCar = TxtAbrGrandeur.FindOneOf("|");
			if (SpeCar < 0)
				// Toute la chaine � �crire
				return (TxtAbrGrandeur); 
			else
			{
				// 2 chaines distinctes
				CString LTxtAbrGrandeur = TxtAbrGrandeur.Left(SpeCar);
				CString RTxtAbrGrandeur = TxtAbrGrandeur.Mid(SpeCar+1);

				// Cas sp�cifique nb contacts ([+] en macro-proba....)
				if (IndexGrandeur == GrdEch_NbContact000)
				{
					if (m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si moteur autre que Full Binomial
						return (RTxtAbrGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtAbrGrandeur); 
				}
				else
				{
					// Distinction nb insertions et type moteur
					if (m_NbInsertions > 1 || m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si plus d'une insertion partie droite (GRP+, Cts+ etc...)
						return (RTxtAbrGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtAbrGrandeur); 
				}
			}
		}
		break;

		// Mapping
		case ModuleMap : 
		{

			// Recherche libell�
			m_MapAbrGrandMap.MoveTo(IndexGrandeur);
			ASSERT(m_MapAbrGrandMap.IsValid());
			CString TxtAbrGrandeur = m_MapAbrGrandMap.GetItem();

			// Attention dans certains cas le libell� change
			int SpeCar = TxtAbrGrandeur.FindOneOf("|");
			if (SpeCar < 0)
				// Toute la chaine � �crire
				return (TxtAbrGrandeur); 
			else
			{
				// 2 chaines distinctes
				CString LTxtAbrGrandeur = TxtAbrGrandeur.Left(SpeCar);
				CString RTxtAbrGrandeur = TxtAbrGrandeur.Mid(SpeCar+1);

				// Cas sp�cifique nb contacts ([+] en macro-proba....)
				if (IndexGrandeur == GrdMap_NbContact000)
				{
					if (m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si moteur autre que Full Binomial
						return (RTxtAbrGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtAbrGrandeur); 
				}
				else
				{
					// Distinction nb insertions et type moteur
					if (m_NbInsertions > 1 || m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si plus d'une insertion partie droite (GRP+, Cts+ etc...)
						return (RTxtAbrGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtAbrGrandeur); 
				}
			}
		}
		break;

		case ModuleCum :
		{
			// Test si index valide
			ASSERT(static_cast<INT_PTR>(IndexGrandeur) < m_TabAbrGrandCum.GetCount());

			// Renvoi libell� abr�g� grandeur pour cet index
			CString TxtAbrGrandeur = m_TabAbrGrandCum.GetAt(IndexGrandeur);

			// Attention dans certains cas le libell� change
			int SpeCar = TxtAbrGrandeur.FindOneOf("|");
			if (SpeCar < 0)
				// Toute la chaine � �crire
				return (TxtAbrGrandeur); 
			else
			{
				// 2 chaines distinctes
				CString LTxtAbrGrandeur = TxtAbrGrandeur.Left(SpeCar);
				CString RTxtAbrGrandeur = TxtAbrGrandeur.Mid(SpeCar+1);

				// Cas sp�cifique nb contacts ([+] en macro-proba....)
				if (IndexGrandeur == GrdCum_NbContact000)
				{
					if (m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si moteur autre que Full Binomial
						return (RTxtAbrGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtAbrGrandeur); 
				}
				else
				{
					// Distinction nb insertions et type moteur
					if (m_NbInsertions > 1 || m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
						// Si plus d'une insertion partie droite (GRP+, Cts+ etc...)
						return (RTxtAbrGrandeur); 
					else
						// Sinon partie gauche
						return (LTxtAbrGrandeur); 
				}
			}

			// Renvoi libell� grandeur pour cet index
			// return (m_TabAbrGrandCum.GetAt(IndexGrandeur)); 
		}
		break;

		case ModuleDup :
			return "";
			break;
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Archivage �lmt Choix Support
JVoid JATPDocChoixSupport::Send(JStream & Stream)
{
	CATPDocCSSerializer::Send(Stream, this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// D�sarchivage �lmt Choix Support
JVoid JATPDocChoixSupport::Recv(JStream & Stream)
{
	CATPDocCSSerializer::Recv(Stream, this);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// D�sarchivage �lmt No Version Choix Support  (Bidouille pour r�cup�rer N� Version ss mode CFile)
JVoid JATPDocChoixSupport::RecvNoVersion(JStream & Stream, JUnt32 & NoVersion )
{
	CATPDocCSSerializer::RecvNoVersion(Stream, NoVersion, this);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Affectation �lmt Choix Support
JATPDocChoixSupport & JATPDocChoixSupport::operator =(const JATPDocChoixSupport & Source)
{
	return (*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Permutation documents choix support
JVoid JATPDocChoixSupport::Swap(JATPDocChoixSupport & Source)
{
	// Swap the elements
	JUnt32	IdTmp;
	JDate	DateTmp;

	// Etat modif
	BOOL ModifTmp = Source.IsModified(); 
	SetModifiedFlag(Source.IsModified());
	Source.SetModifiedFlag(ModifTmp); 

	// Le plateau
	m_KEYPlateau				.Swap(Source.m_KEYPlateau);

	// La date tarifaire
	DateTmp						=	m_DateTarifaire;
	m_DateTarifaire				=	Source.m_DateTarifaire;
	Source.m_DateTarifaire		=	DateTmp;

	// Map des supports s�lectionn�s
	m_MapIdSupportsSel			.Swap(Source.m_MapIdSupportsSel);

	// Map des supports visibles
	m_MapIdSupportsVisible			.Swap(Source.m_MapIdSupportsVisible);

	// Map des couleurs supports s�lectionn�s
	m_MapIdColorSupports		.Swap(Source.m_MapIdColorSupports);

	// Map des cibles audience s�lectionn�s
	m_LstMapIdCiblesAudienceSel	.Swap(Source.m_LstMapIdCiblesAudienceSel);

	// Map des Grandeurs s�lectionn�es pour tous les modules
	m_MapIdGrandEchSel			.Swap(Source.m_MapIdGrandEchSel);
	//m_MapIdGrandMapSel			.Swap(Source.m_MapIdGrandMapSel);
	m_MapIdGrandCumSel			.Swap(Source.m_MapIdGrandCumSel);
	m_MapIdGrandDupSel			.Swap(Source.m_MapIdGrandDupSel);

	// Grandeur abscisse et ordonn�e du mapping
	IdTmp						=	m_IdGrandMapSelAbs;
	m_IdGrandMapSelAbs			=	Source.m_IdGrandMapSelAbs;
	Source.m_IdGrandMapSelAbs	=   IdTmp;
	IdTmp						=	m_IdGrandMapSelOrd;
	m_IdGrandMapSelOrd			=	Source.m_IdGrandMapSelOrd;
	Source.m_IdGrandMapSelOrd	=   IdTmp;

	// Map des Formats s�lectionn�s pour tous les modules
	m_MapIdFormatSpecifEch		.Swap(Source.m_MapIdFormatSpecifEch);
	m_MapIdFormatSpecifMap		.Swap(Source.m_MapIdFormatSpecifMap);
	m_MapIdFormatSpecifCum		.Swap(Source.m_MapIdFormatSpecifCum);
	m_MapIdFormatSpecifDup		.Swap(Source.m_MapIdFormatSpecifDup);

	// Map des positions Mapping
	m_MapPointMapping			.Swap(Source.m_MapPointMapping);  

	// Nb insertions s�lectionn� pour calcul
	IdTmp						=	m_NbInsertions;
	m_NbInsertions				=	Source.m_NbInsertions;
	Source.m_NbInsertions		=   IdTmp;

	// Ordonnancement Liste Grandeurs Echelle � afficher
	m_LstOrdoGrandeurEch		.Swap(Source.m_LstOrdoGrandeurEch);

	// La liste des sources d'audience par module
	m_LstIdSrcAudienceSel		.Swap(Source.m_LstIdSrcAudienceSel);

	// La liste des source de ciblage par module
	m_LstIdSrcCiblageSel		.Swap(m_LstIdSrcCiblageSel);

	// Map des terrains s�lectionn�s
	m_LstMapIdTerrainsAudienceSel	.Swap(Source.m_LstMapIdTerrainsAudienceSel);  

	// Map des segments s�lectionn�s
	m_LstMapIdSegmentsAudienceSel	.Swap(Source.m_LstMapIdSegmentsAudienceSel);  

	// La liste des idents terrain pour ciblage
	m_LstIdTerrainCiblageSel	.Swap(Source.m_LstIdTerrainCiblageSel);

	// Map des formats s�lectionn�s
	m_LstMapIdFormatsSel		.Swap(Source.m_LstMapIdFormatsSel);

	// Ident segment d'audience
	m_LstIdSegmentAudienceSel	.Swap(Source.m_LstIdSegmentAudienceSel); 

	// Ident segment pour ciblage
	m_LstIdSegmentCibleSel		.Swap(Source.m_LstIdSegmentCibleSel);

	// Permut infos filtre audience
	m_LstIdFiltreAudienceSel	.Swap(Source.m_LstIdFiltreAudienceSel);

	// Permute l'offre
	m_Offre						.Swap(Source.m_Offre);

	// Map des items offre
	m_MAPpItems					.Swap(Source.m_MAPpItems);

	// Tab ordre header grille �chelle
	INT_PTR i = 0;
	for (; i < m_OrdreEnteteEch.GetCount(); i++)
	{
		JUnt32 Tmp					=	m_OrdreEnteteEch[i];
		m_OrdreEnteteEch[i]			=	Source.m_OrdreEnteteEch[i];
		Source.m_OrdreEnteteEch[i]	=   Tmp;
	}

	// Tab informations tri colonnes
	for (i = 0; i < m_TabInfoTriCol.GetCount(); i++)
	{
		CInfoTriCol  Tmp			=	m_TabInfoTriCol[i];
		m_TabInfoTriCol[i]			=	Source.m_TabInfoTriCol[i];
		Source.m_TabInfoTriCol[i]	=   Tmp;
	}

	// Param�trage d'affichage nombre de d�cimal pour les co�ts
	m_NbDecCout = Source.m_NbDecCout;

	// Param�trage d'affichage des rangs r�sultats
	m_AFFRangResultatEch = Source.m_AFFRangResultatEch;

	// Param�trage d'affichage de la moyenne
	m_AFFMoyenneResultatEch = Source.m_AFFMoyenneResultatEch;

	// Recopie Liste Grandeurs destin�es aux calculs des moyenne
	for (i = ModuleEch; i <= ModuleDup; i++)
		m_TblIdGrandeurMoyenne.Item(static_cast<JInt32>(i)) = Source.m_TblIdGrandeurMoyenne.Item(static_cast<JInt32>(i)); 

	// Recopie map �tat des rangs � afficher dans Echelle
	m_MapEtatRangResultEch	.Swap(Source.m_MapEtatRangResultEch);

	// Les options calcul
	m_bRepriseEnMain	= Source.m_bRepriseEnMain; 
	m_bLNMMag			= Source.m_bLNMMag;
    m_bLDPQtd			= Source.m_bLDPQtd; 		
	m_bLDP30J			= Source.m_bLDP30J; 
	m_CumulFamille		= Source.m_CumulFamille; 
	int TypeMoteur		= Source.m_MoteurPresse.GetType(); 
	m_MoteurPresse.SetType((CMoteurPresse::TYPEMOTEUR)TypeMoteur);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Validit� du document choix support
JBool JATPDocChoixSupport::IsValid() const
{
	// Valide si date tarifaire OK
	return (m_DateTarifaire.IsValid());
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Synchronisation s�lection supports 
JVoid JATPDocChoixSupport::SynchroSupports()
{

}

///////////////////////////////////////////////////
// la fonction pour pr�parer le contenu du document
BOOL JATPDocChoixSupport::OnPrepareContents()
{
	// on initialise l'indicateur
	BOOL Ret = TRUE;

	// on teste les exceptions
	try
	{
		// Synchronisation des listes supports (Echelle +�Plateau)
		this->SynchroSupports(); 
	}
	catch (JException * pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
	}

	// on renvoie l'indicateur
	return (Ret);
}

/////////////////////////////////////////////
// la fonction pour mettre � jour le document
VOID JATPDocChoixSupport::OnUpdate(const JFCUpdate & Message)
{
	// on positionne le curseur en mode attente
	CWaitCursor Wait;
	Wait.Restore();

	/////////////////// CHANGEMENT DE L'OFFRE OU INIT  /////////////////////
	// 
	if (Message == UPDATE_OFFRE			|| 
		Message == UPDATE_INITIAL		|| 
		Message == UPDATE_LOADPLATEAU	||
		Message == UPDATE_SEL_SEGMENT)
	{
		// on teste les exceptions
		try
		{
			// on charge les Items Cibles
			this->OnLoadItems();

			// on charge les Supports
			this->OnLoadSupports();

			// on charge les segments 
			this->OnLoadSegPop();

			// on charge les formats
			this->OnLoadFormats();

			// on charge les terrains que pour update initial ou update plateau
			if (Message == UPDATE_INITIAL || Message == UPDATE_LOADPLATEAU)
			{
				// Init Moteur Atelier Presse
				JUnt32 IdFiltreAudienceSel		= GetIdFiltreAudienceSel();
				m_MoteurPresse.Init(m_Offre,IdFiltreAudienceSel,
					m_bLDPQtd,m_bLNMMag,m_bLDP30J,m_bRepriseEnMain); 

				// Positionne le type de moteur (// A FAIRE)
				//JUnt32 Type = AfxGetApp()->GetProfileInt("Moteur", "Type", 1);
				//m_MoteurPresse.SetType((CMoteurPresse::TYPEMOTEUR)Type);
					
				// Charge terrains
				if (!this->OnLoadTerrains())
				{
					AfxMessageBox("Chargement terrains impossible !!!",MB_ICONINFORMATION);
					return;
				}

				CKeyTerrain keyTerrain;
				JMap <JUnt32,JUnt32> *pMapIdTerrainsAudienceSel = GetMapIdTerrainsAudienceSel();
				pMapIdTerrainsAudienceSel->MoveFirst();

				keyTerrain.Set(GetIdSrcAudienceSel(), pMapIdTerrainsAudienceSel->GetKey());
				if (!m_MoteurPresse.CanAbaque(keyTerrain))
					m_MoteurPresse.SetType(CMoteurPresse::PROBA);

				// R�init moteur avec segments en cours.
				m_MoteurPresse.SetOffre(m_Offre, GetIdFiltreAudienceSel(), GetMapIdSegmentsAudienceSel()); 
			}

			if (Message == UPDATE_OFFRE || Message == UPDATE_SEL_SEGMENT)
			{
				// Changement terrains
				this->OnChangeTerrains();

				// R�init moteur avec nvlle offre (VOIR OPTIM)
				JUnt32 IdFiltreAudienceSel = GetIdFiltreAudienceSel();
				JMap <JUnt32, JUnt32> * MapIdSegAudSel = GetMapIdSegmentsAudienceSel();
				m_MoteurPresse.SetOffre(m_Offre, GetIdFiltreAudienceSel(), GetMapIdSegmentsAudienceSel()); 
	
				CKeyTerrain keyTerrain;
				JMap <JUnt32,JUnt32> *pMapIdTerrainsAudienceSel = GetMapIdTerrainsAudienceSel();
				pMapIdTerrainsAudienceSel->MoveFirst();
				keyTerrain.Set(GetIdSrcAudienceSel(), pMapIdTerrainsAudienceSel->GetKey());
				if (!m_MoteurPresse.CanAbaque(keyTerrain))
				{
					// Change le type du moteur
					m_MoteurPresse.SetType(CMoteurPresse::PROBA);

					// Mise � jour 
					// UpdateAllViews(UPDATE_TYPEMOTEUR);
				}
			}
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	/////////////////// CHANGEMENT DATE TARIFAIRE ////////////////////////////
	//
	else if (Message == UPDATE_DATE_TARIFAIRE)
	{
		// on teste les exceptions
		/*
		try
		{
			// Mise � jour Info Date Tarifaire
			AfxMessageBox("A FAIRE // DATE TARIFAIRE A CHANGEE");
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
		*/
	}

	////////////////////// CHANGEMENT SELECTION TERRAINS //////////////////////
	//
	else if (Message == UPDATE_SEL_TERRAIN)
	{
		// on teste les exceptions
		try
		{
			// Mise � jour des r�sultats via nouveaux terrains s�lectionn�s
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	////////////////////// CHANGEMENT SELECTION CIBLES //////////////////////
	//
	else if (Message == UPDATE_SEL_CIBLE)
	{
		// on teste les exceptions
		try
		{
			// Mise � jour des r�sultats via nouvelles cibles s�lectionn�es
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	/*
	////////////////////// CHANGEMENT SELECTION SEGMENTS //////////////////////
	//
	else if (Message == UPDATE_SEL_SEGMENT)
	{
		// on teste les exceptions
		try
		{
			// Mise � jour des r�sultats via nouvelles cibles s�lectionn�es
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	*/

	////////////////////// CHARGEMENT D'UN PLATEAU ////////////////
	//
	else if (Message == UPDATE_KEY_PLATEAU)
	{
		// on teste les exceptions
		try
		{
			
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	///////////////////////////// CHANGEMENT TARIFS ///////////////////////////
	//
	else if (Message == UPDATE_TARIFS)
	{
		// on teste les exceptions
		try
		{
			// Mise � jour des r�sultats via nouveaux tarifs
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	///////////////////////////// CHANGEMENT FORMATS ///////////////////////////
	//
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// on teste les exceptions
		try
		{
			// Mise � jour des r�sultats suite modif format actif
			OnLoadFormats();
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}

	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Repositionnment moteur via nouvelle offre
JVoid JATPDocChoixSupport::UpdateOffreMoteur()
{
	// R�init moteur avec nvlle offre
	m_MoteurPresse.SetOffre(m_Offre, GetIdFiltreAudienceSel(), GetMapIdSegmentsAudienceSel()); 	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Test si s�lection valide
JBool JATPDocChoixSupport::SelectionValid()
{
	// Il faut au moins 1 s�lection support /terrain / cible / format / grandeur et date tar valid et nb insertions <> 0
	if (m_MapIdSupportsSel.GetCount()				> 0  &&
		GetMapIdTerrainsAudienceSel()->GetCount()	> 0  &&
		GetMapIdCiblesAudienceSel()->GetCount()		> 0  &&
		m_MapIdGrandEchSel.GetCount()				> 0  &&	 
		GetMapIdFormatsSel()->GetCount()			== 1 &&
		m_NbInsertions > 0 && m_DateTarifaire.IsValid())
		return true;
	else 
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Nb supports s�lectionn�s
JUnt32 JATPDocChoixSupport::GetNbSupportsSel()
{
	// Nb Selection 
	JUnt32 NbSelect = 0;

	// Map des supports s�lectionn�es
	for (m_MapIdSupportsSel.MoveFirst(); m_MapIdSupportsSel.IsValid(); m_MapIdSupportsSel.MoveNext())
	{
		if (m_MapIdSupportsSel.GetItem() == true) 
			NbSelect ++;
	}
	return NbSelect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Nb supports visibles
JUnt32 JATPDocChoixSupport::GetNbSupportsVisible()
{
	// Nb Selection 
	JUnt32 NbVisible = 0;

	// Map des supports s�lectionn�es
	for (m_MapIdSupportsVisible.MoveFirst(); 
		 m_MapIdSupportsVisible.IsValid(); 
		 m_MapIdSupportsVisible.MoveNext())
	{
		// Id support
		JUnt32 IdSupport = m_MapIdSupportsVisible.GetKey();

		// test si support visible
		JBool EtatVisible = m_MapIdSupportsVisible.GetItem();
		// if (m_MapIdSupportsVisible.GetItem() == true) 
		if (m_MapIdSupportsVisible.GetItem()) 
			NbVisible ++;
	}
	return NbVisible;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Nb supports 
JUnt32 JATPDocChoixSupport::GetNbSupports()
{
	return (m_MapIdSupportsSel.GetCount());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des �lmts terrains 
JBool JATPDocChoixSupport::OnLoadTerrains()
{
	//on verifie la validit� des elements dans l'offre

	// Flag modification de l'audience source
	bool bSource = false;

	// A REVOIR mais pour le moment on prend l'ident source d'audience du 1er module (Echelle)
	m_LstIdSrcAudienceSel.MoveTo(m_InxModuleCur);
	if (!m_LstIdSrcAudienceSel.IsValid()) 
		return false;

	JUnt32 &IdSrcAudienceSel = m_LstIdSrcAudienceSel.GetItem(); 
	bSource = (m_Offre.m_IdSourceAudience == IdSrcAudienceSel); 

	JSRCTables * m_srcTables	= (JSRCTables*)& JSRCTables::GetInstance();

	//Si la source n'est pas disponible
	if( m_srcTables->m_TBLSources.GetItem(m_Offre.m_IdSourceAudience) == NULL && IsValid())
	{
		m_Offre.m_IdSourceAudience = 0;
		m_Offre.m_TBLIdTerrainsAudience.Reset();

		// Init source d'audience
		IdSrcAudienceSel = 0;

		// Init des maps terrains pour tous les modules
		for (m_LstMapIdTerrainsAudienceSel.MoveFirst();
			m_LstMapIdTerrainsAudienceSel.IsValid();
			m_LstMapIdTerrainsAudienceSel.MoveNext())
		{
			JMap <JUnt32,JUnt32> &MapIdTerrains = m_LstMapIdTerrainsAudienceSel.GetItem(); 
			MapIdTerrains.Reset();
		}

		this->SetModifiedFlag();

		// ::AfxMessageBox(IDS_CR_ERRSRC, MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	// on teste maintenant la pr�sence des terrains
	bool bRem = false;
	m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
	while(m_Offre.m_TBLIdTerrainsAudience.IsValid())
	{
		// R�cup�re le terrain de l'offre
		JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();

		// La source d'audience en cours
		JInt32 SrcID				= m_Offre.m_IdSourceAudience;  
		// R�cup le nouveau terrain
		JInt32 TerID				= IdTerrain;

		const JTerrain * pt = m_srcTables->m_TBLTerrains.GetItem(SrcID,TerID);

		if (!pt)
		{
			//le terrain n'existe pas on le retire
			JMap <JUnt32,JUnt32> *pMapIdTerrainsAudienceSel = GetMapIdTerrainsAudienceSel();
			pMapIdTerrainsAudienceSel->MoveTo(TerID);
			if (pMapIdTerrainsAudienceSel->IsValid())
				pMapIdTerrainsAudienceSel->Remove();
				
			m_Offre.m_TBLIdTerrainsAudience.Remove(+1);
			this->SetModifiedFlag();
			bRem = true;
		}
		else
			m_Offre.m_TBLIdTerrainsAudience.MoveNext();
	}

	// No terrains loaded so return false
//	if(!TmpTerrains.GetCount())
//		return false;


	// Set the current selected Source Audience ID
	// Pour tous les modules (pour le moment �a sera le m�me)
	for (JUnt32 InxModule = ModuleEch; InxModule <= ModuleDup; InxModule++)
	{
		m_LstIdSrcAudienceSel.MoveTo(InxModule);
		if (m_LstIdSrcAudienceSel.IsValid())
		{
			JUnt32 &IdSrcAudience = m_LstIdSrcAudienceSel.GetItem();
			IdSrcAudience = m_Offre.m_IdSourceAudience;
		}
	}
	
	// Balayage de tous les maps terrain de chacun des modules
	for (JUnt32 InxModule = ModuleEch; InxModule <= ModuleDup; InxModule++)
	{
		JMap <JUnt32,JUnt32> MapTmp;
		
		m_LstMapIdTerrainsAudienceSel.MoveTo(InxModule);
		if (m_LstMapIdTerrainsAudienceSel.IsValid())
		{
			JMap <JUnt32,JUnt32> &MapIdTerrainsAudienceSel = m_LstMapIdTerrainsAudienceSel.GetItem();

			if (MapIdTerrainsAudienceSel.GetCount() > 0)
			{
				for (MapIdTerrainsAudienceSel.MoveFirst();
					 MapIdTerrainsAudienceSel.IsValid();
					 MapIdTerrainsAudienceSel.MoveNext())
				{
					// Ident terrain d�j� s�lectionn�
					JUnt32 IdTerrainSel = MapIdTerrainsAudienceSel.GetKey();
					if(IdTerrainSel)
					{
						// Check if we still have the terrain
						for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
							m_Offre.m_TBLIdTerrainsAudience.IsValid();
							m_Offre.m_TBLIdTerrainsAudience.MoveNext())
						{
							if(m_Offre.m_TBLIdTerrainsAudience.GetItem() == IdTerrainSel)
							{	
								JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
								MapTmp.Add(IdTerrain); 
							}	
						}
					}
				}

				if (MapTmp.GetCount() == 0)
				{
					// si aucun terrain s�lectionn� on prend le 1er terrain de la liste des terrains offre
					m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

					// Ajoute ce terrain au map terrains s�lectionn�s
					if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
					{
						JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
						MapTmp.Add(IdTerrain);  
					}
				}

				// Puis on swap avec nouveau map id terrains s�lectionn�s
				MapIdTerrainsAudienceSel.Swap(MapTmp);
			}

			else
			{
				// Select the First terrain in the Map
				m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

				// Ajoute ce terrain au map terrains s�lectionn�s
				if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
				{
					JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
					MapIdTerrainsAudienceSel.Add(IdTerrain);  
				}
				else
					// probl�me s�lection terrain
					return false;
			}
		}
	}


	return true;
}

////////////////////////////////////////////////////////////////////
// Changement des terrains
JBool JATPDocChoixSupport::OnChangeTerrains()
{
	// Set the current selected Source Audience ID
	// pour tous les modules pour le moment
	for (m_LstIdSrcAudienceSel.MoveFirst(); m_LstIdSrcAudienceSel.IsValid(); m_LstIdSrcAudienceSel.MoveNext())  
	{
		m_LstIdSrcAudienceSel.GetItem() = m_Offre.m_IdSourceAudience;
	}

	
	// A faire pour tous les modules
	for (m_LstMapIdTerrainsAudienceSel.MoveFirst();
		 m_LstMapIdTerrainsAudienceSel.IsValid();
		 m_LstMapIdTerrainsAudienceSel.MoveNext())
	{
		// Map temporaire des terrains
		JMap <JUnt32,JUnt32> MapTmp;

		// 
		JMap <JUnt32,JUnt32> &MapIdTerrainsAudienceSel = m_LstMapIdTerrainsAudienceSel.GetItem();

		if (MapIdTerrainsAudienceSel.GetCount() > 0)
		{
			for (MapIdTerrainsAudienceSel.MoveFirst();
				 MapIdTerrainsAudienceSel.IsValid();
				 MapIdTerrainsAudienceSel.MoveNext())
			{
				JUnt32 IdTerrainSel = MapIdTerrainsAudienceSel.GetKey();

				if(IdTerrainSel)
				{
					// Check if we still have the terrain
					for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
						m_Offre.m_TBLIdTerrainsAudience.IsValid();
						m_Offre.m_TBLIdTerrainsAudience.MoveNext())
					{
						if(m_Offre.m_TBLIdTerrainsAudience.GetItem() == IdTerrainSel)
						{	
							JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
							MapTmp.Add(IdTerrain); 
						}	
					}	
				}
			}

			if (MapTmp.GetCount() == 0)
			{
				// dans ce cas on prend le 1er terrain
				m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

				// Ajoute ce terrain au map terrains s�lectionn�s
				if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
				{
					JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
					MapTmp.Add(IdTerrain);  
				}
			}

			// Puis on swap avec nouveau map id terrains s�lectionn�s
			MapIdTerrainsAudienceSel.Swap(MapTmp);

		}
		else
		{
			// Select the First terrain in the Map
			m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

			// Ajoute ce terrain au map terrains s�lectionn�s
			if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
			{
				JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
				MapIdTerrainsAudienceSel.Add(IdTerrain);  
			}
			else
				// probl�me s�lection terrain
				return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des Supports
JBool JATPDocChoixSupport::OnLoadSupports()
{
	// Verify changes to the supports list
	CRegroupements * pReg = CRegroupements::GetInstance();

	//Chargement des titres Apparies (titres sans GRP)
	for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
		m_Offre.m_TBLIdTerrainsAudience.IsValid();
		m_Offre.m_TBLIdTerrainsAudience.MoveNext())
		pReg->GetTitresApparies(m_Offre.m_IdSourceAudience, m_Offre.m_TBLIdTerrainsAudience.GetItem());

	if(!m_Offre.m_TBLIdSupports.GetCount() && !m_Offre.m_MapSelFamilles.GetCount())
		return false;

	// Attention aux familles fabriqu�es ailleurs.....
	JBool FamilleInconnue = false;

	// Test si au moins 1 famille non reconnue
	m_Offre.m_TBLIdSupports.MoveFirst();
	while (m_Offre.m_TBLIdSupports.IsValid())
	{
		// R�cup id support
		IElem *pElem = pReg->GetIElemByID(m_Offre.m_TBLIdSupports.GetItem());
		if (!pElem)
		{
			// Au moins 1 famille non reconnue
			FamilleInconnue = true;
			break;
		}

		// Passe au support suivant
		m_Offre.m_TBLIdSupports.MoveNext(); 
	}

	// Si au moins 1 famille non reconnue, on red�compose l'offre en supports simples
	if (FamilleInconnue == true)
	{
		// Nouvelle offre d�compos�e
		JATPOffre offre;
		offre = m_Offre;

		// Balaye tous les supports
		offre.m_TBLIdSupports.Reset();
		for(m_MapIdSupportsSel.MoveFirst(); m_MapIdSupportsSel.IsValid(); m_MapIdSupportsSel.MoveNext())
		{
			// Ajoute le support
			offre.m_TBLIdSupports.AddTail() = m_MapIdSupportsSel.GetKey();
		}

		// Repositionne l'offre
		m_Offre = offre;

		// Repositionne offre moteur
		m_MoteurPresse.SetOffre(m_Offre,GetIdFiltreAudienceSel(), GetMapIdSegmentsAudienceSel()); 
		// m_MoteurPresse.SetOffre(m_Offre,GetIdFiltreAudienceSel()); 
	}


	// Map temporaire s�lection supports
	JMap<JUnt32,bool> TmpMapSel;

	// Map temporaire s�lection supports
	JMap<JUnt32,bool> TmpMapVisible;

	// Map temporaire s�lection couleur supports
	JMap<JUnt32,JUnt32> TmpColorMap;

	// Balayage de tous les supports de l'offre
	int NbSupp = m_Offre.m_TBLIdSupports.GetCount(); 
	m_Offre.m_TBLIdSupports.MoveFirst();
	while( m_Offre.m_TBLIdSupports.IsValid())
	{
		// R�cup id support
		IElem *pElem = pReg->GetIElemByID(m_Offre.m_TBLIdSupports.GetItem());
		if (!pElem)
		{
			m_Offre.m_TBLIdSupports.Remove(+1);
			continue;
		}
		if(pElem->IsLot())
		{
			// Cr�e liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// R�cup�re la liste des titres
				pLot->GetFils(ListSup);

				// Ajoute s�lection, visibilit�, et info map pour famille
				JUnt32 IdFamille = pLot->GetID(); 
				TmpMapSel.MoveTo(IdFamille);
				if(!TmpMapSel.IsValid())
				{
					// Ajoute famille non s�lectionn�e
					TmpMapSel.Add(IdFamille) = false;

					// Ajoute �tat visibilit� nouveau support (par d�faut visible)
					TmpMapVisible.Add(IdFamille) = true;

					// Pas de couleur pour cette famille
					TmpColorMap.Add(IdFamille) = 0;
				}

				// Balaye tous les titres et teste si valide
				for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				{
					// Check if the support is already in the map
					TmpMapSel.MoveTo(ListSup.GetItem());
					if(!TmpMapSel.IsValid())
					{
						// Ajoute support non s�lectionn�
						TmpMapSel.Add(ListSup.GetItem()) = false;

						// Ajoute �tat visibilit� nouveau suport (par d�faut visible)
						TmpMapVisible.Add(ListSup.GetItem()) = true;

						// Pas de couleur pour ce support
						TmpColorMap.Add(ListSup.GetItem()) = 0;
					}
				}
			}
		}
		else
		{
			// Check if the support is already in the map
			TmpMapSel.MoveTo(m_Offre.m_TBLIdSupports.GetItem());
			if(!TmpMapSel.IsValid())
			{
				// on ajout le support au map avec �tat non s�lection 
				TmpMapSel.Add(m_Offre.m_TBLIdSupports.GetItem()) = false; 

				// on ajout le support au map avec �tat visible par d�faut
				TmpMapVisible.Add(m_Offre.m_TBLIdSupports.GetItem()) = true; 

				// on ajoute info couleur pour ce support
				TmpColorMap.Add(m_Offre.m_TBLIdSupports.GetItem()) = 0; 
			}
		}
		m_Offre.m_TBLIdSupports.MoveNext();
	}
	
	// Balayage de toutes les familles de l'offre
	int NbFamille = m_Offre.m_MapSelFamilles.GetCount(); 
	m_Offre.m_MapSelFamilles.MoveFirst();
	while(m_Offre.m_MapSelFamilles.IsValid())
	{
		// R�cup id famille
		IElem *pElem = pReg->GetIElemByID(m_Offre.m_MapSelFamilles.GetKey());
		if (!pElem)
		{
			m_Offre.m_MapSelFamilles.Remove(+1);
			continue;
		}
					
		// Ajoute s�lection, visibilit�, et info map pour famille
		JUnt32 IdFamille = pElem->GetID(); 
		TmpMapSel.MoveTo(IdFamille);
		if(!TmpMapSel.IsValid())
		{
			// Ajoute famille non s�lectionn�e
			TmpMapSel.Add(IdFamille) = false;

			// Ajoute �tat visibilit� nouveau support (par d�faut visible)
			TmpMapVisible.Add(IdFamille) = true;

			// Pas de couleur pour cette famille
			TmpColorMap.Add(IdFamille) = 0;
		}
		
		// Passe � la famille suivante
		m_Offre.m_MapSelFamilles.MoveNext();
	}


	//on verifie la syncho avec le choix s�lection des supports 
	for (m_MapIdSupportsSel.MoveFirst();m_MapIdSupportsSel.IsValid();m_MapIdSupportsSel.MoveNext())
	{
		// R�cup ident support
		JUnt32 id = m_MapIdSupportsSel.GetKey();
		IElem *pElem = pReg->GetIElemByID(id);
		TmpMapSel.MoveTo(id);

		//si on a un support dans choix support mais il n'est pas dans la map �a veut dire qu'il vient d'une famille supprim�e
		if(pElem)
		{
			if (TmpMapSel.IsValid())
			{
				// Il est d�j� dans le map, on indique seulement son �tat
				JBool &EtatSupport = TmpMapSel.GetItem();
				EtatSupport = m_MapIdSupportsSel.GetItem();
			}
		}
	}

	//on verifie la syncho avec l'�tat visibilit� des supports 
	for (m_MapIdSupportsVisible.MoveFirst();
		 m_MapIdSupportsVisible.IsValid();
		 m_MapIdSupportsVisible.MoveNext())
	{
		// R�cup ident support
		JUnt32 id = m_MapIdSupportsVisible.GetKey();
		IElem *pElem = pReg->GetIElemByID(id);
		TmpMapVisible.MoveTo(id);

		//si on a un support dans choix support mais il n'est pas dans la map �a veut dire qu'il vient d'une famille supprim�e
		if(pElem)
		{
			if (TmpMapVisible.IsValid())
			{
				// Il est d�j� dans le map, on indique seulement son �tat
				JBool &EtatSupport = TmpMapVisible.GetItem();
				EtatSupport = m_MapIdSupportsVisible.GetItem();
			}
		}
	}

	//on verifie la syncho avec le choix des couleurs des supports 
	for (m_MapIdColorSupports.MoveFirst(); m_MapIdColorSupports.IsValid(); m_MapIdColorSupports.MoveNext())
	{
		// R�cup ident support
		JUnt32 id = m_MapIdColorSupports.GetKey();
		IElem *pElem = pReg->GetIElemByID(id);
		TmpColorMap.MoveTo(id);

		//si on a un support dans choix support mais il n'est pas dans la map �a veut dire qu'il vient d'une famille supprim�e
		if(pElem)
		{
			if (TmpColorMap.IsValid())
			{
				// On le retrouve bien dans le map, on positionne son �tat couleur
				JUnt32 &ColorSupport = TmpColorMap.GetItem();
				ColorSupport = m_MapIdColorSupports.GetItem();
			}
		}
	}

	// Mise � jour nouvelle s�lection supports
	m_MapIdSupportsSel.Swap(TmpMapSel);

	// Mise � jour �tat visibilit� des support en cours
	m_MapIdSupportsVisible.Swap(TmpMapVisible);

	// Mise � jour id couleur associ� aux supports
	m_MapIdColorSupports.Swap(TmpColorMap);

	return true;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des items cibles
JBool JATPDocChoixSupport::OnLoadItems()
{
	// Flag modification de l'audience source
	JMap<JUnt32 , IItem * >	TmpItems;
	m_MAPpItems.Reset();

	// Mise � jour du Map Items courant 
	bool bRem = false;
	CString msg;
	m_Offre.m_TBLIdItems.MoveFirst();
	while (m_Offre.m_TBLIdItems.IsValid())
	{
		// R�cup�re le Item de l'offre
		JUnt32 IdItem = m_Offre.m_TBLIdItems.GetItem();

		// R�cup Table des items
		// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

		// R�cup item correspondant � l'indice
		// IItem * pItem = pItemConst->GetIItemByID(IdItem);
		IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdItem);

		if(pItem)
		{
			if(pItem && pItem->IsItemConstruit())
			{
				// Add the Item contruit
				TmpItems.Add(IdItem) = pItem;
			}
			if(pItem && pItem->IsPalette())
			{
				CPalettes *pPal = (CPalettes *)pItem->AsPalette();
				JList<JInt32> idList;
				pPal->GetItems(idList);
				for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
				{
					// IItem * pItemfromPal = pItemConst->GetIItemByID(idList.GetItem());
					IItem * pItemfromPal = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(idList.GetItem());
					TmpItems.Add(pItemfromPal->GetID()) = pItemfromPal;
				}
			}
			m_Offre.m_TBLIdItems.MoveNext();
		}
		else
		{
			m_Offre.m_mapLblItem.MoveTo(m_Offre.m_TBLIdItems.GetItem());
			if (m_Offre.m_mapLblItem.IsValid())
			{
				// Item non trouv� dans la liste des items courants
				msg.Append(m_Offre.m_mapLblItem.GetItem().AsJCharPtr());
				msg += "\r\n";
				m_Offre.m_mapLblItem.Remove();
			}

			m_Offre.m_TBLIdItems.Remove(+1);
			this->SetModifiedFlag();
			bRem = true;
		}
	}
	if (bRem)
	{
		CString str;
		str.FormatMessage(IDS_FD_ERRITEM, msg);
		::AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
	}

	// No Items loaded
	if(!TmpItems.GetCount())
	{
		if (IsValid())
		{
			//on reselectionne l'item ensemble
			// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();
			// IItem * pItem = pItemConst->GetIItemByID(pItemConst->GetDefaultItemId());
			CItemsConstruitsManager * pItemsConstruitsManager = CItemsConstruitsManager::GetInstance();
			IItem * pItem = pItemsConstruitsManager->GetItemsInstance()->GetIItemByID(pItemsConstruitsManager->GetItemsInstance()->GetDefaultItemId());

			if(pItem)
				TmpItems.Add(pItemsConstruitsManager->GetItemsInstance()->GetDefaultItemId()) = pItem;
		}
		else
			return false;
	}

	// Swap the Item maps
	m_MAPpItems.Swap(TmpItems);

	// Reconstitution s�lection items pour tous les modules
	for (m_LstMapIdCiblesAudienceSel.MoveFirst();
		 m_LstMapIdCiblesAudienceSel.IsValid();
		 m_LstMapIdCiblesAudienceSel.MoveNext())
	{
		JMap <JUnt32,JUnt32> MapTmp;

		// Va sur le map du module
		JMap <JUnt32,JUnt32> &MapIdCiblesAudienceSel = m_LstMapIdCiblesAudienceSel.GetItem();
		if (MapIdCiblesAudienceSel.GetCount() > 0)
		{
			for (MapIdCiblesAudienceSel.MoveFirst();
				 MapIdCiblesAudienceSel.IsValid();
				 MapIdCiblesAudienceSel.MoveNext())
			{
				JUnt32 IdCibleSel = MapIdCiblesAudienceSel.GetKey();

				if(IdCibleSel)
				{
					// V�rifie si cible pr�sente dans nouveau map
					m_MAPpItems.MoveTo(IdCibleSel);
					if(m_MAPpItems.IsValid())
					{	
						JUnt32 IdCible = m_MAPpItems.GetItem()->GetID();
						MapTmp.Add(IdCible); 
					}	
				}
			}

			if (MapTmp.GetCount() == 0)
			{
				// si aucune cible s�lectionn�e on prend la 1ere cible de la liste des cibles offre
				m_MAPpItems.MoveFirst();

				// Ajoute cette cible au map cibles s�lectionn�es
				if (m_MAPpItems.IsValid())
				{
					JUnt32 IdCible = m_MAPpItems.GetItem()->GetID();
					MapTmp.Add(IdCible);  
				}
			}

			// Puis on swap avec nouveau map id cibles s�lectionn�es
			MapIdCiblesAudienceSel.Swap(MapTmp); 
		}
		else
		{
			// Selectionne 1er cible du map
			m_MAPpItems.MoveFirst();

			// Ajoute cette cible au map cibles s�lectionn�es
			if (m_MAPpItems.IsValid())
			{
				JUnt32 IdCible				= m_MAPpItems.GetItem()->GetID();
				MapIdCiblesAudienceSel.Add(IdCible);
			}
			else
				// probl�me s�lection cible
				return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des segments
JVoid JATPDocChoixSupport::OnLoadSegPop()
{
	// Pour tous les modules mise � jour des maps formats s�lectionn�s
	for (JUnt32 InxModule = ModuleEch; InxModule <= ModuleDup; InxModule++)
	{
		// Mise � jour des formats dispo suite � nvlle offre
		JMap <JUnt32,JUnt32> MapTmp;

		// On se positionne sur le map du module
		m_LstMapIdSegmentsAudienceSel.MoveTo(InxModule);

		// V�rifie s'il est valide
		if (m_LstMapIdSegmentsAudienceSel.IsValid())
		{
			// R�cup�re le map du module
			JMap <JUnt32,JUnt32> &MapIdSegPopSel = m_LstMapIdSegmentsAudienceSel.GetItem();		

			// Si au moins 1 �lmt
			if (MapIdSegPopSel.GetCount() > 0)
			{
				// Boucle sur les segments de l'offre
				for (m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
					 m_Offre.m_TBLIdSegmentsAudience.IsValid();
					 m_Offre.m_TBLIdSegmentsAudience.MoveNext())
				{
					JUnt32 IdSegPop = m_Offre.m_TBLIdSegmentsAudience.GetItem();
					MapIdSegPopSel.MoveTo(IdSegPop);

					// Voit s'il �tait d�j� s�lectionn�
					if (MapIdSegPopSel.IsValid())
						// Reste comme format s�lectionn�
						MapTmp.Add(IdSegPop);
				}
			}				
		
			// Swap les 2 maps
			MapIdSegPopSel.Swap(MapTmp);

			// Si aucun segment s�lectionn�, on les s�lectionne tous
			if (MapIdSegPopSel.GetCount() == 0 && m_Offre.m_TBLIdSegmentsAudience.GetCount() > 0)
			{
				// Boucle sur tous les segments s�lectionn�s dans l'offre
				for (m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
					 m_Offre.m_TBLIdSegmentsAudience.IsValid();
					 m_Offre.m_TBLIdSegmentsAudience.MoveNext())
				{
					// R�cup�re l'ident segment
					JUnt32 IdSegPop = m_Offre.m_TBLIdSegmentsAudience.GetItem();

					// Ajoute au map s�lection par moduke
					MapIdSegPopSel.Add(IdSegPop);
				}
			}
		}
		else
			// Big problem
			throw JInternalError::GetInstance();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Chargement des formats
JVoid JATPDocChoixSupport::OnLoadFormats()
{
	// Pour tous les modules mise � jour des maps formats s�lectionn�s
	for (JUnt32 InxModule = ModuleEch; InxModule <= ModuleDup; InxModule++)
	{
		// Mise � jour des formats dispo suite � nvlle offre
		JMap <JUnt32,JUnt32> MapTmp;

		// On se positionne sur le map du module
		m_LstMapIdFormatsSel.MoveTo(InxModule);

		// V�rifie s'il est valide
		if (m_LstMapIdFormatsSel.IsValid())
		{
			// R�cup�re le map du module
			JMap <JUnt32,JUnt32> &MapIdFormatsSel = m_LstMapIdFormatsSel.GetItem();		

			// Si au moins 1 �lmt
			if (MapIdFormatsSel.GetCount() > 0)
			{
				// Boucle sur les formats de l'offre
				for (m_Offre.m_TBLIdFormats.MoveFirst();m_Offre.m_TBLIdFormats.IsValid();m_Offre.m_TBLIdFormats.MoveNext())
				{
					JUnt32 IdFormat = m_Offre.m_TBLIdFormats.GetItem();
					MapIdFormatsSel.MoveTo(IdFormat);

					// Voit s'il �tait d�j� s�lectionn�
					if (MapIdFormatsSel.IsValid())
						// Reste comme format s�lectionn�
						MapTmp.Add(IdFormat);
				}
			}				
		
			// Swap les 2 maps
			MapIdFormatsSel.Swap(MapTmp);

			// Si aucun format s�lectionn�, on en s�lectionne 1
			if (MapIdFormatsSel.GetCount() == 0 && m_Offre.m_TBLIdFormats.GetCount() > 0)
			{
				m_Offre.m_TBLIdFormats.MoveFirst();
				JUnt32 IdFormat = m_Offre.m_TBLIdFormats.GetItem();
				MapIdFormatsSel.Add(IdFormat);
			}
		}
		else
			// Big problem
			throw JInternalError::GetInstance();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration des idents libell�s format possible pour un support
void JATPDocChoixSupport::GetListIdFormats(JUnt32 IdSupport, JMap <JUnt32,JUnt32> *pMapIdFormats)
{
	// D�claration et init liste provisoire ident support
	pMapIdFormats->Reset(); 

	// Boucle sur les formats de l'offre
	for (m_Offre.m_TBLIdFormats.MoveFirst();m_Offre.m_TBLIdFormats.IsValid();m_Offre.m_TBLIdFormats.MoveNext())
	{
		JUnt32 IdFormat = m_Offre.m_TBLIdFormats.GetItem();
	
		// R�cup�re Tarif pour ce format
		CTarifs * pTarifs	= CTarifs::GetInstance();
		if(pTarifs)
		{
			JUnt32 Tarif;
			JUnt32 Duree;
			JDate DateExact;
			/*
			if(pTarifs->GetTarifByVersion(IdSupport, IdFormat, m_DateTarifaire,Tarif))
				// On inclut ce format si tarif valide
				pMapIdFormats->Add(IdFormat); 
			*/
			if(pTarifs->GetTarifByDate(IdSupport, IdFormat, m_DateTarifaire,m_DateTarifaire,DateExact,Duree,Tarif))
				// On inclut ce format si tarif valide
				pMapIdFormats->Add(IdFormat); 
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise � jour du plateau s�lection supports venant de l'offre
JVoid JATPDocChoixSupport::UpdatePlateau()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration p�riodicit� support
void JATPDocChoixSupport::GetPeriodicite(JUnt32 IdSupport, JString &TxtPeriod, JString &TxtAbrPeriod)
{
	// Init chaines p�riodicit�
	TxtPeriod    = "non d�finie";
	TxtAbrPeriod = "n.d";

	// R�cup info �lmt
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem = pReg->GetIElemByID(IdSupport);
	if(pElem)
	{
		// Test si c'est un couplage d'abord (car couplage fixe � la fois titre et couplage)
		if(pElem->IsCouplage())
		{
			TxtPeriod    = "couplage";
			TxtAbrPeriod = "CPL";
		}

		// on recupere la periodicit�
		else if (pElem->IsTitre())
		{
			JUnt32 Periodicite = pElem->AsITitre()->GetPeriodicite();
			if(Periodicite)
			{
				// R�cup�re libell� et libell� abr�g� de la p�riodicit�
				const JSRCPresse & SrcPresse		= JSRCPresse::GetInstance();
				TxtPeriod		= SrcPresse.m_TBLPeriodicites.GetLabelByID(Periodicite);
				TxtAbrPeriod	= SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(Periodicite); 
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration p�riodicit� support
JUnt32 JATPDocChoixSupport::GetMaxInsertions(JUnt32 IdSupport)
{
	// Texte p�riodicit�
	JString TxtPeriod;

	// R�cup info �lmt
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem = pReg->GetIElemByID(IdSupport);
	if(pElem)
	{
		// Test si c'est un couplage (A FAIRE)
		JUnt32 Periodicite = 1000;
		
		// on recupere la periodicit�
		if (pElem->IsTitre())
			Periodicite = pElem->AsITitre()->GetPeriodicite();

		if(pElem->IsCouplage())
		{
			JList<IElem*> lstlElem;
			pElem->AsCouplage()->GetFils(lstlElem);
			for(lstlElem.MoveFirst(); lstlElem.IsValid(); lstlElem.MoveNext())
			{
				JUnt32 Period = lstlElem.GetItem()->AsITitre()->GetPeriodicite();
				if (Period < Periodicite)
					Periodicite = Period;
			}
		}


		switch (Periodicite)
		{
		case 1:
			// Les supports quotidiens
			return 366;
		case 2:
			// Les supports bi-hebdomadaires (2 fois par semaine)
			return 52 + 53;
		case 3:
			// Les supports hebdomadaires
			return 53;
		case 4:
			// Les supports bimensuels (2 fois par mois)
			return 24;
		case 5:
			// Les supports mensuels
			return 12;
		case 6:
			// Les supports bimestriels (tous les 2 mois)
			return 6;
		case 7:
			// Les supports trimestriels 
			return 4;
		case 8:
			// Les supports hors-s�ries
			return 1;
		default:
			// A VOIR pour les cas non d�finis
			return 1;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// D�coupage chaine par milliers
void JATPDocChoixSupport::FormatString(CString &Txt, JUnt32 PasDecoup)
{
	// Recupere les decimales
	CString TxtDec = " ";
	CString Dec;
	Dec.AppendChar((TCHAR)CFormatStr::GetDecimalPoint());
	int PosDec = 0;
	PosDec = Txt.Find(Dec);
	if (PosDec > 0)
	{
		TxtDec = Txt.Mid(PosDec) + " ";
		Txt = Txt.Left(PosDec); 
	}

	// Nouvelle chaine
	CString NewTxt = "";

	// Formatage new chaine
	while (Txt.GetLength() > PasDecoup)
	{
		// Ajoute bloc new chaine
		NewTxt = Txt.Mid(Txt.GetLength() - PasDecoup, PasDecoup) + " " + NewTxt;

		// Enleve bloc old chaine
		Txt.Delete(Txt.GetLength() - PasDecoup, PasDecoup); 
	}

	// S'il reste des caract�re, on les ajoute
	if (Txt.GetLength() > 0)
		NewTxt = Txt + " " + NewTxt;

	// Puis on r�cup�re la chaine finale
	Txt = NewTxt;

	// Vire espace si � la fin
	Txt = Txt.TrimRight(); 

	// Ajoute les d�cimales s'il y a lieu
	Txt += TxtDec;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�initialise tous les r�sultats
void JATPDocChoixSupport::ResetAllResultat()
{
	// Reset r�sultats �chelle
	m_MapResultatEch.Reset(); 

	// Reset r�sultats mapping
	m_MapResultatMap.Reset(); 

	// Reset r�sultats cumulativit�
	m_MapResultatCum.Reset(); 

	// Reset r�sultats duplication
	m_MapResultatDup.Reset(); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�initialise r�sultats d'un module
void JATPDocChoixSupport::ResetResultat(JUnt32 InxModule)
{
	switch (InxModule)
	{
		case ModuleEch :
		{
			// Reset r�sultats �chelle
			m_MapResultatEch.Reset(); 
			break;
		}

		case ModuleMap :
		{
			// Reset r�sultats mapping
			m_MapResultatMap.Reset(); 
			break;
		}
	
		case ModuleCum :
		{
			// Reset r�sultats cumulativit�
			m_MapResultatCum.Reset(); 
			break;
		}

		case ModuleDup :
		{
			// Reset r�sultats duplication
			m_MapResultatDup.Reset(); 
			break;
		}

	}		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pr�calcul selon le mode de tra�age (0/ Tra�age s�par� 1/ Tra�age regroup� par couleur)
void JATPDocChoixSupport::PrepareMatriceTraceCum()
{
	// Init Map r�sultats courbe et map p�riodicit� des points
	m_MapResultCrbCum.Reset(); 
	m_MapResultEtalCrbCum.Reset(); 
		
	// Recup le terrain en cours
	JUnt32 IdTerrain;
	GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain s�lectionn�
		IdTerrain = GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return;

	// Recup la cible en cours
	JUnt32 IdCible;
	GetMapIdCiblesAudienceSel()->MoveFirst();
	if (GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible s�lectionn�e
		IdCible = GetMapIdCiblesAudienceSel()->GetKey();  
	}
	else
		return;

	// Calcul MaxInsertions pour tous les supports s�lectionn�es
	JUnt32 MaxInsertions = 0;
	for (m_MapIdColorSupports.MoveFirst();
		 m_MapIdColorSupports.IsValid();
		 m_MapIdColorSupports.MoveNext())
	{
		// R�cup�re Id Support
		JUnt32 IdSupport = m_MapIdColorSupports.GetKey(); 

		// R�cup�re Id Couleur Associ�e
		JUnt32 IdColor = m_MapIdColorSupports.GetItem();

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = GetSupportVisible(IdSupport); 

		// Voir si support s�lectionn�
		if (IdColor > 0 && EtatVisible)
		{
			// Max insertions pour ce support
			JUnt32 MaxInsertSupp = GetMaxInsertions(IdSupport);
			if (MaxInsertSupp > MaxInsertions)
				MaxInsertions = MaxInsertSupp;
		}
	}

	// Calcul du nombre de jours maxi � prendre en compte selon �talonnage de chacun des supports s�lectionn�s
	JUnt32 MaxJours = MaxJourCourbeCum(IdTerrain,IdCible);
	
	// Boucle sur les supports s�lectionn�s
	for (m_MapIdColorSupports.MoveFirst();
		 m_MapIdColorSupports.IsValid();
		 m_MapIdColorSupports.MoveNext())
	{
		
		// R�cup�re Id Support
		JUnt32 IdSupport = m_MapIdColorSupports.GetKey(); 

		// R�cup�re Id Couleur Associ�e
		JUnt32 IdColor = m_MapIdColorSupports.GetItem();

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = GetSupportVisible(IdSupport); 

		// Voir si support s�lectionn�
		if (IdColor > 0 && EtatVisible)
		{
			// Cr�ation du tableau r�sultats / support
			JArray <JFlt64> TTmpResult;
			TTmpResult.SetCount(MaxJours); 
			for (int k = 0; k<TTmpResult.GetCount();k++)
				TTmpResult.Item(k) = 0.0;

			// Cr�ation du tableau p�riodicit� r�sultats / support
			JArray <bool> TTmpEtalResult;
			TTmpEtalResult.SetCount(MaxJours); 
			for (int k = 0; k<TTmpEtalResult.GetCount();k++)
				TTmpEtalResult.Item(k) = false;

			// Boucle sur toutes les insertions
			JFlt64 LastResult = 0;
			JUnt32 LastPtEtal = 0;
			JUnt32 LastIndex  = 0;
			for (JUnt32 IdxInsert = 0; IdxInsert < MaxInsertions; IdxInsert++)
			{
				// Max insertions pour ce support
				JUnt32 MaxInsertSupp = GetMaxInsertions(IdSupport);

				// R�cup�re le nombre d'insertions correspondant au pas i
				JUnt32 NbInsert;
				NbInsert = IdxInsert + 1;
				
				// Recup r�sultat
				JFlt64 Result;
				JUnt32 PtEtal		= 0;
				JUnt32 IndexJour	= 0;
				
				if (NbInsert <= MaxInsertSupp)
				{	
					// Calcul resultat pour nouvelle insertion
					Result = RecupResultatCum(IdSupport,IdTerrain,IdCible,NbInsert,PtEtal);

					// Index Jour pour stockage r�sultat
					if (PtEtal > 0)
					{
						IndexJour = NbInsert * PtEtal; 
						TTmpResult.Item(IndexJour) = Result;
						LastPtEtal = PtEtal;
					}
				}

				// Point d'�talonnage (correspondant � la p�riodicit� du support)
				if (LastPtEtal > 0 && IndexJour >= LastPtEtal)
					TTmpEtalResult.Item(IndexJour) = (IndexJour % LastPtEtal == 0);
					
			}

			// Lissage r�sultats (pour les trous �ventuels)- �vite de calculer par eemple tous les jours jusqu'� 365 jours pours les quotidiens
			JUnt32 IdxDeb = 0;
			JUnt32 IdxFin = 0;

			for (JInt32 i=0; i< TTmpResult.GetCount(); i++)
			{
				// Test si point non calcul�
				if (TTmpResult.Item(i) == 0)
				{
					// Sauve indice valeur � 0
					if (i > 0)
						IdxDeb = i-1;
					else
						IdxDeb = 0;

					// Va jusqu'au prochaine valeur != 0
					IdxFin = 0;
					for (JInt32 j=i+1; j<TTmpResult.GetCount(); j++)
					{
						if (TTmpResult.Item(j) != 0)
						{
							IdxFin = j-1;
							break;
						}
					}

					if (IdxFin != 0)
					{
						// Redistribue les valeurs interm�diaires
						JFlt64 ValDep = 0;
						if (IdxDeb > 0)
							ValDep   = TTmpResult.Item(IdxDeb);

						JFlt64 ValFin   = TTmpResult.Item(IdxFin+1);
						JFlt64 Ecart    = (ValFin - ValDep);
						JUnt32 NbRepart = IdxFin - IdxDeb + 1;
						JFlt64 Repart   = Ecart / NbRepart;
						for (JUnt32 k = IdxDeb; k <= IdxFin; k++)
						{
							// Calcul valeur interm�diaire
							JFlt64 ValLissage = TTmpResult.Item(IdxDeb) + (k-IdxDeb) * Repart;
							TTmpResult.Item(k) = TTmpResult.Item(IdxDeb) + (k-IdxDeb) * Repart;	

							// Point d'�talonnage
							if (LastPtEtal > 0 && k >= LastPtEtal)
								TTmpEtalResult.Item(k) = (k % LastPtEtal == 0);
						}

						// repositionne dernier indice 
						i = IdxFin;
					}
					else if (IdxDeb != IdxFin)
					{
						// Recopie des derni�res valeurs non d�finies (derni�re valeur)
						for (JInt32 j=IdxDeb; j<TTmpResult.GetCount(); j++)
						{
							// R�cup�re derni�re valeur
							TTmpResult.Item(j) = TTmpResult.Item(IdxDeb);

							// Point d'�talonnage
							if (LastPtEtal > 0)
								TTmpEtalResult.Item(j) = (j % LastPtEtal == 0);
						}
					}
				}
			}

			// Ajoute au map global r�sultats supports
			m_MapResultCrbCum.Add(IdSupport).Swap(TTmpResult);

			// Ajoute map des p�riodicit�s pour la courbe
			m_MapResultEtalCrbCum.Add(IdSupport).Swap(TTmpEtalResult);
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////
// Calcul maximum de jours � prendre en compte pour la courbe cumulativit�
JUnt32 JATPDocChoixSupport::MaxJourCourbeCum(JUnt32 IdTerrain, JUnt32 IdCible)
{
	// Init MaxJours
	JUnt32 MaxJours = 0;

	// Boucle sur les supports s�lectionn�s
	for (m_MapIdColorSupports.MoveFirst();
		 m_MapIdColorSupports.IsValid();
		 m_MapIdColorSupports.MoveNext())
	{
		
		// R�cup�re Id Support
		JUnt32 IdSupport = m_MapIdColorSupports.GetKey(); 

		// R�cup�re Id Couleur Associ�e
		JUnt32 IdColor = m_MapIdColorSupports.GetItem();

		// Voir si support s�lectionn�
		if (IdColor > 0)
		{
			// Max insertions pour ce support
			JUnt32 MaxInsertSupp = GetMaxInsertions(IdSupport);

			// Calcul �talonnage
			JUnt32 PtEtal = 0;
			JFlt64 Result = RecupResultatCum(IdSupport,IdTerrain,IdCible,MaxInsertSupp,PtEtal);

			// Mise � jour maximum jours � prendre en compte
			if (PtEtal * MaxInsertSupp > MaxJours)
				MaxJours = PtEtal * MaxInsertSupp;
		}
	}

	// Si au moins 1 jour, on prend 1 jour de +
	if (MaxJours)
		MaxJours ++;

	return MaxJours;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Positionnement Id grandeur destin�e � la moyenne
JVoid JATPDocChoixSupport::SetIdGrandeurMoyenne(JUnt32 TypeModule,JUnt32 IdGrandeur)
{
	if (TypeModule >= ModuleEch && TypeModule <= ModuleDup)
		// Nouvel ident grandeur pour la moyenne
		m_TblIdGrandeurMoyenne.Item(TypeModule) = (JInt32)IdGrandeur;
	else
		// Big problem, type module incompatible
		throw JInternalError::GetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re Id grandeur destin�e � la moyenne
JInt32 JATPDocChoixSupport::GetIdGrandeurMoyenne(JUnt32 TypeModule)
{
	if (TypeModule >= ModuleEch && TypeModule <= ModuleDup)
		// R�cup ident grandeur pour la moyenne
		return m_TblIdGrandeurMoyenne.Item(TypeModule);
	else
		// Big problem, type module incompatible
		throw JInternalError::GetInstance();
}

// Validit� grandeur destin�e � la moyenne
JBool JATPDocChoixSupport::IdGrandeurMoyenneValid(JUnt32 TypeModule, JUnt32 IdGrandeur)
{
	// On se place sur le map s�lection grandeur
	m_MapIdGrandEchSel.MoveTo(IdGrandeur);

	// Test si grandeur s�lectionn�e
	for (m_MapIdGrandEchSel.MoveFirst(); m_MapIdGrandEchSel.IsValid(); m_MapIdGrandEchSel.MoveNext())
	{
		JUnt32 IdG = m_MapIdGrandEchSel.GetItem(); 
		if (IdG == IdGrandeur)
			return true;
	}

	// Test si pr�sent
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration r�sultat cumulativit�
JFlt64 JATPDocChoixSupport::RecupResultatCum(JUnt32 IdSupport, JUnt32 IdTerrain, JUnt32 IdCible, JUnt32 NbInsertions, JUnt32 &NbJourPtEtal)
{
	// Valeur r�sultat de retour
	JFlt64 Result = 0.0;

	// Cl� de recherche r�sultat
	CKeyMoteur KeyMoteur;

	// Cl� archivage r�sultat
	CKeyResultCum KeyResultCum;

	// Information r�sultat archiv�
	CResultCum	ResultCum;

	// Gestion tarif
	CTarifs *pTarifs	= CTarifs::GetInstance();

	// R�cup�re le format du support s�lectionn� pour la cumulativit�
	JUnt32 IdFormatSupport = 0;
	m_MapIdFormatSpecifCum.MoveTo(IdSupport);
	if (m_MapIdFormatSpecifCum.IsValid())
		IdFormatSupport = m_MapIdFormatSpecifCum.GetItem();

	// R�cup�re Tarif item selon format (via date d�but tarifaire)
	JUnt32 Tarif;
	JBool TarifManuel = false;
	if(pTarifs)
	{
		Tarif = GetTarifSupport(IdSupport, IdFormatSupport, TarifManuel);
	}

	// Positionne sur clef r�sultats
	KeyResultCum.m_IdSupport = IdSupport;				// Cl� support
	KeyResultCum.m_IdTerrain = IdTerrain;				// Cl� terrain
	KeyResultCum.m_IdCible   = IdCible;					// Cl� cible
	KeyResultCum.m_IdSegment = ClefSegment(ModuleCum);  // Calcul indice cl� segment
	KeyResultCum.m_NbInsert  = NbInsertions;			// Cl� nombre d'insertions
	KeyResultCum.m_IdMoteur  = m_MoteurPresse.GetType();// Type du moteur 

	CResultCum	ResultCumMaxInsert;
	m_MapResultatCum.MoveTo(KeyResultCum); 
	if (m_MapResultatCum.IsValid())
	{
		// Information r�sultat archiv�
		CResultCum	&ResultCum = m_MapResultatCum.GetItem();

		// Le r�sultat pour la cellule
		Result = ResultGrandeurCum(Tarif, ResultCum.m_VolLecture , ResultCum.m_Audience, ResultCum.m_AudProba1Ins, 
								       ResultCum.m_Effectif, ResultCum.m_NbCas, NbInsertions);

		// Le nombre de jours correspondant � 1 insertion
		NbJourPtEtal = ResultCum.m_PtEtalonnage; 

		
		// Ne faut-il pas mieux �talonn� via la p�riodicit�
		CRegroupements* regroup = CRegroupements::GetInstance();

		IElem * pElem = regroup->GetIElemByID(IdSupport);
		JUnt32 idperiod = 0;
		if(pElem && pElem->IsTitre() && !pElem->IsCouplage())
				idperiod =  pElem->AsITitre()->GetPeriodicite();

		JUnt32 period = 1;
		switch (idperiod)
		{
			case 1://quotidien
				period = 1;
				break;
			case 2://bihebdo
				period = 3;
				break;
			case 3://hebdo
				period = 7;
				break;
			case 4://bimensuel
				period = 15;
				break;
			case 5://mensuel
				period = 30;
				break;
			case 6://bimestriel
				period = 60;
				break;
			case 7://trimestriel
				period = 90;
				break;
			case 8://hs
				period = 1;
				break;
		}
		
		// Attention on �talonnera avec la p�riodicit� d�sormais
		if (NbJourPtEtal < 7)
			// 	NbJourPtEtal = 1;
			NbJourPtEtal = period;
	}

	return Result;
}

////////////////////////////////////////////////////////////////////////
// R�sultat via infos r�sultats g�n�raux
JFlt64 JATPDocChoixSupport::ResultGrandeurCum(JFlt64 Tarif, JFlt64 VolLecture, JFlt64 Audience, JFlt64 Audience1Ins,
								              JFlt64 Effectif,   JFlt64 NbCas, JInt32 NbInsertions)
{
	
	// R�sultat non existant par d�faut
	JFlt64 Result = -1.0;

	// R�cup�re index grandeur
	JUnt32 IdGrandeur = 0;
	m_MapIdGrandCumSel.MoveFirst();
	if (m_MapIdGrandCumSel.IsValid())
		IdGrandeur = m_MapIdGrandCumSel.GetItem(); 

	// Calcul selon grandeurs
	switch(IdGrandeur)
	{
		//////////////////////////////////// CALCUL AUDIENCE ////////////////////////////////////
		case GrdCum_Audience : 
		{
			Result = Audience;
		}
		break;

		///////////////////////////// CALCUL AUDIENCE (en milliers) /////////////////////////////////
		case GrdCum_Audience000 : 
		{
			Result = (Audience + 500) / 1000;
		}
		break;

		/////////////////////////////////// CALCUL NB CONTACT (000) /////////////////////////////////
		case GrdCum_NbContact000 : 
		{
			Result = (VolLecture + 500)/1000;
		}
		break;

		////////////////////////////////////// CALCUL GRP ///////////////////////////////////////////
		case GrdCum_GRP :
		{
			if (Effectif > 0)
			{
				// Calcul du GRP si au moins effectif valide
				//Result = (100.0 * Audience) / Effectif;
				// GRP calcul� avec nb insertions
				Result = (100.0 * Audience1Ins * NbInsertions) / Effectif;
			}
		}
		break;

		////////////////////////////////////// CALCUL X-GRP ///////////////////////////////////////////
		case GrdCum_XGRP :
		{
			// Test si calcul possible
			if (Effectif > 0 && VolLecture >= 0)
			{
				// Calcul du GRP[+]
				Result = (VolLecture * 100.0) / Effectif;
			}
		}
		break;

		/////////////////////////////////// CALCUL COUVERTURE //////////////////////////////
		case GrdCum_CouvCum : 
		{
			if (Effectif >= 0 && Audience >= 0)
				// Calcul couverture
				Result = (Audience / Effectif) * 100.0;
		}
		break;

		//////////////////////////////// CALCUL REPETITION ///////////////////////////////////
		case GrdCum_Repetition :
		{
			// Il faut au moins de l'audience
			if (Audience > 0)
			{
				// Calcul r�p�tition
				Result = (Audience1Ins * NbInsertions) / Audience;
			}
		}
		break;

		//////////////////////////////// CALCUL REPETITION + ///////////////////////////////////
		case GrdCum_XRepetition :
		{
			// Il faut au moins de l'audience
			if (Audience > 0)
			{
				// Calcul r�p�tition
				Result = VolLecture / Audience;
			}
		}
		break;

		/////////////////////////////////// CALCUL CUMULATIVITE ///////////////////////////////////
		case GrdCum_Cumulativite : 
		{
			if (Audience1Ins > 0)
				Result = Audience/Audience1Ins;
		}
		break;

		////////////////////////////// CALCUL COUT COUVERTURE /////////////////////////////////
		case GrdCum_CoutCouv : 
		{
			if (Effectif >= 0 && Audience >= 0)
			{
				// Calcul couverture
				JFlt64 Couv = (Audience / Effectif) * 100.0;

				// Calcul Cout Point Couverture
				if (Couv > 0)
					Result = (Tarif*NbInsertions)/Couv;
			}
		}
		break;

		default:
		break;
	}
	
	// Renvoi r�sultat brut
	return Result;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du libell� format par d�faut
CString JATPDocChoixSupport::GetFormatPrincipal(bool Abrege)
{
	// -------------------------------------------- Le format utilis� ------------------------------------------------
	JUnt32 IdFormat;
	GetMapIdFormatsSel()->MoveFirst();
	if (GetMapIdFormatsSel()->IsValid())
	{
		// R�cup�re l'ident du format courant
		IdFormat		= GetMapIdFormatsSel()->GetKey();

		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
		{			
			if (Abrege)
				// libell� format abr�g�
				return pSrcPresse->m_TBLFormats.GetLabelCourtByID(IdFormat); 
			else
				// libell� format complet
				return pSrcPresse->m_TBLFormats.GetLabelByID(IdFormat); 
		}
		else
		{
			if (Abrege)
				// libell� format abr�g�
				return "n.d";
			else
				// libell� format complet
				return "non d�fini";
		}
	}
	else

		if (Abrege)
			// libell� format abr�g�
			return "n.d";
		else
			// libell� format complet
			return "non d�fini";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration libell� format courant support et liste des formats support		
void JATPDocChoixSupport::GetFormatsSupport(JUnt32 IdSupport, 
											long &InxSelFormat,
											CString &ListFormatSupp, 
											JUnt32 TypeModule)
{
	// Init Index Format Combo Box Formats
	InxSelFormat = -1;

	// on r�cup�re ident et libell� format s�lectionn� dans module (s�lection pour tous les supports)
	JUnt32  IdFormatSel  = GetIdFormatPrincipal();
	// CString LibFormatSel = GetFormat(IdFormatDef, true);

	// R�cup�re liste des idents formats possibles pour ce support (parmi les formats s�lectionn�s)
	JMap <JUnt32,JUnt32> MapIdFormats;
	GetListIdFormats(IdSupport,&MapIdFormats);

	if (MapIdFormats.GetCount() > 0)
	{
		// Reconstitue la chaine des formats possibles (pour affichage combox box)
		ListFormatSupp = "*\n";

		// Indique index non d�fini par d�faut
		InxSelFormat   = 0;

		// Selon les modules -- R�cup�ration de l'ident format pr�c�demment s�lectionn� pour ce support
		JUnt32 IdFormatSelSupport = 0;
		if (TypeModule == ModuleEch)
		{
			// Recherche pour ce support du format s�lectionn� pour �chelle
			m_MapIdFormatSpecifEch.MoveTo(IdSupport);
			if (m_MapIdFormatSpecifEch.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifEch.GetItem();
		}
		else if (TypeModule == ModuleMap)
		{
			// Recherche pour ce support du format s�lectionn� pour mapping
			m_MapIdFormatSpecifMap.MoveTo(IdSupport);
			if (m_MapIdFormatSpecifMap.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifMap.GetItem();
		}
		else if (TypeModule == ModuleCum)
		{
			// Recherche pour ce support du format s�lectionn� pour cumulativit�
			m_MapIdFormatSpecifCum.MoveTo(IdSupport);
			if (m_MapIdFormatSpecifCum.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifCum.GetItem();
		}
		else if (TypeModule == ModuleDup)
		{	
			// Recherche pour ce support du format s�lectionn� pour duplication
			m_MapIdFormatSpecifDup.MoveTo(IdSupport);
			if (m_MapIdFormatSpecifDup.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifDup.GetItem();
		}

		// Boucle sur les formats supports possibles
		long InxFormat = 1;
		for (MapIdFormats.MoveFirst(); MapIdFormats.IsValid(); MapIdFormats.MoveNext())   
		{
			// R�cup�re l'ident format
			JUnt32 IdFormat = MapIdFormats.GetKey(); 

			// On r�cup�re index du format s�lectionn� dans la liste des formats possibles support
			if (IdFormat == IdFormatSelSupport)
				InxSelFormat = InxFormat;

			// R�cup�re le texte abr�g� associ�
			CString FormatOK = GetFormat(IdFormat,true);
			ListFormatSupp += FormatOK;
			ListFormatSupp += "\n";

			// Passe � l'index format suivant dans la liste qui sera dans le combo box des formats
			InxFormat++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration libell� format courant famille et liste des formats famille
void JATPDocChoixSupport::GetFormatsFamille (JUnt32		IdFamille, 
											 long		&InxSelFormat,
											 CString	&ListFormatFamille, 
											 JUnt32		TypeModule)
{
	// Init Index Format Combo Box Formats
	InxSelFormat = -1;

	// on r�cup�re ident et libell� format s�lectionn� dans module (s�lection pour tous les supports)
	JUnt32  IdFormatSel  = GetIdFormatPrincipal();

	// R�cup�re liste des idents formats possibles pour ce support (parmi les formats s�lectionn�s)
	JMap <JUnt32,JUnt32> MapIdFormatsFamille;
	MapIdFormatsFamille.Reset(); 

	// R�cup �lmt famille
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem *pElem = pReg->GetIElemByID(IdFamille);

	// Si famille et mode famille actif
	if (pElem->IsLot()) //&& m_AFFModeFamilles == TRUE)
	{
		// Cr�e liste des supports lot
		JList<JUnt32> ListSup;
		ILot * pLot = pElem->AsLot();
		if(pLot)
		{
			// R�cup�re liste des composants
			pLot->GetFils(ListSup);

			// Nb de supports actifs
			JUnt32 NbSuppActif = 0;

			// R�cup�re map des supports valides sur les segments en cours
			JMap <JUnt32,JUnt32> MapSuppSegTerr;
			GetMapSupportValideSegTerrain(MapSuppSegTerr);

			// Calcul le tarif des composants existants
			for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				// R�cup composant famille
				JUnt32 IdCompo = ListSup.GetItem(); 

				// Voir si support existant sur enquete
				MapSuppSegTerr.MoveTo(IdCompo);
				if (MapSuppSegTerr.IsValid())
				{
					// R�cup�re liste des idents formats possibles pour ce support (parmi les formats s�lectionn�s)
					JMap <JUnt32,JUnt32> MapIdFormats;
					GetListIdFormats(IdCompo,&MapIdFormats);

					// Mise � jour du map format famille
					for (MapIdFormats.MoveFirst(); MapIdFormats.IsValid(); MapIdFormats.MoveNext())
					{
						// Id format support
						JUnt32 IdFormat = MapIdFormats.GetKey();
						
						// V�rifie si nouveau format famille, dans ce cas on l'ajoute
						MapIdFormatsFamille.MoveTo(IdFormat);
						if (!MapIdFormatsFamille.IsValid()) 
							MapIdFormatsFamille.Add(IdFormat);
					}
				}
			}
		}
	}
	
	if (MapIdFormatsFamille.GetCount() > 0)
	{
		// Reconstitue la chaine des formats possibles (pour affichage combox box)
		ListFormatFamille = "*\n";

		// Indique index non d�fini par d�faut
		InxSelFormat   = 0;

		// Selon les modules -- R�cup�ration de l'ident format pr�c�demment s�lectionn� pour ce support
		JUnt32 IdFormatSelSupport = 0;
		if (TypeModule == ModuleEch)
		{
			// Recherche pour ce support du format s�lectionn� pour �chelle
			m_MapIdFormatSpecifEch.MoveTo(IdFamille);
			if (m_MapIdFormatSpecifEch.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifEch.GetItem();
		}
		else if (TypeModule == ModuleMap)
		{
			// Recherche pour ce support du format s�lectionn� pour mapping
			m_MapIdFormatSpecifMap.MoveTo(IdFamille);
			if (m_MapIdFormatSpecifMap.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifMap.GetItem();
		}
		else if (TypeModule == ModuleCum)
		{
			// Recherche pour ce support du format s�lectionn� pour cumulativit�
			m_MapIdFormatSpecifCum.MoveTo(IdFamille);
			if (m_MapIdFormatSpecifCum.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifCum.GetItem();
		}
		else if (TypeModule == ModuleDup)
		{	
			// Recherche pour ce support du format s�lectionn� pour duplication
			m_MapIdFormatSpecifDup.MoveTo(IdFamille);
			if (m_MapIdFormatSpecifDup.IsValid())
				IdFormatSelSupport  = m_MapIdFormatSpecifDup.GetItem();
		}

		// Boucle sur les formats famille possibles
		long InxFormat = 1;
		for (MapIdFormatsFamille.MoveFirst(); MapIdFormatsFamille.IsValid(); MapIdFormatsFamille.MoveNext())   
		{
			// R�cup�re l'ident format
			JUnt32 IdFormat = MapIdFormatsFamille.GetKey(); 

			// On r�cup�re index du format s�lectionn� dans la liste des formats possibles support
			if (IdFormat == IdFormatSelSupport)
				InxSelFormat = InxFormat;

			// R�cup�re le texte abr�g� associ�
			CString FormatOK  = GetFormat(IdFormat,true);
			ListFormatFamille += FormatOK;
			ListFormatFamille += "\n";

			// Passe � l'index format suivant dans la liste qui sera dans le combo box des formats
			InxFormat++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'ident format par d�faut
JUnt32 JATPDocChoixSupport::GetIdFormatPrincipal()
{
	// Recherche l'ident format du module courant
	JUnt32 IdFormat = 0;
	GetMapIdFormatsSel()->MoveFirst();
	if (GetMapIdFormatsSel()->IsValid())
	{
		// R�cup�re l'ident du format courant
		IdFormat		= GetMapIdFormatsSel()->GetKey();
	}

	return IdFormat;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'ident format via libell�
JUnt32 JATPDocChoixSupport::GetIdFormat(CString LibFormat, bool Abrege)
{
	// Init ident format trouv�
	JUnt32 IdFormatTrouve = 0;

	const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
	if(pSrcPresse && LibFormat != "")
	{
		// Boucle sur les formats de l'offre
		for (m_Offre.m_TBLIdFormats.MoveFirst();m_Offre.m_TBLIdFormats.IsValid();m_Offre.m_TBLIdFormats.MoveNext())
		{
			// R�cup�re ident format
			JUnt32 IdFormat = m_Offre.m_TBLIdFormats.GetItem();

			if (Abrege)
			{
				// libell� format abr�g� est-il identique
				if (pSrcPresse->m_TBLFormats.GetLabelCourtByID(IdFormat) == LibFormat)
				{
					IdFormatTrouve = IdFormat;
					break;
				}
			}
			else
			{
				// libell� format complet
				if (pSrcPresse->m_TBLFormats.GetLabelByID(IdFormat) == LibFormat)
				{
					IdFormatTrouve = IdFormat;
					break;
				}
			}
		}
	}

	// Retourne l'ident format trouv�
	return IdFormatTrouve;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration du libell� format (abr�g� ou complet)
CString JATPDocChoixSupport::GetFormat(JUnt32 IdFormat, bool Abrege)
{
	// -------------------------------------------- Le format utilis� ------------------------------------------------
	const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
	if(pSrcPresse && IdFormat != 0)
	{			
		if (Abrege)
			// libell� format abr�g�
			return pSrcPresse->m_TBLFormats.GetLabelCourtByID(IdFormat); 
		else
			// libell� format complet
			return pSrcPresse->m_TBLFormats.GetLabelByID(IdFormat); 
	}
	else
	{
		return "n.d";
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Positionnement affichage des rangs r�sultats
JVoid JATPDocChoixSupport::SetRangVisible(JUnt32 TypeModule, JBool EtatAffichRang, JInt32 IdGrandeur)
{
	///////////////////////////// Maj affichage rang r�sultats Echelle ////////////////////////
	if (TypeModule == ModuleEch)
	{			
		if (IdGrandeur < 0)
		{
			// Init Map �tat affichage rang �chelle pour toutes les grandeurs r�sultats
			m_MapEtatRangResultEch.Reset();
			for (m_LstOrdoGrandeurEch.MoveFirst(); m_LstOrdoGrandeurEch.IsValid(); m_LstOrdoGrandeurEch.MoveNext())
			{
				// R�cup index
				JUnt32 IdGrandeur = m_LstOrdoGrandeurEch.GetItem();
				
				// Ajout �tat rang pour cette grandeur
				m_MapEtatRangResultEch.Add(IdGrandeur) = EtatAffichRang; 
			}
		}
		else
		{
			// Id Grandeur R�elle
			IdGrandeur = IdGrandeur % 1000;

			// Positionne �tat affichage pour 1 seule grandeur r�sultat �chelle
			m_MapEtatRangResultEch.MoveTo(IdGrandeur);
			if (m_MapEtatRangResultEch.IsValid())
			{
				// Modifie �tat affichage rang pour cette grandeur
				JBool &EtatRang = m_MapEtatRangResultEch.GetItem();
				EtatRang = EtatAffichRang;
			}		
			else
			{
				// Ajout si affichage rang pour cette garndeur non positionn�
				m_MapEtatRangResultEch.Add(IdGrandeur) = EtatAffichRang;
			}
		}
	}		

	/////////////////////////////// Voir pour les autres modules ////////////////////
	// A VOIR
}

///////////////////////////////////////////////////////////////////////////////////////
// R�cup �tat des rangs afficher dans Echelle
JBool JATPDocChoixSupport::GetRangVisible(JUnt32 TypeModule, JInt32 IdGrandeur)
{
	///////////////////////////// Affichage rang r�sultats Echelle ////////////////////////
	if (TypeModule == ModuleEch)
	{			

		// Tout d'abord on v�rifie si l'option affichage rang est s�lectionn�e
		// if (!m_AFFRangResultatEch) return false;

		// Id Grandeur r�elle 
		IdGrandeur = IdGrandeur % 1000;

		// Positionne sur �tat affichage grandeur
		m_MapEtatRangResultEch.MoveTo(IdGrandeur);
		if (m_MapEtatRangResultEch.IsValid())
		{
			// Modifie �tat affichage rang pour cette grandeur
			JBool EtatRang = m_MapEtatRangResultEch.GetItem();
			return m_MapEtatRangResultEch.GetItem();
		}
		// Etat affichage grandeur non positionn�, donc pas visible
		return false;
	}		

	// Pour les autres modules pas de notion de rang
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Compl�te chaine num�rique avec des caract�res devant (pour palier au probl�me tri)
void JATPDocChoixSupport::CompleteString(CString &Txt, JUnt32 NbTotCar)
{
	// Nouvelle chaine
	CString NewTxt = Txt;
	for (int i = Txt.GetLength() ; i < static_cast<int>(NbTotCar); i++)
		NewTxt = "0" + NewTxt;

	Txt = NewTxt;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Recompose la chaine selon le formatage demand�e
void JATPDocChoixSupport::RebuildString(CString &Txt, JUnt32 ModeRebuild, JUnt32 NbDecimale)
{
	if (Txt.Find("*", 0) > 0)
	{
		// Traitement sp�cifique des r�sultats non d�termin�s
		Txt = "*";
		return;
	}

	if (ModeRebuild == FMT_TEXT_MILLIERS)
	{
		// Init nouvelle chaine de caract�re
		CString NewTxt = "";

		// Vire les espaces
		for (int i = 0; i < Txt.GetLength(); i++)
		{
			if (Txt.Mid(i,1) != " ") 
				NewTxt += Txt.Mid(i,1);
		}
		
		// r�cup�re chaine sans les espaces
		Txt = NewTxt;

		// Prendre la valeur de la chaine
		if (Txt.Find(".",0) > 0 || Txt.Find(",",0) > 0) 
		{
			// Ici chaine num�rique avec virgule flottante
			JFlt64 ValTxt = atof(Txt);

			// Refaire la chaine avec les espaces
			if (ValTxt > 0.0)
				if (NbDecimale == 1)
					Txt.Format("%0.1f",ValTxt);
				else
					// Pas plus de 2 d�cimales
					Txt.Format("%0.2f",ValTxt);
			else
				Txt = "0";
		}
		else
		{
			// Chaine num�rique enti�re
			JUnt32 ValTxt = atoi(Txt);

			// Refaire la chaine avec les espaces
			if (ValTxt > 0)
			{
				Txt.Format("%d",ValTxt);
				FormatString(Txt,3);
			}
			else
				Txt = "0";
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re tarif associ� au support
JInt32 JATPDocChoixSupport::GetTarifSupport(JUnt32 IdSupport, JUnt32 IdFormat, JBool &TarifManuel)
{
	JUnt32 Duree;
	JDate  DateExact;
	JUnt32 Tarif = 0;

	// Instance des tarifs
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Cl� nouveau tarif manuel
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = IdSupport;
	KeyTarifManuel.m_IdFormat   = IdFormat;

	// Test si tarif manuel existe pour ce support
	TarifManuel = false;
	m_MapTarifManuel.MoveTo(KeyTarifManuel);
	if (m_MapTarifManuel.IsValid())
	{
		// R�cup�re tarif manuel 
		Tarif = (JUnt32) m_MapTarifManuel.GetItem(); 

		// C'est un tarif manuel
		TarifManuel = true;
	}

	else if(!pTarifs->GetTarifByDate(IdSupport, IdFormat, m_DateTarifaire,m_DateTarifaire+365,DateExact,Duree,Tarif))
		Tarif = 0;

	return Tarif;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re le tarif associ� � une famille
JInt32 JATPDocChoixSupport::GetTarifFamille(JUnt32 IdFamille, JUnt32 IdFormat, JDate DateTarifaire, 
											JDate &DateExact, JUnt32 &Duree, JMap <JUnt32,JUnt32> &MapSuppSegTerr,
											JBool TarifManuel)
{
	// Init Tarif Famille
	JInt32	TarifFamille = 0;

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// Cl� nouveau tarif manuel
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = IdFamille;
	KeyTarifManuel.m_IdFormat   = IdFormat;

	// Test si tarif manuel existe pour cette famille
	m_MapTarifManuel.MoveTo(KeyTarifManuel);
	if (m_MapTarifManuel.IsValid())
		// R�cup�re tarif manuel 
		TarifFamille = (JUnt32) m_MapTarifManuel.GetItem(); 

	else
	{
		// R�cup �lmt famille
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem *pElem = pReg->GetIElemByID(IdFamille);

		// Si famille et mode famille actif
		if (pElem->IsLot() && m_AFFModeFamilles == TRUE)
		{
			// Cr�e liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// R�cup�re liste des composants
				pLot->GetFils(ListSup);

				// Nb de supports actifs
				JUnt32 NbSuppActif = 0;

				// Calcul le tarif des composants existants
				for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				{
					// R�cup composant famille
					JUnt32 IdCompo = ListSup.GetItem(); 

					// Voir si support existant sur enquete
					MapSuppSegTerr.MoveTo(IdCompo);
					if (MapSuppSegTerr.IsValid())
					{
						// R�cup tarif composant
						JUnt32 Tarif = 0;
						JInt32 TarifExist = pTarifs->GetTarifByDate(IdCompo, IdFormat, DateTarifaire, DateTarifaire+365,
													DateExact,Duree,Tarif);
						/* attention, si tarif 0 renvoie une valeur > 0 ??? donc difficile de tester si tarif valid)
							if(!pTarifs->GetTarifByDate(IdCompo, IdFormat, DateTarifaire, DateTarifaire+365,
													DateExact,Duree,Tarif))
						*/

						// Si tarif inexistant ou tarif � 0, le tarif et calcul sur co�t ne seront pas calcul�s
						if (!TarifExist || Tarif == 0)
						{
							// Pas la peine de continuer, 1 tarif au moins de composant non valide
							TarifFamille = 0;
							break;
						}
						else
						{
							// Cumul des tarifs composants
							TarifFamille += Tarif;
							NbSuppActif++;
						}
					}
				}

				// Puis il faut faire la moyenne ????
				// Comment car d�pend des composants valides sur terrain
				/*
				if (ListSup.GetCount() > 0) 
					TarifFamille /= ListSup.GetCount();
				*/

				// On fait la moyenne si on n'est pas en mode famille cumul�e
				if (NbSuppActif > 0 && m_CumulFamille == false) 
					TarifFamille /= NbSuppActif;
			}
		}
	}

	return TarifFamille;
}

///////////////////////////////////////////////////////////////////////////////////
// R�cup�re la p�riodicit� de la famille
JString JATPDocChoixSupport::GetPeriodiciteFamille(JUnt32 IdFamille)
{
	// Init P�riodicit� Famille
	JString TxtAbrPeriodFamille = "*";

	// R�cup �lmt famille
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem *pElem = pReg->GetIElemByID(IdFamille);

	// Si famille et mode famille actif
	if (pElem->IsLot() && m_AFFModeFamilles == TRUE)
	{
		// Cr�e liste des supports lot
		JList<JUnt32> ListSup;
		ILot * pLot = pElem->AsLot();
		if(pLot)
		{
			// R�cup�re liste des composants
			pLot->GetFils(ListSup);

			// V�rifie p�riodicit� de chacun des composants
			for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				// R�cup composant famille
				JUnt32 IdCompo = ListSup.GetItem(); 

				// Ecriture P�riodicit�
				JString TxtPeriodicite,TxtAbrPeriodicite;
				GetPeriodicite(IdCompo, TxtPeriodicite, TxtAbrPeriodicite);

				// Test si p�riodicit� inchang�e
				if (TxtAbrPeriodFamille == "*")
					TxtAbrPeriodFamille = TxtAbrPeriodicite;

				if (TxtAbrPeriodFamille != TxtAbrPeriodicite)
				{
					// P�riodicit� multiple
					TxtAbrPeriodFamille = "Multiple";
					break;
				}
			}
		}
	}

	// Renvoi la p�riodicit� famille
	return TxtAbrPeriodFamille;
}

/* rien � voir ici
//////////////////////////////////////////////////////////////////////////////
// Nb composants famille
JBool JATPDocChoixSupport::ComposantFamille(IElem* pFamille, JUnt32 NbCompoTot, JUnt32 NbCompoValid)
{
	// A FAIRE
	return true;
}
*/

