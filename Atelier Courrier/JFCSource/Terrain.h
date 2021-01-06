
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

#include "Enquete.h"
#include "ItemsNatifManager.h"
#include "ReperesAbsolus.h"
#include "RelationsEquivalence.h"
#include "VariablesNumeriques.h"
#include "VarAgregationManager.h"
#include "Poids.h"
#include "FacteursSupports.h"
#include "Segments.h"
#include "FiltresAudience.h"
#include "Security.h"
#include "SecuritySupports.h"
#include "AttributSupports.h"
#include "Marques.h"
#include "Secteurs.h"

/// Auto Pointer to a CEnquete object
typedef std::auto_ptr<CEnquete> PTR_ENQUETE;
/// Auto Pointer to a CItemsNatifManager object
typedef std::auto_ptr<CItemsNatifManager > PTR_ITEMSNATIFMANAGER;
/// Auto Pointer to a CReperesAbsolus object
typedef std::auto_ptr<CReperesAbsolus > PTR_REPERESABSOLUS;
/// Auto Pointer to a CRelationsEquivalence object
typedef std::auto_ptr<CRelationsEquivalence > PTR_RELATIONSEQUIVALENCE;
/// Auto Pointer to a CVariablesNumeriques object
typedef std::auto_ptr<CVariablesNumeriques > PTR_VARIABLESNUMERIQUES;
/// Auto Pointer to a CVarAgregationManager object
typedef std::auto_ptr<CVarAgregationManager > PTR_VARAGREGATIONMANAGER;
/// Auto Pointer to a CPoids object
typedef std::auto_ptr<CPoids > PTR_POIDS;
/// Auto Pointer to a CFacteursSupports object
typedef std::auto_ptr<CFacteursSupports> PTR_FACTEURSSUPPORTS;
/// Auto Pointer to a CSegments object
typedef std::auto_ptr<CSegments> PTR_SEGMENTS;
/// Auto Pointer to a CFiltresAudience object
typedef std::auto_ptr<CFiltresAudience> PTR_FILTRESAUDIENCE;
/// Auto Pointer to a CSecurity object
typedef std::auto_ptr<CSecurity> PTR_SECURITY;
/// Auto Pointer to a CSecurity object
typedef std::auto_ptr<CAttributSupports> PTR_ATTRIBUTSUPPORTS;
/// Auto Pointer to a CSecurity object
typedef std::auto_ptr<CSecuritySupports> PTR_SECURITYSUPPORTS;
/// Auto Pointer to a CMarques object
typedef std::auto_ptr<CMarques> PTR_MARQUES;
/// Auto Pointer to a CSecteurs object
typedef std::auto_ptr<CSecteurs> PTR_SECTEURS;

