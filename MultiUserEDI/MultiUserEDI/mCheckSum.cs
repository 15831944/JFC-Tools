// MultiUserEDI.mCheckSum
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using System;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace MultiUserEDI
{

    [StandardModule]
    internal sealed class mCheckSum
    {
        private static readonly int[] Crc32Table = new int[256];

        private static int InitCrc32(int Seed = -306674912, int Precondition = -1)
        {
            short num = 0;
            checked
            {
                do
                {
                    int num2 = num;
                    short num3 = 0;
                    do
                    {
                        int num4 = num2 & -2;
                        num4 = unchecked(num4 / 2);
                        num4 &= 0x7FFFFFFF;
                        num2 = (((num2 & 1) == 0) ? num4 : (num4 ^ Seed));
                        num3 = (short)unchecked(num3 + 1);
                    }
                    while (num3 <= 7);
                    Crc32Table[num] = num2;
                    num = (short)unchecked(num + 1);
                }
                while (num <= 255);
                return Precondition;
            }
        }

        private static int AddCrc32(ref byte[] Data, int CRC32)
        {
            Application.DoEvents();
            int num = Information.UBound(Data);
            for (int i = 0; i <= num; i = checked(i + 1))
            {
                byte b = Data[i];
                int num2 = ((CRC32 & -256) / 256) & 0xFFFFFF;
                int num3 = CRC32 & 0xFF;
                num3 ^= b;
                int num4 = Crc32Table[num3];
                CRC32 = num2 ^ num4;
            }
            return CRC32;
        }

        private static int GetCrc32(int CRC32)
        {
            return CRC32 ^ -1;
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        public static string Append(string FileName)
        {
            int num = 0;
            int num2 = 1048576;
            string result = "";
            checked
            {
                short num3 = (short)FileSystem.FreeFile();
                FileSystem.FileOpen(num3, FileName, OpenMode.Binary, OpenAccess.Read);
                int num4 = (int)FileSystem.LOF(num3);
                if (num4 <= 0)
                {
                    FileSystem.FileClose(num3);
                }
                else
                {
                    int cRC = InitCrc32();
                    while (num < num4)
                    {
                        if (num4 - num < num2)
                        {
                            num2 = num4 - num;
                        }
                        byte[] array = new byte[num2 - 1 + 1];
                        Array Value = array;
                        FileSystem.FileGet(num3, ref Value, -1L);
                        array = (byte[])Value;
                        num += num2;
                        cRC = AddCrc32(ref array, cRC);
                    }
                    FileSystem.FileClose(num3);
                    cRC = GetCrc32(cRC);
                    result = Conversion.Hex(cRC);
                }
                return result;
            }
        }

        public static string AppendString(string svData)
        {
            int i = 1;
            int num = Strings.Len(svData);
            checked
            {
                byte[] Data = new byte[num - 1 + 1];
                for (; i < num; i += 2)
                {
                    Data[i - 1] = (byte)Strings.Asc(Strings.Mid(svData, i, 2));
                }
                int cRC = InitCrc32();
                cRC = AddCrc32(ref Data, cRC);
                cRC = GetCrc32(cRC);
                return Conversion.Hex(cRC);
            }
        }
    }
}