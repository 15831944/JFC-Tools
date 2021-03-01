//
// Fichier: JITPDATHypothese.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    29/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JITP_DAT_HYPOTHESE_H

// on définit la macro pour éviter les inclusions multiples
#define JITP_DAT_HYPOTHESE_H

// on inclut les définitions nécessaires
#include "JFCDistribution.h"
#include "JFCMaille.h"

#define FORMATTOTAL (-10)
#define FORMATSACTIFS (-15)



class JITPDATHypothese
{
public:
	// les constructeurs
	JITPDATHypothese();

	// le destructeur
	~JITPDATHypothese();

	void Swap(JITPDATHypothese & Rhs); 

	void Reset(); 
	bool IsValid() const; 

	void InitNbJours(int nbJours);

	
	
private:
	// l'opérateur pour recopier l'élément
	JITPDATHypothese & operator =(const JITPDATHypothese & Source);

	// le constructeur copie
	JITPDATHypothese(const JITPDATHypothese & Source);

public:
	bool					m_budgetOK;
	bool					m_ResultOK;
	JFlt64					m_Budget;           // le budget dans le tablier
	JFlt64					m_VolLecture;       // le volume de lecture dans le tablier
	JFlt64					m_AudienceRef;			// l'audience dans le tablier
	JFlt64					m_Couverture;				// la couverture (en individus) dans le tablier
	JArray<JFlt64>			m_NbContactsJour;   // le nombre de contacts par jour
	JArray<JFlt64>			m_AudienceJour;   // l'audience par jour pour le calcul du GRP
	JArray<JFCDistribution>	m_DistributionJour; // la distribution par jour
	JFCMaille				m_maille;


};


class JITPDATHypoFormat
{
public:
	// les constructeurs
	JITPDATHypoFormat();

	// le destructeur
	~JITPDATHypoFormat();

	void Swap(JITPDATHypoFormat & Rhs); 

	void Reset(); 
	bool IsValid(); 

	const JITPDATHypothese & GetDATHypoByFormat(JInt32 idFormat) const ;
	const JITPDATHypothese & GetDATHypoTotal() const ;
	const JITPDATHypothese & GetDATHypoFormatActif() const ;

	JITPDATHypothese & GetDATHypoByFormat(JInt32 idFormat);
	JITPDATHypothese & GetDATHypoTotal();
	JITPDATHypothese & GetDATHypoFormatActif();
	JFlt64		GetEffectif() const;         // l'effectif

	void InitNbJours(int nbJours);
	void InitFormats(JList<JUnt32> &listFormat);

	JMap<JInt32,JITPDATHypothese> m_mapDATHypo;
	JFlt64					m_Effectif;         // l'effectif

	CBitArray				m_JoursActifs;		// Les jours couvert par les insertions format actif distribués selon alpha
	CBitArray				m_JoursInsertExist;	// Les jours couvert par les insertions
	

private:
	// l'opérateur pour recopier l'élément
	JITPDATHypoFormat & operator =(const JITPDATHypoFormat & Source);
	// le constructeur copie
	JITPDATHypoFormat(const JITPDATHypoFormat & Source);

};



// fin de l'inclusion conditionnelle
#endif
