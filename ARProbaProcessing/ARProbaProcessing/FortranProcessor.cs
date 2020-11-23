using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Security;

namespace ARProbaProcessing
{
    public class FortranProcessor
    {
        private ARProba aRProba;

        public FortranProcessor(ARProba aRProba)
        {
            this.aRProba = aRProba;

            variableHandlers = new VarHandlers(aRProba, this);

        }

        public void CompileLinkAndRun(string fileName, bool run)
        {
            string efile = aRProba.ProgramPath + fileName + ".exe";
            string ofile = aRProba.ProgramPath + fileName + ".obj";
            string errfile = aRProba.ProgramPath + fileName + ".err";

            string outfile = efile + ".output";

            // First clean the old files
            if (File.Exists(efile))
                File.Delete(efile);
            if (File.Exists(ofile))
                File.Delete(ofile);

            // Build the file
            ProcessStartInfo pbuild = new ProcessStartInfo("make.bat", fileName + ".for");
            pbuild.WorkingDirectory = aRProba.ProgramPath;
            pbuild.WindowStyle = ProcessWindowStyle.Hidden;
            Process mp = Process.Start(pbuild);
            mp.WaitForExit();

            mp.Close();

            if (File.Exists(errfile))
            {
                Console.WriteLine(File.ReadAllText(errfile));
            }

            if (!File.Exists(efile))
                throw new Exception("Compile error on " + fileName);

            // remove obj file
            if (File.Exists(ofile))
                File.Delete(ofile);

            if (run)
            {
                //ProcessStartInfo prun = new ProcessStartInfo(efile);

                //prun.UseShellExecute = false;
                //prun.RedirectStandardOutput = true;

                //prun.CreateNoWindow = true;


                //ProcessStartInfo prun = new ProcessStartInfo(aRProba.ProgramPath + "run.bat", aRProba.ProgramShortPath + fileName + ".exe");
                ProcessStartInfo prun = new ProcessStartInfo("run.bat", fileName + ".exe");
                //ProcessStartInfo prun = new ProcessStartInfo(aRProba.ProgramShortPath + fileName + ".exe");
                //ProcessStartInfo prun = new ProcessStartInfo(efile);
                //ProcessStartInfo prun = new ProcessStartInfo(fileName);
                prun.WorkingDirectory = aRProba.ProgramPath;
                //prun.WorkingDirectory = aRProba.OutputPath;
                //prun.UseShellExecute = false;
                prun.UseShellExecute = true;
                prun.LoadUserProfile = true;

                //prun.CreateNoWindow = true;
                //prun.CreateNoWindow = true;
                //prun.ErrorDialog = true;
                prun.WindowStyle = ProcessWindowStyle.Hidden;

                //prun.RedirectStandardOutput = true;

                Console.WriteLine("Processing : " + efile);
                var rp = Process.Start(prun);

                //char[] buffer = new char[512];
                //while (!rp.HasExited)
                //{
                //    int len = rp.StandardOutput.Read(buffer, 0, buffer.Length);
                //    while (len != 0)
                //    {
                //        Console.Write(buffer, 0, len);
                //        len = rp.StandardOutput.Read(buffer, 0, buffer.Length);
                //    }
                //    Thread.Sleep(500);
                //}

                rp.WaitForExit();

                Console.WriteLine("Done!");

                //string res = rp.StandardOutput.ReadToEnd();
                rp.Close();

                Console.WriteLine(File.ReadAllText(outfile));

                //Console.WriteLine(res);

            }

        }

        public void ProcessFortanFile(string fileName)
        {
            string fortanTplFile = aRProba.ProgramPath + @"ForTemplate\" + fileName + ".for";
            string outputFile = aRProba.ProgramPath + fileName + ".for";

            var lines = File.ReadAllLines(fortanTplFile, Encoding.GetEncoding("Windows-1252"));
            var fortranOutput = new StreamWriter(outputFile, false, Encoding.GetEncoding("Windows-1252"));

            foreach (var line in lines)
            {
                StringBuilder newLine = new StringBuilder();
                var sections = line.Split('#');
                bool variableMode = false;
                foreach (var sec in sections)
                {
                    if (variableMode)
                    {
                        string variableName = sec;

                        HandleVariable(variableName, newLine);

                        // The next item is not a variable any more
                        variableMode = false;
                    }
                    else
                    {
                        newLine.Append(sec);

                        // The next item is a variable
                        variableMode = true;
                    }
                }

                fortranOutput.Write(newLine.ToString());
                fortranOutput.WriteLine();
            }


            fortranOutput.Close();
        }



