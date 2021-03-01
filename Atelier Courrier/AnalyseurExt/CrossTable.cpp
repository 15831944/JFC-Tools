#include "stdafx.h"
#include <afxadv.h>
#include "crosstable.h"
#include "CoordRowCol.h"
#include "colsortkey.h"
#include "cellresult.h"
#include "cellitemmoyenresult.h"
#include "AnalyseurEnums.h"
#include "DlgParamsTri.h"
#include "Resource.h"
#include "DlgAnalyseur.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCrossTable::CCrossTable(void)
{
	m_pDoc				=	0;

	m_Base100			=   FALSE;

	m_pBut_Surlignage	=	0;

	m_Result_Col_Count	=	1;
	m_Row_StartItem		=	0;
	m_ItemRowCount		=	0;
	m_FixedRows			=	0;
	m_NbRows			=	0;
	m_ItemRowIndex.		Reset();

	m_Result_Col_Count	=	1;
	m_Col_StartItem		=	0;
	m_ItemColCount		=	0;
	m_FixedCols			=	0;
	m_NbCols			=	0;
	m_ItemColIndex.		Reset();

//	m_CurPage			=	0;
	m_NbPages			=	0;
	m_ItemPageIndex.	Reset();

	m_pGrid				=	0;

	m_Result_Col_Count  =	0;
	m_Result_Row_Count	=	0;

	m_SortedRowIndex.		Reset();
	m_CurRow			=	-1;
	m_CurCol			=	-1;

	// Init ligne et col header (survol cellule)
	m_RowCurHeader      =   -1;
	m_ColCurHeader		=   -1;

	// Infos terrains et grandeurs
	JUnt32 NbHeaderColTerrain  = 0;
	JUnt32 NbHeaderColGrandeur = 0;
	JUnt32 NbHeaderRowTerrain  = 0;
	JUnt32 NbHeaderRowGrandeur = 0;
}

CCrossTable::~CCrossTable(void)
{
}

// Setup Cross Table Grid display
// bool CCrossTable::SetupGrid(CAnCrossTable * pGrid, CAnalyseurDoc *pDoc, CButtonST* pBut_Base100, CButtonST* pBut_Surlignage)
bool CCrossTable::SetupGrid(CAnCrossTable * pGrid, CAnalyseurDoc *pDoc, BOOL Base100, CButtonST* pBut_Surlignage)
{
	m_pDoc				= pDoc;
	m_pGrid				= pGrid;

	m_Base100 = Base100;

	m_pBut_Surlignage	= m_pBut_Surlignage;

	if(m_pGrid)
	{
		m_pGrid->SetIItemTable(this);

		m_pGrid->SetMergeCellsMode(gxnMergeDelayEval  ,false);

		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi le nombre de ligne et colonne par cellule résultat (selon type affichage et grandeurs sélectionnées)
void CCrossTable::GetResultRowColCount(JUnt32 &ResultRowCount, JUnt32 &ResultColCount)
{
	// Init nb row el col
	ResultRowCount = 0;
	ResultColCount = 0;

	int Cnt = 0;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
		++Cnt;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
		++Cnt;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0 )
		++Cnt;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		++Cnt;

	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		// Affichage des résultats en mode ligne (les uns en dessous des autres)
		switch(Cnt)
		{
			case 0:
			break;

			case 1:
			{
				ResultRowCount = 1;
				ResultColCount = 1;
			}
			break;

			case 2:
			{
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// Sélection Résultat
					if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						// En + résultat vertical
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + résultat horizontal
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
					else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						// En + résultat global
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
				}
				// Résultat horizontal sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
				{
					// En + résultat vertical
					if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						ResultRowCount = 2;
						ResultColCount = 1;
						break;

					}
					// En + résultat global
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
					{
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
				}
				// Résultat vertical sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
				{
					// En + résultat global
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
				}
			}

			// 3 options résultat choisis
			case 3:
			{
				// Option calcul Résultat
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// En + calcul horizontal
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + calcul vertical
						if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
						{
							ResultRowCount = 3;
							ResultColCount = 1;
							break;
						}
						// En + calcul global
						else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
						{
							ResultRowCount = 3;
							ResultColCount = 1;
							break;
						} 
					}
					else
					{
						// En + résultat vertical et global en position 2 et 3
						ResultRowCount = 3;
						ResultColCount = 1;
						break;
					}
				}
				// Résultat horizontal, vertical et global
				else 
				{
					ResultRowCount = 3;
					ResultColCount = 1;
					break;
				}
			}

			// 4 options résultats choisis
			case 4:
			{
				ResultRowCount = 4;
				ResultColCount = 1;
			}
			break;
		}
	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		// Affichage des résultats en mode colonne (les uns à côté des autres)
		switch(Cnt)
		{
			case 0:
			break;

			case 1:
			{
				ResultRowCount = 1;
				ResultColCount = 1;
			}
			break;

			case 2:
			{
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// Sélection Résultat
					if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						// En + résultat vertical
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + résultat horizontal
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
					else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						// En + résultat global
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
				}
				// Résultat horizontal sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
				{
					// En + résultat vertical
					if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
					// En + résultat global
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
					{
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
				}
				// Résultat vertical sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
				{
					// En + résultat global
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
				}
			}

			// 3 options résultat choisis
			case 3:
			{
				// Option calcul Résultat
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// En + calcul horizontal
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + calcul vertical
						if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
						{
							ResultRowCount = 1;
							ResultColCount = 3;
							break;
						}
						// En + calcul global
						else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
						{
							ResultRowCount = 1;
							ResultColCount = 3;
							break;
						} 
					}
					else
					{
						// En + résultat vertical et global en position 2 et 3
						ResultRowCount = 1;
						ResultColCount = 3;
						break;
					}
				}
				// Résultat horizontal, vertical et global
				else 
				{
					ResultRowCount = 1;
					ResultColCount = 3;
					break;
				}
			}

			// 4 options résultats choisis
			case 4:
			{
				ResultRowCount = 1;
				ResultColCount = 4;
			}
			break;
		}

	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		// Présentation en 4 cellules 2 par 2 (global, H%, V%, Result) 
		switch(Cnt)
		{
			case 0:
			break;

			case 1:
			{
				ResultRowCount = 1;
				ResultColCount = 1;
			}
			break;

			case 2:
			{
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
				}
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
					{
						ResultRowCount = 2;
						ResultColCount = 1;
						break;
					}
				}
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						ResultRowCount = 1;
						ResultColCount = 2;
						break;
					}
				}
			}

			case 3:
			case 4:
			{
				ResultRowCount = 2;
				ResultColCount = 2;
			}
			break;
		}
	}
}

// Sets the Grid results styles
void CCrossTable::CalcRowColCells()
{
	int Big = 62;
	int Small = 62;
	m_CellResultStyles.Reset();

	m_Result_Row_Count = 0;
	m_Result_Col_Count = 0;

	int Cnt = 0;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
	{
		++Cnt;
		const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();
		for(CalcTypes.MoveFirst(); CalcTypes.IsValid(); CalcTypes.MoveNext())
		{
			switch(CalcTypes.GetItem())
			{
				case RTYPE_QUANTITY_000:
				case RTYPE_EFFECTIVE_CIBLE:
				{
					Big = 70;
				}
				break;

				case RTYPE_NBCAS:
				case RTYPE_AVERAGEQUANTUM:
				case RTYPE_AVERAGEQUANTUM_P00:
				case RTYPE_PENETRATION: 
				case RTYPE_CENTRAGE: 
				case RTYPE_EFFECTIVE_000:
				case RTYPE_CONCOMITENCE:
				case RTYPE_COEFFYULE:
				case RTYPE_ZSCORE:
				break;
			}
		}
	}

	if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
		++Cnt;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0 )
		++Cnt;
	if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
		++Cnt;

	m_ColWidth[0] = Big;
	m_ColWidth[1] = Small;


	if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_LIGNE)
	{
		// Affichage des résultats en mode ligne (les uns en dessous des autres)
		switch(Cnt)
		{
			case 0:
			break;

			case 1:
			{
				m_Result_Row_Count = 1;
				m_Result_Col_Count = 1;
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
				{
					m_ColWidth[1] = Big;
					m_CellResultStyles.AddTail() = RSTYLE_RESULT;
				}
				else
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0 )
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
				}
			}
			break;

			case 2:
			{
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// Sélection Résultat
					if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						// En + résultat vertical
						m_ColWidth[1] = Big;
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						break;
					}
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + résultat horizontal
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						break;
					}
					else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						// En + résultat global
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				// Résultat horizontal sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
				{
					// En + résultat vertical
					if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						break;

					}
					// En + résultat global
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				// Résultat vertical sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
				{
					// En + résultat global
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
			}

			// 3 options résultat choisis
			case 3:
			{
				// Option calcul Résultat
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// En + calcul horizontal
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + calcul vertical
						if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
						{
							m_CellResultStyles.AddTail() = RSTYLE_RESULT;
							m_Result_Row_Count = 3;
							m_Result_Col_Count = 1;
							m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
							m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
							break;
						}
						// En + calcul global
						else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
						{
							m_CellResultStyles.AddTail() = RSTYLE_RESULT;
							m_Result_Row_Count = 3;
							m_Result_Col_Count = 1;
							m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
							m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
							break;
						} 
					}
					else
					{
						// En + résultat vertical et global en position 2 et 3
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 3;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				// Résultat horizontal, vertical et global
				else 
				{
					m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
					m_Result_Row_Count = 3;
					m_Result_Col_Count = 1;
					m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
					m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
					break;
				}
			}

			// 4 options résultats choisis
			case 4:
			{
				m_Result_Row_Count = 4;
				m_Result_Col_Count = 1;
				m_CellResultStyles.AddTail() = RSTYLE_RESULT;
				m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
				m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
				m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
			}
			break;
		}
	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_COLONNE)
	{
		// Affichage des résultats en mode colonne (les uns à côté des autres)
		switch(Cnt)
		{
			case 0:
			break;

			case 1:
			{
				m_Result_Row_Count = 1;
				m_Result_Col_Count = 1;
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
				{
					m_ColWidth[1] = Big;
					m_CellResultStyles.AddTail() = RSTYLE_RESULT;
				}
				else
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0 )
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
				}
			}
			break;

			case 2:
			{
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// Sélection Résultat
					if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						// En + résultat vertical
						m_ColWidth[1] = Big;
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						break;
					}
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + résultat horizontal
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						break;
					}
					else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						// En + résultat global
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				// Résultat horizontal sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
				{
					// En + résultat vertical
					if ((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						break;

					}
					// En + résultat global
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				// Résultat vertical sélectionné
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
				{
					// En + résultat global
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
			}

			// 3 options résultat choisis
			case 3:
			{
				// Option calcul Résultat
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					// En + calcul horizontal
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						// En + calcul vertical
						if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
						{
							m_CellResultStyles.AddTail() = RSTYLE_RESULT;
							m_Result_Row_Count = 1;
							m_Result_Col_Count = 3;
							m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
							m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
							break;
						}
						// En + calcul global
						else if ((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
						{
							m_CellResultStyles.AddTail() = RSTYLE_RESULT;
							m_Result_Row_Count = 1;
							m_Result_Col_Count = 3;
							m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
							m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
							break;
						} 
					}
					else
					{
						// En + résultat vertical et global en position 2 et 3
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 3;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				// Résultat horizontal, vertical et global
				else 
				{
					m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
					m_Result_Row_Count = 1;
					m_Result_Col_Count = 3;
					m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
					m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
					break;
				}
			}

			// 4 options résultats choisis
			case 4:
			{
				m_Result_Row_Count = 1;
				m_Result_Col_Count = 4;
				m_CellResultStyles.AddTail() = RSTYLE_RESULT;
				m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
				m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
				m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
			}
			break;
		}

	}
	else if (m_pDoc->GetModeAffichResult() == m_pDoc->AFFRESULT_2X2)
	{
		// Présentation en 4 cellules 2 par 2 (global, H%, V%, Result) 
		switch(Cnt)
		{
			case 0:
			break;

			case 1:
			{
				m_Result_Row_Count = 1;
				m_Result_Col_Count = 1;
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) > 0)
				{
					m_ColWidth[1] = Big;
					m_CellResultStyles.AddTail() = RSTYLE_RESULT;
				}
				else
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) > 0)
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) > 0 )
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
				}
			}
			break;

			case 2:
			{
				if((m_pDoc->Get_ResultStyle()&RSTYLE_RESULT) != 0)
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
					{
						m_ColWidth[1] = Big;
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						break;
					}
					else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_RESULT;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						break;
					}
				}
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_HORIZONTAL) != 0)
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) > 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
						m_Result_Row_Count = 2;
						m_Result_Col_Count = 1;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
				else if((m_pDoc->Get_ResultStyle()&RSTYLE_VERTICAL) != 0)
				{
					if((m_pDoc->Get_ResultStyle()&RSTYLE_GLOBAL) != 0)
					{
						m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
						m_Result_Row_Count = 1;
						m_Result_Col_Count = 2;
						m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
						break;
					}
				}
			}

			case 3:
			case 4:
			{
				m_Result_Row_Count = 2;
				m_Result_Col_Count = 2;
				m_CellResultStyles.AddTail() = RSTYLE_RESULT;
				m_CellResultStyles.AddTail() = RSTYLE_HORIZONTAL;
				m_CellResultStyles.AddTail() = RSTYLE_VERTICAL;
				m_CellResultStyles.AddTail() = RSTYLE_GLOBAL;
			}
			break;
		}
	}
}

CCalcItemParams * CCrossTable::GetTargetCalcParams(int Univers)
{
	if(!m_pDoc || !m_pDoc->Get_SelectedCible() || Univers >= m_UniversTarget.GetCount())
		return 0;

	m_UniversTarget.MoveTo(Univers);
	return m_UniversTarget.GetItem().get();
}

// Calculate all the vectors
bool CCrossTable::CalculateTargets()
{
	m_UniversTarget.Reset();

	if(m_pDoc && (m_pDoc->Get_SelectedCible() != m_pDoc->Get_ItemEnsemble()))
	{
		for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
		{
			CCalcItemParams * pTParams = (m_UniversTarget.AddTail() = PTR_CALCITEMPARAMS(new CCalcItemParams)).get();
			if(!pTParams->GetResults())
			{
				pTParams->CopyParams(*(m_UniversParams.GetItem()));
				m_pDoc->Get_SelectedCible()->Calculate(pTParams); 
			}
		}
	}
	return true;
}

