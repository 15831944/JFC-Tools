// Question.cpp: implementation of the CQuestion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Question.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestion::CQuestion(IMODALITEMAP * pElements, IMODALITEMAP * pModalites, CSecurity *pSecurity)
:m_pElements(pElements), m_pModalites(pModalites), m_pSecurity(pSecurity)
{ 
	m_ID			= 0;
	m_lBlocID		= 0;
	m_bContingent	= 0xFFFF;
	m_lCategorie	= 0;
	m_Label			= "";
	m_lNbTermes		= 0;
	m_ColItemNatif	= 0;
	m_ColVarNum		= 0;
	m_bEchantillon	= 0;;
};


CQuestion::~CQuestion()
{
}

JArchive& CQuestion::Recv(JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			lhs.Recv(m_lBlocID);
			lhs.Recv(m_bContingent);
			lhs.Recv(m_bEchantillon);
			lhs.Recv(m_lCategorie);
			m_Label.Recv(lhs);

			JInt32 NbDim = 0;
			lhs.Recv(NbDim);

			// TRACE("Question %d, %s, NbDims %d, ", m_ID,m_Label.AsJCharPtr(), NbDim);
			for(JInt32 Dim = 0; Dim < NbDim; ++Dim)
			{
				JInt32 DimId; lhs.Recv(DimId);
				// TRACE(" %d,", DimId);
				m_DimIds.AddTail() = DimId;
				JInt32 Nature = 0; lhs.Recv(Nature);
				if(Nature == 0)
					m_DimType.AddTail() = IQuestion::ELEMENT;
				else
					m_DimType.AddTail() = IQuestion::MODALITE;

				JInt32 Count = 0; lhs.Recv(Count);
				JINT32VECTOR & data = m_DimModaElemIds.AddTail();
				data.SetCount(Count);
				for(data.MoveFirst(); data.IsValid(); data.MoveNext())
					lhs.Recv(data.GetItem());
			}
			//TRACE("\n");

			lhs.Recv(m_lNbTermes);

			// The number of termes must be a multiple of dimensions elements + modalities
			if(m_lNbTermes%GetKeySize())
			{
				//TRACE("Question %d, invalid number of termes",m_ID);
				throw JInternalError::GetInstance(); //("Question , invalid number of termes");
			}

			// Get the number of items to read
			JInt32 NbItems = GetNbItems();

			// Set the Item Natif column
			m_ColItemNatif = GetKeySize();
			// Set the variable index column
			m_ColVarNum  = m_ColItemNatif;

			if(IsAgregatDataType() || IsNumericDataType())
				m_ColVarNum++;

			// TRACE("Question %d, NbTermes %d, Terme Size %d\n", m_ID,NbItems,m_ColVarNum+1);

			// Reserve memory for the matrix of ColItemNatif+1
			m_xItemKey.SetCount(NbItems,m_ColVarNum+1);

			for(JInt32 r = 0; r < NbItems; r++)
			{
//				TRACE("Row %4d, ", r);
				JInt32 c = 0;
				for(c = 0; c < m_ColVarNum+1; c++)
				{
					JInt32 & Col = m_xItemKey.Item(r,c);
					lhs.Recv(Col);
//					TRACE("%6d, ", Col);
				}
//				TRACE("\n");
				// Set Item Natif index
				if(r==0)
					m_ItemStart = m_xItemKey.Item(r,m_ColItemNatif);

				if(!m_pSecurity->IsValidItemNatif(m_xItemKey.Item(r,m_ColItemNatif))) 
				{
					// TRACE("Question %d, Terme %d, Item %d Removed\n", m_ID,r,m_xItemKey.Item(r,m_ColItemNatif));
					m_xItemKey.Item(r,m_ColItemNatif) = -1;
				}
			}
//			m_ItemStart = m_xItemKey.Item(0,m_ColItemNatif);

			if(!VerifyKeys())
			{
				//TRACE("Question %d Invalid Modalities or Elements\n", m_ID);
				throw JInternalError::GetInstance(); //("Invalid Modalities or Elements");
			}
			// TRACE("Question %d variable Type is %d\n", m_ID, m_lCategorie);
		}
		break;

		default:
			//TRACE("CQuestion Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

