// MultiUserEDI.mLaunchAppAndWait
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using MultiUserEDI.My;


namespace MultiUserEDI
{
    [StandardModule]
    internal sealed class mLaunchAppAndWait
    {
        public const int SYNCHRONIZE = 1048576;

        public const int INFINITE = 65535;

        public const int WAIT_TIMEOUT = 258;

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int OpenProcess(int dwDesiredAccess, int bInheritHandle, int dwProcessID);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int WaitForSingleObject(int hHandle, int dwMilliseconds);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int CloseHandle(int hObject);

        public static void LaunchAppAndWait(string szProgram, string szCmdLine = null, string szWorkingDirectory = null)
        {
            try
            {
                Process process = new Process();
                int num;
                if (MyProject.Forms.MultiUserEDI.bUseProcessExecute)
                {
                    if (MyProject.Forms.MultiUserEDI.bUseShellExecute)
                    {
                        process.StartInfo.UseShellExecute = true;
                    }
                    if (Operators.CompareString(szWorkingDirectory, null, TextCompare: false) == 0)
                    {
                        szWorkingDirectory = MyProject.Forms.MultiUserEDI.AppPath;
                    }
                    process.StartInfo.WorkingDirectory = szWorkingDirectory;
                    process.StartInfo.FileName = szProgram;
                    process.StartInfo.WindowStyle = ProcessWindowStyle.Normal;
                    process.StartInfo.Arguments = szCmdLine;
                    process.Start();
                    num = process.Id;
                }
                else
                {
                    if (Operators.CompareString(szCmdLine, "", TextCompare: false) != 0)
                    {
                        szCmdLine = " " + szCmdLine;
                    }
                    num = Interaction.Shell(szProgram + szCmdLine, AppWinStyle.NormalFocus);
                }
                Application.DoEvents();
                if (num != 0)
                {
                    int num2 = OpenProcess(1048576, 0, num);
                    int dwMilliseconds = 100;
                    while (258 == WaitForSingleObject(num2, dwMilliseconds))
                    {
                        Application.DoEvents();
                    }
                    if (num2 != 0)
                    {
                        CloseHandle(num2);
                    }
                    num2 = 0;
                    if (MyProject.Forms.MultiUserEDI.bUseProcessExecute)
                    {
                        process.Close();
                    }
                }
                Application.DoEvents();
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox("LaunchApp: " + ex2.Message + "\r\n(\"" + szProgram + "\")", MsgBoxStyle.Critical);
                ProjectData.ClearProjectError();
            }
        }
    }
}