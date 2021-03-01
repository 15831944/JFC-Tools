// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments
JMTFR03TBLPonderations & JMTFR03TBLPonderations::operator =(const JMTFR03TBLPonderations & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();

	// m_TblPonderations = Source.m_TblPonderations; 
}

////////////////////////////////////////////////////////////////
// Fonction d'initialisation
JVoid JMTFR03TBLPonderations::InitPonderation(JInt32 NbIndivs, JFlt64x ValInit)
{
	// On redimensionne le vecteur pond�ration
	m_TblPonderations.SetCount(NbIndivs);

	// puis on initialise toutes les pond�rations � 1
	for (JInt32 IdxIndiv = 0; IdxIndiv < NbIndivs; IdxIndiv++)
	{
		m_TblPonderations.Item(IdxIndiv) = ValInit;
	}
}

////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments
JInt32 JMTFR03TBLPonderations::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TblPonderations.GetCount());
}

////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer la pond�ration d'un indiv
JFlt64x JMTFR03TBLPonderations::GetPonderations(JInt32 IdxIndiv) const
{
	// on v�rifie la validit� de l'�l�ment
	if (IdxIndiv < 0 || IdxIndiv > m_TblPonderations.GetCount()) throw JInvalidCall::GetInstance();

	// on r�cup�re la pond�ration
	return (m_TblPonderations.Item(IdxIndiv));
}

////////////////////////////////////////////////////////////////
// la fonction pour positionner la pond�ration d'un indiv
JVoid JMTFR03TBLPonderations::SetPonderations(JInt32 IdxIndiv, JFlt64x Ponderation)
{
	// on v�rifie la validit� de l'�l�ment
	if (IdxIndiv < 0 || IdxIndiv > m_TblPonderations.GetCount()) throw JInvalidCall::GetInstance();	

	// On ins�re la nouvelle pond�ration
	m_TblPonderations.Item(IdxIndiv) = Ponderation;
}

////////////////////////////////////////////////////////////////
// la fonction pour permuter les �l�ments
JVoid JMTFR03TBLPonderations::Swap(JMTFR03TBLPonderations & Source)
{
	// on permute les �l�ments
	m_TblPonderations.Swap(Source.m_TblPonderations);
}

////////////////////////////////////////////////////////////////
// la fonction pour lib�rer les �l�ments
JVoid JMTFR03TBLPonderations::Reset()
{
	// on lib�re les �l�ments
	m_TblPonderations.Reset();
}

////////////////////////////////////////////////////////////////
// le destructeur
JMTFR03TBLPonderations::~JMTFR03TBLPonderations()
{
	// on ne fait rien
}