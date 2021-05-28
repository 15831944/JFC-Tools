using System;
using System.Diagnostics;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mKillProcess
    {
        public static string KillProcess(string szNameProcess)
        {
            Process[] processes = Process.GetProcesses();
            foreach (Process process in processes)
            {
                if (Operators.CompareString(process.ProcessName, szNameProcess, TextCompare: false) == 0)
                {
                    try
                    {
                        process.Kill();
                        return "Stopped";
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        string message = ex2.Message;
                        ProjectData.ClearProjectError();
                        return message;
                    }
                }
            }
            return "Not found";
        }
    }
}