// Agregat type variable
JBool	CQuestion::IsAgregatDataType() const
{
	if(m_lCategorie >= Variable_Etalonage_D && m_lCategorie <= Variable_Etalonage_F)
		return true;

	if(m_lCategorie >= Univers_Appariement_D  && m_lCategorie <= Univers_Appariement_F)
		return true;

	if(m_lCategorie >= Segments_Appariement_D && m_lCategorie <= Segments_Appariement_F)
		return true;

	if(m_lCategorie >= Variables_Transfert_D  && m_lCategorie <= Variables_Transfert_F)
		return true;

	if(m_lCategorie >= Quanta_Freq_D  && m_lCategorie <= Quanta_Freq_F)
		return true;

	switch(m_lCategorie)
	{
		case LDP:
		case LNM:
		case Probabilite_LDP:
		case Probabilite_LNM:
		case Poids_QH:
		case Probabilite_Expo:
		case Quanta_Freq:
		case Quanta_Freq_Brand:
		case Quanta_Freq_Channel:
		case Quanta_Freq_Rollup:
			return true;

		default: return false;
	}
}

// Numeric type variable
JBool	CQuestion::IsNumericDataType() const
{
	switch(m_lCategorie)
	{
		case Quantitative_Pure:
		case Quantitative_Assim:
			return true;

		default: return false;
	}
}

// Has Values
JBool	CQuestion::HasValues() const
{
	if(m_lCategorie >= Quanta_Freq_D  && m_lCategorie <= Quanta_Freq_F)
		return true;

	switch(m_lCategorie)
	{
		case Quantitative_Pure:
		case Quantitative_Assim:
		case Quanta_Freq:
		case Quanta_Freq_Brand:
		case Quanta_Freq_Channel:
		case Quanta_Freq_Rollup:
			return true;

		default: return false;
	}
}

// Is Probability
JBool	CQuestion::IsProbability() const
{
	if(m_lCategorie >= Variable_Etalonage_D && m_lCategorie <= Variable_Etalonage_F)
		return true;

	if(m_lCategorie >= Univers_Appariement_D  && m_lCategorie <= Univers_Appariement_F)
		return true;

	if(m_lCategorie >= Segments_Appariement_D && m_lCategorie <= Segments_Appariement_F)
		return true;

	// *** PROBLEME *** Octobre 2006, Attention ce ne doit pas être une proba!!!!!
	if(m_lCategorie >= Variables_Transfert_D  && m_lCategorie <= Variables_Transfert_F)
	{
		return true;
	}

	switch(m_lCategorie)
	{
		case LDP:
		case LNM:
		case Probabilite_LDP:
		case Probabilite_LNM:
		case Poids_QH:
		case Probabilite_Expo:
			return true;

		default: return false;
	}
}

/// Returns the dimension type
JInt32 CQuestion::DimensionType(JInt32 Dim)	const
{
	m_DimType.MoveTo(Dim);
	if(!m_DimType.IsValid())
		throw JInternalError::GetInstance();
	return m_DimType.GetItem(); 
}

/// Returns si question sera automatiquement moyennable (ssi toutes les dimensions de type 0)
JBool CQuestion:: AutoMoyennable() const
{
	// Nombre de dimensions
	JUnt32 NbDimension = m_DimType.GetCount();

	/*
	// Boucle sur toutes les dimensions de la question
	for (JUnt32 InxDim = 0; InxDim < m_DimType.GetCount(); InxDim++)
	{
		if (DimensionType(InxDim) == TypePresse)
		{
			// La question a des dimensions de type presse, on ne les prend pas en compte d'une manière automatique
			// pour créer les items moyennables automatiques
			return false;
		}
		else
		{
			JUnt32 Type = DimensionType(InxDim);
			JBool Ok = true;
		}
	}
	*/

	// if (NbDimension > 1) return false;

	return this->IsQuantitative();
}

