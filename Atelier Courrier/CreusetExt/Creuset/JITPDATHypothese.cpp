//
// Fichier: JITPDATHypothese.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    29/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JITPDATHypothese.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////
// les constructeurs

JITPDATHypothese::JITPDATHypothese()
{
	Reset();	// on ne fait rien
}

JITPDATHypothese::JITPDATHypothese(const JITPDATHypothese & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JITPDATHypothese & JITPDATHypothese::operator =(const JITPDATHypothese & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JITPDATHypothese::~JITPDATHypothese()
{
	// on ne fait rien
}

bool JITPDATHypothese::IsValid() const
{
	if(m_NbContactsJour.GetCount() != 0)
		return true;
	return false;
}


void JITPDATHypothese::Swap(JITPDATHypothese & Rhs)
{
	m_budgetOK			=	Rhs.m_budgetOK;
	m_ResultOK			=	Rhs.m_ResultOK;
	m_Budget			=	Rhs.m_Budget ;				// le budget
	m_VolLecture		=	Rhs.m_VolLecture ;			// le volume de lecture
	m_AudienceRef		=	Rhs.m_AudienceRef ;			// l'audience
	m_Couverture		=	Rhs.m_Couverture ;			// la couverture
	m_NbContactsJour.Swap(Rhs.m_NbContactsJour) ;		// le nombre de contacts par jour
	m_AudienceJour.Swap(Rhs.m_AudienceJour) ;			// l'audience par jour pour le calcul du GRP
	m_DistributionJour.Swap(Rhs.m_DistributionJour) ;	// la distribution par jour
	m_maille.Swap(Rhs.m_maille);
}

void JITPDATHypothese::Reset()
{
	m_budgetOK			=	true;
	m_ResultOK			=	true;
	m_Budget			=	0.0;				// le budget
	m_VolLecture		=	0.0;			// le volume de lecture
	m_AudienceRef		=	0.0;			// l'audience
	m_Couverture		=	0.0;			// la couverture
	m_NbContactsJour.Reset() ;		// le nombre de contacts par jour
	m_AudienceJour.Reset() ;		// l'audience par jour pour le calcul du GRP
	m_DistributionJour.Reset() ;	// la distribution par jour
	m_maille.Reset();
}

void JITPDATHypothese::InitNbJours(int nbJours)
{
	m_NbContactsJour.SetCount(nbJours);
	m_AudienceJour.SetCount(nbJours);
	for(int i=0; i<nbJours;i++)
		m_AudienceJour.Item(i) = m_NbContactsJour.Item(i) = 0.0;

	m_DistributionJour.SetCount(nbJours);
}

/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////
/////////////////



JITPDATHypoFormat::JITPDATHypoFormat()
{
	Reset();
}

void JITPDATHypoFormat::InitFormats(JList<JUnt32> &listFormat)
{
	m_Effectif			=	0.0;			// l'effectif
	m_mapDATHypo.Reset();
	for(listFormat.MoveFirst(); listFormat.IsValid(); listFormat.MoveNext())	
		m_mapDATHypo.Add( listFormat.GetItem());
	m_mapDATHypo.Add(FORMATTOTAL);
	m_mapDATHypo.Add(FORMATSACTIFS);
}


void JITPDATHypoFormat::InitNbJours(int nbJours)
{
	for(m_mapDATHypo.MoveFirst(); m_mapDATHypo.IsValid(); m_mapDATHypo.MoveNext())
		m_mapDATHypo.GetItem().InitNbJours(nbJours);

	m_JoursActifs.SetSize(nbJours);
	m_JoursInsertExist.SetSize(nbJours);
}

/////////////////
// le destructeur

JITPDATHypoFormat::~JITPDATHypoFormat()
{
	// on ne fait rien
}

bool JITPDATHypoFormat::IsValid()
{
	bool bRet = true;
	for(m_mapDATHypo.MoveFirst();m_mapDATHypo.IsValid();m_mapDATHypo.MoveNext())
	{
		if(!m_mapDATHypo.GetItem().IsValid())
			return false;
	}
	return true;
}


void JITPDATHypoFormat::Swap(JITPDATHypoFormat & Rhs)
{
	Reset();
	m_Effectif			=	Rhs.m_Effectif ;			// l'effectif
	m_mapDATHypo.Swap(Rhs.m_mapDATHypo);
	m_JoursActifs		=	Rhs.m_JoursActifs ;			// Les jours couvert par les insertions distribués selon alpha
	m_JoursInsertExist	=	Rhs.m_JoursInsertExist ;	// Les jours couvert par les insertions
}

void JITPDATHypoFormat::Reset()
{
	m_Effectif			=	0.0;			// l'effectif
	//m_mapDATHypo.Reset();
	for(m_mapDATHypo.MoveFirst();m_mapDATHypo.IsValid();m_mapDATHypo.MoveNext())
		m_mapDATHypo.GetItem().Reset();

	m_JoursActifs.SetSize(0);			// Les jours couvert par les insertions distribués selon alpha
	m_JoursInsertExist.SetSize(0);	// Les jours couvert par les insertions
}

JITPDATHypothese & JITPDATHypoFormat::GetDATHypoByFormat(JInt32 idFormat)
{
	if(idFormat == -1)
		m_mapDATHypo.MoveTo(FORMATTOTAL);
	else
		m_mapDATHypo.MoveTo(idFormat);
	return m_mapDATHypo.GetItem();
}
JITPDATHypothese & JITPDATHypoFormat::GetDATHypoTotal()
{
	m_mapDATHypo.MoveTo(FORMATTOTAL);
	return m_mapDATHypo.GetItem();
}
JITPDATHypothese & JITPDATHypoFormat::GetDATHypoFormatActif()
{
	m_mapDATHypo.MoveTo(FORMATSACTIFS);
	return m_mapDATHypo.GetItem();
}
const JITPDATHypothese & JITPDATHypoFormat::GetDATHypoByFormat(JInt32 idFormat) const
{
	if(idFormat == -1)
		m_mapDATHypo.MoveTo(FORMATTOTAL);
	else
		m_mapDATHypo.MoveTo(idFormat);
	return m_mapDATHypo.GetItem();
}
const JITPDATHypothese & JITPDATHypoFormat::GetDATHypoTotal() const
{
	m_mapDATHypo.MoveTo(FORMATTOTAL);
	return m_mapDATHypo.GetItem();
}
const JITPDATHypothese & JITPDATHypoFormat::GetDATHypoFormatActif() const
{
	m_mapDATHypo.MoveTo(FORMATSACTIFS);
	return m_mapDATHypo.GetItem();
}
JFlt64 JITPDATHypoFormat::GetEffectif() const
{
	return m_Effectif;
}

JITPDATHypoFormat::JITPDATHypoFormat(const JITPDATHypoFormat & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JITPDATHypoFormat & JITPDATHypoFormat::operator =(const JITPDATHypoFormat & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