/// The Main class for Loading and accessing a Terrain
/**
 * \ingroup JFCSOURCE EXTERNAL
 * This Class deals with the loading and management of a Terrain.
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CTerrain  
{
public:

	// Destructeur
	virtual ~CTerrain();

	/**
	 * Loads the Terrain
	 * \param &Name 
	 * Name of the Terrain, with a leading \
	 * \return 
	 * True if the Terrain loaded successfuly\n
	 * False if the Terrain was not loaded
	 */
	JBool	Load(JStringEx &Name, JInt32 SeuilRedhibitoire, JInt32 SeuilAvertissement, JBool AffichTerOk = true);

	/**
	 * Check if this Terrain is load
	 * \return false if not loaded
	 */
	JBool	IsLoaded() const { return m_fLoaded; } ;

	/**
	 * Gets the Source ID of this Terrain
	 * \return 
	 * Source ID
	 */
	JInt32					GetSourceID() const;

	/**
	 * Gets the ID of this Terrain
	 * \return 
	 * Terrain ID
	 */
	JInt32					GetTerrainID() const;


	/**
	 * Gets the (Calculated) variable values.
	 * \param VarId 
	 * The Aggregation Variable Index.
	 * \param SupportID 
	 * The Support ID.
	 * \return 
	 * The calculated aggregation variables values.
	 */
	JBool					VarHasBinValues(JInt32 Repere);
	JBool					VarHasNumValues(JInt32 Repere);
	JInt32					GetVarDividerByRepAbs(JInt32 Repere);
	const JFLT32VECTOR *	GetVarNumValuesByRepAbs(JInt32 Repere);
	const CBitArray *		GetVarBinValuesByRepAbs(JInt32 Repere);
	JInt32					GetVarAgrDividerById(JInt32 VarId);
	const JFLT32VECTOR *	GetVarAgrNumValuesByIdx(JInt32 VarId, JInt32 Index);
	const CBitArray *		GetVarAgrBinValuesByIdx(JInt32 VarId, JInt32 Index);
	const JFLT32VECTOR *	GetVarAgrNumValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds);
	const CBitArray *		GetVarAgrBinValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds);
	const JFLT32VECTOR *	GetVarNumValuesByIdx(JInt32 VarIndex);
	JInt32					GetVarNumDividerByIdx(JInt32 VarIndex);
	/// Gets the Numeric variable Number for the given Repere\n
	JBool					GetVarAgrIdByRepAbs(JInt32 Repere, JInt32 &VarId) const;
	/// Gets the Numeric variable Number for the given ItemNatif\n
	JBool					GetVarAgrIdByItemNatif(JInt32 ItemNatif, JInt32 &VarId) const;
	/// Gets the Numeric variable Index for the given Repere\n
	JBool					GetVarIdxByRepAbs(JInt32 Repere, JInt32 &Index) const;
	/// Gets the Numeric variable Index for the given ItemNatif\n
	JBool					GetVarIdxByItemNatif(JInt32 ItemNatif, JInt32 &Index) const;
	/// Gets a vector of individuals from a given Item natif 
	const CBitArray *		GetIndividusByItemNatif(JInt32 ItemNatif, const JVector<JInt32> & IdSegments);
	/// Gets a vector of individuals from a given Repere Absolut 
	const CBitArray *		GetIndividusByRepereAbsolut(JInt32 Repere, const JVector<JInt32> & IdSegments);
	/// Fills a tree with the index thematique
	JBool					FillIndexThematique(CTreeCtrl &Tree,HTREEITEM hParent, JBool AllLeaves = false, JBool OnlyQuanti = false);
	/// Gets the list of questions with BlocID\n
	/// If blocID = 0 then all questions are taken
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, JBool OnlyQuanti = false) const;
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, JBool OnlyQuanti = false) const; 	
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti = false) const;
	JBool	HasQuestionsByBlocID(JInt32 BlocID) const;
	JBool	HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter) const; 	
	JBool	HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap) const;
	/// Returns the question corresponding to the question ID
	const CQuestion  *		GetQuestionByID(JInt32 QuestionID) const;
	/// Returns the question corresponding to the Repere Absolut
	const CQuestion  *		GetQuestionByRepAbs(JInt32 Repere) const;
	/// Returns the question corresponding to the ItemNatif
	const CQuestion  *		GetQuestionByItemNatif(JInt32 ItemNatif) const;
	/// Gets the Question ID for the given Repere\n
	/// Return 0 if not found
	JInt32					GetQuestionIDByRepAbs(JInt32 Repere) const;
	/// Gets the Question ID for the given ItemNatif\n
	/// Return 0 if not found
	JInt32					GetQuestionIDByItemNatif(JInt32 ItemNatif) const;
	/// Gets the Termes of the given item native into the Vector\n
	/// and returns the Question ID
	JInt32					GetTermesByItemNatif(JInt32 ItemNatif, JVector<JInt32> &Vector) const;
	/// Gets the Termes of the given repere absolut into the Vector\n
	/// and returns the Question ID
	JInt32					GetTermesByRepAbs(JInt32 Repere, JVector<JInt32> &Vector) const;
	/// Returns the repere absolut of the given question and termes
	JInt32					GetRepAbsByTermes(JInt32 QuestionID, JVector<JInt32> &Termes) const;
	/// Returns true if the given repere exists in this terrain
	JBool					HasRepere(JInt32 Repere) const;
	/// Returns true if the given repere has a relation equivalence
	JBool					HasEquivalence(JInt32 Repere) const;
	/// Returns the relation equivalence for the given repere
	const JChar *			GetEquivalence(JInt32 Repere) const;
	/// Return the default univers ID
	JInt32					GetDefaultUnivers() const;
	/// Return the Homothety factor for the given echantillon and univers
	double					GetFacteurHomothety(JInt32 EchantillonID, JInt32 UniversID) const;
	/// Return the individuals filter for the given echantillon
	const CBitArray *		GetEchantillonFilter(JInt32 EchantillonID) const;
	/// Returns true if this terrain has a given Echantillion Univers and TypePoids
	JBool					HasStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID) const;
	/// Returns the Statistical unit vector for a given Echantillion Univers and TypePoids
	const JDBLVECTOR *		GetStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID);
	/// Returns the Audience filters
	JBool					GetFiltresAudience(JVector<CFiltreAudience  *> & Filtres);
	/// Returns the Audience filter of a given SupportID and FilterID
	const CFiltreAudience * GetFilterAudience(JINT32VECTOR & ReperePrim, JINT32VECTOR & SupportID, JInt32 FiltreID); 
	/// Returns the Population Segments for this terrain
	JBool					GetSegments(JVector<CSegment *> & Segments);
	/// Gets the default population segment and contingent mask
	JInt32					GetDefaultPopSegment(CBitArray & PopSegment, JUnt32 & ContingentMask,bool AllSegment = false);
	///Teste si un element existe
	JBool					HasElement(JInt32 elemID) const;
	///Get a pointeer to the element
	const CElement *		GetElement(JInt32 elemID) const;
	///Teste si un modalite existe
	JBool					HasModalite(JInt32 modaID) const;
	// Get a pointer to the modalite
	const CModalite *		GetModalite(JInt32 modaID) const;
	///Teste si un support existe dans le terrain
	JBool					HasSupport(JInt32 elemID) const;
	///Teste si un support appartient à un segment
	JBool SupportBelongsToSegment(JInt32 elemID, JInt32 segId) const;
	///Teste si un support appartient à un segment
	bool SupportBelongsToSegment(const CSegment * pSegment, JInt32 supportID) const;
	///	Returns true if poids ID exists
	JBool					HasTypePoids(JInt32 poidsID) const;
	/// Returns the default type Poids ID
	JInt32					GetDefaultTypePoidsID() const;
	/// Returns the vector of ID of poids
	JBool					GetTypesPoids(JVector<JInt32>& vect) const;
	/// Returns the default echantillon for a given Universe
	JInt32					GetDefaultEchantillonID(JInt32 UniversID) const;
	/// Recupere les coefs alpha pour un support (structure de répartition)
	JBool GetCoefAlpha(JInt32 SupportId, JInt32 & dureeVie, JVector<JInt32>& vect, JInt32 & diviseur);
	/// Gets the Supports Attributs
	const CAttributSupport *		GetSupportAttributByIdx(JInt32 Index) const;
	/// Gets the Supports Attributs
	const CAttributSupport *		GetSupportAttributByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds) const;
	/// Gets a list of Elements containing the Filter string
	bool	FindElements(JMap<JInt32,bool> & ElementMap, const CString & sFilter) const;
	/// Gets a list of Modalities containing the Filter string
	bool	FindModalities(JMap<JInt32,bool> & ModalityMap, const CString & sFilter) const;

	// Récupération des seuils
	JInt32	GetSeuilRedhibitoire() const;
	JInt32	GetSeuilAvertissement() const;

	// Les elmts enfants et parents (marques et secteurs)
	JBool GetLienMarqueSecteur(JMap<JInt32, JInt32> &MapMarqueSecteur);

