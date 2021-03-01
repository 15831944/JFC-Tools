// Enquete.cpp: implementation of the CEnquete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Enquete.h"
#include "Convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnquete::CEnquete()
{
	// File Name
	m_sName = "";
	m_lSourceID = 0;
	m_lTerrainID = 0;
	m_pIndexThematique = 0;
}

CEnquete::~CEnquete()
{
	if(m_pIndexThematique)
	{
		delete m_pIndexThematique;
		m_pIndexThematique = 0;
	}
}

JBool CEnquete::Load(JStringEx &Name, CSecurity * pSecurity)
{
	m_sName = Name;
	m_sName.Append (".01");

	try
	{
		JArchive * pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadModalites		(pFile, m_FileHeader.GetVersion ());
		LoadElements		(pFile, m_FileHeader.GetVersion ());
		LoadIndexThematique (pFile, m_FileHeader.GetVersion ());
		LoadQuestions		(pFile, m_FileHeader.GetVersion (),pSecurity);

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CEnquete::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35901)
		throw JInternalError::GetInstance(); //("File Header, Magic Number field error");

	// No compression
	if(m_FileHeader.GetCompression () != 0)
		throw JInternalError::GetInstance(); //("File Header, Compression field error");

	// No cryptage
	if(m_FileHeader.GetCryptage () != 0)
		throw JInternalError::GetInstance(); //("File Header, Cryptage field error");

	// Must not be greater than my current version
	if(m_FileHeader.GetVersion () > CURRENT_VERSION)
		throw JInternalError::GetInstance(); //("File Header, Version field error");
}


