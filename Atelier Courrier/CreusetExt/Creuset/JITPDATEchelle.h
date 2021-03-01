//
// Fichier: JITPDATEchelle.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    29/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JITP_DAT_ECHELLE_H

// on définit la macro pour éviter les inclusions multiples
#define JITP_DAT_ECHELLE_H

#include "JFCDistribution.h"
// on inclut les définitions nécessaires

class JITPDATEchelle
{
public:
	// les constructeurs
	JITPDATEchelle();

	// le destructeur
	~JITPDATEchelle();
	
private:
	// l'opérateur pour recopier l'élément
	JITPDATEchelle & operator =(const JITPDATEchelle & Source);

	// le constructeur copie
	JITPDATEchelle(const JITPDATEchelle & Source);

public:
	JFlt32			m_Tarif;			// le tarif
	JBool			m_TarifManuel;		// est ce un tarif manuel ???	
	JInt32			m_Periodicite;		// la périodicité
	JFlt64			m_AudienceRef;		// l'audience Ref
	JFlt64			m_AudienceProba;    // l'audience Proba
	JFlt64			m_VolLecture;		// le volume de lecture dans le mode en cours
	JFlt64			m_Effectif;			// l'effectif
	JFlt64			m_IndAffinite;		// l'indice d'affinité
	JFlt64			m_PrcAffinite;		// le pourcentage d''affinité
	JFCDistribution	m_Distribution;		// la distribution
	JFlt64			m_Apport;			// l'apport sur plan courant

};

// fin de l'inclusion conditionnelle
#endif
