using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{
    internal sealed class mFunction
    {
        public struct LARGE_INTEGER
        {
            public long lowpart;

            public long highpart;
        }

        public const short DIRECTORY = 1;

        public const short DISK = 2;

        public const short EXTENSION_ONLY = 3;

        public const short FILENAME = 4;

        public const short FILENAME_ONLY = 5;

        public const short PATH = 6;

        public const short PATH_FILENAME_ONLY = 7;

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "GetDiskFreeSpaceExA", ExactSpelling = true, SetLastError = true)]
        public static extern long GetDiskFreeSpaceEx([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpRootPathName, ref LARGE_INTEGER lpFreeBytesAvailableToCaller, ref LARGE_INTEGER lpTotalNumberOfBytes, ref LARGE_INTEGER lpTotalNumberOfFreeBytes);

        [DllImport("kernel32", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern int GetShortPathName([MarshalAs(UnmanagedType.VBByRefStr)] ref string longPath, StringBuilder shortPath, int shortBufferSize);

        public static double DateFile(string svFile)
        {
            string str = Conversions.ToString(FileSystem.FileDateTime(svFile));
            return Conversion.Val(Strings.Mid(str, 7, 4) + Strings.Mid(str, 4, 2) + Strings.Mid(str, 1, 2) + Strings.Mid(str, 12, 2) + Strings.Mid(str, 15, 2) + Strings.Mid(str, 18, 2));
        }

        public static double CLargeInt(long Lo, long Hi)
        {
            double num = ((Lo >= 0) ? ((double)Lo) : (Math.Pow(2.0, 32.0) + (double)Lo));
            double num2 = ((Hi >= 0) ? ((double)Hi) : (Math.Pow(2.0, 32.0) + (double)Hi));
            return num + num2 * 4294967296.0;
        }

        public static double GetFreeSpace(string szPath)
        {
            try
            {
                LARGE_INTEGER lpFreeBytesAvailableToCaller = default(LARGE_INTEGER);
                LARGE_INTEGER lpTotalNumberOfBytes = default(LARGE_INTEGER);
                LARGE_INTEGER lpTotalNumberOfFreeBytes = default(LARGE_INTEGER);
                GetDiskFreeSpaceEx(ref szPath, ref lpFreeBytesAvailableToCaller, ref lpTotalNumberOfBytes, ref lpTotalNumberOfFreeBytes);
                return CLargeInt(lpFreeBytesAvailableToCaller.lowpart, lpFreeBytesAvailableToCaller.highpart);
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                double result = -1.0;
                ProjectData.ClearProjectError();
                return result;
            }
        }

        public static bool IsDirectoryWritable(string path)
        {
            return (new DirectoryInfo(path).Attributes & FileAttributes.ReadOnly) != FileAttributes.ReadOnly;
        }

        public static string RemoveFirstFolder(string szString)
        {
            checked
            {
                if (Operators.CompareString(Strings.Left(szString, 1), "\\", TextCompare: false) == 0)
                {
                    szString = Strings.Mid(szString, 2, szString.Length - 1);
                }
                int num = Strings.InStr(szString, "\\");
                int num2 = Strings.Len(szString);
                if (num > 0)
                {
                    return Strings.Mid(szString, num + 1, num2 - 1);
                }
                return szString;
            }
        }

        public static string ParsePath(string szPath, short nOperation)
        {
            checked
            {
                int num = Strings.InStrRev(szPath, ".") - 1;
                int num2 = Strings.InStrRev(szPath, "\\");
                int num3 = Strings.Len(szPath);
                switch (nOperation)
                {
                    case 3:
                        return Strings.Right(szPath, num3 - num);
                    case 5:
                        if (unchecked(num == -1 || num2 > num))
                        {
                            return Strings.Right(szPath, num3 - num2);
                        }
                        return Strings.Mid(szPath, num2 + 1, num - num2);
                    case 4:
                        return Strings.Right(szPath, num3 - num2);
                    case 6:
                        return Strings.Left(szPath, num2);
                    case 7:
                        return Strings.Left(szPath, num);
                    default:
                        return szPath;
                }
            }
        }

        public static int StrCompTextVersions(string version1, string version2)
        {
            int result = 0;
            checked
            {
                try
                {
                    string[] array = version1.Split('.');
                    string[] array2 = version2.Split('.');
                    int num = array.Length - 1;
                    for (int i = 0; i <= num; i++)
                    {
                        if (Conversion.Val(array[i]) > Conversion.Val(array2[i]))
                        {
                            return 1;
                        }
                        if (Conversion.Val(array[i]) < Conversion.Val(array2[i]))
                        {
                            return -1;
                        }
                    }
                    return result;
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    result = 1;
                    ProjectData.ClearProjectError();
                    return result;
                }
            }
        }

        public static long DirSize(DirectoryInfo d)
        {
            long num = 0L;
            FileInfo[] files = d.GetFiles();
            checked
            {
                foreach (FileInfo fileInfo in files)
                {
                    num += fileInfo.Length;
                }
                DirectoryInfo[] directories = d.GetDirectories();
                foreach (DirectoryInfo d2 in directories)
                {
                    num += DirSize(d2);
                }
                return num;
            }
        }

        public static string ConvertOctets(double Octets)
        {
            if (Octets >= 1099511627776.0)
            {
                return Virgul(Octets / 1099511627776.0) + "To";
            }
            if (Octets >= 1073741824.0)
            {
                return Virgul(Octets / 1073741824.0) + "Go";
            }
            if (Octets >= 1048576.0)
            {
                return Virgul(Octets / 1048576.0) + "Mo";
            }
            if (Octets >= 1024.0)
            {
                return Virgul(Octets / 1024.0) + "Ko";
            }
            return Strings.Trim(Conversions.ToString(Octets)) + "o";
        }

        private static string Virgul(double Val_Renamed)
        {
            int num = Strings.Len(Strings.Trim(Conversions.ToString(Conversion.Int(Val_Renamed))));
            int digits = ((num < 3) ? checked(3 - num) : 0);
            return Strings.Trim(Conversions.ToString(Math.Round(Val_Renamed, digits)));
        }

        public static string ShortPathName(string szPathName)
        {
            StringBuilder stringBuilder = new StringBuilder(260);
            GetShortPathName(ref szPathName, stringBuilder, stringBuilder.Capacity);
            return stringBuilder.ToString();
        }
    }

}