// Gets the row params
JList<CCalcItemParams> &	CCrossTable::GetRowItParams(int Universe)
{
	m_Dim_ItemParams[m_pDoc->Get_RowIndex()].MoveTo(Universe);
	if(!m_Dim_ItemParams[m_pDoc->Get_RowIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_ItemParams[m_pDoc->Get_RowIndex()].GetItem();
}

// Gets the Col params
JList<CCalcItemParams> &	CCrossTable::GetColItParams(int Universe)
{
	m_Dim_ItemParams[m_pDoc->Get_ColIndex()].MoveTo(Universe);
	if(!m_Dim_ItemParams[m_pDoc->Get_ColIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_ItemParams[m_pDoc->Get_ColIndex()].GetItem();
}

// Gets the page params
JList<CCalcItemParams> &	CCrossTable::GetPageItParams(int Universe)
{
	m_Dim_ItemParams[m_pDoc->Get_PageIndex()].MoveTo(Universe);
	if(!m_Dim_ItemParams[m_pDoc->Get_PageIndex()].IsValid())
		AfxThrowUserException( );
	return m_Dim_ItemParams[m_pDoc->Get_PageIndex()].GetItem();
}

// Gets the row params
JList<CCalcItemParams> &	CCrossTable::GetRowExParams(int Universe)
{
	m_Dim_ExItemParams[m_pDoc->Get_RowIndex()].MoveTo(Universe);
	if(!m_Dim_ExItemParams[m_pDoc->Get_RowIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_ExItemParams[m_pDoc->Get_RowIndex()].GetItem();
}

// Gets the row params
JList<CCalcItemParams> &	CCrossTable::GetRowDuParams(int Universe)
{
	m_Dim_DuItemParams[m_pDoc->Get_RowIndex()].MoveTo(Universe);
	if(!m_Dim_DuItemParams[m_pDoc->Get_RowIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_DuItemParams[m_pDoc->Get_RowIndex()].GetItem();
}

// Gets the Col params
JList<CCalcItemParams> &	CCrossTable::GetColExParams(int Universe)
{
	m_Dim_ExItemParams[m_pDoc->Get_ColIndex()].MoveTo(Universe);
	if(!m_Dim_ExItemParams[m_pDoc->Get_ColIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_ExItemParams[m_pDoc->Get_ColIndex()].GetItem();
}

// Gets the Col params
JList<CCalcItemParams> &	CCrossTable::GetColDuParams(int Universe)
{
	m_Dim_DuItemParams[m_pDoc->Get_ColIndex()].MoveTo(Universe);
	if(!m_Dim_DuItemParams[m_pDoc->Get_ColIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_DuItemParams[m_pDoc->Get_ColIndex()].GetItem();
}

// Gets the page params
JList<CCalcItemParams> &	CCrossTable::GetPageExParams(int Universe)
{
	m_Dim_ExItemParams[m_pDoc->Get_PageIndex()].MoveTo(Universe);
	if(!m_Dim_ExItemParams[m_pDoc->Get_PageIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_ExItemParams[m_pDoc->Get_PageIndex()].GetItem();
}

// Gets the page params
JList<CCalcItemParams> &	CCrossTable::GetPageDuParams(int Universe)
{
	m_Dim_DuItemParams[m_pDoc->Get_PageIndex()].MoveTo(Universe);
	if(!m_Dim_DuItemParams[m_pDoc->Get_PageIndex()].IsValid())
		AfxThrowUserException( );

	return m_Dim_DuItemParams[m_pDoc->Get_PageIndex()].GetItem();
}

// Gets the row params
JList<CCalcItemParams> &	CCrossTable::GetRowCalcParams(int Universe)
{
	if(GetRowItParams(Universe).GetCount() == 0)
		CalculateRows(Universe);

	if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
	{
		if(GetRowExParams(Universe).GetCount() == 0)
			CalculateRows(Universe);
		return GetRowExParams(Universe);
	}
	else if (m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE)
	{
		if(GetRowDuParams(Universe).GetCount() == 0)
			CalculateRows(Universe);
		return GetRowDuParams(Universe);
	}
	else
		return GetRowItParams(Universe);
}

// Gets the Col params
JList<CCalcItemParams> &	CCrossTable::GetColCalcParams(int Universe)
{
	if(GetColItParams(Universe).GetCount() == 0)
		CalculateCols(Universe);

	if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
	{
		if(GetColExParams(Universe).GetCount() == 0)
			CalculateCols(Universe);
		return GetColExParams(Universe);
	}
	else if (m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE)
	{
		if(GetColDuParams(Universe).GetCount() == 0)
			CalculateCols(Universe);
		return GetColDuParams(Universe);
	}
	else
		return GetColItParams(Universe);
}

// Gets the page params
JList<CCalcItemParams> &	CCrossTable::GetPageCalcParams(int Universe)
{
	if(GetPageItParams(Universe).GetCount() == 0)
		CalculatePages(Universe);

	if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
	{
		if(GetPageExParams(Universe).GetCount() == 0)
			CalculatePages(Universe);
		return GetPageExParams(Universe);
	}
	else if (m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE)
	{
		if(GetPageDuParams(Universe).GetCount() == 0)
			CalculatePages(Universe);
		return GetPageDuParams(Universe);
	}
	else
		return GetPageItParams(Universe);
}

MAPCELLRESULTS * CCrossTable::GetCellResults(int Universe)
{
	m_CellResults.MoveTo(Universe);
	if(!m_CellResults.IsValid())
		AfxThrowUserException( );
	return m_CellResults.GetItem().get();
}

int CCrossTable::CalcColSize()
{
	int ItemPos				=	0;
	const CONST_LIST_ITEM_PUPITRE & Items =	m_pDoc->GetColItems();
	// The first item in the Col dimension items list
	m_Col_StartItem			=	0;
	m_ItemColCount			=	0;
	// Item Col Start
	m_NbCols				=	0;
	// List of Item Col Types
	m_Item_Col_Types.		Reset();
	m_ItemColIndex.			Reset();
	m_CurCol				=	0;
	m_FixedCols				=	0;
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	CalcRowColCells();

	// Calculate the number of Col headers
	m_Col_Hdr_Type.			Reset();
	m_Col_Hdr_Type.			AddTail() = CTYPE_HEADER1;
	m_Col_Hdr_Type.			AddTail() = CTYPE_HEADER2;

	if(!(m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1)
		m_Col_Hdr_Type.AddTail() = CTYPE_HEADERTERRAIN;

	if(!(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
		m_Col_Hdr_Type.AddTail() = CTYPE_HEADERCALC;

	// Check and add the univers
	if(m_UniversParams.GetCount() > 0 && 
		(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX || (m_pDoc->Get_TableStyle() & TSTYLE_NICHEC && Items.GetCount() == 0)))
	{
		m_ItemColIndex.AddTail() = ItemPos;
		++m_FixedCols;
	}
	++ItemPos;
	m_Item_Col_Types.AddTail() = ITYPE_UNIVERSE;

	// Check and add the niche
	if(m_pDoc->Get_TableStyle() & TSTYLE_NICHEC && Items.GetCount() > 0)
	{
		m_ItemColIndex.AddTail() = ItemPos;
		if(!(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX) && m_pDoc->Get_BaseColItem() == 0)
			m_pDoc->Set_BaseColItem(1+(m_pDoc->Get_BaseColItem()));
		if(m_FixedCols == 0)
			++m_FixedCols;
	}
	++ItemPos;
	m_Col_StartItem = ItemPos;
	m_Item_Col_Types.AddTail() =  ITYPE_NICHE;

	// Check and add the items
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
	{
		if(m_pDoc->Get_TableStyle() & TSTYLE_ITEMSC)
			m_ItemColIndex.AddTail() = ItemPos;
		++ItemPos;
		m_Item_Col_Types.AddTail() =  ITYPE_ITEM;
	}

	// Check for Item Moyen Cols
	if(Items.GetCount() > 0 && m_pDoc->Get_TableStyle() & TSTYLE_ITEMMOYENC)
		m_ItemColIndex.AddTail() = ItemPos;
	++ItemPos;
	m_Item_Col_Types.AddTail() =  ITYPE_MOYEN;

	// Check for contre nich Col
	if(Items.GetCount() > 0 && m_pDoc->Get_TableStyle() & TSTYLE_CONTRENICHEC)
		m_ItemColIndex.AddTail() = ItemPos;
	++ItemPos;
	m_Item_Col_Types.AddTail() =  ITYPE_CONTRENICHE;

	// Calculate the Cols size per item
	m_ItemColCount = m_Result_Col_Count;
	
	if((m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
		m_ItemColCount *= CalcTypes.GetCount();
	if((m_pDoc->Get_TableStyle() &TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1)
		m_ItemColCount *= m_UniversParams.GetCount();
	

	m_FixedCols *= m_ItemColCount;
	// Calculate the exact number of Cols
	m_NbCols = (m_ItemColCount*m_ItemColIndex.GetCount())+m_Col_Hdr_Type.GetCount();

	return m_NbCols;
}

int CCrossTable::CalcRowSize()
{
	int ItemPos				=	0;
	const CONST_LIST_ITEM_PUPITRE & Items =	m_pDoc->GetRowItems();
	// The first item in the Row dimension items list
	m_Row_StartItem			=	0;
	m_ItemRowCount			=	0;
	// Item rows Start
	m_NbRows				=	0;
	// List of Item row Types
	m_Item_Row_Types.		Reset();
	m_ItemRowIndex.			Reset();
	m_CurRow				=	0;
	m_FixedRows				=	0;

	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	CalcRowColCells();

	// Calculate the number of row headers
	m_Row_Hdr_Type.			Reset();
	m_Row_Hdr_Type.			AddTail() = CTYPE_HEADER1;
	m_Row_Hdr_Type.			AddTail() = CTYPE_HEADER2;

	if((m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1)
		m_Row_Hdr_Type.AddTail() = CTYPE_HEADERTERRAIN;

	if((m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
		m_Row_Hdr_Type.AddTail() = CTYPE_HEADERCALC;

	// Check and add the univers
	if(m_UniversParams.GetCount() > 0 && 
		(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX || (m_pDoc->Get_TableStyle() & TSTYLE_NICHER && Items.GetCount() == 0)))
	{
		m_ItemRowIndex.AddTail() = ItemPos;
		++m_FixedRows;
	}
	++ItemPos;
	m_Item_Row_Types.AddTail() =  ITYPE_UNIVERSE;

	// Check and add the niche
	if(m_pDoc->Get_TableStyle() & TSTYLE_NICHER && Items.GetCount() > 0)
	{
		m_ItemRowIndex.AddTail() = ItemPos;
		if(!(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX) && m_pDoc->Get_BaseRowItem() == 0)
			m_pDoc->Set_BaseRowItem(1+(m_pDoc->Get_BaseRowItem()));
		if(m_FixedRows == 0)
			++m_FixedRows;
	}
	++ItemPos;

	m_Row_StartItem = ItemPos;
	m_Item_Row_Types.AddTail() =  ITYPE_NICHE;

	// Check and add the items
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
	{
		if(m_pDoc->Get_TableStyle() & TSTYLE_ITEMSR)
			m_ItemRowIndex.AddTail() = ItemPos;
		++ItemPos;
		m_Item_Row_Types.AddTail() =  ITYPE_ITEM;
	}

	// Check for Item Moyen
	if(Items.GetCount() > 0 && m_pDoc->Get_TableStyle() & TSTYLE_ITEMMOYENR)
		m_ItemRowIndex.AddTail() = ItemPos;
	++ItemPos;
	m_Item_Row_Types.AddTail() =  ITYPE_MOYEN;

	// Check for contre nich Col
	if(Items.GetCount() > 0 && m_pDoc->Get_TableStyle() & TSTYLE_CONTRENICHER)
		m_ItemRowIndex.AddTail() = ItemPos;
	++ItemPos;
	m_Item_Row_Types.AddTail() =  ITYPE_CONTRENICHE;

	// Calculate the row size per item
	m_ItemRowCount =	m_Result_Row_Count;

	if(!(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
		m_ItemRowCount *= CalcTypes.GetCount();

	if(!(m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1)
		m_ItemRowCount *= m_UniversParams.GetCount();

	m_FixedRows *= m_ItemRowCount;

	// Définition des valeurs utilisées
	//
	// m_ItemRowCount : Nombre de lignes affichés par item = nb grandeurs
	// m_ItemRowIndex : Nombre d'items en lignes
	// m_Row_Hdr_Type : Nombre de headers rows

	// Calculate the exact number of rows
	int ValRowIndex = m_ItemRowIndex.GetCount();
	int ValHdrType  = m_Row_Hdr_Type.GetCount();
	m_NbRows = (m_ItemRowCount*m_ItemRowIndex.GetCount())+m_Row_Hdr_Type.GetCount();

	return m_NbRows;
}

int CCrossTable::CalcPageSize()
{
	int ItemPos				=	0;
	const CONST_LIST_ITEM_PUPITRE & Items =	m_pDoc->GetPageItems();
	// List of Item row Types
	m_Item_Page_Types.		Reset();
	m_ItemPageIndex.		Reset();
//	m_CurPage				=	0;
	m_NbPages				=	0;
	m_PageChoice.			Empty();
	m_BasePageItem			=	0;

	// Check and add the univers
	if(m_UniversParams.GetCount() > 0 && 
		(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX || (m_pDoc->Get_TableStyle() & TSTYLE_NICHEP  && Items.GetCount() == 0)))
	{
		m_ItemPageIndex.AddTail() = ItemPos;
		if(m_pDoc->Get_ResultStyle()&RSTYLE_CIBLES)
			m_PageChoice.Format(IDS_AN_LABEL_CIBLES);
		else
			m_PageChoice.Format(IDS_AN_LABEL_UNIVERS);

		//if(m_pTargetItem)
		//	m_PageChoice.Format(m_pTargetItem->GetLabel().AsJCharPtr());

		m_PageChoice += _T("\n");
	}
	++ItemPos;
	m_Item_Page_Types.AddTail() =  ITYPE_UNIVERSE;

	// Check and add the niche
	if(m_pDoc->Get_TableStyle() & TSTYLE_NICHEP && Items.GetCount() > 0)
	{
		m_ItemPageIndex.AddTail() = ItemPos;
		m_PageChoice.AppendFormat(IDS_AN_NICHE);
		m_PageChoice += _T("\n");
		if(!(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX))
			++m_BasePageItem;
	}
	++ItemPos;
	m_Page_StartItem = ItemPos;
	m_Item_Page_Types.AddTail() =  ITYPE_NICHE;

	// Check and add the items
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
	{
		if(m_pDoc->Get_TableStyle() & TSTYLE_ITEMSP)
		{
			m_ItemPageIndex.AddTail() = ItemPos;
			m_PageChoice += Items.GetItem()->GetLabel();
			m_PageChoice += _T("\n");
		}
		++ItemPos;
		m_Item_Page_Types.AddTail() =  ITYPE_ITEM;
	}

	// Check for Item Moyen Cols
	if(Items.GetCount() > 0 && m_pDoc->Get_TableStyle() & TSTYLE_ITEMMOYENP)
	{
		m_ItemPageIndex.AddTail() = ItemPos;
		m_PageChoice.AppendFormat(IDS_AN_ITEMMOYEN);
		m_PageChoice += _T("\n");
	}
	++ItemPos;
	m_Item_Page_Types.AddTail() =  ITYPE_MOYEN;

	// Check for contre nich Col
	if(Items.GetCount() > 0 && m_pDoc->Get_TableStyle()& TSTYLE_CONTRENICHEP)
	{
		m_ItemPageIndex.AddTail() = ItemPos;
		m_PageChoice.AppendFormat(IDS_AN_CONTRENICHE);
		m_PageChoice += _T("\n");
	}
	++ItemPos;
	m_Item_Page_Types.AddTail() =  ITYPE_CONTRENICHE;

	// Calculate the exact number of rows
	if(m_UniversParams.GetCount()>0)
		m_NbPages = m_ItemPageIndex.GetCount();
	else
		m_NbPages = 0;

	return m_NbPages;
}

bool CCrossTable::CalculateCols()
{
	ResetSort();
	const CONST_LIST_ITEM_PUPITRE & Items = m_pDoc->GetColItems();

	m_Item_Col_Types		.Reset(); 
	m_Col_StartItem			= 0;
	m_NbCols				= 0;
//	m_pDoc->Set_BaseColItem(0);

	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{
		CCalcItemParams & UParams			=	*(m_UniversParams.GetItem());
		JList<CCalcItemParams> & ParamsIt	=	GetColItParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ParamsEx	=	GetColExParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ParamsDu	=	GetColDuParams(m_UniversParams.GetIndex());

		ParamsIt.Reset();
		ParamsEx.Reset();
		ParamsDu.Reset();
		CalculateDimensionIt(Items, UParams, ParamsIt);

		if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
			CalculateDimensionEx(Items, UParams, ParamsIt, ParamsEx);

		if(m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE)
			CalculateDimensionDu(Items, UParams, ParamsIt, ParamsDu);
	}
	return true;
}

bool CCrossTable::CalculateCols(int Univers)
{
	const CONST_LIST_ITEM_PUPITRE & Items = m_pDoc->GetColItems();

	int UniversIdx = m_UniversParams.GetIndex();

	m_UniversParams.MoveTo(Univers);

	if(m_UniversParams.IsValid())
	{
		CCalcItemParams & UParams			=	*(m_UniversParams.GetItem());
		JList<CCalcItemParams> & ParamsIt	=	GetColItParams(Univers);
		JList<CCalcItemParams> & ParamsEx	=	GetColExParams(Univers);
		JList<CCalcItemParams> & ParamsDu	=	GetColDuParams(Univers);

		if(ParamsIt.GetCount() == 0)
			CalculateDimensionIt(Items, UParams, ParamsIt);

		if((m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE) && ParamsEx.GetCount() == 0)
			CalculateDimensionEx(Items, UParams, ParamsIt, ParamsEx);

		if((m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE) && ParamsDu.GetCount() == 0)
			CalculateDimensionDu(Items, UParams, ParamsIt, ParamsDu);
	}

	m_UniversParams.MoveTo(UniversIdx);

	return true;
}

bool CCrossTable::CalculateRows()
{
	ResetSort();
	const CONST_LIST_ITEM_PUPITRE & Items = m_pDoc->GetRowItems();

	m_Item_Row_Types		.Reset(); 
	m_Row_StartItem			= 0;
	m_NbRows				= 0;
//	m_pDoc->Set_BaseRowItem(0);

	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{
		CCalcItemParams & UParams			=	*(m_UniversParams.GetItem());
		JList<CCalcItemParams> & ParamsIt	=	GetRowItParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ParamsEx	=	GetRowExParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ParamsDu	=	GetRowDuParams(m_UniversParams.GetIndex());

		ParamsIt.Reset();
		ParamsEx.Reset();
		ParamsDu.Reset();
		CalculateDimensionIt(Items, UParams, ParamsIt);

		if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
			CalculateDimensionEx(Items, UParams, ParamsIt, ParamsEx);

		if(m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE)
			CalculateDimensionDu(Items, UParams, ParamsIt, ParamsDu);
	}
	return true;
}

bool CCrossTable::CalculateRows(int Univers)
{
	const CONST_LIST_ITEM_PUPITRE & Items = m_pDoc->GetRowItems();

	int UniversIdx = m_UniversParams.GetIndex();

	m_UniversParams.MoveTo(Univers);

	if(m_UniversParams.IsValid())
	{
		CCalcItemParams & UParams			=	*(m_UniversParams.GetItem());
		JList<CCalcItemParams> & ParamsIt	=	GetRowItParams(Univers);
		JList<CCalcItemParams> & ParamsEx	=	GetRowExParams(Univers);
		JList<CCalcItemParams> & ParamsDu	=	GetRowDuParams(Univers);

		if(ParamsIt.GetCount() == 0)
			CalculateDimensionIt(Items, UParams, ParamsIt);

		if((m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE) && ParamsEx.GetCount() == 0)
			CalculateDimensionEx(Items, UParams, ParamsIt, ParamsEx);

		if((m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE) && ParamsDu.GetCount() == 0)
			CalculateDimensionDu(Items, UParams, ParamsIt, ParamsDu);
	}

	m_UniversParams.MoveTo(UniversIdx);

	return true;
}

bool CCrossTable::CalculatePages()
{
	ResetSort();
	const CONST_LIST_ITEM_PUPITRE & Items = m_pDoc->GetPageItems();

	m_Item_Page_Types.Reset(); 
	m_NbPages = 0;

	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{
		CCalcItemParams & UParams			=	*(m_UniversParams.GetItem());
		JList<CCalcItemParams> & ParamsIt	=	GetPageItParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ParamsEx	=	GetPageExParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ParamsDu	=	GetPageDuParams(m_UniversParams.GetIndex());

		ParamsIt.Reset();
		ParamsEx.Reset();
		ParamsDu.Reset();
		CalculateDimensionIt(Items, UParams, ParamsIt);

		if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
			CalculateDimensionEx(Items, UParams, ParamsIt, ParamsEx);

		if(m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE)
			CalculateDimensionDu(Items, UParams, ParamsIt, ParamsDu);
	}
	return true;
}

bool CCrossTable::CalculatePages(int Univers)
{
	const CONST_LIST_ITEM_PUPITRE & Items = m_pDoc->GetPageItems();

	int UniversIdx = m_UniversParams.GetIndex();

	m_UniversParams.MoveTo(Univers);

	if(m_UniversParams.IsValid())
	{
		CCalcItemParams & UParams			=	*(m_UniversParams.GetItem());
		JList<CCalcItemParams> & ParamsIt	=	GetPageItParams(Univers);
		JList<CCalcItemParams> & ParamsEx	=	GetPageExParams(Univers);
		JList<CCalcItemParams> & ParamsDu	=	GetPageDuParams(Univers);

		if(ParamsIt.GetCount() == 0)
			CalculateDimensionIt(Items, UParams, ParamsIt);

		if((m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE) && ParamsEx.GetCount() == 0)
			CalculateDimensionEx(Items, UParams, ParamsIt, ParamsEx);

		if((m_pDoc->Get_TableStyle() & TSTYLE_DUPLIQUE) && ParamsDu.GetCount() == 0)
			CalculateDimensionDu(Items, UParams, ParamsIt, ParamsDu);
	}

	m_UniversParams.MoveTo(UniversIdx);

	return true;
}

bool CCrossTable::CalculateDimensionIt(const CONST_LIST_ITEM_PUPITRE & Items, CCalcItemParams & UParams, JList<CCalcItemParams> & ParamsIt)
{
	const CBitArray * pResult = 0;
	const JFLT32VECTOR * pProbas = 0;
	CIBuilder IBuilder;

	// Add the univers to the Item Params
	CCalcItemParams & upIt		=  ParamsIt.AddTail();
	upIt.	CopyParams(UParams);
	upIt.	SetResults(UParams.GetResults());
	upIt.	SetProbas(UParams.GetProbas());
	upIt.	SetNumericalValues(UParams.GetNumericalValues());
	// Add the Niche
	int NicheIndex	=	ParamsIt.GetCount();

	UParams.SetCumulQuanti(m_pDoc->m_bCumulQuanti);
	ParamsIt.AddTail().		CopyParams(UParams); 

	int NbItems		=	Items.GetCount();
	int ItemIndex	=	ParamsIt.GetCount();
	// Add the items
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
	{
		CCalcItemParams & cp	=	ParamsIt.AddTail();
		cp.CopyParams(UParams);
		Items.GetItem()->PtrItem()->Calculate(&cp);

		/*
		// DECEMBRE 2013 (On vérifie si la niche est quantifiable) -- A VOIR
		if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau)
		{
			// Récup le terrain en cours
			CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());
			if (pTerrain)
			{
				// Cet item est-il moyennable
				// Récupére la categorie question
				const CItemWrapper *pItemWrapper = Items.GetItem();

				// Vérifie si on a affaire à un item numérisé quantitatif
				const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem();

				// Récup lé définition de l'item
				pItemConstruit->GetItemBuilder(pTerrain,IBuilder);

				// Par défaut ce n'est pas un quantitatif
				JBool Quantitatif = false;

				// Si aucun segemnt détecté on considère l'élmt comme quantitatif
				if (IBuilder.GetSegmentCount() == 0) 
					m_pDoc->SetNicheQuantifiable(true);
			}

			// Cet item est-il moyennable
			if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
			{
				m_pDoc->SetNicheQuantifiable(true);
			}
		}
		*/




		/*
		// Cet item est-il moyennable
		// Récupére la categorie question
		const CItemWrapper *pItemWrapper = Items.GetItem();

		// Vérifie si on a affaire à un item numérisé quantitatif
		const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem();
		if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
		{
			m_pDoc->SetNicheQuantifiable(true);
		}
		*/

	}

	// Fill in the params for the niche
	// VOIR MARS 2014
	ParamsIt.	MoveTo(NicheIndex);
	if(ParamsIt.IsValid())
	{
		CCalcItemParams & NparamIt = ParamsIt.GetItem();
		CResults NicheResIt(0,IBloc::Or);
		ParamsIt.MoveTo(ItemIndex);
		for(int x = 0; x < NbItems ; ParamsIt.MoveNext(),++x)
		{
			CCalcItemParams & ItemParamIt	=  ParamsIt.GetItem();
			if(ItemParamIt.GetResults())
			{
				CResults * pResIt	= new CResults(&NicheResIt,IBloc::And);
				pResIt->	SetResult(ItemParamIt.GetResults());
				pResIt->	SetProbas(ItemParamIt.GetProbas());
				pResIt->	SetValues(ItemParamIt.GetNumericalValues());
				NicheResIt.	AddResult(pResIt);
			}
		}
		NicheResIt.	Calculate();

		if(NicheResIt.GetResult(pResult))
			NparamIt.	SetResults(pResult);
		else
			NparamIt.	SetResultsD (pResult);
		if(NicheResIt.	GetProbas(pProbas))
			NparamIt.	SetProbas(pProbas);
		else
			NparamIt.	SetProbasD(pProbas);
		JMap<const JFLT32VECTOR*, JInt32> MapValues;

		// Modif Aout 2013 : Ajout Info Diviser pour Niche
		const JMap<const JFLT32VECTOR*, JInt32> & ValuesMap = NicheResIt.GetAllValues(MapValues);

		// @@@@
		if (ValuesMap.GetCount() > 0)
			NparamIt.SetDivSpeNiche(ValuesMap.GetCount());
	
		// DECEMBRE 2013  (Pour niche quantifiable, il faudra diviser par nb de composants)
		// Modif MARS 2014
		/*
		if ( m_pDoc->GetNicheQuantifiable())
			NparamIt.	SetNumericalValues(NicheResIt.GetAllValues(MapValues), true);
		else
			NparamIt.	SetNumericalValues(NicheResIt.GetAllValues(MapValues), false);
		*/
		NparamIt.	SetNumericalValues(NicheResIt.GetAllValues(MapValues), true, m_pDoc->GetNicheQuantifiable());
		//
		///////// FIN MDIF POUR PB NICHE QUANTIFIABLE ///////////////////
	}

	// do Item Moyen Cols
	ParamsIt.AddTail().	CopyParams(UParams);

	// Do for contre niche Col
	CCalcItemParams & NparamIt	=	ParamsIt.AddTail();

	NparamIt.	CopyParams(UParams); 

	CResults NicheResIt(0,IBloc::Not);
	ParamsIt.	MoveTo(ItemIndex);
	for(int x=0; x < NbItems ; ParamsIt.MoveNext(),++x)
	{
		CCalcItemParams & ItemParamIt	=  ParamsIt.GetItem();
		if(ItemParamIt.GetResults())
		{
			CResults * pResIt	=	new CResults(&NicheResIt,IBloc::And);
			pResIt->	SetResult(ItemParamIt.GetResults());
			pResIt->	SetProbas(ItemParamIt.GetProbas());
			pResIt->	SetValues(ItemParamIt.GetNumericalValues());
			NicheResIt.	AddResult(pResIt);
		}
	}
	NicheResIt.	Calculate();
	if(NicheResIt.	GetResult(pResult))
		NparamIt.	SetResults(pResult);
	else
		NparamIt.	SetResultsD(pResult);

	if(NicheResIt.	GetProbas(pProbas))
		NparamIt.	SetProbas(pProbas);  
	else
		NparamIt.	SetProbasD(pProbas);  
	JMap<const JFLT32VECTOR*, JInt32> MapValues;

	// Modif Aout 2013 : Ajout Info Divisuer pour Niche
	const JMap<const JFLT32VECTOR*, JInt32> & ValuesMap = NicheResIt.GetAllValues(MapValues);

	// @@@@
	if (ValuesMap.GetCount() > 0)
		NparamIt.SetDivSpeNiche(ValuesMap.GetCount());

	NparamIt.	SetNumericalValues(NicheResIt.GetAllValues(MapValues));



	// NparamIt.	SetNumericalValues(NicheResIt.GetAllValues(MapValues));

	return true;
}

bool CCrossTable::CalculateDimensionEx(const CONST_LIST_ITEM_PUPITRE & Items, CCalcItemParams & UParams, JList<CCalcItemParams> & ParamsIt, JList<CCalcItemParams> & ParamsEx)
{
	const CBitArray    * pResult = 0;
	const JFLT32VECTOR * pProbas = 0;

	// Add the univers to the Item Params
	CCalcItemParams & upEx = ParamsEx.AddTail();
	upEx.CopyParams(UParams);
	upEx.SetResults(UParams.GetResults());
	upEx.SetProbas (UParams.GetProbas());
	upEx.SetNumericalValues(UParams.GetNumericalValues());

	// Get Niche index and add it
	int NicheIndex = ParamsEx.GetCount();
	ParamsEx.AddTail().CopyParams(UParams); 

	int NbItems   = Items   .GetCount();
	int ItemIndex = ParamsEx.GetCount();

	// Add the items
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		ParamsEx.AddTail().CopyParams(UParams);

	JList<CCalcItemParams*> ParamsCp;
	for (ParamsIt.MoveFirst(); ParamsIt.IsValid(); ParamsIt.MoveNext())
		ParamsCp.AddTail() = &ParamsIt.GetItem();

	// Fill in the params for the exclusive items
	ParamsIt.MoveTo(ItemIndex);
	ParamsEx.MoveTo(ItemIndex);
	if(ParamsIt.IsValid())
	{
		for(int x=0; x < NbItems ; ++x, ParamsIt.MoveNext(), ParamsEx.MoveNext())
		{
			// Get the parameters
			CCalcItemParams & NparamIt = ParamsIt.GetItem();
			CCalcItemParams & NparamEx = ParamsEx.GetItem();

			if(NparamIt.GetResults())
			{
				// Set the Header result blocs
				CResults ResEx(0,IBloc::And);

				// Sub result bloc for item
				CResults * pResIt = ResEx.AddResult(new CResults(&ResEx,IBloc::Nop));
				pResIt->SetResult(NparamIt.GetResults());
				pResIt->SetProbas(NparamIt.GetProbas());
				pResIt->SetValues(NparamIt.GetNumericalValues());

				// Sub result bloc for other items
				CResults * pResOt = ResEx.AddResult(new CResults(&ResEx,IBloc::And));
				ParamsCp.MoveTo(ItemIndex);
				for(int y=0; y < NbItems ; ++y, ParamsCp.MoveNext())
				{
					if(x!=y)
					{
						// Get the parameters
						CCalcItemParams & NparamCp = *ParamsCp.GetItem();
						// Set the Header result blocs
						CResults * pResNt = pResOt->AddResult(new CResults(pResOt,IBloc::Not));
						CResults * pResIt = pResNt->AddResult(new CResults(pResNt,IBloc::Nop));
						pResIt->SetResult(NparamCp.GetResults());
						pResIt->SetProbas(NparamCp.GetProbas());
						pResIt->SetValues(NparamCp.GetNumericalValues());
					}
				}

				// Calculate the exclusives
				ResEx.Calculate();

				if(ResEx.GetResult(pResult))
					NparamEx.SetResults(pResult);
				else
					NparamEx.SetResultsD(pResult);

				if(ResEx.GetProbas(pProbas))
					NparamEx.SetProbas(pProbas);
				else
					NparamEx.SetProbasD(pProbas);

				JMap<const JFLT32VECTOR*, JInt32> MapValues;
				NparamEx.SetNumericalValues(ResEx.GetAllValues(MapValues));
			}
		}
	}

	// Fill in the params for the niche
	ParamsIt.MoveTo(NicheIndex);
	ParamsEx.MoveTo(NicheIndex);
	if(ParamsIt.IsValid() && ParamsEx.IsValid())
	{
		CCalcItemParams & NparamIt = ParamsIt.GetItem();
		CCalcItemParams & NparamEx = ParamsEx.GetItem();

		CResults NicheEx(0,IBloc::Or);

		ParamsEx.MoveTo(ItemIndex);
		for(int x=0; x < NbItems ; ParamsEx.MoveNext(), ++x)
		{
			CCalcItemParams & ItemParamEx = ParamsEx.GetItem();
			if(ItemParamEx.GetResults())
			{
				CResults * pResEx =	new CResults(&NicheEx,IBloc::And);
				pResEx->SetResult(ItemParamEx.GetResults());
				pResEx->SetProbas(ItemParamEx.GetProbas());
				pResEx->SetValues(ItemParamEx.GetNumericalValues());
				NicheEx.AddResult(pResEx);
			}
		}
		NicheEx.Calculate();

		if(NicheEx.GetResult(pResult))
			NparamEx.SetResults(pResult);
		else
			NparamEx.SetResultsD(pResult);

		if(NicheEx.GetProbas(pProbas))
			NparamEx.SetProbas(pProbas);
		else
			NparamEx.SetProbasD(pProbas);

		JMap<const JFLT32VECTOR*, JInt32> MapValues;

		NparamEx.SetNumericalValues(NicheEx.GetAllValues(MapValues), true);
	}

	// do Item Moyen Cols
	ParamsEx.AddTail().CopyParams(UParams);

	// Do for contre niche Col
	ParamsIt.MoveTo(ParamsEx.GetCount());
	CCalcItemParams & NparamIt = ParamsIt.GetItem();
	CCalcItemParams & NparamEx = ParamsEx.AddTail();
	NparamEx.CopyParams(UParams); 

	CResults NicheResEx(0,IBloc::Not);

	ParamsEx.MoveTo(ItemIndex);
	for(int x=0; x < NbItems ; ParamsEx.MoveNext(), ++x)
	{
		CCalcItemParams & ItemParamEx = ParamsEx.GetItem();
		if(ItemParamEx.GetResults())
		{
			CResults * pResEx =	new CResults(&NicheResEx,IBloc::And);
			pResEx->SetResult(ItemParamEx.GetResults());
			pResEx->SetProbas(ItemParamEx.GetProbas());
			pResEx->SetValues(ItemParamEx.GetNumericalValues());
			NicheResEx.AddResult(pResEx);
		}
	}
	NicheResEx.Calculate();

	if(NicheResEx.GetResult(pResult))
		NparamEx.SetResults(pResult);
	else
		NparamEx.SetResultsD(pResult);

	if(NicheResEx.GetProbas(pProbas))
		NparamEx.SetProbas(pProbas);  
	else
		NparamEx.SetProbasD(pProbas);

	JMap<const JFLT32VECTOR*, JInt32> MapValues;
	NparamEx.SetNumericalValues(NicheResEx.GetAllValues(MapValues), true);

	return true;
}

bool CCrossTable::CalculateDimensionDu(const CONST_LIST_ITEM_PUPITRE & Items, CCalcItemParams & UParams, JList<CCalcItemParams> & ParamsIt, JList<CCalcItemParams> & ParamsDu)
{
	const CBitArray    * pResult = 0;
	const JFLT32VECTOR * pProbas = 0;

	// Add the univers to the Item Params
	CCalcItemParams & upDu = ParamsDu.AddTail();
	upDu.CopyParams(UParams);
	upDu.SetResults(UParams.GetResults());
	upDu.SetProbas (UParams.GetProbas());
	upDu.SetNumericalValues(UParams.GetNumericalValues());

	// Get Niche index and add it
	int NicheIndex = ParamsDu.GetCount();
	ParamsDu.AddTail().CopyParams(UParams); 

	int NbItems   = Items   .GetCount();
	int ItemIndex = ParamsDu.GetCount();

	// Add the items
	for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		ParamsDu.AddTail().CopyParams(UParams);

	JList<CCalcItemParams*> ParamsCp;
	for (ParamsIt.MoveFirst(); ParamsIt.IsValid(); ParamsIt.MoveNext())
		ParamsCp.AddTail() = &ParamsIt.GetItem();

	// Fill in the params for the exclusive items
	ParamsIt.MoveTo(ItemIndex);
	ParamsDu.MoveTo(ItemIndex);
	if(ParamsIt.IsValid())
	{
		for(int x=0; x < NbItems ; ++x, ParamsIt.MoveNext(), ParamsDu.MoveNext())
		{
			// Get the parameters
			CCalcItemParams & NparamIt = ParamsIt.GetItem();
			CCalcItemParams & NparamDu = ParamsDu.GetItem();

			if(NparamIt.GetResults())
			{
				// Set the Header result blocs
				CResults ResDu(0,IBloc::And);

				// Sub result bloc for item
				CResults * pResIt = ResDu.AddResult(new CResults(&ResDu,IBloc::Nop));
				pResIt->SetResult(NparamIt.GetResults());
				pResIt->SetProbas(NparamIt.GetProbas());
				pResIt->SetValues(NparamIt.GetNumericalValues());

				// Sub result bloc for other items
				CResults * pResOt = ResDu.AddResult(new CResults(&ResDu,IBloc::Or));
				ParamsCp.MoveTo(ItemIndex);
				for(int y=0; y < NbItems ; ++y, ParamsCp.MoveNext())
				{
					if(x!=y)
					{
						// Get the parameters
						CCalcItemParams & NparamCp = *ParamsCp.GetItem();
						// Set the Header result blocs
						CResults * pResIt = pResOt->AddResult(new CResults(pResOt,IBloc::Nop));
						pResIt->SetResult(NparamCp.GetResults());
						pResIt->SetProbas(NparamCp.GetProbas());
						pResIt->SetValues(NparamCp.GetNumericalValues());
					}
				}

				// Calculate the exclusives
				ResDu.Calculate();

				if(ResDu.GetResult(pResult))
					NparamDu.SetResults(pResult);
				else
					NparamDu.SetResultsD(pResult);

				if(ResDu.GetProbas(pProbas))
					NparamDu.SetProbas(pProbas);
				else
					NparamDu.SetProbasD(pProbas);

				JMap<const JFLT32VECTOR*, JInt32> MapValues;
				NparamDu.SetNumericalValues(ResDu.GetAllValues(MapValues));
			}
		}
	}

	// Fill in the params for the niche
	ParamsIt.MoveTo(NicheIndex);
	ParamsDu.MoveTo(NicheIndex);
	if(ParamsIt.IsValid() && ParamsDu.IsValid())
	{
		CCalcItemParams & NparamIt = ParamsIt.GetItem();
		CCalcItemParams & NparamDu = ParamsDu.GetItem();

		CResults NicheDu(0,IBloc::Or);

		ParamsDu.MoveTo(ItemIndex);
		for(int x=0; x < NbItems ; ParamsDu.MoveNext(), ++x)
		{
			CCalcItemParams & ItemParamDu = ParamsDu.GetItem();
			if(ItemParamDu.GetResults())
			{
				CResults * pResDu =	new CResults(&NicheDu,IBloc::And);
				pResDu->SetResult(ItemParamDu.GetResults());
				pResDu->SetProbas(ItemParamDu.GetProbas());
				pResDu->SetValues(ItemParamDu.GetNumericalValues());
				NicheDu.AddResult(pResDu);
			}
		}
		NicheDu.Calculate();

		if (!NicheDu.HasProbas())
		{
			if(NicheDu.GetResult(pResult))
				NparamDu.SetResults(pResult);
			else
				NparamDu.SetResultsD(pResult);

			if(NicheDu.GetProbas(pProbas))
				NparamDu.SetProbas(pProbas);
			else
				NparamDu.SetProbasD(pProbas);

			JMap<const JFLT32VECTOR*, JInt32> MapValues;
			NparamDu.SetNumericalValues(NicheDu.GetAllValues(MapValues));
		}
	}

	// do Item Moyen Cols
	ParamsDu.AddTail().CopyParams(UParams);

	// Do for contre niche Col
	ParamsIt.MoveTo(ParamsDu.GetCount());
	CCalcItemParams & NparamIt = ParamsIt.GetItem();
	CCalcItemParams & NparamDu = ParamsDu.AddTail();
	NparamDu.CopyParams(UParams);

	CResults NicheResDu(0,IBloc::Not);

	ParamsDu.MoveTo(ItemIndex);
	for(int x=0; x < NbItems ; ParamsDu.MoveNext(), ++x)
	{
		CCalcItemParams & ItemParamDu = ParamsDu.GetItem();
		if(ItemParamDu.GetResults())
		{
			CResults * pResDu =	new CResults(&NicheResDu,IBloc::And);
			pResDu->SetResult(ItemParamDu.GetResults());
			pResDu->SetProbas(ItemParamDu.GetProbas());
			pResDu->SetValues(ItemParamDu.GetNumericalValues());
			NicheResDu.AddResult(pResDu);
		}
	}
	NicheResDu.Calculate();

	if (!NicheResDu.HasProbas())
	{
		if(NicheResDu.GetResult(pResult))
			NparamDu.SetResults(pResult);
		else
			NparamDu.SetResultsD(pResult);

		if(NicheResDu.GetProbas(pProbas))
			NparamDu.SetProbas(pProbas);  
		else
			NparamDu.SetProbasD(pProbas);

		JMap<const JFLT32VECTOR*, JInt32> MapValues;
		NparamDu.SetNumericalValues(NicheResDu.GetAllValues(MapValues));
	}

	return true;
}

bool CCrossTable::CalculateCells()
{
	// Derniere modif 2.7 track 73
	// m_pDoc->m_AtLeastOneRowAverage = false;
	m_pDoc->m_AtLeastOneRowAverage = true;

	// Barre de progression pour les calculs
	CProgressCtrl & Prog = m_pDoc->GetProgressCtrl(); 

	// calculate NbSteps
	int NbSteps = 0;
	int NbStepCL = 0; 
	int NbStepL  = 1;
	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{
		JList<CCalcItemParams> & RowP	= GetRowCalcParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ColP	= GetColCalcParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & PageP	= GetPageCalcParams(m_UniversParams.GetIndex());

		// AVANT
		// NbSteps += RowP.GetCount() * ColP.GetCount() * PageP.GetCount();

		int NbCol = ColP.GetCount();
		int NbPage = PageP.GetCount();
		int NbStepL   = RowP.GetCount()/5000;
		if (NbStepL == 0) NbStepL = 1;
		int NbRows = RowP.GetCount();

		// Calcul Nb Page Active
		int NbPageActive = 0;
		for(int Page = 0; Page < PageP.GetCount(); ++Page)
		{
			m_Item_Page_Types.MoveTo(Page);
			ITEMTYPE PageType	= m_Item_Page_Types.GetItem();

			CCalcItemParams * cpP			= 0;
			const CBitArray * cpPResults	= 0;
			const JFLT32VECTOR * cpPProbas	= 0;
			const JFLT32VECTOR * cpPValues	= 0;

			// Get the Params for the current page
			if(PageType != ITYPE_MOYEN && PageType != ITYPE_UNIVERSE)
			{
				PageP.MoveTo(Page);
				cpP			= &(PageP.GetItem());

				cpPResults	= cpP->GetResults();

				if(cpPResults == 0)
				{
					continue;
				}
			}
			NbPageActive++;
		}

		// NbSteps += ColP.GetCount() * PageP.GetCount() * NbStepL;
		NbSteps += ColP.GetCount() * NbPageActive * NbStepL;
		NbStepCL = ColP.GetCount() * NbStepL; 
	}

	Prog.SetRange32(0, NbSteps);

	// Voir les modes moyennes éventuellement en ligne ou colonne (pour conversion des vecteurs lignes ou colonnes en binaire)
	bool VecteurColonneEnModeValeursActif = false;
	if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU && m_pDoc->m_ModeMoyenne != m_pDoc->MOY_LIGNE)
		VecteurColonneEnModeValeursActif = true;
		
	bool VecteurLigneEnModeValeursActif = false;
	if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU && m_pDoc->m_ModeMoyenne != m_pDoc->MOY_COLONNE)
		VecteurLigneEnModeValeursActif = true;


	// for each univers
	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{
		// continue;

		// Index de l'univers
		JInt32 InxUniv = m_UniversParams.GetIndex();

		// Tableau des résultats cellules
		MAPCELLRESULTS * TCellResults		= GetCellResults(m_UniversParams.GetIndex());
	
		TCellResults->Reset();

		JList<CCalcItemParams> & RowP		= GetRowCalcParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & ColP		= GetColCalcParams(m_UniversParams.GetIndex());
		JList<CCalcItemParams> & PageP		= GetPageCalcParams(m_UniversParams.GetIndex());

		CCalcItemParams		* cpT			= GetTargetCalcParams(m_UniversParams.GetIndex());
		const CBitArray		* cpTResults	= 0;
		const JFLT32VECTOR	* cpTProbas		= 0;
		const JFLT32VECTOR	* cpTValues		= 0;

		// Parametrages pour calcul des valeurs
		CCalcItemParams ParamsVarMoy;

		if(cpT)
		{
			// Ici on a bien une cible en guise d'univers
			cpTResults	= cpT->GetResults();
			cpTProbas	= cpT->GetProbas();
			// cpTValues	= cpT->GetNumericalValues(false, m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
			cpTValues	= cpT->GetNumericalValues();
		}

		// Attention si Moyenne en mode ligne, le vecteur univers ne sera pas pris avec les valeurs mais en binaire
		if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU && m_pDoc->m_pItemWrapperMoy)
		{
			// Mise en place des paramètres de calcul
			// CCalcItemParams ParamsVarMoy;
			ParamsVarMoy.SetTerrain(m_UniversParams.GetItem()->GetTerrain());
			ParamsVarMoy.SetEchantillonID(1);
			ParamsVarMoy.SetUniversID(m_UniversParams.GetItem()->GetTerrain()->GetDefaultUnivers());
			ParamsVarMoy.SetTypePoidsID(m_UniversParams.GetItem()->GetTypePoidsID());
			ParamsVarMoy.SetPopulationSegment(m_UniversParams.GetItem()->GetPopulationSegment());

			// Récup les segments
			JVector<CSegment *> Segments;
			m_UniversParams.GetItem()->GetTerrain()->GetSegments(Segments);
			JList<JInt32> LstSegIds;
			LstSegIds.Reset();
			for (Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
			{
				LstSegIds.AddTail() = Segments.GetItem()->GetID();
			}
			ParamsVarMoy.SetSegmentsIDs(LstSegIds);

			// Calcul pour définir les valeurs numériques 
			/*
			if (m_pDoc->m_IdValMoyennable < 0)
			{
				// Récupération des valeurs numériques d'un item quanti automatique
				CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeAutomatique);
				CItemConstruit * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(-m_pDoc->m_IdValMoyennable);
				pItem->CalculateEffectif(&ParamsVarMoy); 
				CItemsConstruitsManager::GetInstance()->SetMode(CItemsConstruitsManager::ModeUtilisateur);
			}
			else
			{
				// Récupération des valeurs numériques d'un item quanti utilisateur
				CItemConstruit * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(m_pDoc->m_IdValMoyennable);
				pItem->CalculateEffectif(&ParamsVarMoy); 
			}
			*/

			// 
			if (m_pDoc->m_pItemWrapperMoy)
			{
				m_pDoc->m_pItemWrapperMoy->PtrItem()->CalculateEffectif(&ParamsVarMoy);
			}

			// Récupération des valeurs
			if (!cpT)
			{
				// Cas de l'univers ensemble (il faut récupérer les vecteurs)
				cpTResults	= ParamsVarMoy.GetResults();
				cpTProbas	= ParamsVarMoy.GetProbas();
			}

			// Récupération des valeurs numériques via la variable moyennable
			// cpTValues	= ParamsVarMoy.GetNumericalValues(false, m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
			cpTValues	= ParamsVarMoy.GetNumericalValues();
		}

		// Nombre de pages, colonnes, lignes
		int P,R,C;
		int Pcnt,Rcnt,Ccnt;
		Pcnt = PageP.GetCount();
		Rcnt = RowP.GetCount();
		Ccnt = ColP.GetCount();

		// Lets calculate all the Cells
		// Loop on pages
		for(P = 0; P < Pcnt; ++P)
		{
			m_Item_Page_Types.MoveTo(P);
			ITEMTYPE PageType	= m_Item_Page_Types.GetItem();

			CCalcItemParams * cpP			= 0;
			const CBitArray * cpPResults	= 0;
			const JFLT32VECTOR * cpPProbas	= 0;
			const JFLT32VECTOR * cpPValues	= 0;

			// Get the Params for the current page
			if(PageType != ITYPE_MOYEN && PageType != ITYPE_UNIVERSE)
			{
				PageP.MoveTo(P);
				cpP			= &(PageP.GetItem());

				cpPResults	= cpP->GetResults();

				if(cpPResults == 0)
				{
					continue;
				}

				cpPProbas	= cpP->GetProbas();
				// cpPValues	= cpP->GetNumericalValues(false, m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
				cpPValues	= cpP->GetNumericalValues();
			}


			// Loop on columns
			for(C = 0, m_Item_Col_Types.MoveFirst(), ColP.MoveFirst(); C < Ccnt; ++C, m_Item_Col_Types.MoveNext(), ColP.MoveNext())
			{
				ITEMTYPE ColType = m_Item_Col_Types.GetItem();

				CCalcItemParams * cpC			= 0;
				const CBitArray * cpCResults	= 0;
				const JFLT32VECTOR * cpCProbas	= 0;
				const JFLT32VECTOR * cpCValues	= 0;

				// Get the Params for the current col
				if(ColType != ITYPE_MOYEN && ColType != ITYPE_UNIVERSE)
				{
					// ColP.MoveTo(C);
					cpC			= &(ColP.GetItem());
					cpCResults	= cpC->GetResults();

					if(cpCResults == 0)
					{
						for (int CL = 0; CL < NbStepL; CL++)
							Prog.StepIt(); // plus long mais visuellement plus fluide
						continue;
					}

					if(cpP && cpPResults == cpCResults)
					{
						cpC			= 0;
						cpCResults	= 0;
					}
					else
					{
						cpCResults	= cpC->GetResults();
						cpCProbas	= cpC->GetProbas();

						// Convert le vecteur numérique colonne en vecteur binaire (pour les moyennes)
						if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_COLONNE)
						{
							cpCValues	= cpC->GetNumericalValues(true);
						}
						else
						{
							// En mode moyenne colonne on garde les valeurs numériques
							cpCValues	= cpC->GetNumericalValues(false); // , m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
						}
					}
				}

				// Loop on rows 
				for(R = 0, m_Item_Row_Types.MoveFirst(), RowP.MoveFirst(); R < Rcnt; ++R, m_Item_Row_Types.MoveNext(), RowP.MoveNext())
				{
					// Quel type de row
					ITEMTYPE RowType = m_Item_Row_Types.GetItem();

					if (RowType == ITYPE_NICHE)
					{
						bool Ok = true;

						if(m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE)
						{
							bool Ok2 = true;
						}
					}

					// plus long mais visuellement plus fluide
					if (R % 5000 == 0)
						Prog.StepIt(); 

					if(PageType == ITYPE_MOYEN || ColType == ITYPE_MOYEN || RowType == ITYPE_MOYEN)
					{
						// Get the cell item moyen results object
						CCellItemMoyenResult * cRes			= new CCellItemMoyenResult(P,R,C,this,TCellResults);
						TCellResults->Add(cRes->GetCoord()) = PTR_ICELLRESULT(cRes);
						continue;
					}

					CCalcItemParams		* cpR			= 0;
					const CBitArray		* cpRResults	= 0;
					const JFLT32VECTOR	* cpRProbas		= 0;
					const JFLT32VECTOR	* cpRValues		= 0;
					
					// Get the Params for the current Col
					cpR			= &(RowP.GetItem());
					cpRResults	= cpR->GetResults();

					if(cpRResults == 0)
					{
						continue;
					}

					if((cpP && cpPResults == cpRResults) || (cpC && cpCResults == cpRResults))
					{
						cpR			= 0;
						cpRResults	= 0;
					}
					else
					{
						cpRProbas	= cpR->GetProbas();
						// cpRValues	= cpR->GetNumericalValues(false, m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_INCLUS);
						cpRValues	= cpR->GetNumericalValues();
					}

					// Get the cell results object
					CCellResult * cRes = new CCellResult();

					// Set cell coordinates
					CCoordRowCol Cell(P,R,C);									
					TCellResults->Add(Cell) = PTR_ICELLRESULT(cRes);

					// Copy the universe params 
					cRes->CopyParams(*m_UniversParams.GetItem());

					bool Res = false;
					CResults *pPu = 0;

					// Add Page result object
					if(cpPResults != 0)
					{
						pPu = cRes->AddResult(IBloc::Nop);

						pPu->SetResult(cpPResults);

						pPu->SetProbas(cpPProbas);

						// Modif V 2.6 Sept 2008
						// On ne tient plus compte des valeurs numériques en pages (seulement en ligne / sinon incohérence de calcul)
						// pPu->SetValues(cpPValues);
						Res = true;
					}

					// !!!! OPTIM
					// Add Col result object
					if(cpCResults != 0)
					{
						pPu = cRes->AddResult(IBloc::Nop);
						pPu->SetResult(cpCResults);
						pPu->SetProbas(cpCProbas);

						// Attention à l'univers s celui-ci est un univers "quantitatif", et que l'option Moyenne via Univers est sélectionné, alors les lignes également deviennent
						// des vecteurs binaires
						if (VecteurColonneEnModeValeursActif)
						{
							// Vecteur colonne en mode valeurs actif
							pPu->SetValues(cpCValues);
						}


						Res = true;
					}

					// !!!! OPTIM
					// Add Row result object
					if(cpRResults != 0)
					{
						pPu = cRes->AddResult(IBloc::Nop);
						pPu->SetResult(cpRResults);
						pPu->SetProbas(cpRProbas);

						// Attention à l'univers s celui-ci est un univers "quantitatif", et que l'option Moyenne via Univers est sélectionné, alors les lignes également deviennent
						// des vecteurs binaires
						if (VecteurLigneEnModeValeursActif)
						{
							// Vecteur ligne en mode valeurs actif
							pPu->SetValues(cpRValues);
						}

						Res = true;
					}

					// Add target result object   (???? voir pour les targets si on tient compte des valeurs numériques)
					if(cpTResults != 0)
					{
						pPu = cRes->AddTarget(IBloc::Nop);

						pPu->SetResult(cpTResults);

						pPu->SetProbas(cpTProbas);
						pPu->SetValues(cpTValues);
						Res = true;
					}

					// OPTIM 2.7
					if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)
					{ 
						// Calcul s'il ya lieu des moyennes en mode tableau
						cRes->GetResult(RTYPE_NBCAS, cpTValues, m_pDoc->m_ModeCalculMoyenne);
					}
					else if (m_pDoc->m_ElmtGrandeurMoyennableSel == true && m_pDoc->m_AtLeastOneRowAverage)
					{
						// Calcul des cellules HYPER_OPTIM 2.7 ????????
						if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
						{
							// Calcul effectuée que si ligne est moyennable
							bool MoyQuantiRow		= false;
							GetRowCellMoyennable(CTYPE_HEADER2, R, C, MoyQuantiRow, RowType);

							// Modif Aout 2013 en mode Niche
							if (MoyQuantiRow || m_pDoc->GetNicheQuantifiable())
							{
								// Calcul s'il ya lieu des moyennes en mode ligne
								cRes->GetResult(RTYPE_NBCAS, cpRValues, m_pDoc->m_ModeCalculMoyenne, RowType, m_pDoc->Get_TableStyle() & TSTYLE_EXCLUSIVE);
							}
						}

						else if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
						{
							// Calcul effectuée que si colonne est moyennable
							bool MoyQuantiCol		= false;
							GetColCellMoyennable(CTYPE_HEADER2, R, C, MoyQuantiCol, ColType);

							// Modif Aout 2013 en mode Niche
							if (MoyQuantiCol || m_pDoc->GetNicheQuantifiable())
							{
								// Calcul s'il ya lieu des moyennes en mode colonne
								cRes->GetResult(RTYPE_NBCAS, cpCValues, m_pDoc->m_ModeCalculMoyenne);
							}
						}
					}

					////////////////////////////
					// OPTIM 2.7
					// int CategorieItemRow	= 0;
					// int NbModasItemRow		= 0;
					// bool MoyQuantiRow		= false;
					// GetRowCellCategorie(CTYPE_HEADER2,InxUniv,R,C, CategorieItemRow, NbModasItemRow, MoyQuantiRow);
					bool MoyQuantiRow		= false;
					GetRowCellMoyennable(CTYPE_HEADER2, R, C, MoyQuantiRow, RowType);
					if (MoyQuantiRow)
						m_pDoc->m_AtLeastOneRowAverage = true;
					////////////////////////////
					// FIN OPTIM 2.7

				} // End Loop on rows 
			}
		}
	}
	return true;
}

// bool CCrossTable::CalculateUniverse()
void CCrossTable::CalculateUniverse()
{
	int ColIndex	=	m_pDoc->Get_ColIndex();
	int RowIndex	=	m_pDoc->Get_RowIndex();
	int PageIndex	=	m_pDoc->Get_PageIndex();
	const JList<CCalcItemParams *> & params = m_pDoc->Get_UniverseParams();

	// Here we verify the universes and adjust all universe indexed data
	JList<CCalcItemParams *>		Temp;				// Temporary univers params
	JList< JList<CCalcItemParams> > TempItDim[3];		// Temporary Row, Col and Page params
	JList< JList<CCalcItemParams> > TempExDim[3];		// Temporary Row, Col and Page params
	JList< JList<CCalcItemParams> > TempDuDim[3];		// Temporary Row, Col and Page params
	JList< PTR_MAPCELLRESULTS >		TempCellResults;	// Temporary Cell results
	JList<PTR_CALCITEMPARAMS>		TempTarget;

	for(params.MoveFirst(); params.IsValid(); params.MoveNext())
	{
		// Step to first univers params
		m_Dim_ItemParams[RowIndex].		MoveFirst();
		m_Dim_ItemParams[ColIndex].		MoveFirst();
		m_Dim_ItemParams[PageIndex].	MoveFirst(); 
		m_Dim_ExItemParams[RowIndex].	MoveFirst();
		m_Dim_ExItemParams[ColIndex].	MoveFirst();
		m_Dim_ExItemParams[PageIndex].	MoveFirst();
		m_Dim_DuItemParams[RowIndex].	MoveFirst();
		m_Dim_DuItemParams[ColIndex].	MoveFirst();
		m_Dim_DuItemParams[PageIndex].	MoveFirst(); 
		m_CellResults.					MoveFirst();
		m_UniversTarget.				MoveFirst();

		// Check if the param already exists
		for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
		{
			// We have a matching param lets break out
			if(params.GetItem() == m_UniversParams.GetItem())
				break;

			// Not this one lets step over
			m_Dim_ItemParams[RowIndex].		MoveNext();
			m_Dim_ItemParams[ColIndex].		MoveNext();
			m_Dim_ItemParams[PageIndex].	MoveNext();
			m_Dim_ExItemParams[RowIndex].	MoveNext();
			m_Dim_ExItemParams[ColIndex].	MoveNext();
			m_Dim_ExItemParams[PageIndex].	MoveNext();
			m_Dim_DuItemParams[RowIndex].	MoveNext();
			m_Dim_DuItemParams[ColIndex].	MoveNext();
			m_Dim_DuItemParams[PageIndex].	MoveNext();
			m_CellResults.					MoveNext();
			m_UniversTarget.				MoveNext();
		}

		// Did we have a matching param
		if(m_UniversParams.IsValid() && m_Dim_ItemParams[RowIndex].IsValid() && m_Dim_ItemParams[ColIndex].IsValid() && m_Dim_ItemParams[PageIndex].IsValid() && m_CellResults.IsValid())
		{
			// Yes so keep the old settings and calculations
			TempItDim[RowIndex].AddTail().	Swap(m_Dim_ItemParams[RowIndex].GetItem());
			TempItDim[ColIndex].AddTail().	Swap(m_Dim_ItemParams[ColIndex].GetItem());
			TempItDim[PageIndex].AddTail().	Swap(m_Dim_ItemParams[PageIndex].GetItem());
			TempExDim[RowIndex].AddTail().	Swap(m_Dim_ExItemParams[RowIndex].GetItem());
			TempExDim[ColIndex].AddTail().	Swap(m_Dim_ExItemParams[ColIndex].GetItem());
			TempExDim[PageIndex].AddTail().	Swap(m_Dim_ExItemParams[PageIndex].GetItem());
			TempDuDim[RowIndex].AddTail().	Swap(m_Dim_DuItemParams[RowIndex].GetItem());
			TempDuDim[ColIndex].AddTail().	Swap(m_Dim_DuItemParams[ColIndex].GetItem());
			TempDuDim[PageIndex].AddTail().	Swap(m_Dim_DuItemParams[PageIndex].GetItem());

			TempCellResults.AddTail() = m_CellResults.GetItem();

			if(m_UniversTarget.IsValid())
				TempTarget.AddTail() = m_UniversTarget.GetItem();
		}
		else
		{
			// No so add the new settings
			TempItDim[RowIndex].	AddTail();
			TempItDim[ColIndex].	AddTail();
			TempItDim[PageIndex].	AddTail();
			TempExDim[RowIndex].	AddTail();
			TempExDim[ColIndex].	AddTail();
			TempExDim[PageIndex].	AddTail();
			TempDuDim[RowIndex].	AddTail();
			TempDuDim[ColIndex].	AddTail();
			TempDuDim[PageIndex].	AddTail();
			TempCellResults.		AddTail() = PTR_MAPCELLRESULTS(new MAPCELLRESULTS);
			TempTarget.				AddTail(); 
		}
		Temp.AddTail() = params.GetItem();
	}

	// Lets put the right configuration back into perspective
	m_UniversParams.				Swap(Temp);
	m_Dim_ItemParams[RowIndex].		Swap(TempItDim[RowIndex]);
	m_Dim_ItemParams[ColIndex].		Swap(TempItDim[ColIndex]);
	m_Dim_ItemParams[PageIndex].	Swap(TempItDim[PageIndex]);
	m_Dim_ExItemParams[RowIndex].	Swap(TempExDim[RowIndex]);
	m_Dim_ExItemParams[ColIndex].	Swap(TempExDim[ColIndex]);
	m_Dim_ExItemParams[PageIndex].	Swap(TempExDim[PageIndex]);
	m_Dim_DuItemParams[RowIndex].	Swap(TempDuDim[RowIndex]);
	m_Dim_DuItemParams[ColIndex].	Swap(TempDuDim[ColIndex]);
	m_Dim_DuItemParams[PageIndex].	Swap(TempDuDim[PageIndex]);
	m_CellResults.					Swap(TempCellResults);
	m_UniversTarget.				Swap(TempTarget);

	// return true;
}


// Calculate all the vectors
bool CCrossTable::CalculateItems(bool Redraw)
{
	// Sauve les tous derniers paramètres de calcul
	m_pDoc->SetLastCalculation();

	m_pGrid->LockUpdate(true);
	CProgressCtrl & Prog = m_pDoc->GetProgressCtrl();
	Prog.SetRange32(0, 0);
	Prog.SetStep(1);
	Prog.SetPos(0);

	// Aout 2013 (Je le fais systematiquement ici car je suis sur qu'à chaque nouveau calcul, le flag sera poistionné)
	// En mode Niche il faut verifier que la niche est moyennable (c'est à dire que la composition des elmts lignes et colonnes a au moins 1 elmt quantifiable)
	// Flag pour indiquer si Niche contient des elmts quantifiables (en lignes ou colonnes)
	int BaseColItem		= m_pDoc->Get_BaseColItem();
	int BaseRowItem		= m_pDoc->Get_BaseRowItem();

	/*
	m_Item_Col_Types.MoveTo(BaseColItem);
	m_Item_Row_Types.MoveTo(BaseRowItem);
	if (m_Item_Row_Types.IsValid() && m_Item_Col_Types.IsValid())
	{
		ITEMTYPE RowType = m_Item_Row_Types.GetItem();
		ITEMTYPE ColType = m_Item_Col_Types.GetItem();

		if (m_Item_Row_Types.GetItem() == ITYPE_NICHE || m_Item_Col_Types.GetItem() == ITYPE_NICHE)
		{
			// Voir en ligne
			const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
			m_pDoc->SetNicheQuantifiable(false);
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
			{
				// Récupére la categorie question
				const CItemWrapper *pItemWrapper = Items.GetItem();

				// Vérifie si on a affaire à un item numérisé quantitatif
				const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

				// Cet item est-il moyennable
				if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
				{
					m_pDoc->SetNicheQuantifiable(true);
					
					if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU)
					{
						if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE)
						{
							m_pDoc->m_ModeMoyenne = m_pDoc->MOY_LIGNE;
						}
					}
					
					break;
				}
			}

			// Voir en colonnes
			if (!m_pDoc->GetNicheQuantifiable())
			{
				const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
				for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				{
					// Récupére la categorie question
					const CItemWrapper *pItemWrapper = Items.GetItem();

					// Vérifie si on a affaire à un item numérisé quantitatif
					const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

					// Cet item est-il moyennable
					if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
					{
						m_pDoc->SetNicheQuantifiable(true);
	
						if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU)
						{
							if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE)
							{
								m_pDoc->m_ModeMoyenne = m_pDoc->MOY_COLONNE;
							}
						}
						break;
					}
				}
			}
		}
	}
	*/

	// Update the necessary items
	while (m_pDoc->Get_Calculation())
	{
		if(m_pDoc->Get_Calculation() & CALC_TRANSPOSE)
		{
			ResetSort();
			Transpose();
			m_pDoc->Remove_Calculation(CALC_TRANSPOSE);
			m_pDoc->Add_Calculation(CALC_COLSIZE);
			m_pDoc->Add_Calculation(CALC_ROWSIZE);
			m_pDoc->Add_Calculation(CALC_SURLIGNAGE);
			m_pDoc->Add_Calculation(CALC_GRIDUPDATE);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_UNIVERSE)
		{
			ResetSort();
			CalculateUniverse();
			m_pDoc->Remove_Calculation(CALC_UNIVERSE);
			m_pDoc->Add_Calculation(CALC_CIBLES);
			m_pDoc->Add_Calculation(CALC_PAGES);
			m_pDoc->Add_Calculation(CALC_ROWS); 
			m_pDoc->Add_Calculation(CALC_COLS); 
			m_pDoc->Add_Calculation(CALC_CELLS); 
			continue;
		}
	
		if(m_pDoc->Get_Calculation() & CALC_CIBLES)
		{
			CalculateTargets();
			m_pDoc->Remove_Calculation(CALC_CIBLES);
			m_pDoc->Add_Calculation(CALC_CELLS);
			m_pDoc->Add_Calculation(CALC_PAGESIZE);
			continue;
		}

		// Update the necessary items
		if(m_pDoc->Get_Calculation() & CALC_PAGES)
		{
			ResetSort();
			CalculatePages();
			m_pDoc->Remove_Calculation(CALC_PAGES);
			m_pDoc->Add_Calculation(CALC_PAGESIZE);
			m_pDoc->Add_Calculation(CALC_CELLS);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_PAGESIZE)
		{
			CalcPageSize();
			m_pDoc->Remove_Calculation(CALC_PAGESIZE);
			m_pDoc->Add_Calculation(CALC_GRIDUPDATE);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_COLS)
		{
			ResetSort();
			CalculateCols();
			m_pDoc->Remove_Calculation(CALC_COLS);
			m_pDoc->Add_Calculation(CALC_COLSIZE);
			m_pDoc->Add_Calculation(CALC_CELLS); 
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_COLSIZE)
		{
			CalcColSize();
			m_pDoc->Remove_Calculation(CALC_COLSIZE);
			m_pDoc->Add_Calculation(CALC_GRIDUPDATE);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_ROWS)
		{
			ResetSort();
			CalculateRows();
			m_pDoc->Remove_Calculation(CALC_ROWS);
			m_pDoc->Add_Calculation(CALC_ROWSIZE);
			m_pDoc->Add_Calculation(CALC_CELLS); 
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_ROWSIZE)
		{
			CalcRowSize();
			m_pDoc->Remove_Calculation(CALC_ROWSIZE);
			m_pDoc->Add_Calculation(CALC_GRIDUPDATE);
			continue;
		}
		
		if(m_pDoc->Get_Calculation() & CALC_CELLS)
		{
			// OPTIM 2.7 A FAIRE
			CalculateCells();

			m_pDoc->Remove_Calculation(CALC_CELLS);
			m_pDoc->Add_Calculation(CALC_SURLIGNAGE);
			m_pDoc->Add_Calculation(CALC_SORT);
			m_pDoc->Add_Calculation(CALC_GRIDUPDATE);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_SURLIGNAGE)
		{
			CalculateSurlignage();
			m_pDoc->Remove_Calculation(CALC_SURLIGNAGE);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_GRIDUPDATE)
		{
			SetGridData();
			m_pDoc->Remove_Calculation(CALC_GRIDUPDATE);
			continue;
		}

		if(m_pDoc->Get_Calculation() & CALC_SORT)
		{
			SortColumn();
			m_pDoc->Remove_Calculation(CALC_SORT);
			m_pGrid->SetMergeCellsMode(gxnMergeDelayEval  ,false);
			m_pGrid->DelayMergeCells(CGXRange().SetCols(0,m_Col_Hdr_Type.GetCount()));
			m_pGrid->RedrawRowCol(CGXRange().SetTable(),GX_INVALIDATE);
			continue;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// A VOIR AOUT 2013
	//
	// Recup CumulQuanti pour savoir si on est sur le nouveau mode de calcul
	// JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
	JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;
	JBool Recalcul = false;

	BaseColItem		= m_pDoc->Get_BaseColItem();
	BaseRowItem		= m_pDoc->Get_BaseRowItem();

	m_Item_Col_Types.MoveTo(BaseColItem);
	m_Item_Row_Types.MoveTo(BaseRowItem);
	if (m_Item_Row_Types.IsValid() && m_Item_Col_Types.IsValid())
	{
		ITEMTYPE RowType = m_Item_Row_Types.GetItem();
		ITEMTYPE ColType = m_Item_Col_Types.GetItem();

		if (m_Item_Row_Types.GetItem() == ITYPE_NICHE || m_Item_Col_Types.GetItem() == ITYPE_NICHE)
		{
			// Essayer toujours de se mettre sur la dimension où il n' y a que des quantis (si c'est possible)
			bool AllQuantiRow = true;
			bool AllQuantiCol = true;

			const CONST_LIST_ITEM_PUPITRE &	RowItems =	m_pDoc->GetRowItems();
			m_pDoc->SetNicheQuantifiable(false);
			for(RowItems.MoveFirst(); RowItems.IsValid(); RowItems.MoveNext())
			{
				// Récupére la categorie question
				const CItemWrapper *pItemWrapper = RowItems.GetItem();

				// Vérifie si on a affaire à un item numérisé quantitatif
				const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

				// Cet item est-il moyennable
				if (!m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
				{
					AllQuantiRow = false;
					break;
				}
			}

			const CONST_LIST_ITEM_PUPITRE &	ColItems =	m_pDoc->GetColItems();
			for(ColItems.MoveFirst(); ColItems.IsValid(); ColItems.MoveNext())
			{
				// Récupére la categorie question
				const CItemWrapper *pItemWrapper = ColItems.GetItem();

				// Vérifie si on a affaire à un item numérisé quantitatif
				const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

				// Cet item est-il moyennable
				if (!m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
				{
					AllQuantiCol = false;
					break;
				}
			}

			// Voir en ligne
			const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
			m_pDoc->SetNicheQuantifiable(false);
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
			{
				// Récupére la categorie question
				const CItemWrapper *pItemWrapper = Items.GetItem();

				// Vérifie si on a affaire à un item numérisé quantitatif
				const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

				// Cet item est-il moyennable
				if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
				{
					m_pDoc->SetNicheQuantifiable(true);

					if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU)
					{
						// On changera le mode moyenne pour se mettre sur la dimension quantitative
						if (CumulQuanti)
						{
							if (AllQuantiRow)
							{
								if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_LIGNE)
								{
									m_pDoc->m_ModeMoyenne = m_pDoc->MOY_LIGNE;
									Recalcul = true;
								}
							}
							else if (AllQuantiCol)
							{	
								if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_COLONNE)
								{
									m_pDoc->m_ModeMoyenne = m_pDoc->MOY_COLONNE;
									Recalcul = true;
								}
							}
							if (Recalcul)
							{
								// Remet la cible à calculer
								int Sel = m_pDoc->m_DlgAnalyseur.GetIndexSelCible();
								if(Sel != LB_ERR)
								{
									m_pDoc->Set_SelectedCible(0);
									m_pDoc->Set_SelectedCible(m_pDoc->m_DlgAnalyseur.GetItemCible(Sel));
								}
								else 
									m_pDoc->Set_SelectedCible(0);

								CalculateItems(Redraw);
							}
						}

						/*
						// On permute uniquement si flag CumulQuanti à true (comme pour BAlmétrie)
						if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE && CumulQuanti)
						{
							m_pDoc->m_ModeMoyenne = m_pDoc->MOY_LIGNE;

							// Remet la cible à calculer
							int Sel = m_pDoc->m_DlgAnalyseur.GetIndexSelCible();
							if(Sel != LB_ERR)
							{
								m_pDoc->Set_SelectedCible(0);
								m_pDoc->Set_SelectedCible(m_pDoc->m_DlgAnalyseur.GetItemCible(Sel));
							}
							else 
								m_pDoc->Set_SelectedCible(0);

							CalculateItems(Redraw);
						}
						*/
					}
					break;
				}
			}

			// Voir en colonnes
			if (!m_pDoc->GetNicheQuantifiable())
			{
				const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
				for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				{
					// Récupére la categorie question
					const CItemWrapper *pItemWrapper = Items.GetItem();

					// Vérifie si on a affaire à un item numérisé quantitatif
					const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

					// Cet item est-il moyennable
					if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
					{
						m_pDoc->SetNicheQuantifiable(true);
						
						if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_TABLEAU)
						{
							// On changera le mode moyenne pour se mettre sur la dimension quantitative
							if (CumulQuanti)
							{
								if (AllQuantiCol)
								{
									if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_COLONNE)
									{
										m_pDoc->m_ModeMoyenne = m_pDoc->MOY_COLONNE;
										Recalcul = true;
									}
								}
								else if (AllQuantiRow)
								{	
									if (m_pDoc->m_ModeMoyenne != m_pDoc->MOY_LIGNE)
									{
										m_pDoc->m_ModeMoyenne = m_pDoc->MOY_LIGNE;
										Recalcul = true;
									}
								}
								if (Recalcul)
								{
									// Remet la cible à calculer
									int Sel = m_pDoc->m_DlgAnalyseur.GetIndexSelCible();
									if(Sel != LB_ERR)
									{
										m_pDoc->Set_SelectedCible(0);
										m_pDoc->Set_SelectedCible(m_pDoc->m_DlgAnalyseur.GetItemCible(Sel));
									}
									else 
										m_pDoc->Set_SelectedCible(0);

									CalculateItems(Redraw);
								}
							}
							/*
							// On permute uniquement si falg CumulQuanti à true (comme pour BAlmétrie)
							if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE && CumulQuanti)
							{
								m_pDoc->m_ModeMoyenne = m_pDoc->MOY_COLONNE;

								// Remet la cible à calculer
								int Sel = m_pDoc->m_DlgAnalyseur.GetIndexSelCible();
								if(Sel != LB_ERR)
								{
									m_pDoc->Set_SelectedCible(0);
									m_pDoc->Set_SelectedCible(m_pDoc->m_DlgAnalyseur.GetItemCible(Sel));
								}
								else 
									m_pDoc->Set_SelectedCible(0);

								CalculateItems(Redraw);
							}
							*/
						}
						
						break;
					}
				}
			}
		}
	}
	
	Prog.SetPos(0);
	m_pGrid->LockUpdate(false); 

	// Remet à jour la grid avec les nouvelles infos
	// OPTIM 2.7 ???
	if(Redraw)
		m_pGrid->Redraw();

	return true;
}

void CCrossTable::RedrawGrid()
{
	m_pGrid->Redraw();
}

void CCrossTable::SetGridData()
{
	if(m_pGrid)
	{
		int RowHdrCount = m_Row_Hdr_Type.GetCount();
		int ColHdrCount = m_Col_Hdr_Type.GetCount();
		int Rows = m_pGrid->GetRowCount();
		int Cols = m_pGrid->GetColCount();
		if(Rows >= 2)
		{
			// Hauteur des headers lignes
			m_pDoc->SetRowHdrHeight(0,m_pGrid->GetRowHeight(0));
			m_pDoc->SetRowHdrHeight(1,m_pGrid->GetRowHeight(1));
		}
		if(Cols >= 2)
		{
			// Largeur des headers colonnes
			m_pDoc->SetColHdrWidth(0,m_pGrid->GetColWidth(0));
			m_pDoc->SetColHdrWidth(1,m_pGrid->GetColWidth(1));
		}

		m_pGrid->SetColCount(0,GX_INVALIDATE);	
		m_pGrid->SetRowCount(0,GX_INVALIDATE);
		m_pGrid->SetColCount(m_NbCols-1,GX_INVALIDATE);
		m_pGrid->SetRowCount(m_NbRows-1,GX_INVALIDATE);
		m_pGrid->SetFrozenCols(ColHdrCount+m_FixedCols-1 ,ColHdrCount-1,GX_INVALIDATE);
		m_pGrid->SetFrozenRows(RowHdrCount+m_FixedRows-1 ,RowHdrCount-1,GX_INVALIDATE);

		if(m_NbPages > 1)
		{
			m_pGrid->SetCoveredCellsRowCol(0,0,RowHdrCount-2,ColHdrCount-1,GX_INVALIDATE);
			m_pGrid->SetCoveredCellsRowCol(RowHdrCount-1,0,RowHdrCount-1,ColHdrCount-1,GX_INVALIDATE);

			m_pGrid->SetStyleRange(
				CGXRange(RowHdrCount-1,0,RowHdrCount-1,ColHdrCount-1),
				CGXStyle()
					.SetControl(IDS_AN_CTRL_INDEXDROPDOWNLIST)
					.SetValue((long)m_pDoc->GetCurPage())
					.SetChoiceList(m_PageChoice),
				gxOverride,0,GX_INVALIDATE
			);
		}
		else
		{
			CString TString; CString TString2;
			GetPageCellString(TString,TString2,m_pDoc->GetCurPage());
			m_pGrid->SetCoveredCellsRowCol(0,0,RowHdrCount-1,ColHdrCount-1,GX_SMART);
			m_pGrid->SetStyleRange(
				CGXRange(0,0,RowHdrCount-1,ColHdrCount-1),
				CGXStyle()
					.SetValue(TString)
					.SetControl(GX_IDS_CTRL_HEADER),
				gxOverride,0,GX_INVALIDATE
			);
		}
		m_pGrid->SetMergeCellsMode(gxnMergeDelayEval  ,false);

		// Set Grid row heights
		for(int x = 0 ; x < RowHdrCount ; x++)
		{
			int ValHeight = m_pDoc->GetRowHdrHeight(x);
			m_pGrid->SetRowHeight(x,x,m_pDoc->GetRowHdrHeight(x),0,GX_INVALIDATE);
		}

		// Set Grid column Widths
		for(int x = 0 ; x < ColHdrCount ; x++)
			m_pGrid->SetColWidth(x,x,m_pDoc->GetColHdrWidth(x),0,GX_INVALIDATE );

		bool Switch = false;
		for(int x = ColHdrCount; x < m_NbCols; ++x)
		{
			if(Switch)
			{
				m_pGrid->SetColWidth(x,x,m_ColWidth[1],0,GX_INVALIDATE );
				Switch = false;
			}
			else
			{
				m_pGrid->SetColWidth(x,x,m_ColWidth[0],0,GX_INVALIDATE );
				Switch = true;
			}
		}

		m_pGrid->SetTopRow(RowHdrCount,GX_INVALIDATE);
		m_pGrid->SetLeftCol(ColHdrCount,GX_INVALIDATE);
		m_pGrid->DelayMergeCells(CGXRange().SetCols(0,ColHdrCount));
		m_pGrid->DelayMergeCells(CGXRange().SetRows(0,RowHdrCount));
		m_pGrid->Redraw();
	}
}

// returns the type and coordinates of a given raw row position
CELLTYPE CCrossTable::GetRowCoordinates(int RawRow,  int & Item, int & Universe, int & Calc, int & Value)
{
	// We are on a header cell
	Item		=	0;
	Universe	=	0;
	Calc		=	0;
	Value		=	0;
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	int CalcCount	=	1;
	if(!(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
		CalcCount = CalcTypes.GetCount();

	int RowsCalc	=	m_Result_Row_Count * CalcCount;

	// Check if we are on an items cell
	if(RawRow >= m_Row_Hdr_Type.GetCount())
	{
		// Get real row number removing headers
		Item		=	RawRow-m_Row_Hdr_Type.GetCount();
		Universe	=	Item%m_ItemRowCount;
		Item		/=	m_ItemRowCount;
		Value		=	Universe%m_Result_Row_Count;
		Calc		=	Universe%RowsCalc;
		Universe	/=	RowsCalc;
		Calc		/=	m_Result_Row_Count;

		if(m_pDoc->Get_SortDirection() == 0 || m_SortedRowIndex.GetCount() <= Item)
		{
			m_ItemRowIndex.MoveTo(Item);
			if(m_ItemRowIndex.IsValid())
				Item = m_ItemRowIndex.GetItem();
		}
		else
			Item = m_SortedRowIndex.Item(Item);

		return CTYPE_ITEMCELL;
	}

	// A OPTIMISER 2.7
	m_Row_Hdr_Type.MoveTo(RawRow);
	if(m_Row_Hdr_Type.IsValid())
		return m_Row_Hdr_Type.GetItem();

	return CTYPE_ERROR;
}

// returns the type and coordinates of a given raw row position
CELLTYPE CCrossTable::GetColCoordinates(int RawCol,  int & Item, int & Universe, int & Calc, int & Value)
{
	// We are on a header cell
	Item		=	0;
	Universe	=	0;
	Calc		=	0;
	Value		=	0;
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	int CalcCount	=	1;
	if(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL && CalcTypes.GetCount() > 1)
		CalcCount = CalcTypes.GetCount();

	// Must return the universe, Item, CalcType and ValueType
	int ColsCalc	=	m_Result_Col_Count * CalcCount;

	// Check if we are on an items cell
	if(RawCol >= m_Col_Hdr_Type.GetCount())
	{
		// Get real row number removing headers
		Item		=	RawCol-m_Col_Hdr_Type.GetCount();
		Universe	=	Item%m_ItemColCount;
		Item		/=	m_ItemColCount;
		Value		=	Universe%m_Result_Col_Count;
		Calc		=	Universe%ColsCalc;
		Universe	/=	ColsCalc;
		Calc		/=	m_Result_Col_Count;

		m_ItemColIndex.MoveTo(Item);
		if(m_ItemColIndex.IsValid())
			Item = m_ItemColIndex.GetItem();

		return CTYPE_ITEMCELL;
	}

	// A OPTIMISER 2.7
	m_Col_Hdr_Type.MoveTo(RawCol);
	if(m_Col_Hdr_Type.IsValid())
		return m_Col_Hdr_Type.GetItem();

	return CTYPE_ERROR;
}

int CCrossTable::GetPageItem(int Item)
{
	if(Item < m_NbPages)
	{
		m_ItemPageIndex.MoveTo(Item);
		if(m_ItemPageIndex.IsValid())
			return m_ItemPageIndex.GetItem();
	}
	return 0;
}

// Est ce une ligne moyennable ?? 
BOOL CCrossTable::TypeRowMoyennable(ROWCOL nRow, ROWCOL nCol)
{

	// Style paramétrage tableur	
	UINT TableStyle		=	m_pDoc->Get_TableStyle();

	// Liste des types de grandeurs utilisées
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	// Récupère coordonnées lignes et colonnes
	int Ri,Ru,Rc,Rv;
	CELLTYPE Rt			=	GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);
	int Ci,Cu,Cc,Cv;
	CELLTYPE Ct			=	GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

	// Style du résultat
	RESULTSTYLE ResStyle	=	GetResultStyle(Rv,Cv);

	// Univers en ligne ou en colonne ?
	int Universe = 0;
	if(TableStyle & TSTYLE_TERRAINCOL)
		Universe = Cu;
	else
		Universe = Ru;

	// Grandeurs en ligne ou en colonne ?
	if(TableStyle & TSTYLE_GRANDEURCOL)
		CalcTypes.MoveTo(Cc);
	else
		CalcTypes.MoveTo(Rc);

	// Type de résultat
	RESULTTYPE Type			= CalcTypes.GetItem();

	// Par défaut la cellule ne sera pas en italique
	JBool CellItalique = false;

	// sauf pour les libellés grandeurs Quantité agrégée et valeur moyenne
	if ((Rt == CTYPE_HEADERCALC) || (Ct == CTYPE_HEADERCALC))
	{
		// Type de résultat
		RESULTTYPE Type = CalcTypes.GetItem();
		if (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00)
			return true;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets the row col Grid reference style
BOOL CCrossTable::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle & style, GXModifyType mt, int nType)
{
	// Flag en attente de résoudre le problème survol et coloriage cellule
	JBool ColorCellule = false;

	// Test non dépassement des limites lignes et colonnes du grid
	if(nRow >= m_NbRows || nCol >= m_NbCols)
		return false;

	// Liste des types de grandeurs utilisées
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	// Style paramétrage tableur	
	UINT TableStyle		=	m_pDoc->Get_TableStyle();

	// Index Ligne et Colonne de base
	int BaseColItem		=	m_pDoc->Get_BaseColItem();
	int BaseRowItem		=	m_pDoc->Get_BaseRowItem();

	// Index dimension page courante
	int Pi				=	GetPageItem(m_pDoc->GetCurPage());

	// Récupère coordonnées lignes et colonnes
	int Ri,Ru,Rc,Rv;
	CELLTYPE Rt			=	GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);
	int Ci,Cu,Cc,Cv;
	CELLTYPE Ct			=	GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

	// Style du résultat
	RESULTSTYLE ResStyle	=	GetResultStyle(Rv,Cv);

	
	// Univers en ligne ou en colonne ?
	int Universe = 0;
	if(TableStyle & TSTYLE_TERRAINCOL)
		Universe = Cu;
	else
		Universe = Ru;

	// Grandeurs en ligne ou en colonne ?
	if(TableStyle & TSTYLE_GRANDEURCOL)
		CalcTypes.MoveTo(Cc);
	else
		CalcTypes.MoveTo(Rc);

	// Type de résultat
	RESULTTYPE Type			= CalcTypes.GetItem();

	// Par défaut la cellule ne sera pas en italique
	JBool CellItalique = false;

	// sauf pour les libellés grandeurs Quantité agrégée et valeur moyenne
	if ((Rt == CTYPE_HEADERCALC) || (Ct == CTYPE_HEADERCALC))
	{
		// Type de résultat
		RESULTTYPE Type = CalcTypes.GetItem();
		if (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00)
			CellItalique = true;
	}

	// Type de bordure des cellules
	style.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL));

	// Initialisation des chaines de caractères à récupérer
	CString TString			= _T(" ");
	CString TString2		= _T(" ");

	// Infos cellule item quantitatif
	int CategorieItem		= 0;
	int NbModasItemRow		= 0;
	int NbModasItemCol		= 0;
	JBool ItemQuantitatif	= false;

	// Y a t-il plusieurs terrains sélectionnés
	JUnt32 NbRowTerrains = 0;
	JUnt32 NbColTerrains  = 0;

	if (m_pDoc->Get_NbSelectedTerrains() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL)
			NbRowTerrains = 1;
		else
			NbColTerrains = 1;
	}
		
	// Y a t-il des grandeurs en header ligne
	JUnt32 NbRowGrandeurs = 0;
	JUnt32 NbColGrandeurs = 0;
	if (m_pDoc->Get_NbSelectedGrandeurs() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
			NbRowGrandeurs = 1;
		else
			NbColGrandeurs = 1;
	}
	
	// Traitement des différents type de ligne
	switch(Rt)
	{	
		// Traitement 1er ligne header
		case CTYPE_HEADER1:
		{
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Traitement des colonnes
			switch(Ct)
			{
				// Les colonnes headers
				case CTYPE_HEADER1:
				case CTYPE_HEADER2:
				case CTYPE_HEADERTERRAIN:
				case CTYPE_HEADERCALC:
				{
					// On vérifie qu'on a au moins une page de valide
					if(m_NbPages)
					{
						// Récupère les strings
						GetPageCellString(TString,TString2,Pi);
						
						// Entete headers colonnes
						style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
							.SetValue(TString)
							.SetHorizontalAlignment(DT_CENTER)
							.SetVerticalAlignment(DT_VCENTER)
							.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
							.SetEnabled(false)
							.SetDraw3dFrame(gxFrameNormal)
							.SetFont(CGXFont(CATPColors::GetFontName())
								.SetSize(CATPColors::GetFontStaticSize())
								.SetItalic(ItemQuantitatif && CellItalique)
								.SetBold(false))
							;
						return true;
					}
					break;
				}

				// Les colonnes correspondant aux cellules items
				case CTYPE_ITEMCELL:
				{
					// Récupère le texte associé
					GetColCellString(TString,Rt,Universe,Ci,Cc);

					// Traitement spécial font pour les entetes items lignes quantitatifs (header col 1 et 2)
					bool MoyQuantiCol = false;
					GetColCellCategorie(Rt,Universe,Ci,Cc, CategorieItem, NbModasItemCol, MoyQuantiCol);

					// Indique si l'item est quantitatif  (pour affichage en italique)
					if ((Rt == CTYPE_HEADER1 || Rt == CTYPE_HEADER2) && (CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure) && NbModasItemCol <=1)
						ItemQuantitatif = true;

					// Style des entetes headers colonnes
					style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
						.SetValue(TString)
						.SetHorizontalAlignment(DT_CENTER)
						.SetVerticalAlignment(DT_VCENTER)
						.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
						.SetEnabled(false)
						.SetDraw3dFrame(gxFrameNormal)
						.SetFont(CGXFont(CATPColors::GetFontName())
							.SetSize(CATPColors::GetFontStaticSize())
							.SetItalic(ItemQuantitatif && CellItalique)
							.SetBold(false))
						;
					return true;
				}
				break;
			}

			// Sortie traitement ligne header 1
			break;
		}

		// Traitement des headers lignes intermédiares
		case CTYPE_HEADERTERRAIN:
		case CTYPE_HEADERCALC:
		case CTYPE_HEADER2:
		{
			// Surligne la colonne header cellule survolée
			if (NbRowTerrains == 0 && NbRowGrandeurs == 0)
			{
				// Pas de terrains ni grandeurs en entete ligne
				if (Rt == CTYPE_HEADER2 && nCol >= (m_ColCurHeader)  && nCol < (m_ColCurHeader + m_ItemColCount))
					style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
			}
			else if (NbRowTerrains > 0 && NbRowGrandeurs == 0)
			{
				// Seulement terrains en entete ligne
				if (Rt == CTYPE_HEADERTERRAIN && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count)))
				{
					style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
				}
			}
			else if (NbRowTerrains == 0 && NbRowGrandeurs > 0)
			{
				// Seulement grandeurs en entete ligne
				if (Rt == CTYPE_HEADERCALC && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count)))
					style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
			}
			else
			{
				// Terrains et grandeurs en entete ligne (on surligne l'entete des grandeurs)
				if (Rt == CTYPE_HEADERCALC && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count)))
					style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
			}

			// Traitement des colonnes
			switch(Ct)
			{
				// Les colonnes correspondant aux cellules items
				case CTYPE_ITEMCELL:
				{
					// Récupère le texte associé
					GetColCellString(TString,Rt,Universe,Ci,Cc);

					// Traitement spécial font pour les entetes items lignes quantitatifs (header col 1 et 2)
					bool MoyQuantiCol = false;
					GetColCellCategorie(Rt,Universe,Ci,Cc, CategorieItem, NbModasItemCol,MoyQuantiCol);

					// Indique si l'item est quantitatif  (pour affichage en italique)
					if ((Rt == CTYPE_HEADER1 || Rt == CTYPE_HEADER2) && (CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure) && NbModasItemCol <=1)
						ItemQuantitatif = true;

					// Style des entetes headers colonnes
					style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
						.SetValue(TString)
						.SetHorizontalAlignment(DT_CENTER)
						.SetVerticalAlignment(DT_VCENTER)
						.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
						.SetEnabled(false)
						.SetDraw3dFrame(gxFrameNormal)
						.SetFont(CGXFont(CATPColors::GetFontName())
							.SetSize(CATPColors::GetFontStaticSize())
							.SetItalic(ItemQuantitatif && CellItalique)
							.SetBold(false))
						;

					// On teste si l'on se trouve sur des élmts triés
					// MODIF A FAIRE (RECUP ELMT TRI) 
					m_Item_Col_Types.MoveTo(Ci);
					if(m_pDoc->Get_SortDirection() == 0 || Ci != m_pDoc->Get_SortCol() || !m_Item_Col_Types.IsValid())
						return true;

					if(m_Item_Col_Types.GetItem() & ITYPE_ITEM && nRow != m_Row_Hdr_Type.GetCount()-1)
						// Return if not on last row header
						return true;

					if((TableStyle & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1 && Universe != m_pDoc->Get_SortUniv())
							break;

					// Traitement des élmts triés
					switch(Rt)
					{
						case CTYPE_HEADERCALC:
							if(!CalcTypes.IsValid() || !(m_pDoc->Get_SortCalc() & CalcTypes.GetItem()))
								break;

						case CTYPE_HEADER2:
						case CTYPE_HEADERTERRAIN:

							// Mise en évidence de la colonne triée
							if(nRow == m_Row_Hdr_Type.GetCount()-1)
							 	style.SetInterior(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR));

						break;
			
						default: 
							if((TableStyle & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1)
								break;
							if((TableStyle & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
								break;

							// Marquage
							style.SetInterior(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR));
						break;
					}
					return true;
				}

				break;
			}
		}
		// Sortie traitement des autres headers lignes
		break;

		// Traitement des autres lignes (en dehors des headers)
		case CTYPE_ITEMCELL:
		{
			// Récupère le texte associé
			GetRowCellString(TString,Ct,Universe,Ri,Rc);

			// Validation écriture en italique
			CellItalique = (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);

			// Traitement spécial font pour les entetes items lignes quantitatifs (header col 1 et 2)
			bool MoyQuantiRow = false;
			GetRowCellCategorie(Ct,Universe,Ri,Rc, CategorieItem, NbModasItemRow, MoyQuantiRow);

			// Indique si l'item est quantitatif  (pour affichage en italique)
			if ((Ct == CTYPE_HEADER1 || Ct == CTYPE_HEADER2) && (CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure) && NbModasItemRow <=1)
				ItemQuantitatif = true;
			else if (MoyQuantiRow)
				ItemQuantitatif = true;

			// En mode tableau et sur les cellules moyennes, on force ItemQuantitatif à true 
			if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU && (Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00) && Ct != CTYPE_HEADER1 && Ct!= CTYPE_HEADER2)
				ItemQuantitatif = true;

			// Ecriture libellé entete type de calcul en italique si quantité agrégé ou moyenne
			if (Ct == CTYPE_HEADERCALC && (Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00 || Type == RTYPE_QUANTITY_000))
				ItemQuantitatif = true;

			// Ajout OCTOBRE 2913
			const CItemConstruit *pItemConstruit = m_pDoc->Get_SelectedCible();
			if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
			{
				ItemQuantitatif = true;
			}

			// Style des entetes headers lignes
			/*
			style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
				.SetValue(TString)
				.SetHorizontalAlignment(DT_CENTER)
				.SetVerticalAlignment(DT_VCENTER)
				.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
				.SetEnabled(false)
				.SetDraw3dFrame(gxFrameNormal)
				.SetFont(CGXFont(CATPColors::GetFontName())
				.SetSize(CATPColors::GetFontStaticSize())
				.SetItalic(ItemQuantitatif && CellItalique)
				.SetBold(false))
				;
				*/
			// AVANT
			style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
				.SetValue(TString)
				.SetHorizontalAlignment(DT_CENTER)
				.SetVerticalAlignment(DT_VCENTER)
				.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
				.SetEnabled(false)
				.SetDraw3dFrame(gxFrameNormal)
				.SetFont(CGXFont(CATPColors::GetFontName())
					.SetSize(CATPColors::GetFontStaticSize())
					.SetItalic(ItemQuantitatif && CellItalique)
					.SetBold(false))
				;
			
			// Surligne la ligne header ligne survolée
			//if (Ct == CTYPE_HEADER2 && (nRow >= (m_RowCurHeader + NbRowTerrains + NbRowGrandeurs) &&  nRow < (m_RowCurHeader + NbRowTerrains + NbRowGrandeurs + m_ItemRowCount)))
			//	style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

			////////////////////////////////////////////////////////////// A SIMPLIFIER /////////////////////////////////////////////////////////////
			//
			switch (Ct)
			{
				// Traitement des headers colonnes
				case CTYPE_HEADER1:
				case CTYPE_HEADER2:
				case CTYPE_HEADERTERRAIN :
				case CTYPE_HEADERCALC:
				{
					// Récup texte associé
					GetRowCellString(TString,Ct,Universe,Ri,Rc);

					// A VOIR ?????
					//style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleColHeader)
					//	.SetValue(TString)
					//	.SetHorizontalAlignment(DT_CENTER)
					//	.SetVerticalAlignment(DT_VCENTER)
					//	.SetMergeCell(GX_MERGE_HORIZONTAL|GX_MERGE_VERTICAL|GX_MERGE_COMPVALUE)
					//	.SetEnabled(false)
					//	.SetDraw3dFrame(gxFrameNormal)
					//	.SetFont(CGXFont(CATPColors::GetFontName())
					//		.SetSize(CATPColors::GetFontStaticSize())
					//		.SetItalic(ItemQuantitatif && (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE || m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE))
					//		.SetBold(false))
					//	;

					// Surligne la ligne header cellule survolée
					//if (Ct == CTYPE_HEADERCALC && (nRow >= (m_RowCurHeader  + NbRowTerrains + NbRowGrandeurs) &&  nRow < (m_RowCurHeader  + NbRowTerrains + NbRowGrandeurs + m_ItemRowCount)))
					//	style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

					// Surligne la ligne header cellule survolée
					if (NbColTerrains == 0 && NbColGrandeurs == 0)
					{
						// Pas de terrains ni grandeurs en entete colonne
						if (Ct == CTYPE_HEADER2 && nRow >= (m_RowCurHeader)  && nRow < (m_RowCurHeader + m_ItemRowCount))
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
					}
					else if (NbColTerrains > 0 && NbColGrandeurs == 0)
					{
						// Seulement terrains en entete colonne
						if (Ct == CTYPE_HEADERTERRAIN && (nRow >= m_RowCurHeader && (nRow < m_RowCurHeader + m_Result_Row_Count)))
						{
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
						}
					}
					else if (NbColTerrains == 0 && NbColGrandeurs > 0)
					{
						// Seulement grandeurs en entete colonne
						if (Ct == CTYPE_HEADERCALC && (nRow >= m_RowCurHeader && (nRow < m_RowCurHeader + m_Result_Row_Count)))
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
					}
					else
					{
						// Terrains et grandeurs en entete colonnne (on surligne l'entete des grandeurs)
						if (Ct == CTYPE_HEADERCALC && (nRow >= m_RowCurHeader && (nRow < m_RowCurHeader + m_Result_Row_Count)))
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
					}
					
					return true;
				}
				break;

				// Traitement des autres cellules
				case CTYPE_ITEMCELL:
				{
					// Type de résultat
					RESULTTYPE Type = CalcTypes.GetItem();

					CellItalique = (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);

					// Croisement ligne/colonne avec un élmt quantitatif > on met le résultat cellule en italique
					int CategorieItemRow = 0;
					int CategorieItemCol = 0;
					bool MoyQuantiRow = false;
					bool MoyQuantiCol = false;
					GetRowCellCategorie(CTYPE_HEADER2,Universe,Ri,Rc, CategorieItemRow, NbModasItemRow, MoyQuantiRow);
					GetColCellCategorie(CTYPE_HEADER2,Universe,Ci,Cc, CategorieItemCol, NbModasItemCol, MoyQuantiCol);


					// Croisement Item défini comme quantitatif si item ligne et/ou item colonne quantitatif
					ItemQuantitatif = (CategorieItemRow == CQuestion::CATEGORIE::Quantitative_Assim	|| 
						              CategorieItemRow == CQuestion::Quantitative_Pure				||
									  CategorieItemCol == CQuestion::CATEGORIE::Quantitative_Assim  || 
									  CategorieItemCol == CQuestion::Quantitative_Pure) &&
									  (NbModasItemRow <=1 && NbModasItemCol <= 1) &&
									  (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);


					// en ++
					if (MoyQuantiRow || MoyQuantiCol || m_pDoc->GetForceQuantiCalcul()) 
						ItemQuantitatif = true;

					// Surlignage des résultats croisés quanti (il faut que l'on ait en ligne et en colonne des items quantis + type de calcul Quantité agrégée ou Valeur moyenne)
					bool ItemQuantitatifSurligne = (CategorieItemRow == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItemRow == CQuestion::Quantitative_Pure) &&
												   (CategorieItemCol == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItemCol == CQuestion::Quantitative_Pure) &&
												   (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);


					// En mode tableau et sur les cellules moyennes, on force ItemQuantitatif à true 
					if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU 
						&& (Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00 || Type == RTYPE_QUANTITY_000) && Ct != CTYPE_HEADER1 && Ct!= CTYPE_HEADER2)
						ItemQuantitatif = true;	

					// Ajout OCTOBRE 2913
					const CItemConstruit *pItemConstruit = m_pDoc->Get_SelectedCible();
					if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
					{
						ItemQuantitatif = true;
					}

					// Default styles for the cell
					style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleStandard)
						.SetHorizontalAlignment(DT_RIGHT)
						.SetVerticalAlignment(DT_VCENTER)
						.SetMergeCell(GX_MERGE_DISABLED)
						.SetEnabled(false)
						.SetValue(TString)
						.SetFont(CGXFont(CATPColors::GetFontName())
							.SetSize(CATPColors::GetFontStaticSize())
							.SetItalic(ItemQuantitatif && CellItalique)
							.SetUnderline(ItemQuantitatifSurligne));

					if(ResStyle & RSTYLE_RESULT)
						style.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));

					if(Ri == BaseRowItem || Ci == BaseColItem)
					{
						// Marquage cellules base 100
						style.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR));
						style.SetTextColor(CATPColors::GetColorBlack());
					}
					else
					{
						// Test si cellule à surligner (inclu dans mpat total des cellules à surligner)
						CKeyRowCol KeyRowCol;
						KeyRowCol.m_NoRow = nRow;  // Ri;	 // nRow
						KeyRowCol.m_NoCol = nCol;  // Ci;  // nCol; 
						m_pDoc->m_MapCellTotSurlign.MoveTo(KeyRowCol);
						if (m_pDoc->m_MapCellTotSurlign.IsValid()) 
						{
							// Cellule à surligner, on récupère la couleur de surlignage
							const COLORREF &ColorSurlign = m_pDoc->m_MapCellTotSurlign.GetItem(); 

							// On surligne la cellule
							style.SetInterior(ColorSurlign);
						}
						else
						{
							// Cellule normale  rien de spécial
							style.SetInterior(CATPColors::GetColorWhite());//Light(CATPColors::COLORANALYSEUR));
						}
					}

					// Encadre la cellule survolée (meilleure visualisation)
					if (ColorCellule && ((nRow >= m_RowCurHeader &&  nRow < (m_RowCurHeader + m_Result_Row_Count)) && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count))))
					{
						// Marquage cellule survolée couleur claire
						style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

						// Séparation des cellules par une bordure point par point
						style.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_DOT).SetColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR))
															  .SetWidth(1));
					}
					else
					{
						// Séparation cellule ligne pleine
						style.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL));
					}
						
					// Bordure pour les extrèmes
					if(Rv == m_Result_Row_Count-1 && nRow >= (m_NbRows - m_Result_Row_Count - 1))
					{
						style.SetBorders(gxBorderBottom, CGXPen().SetStyle(PS_SOLID).SetColor(CATPColors::GetColorLight(CATPColors::GREY_MOUSE)));//RGB(0,0,0)));
					}

					// Dessine trait au dessus du bloc cellule résultat
					if(Rv == 0)
					{
						style.SetBorders(gxBorderTop, CGXPen().SetStyle(PS_SOLID).SetColor(CATPColors::GetColorLight(CATPColors::GREY_MOUSE)));//RGB(0,0,0)));
					}

					// Dessine trait en fin de derniere colonne
					if(Cv == m_Result_Col_Count-1)
					{
						style.SetBorders(gxBorderRight, CGXPen().SetStyle(PS_SOLID).SetColor(CATPColors::GetColorLight(CATPColors::GREY_MOUSE)));//RGB(0,0,0)));
					}

					if((m_pDoc->Get_ResultStyle()&ResStyle) == 0 || GetCellResults(Universe) == 0 || !CalcTypes.IsValid())
						break;

					// RESULTTYPE Type = CalcTypes.GetItem();
					double Result = 0.0;

					// Flag si division par zéro
					bool DivZero			= false;

					// Flag si marge d'erreur atteinte
					bool MargeErreurDepasse = false;

					// Indication nombre de cas
					double NbCas = 0.0;

					// Marge d'erreur
					double MargeErreur = 0.0;

					// Intervalle confiance
					double IntervalleConfiance	= 0.0;

					if(!GetCellValue(Result, Type, Ri, Ci, Universe, ResStyle, Pi, BaseColItem, BaseRowItem, DivZero, NbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance))
						break;

					if(m_pDoc->Get_SortDirection() > 0 && Ci == m_pDoc->Get_SortCol() && Type == m_pDoc->Get_SortCalc() && Universe == m_pDoc->Get_SortUniv() && ResStyle == m_pDoc->Get_SortedStyle())
					{
						m_Item_Row_Types.MoveTo(Ri);
					}

					// Selon le style de résultat, on écrit avec une couleur différente
					switch(ResStyle)
					{
						case RSTYLE_HORIZONTAL:
						case RSTYLE_VERTICAL:
						case RSTYLE_GLOBAL:
						{
							// AVERTISSEMENT SEUIL : Cas hors limite calcul moteur
							if(Ri != BaseRowItem && Ci != BaseColItem)
							{
								if(NbCas <= (double)m_pDoc->Get_SeuileAvert() && NbCas > (double)m_pDoc->Get_SeuileLimit())
								{
									// Si marge d'erreur demandée
									//if (m_pDoc->CalculMargeErreur() == true)
										// Marquage en couleur rouge						
									//	style.SetTextColor(CATPColors::GetColorRed());

									// Marquage en couleur rouge						
									style.SetTextColor(CATPColors::GetColorRed());
								}
								else
								{
									// Couleur résult différente selon type de résult
									if (ResStyle == RSTYLE_HORIZONTAL)
									{
										// Résultat horizontal H%
										style.SetTextColor(CATPColors::GetColorResult3());
									}
									else if (ResStyle == RSTYLE_VERTICAL)
									{
										// Résultat vertical V%
										style.SetTextColor(CATPColors::GetColorResult2());
									}
									else if (ResStyle == RSTYLE_GLOBAL)
									{
										// Autres résultats 
										style.SetTextColor(CATPColors::GetColorResult4());
									}
								}
							}
							else
							{
								// Couleur résult différente selon type de résult
								if (ResStyle == RSTYLE_HORIZONTAL)
								{
									style.SetTextColor(CATPColors::GetColorResult3());
								}
								else if (ResStyle == RSTYLE_VERTICAL)
								{
									style.SetTextColor(CATPColors::GetColorResult2());
								}
								else if (ResStyle == RSTYLE_GLOBAL)
								{
									style.SetTextColor(CATPColors::GetColorResult4());
								}
							}
						}
						break;

						// Cas des effectifs (avec marge d'erreur compris entre m_SeuilMargeErreurUtil et m_SeuilMargeErreurAppli
						case RSTYLE_RESULT:
						{
							// Marquage si marge d'erreur compris entre seuil utilsateur et seuil appli
							if (MargeErreur >= (double)m_pDoc->Get_Seuil_MargeErreur_Util() 
								&& MargeErreur <= (double)m_pDoc->Get_Seuil_MargeErreur_Appli())
							{
								// Marquage en rouge si marge d'erreur demandée
								if (m_pDoc->CalculMargeErreur() == true)
								{
									style.SetTextColor(CATPColors::GetColorRed());
								}
								else
									style.SetTextColor(CATPColors::GetColorResult1());
							}
							else
							{
								// Tracé couleur de base pour les autres case
								style.SetTextColor(CATPColors::GetColorResult1());
							}

							/* MAi 2014
							if (Type == RTYPE_NBCAS)
							{
								if(NbCas <= (double)m_pDoc->Get_SeuileAvert() && NbCas > (double)m_pDoc->Get_SeuileLimit())
								{
									style.SetTextColor(CATPColors::GetColorRed());
								}
							}
							*/
						}
						break;
					}

					// Attention aux limites pour indice corrélation
					if(ResStyle & RSTYLE_GBL_CORRELATION)
					{
						// AVERTISSEMENT SEUIL en mode Corrélation -- Cas hors limite calcul moteur
						if(Ri != BaseRowItem && Ci != BaseColItem)
						{
							// Nombre de cas en horizontal et vertical
							double  NbCasHorz				= 0.0;
							double  NbCasVert				= 0.0;

							// Variables résultat temporaires pour calcul NbCas Horz et Vert
							double	ResultH					= 0.0;
							double	ResultV					= 0.0;
							bool	MargeErreurDepasseH		= false;
							bool	MargeErreurDepasseV		= false;
							double	MargeErreurH			= 0.0;
							double	MargeErreurV			= 0.0;
							double	IntervalleConfianceH	= 0.0;
							double	IntervalleConfianceV	= 0.0;
							bool	DivZeroH				= false;
							bool	DivZeroV				= false;

							// Couleur résult différente selon type de résult
							style.SetTextColor(CATPColors::GetColorResult4());
							
							if(GetCellValue(ResultH, Type, Ri, Ci, Universe, RSTYLE_HORIZONTAL, Pi, BaseColItem, BaseRowItem, DivZeroH, NbCasHorz, MargeErreurDepasseH,MargeErreurH,IntervalleConfianceH)
								&& GetCellValue(ResultV, Type, Ri, Ci, Universe, RSTYLE_VERTICAL, Pi, BaseColItem, BaseRowItem, DivZeroV, NbCasVert, MargeErreurDepasseV,MargeErreurV,IntervalleConfianceV))
							{
								// Les 2 indices en dessous du seuil limite
								if (NbCasHorz <= (double)m_pDoc->Get_SeuileLimit() && NbCasVert <= (double)m_pDoc->Get_SeuileLimit())
									// Marquage cellue #
									MargeErreurDepasse = true;

								// Les 2 indices sont compris entre seuil avertissement et seuil limite
								else if((NbCasHorz <= (double)m_pDoc->Get_SeuileAvert() && NbCasHorz > (double)m_pDoc->Get_SeuileLimit()) &&
										(NbCasVert <= (double)m_pDoc->Get_SeuileAvert() && NbCasVert > (double)m_pDoc->Get_SeuileLimit()))
								{
									// si marge d'erreur demandée
									// if (m_pDoc->CalculMargeErreur() == true)
									//	// Couleur font en rouge
									//	style.SetTextColor(CATPColors::GetColorRed());

									// Couleur font en rouge
									style.SetTextColor(CATPColors::GetColorRed());
								}

								// 1 des 2 indices en dessous du seuil
								else if (NbCasHorz <= (double)m_pDoc->Get_SeuileLimit() || NbCasVert <= (double)m_pDoc->Get_SeuileLimit())
								{
									// Si marge d'erreur demandée
									// if (m_pDoc->CalculMargeErreur() == true)
									// 	// Couleur font en rouge
									//	style.SetTextColor(CATPColors::GetColorRed());

									// Couleur font en rouge
									style.SetTextColor(CATPColors::GetColorRed());
								}	
							}
						}
					}
					
					// Récupère le résultat à afficher dans la cellule
					GetFormattedResult(TString, Result, ResStyle, Type, Ri, Ci, BaseRowItem, BaseColItem, DivZero, NbCas, MargeErreurDepasse, ItemQuantitatif, MoyQuantiRow, MoyQuantiCol);
					style.SetValue(TString); 

					// Cellule définie, on pourra afficher la ligne	
					if (TString != "*")
					{
						m_pGrid->m_MapRowMoyenneAffich.MoveTo(nRow);
						if (m_pGrid->m_MapRowMoyenneAffich.IsValid())
						{
							// Si au moins 1 valeur sur la ligne >> la ligne doit être affichée
							// bool &Affich = m_pGrid->m_MapRowMoyenneAffich.GetItem();
							// Affich = true;
							m_pGrid->m_MapRowMoyenneAffich.GetItem() = true;
						}
					}
					else
					{
						int NonAffich = 1;
					}
					return true;
				}
				break;
			}
		}
		break;

		////////////////////////////////////////////////////////////////////// FIN A SIMPLIFIER ////////////////////////////////////////////////////////

		// Traitement erreur
		case CTYPE_ERROR:
			return false;
	}

	// Sinon return false
	return false;
}


