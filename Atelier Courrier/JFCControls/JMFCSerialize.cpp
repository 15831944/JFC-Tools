//==============================
// fichier: JMFCSerialize.cpp
// 
// date: 21/02/2003
// auteur: JB
//------------------------------

// on inclut les fichiers n�cessaires
#include "stdafx.h"

#include <stdio.h>


#include "JMFCSerialize.h"
#include "JInvalidCall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//=======================================================
// le constructeur:
//-------------------------------------------------------

JMFCSerialize::JMFCSerialize(CArchive & ar) : m_Archive(ar)
{
	// on initialise les param�tres du tampon
	m_Buffer      = 0;
	m_BufferCount = 0;
}

//=======================================================
// la fonction d'allocation d'un JMFCSerialize:
//-------------------------------------------------------
JMFCSerialize * JMFCSerialize::Create(CArchive & ar)
{
	// on initialise un pointeur
	JMFCSerialize * pSerialize = new JMFCSerialize(ar);

	// on teste l'allocation
	if (pSerialize == 0) throw (JMemoryFull::GetInstance());

	// on renvoie le pointeur
	return (pSerialize);
}

//=======================================================
// la fonction pour tester le tampon:
//-------------------------------------------------------

JBool JMFCSerialize::IsEmpty()
{
	// on teste la valeur du tampon
	if (m_BufferCount == 0)
	{
		// on teste la lecture d'un �l�ment
		if (this->m_Archive.Read((void*)&m_Buffer, 1) < 1)
		{
			// il n'y a plus d'�l�ment � lire dans l'archive
			return (true);
		}
		else
		{
			// il y a un �l�ment � lire dans l'archive
			m_BufferCount = 1;
		}
	}
	// il reste des �l�ments � lire
	return (false);
}

//=======================================================
// les fonctions pour transf�rer les donn�es:
//-------------------------------------------------------

JVoid JMFCSerialize::Send(JUnt08 Data)
{
	// on ajoute la donn�e � l'archive
	m_Archive <<(BYTE) Data;
}

JUnt08 JMFCSerialize::Recv()
{
	// on teste la valeur du tampon
	if (m_BufferCount == 0)
	{
		// on teste la lecture d'un �l�ment
		if (m_Archive.Read((void*)&m_Buffer, 1) < 1)
		{
			// on l�ve une exception
			throw JInvalidCall::GetInstance();
		}
		else
		{
			// on met � jour la taille du tampon
			m_BufferCount = 1;
		}
	}
	// on d�cr�mente le nombre d'�l�ments du tampon
	m_BufferCount = 0;

	// on renvoie l'�l�ment du buffer
	return (m_Buffer);
}

//=======================================================
// la fonction pour vider le tampon:
//-------------------------------------------------------

JVoid JMFCSerialize::Flush()
{
	m_Archive.Flush(); 
}

//=======================================================
// le destructeur:
//-------------------------------------------------------

JMFCSerialize::~JMFCSerialize()
{
	// on ne fait rien
}

//=======================================================
// l'op�rateur d'affectation:
//-------------------------------------------------------

JMFCSerialize & JMFCSerialize::operator =(const JMFCSerialize & Source)
{
	// on l�ve une exception
	throw JInvalidCall::GetInstance();
}

//=======================================================
// le constructeur copie:
//-------------------------------------------------------

JMFCSerialize::JMFCSerialize(const JMFCSerialize & Source) : m_Archive(Source.m_Archive)
{
	// on l�ve une exception
	throw JInvalidCall::GetInstance();
}

CArchive & JMFCSerialize::GetCArchive()
{
	return m_Archive;
}

