using Facility_Management.My;
using System;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace Facility_Management
{
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
		public static string Append(string szFileName, bool CreateCRC = true)
		{
			if (Strings.StrComp(mFunction.ParsePath(Application.ExecutablePath, 4), mFunction.ParsePath(szFileName, 4), CompareMethod.Text) == 0)
			{
				CreateCRC = false;
			}
			string path = mFunction.ParsePath(szFileName, 7) + MyProject.Forms.FormBox.mExtFileCRC;
			if (File.Exists(path))
			{
				File.Delete(path);
			}
			string text = szFileName + MyProject.Forms.FormBox.mExtFileCRC;
			string text2;
			if (File.Exists(text) && CreateCRC)
			{
				if (DateTime.Compare(File.GetLastWriteTime(text), File.GetLastWriteTime(szFileName)) == 0)
				{
					text2 = File.ReadAllText(text, Encoding.Default);
				}
				else
				{
					if (Interaction.MsgBox("File date error: " + text + "\r\n\r\nDo you want to create it again?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "CRC error") != MsgBoxResult.No)
					{
						File.Delete(text);
						goto IL_00d1;
					}
					text2 = File.ReadAllText(text, Encoding.Default);
				}
				goto IL_026d;
			}
			if (File.Exists(text))
			{
				File.Delete(text);
			}
			goto IL_00d1;
		IL_026d:
			return text2;
		IL_00d1:
			int num = 0;
			int num2 = 1048576;
			text2 = "";
			MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = "Check CRC: " + mFunction.ParsePath(szFileName, 5);
			checked
			{
				short num3 = (short)FileSystem.FreeFile();
				FileSystem.FileOpen(num3, szFileName, OpenMode.Binary, OpenAccess.Read);
				int num4 = (int)FileSystem.LOF(num3);
				Application.DoEvents();
				if (num4 <= 0)
				{
					FileSystem.FileClose(num3);
				}
				else
				{
					int cRC = InitCrc32();
					while (num < num4)
					{
						int num5 = (int)Math.Round((double)num / (double)num4 * 100.0);
						if (num5 != MyProject.Forms.FormBox.ToolStripProgressBar1.Value)
						{
							MyProject.Forms.FormBox.ToolStripProgressBar1.Value = num5;
							Application.DoEvents();
						}
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
					MyProject.Forms.FormBox.ToolStripProgressBar1.Value = 100;
					cRC = GetCrc32(cRC);
					text2 = Conversion.Hex(cRC);
					if (CreateCRC)
					{
						File.WriteAllText(text, text2, Encoding.Default);
						File.SetCreationTime(text, File.GetCreationTime(szFileName));
						File.SetLastAccessTime(text, File.GetLastAccessTime(szFileName));
						File.SetLastWriteTime(text, File.GetLastWriteTime(szFileName));
					}
					MyProject.Forms.FormBox.ToolStripStatusLabelFM.Text = null;
				}
				goto IL_026d;
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
					Application.DoEvents();
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