// A VOIR OPTIM EVENTUELLE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Idem GetStyleRowCol OPTIM 2.7 // Gets the rows cols Grid reference style
BOOL CCrossTable::GetTotStyleRowCol(ROWCOL nRowBegin, ROWCOL nRowEnd, ROWCOL nColBegin, ROWCOL nColEnd, CGXStyle & style, GXModifyType mt, int nType)
{
	// Flag en attente de résoudre le problème survol et coloriage cellule
	JBool ColorCellule = false;

	// Test non dépassement des limites lignes et colonnes du grid
	if(nRowBegin >= m_NbRows || nRowEnd >= m_NbRows  || nColBegin >= m_NbCols || nColEnd >= m_NbCols)
		return false;

	// Liste des types de grandeurs utilisées
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	// Style paramétrage tableur	
	UINT TableStyle		=	m_pDoc->Get_TableStyle();

	// Index Ligne et Colonne de base
	int BaseColItem		=	m_pDoc->Get_BaseColItem();
	int BaseRowItem		=	m_pDoc->Get_BaseRowItem();

	// Index dimension page courante
	int Pi				=	GetPageItem(m_pDoc->GetCurPage());

	m_pGrid->m_MapRowMoyenneAffich.MoveFirst();

	ROWCOL nRow;
	for (nRow = nRowBegin, m_pGrid->m_MapRowMoyenneAffich.MoveFirst(); 
		 nRow <= nRowEnd;
		 nRow++, m_pGrid->m_MapRowMoyenneAffich.MoveNext())
	{
		for (ROWCOL nCol = nColBegin; nCol <= nColEnd; nCol++)
		{
			// Récupère coordonnées lignes et colonnes
			int Ri,Ru,Rc,Rv;
			CELLTYPE Rt			=	GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);
			int Ci,Cu,Cc,Cv;
			CELLTYPE Ct			=	GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

			// Style du résultat
			RESULTSTYLE ResStyle	=	GetResultStyle(Rv,Cv);

			// Univers en ligne ou en colonne ?
			int Universe = 0;
			if(TableStyle & TSTYLE_TERRAINCOL)
				Universe = Cu;
			else
				Universe = Ru;

			// Grandeurs en ligne ou en colonne ?
			if(TableStyle & TSTYLE_GRANDEURCOL)
				CalcTypes.MoveTo(Cc);
			else
				CalcTypes.MoveTo(Rc);

			// Type de résultat
			RESULTTYPE Type			= CalcTypes.GetItem();

			// Par défaut la cellule ne sera pas en italique
			JBool CellItalique = false;

			// sauf pour les libellés grandeurs Quantité agrégée et valeur moyenne
			if ((Rt == CTYPE_HEADERCALC) || (Ct == CTYPE_HEADERCALC))
			{
				// Type de résultat
				RESULTTYPE Type = CalcTypes.GetItem();
				if (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00)
					CellItalique = true;
			}

			// Type de bordure des cellules
			style.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL));

			// Initialisation des chaines de caractères à récupérer
			CString TString			= _T(" ");
			CString TString2		= _T(" ");

			// Infos cellule item quantitatif
			int CategorieItem		= 0;
			int NbModasItemRow		= 0;
			int NbModasItemCol		= 0;
			JBool ItemQuantitatif	= false;

			// Y a t-il plusieurs terrains sélectionnés
			JUnt32 NbRowTerrains = 0;
			JUnt32 NbColTerrains  = 0;

			if (m_pDoc->Get_NbSelectedTerrains() > 1)
			{
				if (m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL)
					NbRowTerrains = 1;
				else
					NbColTerrains = 1;
			}

			// Y a t-il des grandeurs en header ligne
			JUnt32 NbRowGrandeurs = 0;
			JUnt32 NbColGrandeurs = 0;
			if (m_pDoc->Get_NbSelectedGrandeurs() > 1)
			{
				if (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
					NbRowGrandeurs = 1;
				else
					NbColGrandeurs = 1;
			}

			// Traitement des différents type de ligne
			switch(Rt)
			{	
				// Traitement 1er ligne header
			case CTYPE_HEADER1:
				{
					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Traitement des colonnes
					switch(Ct)
					{
						// Les colonnes headers
					case CTYPE_HEADER1:
					case CTYPE_HEADER2:
					case CTYPE_HEADERTERRAIN:
					case CTYPE_HEADERCALC:
						{
							// On vérifie qu'on a au moins une page de valide
							if(m_NbPages)
							{
								// Récupère les strings
								GetPageCellString(TString,TString2,Pi);

								// Entete headers colonnes
								style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
									.SetValue(TString)
									.SetHorizontalAlignment(DT_CENTER)
									.SetVerticalAlignment(DT_VCENTER)
									.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
									.SetEnabled(false)
									.SetDraw3dFrame(gxFrameNormal)
									.SetFont(CGXFont(CATPColors::GetFontName())
									.SetSize(CATPColors::GetFontStaticSize())
									.SetItalic(ItemQuantitatif && CellItalique)
									.SetBold(false))
									;
								// return true;
								continue;
							}
							break;
						}

						// Les colonnes correspondant aux cellules items
					case CTYPE_ITEMCELL:
						{
							// Récupère le texte associé
							GetColCellString(TString,Rt,Universe,Ci,Cc);

							// Traitement spécial font pour les entetes items lignes quantitatifs (header col 1 et 2)
							bool MoyQuantiCol = false;
							GetColCellCategorie(Rt,Universe,Ci,Cc, CategorieItem, NbModasItemCol, MoyQuantiCol);

							// Indique si l'item est quantitatif  (pour affichage en italique)
							if ((Rt == CTYPE_HEADER1 || Rt == CTYPE_HEADER2) && (CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure) && NbModasItemCol <=1)
								ItemQuantitatif = true;

							// Style des entetes headers colonnes
							style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
								.SetValue(TString)
								.SetHorizontalAlignment(DT_CENTER)
								.SetVerticalAlignment(DT_VCENTER)
								.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
								.SetEnabled(false)
								.SetDraw3dFrame(gxFrameNormal)
								.SetFont(CGXFont(CATPColors::GetFontName())
								.SetSize(CATPColors::GetFontStaticSize())
								.SetItalic(ItemQuantitatif && CellItalique)
								.SetBold(false))
								;
							// return true;
							continue;
						}
						break;
					}

					// Sortie traitement ligne header 1
					break;
				}

				// Traitement des headers lignes intermédiares
				case CTYPE_HEADERTERRAIN:
				case CTYPE_HEADERCALC:
				case CTYPE_HEADER2:
				{
					// Surligne la colonne header cellule survolée
					if (NbRowTerrains == 0 && NbRowGrandeurs == 0)
					{
						// Pas de terrains ni grandeurs en entete ligne
						if (Rt == CTYPE_HEADER2 && nCol >= (m_ColCurHeader)  && nCol < (m_ColCurHeader + m_ItemColCount))
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
					}
					else if (NbRowTerrains > 0 && NbRowGrandeurs == 0)
					{
						// Seulement terrains en entete ligne
						if (Rt == CTYPE_HEADERTERRAIN && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count)))
						{
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
						}
					}
					else if (NbRowTerrains == 0 && NbRowGrandeurs > 0)
					{
						// Seulement grandeurs en entete ligne
						if (Rt == CTYPE_HEADERCALC && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count)))
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
					}
					else
					{
						// Terrains et grandeurs en entete ligne (on surligne l'entete des grandeurs)
						if (Rt == CTYPE_HEADERCALC && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count)))
							style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
					}

					// Traitement des colonnes
					switch(Ct)
					{
						// Les colonnes correspondant aux cellules items
					case CTYPE_ITEMCELL:
						{
							// Récupère le texte associé
							GetColCellString(TString,Rt,Universe,Ci,Cc);

							// Traitement spécial font pour les entetes items lignes quantitatifs (header col 1 et 2)
							bool MoyQuantiCol = false;
							GetColCellCategorie(Rt,Universe,Ci,Cc, CategorieItem, NbModasItemCol,MoyQuantiCol);

							// Indique si l'item est quantitatif  (pour affichage en italique)
							if ((Rt == CTYPE_HEADER1 || Rt == CTYPE_HEADER2) && (CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure) && NbModasItemCol <=1)
								ItemQuantitatif = true;

							// Style des entetes headers colonnes
							style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
								.SetValue(TString)
								.SetHorizontalAlignment(DT_CENTER)
								.SetVerticalAlignment(DT_VCENTER)
								.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
								.SetEnabled(false)
								.SetDraw3dFrame(gxFrameNormal)
								.SetFont(CGXFont(CATPColors::GetFontName())
								.SetSize(CATPColors::GetFontStaticSize())
								.SetItalic(ItemQuantitatif && CellItalique)
								.SetBold(false))
								;

							// On teste si l'on se trouve sur des élmts triés
							// MODIF A FAIRE (RECUP ELMT TRI) 
							m_Item_Col_Types.MoveTo(Ci);
							if(m_pDoc->Get_SortDirection() == 0 || Ci != m_pDoc->Get_SortCol() || !m_Item_Col_Types.IsValid())
								//return true;
								continue;

							if(m_Item_Col_Types.GetItem() & ITYPE_ITEM && nRow != m_Row_Hdr_Type.GetCount()-1)
								// Return if not on last row header
								//return true;
								continue;

							if((TableStyle & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1 && Universe != m_pDoc->Get_SortUniv())
								break;

							// Traitement des élmts triés
							switch(Rt)
							{
							case CTYPE_HEADERCALC:
								if(!CalcTypes.IsValid() || !(m_pDoc->Get_SortCalc() & CalcTypes.GetItem()))
									break;

							case CTYPE_HEADER2:
							case CTYPE_HEADERTERRAIN:

								// Mise en évidence de la colonne triée
								if(nRow == m_Row_Hdr_Type.GetCount()-1)
									style.SetInterior(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR));

								break;

							default: 
								if((TableStyle & TSTYLE_TERRAINCOL) && m_UniversParams.GetCount() > 1)
									break;
								if((TableStyle & TSTYLE_GRANDEURCOL) && CalcTypes.GetCount() > 1)
									break;

								// Marquage
								style.SetInterior(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR));
								break;
							}
							// return true;
							continue;
						}

						break;
					}
				}
				// Sortie traitement des autres headers lignes
				break;

				// Traitement des autres lignes (en dehors des headers)
				case CTYPE_ITEMCELL:
				{
					// Récupère le texte associé
					GetRowCellString(TString,Ct,Universe,Ri,Rc);

					// Validation écriture en italique
					CellItalique = (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);

					// Traitement spécial font pour les entetes items lignes quantitatifs (header col 1 et 2)
					bool MoyQuantiRow = false;
					GetRowCellCategorie(Ct,Universe,Ri,Rc, CategorieItem, NbModasItemRow, MoyQuantiRow);

					// Indique si l'item est quantitatif  (pour affichage en italique)
					if ((Ct == CTYPE_HEADER1 || Ct == CTYPE_HEADER2) && (CategorieItem == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItem == CQuestion::Quantitative_Pure) && NbModasItemRow <=1)
						ItemQuantitatif = true;
					else if (MoyQuantiRow)
						ItemQuantitatif = true;

					// En mode tableau et sur les cellules moyennes, on force ItemQuantitatif à true 
					if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU && (Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00) && Ct != CTYPE_HEADER1 && Ct!= CTYPE_HEADER2)
						ItemQuantitatif = true;

					// Ecriture libellé entete type de calcul en italique si quantité agrégé ou moyenne
					if (Ct == CTYPE_HEADERCALC && (Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00 || Type == RTYPE_QUANTITY_000))
						ItemQuantitatif = true;

					// Style des entetes headers lignes
					style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleRowHeader)
						.SetValue(TString)
						.SetHorizontalAlignment(DT_CENTER)
						.SetVerticalAlignment(DT_VCENTER)
						.SetMergeCell(GX_MERGE_VERTICAL|GX_MERGE_HORIZONTAL|GX_MERGE_COMPVALUE)
						.SetEnabled(false)
						.SetDraw3dFrame(gxFrameNormal)
						.SetFont(CGXFont(CATPColors::GetFontName())
						.SetSize(CATPColors::GetFontStaticSize())
						.SetItalic(ItemQuantitatif && CellItalique)
						.SetBold(false))
						;

					// Surligne la ligne header ligne survolée
					// if (Ct == CTYPE_HEADER2 && (nRow >= (m_RowCurHeader + NbRowTerrains + NbRowGrandeurs) &&  nRow < (m_RowCurHeader + NbRowTerrains + NbRowGrandeurs + m_ItemRowCount)))
					// style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

					////////////////////////////////////////////////////////////// A SIMPLIFIER /////////////////////////////////////////////////////////////
					//
					switch (Ct)
					{
						// Traitement des headers colonnes
						case CTYPE_HEADER1:
						case CTYPE_HEADER2:
						case CTYPE_HEADERTERRAIN :
						case CTYPE_HEADERCALC:
						{
							// Récup texte associé
							GetRowCellString(TString,Ct,Universe,Ri,Rc);

							// Surligne la ligne header cellule survolée
							// if (Ct == CTYPE_HEADERCALC && (nRow >= (m_RowCurHeader  + NbRowTerrains + NbRowGrandeurs) &&  nRow < (m_RowCurHeader  + NbRowTerrains + NbRowGrandeurs + m_ItemRowCount)))
							// style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

							// Surligne la ligne header cellule survolée
							if (NbColTerrains == 0 && NbColGrandeurs == 0)
							{
								// Pas de terrains ni grandeurs en entete colonne
								if (Ct == CTYPE_HEADER2 && nRow >= (m_RowCurHeader)  && nRow < (m_RowCurHeader + m_ItemRowCount))
									style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
							}
							else if (NbColTerrains > 0 && NbColGrandeurs == 0)
							{
								// Seulement terrains en entete colonne
								if (Ct == CTYPE_HEADERTERRAIN && (nRow >= m_RowCurHeader && (nRow < m_RowCurHeader + m_Result_Row_Count)))
								{
									style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
								}
							}
							else if (NbColTerrains == 0 && NbColGrandeurs > 0)
							{
								// Seulement grandeurs en entete colonne
								if (Ct == CTYPE_HEADERCALC && (nRow >= m_RowCurHeader && (nRow < m_RowCurHeader + m_Result_Row_Count)))
									style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
							}
							else
							{
								// Terrains et grandeurs en entete colonnne (on surligne l'entete des grandeurs)
								if (Ct == CTYPE_HEADERCALC && (nRow >= m_RowCurHeader && (nRow < m_RowCurHeader + m_Result_Row_Count)))
									style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));
							}

							// return true;
							continue;
						}
						break;

						// Traitement des autres cellules
						case CTYPE_ITEMCELL:
						{
							// Type de résultat
							RESULTTYPE Type = CalcTypes.GetItem();

							CellItalique = (Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);

							// Croisement ligne/colonne avec un élmt quantitatif > on met le résultat cellule en italique
							int CategorieItemRow = 0;
							int CategorieItemCol = 0;
							bool MoyQuantiRow = false;
							bool MoyQuantiCol = false;
							GetRowCellCategorie(CTYPE_HEADER2,Universe,Ri,Rc, CategorieItemRow, NbModasItemRow, MoyQuantiRow);
							GetColCellCategorie(CTYPE_HEADER2,Universe,Ci,Cc, CategorieItemCol, NbModasItemCol, MoyQuantiCol);


							// Croisement Item défini comme quantitatif si item ligne et/ou item colonne quantitatif
							ItemQuantitatif = (CategorieItemRow == CQuestion::CATEGORIE::Quantitative_Assim	|| 
								CategorieItemRow == CQuestion::Quantitative_Pure				||
								CategorieItemCol == CQuestion::CATEGORIE::Quantitative_Assim  || 
								CategorieItemCol == CQuestion::Quantitative_Pure) &&
								(NbModasItemRow <=1 && NbModasItemCol <= 1) &&
								(Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);


							// en ++
							if (MoyQuantiRow || MoyQuantiCol) 
								ItemQuantitatif = true;

							// Surlignage des résultats croisés quanti (il faut que l'on ait en ligne et en colonne des items quantis + type de calcul Quantité agrégée ou Valeur moyenne)
							bool ItemQuantitatifSurligne = (CategorieItemRow == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItemRow == CQuestion::Quantitative_Pure) &&
								(CategorieItemCol == CQuestion::CATEGORIE::Quantitative_Assim || CategorieItemCol == CQuestion::Quantitative_Pure) &&
								(Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);


							// En mode tableau et sur les cellules moyennes, on force ItemQuantitatif à true 
							if (m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU && (Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00) && Ct != CTYPE_HEADER1 && Ct!= CTYPE_HEADER2)
								ItemQuantitatif = true;						

							// Default styles for the cell
							style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleStandard)
								.SetHorizontalAlignment(DT_RIGHT)
								.SetVerticalAlignment(DT_VCENTER)
								.SetMergeCell(GX_MERGE_DISABLED)
								.SetEnabled(false)
								.SetValue(TString)
								.SetFont(CGXFont(CATPColors::GetFontName())
								.SetSize(CATPColors::GetFontStaticSize())
								.SetItalic(ItemQuantitatif && CellItalique)
								.SetUnderline(ItemQuantitatifSurligne));

							if(ResStyle & RSTYLE_RESULT)
								style.SetTextColor(CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR));

							if(Ri == BaseRowItem || Ci == BaseColItem)
							{
								// Marquage cellules base 100
								style.SetInterior(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR));
								style.SetTextColor(CATPColors::GetColorBlack());
							}
							else
							{
								// Test si cellule à surligner (inclu dans mpat total des cellules à surligner)
								CKeyRowCol KeyRowCol;

								KeyRowCol.m_NoRow = nRow;  // Ri;	 // nRow
								KeyRowCol.m_NoCol = nCol;  // Ci;  // nCol; 
								m_pDoc->m_MapCellTotSurlign.MoveTo(KeyRowCol);
								if (m_pDoc->m_MapCellTotSurlign.IsValid()) 
								{
									// Cellule à surligner, on récupère la couleur de surlignage
									const COLORREF &ColorSurlign = m_pDoc->m_MapCellTotSurlign.GetItem(); 

									// On surligne la cellule
									style.SetInterior(ColorSurlign);
								}
								else
								{
									// Cellule normale  rien de spécial
									style.SetInterior(CATPColors::GetColorWhite());//Light(CATPColors::COLORANALYSEUR));
								}
							}

							// Encadre la cellule survolée (meilleure visualisation)
							if (ColorCellule && ((nRow >= m_RowCurHeader &&  nRow < (m_RowCurHeader + m_Result_Row_Count)) && (nCol >= m_ColCurHeader && (nCol < m_ColCurHeader + m_Result_Col_Count))))
							{
								// Marquage cellule survolée couleur claire
								style.SetInterior(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

								// Séparation des cellules par une bordure point par point
								style.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_DOT).SetColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR))
									.SetWidth(1));
							}
							else
							{
								// Séparation cellule ligne pleine
								style.SetBorders(gxBorderAll, CGXPen().SetStyle(PS_NULL));
							}

							// Bordure pour les extrèmes
							if(Rv == m_Result_Row_Count-1 && nRow >= (m_NbRows - m_Result_Row_Count - 1))
							{
								style.SetBorders(gxBorderBottom, CGXPen().SetStyle(PS_SOLID).SetColor(CATPColors::GetColorLight(CATPColors::GREY_MOUSE)));//RGB(0,0,0)));
							}

							// Dessine trait au dessus du bloc cellule résultat
							if(Rv == 0)
							{
								style.SetBorders(gxBorderTop, CGXPen().SetStyle(PS_SOLID).SetColor(CATPColors::GetColorLight(CATPColors::GREY_MOUSE)));//RGB(0,0,0)));
							}

							// Dessine trait en fin de derniere colonne
							if(Cv == m_Result_Col_Count-1)
							{
								style.SetBorders(gxBorderRight, CGXPen().SetStyle(PS_SOLID).SetColor(CATPColors::GetColorLight(CATPColors::GREY_MOUSE)));//RGB(0,0,0)));
							}

							if((m_pDoc->Get_ResultStyle()&ResStyle) == 0 || GetCellResults(Universe) == 0 || !CalcTypes.IsValid())
								break;

							// RESULTTYPE Type = CalcTypes.GetItem();
							double Result = 0.0;

							// Flag si division par zéro
							bool DivZero			= false;

							// Flag si marge d'erreur atteinte
							bool MargeErreurDepasse = false;

							// Indication nombre de cas
							double NbCas = 0.0;

							// Marge d'erreur
							double MargeErreur = 0.0;

							// Intervalle confiance
							double IntervalleConfiance	= 0.0;

							if(!GetCellValue(Result, Type, Ri, Ci, Universe, ResStyle, Pi, BaseColItem, BaseRowItem, DivZero, NbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance))
								break;

							if(m_pDoc->Get_SortDirection() > 0 && Ci == m_pDoc->Get_SortCol() && Type == m_pDoc->Get_SortCalc() && Universe == m_pDoc->Get_SortUniv() && ResStyle == m_pDoc->Get_SortedStyle())
							{
								m_Item_Row_Types.MoveTo(Ri);
							}

							// Selon le style de résultat, on écrit avec une couleur différente
							switch(ResStyle)
							{
							case RSTYLE_HORIZONTAL:
							case RSTYLE_VERTICAL:
							case RSTYLE_GLOBAL:
								{
									// AVERTISSEMENT SEUIL : Cas hors limite calcul moteur
									if(Ri != BaseRowItem && Ci != BaseColItem)
									{
										if(NbCas <= (double)m_pDoc->Get_SeuileAvert() && NbCas > (double)m_pDoc->Get_SeuileLimit())
										{
											// Si marge d'erreur demandée
											/*
											if (m_pDoc->CalculMargeErreur() == true)
											// Marquage en couleur rouge						
											style.SetTextColor(CATPColors::GetColorRed());
											*/
											// Marquage en couleur rouge						
											style.SetTextColor(CATPColors::GetColorRed());
										}
										else
										{
											// Couleur résult différente selon type de résult
											if (ResStyle == RSTYLE_HORIZONTAL)
											{
												// Résultat horizontal H%
												style.SetTextColor(CATPColors::GetColorResult3());
											}
											else if (ResStyle == RSTYLE_VERTICAL)
											{
												// Résultat vertical V%
												style.SetTextColor(CATPColors::GetColorResult2());
											}
											else if (ResStyle == RSTYLE_GLOBAL)
											{
												// Autres résultats 
												style.SetTextColor(CATPColors::GetColorResult4());
											}
										}
									}
									else
									{
										// Couleur résult différente selon type de résult
										if (ResStyle == RSTYLE_HORIZONTAL)
										{
											style.SetTextColor(CATPColors::GetColorResult3());
										}
										else if (ResStyle == RSTYLE_VERTICAL)
										{
											style.SetTextColor(CATPColors::GetColorResult2());
										}
										else if (ResStyle == RSTYLE_GLOBAL)
										{
											style.SetTextColor(CATPColors::GetColorResult4());
										}
									}

									// les pourcentages V% et H% en italique (à voir)
									/*
									JBool  ResultH_V_Italic = false;
									if (ResStyle == RSTYLE_HORIZONTAL || ResStyle == RSTYLE_VERTICAL)
									{
									style.SetBaseStyle(m_pGrid->GetParam()->GetStylesMap()->m_wStyleStandard)
									.SetFont(CGXFont().SetItalic(ResultH_V_Italic));
									}
									*/
								}
								break;

								// Cas des effectifs (avec marge d'erreur compris entre m_SeuilMargeErreurUtil et m_SeuilMargeErreurAppli
							case RSTYLE_RESULT:
								{
									// Marquage si marge d'erreur compris entre seuil utilsateur et seuil appli
									if (MargeErreur >= (double)m_pDoc->Get_Seuil_MargeErreur_Util() 
										&& MargeErreur <= (double)m_pDoc->Get_Seuil_MargeErreur_Appli())
									{
										// Marquage en rouge si marge d'erreur demandée
										if (m_pDoc->CalculMargeErreur() == true)
										{
											style.SetTextColor(CATPColors::GetColorRed());
										}
										else
											style.SetTextColor(CATPColors::GetColorResult1());
									}
									else
									{
										// Tracé couleur de base pour les autres case
										style.SetTextColor(CATPColors::GetColorResult1());
									}

									if (Type == RTYPE_NBCAS)
									{
										if(NbCas <= (double)m_pDoc->Get_SeuileAvert() && NbCas > (double)m_pDoc->Get_SeuileLimit())
										{
											// Marquage en couleur rouge						
											style.SetTextColor(CATPColors::GetColorRed());
										}
									}
								}
								break;
							}

							// Attention aux limites pour indice corrélation
							if(ResStyle & RSTYLE_GBL_CORRELATION)
							{
								// AVERTISSEMENT SEUIL en mode Corrélation -- Cas hors limite calcul moteur
								if(Ri != BaseRowItem && Ci != BaseColItem)
								{
									// Nombre de cas en horizontal et vertical
									double  NbCasHorz				= 0.0;
									double  NbCasVert				= 0.0;

									// Variables résultat temporaires pour calcul NbCas Horz et Vert
									double	ResultH					= 0.0;
									double	ResultV					= 0.0;
									bool	MargeErreurDepasseH		= false;
									bool	MargeErreurDepasseV		= false;
									double	MargeErreurH			= 0.0;
									double	MargeErreurV			= 0.0;
									double	IntervalleConfianceH	= 0.0;
									double	IntervalleConfianceV	= 0.0;
									bool	DivZeroH				= false;
									bool	DivZeroV				= false;

									// Couleur résult différente selon type de résult
									style.SetTextColor(CATPColors::GetColorResult4());

									if(GetCellValue(ResultH, Type, Ri, Ci, Universe, RSTYLE_HORIZONTAL, Pi, BaseColItem, BaseRowItem, DivZeroH, NbCasHorz, MargeErreurDepasseH,MargeErreurH,IntervalleConfianceH)
										&& GetCellValue(ResultV, Type, Ri, Ci, Universe, RSTYLE_VERTICAL, Pi, BaseColItem, BaseRowItem, DivZeroV, NbCasVert, MargeErreurDepasseV,MargeErreurV,IntervalleConfianceV))
									{
										// Les 2 indices en dessous du seuil limite
										if (NbCasHorz <= (double)m_pDoc->Get_SeuileLimit() && NbCasVert <= (double)m_pDoc->Get_SeuileLimit())
											// Marquage cellue #
											MargeErreurDepasse = true;

										// Les 2 indices sont compris entre seuil avertissement et seuil limite
										else if((NbCasHorz <= (double)m_pDoc->Get_SeuileAvert() && NbCasHorz > (double)m_pDoc->Get_SeuileLimit()) &&
											(NbCasVert <= (double)m_pDoc->Get_SeuileAvert() && NbCasVert > (double)m_pDoc->Get_SeuileLimit()))
										{
											/*
											// Si marge d'erreur demandée
											if (m_pDoc->CalculMargeErreur() == true)
											// Couleur font en rouge
											style.SetTextColor(CATPColors::GetColorRed());
											*/
											// Couleur font en rouge
											style.SetTextColor(CATPColors::GetColorRed());
										}

										// 1 des 2 indices en dessous du seuil
										else if (NbCasHorz <= (double)m_pDoc->Get_SeuileLimit() || NbCasVert <= (double)m_pDoc->Get_SeuileLimit())
										{
											/*
											// Si marge d'erreur demandée
											if (m_pDoc->CalculMargeErreur() == true)
											// Couleur font en rouge
											style.SetTextColor(CATPColors::GetColorRed());
											*/
											// Couleur font en rouge
											style.SetTextColor(CATPColors::GetColorRed());
										}	
									}
								}
							}

							// Récupère le résultat à afficher dans la cellule
							GetFormattedResult(TString, Result, ResStyle, Type, Ri, Ci, BaseRowItem, BaseColItem, DivZero, NbCas, MargeErreurDepasse, ItemQuantitatif, MoyQuantiRow, MoyQuantiCol);
							style.SetValue(TString); 

							// Cellule définie, on pourra afficher la ligne	
							if (TString != "*")
							{
								/* OPTIM 2.7
								m_pGrid->m_MapRowMoyenneAffich.MoveTo(nRow);
								if (m_pGrid->m_MapRowMoyenneAffich.IsValid())
								{
									// Si au moins 1 valeur sur la ligne >> la ligne doit être affichée
									// bool &Affich = m_pGrid->m_MapRowMoyenneAffich.GetItem();
									// Affich = true;
									m_pGrid->m_MapRowMoyenneAffich.GetItem() = true;
								}
								*/
								// Si au moins 1 valeur sur la ligne >> la ligne doit être affichée
								// bool &Affich = m_pGrid->m_MapRowMoyenneAffich.GetItem();
								// Affich = true;
								m_pGrid->m_MapRowMoyenneAffich.GetItem() = true;
								// FIN OPTIM 2.7
							}
							else
							{
								int Ok = 0;
							}

							// return true;
							continue;
						}
						break;
					}
				}
				break;

				////////////////////////////////////////////////////////////////////// FIN A SIMPLIFIER ////////////////////////////////////////////////////////

				// Traitement erreur
			case CTYPE_ERROR:
				return false;
			}
		}
	}

	// Sinon return false
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére chaine de caractère des cellules rows headers
void CCrossTable::GetRowCellString(CString & St, CELLTYPE t, int Universe, int i, int c)
{
	// Init libellé 
	St = _T(" ");

	// Vérifie la validité de la ligne en quetsion
	// A OPTIMISER 2.7
	m_Item_Row_Types.MoveTo(i);
	if(!m_Item_Row_Types.IsValid())
		return;

	// Récupére le type de la ligne
	ITEMTYPE RowType = m_Item_Row_Types.GetItem();

	switch(t)
	{
		case CTYPE_HEADER1:
		{
			// Check what type of item we are on
			switch(RowType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Row_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
					Items.MoveTo(i);
					if(Items.IsValid())
						St = Items.GetItem()->GetHeader(); 

					return ;
				}
			}
		}
		case CTYPE_HEADER2:
		{
			// Check what type of item we are on
			switch(RowType)
			{
				case ITYPE_UNIVERSE:
				{
					if(m_pDoc->Get_ResultStyle()& RSTYLE_CIBLES) 
						St.LoadString(IDS_AN_LABEL_CIBLES);
					else
						St.LoadString(IDS_AN_LABEL_UNIVERS);
				}
				break;

				case ITYPE_NICHE:
				{
					St.LoadString(IDS_AN_NICHE);
				}
				break;

				case ITYPE_ITEM:
				{
					i -= m_Row_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
				
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Libellé header 2
						St = Items.GetItem()->GetLabel(); 

						// Cas des items spécifiques quantis
						St.Replace("@","-");

						// Si on est en mode 0 exclu, on vire le 0
						if (m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS)
						{
							if (St.Find(".."))
							{
								// On est bien sur un intervalle libellé construit type 0..y, on remplace le 0 si au début
								if (St.Find("0") == 0)
								{
									// Pour le moment la chaine sera alors du type x..y
									St = "x" + St.Mid(1);
								}
							}
						}
					}
				}
				break;

				case ITYPE_CONTRENICHE:
				{
					St.LoadString(IDS_AN_CONTRENICHE);
				}
				break;

				case ITYPE_EXCLUSIVE:
				{
					St.LoadString(IDS_AN_ITEMEXCLUSIVE);
				}
				break;

				case ITYPE_MOYEN:
				{
					St.LoadString(IDS_AN_ITEMMOYEN);
				}
				break;
			}
		}
		break;

		case CTYPE_HEADERTERRAIN :
		{
			m_UniversParams.MoveTo(Universe);
			if(m_UniversParams.IsValid())
			{
				CTerrain * pTerrain = m_UniversParams.GetItem()->GetTerrain();
				const JSRCTables & Press = JSRCTables::GetInstance();
				const JTerrain * jt = Press.m_TBLTerrains.GetItem(pTerrain->GetSourceID(),pTerrain->GetTerrainID());
				if(jt)
					St = jt->m_Libelle.AsJCharPtr();  
			}
		}
		break;

		case CTYPE_HEADERCALC:
		{
			const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();
			CalcTypes.MoveTo(c);
			if(CalcTypes.IsValid())
			{
				switch(CalcTypes.GetItem())
				{
					case RTYPE_EFFECTIVE_000:
					{
						St.LoadString(IDS_AN_ABR_EFFECTIVE);
					}
					break;
					case RTYPE_NBCAS:
					{
						St.LoadString(IDS_AN_ABR_NBCAS);
					}
					break;
					case RTYPE_PENETRATION: 
					{
						St.LoadString(IDS_AN_ABR_PENETRATION);
					}
					break;
					case RTYPE_CENTRAGE: 
					{
						St.LoadString(IDS_AN_ABR_CENTRAGE);
					}
					break;
					case RTYPE_AVERAGEQUANTUM :
					{
						St.LoadString(IDS_AN_ABR_AVERAGEQUANTUM);
					}
					break;
					case RTYPE_AVERAGEQUANTUM_P00 :
					{
						St.LoadString(IDS_AN_ABR_AVERAGEQUANTUM_P00);
					}
					break;
					case RTYPE_QUANTITY_000 :
					{
						St.LoadString(IDS_AN_ABR_QUANTITY);
					}
					break;
					case RTYPE_EFFECTIVE_CIBLE:
					{
						St.LoadString(IDS_AN_ABR_EFFECTIVEBRUT);
					}
					break;
					case RTYPE_CONCOMITENCE :
					{
						St.LoadString(IDS_AN_ABR_CONCOMITENCE);
					}
					break;
					case RTYPE_COEFFYULE:
					{
						St.LoadString(IDS_AN_ABR_COEFFYULE);
					}
					break;
					case RTYPE_ZSCORE:
					{
						St.LoadString(IDS_AN_ABR_ZSCORE);
					}
					break;
				}
			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère catégorie d'une entete ligne
void CCrossTable::GetRowCellCategorie(CELLTYPE t, int Universe, int i, int c, int & Categorie, int & NbModas, bool & ItemMoyennable)
{
	// Par défaut l'item n'est pas quantitatif
	ItemMoyennable = false;

	m_Item_Row_Types.MoveTo(i);
	if(!m_Item_Row_Types.IsValid())
		return;

	ITEMTYPE RowType = m_Item_Row_Types.GetItem();

	switch(t)
	{
		case CTYPE_HEADER1:
		{
			// Check what type of item we are on
			switch(RowType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Row_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable
						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());

						// Récup catégorie de l'item	
						Categorie = Items.GetItem()->GetCategorie();

						// Renvoi également le nombre de modalités constituant cet item
						NbModas = Items.GetCount(); 
					}
					return;
				}
			}
		}
		
		case CTYPE_HEADER2:
		{
			// Check what type of item we are on
			switch(RowType)
			{
				case ITYPE_ITEM :
				{
					i -= m_Row_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();

					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						///////////////////////////////////////////////////////////////////////////
						// AOUT 2013
						// Libellé de l'item
						// const IItem *pItem = vItems.GetItem();
						// CString LibItem = pItem->GetLabel().AsJCharPtr();
						if (m_pDoc->m_DlgAnalyseur.ItemQuantitatif(pItemConstruit, m_pDoc->Get_TerrainTemoin()))
						{
							ItemMoyennable = true;
						}
						///////////////////////////////////////////////////////////////////////////

						// Cet item est-il moyennable
						else
							ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());

						// Récup catégorie de l'item
						Categorie = Items.GetItem()->GetCategorie()  ;

						// Renvoi également le nombre de modalités constituant cet item
						NbModas = Items.GetCount(); 
					}
				}
				break;

				// Voir avec JFC
				case ITYPE_NICHE:    // Modif 2013
				{

					// A VOIR AVEC JFC ????????  AOUT 2013
					ItemMoyennable = true;

					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();

					for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
					{
						if(Items.IsValid())
						{
							// Récupére la categorie question
							const CItemWrapper *pItemWrapper = Items.GetItem();

							// Vérifie si on a affaire à un item numérisé quantitatif
							const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

							// Cet item est-il moyennable
							if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()) == false)
							{
								ItemMoyennable = false;
								break;
							}
						}
					}
						
					// AVANT
					// ItemMoyennable = true;
				}
			}
		}
		break;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère si item moyennable