        private void HandleVariable(string variableName, StringBuilder newLine)
        {
            List<object> args = new List<object>();
            args.Add(newLine);

            List<Type> argTypes = new List<Type>();

            argTypes.Add(typeof(StringBuilder));

            var words = variableName.Split(',');
            for (int i = 1; i < words.Length; i++)
            {
                args.Add(words[i]);
                argTypes.Add(typeof(String));
            }
            var methodName = words[0];
            var xx = variableHandlers.GetType().GetMethods();
            variableHandlers.GetType().GetMethod(methodName, argTypes.ToArray()).Invoke(variableHandlers, args.ToArray());
        }

        private VarHandlers variableHandlers;

        public class VarHandlers
        {
            private ARProba aRProba;

            private FortranProcessor processor;

            public VarHandlers(ARProba aRProba, FortranProcessor processor)
            {
                this.aRProba = aRProba;
                this.processor = processor;
            }

            public void SIGN_LINE_LEN_FULL(StringBuilder line)
            {
                line.Append(aRProba.SigLineLen + 2);
            }

            public void SIGN_LINE_LEN_BEFORE_HAB(StringBuilder line)
            {
                int len = aRProba.SignVars["LV01"].Position - 1;
                line.Append(len);
            }
                        
            public void SIGN_LINE_LEN_AFTER_HAB(StringBuilder line)
            {
                int len = aRProba.SigLineLen;

                var var = aRProba.SignVars[string.Format("DI{0:00}", aRProba.AllHabStationCount)];
                len = len - (var.Position + var.Repetition) + 1;

                line.Append(len);
            }
            public void SIGN_LINE_LEN_AFTER_HAB_WITH_PAD(StringBuilder line)
            {
                int len = aRProba.SigLineLen;

                var var = aRProba.SignVars[string.Format("DI{0:00}", aRProba.AllHabStationCount + aRProba.HabAndNotoTotalStationListCount)];
                len = len - (var.Position + var.Repetition) + 1;

                line.Append(len);
            }
            public void SIGJFC_BDE(StringBuilder line)
            {
                line.Append(aRProba.FullSigFileShortName);
            }
            public void U1XX_COL(StringBuilder line, string colName)
            {
                int pos = aRProba.U1xxModalityVars[colName];
                line.Append(pos);
            }

            public void NB_STA_ALL_HAB_WITH_TOTAL(StringBuilder line)
            {
                line.Append(aRProba.AllHabStationCount + aRProba.HabAndNotoTotalStationListCount);
            }

            public void OUTPUT(StringBuilder line)
            {
                line.Append(aRProba.OutputShortPath);
            }

            public void INPUT(StringBuilder line)
            {
                line.Append(aRProba.InputShortPath);
            }

            public void YEAR(StringBuilder line)
            {
                line.Append(aRProba.YearName);
            }

            public void NB_INDIV(StringBuilder line)
            {
                line.Append(aRProba.IndivCount);
            }

            public void NB_STA_TOTAL(StringBuilder line)
            {
                line.Append(aRProba.StationCount);
            }

            public void NB_STA_HAB_NOTO_TOTAL(StringBuilder line)
            {
                line.Append(aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount);
            }

            public void NB_STA_HAB_NOTO(StringBuilder line)
            {
                line.Append(aRProba.HabAndNotoStationCount);
            }

            public void NB_STA_HAB_NOTO(StringBuilder line, string soffset)
            {
                int offset = int.Parse(soffset);
                line.Append(aRProba.HabAndNotoStationCount + offset);
            }

            public void NB_STA_IDF(StringBuilder line)
            {
                var req = from sta in aRProba.HabAndNotoStationList
                          where sta.IsIdf
                          select sta;

                line.Append(req.Count());
            }

