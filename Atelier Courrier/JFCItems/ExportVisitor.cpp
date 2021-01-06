#include "stdafx.h"
#include "Exportvisitor.h"
#include "IBItem.h"

#include "ItemsConstruitsBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Constructeur
CExportVisitor::CExportVisitor(void)
{
	m_pExpMap	=	0;
	m_pFile		=	0;
	m_pItems	=	0;
	m_pArchive  =   0;
}

// Destructeur
CExportVisitor::~CExportVisitor(void)
{
}

// Export avec StdioFile
JBool CExportVisitor::Export(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst,CStdioFile *pFile, JMap<JInt32,JBool> *pExpMap)
{
	m_pFile		=	pFile;
	m_pExpMap	=	pExpMap;
	m_pItems	=	pItemConst;
	if(pBuilder && pFile && pExpMap && pItemConst)
		pBuilder->Accept(*this);
	else
		return false;
	return true;
}

// Export avec CArchive
JBool CExportVisitor::Export(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst, CArchive *pArchive, JMap<JInt32,JBool> *pExpMap)
{
	// VOIR SYLVAIN
	// m_pFile		=	pFile;


	m_pArchive	=   pArchive;
	m_pExpMap	=	pExpMap;
	m_pItems	=	pItemConst;
	if(pBuilder && pArchive && pExpMap && pItemConst)
		pBuilder->Accept(*this);
	else
		return false;
	return true;
}

void CExportVisitor::VisitStart(CIBItem & Item)
{
	JList<JInt32> Values;
	Item.GetValues(Values);

	// Check if stocked item type container is Palette, Classe or Quantile
	if(Item.GetID()!=0)
		// Add the container to the list
		Values.AddHead() = Item.GetID();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		JInt32 ID = Values.GetItem();

		m_pExpMap->MoveTo(ID);
		if(!m_pExpMap->IsValid())
		{
			IItem * pItem = m_pItems->GetIItemByID(ID);
			if(pItem)
			{
				// VOIR SYLVAIN
				// AVANT doncPfile dans certain cas NULL
				// pItem->Export(*m_pFile,*m_pExpMap);

				// Voir avec sylvain
				if (m_pFile)
					pItem->Export(*m_pFile,*m_pExpMap);
				else 
					pItem->Export(*m_pArchive,*m_pExpMap);

			}
		}
	}
}