void CCrossTable::GetRowCellMoyennable(CELLTYPE t, int i, int c, bool & ItemMoyennable, ITEMTYPE RowType)
{
	// Par défaut l'item n'est pas quantitatif
	ItemMoyennable = false;

	/* OPTIM 2.7
	m_Item_Row_Types.MoveTo(i);
	if(!m_Item_Row_Types.IsValid())
		return;

	ITEMTYPE RowType = m_Item_Row_Types.GetItem();
	*/

	switch(t)
	{
	case CTYPE_HEADER1:
		{
			// Check what type of item we are on
			switch(RowType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Row_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable
						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());
					}
					return;
				}
			}
		}

		case CTYPE_HEADER2:
		{
			// Check what type of item we are on
			switch(RowType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Row_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();

					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable
						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());
					}
				}
				break;
			}
		}
		break;
	}
}

void CCrossTable::GetPageCellString(CString & St,CString & St2,int Page)
{
	St = _T(" ");
	St2 = _T(" ");

	if(m_pDoc->Get_ResultStyle()& RSTYLE_CIBLES) 
		St.LoadString(IDS_AN_LABEL_CIBLES);
	else
		St.LoadString(IDS_AN_LABEL_UNIVERS);

	m_Item_Page_Types.MoveTo(Page);
	if(!m_Item_Page_Types.IsValid())
		return;

	ITEMTYPE PageType = m_Item_Page_Types.GetItem();

	// Check what type of item we are on
	switch(PageType)
	{
		case ITYPE_UNIVERSE:
		{
			if(m_pDoc->GetPageItems().GetCount() == 0)
			{
				St = " ";
			}
			else
				St2 = St;
		}
		break;
		case ITYPE_NICHE:
		{
			St.LoadString(IDS_AN_NICHE);
			St2 = St;
		}
		break;
		case ITYPE_ITEM:
		{
			Page -= m_Page_StartItem;
			const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetPageItems();
			Items.MoveTo(Page);
			if(Items.IsValid())
			{
				St  = Items.GetItem()->GetHeader(); 
				St2 = Items.GetItem()->GetLabel(); 
			}
		}
		break;
		case ITYPE_CONTRENICHE:
		{
			St.LoadString(IDS_AN_CONTRENICHE);
			St2 = St;
		}
		break;
		case ITYPE_EXCLUSIVE:
		{
			St.LoadString(IDS_AN_ITEMEXCLUSIVE);
			St2 = St;
		}
		break;
		case ITYPE_MOYEN:
		{
			St.LoadString(IDS_AN_ITEMMOYEN);
			St2 = St;
		}
		break;
	}
}


