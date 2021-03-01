#include "StdAfx.h"
#include "palettebuilder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPaletteBuilder::CPaletteBuilder()
{
	m_pIBitem = 0;
	m_pIBquestion = 0;
}

CPaletteBuilder::~CPaletteBuilder()
{
}

CPaletteBuilder::CPaletteBuilder(const CPaletteBuilder &rhs)
{
	*this = rhs;	
}

CPaletteBuilder & CPaletteBuilder::operator=(const CPaletteBuilder &rhs)
{
	m_pIBitem			=	rhs.m_pIBitem;
	m_pIBquestion		=	rhs.m_pIBquestion;
	m_DefaultLabel		=	rhs.m_DefaultLabel;
	m_Label				=	rhs.m_Label;

	m_Map_pIBModalite.Reset();
	for(rhs.m_Map_pIBModalite.MoveFirst(); rhs.m_Map_pIBModalite.IsValid(); rhs.m_Map_pIBModalite.MoveNext())
		m_Map_pIBModalite.Add(rhs.m_Map_pIBModalite.GetKey()) =   rhs.m_Map_pIBModalite.GetItem();

	m_Map_DimType.Reset();
	for(rhs.m_Map_DimType.MoveFirst(); rhs.m_Map_DimType.IsValid(); rhs.m_Map_DimType.MoveNext())
		m_Map_DimType.Add(rhs.m_Map_DimType.GetKey()) =   rhs.m_Map_DimType.GetItem();

	m_Map_pIModalite.Reset();
	for(rhs.m_Map_pIModalite.MoveFirst(); rhs.m_Map_pIModalite.IsValid(); rhs.m_Map_pIModalite.MoveNext())
	{
		JList<const IModalite *> & dst = m_Map_pIModalite.Add(rhs.m_Map_pIModalite.GetKey());
		const JList<const IModalite *> & src = rhs.m_Map_pIModalite.GetItem();
		for(src.MoveFirst(); src.IsValid(); src.MoveNext())
			dst.AddTail() = src.GetItem();
	}

	m_List_DimSel.Reset();
	for(rhs.m_List_DimSel.MoveFirst(); rhs.m_List_DimSel.IsValid(); rhs.m_List_DimSel.MoveNext())
		m_List_DimSel.AddTail() =   rhs.m_List_DimSel.GetItem();

	m_List_DimMode.Reset();
	for(rhs.m_List_DimMode.MoveFirst(); rhs.m_List_DimMode.IsValid(); rhs.m_List_DimMode.MoveNext())
		m_List_DimMode.AddTail() =   rhs.m_List_DimMode.GetItem();

	m_List_NumValue.Reset();
	for(rhs.m_List_NumValue.MoveFirst(); rhs.m_List_NumValue.IsValid(); rhs.m_List_NumValue.MoveNext())
		m_List_NumValue.AddTail() = rhs.m_List_NumValue.GetItem();

	m_Map_DimSel.Reset();
	for(rhs.m_Map_DimSel.MoveFirst(); rhs.m_Map_DimSel.IsValid(); rhs.m_Map_DimSel.MoveNext())
		m_Map_DimSel.Add(rhs.m_Map_DimSel.GetKey()) =   rhs.m_Map_DimSel.GetItem();
	
	return *this;
}

void CPaletteBuilder::Reset()
{
	m_pIBitem			= 0;
	m_pIBquestion		= 0;
	m_Map_pIBModalite.	Reset();
	m_Map_DimType.		Reset();  
	m_Map_pIModalite.	Reset();
	m_List_DimSel.		Reset();
	m_List_DimMode.		Reset();
	m_List_NumValue.	Reset();
	m_Map_DimSel.		Reset();
	m_DefaultLabel.		Empty();
	m_Label.			Empty();
}

void CPaletteBuilder::Swap(CPaletteBuilder & rhs)
{
	m_pIBitem			=	rhs.m_pIBitem;
	m_pIBquestion		=	rhs.m_pIBquestion;
	m_DefaultLabel		=	rhs.m_DefaultLabel;
	m_Label				=	rhs.m_Label;
	m_Map_pIBModalite.	Swap(rhs.m_Map_pIBModalite);
	m_Map_DimType.		Swap(rhs.m_Map_DimType);  
	m_Map_pIModalite.	Swap(rhs.m_Map_pIModalite);
	m_List_DimSel.		Swap(rhs.m_List_DimSel);
	m_List_DimMode.		Swap(rhs.m_List_DimMode);
	m_List_NumValue.	Swap(rhs.m_List_NumValue);
	m_Map_DimSel.		Swap(rhs.m_Map_DimSel);
}

void CPaletteBuilder::ResetDimSelection()
{
	m_List_DimSel.	Reset();
	m_List_DimMode.	Reset();
	m_Map_DimSel.	Reset();
}

void CPaletteBuilder::SetDimSelection(int Index, bool Add, int Mode)
{
	if(Add)
	{
		m_Map_DimSel.MoveTo(Index);
		if(!m_Map_DimSel.IsValid())
		{
			m_List_DimSel.AddTail(true) = Index;
			m_List_DimMode.AddTail(true) = Mode;
		}
	}
	else
	{
		m_Map_DimSel.MoveTo(Index);
		if(m_Map_DimSel.IsValid())
		{
			m_List_DimSel.MoveTo(m_Map_DimSel.GetItem());
			m_List_DimMode.MoveTo(m_Map_DimSel.GetItem());
			if(m_List_DimSel.IsValid())
			{
				m_List_DimSel.Remove();
				m_List_DimMode.Remove();
			}
		}
	}

	m_Map_DimSel.Reset();
	m_List_DimSel.MoveFirst();
	while(m_List_DimSel.IsValid())
	{
		// MAI 2006, Ajout
		m_Map_DimSel.MoveTo(m_List_DimSel.GetItem());
		if (!m_Map_DimSel.IsValid())
			m_Map_DimSel.Add(m_List_DimSel.GetItem()) = m_List_DimSel.GetIndex();

		m_List_DimSel.MoveNext();
	}
}

bool CPaletteBuilder::IsDimensionSelected(int Dim)
{
	m_Map_DimSel.MoveTo(Dim);
	return m_Map_DimSel.IsValid();
}

int	CPaletteBuilder::GetSelectedDimensionIndex(int Dim)
{
	if(IsDimensionSelected(Dim))
	{
		m_Map_DimSel.MoveTo(Dim);
		return m_Map_DimSel.GetItem(); 
	}
	return -1;
}

int	CPaletteBuilder::GetSelectedDimensionMode(int Dim)
{
	if(IsDimensionSelected(Dim))
	{
		m_Map_DimSel.MoveTo(Dim);
		m_List_DimMode.MoveTo(m_Map_DimSel.GetItem());
		return m_List_DimMode.GetItem();
	}
	return 0;
}

void CPaletteBuilder::SetNumValues(const JList<float> & NumValues)
{
	JList<float> List_NumValue;
	for (NumValues.MoveFirst(); NumValues.IsValid(); NumValues.MoveNext()) List_NumValue.AddTail() = NumValues.GetItem();
	m_List_NumValue.Swap(List_NumValue);
}

const JList<float> & CPaletteBuilder::GetNumValues() const
{
	return (m_List_NumValue);
}
