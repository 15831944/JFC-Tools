#include "StdAfx.h"
#include "cellitemmoyenresult.h"
#include "CrossTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCellItemMoyenResult::CCellItemMoyenResult(int Page, int Row, int Col,CCrossTable *pCrossTable,MAPCELLRESULTS  * CellResults)
:m_pCrossTable(pCrossTable), m_pCellResults(CellResults)
{
	m_Coord.SetCoord(Page,Row,Col);  
	m_Effective				=	0.0;
	m_NbCas					=	0.0;
	m_Quantity				=	0.0;
	m_EffectiveBrutNonCible	=	0.0;
	m_Calculated			=	0;

	m_Effective_0Exclu		=   0.0;
	m_Quantity_0Exclu		=	0.0;

}

CCellItemMoyenResult::~CCellItemMoyenResult(void)
{
}

double	CCellItemMoyenResult::GetResult(RESULTTYPE Type, const JFLT32VECTOR * pQuanti, bool ModeMoyenne0Exclu, ITEMTYPE ElmtType, bool ModeExclusif)
{
	if(!Calculate(Type, pQuanti))
		return 0.0;

	switch(Type)
	{
		case RTYPE_EFFECTIVE_000:
			return m_Effective/1000.0;

		case RTYPE_NBCAS:
			return m_NbCas;

		case RTYPE_QUANTITY_000:
			// return m_Quantity;

			// Attention au mode de calcul quantity, avec ou sans les 0
			if (ModeMoyenne0Exclu == false)
				return m_Quantity;
			else
				return m_Quantity_0Exclu/1000.0;
			break;

		case RTYPE_AVERAGEQUANTUM:
		case RTYPE_AVERAGEQUANTUM_P00:
			/*
			if(m_Effective!=0.0)
				return (m_Quantity * 1000.0)/m_Effective;
			*/
			// Attention au mode de calcul moyenne, avec ou sans les 0
			if (ModeMoyenne0Exclu == false)
			{
				if(m_Effective != 0.0)
					return (m_Quantity * 1000.0)/m_Effective;
			}
			else
			{
				if(m_Effective_0Exclu != 0.0)
					return (m_Quantity_0Exclu * 1000.0)/m_Effective_0Exclu;
			}



		case RTYPE_EFFECTIVE_NONCIBLE:
			return m_EffectiveBrutNonCible;

		case RTYPE_EFFECTIVE_CIBLE:
			return m_Effective;

	}
	return 0.0;
}