void CCrossTable::GetColCellString(CString & St, CELLTYPE t,int Universe, int i, int c)
{
	St = _T(" ");

	m_Item_Col_Types.MoveTo(i);
	if(!m_Item_Col_Types.IsValid())
		return;

	ITEMTYPE ColType = m_Item_Col_Types.GetItem();

	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	switch(t)
	{
		case CTYPE_HEADER1:
		{
			// Check what type of item we are on
			switch(ColType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Col_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
					Items.MoveTo(i);
					if(Items.IsValid())
						St = Items.GetItem()->GetHeader(); 

					return;
				}
			}
		}
		case CTYPE_HEADER2:
		{
			// Check what type of item we are on
			switch(ColType)
			{
				case ITYPE_UNIVERSE:
				{
					if(m_pDoc->Get_ResultStyle()& RSTYLE_CIBLES) 
						St.LoadString(IDS_AN_LABEL_CIBLES);
					else
						St.LoadString(IDS_AN_LABEL_UNIVERS);
				}
				break;
				case ITYPE_NICHE:
				{
					St.LoadString(IDS_AN_NICHE);
				}
				break;
				case ITYPE_ITEM:
				{
					i -= m_Col_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
					Items.MoveTo(i);
					if(Items.IsValid())
						St = Items.GetItem()->GetLabel(); 
				}
				break;
				case ITYPE_CONTRENICHE:
				{
					St.LoadString(IDS_AN_CONTRENICHE);
				}
				break;
				case ITYPE_EXCLUSIVE:
				{
					St.LoadString(IDS_AN_ITEMEXCLUSIVE);
				}
				break;
				case ITYPE_MOYEN:
				{
					St.LoadString(IDS_AN_ITEMMOYEN);
				}
				break;
			}
		}
		break;

		case CTYPE_HEADERTERRAIN :
		{
			m_UniversParams.MoveTo(Universe);
			if(m_UniversParams.IsValid())
			{
				CTerrain * pTerrain = m_UniversParams.GetItem()->GetTerrain();
				const JSRCTables & Press = JSRCTables::GetInstance();
				const JTerrain * jt = Press.m_TBLTerrains.GetItem(pTerrain->GetSourceID(),pTerrain->GetTerrainID());
				if(jt)
					St = jt->m_Libelle.AsJCharPtr();  
			}
		}
		break;

		case CTYPE_HEADERCALC:
		{
			const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();
			CalcTypes.MoveTo(c);
			if(CalcTypes.IsValid())
			{
				switch(CalcTypes.GetItem())
				{
					case RTYPE_EFFECTIVE_000:
					{
						St.LoadString(IDS_AN_ABR_EFFECTIVE);
					}
					break;
					case RTYPE_NBCAS:
					{
						St.LoadString(IDS_AN_ABR_NBCAS);
					}
					break;
					case RTYPE_PENETRATION: 
					{
						St.LoadString(IDS_AN_ABR_PENETRATION);
					}
					break;
					case RTYPE_CENTRAGE: 
					{
						St.LoadString(IDS_AN_ABR_CENTRAGE);
					}
					break;
					case RTYPE_AVERAGEQUANTUM :
					{
						St.LoadString(IDS_AN_ABR_AVERAGEQUANTUM);
					}
					break;
					case RTYPE_AVERAGEQUANTUM_P00 :
					{
						St.LoadString(IDS_AN_ABR_AVERAGEQUANTUM_P00);
					}
					break;
					case RTYPE_QUANTITY_000 :
					{
						St.LoadString(IDS_AN_ABR_QUANTITY);
					}
					break;
					case RTYPE_EFFECTIVE_CIBLE:
					{
						St.LoadString(IDS_AN_ABR_EFFECTIVEBRUT);
					}
					break;
					case RTYPE_CONCOMITENCE :
					{
						St.LoadString(IDS_AN_ABR_CONCOMITENCE);
					}
					break;
					case RTYPE_COEFFYULE:
					{
						St.LoadString(IDS_AN_ABR_COEFFYULE);
					}
					break;
					case RTYPE_ZSCORE:
					{
						St.LoadString(IDS_AN_ABR_ZSCORE);
					}
					break;
				}
			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère catégorie d'une entete colonne
void CCrossTable::GetColCellCategorie(CELLTYPE t,int Universe, int i, int c, int & Categorie, int & NbModas, bool & ItemMoyennable)
{
	// Par défaut l'item n'est pas quantitatif
	ItemMoyennable = false;

	m_Item_Col_Types.MoveTo(i);
	if(!m_Item_Col_Types.IsValid())
		return;

	ITEMTYPE ColType = m_Item_Col_Types.GetItem();

	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	switch(t)
	{
		case CTYPE_HEADER1:
		{
			// Check what type of item we are on
			switch(ColType)
			{
			case ITYPE_ITEM:
				{
					i -= m_Col_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable
						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());

						// Récupére la categorie question
						Categorie = Items.GetItem()->GetCategorie()  ;

						// Renvoi également le nombre de modalités constituant cet item
						NbModas = Items.GetCount(); 
					}
					return;
				}
			}
		}

		case CTYPE_HEADER2:
		{
			// Check what type of item we are on
			switch(ColType)
			{
			case ITYPE_ITEM:
				{
					i -= m_Col_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable

						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());

						// Récupére la categorie question
						Categorie = Items.GetItem()->GetCategorie()  ;

						// Renvoi également le nombre de modalités constituant cet item
						NbModas = Items.GetCount(); 
					}
				}
				break;
			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère si item colonne moyennable
void CCrossTable::GetColCellMoyennable(CELLTYPE t,int i, int c, bool & ItemMoyennable, ITEMTYPE ColType)
{
	// Par défaut l'item n'est pas quantitatif
	ItemMoyennable = false;

	/* OPTIM 2.7
	m_Item_Col_Types.MoveTo(i);
	if(!m_Item_Col_Types.IsValid())
		return;

	ITEMTYPE ColType = m_Item_Col_Types.GetItem();
	*/

	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

	switch(t)
	{
		case CTYPE_HEADER1:
		{
			// Check what type of item we are on
			switch(ColType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Col_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable
						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());
					}
					return;
				}
			}
		}

		case CTYPE_HEADER2:
		{
			// Check what type of item we are on
			switch(ColType)
			{
				case ITYPE_ITEM:
				{
					i -= m_Col_StartItem;
					const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
					Items.MoveTo(i);
					if(Items.IsValid())
					{
						// Récupére la categorie question
						const CItemWrapper *pItemWrapper = Items.GetItem();

						// Vérifie si on a affaire à un item numérisé quantitatif
						const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

						// Cet item est-il moyennable
						ItemMoyennable = m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef());
					}
				}
				break;
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												PROCEDURES DE TRI COMPLETEMENT REVUES
////////////////////////////////////////////////////////////////////////////////////////////////
// click droit > rien
BOOL CCrossTable::OnRButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	return FALSE;
}

// click gauche > visualisation menu pop-up de tri  
BOOL CCrossTable::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	// Heres where we will sort the column by the value which was clicked upon
	CELLTYPE Rt,Ct;
	int Ri,Ru,Rc,Rv;
	int Ci,Cu,Cc,Cv;
	Rt = GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);
	Ct = GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

	RESULTSTYLE ResStyle = GetResultStyle(Rv,Cv);

	int Universe = 0;
	if(!(m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL))
		Universe = Ru;
	else
		Universe = Cu;

	if((m_pDoc->Get_ResultStyle()&ResStyle) == 0)
		return true;


	// Nb cols ou rows avec terrains et garndeurs
	JUnt32 NbHeaderRowTerrain  = 0;
	JUnt32 NbHeaderRowGrandeur = 0;

	// Les élmts grandeurs sont-ils en header ????
	if (m_pDoc->Get_NbSelectedGrandeurs() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
		{
			// Au moins 2 grandeurs disposées en row header
			NbHeaderRowGrandeur = 1; 
		}
	}

	// Les élmts terrains sont-ils en header ????
	if (m_pDoc->Get_NbSelectedTerrains() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL)
		{
			// Au moins 2 terrains disposées en row header
			NbHeaderRowTerrain = 1; 
		}
	}

	if(!(nFlags&MK_CONTROL))
	{
		switch(ResStyle)
		{
			case RSTYLE_RESULT: // Result
				if((m_pDoc->Get_ResultStyle()& RSTYLE_RESULT) == 0)
					return FALSE;
			break;

			case RSTYLE_HORIZONTAL: // Horizontal
				if((m_pDoc->Get_ResultStyle()& RSTYLE_HORIZONTAL) == 0)
					return FALSE;
			break;

			case RSTYLE_VERTICAL: // Vertical
				if((m_pDoc->Get_ResultStyle()& RSTYLE_VERTICAL) == 0)
					return FALSE;
			break;

			case RSTYLE_GLOBAL: // Global
				if((m_pDoc->Get_ResultStyle()& RSTYLE_GLOBAL) == 0)
					return FALSE;
			break;
		}
	}

	// On fait apparaitre le menu uniquement sur dernier header ligne
	if ((Rt == CTYPE_HEADER2 && NbHeaderRowTerrain == 0 && NbHeaderRowGrandeur == 0) || 
		(Rt == CTYPE_HEADERTERRAIN && NbHeaderRowTerrain == 1 && NbHeaderRowGrandeur == 0) || 
		(Rt == CTYPE_HEADERCALC && NbHeaderRowGrandeur ==1))
	{
		if (nCol >= m_Col_StartItem)
		{
			// Affichage sélection paramètres de tri
			CDlgParamsTri Dlg(m_pGrid->GetParent());

			// Calcul position fenêtre parametrage
			CRect RectGrid;
			m_pGrid->GetWindowRect(RectGrid);
			pt.x += RectGrid.left; 
			pt.y += RectGrid.top; 

			// Envoi le document à la fenêtre
			Dlg.SetParams(m_pDoc, pt);

			if(Dlg.DoModal() == IDOK)
			{
				bool SortType 			= m_pDoc->Get_SortType();     // (false = toutes les lignes, true = tri par blocs de critères)
			
				if (m_pDoc->Get_ModeTri() == m_pDoc->MODE_LIGNE)
				{
					// Tri sur la totalité des lignes
					m_pDoc->Set_SortUniv(Universe);
					m_pDoc->Set_SortCol(Ci);
					m_pDoc->Set_SortCalc(m_pDoc->Get_GrandeurTri());
					m_pDoc->Set_SortPage(GetPageItem(m_pDoc->GetCurPage()));

					// On positionne le sens du tri (TYPE_BASE = 0, TYPE_DECROISSANT, TYPE_CROISSANT)
					if (m_pDoc->Get_TypeTri() == m_pDoc->TYPE_BASE)
						m_pDoc->Set_SortDirection(0);
					else if (m_pDoc->Get_TypeTri() == m_pDoc->TYPE_DECROISSANT)
						m_pDoc->Set_SortDirection(1);
					else if (m_pDoc->Get_TypeTri() == m_pDoc->TYPE_CROISSANT)
						m_pDoc->Set_SortDirection(2);

					// Style du tri (RSTYLE_RESULT, RSTYLE_HORIZONTAL, RSTYLE_VERTICAL, RSTYLE_GLOBAL)
					ResStyle = m_pDoc->Get_ResultTri();	

					// Attention, sinon le tri ne se fera pas quand on est en mode RSTYLE_GLOBAL
					if (ResStyle & RSTYLE_GLOBAL)
						m_pDoc->Set_SortedStyle(RSTYLE_GLOBAL);
					else
						m_pDoc->Set_SortedStyle(ResStyle);
					
					// Type de tri par lignes	
					m_pDoc->Set_SortType(false);
	
					// Recalcul 
					CalculateItems();
				}
				else
				{
					// Tri par blocs de critères
					m_pDoc->Set_SortUniv(Universe);
					m_pDoc->Set_SortCol(Ci);
					m_pDoc->Set_SortCalc(m_pDoc->Get_GrandeurTri());
					m_pDoc->Set_SortPage(GetPageItem(m_pDoc->GetCurPage()));

					// On positionne le sens du tri (TYPE_BASE = 0, TYPE_DECROISSANT , TYPE_CROISSANT)
					if (m_pDoc->Get_TypeTri()		== m_pDoc->TYPE_BASE)
						m_pDoc->Set_SortDirection(0);
					else if (m_pDoc->Get_TypeTri()	== m_pDoc->TYPE_DECROISSANT)
						m_pDoc->Set_SortDirection(1);
					else if (m_pDoc->Get_TypeTri()  == m_pDoc->TYPE_CROISSANT)
						m_pDoc->Set_SortDirection(2);

					// Style du tri (RSTYLE_RESULT, RSTYLE_HORIZONTAL, RSTYLE_VERTICAL, RSTYLE_GLOBAL)
					ResStyle = m_pDoc->Get_ResultTri();	

					// Attention, sinon le tri ne se fera pas quand on est en mode RSTYLE_GLOBAL
					if (ResStyle & RSTYLE_GLOBAL)
						m_pDoc->Set_SortedStyle(RSTYLE_GLOBAL);
					else
						m_pDoc->Set_SortedStyle(ResStyle);

					// Type de tri par blocs de question
					m_pDoc->Set_SortType(true);

					// Recalcul
					CalculateItems();
				}

				// Envoi message mise à jour info titre avec paramètres de tri
				m_pDoc->UpdateTitre();

				// Puis on met le focus ailleurs
				m_pGrid->SelectRange(CGXRange(0,0,m_pGrid->GetRowCount(),m_pGrid->GetColCount()),FALSE,FALSE);
				m_pGrid->Invalidate(); 

				return true;		
			}
		}
	}
	else if ((nRow !=0 || nCol != 0) && (Rt == CTYPE_HEADER1 || Rt == CTYPE_HEADERTERRAIN))
	{
		// Enlève la sélection arrière plan noir (très moche) quand on clicke sur des headers lignes
		m_pGrid->SelectRange(CGXRange(0,0,m_pGrid->GetRowCount(),m_pGrid->GetColCount()),FALSE,FALSE);
		m_pGrid->Invalidate(); 
	}
	else if ((nRow !=0 || nCol != 0) && (Rt == CTYPE_HEADER1 || Rt == CTYPE_HEADER2 || Rt == CTYPE_HEADERTERRAIN || Rt == CTYPE_HEADERCALC || Ct == CTYPE_HEADER1 || Ct == CTYPE_HEADER2 || Ct == CTYPE_HEADERTERRAIN || Ct == CTYPE_HEADERCALC))
	{
		// Enlève la sélection arrière plan noir (très moche) quand on clicke sur des headers colonnes
		m_pGrid->SelectRange(CGXRange(0,0,m_pGrid->GetRowCount(),m_pGrid->GetColCount()),FALSE,FALSE);
		m_pGrid->Invalidate(); 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//													GESTION DE LA BASE 100 et DU SURLIGNAGE
	else if (Rt == CTYPE_ITEMCELL && Ct == CTYPE_ITEMCELL)
	{
		const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();

		if(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
			CalcTypes.MoveTo(Cc);
		else
			CalcTypes.MoveTo(Rc);

		if(!CalcTypes.IsValid())
			return false;// Bad calc type index

		RESULTTYPE Type = CalcTypes.GetItem();

		// Récup le mode courant click
		UINT ModeClick = m_pDoc->Get_ClickMode();

		// Mode affichage en base 100
		if(m_pDoc->Get_ClickMode() == CMODE_BASE100)
		{
			m_pDoc->Set_BaseColItem(Ci);
			m_pDoc->Set_BaseRowItem(Ri);
			m_pGrid->RedrawRowCol(CGXRange().SetTable(),GX_UPDATENOW,false);

			// On reste en base 100, tant que option non désélectionné
			// m_pDoc->Set_ClickMode(CMODE_TRI);

			if(m_pBut_Surlignage) m_pBut_Surlignage->SetCheck(0);

			m_Base100 = FALSE;
			//			SetGridCursor(0);

			return true;
		}

		// Mode surlignage
		if(m_pDoc->Get_ClickMode() == CMODE_ORDONNER)
		{
			if(m_pDoc->Get_SurlignageCalc() != Type || m_pDoc->Get_SurlignageStyle() != ResStyle)
			{
				m_pDoc->Set_SurlignageCalc(Type);
				m_pDoc->Set_SurlignageStyle(ResStyle);
			}

			RESULTSTYLE ResultStyle = m_pDoc->Get_SurlignageStyle();

			CalculateItems();
			return true;
		}

	}

	return FALSE;
}

void CCrossTable::ResetSort()
{
	m_SortedRowIndex.	Reset();
}

bool CCrossTable::SortColumn()
{
	m_SortedRowIndex.Reset();

	if(m_pDoc->Get_SortDirection() == 0)
		return false;

	JMap<CColSortKey,bool> * pKeyMap;
	JMap<int, JMap<CColSortKey,bool> > KeyMapMap;
	int HdrIndex = 0;
	CString Header = "";
	pKeyMap = 0;

	if(m_pDoc->Get_SortType())
	{
		const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();

		CColSortKey Key;

		for(m_ItemRowIndex.MoveFirst(); m_ItemRowIndex.IsValid(); m_ItemRowIndex.MoveNext())
		{
			int Idx = m_ItemRowIndex.GetItem();
			m_Item_Row_Types.MoveTo(Idx);
			if(m_Item_Row_Types.IsValid() && m_Item_Row_Types.GetItem() == ITYPE_ITEM)
			{
				double Result	= 0.0;

				// Flag si division par zéro
				bool DivZero			= false;

				// Flag si marge d'erreur atteinte
				bool MargeErreurDepasse = false;

				// Indication nombre de cas
				double NbCas = 0.0;

				// Marge d'erreur
				double MargeErreur;

				// Intervalle confiance
				double IntervalleConfiance	= 0.0;

				if(!GetCellValue(Result, m_pDoc->Get_SortCalc(), Idx, m_pDoc->Get_SortCol(), m_pDoc->Get_SortUniv(), m_pDoc->Get_SortedStyle(), GetPageItem(m_pDoc->GetCurPage()), m_pDoc->Get_BaseColItem(), m_pDoc->Get_BaseRowItem(),DivZero,NbCas,MargeErreurDepasse,MargeErreur,IntervalleConfiance) || DivZero)
					Result = -1.0;
				switch(m_pDoc->Get_SortedStyle())
				{
					case RSTYLE_HORIZONTAL:
					case RSTYLE_VERTICAL:
					case RSTYLE_GLOBAL:
					{
						if(NbCas <= (double)m_pDoc->Get_SeuileLimit())
							Result = -1.0;
					}
					break;
				}

				Items.MoveTo(Idx-m_Row_StartItem);
				if(Items.IsValid())
				{
					if(Header != Items.GetItem()->GetHeader())
					{
						Header = Items.GetItem()->GetHeader();
						pKeyMap = &(KeyMapMap.Add(++HdrIndex));
					}
					Key.SetValueRow(HdrIndex,Result,Idx);
				}
				else
					Key.SetValueRow(Result,Idx);

				if(pKeyMap != 0)
					pKeyMap->Add(Key) = true;
			}
		}
	}
	else
	{
		for(m_ItemRowIndex.MoveFirst(); m_ItemRowIndex.IsValid(); m_ItemRowIndex.MoveNext())
		{
			int Idx = m_ItemRowIndex.GetItem();
			m_Item_Row_Types.MoveTo(Idx);
			if(m_Item_Row_Types.IsValid() && m_Item_Row_Types.GetItem() == ITYPE_ITEM)
			{
				if(pKeyMap == 0)
					pKeyMap = &(KeyMapMap.Add(++HdrIndex));
				
				double Result	= 0.0;

				// Flag si division par zéro
				bool DivZero			= false;

				// Flag si marge d'erreur atteinte
				bool MargeErreurDepasse = false;

				// Indication nombre de cas
				double NbCas = 0.0;

				// Marge d'erreur
				double MargeErreur;

				// Intervalle confiance
				double IntervalleConfiance	= 0.0;
				
				if(!GetCellValue(Result, m_pDoc->Get_SortCalc(), Idx, m_pDoc->Get_SortCol(), m_pDoc->Get_SortUniv(), m_pDoc->Get_SortedStyle(), GetPageItem(m_pDoc->GetCurPage()), m_pDoc->Get_BaseColItem(), m_pDoc->Get_BaseRowItem(),DivZero,NbCas,MargeErreurDepasse,MargeErreur,IntervalleConfiance) || DivZero)
					Result = -1.0;
				switch(m_pDoc->Get_SortedStyle())
				{
					case RSTYLE_HORIZONTAL:
					case RSTYLE_VERTICAL:
					case RSTYLE_GLOBAL:
					{
						if(NbCas <= (double)m_pDoc->Get_SeuileLimit())
							Result = -1.0;
					}
					break;
				}

				CColSortKey Key(Result,Idx);
				pKeyMap->Add(Key) = true;
			}
		}
	}

	int Count = m_ItemRowIndex.GetCount();
	m_SortedRowIndex.SetCount(Count);

	switch(m_pDoc->Get_SortDirection())
	{
		case 1:
		{
			for(int x=0 ; x < Count;)
			{
				m_ItemRowIndex.MoveTo(x);
				int Idx = m_ItemRowIndex.GetItem();
				m_Item_Row_Types.MoveTo(Idx);
				if(m_Item_Row_Types.GetItem() == ITYPE_ITEM)
				{
					for(KeyMapMap.MoveFirst(); KeyMapMap.IsValid(); KeyMapMap.MoveNext())
					{
						JMap<CColSortKey,bool>  & KeyMap = KeyMapMap.GetItem();
						for(KeyMap.MoveLast(); KeyMap.IsValid(); KeyMap.MovePrev())
							m_SortedRowIndex.Item(x++) = KeyMap.GetKey().m_Row;
					}
				}
				else
					m_SortedRowIndex.Item(x++) = Idx;
			}
		}
		break;

		case 2:
		{
			for(int x=0 ; x < Count;)
			{
				m_ItemRowIndex.MoveTo(x);
				int Idx = m_ItemRowIndex.GetItem();
				m_Item_Row_Types.MoveTo(Idx);
				if(m_Item_Row_Types.GetItem() == ITYPE_ITEM)
				{
					for(KeyMapMap.MoveFirst(); KeyMapMap.IsValid(); KeyMapMap.MoveNext())
					{
						JMap<CColSortKey,bool>  & KeyMap = KeyMapMap.GetItem();
						for(KeyMap.MoveFirst(); KeyMap.IsValid(); KeyMap.MoveNext())
							m_SortedRowIndex.Item(x++) = KeyMap.GetKey().m_Row;
					}
				}
				else
					m_SortedRowIndex.Item(x++) = Idx;
			}
		}
		break;
	}
	return true;
}

bool CCrossTable::CalculateSurlignage()
{
	JVector<CTableKey> Source;
	m_pDoc->Set_SurlignageList(Source, this);


	if((m_pDoc->Get_ClickMode() & CMODE_ORDONNER) == 0)
		return false;
	if(m_pDoc->Get_SurlignageStyle() == RSTYLE_NOSTYLE)
		return false;

	// Get page dimensions
	int NbPages = m_ItemPageIndex.GetCount();
	int ItemPages = m_pDoc->GetPageItems().GetCount();
	bool bItemP = false;
	if(ItemPages > 0 && m_pDoc->Get_TableStyle() & TSTYLE_ITEMSP)
		bItemP = true;

	// Get column dimensions
	int NbCols	= m_ItemColIndex.GetCount();
	int ItemCols = m_pDoc->GetColItems().GetCount();
	bool bItemC = false;
	if(ItemCols > 0 && m_pDoc->Get_TableStyle() & TSTYLE_ITEMSC)
		bItemC = true;

	// Get row dimensions
	int NbRows	= m_ItemRowIndex.GetCount();
	int ItemRows = m_pDoc->GetRowItems().GetCount();
	bool bItemR = false;
	if(ItemRows > 0 && m_pDoc->Get_TableStyle() & TSTYLE_ITEMSR)
		bItemR = true;

	// If there are no items then there is nothing to do
	if(!bItemP && !bItemR && !bItemC)
		return false;

	// Cell count
	int NbCells = 0;

	if(m_pDoc->Get_TableStyle() & TSTYLE_ORDONNERPAGE)
	{
		NbCells = 1;
		NbPages = 1;
	}
	else
	{
		if(bItemP)
			NbCells = ItemPages;
		else
			NbCells = 1;
	}

	if(bItemC)
		NbCells *= ItemCols;
	else
		NbCells *= 1;

	if(bItemR)
		NbCells *= ItemRows;
	else
		NbCells *= 1;

	NbCells *= m_UniversParams.GetCount();

	// Set the key count
	CTableSortKey * pTableKeys = new CTableSortKey[NbCells];

	int KeyCount = 0;
	// Step through the universes
	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{
		MAPCELLRESULTS * TCellResults = GetCellResults(m_UniversParams.GetIndex());
		int UniversIdx = m_UniversParams.GetIndex();

		// Step through the pages
		for(int Page = 0; Page < NbPages; ++Page)
		{
			// Check if we have page items or we are on the first page
			if(!bItemP && Page != 0)
				continue;

			int PageIdx = 0;

			if(m_pDoc->Get_TableStyle() & TSTYLE_ORDONNERPAGE)
			{
				PageIdx = GetPageItem(m_pDoc->GetCurPage());
			}
			else
			{
				// Find the page type
				m_ItemPageIndex.MoveTo(Page);
				PageIdx = m_ItemPageIndex.GetItem();
				m_Item_Page_Types.MoveTo(PageIdx);

				// If we have items and the page type is an item
				if(bItemP && m_Item_Page_Types.GetItem() != ITYPE_ITEM)
					continue;
			}

			// Step through the columns
			for(int Col = 0; Col < NbCols ; ++Col)
			{
				// Check if we have column items or we are on first column
				if(!bItemC && Col != 0)
					continue;

				// Find the column type
				m_ItemColIndex.MoveTo(Col);
				int ColIdx = m_ItemColIndex.GetItem();
				m_Item_Col_Types.MoveTo(ColIdx);

				// Check if we have items and the column type is an item
				if(bItemC && m_Item_Col_Types.GetItem() != ITYPE_ITEM)
					continue;

				// Step through the rows
				for(int Row = 0; Row < NbRows; ++Row)
				{
					// Check that we have row items or we are on first row
					if(!bItemR && Row != 0)
						continue;

					// find the row type
					m_ItemRowIndex.MoveTo(Row);
					int RowIdx = m_ItemRowIndex.GetItem();
					m_Item_Row_Types.MoveTo(RowIdx);

					// Check that we have items and that the row type is an item
					if(bItemR && m_Item_Row_Types.GetItem() != ITYPE_ITEM)
						continue;

					// Calculate the cell value
					double Result	= 0.0;

					// Flag si division par zéro
					bool DivZero			= false;

					// Flag si marge d'erreur atteinte
					bool MargeErreurDepasse = false;

					// Indication nombre de cas
					double NbCas = 0.0;

					// Marge d'erreur
					double MargeErreur;

					// Intervalle confiance
					double IntervalleConfiance	= 0.0;

					if(!GetCellValue(Result, m_pDoc->Get_SurlignageCalc(), RowIdx, ColIdx, UniversIdx, m_pDoc->Get_SurlignageStyle(), PageIdx, m_pDoc->Get_BaseColItem(), m_pDoc->Get_BaseRowItem(),DivZero,NbCas,MargeErreurDepasse,MargeErreur,IntervalleConfiance) || DivZero)
						Result = -1.0;
					switch(m_pDoc->Get_SurlignageStyle())
					{
						case RSTYLE_HORIZONTAL:
						case RSTYLE_VERTICAL:
						case RSTYLE_GLOBAL:
						{
							if(NbCas <= (double)m_pDoc->Get_SeuileLimit())
								Result = -1.0;
						}
						break;
					}
					// Stock the cell value
					pTableKeys[KeyCount].SetValue(Result,UniversIdx,PageIdx,RowIdx,ColIdx);
					++KeyCount;
				}
			}
		}
	}
	std::sort(pTableKeys,pTableKeys+NbCells);

	Source.Reset();
	Source.SetCount(NbCells);

	int i;
	for(i = NbCells-1, Source.MoveFirst(); i >= 0; Source.MoveNext(), --i)
		Source.GetItem() = pTableKeys[i];

	delete [] pTableKeys;
	m_pDoc->Set_SurlignageList(Source, this);
	return true;
}

// Column tracking
BOOL CCrossTable::OnTrackColWidth(ROWCOL nCol)
{
	// Only track row header widths
	return (nCol<m_Col_Hdr_Type.GetCount() && m_CurRow >= m_Row_Hdr_Type.GetCount());
}

// Row tracking
BOOL CCrossTable::OnTrackRowHeight(ROWCOL nRow)
{
	// Only track column header heights
	return (nRow<m_Row_Hdr_Type.GetCount() && m_CurCol >= m_Col_Hdr_Type.GetCount());
}

// Récupere les infos nb terrains et nb grandeurs en header col ou header ligne
JUnt32 CCrossTable::Get_NbHeaderColTerrain()
{
	// Les élmts terrains sont-ils en header ????
	m_NbHeaderColTerrain = 0;
	if (m_pDoc->Get_NbSelectedTerrains() > 1)
	{
		if (!(m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL))
		{	
			// Au moins 2 terrains disposés en col header
			m_NbHeaderColTerrain = 1;
		}
	}

	return m_NbHeaderColTerrain; 
}

JUnt32 CCrossTable::Get_NbHeaderColGrandeur()
{
	m_NbHeaderColGrandeur = 0;

	// Les élmts grandeurs sont-ils en header ????
	if (m_pDoc->Get_NbSelectedGrandeurs() > 1)
	{
		if (!(m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL))
		{
			// Au moins 2 grandeurs disposés en col header
			m_NbHeaderColGrandeur = 1;
		}
	}

	return m_NbHeaderColGrandeur;
}

JUnt32 CCrossTable::Get_NbHeaderRowTerrain()
{
	m_NbHeaderRowTerrain = 0;

	// Les élmts terrains sont-ils en header ????
	if (m_pDoc->Get_NbSelectedTerrains() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL)
		{
			// Au moins 2 terrains disposées en row header
			m_NbHeaderRowTerrain = 1; 
		}
	}

	return m_NbHeaderRowTerrain;
}

