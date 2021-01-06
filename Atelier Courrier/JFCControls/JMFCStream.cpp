//=============================
// fichier: JMFCStream.cpp
//
// date: 21/02/2003
// auteur: JB
//-----------------------------

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "JMFCStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//=====================================================================
// le constructeur:
//---------------------------------------------------------------------

JMFCStream::JMFCStream(JMFCSerialize * pPrev, JMFCSerialize * pNext) : JStream(pPrev, pNext)
{
	// on initialise le flot
	m_pSerialize = 0;
}

//=====================================================================
// la fonction pour cr�er une instance:
//---------------------------------------------------------------------

JMFCStream* JMFCStream::Create(CArchive & ar)
{
	// on cr�e le JMFCSerialize
	JMFCSerialize* pSerialize = JMFCSerialize::Create(ar);
	
	// on cr�e le stream
	JMFCStream * pStream = new JMFCStream(pSerialize, pSerialize);

	// on teste l'allocation
	if (pStream == 0)
	{
		// on lib�re la serialisation
		delete (pSerialize);

		// on l�ve une exception
		throw (JMemoryFull::GetInstance());
	}
	
	// on m�morise le flot
	pStream->m_pSerialize = pSerialize;
		
	// on renvoie le pointeur
	return (pStream);	
}

//=====================================================================
// le destructeur:
//---------------------------------------------------------------------

JMFCStream::~JMFCStream()
{
	// on lib�re le JMFCSerialize
	delete m_pSerialize;
}

//=====================================================================
// l'op�rateur d'affectation:
//---------------------------------------------------------------------

JMFCStream & JMFCStream::operator =(const JMFCStream & Source)
{
	// on l�ve une exception
	throw (JInvalidCall::GetInstance());
}


//=====================================================================
// le constructeur copie:
//---------------------------------------------------------------------

JMFCStream::JMFCStream(const JMFCStream & Source) : JStream(0, 0)
{
	// on l�ve une exception
	throw (JInvalidCall::GetInstance());
}

/////////////////////////////////////////////////////////
// R�cup�ration �lmt type CFile
CFile * JMFCStream::GetCFile()
{
	return m_pSerialize->GetCArchive().GetFile();
}

/////////////////////////////////////////////////////////
// R�cup�ration �lmt type CArchive
CArchive * JMFCStream::GetCArchive()
{
	return &(m_pSerialize->GetCArchive());
}