// on inclut les définitions nécessaires
#include "JMTFR03TBLPonderations.h"

////////////////////////////////////////////////////////////////
// les constructeurs
JMTFR03TBLPonderations::JMTFR03TBLPonderations()
{
	// On ne fait rien
}

////////////////////////////////////////////////////////////////
// le constructeur copie
JMTFR03TBLPonderations::JMTFR03TBLPonderations(const JMTFR03TBLPonderations & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////////////////
// l'opérateur pour recopier les éléments
JMTFR03TBLPonderations & JMTFR03TBLPonderations::operator =(const JMTFR03TBLPonderations & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();

	// m_TblPonderations = Source.m_TblPonderations; 
}

////////////////////////////////////////////////////////////////
// Fonction d'initialisation
JVoid JMTFR03TBLPonderations::InitPonderation(JInt32 NbIndivs, JFlt64x ValInit)
{
	// On redimensionne le vecteur pondération
	m_TblPonderations.SetCount(NbIndivs);

	// puis on initialise toutes les pondérations à 1
	for (JInt32 IdxIndiv = 0; IdxIndiv < NbIndivs; IdxIndiv++)
	{
		m_TblPonderations.Item(IdxIndiv) = ValInit;
	}
}

////////////////////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments
JInt32 JMTFR03TBLPonderations::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TblPonderations.GetCount());
}

////////////////////////////////////////////////////////////////
// la fonction pour récupérer la pondération d'un indiv
JFlt64x JMTFR03TBLPonderations::GetPonderations(JInt32 IdxIndiv) const
{
	// on vérifie la validité de l'élément
	if (IdxIndiv < 0 || IdxIndiv > m_TblPonderations.GetCount()) throw JInvalidCall::GetInstance();

	// on récupère la pondération
	return (m_TblPonderations.Item(IdxIndiv));
}

////////////////////////////////////////////////////////////////
// la fonction pour positionner la pondération d'un indiv
JVoid JMTFR03TBLPonderations::SetPonderations(JInt32 IdxIndiv, JFlt64x Ponderation)
{
	// on vérifie la validité de l'élément
	if (IdxIndiv < 0 || IdxIndiv > m_TblPonderations.GetCount()) throw JInvalidCall::GetInstance();	

	// On insére la nouvelle pondération
	m_TblPonderations.Item(IdxIndiv) = Ponderation;
}

////////////////////////////////////////////////////////////////
// la fonction pour permuter les éléments
JVoid JMTFR03TBLPonderations::Swap(JMTFR03TBLPonderations & Source)
{
	// on permute les éléments
	m_TblPonderations.Swap(Source.m_TblPonderations);
}

////////////////////////////////////////////////////////////////
// la fonction pour libérer les éléments
JVoid JMTFR03TBLPonderations::Reset()
{
	// on libère les éléments
	m_TblPonderations.Reset();
}

////////////////////////////////////////////////////////////////
// le destructeur
JMTFR03TBLPonderations::~JMTFR03TBLPonderations()
{
	// on ne fait rien
}