JUnt32 CCrossTable::Get_NbHeaderRowGrandeur()
{
	m_NbHeaderRowGrandeur = 0;

	// Les élmts grandeurs sont-ils en header ????
	if (m_pDoc->Get_NbSelectedGrandeurs() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
		{
			// Au moins 2 grandeurs disposées en row header
			m_NbHeaderRowGrandeur = 1; 
		}
	}

	return m_NbHeaderRowGrandeur;
}

// Mouse move over
BOOL CCrossTable::OnMouseMoveOver(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	// m_pGrid->LockUpdate(true);
	
    // Recup info cellules
	if (nRow * 10000 + nCol == m_pDoc->m_NoCellSurvol) return TRUE;

	m_pDoc->m_NoCellSurvol = nRow * 10000 + nCol;

	// Validité colonne et ligne
	ROWCOL nColValid = m_pGrid->CalcClientColFromPt(pt);
	ROWCOL nRowValid = m_pGrid->CalcClientRowFromPt(pt);

	// Les élmts grandeurs soFstnt-ils en header ????
	if (m_pDoc->Get_NbSelectedGrandeurs() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_GRANDEURCOL)
		{
			// Au moins 2 grandeurs disposées en row header
			m_NbHeaderRowGrandeur = 1; 
		}
		else
		{
			// Au moins 2 grandeurs disposés en col header
			m_NbHeaderColGrandeur = 1;
		}
	}

	// Les élmts terrains sont-ils en header ????
	if (m_pDoc->Get_NbSelectedTerrains() > 1)
	{
		if (m_pDoc->Get_TableStyle() & TSTYLE_TERRAINCOL)
		{
			// Au moins 2 terrains disposées en row header
			m_NbHeaderRowTerrain = 1; 
		}
		else
		{
			// Au moins 2 terrains disposés en col header
			m_NbHeaderColTerrain = 1;
		}
	}

	// Récupère ligne entete courante
	m_RowCurHeader      =   nRow;
	if (m_ItemRowCount > 0)
		m_RowCurHeader = ((m_RowCurHeader - (m_Row_StartItem + m_NbHeaderRowTerrain + m_NbHeaderRowGrandeur) ) / m_Result_Row_Count) * m_Result_Row_Count + m_Row_StartItem + m_NbHeaderRowTerrain + m_NbHeaderRowGrandeur;  

	// Récupère colonne entete courante
	m_ColCurHeader		=   nCol;
	if (m_ItemColCount > 0)
		m_ColCurHeader = ((m_ColCurHeader - (m_Col_StartItem + m_NbHeaderColTerrain + m_NbHeaderColGrandeur) ) / m_Result_Col_Count) * m_Result_Col_Count + m_Col_StartItem + m_NbHeaderColTerrain + m_NbHeaderColGrandeur;  


	// Coordonnées ligne colonne
	CELLTYPE Rt,Ct;
	int Ri,Ru,Rc,Rv;
	int Ci,Cu,Cc,Cv;
	Rt = GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);
	Ct = GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

	// Test si mouse dans le grid
	if (nColValid == GX_INVALID || nRowValid == GX_INVALID)                      
	{
		// Désaffiche l'objet tooltip
		this->m_pGrid->AfficheInfoCellule(nRow,nCol,"",false); 

		// Initialisation de la ligne et de la colonnne courante
		m_CurRow = -1;
		m_CurCol = -1;
		
		// Réinit cellule en cours
		m_RowCurHeader      =   -1;
		m_ColCurHeader		=   -1;

		return true;
	}

	// Client rect bas de la cellule survolée
	int TopRow		= 0;
	int BottomRow	= 0;
	this->m_pGrid->CalcLimitTopBottomRow(nRow, TopRow, BottomRow);

	//¨On reste sur la même cellule
	if(m_CurRow == nRow && m_CurCol == nCol)
	{
		if (nCol >= m_Col_StartItem)
		{
			// Affichage icone tri sur dernière ligne header ligne
			if (m_NbHeaderRowGrandeur == 1)
			{
				if (Rt == CTYPE_HEADERCALC && BottomRow - pt.y > 2)
				{
					// Repositionne curseur tri à false
					m_pDoc->SetCurseurTri(true); 

					// On set dans le mode Tri -- Et modifie cursor du Grid (StingRay)
					this->m_pGrid->SetGridCursor(IDC_AN_TRI);
					m_CurRow == nRow;
					m_CurCol == nCol;
					return true;
				}	
			}
			else if (m_NbHeaderRowTerrain == 1)
			{
				if (Rt == CTYPE_HEADERTERRAIN && BottomRow - pt.y > 2)
				{
					// Repositionne curseur tri à false
					m_pDoc->SetCurseurTri(true); 

					// On set dans le mode Tri -- Et modifie cursor du Grid (StingRay)
					this->m_pGrid->SetGridCursor(IDC_AN_TRI);
					m_CurRow == nRow;
					m_CurCol == nCol;
					return true;
				}	
			}
			else
			{
				if (Rt == CTYPE_HEADER2 && BottomRow - pt.y > 2)
				{
					// Repositionne curseur tri à false
					m_pDoc->SetCurseurTri(true); 

					// On set dans le mode Tri -- Et modifie cursor du Grid (StingRay)
					this->m_pGrid->SetGridCursor(IDC_AN_TRI);
					m_CurRow == nRow;
					m_CurCol == nCol;
					return true;
				}	
			}
		}
	}

	// Initialise timing tooltip
	this->m_pGrid->ToolTipSetDelayTime(100);
	
	// Affichage icone tri sur dernière ligne header ligne
	if (nCol >= m_Col_StartItem)
	{	
		if (m_NbHeaderRowGrandeur == 1)
		{
			if (Rt == CTYPE_HEADERCALC && BottomRow - pt.y > 2)
			{
				// Repositionne curseur tri à false
				m_pDoc->SetCurseurTri(true); 

				// On set dans le mode Tri -- Et modifie cursor du Grid (StingRay)
				this->m_pGrid->SetGridCursor(IDC_AN_TRI);
				m_CurRow == nRow;
				m_CurCol == nCol;
				return true;
			}	
		}
		else if (m_NbHeaderRowTerrain == 1)
		{
			if (Rt == CTYPE_HEADERTERRAIN && BottomRow - pt.y > 2)
			{
				// Repositionne curseur tri à false
				m_pDoc->SetCurseurTri(true); 

				// On set dans le mode Tri -- Et modifie cursor du Grid (StingRay)
				this->m_pGrid->SetGridCursor(IDC_AN_TRI);
				m_CurRow == nRow;
				m_CurCol == nCol;
				return true;
			}	
		}
		else
		{
			if (Rt == CTYPE_HEADER2 && BottomRow - pt.y > 2)
			{
				CString StrCell = "";

				// Repositionne curseur tri à false
				m_pDoc->SetCurseurTri(true); 

				// On set dans le mode Tri -- Et modifie cursor du Grid (StingRay)
				this->m_pGrid->SetGridCursor(IDC_AN_TRI);
				m_CurRow == nRow;
				m_CurCol == nCol;

				/*
				int Ci,Cu,Cc,Cv;
				CELLTYPE Ct			=	GetColCoordinates(nCol,Ci,Cu,Cc,Cv);
				GetColCellString(StrCell, CTYPE_HEADER2,Cu, nRow, nCol);
				*/

				StrCell = this->m_pGrid->GetValueRowCol(nRow, nCol);
				this->m_pGrid->AfficheInfoCellule(nRow,nCol,StrCell,true); 
				return true;
			}	
		}
	}

	// Repositionne curseur tri à false
	m_pDoc->SetCurseurTri(false); 

	// Remet le curseur via le mode en cours
	m_pDoc->UpdateCursor(); 

	// Surlignage des headers rows et cols de la cellule courante
	if(m_CurRow != nRow || m_CurCol != nCol)
	{
		if (Rt == CTYPE_ITEMCELL  && Ct == CTYPE_ITEMCELL)
		{
			// A VOIR
			/*
			// Surlignage des headeres row et col associés à la cellule courante
			CGXStyle style;
			GXModifyType mt = gxCopy;
			int nType		= 0;
			GetStyleRowCol(nRow, nCol, style, mt, nType);
			
			// Mse à jour uniquement des zones à rafraichir
			CRgn rgn;
			CRect Rect;
			Rect = this->m_pGrid->CalcRectFromRowCol(nRow, nCol);

			// puis on calcule la zone maxi à rafraichir pour la cellule
			int HtCell	= this->m_pGrid->GetRowHeight(nRow);
			int LgCell	= this->m_pGrid->GetColWidth(nCol); 

			Rect.top	-= HtCell * m_ItemRowCount; // * 2;   
			Rect.bottom += HtCell * m_ItemRowCount; // * 2;   
			Rect.left   -= LgCell * m_ItemColCount; // * 2;    
			Rect.right  += LgCell * m_ItemColCount; // * 2;    
			rgn.CreateRectRgnIndirect(&Rect);
			this->m_pGrid->InvalidateRgn(&rgn); 
			rgn.DeleteObject();

			// puis on calcule a zone maxi à rafraichir pour la zone header ligne
			ROWCOL nColColor = 1 + NbHeaderColGrandeur + NbHeaderColTerrain;
			Rect = this->m_pGrid->CalcRectFromRowCol(nRow, nColColor);
			Rect.top -= HtCell * m_ItemRowCount;
			Rect.bottom += HtCell * m_ItemRowCount;
			rgn.CreateRectRgnIndirect(&Rect);
			this->m_pGrid->InvalidateRgn(&rgn); 
			rgn.DeleteObject();

			// puis on calcule a zone maxi à rafraichir pour la zone header colonne
			ROWCOL nRowColor = 1 + NbHeaderRowGrandeur + NbHeaderRowTerrain;
			Rect = this->m_pGrid->CalcRectFromRowCol(nRowColor, nCol);
			Rect.left  -= LgCell * m_ItemColCount;
			Rect.right += LgCell * m_ItemColCount;
			rgn.CreateRectRgnIndirect(&Rect);
			this->m_pGrid->InvalidateRgn(&rgn); 
			rgn.DeleteObject();
			*/
		}
	}
	
	// Uniquement si on n'est en mode marge erreur
	if (m_pDoc->CalculMargeErreur() == true)
	{
		m_CurRow = nRow;
		m_CurCol = nCol;
		
		// Calcul valeur réelle cellule
		CString StrCell = "";
		// StrCell = this->m_pGrid->GetValueRowCol(nRow, nCol);

		// Calcul valeur réelle marge erreur
		// if (StrCell == "***")
		int NbHeaderRow = m_Row_Hdr_Type.GetCount();
		int NbHeaderCol = m_Col_Hdr_Type.GetCount();

		// Attention, n'afficher la marge d'erreur que si type == RTYPE_EFFECTIVE_000 ou == RTYPE_EFFECTIVE_CIBLE
		const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();
		
		int BaseColItem		=	m_pDoc->Get_BaseColItem();
		int BaseRowItem		=	m_pDoc->Get_BaseRowItem();
		int Pi				=	GetPageItem(m_pDoc->GetCurPage());

		int Ri,Ru,Rc,Rv;
		CELLTYPE Rt			= GetRowCoordinates(nRow, Ri, Ru, Rc, Rv);
		CalcTypes.MoveTo(Rc);
		RESULTTYPE Type	= CalcTypes.GetItem();

		int Ci,Cu,Cc,Cv;
		CELLTYPE Ct			=	GetColCoordinates(nCol,Ci,Cu,Cc,Cv);

		// Style de résultat
		RESULTSTYLE ResStyle = GetResultStyle(Rv,Cv);

		// Uniquement en style grandeur, dans les cellules chiffrées et en effectif
		if (ResStyle == RSTYLE_RESULT &&  nRow >= NbHeaderRow && nCol >= NbHeaderCol &&  ((Type == RTYPE_EFFECTIVE_000) || (Type == RTYPE_EFFECTIVE_CIBLE))) 
		{
			// recup univers
			UINT TableStyle		=	m_pDoc->Get_TableStyle();
			int Universe = 0;

			// Selon style
			if(TableStyle & TSTYLE_TERRAINCOL)
			{
				int Ci,Cu,Cc,Cv;
				CELLTYPE Ct			=	GetColCoordinates(nCol,Ci,Cu,Cc,Cv);
				Universe = Cu;
			}
			else
			{
				int Ri,Ru,Rc,Rv;
				CELLTYPE Rt			=	GetRowCoordinates(nRow,Ri,Ru,Rc,Rv);
				Universe = Ru;
			}

			// Paramétre de retour
			double Result				= 0.0;
			bool DivZero				= false;
			double NbCas				= 0.0;
			bool MargeErreurDepasse		= false;
			double MargeErreur			= 0.0;
			double IntervalleConfiance	= 0.0;
			
			// Récupére valeurs cellules
			if (GetCellValue(Result,   RTYPE_EFFECTIVE_000, Ri,	Ci,
							Universe, RSTYLE_RESULT, Pi, BaseColItem, BaseRowItem,
							DivZero, NbCas, MargeErreurDepasse, MargeErreur,IntervalleConfiance))
			{
				// Affiche valeur réelle de la marge d'erreur
				if (MargeErreur <= m_pDoc->Get_Seuil_MargeErreur_Appli())   //  MargeErreurMax plus utilisée
				{
					// Affichage intervalle de confiance
					if (Type == RTYPE_EFFECTIVE_000)
						StrCell.Format("%s%0.0f","Intervalle de confiance ± ",IntervalleConfiance/1000); 
					else
						StrCell.Format("%s%0.0f","Intervalle de confiance ± ",IntervalleConfiance); 
				}
				else if (MargeErreur > m_pDoc->Get_Seuil_MargeErreur_Appli() && (MargeErreur < 100000)) //  MargeErreurMax plus utilisée
				{
					// Marge d'erreur infini due à une division par zero (voir formule)
					StrCell.Format("%s%0.2f%%","Marge d'erreur : ",MargeErreur); 
				}
				else if (MargeErreur >= 100000)
				{
					// On indique impossibilité de déterminer la marge d'erreur
					StrCell.Format("%s","Intervalle de tolérance indéterminé"); 
				}

				// Affiche l'objet tooltip (Modif Juillet 2013)
				this->m_pGrid->AfficheInfoCellule(nRow,nCol,StrCell,true); 
			}
			else
			{
				// Affiche l'objet tooltip (Modif Juillet 2013)
				StrCell.Format("%s","IC non défini");
				this->m_pGrid->AfficheInfoCellule(nRow,nCol,StrCell,true); 
			}
	
			/*
			IntervalleConfiance = 999;
			StrCell.Format("%s%0.0f","Intervalle de confiance ± ",IntervalleConfiance/1000); 
			StrCell.Format("%s%0.0f","Intervalle de confiance ± ",IntervalleConfiance/1000);
			// Affiche l'objet tooltip (Modif Juillet 2013)
			this->m_pGrid->AfficheInfoCellule(nRow,nCol,StrCell,true); 
			*/

		}
		else
		{
			// Désaffiche l'objet tooltip
			// this->m_pGrid->AfficheInfoCellule(nRow,nCol,"",false); 

			StrCell.Format("%s","IC non défini !!!");
			this->m_pGrid->AfficheInfoCellule(nRow,nCol,StrCell,false); 
		}
	}
	
	ROWCOL Top,Left,Bottom,Right;
	Top = Left = 0;
	Bottom = m_Row_Hdr_Type.GetCount()-1; 
	Right = m_Col_Hdr_Type.GetCount()-1;

	if(m_CurRow != nRow)
	{
		m_CurRow = nRow;
		Bottom = m_NbRows;
	}
	if(m_CurCol != nCol)
	{
		m_CurCol = nCol;
		Right = m_NbCols;
	}

	// m_pGrid->LockUpdate(false);

	return true;
}

