using Facility_Management.My;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Kantar.MediaPlanning.Logger;
using Microsoft.VisualBasic;

namespace Facility_Management
{
    internal sealed class mLaunchAppAndWait
    {
        public const int SYNCHRONIZE = 1048576;

        public const int INFINITE = 65535;

        public const short WAIT_OBJECT_0 = 0;

        public const int WAIT_TIMEOUT = 258;

        public const byte WAIT = 1;

        public const byte NOWAIT = 0;

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int OpenProcess(int dwDesiredAccess, int bInheritHandle, int dwProcessID);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int WaitForSingleObject(int hHandle, int dwMilliseconds);

        [DllImport("kernel32", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int CloseHandle(int hObject);

        public static void LaunchAppAndWait(string svApplication, string svCommandLine = "", ProcessWindowStyle Style = ProcessWindowStyle.Normal, byte WaitInfinity = 1)
        {
            if (!File.Exists(svApplication))
            {
                TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, svApplication + " n'existe pas");
                MessageBox.Show(svApplication + " n'existe pas");

                return;
            }

            MessageBox.Show("Application.StartupPath : " + Application.StartupPath);
            MessageBox.Show("svApplication : " + svApplication);
            MessageBox.Show("svCommandLine : " + svCommandLine);

            Process process = new Process();
            process.StartInfo.WorkingDirectory = Application.StartupPath;
            process.StartInfo.FileName = svApplication;
            process.StartInfo.WindowStyle = Style;
            process.StartInfo.Arguments = svCommandLine;
            process.StartInfo.UseShellExecute = MyProject.Forms.FormBox.bUseShellExecute;
            process.StartInfo.CreateNoWindow = true;
            process.Start();
            if (process.Id != 0)
            {
                if (WaitInfinity != 1)
                {
                    //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "WaitInfinity");
                    MessageBox.Show("WaitInfinity");
                    return;
                }
                int num = OpenProcess(1048576, 0, process.Id);

                //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "num : " + num);
                MessageBox.Show("num : " + num);

                if (num != 0)
                {
                    Application.DoEvents();
                    int dwMilliseconds = 100;
                    while (258 == WaitForSingleObject(num, dwMilliseconds))
                    {
                        Application.DoEvents();
                    }
                    CloseHandle(num);
                }
            }
            else
            {
                Interaction.MsgBox("Unable to launch: " + svApplication + "\r\nArguments: " + svCommandLine, MsgBoxStyle.Critical, "Error: Id");
            }

            //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "Fin de LaunchAppAndWait");
            MessageBox.Show("Fin de LaunchAppAndWait");
        }
    }
}
