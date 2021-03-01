using Facility_Management.My;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{
    internal sealed class mMapDrive
    {
        public struct NETRESOURCE
        {
            public int dwScope;

            public int dwType;

            public int dwDisplayType;

            public int dwUsage;

            public string lpLocalName;

            public string lpRemoteName;

            public string lpComment;

            public string lpProvider;
        }

        public const int ForceDisconnect = 1;

        public const long RESOURCETYPE_DISK = 1L;

        private const long ERROR_BAD_NETPATH = 53L;

        private const long ERROR_NETWORK_ACCESS_DENIED = 65L;

        private const long ERROR_INVALID_PASSWORD = 86L;

        private const long ERROR_NETWORK_BUSY = 54L;

        [DllImport("mpr.dll", CharSet = CharSet.Ansi, EntryPoint = "WNetAddConnection2A", ExactSpelling = true, SetLastError = true)]
        public static extern int WNetAddConnection2(ref NETRESOURCE lpNetResource, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpPassword, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpUserName, int dwFlags);

        [DllImport("mpr", CharSet = CharSet.Ansi, EntryPoint = "WNetCancelConnection2A", ExactSpelling = true, SetLastError = true)]
        public static extern int WNetCancelConnection2([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpName, int dwFlags, int fForce);

        public static bool MapDrive(string DriveLetter, string UNCPath, string strUsername = null, string strPassword = null)
        {
            NETRESOURCE nETRESOURCE = default(NETRESOURCE);
            nETRESOURCE.lpRemoteName = UNCPath;
            nETRESOURCE.lpLocalName = DriveLetter + ":";
            nETRESOURCE.lpProvider = null;
            nETRESOURCE.dwType = 1;
            NETRESOURCE lpNetResource = nETRESOURCE;
            int num = WNetAddConnection2(ref lpNetResource, ref strPassword, ref strUsername, 0);
            switch (num)
            {
                case 0:
                    MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = null;
                    return true;
                case 53:
                    MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = "QA4001I Bad path could not connect to Star Directory";
                    break;
                case 86:
                    MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = "QA4002I Invalid password could not connect to Star Directory";
                    break;
                case 65:
                    MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = "QA4003I Network access denied could not connect to Star Directory";
                    break;
                case 54:
                    MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = "QA4004I Network busy could not connect to Star Directory";
                    break;
            }
            Interaction.MsgBox("Unable to map drive (" + Conversions.ToString(num).Trim() + ")\r\n\r\nLETTER=" + DriveLetter + "\r\nPATH=" + UNCPath, MsgBoxStyle.Critical);
            return false;
        }

        public static bool UnMapDrive(string DriveLetter)
        {
            string lpName = DriveLetter + ":";
            if (WNetCancelConnection2(ref lpName, 0, 1) == 0)
            {
                return true;
            }
            return false;
        }

        public static void MapDriveNetUse(string DriveLetter, string UNCPath)
        {
            Process process = new Process();
            process.StartInfo.FileName = "net.exe";
            process.StartInfo.Arguments = " use " + DriveLetter + ": " + UNCPath;
            process.StartInfo.CreateNoWindow = true;
            process.Start();
            process.WaitForExit();
        }
    }
}
