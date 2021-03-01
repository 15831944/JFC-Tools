
#pragma once

#include "itemconstruit.h"

class AFX_EXT_CLASS CItemWrapper
{
public:
	CItemWrapper(void);
	virtual ~CItemWrapper(void);
	virtual CString GetHeader() const { return m_Header; } ;
	virtual CString GetLabel() const { return m_Label; };

	// Récupére categorie de l'élément
	virtual JInt32 CItemWrapper::GetCategorie() const {return m_Categorie; };

	virtual void SetLabel(CString & Label) { m_Label = Label; };
	virtual void SetHeader(CString & Header) { if(!Header.IsEmpty())
												m_Header = Header; };
	virtual void SetItemConstruit(CItemConstruit *pItem);

	virtual void SetCategorie(JInt32 Categorie) {m_Categorie = Categorie; };

	virtual CItemConstruit * PtrItem() const { return m_pItem; };

	virtual bool Read(CStdioFile & FileIn, JMap<JInt32,CItemWrapper *> & WrapperMap, JMap<JInt32, const IItem *> & ImpMap);
	virtual bool Write(CStdioFile & FileOut, JMap<JInt32,JBool> & ExpMap);

protected:
	CString				m_Header;
	CString				m_Label;
	CItemConstruit *	m_pItem;
	JInt32				m_Categorie;
};

typedef JList<CItemWrapper *>		LIST_ITEM_PUPITRE;
typedef JList<const CItemWrapper *> CONST_LIST_ITEM_PUPITRE;