void CCrossTable::PageChanged(int Page)
{
	if(m_NbPages && m_pDoc->GetCurPage() != Page)
	{
		m_pDoc->SetCurPage(Page);
		m_pDoc->Set_SortPage(GetPageItem(Page));
		m_pDoc->Add_Calculation(CALC_SURLIGNAGE);
		CalculateItems();//SortColumn();
	}
}

void CCrossTable::GetFormattedResult(CString & ResultOut, double ResultIn, RESULTSTYLE Style, RESULTTYPE Type, int RowItem, int ColItem, int BaseRowItem, int BaseColItem, bool DivZero, double NbCas, bool MargeErreurDepasse, bool ItemQuanti, bool RowMoyennable, bool ColMoyennable)
{
	bool NicheQuantifiable = false;

	ResultOut.Empty();

	m_Item_Col_Types.MoveTo(BaseColItem);
	m_Item_Row_Types.MoveTo(BaseRowItem);
	bool bPct = false;


	if (m_Item_Row_Types.GetItem() == ITYPE_NICHE)
		bPct = true;


	if(m_pDoc->Get_TableStyle() & TSTYLE_UNIVERSEX)
	{
		if((m_Item_Col_Types.GetItem() == ITYPE_UNIVERSE || m_Item_Col_Types.GetItem() == ITYPE_NICHE) && (m_Item_Row_Types.GetItem() == ITYPE_UNIVERSE || m_Item_Row_Types.GetItem() == ITYPE_NICHE))
			bPct = true;
	}
	else
	{
		if((m_Item_Col_Types.GetItem() == ITYPE_UNIVERSE || m_Item_Col_Types.GetItem() == ITYPE_NICHE) && (m_Item_Row_Types.GetItem() == ITYPE_UNIVERSE || m_Item_Row_Types.GetItem() == ITYPE_NICHE))
			bPct = true;
	}


	ITEMTYPE RowType = m_Item_Row_Types.GetItem();
	ITEMTYPE ColType = m_Item_Col_Types.GetItem();

	// Flag pour indiquer si Niche contient des elmts quantifiables (en lignes ou colonnes)
	if (m_Item_Row_Types.GetItem() == ITYPE_NICHE || m_Item_Col_Types.GetItem() == ITYPE_NICHE)
	{
		// Voir en ligne
		const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetRowItems();
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		{
			// Récupére la categorie question
			const CItemWrapper *pItemWrapper = Items.GetItem();

			// Vérifie si on a affaire à un item numérisé quantitatif
			const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

			// Cet item est-il moyennable
			if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
			{
				NicheQuantifiable = true;
				break;
			}
		}

		// Voir en colonnes
		if (!NicheQuantifiable)
		{
			const CONST_LIST_ITEM_PUPITRE &	Items =	m_pDoc->GetColItems();
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
			{
				// Récupére la categorie question
				const CItemWrapper *pItemWrapper = Items.GetItem();

				// Vérifie si on a affaire à un item numérisé quantitatif
				const CItemConstruit *pItemConstruit = pItemWrapper->PtrItem(); 

				// Cet item est-il moyennable
				if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
				{
					NicheQuantifiable = true;
					break;
				}
			}
		}
	}
	
	switch(Style)
	{
		// The main value
		case RSTYLE_RESULT:
		{
			switch(Type)
			{
				// Résultat effectifs / nombre de cas
				case RTYPE_EFFECTIVE_000:
				case RTYPE_NBCAS:
				case RTYPE_EFFECTIVE_CIBLE:
					ResultOut.Format("%.0f",ResultIn);
				break;

				// Résultat quantité agrégé
				case RTYPE_QUANTITY_000:
				{	
					if (((RowMoyennable && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE) || (ColMoyennable && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE) || (m_pDoc->m_pItemWrapperMoy && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)) && (ResultIn > 0.0 || ItemQuanti))
						// Affiche les résultats si on est sur des items quantis ou que les valeurs sont définies
						ResultOut.Format("%.0f",ResultIn);
					else
					{
						ResultOut = _T("*");

						// AOUT 2013 Voir si Univers Cible est quantitatif, auquel cas on affiche
						const CItemConstruit *pItemConstruit = m_pDoc->Get_SelectedCible();
						if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
						{
							ResultOut.Format("%.0f",ResultIn);
						}
						
						if (ResultIn > 0)
						{
							// Uniquement pour test
							if (m_pDoc->GetForceQuantiCalcul())
							{
								ResultOut.Format("%.0f",ResultIn);
							}

							// Modif spécial pour Niche si au moins 1 elmt moyennable
							if (m_pDoc->GetNicheQuantifiable() == true)
							{
								ResultOut.Format("%.0f",ResultIn);
							}
						}
					}
				}
				break;

				// Résultat valeur moyenne
				case RTYPE_AVERAGEQUANTUM:
				{	
					// Affiche les résultats si on est sur des items quantis ou que les valeurs sont définies
					if (((RowMoyennable && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE) || (ColMoyennable && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE) || (m_pDoc->m_pItemWrapperMoy && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)) && (ResultIn > 0.0 || ItemQuanti))
						ResultOut.Format("%.0f",ResultIn);
					else
					{
						ResultOut = _T("*");

						// AOUT 2013Voir si Univers Cible est quantitatif, auquel cas on affiche
						const CItemConstruit *pItemConstruit = m_pDoc->Get_SelectedCible();
						if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
						{
							ResultOut.Format("%.0f",ResultIn);
						}

						if (ResultIn > 0)
						{
							// Uniquement pour test
							if (m_pDoc->GetForceQuantiCalcul())
								ResultOut.Format("%.0f",ResultIn);

							// Modif spécial pour Niche si au moins 1 elmt moyennable
							if (m_pDoc->GetNicheQuantifiable() == true)
							{
								ResultOut.Format("%.0f",ResultIn);
							}
						}

						/*
						if(NicheQuantifiable == true)
						{
							ResultOut.Format("%.0f",ResultIn);
						}
						*/
					}
				}
				break;

				// Résultat valeur moyenne (avec 2 décimales)
				case RTYPE_AVERAGEQUANTUM_P00:
				{
					// Affiche les résultats si on est sur des items quantis ou que les valeurs sont définies
					if (((RowMoyennable && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_LIGNE) || (ColMoyennable && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_COLONNE) || (m_pDoc->m_pItemWrapperMoy && m_pDoc->m_ModeMoyenne == m_pDoc->MOY_TABLEAU)) && (ResultIn > 0.0 || ItemQuanti))
						ResultOut.Format("%.2f",ResultIn);
					else
					{
						ResultOut = _T("*");

						// AOUT 2013 Voir si Univers Cible est quantitatif, auquel cas on affiche
						const CItemConstruit *pItemConstruit = m_pDoc->Get_SelectedCible();
						if (m_pDoc->m_DlgAnalyseur.m_pDlgTableau->ItemQuantitatif(pItemConstruit,m_pDoc->Get_SourceIdRef(), m_pDoc->Get_TerrainIdRef()))
						{
							ResultOut.Format("%.2f",ResultIn);
						}

						if (ResultIn > 0)
						{
							// Uniquement pour test
							if (m_pDoc->GetForceQuantiCalcul())
								ResultOut.Format("%.2f",ResultIn);

							// Modif spécial pour Niche si au moins 1 elmt moyennable
							if(m_pDoc->GetNicheQuantifiable() == true)
							{
								ResultOut.Format("%.2f",ResultIn);
							}
						}
						
						/*
						if(NicheQuantifiable == true)
						{
							ResultOut.Format("%.2f",ResultIn);
						}
						*/
					}
				}
				break;

				// Autres résultats
				case RTYPE_CONCOMITENCE:
				case RTYPE_COEFFYULE:
					ResultOut.Format("%.0f",ResultIn*100.0);
				break;

				// Formattage du ZScore
				case RTYPE_ZSCORE:
					ResultOut.Format("%.0f",ResultIn);
				break;

	
				case RTYPE_PENETRATION:
				case RTYPE_CENTRAGE:
				if(bPct)
					ResultOut.Format("%.2f%%",ResultIn*100.0);
				else
					ResultOut.Format("%.0f",ResultIn*100.0);
				break;
			}
		}
		break;

		// The horizontal value
		case RSTYLE_HORIZONTAL:
		{
			if(RowItem != BaseRowItem && ColItem != BaseColItem && NbCas <= (double)m_pDoc->Get_SeuileLimit())
			{
				ResultOut = _T("#");
				return;
			}

			// Gestion du style "Corrélation" - Modif ALAIN
			UINT ValResult = m_pDoc->Get_ResultStyle();
			UINT VAlCorr = RSTYLE_HORZ_CORRELATION;

			if(m_pDoc->Get_ResultStyle()& RSTYLE_HORZ_CORRELATION)
			{
				// Format spécifique corrélation
				ResultOut.Format("%.0f",ResultIn*100.0);
				return;
			}

			switch(Type)
			{
				case RTYPE_EFFECTIVE_000:
				case RTYPE_NBCAS:
				case RTYPE_QUANTITY_000:
				case RTYPE_EFFECTIVE_CIBLE:
				{
					// Percentage
					if(bPct)
					{
						ResultOut.Format("%.2f%%",ResultIn*100.0);
						break;
					}
				}

				case RTYPE_PENETRATION:
				case RTYPE_CENTRAGE:
				case RTYPE_AVERAGEQUANTUM:
				case RTYPE_AVERAGEQUANTUM_P00:
				case RTYPE_CONCOMITENCE:
				case RTYPE_COEFFYULE:
				{
					// Indices
					ResultOut.Format("%.0f",ResultIn*100.0);
				}
				break;

				// Formattage du ZScore
				case RTYPE_ZSCORE:
				{
					if (bPct)
						ResultOut.Format("%.0f",ResultIn * 100.0);
					else
						ResultOut.Format("%.0f",ResultIn);
				}
				break;
			}
		}
		break;

		case RSTYLE_VERTICAL:
		{
			if(RowItem != BaseRowItem && ColItem != BaseColItem && NbCas <= (double)m_pDoc->Get_SeuileLimit())
			{
				ResultOut = _T("#");
				return;
			}
			switch(Type)
			{
				case RTYPE_EFFECTIVE_000:
				case RTYPE_NBCAS:
				case RTYPE_QUANTITY_000:
				case RTYPE_EFFECTIVE_CIBLE:
				{
					// Percentage
					if(bPct)
					{
						ResultOut.Format("%.2f%%",ResultIn*100.0);
						break;
					}
				}

				
				case RTYPE_PENETRATION:
				case RTYPE_CENTRAGE:
				case RTYPE_AVERAGEQUANTUM:
				case RTYPE_AVERAGEQUANTUM_P00:
				case RTYPE_CONCOMITENCE:
				case RTYPE_COEFFYULE:
				{
					// Indices
					ResultOut.Format("%.0f",ResultIn*100.0);
				}
				break;

				// Formattage du ZScore
				case RTYPE_ZSCORE:
				{
					if (bPct)
						ResultOut.Format("%.0f",ResultIn * 100.0);
					else
						ResultOut.Format("%.0f",ResultIn);
				}
				break;

			}
		}
		break;

		// The global value
		case RSTYLE_GLOBAL:
		{
			if(RowItem != BaseRowItem && ColItem != BaseColItem && NbCas <= (double)m_pDoc->Get_SeuileLimit())
			{
				ResultOut = _T("#");
				return;
			}
			if(m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION|RSTYLE_GBL_PROXIMITY|RSTYLE_GBL_ZSCORE))
			{
				if(RowItem == m_pDoc->Get_BaseRowItem() || ColItem == m_pDoc->Get_BaseColItem())
					break;

				// Not in the 
				ResultOut.Format("%.0f",ResultIn*100.0);
			}
			else
			{
				switch(Type)
				{
					case RTYPE_EFFECTIVE_000:
					case RTYPE_NBCAS:
					case RTYPE_QUANTITY_000:
					case RTYPE_EFFECTIVE_CIBLE:
					{
						// Percentage
						if(bPct)
						{
							ResultOut.Format("%.2f%%",ResultIn*100.0);

							break;
						}
					}

					case RTYPE_AVERAGEQUANTUM:
					case RTYPE_AVERAGEQUANTUM_P00:
					{
						// Indice
						ResultOut.Format("%.0f",ResultIn*100.0);
					}
					break;

					case RTYPE_PENETRATION:
					case RTYPE_CENTRAGE:
					case RTYPE_CONCOMITENCE:
					case RTYPE_COEFFYULE:
					{
						// Indice
						ResultOut.Format("");
					}
					break;

					// Formattage du ZScore
					case RTYPE_ZSCORE:
					{
						if (bPct)
							ResultOut.Format("%.0f",ResultIn * 100.0);
						else
							ResultOut.Format("%.0f",ResultIn);
					}
					break;
				}
			}
		}
		break;
	}

	// Dernier Formatage Résultat
	if(DivZero)
	{
		// Division par zéro, indiquer l'incohérence
		ResultOut = _T("*");
	}
	else if (MargeErreurDepasse && m_pDoc->CalculMargeErreur() == true)
	{
		// Marge d'erreur atteinte, le signaler
		ResultOut = _T("***");
	}
	/*
	else if (Type == RTYPE_NBCAS && NbCas < (double)m_pDoc->Get_SeuileLimit())
	{
		// Indique que nb cas non affichable / trop petit (MAI 2014 voir avec JF)					
		ResultOut = _T("###");
	}
	*/
	else		
	{
		// Tous les autres cas, résultat valide
		Format(ResultOut);
		if (ResultOut == "-0") ResultOut = "0";
	}
}

// Format the given data values
void CCrossTable::Format(CString &Data)
{
	// Spaces, seperate thousands
	// . replaced by ,
	// 1230200.34 becomes 1 230 200,34
	int		count	= Data.GetLength();
	int		Ths		= 0;
	bool	Pct		= false;
	CString Str		= "";

	for(int x = count-1; x >= 0 ; --x)
	{
		switch(Data.GetAt(x))
		{
			case '%':
				Pct = true;
				Ths = 0;
				Str.Insert(0,Data.GetAt(x));
			break;

			case ',':
			case '.':
//				Str.Insert(0,_T(","));
				Str.Insert(0,Data.GetAt(x));
				Ths = 0;
				if(Pct)
					Pct = false;
			break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(Ths == 3)
				{
					Str.Insert(0,_T(" "));
					Ths = 0;
				}
				Str.Insert(0,Data.GetAt(x));
				++Ths;
			break;

			default:
				Ths = 0;
				Str.Insert(0,Data.GetAt(x));
			break;
		}
	}
	Data = Str;
}

bool CCrossTable::GetCellValue(double & Result, RESULTTYPE Type, int Ri, int Ci, int Universe, RESULTSTYLE Style,int PageItem, int BaseColItem, int BaseRowItem, bool & DivZero, double & NbCas, bool &MargeErreurDepasse, double & MargeErreur, double & IntervalleConfiance)
{
	Result = 0.0;
	CCoordRowCol ckey;
	ICellResult * pCellRes	=	0;
	ICellResult * pGlobRes	=	0;
	ICellResult * pHorzRes	=	0;
	ICellResult * pVertRes	=	0;
	double ValCell			=	0.0;
	double ValHorz			=	0.0;
	double	ValRef			=	0.0;
	double ValVert			=	0.0;

	
	MAPCELLRESULTS * CellResults = GetCellResults(Universe);
	if(CellResults == 0)
		return false;// Bad universe index

	// Get current cell results
	ckey.SetCoord(PageItem,Ri,Ci);
	CellResults->MoveTo(ckey);
	if(!CellResults->IsValid())
		return false;// Bad cell
	// Get the Cell Results
	pCellRes = CellResults->GetItem().get();
	// Get the number of cases for the cell
	NbCas = pCellRes->GetResult(RTYPE_NBCAS);

	// Get Global Cell results
	ckey.SetCoord(PageItem ,BaseRowItem ,BaseColItem);
	CellResults->MoveTo(ckey);
	if(!CellResults->IsValid())
		return false;// Bad cell
	pGlobRes = CellResults->GetItem().get();

	// Get Horizontal Cell
	ckey.SetCoord(PageItem,Ri ,BaseColItem);
	CellResults->MoveTo(ckey);
	if(!CellResults->IsValid())
		return false;// Bad cell
	pHorzRes = CellResults->GetItem().get();

	// Get vertical Cell
	ckey.SetCoord(PageItem,BaseRowItem ,Ci);
	CellResults->MoveTo(ckey);
	if(!CellResults->IsValid())
		return false;// Bad cell
	pVertRes = CellResults->GetItem().get();

	// Get Global Cell results
	ckey.SetCoord(0,0,0);
	CellResults->MoveTo(ckey);
	if(!CellResults->IsValid())
		return false;// Bad cell
	ICellResult * pUnivRes = CellResults->GetItem().get();


	// Pour marge erreur
	double ConstTerrainME		= 0.0;					// Facteur Constante Marge d'Erreur
	IntervalleConfiance			= 0.0;					// Intervalle de confiance	
	MargeErreur					= 0.0;					// Marge d'erreur finale
	
	double EffBase		= pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE, 0, m_pDoc->m_ModeCalculMoyenne);
	double NbCasBase	= pGlobRes->GetResult(RTYPE_NBCAS, 0, m_pDoc->m_ModeCalculMoyenne);

	// Calcul Constante Terrain
	if (NbCasBase)
		ConstTerrainME = (1.96 * EffBase) / sqrt(NbCasBase);

	// Voir pour SQN-NetRating
	// Poids moyen d'un individu
	// double PoidMoyenIndiv = 0.0;
	// double AudienceExtrapoleeSite = 0.0;
	// double SeuilLimite = 40.0 
	// if (NbCasUniv)
	// {
	// 	PoidMoyenIndiv = EffUniv / NbCasUniv;
	//	AudienceExtrapoleeSite = PoidMoyenIndiv * SeuilLimite
	// }

	// Recherche résultat selon le type de grandeur désirée
	switch(Type)
	{
		case RTYPE_EFFECTIVE_000:
		case RTYPE_EFFECTIVE_CIBLE:
		case RTYPE_NBCAS:
		case RTYPE_QUANTITY_000:
		case RTYPE_AVERAGEQUANTUM:
		case RTYPE_AVERAGEQUANTUM_P00:
		{
			ValCell =	pCellRes->GetResult(Type,0,m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
			ValHorz =	pHorzRes->GetResult(Type,0,m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
			ValVert =	pVertRes->GetResult(Type,0,m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);
			ValRef  =	pGlobRes->GetResult(Type,0,m_pDoc->m_ModeCalculMoyenne == m_pDoc->CALC_MOY_0_EXCLUS);

			if (Type  == RTYPE_EFFECTIVE_000 || Type == RTYPE_EFFECTIVE_CIBLE)
			{
				// Calcul Marge d'erreur
				double PenItem = 0.0;

				// if (pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE) > 0.0)
				if (EffBase > 0.0)
				{
					// Calcul Pénétration Item via Univers
					double EffItem = pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
					PenItem = EffItem / EffBase;

					// Calcul intervalle de confiance
					double RacineCarre = sqrt(PenItem * (1-PenItem));
					IntervalleConfiance = ConstTerrainME * sqrt(PenItem * (1-PenItem));

					// Calcul marge erreur
					if (EffItem > 0.0)
					{
						// Des invidus sont présents, on peut calculer la marge d'erreur
						MargeErreur = (IntervalleConfiance / EffItem) * 100;
					}
					// On laisse 0 en indiquant Intervalle de tolérance indéterminé
					else
					{
						// Cas où non défini, puisque aucun individu
						MargeErreur = 100000;
					}
				}
			}
		}
		break;

		case RTYPE_PENETRATION:
		{
			if(pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE))
			{
				ValCell =	pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
				ValRef	=	pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
				ValHorz	=	pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
				ValVert =	pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
			}
		}
		break;

		case RTYPE_CENTRAGE:
		{
			if(pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE) != 0.0)
				ValHorz = pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

			if(pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE) != 0.0)
				ValVert = pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

			if(pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE) != 0.0)
				ValRef  = pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

			if(pCellRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)!= 0.0)
				ValCell = pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	/	pCellRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE);
		}
		break;

		case RTYPE_CONCOMITENCE:
		{
			if(pCellRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)!= 0.0)
				ValCell =	(pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE)		*	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE))	/
							(pCellRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	*	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE));

			if(pHorzRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE) != 0.0)
				ValHorz =	(pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE)		*	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE))	/
							(pHorzRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	*	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE));

			if(pVertRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE) != 0.0)
				ValVert =	(pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE)		*	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE))	/
							(pVertRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	*	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE));

			if(pGlobRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE) != 0.0)
				ValRef =	(pGlobRes->GetResult(RTYPE_EFFECTIVE_CIBLE)		*	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE))	/
							(pGlobRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	*	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE));
		}
		break;

		// Calcul du coefficient de Yule
		case RTYPE_COEFFYULE:
		{
			double r	=	pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
			if(abs(r) < 1.0e-2)	r = 0.0;
			double c	=	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r;
			if(abs(c) < 1.0e-2)	c = 0.0;
			double t	=	pCellRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	-	r;
			if(abs(t) < 1.0e-2)	t = 0.0;
			double v	=	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	-	(pCellRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	+
																				pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r);
			if(abs(v) < 1.0e-2)	v = 0.0;

			double rv	=	r	*	v;
			double ct	=	c	*	t;

			ValCell = 0.0;
			double rvct =	rv+ct;
			if(abs(rvct) >= 1.0e-2)
				ValCell = 1.0-((1.0	+	(((rv)	-	(ct))	/	((rv)	+	(ct))))	/	2.0);
			else
				DivZero = true;

			r	=	pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
			if(abs(r) < 1.0e-2)	r = 0.0;
			c	=	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r;
			if(abs(c) < 1.0e-2)	c = 0.0;
			t	=	pHorzRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	-	r;
			if(abs(t) < 1.0e-2)	t = 0.0;
			v	=	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	-	(pHorzRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	+	
																		pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r);
			if(abs(v) < 1.0e-2)	v = 0.0;

			rv	=	r	*	v;
			ct	=	c	*	t;

			ValHorz =	0.0;
			rvct =	rv+ct;
			if(abs(rvct) >= 1.0e-2)
				ValHorz	=	1.0-((1.0	+	(((rv)	-	(ct))	/	((rv)	+	(ct))))	/	2.0);

			r	=	pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE);
			if(abs(r) < 1.0e-2)	r = 0.0;
			c	=	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r;
			if(abs(c) < 1.0e-2)	c = 0.0;
			t	=	pVertRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	-	r;
			if(abs(t) < 1.0e-2)	t = 0.0;
			v	=	pUnivRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	-	(pVertRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	+	
																		pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r);
			if(abs(v) < 1.0e-2)	v = 0.0;

			rv	=	r	*	v;
			ct	=	c	*	t;

			ValVert =	0.0;
			rvct =	rv+ct;
			if(abs(rvct) >= 1.0e-2)
				ValVert =	1.0-((1.0	+	(((rv)	-	(ct))	/	((rv)	+	(ct))))	/	2.0);

			r	=	pGlobRes->	GetResult(RTYPE_EFFECTIVE_CIBLE);
			if(abs(r) < 1.0e-2)	r = 0.0;
			c	=	pUnivRes->	GetResult(RTYPE_EFFECTIVE_CIBLE)		-	r;
			if(abs(c) < 1.0e-2)	c = 0.0;
			t	=	pGlobRes->	GetResult(RTYPE_EFFECTIVE_NONCIBLE)		-	r;
			if(abs(t) < 1.0e-2)	t = 0.0;
			v	=	pUnivRes->	GetResult(RTYPE_EFFECTIVE_NONCIBLE)		-	(pGlobRes->GetResult(RTYPE_EFFECTIVE_NONCIBLE)	+	
																			pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE)	-	r);
			if(abs(v) < 1.0e-2)	v = 0.0;

			rv	=	r	*	v;
			ct	=	c	*	t;

			ValRef =	0.0;
			rvct =	rv+ct;
			if(abs(rvct) >= 1.0e-2)
				ValRef =	1.0-((1.0	+	(((rv)	-	(ct))	/	((rv)	+	(ct))))	/	2.0);
		}
		break;

		// Calcul du ZScore
		// Z-Score = (P1-P2) / (sqr(P(1-P))*((1/n1)+(1/n2))
		// où P =((n1*P1)+(n2*P2))/(n1+n2)
		case RTYPE_ZSCORE:
		{
			double EffUniv = pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

			// Effectif résultant 
			double EffCellule			=	pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

			// Effectif en horizontal
			double EffHorz				=	pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

			// Effectif en vertical
			double EffVert_n1			=	pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE);	

			// Effectif Univers en horizontal
			double ResRef_n2			=	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);	

			// P1 et P2
			double PrctCellule_P1	= 0.0;		
			double PrctUniv_P2		= 0.0;		
			if (EffVert_n1 > 0 && ResRef_n2 > 0)
			{
				// Calcul P1 et P2
				PrctCellule_P1	=  EffCellule / EffVert_n1;		
				PrctUniv_P2		=  EffHorz / ResRef_n2;	

				// Calcul P
				if (EffVert_n1 + ResRef_n2 > 0)
				{
					double Prop = ((EffVert_n1 * PrctCellule_P1)+(ResRef_n2 * PrctUniv_P2)) 
									/ (EffVert_n1 + ResRef_n2);

					// Partie calcul commun
					double Fract = (1/EffVert_n1) + (1/ResRef_n2);
					double SqrtZscore = 0.0;

					if (PrctCellule_P1 >= 0 && PrctCellule_P1 <= 1 && PrctUniv_P2 >= 0 && PrctUniv_P2 <= 1)
					{
						// Partie racine carré avec PropHorz1 er PropVert2 compris entre 0 et 1
						SqrtZscore = sqrt(Prop*(1.0-Prop)*Fract);
					}
					else
					{
						// Partie racine carré avec PropHorz1 er PropVert2 compris entre 0 et 100
						SqrtZscore = sqrt(Prop*(100.0-Prop)*Fract);
					}
					
					// Résultat final ZScore
					if (SqrtZscore > 0)
					{
						ValCell = (PrctCellule_P1 - PrctUniv_P2) / SqrtZscore;
					}
				}
			}
		}
		break;
	}

	switch(Style)
	{
		case RSTYLE_RESULT:
		{
			// Tenir compte de la marge d'erreur
			Result =	ValCell;

			// Envoie indication marge d'erreur dépassée
			// if (MargeErreur > MargeErreurMax && MargeErreur < 100000)
			if (MargeErreur > m_pDoc->Get_Seuil_MargeErreur_Appli() && MargeErreur < 100000)
				MargeErreurDepasse = true;
		}
		break;

		// Horizontal result
		case RSTYLE_HORIZONTAL:
		{

			// Modif ALAIN // (Pour Corrélation en horizontale)
			NbCas =		pHorzRes->GetResult(RTYPE_NBCAS);

			// If Global correlation
			if(m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_CORRELATION)
			{
				double ResRef	=	1.0;
				double ResHorz	=	1.0;
				double ResVert	=	1.0;
				double ResNbCas =	0.0;

				GetCellValue(ResRef,	Type, BaseRowItem,	BaseColItem,	Universe,	RSTYLE_RESULT, PageItem, BaseColItem, BaseRowItem, DivZero, ResNbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance);
				GetCellValue(ResHorz,	Type, Ri,			BaseColItem,	Universe,	RSTYLE_RESULT, PageItem, BaseColItem, BaseRowItem, DivZero, ResNbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance);
				GetCellValue(ResVert,	Type, BaseRowItem,	Ci,				Universe,	RSTYLE_RESULT, PageItem, BaseColItem, BaseRowItem, DivZero, ResNbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance);

				double cr =	ValCell * ResRef;
				double hv =	ResHorz * ResVert;
				if(abs(hv) >= 1.0e-2)
					Result	=	cr/hv;
				else
					DivZero =	true;
				
				break;
			}

			if(abs(ValHorz) >= 1.0e-2)
				Result =	ValCell	/	ValHorz; 
			else
				DivZero =	true;
		}
		break;

		// Vertical result
		case RSTYLE_VERTICAL:
		{
			NbCas =		pVertRes->GetResult(RTYPE_NBCAS);

			if(abs(ValVert) >= 1.0e-2)
				Result =	ValCell	/	ValVert;
			else
				DivZero =	true;
		}
		break;

		// Global Result
		case RSTYLE_GLOBAL:
		{
			NbCas =		pGlobRes->GetResult(RTYPE_NBCAS);

			// If Global correlation
			if(m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION|RSTYLE_GBL_PROXIMITY|RSTYLE_GBL_ZSCORE))
			{
				if(Ri != BaseRowItem && Ci != BaseColItem)
				{
					double ResRef	=	1.0;
					double ResHorz	=	1.0;
					double ResVert	=	1.0;
					double ResNbCas =	0.0;

					GetCellValue(ResRef,	Type, BaseRowItem,	BaseColItem,	Universe,	RSTYLE_RESULT, PageItem, BaseColItem, BaseRowItem, DivZero, ResNbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance);
					GetCellValue(ResHorz,	Type, Ri,			BaseColItem,	Universe,	RSTYLE_RESULT, PageItem, BaseColItem, BaseRowItem, DivZero, ResNbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance);
					GetCellValue(ResVert,	Type, BaseRowItem,	Ci,				Universe,	RSTYLE_RESULT, PageItem, BaseColItem, BaseRowItem, DivZero, ResNbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance);

					UINT Valeur = m_pDoc->Get_ResultStyle();

					if(m_pDoc->Get_ResultStyle()& RSTYLE_GBL_CORRELATION)
					{
						double cr =	ValCell * ResRef;
						double hv =	ResHorz * ResVert;
						if(abs(hv) >= 1.0e-2)
							Result	=	cr/hv;
						else
							DivZero =	true;
					}
					// Calcul de l'indice proximité via les valeurs du tableau
					else if (m_pDoc->Get_ResultStyle()& RSTYLE_GBL_PROXIMITY)
					{
						double r	=	ValCell;
						double c	=	ResVert		-	r;
						double t	=	ResHorz		-	r;
						double v	=	ResRef		-	(ResHorz	+	ResVert	-	r);
						double rv	=	r	*	v;
						double ct	=	c	*	t;
						double rvct =	rv+ct;
						if(abs(rvct) >= 1.0e-2)
							Result	=	1.0-((1.0	+	(((rv)	-	(ct))	/	rvct))	/	2.0);
						else
							DivZero =	true;
					}

					// Calcul de l'indice proximité via les valeurs du tableau
					else if (m_pDoc->Get_ResultStyle()& RSTYLE_GBL_ZSCORE)
					{
						// Effectif résultant 
						double EffCellule		=	pCellRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

						// Effectif en horizontal
						double EffHorz			=	pHorzRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

						// Effectif en vertical
						double EffVert_n1		=	pVertRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

						// Effectif Univers de réference
						double ResRef_n2		=	pUnivRes->GetResult(RTYPE_EFFECTIVE_CIBLE);

						// P1
						double PrctCellule_P1	= 0.0;		// P1
						double PrctUniv_P2		= 0.0;		// P2

						if (EffVert_n1 > 0 && ResRef_n2 > 0)
						{
							// Calcul P1 et P2
							PrctCellule_P1	=  EffCellule / EffVert_n1;		
							PrctUniv_P2		=  EffHorz / ResRef_n2;	

							// Calcul P
							if (EffVert_n1 + ResRef_n2 > 0)
							{
								double Prop = ((EffVert_n1 * PrctCellule_P1)+(ResRef_n2 * PrctUniv_P2)) 
												/ (EffVert_n1 + ResRef_n2);

								// Partie calcul commun
								double Fract = (1/EffVert_n1) + (1/ResRef_n2);
								double SqrtZscore = 0.0;

								if (PrctCellule_P1 >= 0 && PrctCellule_P1 <= 1 && PrctUniv_P2 >= 0 && PrctUniv_P2 <= 1)
								{
									// Partie racine carré avec PropHorz1 er PropVert2 compris entre 0 et 1
									SqrtZscore = sqrt(Prop*(1.0-Prop)*Fract);
								}
								else
								{
									// Partie racine carré avec PropHorz1 er PropVert2 compris entre 0 et 100
									SqrtZscore = sqrt(Prop*(100.0-Prop)*Fract);
								}
								
								// Résultat final ZScore
								if (SqrtZscore > 0)
								{
									Result = (PrctCellule_P1 - PrctUniv_P2) / SqrtZscore;

									// Ici on redivise par 100, car tous les résultats de cette partie
									// sont remultipliés par 100
									Result = Result / 100.0;
								}
							}
						}
					}
					break;
				}
			}
			if(abs(ValRef) >= 1.0e-2)
				Result =	ValCell	/	ValRef; 
			else 
				DivZero =	true;
		}
		break;
	}
	return true;
}

