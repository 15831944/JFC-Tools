using System;
using System.IO;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mLog
    {
        private static string svPathLog = Environment.CurrentDirectory + "\\Logs\\";

        private static string szformatdate = "yyyyMMdd";

        private static long nvDayPurge = 30L;

        public static void WriteLog(string svString)
        {
            if (!Directory.Exists(svPathLog))
            {
                Directory.CreateDirectory(svPathLog);
            }
            string path = svPathLog + Conversions.ToString(DateTime.Now.Year) + StrFormat2Char(DateTime.Now.Month) + StrFormat2Char(DateTime.Now.Day) + ".log";
            if (!File.Exists(path))
            {
                PurgeLogs();
            }
            StreamWriter streamWriter = File.AppendText(path);
            streamWriter.WriteLine(DateTime.Now.ToString() + "  " + svString);
            streamWriter.Flush();
            streamWriter.Close();
        }

        private static void PurgeLogs()
        {
            string[] files = Directory.GetFiles(svPathLog, "????????.log", SearchOption.TopDirectoryOnly);
            foreach (string text in files)
            {
                string s = mFileFunction.ParsePath(text, 1);
                try
                {
                    DateTime date = DateTime.ParseExact(s, szformatdate, null);
                    if (DateAndTime.DateDiff(DateInterval.Day, date, DateTime.Now) > nvDayPurge)
                    {
                        File.Delete(text);
                    }
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    File.Delete(text);
                    ProjectData.ClearProjectError();
                }
            }
        }

        private static string StrFormat2Char(object Obj)
        {
            return Strings.Right("00" + Conversions.ToString(Obj).Trim(), 2);
        }
    }
}