private:

	friend class CTerrainManager;
	CTerrain();

	/// Flag to determin if the Terrain was loaded
	JBool						m_fLoaded;

	/// Terrain name
	JStringEx					m_sTerrainName;

	/// Les seuils du terrain 
	/// Seuil au delà duquel une cible peut être utilisée, sinon inutilisable si en dessous
	JInt32						m_SeuilRedhibitoire;	

	/// seuil au delà duquel avertissement cible avec nb contact insuffisant, mais utilisable
	JInt32						m_SeuilAvertissement;		

	/// The enquete data
	PTR_ENQUETE 				m_pEnquete;

	/// The items natif data
	PTR_ITEMSNATIFMANAGER		m_pItemsNatifManager;

	/// The reperes absolus data
	PTR_REPERESABSOLUS 			m_pRepAbs;

	/// The reperes d'equivalence data
	PTR_RELATIONSEQUIVALENCE	m_pRelEquival;

	/// The numeric variable data
	PTR_VARIABLESNUMERIQUES 	m_pVarNum;

	/// The Agregated variable data
	PTR_VARAGREGATIONMANAGER 	m_pVarAgr;

	/// The individual weights data
	PTR_POIDS					m_pPoids;

	/// The facteurs de valorisation des supports data
	PTR_FACTEURSSUPPORTS 		m_pFactSupp;

	/// The Population Segments data
	PTR_SEGMENTS		 		m_pSegment;

	/// The Filtres d'Audience data
	PTR_FILTRESAUDIENCE			m_pFiltre;

	/// Infos Lien Marque / Secteur
	PTR_SECTEURS				m_pSecteurs;

	/// Infos Lien Secteur / MArques
	PTR_MARQUES					m_pMarques;

	/// The security filters
	PTR_SECURITY				m_pSecurity;

	/// The Support Attributs
	PTR_ATTRIBUTSUPPORTS		m_pAttributSupp;

	/// The security filters
	PTR_SECURITYSUPPORTS		m_pSecuritySupports;

	/// assignment operator
	CTerrain & operator =(const CTerrain & rhs);

	/// copy constructor
	CTerrain(const CTerrain & rhs);

};

typedef std::auto_ptr<CTerrain> PTR_TERRAIN;