JVoid CEnquete::LoadHeader(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Load Source and Terrain ID's
			pFile->Recv(m_lSourceID);
			pFile->Recv(m_lTerrainID);
		}
		break;

		default:
			TRACE("Enquete Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CEnquete::LoadModalites(JArchive *pFile, JInt32 Version)
{
	CString Text;
	switch(Version)
	{
		case 1:
		{
			// Get number of modalités
			JInt32 Count;
			pFile->Recv(Count);

			for(JInt32 x=0; x < Count; x++)
			{
				PTR_MODALITE pItem(new CModalite);

				pItem->Recv(*pFile, Version);

				/* Test pour longueur libellé / A VIRER
				Text ="Télévision connectée / Smart TV (avec un accès Internet intégré, qui permet de visualiser sur l'écran toute une série d'applications (actualité, jeux, services pratiques comme la météo, service photo comme Picasa, vidéo en ligne comme YouTube ou DailyMotion, etc.) et en particulier de naviguer sur Internet via un navigateur comme sur un ordinateur)";
				// Text.Format("%s",pItem->GetLabel().AsJCharPtr());
				int Taille = Text.GetLength();
				if (MaxTaille < Taille)
					MaxTaille = Taille;
				*/

				// Add modalité to the set
				//TRACE("Modalite ID %d\n",pItem->GetID());
				m_mModalites.Add(pItem->GetID()) = PTR_IMODALITE(pItem.release()->AsIModalite());
			}
		}
		break;

		default:
			TRACE("Enquete Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CEnquete::LoadElements(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of elements
			JInt32 Count;
			pFile->Recv(Count);

			for(JInt32 x=0; x < Count; x++)
			{
				PTR_ELEMENT pItem(new CElement);

				pItem->Recv(*pFile, Version);

				// Add elements to the set
				//TRACE("Element ID %d\n",pItem->GetID());
				m_mElements.Add(pItem->GetID()) = PTR_IMODALITE(pItem.release()->AsIModalite());
			}
		}
		break;

		default:
			TRACE("Enquete Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}


JVoid CEnquete::LoadIndexThematique(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			m_pIndexThematique = new CIndexThematique();
			m_pIndexThematique->Recv(*pFile, Version);
		}
		break;

		default:
			TRACE("Enquete Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CEnquete::LoadQuestions(JArchive *pFile, JInt32 Version, CSecurity * pSecurity)
{
	m_Questions.SetData (&m_mElements, &m_mModalites, pSecurity);
	m_Questions.Recv(*pFile, Version); 
}

JBool	CEnquete::FillIndexThematique(CTerrain *pTerrain, CTreeCtrl &Tree,HTREEITEM hParent, JBool AllLeaves, JBool OnlyQuanti)
{
	return m_pIndexThematique->FillTree(pTerrain, Tree, hParent, AllLeaves, OnlyQuanti);
}

void	CEnquete::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, JBool OnlyQuanti) const
{
	m_Questions.GetQuestionsByBlocID(BlocID, List, OnlyQuanti);
}

void	CEnquete::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, JBool OnlyQuanti)  const
{
	CString xFilter = sFilter;
	Convert::ToSearchString(xFilter);

	if(xFilter.IsEmpty())
		m_Questions.GetQuestionsByBlocID(BlocID, List, OnlyQuanti);
	else
		m_Questions.GetQuestionsByBlocID(BlocID, List, xFilter, OnlyQuanti);
}

void	CEnquete::GetQuestionsByBlocID	(JInt32 BlocID, JList<IQuestion const *> &List, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti)  const
{
	m_Questions.GetQuestionsByBlocID(BlocID, List, ElementMap, ModalityMap, OnlyQuanti);
}

void	CEnquete::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> & List, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti)  const
{
	CString xFilter = sFilter;
	Convert::ToSearchString(xFilter);

	if(xFilter.IsEmpty())
		GetQuestionsByBlocID(BlocID, List, ElementMap, ModalityMap, OnlyQuanti);
	else
		m_Questions.GetQuestionsByBlocID(BlocID, List, xFilter, ElementMap, ModalityMap, OnlyQuanti);
}

JBool	CEnquete::HasQuestionsByBlocID(JInt32 BlocID) const
{
	return m_Questions.HasQuestionsByBlocID(BlocID);
}

JBool	CEnquete::HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter)  const
{
	CString xFilter = sFilter;
	Convert::ToSearchString(xFilter);

	if(xFilter.IsEmpty())
		return m_Questions.HasQuestionsByBlocID(BlocID);
	else
		return m_Questions.HasQuestionsByBlocID(BlocID, xFilter);
}

JBool	CEnquete::HasQuestionsByBlocID	(JInt32 BlocID, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const
{
	return m_Questions.HasQuestionsByBlocID(BlocID, ElementMap, ModalityMap);
}

JBool	CEnquete::HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const
{
	CString xFilter = sFilter;
	Convert::ToSearchString(xFilter);

	if(xFilter.IsEmpty())
		return HasQuestionsByBlocID(BlocID, ElementMap, ModalityMap);
	else
		return m_Questions.HasQuestionsByBlocID(BlocID, xFilter, ElementMap, ModalityMap);
}

bool CEnquete::FindElements(JMap<JInt32,bool> & ElementMap, const CString & sFilter)
{
	CString xFilter = sFilter;
	Convert::ToSearchString(xFilter);

	if(xFilter.IsEmpty())
	{
		for(m_mElements.MoveFirst(); m_mElements.IsValid(); m_mElements.MoveNext())
			ElementMap.Add(m_mElements.GetItem()->GetID()) = true;
	}
	else
	{
		for(m_mElements.MoveFirst(); m_mElements.IsValid(); m_mElements.MoveNext())
		{
			CString xString = Convert::ToSearchString(m_mElements.GetItem()->GetLabel().AsJCharPtr());
			if(xString.Find(xFilter) != -1)
				ElementMap.Add(m_mElements.GetItem()->GetID()) = true;
		}
	}

	return (ElementMap.GetCount() > 0);
}

bool CEnquete::FindModalities(JMap<JInt32,bool> & ModalityMap, const CString & sFilter)
{
	CString xFilter = sFilter;
	Convert::ToSearchString(xFilter);

	if(xFilter.IsEmpty())
	{
		for(m_mModalites.MoveFirst(); m_mModalites.IsValid(); m_mModalites.MoveNext())
			ModalityMap.Add(m_mModalites.GetItem()->GetID()) = true;
	}
	else
	{
		for(m_mModalites.MoveFirst(); m_mModalites.IsValid(); m_mModalites.MoveNext())
		{
			CString xString = Convert::ToSearchString(m_mModalites.GetItem()->GetLabel().AsJCharPtr());
			if(xString.Find(xFilter) != -1)
				ModalityMap.Add(m_mModalites.GetItem()->GetID()) = true;
		}
	}
	return (ModalityMap.GetCount() > 0);
}

// Returns the question corresponding to the questio ID
CQuestion const *	CEnquete::GetQuestionByID(JInt32 QuestionID) const
{
	return m_Questions.GetQuestionByID(QuestionID);
}

// Returns the Item Natif by the given question id and termes
JInt32	CEnquete::GetItemNatifByTermes(JInt32 QuestionID, JVector<JInt32> &Termes) const
{
	return m_Questions.GetItemNatifByTermes(QuestionID, Termes);
}

// Gets the Numeric variable Numver for the given ItemNatif
// This must only be called if the question is a variable quanti
// Return -1 if not found
JInt32	CEnquete::GetVarAgrIdByItemNatif(JInt32 ItemNatif) const
{
	return m_Questions.GetVarAgrIdByItemNatif(ItemNatif);
}

// Gets the Aggregat variable Index for the given ItemNatif
// This must only be called if the question is a variable quanti
JBool	CEnquete::GetVarIdxByItemNatif(JInt32 ItemNatif, JInt32 & Index) const
{
	return m_Questions.GetVariableIndexByItemNatif (ItemNatif,Index);
}

// Gets the Question ID for the given ItemNatif
// Return 0 if not found
JInt32	CEnquete::GetQuestionIDByItemNatif(JInt32 ItemNatif) const
{
	return m_Questions.GetQuestionIDByItemNatif(ItemNatif);
}

// Returns the question corresponding to the ItemNatif
const CQuestion  *	CEnquete::GetQuestionByItemNatif(JInt32 ItemNatif) const
{
	return m_Questions.GetQuestionByItemNatif(ItemNatif);
}

//Teste si un element existe
JBool CEnquete::HasElement(JInt32 elemID) const
{
	m_mElements.MoveTo(elemID);
	if (m_mElements.IsValid())
		return m_mElements.GetItem()->IsElement();
	return false;
}

//Teste si un modalite existe
JBool CEnquete::HasModalite(JInt32 modaID) const
{
	m_mModalites.MoveTo(modaID);
	if (m_mModalites.IsValid())
		return m_mModalites.GetItem()->IsModalite();
	return false;
}

//Teste si un element existe
const CElement * CEnquete::GetElement(JInt32 elemID) const
{
	m_mElements.MoveTo(elemID);
	if (m_mElements.IsValid())
		return m_mElements.GetItem()->AsElement();
	return 0;
}

//Teste si un modalite existe
const CModalite * CEnquete::GetModalite(JInt32 modaID) const
{
	m_mModalites.MoveTo(modaID);
	if (m_mModalites.IsValid())
		return m_mModalites.GetItem()->AsModalite();
	return false;
}

//Teste si un support existe dans le terrain
JBool CEnquete::HasSupport(JInt32 elemID) const
{
	JBool bret = false;
	m_mElements.MoveTo(elemID);
	if (m_mElements.IsValid())
		bret = m_mElements.GetItem()->IsSupport();
	return bret;
}