JBool	CQuestion::IsQuantitative() const
{
	switch(m_lCategorie)
	{
		case Quantitative_Pure:
		case Quantitative_Assim:
			return true;

		default: return false;
	}
}

JBool	CQuestion::IsQualitative() const
{
	switch(m_lCategorie)
	{

		case Qualitative_Simple:
		case Qualitative_Multiple:
			return true;

		default: return false;
	}
}

///  gets the IModalite object for the given dimension and ID
const IModalite * CQuestion::GetIModalite(JInt32 ModaliteID, JInt32 Dim)	const
{
	if(!GetKeySize())
		throw JInternalError::GetInstance();

	const IMODALITEMAP * pMap = 0;

	switch(DimensionType(Dim))
	{
		case IQuestion::MODALITE:
			pMap = m_pModalites; break;
	
		case IQuestion::ELEMENT:
			pMap = m_pElements; break;

		default:
			throw JInternalError::GetInstance();
	}

	pMap->MoveTo(ModaliteID);
	if(pMap->IsValid())
		return pMap->GetItem()->AsIModalite();

	return 0;
}

///	Returns true is the questions has any of the modalities in the given map
JBool	CQuestion::HasModality(const JMap<JInt32,bool> & ModalityMap) const
{
	if(ModalityMap.GetCount() == 0)
		return false;

	for(m_DimType.MoveFirst(); m_DimType.IsValid() ; m_DimType.MoveNext())
	{
		if(m_DimType.GetItem() != IQuestion::MODALITE)
			continue;
		m_DimModaElemIds.MoveTo(m_DimType.GetIndex());
		if(m_DimModaElemIds.IsValid())
		{
			const JINT32VECTOR & data = m_DimModaElemIds.GetItem();
			for(data.MoveFirst(); data.IsValid(); data.MoveNext())
			{
				ModalityMap.MoveTo(data.GetItem());
				if(ModalityMap.IsValid())
					return true;
			}
		}
	}

	return false;
}

///	Returns true is the questions has any of the elements in the given map
JBool	CQuestion::HasElement(const JMap<JInt32,bool> & ElementMap) const
{
	if(ElementMap.GetCount() == 0)
		return false;

	for(m_DimType.MoveFirst(); m_DimType.IsValid() ; m_DimType.MoveNext())
	{
		if(m_DimType.GetItem() != IQuestion::ELEMENT)
			continue;
		m_DimModaElemIds.MoveTo(m_DimType.GetIndex());
		if(m_DimModaElemIds.IsValid())
		{
			const JINT32VECTOR & data = m_DimModaElemIds.GetItem();
			for(data.MoveFirst(); data.IsValid(); data.MoveNext())
			{
				ElementMap.MoveTo(data.GetItem());
				if(ElementMap.IsValid())
					return true;
			}
		}
	}

	return false;
}

JBool	CQuestion::GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID, JInt32 Dim, JUnt32 ContingentMask) const
{
	if(!GetKeySize())
		throw JInternalError::GetInstance();

	const IMODALITEMAP * pMap = 0;

	switch(DimensionType(Dim))
	{
		case IQuestion::MODALITE:
			pMap = m_pModalites; break;
	
		case IQuestion::ELEMENT:
			pMap = m_pElements; break;

		default:
			throw JInternalError::GetInstance();
	}

	m_DimModaElemIds.MoveTo(Dim);
	if(m_DimModaElemIds.IsValid())
	{
		const JINT32VECTOR & data = m_DimModaElemIds.GetItem();
		JList<IModalite *> stock;
		for(data.MoveFirst(); data.IsValid(); data.MoveNext())
		{
			pMap->MoveTo(data.GetItem());
			if(pMap->IsValid() && pMap->GetItem()->IsInContingent(ContingentMask))
				stock.AddTail() = pMap->GetItem()->AsIModalite();
		}
		if(stock.GetCount())
		{
			Vector.SetCount(stock.GetCount());
			for(stock.MoveFirst(), Vector.MoveFirst(); stock.IsValid(); stock.MoveNext(), Vector.MoveNext())
				Vector.GetItem() = stock.GetItem();
		}
	}
	return Vector.GetCount() > 0;
}

