//=============================
// fichier: JMFCStream.cpp
//
// date: 21/02/2003
// auteur: JB
//-----------------------------

// on inclut les fichiers nécessaires
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
// la fonction pour créer une instance:
//---------------------------------------------------------------------

JMFCStream* JMFCStream::Create(CArchive & ar)
{
	// on crée le JMFCSerialize
	JMFCSerialize* pSerialize = JMFCSerialize::Create(ar);
	
	// on crée le stream
	JMFCStream * pStream = new JMFCStream(pSerialize, pSerialize);

	// on teste l'allocation
	if (pStream == 0)
	{
		// on libère la serialisation
		delete (pSerialize);

		// on lève une exception
		throw (JMemoryFull::GetInstance());
	}
	
	// on mémorise le flot
	pStream->m_pSerialize = pSerialize;
		
	// on renvoie le pointeur
	return (pStream);	
}

//=====================================================================
// le destructeur:
//---------------------------------------------------------------------

JMFCStream::~JMFCStream()
{
	// on libère le JMFCSerialize
	delete m_pSerialize;
}

//=====================================================================
// l'opérateur d'affectation:
//---------------------------------------------------------------------

JMFCStream & JMFCStream::operator =(const JMFCStream & Source)
{
	// on lève une exception
	throw (JInvalidCall::GetInstance());
}


//=====================================================================
// le constructeur copie:
//---------------------------------------------------------------------

JMFCStream::JMFCStream(const JMFCStream & Source) : JStream(0, 0)
{
	// on lève une exception
	throw (JInvalidCall::GetInstance());
}

/////////////////////////////////////////////////////////
// Récupération élmt type CFile
CFile * JMFCStream::GetCFile()
{
	return m_pSerialize->GetCArchive().GetFile();
}

/////////////////////////////////////////////////////////
// Récupération élmt type CArchive
CArchive * JMFCStream::GetCArchive()
{
	return &(m_pSerialize->GetCArchive());
}