// Transposes Grid rows and Cols
bool CCrossTable::Transpose()
{
	int ColIndex	= m_pDoc->Get_ColIndex();
	int RowIndex	= m_pDoc->Get_RowIndex();
	int PageIndex	= m_pDoc->Get_PageIndex();
	int OldRow		= m_pDoc->Get_ColIndex();
	int OldCol		= m_pDoc->Get_RowIndex();

	int Pos[3];

	for(m_UniversParams.MoveFirst(); m_UniversParams.IsValid(); m_UniversParams.MoveNext())
	{

		MAPCELLRESULTS * CellResults = 0;
		MAPCELLRESULTS NewResults;

		CellResults = GetCellResults(m_UniversParams.GetIndex());

		// The results are already done
		if(CellResults->GetCount())
		{
			for(CellResults->MoveFirst(); CellResults->IsValid(); CellResults->MoveNext())
			{
				PTR_ICELLRESULT Result		=	CellResults->GetItem();
				const CCoordRowCol & Key	=	CellResults->GetKey();
				Pos[OldRow]					=	Key.Row();
				Pos[OldCol]					=	Key.Col();
				Pos[PageIndex]			=	Key.Page(); 
				CCoordRowCol NewKey(Pos[PageIndex],Pos[RowIndex],Pos[ColIndex]);

				if(Result->IsCCellItemMoyenResult())
					Result->AsCCellItemMoyenResult()->SetCoord(NewKey); 
				NewResults.Add(NewKey) = Result;
			}
		}
		CellResults->Swap(NewResults);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération style de résultat (Grandeur, H%, V%, Divers)
RESULTSTYLE CCrossTable::GetResultStyle(int Rv,int Cv)
{
	int Pos = Rv *m_Result_Col_Count;
	Pos += Cv;
	m_CellResultStyles.MoveTo(Pos);
	if(m_CellResultStyles.IsValid())
		return m_CellResultStyles.GetItem();
	return RSTYLE_NOSTYLE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupération type de résultat (Effectif, Nb Cas etc....)
RESULTTYPE CCrossTable::GetResultType(int Rv,int Cv)
{
	/* A FAIRE
	int Pos = Rv * m_Result_Col_Count;
	Pos += Cv;
	m_CellResultTypes.MoveTo(Pos);
	if(m_CellResultTypes.IsValid())
		return m_CellResultTypes.GetItem();
	*/
	return RTYPE_EFFECTIVE_000;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Nouvel export avec les lignes et colonnes sélectionnées
// Exports the table to the given file
bool CCrossTable::Export(CFile & exportFile)
{
	CProgressCtrl & Prog = m_pDoc->GetProgressCtrl(); 
	int Universe		= 0;
	CString TString		= _T(" ");
	UINT TableStyle		= m_pDoc->Get_TableStyle();
	bool bTerrainCol	= (TableStyle&TSTYLE_TERRAINCOL);
	bool bGrandeurCol	= (TableStyle&TSTYLE_GRANDEURCOL);
	int BaseColItem		= m_pDoc->Get_BaseColItem();
	int BaseRowItem		= m_pDoc->Get_BaseRowItem();
	const JList<RESULTTYPE> & CalcTypes   = m_pDoc->Get_Grandeurs();
	UINT ResultStyle	= m_pDoc->Get_ResultStyle(); 

	// Pour être sûr d'avoir les dernieres largeurs et hauteurs du grid
	if(m_pGrid->GetRowCount() >= 2)
	{
		// Hauteur des headers lignes
		m_pDoc->SetRowHdrHeight(0,m_pGrid->GetRowHeight(0));
		m_pDoc->SetRowHdrHeight(1,m_pGrid->GetRowHeight(1));
	}
	if(m_pGrid->GetColCount() >= 2)
	{
		// Largeur des headers colonnes
		m_pDoc->SetColHdrWidth(0,m_pGrid->GetColWidth(0));
		m_pDoc->SetColHdrWidth(1,m_pGrid->GetColWidth(1));
	}
	
	m_pDoc->ExportHeader((CFile *) &exportFile);

	// ???? Ici on utilisera cet espace inutilisé pour afficher titre du tableau
	TString.Format(_T("@TABLEAU@\t%s\r\n"), m_pDoc->GetTitle()); 

	exportFile.Write(TString.GetBuffer(), TString.GetLength());
	TString.Format(_T("@VERSION@\t1\r\n")); 
	exportFile.Write(TString.GetBuffer(), TString.GetLength());

	// Positionnement de la barre de progression
	Prog.SetRange32(0,m_NbPages*m_NbCols*m_NbRows);
	Prog.SetStep(1);
	Prog.SetPos(0);

	// Tab Index lignes
	// CRowColArray ArrayRows;
	// ROWCOL nRowCount = m_CrossTable.GetSelectedRows(ArrayRows);

	// Tab Index colonnes
	// CRowColArray ArrayCols;
	// ROWCOL nColCount = m_CrossTable.GetSelectedCols(ArrayCols);

	// Balayage de toutes les pages
	for(int nPage = 0; nPage < m_NbPages; ++nPage)
	{
		int Pi = GetPageItem(nPage);
		CString PString; CString PString2;
		GetPageCellString(PString,PString2,Pi);

		TString.Format(_T("@PAGE@\t%s\t%s\n"),PString,PString2);
		exportFile.Write(TString.GetBuffer(), TString.GetLength());

		// Balayage de toutes les lignes sélectionnées
		for(int nRow = 0; nRow < m_NbRows; ++nRow)
		{
			// Attention aux lignes cachées (bidouille pour cacher les moyennes non actives)
			if (m_pGrid->GetRowHeight(nRow) != 0)
			{
				int Ri,Ru,Rc,Rv;
				CELLTYPE Rt			= GetRowCoordinates(nRow, Ri, Ru, Rc, Rv);
				bool bRtItemCell	= (Rt&CTYPE_ITEMCELL);
				CStringArray  RowString;
				CBitArray		bValidRow;
				CString ColString;

				if(!bGrandeurCol)
					CalcTypes.MoveTo(Rc);
				if(!bTerrainCol)
					Universe = Ru;

				if(bRtItemCell)
				{
					RowString.SetSize(m_Result_Col_Count);
					bValidRow.SetSize(m_Result_Col_Count);
					bValidRow.SetAll(true);
				}
				else
				{
					RowString.SetSize(1);
					bValidRow.SetSize(1);
					bValidRow.SetAll(true);
				}

				int CntFirstCell = RowString.GetSize();
				int vRow = 0;

				// Balayage de toutes les colonnes sélectionnées
				for(int nCol = 0; nCol < m_NbCols; ++nCol, ++vRow)
				{
					Prog.StepIt();
					int Ci,Cu,Cc,Cv;
					CELLTYPE Ct				= GetColCoordinates(nCol, Ci, Cu, Cc, Cv);
					bool bCtItemCell		= (Ct&CTYPE_ITEMCELL);

					if(bGrandeurCol)
						CalcTypes.MoveTo(Cc);
					if(bTerrainCol)
						Universe = Cu;

					ColString	= _T(" ");

					if(!bRtItemCell)
					{
						if(vRow >= m_Result_Col_Count)
						// Reset the virtual row
							vRow = 0;
						// Not a row item
						if(bCtItemCell)
						{
							if(CntFirstCell != 0)
							{
								// Row item cell
								ColString = _T("Indicateur");
								ColString.Append(_T("\t"));
								RowString[0].Append(ColString);
								--CntFirstCell;
								ColString	= _T(" ");
							}
							// Cell item
							if(vRow != 0)
								continue;

							GetColCellString(ColString, Rt, Universe, Ci, Cc);
						}
						else
						{
							if((Rt&CTYPE_HEADER1) != 0)
								ColString = PString;
						}

						ColString.Append(_T("\t"));
						RowString[0].Append(ColString);
					}
					else
					{
						// Row Item Cell
						if(bCtItemCell)
						{
							RESULTSTYLE ResStyle	= GetResultStyle(Rv, Cv);

							// Reset the virtual row
							if(vRow >= RowString.GetSize())
								vRow = 0;

							if(CntFirstCell != 0)
							{
								// Row header cell
								switch(ResStyle)
								{
									case RSTYLE_RESULT:
										// ColString.Format("Grandeur");
										ColString.LoadString(IDS_AN_LABEL_GRANDEUR);
									break;

									case RSTYLE_HORIZONTAL:
										// Spécif Advision (IC = IA / Indice affinité)
										if(m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_CORRELATION)
											// ColString.LoadString(IDS_AN_ABR_INDAFFINITE);
											// Aout 2013
											ColString.LoadString(IDS_AN_ABR_CONCOMITENCE);
										else
											ColString.Format("H%%");
									break;

									case RSTYLE_VERTICAL:
										ColString.Format("V%%");
									break;

									case RSTYLE_GLOBAL:
									{
										if(m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION|RSTYLE_GBL_PROXIMITY|RSTYLE_GBL_ZSCORE))
										{
											// Affichage des abréviations grandeurs utilisées en Global
											if((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION)) == RSTYLE_GBL_CORRELATION)
												ColString.LoadString(IDS_AN_ABR_CORRELATION);
											else if ((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_PROXIMITY)) == RSTYLE_GBL_PROXIMITY)
												ColString.LoadString(IDS_AN_ABR_PROXIMITY);
											else
												ColString.LoadString(IDS_AN_ABR_ZSCORE);
										}
										else
											ColString.LoadString(IDS_AN_ABR_GLOBAL);
									}
									break;
								}
								ColString.Append(_T("\t"));
								RowString[vRow].Append(ColString);
								--CntFirstCell;
								ColString	= _T(" ");
							}
							// Column item cell
							if((ResultStyle&ResStyle) != 0 && GetCellResults(Universe) != 0 && CalcTypes.IsValid())
							{
								// Type de résultat
								RESULTTYPE Type = CalcTypes.GetItem();

								// Croisement ligne/colonne avec un élmt quantitatif > on met le résultat cellule en italique
								int CategorieItemRow = 0;
								int CategorieItemCol = 0;
								int NbModasItemRow   = 0;
								int NbModasItemCol   = 0;
								bool MoyQuantiRow = false;
								bool MoyQuantiCol = false;
								GetRowCellCategorie(CTYPE_HEADER2,Universe,Ri,Rc, CategorieItemRow, NbModasItemRow, MoyQuantiRow);
								GetColCellCategorie(CTYPE_HEADER2,Universe,Ci,Cc, CategorieItemCol, NbModasItemCol, MoyQuantiCol);

								// Croisement Item défini comme quantitatif si item ligne et/ou item colonne quantitatif
								bool ItemQuantitatif = (CategorieItemRow == CQuestion::CATEGORIE::Quantitative_Assim	|| 
														CategorieItemRow == CQuestion::Quantitative_Pure				||
														CategorieItemCol == CQuestion::CATEGORIE::Quantitative_Assim  || 
														CategorieItemCol == CQuestion::Quantitative_Pure) &&
														(Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);

								// en +
								if (MoyQuantiRow || MoyQuantiCol) ItemQuantitatif = true;

								double Result	= 0.0;

								// Flag si division par zéro
								bool DivZero			= false;

								// Flag si marge d'erreur atteinte
								bool MargeErreurDepasse = false;

								// Indication nombre de cas
								double NbCas = 0.0;

								// Marge d'erreur
								double MargeErreur = 0.0;

								// Intervalle confiance
								double IntervalleConfiance = 0.0;
								
								if(GetCellValue(Result, Type, Ri, Ci, Universe, ResStyle, Pi, BaseColItem, BaseRowItem, DivZero, NbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance))
									GetFormattedResult(ColString, Result, ResStyle, Type, Ri, Ci, BaseRowItem, BaseColItem, DivZero, NbCas, MargeErreurDepasse, ItemQuantitatif, MoyQuantiRow, MoyQuantiCol);
							}
							else
								bValidRow.SetAt(vRow,false);

							ColString.Append(_T("\t"));
							RowString[vRow].Append(ColString);
						}
						else
						{
							GetRowCellString(ColString, Ct, Universe, Ri, Rc);
							ColString.Append(_T("\t"));
							for(int x = 0; x < RowString.GetSize(); ++x) 
								RowString[x].Append(ColString);
						}
					}
				}
				for(int x = 0; x < RowString.GetSize(); ++x) 
				{
					if(bValidRow.GetAt(x))
					{
						RowString[x].Append(_T("\n")); 
						exportFile.Write(RowString[x].GetBuffer(), RowString[x].GetLength());
					}
				}
			}	
		} // Fin boucle sur les rows

		TString = _T("\n");
		exportFile.Write(TString.GetBuffer(), TString.GetLength());
	}
	Prog.SetPos(0);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Copy toutes les pages du grid dans clipboard
bool CCrossTable::CopyAllToClipboard()
{
	// Copie de la zone sélectionnée
	if (CopyToClipboard())
		return true;
	else
	{
		// Préparation du clipboard
		CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

		if(Export(clipb))
		{

			VERIFY(m_pGrid->OpenClipboard());
			BOOL bEmpty = EmptyClipboard();
			VERIFY(CloseClipboard());

			HGLOBAL hData = 0;
			HANDLE hResult = 0;
			hData = clipb.Detach();

			// Now, hand over memory block to clipboard
			if (bEmpty && hData)
			{
				VERIFY(m_pGrid->OpenClipboard());
				hResult = SetClipboardData(CF_TEXT, hData);
				VERIFY(CloseClipboard());
				return true;
			}
		}
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Copy sélection cellules du grid dans clipboard
bool CCrossTable::CopyToClipboard()
{
	// Init barre de progression
	CProgressCtrl & Prog = m_pDoc->GetProgressCtrl(); 
	CRowColArray Rows,Cols;

	// Flag aucune sélection préalable
	bool AucuneSelection = false;

	// Nombre de colonnes et lignes sélectionnées pour le clipboard
	ROWCOL NbCols = m_pGrid->GetSelectedCols(Cols,false,false);
	ROWCOL NbRows = m_pGrid->GetSelectedRows(Rows,false,false);

	// Si aucune cellule sélectonnée, on prend tout le tableur
	if(NbRows == 0 && NbCols == 0)
	{
		// Sélection globale du grid
		POSITION area = m_pGrid->GetParam( )->GetRangeList( )->AddTail(new CGXRange);
		m_pGrid->SetSelection(area, 0, 0, m_pGrid->GetRowCount(), m_pGrid->GetColCount());

		// Récupère la séection globale colonnes / lignes
		NbCols = m_pGrid->GetSelectedCols(Cols,false,false);
		NbRows = m_pGrid->GetSelectedRows(Rows,false,false);

		// Positionnement du flag aucune slélection
		AucuneSelection = true;
	}

	// Récupération maximum de colonnes
	ROWCOL MaxCol = m_Col_Hdr_Type.GetCount();
	if(Cols.GetCount()!= 0 && MaxCol > Cols.GetAt(0))
		MaxCol = Cols.GetAt(0);

	CRowColArray NCols;
	for(int x = 0; x < MaxCol; ++x)
		NCols.Add(x);
	NCols.Append(Cols);
	NbCols = NCols.GetCount();

	// Récupération maximum de lignes
	ROWCOL MaxRow = m_Row_Hdr_Type.GetCount();
	if(Rows.GetCount()!= 0 && MaxRow > Rows.GetAt(0))
		MaxRow = Rows.GetAt(0);
	CRowColArray NRows;
	for(int x = 0; x < MaxRow; ++x)
		NRows.Add(x);
	NRows.Append(Rows);
	NbRows = NRows.GetCount();

	// Récupération informations générales
	int Universe		= 0;
	CString TString		= _T(" ");
	UINT TableStyle		= m_pDoc->Get_TableStyle();
	bool bTerrainCol	= (TableStyle&TSTYLE_TERRAINCOL);
	bool bGrandeurCol	= (TableStyle&TSTYLE_GRANDEURCOL);
	int BaseColItem		= m_pDoc->Get_BaseColItem();
	int BaseRowItem		= m_pDoc->Get_BaseRowItem();
	const JList<RESULTTYPE> & CalcTypes = m_pDoc->Get_Grandeurs();
	UINT ResultStyle	= m_pDoc->Get_ResultStyle(); 

	// Tréparation du clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Export dans clipboard l'entete infoemation
	m_pDoc->ExportHeader((CFile *) & clipb);

	// ???? Ici on utilisera cet espace inutilisé pour afficher titre du tableau
	// TString.Format(_T("@TABLEAU@\r\n")); 
	TString.Format(_T("@TABLEAU@\t%s\r\n"), m_pDoc->GetTitle()); 
	clipb.Write(TString.GetBuffer(),TString.GetLength());
	TString.Format(_T("@VERSION@\t1\r\n")); 
	clipb.Write(TString.GetBuffer(),TString.GetLength());

	int Pi = GetPageItem(m_pDoc->GetCurPage());
	CString PString; CString PString2;
	GetPageCellString(PString,PString2,Pi);
	TString.Format(_T("@PAGE@\t%s\t%s\n"),PString,PString2);
	clipb.Write(TString.GetBuffer(),TString.GetLength());

	Prog.SetRange32(0,NbCols*NbRows);
	Prog.SetStep(1);
	Prog.SetPos(0);

	// Boucle sur toutes les lignes du tableau croisé
	for(int nRow = 0; nRow < NbRows; ++nRow)
	{
		// Attention aux lignes cachées (bidouille pour cacher les moyennes non actives)
		if (m_pGrid->GetRowHeight(nRow) != 0)
		{
			int Ri,Ru,Rc,Rv;
			CELLTYPE Rt			= GetRowCoordinates(NRows.GetAt(nRow), Ri, Ru, Rc, Rv);
			bool bRtItemCell	= (Rt&CTYPE_ITEMCELL);
			CStringArray  RowString;
			CBitArray		bValidRow;
			CString ColString;

			if(!bGrandeurCol)
				CalcTypes.MoveTo(Rc);
			if(!bTerrainCol)
				Universe = Ru;

			if(bRtItemCell)
			{
				RowString.SetSize(m_Result_Col_Count);
				bValidRow.SetSize(m_Result_Col_Count);
				bValidRow.SetAll(true);
			}
			else
			{
				RowString.SetSize(1);
				bValidRow.SetSize(1);
				bValidRow.SetAll(true);
			}

			int CntFirstCell = RowString.GetSize();
			int vRow = 0;


			for(int nCol = 0; nCol < NbCols; ++nCol, ++vRow)
			{
				Prog.StepIt();
				int Ci,Cu,Cc,Cv;
				CELLTYPE Ct				= GetColCoordinates(NCols.GetAt(nCol), Ci, Cu, Cc, Cv);
				bool bCtItemCell		= (Ct&CTYPE_ITEMCELL);

				if(bGrandeurCol)
					CalcTypes.MoveTo(Cc);
				if(bTerrainCol)
					Universe = Cu;


				ColString	= _T(" ");

				if(!bRtItemCell)
				{
					if(vRow >= m_Result_Col_Count)
					// Reset the virtual row
						vRow = 0;
					// Not a row item
					if(bCtItemCell)
					{
						if(CntFirstCell != 0)
						{
							// Row item cell
							ColString = _T("Indicateur");
							ColString.Append(_T("\t"));
							RowString[0].Append(ColString);
							--CntFirstCell;
							ColString	= _T(" ");
						}
						// Cell item
						// if(vRow == 0 || nCol == (NbCols-1))
						if(vRow == 0)
						{
							GetColCellString(ColString, Rt, Universe, Ci, Cc);
							ColString.Append(_T("\t"));
							RowString[0].Append(ColString);
						}
					}
					else
					{
						if((Rt&CTYPE_HEADER1) != 0)
							ColString = PString;
						ColString.Append(_T("\t"));
						RowString[0].Append(ColString);
					}
				}
				else
				{
					// Row Item Cell
					if(bCtItemCell)
					{
						RESULTSTYLE ResStyle	= GetResultStyle(Rv, Cv);

						// Reset the virtual row
						if(vRow >= RowString.GetSize())
							vRow = 0;

						if(CntFirstCell != 0)
						{
							// Row header cell
							switch(ResStyle)
							{
								// Résultat général (cellule en haut à gauche)  x-
								//                                              --
								case RSTYLE_RESULT:
									ColString.Format("Grandeur");
								break;

								// Résultat horizontal (cellule en haut à droite)  -x
								//                                                 --
								case RSTYLE_HORIZONTAL:
									if(m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_CORRELATION)
									{
										// Spécif Advision (IC = IA / Indice affinité)
										// ColString.LoadString(IDS_AN_ABR_INDAFFINITE);
										// Aout 2013
										ColString.LoadString(IDS_AN_ABR_CONCOMITENCE);
									}
									else
										ColString.Format("H%%");
								break;

								// Résultat général (cellule en haut à droite)  --
								//                                              x-
								case RSTYLE_VERTICAL:
									ColString.Format("V%%");
								break;

								case RSTYLE_GLOBAL:
								{
									if(m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION|RSTYLE_GBL_PROXIMITY|RSTYLE_GBL_ZSCORE))
									{
										// Affichage des abréviations grandeurs utilisées en Global
										if((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION)) == RSTYLE_GBL_CORRELATION)
											ColString.LoadString(IDS_AN_ABR_CORRELATION);
										else if ((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_PROXIMITY)) == RSTYLE_GBL_PROXIMITY)
											ColString.LoadString(IDS_AN_ABR_PROXIMITY);
										else if ((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_ZSCORE)) == RSTYLE_GBL_ZSCORE)
											ColString.LoadString(IDS_AN_ABR_ZSCORE);
									}
									else
										ColString.LoadString(IDS_AN_ABR_GLOBAL);
								}
								break;
							}
							ColString.Append(_T("\t"));
							RowString[vRow].Append(ColString);
							--CntFirstCell;
							ColString	= _T(" ");
						}
						// Column item cell
						if((ResultStyle&ResStyle) != 0 && GetCellResults(Universe) != 0 && CalcTypes.IsValid())
						{
							// Type de résultat
							RESULTTYPE Type = CalcTypes.GetItem();

							// Croisement ligne/colonne avec un élmt quantitatif > on met le résultat cellule en italique
							int CategorieItemRow = 0;
							int CategorieItemCol = 0;
							int NbModasItemRow   = 0;
							int NbModasItemCol   = 0;
							bool MoyQuantiRow = false;
							bool MoyQuantiCol = false;
							GetRowCellCategorie(CTYPE_HEADER2,Universe,Ri,Rc, CategorieItemRow, NbModasItemRow, MoyQuantiRow);
							GetColCellCategorie(CTYPE_HEADER2,Universe,Ci,Cc, CategorieItemCol, NbModasItemCol, MoyQuantiCol);

							// Croisement Item défini comme quantitatif si item ligne et/ou item colonne quantitatif
							bool ItemQuantitatif = (CategorieItemRow == CQuestion::CATEGORIE::Quantitative_Assim	|| 
													CategorieItemRow == CQuestion::Quantitative_Pure				||
													CategorieItemCol == CQuestion::CATEGORIE::Quantitative_Assim  || 
													CategorieItemCol == CQuestion::Quantitative_Pure) &&
													(NbModasItemRow<=1 && NbModasItemCol <= 1) &&
													(Type == RTYPE_QUANTITY_000 || Type == RTYPE_AVERAGEQUANTUM || Type == RTYPE_AVERAGEQUANTUM_P00);

							double Result	= 0.0;

							// Flag si division par zéro
							bool DivZero			= false;

							// Flag si marge d'erreur atteinte
							bool MargeErreurDepasse = false;

							// Indication nombre de cas
							double NbCas = 0.0;

							// Marge d'erreur
							double MargeErreur = 0.0;

							// Intervalle de confiance
							double IntervalleConfiance = 0.0;
							
							if(GetCellValue(Result, Type, Ri, Ci, Universe, ResStyle, Pi, BaseColItem, BaseRowItem, DivZero, NbCas, MargeErreurDepasse, MargeErreur,IntervalleConfiance))
								GetFormattedResult(ColString, Result, ResStyle, Type, Ri, Ci, BaseRowItem, BaseColItem, DivZero, NbCas, MargeErreurDepasse, ItemQuantitatif, MoyQuantiRow, MoyQuantiCol);


							// Définition de la clef
							CKeyRowCol KeyRowCol;
							KeyRowCol.m_NoRow = nRow; // Ri;
							KeyRowCol.m_NoCol = nCol; // Ci; 


							// POUR EXPORT FRED tenir compte couleur surlignage A FAIRE
							m_pDoc->m_MapCellTotSurlign.MoveTo(KeyRowCol);
							if (m_pDoc->m_MapCellTotSurlign.IsValid() && ColString != "***")
							{
								COLORREF &ColorSurlign = m_pDoc->m_MapCellTotSurlign.GetItem();
								long ValColor = (long)ColorSurlign;
								CString TxtCol = ColString; 
								ColString.Format("%s /%d", TxtCol, ValColor);
							}
						}
						else
							bValidRow.SetAt(vRow,false);

						ColString.Append(_T("\t"));
						RowString[vRow].Append(ColString);
					}
					else
					{
						GetRowCellString(ColString, Ct, Universe, Ri, Rc);
						ColString.Append(_T("\t"));
						for(int x = 0; x < RowString.GetSize(); ++x) 
							RowString[x].Append(ColString);
					}
				}
			}
			for(int x = 0; x < RowString.GetSize(); ++x) 
			{
				if(bValidRow.GetAt(x))
				{
					RowString[x].Append(_T("\n")); 
					clipb.Write(RowString[x].GetBuffer(),RowString[x].GetLength());
				}
			}

		}
	}

	Prog.SetPos(0);
	TString = _T("\n");
	clipb.Write(TString.GetBuffer(),TString.GetLength());

	VERIFY(m_pGrid->OpenClipboard());
	BOOL bEmpty = EmptyClipboard();
	VERIFY(CloseClipboard());

	HGLOBAL hData = 0;
	HANDLE hResult = 0;
	hData = clipb.Detach();

	// Now, hand over memory block to clipboard
	if (bEmpty && hData)
	{
		VERIFY(m_pGrid->OpenClipboard());
		hResult = SetClipboardData(CF_TEXT, hData);
		VERIFY(CloseClipboard());
	}

	// Déselection globale si aucune sélection préalable
	if (AucuneSelection)
		m_pGrid->SetSelection(NULL, 0, 0, 0, 0);


	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Index premiere ligne tableau affilié avec élmt item
JUnt32 CCrossTable::FirstIndexItemRow()
{	
	JUnt32 PosItemRow = 0;
	for(m_ItemRowIndex.MoveFirst(); m_ItemRowIndex.IsValid(); m_ItemRowIndex.MoveNext())
	{
		int Idx = m_ItemRowIndex.GetItem();
		m_Item_Row_Types.MoveTo(Idx);
		if(m_Item_Row_Types.IsValid() && m_Item_Row_Types.GetItem() == ITYPE_ITEM)
		{
			return PosItemRow;
		}
		PosItemRow++;
	}

	return PosItemRow;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Index premiere colonne tableau affilié avec élmt item
JUnt32 CCrossTable::FirstIndexItemCol()
{	
	JUnt32 PosItemCol = 0;
	for(m_ItemColIndex.MoveFirst(); m_ItemColIndex.IsValid(); m_ItemColIndex.MoveNext())
	{
		int Idx = m_ItemColIndex.GetItem();
		m_Item_Col_Types.MoveTo(Idx);
		if(m_Item_Col_Types.IsValid() && m_Item_Col_Types.GetItem() == ITYPE_ITEM)
		{
			return PosItemCol;
		}
		PosItemCol++;
	}

	return PosItemCol;
}
