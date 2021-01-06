// Terrain.cpp: implementation of the CTerrain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Terrain.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTerrain::CTerrain()
{
	m_sTerrainName = "";
	m_fLoaded = false;

	// Initialisation des seuils*
	m_SeuilRedhibitoire = 0;
	m_SeuilAvertissement= 0;
}

CTerrain::~CTerrain()
{
	// Nothing to do here as all are smart pointers!
}

//////////////////////////////////////////////////////////////////////////////////////////
// Chargement d'un terrain
JBool CTerrain::Load(JStringEx &Name, JInt32 SeuilRedhibitoire, JInt32 SeuilAvertissement, JBool AffichTerOk)
{
	// Message d'erreur pour les fichiers manquants
	CString ErrMessage;

	// Récupération des seuils
	m_SeuilRedhibitoire		= SeuilRedhibitoire;	
	m_SeuilAvertissement	= SeuilAvertissement;	

	PTR_SECURITY pSecurity(new CSecurity);
	if(!pSecurity->Load(Name))
	{
		ErrMessage.Format("%s (%s.OE)","Problème chargement fichier sécurité", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	PTR_SECURITYSUPPORTS pSecuritySupports(new CSecuritySupports);
	if(!pSecuritySupports->Load(Name))
	{
		ErrMessage.Format("%s (%s.ZZ)","Problème chargement fichier sécurité supports", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Load the enquete
	PTR_ENQUETE pEnquete(new CEnquete);
	if(!pEnquete->Load(Name, pSecurity.get()))
	{
		ErrMessage.Format("%s (%s.01)","Problème chargement fichier enquête", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Load the Items natif
	PTR_ITEMSNATIFMANAGER pItemsNatifManager(new CItemsNatifManager);
	if(!pItemsNatifManager->Load(Name))
	{
		ErrMessage.Format("%s (%s.02)","Problème chargement fichier items natifs", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Load the reperes absolus
	PTR_REPERESABSOLUS pRepAbs(new CReperesAbsolus);
	/// TODO: Implement filtering with security
	if(!pRepAbs->Load(Name,pSecurity.get()))
	{
		ErrMessage.Format("%s (%s.05)","Problème chargement fichier repères absolus", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Load the relations d'equivalence
	PTR_RELATIONSEQUIVALENCE pRelEquival(new CRelationsEquivalence);
	/// TODO: Implement filtering with security
	if(!pRelEquival->Load(Name))//, pSecurity.get()))
	{
		ErrMessage.Format("%s (%s.06)","Problème chargement fichier relations équivalences", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Load the variables numeriques
	PTR_VARIABLESNUMERIQUES pVarNum(new CVariablesNumeriques);
	if(!pVarNum->Load(Name))
	{
		ErrMessage.Format("%s (%s)","Problème chargement fichier variables numériques", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Create the agregation variables manager
	PTR_VARAGREGATIONMANAGER pVarAgr(new CVarAgregationManager(Name));

	// Load the poids
	PTR_POIDS pPoids(new CPoids);
	if(!pPoids->Load(Name))
	{
		ErrMessage.Format("%s (%s.09)","Problème chargement fichier poids", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}
	
	// Load the facteurs de valorisation des supports
	PTR_FACTEURSSUPPORTS pFactSupp(new CFacteursSupports);
	if(!pFactSupp->Load(Name))
	{
		ErrMessage.Format("%s (%s.39)","Problème chargement fichier facteurs de valorisation des supports", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}

	// Load the Population Segments
	PTR_SEGMENTS pSegment(new CSegments);
	if(!pSegment->Load(Name))
	{
		ErrMessage.Format("%s (%s.0A)","Problème chargement fichier population segments", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false; // unable to load the population segments
	}

	///////////////////////////////////////////////////////////////
	// A VIRER @@@@@
	PTR_SECTEURS pSecteurs(new CSecteurs);
	if (!pSecteurs->Load(Name))
	{
		/* A VOIR
		ErrMessage.Format("%s (%s.F1)","Problème chargement fichier Secteurs", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false; // unable to load the population segments
		*/
	}
	PTR_MARQUES pMarques(new CMarques);
	if (!pMarques->Load(Name))
	{
		/* A VOIR
		ErrMessage.Format("%s (%s.F2)","Problème chargement fichier Marques", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false; // unable to load the population segments
		*/
	}


	// Load the Filtres d'Audience
	PTR_FILTRESAUDIENCE pFiltre(new CFiltresAudience);
	if(!pFiltre->Load(Name))
	{
		ErrMessage.Format("%s (%s.04)","Problème chargement fichier filtres audience", Name.AsJCharPtr()); 
		if (AffichTerOk) AfxMessageBox(ErrMessage, MB_ICONERROR);
		return false;
	}


	// Load the Attributs de supports
	PTR_ATTRIBUTSUPPORTS pAttributSupp(new CAttributSupports);
//	if(!pAttributSupp->Load(Name))
//		return false; // unable to load the attributs des supports


	// Now update this object
	m_sTerrainName			= Name;
	m_pSecurity				= pSecurity;
	m_pSecuritySupports		= pSecuritySupports;
	m_pEnquete				= pEnquete;
	m_pItemsNatifManager	= pItemsNatifManager;
	m_pRepAbs				= pRepAbs;
	m_pRelEquival			= pRelEquival;
	m_pVarNum				= pVarNum;
	m_pVarAgr				= pVarAgr;
	m_pPoids				= pPoids;
	m_pFactSupp				= pFactSupp;
	m_pSegment				= pSegment;
	m_pFiltre				= pFiltre;

	// En ++ MArques et Secteurs
	m_pSecteurs				= pSecteurs;
	m_pMarques				= pMarques;
	m_pAttributSupp			= pAttributSupp;
	m_fLoaded				= true;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// Récupération du seuil rédhibitoire
JInt32	CTerrain::GetSeuilRedhibitoire() const
{
	return m_SeuilRedhibitoire;
}

////////////////////////////////////////////////////////////////////////////////////
// Récupération du seuil avertissement
JInt32	CTerrain::GetSeuilAvertissement() const
{
	return m_SeuilAvertissement;
}

JInt32	CTerrain::GetVarDividerByRepAbs(JInt32 Repere)
{
	if(!m_fLoaded)
		return 1;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	const CQuestion * pQuestion = GetQuestionByRepAbs(Repere);
	if(pQuestion)
	{
		if(pQuestion->IsNumericDataType())
		{
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return GetVarNumDividerByIdx(VarIndex);
		}
		else if(pQuestion->IsAgregatDataType())
		{
			JInt32 VarId = pQuestion->GetVarAgregatId();
			return GetVarAgrDividerById(VarId);
		}
	}
	return 1;
}

JBool	CTerrain::VarHasNumValues(JInt32 Repere)
{
	if(!m_fLoaded)
		return false;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	const CQuestion * pQuestion = GetQuestionByRepAbs(Repere);
	if(pQuestion)
	{
		if(pQuestion->IsNumericDataType())
		{
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return true;
		}
		else if(pQuestion->IsAgregatDataType())
		{
			JInt32 VarId = pQuestion->GetVarAgregatId();
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return !m_pVarAgr->HasVarBinValues(VarId);
		}
	}
	return false;
}

JBool	CTerrain::VarHasBinValues(JInt32 Repere)
{
	if(!m_fLoaded)
		return false;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	const CQuestion * pQuestion = GetQuestionByRepAbs(Repere);
	if(pQuestion)
	{
		if(pQuestion->IsAgregatDataType())
		{
			JInt32 VarId = pQuestion->GetVarAgregatId();
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return m_pVarAgr->HasVarBinValues(VarId);
		}
	}
	return false;
}

const CBitArray *	CTerrain::GetVarBinValuesByRepAbs(JInt32 Repere)
{
	if(!m_fLoaded)
		return 0;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	const CQuestion * pQuestion = GetQuestionByRepAbs(Repere);
	if(pQuestion)
	{
		if(pQuestion->IsAgregatDataType())
		{
			JInt32 VarId = pQuestion->GetVarAgregatId();
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return GetVarAgrBinValuesByIdx(VarId,VarIndex);
		}
	}
	return 0;
}

const JFLT32VECTOR *	CTerrain::GetVarNumValuesByRepAbs(JInt32 Repere)
{
	if(!m_fLoaded)
		return 0;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	const CQuestion * pQuestion = GetQuestionByRepAbs(Repere);
	if(pQuestion)
	{
		if(pQuestion->IsNumericDataType())
		{
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return GetVarNumValuesByIdx(VarIndex);
		}
		else if(pQuestion->IsAgregatDataType())
		{
			JInt32 VarId = pQuestion->GetVarAgregatId();
			JInt32 VarIndex = 0;
			if(m_pEnquete->GetVarIdxByItemNatif(ItemNatif,VarIndex))
				return GetVarAgrNumValuesByIdx(VarId,VarIndex);
		}
	}
	return 0;
}


JInt32  CTerrain::GetVarAgrDividerById(JInt32 VarId)
{
	if(!m_fLoaded)
		return 1;

	return m_pVarAgr->GetVarAgregatDivider(VarId);
}

const JFLT32VECTOR * CTerrain::GetVarAgrNumValuesByIdx(JInt32 VarId, JInt32 Index)
{
	if(!m_fLoaded)
		return NULL;

	const JFLT32VECTOR * pData = m_pVarAgr->GetVarNumValuesByIdx (VarId, Index);
	return pData;
}

const CBitArray * CTerrain::GetVarAgrBinValuesByIdx(JInt32 VarId, JInt32 Index)
{
	if(!m_fLoaded)
		return NULL;

	const CBitArray * pData = m_pVarAgr->GetVarBinValuesByIdx (VarId, Index);
	return pData;
}

const JFLT32VECTOR * CTerrain::GetVarAgrNumValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds)
{
	if(!m_fLoaded)
		return NULL;

	const JFLT32VECTOR * pData = m_pVarAgr->GetVarNumValuesByIds (VarId, DimensionIds, SupportIds);
	return pData;
}

const CBitArray * CTerrain::GetVarAgrBinValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds)
{
	if(!m_fLoaded)
		return NULL;

	const CBitArray * pData = m_pVarAgr->GetVarBinValuesByIds (VarId, DimensionIds, SupportIds);
	return pData;
}

const JFLT32VECTOR * CTerrain::GetVarNumValuesByIdx(JInt32 VarIndex)
{
	if(!m_fLoaded)
		return NULL;

	return m_pVarNum->GetNumValues(VarIndex);
}

JInt32  CTerrain::GetVarNumDividerByIdx(JInt32 VarIndex)
{
	if(!m_fLoaded)
		return 1;

	return m_pVarNum->GetDivider(VarIndex);
}

const CBitArray *	CTerrain::GetIndividusByRepereAbsolut(JInt32 Repere, const JVector<JInt32> & IdSegments)
{
	if(!m_fLoaded)
		return NULL;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	if(ItemNatif < 0)
		return 0;

	return GetIndividusByItemNatif(ItemNatif, IdSegments);
}

const CBitArray *	CTerrain::GetIndividusByItemNatif(JInt32 ItemNatif, const JVector<JInt32> & IdSegments)
{
	if(!m_fLoaded)
		return NULL;

	if (!m_pSecurity->IsValidItemNatif(ItemNatif, IdSegments))
		return NULL;

	const CBitArray *	pData = m_pItemsNatifManager->GetVectorIndividus(ItemNatif);
	return pData;
}

JBool	CTerrain::FillIndexThematique(CTreeCtrl &Tree,HTREEITEM hParent, JBool AllLeaves, JBool OnlyQuanti)
{
	if(!m_fLoaded)
		return false;

	return m_pEnquete->FillIndexThematique(this, Tree, hParent, AllLeaves, OnlyQuanti); 

}

void	CTerrain::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, JBool OnlyQuanti) const
{
	if(m_fLoaded)
		m_pEnquete->GetQuestionsByBlocID(BlocID, List, OnlyQuanti);
}

void	CTerrain::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter,JBool OnlyQuanti) const
{
	if(!m_fLoaded)
		return;

	if(sFilter.IsEmpty())
		GetQuestionsByBlocID(BlocID,List,OnlyQuanti);
	else
		m_pEnquete->GetQuestionsByBlocID(BlocID, List, sFilter,OnlyQuanti);
}

void	CTerrain::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti) const
{
	if(!m_fLoaded)
		return;

	if(sFilter.IsEmpty())
		m_pEnquete->GetQuestionsByBlocID(BlocID, List, ElementMap, ModalityMap, OnlyQuanti);
	else
		m_pEnquete->GetQuestionsByBlocID(BlocID, List, sFilter, ElementMap, ModalityMap, OnlyQuanti);
}

JBool	CTerrain::HasQuestionsByBlocID(JInt32 BlocID) const
{
	if(!m_fLoaded)
		return false;

	return m_pEnquete->HasQuestionsByBlocID(BlocID);
}

JBool	CTerrain::HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter) const
{
	if(!m_fLoaded)
		return false;

	if(sFilter.IsEmpty())
		return HasQuestionsByBlocID(BlocID);
	else
		return m_pEnquete->HasQuestionsByBlocID(BlocID, sFilter);
}

JBool	CTerrain::HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap) const
{
	if(!m_fLoaded)
		return false;

	if(sFilter.IsEmpty())
		return m_pEnquete->HasQuestionsByBlocID(BlocID, ElementMap, ModalityMap);
	else
		return m_pEnquete->HasQuestionsByBlocID(BlocID, sFilter, ElementMap, ModalityMap);
}

/// Gets a list of elements containing the Filter string
bool	CTerrain::FindElements(JMap<JInt32,bool> & ElementMap, const CString & sFilter) const
{
	if(!m_fLoaded)
		return false;

	return m_pEnquete->FindElements(ElementMap,sFilter);
}

/// Gets a list of Modalities containing the Filter string
bool	CTerrain::FindModalities(JMap<JInt32,bool> & ModalityMap, const CString & sFilter) const
{
	if(!m_fLoaded)
		return false;

	return m_pEnquete->FindModalities(ModalityMap,sFilter);
}

// Returns the question corresponding to the questio ID
const CQuestion  *	CTerrain::GetQuestionByID(JInt32 QuestionID) const
{
	if(!m_fLoaded)
		return 0;
	
	return m_pEnquete->GetQuestionByID(QuestionID);
}

// Gets the Termes of the given item native into the Vector
// and returns the Question ID
JInt32	CTerrain::GetTermesByItemNatif(JInt32 ItemNatif, JVector<JInt32> &Vector) const
{
	if(!m_fLoaded)
		return 0;

	return m_pEnquete->GetTermesByItemNatif(ItemNatif, Vector);
}

// Gets the Termes of the given repere absolut into the Vector
// and returns the Question ID
JInt32	CTerrain::GetTermesByRepAbs(JInt32 Repere, JVector<JInt32> &Vector) const
{
	if(!m_fLoaded)
		return 0;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	if(ItemNatif < 0)
		return 0;

	return m_pEnquete->GetTermesByItemNatif(ItemNatif, Vector);
}

// Returns the repere absolut of the given question and termes
JInt32	CTerrain::GetRepAbsByTermes(JInt32 QuestionID, JVector<JInt32> &Termes) const
{
	if(!m_fLoaded)
		return 0;

	JInt32 ItemNatif = m_pEnquete->GetItemNatifByTermes(QuestionID, Termes);
	if(ItemNatif < 0)
		return 0;

	return m_pRepAbs->GetRepere(ItemNatif);
}

// Returns true if the given repere exists in this terrain
JBool	CTerrain::HasRepere(JInt32 Repere) const
{
	if(!m_fLoaded)
		return false;

	return (m_pRepAbs->GetItemNatif(Repere) >= 0);
}

// Returns true if the given repere has a relation equivalence
JBool	CTerrain::HasEquivalence(JInt32 Repere) const
{
	if(!m_fLoaded)
		return false;

	return m_pRelEquival->HasEquivalence(Repere); 
}

// Returns the relation equivalence for the given repere
const JChar *	CTerrain::GetEquivalence(JInt32 Repere) const
{
	if(!m_fLoaded)
		return 0;

	return m_pRelEquival->GetEquivalence(Repere); 
}

// Gets the Numeric variable index for the given Repere / ItemNatif
// This must only be called if the question is a variable quanti
// Return -1 if not found
JBool	CTerrain::GetVarAgrIdByRepAbs(JInt32 Repere, JInt32 &VarId) const
{
	if(!m_fLoaded)
		return false;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	if(ItemNatif < 0)
		return false;

	return GetVarAgrIdByItemNatif(ItemNatif,VarId);
}

JBool	CTerrain::GetVarAgrIdByItemNatif(JInt32 ItemNatif, JInt32 &VarId) const
{
	if(!m_fLoaded)
		return false;

	if(ItemNatif < 0)
		return false;

	VarId = m_pEnquete->GetVarAgrIdByItemNatif(ItemNatif);
	return VarId > 0;
}

// Gets the Numeric variable Num and Index for the given Repere
// This must only be called if the question is a variable quanti
JBool	CTerrain::GetVarIdxByRepAbs(JInt32 Repere, JInt32 &Index) const
{
	if(!m_fLoaded)
		return false;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	if(ItemNatif < 0)
		return false;

	return GetVarIdxByItemNatif(ItemNatif,Index);
}

// Gets the Numeric variable Num and Index for the given ItemNatif
// This must only be called if the question is a variable quanti
JBool	CTerrain::GetVarIdxByItemNatif(JInt32 ItemNatif, JInt32 &Index) const
{
	if(!m_fLoaded)
		return false;

	if(ItemNatif < 0)
		return false;

	return m_pEnquete->GetVarIdxByItemNatif (ItemNatif,Index);
}


// Gets the Question ID for the given Repere / ItemNatif
// Return 0 if not found
JInt32	CTerrain::GetQuestionIDByRepAbs(JInt32 Repere) const
{
	if(!m_fLoaded)
		return 0;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	if(ItemNatif < 0)
		return 0;

	return GetQuestionIDByItemNatif(ItemNatif);
}

JInt32	CTerrain::GetQuestionIDByItemNatif(JInt32 ItemNatif) const
{
	if(!m_fLoaded)
		return 0;

	if(ItemNatif < 0)
		return 0;

	return m_pEnquete->GetQuestionIDByItemNatif(ItemNatif);
}

// Returns the question corresponding to the Repere Absolut
const CQuestion  *	CTerrain::GetQuestionByRepAbs(JInt32 Repere) const
{
	if(!m_fLoaded)
		return 0;

	JInt32 ItemNatif = 	m_pRepAbs->GetItemNatif(Repere);
	if(ItemNatif < 0)
		return 0;

	return GetQuestionByItemNatif(ItemNatif);
}

// Returns the question corresponding to the ItemNatif
const CQuestion  *	CTerrain::GetQuestionByItemNatif(JInt32 ItemNatif) const
{
	if(!m_fLoaded)
		return 0;

	if(ItemNatif < 0)
		return 0;

	return m_pEnquete->GetQuestionByItemNatif(ItemNatif);
}

// Return the default univers ID
JInt32	CTerrain::GetDefaultUnivers() const
{
	if(!m_fLoaded)
		return 0;

	return m_pPoids->GetDefaultUnivers();
}

// Return the Homothety factor for the given echantillon and univers
double	CTerrain::GetFacteurHomothety(JInt32 EchantillonID, JInt32 UniversID) const
{
	if(!m_fLoaded)
		return 0;

	return m_pPoids->GetFacteur(EchantillonID, UniversID); 
}

// Return the individuals filter for the given echantillon
const CBitArray *	CTerrain::GetEchantillonFilter(JInt32 EchantillonID) const
{
	if(!m_fLoaded)
		return 0;

	return m_pPoids->GetEchantillonFilter(EchantillonID);
}

// Returns true if this terrain has a given Echantillion and Statistic Unit
JBool	CTerrain::HasStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID) const
{
	if(!m_fLoaded)
		return false;

	return m_pPoids->HasStatisticValues(EchantillonID, UniversID, TypePoidsID);
}

// Returns the Statistical unit vector for a given Echantillion and Statistic Unit
const JDBLVECTOR *	CTerrain::GetStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID)
{
	if(!m_fLoaded)
		return 0;

	return m_pPoids->GetStatisticValues(EchantillonID, UniversID, TypePoidsID);

}

// Returns the Source ID
JInt32	CTerrain::GetSourceID() const
{
	if(!m_fLoaded)
		return 0;

	return m_pEnquete->GetSourceID();
}

// Returns the Terrain ID
JInt32	CTerrain::GetTerrainID() const
{
	if(!m_fLoaded)
		return 0;

	return m_pEnquete->GetTerrainID();
}

// Gets the default population segment and contingent mask
JInt32	CTerrain::GetDefaultPopSegment(CBitArray & PopSegment, JUnt32 & ContingentMask, bool AllSegment)
{
	if(!m_fLoaded)
		return -1;

	return m_pSegment->GetDefaultPopSegment(PopSegment,ContingentMask, AllSegment);
}

// Returns the strates for this terrain
JBool CTerrain::GetSegments(JVector<CSegment *> & Segments)
{
	if(!m_fLoaded)
		return false;

	return m_pSegment->GetSegments(Segments);
}

 JBool CTerrain::GetLienMarqueSecteur(JMap<JInt32, JInt32> &MapMarqueSecteur)
{
	if(!m_fLoaded)
		return false;

	return m_pMarques->GetParentMarques(MapMarqueSecteur);
}

// Returns the Audience filters
JBool CTerrain::GetFiltresAudience(JVector<CFiltreAudience  *> & Filtres)
{
	if(!m_fLoaded)
		return false;

	return m_pFiltre->GetFiltres(Filtres);
}

// Returns the Audience filter of a given SupportID and FilterID
const CFiltreAudience * CTerrain::GetFilterAudience(JINT32VECTOR & ReperePrim, JINT32VECTOR & SupportID, JInt32 FiltreID)
{
	if(!m_fLoaded)
		return 0;

	return m_pFiltre->GetFilter(ReperePrim,SupportID,FiltreID); 
}

//Teste si un element existe
JBool CTerrain::HasElement(JInt32 elemID) const
{
	if(!m_fLoaded)
		return false;
	return m_pEnquete->HasElement(elemID);
}

///Get a pointeer to the element
const CElement *	CTerrain::GetElement(JInt32 elemID) const
{
	if(!m_fLoaded)
		return 0;
	return m_pEnquete->GetElement(elemID);
}

JBool CTerrain::HasModalite(JInt32 modaID) const
{
	if(!m_fLoaded)
		return false;
	return m_pEnquete->HasModalite(modaID);
}

// Get a pointer to the modalite
const CModalite *	CTerrain::GetModalite(JInt32 modaID) const
{
	if(!m_fLoaded)
		return 0;
	return m_pEnquete->GetModalite(modaID);
}

//Teste si un support existe dans le terrain
JBool CTerrain::HasSupport(JInt32 elemID) const
{
	if(!m_fLoaded)
		return false;
	return m_pEnquete->HasSupport(elemID) && m_pSecuritySupports->IsSupportValid(elemID);
	//return m_pEnquete->HasSupport(elemID);
}

//Teste si un support appartient à un segment
JBool CTerrain::SupportBelongsToSegment(JInt32 elemID, JInt32 segId) const
{
	if(!m_fLoaded)
		return false;

	if(!HasSupport(elemID))
		return false;

	bool bFound = false;
	JVector<CSegment *> Segments;
	//on recupere les segments
	m_pSegment->GetSegments(Segments);
	//On recupere le masque de contingent
	//JUnt32 contingetMask;
	for(Segments.MoveFirst();Segments.IsValid();Segments.MoveNext())
	{
		if (Segments.GetItem()->GetID() == segId)
		{
			return SupportBelongsToSegment(Segments.GetItem(), elemID);
			//contingetMask = Segments.GetItem()->GetContingentMask();
			//bFound = true;
		}
	}
	return false;
	////si on a trouvé un masque de contingent
	//if (bFound)
	//{
	//	//on teste si le support appartient au contingent
	//	const CElement * element = m_pEnquete->GetElement(elemID);
	//	bFound = element->IsInContingent(contingetMask);
	//}
	//return bFound;
}


// Teste si un support appartient à un segment
JBool CTerrain::SupportBelongsToSegment(const CSegment * pSegment, JInt32 supportID) const
{
	// On recupere le masque de contingent du segment spécifié
	// on teste si le support appartient au contingent
	return m_pEnquete->GetElement(supportID)->IsInContingent(pSegment->GetContingentMask());
}


// Returns the vector of ID of poids
JBool CTerrain::GetTypesPoids(JVector<JInt32>& vect) const
{
	if(!m_fLoaded)
		return false;

	return m_pPoids->GetTypesPoids(vect);
}

///	Returns true if poids ID exists
JBool	CTerrain::HasTypePoids(JInt32 poidsID) const
{
	if(!m_fLoaded)
		return false;

	return m_pPoids->HasTypePoids(poidsID);
}

/// Returns the default type Poids ID
JInt32	CTerrain::GetDefaultTypePoidsID() const
{
	if(!m_fLoaded)
		return 0;

	return m_pPoids->GetDefaultTypePoidsID();
}

JInt32	CTerrain::GetDefaultEchantillonID(JInt32 UniversID) const
{
	if(!m_fLoaded)
		return 0;

	return m_pPoids->GetDefaultEchantillonID(UniversID);
}

/// Gets the Supports Attributs
const CAttributSupport * CTerrain::GetSupportAttributByIdx(JInt32 Index) const
{
	if(!m_fLoaded)
		return 0;

	return m_pAttributSupp->GetAttributSupportByIndex(Index);
}

const CAttributSupport * CTerrain::GetSupportAttributByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds) const
{
	if(!m_fLoaded)
		return 0;

	return m_pAttributSupp->GetAttributSupportByIds(DimensionIds, SupportIds);
}

/// Recupere les coefs alpha pour un support (structure de répartition)
//Si la structure n'existe pas on a un seul élément à 100%
JBool CTerrain::GetCoefAlpha(JInt32 SupportId, JInt32 & dureeVie, JVector<JInt32>& vect, JInt32 & diviseur)
{
	if(!m_fLoaded)
		return false;

	if (!m_pFactSupp->GetRepartionStruct(SupportId, dureeVie, vect, diviseur))
	{
		//on n'a pas de structure on en construit une
		dureeVie = 1;
		diviseur = 100;
		vect.SetCount(dureeVie);
		vect.MoveFirst();
		vect.GetItem() = 100;
	}
	return true;

}