            public void STA_IDF_LIST_COMMENT(StringBuilder line)
            {
                var req = from sta in aRProba.HabAndNotoStationList
                          where sta.IsIdf
                          select sta;
                line.AppendLine("C Les stations IDF sont les suivantes:");
                foreach (var s in req)
                {
                    line.AppendLine("C           " + s.Name);
                }
            }

            public void STA_IDF_LIST_NO_SUDRAD_MASK(StringBuilder line)
            {
                int count = 0;
                bool first = true;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (!station.Name.ToUpper().Equals("SUD RADIO"))
                    {
                        if (count == NB_ITEM_ON_A_LINE)
                        {
                            line.AppendLine(",");
                            line.Append("     -");
                            count = 0;
                        }
                        else
                        {
                            count++;
                            if (first)
                                first = false;
                            else
                                line.Append(',');
                        }
                        if (station.IsIdf)
                            line.Append(1);
                        else
                            line.Append(0);
                    }
                }
            }

            public void NB_STA_HAB(StringBuilder line)
            {
                line.Append(aRProba.HabStationCount);
            }

            public void NB_STA_ALL_HAB(StringBuilder line)
            {
                line.Append(aRProba.AllHabStationCount);
            }

            public void NB_STA_TOTAL_ONLY(StringBuilder line)
            {
                line.Append(aRProba.HabAndNotoTotalStationListCount);
            }

            public void JFC_FILE_LEN_ENR(StringBuilder line)
            {
                int len = 6 * (aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount) + 96;
                line.Append(len);
            }

            public void JFC_FILE_LEN_ENR_COMMENT(StringBuilder line)
            {
                int nb_sta = aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount;
                int len = 6 * (aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount) + 96;
                line.Append("Dans les fichier .jfc, chaque enregistrement (individu) est de " + len + " short = 6 short (96 qh)*" + nb_sta + " stations + poids qh (96 short)");
            }

            public void JFC_FILE_LEN_ENR_SHORT(StringBuilder line)
            {
                int len = 6 * (aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount);
                line.Append(len);
            }

            public void JFC_FILE_LEN_ENR_SHORT_COMMENT(StringBuilder line)
            {
                int nb_sta = aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount;
                int len = 6 * (aRProba.HabAndNotoStationCount + aRProba.HabAndNotoTotalStationListCount);
                line.Append("Dans les fichier .jfc, chaque enregistrement (individu) est de " + len + " short = 6 short (96 qh)*" + nb_sta + " stations");
            }

            public void COL(StringBuilder line, string colName)
            {
                int pos = aRProba.SignVars[colName].Position;
                line.Append(pos);
            }

            public void COL(StringBuilder line, string colName, string idx)
            {
                int pos = aRProba.SignVars[colName].Position;
                int index = int.Parse(idx);

                line.Append(pos + index);
            }

            public void COLAPRES(StringBuilder line, string colName, string idx)
            {
                int pos = aRProba.SignVars[colName].Position;
                int index = int.Parse(idx);

                var varLastDI = aRProba.SignVars[string.Format("DI{0:00}", aRProba.AllHabStationCount)];
                int offset = (varLastDI.Position + varLastDI.Repetition);

                line.Append(pos + index - offset + 1);
            }

            public const int NB_ITEM_ON_A_LINE = 15;

            public void HAB_NOTO_STA_LIST_ID(StringBuilder line)
            {
                int count = 0;
                bool first = true;
                foreach (var station in aRProba.HabAndNotoStationList.Concat(aRProba.HabAndNotoTotalStationList))
                {
                    if (count == NB_ITEM_ON_A_LINE)
                    {
                        line.AppendLine(",");
                        line.Append("     -");
                        count = 0;
                    }
                    else
                    {
                        count++;
                        if (first)
                            first = false;
                        else
                            line.Append(',');
                    }
                    line.Append(station.Id);

                }

            }

