// BaseCinema.h: interface for the CBaseCinema class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASECINEMA_H__EBAFD90B_CB9D_4155_9DEC_D275C05DC3F4__INCLUDED_)
#define AFX_BASECINEMA_H__EBAFD90B_CB9D_4155_9DEC_D275C05DC3F4__INCLUDED_

#include "TableComplexe.h"
#include "TableBAC.h"
#include "TableProduit.h"
#include "TableFormat.h"
#include "TableTempo.h"	
#include "TableNbEntreeComplexe.h"
#include "TableRegie.h"
#include "TableGroupeBAC.h"
#include "TableGroupeComplexe.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseCinema  
{
public:

	CBaseCinema();
	virtual ~CBaseCinema();

	LPCSTR FindLibelleFromCode(CString code);
	LPCSTR FindCodeFromLibelle(CString Libelle);
	bool IsCodeDefined(LPCSTR Code);
	void SetTableProduit(CTableProduit tableproduit);
	void SetTableFormat(CTableFormat tableformat);
	void SetTableTempo(CTableTempo tabletempo);
	void SetTableBac(CTableBAC tablebac);
	void SetTableComplexe(CTableComplexe tabcomplexe);
	void SetTableNbEntree(CTableNbEntreeComplexe tablenbentree);

	CTableComplexe * GetTableComplexe();
	CTableTempo GetNewInstanceTableTempo();
	CTableFormat GetNewInstanceTableFormat();
	CTableBAC GetNewInstanceTableBac();
	CTableProduit GetNewInstanceTableProduit();
	CTableComplexe GetNewInstanceTableComplexe();
	CTableNbEntreeComplexe GetNewInstanceTableNbEntree();

	CTableGroupeBAC GetNewInstanceTableGroupeBac();
	CTableGroupeComplexe GetNewInstanceTableGroupeComplexe();

	bool SaveTable();
	bool LoadTable(int type);

	CTableProduit * GetTableProduit();
	CTableTempo * GetTableTempo();
	CTableBAC * GetTableBac();
	CTableRegie * GetTableRegie();
	CTableFormat * GetTableFormat();
	CTableNbEntreeComplexe * GetTableNbEntree();

	// Récup table des groupes de complexe
	CTableGroupeComplexe * GetTableGroupeComplexe();

	// Récup table des groupes de bacs
	CTableGroupeBAC * GetTableGroupeBac();

	// Création des bacs régie
	bool CreateBacRegie();

private: // opérations
	bool CalculPartDeMarche();
	bool LinkTable();
	bool LinkProduit();

private: // Atributs
	CFile LockData;
	CTableTempo				m_TableTempo;
	CTableFormat			m_TableFormat;
	CTableProduit			m_TableProduit;
	CTableComplexe			m_TableComplexe;
	CTableBAC				m_TableBac;
	CTableRegie				m_TableRegie;
	CTableNbEntreeComplexe	m_TableNbEntree;

	// Instance Tables pour les groupes de Bacs et de Complexes
	CTableGroupeBAC			m_TableGroupeBac;
	CTableGroupeComplexe	m_TableGroupeComplexe;
};

#endif // !defined(AFX_BASECINEMA_H__EBAFD90B_CB9D_4155_9DEC_D275C05DC3F4__INCLUDED_)