JBool	CQuestion::IsValidTerme(const JVector<JInt32> & IDTerme)	const
{
	if(GetTermeIndex(IDTerme)==-1)
		return false;
	return true;
}

JBool	CQuestion::ToItemString(JStringEx &String) const
{
	JChar t[20];
	_itoa_s(m_ID,t,20,10);
	String = "Q ";
	String.Append(t);
	return true;
}

// Verifies that the modalities and elements exist
JBool CQuestion::VerifyKeys()
{
	int NbDim = GetKeySize();
	for(JInt32 Dim = 0; Dim < GetKeySize() ; Dim++)
	{
		const IMODALITEMAP * pMap = 0;

		switch(DimensionType(Dim))
		{
			case IQuestion::MODALITE:
				pMap = m_pModalites; break;
		
			case IQuestion::ELEMENT:
				pMap = m_pElements; break;

			default:
				throw JInternalError::GetInstance();
		}

		m_DimModaElemIds.MoveTo(Dim);
		if(m_DimModaElemIds.IsValid())
		{
			JList<JInt32> Copy;
			JINT32VECTOR & Data = m_DimModaElemIds.GetItem(); 
	
			for(Data.MoveFirst(); Data.IsValid(); Data.MoveNext())
			{
				JInt32 Id = Data.GetItem();
				JInt32 Count = 0;	

				for(JInt32 Row = 0; Row < m_xItemKey.GetRowCount(); Row++)
				{
					if(m_xItemKey.Item(Row,m_ColItemNatif)==-1)
						continue;

					JInt32 nId = m_xItemKey.Item(Row,Dim);
					if(nId != Id)
					{
						pMap->MoveTo(nId);
						if(!pMap->IsValid())
						{
							switch(DimensionType(Dim))
							{
								case IQuestion::MODALITE:
									//TRACE("QUID %d has Invalid Modalité : %d\n",m_ID, nId);
								break;
							
								case IQuestion::ELEMENT:
									//TRACE("QUID %d has Invalid Element : %d\n",m_ID, nId);
								break;
							}
							return false;
						}
						continue;
					}
					Count++;
				}
				if(Count)
					Copy.AddTail() = Id;
			}
			if(Copy.GetCount() != Data.GetCount())
			{
				JINT32VECTOR v;
				v.SetCount(Copy.GetCount());
				for(v.MoveFirst(), Copy.MoveFirst(); v.IsValid(); v.MoveNext(), Copy.MoveNext())
					v.GetItem() = Copy.GetItem();
				Data.Swap(v);
			}
		}
	}
	return true;
}

JBool CQuestion::IsValidQuestion() const
{
	for(JInt32 Row = 0; Row < m_xItemKey.GetRowCount(); Row++)
	{
		if(m_xItemKey.Item(Row,m_ColItemNatif) != -1)
			return true;
	}
	return false;

}

// returns the row index of a given terme
JInt32	CQuestion::GetTermeIndex(const JVector<JInt32> & Terme)	const
{
	if(Terme.GetCount() == GetKeySize())
	{
		for(JInt32 Row = 0; Row < m_xItemKey.GetRowCount(); Row++)
		{
			JInt32 Col = 0;
			JBool Found = true;
			
			if(m_xItemKey.Item(Row,m_ColItemNatif)==-1)
				continue;

			for(Terme.MoveFirst(); Terme.IsValid(); Terme.MoveNext(), Col++)
			{
				if(m_xItemKey.Item(Row,Col) != Terme.GetItem())
				{
					Found = false;
					break;
				}
			}
			if(Found)
				return Row;
		}
	}
	return -1;
}