            public void HAB_NOTO_STA_LIST_COMMENT(StringBuilder line)
            {
                line.AppendLine("C	id	idx    sta");
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    line.AppendLine(string.Format("C	{0:00}	{1:000}    {2}", station.Id, station.Index, station.Name));
                }
                line.AppendLine();
                line.AppendLine("C Attention Total radio est obligatoirement a le fin avec total TV si present");
                foreach (var station in aRProba.HabAndNotoTotalStationList)
                {
                    line.AppendLine(string.Format("C	{0:00}	{1:000}    {2}", station.Id, station.Index, station.Name));
                }
            }

            private int GetTotalRadioIndex()
            {
                if (aRProba.HabAndNotoTotalStationList.Count() == 0)
                {
                    var list = (from sta in aRProba.StationList
                                where sta.Mode != ARProbaProcessing.ARProba.Station.eSignVariable.None
                                   && (sta.Name.Equals("total TV", StringComparison.CurrentCultureIgnoreCase)
                                       || sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                                       )
                                select sta).ToArray();
                    return list.First().Index;
                }
                else
                {
                    var req = from sta in aRProba.HabAndNotoTotalStationList
                              where sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                              select sta;

                    return req.First().Index;
                }
            }


            public void TOTAL_RADIO_INDEX(StringBuilder line)
            {
                line.Append(GetTotalRadioIndex());
            }

