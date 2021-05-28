using System.Runtime.InteropServices;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mHddSerial
    {
        private const int MAX_PATH = 255;

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "GetVolumeInformationA", ExactSpelling = true, SetLastError = true)]
        private static extern int GetVolumeInformation([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpRootPathName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpVolumeNameBuffer, int nVolumeNameSize, ref int lpVolumeSerialNumber, ref int lpMaximumComponentLength, ref int lpFileSystemFlags, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpFileSystemNameBuffer, int nFileSystemNameSize);

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "GetWindowsDirectoryA", ExactSpelling = true, SetLastError = true)]
        private static extern int GetWindowsDirectory([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpBuffer, int nSize);

        public static string HddSerial()
        {
            string lpBuffer = new string('\0', 255);
            GetWindowsDirectory(ref lpBuffer, 255);
            string str = Strings.Left(lpBuffer, 3);
            string lpVolumeNameBuffer = new string('\0', 255);
            string lpFileSystemNameBuffer = new string('\0', 255);
            string lpRootPathName = Strings.Left(str, 3);
            int lpMaximumComponentLength = 0;
            int lpFileSystemFlags = 0;
            int lpVolumeSerialNumber = default(int);
            GetVolumeInformation(ref lpRootPathName, ref lpVolumeNameBuffer, 255, ref lpVolumeSerialNumber, ref lpMaximumComponentLength, ref lpFileSystemFlags, ref lpFileSystemNameBuffer, 255);
            checked
            {
                lpVolumeNameBuffer = Strings.Left(lpVolumeNameBuffer, Strings.InStr(1, lpVolumeNameBuffer, "\0") - 1);
                lpFileSystemNameBuffer = Strings.Left(lpFileSystemNameBuffer, Strings.InStr(1, lpFileSystemNameBuffer, "\0") - 1);
                double d = lpVolumeSerialNumber;
                return Strings.Trim(Dec2Hex(ref d));
            }
        }

        private static string Dec2Hex(ref double d)
        {
            if (d > 4294967295.0)
            {
                return "Erreur";
            }
            if (d < 0.0)
            {
                d = (d / 2.0 + 2147483648.0) * 2.0;
            }
            string text = Conversion.Hex(Conversion.Int(d / 16.0)) + "0";
            text = new string('0', checked(8 - Strings.Len(text))) + text;
            return Strings.Left(text, 7) + Strings.Right(Conversion.Hex(d - Conversion.Val("&H" + Strings.Left(text, 1)) * 268435456.0), 1);
        }
    }
}