JBool	CQuestion::GetTermeVector(JVector<JInt32> &Terme, JInt32 Index) const
{
	if(Index >= m_xItemKey.GetRowCount() || m_xItemKey.Item(Index,m_ColItemNatif)==-1)
		return false;

	Terme.SetCount(GetKeySize());
	JInt32 Col=0;
	for(Col=0, Terme.MoveFirst(); Col < GetKeySize(); Col++, Terme.MoveNext())
	{
		Terme.GetItem() = m_xItemKey.Item(Index,Col);
	}
	return true;
}

// Returns the Item Natif of the given terme
JInt32	CQuestion::GetItemNatifByTerme(const JVector<JInt32> & IDTerme)	const
{
	JInt32 RowIndex = GetTermeIndex(IDTerme);
	if(RowIndex == -1)
	{
		//TRACE("GetVariableNum Terme not valid");
		return -1;
	}
	return m_xItemKey.Item(RowIndex,m_ColItemNatif);
}

// Gets the Numeric variable index for the given terme index
// This must only be called if the question is a variable quanti
JInt32	CQuestion::GetVariableIndexByIndex(JInt32 Index) const
{ 
	if(IsNumericDataType() || IsAgregatDataType())
	{
		if(Index < 0 || Index >= m_xItemKey.GetRowCount())
			return -1;

		if(m_xItemKey.Item(Index,m_ColItemNatif)!=-1)
			return m_xItemKey.Item(Index,m_ColVarNum);
		else
			return -1;
	}
	else
	{
		//TRACE("GetVariableNumByIndex called when question is not a quanti");
	}
	throw JInternalError::GetInstance();
}

// Gets the Numeric variable index for the given terme
// This must only be called if the question is a variable quanti
JInt32	CQuestion::GetVariableIndexByTerme(const JVector<JInt32> & Terme) const
{ 
	if(IsNumericDataType() || IsAgregatDataType())
	{
		JInt32 RowIndex = GetTermeIndex(Terme);
		if(RowIndex == -1)
		{
			//TRACE("GetVariableNumByTerme Terme not valid");
			return -1;
		}
		return m_xItemKey.Item(RowIndex,m_ColVarNum);
	}
	else
	{
		//TRACE("GetVariableNum called when question is not a quanti");
	}

	throw JInternalError::GetInstance();
}

// Returns true is this is a valid term index
JBool	CQuestion::IsValidItem(JInt32 Index) const
{
	if(Index < 0 || Index >= m_xItemKey.GetRowCount())
		return false;
	if(m_xItemKey.Item(Index,m_ColItemNatif)==-1)
		return false;
	else
		return true;
}

JInt32	CQuestion::GetVarAgregatId() const
{
	if(IsAgregatDataType())
		return m_lCategorie;
	return 0;
}

JInt32 CQuestion::GetSupportID(JInt32 RowIndex) const
{
	// Check all elements
	if(m_xItemKey.Item(RowIndex,m_ColItemNatif)==-1)
		return -1;

	for(JInt32 Col = 0; Col < GetKeySize() ; Col++)
	{
		const IMODALITEMAP * pMap = 0;

		switch(DimensionType(Col))
		{
			case IQuestion::MODALITE:
				pMap = m_pModalites; break;
		
			case IQuestion::ELEMENT:
				pMap = m_pElements; break;

			default:
				throw JInternalError::GetInstance();
		}

		pMap->MoveTo(m_xItemKey.Item(RowIndex,Col));
		if(pMap->IsValid() && pMap->GetItem()->IsSupport())
			return pMap->GetItem()->GetID();
	}
	return -1;
}