bool CCellItemMoyenResult::Calculate(RESULTTYPE Type, const JFLT32VECTOR * pQuanti)
{
	if(Type&m_Calculated)
		return true;

	int RowStart	= 0;
	int RowStop		= 0;
	int ColStart	= 0;
	int ColStop		= 0;
	int PageStart	= 0;
	int PageStop	= 0;

	// Get the item cells to calculate
	JList<ITEMTYPE> & Item_Col_Types	= m_pCrossTable->GetItemColTypes();
	JList<ITEMTYPE> & Item_Row_Types	= m_pCrossTable->GetItemRowTypes();
	JList<ITEMTYPE> & Item_Page_Types	= m_pCrossTable->GetItemPageTypes();

	Item_Col_Types.	MoveTo(m_Coord.Col());
	Item_Row_Types.	MoveTo(m_Coord.Row());
	Item_Page_Types.MoveTo(m_Coord.Page());

	if(Item_Col_Types.IsValid() && Item_Row_Types.IsValid() && Item_Page_Types.IsValid())
	{
		RowStart	= RowStop	= m_Coord.Row();
		ColStart	= ColStop	= m_Coord.Col();
		PageStart	= PageStop	= m_Coord.Page();

		if(Item_Row_Types.GetItem() & ITYPE_MOYEN)
		{
			bool bFirst = true;
			for(Item_Row_Types.MoveFirst(); Item_Row_Types.IsValid(); Item_Row_Types.MoveNext())
			{
				if(Item_Row_Types.GetItem() & ITYPE_ITEM)
				{
					if(bFirst)
					{
						RowStop = RowStart = Item_Row_Types.GetIndex();
						bFirst = false;
					}
					else
						RowStop = Item_Row_Types.GetIndex();
				}
			}
		}
		if(Item_Col_Types.GetItem() & ITYPE_MOYEN)
		{
			bool bFirst = true;
			for(Item_Col_Types.MoveFirst(); Item_Col_Types.IsValid(); Item_Col_Types.MoveNext())
			{
				if(Item_Col_Types.GetItem() & ITYPE_ITEM)
				{
					if(bFirst)
					{
						ColStop = ColStart = Item_Col_Types.GetIndex();
						bFirst = false;
					}
					else
						ColStop = Item_Col_Types.GetIndex();
				}
			}
		}
		if(Item_Page_Types.GetItem() & ITYPE_MOYEN)
		{
			bool bFirst = true;
			for(Item_Page_Types.MoveFirst(); Item_Page_Types.IsValid(); Item_Page_Types.MoveNext())
			{
				if(Item_Page_Types.GetItem() & ITYPE_ITEM)
				{
					if(bFirst)
					{
						PageStop = PageStart = Item_Page_Types.GetIndex();
						bFirst = false;
					}
					else
						PageStop = Item_Page_Types.GetIndex();
				}
			}
		}
	}

	switch(Type)
	{
		case RTYPE_EFFECTIVE_000:
		case RTYPE_EFFECTIVE_CIBLE:
		case RTYPE_NBCAS:
		{
			m_Effective			= 0.0;
			m_Effective_0Exclu	= 0.0;
			m_NbCas				= 0.0;

			int Count = 0;

			for(int p = PageStart; p <= PageStop; ++p)
			{
				for(int r = RowStart; r <= RowStop; ++r)
				{
					for(int c = ColStart; c <= ColStop; ++c)
					{
						CCoordRowCol cc(p,r,c);
						m_pCellResults->MoveTo(cc);
						if(m_pCellResults->IsValid())
						{
							m_Effective			+= m_pCellResults->GetItem()->GetResult(RTYPE_EFFECTIVE_CIBLE);
							m_Effective_0Exclu	+= m_pCellResults->GetItem()->GetResult(RTYPE_EFFECTIVE_CIBLE, pQuanti, true);
							m_NbCas += m_pCellResults->GetItem()->GetResult(RTYPE_NBCAS);
							++Count;
						}
					}
				}
			}
			if(Count)
			{
				m_Effective /= Count;
				m_NbCas /= Count;
			}
			m_Calculated |= (RTYPE_EFFECTIVE_000|RTYPE_NBCAS|RTYPE_EFFECTIVE_CIBLE);
		}
		break;

		case RTYPE_QUANTITY_000:
		{
			if(Calculate(RTYPE_EFFECTIVE_000, pQuanti))
			{
				m_Quantity = 0.0;
				int Count = 0;
				for(int p = PageStart; p <= PageStop; ++p)
				{
					for(int r = RowStart; r <= RowStop; ++r)
					{
						for(int c = ColStart; c <= ColStop; ++c)
						{
							CCoordRowCol cc(p,r,c);
							m_pCellResults->MoveTo(cc);
							if(m_pCellResults->IsValid())
							{
								m_Quantity			+= m_pCellResults->GetItem()->GetResult(RTYPE_QUANTITY_000);
								m_Quantity_0Exclu	+= m_pCellResults->GetItem()->GetResult(RTYPE_QUANTITY_000, pQuanti, true);
								++Count;
							}
						}
					}
				}
				if(Count)
				{
					m_Quantity /= Count;
				}
				m_Calculated |= Type;
			}
		}
		break;

		case RTYPE_AVERAGEQUANTUM:
		case RTYPE_AVERAGEQUANTUM_P00:
		{
			if(Calculate(RTYPE_QUANTITY_000, pQuanti))
				m_Calculated |= Type;
		}
		break;

		case RTYPE_EFFECTIVE_NONCIBLE:
		{
			m_EffectiveBrutNonCible = 0.0;

			int Count = 0;

			for(int p = PageStart; p <= PageStop; ++p)
			{
				for(int r = RowStart; r <= RowStop; ++r)
				{
					for(int c = ColStart; c <= ColStop; ++c)
					{
						CCoordRowCol cc(p,r,c);
						m_pCellResults->MoveTo(cc);
						if(m_pCellResults->IsValid())
						{
							m_EffectiveBrutNonCible += m_pCellResults->GetItem()->GetResult(Type);
							++Count;
						}
					}
				}
			}
			if(Count)
				m_EffectiveBrutNonCible /= Count;

			m_Calculated |= Type;
		}
		break;
	}
	return (Type&m_Calculated);
}