            public void HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO(StringBuilder line)
            {
                int totalidx = GetTotalRadioIndex();
                int count = 0;
                bool first = true;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (count == NB_ITEM_ON_A_LINE)
                    {
                        line.AppendLine(",");
                        line.Append("     -");
                        count = 0;
                    }
                    else
                    {
                        count++;
                        if (first)
                            first = false;
                        else
                            line.Append(',');
                    }
                    if (station.Mode == ARProba.Station.eSignVariable.Habit)
                        line.Append(station.Index);
                    else
                        line.Append(totalidx);

                }

            }

            public void HAB_0_NOTO_1_STA_LIST_MASK(StringBuilder line)
            {
                int totalidx = GetTotalRadioIndex();
                int count = 0;
                bool first = true;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (count == NB_ITEM_ON_A_LINE)
                    {
                        line.AppendLine(",");
                        line.Append("     -");
                        count = 0;
                    }
                    else
                    {
                        count++;
                        if (first)
                            first = false;
                        else
                            line.Append(',');
                    }
                    if (station.Mode == ARProba.Station.eSignVariable.Habit)
                        line.Append(0);
                    else
                        line.Append(1);

                }

            }
            public void CRENONIN_NOTO_FILTER(StringBuilder line, string varName)
            {
                bool first = true;
                int count = 0;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    count++;
                    if (station.Mode == ARProba.Station.eSignVariable.Notoriety)
                    {
                        if (first)
                        {
                            first = false;
                        }
                        else
                        {
                            line.AppendLine();
                            line.Append("     -.AND.");
                        }
                        line.Append(string.Format("({0}.NE.{1})", varName, count));
                    }
                }
            }

            public void CRENONIN_NOTO_HANDLING(StringBuilder line)
            {
                // Add comments
                line.AppendLine("C	id	idx    sta");
                int count = 0;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    count++;
                    if (station.Mode == ARProba.Station.eSignVariable.Notoriety)
                        line.AppendLine(string.Format("C	{0:00}	{1:000}    {2} {3}", station.Id, station.Index, station.Name, count));
                }
                line.AppendLine();

                var varLastDI = aRProba.SignVars[string.Format("DI{0:00}", aRProba.AllHabStationCount)];
                int offset = (varLastDI.Position + varLastDI.Repetition);

                count = 0;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    count++;
                    if (station.Mode == ARProba.Station.eSignVariable.Notoriety)
                    {
                        int notoNum = station.Index / 100;
                        int notoMod = station.Index % 100;

                        var ques = aRProba.SignVars[string.Format("NOTO{0}", notoNum)];

                        int pos = ques.Position + notoMod - 1;

                        line.AppendLine(string.Format("C {0}", station.Name));
                        line.AppendLine(string.Format("        IF(NOP.EQ.{0}) THEN", count));
                        line.AppendLine(string.Format("          IF((APRES({0})-48).EQ.1) GO TO 201", pos - offset + 1));
                        line.AppendLine(string.Format("        ENDIF"));
                    }
                }
        
            }

            public void NB_U1XX_STATION(StringBuilder line)
            {
                line.Append(aRProba.U1xxStationCount);
            }

            public void NB_U1XX_MODALITY(StringBuilder line)
            {
                line.Append(aRProba.U1xxModalityCount);
            }

            public void NB_U1XX_MODALITY_PLUS_2(StringBuilder line)
            {
                line.Append(aRProba.U1xxModalityCount + 2);
            }

            public void U1XX_TXT_POP_LV(StringBuilder line)
            {
                line.Append(aRProba.U1xxPopTxt[0]);
            }

            public void U1XX_TXT_POP_SA(StringBuilder line)
            {
                line.Append(aRProba.U1xxPopTxt[1]);
            }

            public void U1XX_TXT_POP_DI(StringBuilder line)
            {
                line.Append(aRProba.U1xxPopTxt[2]);
            }

            public void HAB_NOTO_STA_LIST_U1XX_INDEXES(StringBuilder line)
            {
                int count = 0;
                bool first = true;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (count == NB_ITEM_ON_A_LINE)
                    {
                        line.AppendLine(",");
                        line.Append("     -");
                        count = 0;
                    }
                    else
                    {
                        count++;
                        if (first)
                            first = false;
                        else
                            line.Append(',');
                    }
                    line.Append(station.U1xxIdx);
                }
            }

            public void IDXSUDRAD(StringBuilder line)
            {
                int idx = 1;
                bool done = false;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (station.Name.ToUpper().Equals("SUD RADIO"))
                    {
                        line.Append(idx);
                        done = true;
                    }
                    idx++;
                }

                if (!done)
                    line.Append(999);

            }

            public void PENETRE_WRITE_ALL_STATION_HANDLING(StringBuilder line)
            {
                int count = 0;
                bool first = true;
                foreach (var station in aRProba.HabAndNotoStationList.Concat(aRProba.HabAndNotoTotalStationList))
                {
                    count++;
                    line.Append("        ");
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        line.Append("ELSE ");
                    }
                    line.AppendLine(string.Format("IF (K.EQ.{0}) THEN", count));
                    line.AppendLine(string.Format("        WRITE(13,{0:000}) 100.*REAL(ITAP(K))/{1}", count + 100, aRProba.U1xxPopTxt[0]));
                }

                line.AppendLine("        ENDIF");
            }

            public void PENETRE_FORMAT_ALL_STATION_HANDLING(StringBuilder line)
            {
                int count = 100;
                foreach (var station in aRProba.HabAndNotoStationList.Concat(aRProba.HabAndNotoTotalStationList))
                {
                    count++;
                    line.AppendLine(string.Format("  {0:000} FORMAT(' {1,48}',10(F8.2,2X))", count, station.Name));
                }

            }


            public void ASYMPT_WRITE_ALL_STATION_NO_SUDRAD_HANDLING(StringBuilder line)
            {
                int count = 0;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (!station.Name.ToUpper().Equals("SUD RADIO"))
                    {
                        count++;
                        line.AppendLine("      DO I=1,N");
                        line.AppendLine("      COUV(I)=0.");
                        line.AppendLine("      DO IND=1,NIND");
                        line.AppendLine(string.Format("      IF(TABRES(I,{0},IND).EQ.1) COUV(I)=COUV(I)+IPOI(IND)", count));
                        line.AppendLine("      ENDDO");
                        line.AppendLine("      COUV(I)=COUV(I)*100./IPOPS(I)");
                        line.AppendLine("      ENDDO");
                        line.AppendLine(string.Format("      write(16,{0})COUV", count + 100));

                    }
                }
            }

            public void ASYMPT_FORMAT_ALL_STATION_NO_SUDRAD_HANDLING(StringBuilder line)
            {
                int count = 100;
                foreach (var station in aRProba.HabAndNotoStationList)
                {
                    if (!station.Name.ToUpper().Equals("SUD RADIO"))
                    {
                        count++;
                        line.AppendLine(string.Format("  {0:000} FORMAT(' {1,48}',10(F8.2,2X))", count, station.Name));
                    }
                }

            }


        }

    }
}
