#include "StdAfx.h"
#include ".\xlprintthread.h"


CXLPrintThread::CXLPrintThread(void)
: m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR)
{
	m_pdlgProgress = NULL;
	m_bExcelInit = false;
}

CXLPrintThread::~CXLPrintThread(void)
{
}

bool CXLPrintThread::ExecuteMacro()
{
	CoInitialize(NULL);
	
	bool bRet = FALSE;

	if (m_params.typeAction == XLParams::OPEN)
		bRet = InitExcel(TRUE);
	else
		bRet = InitExcel(FALSE);

	if(bRet)
		bRet = RunMacro(m_params.macroPath, m_params.methodeName);

	if(bRet)
	{
		switch(m_params.typeAction)
		{
			case XLParams::SAVE :		bRet = Save();
				break;
			case XLParams::PREVIEW :	bRet = PrintPreview();
				break;
			case XLParams::PRINT :		bRet = DirectPrint();
				break;
			case XLParams::OPEN :		bRet = Open();
				break;
		}
	}

	// si on a bloqué la mise à jour d'Excel, on debloque
	if (m_app.get_ScreenUpdating() == FALSE)
		m_app.put_ScreenUpdating(TRUE);
	
	// on ferme excel pour tous modes d'execution de macros autres que 'Ouvrir dans Excel...'!!
	if (m_params.typeAction != XLParams::OPEN)
		bRet =  ExitExcel();

	return bRet;
}

bool CXLPrintThread::Save()
{
	TRY
	{
		CWorksheets sheets = m_book.get_Worksheets();
		CWorksheet sheet = sheets.get_Item(COleVariant((short)1));
		sheet.Select(m_covOptional);
		COleVariant variantStr(m_params.filePath);
		m_book.SaveAs(variantStr, m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional,0,m_covOptional,m_covOptional,m_covOptional,m_covOptional);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Impossible de sauver le fichier.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Impossible de sauver le fichier.");
#endif
		return false;
	}
	END_CATCH
	return true;
}
bool CXLPrintThread::PrintPreview()
{
	TRY
	{
		m_app.put_Visible (TRUE);
		CWorksheets sheets;
		sheets = m_book.get_Worksheets();
		CWorksheet sheet = sheets.get_Item(COleVariant((short)1));
		sheet.Select(m_covOptional);
		sheets.PrintPreview(m_covOptional);
		m_app.put_Visible (FALSE);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Impossible de lancer l'aperçu avant impression.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Impossible de lancer l'aperçu avant impression.");
#endif
		return false;
	}
	END_CATCH
	return true;
}
bool CXLPrintThread::DirectPrint()
{
	TRY
	{
		CWorksheets sheets;
		sheets = m_book.get_Worksheets();
		CWorksheet sheet = sheets.get_Item(COleVariant((short)1));
		sheet.Select(m_covOptional);
		//on test la version d'excel
		CString version = m_app.get_Version();
		int curpos = 0;
		int numVersionMajor = atoi(version.Tokenize(".", curpos));
		if (numVersionMajor <= 8)
			//On utilise donc la version précédente compatible avec Excel 97
			m_book._PrintOut(m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional);
		else
			m_book.PrintOut(m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional,m_covOptional);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		//e->ReportError();
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Impossible de lancer l'impression.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Impossible de lancer l'impression.");
#endif
		return false;
	}
	END_CATCH
		return true;
}

bool CXLPrintThread::Open()
{
	TRY
	{
		CWorksheets sheets = m_book.get_Worksheets();
		CWorksheet sheet = sheets.get_Item(COleVariant((short)1));
		sheet.Select(m_covOptional);
		COleVariant variantStr(m_params.filePath);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Impossible d'ouvrir Excel.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Impossible d'ouvrir Excel.");
#endif
		return false;
	}
	END_CATCH
		return true;
}


bool CXLPrintThread::ExitExcel()
{
	if(m_bExcelInit)
	{
		CloseWorkbook ();

		TRY
		{
			m_app.put_ScreenUpdating(TRUE);
			m_app.Quit();
			m_app.ReleaseDispatch();

			m_bExcelInit = false;
		}
		//Clean up if something went wrong.
		CATCH(CException, e)
		{
#ifdef DLG_ERROR
			m_pdlgProgress->Error("Impossible de fermer Excel.");
#endif
#ifdef DLG_AFX
			AfxMessageBox("Impossible de fermer Excel.");
#endif
			return false;
		}
		END_CATCH
	}
	return true;
}




