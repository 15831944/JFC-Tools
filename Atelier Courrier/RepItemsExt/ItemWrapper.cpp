#include "StdAfx.h"
#include "itemwrapper.h"
#include "ItemsConstruitsManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CItemWrapper::CItemWrapper(void)
{
	m_Header	= _T("");
	m_pItem		=	0;

	// Init categorie de l'ite
	m_Categorie =   0;
}

CItemWrapper::~CItemWrapper(void)
{
	if(m_pItem && !m_pItem->IsRegistered())
		delete m_pItem;
}

void CItemWrapper::SetItemConstruit(CItemConstruit *pItem)
{
	m_pItem = pItem;
	if(pItem)
		m_Label = pItem->GetLabel().AsJCharPtr();
	else
		m_Label = _T("?");
}

bool CItemWrapper::Read(CStdioFile &FileIn, JMap<JInt32,CItemWrapper *> & WrapperMap, JMap<JInt32, const IItem *> & ImpMap)
{
	int Version = 0;
	FileIn.Read(&Version, sizeof(Version));
	switch(Version)
	{
		case 1:
		{
			// Lecture header & 
			FileIn.ReadString(m_Header);
			FileIn.ReadString(m_Label);

			JInt32 Id = 0;
			FileIn.Read(&Id, sizeof(Id));
			// if(!CItemsConstruits::GetInstance()->Import(FileIn,ImpMap))
			if(!CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Import(FileIn,ImpMap))
				return false;

			ImpMap.MoveTo(Id);
			if(ImpMap.IsValid())
			{
				m_pItem = (CItemConstruit *) ImpMap.GetItem();
				WrapperMap.Add(Id) = this;
				return true;
			}
		}
		break;

		// A partir de la version 2 on récupère la catégorie de l'item
		case 2:
		{
			// Lecture header
			FileIn.ReadString(m_Header);

			// Lecture label
			FileIn.ReadString(m_Label);

			// Lecture catégorie
			m_Categorie = 0;
			FileIn.Read(&m_Categorie, sizeof(m_Categorie));

			// Lecture ident item et de sa composition
			JInt32 Id = 0;
			FileIn.Read(&Id, sizeof(Id));
			// if(!CItemsConstruits::GetInstance()->Import(FileIn,ImpMap))
			if(!CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Import(FileIn,ImpMap))
				return false;

			ImpMap.MoveTo(Id);
			if(ImpMap.IsValid())
			{
				m_pItem = (CItemConstruit *) ImpMap.GetItem();
				WrapperMap.Add(Id) = this;
				return true;
			}
		}
		break;

	}
	return false;
}

bool CItemWrapper::Write(CStdioFile &FileOut, JMap<JInt32,JBool> & ExpMap)
{
	// int Version = 1;

	// Passe en version 2, sauvegarde de la catégorie item  (pour le moment pour détecter les items quantitatifs)
	int Version = 2;

	if(m_pItem)
	{
		// Ecriture n° de la version
		FileOut.Write(&Version,sizeof(Version));

		// Ecriture de l'header
		FileOut.WriteString(m_Header + "\n");

		// Ecriture du label
		FileOut.WriteString(m_Label + "\n");

		// Ecriture catégorie item
		FileOut.Write(&m_Categorie, sizeof(m_Categorie));

		// Ecriture de l'ident item
		JInt32 Id = m_pItem->GetID();
		if(Id == 0)
			return false;
		FileOut.Write(&Id,sizeof(Id));
		
		// if(CItemsConstruits::GetInstance()->Export(FileOut,m_pItem,ExpMap))
		if(CItemsConstruitsManager::GetInstance()->GetItemsInstance()->Export(FileOut,m_pItem,ExpMap))
			return true;
	}
	return false;
}
