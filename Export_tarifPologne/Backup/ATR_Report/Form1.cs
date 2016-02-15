﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using FileHelpers;
using FileHelpers.DataLink;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using System.Reflection;
using Excel = Microsoft.Office.Interop.Excel;
using System.Globalization;
using System.Threading;









namespace ATR_Report
{

    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        delegate void SetStringDelegate(string parameter);
        void SetStatus(string status)
        {
            if (!InvokeRequired)

                label6.Text = status;

            else

                Invoke(new SetStringDelegate(SetStatus), new object[] { status });
        }
        void SetStatus0(string status0)
        {
            if (!InvokeRequired)

                label8.Text = status0;

            else

                Invoke(new SetStringDelegate(SetStatus0), new object[] { status0 });
        }

        void SetStatus1(string dates)
        {
            if (!InvokeRequired)

                label7.Text = dates;

            else
                Invoke(new SetStringDelegate(SetStatus1), new object[] { dates });
        }

        void SetStatus2(string result)
        {
            if (!InvokeRequired)
                label5.Text = result;
            else
                Invoke(new SetStringDelegate(SetStatus2), new object[] { result });
        }

        private void AppendToFile(string txt, string filepath)
        {
            StreamWriter swFromFileTrueUTF8 = new StreamWriter(filepath, true, System.Text.Encoding.Default);
            swFromFileTrueUTF8.WriteLine(txt);
            swFromFileTrueUTF8.Flush();
            swFromFileTrueUTF8.Close();
        }
        private void CreateExcel(string filepath0, int tab, string sheetname, string Data, string StaName, int Blocqty)
        {
            RegistryKey masterKey = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
            string pathexcel = masterKey.GetValue("Path") + "/cenniki/cennik.xls";
            masterKey.Close();
            sheetname = sheetname + tab.ToString();

            if (tab == 1)
            {
                File.Delete(pathexcel);
                OpenWorkbook(pathexcel, sheetname);
                writeRec(filepath0, pathexcel, sheetname, Blocqty, Data, StaName);
            }
            else
            {

                AddWorksheet(pathexcel, sheetname);
                writeRec(filepath0, pathexcel, sheetname, Blocqty, Data, StaName);
            }
        }
        private void writeRec(string filepath0, string pathexcel, string sheetname, int recqty, string CenDate, string StationName)
        {
            using (new ExcelUILanguageHelper())
            {
                Excel.Application oXL;
                Excel._Workbook oWB;
                Excel._Worksheet oSheet;

                oXL = new Excel.Application();
                oXL.Visible = false;
                oXL.DisplayAlerts = false;
                oWB = (Excel._Workbook)oXL.Workbooks.Open(pathexcel, Missing.Value, false, Missing.Value, Missing.Value, Missing.Value, true, Missing.Value, Missing.Value, true, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);


                oSheet = (Excel._Worksheet)oWB.Worksheets[sheetname];
                // Create an array for the headers and add it to cells A1:B1.
                string shortsheetname = StationName.Substring(0, StationName.Length);
                object[] objHeaders = { shortsheetname, CenDate };
                oSheet.get_Range("A1", "B1").Value2 = objHeaders;

                //Format A2:H2 as bold, vertical alignment = center.
                oSheet.get_Range("A1", "B1").Font.Bold = true;
                oSheet.get_Range("A1", "B1").VerticalAlignment = Excel.XlVAlign.xlVAlignCenter;

                // Create an array for the headers and add it to cells A2:H2.
                object[] objHeaders0 = { "Period", "Pon", "Wt", "Śr", "Cz", "Pt", "So", "Nd" };
                oSheet.get_Range("A2", "H2").Value2 = objHeaders0;

                //Format A2:H2 as bold, vertical alignment = center.
                oSheet.get_Range("A2", "H2").Font.Bold = true;
                oSheet.get_Range("A2", "H2").VerticalAlignment = Excel.XlVAlign.xlVAlignCenter;

                FileHelperEngine Exceng = new FileHelperEngine(typeof(CennikBloc));

                CennikBloc[] res = (CennikBloc[])Exceng.ReadFile(filepath0);
                // Create an array to multiple values at once.

                int EndTimeInt = 0;
                int StartTimeInt = 0;
                int rownumber = 2;
                int rownumberf = 2;

                foreach (CennikBloc cen in res)
                {



                    //Correct time format 0-24
                    if (Convert.ToInt32(cen.heuredebut.Substring(0, 2)) >= 24)
                    {
                        StartTimeInt = Convert.ToInt32(cen.heuredebut.Substring(0, 2)) - 24;
                        cen.heuredebut = StartTimeInt.ToString() + cen.heuredebut.Substring(2, 3);

                    }

                    if (Convert.ToInt32(cen.heurefin.Substring(0, 2)) >= 24)
                    {
                        EndTimeInt = Convert.ToInt32(cen.heurefin.Substring(0, 2)) - 24;
                        cen.heurefin = EndTimeInt.ToString() + cen.heurefin.Substring(2, 3);

                    }


                    String datetimedebut = cen.heuredebut;
                    String datetimefin = cen.heurefin;

                    System.DateTime time0 = Convert.ToDateTime(datetimedebut);
                    System.DateTime time1 = Convert.ToDateTime(datetimefin);

                    if (time1 < time0)
                    {
                        System.DateTime time0bis = Convert.ToDateTime("00:00");
                        System.DateTime time1bis = Convert.ToDateTime("23:59");

                        while (time0 < time1bis.AddMinutes(-13))
                        {
                            rownumber = rownumber + 1;
                            string[,] saPricebis = new string[1, 8];

                            System.DateTime time2bis = time0.AddMinutes(14);

                            string periodbis = time0.ToString("HH:mm") + "-" + time2bis.ToString("HH:mm");

                            saPricebis[0, 0] = periodbis;

                            //Create Array
                            int daystartbis = cen.jourdebut;
                            int dayendbis = cen.jourfin;

                            for (int d = daystartbis; d < dayendbis + 1; d++)
                            {

                                string CorrPricebis = cen.prix.Replace(".", ",");
                                if (CorrPricebis.Trim() == "-100,00")
                                {
                                    saPricebis[0, d] = " ";
                                }
                                else
                                {                                    
                                    saPricebis[0, d] = CorrPricebis;
                                }
                            }

                            //Fill A2:B(recqty+1) with an array of values .
                            oSheet.get_Range("A" + rownumber.ToString(), "H" + rownumber.ToString()).Value2 = saPricebis;

                            time0 = time0.AddMinutes(15);

                        }
                        while (time0bis < time1.AddMinutes(-13))
                        {
                            rownumber = rownumber + 1;
                            string[,] saPricebiss = new string[1, 8];

                            System.DateTime time2biss = time0bis.AddMinutes(14);

                            string periodbiss = time0bis.ToString("HH:mm") + "-" + time2biss.ToString("HH:mm");

                            saPricebiss[0, 0] = periodbiss;

                            //Create Array
                            int daystartbiss = cen.jourdebut;
                            int dayendbiss = cen.jourfin;
                            for (int d = daystartbiss; d < dayendbiss + 1; d++)
                            {
                                string CorrPricebiss = cen.prix.Replace(".", ",");
                                saPricebiss[0, d] = CorrPricebiss;
                                if (CorrPricebiss.Trim() == "-100,00")
                                {
                                    saPricebiss[0, d] = " ";
                                }
                                else
                                {
                                    saPricebiss[0, d] = CorrPricebiss;
                                }

                            }

                            //Fill A2:B(recqty+1) with an array of values .
                            oSheet.get_Range("A" + rownumber.ToString(), "H" + rownumber.ToString()).Value2 = saPricebiss;

                            time0bis = time0bis.AddMinutes(15);

                        }

                    }
                    else
                    {

                        while (time0 < time1.AddMinutes(-13))
                        {
                            rownumber = rownumber + 1;
                            string[,] saPrice = new string[1, 8];

                            System.DateTime time2 = time0.AddMinutes(14);

                            string period = time0.ToString("HH:mm") + "-" + time2.ToString("HH:mm");

                            saPrice[0, 0] = period;

                            //Create Array
                            int daystart = cen.jourdebut;
                            int dayend = cen.jourfin;
                            for (int d = daystart; d < dayend + 1; d++)
                            {
                                string CorrPrice = cen.prix.Replace(".", ",");
                                if (CorrPrice == "-100,00")
                                {
                                    saPrice[0, d] = " ";
                                }
                                else
                                {
                                    saPrice[0, d] = CorrPrice;
                                }
                            }

                            //Fill A2:B(recqty+1) with an array of values .
                            oSheet.get_Range("A" + rownumber.ToString(), "H" + rownumber.ToString()).Value2 = saPrice;

                            time0 = time0.AddMinutes(15);

                        }
                    }
                }
                oWB.SaveAs(pathexcel, Excel.XlFileFormat.xlWorkbookNormal, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Excel.XlSaveAsAccessMode.xlExclusive, Excel.XlSaveConflictResolution.xlLocalSessionChanges, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
                oWB.Close(true, Missing.Value, Missing.Value);

                string sheetPon = sheetname + "Pon";
                string sheetWto = sheetname + "Wto";
                string sheetSro = sheetname + "Sro";
                string sheetCzw = sheetname + "Czw";
                string sheetPia = sheetname + "Pia";
                string sheetSob = sheetname + "Sob";
                string sheetNied = sheetname + "Nied";

                AddWorksheet(pathexcel, sheetPon);
                AddWorksheet(pathexcel, sheetWto);
                AddWorksheet(pathexcel, sheetSro);
                AddWorksheet(pathexcel, sheetCzw);
                AddWorksheet(pathexcel, sheetPia);
                AddWorksheet(pathexcel, sheetSob);
                AddWorksheet(pathexcel, sheetNied);

                Excel._Workbook oWBPon;
                Excel._Worksheet oSheetOr;
                Excel._Worksheet oSheetPon;
                Excel._Worksheet oSheetWto;
                Excel._Worksheet oSheetSro;
                Excel._Worksheet oSheetCzw;
                Excel._Worksheet oSheetPia;
                Excel._Worksheet oSheetSob;
                Excel._Worksheet oSheetNied;

                Excel.Range oRngPonCol;
                // oXL.Visible = true;
                oWBPon = (Excel._Workbook)oXL.Workbooks.Open(pathexcel, Missing.Value, false, Missing.Value, Missing.Value, Missing.Value, true, Missing.Value, Missing.Value, true, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
                oSheetOr = (Excel._Worksheet)oWBPon.Worksheets[sheetname];
                oSheetPon = (Excel._Worksheet)oWBPon.Worksheets[sheetPon];
                oSheetWto = (Excel._Worksheet)oWBPon.Worksheets[sheetWto];
                oSheetSro = (Excel._Worksheet)oWBPon.Worksheets[sheetSro];
                oSheetCzw = (Excel._Worksheet)oWBPon.Worksheets[sheetCzw];
                oSheetPia = (Excel._Worksheet)oWBPon.Worksheets[sheetPia];
                oSheetSob = (Excel._Worksheet)oWBPon.Worksheets[sheetSob];
                oSheetNied = (Excel._Worksheet)oWBPon.Worksheets[sheetNied];

                oRngPonCol = oSheetOr.get_Range("A:A", "B:B");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetPon.get_Range("A1", "B1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);

                //MessageBox.Show(oSheetPon.UsedRange.Rows.Count.ToString());

                for (int r = 2; r < oSheetPon.UsedRange.Rows.Count; r++)
                {
                    while (oSheetPon.get_Range("B" + r.ToString(), "B" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetPon.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }

                oRngPonCol = oSheetOr.get_Range("A:A", "C:C");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetWto.get_Range("A1", "C1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);


                for (int r = 2; r < oSheetWto.UsedRange.Rows.Count; r++)
                {
                    while (oSheetWto.get_Range("C" + r.ToString(), "C" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetWto.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }

                oRngPonCol = oSheetOr.get_Range("A:A", "D:D");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetSro.get_Range("A1", "D1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);


                for (int r = 2; r < oSheetSro.UsedRange.Rows.Count; r++)
                {
                    while (oSheetSro.get_Range("D" + r.ToString(), "D" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetSro.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }
                oRngPonCol = oSheetOr.get_Range("A:A", "E:E");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetCzw.get_Range("A1", "E1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);


                for (int r = 2; r < oSheetCzw.UsedRange.Rows.Count; r++)
                {
                    while (oSheetCzw.get_Range("E" + r.ToString(), "E" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetCzw.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }
                oRngPonCol = oSheetOr.get_Range("A:A", "F:F");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetPia.get_Range("A1", "F1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);


                for (int r = 2; r < oSheetPia.UsedRange.Rows.Count; r++)
                {
                    while (oSheetPia.get_Range("F" + r.ToString(), "F" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetPia.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }
                oRngPonCol = oSheetOr.get_Range("A:A", "G:G");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetSob.get_Range("A1", "G1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);


                for (int r = 2; r < oSheetSob.UsedRange.Rows.Count; r++)
                {
                    while (oSheetSob.get_Range("G" + r.ToString(), "G" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetSob.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }
                oRngPonCol = oSheetOr.get_Range("A:A", "H:H");
                oRngPonCol.EntireColumn.Copy(Missing.Value);

                oSheetNied.get_Range("A1", "H1").PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteAll, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);


                for (int r = 2; r < oSheetNied.UsedRange.Rows.Count; r++)
                {
                    while (oSheetNied.get_Range("H" + r.ToString(), "H" + r.ToString()).Value2 == null)
                    {
                        int rowcount = oSheetPon.UsedRange.Rows.Count;
                        if (r < rowcount)
                        {
                            oSheetNied.get_Range("A" + r.ToString(), "A" + r.ToString()).EntireRow.Delete(Excel.XlDirection.xlUp);
                        }
                        else { break; }
                    }
                }
                oWBPon.SaveAs(pathexcel, Excel.XlFileFormat.xlWorkbookNormal, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Excel.XlSaveAsAccessMode.xlExclusive, Excel.XlSaveConflictResolution.xlLocalSessionChanges, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
                oWBPon.Close(true, Missing.Value, Missing.Value);

                string sheetform = sheetname + "f";

                AddWorksheet(pathexcel, sheetform);
                Excel._Workbook oWBf;
                Excel._Worksheet oSheetf;
                Excel.Range oRng;

                oWBf = (Excel._Workbook)oXL.Workbooks.Open(pathexcel, Missing.Value, false, Missing.Value, Missing.Value, Missing.Value, true, Missing.Value, Missing.Value, true, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);

                oSheetf = (Excel._Worksheet)oWBf.Worksheets[sheetform];


                object[] objHeadersf = { shortsheetname, CenDate };
                oSheetf.get_Range("A1", "B1").Value2 = objHeadersf;

                //Format A2:H2 as bold, vertical alignment = center.
                oSheetf.get_Range("A1", "B1").Font.Bold = true;
                oSheetf.get_Range("A1", "B1").VerticalAlignment = Excel.XlVAlign.xlVAlignCenter;

                // Create an array for the headers and add it to cells A2:H2.
                object[] objHeadersf0 = { "Period", "Pon", "Wt", "Śr", "Cz", "Pt", "So", "Nd" };
                oSheetf.get_Range("A2", "H2").Value2 = objHeadersf0;

                //Format A2:H2 as bold, vertical alignment = center.
                oSheetf.get_Range("A2", "H2").Font.Bold = true;
                oSheetf.get_Range("A2", "H2").VerticalAlignment = Excel.XlVAlign.xlVAlignCenter;


                String datetimedebutf = "00:00";
                String datetimefinf = "23:59";

                System.DateTime time3 = Convert.ToDateTime(datetimedebutf);
                System.DateTime time4 = Convert.ToDateTime(datetimefinf);


                while (time3 < time4.AddMinutes(-13))
                {
                    rownumberf = rownumberf + 1;


                    System.DateTime time5 = time3.AddMinutes(14);

                    string periodf = time3.ToString("HH:mm") + "-" + time5.ToString("HH:mm");
                    oSheetf.Cells[rownumberf, 1] = periodf;
                    oRng = oSheetf.get_Range("B" + rownumberf.ToString(), "B" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetPon + "'!R1:R65536,2,FALSE)";
                    oRng = oSheetf.get_Range("C" + rownumberf.ToString(), "C" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetWto + "'!R1:R65536,3,FALSE)";
                    oRng = oSheetf.get_Range("D" + rownumberf.ToString(), "D" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetSro + "'!R1:R65536,4,FALSE)";
                    oRng = oSheetf.get_Range("E" + rownumberf.ToString(), "E" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetCzw + "'!R1:R65536,5,FALSE)";
                    oRng = oSheetf.get_Range("F" + rownumberf.ToString(), "F" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetPia + "'!R1:R65536,6,FALSE)";
                    oRng = oSheetf.get_Range("G" + rownumberf.ToString(), "G" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetSob + "'!R1:R65536,7,FALSE)";
                    oRng = oSheetf.get_Range("H" + rownumberf.ToString(), "H" + rownumberf.ToString());
                    oRng.FormulaR1C1 = "=VLOOKUP(RC1,'" + sheetNied + "'!R1:R65536,8,FALSE)";


                    time3 = time3.AddMinutes(15);

                }
                oSheetf.Cells.Select();
                oSheetf.Cells.Copy(Missing.Value);
                oSheetf.Cells.PasteSpecial(Microsoft.Office.Interop.Excel.XlPasteType.xlPasteValues, Microsoft.Office.Interop.Excel.XlPasteSpecialOperation.xlPasteSpecialOperationNone, Missing.Value, Missing.Value);

                string[] strUserNames = new String[8] { sheetname, sheetPon, sheetWto, sheetSro, sheetCzw, sheetPia, sheetSob, sheetNied };
                foreach (Excel.Worksheet work in oWBf.Worksheets)
                {
                    //MessageBox.Show(work.Name.ToString());
                    foreach (string str in strUserNames)
                    {
                        if (work.Name.ToString() == str)
                        {
                            work.Delete();
                            break;
                        }
                    }
                }
                oSheetf.Name = sheetname;

                oWBf.SaveAs(pathexcel, Excel.XlFileFormat.xlWorkbookNormal, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Excel.XlSaveAsAccessMode.xlExclusive, Excel.XlSaveConflictResolution.xlLocalSessionChanges, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
                oWBf.Close(true, Missing.Value, Missing.Value);
                oXL.Quit();

                releaseObject(oSheetPon);
                releaseObject(oSheetWto);
                releaseObject(oSheetSro);
                releaseObject(oSheetCzw);
                releaseObject(oSheetPia);
                releaseObject(oSheetSob);
                releaseObject(oSheetNied);
                releaseObject(oWBPon);
                releaseObject(oSheet);
                releaseObject(oWB);
                releaseObject(oSheetf);
                releaseObject(oWBf);
                releaseObject(oXL);
            }

        }

        private void OpenWorkbook(string xlsfile, string worksheetname)
        {
            using (new ExcelUILanguageHelper())
            {
                Excel.Application xlApp;
                Excel.Workbook xlWorkBook;
                Excel.Worksheet xlWorkSheet;
                object misValue = System.Reflection.Missing.Value;

                xlApp = new Excel.ApplicationClass();
                xlWorkBook = xlApp.Workbooks.Add(misValue);
                xlWorkSheet = (Excel.Worksheet)xlWorkBook.Worksheets.get_Item(1);
                xlWorkSheet.Name = worksheetname;

                xlWorkBook.SaveAs(xlsfile, Excel.XlFileFormat.xlWorkbookNormal, misValue, misValue, misValue, misValue, Excel.XlSaveAsAccessMode.xlExclusive, misValue, misValue, misValue, misValue, misValue);
                xlWorkBook.Close(true, misValue, misValue);
                xlApp.Quit();

                releaseObject(xlWorkSheet);
                releaseObject(xlWorkBook);
                releaseObject(xlApp);

            }
        }
        private void AddWorksheet(string xlsfile, string worksheetname)
        {
            using (new ExcelUILanguageHelper())
            {
                Excel.Application xlApp = null;
                Excel.Workbook xlWorkbook = null;
                Excel.Sheets xlSheets = null;
                Excel.Worksheet xlNewSheet = null;

                try
                {
                    xlApp = new Excel.Application();

                    if (xlApp == null)
                        return;

                    xlWorkbook = xlApp.Workbooks.Open(xlsfile, 0, false, 5, "", "", false, Excel.XlPlatform.xlWindows, "", true, false, 0, true, false, false);
                    xlSheets = xlWorkbook.Sheets as Excel.Sheets;

                    // The first argument below inserts the new worksheet as the first one
                    xlNewSheet = (Excel.Worksheet)xlSheets.Add(xlSheets[1], Type.Missing, Type.Missing, Type.Missing);
                    xlNewSheet.Name = worksheetname;

                    xlWorkbook.Save();
                    xlWorkbook.Close(Type.Missing, Type.Missing, Type.Missing);
                    xlApp.Quit();
                }


                finally
                {
                    releaseObject(xlNewSheet);
                    releaseObject(xlSheets);
                    releaseObject(xlWorkbook);
                    releaseObject(xlApp);

                }

            }
        }

        private void releaseObject(object obj)
        {
            System.Runtime.InteropServices.Marshal.ReleaseComObject(obj);

            obj = null;

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
            GC.WaitForPendingFinalizers();


        }




        private void Form1_Load(object sender, EventArgs e)
        {



            RegistryKey masterKey = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
            string path1 = masterKey.GetValue("Path") + "/ZONE/PL_REGIO.TBL";
            string path2 = masterKey.GetValue("Path") + "/ZONE/PL_REGIO.txt";
            masterKey.Close();

            string myFile = "";

            File.Delete(path2);
            string fileName = path1;

            using (StreamReader srRead = new StreamReader(fileName))
            {

                for (int z = 0; z < 8; z++)
                {

                    srRead.ReadLine();


                }

                while ((myFile = srRead.ReadLine()) != null)
                {
                    AppendToFile(myFile, path2);

                }

                srRead.Close();








                FileHelperEngine engine = new FileHelperEngine(typeof(region));

                region[] res = (region[])engine.ReadFile(path2);

                foreach (region Reg in res)
                {
                    listBox1.Items.Add(Reg.LongName);
                }
            }


        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            RegistryKey masterKey = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
            string path2 = masterKey.GetValue("Path") + "\\ZONE\\PL_REGIO.txt";
            string pathStat = masterKey.GetValue("Path") + "\\ZONE\\";
            masterKey.Close();

            string curItem = listBox1.SelectedItem.ToString();
            string myFile = "";


            FileHelperEngine engine = new FileHelperEngine(typeof(region));

            region[] res = (region[])engine.ReadFile(path2);

            foreach (region Reg in res)
            {
                if (curItem == Reg.LongName)
                {
                    label3.Text = Reg.Code;
                    string pathStat1 = pathStat + Reg.Code + "\\PLSTATIO.TBL";
                    string pathStat2 = pathStat + Reg.Code + "\\PLSTATIO.txt";
                    File.Delete(pathStat2);
                    string fileName = pathStat1;


                    using (StreamReader srRead = new StreamReader(fileName))
                    {
                        string nbstat = "";
                        for (int z = 0; z < 11; z++)
                        {
                            srRead.ReadLine();
                        }

                        int n = 1;
                        while (n < 2)
                        {
                            myFile = srRead.ReadLine();
                            nbstat = myFile.Substring(1);
                            n++;
                        }
                        for (int z = 12; z < 16; z++)
                        {
                            srRead.ReadLine();
                        }


                        for (int z = 16; z < 16 + Convert.ToInt32(nbstat); z++)
                        {
                            myFile = srRead.ReadLine();
                            AppendToFile(myFile, pathStat2);

                        }

                        srRead.Close();
                    }
                    if (File.Exists(pathStat2))
                    {
                        FileHelperEngine engine2 = new FileHelperEngine(typeof(Station));

                        Station[] Stat = (Station[])engine2.ReadFile(pathStat2);

                        foreach (Station Sta in Stat)
                        {
                            listBox2.Items.Add(Sta.LongName);
                            listBox2.Sorted = true;
                        }
                    }

                    else
                    {
                        MessageBox.Show("There is no Station for this region!");
                    }
                }


            }

        }



        private void listBox2_DoubleClick(object sender, System.EventArgs e)
        {
            for (int i = 0; i < this.listBox2.SelectedItems.Count; i++)
            {

                string curItem = listBox2.SelectedItems[i].ToString();


                listBox3.Items.Add(curItem);

            }


        }

        private void button2_Click(object sender, EventArgs e)
        {
            RegistryKey masterKey = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
            string pathini = masterKey.GetValue("Path").ToString() + "\\ATAAETUD.INI";
            string pathStat = masterKey.GetValue("Path") + "\\ZONE\\";
            string pathStat2 = pathStat + label3.Text + "\\PLSTATIO.txt";
            masterKey.Close();
            label4.Text = this.listBox3.Items.Count.ToString() + ", ";
            for (int i = 0; i < this.listBox3.Items.Count; i++)
            {
                if (i != this.listBox3.Items.Count - 1)
                {
                    string Itemi = listBox3.Items[i].ToString();

                    FileHelperEngine engine2 = new FileHelperEngine(typeof(Station));

                    Station[] Stat = (Station[])engine2.ReadFile(pathStat2);

                    foreach (Station Sta in Stat)
                    {
                        if (Itemi == Sta.LongName)
                        {
                            label4.Text = label4.Text + Sta.Nr.Substring(2) + ", ";
                        }
                    }
                }
                else
                {
                    string Itemi = listBox3.Items[i].ToString();

                    FileHelperEngine engine2 = new FileHelperEngine(typeof(Station));

                    Station[] Stat = (Station[])engine2.ReadFile(pathStat2);

                    foreach (Station Sta in Stat)
                    {
                        if (Itemi == Sta.LongName)
                        {
                            label4.Text = label4.Text + Sta.Nr.Substring(2);
                        }
                    }

                }
            }
            // MessageBox.Show(label4.Text);
            IniFile ini = new IniFile(pathini);
            ini.IniWriteValue("DefOption", "TARIFEXPORTER", label3.Text + ", " + label4.Text);
            DialogResult dlgResult = MessageBox.Show("Now please execute export from ATR (Cenniki/Export rates) into the catalog cenniki.When done, click ok.", "Go for export", MessageBoxButtons.OK);
            if (dlgResult == DialogResult.OK)
            {
                button2.Visible = false;
                button3.Visible = true;
            }



        }

        private void button3_Click(object sender, EventArgs e)
        {
            button3.Visible = false;

            ProgressWindow progress = new ProgressWindow();
            progress.Text = "Work";
            System.Threading.ThreadPool.QueueUserWorkItem(new System.Threading.WaitCallback(DoSomeWork), progress);
            progress.ShowDialog();
        }

        private void DoSomeWork(object status)
        {


            RegistryKey masterKey = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
            string pathStat = masterKey.GetValue("Path") + "/ZONE/";
            string filepath = masterKey.GetValue("Path").ToString() + "\\cenniki\\";
            string pathCen = masterKey.GetValue("Path").ToString() + "\\cenniki\\cennik.txt";
            string pathCen2 = masterKey.GetValue("Path").ToString() + "\\cenniki\\cennik";
            string pathStat2 = pathStat + label3.Text + "/PLSTATIO.txt";
            masterKey.Close();
            string fileName = pathCen;
            string myFile = "";
            int GrilQty = 0;
            File.Delete(pathCen2 + "opis.txt");

            IProgressCallback callback = status as IProgressCallback;

            try
            {

                //Determine quantite de grille

                using (StreamReader srRead0 = new StreamReader(fileName))
                {
                    for (int x = 0; x < 1; x++)
                    {

                        srRead0.ReadLine();

                    }
                    int n0 = 1;
                    while (n0 < 2)
                    {
                        myFile = srRead0.ReadLine();
                        AppendToFile(myFile, pathCen2 + "opis.txt");
                        n0++;

                    }
                    srRead0.Close();
                }

                FileHelperEngine engine0 = new FileHelperEngine(typeof(CennikOpis));

                CennikOpis[] res0 = (CennikOpis[])engine0.ReadFile(pathCen2 + "opis.txt");

                foreach (CennikOpis Gril0 in res0)
                {
                    GrilQty = Gril0.nbgrille;
                }
                if (GrilQty > 0)
                {
                    int porcentpart = 100 / GrilQty;
                    int step = porcentpart / 5;
                    int stepini = 0;
                    //ouvre un nouveau fichier texte pour chaque grille
                    callback.Begin(0, 100);

                    for (int y = 1; y < GrilQty + 1; y++)
                    {

                        int stepFinal = stepini + step;
                        callback.SetText(String.Format("Preparing file nr: {0}", y));
                        callback.StepTo(stepFinal);


                        string pathCen3 = pathCen2 + Convert.ToString(y) + ".txt";

                        File.Delete(pathCen3);


                        //recupere l'intitule de la grille

                        int nline = Convert.ToInt32(this.label5.Text);

                        using (StreamReader srRead1 = new StreamReader(fileName))
                        {

                            for (int z = 0; z < 2 + nline; z++)
                            {

                                srRead1.ReadLine();

                            }
                            int n1 = 1;
                            while (n1 < 2)
                            {
                                myFile = srRead1.ReadLine();
                                AppendToFile(myFile, pathCen3);
                                n1++;

                            }

                            srRead1.Close();
                        }
                        //recupere les blocs de la grille
                        callback.StepTo(stepFinal + step);

                        FileHelperEngine engine = new FileHelperEngine(typeof(CennikGrille));

                        CennikGrille[] res = (CennikGrille[])engine.ReadFile(pathCen3);

                        foreach (CennikGrille Gril in res)
                        {
                            FileHelperEngine statread = new FileHelperEngine(typeof(Station));

                            Station[] Stat = (Station[])statread.ReadFile(pathStat2);

                            foreach (Station Sta in Stat)
                            {
                                if (Gril.Stat == Convert.ToInt32(Sta.Nr))
                                {
                                    int NaLenght = Sta.LongName.Length;
                                    if (NaLenght > 25)
                                    {
                                        SetStatus(Sta.LongName.Replace(" ", "").Replace("_", "").Replace("/", "").Replace("(", "").Replace(")", "").Substring(0, 20));
                                    }
                                    else
                                    {
                                        SetStatus(Sta.LongName.Replace(" ", "").Replace("_", "").Replace("/", "").Replace("(", "").Replace(")", ""));

                                    }
                                    SetStatus1(Gril.StartDate.ToString() + "-" + Gril.EndDate.ToString());
                                    SetStatus0(Sta.LongName);
                                    break;
                                }
                            }
                            callback.StepTo(stepFinal + (step * 2));
                            string filepath2 = filepath + this.label6.Text + ".txt";
                            File.Delete(filepath2);
                            using (StreamReader srRead2 = new StreamReader(fileName))
                            {

                                for (int z = 0; z < 3 + nline; z++)
                                {

                                    srRead2.ReadLine();

                                }
                                int n2 = 1;
                                while (n2 < Gril.BlocQty + 1)
                                {
                                    myFile = srRead2.ReadLine();
                                    AppendToFile(myFile, filepath2);
                                    n2++;

                                }

                                srRead2.Close();
                            }
                            callback.StepTo(stepFinal + (3 * step));
                            //situe le curseur de la prochaine grille

                            SetStatus2(Convert.ToString(Convert.ToInt32(this.label5.Text) + Gril.BlocQty + 1));

                            CreateExcel(filepath2, y, this.label6.Text, this.label7.Text, this.label8.Text, Gril.BlocQty);
                            callback.StepTo(stepFinal + (4 * step));
                            stepini = stepFinal + (4 * step);
                        }



                    }
                }
            
            else
            {
                MessageBox.Show ("No price list for this station!");
                callback.End();
                Application.Restart();
            }
            }
            catch (System.Threading.ThreadAbortException)
            {

            }
            catch (System.Threading.ThreadInterruptedException)
            {
                // And here, if we can

            }
            finally
            {
                if (callback != null)
                {
                    Thread.Sleep(5000);
                    MessageBox.Show("Excel File ready! Thank you.");

                    callback.End();
                    this.Closeit();
                }
            }

        }
        public void Closeit()
        {
            using (new ExcelUILanguageHelper())
            {
                RegistryKey masterKeyFinal = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
                string pathexcelFinal = masterKeyFinal.GetValue("Path") + "/cenniki/cennik.xls";
                masterKeyFinal.Close();

                Excel.Application xlAppFinal = null;
                Excel.Workbook xlWorkbookFinal = null;


                xlAppFinal = new Excel.Application();
                xlAppFinal.Visible = true;

                xlWorkbookFinal = xlAppFinal.Workbooks.Open(pathexcelFinal, 0, false, 5, "", "", false, Excel.XlPlatform.xlWindows, "", true, false, 0, true, false, false);
                CloseForm(this);
            }
        }

        delegate void CloseMethod(Form form);
        static private void CloseForm(Form form)
        {
            if (!form.IsDisposed)
            {
                if (form.InvokeRequired)
                {
                    CloseMethod method = new CloseMethod(CloseForm);
                    form.Invoke(method, new object[] { form });
                }
                else
                {
                    form.Close();
                }
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            listBox2.SelectedItems.Clear();
            int index = listBox2.FindString(textBox1.Text);

            // Determine if a valid index is returned. Select the item if it is valid.
            if (index != -1)
            {

                listBox2.SetSelected(index, true);
            }
            else
            {
                MessageBox.Show("The search string did not match any items in the ListBox");
            }
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            for (int i = 0; i < this.listBox2.SelectedItems.Count; i++)
            {

                string curItem = listBox2.SelectedItems[i].ToString();


                listBox3.Items.Add(curItem);

            }

        }

        private void button4_Click(object sender, EventArgs e)
        {
            for (int ii = 0; ii < this.listBox3.SelectedItems.Count; ii++)
            {
                string todel = listBox3.SelectedItems[ii].ToString();
                listBox3.Items.Remove(todel);
            }

        }

        

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            
                 RegistryKey masterKey = Registry.CurrentUser.CreateSubKey("Software\\JFC\\APPRadio\\Settings");
            string pathini = masterKey.GetValue("Path").ToString() + "\\ATAAETUD.INI";
            string pathStat = masterKey.GetValue("Path") + "\\ZONE\\";
            string pathStat2 = pathStat + label3.Text + "\\PLSTATIO.txt";
            masterKey.Close();

            if (checkBox1.Checked)
            {
                DateTime dtnow = DateTime.Now;
                listBox2.Items.Clear();

                FileHelperEngine engine3 = new FileHelperEngine(typeof(Station));

                Station[] Stat = (Station[])engine3.ReadFile(pathStat2);

                foreach (Station Sta in Stat)
                {
                    DateTime dtend = DateTime.ParseExact(Sta.DateTo, "ddMMyy", System.Globalization.CultureInfo.InvariantCulture);

                    if (dtend.Year == 1959)
                    {
                        //MessageBox.Show(dtend.ToString());
                        listBox2.Items.Add(Sta.LongName);
                        listBox2.Sorted = true;
                    }
                    else
                    {

                        int result = DateTime.Compare(dtend, dtnow);
                        if (result > 0)
                        {
                           // MessageBox.Show(dtend.ToString());
                            listBox2.Items.Add(Sta.LongName);
                            listBox2.Sorted = true;
                        }

                    }
                }
            }
            else
            {
                listBox2.Items.Clear();

                FileHelperEngine engine3 = new FileHelperEngine(typeof(Station));

                Station[] Stat = (Station[])engine3.ReadFile(pathStat2);

                foreach (Station Sta in Stat)
                {
                    listBox2.Items.Add(Sta.LongName);
                    listBox2.Sorted = true;
                }

            }






        }
    }
}

            
           





