#pragma once

#include "Terrain.h"
#include "BitArray.h"

#include <list>

/**
 * \ingroup JFCITEMS
 *
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
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CCalcItemParams
{
public:
	CCalcItemParams(void);
	virtual ~CCalcItemParams(void);

public:
	/// Sets up and calculates the Effectif
	virtual bool		DoEffectif(const JFLT32VECTOR * pQuanti = 0);

	/// Sets up and calculate the Numerical Values (If any)  /: attention au mode de calcul sans les 0 en valuers numériques
	// virtual bool		DoNumericalValues(bool Moyenne0Exclu = false);
	virtual bool		DoNumericalValues(bool Moyenne0Exclu = false, const JFLT32VECTOR * pQuanti = 0);

	// On calcule l'effectif sans les zéros du vecteur quantitatif
	virtual bool		DoEffectif0Exclu(const JFLT32VECTOR * pQuanti);

	/// Copies the parameters from rhs
	virtual void		CopyParams(const CCalcItemParams &rhs) ;
	/// Sets the Terrain
	virtual void		SetTerrain(CTerrain * pTerrain) { m_pTerrain = pTerrain; } ;
	virtual CTerrain *	GetTerrain() { return m_pTerrain; } ;
	/// Sets the Echantillon ID
	virtual void		SetEchantillonID(JInt32 EchantillonID) { m_EchantillonID = EchantillonID; } ;
	virtual JInt32		GetEchantillonID() { return m_EchantillonID; } ;
	/// Sets the Univers ID
	virtual void		SetUniversID(JInt32 UniversID) { m_UniversID = UniversID; };
	virtual JInt32		GetUniversID() { return m_UniversID; };
	/// Sets the Type Poids ID
	virtual void		SetTypePoidsID(JInt32 TypePoidsID) { m_TypePoidsID = TypePoidsID; };
	virtual JInt32		GetTypePoidsID() { return m_TypePoidsID; };
	/// Sets the Segment
	virtual void		SetPopulationSegment(const CBitArray & Segment) { m_PopSegment = Segment; };
	virtual CBitArray &	GetPopulationSegment() { return m_PopSegment; };
	/// Sets the Segments IDs
	virtual void		SetSegmentsIDs(const JVector<JInt32> & IdSegments);
	virtual void		SetSegmentsIDs(const JList<JInt32> & IdSegments);
	virtual void		SetSegmentsIDs(const JList<JUnt32> & IdSegments);
	virtual void		SetSegmentsIDs(const std::list<JUnt32> & IdSegments);

	virtual const JVector<JInt32> & GetSegmentsIDs() const { return m_IdSegments; }

	// Indique que l'item ne sera pas calculé, et récupère l'info
	virtual void		SetItemAutorise(JBool Autorise) {m_ItemAutorise = Autorise; }; 
	virtual JBool		GetItemAutorise() {return m_ItemAutorise; }; 

	// Libere map 
	virtual void		Raz_pValuesD();

private:
	/// Pointer to current Terrain
	CTerrain *m_pTerrain;
	/// Echantillon ID
	JInt32	m_EchantillonID;
	/// Univers ID
	JInt32	m_UniversID;
	/// Type Poids ID
	JInt32	m_TypePoidsID;
	/// Population Segment
	CBitArray m_PopSegment;
	/// Segments IDs
	JVector<JInt32> m_IdSegments;

public:
	/// Results obtained

	/// Gets the Effectif
	virtual double	GetEffectif() const { return m_Effectif; } ;
	virtual	void 	SetEffectif(double Effectif) { m_Effectif = Effectif; } ;
	/// Gets the Number of Cases
	virtual JInt32	GetNbCas() const { return m_NbCas ; } ;
	virtual	void 	SetNbCas(JInt32 NbCas) { m_NbCas = NbCas ; } ;
	/// Gets the Total Effectif
	virtual double	GetEffectifTot() const { return m_EffectifTot ; } ;
	virtual	void 	SetEffectifTot(double EffectifTot) { m_EffectifTot = EffectifTot ; } ;
	/// Gets the Total Number of cases
	virtual JInt32	GetNbCasTot() const { return m_NbCasTot ; } ;
	virtual	void	SetNbCasTot(JInt32 NbCasTot) { m_NbCasTot = NbCasTot ; } ;


	/// Gets the Effectif
	virtual double	GetEffectif0Exclu() const { return m_Effectif0Exclu; } ;
	virtual	void 	SetEffectif0Exclu(double Effectif) { m_Effectif0Exclu = Effectif; } ;
	/// Gets the Number of Cases
	virtual JInt32	GetNbCas0Exclu() const { return m_NbCas0Exclu ; } ;
	virtual	void 	SetNbCas0Exclu(JInt32 NbCas) { m_NbCas0Exclu = NbCas ; } ;
	/// Gets the Total Effectif
	virtual double	GetEffectifTot0Exclu() const { return m_EffectifTot0Exclu ; } ;
	virtual	void 	SetEffectifTot0Exclu(double EffectifTot) { m_EffectifTot0Exclu = EffectifTot ; } ;
	/// Gets the Total Number of cases
	virtual JInt32	GetNbCasTot0Exclu() const { return m_NbCasTot0Exclu ; } ;
	virtual	void	SetNbCasTot0Exclu(JInt32 NbCasTot) { m_NbCasTot0Exclu = NbCasTot ; } ;


	/// Gets the Min Borne
	virtual JFlt32	GetMinBorne() const { return (m_MinBorne); };
	virtual void	SetMinBorne(JFlt32 MinBorne) { m_MinBorne = MinBorne; }; 
	/// Gets the Max Borne
	virtual JFlt32	GetMaxBorne() const { return (m_MaxBorne); };
	virtual void	SetMaxBorne(JFlt32 MaxBorne) { m_MaxBorne = MaxBorne; }; 

	// Get and Set Mode Cumul Quanti
	virtual JBool	GetCumulQuanti() const { return (m_CumulQuanti); };
	virtual void    SetCumulQuanti(JBool CumulQuanti) {m_CumulQuanti = CumulQuanti; };
	

	virtual	void 	SetResultsD(const CBitArray * pResults);
	virtual	void 	SetResults(const CBitArray * pResults);
	/// Gets the Calculated individuals vector without the Segment & Echantillon filters
	//virtual const CBitArray *	GetResults() const { if(m_pResultsD) return m_pResultsD;
	//																else return m_pResults; } ;
	virtual const CBitArray *	GetResults() const
	{
		if(m_pResultsD)
		{
			// AfxMessageBox("On utilise le D (celui qui sera détruit");
			return m_pResultsD;
		}
		else
		{
			// AfxMessageBox("On utilise l'autre)");
			return m_pResults;
		}
	} ;

	virtual	void 	SetProbas(const JFLT32VECTOR * pProba) ;
	virtual	void 	SetProbasD(const JFLT32VECTOR * pProba) ;
	/// Gets the Calculated Probailities vector
	virtual const JFLT32VECTOR *	GetProbas() const { if(m_pProbasD) return m_pProbasD;
														else return m_pProbas; } ;
	/// Gets the statistic values for the echantillon, univers and typepoids values
	virtual const JDBLVECTOR *		GetStatisticValues();
	/// Gets the individual filter for the given echantillon
	virtual const CBitArray *		GetEchantillonFilter() { return m_pTerrain->GetEchantillonFilter(m_EchantillonID); } ;
	/// Gets the homothety factor  for the Echantillon and univers
	virtual double					GetFactor() { return m_pTerrain->GetFacteurHomothety(m_EchantillonID,m_UniversID); } ;
	/// Numerical variables values
	virtual	void					SetNumericalValues(const JFLT32VECTOR *	pValues);
	virtual	void					SetNumericalValues(const JMap<const JFLT32VECTOR*, JInt32> & ValuesMap, bool SpeItemNiche = false, bool NicheQuanti = true);

	/// Numerical variables values
	// virtual const JFLT32VECTOR *	GetNumericalValues(bool TransfertBinaire = false) const;
	virtual const JFLT32VECTOR *	GetNumericalValues(bool TransfertBinaire = false, bool Value0_Ok = true) const;
	

	/// Agregated Quantity
	double							GetAgregatedQuantity() const { return m_Values; } ;

	/// Gets & Set Specific divisor for Niche
	virtual double					GetDivSpeNiche() const { return m_DivSpeNiche; } ;
	virtual void 					SetDivSpeNiche(double DivSpeNiche) { m_DivSpeNiche = DivSpeNiche; } ;


private:
	/// Copy operator
	CCalcItemParams &	operator=(const CCalcItemParams & rhs);
	/// Copy constructor
	CCalcItemParams(const CCalcItemParams & rhs);

	/// Results
	/// Item Effectif 
	double					m_Effectif;

	/// Item No of cases
	JInt32					m_NbCas;

	/// Item No of Cases Total
	JInt32					m_NbCasTot;

	/// Item Effectif Total
	double					m_EffectifTot;

	/// Item Effectif sans les zéros en valeur numérique
	double					m_Effectif0Exclu;

	/// Item No of cases sans les zéros en valeur numérique
	JInt32					m_NbCas0Exclu;

	/// Item No of Cases Total sans les zéros en valeur numérique
	JInt32					m_NbCasTot0Exclu;

	/// Item Effectif Total sans les zéros en valeur numérique
	double					m_EffectifTot0Exclu;


	/// Item indiviuals vector
	const CBitArray * 		m_pResults;
	CBitArray * 			m_pResultsD;

	/// Probability vector
	const JFLT32VECTOR *	m_pProbas;
	JFLT32VECTOR *			m_pProbasD;

	/// Numerical variables values
	JFLT32VECTOR*			m_pValuesD;
	JFLT32VECTOR*			m_pValuesD_Binaire;


	// Binary transform Numerical variables values
	JFLT32VECTOR*			m_pValueBinaire;

	/// Item Values 
	double					m_Values;

	/// Borne Min et Max
	JFlt32					m_MinBorne;
	JFlt32					m_MaxBorne;

	// Item autorisé pour le calcul
	JBool					m_ItemAutorise;

	// Mode calcul
	JBool					m_CumulQuanti;

	// Diviseur pour le calcul agrégé des niches
	double					m_DivSpeNiche;
};

typedef std::auto_ptr<CCalcItemParams> PTR_CALCITEMPARAMS;