bool CXLPrintThread::InitExcel(BOOL bVisible)
{
	// start excel process
	if(!m_bExcelInit)
	{
		TRY
		{
			COleException pError;
			if(!m_app.CreateDispatch("Excel.Application", &pError))
			{
				m_pdlgProgress->Error("Impossible de lancer Excel.");
				//AfxMessageBox("Impossible de lancer Excel.");
				return false;
			}
			m_app.put_DisplayAlerts (VARIANT_FALSE);
			m_app.put_UserControl(bVisible);
			m_app.put_Visible (bVisible);
			m_app.put_ScreenUpdating(FALSE);
			m_bExcelInit = true;
		}
		//Clean up if something went wrong.
		CATCH(CException, e)
		{
#ifdef DLG_ERROR
			m_pdlgProgress->Error("Impossible de lancer Excel.");
#endif
#ifdef DLG_AFX
			AfxMessageBox("Impossible de lancer Excel.");
#endif
			return false;
		}
		END_CATCH
	}
	return m_bExcelInit;
}

// Close Excel workbook
bool CXLPrintThread::CloseWorkbook()
{
	TRY
	{
		// close already-opened workbook
		m_book.ReleaseDispatch ();
		m_bookMacro.ReleaseDispatch ();
		m_WorkBooks.Close();
		m_WorkBooks.ReleaseDispatch ();
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Impossible de fermer les documents Excel.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Impossible de fermer les documents Excel.");
#endif
		return false;
	}
	END_CATCH
	return true;
}
bool CXLPrintThread::RunMacro(CString macroPath, CString methodeName)
{
	//CloseWorkbook ();

	CWorksheets sheets;
	CWorksheet sheet;

	// open the specified workbook
	TRY
	{
		// Get Workbooks collection.
		LPDISPATCH lpDisp;
		lpDisp = m_app.get_Workbooks();  // Get an IDispatch pointer
		ASSERT(lpDisp);
		m_WorkBooks.AttachDispatch( lpDisp );  // Attach the IDispatch pointer to the books object.

		
		//Set CWorkbook to use lpDisp, the IDispatch* of the actual workbook.
		lpDisp = m_WorkBooks.Open(macroPath,
						m_covOptional, m_covOptional, m_covOptional, m_covOptional,
						m_covOptional, m_covOptional, m_covOptional, m_covOptional,
						m_covOptional, m_covOptional, m_covOptional, m_covOptional
						);
		ASSERT(lpDisp);
		m_bookMacro.AttachDispatch(lpDisp);

		//Set CWorkbook to use lpDisp, the IDispatch* of the actual workbook.
		lpDisp = m_WorkBooks.Add(m_covOptional);
		ASSERT(lpDisp);
		m_book.AttachDispatch(lpDisp);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Erreur lors de l'ouverture de la macro.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Erreur lors de l'ouverture de la macro.");
#endif
		return false;
	}
	END_CATCH


	TRY
	{
		// show the workbbok
		// get the selected worksheet
		sheets = m_book.get_Worksheets();
		sheet = sheets.get_Item(COleVariant((short)1));
		sheet.Select(m_covOptional);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Erreur lors de l'ouverture de la macro.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Erreur lors de l'ouverture de la macro.");
#endif
		return false;
	}
	END_CATCH


	TRY
	{
		//sheet.
		sheet.Paste(m_covOptional, m_covOptional);
		
		CString macro;
		macro = m_bookMacro.get_Name();
		macro.AppendFormat("!%s()", methodeName);
		
		m_app.ExecuteExcel4Macro(macro);		
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
#ifdef DLG_ERROR
		m_pdlgProgress->Error("Erreur lors de l'execution de la macro.");
#endif
#ifdef DLG_AFX
		AfxMessageBox("Erreur lors de l'execution de la macro.");
#endif
		return false;
	}
	END_CATCH

	return true;
}	


